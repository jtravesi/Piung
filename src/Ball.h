#pragma once

#include <SFML/Graphics.hpp>

enum class WallCollisions
{
    NONE,
    TOPBOTTOMWALL,
    PLAYERSIDE,
    ENEMYSIDE
};

class Ball
{
public:
    Ball(const sf::Vector2f& startPos, const sf::Vector2f& startDir);

    void Render(sf::RenderTarget& target) const;

    // Movement
    void Update(float dt);
    void Rebound(sf::Vector2f newDir);
    void IncrementSpeed();
    void Respawn();
    void SetX(float x);

    // Collision
    WallCollisions DoWallCollisions(const sf::FloatRect& walls);

    // Getters
    sf::FloatRect GetRect() const;
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetDirection() const;
    float GetDiameter() const;

private:
    static constexpr float radius = 7.0f;
    static constexpr float speedRate = 20.0f;
    static constexpr float startSpeed = 500.0f;
    // How far past the left or right edge the ball travels before it
    // counts as a point, so it visibly leaves the screen first.
    static constexpr float goalMargin = 200.0f;

    sf::CircleShape ball;

    const sf::Vector2f startPos;
    const sf::Vector2f startDir;
    sf::Vector2f pos;
    sf::Vector2f dir;
    float startDirX = 1.0f; // Serves alternate sides after each point
    float speed = startSpeed;
};
