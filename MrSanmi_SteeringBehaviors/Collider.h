#pragma once

/**
 * @brief Simple 2D vector used for collider spatial data.
 */
struct Vec2
{
public:
    float x, y;
};

/**
 * @Author: Marco Antonio Garcia
 * @Date: November, 2025
 * @file: Collider.h
 * @brief Circular collider representation used for basic physics interactions.
 */
class Collider
{
public:
    float radius; ///< Radius of the collider.

    Vec2 position; ///< World position of the collider center.

    Collider() {}

    /**
     * @brief Creates a circular collider.
     * @param radius Collision radius.
     * @param x Initial x coordinate.
     * @param y Initial y coordinate.
     */
    Collider(float radius, float x, float y);

    /**
     * @brief Updates collider position.
     * @param x New x coordinate.
     * @param y New y coordinate.
     */
    void Update(float x, float y);
};
