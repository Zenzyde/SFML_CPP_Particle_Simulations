#pragma once
#include "SandboxWaterParticle.h"

#ifndef SANDBOXACIDPARTICLE
#define SANDBOXACIDPARTICLE

class SandboxAcidParticle : public SandboxWaterParticle
{
public:
	SandboxAcidParticle(EParticleType type, sf::Vector2f pos, sf::Vector2f size, SandboxGridNode& _nodeParent);
	~SandboxAcidParticle();

	void Update() override;
};

#endif