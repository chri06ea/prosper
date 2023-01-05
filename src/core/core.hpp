#pragma once

#include <functional>
#include <vector>
#include <algorithm>
#include <array>
#include <stdexcept>
#include <span>
#include <iostream>
#define LOG_INFORMATION(x) std::cout << x << std::endl;
#define CRITICAL_ERROR(msg) LOG_INFORMATION(msg)


#include "math.hpp"

#include "allocator.hpp"
#include "buffer.hpp"
