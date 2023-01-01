#include <windows.h>
#include <stdexcept>

#include "platform.hpp"
#include "game.hpp"

using namespace prosper;

int main()
{
    prosper::Renderer *renderer{};
    prosper::Window *window{};

    auto gamestate = create_new_gamestate();

    // event handler responsible for forwarding window events to the game layer
    auto window_event_handler = [&](WindowEvent event, const WindowEventContext &context)
    {
        if (event == WindowEvent::Input)
        {
            process_input(gamestate, context.input.key, context.input.keystate);
        }
        else if (event == WindowEvent::Resize)
        {
            renderer->on_resize(context.resize.width, context.resize.height);
        }
    };

    window = create_window(window_event_handler);

    renderer = create_renderer(window);

    window->show();

    while (true)
    {
        window->process_messages();

        process_tick(gamestate);

        process_rendering(gamestate, renderer);

        window->swap_buffers();
    }
}