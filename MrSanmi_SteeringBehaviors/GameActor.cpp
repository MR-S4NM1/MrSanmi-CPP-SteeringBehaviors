#include "GameActor.h"

/**
 * @brief Base update method for all game actors.
 * GameActor itself has no default update logic. Derived classes such as
 * Agent or Obstacle override this method to implement their own behavior.
 *
 * @param deltaTime Time elapsed since last frame.
 */
void GameActor::Update(float deltaTime)
{
}
