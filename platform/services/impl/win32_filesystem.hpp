#pragma once

#include <platform/services/filesystem.hpp>

namespace lib
{
	class Win32FileSystem : public IFileSystem
	{
	public:

		virtual FileHandle open_file(const String& path, DataPermissionFlags flags = DataPermissionFlag::ReadWrite) override;

		virtual void close_file(FileHandle file_handle) override;

		virtual void read_file(FileHandle file, void* buffer, size_t size, size_t offset = 0) override;

		virtual void write_file(FileHandle file, const void* buffer, size_t size, size_t offset = 0) override;

		virtual void set_base_path(const String& path) override;

		virtual bool file_exists(const String& path) override;

		virtual size_t get_file_size(FileHandle file) override;

	private:

		String _base_path{};
	};
}