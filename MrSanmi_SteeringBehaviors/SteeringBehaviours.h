#pragma once
#include <SFML/Graphics.hpp>
#include <random>

/**
 * @Author: Miguel Angel Garcia Elizalde and Marco Antonio Garcia
 * @Date: November, 2025
 * @file: SteeringMath.h
 * @brief: Utility math functions supporting steering behavior calculations.
 * @details: Provides vector operations, random utilities, and angle calculations
 *           required for steering algorithms such as Seek, Wander, Arrive, etc.
 *           These helpers are lightweight and inline for optimal performance.
 */
struct SteeringMath {

    /**
     * @brief Computes the magnitude of a 2D vector.
     * @param v Input vector.
     * @return Length of the vector.
     */
    static inline float length(sf::Vector2f v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    /**
     * @brief Normalizes a vector to unit length.
     * @param v Input vector.
     * @return Normalized vector (unit vector).
     * @warning Does not check for zero-length vectors.
     */
    static inline sf::Vector2f normalize(sf::Vector2f v)
    {
        float _length = length(v);
        return sf::Vector2f(v.x / _length, v.y / _length);
    }

    /**
     * @brief Limits a vector's magnitude to a maximum value.
     * @param v Input vector.
     * @param maxMagnitude Maximum allowed magnitude.
     * @return Truncated vector.
     */
    static inline sf::Vector2f truncate(sf::Vector2f v, float maxMagnitude)
    {
        float _length = length(v);

        if (_length > maxMagnitude)
        {
            float scale = maxMagnitude / _length;
            return sf::Vector2f(v.x * scale, v.y * scale);
        }

        return v;
    }

    /**
     * @brief Checks whether a position lies within a certain radius from another.
     * @param destiny Target point.
     * @param actualPosition Current point.
     * @param range Distance threshold.
     * @return True if the target is within range, false otherwise.
     */
    static inline bool isWithinTheRange(sf::Vector2f destiny, sf::Vector2f actualPosition, float range) {
        return length(destiny - actualPosition) < range;
    }

    /**
     * @brief Generates a random float between two values.
     * @param min Minimum value.
     * @param max Maximum value.
     * @return Random float within range.
     */
    static inline float randomFloat(float min, float max) {
        static std::random_device random;
        static std::mt19937 gen(random());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }

    /**
     * @brief Calculates the angle (in radians) between two points.
     * @return Angle in radians.
     */
    static inline float calculateAngleRad(float x1, float y1, float x2, float y2) {
        float dx = x2 - x1;
        float dy = y2 - y1;
        return std::atan2(dy, dx);
    }

    /**
     * @brief Calculates the angle (in degrees) between two points.
     * @return Angle in degrees.
     */
    static inline float calculateAngleUsingGrad(float x1, float y1, float x2, float y2) {
        const float PI = 3.14159265f;
        float dx = x2 - x1;
        float dy = y2 - y1;
        float radians = std::atan2(dy, dx);
        float degrees = radians * (180.0f / PI);

        return degrees;
    }

    /// Stores the last known mouse position (global utility used externally)
    static sf::Vector2i mousePosition;
};

/**
 * @Author: Miguel Angel Garcia Elizalde and Marco Antonio Garcia
 * @Date: November, 2025
 * @file: SteeringBehaviors.h
 * @brief: Collection of steering algorithms used for autonomous agent movement.
 * @details: Implements classical steering behaviors described by Craig Reynolds,
 *           including Seek, Arrive, Wander, and RunAway. These behaviors produce
 *           directional acceleration vectors that can be applied to agents
 *           for smooth and responsive movement in 2D space.
 */
class SteeringBehaviors
{
public:

    /**
     * @brief Constructs a steering controller with customizable limits.
     * @param maxSpeed Maximum movement speed for steering output.
     * @param maxForce Maximum force applied for steering acceleration.
     */
    SteeringBehaviors(float maxSpeed = 300.f, float maxForce = 800.f)
        : m_maxSpeed(maxSpeed), m_maxForce(maxForce) {
    }

    /**
     * @brief Classic Seek behavior: moves directly toward a target.
     * @param position Current agent position.
     * @param velocity Current agent velocity.
     * @param target Desired destination.
     * @return Steering force vector pointing toward the target.
     */
    sf::Vector2f Seek(sf::Vector2f position,
        sf::Vector2f velocity,
        sf::Vector2f target);

    /**
     * @brief Arrive behavior: approaches a target and slows down near it.
     * @param position Current agent position.
     * @param velocity Current agent velocity.
     * @param target Desired destination.
     * @param slowingRadius Distance within which the agent begins to decelerate.
     * @return Steering vector that eases the approach smoothly.
     */
    sf::Vector2f Arrive(sf::Vector2f position,
        sf::Vector2f velocity,
        sf::Vector2f target,
        float slowingRadius = 100.f);

    /**
     * @brief RunAway behavior: accelerates directly away from a target.
     * @param position Current agent position.
     * @param velocity Current velocity.
     * @param target Threat or object to flee from.
     * @return Force vector pointing in the opposite direction.
     */
    sf::Vector2f RunAway(sf::Vector2f position,
        sf::Vector2f velocity,
        sf::Vector2f target);

    /**
     * @brief Wander behavior: produces natural, random drifting movement.
     * @param position Current agent position.
     * @param velocity Current velocity.
     * @param target Internal wander target.
     * @param slowingRadius Optional deceleration radius.
     * @return Steering force for wandering.
     * @details The target parameter is generally generated by the agent
     *          each frame to create unpredictable but smooth motion.
     */
    sf::Vector2f Wander(sf::Vector2f position,
        sf::Vector2f velocity,
        sf::Vector2f target,
        float slowingRadius = 100.f);

    /// @return Maximum movement speed.
    float maxSpeed() { return m_maxSpeed; }

    /// @return Maximum steering force.
    float maxForce() { return m_maxForce; }

    /// @brief Sets max speed.
    void setMaxSpeed(float v) { m_maxSpeed = v; }

    /// @brief Sets max force.
    void setMaxForce(float v) { m_maxForce = v; }

    /// @brief Gets internal target reference used by Wander, etc.
    sf::Vector2f getTarget() { return target; }

private:

    /// Target used internally by some behaviors (Wander, Arrive).
    sf::Vector2f target;

    /// Maximum allowed speed for any movement.
    float m_maxSpeed;

    /// Maximum allowed steering force.
    float m_maxForce;
};
