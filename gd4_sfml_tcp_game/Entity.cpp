#include "Entity.hpp"

Entity::Entity(int hitpoints)
    :hitpoints_(hitpoints)
{
}

void Entity::SetVelocity(sf::Vector2f velocity)
{
    velocity_ = velocity;
}

void Entity::SetVelocity(float vx, float vy)
{
    velocity_.x = vx;
    velocity_.y = vy;
}

sf::Vector2f Entity::GetVelocity() const
{
    return velocity_;
}

void Entity::Accelerate(sf::Vector2f velocity)
{
    velocity_ += velocity;
}

void Entity::Accelerate(float vx, float vy)
{
    velocity_.x += vx;
    velocity_.y += vy;
}

int Entity::GetHitPoints() const
{
    return hitpoints_;
}

void Entity::Repair(int points)
{
    assert(points > 0);
    //TODO Limit hitpoints
    hitpoints_ += points;
}

void Entity::Damage(int points)
{
    assert(points > 0);
    hitpoints_ -= points;
}

void Entity::Destroy()
{
    hitpoints_ = 0;
}

bool Entity::IsDestroyed() const
{
    return hitpoints_ <= 0;
}

void Entity::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
    move(velocity_ * dt.asSeconds());
}
