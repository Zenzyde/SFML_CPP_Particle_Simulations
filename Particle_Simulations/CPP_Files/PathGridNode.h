#include <SFML/Graphics.hpp>

#pragma once

enum class ENodeType
{
	Empty,
	Occupied,
	Blocked,
	Start,
	Goal,
	Hazard,
	Safe,
	LAST_ENUM_VALUE
};

class PathGridNode
{
public:
	PathGridNode(sf::Vector2f position, sf::Vector2f size, ENodeType type);
	PathGridNode(const PathGridNode* node);
	PathGridNode(const PathGridNode& node);
	PathGridNode();
	~PathGridNode();

	sf::Vector2f GetPosition();
	int GetGScore();
	int GetFScore();
	void SetFScore(int fScore);
	void SetGScore(int gScore);
	std::vector<PathGridNode*> GetPath();
	PathGridNode* GetSinglePath();
	void SetPreviousPath(PathGridNode* previousPath);
	void SetNodeType(ENodeType type);
	ENodeType GetNodeType();
	void Draw(sf::RenderWindow& window);
	bool IsInsideNode(sf::Vector2f position);
	bool IsTraversable();
	void ResetNode();
private:
	sf::Vector2f position;
	sf::Vector2f size;
	std::unique_ptr<sf::RectangleShape> shape;

	int fScore;
	int gScore;
	PathGridNode* path;
	bool bIsTraversable;
	ENodeType nodeType;
	ENodeType previousNodeType;
};

