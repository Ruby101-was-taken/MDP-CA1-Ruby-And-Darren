#include "particle_node.hpp"
#include "data_tables.hpp"
#include "resource_holder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace
{
    const std::vector<ParticleData> Table = InitializeParticleData();
}

ParticleNode::ParticleNode(ParticleType type, const TextureHolder& textures)
    : SceneNode()
    , texture_(textures.Get(TextureID::kParticle))
    , type_(type)
    , vertex_array_(sf::PrimitiveType::TriangleStrip)
    , needs_vertex_update_(true)
{
}

void ParticleNode::AddParticle(sf::Vector2f position)
{
    Particle particle;
    particle.position_ = position;
    particle.color_ = Table[static_cast<int>(type_)].color;
    particle.lifetime_ = Table[static_cast<int>(type_)].lifetime;

    particles_.emplace_back(particle);
}

ParticleType ParticleNode::GetParticleType() const
{
    return type_;
}

unsigned int ParticleNode::GetCategory() const
{
    return static_cast<int>(ReceiverCategories::kParticleSystem);
}

void ParticleNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
    //Remove expired particles at beginning
    while (!particles_.empty() && particles_.front().lifetime_ <= sf::Time::Zero)
    {
        particles_.pop_front();
    }

    //Decrease lifetime of existing particles
    for (Particle& particle : particles_)
    {
        particle.lifetime_ -= dt;
    }
    needs_vertex_update_ = true;
}

void ParticleNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (needs_vertex_update_)
    {
        ComputeVertices();
        needs_vertex_update_ = false;
    }

    //Apply particle texture
    states.texture = &texture_;

    //Draw the vertices
    target.draw(vertex_array_, states);
}

void ParticleNode::AddVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const
{
    sf::Vertex vertex;
    vertex.position = sf::Vector2f(worldX, worldY);
    vertex.texCoords = sf::Vector2f(texCoordX, texCoordY);
    vertex.color = color;

    vertex_array_.append(vertex);
}

void ParticleNode::ComputeVertices() const
{
    sf::Vector2f size(texture_.getSize());
    sf::Vector2f half = size / 2.f;

    vertex_array_.clear();

    for (const Particle& particle : particles_)
    {
        sf::Vector2f pos = particle.position_;
        sf::Color color = particle.color_;

        float ratio = particle.lifetime_.asSeconds() / Table[static_cast<int>(type_)].lifetime.asSeconds();
        color.a = static_cast<uint8_t>(255 * std::max(ratio, 0.f));

        AddVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
        AddVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
        AddVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
        AddVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
    }

}
