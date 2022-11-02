#include "SandboxWaterParticle.h"

SandboxWaterParticle::SandboxWaterParticle(ParticleType _type, sf::Vector2f pos, sf::Vector2f size, SandboxGridNode& _nodeParent) :
	SandboxParticle(_type, pos, size, _nodeParent)
{
	bUseGravity = true;
}

SandboxWaterParticle::~SandboxWaterParticle()
{
}

void SandboxWaterParticle::Update()
{
	if (bUseGravity)
	{
		int nextNode = 0;
		std::shared_ptr<SandboxGridNode> node = NULL;

		SandboxGrid& grid = nodeParent->GetGrid();

		bool prioritizeLeft = (rand() % 100) > 50;

		nextNode = nodeParent->GetLowerNode();
		node = nodeParent->GetGrid().GetNode(nextNode);

		if (prioritizeLeft)
		{
			if (node == NULL || node->IsOccupied())
			{
				nextNode = nodeParent->GetLowerLeftNode();
				node = nodeParent->GetGrid().GetNode(nextNode);

				if (node == NULL || node->IsOccupied())
				{
					nextNode = nodeParent->GetLowerRightNode();
					node = nodeParent->GetGrid().GetNode(nextNode);

					if (node == NULL || node->IsOccupied())
					{
						nextNode = nodeParent->GetLeftNode();
						node = nodeParent->GetGrid().GetNode(nextNode);

						if (node == NULL || node->IsOccupied())
						{
							nextNode = nodeParent->GetRightNode();
							node = nodeParent->GetGrid().GetNode(nextNode);

							if (node == NULL || node->IsOccupied())
								return;

							grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode, true);
						}
						else
						{
							grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode);
						}
					}
					else
					{
						grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode, true);
					}
				}
				else
				{
					grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode, true);
				}
			}
			else
			{
				grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode, true);
			}
		}
		else
		{
			if (node == NULL || node->IsOccupied())
			{
				nextNode = nodeParent->GetLowerRightNode();
				node = nodeParent->GetGrid().GetNode(nextNode);

				if (node == NULL || node->IsOccupied())
				{
					nextNode = nodeParent->GetLowerLeftNode();
					node = nodeParent->GetGrid().GetNode(nextNode);

					if (node == NULL || node->IsOccupied())
					{
						nextNode = nodeParent->GetRightNode();
						node = nodeParent->GetGrid().GetNode(nextNode);

						if (node == NULL || node->IsOccupied())
						{
							nextNode = nodeParent->GetLeftNode();
							node = nodeParent->GetGrid().GetNode(nextNode);

							if (node == NULL || node->IsOccupied())
								return;

							grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode);
						}
						else
						{
							grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode, true);
						}
					}
					else
					{
						grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode, true);
					}
				}
				else
				{
					grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode, true);
				}
			}
			else
			{
				grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode, true);
			}
		}
	}
}
