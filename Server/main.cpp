#include <SFML/Network.hpp>
#include <list>
#include <iostream>

enum PacketType { PLAYER = 0 };

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
                            std::cout << "Packet received" << std::endl;
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
                                        std::cout << p.name << " added to player list." << std::endl;
                                    }
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
