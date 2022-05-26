#pragma once
#include "Sound_System.h"
class NullSoundSystem :
    public Sound_System
{
    void Play(const SoundID& /*id*/, const int /*volume*/) override
    {
        std::cout << "NULL MUSIC\n";
    };

    void Update() override{};
    void StartSound(Mix_Music* /*id*/, int /*volume*/) override {};
    Mix_Chunk* LoadSound(const char* /*name*/) override { return nullptr; }
};

