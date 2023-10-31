#pragma once
#include "SandboxParticle.h"

#ifndef SANDBOXAIRPARTICLE
#define SANDBOXAIRPARTICLE

class SandboxAirParticle : public SandboxParticle
{
public:
	SandboxAirParticle(EParticleType type, sf::Vector2f pos, sf::Vector2f size, SandboxGridNode& _nodeParent);
	~SandboxAirParticle();

	void Update() override;
};

#endif