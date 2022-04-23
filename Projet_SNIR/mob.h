#pragma once

#include <SFML/Graphics.hpp>

class mob : public sf::Drawable
{
public :

	virtual bool IsLiving() const = 0;
	
	virtual void Die() = 0;

private :

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	sf::Texture Texture;
	sf::Sprite Sprite;

	int HealthPoints;
};

