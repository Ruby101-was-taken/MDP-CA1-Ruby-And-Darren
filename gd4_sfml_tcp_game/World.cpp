#include "world.hpp"
#include "pickup.hpp"
#include "projectile.hpp"
#include "particle_node.hpp"
#include "sound_node.hpp"

World::World(sf::RenderTarget& output_target, FontHolder& font, SoundPlayer& sounds)
	:target_(output_target)
	,camera_(output_target.getDefaultView())
	,textures_()
	,fonts_(font)
	,sounds_(sounds)
	,scenegraph_(ReceiverCategories::kNone)
	,scene_layers_()
	,world_bounds_({ 0.f,0.f }, { camera_.getSize().x, 3000.f })
	,spawn_position_(camera_.getSize().x/2.f, world_bounds_.size.y - camera_.getSize().y/2.f)
	,scrollspeed_(-50.f)
	,player_aircraft_(nullptr)
	,scene_texture_({ target_.getSize().x, target_.getSize().y })
{
	LoadTextures();
	BuildScene();
	camera_.setCenter(spawn_position_);
}

void World::Update(sf::Time dt)
{
	//Scroll the world
	camera_.move({ 0, scrollspeed_ * dt.asSeconds() });
	
	player_aircraft_->SetVelocity(0.f, 0.f);

	DestroyEntitiesOutsideView();
	GuideMissiles();

	//Forward commands to the scenegraph
	while (!command_queue_.IsEmpty())
	{
		scenegraph_.OnCommand(command_queue_.Pop(), dt);
	}
	AdaptPlayerVelocity();

	HandleCollisions();

	scenegraph_.RemoveWrecks();

	SpawnEnemies();

	scenegraph_.Update(dt, command_queue_);
	AdaptPlayerPosition();
}

void World::Draw()
{
	if (PostEffect::IsSupported())
	{
		scene_texture_.clear();
		scene_texture_.setView(camera_);
		scene_texture_.draw(scenegraph_);
		scene_texture_.display();
		bloom_effect_.Apply(scene_texture_, target_);
	}
	else
	{
		target_.setView(camera_);
		target_.draw(scenegraph_);
	}
}

CommandQueue& World::GetCommandQueue()
{
	return command_queue_;
}

bool World::HasAlivePlayer() const
{
	return !player_aircraft_->IsMarkedForRemoval();
}

bool World::HasPlayerReachedEnd() const
{
	return !world_bounds_.contains(player_aircraft_->getPosition());
}

void World::LoadTextures()
{
	textures_.Load(TextureID::kEagle, "Media/Textures/Eagle.png");
	textures_.Load(TextureID::kRaptor, "Media/Textures/Raptor.png");
	textures_.Load(TextureID::kAvenger, "Media/Textures/Avenger.png");
	textures_.Load(TextureID::kLandscape, "Media/Textures/Desert.png");
	textures_.Load(TextureID::kBullet, "Media/Textures/Bullet.png");
	textures_.Load(TextureID::kMissile, "Media/Textures/Missile.png");

	textures_.Load(TextureID::kHealthRefill, "Media/Textures/HealthRefill.png");
	textures_.Load(TextureID::kMissileRefill, "Media/Textures/MissileRefill.png");
	textures_.Load(TextureID::kFireSpread, "Media/Textures/FireSpread.png");
	textures_.Load(TextureID::kFireRate, "Media/Textures/FireRate.png");
	textures_.Load(TextureID::kFinishLine, "Media/Textures/FinishLine.png");

	textures_.Load(TextureID::kEntities, "Media/Textures/Entities.png");
	textures_.Load(TextureID::kJungle, "Media/Textures/Jungle.png");
	textures_.Load(TextureID::kExplosion, "Media/Textures/Explosion.png");
	textures_.Load(TextureID::kParticle, "Media/Textures/Particle.png");


}

void World::BuildScene()
{
	//Initialize the different layers
	for (std::size_t i = 0; i < static_cast<int>(SceneLayers::kLayerCount); ++i)
	{
		ReceiverCategories category = (i == static_cast<int>(SceneLayers::kLowerAir)) ? ReceiverCategories::kScene : ReceiverCategories::kNone;
		SceneNode::Ptr layer(new SceneNode(category));
		scene_layers_[i] = layer.get();
		scenegraph_.AttachChild(std::move(layer));
	}

	//Prepare the background
	sf::Texture& texture = textures_.Get(TextureID::kJungle);
	sf::IntRect textureRect(world_bounds_);
	texture.setRepeated(true);

	//Add the background sprite to the world
	std::unique_ptr<SpriteNode> background_sprite(new SpriteNode(texture, textureRect));
	background_sprite->setPosition({ world_bounds_.position.x, world_bounds_.position.y });
	scene_layers_[static_cast<int>(SceneLayers::kBackground)]->AttachChild(std::move(background_sprite));

	//Add the finish line
	sf::Texture& finish_texture = textures_.Get(TextureID::kFinishLine);
	std::unique_ptr<SpriteNode> finish_sprite(new SpriteNode(finish_texture));
	finish_sprite->setPosition({ 0.f, -76.f });
	scene_layers_[static_cast<int>(SceneLayers::kBackground)]->AttachChild(std::move(finish_sprite));

	//Add the player's aircraft
	std::unique_ptr<Aircraft> leader(new Aircraft(AircraftType::kEagle, textures_, fonts_));
	player_aircraft_ = leader.get();
	player_aircraft_->setPosition(spawn_position_);
	player_aircraft_->SetVelocity(40.f, scrollspeed_);
	scene_layers_[static_cast<int>(SceneLayers::kUpperAir)]->AttachChild(std::move(leader));

	//Add the particle nodes to the scene
	std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(ParticleType::kSmoke, textures_));
	scene_layers_[static_cast<int>(SceneLayers::kLowerAir)]->AttachChild(std::move(smokeNode));

	std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(ParticleType::kPropellant, textures_));
	scene_layers_[static_cast<int>(SceneLayers::kLowerAir)]->AttachChild(std::move(propellantNode));

	// Add sound effect node
	std::unique_ptr<SoundNode> soundNode(new SoundNode(sounds_));
	scenegraph_.AttachChild(std::move(soundNode));

	AddEnemies();

	/*std::unique_ptr<Aircraft> left_escort(new Aircraft(AircraftType::kRaptor, textures_, fonts_));
	left_escort->setPosition(-80.f, 50.f);
	player_aircraft_->AttachChild(std::move(left_escort));

	std::unique_ptr<Aircraft> right_escort(new Aircraft(AircraftType::kRaptor, textures_, fonts_));
	right_escort->setPosition(80.f, 50.f);
	player_aircraft_->AttachChild(std::move(right_escort));*/
}

void World::AdaptPlayerPosition()
{
	//keep the player on the screen
	sf::FloatRect view_bounds(camera_.getCenter() - camera_.getSize() / 2.f, camera_.getSize());
	const float border_distance = 40.f;

	sf::Vector2f position = player_aircraft_->getPosition();
	position.x = std::max(position.x, view_bounds.position.x + border_distance);
	position.x = std::min(position.x, view_bounds.position.x + view_bounds.size.x - border_distance);
	position.y = std::max(position.y, view_bounds.position.y + border_distance);
	position.y = std::min(position.y, view_bounds.position.y + view_bounds.size.y -border_distance);
	player_aircraft_->setPosition(position);
}

void World::AdaptPlayerVelocity()
{
	sf::Vector2f velocity = player_aircraft_->GetVelocity();

	//If they are moving diagonally divide by sqrt 2
	if (velocity.x != 0.f && velocity.y != 0.f)
	{
		player_aircraft_->SetVelocity(velocity / std::sqrt(2.f));
	}
	//Add scrolling velocity
	player_aircraft_->Accelerate(0.f, scrollspeed_);
}

void World::SpawnEnemies()
{
	//Spawn an enemy when it is relevant i.e when it is in the Battlefieldboudns
	while (!enemy_spawn_points_.empty() && enemy_spawn_points_.back().y > GetBattleFieldBounds().position.y)
	{
		SpawnPoint spawn = enemy_spawn_points_.back();
		std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.type, textures_, fonts_));
		enemy->setPosition({ spawn.x, spawn.y });
		enemy->setRotation(sf::degrees(180.f));
		scene_layers_[static_cast<int>(SceneLayers::kUpperAir)]->AttachChild(std::move(enemy));
		enemy_spawn_points_.pop_back();
	}
}

void World::AddEnemies()
{
	AddEnemy(AircraftType::kRaptor, 0.f, 500.f);
	AddEnemy(AircraftType::kRaptor, 0.f, 1000.f);
	AddEnemy(AircraftType::kRaptor, 100.f, 1100.f);
	AddEnemy(AircraftType::kRaptor, -100.f, 1100.f);
	AddEnemy(AircraftType::kAvenger, -70.f, 1400.f);
	AddEnemy(AircraftType::kAvenger, 70.f, 1400.f);
	AddEnemy(AircraftType::kAvenger, 70.f, 1600.f);

	//Sort the enemies according to y-value so that enemies are checked first
	std::sort(enemy_spawn_points_.begin(), enemy_spawn_points_.end(), [](SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.y < rhs.y;
	});

}

void World::AddEnemy(AircraftType type, float relx, float rely)
{
	SpawnPoint spawn(type, spawn_position_.x + relx, spawn_position_.y - rely);
	enemy_spawn_points_.emplace_back(spawn);
}

sf::FloatRect World::GetViewBounds() const
{
	return sf::FloatRect(camera_.getCenter() - camera_.getSize()/2.f, camera_.getSize());
}

sf::FloatRect World::GetBattleFieldBounds() const
{
	//Return camera bounds + a small area at the top where enemies spawn
	sf::FloatRect bounds = GetViewBounds();
	bounds.position.y -= 100.f;
	bounds.size.y += 100.f;

	return bounds;

}

void World::DestroyEntitiesOutsideView()
{
	Command command;
	command.category = static_cast<int>(ReceiverCategories::kEnemyAircraft) | static_cast<int>(ReceiverCategories::kProjectile);
	command.action = DerivedAction<Entity>([this](Entity& e, sf::Time dt)
		{
			//Does the object intersect with the battlefield
			if (!GetBattleFieldBounds().findIntersection(e.GetBoundingRect()).has_value())
			{
				e.Destroy();
			}
		});
	command_queue_.Push(command);
}

void World::GuideMissiles()
{
	//Target the closest enemy in the world
	Command enemyCollector;
	enemyCollector.category = static_cast<int>(ReceiverCategories::kEnemyAircraft);
	enemyCollector.action = DerivedAction<Aircraft>([this](Aircraft& enemy, sf::Time)
		{
			if (!enemy.IsDestroyed())
			{
				active_enemies_.emplace_back(&enemy);
			}
		});

	Command missileGuider;
	missileGuider.category = static_cast<int>(ReceiverCategories::kAlliedProjectile);
	missileGuider.action = DerivedAction<Projectile>([this](Projectile& missile, sf::Time dt)
		{
			if (!missile.IsGuided())
			{
				return;
			}

			float min_distance = std::numeric_limits<float>::max();
			Aircraft* closest_enemy = nullptr;

			for (Aircraft* enemy : active_enemies_)
			{
				float enemy_distance = Distance(missile, *enemy);
				if (enemy_distance < min_distance)
				{
					closest_enemy = enemy;
					min_distance = enemy_distance;
				}
			}

			if (closest_enemy)
			{
				missile.GuideTowards(closest_enemy->GetWorldPosition());
			}
		});

	command_queue_.Push(enemyCollector);
	command_queue_.Push(missileGuider);
	active_enemies_.clear();
}

bool MatchesCategories(SceneNode::Pair& colliders, ReceiverCategories type1, ReceiverCategories type2)
{
	unsigned int category1 = colliders.first->GetCategory();
	unsigned int category2 = colliders.second->GetCategory();

	if (static_cast<int>(type1) & category1 && static_cast<int>(type2) & category2)
	{
		return true;
	}
	else if (static_cast<int>(type1) & category2 && static_cast<int>(type2) & category1)
	{ 
		std::swap(colliders.first, colliders.second);
	}
	else
	{
		return false;
	}
}


void World::HandleCollisions()
{
	std::set<SceneNode::Pair> collision_pairs;
	scenegraph_.CheckSceneCollision(scenegraph_, collision_pairs);
	for (SceneNode::Pair pair : collision_pairs)
	{
		if (MatchesCategories(pair, ReceiverCategories::kPlayerAircraft, ReceiverCategories::kEnemyAircraft))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& enemy = static_cast<Aircraft&>(*pair.second);
			//Collision response
			player.Damage(enemy.GetHitPoints());
			enemy.Destroy();
		}

		else if (MatchesCategories(pair, ReceiverCategories::kPlayerAircraft, ReceiverCategories::kPickup))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);
			//Collision response
			pickup.Apply(player);
			pickup.Destroy();
			player.PlayLocalSound(command_queue_, SoundEffect::kCollectPickup);
		}
		else if (MatchesCategories(pair, ReceiverCategories::kPlayerAircraft, ReceiverCategories::kEnemyProjectile) || MatchesCategories(pair, ReceiverCategories::kEnemyAircraft, ReceiverCategories::kAlliedProjectile))
		{
			auto& aircraft = static_cast<Aircraft&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);
			//Collision response
			aircraft.Damage(projectile.GetDamage());
			projectile.Destroy();
		}
	}
}

void World::UpdateSounds()
{
	// Set listener's position to player position
	sounds_.SetListenerPosition(player_aircraft_->GetWorldPosition());

	// Remove unused sounds
	sounds_.RemoveStoppedSounds();
}
