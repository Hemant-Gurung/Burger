#include "MiniginPCH.h"
#include "SoundStream.h"


SoundStream::SoundStream(const std::string& path)
{
	m_pMixMusic = Mix_LoadMUS(path.c_str());
	//auto a = Mix_LoadWAV(path.c_str());
	//std::cout << a;
	if (m_pMixMusic == nullptr)
	{
		std::cout<<"SoundStream: Failed to load " + path + ": " + Mix_GetError();
		
	}
	m_IsLoaded = true;
}

SoundStream::~SoundStream()
{
	Mix_FreeMusic(m_pMixMusic);
	m_pMixMusic = nullptr;
}

bool SoundStream::IsLoaded() const
{
	return m_IsLoaded;
}

void SoundStream::Play(bool repeat) const
{
	if (!m_IsLoaded)
		std::cout << ("SoundStream not loaded when trying to play");
	Mix_PlayMusic(m_pMixMusic, repeat ? -1 : 1);
}