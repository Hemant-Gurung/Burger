#include "MiniginPCH.h"
#include "sdl_sound_system.h"
sdl_sound_system::sdl_sound_system():m_Thread()
{
	
	//auto a = Mix_LoadMUS("prasansa.mp3");
	//Mix_PlayMusic(a, 1);
	m_isStartPlaying = true;
	m_isGameMusicPlaying = false;
	m_isLevelEnded = false;
	m_isPlayerDead = false;
	
}

sdl_sound_system::~sdl_sound_system()
{
	{
		Mix_FreeMusic(gMusic);

		Mix_FreeChunk(_sample[0]);
		Mix_FreeChunk(_sample[1]);
		Mix_FreeChunk(_sample[2]);
		//m_Thread.join();
	}
}

void sdl_sound_system::Play(const SoundID& id, const int volume)
{
	{
		for(int i = head_;i!=tail_;i=(i+1)%MAX_PENDING)
		{
			if(pending_[i].id==id)
			{
				pending_[i].volume = std::max(volume, pending_[i].volume);
				return;
			}
		}
		//add it to the end of the list
		pending_[tail_].id = id;
		pending_[tail_].volume = volume;

		std::lock_guard<std::mutex>lock(mMutex);
		tail_=(tail_+1)%MAX_PENDING;
	
	}
}

void sdl_sound_system::Update()
{
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 0);

	switch ((int)pending_[head_].id)
	{
	case (int)SoundID::WALK:
		//_sample[0] = LoadSound(_waveFileNames[1]);
		////	_sample[0] = dae::ResourceManager::GetInstance().LoadSound("Sounds/Start.wav");
		//Mix_PlayChannel(-1, _sample[0], 0);
		break;

	case (int)SoundID::STARTSCREEN:
		_sample[1] = LoadSound(_waveFileNames[0]);
		if (!Mix_Playing(1) && !Mix_Playing(0))
		{
			Mix_PlayChannel(0, _sample[1], 1);
		}
		break;

	case (int)SoundID::GAMESOUND:
		_sample[1] = LoadSound(_waveFileNames[1]);
		if (Mix_Playing(0))
		{
			Mix_HaltChannel(0);
		}
		
		if (!Mix_Playing(1))
		{
			Mix_PlayChannel(1, _sample[1], 0);
		}
		break;
	case (int)SoundID::LEVEL_END:
		_sample[1] = LoadSound(_waveFileNames[3]);
		//	_sample[0] = dae::ResourceManager::GetInstance().LoadSound("Sounds/Start.wav");
		Mix_PlayChannel(2, _sample[1], 0);
		break;
	
	case (int)SoundID::NONE:
		
		break;
	}

	{
		//StartSound(gMusic, 50);
		std::lock_guard<std::mutex>lock(mMutex);
		head_ = (head_ + 1) % MAX_PENDING;


	}
}

void sdl_sound_system::StartSound(Mix_Music* resource, int)
{
	{
		if (resource == nullptr)
		{
			std::cout << Mix_GetError() << std::endl;
		}
		else
		{
			//
			Mix_PlayMusic(resource, 1);
		}
	}
}

Mix_Chunk* sdl_sound_system::LoadSound(const char* name)
{
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 0);
	{
		auto a = Mix_LoadWAV( name);
		if (a == nullptr)
		{
			std::cout << Mix_GetError() << std::endl;
		}
		
		return a;
	}
}
