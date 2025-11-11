#pragma once
#include "scene_node.hpp"
#include "particle_type.hpp"
#include "particle_node.hpp"

class EmitterNode : public SceneNode
{
public:
	explicit EmitterNode(ParticleType type);

private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);
	void EmitParticles(sf::Time dt);

private:
	sf::Time accumulated_time_;
	ParticleType type_;
	ParticleNode* particle_system_;
};

