

#include "Sound_System.h"
#include "SDL_mixer.h"
#include "mutex"

class sdl_sound_system:public Sound_System
{
	const char* _waveFileNames[2] =
	{
		"prasansa.mp3",
		"BOW.mp3",
	};

	Mix_Chunk* _sample[2];
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
};