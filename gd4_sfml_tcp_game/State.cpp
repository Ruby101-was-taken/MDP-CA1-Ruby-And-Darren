#include "State.hpp"
#include "StateID.hpp"
#include "StateStack.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player, MusicPlayer& music, SoundPlayer& sounds) : window(&window), textures(&textures), fonts(&fonts), player(&player), music(&music), sounds(&sounds)
{
}

State::State(StateStack& stack, Context context) : stack_(&stack), context_(context)
{
}

State::~State()
{
}

void State::RequestStackPush(StateID state_id)
{
    stack_->PushState(state_id);
}

void State::RequestStackPop()
{
    stack_->PopState();
}

void State::RequestStackClear()
{
    stack_->ClearStack();
}

State::Context State::GetContext() const
{
    return context_;
}
