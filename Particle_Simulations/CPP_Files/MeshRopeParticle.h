#include "SFML/Graphics.hpp"

#pragma once
class MeshRopeParticle
{
public:
	MeshRopeParticle(bool applyGravity, float const gravity, float const maxRopeLength, float const particleRadius, sf::Color color, sf::Vector2f pos, const int i, const int j, sf::RenderWindow* window);
	~MeshRopeParticle();

	void Fall();
	void Constrict();
	void AssignUpNeighbour(MeshRopeParticle* neighbour);
	void AssignDownNeighbour(MeshRopeParticle* neighbour);
	void AssignLeftNeighbour(MeshRopeParticle* neighbour);
	void AssignRightNeighbour(MeshRopeParticle* neighbour);
	void DrawRopes(sf::RenderWindow &window);
	void RemoveRope(MeshRopeParticle* neighbour);
	bool IsGravityApplied();
	bool IsMouseOverlappingParticle(sf::Vector2f mousePos);
	void AvoidParticle(sf::Vector2f ParticlePos);
	void SetMouseAttachedState(bool state);
	void MoveToMouse(sf::Vector2f MousePos);
	bool IsMouseAttached();

	void DrawParticle();

	sf::CircleShape &GetParticle();

private:
	bool ApplyGravity;
	float const Gravity;
	float const MaxRopeLength;
	float X;
	float Y;
	float const ParticleRadius;
	sf::Vector2f Position;
	sf::Vector2f Velocity;
	sf::Vector2f Acceleration;
	sf::Color Color;
	std::shared_ptr<sf::CircleShape> Particle;
	sf::Vertex UpLines[2];
	std::shared_ptr<MeshRopeParticle> UpNeighbour;
	sf::Vertex DownLines[2];
	std::shared_ptr<MeshRopeParticle> DownNeighbour;
	sf::Vertex LeftLines[2];
	std::shared_ptr<MeshRopeParticle> LeftNeighbour;
	sf::Vertex RightLines[2];
	std::shared_ptr<MeshRopeParticle> RightNeighbour;
	bool bIsMouseAttached;

	int I;
	int J;

	sf::RenderWindow* RenderWindow;

	float Distance(sf::Vector2f inputvector);
	sf::Vector2f normalize(sf::Vector2f pos);
};