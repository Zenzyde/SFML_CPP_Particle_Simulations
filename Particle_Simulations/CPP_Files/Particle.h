#include "SFML/Graphics.hpp"

#pragma once
class Particle
{
public:
    Particle(float radius, sf::Color color, float X, float speed, float bottom, sf::RenderWindow* window);
    ~Particle();

    void RandomizeParticle();

    void Fall();
    bool ParticlePassedBottom();
    void DrawParticle();

private:
    std::shared_ptr<sf::CircleShape> particle;
    float fallingSpeed;
    float _radius;
    sf::Color _color;
    float _bottom;
    float _X;

    sf::RenderWindow* RenderWindow;
};

