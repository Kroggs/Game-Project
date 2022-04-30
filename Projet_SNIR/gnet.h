#pragma once
#include <SFML/Network.hpp>
#include "eplayer_struct.h"

enum PacketType { PLAYER = 0, PLAYERDC = 1, PLAYERAMOUNT = 2, PLARESPONSE = 3, PLAYERPOS = 4, PLAPOSRESPONSE = 5, PLAYERUPDATE = 6,
	              PLAYERJOINRESPONSE = 7, GETPLAYERUID = 8, GETPLAYER = 9
};

sf::Packet& operator >>(sf::Packet& packet, EPlayer& character)
{
	return packet >> character.posx >> character.posy >> character.speed >> character.isMoving >> character.isBehindTile >> character.name >> character.uid >> character.animx >> character.animy;
}

sf::Packet& operator << (sf::Packet& packet, EPlayer& character)
{
	return packet << character.posx << character.posy << character.speed << character.isMoving << character.isBehindTile << character.name << character.uid << character.animx << character.animy;
}

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
		int playerAmount = 0;
		sf::Packet packet, packetResponse;
		packet << static_cast<int>(PLAYERAMOUNT);
		if (socket.send(packet) != sf::Socket::Done)
			throw std::runtime_error("Unable to send packet.");
		while (!playerAmount) {
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

	void sendPacket(sf::Packet& packet) {
		socket.send(packet);
	}

	void closeConnextion(int uid)
	{
		sf::Packet packet;
		packet << static_cast<int>(PLAYERDC)
			   << uid;
		socket.send(packet);
		socket.disconnect();
	}

	void joinServer(EPlayer p)
	{
		sf::Packet packet;
		packet << static_cast<int>(PLAYER) << p;
		socket.send(packet);
	}

	bool getPacket(sf::Packet& packet)
	{
		if (socket.receive(packet) == sf::Socket::Done) {
			return true;
		}
	}

	int getPlayerUid(const int index, const int uid, int& target)
	{
		sf::Packet packet, packetResponse;
		packet << static_cast<int>(GETPLAYERUID) << index;
		socket.send(packet);
		int pUid = 0; bool done = false; 
		while (!done) {
			if (socket.receive(packetResponse) == sf::Socket::Done) {
				done = true;
				packetResponse >> pUid;
				if (pUid == uid)
					return 0;
				else {
					target = pUid;
					return pUid;
				}
					
			}
		}

		return 0;
	}

	EPlayer getPlayerByUid(const int uid)
	{
		sf::Packet packet, packetResponse;
		packet << static_cast<int>(GETPLAYER) << uid;
		socket.send(packet);
		EPlayer dummyPlayer{ 0, 0, 0, 0, 0, 0, "dummy", 0, 0, 0 };
		EPlayer player = dummyPlayer; bool done = false;
		while (!done) {
			if (socket.receive(packetResponse) == sf::Socket::Done) {
				done = true;
				packetResponse >> player;
				if (player.uid != uid)
					std::cout << "No player found with this uid...";
			}
		}

		return player;
	}
}