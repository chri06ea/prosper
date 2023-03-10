#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <assert.h>
#include <optional>
#include <limits>


#include "memory.hpp"

#ifndef RELEASE
#include <iostream>
#define FAIL_WITH_ERROR(x)              \
{                                       \
 std::cout << x << std::endl;           \
 throw;									\
}            
#endif

#define CRITICAL_ERROR(x) FAIL_WITH_ERROR(x)

using std::nullopt;

template <typename T>
using Optional = std::optional<T>;

template <typename Fn>
using Function = std::function<Fn>;

template <typename T>
using List = std::vector<T>;

using FileHandle = void*;

enum DataPermissionFlag : size_t
{
	Readable = (1 << 1),
	Writable = (1 << 2),
	Executable = (1 << 3),

	ReadWrite = Readable | Writable,
};

using DataPermissionFlags = size_t;

struct Integer
{

};

using Entity = int;

using u64 = uint64_t;
using u32 = uint32_t;

using String = std::string;

template <typename T>
using Shared = std::shared_ptr<T>;

template<typename T, typename... Args>
inline Shared<T> make_shared(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#define DECLARE_ABSTRACT_FACTORY(x, ...)							\
class x##Factory													\
{																	\
public:																\
																	\
	virtual Shared<##x> create(__VA_ARGS__) = 0;					\
};																	\


enum class TypeId
{
	None,
	Float
};

static inline auto typeid_size = [](TypeId id)
{
	switch(id)
	{
		case TypeId::Float:
		{
			return sizeof(float);
		}
		default: CRITICAL_ERROR("Invalid element type");
	};
};

// Constexpr sqrt
//  https://stackoverflow.com/a/34134071

static inline constexpr double sqrt_newton_raphson(float x, float curr, float prev)
{
	return curr == prev
		? curr
		: sqrt_newton_raphson(x, 0.5f * (curr + x / curr), curr);
}

static inline constexpr double csqrt(float x)
{
	return x >= 0.f && x < std::numeric_limits<float>::infinity()
		? sqrt_newton_raphson(x, x, 0.f)
		: std::numeric_limits<float>::quiet_NaN();
}
