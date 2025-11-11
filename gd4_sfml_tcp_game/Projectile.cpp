#include "projectile.hpp"
#include "data_tables.hpp"
#include "resource_holder.hpp"
#include "utility.hpp"
#include "emitter_node.hpp"
#include "particle_type.hpp"

namespace
{
    const std::vector<ProjectileData> Table = InitializeProjectileData();
}

Projectile::Projectile(ProjectileType type, const TextureHolder& textures)
    : Entity(1), type_(type), sprite_(textures.Get(Table[static_cast<int>(type)].texture), Table[static_cast<int>(type)].texture_rect)
{
    Utility::CentreOrigin(sprite_);

    //Add particle system for missiles
    if (IsGuided())
    {
        std::unique_ptr<EmitterNode> smoke(new EmitterNode(ParticleType::kSmoke));
        smoke->setPosition({ 0.f, GetBoundingRect().size.y / 2.f });
        AttachChild(std::move(smoke));

        std::unique_ptr<EmitterNode> propellant(new EmitterNode(ParticleType::kPropellant));
        propellant->setPosition({0.f, GetBoundingRect().size.y / 2.f});
        AttachChild(std::move(propellant));
    }
}

void Projectile::GuideTowards(sf::Vector2f position)
{
    assert(IsGuided());
    target_direction_ = Utility::UnitVector(position - GetWorldPosition());
}

bool Projectile::IsGuided() const
{
    return type_ == ProjectileType::kMissile;
}

unsigned int Projectile::GetCategory() const
{
    if (type_ == ProjectileType::kEnemyBullet)
    {
        return static_cast<int>(ReceiverCategories::kEnemyProjectile);
    }
    else
        return static_cast<int>(ReceiverCategories::kAlliedProjectile);
}
 
sf::FloatRect Projectile::GetBoundingRect() const
{
    return GetWorldTransform().transformRect(sprite_.getGlobalBounds());
}

float Projectile::GetMaxSpeed() const
{
    return Table[static_cast<int>(type_)].speed;
}

float Projectile::GetDamage() const
{
    return Table[static_cast<int>(type_)].damage;
}

void Projectile::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (IsGuided())
    {
        const float approach_rate = 200;
        sf::Vector2f new_velocity = Utility::UnitVector(approach_rate * dt.asSeconds() * target_direction_ + GetVelocity());
        new_velocity *= GetMaxSpeed();
        float angle = std::atan2(new_velocity.y, new_velocity.x);
        setRotation(sf::radians(angle) + sf::degrees(90.f));
        SetVelocity(new_velocity);
    }
    Entity::UpdateCurrent(dt, commands);
}

void Projectile::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite_, states);
}
