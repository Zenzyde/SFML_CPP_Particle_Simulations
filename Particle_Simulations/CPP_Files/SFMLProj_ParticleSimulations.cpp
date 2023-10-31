// This tutorial made shit work: https://www.youtube.com/watch?v=_VgRjlvGpPk

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>
#include "Button.h"
#include "SandboxGrid.h"
#include <SFML/System/Clock.hpp>
#include "SortingNode.h"
#include "PathfindGrid.h"
#include "PathGridNode.h"

using namespace std;

enum class EProgramMode
{
    QuickSort,
    Sandbox,
    Pathfinding
};

EProgramMode activeEProgramMode;

EParticleType selectedParticleType = EParticleType::stone;

vector<SortingNode> SortingArray;
int numSortingElements = 2500;
int minElementValue = 70;
int maxElementValue = 7000;

ENodeType selectedNodeType = ENodeType::Blocked;

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

// Reference: https://stackoverflow.com/questions/3451553/value-remapping
float Remap(float value, float low1, float high1, float low2, float high2)
{
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

void SetupSortingArray(int NumElements, float Min, float Max, sf::RenderWindow* renderWindow)
{
    if (NumElements <= 1) return;
    SortingArray.clear();

    float elementWidth = ((renderWindow->getSize().x * 1.0f) / (NumElements * 1.0f));

    float MaxHeight = Max;
    float MinHeight = Min;

    if (Max > renderWindow->getSize().y)
    {
        // Max is greater than window size, rescale max and min to fit within window
        MaxHeight = Remap(Max, Min, Max, 0, renderWindow->getSize().y);
        MinHeight = Remap(Min, Min, Max, 0, renderWindow->getSize().y);
    }

    for (size_t i = 1; i < NumElements + 1; i++)
    {
        float r = static_cast<float>(rand());
        double nextElement = static_cast<double>(fmod(r, (MaxHeight - MinHeight))) + MinHeight;
        float X = i * elementWidth;
        SortingNode Node(nextElement, X, renderWindow->getSize().y, elementWidth, *renderWindow);

        SortingArray.push_back(Node);
    }
}

void SimulateSandboxGrid(SandboxGrid& sandboxGrid, sf::RenderWindow& window, float MouseRadius, bool bAddParticle, bool bRemoveParticle, float deltaSeconds)
{
    sf::Vector2i mousePos(sf::Mouse::getPosition(window));

    sf::Vector2f actualMouse(mousePos.x, mousePos.y);

    if (bAddParticle)
        sandboxGrid.AddParticleToGrid(actualMouse, selectedParticleType);
    else if (bRemoveParticle)
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
        for (size_t i = 0; i < SortingArray.size(); i++)
        {
            SortingArray[i].DrawNode();
        }
        window.display();
        
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

PathfindGrid InitializePathGrid(sf::Vector2f gridSize, sf::Vector2f nodeSize, sf::Vector2f gridOffset)
{
    PathfindGrid grid(gridSize, nodeSize, gridOffset);
    return grid;
}

int main()
{
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Particle Simulations");

    // Initialize random
    srand((unsigned)time(0));

    SandboxGrid sandboxGrid(SetupSandboxGrid(window));

    PathfindGrid pathGrid(InitializePathGrid(sf::Vector2f(1010, 690), sf::Vector2f(15, 15), sf::Vector2f(250, 10)));

    Button quicksortButton(sf::Vector2f(10, 10), sf::Vector2f(200, 35), sf::Color::Blue, sf::Vector2f(15, 10), sf::Vector2f(1, 1), 24, sf::Color::White, "Quick Sorting", window);
    Button SortingTextButton(sf::Vector2f(40, window.getSize().y - 45), sf::Vector2f(80, 30), sf::Color::Blue, sf::Vector2f(45, window.getSize().y - 45), sf::Vector2f(1, 1), 24, sf::Color::White, "Sort", window);
    Button RandomizeTextButton(sf::Vector2f(200, window.getSize().y - 45), sf::Vector2f(150, 30), sf::Color::Blue, sf::Vector2f(205, window.getSize().y - 45), sf::Vector2f(1, 1), 24, sf::Color::White, "Randomize", window);

    Button pathfindingButton(sf::Vector2f(10, 50), sf::Vector2f(170, 35), sf::Color::Blue, sf::Vector2f(15, 50), sf::Vector2f(1, 1), 24, sf::Color::White, "Pathfinding", window);
    Button pathTypeTextButton(sf::Vector2f(10, window.getSize().y - 45), sf::Vector2f(120, 30), sf::Color::Blue, sf::Vector2f(15, window.getSize().y - 45), sf::Vector2f(1, 1), 24, sf::Color::White, "Blocked", window);
    Button startPathfindButton(sf::Vector2f(10, window.getSize().y - 85), sf::Vector2f(220, 30), sf::Color::Blue, sf::Vector2f(15, window.getSize().y - 85), sf::Vector2f(1, 1), 24, sf::Color::White, "Start Pathfinding", window);
    Button clearPathButton(sf::Vector2f(10, window.getSize().y - 125), sf::Vector2f(160, 30), sf::Color::Blue, sf::Vector2f(15, window.getSize().y - 125), sf::Vector2f(1, 1), 24, sf::Color::White, "Clear Path", window);

    Button sandboxButton(sf::Vector2f(10, 90), sf::Vector2f(220, 35), sf::Color::Blue, sf::Vector2f(15, 90), sf::Vector2f(1, 1), 24, sf::Color::White, "Particle Sandbox", window);
    Button particleTypeTextButton(sf::Vector2f(40, window.getSize().y - 45), sf::Vector2f(80, 30), sf::Color::Blue, sf::Vector2f(45, window.getSize().y - 45), sf::Vector2f(1, 1), 24, sf::Color::White, "Stone", window);

    while (window.isOpen())
    {
        bool bAddParticle = false;
        bool bRemoveParticle = false;
        bool bAddNodeType = false;
        bool bSetNodeEmpty = false;
 
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
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                float delta = event.mouseWheelScroll.delta;
                int nextNodeType = (int)selectedNodeType + delta;
                int nextParticleType = (int)selectedParticleType + delta;
                switch (activeEProgramMode)
                {
                case EProgramMode::QuickSort:
                    break;
                case EProgramMode::Pathfinding:
                    if (delta > 0)
                    {
                        nextNodeType %= (int)ENodeType::LAST_ENUM_VALUE;
                        if ((ENodeType)nextNodeType == ENodeType::Empty)
                            nextNodeType++;
                        if ((ENodeType)nextNodeType == ENodeType::Occupied)
                            nextNodeType++;
                    }
                    else if (delta < 0)
                    {
                        if ((ENodeType)nextNodeType == ENodeType::Empty || (ENodeType)nextNodeType == ENodeType::Occupied)
                        {
                            nextNodeType = (int)ENodeType::LAST_ENUM_VALUE;
                            nextNodeType--;
                        }
                    }
                    selectedNodeType = (ENodeType)nextNodeType;
                    switch (selectedNodeType)
                    {
                    case ENodeType::Empty:
                        pathTypeTextButton.SetText("Empty");
                        break;
                    case ENodeType::Blocked:
                        pathTypeTextButton.SetText("Blocked");
                        break;
                    case ENodeType::Start:
                        pathTypeTextButton.SetText("Start");
                        break;
                    case ENodeType::Goal:
                        pathTypeTextButton.SetText("Goal");
                        break;
                    case ENodeType::Hazard:
                        pathTypeTextButton.SetText("Hazard");
                        break;
                    case ENodeType::Safe:
                        pathTypeTextButton.SetText("Safe");
                        break;
                    default:
                        break;
                    }
                    break;
                case EProgramMode::Sandbox:
                    if (delta > 0)
                    {
                        nextParticleType %= (int)EParticleType::LAST_ENUM_VALUE;
                    }
                    else if (delta < 0 && nextParticleType < 0)
                    {
                        nextParticleType = (int)EParticleType::LAST_ENUM_VALUE;
                        nextParticleType--;
                    }
                    selectedParticleType = (EParticleType)nextParticleType;
                    switch (selectedParticleType)
                    {
                    case EParticleType::wood:
                        particleTypeTextButton.SetText("Sand");
                        break;
                    case EParticleType::stone:
                        particleTypeTextButton.SetText("Wood");
                        break;
                    case EParticleType::water:
                        particleTypeTextButton.SetText("Stone");
                        break;
                    case EParticleType::acid:
                        particleTypeTextButton.SetText("Water");
                        break;
                    case EParticleType::air:
                        particleTypeTextButton.SetText("Acid");
                        break;
                    case EParticleType::sand:
                        particleTypeTextButton.SetText("Air");
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (quicksortButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                    activeEProgramMode = EProgramMode::QuickSort;
                
                if (pathfindingButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                    activeEProgramMode = EProgramMode::Pathfinding;
                
                if (sandboxButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                    activeEProgramMode = EProgramMode::Sandbox;
                
                switch (activeEProgramMode)
                {
                case EProgramMode::QuickSort:
                    if (SortingTextButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                    {
                        Quicksort(0, SortingArray.size() - 1, window);
                    }

                    if (RandomizeTextButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                    {
                        SetupSortingArray(numSortingElements, minElementValue, maxElementValue, &window);
                    }
                    break;
                case EProgramMode::Pathfinding:
                    if (startPathfindButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                    {
                        pathGrid.FindPath();
                    }
                    else if (clearPathButton.WasClicked(sf::Vector2f(sf::Mouse::getPosition(window))))
                    {
                        pathGrid.ResetPath();
                    }
                    else
                    {
                        bAddNodeType = true;
                    }
                    break;
                case EProgramMode::Sandbox:
                    bAddParticle = true;
                    break;
                default:
                    break;
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                switch (activeEProgramMode)
                {
                case EProgramMode::QuickSort:
                    break;
                case EProgramMode::Pathfinding:
                    bSetNodeEmpty = true;
                    break;
                case EProgramMode::Sandbox:
                    break;
                default:
                    break;
                }
            }
        }

        window.clear();

        sf::Time elapsedTime = clock.restart();
        float deltaTime = elapsedTime.asMilliseconds();

        switch (activeEProgramMode)
        {
        case EProgramMode::QuickSort:
            if (SortingArray.size() == 0)
                SetupSortingArray(numSortingElements, minElementValue, maxElementValue, &window);
            
            SimulateSorting();

            SortingTextButton.DrawButton();

            RandomizeTextButton.DrawButton();
            break;
        case EProgramMode::Pathfinding:
            if (SortingArray.size() > 0)
                SortingArray.clear();
            
            if (bAddNodeType)
            {
                sf::Vector2i mousePos(sf::Mouse::getPosition(window));
                sf::Vector2f actualMouse(mousePos.x, mousePos.y);
                pathGrid.SetNodeType(actualMouse, selectedNodeType);
            }
            if (bSetNodeEmpty)
            {
                sf::Vector2i mousePos(sf::Mouse::getPosition(window));
                sf::Vector2f actualMouse(mousePos.x, mousePos.y);
                pathGrid.SetNodeType(actualMouse, ENodeType::Empty);
            }
            pathGrid.Draw(window);

            pathTypeTextButton.DrawButton();
            startPathfindButton.DrawButton();
            clearPathButton.DrawButton();
            break;
        case EProgramMode::Sandbox:
            if (SortingArray.size() > 0)
                SortingArray.clear();
            
            SimulateSandboxGrid(sandboxGrid, window, 5, bAddParticle, bRemoveParticle, deltaTime);

            particleTypeTextButton.DrawButton();
            break;
        }

        quicksortButton.DrawButton();

        pathfindingButton.DrawButton();

        sandboxButton.DrawButton();

        window.display();
    }

    return 0;
}