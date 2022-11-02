#include "SandboxSandParticle.h"

SandboxSandParticle::SandboxSandParticle(ParticleType type, sf::Vector2f pos, sf::Vector2f size, SandboxGridNode& _nodeParent) :
	SandboxParticle(type, pos, size, _nodeParent)
{
	bUseGravity = true;
}

SandboxSandParticle::~SandboxSandParticle()
{
}

void SandboxSandParticle::Update()
{
	if (bUseGravity)
	{
		int nextNode = 0;
		std::shared_ptr<SandboxGridNode> node = NULL;

		SandboxGrid& grid = nodeParent->GetGrid();

		bool prioritizeLeft = (rand() % 100) > 50;

		nextNode = nodeParent->GetLowerNode();
		node = nodeParent->GetGrid().GetNode(nextNode);

		if (node == NULL || node->IsOccupied())
		{
			nextNode = nodeParent->GetLowerLeftNode();
			node = nodeParent->GetGrid().GetNode(nextNode);

			if (node == NULL || node->IsOccupied())
			{
				nextNode = nodeParent->GetLowerRightNode();
				node = nodeParent->GetGrid().GetNode(nextNode);

				if (node == NULL || node->IsOccupied())
					return;
			}
		}

		grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode, true);
	}
}
