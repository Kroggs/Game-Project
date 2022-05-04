#pragma once

#include <SFML/Graphics.hpp>
#include "map.h"
#include "eplayer_struct.h"
#include "text.h"

namespace ndir
{
	enum ndir { nDOWN, nLEFT, nRIGHT, nUP };
}

class NetPlayer : public sf::Drawable, public sf::Transformable
{
public :
	NetPlayer();
	
	void Update();

	void AssignStruct(EPlayer eplayer);

	void SetCurrentMapLocation(Map* map);
	void RenderShadow();
	void TakeDamage(const int amount);

	bool IsBehindTile() const;

	sf::Vector2f GetPosition() const;
	std::string GetUsername() const;
	int getUid() const;

private :
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Sprite PSprite, PBlackSprite;
	sf::Texture PTexture, PBlackTexture;
	sf::Vector2i PAnim;
	sf::RenderWindow* ParentWindow;

	sf::Vector2f Position;

	std::string Username;
	sf::Text TUsername;
	sf::Font TUsername_Font;

	Map* CurrentMapLocation;

	sf::Clock TimeSinceLastMove;

	float CurrentSpeed, Acceleration, MaxSpeed;
	bool IsMoving, PIsBehindTile;

	sf::Clock Time;
	float FpsCount,
		  SwitchFps,
	      FpsSpeed;

	int Health, DamageReceived, UID;
};