#include <SFML/Network.hpp>
#include <list>
#include <iostream>

int main()
{
    bool running = true;

    sf::TcpListener listener;
    listener.listen(8080);

    std::list<sf::TcpSocket*> clients;
    sf::SocketSelector selector;
    selector.add(listener);

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

                        }
                    }
                }
            }
        }
    }
}
