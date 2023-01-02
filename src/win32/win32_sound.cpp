#include "win32_sound.hpp"

namespace prosper
{

    Win32Sound *Win32Sound::create()
    {
        static Win32Sound sound;

        return &sound;
    }
}

void prosper::Win32Sound::initialize(void *hwnd)
{
}

void prosper::Win32Sound::load(std::string_view name)
{
}

void prosper::Win32Sound::play(SoundHandle sound) {}
