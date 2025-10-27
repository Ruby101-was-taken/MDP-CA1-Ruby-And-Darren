#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "receiver_categories.hpp"
#include "command_queue.hpp"
#include "command.hpp"

#include <set>



class SceneNode : public sf::Transformable, public sf::Drawable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
	SceneNode(ReceiverCategories cateogry = ReceiverCategories::kNone);
	void AttachChild(Ptr child);
	Ptr DetachChild(const SceneNode& node);

	void Update(sf::Time dt, CommandQueue& commands);

	sf::Vector2f GetWorldPosition() const;
	sf::Transform GetWorldTransform() const;

	void OnCommand(const Command& command, sf::Time dt);

	virtual sf::FloatRect GetBoundingRect() const;
	void DrawBoundingRect(sf::RenderTarget& target, sf::RenderStates states, sf::FloatRect& rect) const;

	void CheckSceneCollision(SceneNode& scene_graph, std::set<Pair>& collison_pairs);
	void RemoveWrecks();
	virtual unsigned int GetCategory() const;

private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);
	void UpdateChildren(sf::Time dt, CommandQueue& commands);

	//Note draw() is from sf::Drawable and hence the name
	//Do not be tempted to call this method Draw()
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
	

	void CheckNodeCollision(SceneNode& node, std::set<Pair>& collison_pairs);
	virtual bool IsDestroyed() const;
	virtual bool IsMarkedForRemoval() const;

private:
	std::vector<Ptr> children_;
	SceneNode* parent_;
	ReceiverCategories default_category_;
};
float Distance(const SceneNode& lhs, const SceneNode& rhs);
bool Collision(const SceneNode& lhs, const SceneNode& rhs);

