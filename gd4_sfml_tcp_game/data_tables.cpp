#include "data_tables.hpp"
#include "aircraft_type.hpp"
#include "projectile_type.hpp"
#include "pickup_type.hpp"
#include "aircraft.hpp"
#include "particle_type.hpp"

std::vector<AircraftData> InitializeAircraftData()
{
    std::vector<AircraftData> data(static_cast<int>(AircraftType::kAircraftCount));

    data[static_cast<int>(AircraftType::kEagle)].hitpoints = 100;
    data[static_cast<int>(AircraftType::kEagle)].speed = 200.f;
    data[static_cast<int>(AircraftType::kEagle)].fire_interval = sf::seconds(1);
    data[static_cast<int>(AircraftType::kEagle)].texture = TextureID::kEntities;
    data[static_cast<int>(AircraftType::kEagle)].texture_rect = sf::IntRect({ 0, 0 }, { 48, 64 });
    data[static_cast<int>(AircraftType::kEagle)].has_roll_animation = true;

    data[static_cast<int>(AircraftType::kRaptor)].hitpoints = 20;
    data[static_cast<int>(AircraftType::kRaptor)].speed = 80.f;
    data[static_cast<int>(AircraftType::kRaptor)].fire_interval = sf::Time::Zero;
    data[static_cast<int>(AircraftType::kRaptor)].texture = TextureID::kEntities;
    data[static_cast<int>(AircraftType::kRaptor)].texture_rect = sf::IntRect({ 144, 0 }, { 84, 64 });
    data[static_cast<int>(AircraftType::kRaptor)].has_roll_animation = false;

    //AI for Raptor
    data[static_cast<int>(AircraftType::kRaptor)].directions.emplace_back(Direction(+45.f, 80.f));
    data[static_cast<int>(AircraftType::kRaptor)].directions.emplace_back(Direction(-45.f, 160.f));
    data[static_cast<int>(AircraftType::kRaptor)].directions.emplace_back(Direction(+45.f, 80.f));


    data[static_cast<int>(AircraftType::kAvenger)].hitpoints = 40;
    data[static_cast<int>(AircraftType::kAvenger)].speed = 50.f;
    data[static_cast<int>(AircraftType::kAvenger)].fire_interval = sf::seconds(2);
    data[static_cast<int>(AircraftType::kAvenger)].texture = TextureID::kEntities;
    data[static_cast<int>(AircraftType::kAvenger)].texture_rect = sf::IntRect({ 228, 0 }, { 60, 59 });
    data[static_cast<int>(AircraftType::kAvenger)].has_roll_animation = false;

    //AI for Raptor
    data[static_cast<int>(AircraftType::kAvenger)].directions.emplace_back(Direction(+45.f, 50.f));
    data[static_cast<int>(AircraftType::kAvenger)].directions.emplace_back(Direction(0.f, 50.f));
    data[static_cast<int>(AircraftType::kAvenger)].directions.emplace_back(Direction(-45.f, 100.f));
    data[static_cast<int>(AircraftType::kAvenger)].directions.emplace_back(Direction(0.f, 50.f));
    data[static_cast<int>(AircraftType::kAvenger)].directions.emplace_back(Direction(45.f, 50.f));

    return data;
}

std::vector<ProjectileData> InitializeProjectileData()
{
    std::vector<ProjectileData> data(static_cast<int>(ProjectileType::kProjectileCount));
    data[static_cast<int>(ProjectileType::kAlliedBullet)].damage = 10;
    data[static_cast<int>(ProjectileType::kAlliedBullet)].speed = 300;
    data[static_cast<int>(ProjectileType::kAlliedBullet)].texture = TextureID::kEntities;
    data[static_cast<int>(ProjectileType::kAlliedBullet)].texture_rect = sf::IntRect({ 175, 64 }, { 3, 14 });

    data[static_cast<int>(ProjectileType::kEnemyBullet)].damage = 10;
    data[static_cast<int>(ProjectileType::kEnemyBullet)].speed = 300;
    data[static_cast<int>(ProjectileType::kEnemyBullet)].texture = TextureID::kEntities;
    data[static_cast<int>(ProjectileType::kEnemyBullet)].texture_rect = sf::IntRect({ 175, 64 }, { 3, 14 });


    data[static_cast<int>(ProjectileType::kMissile)].damage = 200;
    data[static_cast<int>(ProjectileType::kMissile)].speed = 150;
    data[static_cast<int>(ProjectileType::kMissile)].texture = TextureID::kEntities;
    data[static_cast<int>(ProjectileType::kMissile)].texture_rect = sf::IntRect({ 160, 64 }, { 15, 32 });

    return data;
}

std::vector<PickupData> InitializePickupData()
{
    std::vector<PickupData> data(static_cast<int>(PickupType::kPickupCount));
    data[static_cast<int>(PickupType::kHealthRefill)].texture = TextureID::kEntities;
    data[static_cast<int>(PickupType::kHealthRefill)].texture_rect = sf::IntRect({ 0, 64 }, { 40, 40 });
    data[static_cast<int>(PickupType::kHealthRefill)].action = [](Aircraft& a)
        {
            a.Repair(25);
        };

    data[static_cast<int>(PickupType::kMissileRefill)].texture = TextureID::kEntities;
    data[static_cast<int>(PickupType::kMissileRefill)].texture_rect = sf::IntRect({ 40, 64 }, { 40, 40 }); 
    
    data[static_cast<int>(PickupType::kMissileRefill)].action = std::bind(&Aircraft::CollectMissile, std::placeholders::_1, 3);

    data[static_cast<int>(PickupType::kFireSpread)].texture = TextureID::kEntities;
    data[static_cast<int>(PickupType::kFireSpread)].texture_rect = sf::IntRect({ 80, 64 }, { 40, 40 });
    data[static_cast<int>(PickupType::kFireSpread)].action = std::bind(&Aircraft::IncreaseFireSpread, std::placeholders::_1);

    data[static_cast<int>(PickupType::kFireRate)].texture = TextureID::kEntities;
    data[static_cast<int>(PickupType::kFireRate)].texture_rect = sf::IntRect({ 120, 64 }, { 40, 40 });
    data[static_cast<int>(PickupType::kFireRate)].action = std::bind(&Aircraft::IncreaseFireRate, std::placeholders::_1);
    
    return data;
}

std::vector<ParticleData> InitializeParticleData()
{
    std::vector<ParticleData> data(static_cast<int>(ParticleType::kParticleCount));

    data[static_cast<int>(ParticleType::kPropellant)].color = sf::Color(255, 255, 50);
    data[static_cast<int>(ParticleType::kPropellant)].lifetime = sf::seconds(0.5f);

    data[static_cast<int>(ParticleType::kSmoke)].color = sf::Color(50, 50, 50);
    data[static_cast<int>(ParticleType::kSmoke)].lifetime = sf::seconds(2.5f);

    return data;
}
