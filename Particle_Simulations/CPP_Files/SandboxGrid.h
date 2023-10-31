#pragma once

#include "SFML/Graphics.hpp"
#include <vector>

#ifndef SANDBOXGRID
#define SANDBOXGRID

enum class EParticleType
{
	sand, wood, stone, water, acid, air, LAST_ENUM_VALUE
};

class SandboxGrid
{
public:
	SandboxGrid(int NodeSize, int Width, int Height, int StartX, int StartY);
	~SandboxGrid();

	void AddParticleToGrid(sf::Vector2f pos, EParticleType type, bool movedFromEarlierNode = false);
	void RemoveParticleFromGrid(sf::Vector2f pos);
	void SimulateSandbox(float deltaSeconds);
	void MoveParticleToGridNode(EParticleType type, int originalIndex, int newIndex, bool movedFromEarlierNode = false, bool switchNodes = false);

	std::shared_ptr<class SandboxGridNode> GetNode(int index);

	int UpperIndex(int index);
	int UpperLeftIndex(int index);
	int UpperRightIndex(int index);
	int LowerIndex(int index);
	int LowerLeftIndex(int index);
	int LowerRightIndex(int index);
	int LeftIndex(int index);
	int RightIndex(int index);

	void DrawSandbox(sf::RenderWindow& window, sf::Vector2f mousePos);
private:
	std::vector<std::shared_ptr<class SandboxGridNode>> nodeGrid;
	int sandboxWidth;
	int sandboxHeight;
	int nodeSize;
	float currentDeltaSeconds;
};

#endif