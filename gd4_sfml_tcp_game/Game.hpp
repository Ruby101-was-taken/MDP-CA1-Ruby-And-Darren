#pragma once
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "Player.hpp"

class Game
{
	public:
		Game();
		void Run();

	private:
		void Update(sf::Time delta_time);
		void Render();
		void ProcessInput();

	private:

		
		static const sf::Time kTimePerFrame;
		sf::RenderWindow window_;
		World world_;
		Player player_;
};