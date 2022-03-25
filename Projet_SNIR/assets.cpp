#include "assets.h"

Rock::Rock()
{
	this->m_Texture.loadFromFile("images/assets/rock_1.png");
	this->m_Sprite.setTexture(this->m_Texture);

	this->m_hasCollision = true;
	this->m_isDestroyable = false;
	this->m_isMovable = false;
}

sf::Sprite Rock::GetSprite() const
{
	return this->m_Sprite;
}

void Rock::SetPosition(const sf::Vector2f newPos)
{
	this->m_Sprite.setPosition(newPos);
}

bool Rock::HasCollision() const
{
	return this->m_hasCollision;
}

void Rock::Interact()
{

}

void Rock::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->m_Sprite);
}