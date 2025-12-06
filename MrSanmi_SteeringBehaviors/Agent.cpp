#include "Agent.h"
#include "LevelManager.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

extern LevelManager manager;

/**
 * @brief Updates the agent using its finite-state machine (FSM).
 */
void Agent::Update(float deltaTime)
{
	CalculateFSM(deltaTime);
}

/**
 * @brief Changes the internal FSM state of the agent.
 * If the state is already active, nothing happens. On state change, the
 * target resets to the current position to avoid unintended movement spikes.
 */
void Agent::setState(AgentStates state)
{
	if (state == m_actualState) return;

	m_target = transform.position;
	m_actualState = state;
}

/**
 * @brief Finite-state machine that dispatches to individual behaviors.
 */
void Agent::CalculateFSM(float deltaTime)
{
	switch (m_actualState)
	{
	case None:
		setState(Wander);
		break;
	case Wander:
		WanderState(deltaTime);
		break;
	case Seek:
		SeekState(deltaTime);
		break;
	case RunAway:
		RunAwayState(deltaTime);
		break;
	case Flock:
		FlockState(deltaTime);
		break;
	}
}


/**
 * @brief SEEK behavior: agent moves toward the mouse position.
 * The agent uses Arrive() to smoothly decelerate as it approaches the target,
 * avoiding overshooting and sudden stops.
 */
void Agent::SeekState(float deltaTime)
{
	m_target = (sf::Vector2f)SteeringMath::mousePosition;
	m_dir = transform.position - m_target;

	// Compute steering force
	steering = behaviour.Arrive(transform.position, velocity, m_target);
	acceleration = steering / mass;
	velocity = acceleration * deltaTime;

	transform.position += velocity;

	// Update graphical representation
	sf::Vector2f fixPosition = transform.position - offset;

	if (usingSprite)
	{
		spriteRenderer.sprite->setPosition(fixPosition);

		rotationAngle = SteeringMath::calculateAngleUsingGrad(
			transform.position.x, transform.position.y,
			transform.position.x + m_dir.x,
			transform.position.y + m_dir.y
		);

		spriteRenderer.sprite->setRotation(sf::degrees(rotationAngle));
	}
	else
	{
		renderer.shape.setPosition(fixPosition);

		rotationAngle = SteeringMath::calculateAngleUsingGrad(
			transform.position.x, transform.position.y,
			transform.position.x + m_dir.x,
			transform.position.y + m_dir.y
		);

		renderer.shape.setRotation(sf::degrees(rotationAngle));
	}

	collider.Update(fixPosition.x, fixPosition.y);

	// When near the target, return to Wander state
	if (SteeringMath::length(steering) < 1.f) {
		setState(Wander);
	}
}

/**
 * @brief WANDER behavior: agent moves randomly with smooth direction changes.
 * This implementation uses polar coordinates to generate random points
 * within a circle, producing fluid, unpredictable motion.
 */
void Agent::WanderState(float deltaTime)
{
	// Generate new wander target
	if (generateNewWanderPosition) {
		wanderAngle = SteeringMath::randomFloat(0, 2 * 3.14159f);
		radius = wanderRadius * std::sqrt(SteeringMath::randomFloat(0, 1));

		m_target = transform.position + sf::Vector2f(
			std::cos(wanderAngle) * radius,
			std::sin(wanderAngle) * radius
		);

		generateNewWanderPosition = false;
	}

	m_dir = transform.position - m_target;

	steering = behaviour.Wander(transform.position, velocity, m_target);
	acceleration = steering / mass;
	velocity = acceleration * deltaTime;

	transform.position += velocity;

	// Update visuals
	sf::Vector2f fixPosition = transform.position - offset;

	if (usingSprite)
	{
		spriteRenderer.sprite->setPosition(fixPosition);

		rotationAngle = SteeringMath::calculateAngleUsingGrad(
			transform.position.x, transform.position.y,
			transform.position.x + m_dir.x,
			transform.position.y + m_dir.y
		);

		spriteRenderer.sprite->setRotation(sf::degrees(rotationAngle));
	}
	else
	{
		renderer.shape.setPosition(fixPosition);

		rotationAngle = SteeringMath::calculateAngleUsingGrad(
			transform.position.x, transform.position.y,
			transform.position.x + m_dir.x,
			transform.position.y + m_dir.y
		);

		renderer.shape.setRotation(sf::degrees(rotationAngle));
	}

	collider.Update(fixPosition.x, fixPosition.y);

	// Request a new target when reaching the current one
	if (SteeringMath::length(steering) < 1.f) {
		generateNewWanderPosition = true;
	}
}

/**
 * @brief RUNAWAY behavior: agent flees from the mouse position.
 *
 * The agent accelerates in the opposite direction of the target until
 * it reaches safe distance. No deceleration is applied.
 */
void Agent::RunAwayState(float deltaTime)
{
	m_target = (sf::Vector2f)SteeringMath::mousePosition;
	m_dir = m_target - transform.position;

	steering = behaviour.RunAway(transform.position, velocity, m_target);
	acceleration = steering / mass;
	velocity = acceleration * deltaTime;

	transform.position += velocity;

	sf::Vector2f fixPosition = transform.position - offset;

	if (usingSprite)
	{
		spriteRenderer.sprite->setPosition(fixPosition);

		rotationAngle = SteeringMath::calculateAngleUsingGrad(
			transform.position.x, transform.position.y,
			transform.position.x + m_dir.x,
			transform.position.y + m_dir.y
		);

		spriteRenderer.sprite->setRotation(sf::degrees(rotationAngle));
	}
	else
	{
		renderer.shape.setPosition(fixPosition);

		rotationAngle = SteeringMath::calculateAngleUsingGrad(
			transform.position.x, transform.position.y,
			transform.position.x + m_dir.x,
			transform.position.y + m_dir.y
		);

		renderer.shape.setRotation(sf::degrees(rotationAngle));
	}

	collider.Update(fixPosition.x, fixPosition.y);
}

/**
 * @brief FLOCK behavior: implements Separation, Alignment, and Cohesion.
 *
 * Each component is weighted equally by default, producing swarm-like motion.
 * Neighbor detection is handled by LevelManager.
 */
void Agent::FlockState(float deltaTime)
{
	float neighboorRadius{ 350 };
	float desiredSeparation{ 30 };

	std::vector<Agent*> neighbors = manager.GetNeighbors(this, neighboorRadius);

	sf::Vector2f separation{ 0.f, 0.f };
	sf::Vector2f alignment{ 0.f, 0.f };
	sf::Vector2f cohesion{ 0.f, 0.f };

	int countSeparation{ 0 };
	int countAligment{ 0 };
	int countCohesion{ 0 };

	// --- Compute Separation, Alignment, Cohesion ---
	for (auto* other : neighbors)
	{
		sf::Vector2f toOther = transform.position - other->transform.position;
		float distance = SteeringMath::length(toOther);

		// Separation: avoid crowding
		if (distance < desiredSeparation)
		{
			sf::Vector2f diff = SteeringMath::normalize(toOther) / distance;
			separation += diff;
			countSeparation++;
		}

		// Alignment & Cohesion use the same neighbor-radius check
		if (distance < neighboorRadius) {
			alignment += other->GetVelocity();
			cohesion += other->transform.position;
			countAligment++;
			countCohesion++;
		}
	}

	// Normalize / average forces
	if (countSeparation > 0)
		separation /= (float)countSeparation;

	if (countAligment > 0)
	{
		alignment /= (float)countAligment;

		if (SteeringMath::length(alignment) > 0.001f)
		{
			alignment = SteeringMath::normalize(alignment) * behaviour.maxSpeed();
			alignment -= velocity;
		}
	}

	if (countCohesion > 0)
	{
		sf::Vector2f center = cohesion / (float)countCohesion;
		sf::Vector2f toCenter = center - transform.position;

		if (SteeringMath::length(toCenter) > 0.1f)
		{
			sf::Vector2f desired = SteeringMath::normalize(toCenter) * behaviour.maxSpeed();
			cohesion = desired - velocity;
		}
	}

	// --- Combine final steering force ---
	float separationWeight{ 1.f };
	float alignmentWeight{ 1.f };
	float cohesionWeight{ 1.f };

	steering = separation * separationWeight +
		alignment * alignmentWeight +
		cohesion * cohesionWeight;

	steering = SteeringMath::truncate(steering, behaviour.maxForce());

	// --- Physics integration ---
	acceleration = steering / mass;
	velocity += acceleration * deltaTime;
	velocity = SteeringMath::truncate(velocity, behaviour.maxSpeed());
	transform.position += velocity * deltaTime;

	// --- Visual update ---
	sf::Vector2f fixPosition = transform.position - offset;
	if (SteeringMath::length(velocity) > 0.01f)
	{
		if (usingSprite)
		{
			spriteRenderer.sprite->setPosition(fixPosition);
		}
		else
		{
			renderer.shape.setPosition(fixPosition);
		}
	}

	renderer.shape.setPosition(transform.position);
	collider.Update(fixPosition.x, fixPosition.y);
}
