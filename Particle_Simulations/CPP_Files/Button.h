#include "SFML/Graphics.hpp"

#pragma once
class Button
{
public:
	Button(sf::Vector2f ButtonPosition, sf::Vector2f ButtonSize, sf::Color ButtonColor, sf::Vector2f TextPosition, sf::Vector2f TextSize, int characterSize, sf::Color Textcolor, std::string Text, sf::RenderWindow& window);
	~Button();

	bool WasClicked(sf::Vector2f MousePos);
	void SetText(std::string text);
	bool GetButtonState();

	void DrawButton();

private:
	std::shared_ptr<sf::RectangleShape> buttonSprite;
	std::shared_ptr<sf::Text> buttonText;

	sf::Font buttonFont;
	bool state;
	sf::RenderWindow* RenderWindow;
};

