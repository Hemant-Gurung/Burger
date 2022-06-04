#include "MiniginPCH.h"
#include "SServiceLocator.h"
 NullSoundSystem SServiceLocator::_default_ss;
Sound_System* SServiceLocator::_ss_instance = &_default_ss;