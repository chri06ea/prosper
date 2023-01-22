#include "filesystem_extensions.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <thirdparty/stb/stb_image.h>

namespace lib
{
	Optional<Image> load_image(IFileSystem& filesystem, const String& path)
	{
		Image image;
		//TODO: Use filesystem
		image.data = stbi_load(path.c_str(), &image.width, &image.height, &image.num_channels, 0);

		if(!image.data) return nullopt;
		
		return image;
	}
}