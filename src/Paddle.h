#pragma once

#include "Ball.h"

class Paddle
{
public:
    Paddle(sf::Vector2f pos, float width, float height);
    virtual ~Paddle() = default;

    void Render(sf::RenderTarget& target) const;

    virtual void Update(float dt, const Ball& ball, const sf::FloatRect& walls) = 0;

    // Bounces the ball if it hits this paddle. Returns whether it did.
    bool DoBallCollision(Ball& ball) const;

protected:
    // Keeps the paddle inside the walls
    float ClampScreen(float y, const sf::FloatRect& walls) const;

    sf::Vector2f pos;
    const float width;
    const float height;
    sf::RectangleShape paddle;

    // Horizontal direction the ball leaves in after a hit: +1 sends it to
    // the right (player paddle), -1 to the left (enemy paddle).
    float ballCollisionDir = 1.0f;
    float speed = 400.0f;
};
