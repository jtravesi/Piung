#include "State.h"

State::State(std::stack<std::unique_ptr<State>>& states)
    :
    states(states)
{
}

void State::EndState()
{
    running = false;
}
