#pragma once
#include "Entity.hpp"
#include "aircraft_type.hpp"
#include "resource_identifiers.hpp"
#include "text_node.hpp"
#include "utility.hpp"
#include "projectile_type.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include "Animation.hpp"
#include "sound_effect.hpp"

class Aircraft : public Entity {
public:
	Aircraft(AircraftType type, const TextureHolder& textures, const FontHolder& fonts);
	unsigned int GetCategory() const override;

	void IncreaseFireRate();
	void IncreaseFireSpread();
	void CollectMissile(unsigned int count);

	void UpdateTexts();
	void UpdateMovementPattern(sf::Time dt);

	float GetMaxSpeed() const;
	void Fire();
	void LaunchMissile();
	void CreateBullet(SceneNode& node, const TextureHolder& textures) const;
	void CreateProjectile(SceneNode& node, ProjectileType type, float x_float, float y_offset, const TextureHolder& textures) const;

	sf::FloatRect GetBoundingRect() const override;
	bool IsMarkedForRemoval() const override;
	void PlayLocalSound(CommandQueue& commands, SoundEffect effect);

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;
	void CheckProjectileLaunch(sf::Time dt, CommandQueue& commands);
	bool IsAllied() const;
	void CreatePickup(SceneNode& node, const TextureHolder& textures) const;
	void CheckPickupDrop(CommandQueue& commands);
	void UpdateRollAnimation();

private:
	AircraftType type_;
	sf::Sprite sprite_;
	Animation explosion_;

	TextNode* health_display_;
	TextNode* missile_display_;
	float distance_travelled_;
	int directions_index_;

	Command fire_command_;
	Command missile_command_;
	Command drop_pickup_command_;

	unsigned int fire_rate_;
	unsigned int spread_level_;
	unsigned int missile_ammo_;

	bool is_firing_;
	bool is_launching_missile_;
	sf::Time fire_countdown_;

	bool is_marked_for_removal_;
	bool show_explosion_;
	bool spawned_pickup_;
	bool played_explosion_sound_;

};

