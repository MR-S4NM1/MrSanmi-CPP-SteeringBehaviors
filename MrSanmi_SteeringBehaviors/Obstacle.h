#pragma once
#include "GameActor.h"

/**
 * @Author: Miguel Angel Garcia Elizalde and Marco Antonio Garcia
 * @Date: November, 2025
 * @file: Obstacle.h
 * @brief Represents a static circular obstacle in the game world.
 * Obstacles inherit from GameActor and are typically used for collision
 * avoidance tests, flocking environments, and steering behavior demos.
 * They do not move or update, they only serve as blocking geometry.
 */
class Obstacle : public GameActor
{
public:
    Obstacle() {}

    /**
     * @brief Creates a new obstacle in the scene.
     * @param radius Radius of the obstacle’s collision circle.
     * @param position World position of the obstacle.
     * @param color Visual color for rendering (default: Red).
     */
    Obstacle(float radius, sf::Vector2f position, sf::Color color = sf::Color::Red)
        : GameActor(position, radius, color) {
    }

    ~Obstacle() override
    {
        std::cout << "Destroying obstacle" << std::endl;
    }
};
