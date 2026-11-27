#include "GameState.h"
#include "Resources.h"

GameState::GameState(std::stack<std::unique_ptr<State>>& states, unsigned int screenWidth, unsigned int screenHeight)
    :
    State(states),
    walls({ 0.0f, 0.0f }, { static_cast<float>(screenWidth), static_cast<float>(screenHeight) }),
    ball({ walls.size.x / 2.0f, walls.size.y / 2.0f }, { 1.0f, 0.0f }),
    playerPaddle({ paddleMargin, walls.size.y / 3.0f }, paddleWidth, paddleHeight),
    enemyPaddle({ walls.size.x - paddleMargin, walls.size.y / 3.0f }, paddleWidth, paddleHeight)
{
    if (!font.openFromFile(ResourcePath("Fonts/bit5x3.ttf")))
    {
        sf::err() << "Could not load the font\n";
    }
    // Sounds are loaded once; each has its own buffer so a point sound
    // never cuts off a hit sound that is still playing.
    if (!hitBuffer.loadFromFile(ResourcePath("Audio/hit.wav")) ||
        !pointBuffer.loadFromFile(ResourcePath("Audio/point.wav")))
    {
        sf::err() << "Could not load the sounds\n";
    }

    startText.setCharacterSize(textSize);
    startText.setPosition({ walls.size.x / 4.0f, walls.size.y / 3.0f });
    startText.setString("Press Enter to start");

    playerScoreText.setCharacterSize(textSize);
    playerScoreText.setPosition({ walls.size.x / 5.0f, walls.size.y / 10.0f });
    playerScoreText.setString(std::to_string(playerScore));

    enemyScoreText.setCharacterSize(textSize);
    enemyScoreText.setPosition({ 4.0f * walls.size.x / 5.0f, walls.size.y / 10.0f });
    enemyScoreText.setString(std::to_string(enemyScore));
}

void GameState::Update(float dt)
{
    // Escape quits from the start screen as well as during play.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        EndState();
        return;
    }

    if (!gameStarted)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
        {
            gameStarted = true;
        }
    }
    else
    {
        ball.Update(dt);

        const WallCollisions wallCollision = ball.DoWallCollisions(walls);

        // Score for enemy
        if (wallCollision == WallCollisions::PLAYERSIDE)
        {
            ball.Respawn();
            ++enemyScore;
            enemyScoreText.setString(std::to_string(enemyScore));
            pointSound.play();
        }
        // Score for player
        else if (wallCollision == WallCollisions::ENEMYSIDE)
        {
            ball.Respawn();
            ++playerScore;
            playerScoreText.setString(std::to_string(playerScore));
            pointSound.play();
        }

        playerPaddle.Update(dt, ball, walls);
        enemyPaddle.Update(dt, ball, walls);

        if (playerPaddle.DoBallCollision(ball) || enemyPaddle.DoBallCollision(ball))
        {
            hitSound.play();
        }
    }
}

void GameState::Render(sf::RenderTarget& target)
{
    if (!gameStarted)
    {
        target.draw(startText);
    }
    else
    {
        ball.Render(target);
        playerPaddle.Render(target);
        enemyPaddle.Render(target);
        target.draw(playerScoreText);
        target.draw(enemyScoreText);
    }
}
