#include "MiniginPCH.h"
#include "Sound_System.h"
 int Sound_System::head_ = 0;
 int Sound_System::tail_ = 0;
 PlayMessage Sound_System::pending_[16];
