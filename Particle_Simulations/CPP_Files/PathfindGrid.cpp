#include "PathfindGrid.h"
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

struct CompareNode
{
	bool operator()(PathGridNode* a, PathGridNode* b)
	{
		return a->GetFScore() > b->GetFScore();
	}
};

PathfindGrid::PathfindGrid(sf::Vector2f gridSize, sf::Vector2f nodeSize, sf::Vector2f gridOffset) : gridSize(gridSize), nodeSize(nodeSize), gridOffset(gridOffset), start(nullptr), goal(nullptr)
{
	pathTypeCost.insert({ENodeType::Empty, 15});
	pathTypeCost.insert({ENodeType::Blocked, 10000});
	pathTypeCost.insert({ENodeType::Occupied, 1});
	pathTypeCost.insert({ENodeType::Start, 0});
	pathTypeCost.insert({ENodeType::Goal, 0});
	pathTypeCost.insert({ENodeType::Hazard, 150});
	pathTypeCost.insert({ENodeType::Safe, 10});

	for (size_t i = 0; i < gridSize.x; i+= nodeSize.x)
	{
		vector<PathGridNode> toAdd;
		for (size_t j = 0; j < gridSize.y; j+= nodeSize.y)
		{
			sf::Vector2f position(i, j);
			position += gridOffset;
			PathGridNode node(position, nodeSize, ENodeType::Empty);
			toAdd.push_back(node);
		}
		grid.push_back(toAdd);
	}

	basePathCost = 1;
}

PathfindGrid::PathfindGrid(const PathfindGrid& gridCopy) : gridSize(gridCopy.gridSize), nodeSize(gridCopy.nodeSize), gridOffset(gridCopy.gridOffset), start(gridCopy.start), goal(gridCopy.goal)
{
	pathTypeCost = gridCopy.pathTypeCost;

	for (size_t i = 0; i < gridCopy.gridSize.x; i += nodeSize.x)
	{
		vector<PathGridNode> toAdd;
		for (size_t j = 0; j < gridCopy.gridSize.y; j += nodeSize.y)
		{
			sf::Vector2f position(i, j);
			position += gridCopy.gridOffset;
			PathGridNode node(position, gridCopy.nodeSize, ENodeType::Empty);
			toAdd.push_back(node);
		}
		grid.push_back(toAdd);
	}

	basePathCost = 1;
}

PathfindGrid::~PathfindGrid()
{
}

// A* algorithm reference: https://en.wikipedia.org/wiki/A*_search_algorithm
void PathfindGrid::FindPath()
{
	ResetPath();

	std::vector<sf::Vector2f> offsets
	{
		sf::Vector2f(0, -1), //up
		sf::Vector2f(nodeSize.x, 0), //right
		sf::Vector2f(0, nodeSize.y), //down
		sf::Vector2f(-1, 0) //left
	};

	std::priority_queue<PathGridNode*, std::vector<PathGridNode*>, CompareNode> nodeQueue;
	nodeQueue.push(start);

	std::vector<PathGridNode*> nodeChecklist;
	nodeChecklist.push_back(start);

	int fScore = GetHeuristic(start->GetPosition());
	start->SetFScore(fScore);
	start->SetGScore(-100000);

	while (!nodeQueue.empty())
	{
		PathGridNode* current = nodeQueue.top();
		
		if (current->GetNodeType() == ENodeType::Goal)
		{
			nodeChecklist.clear();
			while (!nodeQueue.empty())
				nodeQueue.pop();
			path = current->GetPath();
			return;
		}

		nodeQueue.pop();

		for (sf::Vector2f offset : offsets)
		{
			sf::Vector2f neighbourPos(current->GetPosition() + offset);
			PathGridNode* neighbour = GetNode(neighbourPos);
			
			if (neighbour == nullptr || !neighbour->IsTraversable())
				continue;
			
			int temp_gScore = current->GetGScore() + GetWeightToNeighbour(current->GetPosition(), neighbour->GetPosition());

			if (temp_gScore < neighbour->GetGScore())
			{
				neighbour->SetPreviousPath(current);
				neighbour->SetFScore(temp_gScore + GetHeuristic(neighbour->GetPosition()));
				neighbour->SetGScore(temp_gScore);
				
				if (std::find(nodeChecklist.begin(), nodeChecklist.end(), neighbour) == nodeChecklist.end())
				{
					nodeQueue.push(neighbour);
					nodeChecklist.push_back(neighbour);
				}
			}
		}
	}
}

void PathfindGrid::ResetPath()
{
	if (path.size() == 0)
		return;

	path.clear();
	for (size_t i = 0; i < grid.size(); i++)
	{
		for (size_t j = 0; j < grid[i].size(); j++)
		{
			grid[i][j].ResetNode();
		}
	}
}

void PathfindGrid::SetNodeType(sf::Vector2f position, ENodeType type)
{
	for (size_t i = 0; i < grid.size(); i++)
	{
		for (size_t j = 0; j < grid[i].size(); j++)
		{
			if (grid[i][j].IsInsideNode(position))
			{
				if (type == ENodeType::Start && start == nullptr)
				{
					start = &grid[i][j];
					start->SetNodeType(type);
				}
				else if (type == ENodeType::Goal && goal == nullptr)
				{
					goal = &grid[i][j];
					goal->SetNodeType(type);
				}
				else if (type != ENodeType::Start && type != ENodeType::Goal)
				{
					if (grid[i][j].GetNodeType() == ENodeType::Start)
						start = nullptr;
					if (grid[i][j].GetNodeType() == ENodeType::Goal)
						goal = nullptr;
					grid[i][j].SetNodeType(type);
				}
				return;
			}
		}
	}
}

ENodeType PathfindGrid::GetNodeType(sf::Vector2f position)
{
	for (size_t i = 0; i < grid.size(); i++)
	{
		for (size_t j = 0; j < grid[i].size(); j++)
		{
			if (grid[i][j].IsInsideNode(position))
				return grid[i][j].GetNodeType();
		}
	}
}

PathGridNode* PathfindGrid::GetNode(sf::Vector2f position)
{
	for (size_t i = 0; i < grid.size(); i++)
	{
		for (size_t j = 0; j < grid[i].size(); j++)
		{
			if (grid[i][j].IsInsideNode(position))
				return &grid[i][j];
		}
	}
	return nullptr;
}

int PathfindGrid::GetHeuristic(sf::Vector2f position)
{
	PathGridNode* node = GetNode(position);
	return basePathCost + Distance(node->GetPosition(), goal->GetPosition()) + pathTypeCost[node->GetNodeType()];
}

int PathfindGrid::GetWeightToNeighbour(sf::Vector2f current, sf::Vector2f neighbour)
{
	PathGridNode* next = GetNode(neighbour);
	return basePathCost + Distance(current, neighbour) + pathTypeCost[next->GetNodeType()];
}

void PathfindGrid::Draw(sf::RenderWindow& window)
{
	for (size_t i = 0; i < grid.size(); i++)
	{
		for (size_t j = 0; j < grid[i].size(); j++)
		{
			grid[i][j].Draw(window);
		}
	}
	
	if (path.size() == 0)
		return;

	for (size_t k = 0; k < path.size(); k++)
	{
		if (path[k]->GetNodeType() != ENodeType::Occupied)
			path[k]->SetNodeType(ENodeType::Occupied);
		path[k]->Draw(window);
	}
}

float PathfindGrid::Distance(sf::Vector2f a, sf::Vector2f b)
{
	// Euclidean distance: sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2))
	// Manhatan distance: abs(a.x - b.x) + abs(a.y - b.y)
	return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
}