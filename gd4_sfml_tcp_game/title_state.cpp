#include "title_state.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "resource_holder.hpp"
#include "utility.hpp"

TitleState::TitleState(StateStack& stack, Context context)
    : State(stack, context)
    , show_text_(true)
    , text_effect_time_(sf::Time::Zero)
    , background_sprite_(context.textures->Get(TextureID::kTitleScreen))
    , text_(context.fonts->Get(Font::kMain))
{
    //background_sprite_.setTexture(context.textures->Get(TextureID::kTitleScreen));
    //text_.setFont(context.fonts->Get(Font::kMain));
    text_.setString("Press any key to continue");
    Utility::CentreOrigin(text_);
    text_.setPosition(context.window->getView().getSize() / 2.f);
}


void TitleState::Draw()
{
    sf::RenderWindow& window = *GetContext().window;
    window.draw(background_sprite_);

    if (show_text_)
    {
        window.draw(text_);
    }
}

bool TitleState::Update(sf::Time dt)
{
    text_effect_time_ += dt;
    if (text_effect_time_ >= sf::seconds(0.5))
    {
        show_text_ = !show_text_;
        text_effect_time_ = sf::Time::Zero;
    }
    return true;
}

bool TitleState::HandleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        RequestStackPop();
        RequestStackPush(StateID::kMenu);
    }
    return true;
}
