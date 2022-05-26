#pragma once
#include "Sound_System.h"
#include "NullSoundSystem.h"
class SServiceLocator
{
	//service locator
	
		static Sound_System* _ss_instance;
		static NullSoundSystem _default_ss;
	public:
		static Sound_System& get_sound_system() { return *_ss_instance; }
		static void register_sound_system(Sound_System* ss)
		{
			_ss_instance = ss == nullptr ? &_default_ss : ss;
		}
};



