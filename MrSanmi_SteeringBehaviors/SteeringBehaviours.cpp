#include "SteeringBehaviours.h"

// Static mouse position initialization.
sf::Vector2i SteeringMath::mousePosition = sf::Vector2i(0.f, 0.f);

/**
 * @brief Moves the agent toward a target at maximum speed.
 *
 * Seek computes a desired velocity in the direction of the target, then
 * subtracts the current velocity to obtain the steering force. The force
 * is clamped to the agent's maximum allowed force.
 *
 * @param position Current agent position.
 * @param velocity Current agent velocity.
 * @param target   Position the agent wants to reach.
 * @return Steering force to apply.
 */
sf::Vector2f SteeringBehaviors::Seek(sf::Vector2f position,
    sf::Vector2f velocity,
    sf::Vector2f target)
{
    sf::Vector2f direction = target - position;
    direction = SteeringMath::normalize(direction) * m_maxSpeed;

    sf::Vector2f steering = direction - velocity;
    return SteeringMath::truncate(steering, m_maxForce);
}

/**
 * @brief Moves the agent toward a target but slows down smoothly on arrival.
 *
 * Arrive behaves like Seek but adjusts the desired speed based on distance
 * to the target. Once the agent is within a certain radius, it decelerates
 * proportionally to avoid overshooting.
 *
 * @param position       Current agent position.
 * @param velocity       Current agent velocity.
 * @param target         Position the agent wants to reach.
 * @param slowingRadius  Distance within which the agent starts slowing down.
 * @return Steering force to apply.
 */
sf::Vector2f SteeringBehaviors::Arrive(sf::Vector2f position,
    sf::Vector2f velocity,
    sf::Vector2f target,
    float slowingRadius)
{
    sf::Vector2f toTarget = target - position;
    float distance = SteeringMath::length(toTarget);

    if (distance < 0.01f)
        return sf::Vector2f(0.0f, 0.0f);

    float speed = m_maxSpeed;

    // Smooth deceleration when inside the slowing radius.
    if (distance < slowingRadius)
        speed = m_maxSpeed * (distance / slowingRadius);

    sf::Vector2f desired = SteeringMath::normalize(toTarget) * speed;

    sf::Vector2f steering = desired - velocity;
    return SteeringMath::truncate(steering, m_maxForce);
}

/**
 * @brief Causes the agent to flee from a target at full speed.
 *
 * RunAway is essentially Seek in the opposite direction. The agent moves
 * directly away from the target using maximum speed, useful for predator/
 * prey behaviors or avoidance logic.
 *
 * @param position Current agent position.
 * @param velocity Current agent velocity.
 * @param target   Position the agent wants to escape from.
 * @return Steering force to apply.
 */
sf::Vector2f SteeringBehaviors::RunAway(sf::Vector2f position,
    sf::Vector2f velocity,
    sf::Vector2f target)
{
    sf::Vector2f direction = position - target;
    direction = SteeringMath::normalize(direction) * m_maxSpeed;

    sf::Vector2f steering = direction - velocity;
    return SteeringMath::truncate(steering, m_maxForce);
}

/**
 * @brief Basic wandering behavior using a target point that moves over time.
 *
 * This implementation acts similarly to Arrive: the agent moves toward a
 * given target and slows down inside a radius. The logic assumes the external
 * system updates the target randomly to create wandering motion.
 *
 * @param position       Current agent position.
 * @param velocity       Current agent velocity.
 * @param target         Wander target position, usually jittered each frame.
 * @param slowingRadius  Radius for smooth deceleration.
 * @return Steering force to apply.
 */
sf::Vector2f SteeringBehaviors::Wander(sf::Vector2f position,
    sf::Vector2f velocity,
    sf::Vector2f target,
    float slowingRadius)
{
    sf::Vector2f toTarget = target - position;
    float distance = SteeringMath::length(toTarget);

    if (distance < 0.01f)
        return sf::Vector2f(0.0f, 0.0f);

    float speed = m_maxSpeed;

    if (distance < slowingRadius)
        speed = m_maxSpeed * (distance / slowingRadius);

    sf::Vector2f desired = SteeringMath::normalize(toTarget) * speed;

    sf::Vector2f steering = desired - velocity;
    return SteeringMath::truncate(steering, m_maxForce);
}
