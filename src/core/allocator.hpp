#pragma once

namespace prosper
{
	struct Allocator
	{
		virtual void* alloc(size_t size)
		{
			return new unsigned char[size];
		}
	};
}