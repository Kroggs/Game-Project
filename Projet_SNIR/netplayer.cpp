#include "netplayer.h"

#include <cstdlib>

NetPlayer::NetPlayer()
{
	srand(time(NULL));
	if (!this->PTexture.loadFromFile("images/characters/main_character.png"))
		std::cerr << "Couldn\'t load Player Texture." << std::endl;
	if (!this->PBlackTexture.loadFromFile("images/characters/main_character_black.png"))
		std::cerr << "Couldn\'t load Player Shadow Texture." << std::endl;

	this->ParentWindow = nullptr;
	this->CurrentMapLocation = nullptr;

	this->UID = rand() % 0xFFFFFFFF;

	this->PSprite.setTexture(this->PTexture);
	this->PBlackSprite.setTexture(this->PBlackTexture);

	this->PAnim = sf::Vector2i(0, dir::DOWN);
	this->Username = "Guest";

	this->CurrentSpeed = 0;
	this->Acceleration = 0.05f;
	this->MaxSpeed = 2.0f;

	this->IsMoving = false;
	this->PIsBehindTile = false;

	this->FpsCount = 0;
	this->SwitchFps = 100;
	this->FpsSpeed = 500;

	this->Health = 384;
	this->DamageReceived = 0;

}

void NetPlayer::Update()
{

}

void NetPlayer::SetCurrentMapLocation(Map* map)
{
	this->CurrentMapLocation = map;
}

void NetPlayer::RenderShadow()
{
	if (this->ParentWindow)
		this->ParentWindow->draw(this->PBlackSprite);
}

void NetPlayer::TakeDamage(const int amount)
{
	this->DamageReceived += amount;
}

bool NetPlayer::IsBehindTile() const
{
	return this->PIsBehindTile;
}

sf::Vector2f NetPlayer::GetPosition() const
{
	return this->PSprite.getPosition();
}

std::string NetPlayer::GetUsername() const
{
	return this->Username;
}

int NetPlayer::getUid() const
{
	return this->UID;
}

void NetPlayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->PSprite);
}