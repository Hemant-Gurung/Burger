#pragma once

//sound system
#include "SDL_mixer.h"
enum class SoundID
{
	NONE = 0,
	WALK = 1,
	DIE = 2,
	FOOD_DROP = 3
	
};
struct PlayMessage
{
	SoundID id;
	int volume;
};
//using sound_id = unsigned short;
class Sound_System
{
public:
	static void init()
	{
		head_ = 0;
		tail_ = 0;
	}
	virtual ~Sound_System() = default;
	virtual void Play(const SoundID& id, const int volume) = 0;
	virtual void Update()=0;
	virtual Mix_Chunk* LoadSound(const char* name) =0;
	virtual void StartSound(Mix_Music* id, int volume) = 0;
protected:
	static int head_;
	static int tail_;
	static const int MAX_PENDING = 16;
	static PlayMessage pending_[MAX_PENDING];
	static int numPending_;
};


class logging_sound_system:public Sound_System
{
	Sound_System* _real_ss;
public:
	logging_sound_system(Sound_System* ss):_real_ss(ss){}
	~logging_sound_system() { delete _real_ss; }

	void Play(const SoundID& id, const int volume) override
	{
		_real_ss->Play(id, volume);
		std::cout << "Playing " << (int)id << " at volume " << volume << std::endl;
	}
};

