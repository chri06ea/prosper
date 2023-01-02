#include "platform.hpp"

namespace prosper
{
    void sound_test();

    class Win32Sound : public Sound
    {
    public:
        static Win32Sound *create();

        void initialize(void *hwnd);

        virtual void load(std::string_view name) override;

        virtual void play(SoundHandle sound) override;
    };
}