#pragma once

#include <SFML/Graphics.hpp>

class Item : public sf::Drawable
{
public :

	virtual sf::Sprite* sprite() = 0;
	virtual void Update() = 0;

private :
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	sf::Sprite m_IconSprite;
	sf::Texture m_IconTexture;
};


class HealthPotion : public Item
{
public:

	sf::Sprite* sprite();
	void Update();

	HealthPotion();
	
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Sprite m_IconSprite;
	sf::Texture m_IconTexture;

	sf::Vector2i m_Anim;
	sf::Clock time;
	float fpsCount,
		  switchFps,
		  fpsSpeed;
};