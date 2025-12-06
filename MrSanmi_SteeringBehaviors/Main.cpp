#include <iostream>
#include "Physics.h"
#include "Agent.h"
#include "Obstacle.h"
#include "LevelManager.h"
#include <fstream>

/**
 * @brief Global level manager used to track all agents and obstacles.
 */
LevelManager manager;

/**
 * @brief Helper function to spawn obstacles directly at the mouse position.
 *
 * @param radius Radius of the new obstacle.
 */
inline void CreateObstacleInScene(float radius)
{
    Obstacle* obstacle = new Obstacle(radius, (sf::Vector2f)SteeringMath::mousePosition);
    manager.AddActorToLevel(obstacle);
}

int main()
{
    constexpr float MOUSE_RANGE = 150.0f;

    sf::RenderWindow window(sf::VideoMode({ 1000, 1000 }), "Steering Behaviors Demo");
    sf::Clock clock;

    float deltaTime = 0;
    float lastTime = 0;
    bool mouseIsNearAgent = false;
    bool isRightClicking = false;

    // Texture loading test
    std::string texturePath = "Arrow.png";
    std::filesystem::path filePath(texturePath);

    if (std::filesystem::exists(filePath)) {
        std::cout << "Texture file found.\n";
        std::cout << "Size: " << std::filesystem::file_size(filePath) << " bytes\n";
    }
    else {
        std::cout << "Texture file NOT found.\n";
        std::cout << "Checked path: " << std::filesystem::absolute(filePath) << "\n";
    }

    // ---------------------------
    // Agent creation
    // ---------------------------

    std::vector<Agent*> agents;
    sf::Texture texture;
    bool hasTexture = texture.loadFromFile(texturePath);

    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        sf::Vector2f pos(rand() % 1000, rand() % 1000);
        Agent* follower = new Agent(10.f, pos);

        if (hasTexture)
            follower->renderer.shape.setTexture(&texture);

        follower->renderer.shape.setOrigin(sf::Vector2f(10.f, 10.f));

        manager.AddActorToLevel(follower);
        agents.push_back(follower);
    }

    // Main controlled agent (the first one)
    Agent* agent = agents.front();

    // Attempt to reassign texture (conditional looks inverted, but kept as-is)
    if (texture.loadFromFile(texturePath)) {
        std::cout << "Texture failed to load?!\n";
        agent->renderer.shape.setTexture(&texture);
    }

    // ---------------------------
    // Main game loop
    // ---------------------------
    while (window.isOpen())
    {
        mouseIsNearAgent = SteeringMath::isWithinTheRange(
            agent->transform.position,
            (sf::Vector2f)sf::Mouse::getPosition(window),
            MOUSE_RANGE
        );

        // ---------------------------
        // Event handling
        // ---------------------------
        while (const std::optional event = window.pollEvent())
        {
            // Window close
            if (event->is<sf::Event::Closed>())
                window.close();

            // Key input for spawning obstacles
            if (const auto& keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::Num1)
                    CreateObstacleInScene(10);

                if (keyPressed->scancode == sf::Keyboard::Scan::Num2)
                    CreateObstacleInScene(20);

                if (keyPressed->scancode == sf::Keyboard::Scan::Num3)
                    CreateObstacleInScene(30);
            }

            // Mouse pressed
            if (const auto& mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                // Left click -> Seek state
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    SteeringMath::mousePosition = sf::Mouse::getPosition(window);
                    agent->setState(Seek);
                }

                // Right click -> Begin runaway / wander logic
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
                {
                    isRightClicking = true;
                }
            }

            // Mouse released
            if (const auto& mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Right) {
                    isRightClicking = false;

                    // If the agent was fleeing, return to wander
                    if (agent->getAgentState() == RunAway) {
                        agent->setState(Wander);
                    }
                }
            }

            // Spacebar -> flocking behavior for all agents except the main one
            if (const auto& keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::Space) {
                    for (int i = 1; i < 10; i++) {
                        agents[i]->setState(Flock);
                    }
                }
            }
        }

        // ---------------------------
        // Right-click interactions
        // ---------------------------
        if (isRightClicking) {
            if (mouseIsNearAgent) {
                agent->setState(RunAway);
                SteeringMath::mousePosition = sf::Mouse::getPosition(window);
            }
            else {
                agent->setState(Wander);
            }
        }

        // ---------------------------
        // Time calculation
        // ---------------------------
        sf::Time elapsed = clock.getElapsedTime();
        deltaTime = elapsed.asSeconds() - lastTime;

        // Update world
        manager.Update(deltaTime);

        // Debug line for agent direction
        sf::Vector2f dir = agent->GetDirection().normalized() * 100.f;

        std::array line =
        {
            sf::Vertex{sf::Vector2f(agent->Shape().getPosition())},
            sf::Vertex{sf::Vector2f(agent->Shape().getPosition() - dir)}
        };

        // ---------------------------
        // Rendering
        // ---------------------------
        window.clear();
        manager.DrawGameActors(window);
        window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        window.display();

        lastTime = elapsed.asSeconds();
    }

    return 0;
}
