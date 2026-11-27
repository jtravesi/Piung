#pragma once

#include "State.h"

class Game
{
public:
    Game();

    void UpdateEvents();
    void Update();
    void Render();

    bool IsRunning() const;

private:
    static constexpr unsigned int screenWidth = 800;
    static constexpr unsigned int screenHeight = 600;
    static constexpr float maxFrameTime = 1.0f / 30.0f;

    sf::RenderWindow window;
    sf::Clock clock;
    bool running = true;

    std::stack<std::unique_ptr<State>> states;
};
