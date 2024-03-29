#include "SandboxParticle.h"
#include <iostream>

using namespace std;

SandboxParticle::SandboxParticle()
{
}

SandboxParticle::SandboxParticle(EParticleType _type, sf::Vector2f pos, sf::Vector2f size, SandboxGridNode& _nodeParent) :
	particle(std::make_unique<sf::RectangleShape>(sf::RectangleShape(sf::Vector2f(size)))), nodeParent(std::make_unique<SandboxGridNode>(_nodeParent)), gravityDirection(0), bUseGravity(false)
{
	type = _type;
	switch (_type)
	{
	case EParticleType::sand:
		particle->setFillColor(sf::Color::Yellow);
		break;
	case EParticleType::wood:
		particle->setFillColor(sf::Color::Color(139, 69, 19));
		break;
	case EParticleType::stone:
		particle->setFillColor(sf::Color::Color(158, 159, 153));
		break;
	case EParticleType::water:
		particle->setFillColor(sf::Color::Cyan);
		break;
	case EParticleType::acid:
		particle->setFillColor(sf::Color::Green);
		break;
	case EParticleType::air:
		particle->setFillColor(sf::Color::Color(181, 219, 239));
		break;
	}
	particle->setPosition(pos);

	srand((unsigned)time(0));
}

SandboxParticle::~SandboxParticle()
{
	particle.reset();
}

void SandboxParticle::DrawParticle(sf::RenderWindow& window)
{
	if (particle != NULL)
		window.draw(*particle);

	/*DrawUpperParticle(window);
	DrawLowerParticle(window);*/
}

void SandboxParticle::Update()
{
}

void SandboxParticle::DrawUpperParticle(sf::RenderWindow& window)
{
	int nextNode = nodeParent->GetUpperNode();

	std::shared_ptr<SandboxGridNode> node = nodeParent->GetGrid().GetNode(nextNode);
	
	if (node != NULL && !node->IsOccupied())
		node->DrawFilledBox(window);
}

void SandboxParticle::DrawLowerParticle(sf::RenderWindow& window)
{
	int nextNode = nodeParent->GetLowerNode();

	std::shared_ptr<SandboxGridNode> node = nodeParent->GetGrid().GetNode(nextNode);
	
	if (node != NULL && !node->IsOccupied())
		node->DrawFilledBox(window);
}

EParticleType SandboxParticle::GetParticleType()
{
	return type;
}
