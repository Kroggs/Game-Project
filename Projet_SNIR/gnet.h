#pragma once
#include <SFML/Network.hpp>

enum PacketType{PLAYER = 0};

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

	void sendPlayerPacket(sf::Vector2f pos, std::string name, int uid)
	{
		sf::Packet packet;
		packet << static_cast<int>(PLAYER) << pos.x << pos.y << name << uid;
		socket.send(packet);
	}
}