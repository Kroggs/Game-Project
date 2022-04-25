#pragma once
#include <SFML/Network.hpp>
#include <cstring>


namespace netw
{
	enum Events{ CHATLOG };
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect("127.0.0.1", 8080);

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
}