#include "Player.h"
#include "utils.h"
#include <iostream>
#include <functional>
#include <math.h>
#include <cstdlib>

#include "levels.h"

#define ROUND_2_INT(f) ((int)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))

enum Layers{ FIRST = 0, SECOND = 1, THIRD = 2};

Player::Player()
{
	this->m_Username = "Guest";

	if (!this->m_Texture.loadFromFile("images/characters/main_character.png"))
		std::cerr << "Error while loading player texture" << std::endl;
	if (!this->m_Texture_Black.loadFromFile("images/characters/main_character_black.png"))
		std::cerr << "Error while loading player texture" << std::endl;

	this->m_Sprite.setTexture(this->m_Texture);
	this->m_BlackSprite.setTexture(this->m_Texture_Black);
	this->m_Anim = sf::Vector2i(0, dir::DOWN);
	this->m_CurrentSpeed = 0;
	this->m_Acceleration = 0.05f;
	this->m_MaxSpeed = 2.0f;
	this->m_IsMoving = false;
	this->m_isBehindTile = false;
	this->m_CurrentMap = nullptr;
	this->m_camera = nullptr;
	this->m_parentWindow = nullptr;
	this->m_InventorySize = 8;
	this->m_InventoryAmount = 0;

	this->m_CollisionChecker.setSize(sf::Vector2f(32,32));
	this->m_PlayerTileChecker.setSize(sf::Vector2f(32, 32));

	this->m_Uid = rand() % 0xFFFFFFFF;

	this->m_Sprite.setPosition(sf::Vector2f(550, 384));
	this->m_BlackSprite.setPosition(sf::Vector2f(550, 384));
	this->m_BlackSprite.setColor(sf::Color(0, 0, 0, 70));
	fpsCount = 0;
	switchFps = 100;
	fpsSpeed = 500;

	this->m_Health = 384;
	this->m_DamageRecieved = 0;
	this->m_IsDead = false;
	this->m_IsDamageAnimOver = true;

	this->m_EquippedItemIndex = 0;
	this->m_EquippedItem = nullptr;

	this->m_LifeBar.setFillColor(sf::Color::Red);
	this->m_LifeBar.setSize(sf::Vector2f(this->m_Health, 12));
	this->m_LifeBar.setPosition(sf::Vector2f(92, 22));

	this->m_EquippedItemHighlight.setFillColor(sf::Color(255, 240, 240, 100));
	this->m_EquippedItemHighlight.setSize(sf::Vector2f(48, 48));
	this->m_EquippedItemHighlight.setPosition(sf::Vector2f(350, 700));
}

void Player::SetCurrentMapLocation(Map* map)
{
	this->m_CurrentMap = map;
}

void Player::RenderShadow()
{
	if (this->m_parentWindow)
		this->m_parentWindow->draw(this->m_BlackSprite);
}

void Player::SetRefSprite(sf::Sprite* refSprite)
{
	this->m_SpriteRefs.push_back(refSprite);
}

bool Player::IsBehindTile() const
{
	return this->m_isBehindTile;
}

sf::Vector2f Player::GetPosition() const
{
	return this->m_Sprite.getPosition();
}

void Player::AddItem(Item* item)
{
	if (this->m_Inventory.size() < this->m_InventorySize){
		item->sprite()->setPosition(sf::Vector2f(366 + 53 * this->m_InventoryAmount, 715));
		this->m_InventoryAmount++;
		this->m_Inventory.push_back(item);
		this->m_SpriteRefs.push_back(item->sprite());
	}	
	else
	{
		std::cout << "Can't call AddItem()\nm_Inventory : full" << std::endl;
	}
	
}

void Player::DrawInventoryItems()
{
	for (unsigned int i = 0; i < this->m_Inventory.size(); ++i) {
		this->m_parentWindow->draw(*this->m_Inventory[i]);
	}
}

sf::RectangleShape Player::GetLifebar() const
{
	return this->m_LifeBar;
}


void Player::SetView(sf::View* view, sf::RenderWindow* window)
{
	this->m_camera = view;
	this->m_parentWindow = window;
}

int Player::getUid() const
{
	return this->m_Uid;
}

sf::RectangleShape Player::GetItemGui() const
{
	return this->m_EquippedItemHighlight;
}

bool Player::IsColliding()
{

	LevelInformation CurrentMapInfos = this->m_CurrentMap->GetLevelInformations(this->m_CurrentMap->GetCurrentLevel());

	sf::FloatRect bounds = sf::FloatRect(this->m_Sprite.getGlobalBounds().left + 9.0f, this->m_Sprite.getGlobalBounds().top + 9.0f, 15.0f, 23.0f);

	sf::Vector2i PlayerTileCoords = sf::Vector2i(ROUND_2_INT(this->m_Sprite.getPosition().x / 32),
											     ROUND_2_INT((this->m_Sprite.getPosition().y + 16) / 32));

	if (L::SPAWN_LAYER[2][PlayerTileCoords.x + PlayerTileCoords.y * CurrentMapInfos.width] > 0)
		this->m_isBehindTile = true;
	else
		this->m_isBehindTile = false;

	this->m_PlayerTileChecker.setPosition(PlayerTileCoords.x * 32, PlayerTileCoords.y * 32);

	int NextTile = 0;
	int map_size = CurrentMapInfos.height * CurrentMapInfos.width;

	switch (this->m_Anim.y) {
		case dir::UP: bounds.top -= (bounds.height / 2);
			PlayerTileCoords.y--;
			break;
		case dir::DOWN: bounds.top += (bounds.height / 2); 
			PlayerTileCoords.y++;
			break;
		case dir::LEFT: bounds.left -= (bounds.width / 2); 
			PlayerTileCoords.x--;
			break;
		case dir::RIGHT: bounds.left += (bounds.width / 2); 
			PlayerTileCoords.x++;
			break;
	}

	for (unsigned int i = 0; i < CurrentMapInfos.layers; ++i) {
		int NextTileId = L::SPAWN_LAYER[i][PlayerTileCoords.x + PlayerTileCoords.y * CurrentMapInfos.width] - 2; // tile ID of the tile forward the current player direction
		for (unsigned int j = 0; j < CurrentMapInfos.collidableTiles.size(); j++) {
			if (NextTileId == CurrentMapInfos.collidableTiles[j]) { // if the tile has collision
				this->m_CollisionChecker.setPosition(sf::Vector2f(PlayerTileCoords.x * 32, PlayerTileCoords.y * 32));
			}
		}
	}
	

	if (bounds.intersects(this->m_CollisionChecker.getGlobalBounds())) {
		return true;
	}


	std::vector<Asset*> Assets = this->m_CurrentMap->GetAssets();
	for (unsigned int i = 0; i < Assets.size(); i++){
		if (Assets[i]->HasCollision()){
			if (bounds.intersects(Assets[i]->GetSprite().getGlobalBounds()))
				return true;
		}
	}


	return false;
}

std::string Player::GetUsername() const
{
	return this->m_Username;
}

void Player::TakeDamage(const int amount)
{
	this->m_DamageRecieved += amount;
}

void Player::Update()
{
	if (this->m_DamageRecieved) {
		if (this->m_DamageRecieved >= this->m_Health)
			this->m_IsDead = true;
		else if (this->m_timeSinceLastHealthDecrease.getElapsedTime().asMilliseconds() > 10) {
			this->m_timeSinceLastHealthDecrease.restart();
			this->m_DamageRecieved--;
			this->m_Health--;
		}
	}

	if (this->m_Inventory.size()) {
		if (this->m_Inventory[this->m_EquippedItemIndex]) {
			this->m_EquippedItem = this->m_Inventory[this->m_EquippedItemIndex];
		}
	}

	float PosX = this->m_EquippedItemHighlight.getPosition().x - (this->m_EquippedItemIndex * 52.1);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && this->m_EquippedItemIndex != 0)
		this->m_EquippedItemIndex = 0;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && this->m_EquippedItemIndex != 1)
		this->m_EquippedItemIndex = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && this->m_EquippedItemIndex != 2)
		this->m_EquippedItemIndex = 2;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && this->m_EquippedItemIndex != 3)
		this->m_EquippedItemIndex = 3;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) && this->m_EquippedItemIndex != 4)
		this->m_EquippedItemIndex = 4;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) && this->m_EquippedItemIndex != 5)
		this->m_EquippedItemIndex = 5;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7) && this->m_EquippedItemIndex != 6)
		this->m_EquippedItemIndex = 6;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8) && this->m_EquippedItemIndex != 7)
		this->m_EquippedItemIndex = 7;

	this->m_EquippedItemHighlight.setPosition(sf::Vector2f(PosX + (this->m_EquippedItemIndex * 52.1), this->m_EquippedItemHighlight.getPosition().y));

	for (auto& item : this->m_Inventory)
		item->Update();

	if (this->m_Health <= 0)
		this->m_IsDead = true;

	this->m_IsMoving = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
		this->m_Anim.y = dir::UP;
		this->m_timeSinceLastMove.restart();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		this->m_Anim.y = dir::LEFT;
		this->m_timeSinceLastMove.restart();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		this->m_Anim.y = dir::DOWN;
		this->m_timeSinceLastMove.restart();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		this->m_Anim.y = dir::RIGHT;
		this->m_timeSinceLastMove.restart();
	}
	else {
		this->m_IsMoving = false;
	}
		
	if (this->m_IsMoving)
		this->m_CurrentSpeed = utils::Lerp(this->m_CurrentSpeed, this->m_MaxSpeed, this->m_Acceleration);
	else {
		this->m_CurrentSpeed > 0.2f ? this->m_CurrentSpeed -= this->m_Acceleration : this->m_CurrentSpeed = 0;
		this->m_camera->move(0.0f, 0.0f);
	}

	if (!IsColliding())
	{
		switch (this->m_Anim.y)
		{
		case dir::UP:
			this->m_Sprite.move(0, -(this->m_CurrentSpeed));
			this->m_BlackSprite.move(0, -(this->m_CurrentSpeed));
			this->m_camera->move(0, -(this->m_CurrentSpeed));
			for(auto& s : this->m_SpriteRefs)
				s->move(0, -(this->m_CurrentSpeed));
			this->m_LifeBar.move(0, -(this->m_CurrentSpeed));
			this->m_EquippedItemHighlight.move(0, -(this->m_CurrentSpeed));
			break;
		case dir::DOWN:
			this->m_Sprite.move(0, this->m_CurrentSpeed);
			this->m_BlackSprite.move(0, this->m_CurrentSpeed);
			this->m_camera->move(0, this->m_CurrentSpeed);
			for (auto& s : this->m_SpriteRefs)
				s->move(0, this->m_CurrentSpeed);
			this->m_LifeBar.move(0, this->m_CurrentSpeed);
			this->m_EquippedItemHighlight.move(0, this->m_CurrentSpeed);
			break;
		case dir::LEFT:
			this->m_Sprite.move(-(this->m_CurrentSpeed), 0);
			this->m_BlackSprite.move(-(this->m_CurrentSpeed), 0);
			this->m_camera->move(-(this->m_CurrentSpeed), 0);
			for (auto& s : this->m_SpriteRefs)
				s->move(-(this->m_CurrentSpeed), 0);
			this->m_LifeBar.move(-(this->m_CurrentSpeed), 0);
			this->m_EquippedItemHighlight.move(-(this->m_CurrentSpeed), 0);
			break;
		case dir::RIGHT:
			this->m_Sprite.move(this->m_CurrentSpeed, 0);
			this->m_BlackSprite.move(this->m_CurrentSpeed, 0);
			this->m_camera->move(this->m_CurrentSpeed, 0);
			for (auto& s : this->m_SpriteRefs)
				s->move(this->m_CurrentSpeed, 0);
			this->m_LifeBar.move(this->m_CurrentSpeed, 0);
			this->m_EquippedItemHighlight.move(this->m_CurrentSpeed, 0);
			break;
		}
	}
	
	fpsCount += fpsSpeed * time.restart().asSeconds();

	if (fpsCount >= switchFps)
	{
		if(this->m_timeSinceLastMove.getElapsedTime().asMilliseconds() < 500)
			this->m_Anim.x++;
		if (this->m_Anim.x * 32 >= this->m_Texture.getSize().x || this->m_timeSinceLastMove.getElapsedTime().asMilliseconds() > 500)
			this->m_Anim.x = 0;
		fpsCount = 0;
	}

	this->m_BlackSprite.setTextureRect(sf::IntRect((this->m_Anim.x * 32), (this->m_Anim.y * 32), 32, 32));
	this->m_Sprite.setTextureRect(sf::IntRect((this->m_Anim.x * 32), (this->m_Anim.y * 32), 32, 32));
	this->m_parentWindow->setView(*this->m_camera);

	this->m_LifeBar.setSize(sf::Vector2f(this->m_Health, 12));
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->m_Sprite);
}