#include "music_player.hpp"

MusicPlayer::MusicPlayer()
	: volume_(10.f)
{
	filenames_[MusicThemes::kMenuTheme] = "Media/Music/MenuTheme.ogg";
	filenames_[MusicThemes::kMissionTheme] = "Media/Music/MissionTheme.ogg";
}

void MusicPlayer::Play(MusicThemes theme)
{
	std::string filename = filenames_[theme];

	if (!music_.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	music_.setVolume(volume_);
	music_.setLooping(true);
	music_.play();
}

void MusicPlayer::Stop()
{
	music_.stop();
}

void MusicPlayer::SetVolume(float volume)
{
	volume_ = volume;
}

void MusicPlayer::SetPaused(bool paused)
{
	if (paused)
		music_.pause();
	else
		music_.play();
}