# SFML_CPP_Particle_Simulations
This is a project made in pure C++ to refresh &amp; deepen my C++ knowledge outside Unreal Engine.

It features 3 "particle simulation" modes of incremental implementation difficulty.

The Quicksort algorithm.
![Quicksort visual](/images/Particles_Sorting.gif)
This mode also features (a deliberately thread-locking) timelapse sequence as the sorting happens! :D

A mesh simulation.
![Mesh](/images/Particles_Mesh.gif)
This is a fun little experimental implementation of an interactable mesh. You can interact with the mesh by dragging individual vertices around​, or by clicking two different vertices that are connected with a rope, which will cause the rope to be cut and the vertices to be separated. The mesh is also reactive in the sense that vertices will try to keep from overlapping with other vertices, and try to keep a certain distance to a neighboring vertex connected with a rope.

A particle sandbox.
![Sandbox](/images/Particles_Sandbox.gif)
The sandbox has a grid of cells that can be empty or have a particle in them. There's a smaller range of particles such as air, water, acid, wood, stone, and sand available to play around with, and you can add and remove any one of these types of particles as you wish, and they are capable of interacting with each other.