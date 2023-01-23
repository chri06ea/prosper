#include <platform/services/platform.hpp>

namespace lib
{
	class Win32Platform : public IPlatform
	{
	public:

		virtual uint64_t get_tick_count() override;

		virtual uint64_t get_ticks_per_second() override;
	};
}