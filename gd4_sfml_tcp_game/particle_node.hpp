#pragma once
#include "scene_node.hpp"
#include "particle_type.hpp"
#include "resource_identifiers.hpp"
#include "particle.hpp"
#include <deque>

class ParticleNode : public SceneNode
{
public:
	ParticleNode(ParticleType type, const TextureHolder& textures);

	void AddParticle(sf::Vector2f position);
	ParticleType GetParticleType() const;
	virtual unsigned int GetCategory() const;

private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void AddVertex(float worldX, float worldY, float texCoordX, float textCoordY, const sf::Color& color) const;
	void ComputeVertices() const;

private:
	std::deque<Particle> particles_;
	const sf::Texture& texture_;
	ParticleType type_;

	mutable sf::VertexArray vertex_array_;
	mutable bool needs_vertex_update_;

};

