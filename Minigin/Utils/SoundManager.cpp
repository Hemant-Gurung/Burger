#include "MiniginPCH.h"
#include "SoundManager.h"


void SoundManager::Init(std::string&& data)
{
	mDataPath = std::move(data);
	/*AddSoundEffectVolume(0);
	AddSoundStreamVolume(0);*/
}

void SoundManager::LoadSoundEffect(const std::string& name, const std::string& path)
{
	const auto find = m_EffectMap.find(std::move(name));
	if (find != m_EffectMap.end())
		return;

	std::string fullPath{ mDataPath + path };
	std::shared_ptr<SoundEffect> temp = std::make_shared<SoundEffect>(std::move(fullPath));
	m_EffectMap.insert({ std::move(name), temp });
}

void SoundManager::LoadSoundStream(const std::string& name, const std::string& path)
{
	const auto find = m_StreamMap.find(std::move(name));
	if (find != m_StreamMap.end())
		return;

	std::string fullPath{ mDataPath + path };
	std::shared_ptr<SoundStream> temp = std::make_shared<SoundStream>(std::move(fullPath));
	m_StreamMap.insert({ std::move(name), temp });

}

void SoundManager::PlaySoundEffect(const std::string& name, int loops)
{
	const auto find = m_EffectMap.find(std::move(name));
	if (find == m_EffectMap.end())
		std::cout<<"SoundManager::PlaySoundEffect : sound to play not found with name : " + name;

	(*find).second->Play(loops);
}

void SoundManager::PlaySoundStream(const std::string& name, bool repeat)
{
	const auto find = m_StreamMap.find(std::move(name));
	if (find == m_StreamMap.end())
		std::cout << ("SoundManager::PlaySoundStream : sound to play not found with name : " + name);

	m_CurrentActiveSoundStream = name;
	(*find).second->Play(repeat);
}

void SoundManager::StopSoundEffect() const
{
	Mix_HaltChannel(-1);
}

void SoundManager::StopSoundStream()
{
	Mix_HaltMusic();
	m_CurrentActiveSoundStream = "";
}

void SoundManager::HaltSoundEffect() const
{
	Mix_Pause(-1);
}

void SoundManager::HaltSoundStream() const
{
	Mix_PauseMusic();
}

void SoundManager::ResumeSoundEffect() const
{
	Mix_Resume(-1);
}

void SoundManager::ResumeSoundStream() const
{
	Mix_ResumeMusic();
}

//void SoundManager::AddSoundStreamVolume(int value)
//{
//	m_SoundStreamVolume += value;
//	if (m_SoundStreamVolume < 0)
//		m_SoundStreamVolume = 0;
//	if (m_SoundStreamVolume > 100)
//		m_SoundStreamVolume = 100;
//	Mix_VolumeMusic(m_SoundStreamVolume);
//}
//
//void SoundManager::AddSoundEffectVolume(int value)
//{
//	m_SoundEffectVolume += value;
//	if (m_SoundEffectVolume < 0)
//		m_SoundEffectVolume = 0;
//	if (m_SoundEffectVolume > 100)
//		m_SoundEffectVolume = 100;
//	for (auto it = m_EffectMap.begin(); it != m_EffectMap.end(); ++it)
//	{
//		it->second->SetVolume(m_SoundEffectVolume);
//	}
//}
