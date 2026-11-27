#pragma once

#include "Paddle.h"
#include <random>

class EnemyPaddle : public Paddle
{
public:
    EnemyPaddle(sf::Vector2f pos, float width, float height);

    void Update(float dt, const Ball& ball, const sf::FloatRect& walls) override;

private:
    // The AI only re-aims every movementDelay seconds, like a reaction time.
    static constexpr float movementDelay = 0.25f;
    // It stops moving once its centre is this close to where it aims.
    static constexpr float deadZone = 50.0f;
    // Random error added to each aim, in pixels, so it can miss.
    static constexpr float aimError = 50.0f;

    void MoveTowards(float yDest);
    float CalculateYGoal(const sf::Vector2f& ballPos, const sf::Vector2f& ballDir);

    float dirY = 1.0f;
    float timer = 0.0f;
    std::mt19937 rnd{ std::random_device{}() };
    std::uniform_real_distribution<float> dist{ -aimError, aimError };
};
