#include <SFML/Network.hpp>
#include <list>
#include <iostream>

enum PacketType { PLAYER = 0, PLAYERDC = 1, PLAYERAMOUNT = 2, PLARESPONSE = 3};

struct Player
{
    int packetType;
    float posx, posy;
    std::string name;
    int uid;
};

sf::Packet& operator >>(sf::Packet& packet, Player& character)
{
    return packet >> character.posx >> character.posy >> character.name >> character.uid;
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
                    //std::cout << "New client : " << client->getRemoteAddress() << std::endl;
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
                            } 
                        }
                    }
                }
            }
        }
    }

    return 0;
}
