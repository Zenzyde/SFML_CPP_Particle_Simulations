#pragma once
#include "SandboxParticle.h"

#ifndef SANDBOXWATERPARTICLE
#define SANDBOXWATERPARTICLE

class SandboxWaterParticle : public SandboxParticle
{
public:
	SandboxWaterParticle(EParticleType type, sf::Vector2f pos, sf::Vector2f size, SandboxGridNode& _nodeParent);
	~SandboxWaterParticle();

	void Update() override;
};

#endif