#pragma once


#include <functional>
#include <vector>
#include <algorithm>
#include <array>
#include <stdexcept>
#include <span>

// include before windows, since windows headers export stuff like this:
// #define near
#include <math.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#define LOG_INFORMATION(x) std::cout << x << std::endl;
#define CRITICAL_ERROR(msg) LOG_INFORMATION(msg)

namespace prosper
{
	struct Buffer
	{
		unsigned char* data, * cursor = data;

		Buffer()
		{
			data = new unsigned char[0x1000];
			cursor = data;
		}

		template <typename T, size_t N>
		Buffer(const T(&array)[N])
		{
			data = (unsigned char*) (array);
			cursor = data + sizeof(array);
		}

		template <typename...Ts>
		Buffer(const Ts& ...ts)
		{
			Buffer::Buffer();

			(..., push(ts));
		}

		Buffer(void* data, size_t size)
		{
			this->data = reinterpret_cast<unsigned char*>(data);
			this->cursor = this->data + size;
		}


		template <typename T>
		void push(const T& value)
		{
			memcpy(cursor, &value, sizeof(value));
			cursor += sizeof(value);
		}

		template <typename T, typename ...Ts>
		void push(const T& value, const Ts& ... ts)
		{
			push(ts...);
		}

		constexpr size_t size() const
		{
			return cursor - data;
		}

		void clear()
		{
			cursor = data;
		}

		constexpr operator const void* () const
		{
			return data;
		}

		constexpr operator void* ()
		{
			return data;
		}
	};
}