#include "menu_state.hpp"
#include "resource_holder.hpp"
#include "utility.hpp"
#include "button.hpp"

MenuState::MenuState(StateStack& stack, Context context)
    :State(stack, context)
    , background_sprite_(context.textures->Get(TextureID::kTitleScreen))
{
    //sf::Texture& texture = context.textures->Get(TextureID::kTitleScreen);

    //background_sprite_.setTexture(texture);

    auto play_button = std::make_shared<gui::Button>(context);
    play_button->setPosition({ 100, 250 });
    play_button->SetText("Play");
    play_button->SetCallback([this]()
    {
        RequestStackPop();
        RequestStackPush(StateID::kGame);
    });

    auto settings_button = std::make_shared<gui::Button>(context);
    settings_button->setPosition({ 100, 300 });
    settings_button->SetText("Settings");
    settings_button->SetCallback([this]()
    {
        RequestStackPush(StateID::kSettings);
    });

    auto exit_button = std::make_shared<gui::Button>(context);
    exit_button->setPosition({ 100, 350 });
    exit_button->SetText("Exit");
    exit_button->SetCallback([this]()
    {
            RequestStackPop();
    });

    gui_container_.Pack(play_button);
    gui_container_.Pack(settings_button);
    gui_container_.Pack(exit_button);

    //Play the music
    context.music->Play(MusicThemes::kMenuTheme);
}

void MenuState::Draw()
{
    sf::RenderWindow& window = *GetContext().window;
    window.setView(window.getDefaultView());
    window.draw(background_sprite_);
    window.draw(gui_container_);
}

bool MenuState::Update(sf::Time dt)
{
    return true;
}

bool MenuState::HandleEvent(const sf::Event& event)
{
    gui_container_.HandleEvent(event);
    return true;
}

