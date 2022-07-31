#pragma once
#include <SDL_mixer.h>

class SoundStream
{
public:
	SoundStream(const std::string& path);
	~SoundStream();

	bool IsLoaded() const;
	void Play(bool repeat) const;

private:
	Mix_Music *m_pMixMusic;
	bool m_IsLoaded = false;
};

