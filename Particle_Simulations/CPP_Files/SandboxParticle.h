#pragma once

#include "SFML/Graphics.hpp"
#include "SandboxGridNode.h"

#ifndef SANDBOXPARTICLE
#define SANDBOXPARTICLE

class SandboxParticle
{
public:
	SandboxParticle();
	SandboxParticle(ParticleType type, sf::Vector2f pos, sf::Vector2f size, SandboxGridNode& _nodeParent);
	~SandboxParticle();

	void DrawParticle(sf::RenderWindow& window);
	virtual void Update();
	void DrawUpperParticle(sf::RenderWindow& window);
	void DrawLowerParticle(sf::RenderWindow& window);
	ParticleType GetParticleType();

protected:
	std::unique_ptr<sf::RectangleShape> particle;
	bool bUseGravity;
	std::unique_ptr<SandboxGridNode> nodeParent;
	ParticleType type;
	int gravityDirection;
};

#endif