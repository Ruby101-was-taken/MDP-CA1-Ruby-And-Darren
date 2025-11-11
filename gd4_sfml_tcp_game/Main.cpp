#include <SFML/Graphics.hpp>
#include "game.hpp"
#include "resource_identifiers.hpp"
#include <iostream>
#include "application.hpp"

int main()
{
	//TextureHolder game_textures;
	try
	{
		Application app;
		app.Run();
	}
	catch(std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
}