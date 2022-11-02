#include "Particle.h"

Particle::Particle(float radius, sf::Color color, float X, float speed, float bottom, sf::RenderWindow* window) : fallingSpeed(speed), _radius(radius), _color(color), _bottom(bottom), _X(X), particle(new sf::CircleShape(radius)), RenderWindow(window)
{
    particle->setFillColor(_color);
    particle->setOrigin(_radius, _radius);
    particle->setPosition(X, 0);
}

Particle::~Particle()
{
}

void Particle::RandomizeParticle()
{
    sf::Color color(rand() % 255, rand() % 255, rand() % 255);

    particle->setFillColor(color);

    float radius(rand() % 15);

    float X(rand() % (int)(1280 - radius));

    particle->setPosition(X, 0);
}

void Particle::Fall()
{
    sf::Vector2f current(particle->getPosition());

    particle->setPosition(current.x, current.y + fallingSpeed);
}

bool Particle::ParticlePassedBottom()
{
    return particle->getPosition().y - particle->getRadius() > _bottom + _radius;
}

void Particle::DrawParticle()
{
    RenderWindow->draw(*particle);
}

// https://en.sfml-dev.org/forums/index.php?topic=1488.0
//sf::Vector2f Particle::normalize(sf::Vector2f vector)
//{
//    float length(sqrt((vector.x * vector.x) + (vector.y * vector.y)));
//
//    if (length != 0)
//        return sf::Vector2f(vector.x / length, vector.y / length);
//
//    return vector;
//}
//
//float Particle::DistanceToVector(sf::Vector2f vector)
//{
//    return sqrt((vector.x * vector.x) + (vector.y * vector.y));
//}