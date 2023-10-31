#include "SandboxGrid.h"
#include "SandboxGridNode.h"
#include <iostream>

using namespace std;

SandboxGrid::SandboxGrid(int NodeSize, int Width, int Height, int StartX, int StartY)
{
    sandboxWidth = Width;
    sandboxHeight = Height;
    nodeSize = NodeSize;

    int index = 0;
    for (size_t y = StartY; y <= Height; y += NodeSize)
    {
        for (size_t x = StartX; x <= Width; x += NodeSize)
        {
            sf::Vector2f pos(x, y);

            sf::Vector2f extents(NodeSize, NodeSize);

            std::shared_ptr<SandboxGridNode> node = std::make_shared<SandboxGridNode>(pos, extents, index, Width, Height, x, y, *this);

            nodeGrid.push_back(node);

            index++;
        }
    }
}

SandboxGrid::~SandboxGrid()
{
}

void SandboxGrid::AddParticleToGrid(sf::Vector2f pos, EParticleType type, bool movedFromEarlierNode)
{
    for (size_t i = 0; i < nodeGrid.size(); i++)
    {
        if (nodeGrid[i]->IsInsideNode(pos) && !nodeGrid[i]->IsOccupied())
        {
            nodeGrid[i]->AddParticle(type, movedFromEarlierNode);
        }
    }
}

void SandboxGrid::RemoveParticleFromGrid(sf::Vector2f pos)
{
    for (size_t i = 0; i < nodeGrid.size(); i++)
    {
        if (nodeGrid[i]->IsInsideNode(pos))
        {
            nodeGrid[i]->RemoveParticle();
        }
    }
}

void SandboxGrid::SimulateSandbox(float deltaSeconds)
{
    currentDeltaSeconds += deltaSeconds;

    if (currentDeltaSeconds >= 0.0015f)
    {
        //cout << currentDeltaSeconds << endl;

        for (size_t i = 0; i < nodeGrid.size(); i++)
        {
            nodeGrid[i]->UpdateNode();
        }
        currentDeltaSeconds = 0.0f;
    }
}

void SandboxGrid::MoveParticleToGridNode(EParticleType type, int originalIndex, int newIndex, bool movedFromEarlierNode, bool switchNodes)
{
    EParticleType originalType = nodeGrid[originalIndex]->GetParticleType();
    EParticleType newType = nodeGrid[newIndex]->GetParticleType();

    if (!switchNodes)
    {
        nodeGrid[originalIndex]->RemoveParticle();
        if (nodeGrid[newIndex]->IsOccupied())
            nodeGrid[newIndex]->RemoveParticle();
        nodeGrid[newIndex]->AddParticle(type, movedFromEarlierNode);
    }
    else
    {
        nodeGrid[originalIndex]->RemoveParticle();
        nodeGrid[newIndex]->RemoveParticle();

        nodeGrid[originalIndex]->AddParticle(newType, movedFromEarlierNode);
        nodeGrid[newIndex]->AddParticle(originalType, movedFromEarlierNode);
    }
}

std::shared_ptr<SandboxGridNode> SandboxGrid::GetNode(int index)
{
    if (index < 0 || index >= nodeGrid.size() || nodeGrid[index] == NULL || nodeGrid[index]->GetXIndex() == 10 || nodeGrid[index]->GetXIndex() == sandboxWidth)
    {
        //cout << "Node is null" << " : " << index << endl;
        return nullptr;
    }
    return nodeGrid[index];
}

int SandboxGrid::UpperIndex(int index)
{
    return nodeGrid[index]->GetIndex() - ((sandboxWidth / nodeSize));// - 1);
}

int SandboxGrid::UpperLeftIndex(int index)
{
    return nodeGrid[index]->GetIndex() - ((sandboxWidth / nodeSize) - 1);
}

int SandboxGrid::UpperRightIndex(int index)
{
    return nodeGrid[index]->GetIndex() - ((sandboxWidth / nodeSize) + 1);//- 2);
}

int SandboxGrid::LowerIndex(int index)
{
    return nodeGrid[index]->GetIndex() + ((sandboxWidth / nodeSize)); //- 1);
}

int SandboxGrid::LowerLeftIndex(int index)
{
    return nodeGrid[index]->GetIndex() + ((sandboxWidth / nodeSize) - 1);
}

int SandboxGrid::LowerRightIndex(int index)
{
    return nodeGrid[index]->GetIndex() + ((sandboxWidth / nodeSize) + 1);//- 2);
}

int SandboxGrid::LeftIndex(int index)
{
    return nodeGrid[index]->GetIndex() - 1;
}

int SandboxGrid::RightIndex(int index)
{
    return nodeGrid[index]->GetIndex() + 1;
}

void SandboxGrid::DrawSandbox(sf::RenderWindow& window, sf::Vector2f mousePos)
{
	for (size_t i = 0; i < nodeGrid.size(); i++)
	{
        nodeGrid[i]->DrawParticle(window);
	}
}
