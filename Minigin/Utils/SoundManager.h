#pragma once
#include "Singleton.h"
#include "SoundEffect.h"
#include "SoundStream.h"
#include <unordered_map>

class SoundManager final : public dae::Singleton<SoundManager>
{
	std::string mDataPath;
public:
	SoundManager() = default;

	void Init(std::string&& data);

	void LoadSoundEffect(const std::string& name, const std::string& path);
	void LoadSoundStream(const std::string& name, const std::string& path);

	void PlaySoundEffect(const std::string& name, int loops);
	void PlaySoundStream(const std::string& name, bool repeat);
	bool IsSoundStreamPlaying(const std::string& name) const {return name == m_CurrentActiveSoundStream;}

	void StopSoundEffect() const;
	void StopSoundStream();

	void HaltSoundEffect() const;
	void HaltSoundStream() const;

	void ResumeSoundEffect() const;
	void ResumeSoundStream() const;

	//void AddSoundStreamVolume(int value);
	//void AddSoundEffectVolume(int value);

private:
	std::unordered_map<std::string, std::shared_ptr<SoundEffect>> m_EffectMap;
	std::unordered_map<std::string, std::shared_ptr<SoundStream>> m_StreamMap;
	std::string m_CurrentActiveSoundStream{""};
	int m_SoundStreamVolume = 30;
	int m_SoundEffectVolume = 50;
};

