#pragma once
#include "Sound_System.h"
#include "NullSoundSystem.h"
#include "ButtonManager.h"

class SServiceLocator
{
	//service locator
	
	

	public:
		static void  Init()
		{
			m_pButtonManager = m_pDefaultButtonManager;
		}

		static Sound_System& get_sound_system() { return *_ss_instance; }
		static void register_sound_system(Sound_System* ss)
		{
			_ss_instance = ss == nullptr ? &_default_ss : ss;
		}

		static void RegisterButtonManager(std::shared_ptr<ButtonManager> service)
		{
			m_pButtonManager = (service == nullptr) ? m_pDefaultButtonManager : service;
		}
		static std::shared_ptr<ButtonManager>& GetButtonManager()
		{
			return m_pButtonManager;
		}

private:

	static Sound_System* _ss_instance;
	static NullSoundSystem _default_ss;
	static std::shared_ptr<ButtonManager> m_pButtonManager;
	static std::shared_ptr<ButtonManager> m_pDefaultButtonManager;

};



