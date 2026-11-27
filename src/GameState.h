#pragma once

#include "State.h"
#include "Ball.h"
#include "PlayerPaddle.h"
#include "EnemyPaddle.h"

#include <SFML/Audio.hpp>

class GameState : public State
{
public:
    GameState(std::stack<std::unique_ptr<State>>& states, unsigned int screenWidth, unsigned int screenHeight);

    void Update(float dt) override;
    void Render(sf::RenderTarget& target) override;

private:
    // Layout
    static constexpr float paddleWidth = 15.0f;
    static constexpr float paddleHeight = 125.0f;
    // Distance from each side of the screen to the left edge of its paddle
    static constexpr float paddleMargin = 50.0f;
    static constexpr unsigned int textSize = 40;

    bool gameStarted = false;

    // Entities
    sf::FloatRect walls;
    Ball ball;
    PlayerPaddle playerPaddle;
    EnemyPaddle enemyPaddle;

    // UI
    sf::Font font;
    sf::Text startText{ font };
    sf::Text playerScoreText{ font };
    sf::Text enemyScoreText{ font };

    // Score
    int playerScore = 0;
    int enemyScore = 0;

    // Sound
    sf::SoundBuffer hitBuffer;
    sf::SoundBuffer pointBuffer;
    sf::Sound hitSound{ hitBuffer };
    sf::Sound pointSound{ pointBuffer };
};
