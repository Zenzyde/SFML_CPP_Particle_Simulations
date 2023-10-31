# SFML_CPP_Particle_Simulations
This is a project made in pure C++ using the SFML graphics library to refresh &amp; deepen my C++ knowledge outside Unreal Engine.

It features 3 "particle simulation" modes of incremental implementation difficulty.

The Quicksort algorithm.
![Quicksort visual](/images/Particles_Sorting.gif)
This is basically your usual Quicksort implementation, with some added remapping of values during the creation of sorting elements to make sure that all elements, regardless of the amount of them and their ranges in values, should fit within the screen.

An A* Pathfinding grid.
![Mesh](/images/Particles_Pathfinding.gif)
This is pretty much your usual A* pathfinding implementation, with a priority queue. I've added my own touch by introducing some different path types with different weights which can be placed manually in order to manipulate the potential path the algorithm could potentially take.

A particle sandbox.
![Sandbox](/images/Particles_Sandbox.gif)
The sandbox has a grid of cells that can be empty or have a particle in them. There's a smaller range of particles such as air, water, acid, wood, stone, and sand available to play around with, and you can add and remove any one of these types of particles as you wish, and they are capable of interacting with each other.
