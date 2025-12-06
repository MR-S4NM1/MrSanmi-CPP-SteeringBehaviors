README – Steering Behaviors and Flocking AI System

Author: Miguel Angel Garcia Elizalde
Date: December 2025
Language: C++ 17
Library: SFML 3.0.2
Project Type: Game AI / Steering Behaviors / Flocking Simulation

Overview

This project implements a complete Steering Behaviors and Flocking AI system from scratch using C++ and SFML 3.0.2. It showcases classic autonomous agent movement techniques including Seek, Flee, Arrive, Wander, and basic multi-agent flocking logic.

The system was designed as a clean, modular, and extensible framework suitable for teaching, experimentation, and integration into larger gameplay systems. The goal was to demonstrate strong understanding of vector math, autonomous movement algorithms, update loops, collision helpers, and agent modeling.

All classes were implemented manually to highlight core AI and gameplay fundamentals without relying on external engines.

---

Core Features

Steering Behaviors

– Seek: Direct pursuit of a target at maximum speed

– Arrive: Smooth deceleration as the agent approaches a target

– RunAway: Flee behavior away from a designated threat

– Wander: Randomized exploratory behavior with directional variation

– Truncated steering force to ensure stable acceleration limits

– Normalized direction vectors for consistent movement


Autonomous Agent System

– Agent class with position, velocity, collider, and steering controller

– Maximum speed and maximum force constraints

– Independent logic per agent with clean update/draw cycle

– Reusable for pathfinding, NPCs, and gameplay prototypes

Collision and Collider Helper

– Simple circular collider component

– Position offset handling and collider updates

– Suitable for obstacle detection or future physics integration

Level and Actor Management

– LevelManager that stores, updates, and renders all GameActors

– Lookup for neighbors within a radius to support flocking behavior

– Simple midpoint calculation for multi-agent coordination

– GameActor base class for consistent rendering and identity

Flocking Support

– Storage of agents in a specialized list for group logic

– Query for neighbors around a given agent

– Basis for adding Cohesion, Alignment, and Separation rules

Rendering and Integration

– Visual representation of all actors using SFML 3.0.2

– Window rendering loop with update/draw cycle

– Fully self-contained C++ project ready to compile

Design Patterns and Principles Used

– Composition: Agent composed with collider and steering system

– Inheritance: GameActor base class with specialized Obstacle

– Data-Oriented Thinking: Lightweight structs for vectors and physics values

– Separation of Concerns: Rendering, logic, and physics fully separated

– Encapsulation: Private/internal fields hidden and controlled via methods

---

Technical Highlights

– Custom vector math operations through SteeringMath

– Normalization, truncation, and length calculations

– Clean, deterministic update loop for all actors

– Boundary-safe and force-limited steering output

– Efficient neighbor lookup using simple radius filtering

– Minimal dependencies for straightforward portability

---

Agent Behavior Overview

Each agent in the scene supports:

– Independent movement

– Steering accumulation

– Controlled acceleration and velocity

– Collision radius via collider

– Visual representation through colored shapes

---

Example Usage

- Create several agents and obstacles

- Add them to the LevelManager

- Set a target position (e.g., mouse position)

- Apply a behavior (Seek, Arrive, RunAway, Wander)

- Update the LevelManager each frame

- Render all actors using SFML

- The main loop manages timing, window events, and delegates all logic to the LevelManager and agents.

---

How to Run
- Install SFML 3.0.2

- Compile the project with a C++17+ compatible compiler

- Link against SFML system, graphics, and window modules

- Run the application

- Move the mouse or adjust agent targets to observe behaviors

---

Key Scenarios Demonstrated
- Direct pursuit using Seek behavior

- Smooth target approach using Arrive

- Emergency escape behavior using RunAway

- Randomized exploratory motion using Wander

- Multi-agent flocking neighbor detection

- Stable force-limited movement that prevents jitter

- Real-time interaction with user-controlled input

---

Extension Points
- Add full flocking (Cohesion, Alignment, Separation)

- Implement obstacle avoidance using the existing Collider system

- Integrate polygonal obstacles instead of circular ones

- Add debug visualization (force vectors, neighbor radius)

---

Final Notes

This project demonstrates a complete AI steering and movement framework built manually using C++ and SFML. It provides a solid foundation for more complex autonomous behaviors and serves as a portfolio piece highlighting vector math proficiency, AI fundamentals, clean architecture, and gameplay programming skills.

The system is lightweight, maintainable, and ready for extension—ideal for experimentation, student learning, and integration into larger gameplay prototypes or AI-driven simulations.
