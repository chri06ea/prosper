#pragma once

#include <assert.h>

#include <core/allocator.hpp>

namespace prosper
{
	// Buffer implementation

	struct Buffer
	{
		unsigned char* data;
		size_t size, capacity;
		bool constant;

		// Construct with const data
		Buffer(const void* data, size_t size)
			: data((unsigned char*) data), size(size), capacity(size), constant(true)
		{}

		// Construct with non const data
		Buffer(void* data, size_t size)
			: data((unsigned char*) data), size(size), capacity(size), constant(false)
		{}

		// Construct from const array
		template <typename T, size_t N>
		Buffer(const T(&buffer)[N]) : Buffer(buffer, sizeof(buffer)) {}

		// Construct from non const array
		template <typename T, size_t N>
		Buffer(T(&buffer)[N]) : Buffer(buffer, sizeof(buffer)) {}

		// Buffer -> const void* overload
		constexpr operator const void* () const
		{
			return data;
		}

		// Buffer -> void* overload
		constexpr operator void* ()
		{
			return data;
		}

		// Interpret buffer as a list of 'T', and return const ref to element
		template <typename T>
		const T& get(int index) const
		{
			return reinterpret_cast<T*>(data)[index];
		}

		// Interpret buffer as a list of 'T', and return ref to element
		template <typename T>
		T& get(int index)
		{
			assert(!constant);
			assert(sizeof(T) * (index + 1) <= size);
			return reinterpret_cast<T*>(data)[index];
		}

		// Push a value to the buffer
		template <typename T>
		void push(const T& value)
		{
			assert(size + sizeof(value) <= capacity);
			memcpy(data + size, &value, sizeof(value));
			size += sizeof(value);
		}

		// Push values to the buffer
		template <typename T, typename ...Ts>
		void push(const T& value, const Ts& ... ts)
		{
			push(ts...);
		}

		// Pop a value at the top of the buffer and return it
		template <typename T>
		T pop()
		{
			auto value_ptr = reinterpret_cast<T*>(data + size - sizeof(T));
			size -= sizeof(T);
			return *value_ptr;
		}

		// Remove some bytes from the buffer
		void pop(size_t size)
		{
			this->size -= size;
		}

		// Resets the buffer
		void clear()
		{
			this->size = 0;
		}
	};

	template <typename T, size_t N>
	struct StaticBuffer : Buffer
	{
		T elements[N]{};

		StaticBuffer() : Buffer(elements, sizeof(elements)) {}
	};

	struct DynamicBuffer : Buffer
	{
		static constexpr size_t default_alloc_size = 64;

		Allocator* allocator;

		DynamicBuffer(Allocator* allocator) : Buffer(allocator->alloc(default_alloc_size), default_alloc_size), allocator(allocator)
		{
			clear();
		}

		// Push a value to the buffer
		template <typename T>
		void push(const T& value)
		{
			if(size + sizeof(value) > capacity)
			{
				capacity *= 2;

				this->data = (unsigned char*) allocator->resize(this->data, capacity);
			}

			Buffer::push(value);
		}
	};
}