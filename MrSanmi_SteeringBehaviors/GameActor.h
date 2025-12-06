#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"
#include <iostream>

/**
 * @brief Cardinal movement directions for sprite animation or logic.
 */
enum directions
{
    down,
    right,
    up,
    left
};

/**
 * @brief Holds texture and sprite references for renderable actors.
 * @details The engine does not own the texture memory — users are responsible for lifetime management.
 */
struct CustomSpriteRenderer
{
    sf::Sprite* sprite = nullptr;   ///< Pointer to the sprite used for rendering.
    sf::Texture* texture = nullptr; ///< Texture associated with the sprite.
};

/**
 * @brief Basic spatial data for any actor in the scene.
 * @details Position is in world units (10 px = 1 unit as defined by the system).
 */
struct Transform
{
public:
    sf::Vector2f position; ///< World position of the actor.
    float scale{ 10.0f };  ///< Visual scale and collider radius reference.
};

/**
 * @brief Simple shape-based renderer for prototype or debug visuals.
 */
struct Renderer
{
public:
    sf::CircleShape shape; ///< Shape used for drawing.
    sf::Color color;       ///< Fill color of the shape.
};

/**
 * @Author: Miguel Angel Garcia Elizalde and Marco Antonio Garcia
 * @Date: November, 2025
 * @file: GameActor.h
 * @brief Base class for any game entity using sprites or shapes.
 * @details This class provides shared functionality such as transform,
 *          optional sprite rendering, collider generation, and runtime type checking.
 */
class GameActor
{
public:
    CustomSpriteRenderer spriteRenderer; ///< Optional sprite renderer.
    Collider collider;                   ///< Collision component.
    Transform transform;                 ///< Transform component.
    Renderer renderer;                   ///< Prototype renderer.

    /**
     * @brief Default constructor.
     */
    GameActor() {}

    /**
     * @brief Constructor for circle/shape-based actors.
     * @param position Starting world position.
     * @param scale Visual and collider scale.
     * @param color Circle fill color.
     */
    GameActor(sf::Vector2f position, float scale, sf::Color color = sf::Color::Green)
    {
        transform.position = position;
        transform.scale = scale;
        renderer.shape = sf::CircleShape(scale);
        renderer.color = color;
        renderer.shape.setFillColor(renderer.color);
        renderer.shape.setPosition(transform.position);
        collider = Collider(scale, transform.position.x, transform.position.y);
    }

    /**
     * @brief Constructor for sprite-based actors.
     * @param position Initial world position.
     * @param scale Sprite size and collider radius.
     * @param texture Texture applied to the sprite.
     * @note Initializes 4-directional animation frames based on texture layout.
     */
    GameActor(sf::Vector2f position, float scale, sf::Texture& texture)
    {
        usingSprite = true;

        transform.position = position;
        transform.scale = scale;

        spriteRenderer.sprite = new sf::Sprite(texture);
        spriteRenderer.sprite->setPosition(transform.position);
        spriteRenderer.sprite->setOrigin(sf::Vector2f(scale / 2, scale / 2));

        collider = Collider(scale, transform.position.x, transform.position.y);

        for (int i = 0; i < 4; ++i)
        {
            dir[i] = sf::IntRect(
                { static_cast<int>(scale * i), 0 },
                { static_cast<int>(scale), static_cast<int>(scale) }
            );
        }

        spriteRenderer.sprite->setTextureRect(dir[down]);
    }

    /**
     * @brief Virtual destructor for safe cleanup.
     */
    virtual ~GameActor() = default;

    /**
     * @brief Base update method. Overridden by derived classes.
     * @param deltaTime Frame time.
     */
    virtual void Update(float deltaTime);

    /**
     * @return Circle shape for debug/prototype rendering.
     */
    sf::CircleShape Shape() { return renderer.shape; }

    /**
     * @return Pointer to sprite used for rendering.
     */
    sf::Sprite* Sprite() { return spriteRenderer.sprite; }

    /**
     * @brief Runtime type-check helper.
     * @tparam T Class type to compare.
     * @return True if this actor is of type T.
     */
    template <typename T>
    bool Is() const {
        return dynamic_cast<const T*>(this) != nullptr;
    }

protected:
    sf::IntRect dir[4];          ///< Sprite rectangles for directional animations.
    bool usingSprite{ false };   ///< True if the actor uses a sprite instead of a shape.
    sf::Angle spriteRotation = sf::degrees(0.f); ///< Visual sprite rotation.
    float rotationAngle{ 0.f };  ///< Internal rotation accumulator.
};
