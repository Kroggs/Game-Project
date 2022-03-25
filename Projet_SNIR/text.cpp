#include "text.h"

Text::Text(const std::string text, sf::Vector2f position, unsigned int size, sf::Color fillColor, const int font)
{
	this->SetFont(font);
	this->m_Text.setFont(this->m_Font);
	this->m_Text.setString(text);
	this->m_Text.setCharacterSize(size);
	this->m_Text.setPosition(position);
	this->m_Text.setFillColor(fillColor);
}

void Text::FadeIn()
{
	sf::Clock clock;
	int size = 0, size2 = this->m_Text.getCharacterSize();
	this->m_Text.setCharacterSize(0);
	for (unsigned int i = 0; i < size2; ++i) {
		if (clock.getElapsedTime().asMilliseconds() >= 20) {
			clock.restart();
			++size;
			this->m_Text.setCharacterSize(size);
		}
	}
}

void Text::FadeOut()
{
	sf::Clock clock;
	for (unsigned int size = this->m_Text.getCharacterSize(); size > 0; --size) {
		if (clock.getElapsedTime().asMilliseconds() >= 20) {
			clock.restart();
			this->m_Text.setCharacterSize(size);
		}
		else ++size;
	}
	std::cout << clock.getElapsedTime().asMilliseconds() << std::endl;
}

void Text::SetVisible(const bool visibilty)
{
	this->m_IsVisible = visibilty;
}

void Text::SetFont(const int font)
{
	switch (font)
	{
	case Pixer_Regular:
		this->m_Font.loadFromFile("fonts/Pixer-Regular.ttf");
		break;
	}
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(this->m_IsVisible)
		target.draw(this->m_Text);
}