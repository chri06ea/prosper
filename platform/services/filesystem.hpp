#pragma once

#include <platform/core/defs.hpp>

namespace lib
{
	class IFileSystem
	{
	public:

		virtual FileHandle open_file(const String& path, DataPermissionFlags flags = DataPermissionFlag::ReadWrite) = 0;
		
		virtual void close_file(FileHandle file_handle) = 0;
		
		virtual void read_file(FileHandle file, void* buffer, size_t size, size_t offset = 0) = 0;
		
		virtual void write_file(FileHandle file, const void* buffer, size_t size, size_t offset = 0) = 0;
		
		virtual void set_base_path(const String& path) = 0;
		
		virtual bool file_exists(const String& path) = 0;
		
		virtual size_t get_file_size(FileHandle file) = 0;
	};
}