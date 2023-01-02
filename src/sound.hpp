#pragma once

#include <string>

namespace prosper
{
    using SoundHandle = void *;

    class Sound
    {
    public:
        virtual void load(std::string_view name) = 0;

        virtual void play(SoundHandle sound) = 0;
    };
}