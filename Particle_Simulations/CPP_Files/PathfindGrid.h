#include <vector>
#include "PathGridNode.h"
#include <map>

using namespace std;

#pragma once

class PathfindGrid
{
public:
	PathfindGrid(sf::Vector2f gridSize, sf::Vector2f nodeSize, sf::Vector2f gridOffset);
	PathfindGrid(const PathfindGrid& gridCopy);
	~PathfindGrid();

	void FindPath();
	void ResetPath();
	void SetNodeType(sf::Vector2f position, ENodeType type);
	void Draw(sf::RenderWindow& window);
private:
	vector<vector<PathGridNode>> grid;
	int basePathCost;
	map<ENodeType, int> pathTypeCost;
	sf::Vector2f gridSize;
	sf::Vector2f nodeSize;
	sf::Vector2f gridOffset;
	float Distance(sf::Vector2f a, sf::Vector2f b);
	ENodeType GetNodeType(sf::Vector2f position);
	PathGridNode* GetNode(sf::Vector2f position);
	int GetHeuristic(sf::Vector2f position);
	int GetWeightToNeighbour(sf::Vector2f current, sf::Vector2f neighbour);
	PathGridNode* start;
	PathGridNode* goal;
	std::vector<PathGridNode*> path;
};

