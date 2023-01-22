#include <platform/core/defs.hpp>

namespace lib
{
	class IPlatform
	{
	public:

		virtual uint64_t get_tick_count() = 0;

		virtual uint64_t get_ticks_per_second() = 0;
	};
}