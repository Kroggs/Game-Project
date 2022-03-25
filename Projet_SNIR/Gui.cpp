#include "Gui.h"

#include <iostream>
#include <fstream>

Item_Slot::Item_Slot()
{
	if (!this->Texture.loadFromFile("images/GUI/item_slot.png"))
		std::cout << "Couldn't load : Item_Slot" << std::endl;
	else {
		this->Sprite.setTexture(this->Texture);
		this->Sprite.setPosition(sf::Vector2f(350, 700));
	}
}


void Item_Slot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->Sprite);
}

LifeBar::LifeBar()
{
	if (!this->Texture.loadFromFile("images/GUI/lifebar.png"))
		std::cout << "Couldn't load : LifeBar" << std::endl;
	else {
		this->Sprite.setTexture(this->Texture);
		this->Sprite.setPosition(sf::Vector2f(80, 10));
	}
}


void LifeBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->Sprite);
}