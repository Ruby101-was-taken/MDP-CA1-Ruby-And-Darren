#pragma once
#include "resource_identifiers.hpp"
#include "state_id.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include "music_player.hpp"
#include "sound_player.hpp"

namespace sf
{
	class RenderWindow;
}

class Player;
class StateStack;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player, MusicPlayer& music, SoundPlayer& sounds);
		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
		Player* player;
		MusicPlayer* music;
		SoundPlayer* sounds;
	};

public:
	State(StateStack& stack, Context context);
	virtual ~State();
	virtual void Draw() = 0;
	virtual bool Update(sf::Time dt) = 0;
	virtual bool HandleEvent(const sf::Event& event) = 0;


protected:
	void RequestStackPush(StateID state_id);
	void RequestStackPop();
	void RequestStackClear();

	Context GetContext() const;

private:
	StateStack* stack_;
	Context context_;
};

