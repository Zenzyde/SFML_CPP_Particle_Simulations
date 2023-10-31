#include "SandboxGridNode.h"
#include "SandboxParticle.h"
#include "SandboxWaterParticle.h"
#include "SandboxAirParticle.h"
#include "SandboxAcidParticle.h"
#include "SandboxSandParticle.h"
#include <iostream>

using namespace std;

SandboxGridNode::SandboxGridNode(const SandboxGridNode& copy) :
	grid(copy.grid), pos(copy.pos), extents(copy.extents), index(copy.index), sandboxWidth(copy.sandboxWidth), sandboxHeight(copy.sandboxHeight), X(copy.X), Y(copy.Y), spawnedParticle(copy.spawnedParticle)
{
}

SandboxGridNode::SandboxGridNode(sf::Vector2f _pos, sf::Vector2f _extents, int _index, int width, int height, int x, int y, SandboxGrid& _grid) :
	pos(_pos), extents(_extents), index(_index), sandboxWidth(width), sandboxHeight(height), X(x), Y(y), spawnedParticle(false), grid(_grid), particle(nullptr)
{
}

SandboxGridNode::~SandboxGridNode()
{
}

sf::Vector2f SandboxGridNode::GetCenter()
{
	return sf::Vector2f(pos.x + (extents.x / 2.0f), pos.y + (extents.y / 2.0f));
}

sf::Vector2f SandboxGridNode::GetUpperLeft()
{
	return pos;
}

sf::Vector2f SandboxGridNode::GetUpperRight()
{
	return sf::Vector2f(pos.x + extents.x, pos.y);
}

sf::Vector2f SandboxGridNode::GetLowerLeft()
{
	return sf::Vector2f(pos.x, pos.y + extents.y);
}

sf::Vector2f SandboxGridNode::GetLowerRight()
{
	return sf::Vector2f(pos.x + extents.x, pos.y + extents.y);
}

void SandboxGridNode::DrawBox(sf::RenderWindow& window)
{
	sf::Vertex vertices[8];

	vertices[0].color = sf::Color::Green;
	vertices[0].position = pos;
	vertices[1].color = sf::Color::Green;
	vertices[1].position = sf::Vector2f(pos.x + extents.x, pos.y);

	vertices[2].color = sf::Color::Green;
	vertices[2].position = sf::Vector2f(pos.x + extents.x, pos.y);
	vertices[3].color = sf::Color::Green;
	vertices[3].position = sf::Vector2f(pos.x + extents.x, pos.y + extents.y);

	vertices[4].color = sf::Color::Green;
	vertices[4].position = sf::Vector2f(pos.x + extents.x, pos.y + extents.y);
	vertices[5].color = sf::Color::Green;
	vertices[5].position = sf::Vector2f(pos.x, pos.y + extents.y);

	vertices[6].color = sf::Color::Green;
	vertices[6].position = sf::Vector2f(pos.x, pos.y + extents.y);
	vertices[7].color = sf::Color::Green;
	vertices[7].position = pos;

	window.draw(vertices, 8, sf::Lines);
}

void SandboxGridNode::DrawFilledBox(sf::RenderWindow& window)
{
	sf::RectangleShape rectangle(extents);
	rectangle.setPosition(pos);

	if (X == sandboxWidth || X == 10)
	{
		rectangle.setFillColor(sf::Color::Red);
	}
	else
	{
		rectangle.setFillColor(sf::Color::Green);
	}
	
	window.draw(rectangle);
}

void SandboxGridNode::DrawParticle(sf::RenderWindow& window)
{
	if (particle == NULL || !spawnedParticle)
		return;

	if (spawnedParticle && particle == NULL)
	{
		//std::cout << "Spawned particle is null" << " : " << GetIndex() << std::endl;
		return;
	}

	particle->DrawParticle(window);
}

void SandboxGridNode::AddParticle(EParticleType type, bool wasMovedFromEarlierNode)
{
	spawnedParticle = true;

	if (wasMovedFromEarlierNode)
		particleWasMovedFromEarlierNode = true;

	switch (type)
	{
	case EParticleType::sand:
		particle = std::make_unique<SandboxSandParticle>(type, pos, extents, *this);
		break;
	case EParticleType::wood:
		particle = std::make_unique<SandboxParticle>(type, pos, extents, *this);
		break;
	case EParticleType::stone:
		particle = std::make_unique<SandboxParticle>(type, pos, extents, *this);
		break;
	case EParticleType::water:
		particle = std::make_unique<SandboxWaterParticle>(type, pos, extents, *this);
		break;
	case EParticleType::acid:
		particle = std::make_unique<SandboxAcidParticle>(type, pos, extents, *this);
		break;
	case EParticleType::air:
		particle = std::make_unique<SandboxAirParticle>(type, pos, extents, *this);
		break;
	}
}

void SandboxGridNode::RemoveParticle()
{
	if (spawnedParticle)
	{
		spawnedParticle = false;
		//particle->~SandboxParticle();
		particle.reset();
	}
}

bool SandboxGridNode::IsInsideNode(sf::Vector2f _pos)
{
	return _pos.x <= pos.x + extents.x && _pos.x >= pos.x && _pos.y <= pos.y + extents.y && _pos.y >= pos.y;
}

int SandboxGridNode::GetIndex()
{
	return index;
}

int SandboxGridNode::GetXIndex()
{
	return X;
}

int SandboxGridNode::GetYIndex()
{
	return Y;
}

bool SandboxGridNode::IsOccupied()
{
	return particle != NULL;
}

int SandboxGridNode::GetSandboxWidth()
{
	return sandboxWidth;
}

int SandboxGridNode::GetUpperNode()
{
	return index - ((sandboxWidth / extents.x));//- 1);
}

int SandboxGridNode::GetUpperLeftNode()
{
	return index - ((sandboxWidth / extents.x) - 1);
}

int SandboxGridNode::GetUpperRightNode()
{
	return index - ((sandboxWidth / extents.x) + 1);//- 2);
}

int SandboxGridNode::GetLowerNode()
{
	return index + ((sandboxWidth / extents.x));//- 1);
}

int SandboxGridNode::GetLowerLeftNode()
{
	return index + ((sandboxWidth / extents.x) - 1);
}

int SandboxGridNode::GetLowerRightNode()
{
	return index + ((sandboxWidth / extents.x) + 1);//- 2);
}

int SandboxGridNode::GetLeftNode()
{
	return index - 1;
}

int SandboxGridNode::GetRightNode()
{
	return index + 1;
}

SandboxGrid& SandboxGridNode::GetGrid()
{
	return grid;
}

void SandboxGridNode::UpdateNode()
{
	if (particle != NULL && !particleWasMovedFromEarlierNode)
	{
		particle->Update();
	}
	else if (particle != NULL && particleWasMovedFromEarlierNode)
	{
		particleWasMovedFromEarlierNode = false;
	}
}

EParticleType SandboxGridNode::GetParticleType()
{
	if (particle == NULL)
		return EParticleType::stone;
	return particle->GetParticleType();
}
