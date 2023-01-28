#include "win32_filesystem.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace lib
{
	FileHandle Win32FileSystem::open_file(const String& path, DataPermissionFlags flags)
	{
		auto create_file_flags = 0ul;

		if(flags & DataPermissionFlag::Readable)
			create_file_flags |= GENERIC_READ;

		if(flags & DataPermissionFlag::Writable)
			create_file_flags |= GENERIC_WRITE;

		return CreateFileA(
			(_base_path + path).c_str(),
			create_file_flags,
			0,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	}

	void Win32FileSystem::close_file(FileHandle file_handle)
	{
		if(!CloseHandle(file_handle))
		{
			throw std::runtime_error("Closed invalid file handle");
		}
	}

	void Win32FileSystem::read_file(FileHandle file, void* buffer, size_t size, size_t offset)
	{
		auto bytes_read = 0ul;

		if(SetFilePointer(file, offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			throw std::runtime_error("INVALID_SET_FILE_POINTER");
		}

		if(!ReadFile(file, buffer, size, &bytes_read, NULL))
		{
			throw std::runtime_error("Failed reading from file");
		}
	}

	void Win32FileSystem::write_file(FileHandle file, const void* buffer, size_t size, size_t offset)
	{
		auto bytes_written = 0ul;

		if(SetFilePointer(file, offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			throw std::runtime_error("INVALID_SET_FILE_POINTER");
		}

		if(!WriteFile(file, buffer, size, &bytes_written, NULL))
		{
			auto last_error = GetLastError();

			throw std::runtime_error("Failed writing to file");
		}
	}

	void Win32FileSystem::set_base_path(const String& path)
	{
		_base_path = path;
	}

	bool Win32FileSystem::file_exists(const String& path)
	{
		auto file_attributes = GetFileAttributesA((_base_path + path).c_str());

		return file_attributes != INVALID_FILE_ATTRIBUTES;
	}

	size_t Win32FileSystem::get_file_size(FileHandle file)
	{
		auto file_size_high = 0ul;
		auto file_size_low = GetFileSize(file, &file_size_high);
		return file_size_low;
	}
}