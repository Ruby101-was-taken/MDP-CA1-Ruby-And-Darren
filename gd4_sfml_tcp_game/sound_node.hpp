#pragma once
#include "scene_node.hpp"
#include "resource_identifiers.hpp"
#include "sound_effect.hpp"

class SoundPlayer;

class SoundNode : public SceneNode
{
public:
	explicit SoundNode(SoundPlayer& player);
	void PlaySound(SoundEffect sound, sf::Vector2f position);

	virtual unsigned int GetCategory() const override;


private:
	SoundPlayer& sounds_;
};

