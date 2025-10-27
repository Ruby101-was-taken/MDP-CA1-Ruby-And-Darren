#include "Application.hpp"
#include "GameState.hpp"
#include "TitleState.hpp"
#include "MenuState.hpp"
#include "PauseState.hpp"
#include "SettingsState.hpp"
#include "GameOverState.hpp"

const sf::Time Application::kTimePerFrame = sf::seconds(1.f/60.f);

Application::Application() : window_(sf::VideoMode({ 1024, 768 }), "States", sf::Style::Close)
	, stack_(State::Context(window_, textures_, fonts_, player_, music_, sound_))
{
	window_.setKeyRepeatEnabled(false);
	fonts_.Load(Font::kMain, "Media/Fonts/Sansation.ttf");
	textures_.Load(TextureID::kTitleScreen, "Media/Textures/TitleScreen.png");
	textures_.Load(TextureID::kButtonNormal, "Media/Textures/ButtonNormal.png");
	textures_.Load(TextureID::kButtonSelected, "Media/Textures/ButtonSelected.png");
	textures_.Load(TextureID::kButtonActivated, "Media/Textures/ButtonPressed.png");
	textures_.Load(TextureID::kButtons, "Media/Textures/Buttons.png");

	RegisterStates();
	stack_.PushState(StateID::kTitle);
}

void Application::Run()
{
	sf::Clock clock;
	sf::Time time_since_last_update = sf::Time::Zero;
	while (window_.isOpen())
	{
		time_since_last_update += clock.restart();
		while(time_since_last_update > kTimePerFrame)
		{
			time_since_last_update -= kTimePerFrame;
			ProcessInput();
			Update(kTimePerFrame);

			if (stack_.IsEmpty())
			{
				window_.close();
			}
		}
		Render();

	}
}

void Application::ProcessInput()
{
	while (const std::optional event = window_.pollEvent()) {
		stack_.HandleEvent(*event);
		if (event->is<sf::Event::Closed>()) {
			window_.close();
		}
	}
}

void Application::Update(sf::Time dt)
{
	stack_.Update(dt);
}

void Application::Render()
{
	window_.clear();
	stack_.Draw();
	window_.display();
}

void Application::RegisterStates()
{
	stack_.RegisterState<TitleState>(StateID::kTitle);
	stack_.RegisterState<MenuState>(StateID::kMenu);
	stack_.RegisterState<GameState>(StateID::kGame);
	stack_.RegisterState<PauseState>(StateID::kPause);
	stack_.RegisterState<SettingsState>(StateID::kSettings);
	stack_.RegisterState<GameOverState>(StateID::kGameOver);
}
