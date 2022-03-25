#include "item.h"
#include <iostream>

HealthPotion::HealthPotion()
{
	if (!this->m_IconTexture.loadFromFile("images/items/red_potion_full.png"))
		std::cout << "Couldn't load : red_potion_full.png" << std::endl;

	this->m_IconSprite.setTexture(this->m_IconTexture);

	this->fpsCount = 0;
	this->switchFps = 100;
	this->fpsSpeed = 1000;

	this->m_Anim.x = 0;
	this->m_Anim.y = 0;
}

void HealthPotion::Update()
{
	fpsCount += fpsSpeed * time.restart().asSeconds();
	if (fpsCount >= switchFps)
	{
		this->m_Anim.x++;
		if (this->m_Anim.x * 16 >= this->m_IconTexture.getSize().x) {
			this->m_Anim.x = 0;
			this->m_Anim.y++;

			if(this->m_Anim.y == 2){
				this->m_Anim.y = 0;
			}
		}

		fpsCount = 0;
	}

	this->m_IconSprite.setTextureRect(sf::IntRect((this->m_Anim.x * 16), (this->m_Anim.y * 16), 16, 16));
}

void HealthPotion::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->m_IconSprite);
}

sf::Sprite* HealthPotion::sprite()
{
	return &this->m_IconSprite;
}