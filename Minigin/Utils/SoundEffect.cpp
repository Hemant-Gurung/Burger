#include "MiniginPCH.h"
#include "SoundEffect.h"



SoundEffect::SoundEffect(const std::string& path)
	:m_pMixChunk{ Mix_LoadWAV(path.c_str()) }
{
	if (m_pMixChunk == nullptr)
	{
		const std::string error = "Failed to load soundEffect " + path + ": " + Mix_GetError();
		
	}
	m_IsLoaded = true;
}

SoundEffect::~SoundEffect()
{
	Mix_FreeChunk(m_pMixChunk);
	m_pMixChunk = nullptr;
}

bool SoundEffect::IsLoaded() const
{
	return m_IsLoaded;
}

void SoundEffect::Play(int loops) const
{
	if (!m_IsLoaded)
		std::cout<<"SoundEffect not loaded when trying to play";
	Mix_PlayChannel(-1, m_pMixChunk, loops);
}

void SoundEffect::SetVolume(int value) const
{
	if (m_IsLoaded)
		Mix_VolumeChunk(m_pMixChunk, value);
}