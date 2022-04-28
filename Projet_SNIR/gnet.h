#pragma once
#include <SFML/Network.hpp>
#include <cstring>

void Listener()
{

}


namespace netw
{
	enum Events{ CHATLOG};
	sf::TcpSocket socket;
	sf::TcpListener listener;
	sf::Socket::Status status;

	void init()
	{
		status = socket.connect("127.0.0.1", 8080);
	}

	void closeConnextion()
	{
		socket.disconnect();
	}

	bool sendMsg(char* data)
	{
		if (status == sf::Socket::Done)
			if (socket.send(data, strlen(data)) != sf::Socket::Done)
				return false;
		else return false;
	}

	bool sendEvent(auto EventType, char* data)
	{
		switch (EventType)
		{
		case CHATLOG:
			if (status == sf::Socket::Done) {
				char* msg = strcat((char*)"CLOG", data);
				if (socket.send(msg, strlen(msg)) != sf::Socket::Done)
					return false;
				else return false;
			}
				
			break;
		}
	}

	int getPlayerAmount()
	{
		char buffer[100];
		std::size_t received;

		if (status == sf::Socket::Done)
		if (socket.send((char*)"GETPLAYERAMOUNT\n", 0x0F) != sf::Socket::Done)
			return false;
		else return false;

		if (listener.listen(8080) == sf::Socket::Done)
		{
			if (socket.receive(buffer, 100, received) != sf::Socket::Done)
				return 0;
			else {
				std::cout << "Received " << received << " bytes" << std::endl;
			}
		}
		else return 0;
	}

	bool getPlayer(const int index)
	{
		return 0;
	}
}