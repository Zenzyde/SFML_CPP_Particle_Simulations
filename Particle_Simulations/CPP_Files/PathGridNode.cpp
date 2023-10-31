#include "PathGridNode.h"
#include <iostream>

using namespace std;

PathGridNode::PathGridNode(sf::Vector2f position, sf::Vector2f size, ENodeType type) : position(position), size(size), nodeType(type), previousNodeType(type), shape(new sf::RectangleShape(size)), path(nullptr)
{
	bIsTraversable = true;
	fScore = 100000;
	gScore = 100000;
	shape->setPosition(position);
	shape->setFillColor(sf::Color::Transparent);
	shape->setOutlineColor(sf::Color::Red);
	shape->setOutlineThickness(0.9f);
}

PathGridNode::PathGridNode(const PathGridNode* node) : position(node->position), size(node->size), nodeType(node->nodeType), previousNodeType(node->nodeType), shape(new sf::RectangleShape(node->size)), path(nullptr)
{
	bIsTraversable = true;
	fScore = 100000;
	gScore = 100000;
	shape->setPosition(position);
	shape->setFillColor(sf::Color::Transparent);
	shape->setOutlineColor(sf::Color::Red);
	shape->setOutlineThickness(0.9f);
}

PathGridNode::PathGridNode(const PathGridNode& node) : position(node.position), size(node.size), nodeType(node.nodeType), previousNodeType(node.nodeType), shape(new sf::RectangleShape(node.size)), path(nullptr)
{
	bIsTraversable = true;
	fScore = 100000;
	gScore = 100000;
	shape->setPosition(position);
	shape->setFillColor(sf::Color::Transparent);
	shape->setOutlineColor(sf::Color::Red);
	shape->setOutlineThickness(0.9f);
}

PathGridNode::PathGridNode() : position(), size(), nodeType(), previousNodeType(), shape(new sf::RectangleShape()), path(nullptr)
{
	bIsTraversable = false;
	fScore = -1;
	gScore = -1;
	shape->setPosition(position);
	shape->setFillColor(sf::Color::Magenta);
	shape->setOutlineColor(sf::Color::Cyan);
	shape->setOutlineThickness(0.9f);
}

PathGridNode::~PathGridNode()
{
}

sf::Vector2f PathGridNode::GetPosition()
{
	return position;
}

int PathGridNode::GetGScore()
{
	return gScore;
}

int PathGridNode::GetFScore()
{
	return fScore;
}

void PathGridNode::SetFScore(int fScore)
{
	this->fScore = fScore;
}

void PathGridNode::SetGScore(int gScore)
{
	this->gScore = gScore;
}

std::vector<PathGridNode*> PathGridNode::GetPath()
{
	std::vector<PathGridNode*> pathStack;
	while (path != nullptr)
	{
		if (path->GetNodeType() == ENodeType::Start)
		{
			path = nullptr;
			break;
		}
		pathStack.push_back(path);
		path = path->path;
	}
	return pathStack;
}

PathGridNode* PathGridNode::GetSinglePath()
{
	return path;
}

void PathGridNode::SetPreviousPath(PathGridNode* previousPath)
{
	path = previousPath;
}

void PathGridNode::SetNodeType(ENodeType type)
{
	if (type == ENodeType::Occupied)
		previousNodeType = nodeType;
	nodeType = type;
	bIsTraversable = true;
	switch (type)
	{
	case ENodeType::Empty:
		shape->setFillColor(sf::Color::Transparent);
		break;
	case ENodeType::Occupied:
		shape->setFillColor(sf::Color::White);
		break;
	case ENodeType::Blocked:
		bIsTraversable = false;
		shape->setFillColor(sf::Color::Red);
		break;
	case ENodeType::Start:
		shape->setFillColor(sf::Color::Cyan);
		break;
	case ENodeType::Goal:
		shape->setFillColor(sf::Color::Yellow);
		break;
	case ENodeType::Hazard:
		shape->setFillColor(sf::Color::Magenta);
		break;
	case ENodeType::Safe:
		shape->setFillColor(sf::Color::Green);
		break;
	default:
		break;
	}
}

ENodeType PathGridNode::GetNodeType()
{
	return nodeType;
}

void PathGridNode::Draw(sf::RenderWindow& window)
{
	if (!shape)
		return;

	window.draw(*shape);
}

bool PathGridNode::IsInsideNode(sf::Vector2f position)
{
	return position.x >= this->position.x && position.x < this->position.x + size.x && position.y >= this->position.y && position.y < this->position.y + size.y;
}

bool PathGridNode::IsTraversable()
{
	return bIsTraversable;
}

void PathGridNode::ResetNode()
{
	fScore = 100000;
	gScore = 100000;
	if (nodeType == ENodeType::Occupied)
		SetNodeType(previousNodeType);
}
