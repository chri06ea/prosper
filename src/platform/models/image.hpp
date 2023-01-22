#include <platform/core/defs.hpp>

namespace lib
{
	struct Image
	{
		int width, height, num_channels;
		void* data;
	};
}