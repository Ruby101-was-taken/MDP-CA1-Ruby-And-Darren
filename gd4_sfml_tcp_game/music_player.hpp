#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Audio/Music.hpp>

#include <map>
#include <string>

#include "MusicThemes.hpp"

class MusicPlayer 
{
public:
	MusicPlayer();

	void Play(MusicThemes theme);
	void Stop();

	void SetPaused(bool paused);
	void SetVolume(float volume);


private:
	sf::Music music_;
	std::map<MusicThemes, std::string> filenames_;
	float volume_;
};

