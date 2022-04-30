#pragma once
#include <SFML/Network.hpp>

enum PacketType { PLAYER = 0, PLAYERDC = 1, PLAYERAMOUNT = 2, PLARESPONSE = 3 };

namespace netw
{
	sf::TcpSocket socket;
	sf::TcpListener listener;
	sf::Socket::Status status;

	void init()
	{
		status = socket.connect("127.0.0.1", 8080);
	}

	int getPlayerAmount()
	{
		int playerAmount = 0xDEADBEEF;
		sf::Packet packet, packetResponse;
		packet << static_cast<int>(PLAYERAMOUNT);
		if (socket.send(packet) != sf::Socket::Done)
			return false;
		while (playerAmount == 0xDEADBEEF) {
			if (socket.receive(packetResponse) == sf::Socket::Done) {
				int pType; packetResponse >> pType;
				if (pType == PLARESPONSE) {
					packetResponse >> playerAmount;
				}
				else {
					std::cout << "Error occured : aborting...";
					break;
				}
			}
		}

		return playerAmount;
	}

	void closeConnextion(int uid)
	{
		sf::Packet packet;
		packet << static_cast<int>(PLAYERDC)
			   << uid;
		socket.send(packet);
		socket.disconnect();
	}

	void sendPlayerPacket(sf::Vector2f pos, std::string name, int uid)
	{
		sf::Packet packet;
		packet << static_cast<int>(PLAYER) << pos.x << pos.y << name << uid;
		socket.send(packet);
	}
}