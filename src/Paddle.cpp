#include "Paddle.h"

Paddle::Paddle(sf::Vector2f pos, float width, float height)
    :
    pos(pos),
    width(width),
    height(height)
{
    paddle.setPosition(pos);
    paddle.setFillColor(sf::Color::White);
    paddle.setSize({ width, height });
}

void Paddle::Render(sf::RenderTarget& target) const
{
    target.draw(paddle);
}

bool Paddle::DoBallCollision(Ball& ball) const
{
    const sf::FloatRect paddleRect = paddle.getGlobalBounds();
    const sf::FloatRect ballRect = ball.GetRect();

    // Only a ball that moves towards the paddle and has not already got
    // past it can be hit. Otherwise a ball still touching the paddle while
    // moving away was hit again (extra speed-up and sound), and a ball the
    // player had already missed was knocked forward when the paddle moved
    // onto it from the side.
    const bool movingTowards = ball.GetDirection().x * ballCollisionDir < 0.0f;
    const float ballCenterX = ballRect.position.x + ballRect.size.x / 2.0f;
    const bool notPast = ballCollisionDir > 0.0f
        ? ballCenterX > paddleRect.position.x
        : ballCenterX < paddleRect.position.x + paddleRect.size.x;

    if (!movingTowards || !notPast || !paddleRect.findIntersection(ballRect))
    {
        return false;
    }

    // Move the ball out to the paddle's face so it leaves cleanly.
    if (ballCollisionDir > 0.0f)
    {
        ball.SetX(paddleRect.position.x + paddleRect.size.x);
    }
    else
    {
        ball.SetX(paddleRect.position.x - ball.GetDiameter());
    }

    // The further from the paddle's centre the ball hits, the steeper it
    // bounces: ratio goes from -1 (top edge) to 1 (bottom edge).
    const float distFromCenter = (ballRect.position.y + ballRect.size.y / 2.0f) - (pos.y + height / 2.0f);
    const float ratio = distFromCenter / (height / 2.0f);

    ball.Rebound({ ballCollisionDir, ratio });
    ball.IncrementSpeed();

    return true;
}

float Paddle::ClampScreen(float y, const sf::FloatRect& walls) const
{
    const float wallsTop = walls.position.y;
    const float wallsBottom = wallsTop + walls.size.y;
    if (y <= wallsTop)
    {
        return wallsTop;
    }
    if (y + height > wallsBottom)
    {
        return wallsBottom - height;
    }
    return y;
}
