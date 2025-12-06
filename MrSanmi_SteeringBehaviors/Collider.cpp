#include "Collider.h"

/**
 * @brief Constructs a circular collider and initializes its position.
 * 
 * The collider stores its center position internally. The incoming (x, y)
 * parameters represent the top-left corner of the object, so the constructor
 * offsets them by the radius to calculate the center.
 * 
 * @param radius Radius of the circular collider.
 * @param x      X coordinate of the object's top-left position.
 * @param y      Y coordinate of the object's top-left position.
 */
Collider::Collider(float radius, float x, float y)
{
    this->radius = radius;

    // Convert top-left coordinates to collider center.
    position.x = x + radius;
    position.y = y + radius;
}

/**
 * @brief Updates the collider's center position.
 *
 * This function should be called every frame to sync the collider with
 * whatever object owns it. The provided coordinates are assumed to be
 * the new center of the collider.
 *
 * @param x New X coordinate of the collider's center.
 * @param y New Y coordinate of the collider's center.
 */
void Collider::Update(float x, float y)
{
    position.x = x;
    position.y = y;
}
