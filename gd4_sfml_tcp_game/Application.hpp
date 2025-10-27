#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Player.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "StateStack.hpp"
#include "MusicPlayer.hpp"
#include "SoundPlayer.hpp"

class Application
{
public:
	Application();
	void Run();

private:
	void ProcessInput();
	void Update(sf::Time dt);
	void Render();
	void RegisterStates();

private:
	sf::RenderWindow window_;
	Player player_;

	TextureHolder textures_;
	FontHolder fonts_;

	StateStack stack_;
	static const sf::Time kTimePerFrame;

	MusicPlayer music_;
	SoundPlayer sound_;
};

