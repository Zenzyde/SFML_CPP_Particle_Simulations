#pragma once

#include "SFML/Graphics.hpp"
#include "SandboxGrid.h"

#ifndef SANDBOXGRIDNODE
#define SANDBOXGRIDNODE

class SandboxGridNode
{
public:
	SandboxGridNode(const SandboxGridNode& copy);
	SandboxGridNode(sf::Vector2f pos, sf::Vector2f extents, int index, int width, int height, int x, int y, SandboxGrid& _grid);
	~SandboxGridNode();

	sf::Vector2f GetCenter();
	sf::Vector2f GetUpperLeft();
	sf::Vector2f GetUpperRight();
	sf::Vector2f GetLowerLeft();
	sf::Vector2f GetLowerRight();
	void DrawBox(sf::RenderWindow& window);
	void DrawFilledBox(sf::RenderWindow& window);
	void DrawParticle(sf::RenderWindow& window);
	void AddParticle(ParticleType type, bool wasMovedFromEarlierNode = false);
	void RemoveParticle();
	bool IsInsideNode(sf::Vector2f pos);
	int GetIndex();
	int GetXIndex();
	int GetYIndex();
	bool IsOccupied();
	int GetSandboxWidth();
	int GetUpperNode();
	int GetUpperLeftNode();
	int GetUpperRightNode();
	int GetLowerNode();
	int GetLowerLeftNode();
	int GetLowerRightNode();
	int GetLeftNode();
	int GetRightNode();
	SandboxGrid& GetGrid();
	void UpdateNode();
	ParticleType GetParticleType();

private:
	sf::Vector2f pos;
	sf::Vector2f extents;
	int index;
	int sandboxWidth;
	int sandboxHeight;
	int X;
	int Y;
	std::unique_ptr<class SandboxParticle> particle;
	bool spawnedParticle;
	SandboxGrid& grid;
	bool particleWasMovedFromEarlierNode;
};

#endif