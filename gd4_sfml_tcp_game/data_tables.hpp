#pragma once
#include <vector>
#include <SFML/System/Time.hpp>
#include "resource_identifiers.hpp"
#include <functional>
#include "Aircraft.hpp"


struct Direction
{
	Direction(float angle, float distance)
		: angle(angle), distance(distance) {}
	float angle;
	float distance;
};

struct AircraftData
{
	int hitpoints;
	float speed;
	TextureID texture;
	sf::IntRect texture_rect;
	sf::Time fire_interval;
	std::vector<Direction> directions;
	bool has_roll_animation;
};

struct ProjectileData
{
	int damage;
	float speed;
	TextureID texture;
	sf::IntRect texture_rect;
};

struct PickupData
{
	std::function<void(Aircraft&)> action;
	TextureID texture;
	sf::IntRect texture_rect;
};

struct ParticleData
{
	sf::Color color;
	sf::Time lifetime;
};

std::vector<AircraftData> InitializeAircraftData();
std::vector<ProjectileData> InitializeProjectileData();
std::vector<PickupData> InitializePickupData();
std::vector<ParticleData> InitializeParticleData();

