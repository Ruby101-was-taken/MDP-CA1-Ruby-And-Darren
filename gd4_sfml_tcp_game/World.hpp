#pragma once
#include <SFML/Graphics.hpp>
#include "resource_identifiers.hpp"
#include "resource_holder.hpp"
#include "scene_node.hpp"
#include "scene_layers.hpp"
#include "aircraft.hpp"
#include "texture_id.hpp"
#include "sprite_node.hpp"
#include "command_queue.hpp"
#include "bloom_effect.hpp"
#include "sound_player.hpp"

#include <array>

class World 
{
public:
	explicit World(sf::RenderTarget& target, FontHolder& font, SoundPlayer& sounds);
	void Update(sf::Time dt);
	void Draw();

	CommandQueue& GetCommandQueue();

	bool HasAlivePlayer() const;
	bool HasPlayerReachedEnd() const;

private:
	void LoadTextures();
	void BuildScene();
	void AdaptPlayerPosition();
	void AdaptPlayerVelocity();

	void SpawnEnemies();
	void AddEnemies();
	void AddEnemy(AircraftType type, float relx, float rely);
	sf::FloatRect GetViewBounds() const;
	sf::FloatRect GetBattleFieldBounds() const;

	void DestroyEntitiesOutsideView();
	void GuideMissiles();

	void HandleCollisions();
	void UpdateSounds();


private:
	struct SpawnPoint
	{
		SpawnPoint(AircraftType type, float x, float y) :type(type), x(x), y(y)
		{

		}
		AircraftType type;
		float x;
		float y;
	};

private:
	sf::RenderTarget& target_;
	sf::RenderTexture scene_texture_;
	sf::View camera_;
	TextureHolder textures_;
	FontHolder& fonts_;
	SoundPlayer& sounds_;
	SceneNode scenegraph_;
	std::array<SceneNode*, static_cast<int>(SceneLayers::kLayerCount)> scene_layers_;
	sf::FloatRect world_bounds_;
	sf::Vector2f spawn_position_;
	float scrollspeed_;
	Aircraft* player_aircraft_;

	CommandQueue command_queue_;

	std::vector<SpawnPoint> enemy_spawn_points_;
	std::vector<Aircraft*> active_enemies_;

	BloomEffect bloom_effect_;
};

