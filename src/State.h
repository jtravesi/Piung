#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

class State
{
public:
    explicit State(std::stack<std::unique_ptr<State>>& states);
    virtual ~State() = default;

    virtual void Update(float dt) = 0;
    virtual void Render(sf::RenderTarget& target) = 0;
    void EndState();

    bool IsRunning() const { return running; }

protected:
    std::stack<std::unique_ptr<State>>& states;
    bool running = true;
};
