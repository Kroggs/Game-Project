#pragma once
#include <SFML/Network.hpp>
#include <cstring>

namespace netw
{
	sf::TcpSocket socket;

	bool send(char* data)
	{
		sf::Socket::Status status = socket.connect("127.0.0.1", 8080);
		if (status == sf::Socket::Done)
			if (socket.send(data, strlen(data)) != sf::Socket::Done)
				return false;
		else return false;
	}
}