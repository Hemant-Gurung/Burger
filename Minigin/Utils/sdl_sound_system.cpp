#include "MiniginPCH.h"
#include "sdl_sound_system.h"

sdl_sound_system::sdl_sound_system():m_Thread()
{
}

sdl_sound_system::~sdl_sound_system()
{
	{
		Mix_FreeMusic(gMusic);
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
		//assert((tail_ + 1) % MAX_PENDING != head_);

		//add it to the end of the list
		pending_[tail_].id = id;
		pending_[tail_].volume = volume;

		std::lock_guard<std::mutex>lock(mMutex);
		tail_=(tail_+1)%MAX_PENDING;
	
	}
}

void sdl_sound_system::Update()
{
	{

		if (head_ == tail_) return;
		if ((int)pending_[head_].id == (int)SoundID::WALK)
		{
			_sample[0] = LoadSound("prasansa.mp3");
			Mix_PlayChannel(-1, _sample[0], 0);

		}

		else if ((int)pending_[head_].id == (int)SoundID::DIE)
		{
			_sample[1] = LoadSound("BOW.mp3");
			Mix_PlayChannel(-1, _sample[1], 1);


		}

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
	{
		auto a = Mix_LoadWAV(name);
		if (a == nullptr)
		{
			std::cout << Mix_GetError() << std::endl;
		}
		
		return a;
	}
}
