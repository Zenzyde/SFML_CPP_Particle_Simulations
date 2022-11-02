#include "Button.h"
#include <iostream>

Button::Button(sf::Vector2f ButtonPosition, sf::Vector2f ButtonSize, sf::Color ButtonColor, sf::Vector2f TextPosition, sf::Vector2f TextSize, int characterSize, sf::Color Textcolor, std::string Text, sf::RenderWindow& Window) :
	buttonSprite(new sf::RectangleShape(ButtonSize)), state(false), RenderWindow(&Window)
{
	buttonSprite->setPosition(ButtonPosition);
	buttonSprite->setFillColor(ButtonColor);
	buttonSprite->setSize(ButtonSize);

	

	if (!buttonFont.loadFromFile("Fonts/Hey_Comic.ttf"))
	{
		std::cerr << "Failed to load font: " << buttonFont.getInfo().family << std::endl;
		char input;
		std::cin >> input;
		return;
	}

	buttonText = std::make_shared<sf::Text>();

	buttonText->setFont(buttonFont);
	buttonText->setString(Text);
	buttonText->setCharacterSize(characterSize);
	buttonText->setStyle(sf::Text::Bold);

	buttonText->setPosition(TextPosition);
	buttonText->setScale(TextSize);
	buttonText->setFillColor(sf::Color::White);
}

Button::~Button()
{
}

bool Button::WasClicked(sf::Vector2f MousePos)
{
	sf::Vector2f buttonPos(buttonSprite->getPosition());
	sf::Vector2f buttonSize(buttonSprite->getSize());
	if (MousePos.x <= buttonPos.x + buttonSize.x && MousePos.x >= buttonPos.x &&
		MousePos.y <= buttonPos.y + buttonSize.y && MousePos.y >= buttonPos.y)
	{
		state = !state;
		return true;
	}
	return false;
}

void Button::SetText(std::string text)
{
	buttonText->setString(text);
}

bool Button::GetButtonState()
{
	return state;
}

void Button::DrawButton()
{
	RenderWindow->draw(*buttonSprite);
	RenderWindow->draw(*buttonText);
}
