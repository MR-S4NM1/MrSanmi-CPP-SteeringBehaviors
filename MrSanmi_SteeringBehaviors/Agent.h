#pragma once
#include "GameActor.h"
#include "SteeringBehaviours.h"

/**
 * @brief Finite-state-machine states for the Agent.
 */
enum AgentStates
{
    None,
    Wander,
    Seek,
    RunAway,
    Flock
};

/**
 * @Author: Miguel Angel Garcia Elizalde and Marco Antonio Garcia
 * @Date: November, 2025
 * @file: Agent.h
 * @brief AI-driven game actor implementing multiple steering behaviors.
 * @details Each Agent maintains velocity, acceleration, direction,
 *          a steering behavior module, and a finite-state machine (FSM)
 *          controlling its actions.
 */
class Agent : public GameActor
{
public:
    /**
     * @brief Default constructor.
     */
    Agent() {}

    /**
     * @brief Constructor for shape-rendered agents.
     * @param radius Visual and collider radius.
     * @param position Initial position in world space.
     * @param color Optional debug color.
     */
    Agent(float radius, sf::Vector2f position, sf::Color color = sf::Color::White)
        : GameActor(position, radius, color), behaviour(300.f, 800.f)
    {
        usingSprite = false;
        offset = sf::Vector2f(radius, radius);
        renderer.shape.setOrigin(offset);
    }

    /**
     * @brief Constructor for sprite-rendered agents.
     * @param position Initial position.
     * @param scale Sprite size and collider radius.
     * @param texture Texture applied to the sprite.
     */
    Agent(sf::Vector2f position, float scale, sf::Texture texture)
        : GameActor(position, scale, texture), behaviour(300.f, 800.f)
    {
        usingSprite = true;
        offset = sf::Vector2f(radius, radius);
        renderer.shape.setOrigin(offset);
    }

    /**
     * @brief Main update loop applying steering logic.
     * @param deltaTime Frame time.
     */
    void Update(float deltaTime) override;

    /// Target management
    void SetTarget(sf::Vector2f target) { m_target = target; }
    sf::Vector2f GetTarget() { return m_target; }

    /// Motion accessors
    sf::Vector2f GetDirection() { return m_dir; }
    sf::Vector2f GetVelocity() { return velocity; }
    void SetVelocity(sf::Vector2f& v) { velocity = v; }

    /// State machine management
    AgentStates getAgentState() { return m_actualState; }
    void setState(AgentStates state);

    void setWanderRadius(float radius) { wanderRadius = radius; }

    /**
     * @brief Destructor. Logs when an agent is destroyed.
     */
    ~Agent() override {
        std::cout << "Destroying actor!" << std::endl;
    }

    float distanceToMidPoint{ 0.f }; ///< Debug helper for flocking.

private:
    sf::Vector2f offset{ 0.f, 0.f }; ///< Rendering origin offset.
    SteeringBehaviors behaviour;     ///< Core steering behaviours module.

    sf::Vector2f m_target{ 500.f, 500.f }; ///< Current seek target.

    sf::Vector2f velocity{ 0.f, 0.f }; ///< Current velocity.
    sf::Vector2f acceleration{ 0.f, 0.f }; ///< Current acceleration.
    sf::Vector2f m_dir{ 0.f, 0.f }; ///< Direction normalized vector.

    sf::Vector2f steering{ 0.f, 0.f }; ///< Steering force accumulator.

    float mass{ 1.f };         ///< Agent mass affecting acceleration.
    float wanderRadius{ 250.f }; ///< Radius of wander steering behavior.
    float wanderAngle{ 0.f }; ///< Internal wander rotation value.
    float radius{ 0.f };       ///< Collider radius.

    bool generateNewWanderPosition{ true }; ///< Wander recalculation trigger.

#pragma region FSM
    AgentStates m_actualState{ Wander };
    void CalculateFSM(float deltaTime);
    void SeekState(float deltaTime);
    void WanderState(float deltaTime);
    void RunAwayState(float deltaTime);
    void FlockState(float deltaTime);
#pragma endregion
};
