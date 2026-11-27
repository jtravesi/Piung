#include "Paddle.h"
#include <iostream>

Paddle::Paddle(sf::Vector2f pos, float width, float height)
    :
    pos(pos),
    width(width),
    height(height)
{
    paddle.setPosition(pos);
    paddle.setFillColor(sf::Color::White);
    paddle.setSize(sf::Vector2f(width, height));
}

void Paddle::Render(sf::RenderTarget& target)
{
    target.draw(paddle);
}

bool Paddle::DoBallCollision(Ball& ball) const
{
    sf::FloatRect paddleRect = paddle.getGlobalBounds();
    sf::FloatRect ballRect = ball.GetRect();

    // Only a ball that moves towards the paddle and has not already got
    // past it can be hit. Otherwise a ball still touching the paddle while
    // moving away was hit again (extra speed-up and sound), and a ball the
    // player had already missed was knocked forward when the paddle moved
    // onto it from the side.
    const bool movingTowards = ball.GetDirection().x * ballColllisionDir < 0.0f;
    const float ballCenterX = ballRect.position.x + ballRect.size.x / 2.0f;
    const bool notPast = ballColllisionDir > 0.0f
        ? ballCenterX > paddleRect.position.x
        : ballCenterX < paddleRect.position.x + paddleRect.size.x;

    // Collision happens
    if (movingTowards && notPast && paddleRect.findIntersection(ballRect))
    {
        // Move the ball out to the paddle's face so it leaves cleanly.
        if (ballColllisionDir > 0.0f)
        {
            ball.SetX(paddleRect.position.x + paddleRect.size.x);
        }
        else
        {
            ball.SetX(paddleRect.position.x - ball.GetDiameter());
        }

        const float distFromCenter = (ballRect.position.y + ballRect.size.y / 2.0f) - (pos.y + height / 2.0f);
        const float ratio = distFromCenter / (height / 2.0f);

        ball.Rebound({ ballColllisionDir, ratio });
        ball.IncrementSpeed();

        //Original way of colliding with the ball. I'll let there cause why not :D


        /*// Top collision
        if (ballRect.top <= pos.y + height / 3.0f)
        {
            ball.Rebound({ ballColllisionDir, -0.75f });
        }
        // Medium collision
        if (ballRect.top > pos.y + height / 3.0f && ballRect.top < pos.y + 2.0f * height / 3.0f)
        {
            ball.Rebound({ ballColllisionDir, 0.0f });
        }
        // Bottom collision
        if (ballRect.top >= pos.y + 2.0f * height / 3.0f)
        {
            ball.Rebound({ ballColllisionDir, 0.75f });
        }
        */

        return true;
    }

    return false;
}

float Paddle::ClampScreen(float y, const sf::FloatRect& walls)
{
    const float wallsTop = walls.position.y;
    const float wallsBottom = wallsTop + walls.size.y;
    if (y <= wallsTop)
    {
        return wallsTop;
    }
    else if (y + height > wallsBottom)
    {
        return wallsBottom - height;
    }
    else
    {
        return y;
    }
}
