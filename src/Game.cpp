#include "Game.h"
#include <algorithm>

Game::Game()
    :
    window(sf::VideoMode({ screenWidth, screenHeight }), "Piung!", sf::Style::Close | sf::Style::Titlebar)
{

    states.push(std::make_unique<GameState>(states, screenWidth, screenHeight));
    window.setFramerateLimit(120);
}

void Game::UpdateEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            running = false;
    }
}

void Game::Update()
{
    // A long pause between frames would move the ball far enough in one
    // step to pass through a paddle, so cap the step at 1/30 s; the game
    // slows down instead of skipping collisions.
    const float dt = std::min(clock.restart().asSeconds(), maxFrameTime);

    if (!states.empty())
    {
        states.top()->Update(dt);

        if (!states.top()->IsRunning())
        {
            states.pop();
        }
    }
    else
    {
        running = false;
    }
    
}

void Game::Render()
{
    window.clear();
    // Render things here
    if (!states.empty())
    {
        states.top()->Render(window);
    }
    //
    window.display();
}

bool Game::IsRunning() const
{
    return running;
}
