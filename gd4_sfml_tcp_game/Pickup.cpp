#include "pickup.hpp"
#include "data_tables.hpp"
#include "resource_holder.hpp"
#include "utility.hpp"


namespace
{
    const std::vector<PickupData> Table = InitializePickupData();
}

Pickup::Pickup(PickupType type, const TextureHolder& textures)
    : Entity(1)
    , type_(type)
    , sprite_(textures.Get(Table[static_cast<int>(type)].texture), Table[static_cast<int>(type)].texture_rect)
{
    Utility::CentreOrigin(sprite_);
}

unsigned int Pickup::GetCategory() const
{
    return static_cast<int>(ReceiverCategories::kPickup);
}

sf::FloatRect Pickup::GetBoundingRect() const
{
    return GetWorldTransform().transformRect(sprite_.getGlobalBounds());
}

void Pickup::Apply(Aircraft& player) const
{
    Table[static_cast<int>(type_)].action(player);
}

void Pickup::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite_, states);
}
