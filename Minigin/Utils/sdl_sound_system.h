

#include "Sound_System.h"
#include "SDL_mixer.h"
#include "mutex"

class sdl_sound_system:public Sound_System
{
	const char* _waveFileNames[4] =
	{
		"../Data/Sounds/Start.mp3",
		"../Data/Sounds/Game Music.mp3",
		"../Data/Sounds/Lose Life.mp3",
		"../Data/Sounds/Level End.mp3"
	};

	Mix_Chunk* _sample[3];
public:
	sdl_sound_system();
	~sdl_sound_system();
	

	void Play(const SoundID& id, const int volume) override;
	
	// SOUND FEEDBACK
	//
	void Update() override;
	
	void StartSound(Mix_Music* resource, int /*volume*/) override;
	

	Mix_Chunk* LoadSound(const char* name) override;
	
private:
	Mix_Music* gMusic = nullptr;
	std::mutex mMutex;
	std::thread m_Thread;

	bool m_isStartPlaying, m_isGameMusicPlaying, m_isLevelEnded, m_isPlayerDead;
};