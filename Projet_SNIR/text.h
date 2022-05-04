#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>



class Text : public sf::Drawable
{
public :
	
	enum Fonts {Pixer_Regular};
	enum Fades {IN, OUT};

	Text() {
		this->m_IsVisible = false;
	}

	Text(const std::string text, sf::Vector2f position, unsigned int size, sf::Color fillColor, const int font = Pixer_Regular);

	void FadeIn();
	void FadeOut();

	void SetVisible(const bool visibilty);
	void SetFont(const int font);

	void SetPosition(const sf::Vector2f pos);


private :
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool m_IsVisible;

	sf::Text m_Text;
	sf::Font m_Font;

};