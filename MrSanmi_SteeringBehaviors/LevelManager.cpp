#include "LevelManager.h"

/**
 * @brief Inserts a new actor into the level.
 * LevelManager does not take ownership; memory management is handled externally.
 */
void LevelManager::AddActorToLevel(GameActor* actor)
{
	gameActors.push_back(actor);
}

/**
 * @brief Updates all actors and checks for collisions between the first actor
 *        and the rest of the list.
 * This simple collision loop is meant for demos and debugging.
 */
void LevelManager::Update(float deltaTime)
{
	GameActor firstElementValue = *gameActors.front();
	auto it = gameActors.begin();

	if (gameActors.size() > 1)
	{
		std::advance(it, 1);
	}
	else
	{
		it = gameActors.end();
	}

	while (it != gameActors.end())
	{
		if (physics.OnCollisionEnter(firstElementValue.collider, (*it)->collider))
		{
			std::cout << "Hit" << std::endl;
		}
		++it;
	}

	for (auto& actor : gameActors)
		actor->Update(deltaTime);
}

/**
 * @brief Draws all game actors into the given SFML window.
 */
void LevelManager::DrawGameActors(sf::RenderWindow& window)
{
	for (auto& actor : gameActors)
	{
		window.draw(actor->Shape());

		if (actor->Sprite())
			window.draw(*actor->Sprite());
	}
}

/**
 * @brief Returns all agents within a radius of the given agent.
 * Used for flocking and proximity checks.
 */
std::vector<Agent*> LevelManager::GetNeighbors(Agent* agent, float radius)
{
	std::vector<Agent*> neighbors;

	for (auto* gameActor : gameActors)
	{
		Agent* a = dynamic_cast<Agent*>(gameActor);
		if (!a || a == agent) continue;

		float distance = SteeringMath::length(agent->transform.position - a->transform.position);
		if (distance < radius)
			neighbors.push_back(a);
	}

	return neighbors;
}

/**
 * @brief Computes the average position of all agents in the flock list.
 * Used for cohesion calculations and visualization.
 */
sf::Vector2f LevelManager::CalculateMidPoint()
{
	float x_mid = 0.f, y_mid = 0.f;

	Agent firstElementValue = *agentInFlock.front();
	auto it = agentInFlock.begin();

	if (agentInFlock.size() > 1)
		std::advance(it, 1);
	else
		it = agentInFlock.end();

	while (it != agentInFlock.end())
	{
		x_mid += (*it)->transform.position.x;
		y_mid += (*it)->transform.position.y;
		++it;
	}

	int agentCount = agentInFlock.size() - 1;
	midPoint = sf::Vector2f(x_mid / agentCount, y_mid / agentCount);

	return midPoint;
}