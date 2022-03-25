#pragma once

#include <SFML/Graphics.hpp>
#include "assets.h"
#include "item.h"

namespace dir
{
	enum dir{DOWN, LEFT, RIGHT, UP};
}


class Player : public sf::Drawable
{
public:

	Player();

	void Update();

	void SetCurrentMapLocation(Map* map);
	void SetView(sf::View* view, sf::RenderWindow* window);
	void RenderShadow();

	void SetRefSprite(sf::Sprite* refSprite);

	bool IsColliding();
	bool IsBehindTile() const;
	sf::Vector2f GetPosition() const;

	void AddItem(Item* item);
	void DrawInventoryItems();
	
	void TakeDamage(const int amount);

	sf::RectangleShape GetLifebar() const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::vector<sf::RectangleShape> tileCollisions, thirdLayerTiles;
	sf::RectangleShape m_LifeBar;

	int m_Health, m_DamageRecieved;
	bool m_IsDead, m_IsDamageAnimOver;
	std::vector<sf::Sprite*> m_SpriteRefs;

	sf::Sprite m_Sprite, m_BlackSprite;
	sf::Texture m_Texture, m_Texture_Black;
	sf::Vector2i m_Anim;

	std::vector<Item*> m_Inventory;
	
	sf::View* m_camera;
	sf::RenderWindow* m_parentWindow;

	Map* m_CurrentMap;

	sf::Clock m_timeSinceLastMove, m_timeSinceLastHealthDecrease;

	float m_CurrentSpeed, m_Acceleration, m_MaxSpeed;
	bool m_IsMoving, m_isBehindTile;

	int m_InventorySize, m_InventoryAmount;

	sf::Clock time;
	float fpsCount,
		  switchFps,
		  fpsSpeed;
};