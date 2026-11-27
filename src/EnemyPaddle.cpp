#include "EnemyPaddle.h"

EnemyPaddle::EnemyPaddle(sf::Vector2f pos, float width, float height)
    :
    Paddle(pos, width, height)
{
    ballCollisionDir = -1.0f;
}

void EnemyPaddle::MoveTowards(float yDest)
{
    const float posYCenter = pos.y + height / 2.0f;
    if (yDest < posYCenter)
    {
        dirY = -1.0f;
    }
    else if (yDest > posYCenter)
    {
        dirY = 1.0f;
    }
    if (yDest >= posYCenter - deadZone && yDest <= posYCenter + deadZone)
    {
        dirY = 0.0f;
    }
}

void EnemyPaddle::Update(float dt, const Ball& ball, const sf::FloatRect& walls)
{
    timer += dt;

    if (timer >= movementDelay)
    {
        MoveTowards(CalculateYGoal(ball.GetPosition(), ball.GetDirection()));
        timer = 0.0f;
    }

    pos.y += dirY * speed * dt;
    pos.y = ClampScreen(pos.y, walls);
    paddle.setPosition(pos);
}

float EnemyPaddle::CalculateYGoal(const sf::Vector2f& ballPos, const sf::Vector2f& ballDir)
{
    // Where the ball's current straight line crosses the paddle's x, plus
    // a random error so the AI plays more like a human.
    const float m = ballDir.y / ballDir.x;
    const float n = -m * ballPos.x + ballPos.y;

    return m * pos.x + n + dist(rnd);
}
