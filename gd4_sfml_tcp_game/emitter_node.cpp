#include "emitter_node.hpp"

EmitterNode::EmitterNode(ParticleType type)
	:SceneNode()
	, accumulated_time_(sf::Time::Zero)
	, type_(type)
	, particle_system_(nullptr)
{
}

void EmitterNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (particle_system_)
	{
		EmitParticles(dt);
	}
	else
	{
		// Find particle node with the same type as the emitter
		auto finder = [this](ParticleNode& container, sf::Time)
			{
				if (container.GetParticleType() == type_)
					particle_system_ = &container;
			};
		Command command;
		command.category = static_cast<int>(ReceiverCategories::kParticleSystem);
		command.action = DerivedAction<ParticleNode>(finder);

		commands.Push(command);
	}
}

void EmitterNode::EmitParticles(sf::Time dt)
{
	const float emissionRate = 30.f;
	const sf::Time interval = sf::seconds(1.f) / emissionRate;

	accumulated_time_ += dt;
	while (accumulated_time_ > interval)
	{
		accumulated_time_ -= interval;
		particle_system_->AddParticle(GetWorldPosition());
	}
}
