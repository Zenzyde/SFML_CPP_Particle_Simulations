#include "SFML/Graphics.hpp"

#pragma once
class SortingNode
{
public:
	SortingNode(const SortingNode& node);
	SortingNode(double value, float x, float y, float width, sf::RenderWindow& renderWindow);
	~SortingNode();

	void SetPosition(float x, float y);
	sf::Vector2f GetPosition();
	double GetValue();
	void DrawNode();

private:
	double Value;
	float X, Y;
	float Width;

	sf::RectangleShape VisualBar;

	sf::RenderWindow* RenderWindow;
};

