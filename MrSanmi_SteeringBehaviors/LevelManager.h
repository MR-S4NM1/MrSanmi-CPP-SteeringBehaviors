#pragma once
#include "Physics.h"
#include "Agent.h"
#include <list>

/**
 * @Author: Miguel Angel Garcia Elizalde and Marco Antonio Garcia
 * @Date: November, 2025
 * @file: LevelManager.h
 * @brief Manages all actors and agents inside a level.
 * LevelManager handles updates, rendering, and neighbor detection for agents.
 * It works as a lightweight ECS-like container tailored for steering behavior
 * demos and flocking experiments.
 */
class LevelManager
{
public:

    LevelManager() {}

    /**
     * @brief Adds a new GameActor (Agent or Obstacle) to the level.
     * @param actor Pointer to the actor to be stored and updated.
     */
    void AddActorToLevel(GameActor* actor);

    /**
     * @brief Updates all actors in the level.
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime);

    /**
     * @brief Renders all actors into the provided window.
     * @param window SFML render window reference.
     */
    void DrawGameActors(sf::RenderWindow& window);

    /**
     * @brief Returns all agents within a given radius of the specified agent.
     *
     * Used primarily for flocking behaviors such as separation, alignment,
     * and cohesion.
     *
     * @param agent The agent requesting neighbor detection.
     * @param radius Detection radius.
     * @return A vector containing nearby agents.
     */
    std::vector<Agent*> GetNeighbors(Agent* agent, float radius);

private:

    std::vector<GameActor*> gameActors; ///< All actors present in the level.
    std::vector<Agent*> agentInFlock;   ///< Agents participating in flocking behavior.

    Physics physics = Physics();      ///< Physics helper for interactions.

    sf::Vector2f midPoint{ 0.f, 0.f };

    /**
     * @brief Calculates the midpoint of all agents (for flock cohesion).
     */
    sf::Vector2f CalculateMidPoint();
};
