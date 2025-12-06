#pragma once
#include "Collider.h"
#include <cmath>

/**
 * @Author: Miguel Angel Garcia Elizalde and Marco Antonio Garcia
 * @Date: November, 2025
 * @file: Physics.h
 * @brief Simple physics helper class for collision detection.
 */
class Physics
{
public:
    Physics() {}

    /**
     * @brief Checks whether two circular colliders are overlapping.
     * @param a First collider.
     * @param b Second collider.
     * @return True if their radii intersect, false otherwise.
     */
    bool OnCollisionEnter(Collider a, Collider b)
    {
        float ca = b.position.x - a.position.x;
        float co = b.position.y - a.position.y;

        float magnitude = std::sqrt((ca * ca) + (co * co));

        return magnitude <= (b.radius + a.radius);
    }
};
