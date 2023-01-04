#pragma once

namespace prosper
{
	struct Allocator
	{
		virtual void* alloc(size_t size)
		{
			return malloc(size);
		}

		virtual void* resize(void* buffer, size_t size)
		{
			return realloc(buffer, size);
		}
	};
}