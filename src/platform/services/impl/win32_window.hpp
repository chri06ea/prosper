#include <platform/core/defs.hpp>

#include <platform/services/window.hpp>


namespace lib
{
	class Win32Window : public IWindow
	{
	public:

		Win32Window(void* context_handle);

		virtual void swap_buffers() override;

		virtual void process_messages() override;

		virtual void get_size(size_t& width, size_t& height) override;

		virtual void get_position(size_t& width, size_t& height) override;

	private:

		void* _context_handle;
	};

	class Win32WindowFactory : public IWindowFactory
	{
	public:

		virtual Shared<IWindow> create(WindowEventHandler handler);
	};
}