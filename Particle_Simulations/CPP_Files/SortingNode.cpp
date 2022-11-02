#include "SortingNode.h"

SortingNode::SortingNode(const SortingNode& node) : Value(node.Value), X(node.X), Y(node.Y), Width(node.Width), RenderWindow(node.RenderWindow)
{
	VisualBar = sf::RectangleShape(sf::Vector2f(Width, Value));

	VisualBar.setOrigin(0, 0);
	VisualBar.rotate(180); // rotating 180 degrees to make the rectangles base be at the bottom of the screen and point upwards
	VisualBar.setPosition(X, Y);

	// Lerp colors
	int R = (X / RenderWindow->getSize().x) * 255;
	int G = rand() % 255;
	int B = (Y / RenderWindow->getSize().y) * 255;

	VisualBar.setFillColor(sf::Color(R, G, B));
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
	
	/*sf::Vertex lines[4];
	lines[0].position = VisualBar.getPosition();
	lines[0].color = sf::Color::Red;

	lines[1].position = VisualBar.getOrigin();
	lines[1].color = sf::Color::Green;

	lines[2].position = VisualBar.getPosition();
	lines[2].color = sf::Color::Blue;

	lines[3].position = sf::Vector2f(RenderWindow.getSize().x / 2.f, RenderWindow.getSize().y / 2.0f);
	lines[3].color = sf::Color::Yellow;
	
	RenderWindow.draw(lines, 4, sf::Lines);*/
}