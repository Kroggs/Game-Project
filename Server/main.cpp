#include <SFML/Network.hpp>
#include <list>
#include <iostream>

enum PacketType { PLAYER = 0, PLAYERDC = 1, PLAYERAMOUNT = 2, PLARESPONSE = 3, PLAYERPOS = 4, PLAPOSRESPONSE = 5, PLAYERUPDATE = 6,
                  PLAYERJOINRESPONSE = 7, GETPLAYERUID = 8, GETPLAYER = 9, PLAYERUPDATERESPONSE = 10
};

struct Player
{
    int packetType;
    float posx, posy, speed;
    bool isMoving, isBehindTile;
    std::string name;
    int uid, animx, animy;
}; Player dummyPlayer{PLAYER, 0, 0, 0, 0, 0, "dummy", 0, 0, 0};

sf::Packet& operator >>(sf::Packet& packet, Player& character)
{
    return packet >> character.posx >> character.posy >> character.speed >> character.isMoving >> character.isBehindTile >> character.name >> character.uid >> character.animx >> character.animy;
}

sf::Packet& operator << (sf::Packet& packet, Player& character)
{
    return packet << character.posx << character.posy << character.speed << character.isMoving << character.isBehindTile << character.name << character.uid << character.animx << character.animy;
}

int main()
{
    bool running = true;
    std::vector<Player> Clients;


    sf::TcpListener listener;
    listener.listen(8080);

    std::list<sf::TcpSocket*> clients;
    sf::SocketSelector selector;
    selector.add(listener);

    std::cout << "Server is running..." << std::endl;
    while (running)
    {
        if (selector.wait())
        {
            if (selector.isReady(listener))
            {
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done)
                {
                    std::cout << "New client : " << client->getRemoteAddress() << std::endl;
                    clients.push_back(client);
                    selector.add(*client);
                }
                else
                {
                    delete client;
                }
            }
            else
            {
                for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                {
                    sf::TcpSocket& client = **it;
                    if (selector.isReady(client))
                    {
                        sf::Packet packet;
                        if (client.receive(packet) == sf::Socket::Done)
                        {
                            int packetType = 0;
                            if (packet >> packetType) {
                                if (packetType == PLAYER) {
                                    Player p; bool alreadyExists = false;
                                    packet >> p;
                                    for (auto& player : Clients) {
                                        if (player.uid == p.uid) {
                                            alreadyExists = true;
                                        }
                                    }
                                    if (!alreadyExists) {
                                        Clients.push_back(p);
                                        std::cout << p.name << " joined." << std::endl;
                                        sf::Packet broadcast_pjoin;
                                        broadcast_pjoin << static_cast<int>(PLAYERJOINRESPONSE) << p;
                                        for (std::list<sf::TcpSocket*>::iterator pIt = clients.begin(); pIt != clients.end(); ++pIt) {      
                                            if (pIt != it)
                                                reinterpret_cast<sf::TcpSocket&>(**pIt).send(broadcast_pjoin);
                                        }
                                    }
                                }
                                else if (packetType == PLAYERDC) {
                                    int UID; packet >> UID;
                                    for (std::vector<Player>::iterator pIt = Clients.begin(); pIt != Clients.end();) {
                                        if (pIt->uid == UID) {
                                            std::cout << pIt->name << " disconnected." << std::endl;
                                            pIt = Clients.erase(pIt);
                                        }
                                        else {
                                            ++pIt;
                                        }
                                    }
                                }
                                else if (packetType == PLAYERAMOUNT) {
                                    int playerAmount = Clients.size();
                                    sf::Packet packet;
                                    packet << PLARESPONSE << playerAmount;
                                    client.send(packet);
                                }
                                else if (packetType == PLAYERUPDATE) {
                                    Player p; packet >> p;
                                    for (std::vector<Player>::iterator pIt = Clients.begin(); pIt != Clients.end(); ++pIt)
                                        if (pIt->uid == p.uid)
                                            *pIt = p;
                                    sf::Packet broadcast_pupdate;
                                    broadcast_pupdate << static_cast<int>(PLAYERUPDATERESPONSE) << p;
                                    for (std::list<sf::TcpSocket*>::iterator pIt = clients.begin(); pIt != clients.end(); ++pIt) {
                                        if (pIt != it) {
                                            reinterpret_cast<sf::TcpSocket&>(**pIt).send(broadcast_pupdate);
                                        }
                                    }
                                }
                                else if (packetType == GETPLAYERUID) {
                                    int index = 0;
                                    packet >> index;
                                    int uid = Clients[index].uid;
                                    sf::Packet sPacket; sPacket << uid;
                                    client.send(sPacket);
                                }
                                else if (packetType == GETPLAYER) {
                                    int uid = 0;
                                    packet >> uid;
                                    Player pReturn = dummyPlayer;
                                    for (auto& player : Clients) {
                                        if (player.uid == uid) {
                                            pReturn = player;
                                        }
                                    }
                                    sf::Packet returnPacket;
                                    returnPacket << pReturn;
                                    client.send(returnPacket);
                                }
                            } 
                        }
                    }
                }
            }
        }
    }

    for (auto* socket : clients) {
        delete socket;
    }

    return 0;
}
