#include "SortingNode.h"

SortingNode::SortingNode(const SortingNode& node) : Value(node.Value), X(node.X), Y(node.Y), Width(node.Width), RenderWindow(node.RenderWindow)
{
	VisualBar = sf::RectangleShape(sf::Vector2f(Width, Value));

	VisualBar.setOrigin(0, 0);
	VisualBar.rotate(180); // rotating 180 degrees to make the rectangles base be at the bottom of the screen and point upwards
	VisualBar.setPosition(X, Y);

	// Lerp colors
	sf::Color leftColor(sf::Color::Magenta);
	sf::Color rightColor(sf::Color::Cyan);
	
	sf::Color finalColor(
		(1 - (Value / RenderWindow->getSize().y)) * leftColor.r + (Value / RenderWindow->getSize().y) * rightColor.r,
		(1 - (Value / RenderWindow->getSize().y)) * leftColor.g + (Value / RenderWindow->getSize().y) * rightColor.g,
		(1 - (Value / RenderWindow->getSize().y)) * leftColor.b + (Value / RenderWindow->getSize().y) * rightColor.b
	);

	VisualBar.setFillColor(finalColor);
}

SortingNode::SortingNode(double value, float x, float y, float width, sf::RenderWindow& renderWindow) : Value(value), X(x), Y(y), Width(width), RenderWindow(&renderWindow)
{
	VisualBar = sf::RectangleShape(sf::Vector2f(width, Value));

	VisualBar.setOrigin(0, 0);
	VisualBar.rotate(180); // rotating 180 degrees to make the rectangles base be at the bottom of the screen and point upwards
	VisualBar.setPosition(X, Y);
	
	// Lerp colors
	int R = (X / RenderWindow->getSize().x) * 255;
	int G = rand() % 255;
	int B = (Y / RenderWindow->getSize().y) * 255;

	VisualBar.setFillColor(sf::Color(R, G, B));
}

SortingNode::~SortingNode()
{
}

void SortingNode::SetPosition(float x, float y)
{
	X = x;
	Y = y;
	VisualBar.setPosition(X, Y);
}

sf::Vector2f SortingNode::GetPosition()
{
	return sf::Vector2f(X, Y);
}

double SortingNode::GetValue()
{
	return Value;
}

void SortingNode::DrawNode()
{
	RenderWindow->draw(VisualBar);
}