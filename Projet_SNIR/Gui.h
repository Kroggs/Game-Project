#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>

class Gui : public sf::Drawable
{
public:

	virtual sf::Sprite* GetSpritePtr() = 0;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	sf::Texture Texture;
	sf::Sprite Sprite;
};

class Item_Slot : public Gui
{
public :

	Item_Slot();

	sf::Sprite* GetSpritePtr() 
	{
		sf::Sprite* tmp = &this->Sprite;
		return tmp;
	}

private :
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture Texture;
	sf::Sprite Sprite;
};

class LifeBar : public Gui
{
public:

	LifeBar();

	sf::Sprite* GetSpritePtr()
	{
		sf::Sprite* tmp = &this->Sprite;
		return tmp;
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture Texture;
	sf::Sprite Sprite;
};