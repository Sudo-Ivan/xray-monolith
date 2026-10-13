#include "stdafx.h"
#include "file_stream_reader.h"

#if !defined(_WIN32)
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#endif

void CFileStreamReader::construct(LPCSTR file_name, const u32& window_size)
{
#if defined(_WIN32)
	m_file_handle = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	VERIFY(m_file_handle != INVALID_HANDLE_VALUE);
	u32 file_size = (u32)GetFileSize(m_file_handle, NULL);

	HANDLE file_mapping_handle = CreateFileMapping(m_file_handle, 0, PAGE_READONLY, 0, 0, 0);
	VERIFY(file_mapping_handle != INVALID_HANDLE_VALUE);

	inherited::construct(file_mapping_handle, 0, file_size, file_size, window_size);
#else
	int fd = open(file_name, O_RDONLY);
	VERIFY(fd != -1);
	struct stat st;
	VERIFY(fstat(fd, &st) == 0);
	m_file_handle = (HANDLE)(intptr_t)fd;
	// the "mapping handle" is just the fd on POSIX
	inherited::construct(m_file_handle, 0, (u32)st.st_size, (u32)st.st_size, window_size);
#endif
}

void CFileStreamReader::destroy()
{
#if defined(_WIN32)
	HANDLE file_mapping_handle = this->file_mapping_handle();
	inherited::destroy();
	CloseHandle(file_mapping_handle);
	CloseHandle(m_file_handle);
#else
	inherited::destroy();
	::close((int)(intptr_t)m_file_handle);
#endif
}
