#include "Ball.h"
#include <cmath>

Ball::Ball(const sf::Vector2f& startPos, const sf::Vector2f& startDir)
    :
    startPos(startPos),
    startDir(startDir.normalized()),
    pos(startPos),
    dir(this->startDir)
{
    ball.setPosition(pos);
    ball.setRadius(radius);
    ball.setFillColor(sf::Color::White);
}

void Ball::Render(sf::RenderTarget& target) const
{
    target.draw(ball);
}

void Ball::Update(float dt)
{
    pos += dir * speed * dt;
    ball.setPosition(pos);
}

void Ball::Rebound(sf::Vector2f newDir)
{
    dir = newDir.normalized();
}

void Ball::IncrementSpeed()
{
    speed += speedRate;
}

void Ball::Respawn()
{
    startDirX = -startDirX;
    pos = startPos;
    dir = { startDirX * startDir.x, startDir.y };
    speed = startSpeed;
    ball.setPosition(pos);
}

WallCollisions Ball::DoWallCollisions(const sf::FloatRect& walls)
{
    const float left = walls.position.x;
    const float top = walls.position.y;
    const float right = left + walls.size.x;
    const float bottom = top + walls.size.y;

    // Put the ball back inside and point it away from the wall, rather
    // than only flipping its direction, so two consecutive checks can never
    // cancel a bounce out.
    if (pos.y <= top)
    {
        pos.y = top;
        dir.y = std::abs(dir.y);
        ball.setPosition(pos);
        return WallCollisions::TOPBOTTOMWALL;
    }
    if (pos.y + GetDiameter() >= bottom)
    {
        pos.y = bottom - GetDiameter();
        dir.y = -std::abs(dir.y);
        ball.setPosition(pos);
        return WallCollisions::TOPBOTTOMWALL;
    }

    if (pos.x < left - goalMargin)
    {
        return WallCollisions::PLAYERSIDE;
    }
    if (pos.x > right + goalMargin)
    {
        return WallCollisions::ENEMYSIDE;
    }

    return WallCollisions::NONE;
}

void Ball::SetX(float x)
{
    pos.x = x;
    ball.setPosition(pos);
}

sf::FloatRect Ball::GetRect() const
{
    return ball.getGlobalBounds();
}

sf::Vector2f Ball::GetPosition() const
{
    return pos;
}

sf::Vector2f Ball::GetDirection() const
{
    return dir;
}

float Ball::GetDiameter() const
{
    return 2.0f * radius;
}
