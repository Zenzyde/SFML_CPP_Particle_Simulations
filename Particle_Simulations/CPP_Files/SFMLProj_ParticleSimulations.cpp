// This tutorial made shit work: https://www.youtube.com/watch?v=_VgRjlvGpPk

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>
#include "Particle.h"
#include "MeshRopeParticle.h"
#include "Button.h"
#include "SandboxGrid.h"
#include <SFML/System/Clock.hpp>
#include "SortingNode.h"
#include <chrono>
#include <thread>

using namespace std;

vector<Particle> particles;

vector<MeshRopeParticle> meshParticles;

int playMode = 1;

ParticleType particleType = ParticleType::stone;

MeshRopeParticle* particleA;
MeshRopeParticle* particleB;
MeshRopeParticle* attachedMeshParticle;

vector<SortingNode> SortingArray;

void SetupParticleRain(sf::RenderWindow& window)
{
    // Initialize and distribute particles
    for (size_t i = 0; i < 10; i++)
    {
        sf::Color randColor(rand() % 255, rand() % 255, rand() % 255);

        float radius(rand() % 15);

        float X(rand() % (int)(1280 - radius));

        float fallSpeed(((rand() % 5) + 1) / 10.f);

        Particle particle(radius, randColor, X, fallSpeed, 720, &window);

        particles.push_back(particle);
    }
}

void SetupParticleMesh(sf::RenderWindow& window)
{
    int Columns = 25;
    int Rows = 5;
    int XOffset = 50;

    // Initialize and distribute mesh
    for (size_t i = 0; i < Rows; i++)
    {
        for (size_t j = 0; j < Columns; j++)
        {
            sf::Color randColor(rand() % 255, rand() % 255, rand() % 255);

            float radius(10);

            float startRopeLength(40);

            float X((j + 1) * XOffset);

            float Y((i + 1) * startRopeLength + 100);

            float fallSpeed(0.25f);

            float ropeLength(50);

            if (i == 0 && j == 0 || i == 0 && j == Columns - 1)
            {
                MeshRopeParticle particle(false, fallSpeed, ropeLength, radius, randColor, sf::Vector2f(X, Y), j, i, &window);

                meshParticles.push_back(particle);
            }
            else
            {
                if (i == 0)
                {
                    MeshRopeParticle particle(true, fallSpeed, ropeLength, radius, randColor, sf::Vector2f(X, Y), j, i, &window);

                    meshParticles.push_back(particle);
                }
                else
                {
                    MeshRopeParticle particle(true, fallSpeed, ropeLength, radius, randColor, sf::Vector2f(X, Y), j, i, &window);

                    MeshRopeParticle* upNeighbour = &meshParticles[((i * Columns) + j) - Columns]; // (row * columns) + column - columns

                    particle.AssignUpNeighbour(upNeighbour);

                    meshParticles.push_back(particle);
                }
            }
        }
    }

    for (size_t i = 0; i < Rows; i++)
    {
        for (size_t j = 0; j < Columns; j++)
        {
            MeshRopeParticle* current = &meshParticles[(i * Columns) + j];

            if (current->IsGravityApplied() && ((i * Columns) + j) + Columns < meshParticles.size()) // Only add down neighbours
            {
                MeshRopeParticle* downNeighbour = &meshParticles[((i * Columns) + j) + Columns];

                current->AssignDownNeighbour(downNeighbour);
            }

            if (j == 0) // Only add right neighbours
            {
                MeshRopeParticle* rightNeighbour = &meshParticles[((i * Columns) + j) + 1];

                current->AssignRightNeighbour(rightNeighbour);
            }
            else if (j > 0 && j < Columns - 1)// Add both left and right neighbours
            {
                MeshRopeParticle* leftNeighbour = &meshParticles[((i * Columns) + j) - 1];

                current->AssignLeftNeighbour(leftNeighbour);

                MeshRopeParticle* rightNeighbour = &meshParticles[((i * Columns) + j) + 1];

                current->AssignRightNeighbour(rightNeighbour);
            }
            else if (j == Columns - 1) // Only add left neighbours
            {
                MeshRopeParticle* leftNeighbour = &meshParticles[((i * Columns) + j) - 1];

                current->AssignLeftNeighbour(leftNeighbour);
            }
        }
    }
}

SandboxGrid SetupSandboxGrid(sf::RenderWindow& window)
{
    int gridNodeSize = 10;

    int StartX = 10;
    int StartY = 10;

    int Height = window.getSize().y - 10;
    int Width = window.getSize().x - 10;

    return SandboxGrid(gridNodeSize, Width, Height, StartX, StartY);
}

float lerp(float v0, float v1, float t) {
    return (1 - t) * v0 + t * v1;
}

void SetupSortingArray(int NumElements, float elementOffset, float elementWidth, int Min, int Max, sf::RenderWindow* renderWindow)
{
    if (NumElements <= 1) return;

    SortingArray.clear();
    for (size_t i = 1; i < NumElements + 1; i++)
    {
        double nextElement = static_cast<double>(rand() % (Max - Min)) + Min;
        float X = i * elementOffset;
        SortingNode Node(nextElement, X, renderWindow->getSize().y, elementWidth, *renderWindow);

        SortingArray.push_back(Node);
    }
}

void SimulateParticleRain()
{
    if (!particles.empty())
    {
        vector<int> ToDelete;

        for (vector<Particle>::size_type i = particles.size() - 1; i != (vector<Particle>::size_type) - 1; i--)
        {
            Particle* particle = &particles[i];

            particle->Fall();

            if (particle->ParticlePassedBottom())
            {
                particle->RandomizeParticle();
            }

            particle->DrawParticle();
        }
    }
}

void SimulateParticleMesh(sf::RenderWindow& window, int particleEffectMode)
{
    sf::Vector2i mousePos(sf::Mouse::getPosition(window));

    bool skipOtherParticles = false;
    bool mouseOverlappingOneParticle = false;

    if (!meshParticles.empty())
    {
        for (vector<MeshRopeParticle>::size_type i = meshParticles.size() - 1; i != (vector<MeshRopeParticle>::size_type) - 1; i--)
        {
            MeshRopeParticle* particle = &meshParticles[i];

            sf::Vector2f actualMouse(mousePos.x, mousePos.y);

            particle->Fall();

            for (vector<MeshRopeParticle>::size_type j = meshParticles.size() - 1; j != (vector<MeshRopeParticle>::size_type) - 1; j--)
            {
                if (i == j) continue;

                MeshRopeParticle* neighbour = &meshParticles[j];
                particle->AvoidParticle(neighbour->GetParticle().getPosition());
            }

            particle->DrawRopes(window);

            if (particleEffectMode == 1)
            {
                if (!skipOtherParticles && particleA == nullptr && particle->IsMouseOverlappingParticle(actualMouse))
                {
                    particleA = particle;
                    skipOtherParticles = true;
                }
                else if (!skipOtherParticles && particleA != nullptr && particleB == nullptr && particle->IsMouseOverlappingParticle(actualMouse))
                {
                    particleB = particle;
                    skipOtherParticles = true;
                    mouseOverlappingOneParticle = true;
                }
                else if (!skipOtherParticles && particleA != nullptr && particleB == nullptr && !particle->IsMouseOverlappingParticle(actualMouse) && !mouseOverlappingOneParticle && i == (vector<MeshRopeParticle>::size_type) - 1)
                {
                    particleA = nullptr;
                    skipOtherParticles = true;
                }

                if (particleA != nullptr && particleB != nullptr)
                {
                    particleA->RemoveRope(particleB);
                    particleB->RemoveRope(particleA);

                    particleA = nullptr;
                    particleB = nullptr;

                    skipOtherParticles = true;
                }
            }
            else if (particleEffectMode == -1)
            {
                if (skipOtherParticles)
                    continue;
                if (!particle->IsMouseAttached() && particle->IsMouseOverlappingParticle(actualMouse))
                {
                    particle->SetMouseAttachedState(true);
                    attachedMeshParticle = particle;
                    skipOtherParticles = true;
                }
                else if (particle->IsMouseAttached() && particle->IsMouseOverlappingParticle(actualMouse))
                {
                    particle->SetMouseAttachedState(false);
                    attachedMeshParticle = NULL;
                    skipOtherParticles = true;
                }
                else if (particle->IsMouseAttached() && !particle->IsMouseOverlappingParticle(actualMouse))
                {
                    particle->SetMouseAttachedState(false);
                    attachedMeshParticle = NULL;
                    skipOtherParticles = true;
                }
            }

            particle->MoveToMouse(actualMouse);

            particle->Constrict();

            window.draw(particle->GetParticle());
        }
    }
}

void SimulateSandboxGrid(SandboxGrid& sandboxGrid, sf::RenderWindow& window, float MouseRadius, int existanceplayMode, float deltaSeconds)
{
    sf::Vector2i mousePos(sf::Mouse::getPosition(window));

    sf::Vector2f actualMouse(mousePos.x, mousePos.y);

    if (existanceplayMode == 1)
        sandboxGrid.AddParticleToGrid(actualMouse, particleType);
    else if (existanceplayMode == -1)
        sandboxGrid.RemoveParticleFromGrid(actualMouse);

    sandboxGrid.SimulateSandbox(deltaSeconds);
    sandboxGrid.DrawSandbox(window, actualMouse);
}

int QuicksortPartition(int low, int high, sf::RenderWindow& window)
{
    int pivotValue = SortingArray[std::floor((high + low) / 2.0f)].GetValue();

    int leftIndex = low - 1;

    int rightIndex = high + 1;

    while (true)
    {
        do
        {
            leftIndex++;
        } while (SortingArray[leftIndex].GetValue() < pivotValue);

        do
        {
            rightIndex--;
        } while (SortingArray[rightIndex].GetValue() > pivotValue);

        if (leftIndex >= rightIndex)
        {
            return rightIndex;
        }

        // Position swap
        sf::Vector2f Temp = SortingArray[leftIndex].GetPosition();
        SortingArray[leftIndex].SetPosition(SortingArray[rightIndex].GetPosition().x, SortingArray[rightIndex].GetPosition().y);
        SortingArray[rightIndex].SetPosition(Temp.x, Temp.y);

        window.clear();
        SimulateParticleRain();
        for (size_t i = 0; i < SortingArray.size(); i++)
        {
            SortingArray[i].DrawNode();
        }
        window.display();

        std::this_thread::sleep_for(std::chrono::microseconds(3)); // Intentionally slowing down for timelapse-effect
        
        // Index swap
        SortingNode TempNode = SortingArray[leftIndex];
        SortingArray[leftIndex] = SortingArray[rightIndex];
        SortingArray[rightIndex] = TempNode;
    }
}

void Quicksort(int low, int high, sf::RenderWindow& window)
{
    if (low >= 0 && high >= 0 && low < high)
    {
        int pivot = QuicksortPartition(low, high, window);
        Quicksort(low, pivot, window);
        Quicksort(pivot + 1, high, window);
    }
}

void SimulateSorting()
{
    for (size_t i = 0; i < SortingArray.size(); i++)
    {
        SortingArray[i].DrawNode();
    }
}

int main()
{
    sf::Clock clock;

    float lastElapsedTime = 0.f;

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Particle Simulation");

    // Initialize random
    srand((unsigned)time(0));

    SetupParticleRain(window);

    SetupParticleMesh(window);

    SandboxGrid sandboxGrid(SetupSandboxGrid(window));

    SetupSortingArray(255, 5, 2.f, 50, 700, &window);

    Button fallingParticlesButton(sf::Vector2f(10, 10), sf::Vector2f(200, 35), sf::Color::Blue, sf::Vector2f(15, 10), sf::Vector2f(1, 1), 24, sf::Color::White, "Quick Sorting", window);

    Button particleMeshButton(sf::Vector2f(10, 50), sf::Vector2f(170, 35), sf::Color::Blue, sf::Vector2f(15, 50), sf::Vector2f(1, 1), 24, sf::Color::White, "Particle Mesh", window);

    Button sandboxButton(sf::Vector2f(10, 90), sf::Vector2f(220, 35), sf::Color::Blue, sf::Vector2f(15, 90), sf::Vector2f(1, 1), 24, sf::Color::White, "Particle Sandbox", window);

    Button switchBackParticleButton(sf::Vector2f(10, window.getSize().y - 45), sf::Vector2f(20, 30), sf::Color::Blue, sf::Vector2f(15, window.getSize().y - 45), sf::Vector2f(1, 1), 24, sf::Color::White, "-", window);
    Button particleTypeTextButton(sf::Vector2f(40, window.getSize().y - 45), sf::Vector2f(80, 30), sf::Color::Blue, sf::Vector2f(45, window.getSize().y - 45), sf::Vector2f(1, 1), 24, sf::Color::White, "Stone", window);
    Button switchForthParticleButton(sf::Vector2f(130, window.getSize().y - 45), sf::Vector2f(23, 30), sf::Color::Blue, sf::Vector2f(135, window.getSize().y - 45), sf::Vector2f(1, 1), 24, sf::Color::White, "+", window);

    Button SortingTextButton(sf::Vector2f(40, window.getSize().y - 45), sf::Vector2f(80, 30), sf::Color::Blue, sf::Vector2f(45, window.getSize().y - 45), sf::Vector2f(1, 1), 24, sf::Color::White, "Sort", window);
    Button RandomizeTextButton(sf::Vector2f(200, window.getSize().y - 45), sf::Vector2f(150, 30), sf::Color::Blue, sf::Vector2f(205, window.getSize().y - 45), sf::Vector2f(1, 1), 24, sf::Color::White, "Randomize", window);

    while (window.isOpen())
    {
        int particleExistanceplayMode = 0;
        int meshParticleEffect = 0;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (fallingParticlesButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                {
                    playMode = 0;
                }
                
                if (particleMeshButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                {
                    playMode = 1;
                }
                
                if (sandboxButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                {
                    playMode = 2;
                }
                
                if (switchBackParticleButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                {
                    switch (particleType)
                    {
                    case ParticleType::wood:
                        particleType = ParticleType::sand;
                        particleTypeTextButton.SetText("Sand");
                        break;
                    case ParticleType::stone:
                        particleType = ParticleType::wood;
                        particleTypeTextButton.SetText("Wood");
                        break;
                    case ParticleType::water:
                        particleType = ParticleType::stone;
                        particleTypeTextButton.SetText("Stone");
                        break;
                    case ParticleType::acid:
                        particleType = ParticleType::water;
                        particleTypeTextButton.SetText("Water");
                        break;
                    case ParticleType::air:
                        particleType = ParticleType::acid;
                        particleTypeTextButton.SetText("Acid");
                        break;
                    case ParticleType::sand:
                        particleType = ParticleType::air;
                        particleTypeTextButton.SetText("Air");
                        break;
                    }
                }
                
                if (switchForthParticleButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                {
                    switch (particleType)
                    {
                    case ParticleType::sand:
                        particleType = ParticleType::wood;
                        particleTypeTextButton.SetText("Wood");
                        break;
                    case ParticleType::wood:
                        particleType = ParticleType::stone;
                        particleTypeTextButton.SetText("Stone");
                        break;
                    case ParticleType::stone:
                        particleType = ParticleType::water;
                        particleTypeTextButton.SetText("Water");
                        break;
                    case ParticleType::water:
                        particleType = ParticleType::acid;
                        particleTypeTextButton.SetText("Acid");
                        break;
                    case ParticleType::acid:
                        particleType = ParticleType::air;
                        particleTypeTextButton.SetText("Air");
                        break;
                    case ParticleType::air:
                        particleType = ParticleType::sand;
                        particleTypeTextButton.SetText("Sand");
                        break;
                    }
                }

                if (SortingTextButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                {
                    Quicksort(0, SortingArray.size() - 1, window);
                }


                if (RandomizeTextButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                {
                    SetupSortingArray(255, 5, 2.f, 50, 700, &window);
                }
                
                if (playMode == 1)
                {
                    meshParticleEffect = 1;
                }
                
                if (playMode == 2)
                {
                    particleExistanceplayMode = 1;
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                if (playMode == 1)
                {
                    meshParticleEffect = -1;
                }

                if (playMode == 2)
                {
                    particleExistanceplayMode = -1;
                }
            }
        }

        window.clear();

        switch (playMode)
        {
        case 0:
            SimulateParticleRain();
            
            SimulateSorting();

            SortingTextButton.DrawButton();

            RandomizeTextButton.DrawButton();
            break;
        case 1:
            SimulateParticleMesh(window, meshParticleEffect);
            break;
        case 2:
            SimulateSandboxGrid(sandboxGrid, window, 5, particleExistanceplayMode, clock.getElapsedTime().asSeconds() - lastElapsedTime);
            
            switchBackParticleButton.DrawButton();

            switchForthParticleButton.DrawButton();

            particleTypeTextButton.DrawButton();
            break;
        }

        fallingParticlesButton.DrawButton();

        particleMeshButton.DrawButton();

        sandboxButton.DrawButton();

        window.display();

        lastElapsedTime = clock.getElapsedTime().asSeconds();
    }

    return 0;
}