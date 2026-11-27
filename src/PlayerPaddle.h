#pragma once

#include "Paddle.h"

class PlayerPaddle : public Paddle
{
public:
    PlayerPaddle(sf::Vector2f pos, float width, float height);

    void Update(float dt, const Ball& ball, const sf::FloatRect& walls) override;
};
