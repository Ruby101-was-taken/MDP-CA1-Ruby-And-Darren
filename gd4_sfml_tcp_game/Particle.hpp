#pragma once
#include "ParticleType.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

struct Particle
{
	ParticleType type_;
	sf::Vector2f position_;
	sf::Color color_;
	sf::Time lifetime_;
};
