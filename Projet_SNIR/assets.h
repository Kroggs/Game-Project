#pragma once

#include "map.h"

class Rock : public Asset
{
public :
	Rock();
	void Interact();
	void SetPosition(const sf::Vector2f newPos);

	bool HasCollision() const;

	sf::Sprite GetSprite() const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool m_hasCollision;
	bool m_isDestroyable;
	bool m_isMovable;

	sf::Sprite m_Sprite;
	sf::Texture m_Texture;
};