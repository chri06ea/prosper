#pragma once

#include <platform/core/defs.hpp>
#include <platform/services/filesystem.hpp>
#include <platform/models/image.hpp>

namespace lib
{
	Optional<Image> load_image(IFileSystem& filesystem, const String& path);
}