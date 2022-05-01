#pragma once

#include <iostream>

struct EPlayer
{
	int packetType;
	float posx, posy, speed;
	bool isMoving, isBehindTile;
	std::string name;
	int uid, animx, animy;
};
