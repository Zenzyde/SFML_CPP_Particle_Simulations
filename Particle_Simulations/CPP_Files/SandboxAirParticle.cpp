#include "SandboxAirParticle.h"

SandboxAirParticle::SandboxAirParticle(EParticleType _type, sf::Vector2f pos, sf::Vector2f size, SandboxGridNode& _nodeParent) : 
	SandboxParticle::SandboxParticle(_type, pos, size, _nodeParent)
{
	bUseGravity = true;
}

SandboxAirParticle::~SandboxAirParticle()
{
}

void SandboxAirParticle::Update()
{
	if (bUseGravity)
	{
		int nextNode = 0;
		std::shared_ptr<SandboxGridNode> node = NULL;

		SandboxGrid& grid = nodeParent->GetGrid();

		bool prioritizeLeft = (rand() % 100) > 50;

		nextNode = nodeParent->GetUpperNode();
		node = grid.GetNode(nextNode);

		if (prioritizeLeft)
		{
			if (node == NULL || node->IsOccupied())
			{
				nextNode = nodeParent->GetUpperLeftNode();
				node = grid.GetNode(nextNode);

				if (node == NULL || node->IsOccupied())
				{
					nextNode = nodeParent->GetUpperRightNode();
					node = grid.GetNode(nextNode);

					if (node == NULL || node->IsOccupied())
					{
						nextNode = nodeParent->GetLeftNode();
						node = grid.GetNode(nextNode);

						if (node == NULL || node->IsOccupied())
						{
							nextNode = nodeParent->GetRightNode();
							node = grid.GetNode(nextNode);

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
						grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode);
					}
				}
				else
				{
					grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode);
				}
			}
			else
			{
				grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode);
			}
		}
		else
		{
			if (node == NULL || node->IsOccupied())
			{
				nextNode = nodeParent->GetUpperRightNode();
				node = grid.GetNode(nextNode);

				if (node == NULL || node->IsOccupied())
				{
					nextNode = nodeParent->GetUpperLeftNode();
					node = grid.GetNode(nextNode);

					if (node == NULL || node->IsOccupied())
					{
						nextNode = nodeParent->GetRightNode();
						node = grid.GetNode(nextNode);

						if (node == NULL || node->IsOccupied())
						{
							nextNode = nodeParent->GetLeftNode();
							node = grid.GetNode(nextNode);

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
						grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode);
					}
				}
				else
				{
					grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode);
				}
			}
			else
			{
				grid.MoveParticleToGridNode(type, nodeParent->GetIndex(), nextNode);
			}
		}
	}
}
