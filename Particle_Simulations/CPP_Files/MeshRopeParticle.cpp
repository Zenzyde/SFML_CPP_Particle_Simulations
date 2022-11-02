#include "MeshRopeParticle.h"
#include <iostream>

MeshRopeParticle::MeshRopeParticle(bool applyGravity, float const gravity, float const maxRopeLength, float const particleRadius, sf::Color color, sf::Vector2f pos, const int i, const int j, sf::RenderWindow* window) :
	ApplyGravity(applyGravity), Gravity(gravity), MaxRopeLength(maxRopeLength), Color(color), Position(pos), X(pos.x), Y(pos.y), I(i), J(j), Particle(std::make_shared<sf::CircleShape>(particleRadius)),
	ParticleRadius(particleRadius), RenderWindow(window)
{
	Particle->setOrigin(particleRadius, particleRadius);
	Particle->setPosition(X, Y);
	Particle->setFillColor(Color);

	UpNeighbour = NULL;
	DownNeighbour = NULL;
	LeftNeighbour = NULL;
	RightNeighbour = NULL;

	Acceleration = sf::Vector2f(0, Gravity);
}

MeshRopeParticle::~MeshRopeParticle()
{
}

void MeshRopeParticle::Fall()
{
	if (!ApplyGravity)
		return;

	sf::Vector2f Current(Particle->getPosition());

	sf::Vector2f NewPosition(Current + Velocity * 0.01f + Acceleration * (0.01f * 0.01f * 0.5f));
	sf::Vector2f NewVelocity(Velocity + Acceleration * (0.01f * 0.5f));

	Particle->setPosition(NewPosition);//Current.x, Current.y + Gravity);
	Velocity = NewVelocity;
}

void MeshRopeParticle::Constrict()
{
	if (!ApplyGravity)
		return;

	sf::Vector2f resultVector;

	int neighbourCount = 0;

	sf::Vector2f directionVector;

	float length;

	if (ApplyGravity)
	{
		// Calculate forces

		if (LeftNeighbour != NULL)
		{
			directionVector = (LeftNeighbour->GetParticle().getPosition() - Particle->getPosition());
			length = Distance(directionVector);
			directionVector = normalize(directionVector);

			resultVector += directionVector * 0.01f;
			neighbourCount++;
		}

		if (RightNeighbour != NULL)
		{
			directionVector = (RightNeighbour->GetParticle().getPosition() - Particle->getPosition());
			length = Distance(directionVector);
			directionVector = normalize(directionVector);

			resultVector += directionVector * 0.01f;
			neighbourCount++;
		}

		if (UpNeighbour != NULL)
		{
			directionVector = (UpNeighbour->GetParticle().getPosition() - Particle->getPosition());
			length = Distance(directionVector);
			directionVector = normalize(directionVector);

			resultVector += directionVector * 0.01f;
			neighbourCount++;
		}

		if (DownNeighbour != NULL)
		{
			directionVector = (DownNeighbour->GetParticle().getPosition() - Particle->getPosition());
			length = Distance(directionVector);
			directionVector = normalize(directionVector);

			resultVector += directionVector * 0.001f;
			neighbourCount++;
		}

		if (neighbourCount == 0)
			return;

		// Apply forces

		resultVector = sf::Vector2f(resultVector.x / neighbourCount, resultVector.y / neighbourCount);

		Particle->setPosition(Particle->getPosition() + resultVector);
	}

	// Constrain position

	sf::Vector2f constrainVector;

	int constrainCount = 0;

	if (UpNeighbour != NULL)
	{
		sf::Vector2f upVector(Particle->getPosition() - UpNeighbour->GetParticle().getPosition());

		float UpLength = Distance(upVector);

		upVector = normalize(upVector);

		if (UpLength > MaxRopeLength)
		{
			//Particle->setPosition(UpNeighbour->GetParticle().getPosition() + upVector * MaxRopeLength);
			constrainVector += UpNeighbour->GetParticle().getPosition() + upVector * MaxRopeLength;
			constrainCount++;
		}
	}

	if (DownNeighbour != NULL)
	{
		sf::Vector2f upVector(Particle->getPosition() - DownNeighbour->GetParticle().getPosition());

		float UpLength = Distance(upVector);

		upVector = normalize(upVector);

		if (UpLength > MaxRopeLength)
		{
			//Particle->setPosition(UpNeighbour->GetParticle().getPosition() + upVector * MaxRopeLength);
			constrainVector += DownNeighbour->GetParticle().getPosition() + upVector * MaxRopeLength;
			constrainCount++;
		}
	}

	if (LeftNeighbour != NULL)
	{
		sf::Vector2f leftVector(Particle->getPosition() - LeftNeighbour->GetParticle().getPosition());

		float LeftLength = Distance(leftVector);

		leftVector = normalize(leftVector);

		if (LeftLength > MaxRopeLength)
		{
			//Particle->setPosition(UpNeighbour->GetParticle().getPosition() + leftVector * MaxRopeLength);
			constrainVector += LeftNeighbour->GetParticle().getPosition() + leftVector * MaxRopeLength;
			constrainCount++;
		}
	}

	if (RightNeighbour != NULL)
	{
		sf::Vector2f rightVector(Particle->getPosition() - RightNeighbour->GetParticle().getPosition());

		float RightLength = Distance(rightVector);

		rightVector = normalize(rightVector);

		if (RightLength > MaxRopeLength)
		{
			//Particle->setPosition(UpNeighbour->GetParticle().getPosition() + rightVector * MaxRopeLength);
			constrainVector += RightNeighbour->GetParticle().getPosition() + rightVector * MaxRopeLength;
			constrainCount++;
		}
	}

	if (constrainCount == 0)
		return;

	constrainVector = sf::Vector2f(constrainVector.x / constrainCount, constrainVector.y / constrainCount);

	Particle->setPosition(constrainVector);
}

void MeshRopeParticle::AssignUpNeighbour(MeshRopeParticle* neighbour)
{
	UpNeighbour = std::make_shared<MeshRopeParticle>(*neighbour);
}

void MeshRopeParticle::AssignDownNeighbour(MeshRopeParticle* neighbour)
{
	DownNeighbour = std::make_shared<MeshRopeParticle>(*neighbour);;
}

void MeshRopeParticle::AssignLeftNeighbour(MeshRopeParticle* neighbour)
{
	LeftNeighbour = std::make_shared<MeshRopeParticle>(*neighbour);;
}

void MeshRopeParticle::AssignRightNeighbour(MeshRopeParticle* neighbour)
{
	RightNeighbour = std::make_shared<MeshRopeParticle>(*neighbour);;
}

void MeshRopeParticle::DrawRopes(sf::RenderWindow &window)
{
	if (UpNeighbour != NULL)
	{
		UpLines[0].position = UpNeighbour->GetParticle().getPosition();
		UpLines[0].color = UpNeighbour->GetParticle().getFillColor();
		UpLines[1].position = Particle->getPosition();
		UpLines[1].color = Color;

		window.draw(UpLines, 2, sf::Lines);
	}

	if (DownNeighbour != NULL)
	{
		DownLines[0].position = DownNeighbour->GetParticle().getPosition();
		DownLines[0].color = DownNeighbour->GetParticle().getFillColor();
		DownLines[1].position = Particle->getPosition();
		DownLines[1].color = Color;

		window.draw(DownLines, 2, sf::Lines);
	}

	if (LeftNeighbour != NULL)
	{
		LeftLines[0].position = LeftNeighbour->GetParticle().getPosition();
		LeftLines[0].color = LeftNeighbour->GetParticle().getFillColor();
		LeftLines[1].position = Particle->getPosition();
		LeftLines[1].color = Color;

		window.draw(LeftLines, 2, sf::Lines);
	}

	if (RightNeighbour != NULL)
	{
		RightLines[0].position = RightNeighbour->GetParticle().getPosition();
		RightLines[0].color = RightNeighbour->GetParticle().getFillColor();
		RightLines[1].position = Particle->getPosition();
		RightLines[1].color = Color;

		window.draw(RightLines, 2, sf::Lines);
	}
}

void MeshRopeParticle::RemoveRope(MeshRopeParticle* neighbour)
{
	if (UpNeighbour != NULL && UpNeighbour->I == neighbour->I && UpNeighbour->J == neighbour->J)
	{
		UpNeighbour = NULL;
	}

	if (DownNeighbour != NULL && DownNeighbour->I == neighbour->I && DownNeighbour->J == neighbour->J)
	{
		DownNeighbour = NULL;
	}

	if (LeftNeighbour != NULL && LeftNeighbour->I == neighbour->I && LeftNeighbour->J == neighbour->J)
	{
		LeftNeighbour = NULL;
	}

	if (RightNeighbour != NULL && RightNeighbour->I == neighbour->I && RightNeighbour->J == neighbour->J)
	{
		RightNeighbour = NULL;
	}
}

bool MeshRopeParticle::IsGravityApplied()
{
	return ApplyGravity;
}

bool MeshRopeParticle::IsMouseOverlappingParticle(sf::Vector2f mousePos)
{
	float distance(Distance(Particle->getPosition() - mousePos));

	return distance <= ParticleRadius;
}

void MeshRopeParticle::AvoidParticle(sf::Vector2f ParticlePos)
{
	float distance(Distance(Particle->getPosition() - ParticlePos));

	if (distance <= ParticleRadius + ParticleRadius)
	{
		sf::Vector2f direction(normalize(Particle->getPosition() - ParticlePos));
		Particle->setPosition(ParticlePos + direction * (ParticleRadius + ParticleRadius));
	}
}

void MeshRopeParticle::SetMouseAttachedState(bool state)
{
	bIsMouseAttached = state;
}

void MeshRopeParticle::MoveToMouse(sf::Vector2f MousePos)
{
	if (bIsMouseAttached)
	{
		sf::Vector2f ToMouse(MousePos - Particle->getPosition());
		float ToMouseDistance(Distance(ToMouse));
		ToMouse = normalize(ToMouse);
		Particle->setPosition(Particle->getPosition() + ToMouse * ToMouseDistance);
	}
}

bool MeshRopeParticle::IsMouseAttached()
{
	return bIsMouseAttached;
}

void MeshRopeParticle::DrawParticle()
{
	RenderWindow->draw(*Particle);
}

sf::CircleShape &MeshRopeParticle::GetParticle()
{
	return *Particle;
}

float MeshRopeParticle::Distance(sf::Vector2f inputvector)
{
	return sqrt((inputvector.x * inputvector.x) + (inputvector.y * inputvector.y));
}

sf::Vector2f MeshRopeParticle::normalize(sf::Vector2f pos)
{
	float length(sqrt((pos.x * pos.x) + (pos.y * pos.y)));

	if (length != 0)
		return sf::Vector2f(pos.x / length, pos.y / length);

	return pos;
}
