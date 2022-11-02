#pragma once
#include "SandboxParticle.h"

#ifndef SANDBOXSANDPARTICLE
#define SANDBOXSANDPARTICLE

class SandboxSandParticle : public SandboxParticle
{
public:
	SandboxSandParticle(ParticleType type, sf::Vector2f pos, sf::Vector2f size, SandboxGridNode& _nodeParent);
	~SandboxSandParticle();

	void Update() override;
};

#endif