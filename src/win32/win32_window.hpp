#include <platform/platform.hpp>

/*
 *   WIN32(Windows) window implementation
 */


namespace prosper
{
    class Win32Window : public Window
    {
    public:
        static Win32Window *create(WindowEventHandler window_event_handler);

        static void destroy(Win32Window *window);

        virtual void flush_messages() override;

        virtual void show() override;

        virtual void swap_buffers() override;

        virtual void init_opengl() override;

    private:
        // Private ctor. Use create/destroy functions
        Win32Window();

        // Handle used to context (Required to reach 'this' in wndproc)
        void *_context_handle{};
    };
}