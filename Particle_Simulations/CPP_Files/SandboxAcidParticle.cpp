#include "SandboxAcidParticle.h"

SandboxAcidParticle::SandboxAcidParticle(ParticleType _type, sf::Vector2f pos, sf::Vector2f size, SandboxGridNode& _nodeParent) :
	SandboxWaterParticle(_type, pos, size, _nodeParent)
{
	bUseGravity = true;
}

SandboxAcidParticle::~SandboxAcidParticle()
{
}

void SandboxAcidParticle::Update()
{
	if (bUseGravity)
	{
		int nextNode = 0;
		std::shared_ptr<SandboxGridNode> node = NULL;

		SandboxGrid& grid = nodeParent->GetGrid();

		nextNode = nodeParent->GetUpperNode();
		node = nodeParent->GetGrid().GetNode(nextNode);

		if (node != NULL && node->IsOccupied() && node->GetParticleType() != ParticleType::acid)
		{
			grid.RemoveParticleFromGrid(node->GetCenter());
		}

		nextNode = nodeParent->GetLeftNode();
		node = nodeParent->GetGrid().GetNode(nextNode);

		if (node != NULL && node->IsOccupied() && node->GetParticleType() != ParticleType::acid)
		{
			grid.RemoveParticleFromGrid(node->GetCenter());
		}

		nextNode = nodeParent->GetRightNode();
		node = nodeParent->GetGrid().GetNode(nextNode);

		if (node != NULL && node->IsOccupied() && node->GetParticleType() != ParticleType::acid)
		{
			grid.RemoveParticleFromGrid(node->GetCenter());
		}

		nextNode = nodeParent->GetLowerNode();
		node = nodeParent->GetGrid().GetNode(nextNode);

		if (node == NULL)
			return;

		grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode, true);
	}
}
