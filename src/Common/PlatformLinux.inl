#include <stdio.h>
#include <cstring>
#include <stdlib.h> // for malloc
#include <unistd.h> // for rmdir
#ifndef __HAIKU__
#include <sys/fcntl.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>
#include <limits.h> // for PAGESIZE...
#include <math.h>
#include <sched.h>

#include <algorithm> // for min max

#include <string>
#include <alloca.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h> // for mmap / munmap
#include <dlfcn.h>
#include <dirent.h>
#include <utime.h>
#include <ctime>

#define _LINUX // for GameSpy

#define MAX_PATH PATH_MAX + 1

#define WINAPI

#define _copysign copysign

#define _cdecl //__attribute__((cdecl))

#define __cdecl _cdecl
#define __stdcall
#define __fastcall

#define __pragma(...) _Pragma(#__VA_ARGS__)
#define CALLBACK

#define __except(X) catch(...)

#define GetCurrentProcessId getpid

inline void Sleep(int ms)
{
    usleep(ms * 1000);
}

inline void _splitpath(const char* path, // Path Input
        char* drive, // Drive     : Output
        char* dir, // Directory : Output
        char* fname, // Filename  : Output
        char* ext // Extension : Output
        )
{
    if(!path)
        return;

    const char *p, *end;

    if(drive)
        strcpy(drive, "");

    end = NULL;
    for(p = path; *p; p++)
        if(*p == '/' || *p == '\\')
            end = p + 1;

    if(end)
    {
        if(dir)
        {
            memcpy(dir, path, end - path);
            dir[end - path] = 0;
        }
        path = end;
    }
    else if(dir)
        dir[0] = 0;

    end = strchr(path, '.');

    if(!end)
        end = p;

    if(fname)
    {
        memcpy(fname, path, end - path);
        fname[end - path] = 0;
    }

    if(ext)
        strcpy(ext, end);
}

#include <iostream>
inline void OutputDebugString(const char *str) // for linux debugger
{
    std::cerr << str;
}

inline unsigned long GetLastError()
{
    return 0;
}

inline int GetExceptionCode()
{
    return 0;
}

inline void convert_path_separators(char * path);

#include <inttypes.h>
typedef int32_t BOOL;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;
#ifndef _LIBRAW_TYPES_H
typedef int64_t INT64;
typedef uint64_t UINT64;
#endif

typedef char* LPSTR;
typedef char* PSTR;
typedef char* LPTSTR;
typedef const char* LPCSTR;
typedef const char* LPCTSTR;
typedef unsigned int UINT;
typedef long long int LARGE_INTEGER;
typedef unsigned long long int ULARGE_INTEGER;

typedef struct _EXCEPTION_POINTERS {
} EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;

#if defined(XR_ARCHITECTURE_X64) || defined(XR_ARCHITECTURE_ARM64) || defined(XR_ARCHITECTURE_E2K)
typedef int64_t INT_PTR;
typedef uint64_t UINT_PTR;
typedef int64_t LONG_PTR;
#else
typedef int INT_PTR;
typedef unsigned int UINT_PTR;
typedef long LONG_PTR;
#endif

typedef void* HANDLE;
typedef void* HMODULE;
typedef void* PVOID;
typedef void* LPVOID;
#if defined(XR_ARCHITECTURE_PPC64)
typedef LONG_PTR WPARAM;
#else
typedef UINT_PTR WPARAM;
#endif
typedef LONG_PTR LPARAM;
typedef long HRESULT;
typedef long LRESULT;
#ifndef XRAY_COMPAT_GUID_DEFINED
#define XRAY_COMPAT_GUID_DEFINED
struct GUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
};
typedef const GUID& REFGUID;
typedef const GUID& REFIID;
typedef const GUID& REFCLSID;
typedef GUID IID;
typedef GUID CLSID;
#endif

typedef void* HWND;
typedef void* HDC;

typedef struct _RECT {
    long left;
    long top;
    long right;
    long bottom;
} RECT, *PRECT;

typedef struct tagPOINT {
    long x;
    long y;
} POINT, *PPOINT, *LPPOINT;

#define WM_USER 0x0400

#define TRUE true
#define FALSE false
#define NONE 0
#define CONST const

typedef dirent DirEntryType;

#define _O_WRONLY O_WRONLY
#define _O_RDONLY O_RDONLY
#define _O_TRUNC O_TRUNC
#define _O_CREAT O_CREAT
#define _S_IWRITE S_IWRITE
#define _S_IREAD S_IREAD
#define _O_BINARY 0
#define O_BINARY 0
#define O_SEQUENTIAL 0
#define SH_DENYWR 0

#if __has_include(<SDL_stdinc.h>)
#include <SDL_stdinc.h>
#define itoa SDL_itoa
#define _itoa_s SDL_itoa
#else
#define itoa(v, s, r) itoa_impl(v, s, r)
#define _itoa_s(v, s, sz, r) itoa_impl(v, s, r)

inline char* itoa_impl(int v, char* s, int radix)
{
    if (radix == 16)
        sprintf(s, "%x", v);
    else if (radix == 8)
        sprintf(s, "%o", v);
    else
        sprintf(s, "%d", v);
    return s;
}
#endif

#define _stricmp stricmp
#define strcmpi stricmp
#define lstrcpy strcpy
#define stricmp strcasecmp
#define strupr SDL_strupr
// error code numbers from original MS strcpy_s return value
inline int strcpy_s(char *dest, size_t num, const char *source)
{
    if(!dest)
        return EINVAL;

    if(0 == num)
    {
        dest[0] = '\0';
        return ERANGE;
    }

    if(!source)
    {
        dest[0] = '\0';
        return EINVAL;
    }

    size_t i;
    for(i = 0; i < num; i++)
    {
        if((dest[i] = source[i]) == '\0')
            return 0;
    }
    dest[0] = '\0';
    return ERANGE;
}

template <size_t num>
inline int strcpy_s(char (&dest)[num], const char *source) { return strcpy_s(dest, num, source); }

inline int strncpy_s(char * dest, size_t dst_size, const char * source, size_t num)
{
    if (!dest || (0 == dst_size))
        return EINVAL;

    if(0 == num)
    {
        dest[0] = '\0';
        return 0;
    }

    if (!source)
    {
        dest[0] = '\0';
        return EINVAL;
    }

    size_t i, end;
    if(num < dst_size)
        end = num;
    else
        end = dst_size - 1;

    for(i = 0; i < end && source[i]; i++)
        dest[i] = source[i];

    if(!source[i] || end == num)
    {
        dest[i] = '\0';
        return 0;
    }

    dest[0] = '\0';

    return EINVAL;
}

template <size_t dst_sz>
inline int strncpy_s(char (&dest)[dst_sz], const char * source, size_t num) { return strncpy_s(dest, dst_sz, source, num); }

inline int strcat_s(char * dest, size_t num, const char * source)
{
    if(!dest)
        return EINVAL;

    if(!source)
    {
        dest[0] = '\0';
        return EINVAL;
    }

    size_t i, j;
    for(i = 0; i < num; i++)
    {
        if(dest[i] == '\0')
        {
            for(j = 0; (j + i) < num; j++)
            {
                if((dest[j + i] = source[j]) == '\0')
                    return 0;
            }
        }
    }

    dest[0] = '\0';
    return ERANGE;
}

inline int strncat_s(char * dest, size_t num, const char * source, size_t count)
{
    if (!dest || !source)
        return EINVAL;

    size_t i, j;
    for(i = 0; i < num; i++)
    {
        if(dest[i] == '\0')
        {
            for(j = 0; (j + i) < num; j++)
            {
                if(j == count || (dest[j + i] = source[j]) == '\0')
                {
                    dest[j + i] = '\0';
                    return 0;
                }
            }
        }
    }

    return ERANGE;
}

#define _vsnprintf vsnprintf
inline int vsnprintf_s(char* buffer, size_t size, size_t, const char* format, va_list list)
{
    //TODO add bound check
    return vsnprintf(buffer, size, format, list);
}
#define vsprintf_s(dest, size, format, args) vsprintf(dest, format, args)
#define _snprintf snprintf
#define sprintf_s(buffer, buffer_size, stringbuffer, ...) sprintf(buffer, stringbuffer, ##__VA_ARGS__)
//#define GetProcAddress(handle, name) dlsym(handle, name)
#define _chdir chdir
#define _strnicmp strnicmp
#define strnicmp strncasecmp
#define _getcwd getcwd
#define _snwprintf swprintf
#define swprintf_s swprintf
#define wcsicmp _wcsicmp
#define _wcsicmp wcscmp
#define _tempnam tempnam
#define _access access
#define _open open
#define _close close
#define _sopen open
#define _utime utime
#define _utimbuf utimbuf
#define _sopen_s(handle, filename, ...) open(filename, O_RDONLY)
inline int _filelength(int fd)
{
    struct stat file_info;
    ::fstat(fd, &file_info);
    return file_info.st_size;
}
#define _fdopen fdopen
inline int _rmdir(const char *path)
{
    char* conv_fn = strdup(path);
    convert_path_separators(conv_fn);
    int result = rmdir(conv_fn);
    free(conv_fn);
    return result;
}
#define _write write
#define _strupr strupr
#define _read xr_read
#define _set_new_handler std::set_new_handler
#define _finite isfinite
inline int _mkdir(const char *dir) { return mkdir(dir, S_IRWXU); }

#define _wtoi(arg) wcstol(arg, NULL, 10)
#define _wtoi64(arg) wcstoll(arg, NULL, 10)
#undef min
#undef max
#define __max(a, b) std::max(a, b)
#define __min(a, b) std::min(a, b)

#define _locale_t locale_t
#define _isalpha_l isalpha_l
#define _create_locale(category, arg) newlocale(category, arg, (locale_t) 0)

#define ZeroMemory(p, sz) memset((p), 0, (sz))
#define CopyMemory(d, s, n) memcpy(d, s, n)

#define RGB(r,g,b) ( ((DWORD)(uint8_t)r)|((DWORD)((uint8_t)g)<<8)|((DWORD)((uint8_t)b)<<16) )
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define S_OK 0x00000000
#define S_FALSE 0x10000000
#define E_FAIL 0x80004005

#define _MAX_DRIVE	3
#define _MAX_DIR	256
#define _MAX_FNAME	256
#define _MAX_EXT	256

typedef DWORD           FOURCC;

typedef struct _AVIINDEXENTRY {
    DWORD	ckid;
    DWORD	dwFlags;
    DWORD	dwChunkOffset;
    DWORD	dwChunkLength;
} AVIINDEXENTRY;

typedef void *HIC;

inline BOOL SwitchToThread() { return (0 == sched_yield()); }

template <typename T>
decltype(auto) do_nothing(const T& obj)
{
    return obj;
}

// xr_strlwr collides with monolith's own xr_strlwr function in
// _std_extensions.h: the Compiler.inl macro renames it into an
// infinitely recursive strlwr. Provide real strlwr/strupr and keep
// xr_strlwr as a real function.
#undef xr_strlwr
#undef strupr
inline char* strlwr(char* s)
{
    for (char* p = s; *p; ++p)
        *p = (char)tolower((unsigned char)*p);
    return s;
}
inline char* strupr(char* s)
{
    for (char* p = s; *p; ++p)
        *p = (char)toupper((unsigned char)*p);
    return s;
}
#define xr_fs_strlwr(str) do_nothing(str)
#define xr_fs_nostrlwr(str) xr_strlwr(str)

/// For backward compability of FS, for real filesystem delimiter set to back
inline void restore_path_separators(char * path)
{
    while (char* sep = strchr(path, '/')) *sep = '\\'; //
}

inline void convert_path_separators(char * path)
{
    while (char* sep = strchr(path, '\\')) *sep = '/';
}

inline int xr_unlink(const char *path)
{
    char* conv_fn = strdup(path);
    convert_path_separators(conv_fn);
    int result = unlink(conv_fn);
    free(conv_fn);
    return result;
}

inline tm* localtime_safe(const time_t *time, struct tm* result){ return localtime_r(time, result); }

#define xr_strerror(errno, buffer, bufferSize) strerror_r(errno, buffer, sizeof(buffer))

using xrpid_t = pid_t;

// This is a drop-in replacement for calls to linux 'read' function. We use this because unlike other OSes
// Linux 'read' function can return less then the requested number of bytes and might need to be called multiple times.
// Apart from this, it behaves exactly as you would expect and matches the other OSes implementation.
// See also: https://www.man7.org/linux/man-pages/man2/read.2.html
inline ssize_t xr_read(int file_handle, void *buffer, size_t count)
{
    ssize_t total_r_bytes = 0;
    do
    {
        const ssize_t r_bytes =
            read(file_handle, reinterpret_cast<unsigned char*>(buffer) + total_r_bytes, count - total_r_bytes);

        // Check for error
        if (r_bytes == -1)
            return -1;

        // Check for EOF otherwise we would loop indefinitely
        if (r_bytes == 0)
            return total_r_bytes;

        total_r_bytes += r_bytes;
    } while (static_cast<size_t>(total_r_bytes) < count);

    return total_r_bytes;
}

// ============================================================
// xray-monolith additions (below the upstream OpenXRay content)
// ============================================================

#include <malloc.h>
typedef BOOL* LPBOOL;
typedef int* LPINT;

#include <iconv.h>
#include <unordered_map>

// Win32 typedefs monolith code expects
typedef int errno_t;
typedef int INT;
typedef unsigned char BYTE;
typedef char CHAR;
typedef unsigned char UCHAR;
typedef short SHORT;
typedef unsigned short USHORT;
typedef wchar_t WCHAR;
typedef char* LPSTR;
typedef const char* LPCSTR;
typedef WCHAR* LPWSTR;
typedef const WCHAR* LPCWSTR;
typedef unsigned long ULONG;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef ULONG* PULONG;
typedef BYTE* PBYTE;
typedef int* PINT;
typedef unsigned int* PUINT;
typedef const void* LPCVOID;
typedef uintptr_t DWORD_PTR;
typedef intptr_t LONG_PTR;
typedef uintptr_t ULONG_PTR;
typedef void* HINSTANCE;
typedef void* HGLOBAL;
typedef void* HKEY;
typedef void* HFONT;
typedef void* HMENU;
typedef void* HICON;
typedef void* HCURSOR;
typedef void* HBRUSH;
typedef void* HGDIOBJ;
typedef void* FARPROC;
typedef void* HANDLE32;
typedef unsigned long DWORDLONG;

// MSVC float classification -> C99 fpclassify
#define _FPCLASS_SNAN FP_SUBNORMAL
#define _FPCLASS_QNAN FP_NAN
#define _FPCLASS_NINF FP_INFINITE
#define _FPCLASS_PINF FP_INFINITE
#define _FPCLASS_ND FP_SUBNORMAL
#define _FPCLASS_PD FP_SUBNORMAL
#define _FPCLASS_NN FP_NAN
#define _FPCLASS_PN FP_NORMAL
#define _FPCLASS_NZ FP_ZERO
#define _FPCLASS_PZ FP_ZERO
#define _fpclass(x) fpclassify(x)
#define _isnan isnan

// MSVC safe-C errno style
#define _alloca alloca

// Code pages used by the engine (cyrillic game)
#define CP_ACP 0
#define CP_UTF8 65001
#define CP1251 1251

inline const char* xr_codepage_name(unsigned int cp)
{
    switch (cp)
    {
    case CP_UTF8: return "UTF-8";
    case 1251: return "CP1251";
    case CP_ACP:
    default: return "CP1251"; // Anomaly default ANSI page
    }
}

// wchar_t is UTF-32 on Linux
inline int MultiByteToWideChar(unsigned int cp, DWORD, LPCSTR src, int src_len,
                               LPWSTR dst, int dst_len)
{
    if (!src)
        return 0;
    if (src_len < 0)
        src_len = (int)strlen(src) + 1;

    iconv_t cd = iconv_open("UTF-32LE", xr_codepage_name(cp));
    if (cd == (iconv_t)-1)
        return 0;

    if (!dst || dst_len == 0)
    {
        // size query: convert into a throwaway buffer
        size_t in_left = (size_t)src_len;
        char* in_ptr = (char*)src;
        char tmp[4096];
        size_t written = 0;
        while (in_left > 0)
        {
            char* out = tmp;
            size_t out_left = sizeof(tmp);
            size_t r = iconv(cd, &in_ptr, &in_left, &out, &out_left);
            written += sizeof(tmp) - out_left;
            if (r == (size_t)-1 && errno != E2BIG)
                break;
        }
        iconv_close(cd);
        return (int)(written / sizeof(wchar_t));
    }

    size_t in_left = (size_t)src_len;
    size_t out_left = (size_t)dst_len * sizeof(wchar_t);
    char* in_ptr = (char*)src;
    char* out_ptr = (char*)dst;
    iconv(cd, &in_ptr, &in_left, &out_ptr, &out_left);
    iconv_close(cd);
    return (int)((dst_len * sizeof(wchar_t) - out_left) / sizeof(wchar_t));
}

inline int WideCharToMultiByte(unsigned int cp, DWORD, LPCWSTR src, int src_len,
                               LPSTR dst, int dst_len, LPCSTR, LPBOOL)
{
    if (!src)
        return 0;
    if (src_len < 0)
        src_len = (int)wcslen(src) + 1;

    iconv_t cd = iconv_open(xr_codepage_name(cp), "UTF-32LE");
    if (cd == (iconv_t)-1)
        return 0;

    if (!dst || dst_len == 0)
    {
        size_t in_left = (size_t)src_len * sizeof(wchar_t);
        char* in_ptr = (char*)src;
        char tmp[4096];
        size_t written = 0;
        while (in_left > 0)
        {
            char* out = tmp;
            size_t out_left = sizeof(tmp);
            size_t r = iconv(cd, &in_ptr, &in_left, &out, &out_left);
            written += sizeof(tmp) - out_left;
            if (r == (size_t)-1 && errno != E2BIG)
                break;
        }
        iconv_close(cd);
        return (int)written;
    }

    size_t in_left = (size_t)src_len * sizeof(wchar_t);
    size_t out_left = (size_t)dst_len;
    char* in_ptr = (char*)src;
    char* out_ptr = dst;
    iconv(cd, &in_ptr, &in_left, &out_ptr, &out_left);
    iconv_close(cd);
    return (int)(dst_len - out_left);
}

// MSVC CRT file enumeration
struct _finddata64i32_t
{
    unsigned int attrib;
    time_t time_create;
    time_t time_access;
    time_t time_write;
    uint64_t size;
    char name[260];
};

struct _finddata32_t
{
    unsigned int attrib;
    time_t time_create;
    time_t time_access;
    time_t time_write;
    uint32_t size;
    char name[260];
};

#define _finddata_t _finddata64i32_t

// MSVC file attribute bits
#define _A_NORMAL 0x00
#define _A_RDONLY 0x01
#define _A_HIDDEN 0x02
#define _A_SYSTEM 0x04
#define _A_SUBDIR 0x10
#define _A_ARCH 0x20

// SRWLOCK -> pthread_rwlock
typedef pthread_rwlock_t SRWLOCK;
typedef pthread_rwlock_t* PSRWLOCK;
#define InitializeSRWLock(p) pthread_rwlock_init((p), nullptr)
#define AcquireSRWLockExclusive(p) pthread_rwlock_wrlock(p)
#define AcquireSRWLockShared(p) pthread_rwlock_rdlock(p)
#define ReleaseSRWLockExclusive(p) pthread_rwlock_unlock(p)
#define ReleaseSRWLockShared(p) pthread_rwlock_unlock(p)

// CRITICAL_SECTION -> pthread_mutex
typedef pthread_mutex_t CRITICAL_SECTION;
typedef pthread_mutex_t* PCRITICAL_SECTION;
inline int xr_init_critical_section(pthread_mutex_t* m)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    int r = pthread_mutex_init(m, &attr);
    pthread_mutexattr_destroy(&attr);
    return r;
}
#define InitializeCriticalSection(p) xr_init_critical_section(p)
#define DeleteCriticalSection(p) pthread_mutex_destroy(p)
#define EnterCriticalSection(p) pthread_mutex_lock(p)
#define LeaveCriticalSection(p) pthread_mutex_unlock(p)
#define TryEnterCriticalSection(p) (pthread_mutex_trylock(p) == 0)

#define _strdup strdup
#define _alloca_s alloca
#define __debugbreak() __builtin_trap()
#define OutputDebugStringA(str) ((void)0)
#define OutputDebugString(str) ((void)0)
#define DebugBreak() __builtin_trap()

#define UNREFERENCED_PARAMETER(p) ((void)(p))

// more Win32 surface used by monolith
#define INVALID_HANDLE_VALUE ((HANDLE)(intptr_t)-1)
#define INFINITE 0xFFFFFFFF
#define WAIT_OBJECT_0 0
#define WAIT_TIMEOUT 0x102

// MSVC interlocked intrinsics -> GCC atomics (canonical defs live here)
#ifndef _InterlockedIncrement
#define _InterlockedIncrement(p) __sync_add_and_fetch((p), 1)
#define _InterlockedDecrement(p) __sync_sub_and_fetch((p), 1)
#define _InterlockedAdd(p, v) __sync_add_and_fetch((p), (v))
#define _InterlockedSub(p, v) __sync_sub_and_fetch((p), (v))
#define _InterlockedAnd(p, v) __sync_and_and_fetch((p), (v))
#define _InterlockedOr(p, v) __sync_or_and_fetch((p), (v))
#define _InterlockedXor(p, v) __sync_xor_and_fetch((p), (v))
#define _InterlockedCompareExchange(p, x, c) __sync_val_compare_and_swap((p), (c), (x))
#define _InterlockedCompareExchangePointer(p, x, c) __sync_val_compare_and_swap((p), (c), (x))
#define _InterlockedExchange(p, v) __sync_lock_test_and_set((p), (v))
#define _InterlockedExchangePointer(p, v) __sync_lock_test_and_set((p), (v))
#endif

// non-underscore Interlocked names
#define InterlockedIncrement _InterlockedIncrement
#define InterlockedDecrement _InterlockedDecrement
#define InterlockedAdd _InterlockedAdd
#define InterlockedSub _InterlockedSub
#define InterlockedExchange _InterlockedExchange
#define InterlockedExchangePointer _InterlockedExchangePointer
#define InterlockedCompareExchange _InterlockedCompareExchange

// QueryPerformanceCounter/Frequency -> clock_gettime
inline BOOL QueryPerformanceCounter(LARGE_INTEGER* p)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    *p = (LARGE_INTEGER)((uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec);
    return TRUE;
}

inline BOOL QueryPerformanceFrequency(LARGE_INTEGER* p)
{
    *p = (LARGE_INTEGER)1000000000ll;
    return TRUE;
}

// MSVC aligned allocation -> POSIX
inline void* _aligned_malloc(size_t size, size_t alignment)
{
    if (alignment < sizeof(void*))
        alignment = sizeof(void*);
    void* p = nullptr;
    if (posix_memalign(&p, alignment, size))
        return nullptr;
    return p;
}

inline void _aligned_free(void* p)
{
    free(p);
}

inline size_t _aligned_msize(void* p, size_t, size_t)
{
    return p ? malloc_usable_size(p) : 0;
}

inline void* _aligned_realloc(void* p, size_t size, size_t)
{
    // NOTE: loses alignment guarantee beyond _Alignof(max_align_t).
    // Sufficient for M1; revisit with a size table if anything breaks.
    return realloc(p, size);
}

#define _get_osfhandle(fd) (fd)
#define _close close
#define O_NOINHERIT 0
#define _SH_DENYWR 0
#define _SH_DENYNO 0


#define _strlwr strlwr
#define _tzset tzset
#define _strtime(x) _strtime_impl(x)
#define _strdate(x) _strdate_impl(x)
#define _strtoui64 strtoull
#define _i64toa(v, s, r) i64toa_impl(v, s, r)
#define _ui64toa_s(v, s, sz, r) i64toa_impl((long long)(v), s, r)
#define _ui64toa(v, s, r) i64toa_impl((long long)(v), s, r)

inline char* i64toa_impl(long long v, char* s, int radix)
{
    if (radix == 16)
        sprintf(s, "%llx", v);
    else if (radix == 8)
        sprintf(s, "%llo", v);
    else
        sprintf(s, "%lld", v);
    return s;
}

inline char* _strtime_impl(char* buf)
{
    time_t t = time(nullptr);
    strftime(buf, 9, "%H:%M:%S", localtime(&t));
    return buf;
}

inline char* _strdate_impl(char* buf)
{
    time_t t = time(nullptr);
    strftime(buf, 9, "%m/%d/%y", localtime(&t));
    return buf;
}

inline int _splitpath_s(const char* path, char* drive, size_t drive_sz,
                        char* dir, size_t dir_sz, char* fname, size_t fname_sz,
                        char* ext, size_t ext_sz)
{
    _splitpath(path, drive, dir, fname, ext);
    (void)drive_sz;
    (void)dir_sz;
    (void)fname_sz;
    (void)ext_sz;
    return 0;
}

#define TryAcquireSRWLockExclusive(p) (pthread_rwlock_trywrlock(p) == 0)
#define TryAcquireSRWLockShared(p) (pthread_rwlock_tryrdlock(p) == 0)

#define SleepEx(ms, alert) usleep((ms)*1000)

// VFS paths use backslash separators internally; translate to '/'
// at every real filesystem boundary.
inline const char* xr_conv_path(const char* path);

// ---- Win32 file API layer (fd-as-HANDLE model) ----
// HANDLE is an int fd cast through intptr_t. "Mapping handles" are the
// same fd; MapViewOfFile records each mapping length so UnmapViewOfFile
// can munmap correctly.

#define GENERIC_READ 0x1
#define GENERIC_WRITE 0x2
#define FILE_SHARE_READ 0x1
#define FILE_SHARE_WRITE 0x2
#define FILE_SHARE_DELETE 0x4
#define CREATE_NEW 1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define TRUNCATE_EXISTING 5
#define FILE_ATTRIBUTE_NORMAL 0x80
#define FILE_ATTRIBUTE_READONLY 0x01
#define FILE_ATTRIBUTE_DIRECTORY 0x10
#define FILE_ATTRIBUTE_ARCHIVE 0x20
#define FILE_ATTRIBUTE_HIDDEN 0x02
#define FILE_FLAG_RANDOM_ACCESS 0
#define FILE_FLAG_SEQUENTIAL_SCAN 0
#define PAGE_READONLY 0x02
#define PAGE_READWRITE 0x04
#define FILE_MAP_READ 0x04
#define FILE_MAP_WRITE 0x02
#define FILE_MAP_ALL_ACCESS FILE_MAP_WRITE
#define FILE_MAP_COPY 0x01
#define FILE_BEGIN SEEK_SET
#define FILE_CURRENT SEEK_CUR
#define FILE_END SEEK_END

inline int xr_win32_open_flags(unsigned long access, unsigned long create)
{
    int flags = 0;
    const unsigned long rw = access & (GENERIC_READ | GENERIC_WRITE);
    if (rw == (GENERIC_READ | GENERIC_WRITE))
        flags |= O_RDWR;
    else if (rw & GENERIC_WRITE)
        flags |= O_WRONLY;
    else
        flags |= O_RDONLY;

    switch (create)
    {
    case CREATE_NEW: flags |= O_CREAT | O_EXCL; break;
    case CREATE_ALWAYS: flags |= O_CREAT | O_TRUNC; break;
    case OPEN_ALWAYS: flags |= O_CREAT; break;
    case TRUNCATE_EXISTING: flags |= O_TRUNC; break;
    default: break;
    }
    return flags;
}

inline HANDLE CreateFile(LPCSTR path, unsigned long access, unsigned long,
                         void*, unsigned long create, unsigned long, void*)
{
    int fd = open(xr_conv_path(path), xr_win32_open_flags(access, create), 0644);
    return (HANDLE)(intptr_t)fd;
}

inline DWORD GetFileSize(HANDLE h, DWORD*)
{
    struct stat st;
    if (fstat((int)(intptr_t)h, &st))
        return (DWORD)-1;
    return (DWORD)st.st_size;
}

inline DWORD GetFileSize64(HANDLE h)
{
    struct stat st;
    if (fstat((int)(intptr_t)h, &st))
        return (DWORD)-1;
    return (DWORD)st.st_size;
}

inline HANDLE CreateFileMapping(HANDLE h, void*, unsigned long, DWORD, DWORD, LPCSTR)
{
    // the "mapping object" is just the fd itself
    return h;
}

inline BOOL CloseHandle(HANDLE h)
{
    return close((int)(intptr_t)h) == 0;
}

// map/unmap with a length registry so UnmapViewOfFile can munmap
inline std::unordered_map<void*, size_t>& xr_mmap_registry()
{
    static std::unordered_map<void*, size_t> reg;
    return reg;
}

inline pthread_mutex_t& xr_mmap_lock()
{
    static pthread_mutex_t reg_lock = PTHREAD_MUTEX_INITIALIZER;
    return reg_lock;
}

inline void* xr_mmap_register(void* p, size_t len)
{
    pthread_mutex_lock(&xr_mmap_lock());
    xr_mmap_registry()[p] = len;
    pthread_mutex_unlock(&xr_mmap_lock());
    return p;
}

inline void* MapViewOfFile(HANDLE h, unsigned long access, DWORD, DWORD offset, size_t len)
{
    int prot = PROT_READ;
    if (access & FILE_MAP_WRITE)
        prot |= PROT_WRITE;
    void* p = mmap(nullptr, len, prot, MAP_SHARED, (int)(intptr_t)h, offset);
    if (p == MAP_FAILED)
        return nullptr;
    return xr_mmap_register(p, len);
}

inline BOOL UnmapViewOfFile(void* p)
{
    pthread_mutex_lock(&xr_mmap_lock());
    auto& reg = xr_mmap_registry();
    auto it = reg.find(p);
    size_t len = (it != reg.end()) ? it->second : 0;
    if (it != reg.end())
        reg.erase(it);
    pthread_mutex_unlock(&xr_mmap_lock());
    if (!len)
        return FALSE;
    return munmap(p, len) == 0;
}

inline BOOL FlushViewOfFile(void* p, size_t len)
{
    return msync(p, len, MS_SYNC) == 0;
}

inline DWORD SetFilePointer(HANDLE h, long off, long*, DWORD method)
{
    return (DWORD)lseek((int)(intptr_t)h, off, (int)method);
}

inline BOOL ReadFile(HANDLE h, void* buf, DWORD len, DWORD* out, void*)
{
    ssize_t r = read((int)(intptr_t)h, buf, len);
    if (r < 0)
        return FALSE;
    if (out)
        *out = (DWORD)r;
    return TRUE;
}

inline BOOL WriteFile(HANDLE h, const void* buf, DWORD len, DWORD* out, void*)
{
    ssize_t w = write((int)(intptr_t)h, buf, len);
    if (w < 0)
        return FALSE;
    if (out)
        *out = (DWORD)w;
    return TRUE;
}

inline DWORD GetFileAttributes(LPCSTR path)
{
    struct stat st;
    if (stat(xr_conv_path(path), &st))
        return (DWORD)-1;
    DWORD attr = FILE_ATTRIBUTE_NORMAL;
    if (S_ISDIR(st.st_mode))
        attr |= FILE_ATTRIBUTE_DIRECTORY;
    if (!(st.st_mode & S_IWUSR))
        attr |= FILE_ATTRIBUTE_READONLY;
    if (path[0] == '.' || strstr(path, "/."))
        attr |= FILE_ATTRIBUTE_HIDDEN;
    return attr;
}

inline BOOL SetFileAttributes(LPCSTR path, DWORD attr)
{
    struct stat st;
    if (stat(xr_conv_path(path), &st))
        return FALSE;
    mode_t mode = st.st_mode;
    if (attr & FILE_ATTRIBUTE_READONLY)
        mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
    else
        mode |= S_IWUSR;
    return chmod(xr_conv_path(path), mode) == 0;
}

inline BOOL IsBadReadPtr(void*, size_t)
{
    return FALSE;
}

inline BOOL IsBadWritePtr(void*, size_t)
{
    return FALSE;
}

#define filelength _filelength
#define tell _filelengthi64

// correct 4-arg and MSVC 3-arg template overload of vsprintf_s
#undef vsprintf_s
inline int vsprintf_s(char* buf, size_t size, const char* fmt, va_list args)
{
    return vsnprintf(buf, size, fmt, args);
}
template <size_t N>
inline int vsprintf_s(char (&buf)[N], const char* fmt, va_list args)
{
    return vsnprintf(buf, N, fmt, args);
}

#define _msize malloc_usable_size
#define _recalloc(p, num, size) realloc((p), (num) * (size))

#define _expand(p, s) realloc((p), (s))
#define _atoi64(s) strtoll((s), nullptr, 10)
#define _i64toa_s(v, s, sz, r) i64toa_impl((long long)(v), s, r)
#define RaiseException(code, flags, n, args) ((void)0)
#define EXCEPTION_EXECUTE_HANDLER 1
#define EXCEPTION_CONTINUE_SEARCH 0
#define EXCEPTION_CONTINUE_EXECUTION (-1)
#define EXCEPTION_STACK_OVERFLOW 0xC00000FD
#define EXCEPTION_ACCESS_VIOLATION 0xC0000005
#define timeBeginPeriod(x) ((void)0)
#define timeEndPeriod(x) ((void)0)

// MSVC x87 FPU control -> raw control word manipulation
#define _PC_24 0x00000
#define _PC_53 0x10000
#define _PC_64 0x20000
#define _RC_NEAR 0x0000
#define _RC_DOWN 0x0100
#define _RC_UP 0x0200
#define _RC_CHOP 0x0300
#define MCW_PC 0x30000
#define MCW_RC 0x00300
#define MCW_EM 0x0003f
#define _MCW_PC MCW_PC
#define _MCW_RC MCW_RC
#define _MCW_EM MCW_EM

inline unsigned int _control87(unsigned int n, unsigned int mask)
{
    unsigned short cw;
    asm volatile("fstcw %0" : "=m"(cw));
    unsigned short newcw = cw;
    if (mask & MCW_PC)
    {
        static const unsigned short pcmap[4] = {0x000, 0x200, 0x300, 0};
        newcw = (newcw & ~0x300u) | pcmap[(n >> 16) & 3];
    }
    if (mask & MCW_RC)
    {
        static const unsigned short rcmap[4] = {0x000, 0x400, 0x800, 0xC00};
        newcw = (newcw & ~0xC00u) | rcmap[n & 3];
    }
    if (mask & MCW_EM)
        newcw |= 0x3f; // mask all FP exceptions
    if (newcw != cw)
        asm volatile("fldcw %0" :: "m"(newcw));
    return cw;
}

#define _clear87() asm volatile("fnclex")
#define _controlfp _control87

typedef LARGE_INTEGER* PLARGE_INTEGER;
#define DLL_PROCESS_ATTACH 1
#define DLL_THREAD_ATTACH 2
#define DLL_THREAD_DETACH 3
#define DLL_PROCESS_DETACH 0
#define INVALID_SET_FILE_POINTER (DWORD)-1
#define _stat stat

inline const char* xr_commandline()
{
    static char buf[4096] = {0};
    static bool loaded = false;
    if (!loaded)
    {
        loaded = true;
        FILE* f = fopen("/proc/self/cmdline", "r");
        if (f)
        {
            size_t n = fread(buf, 1, sizeof(buf) - 1, f);
            fclose(f);
            for (size_t i = 0; i + 1 < n; ++i)
                if (buf[i] == 0)
                    buf[i] = ' ';
        }
    }
    return buf;
}

#define GetCommandLine() xr_commandline()
#define GetCommandLineA() xr_commandline()

#include <thread>

// MSVC _beginthread -> detached std::thread (returns 0 like a failed handle
// would never be distinguished in this codebase anyway)
inline uintptr_t _beginthread(void (*func)(void*), unsigned, void* arg)
{
    std::thread([func, arg] { func(arg); }).detach();
    return 0;
}

#define _endthread() ((void)0)

inline DWORD GetUserName(LPSTR buf, DWORD* size)
{
    const char* u = getenv("USER");
    if (!u)
        u = "stalker";
    if ((DWORD)strlen(u) + 1 > *size)
        return FALSE;
    strcpy(buf, u);
    return TRUE;
}

inline DWORD GetComputerName(LPSTR buf, DWORD* size)
{
    char hn[256];
    if (gethostname(hn, sizeof(hn)))
        return FALSE;
    if ((DWORD)strlen(hn) + 1 > *size)
        return FALSE;
    strcpy(buf, hn);
    return TRUE;
}

inline DWORD GetCurrentDirectory(DWORD len, LPSTR buf)
{
    if (!getcwd(buf, len))
        return 0;
    return (DWORD)strlen(buf);
}

inline BOOL IsDebuggerPresent()
{
    return FALSE;
}

#define FILE_FLAG_NO_BUFFERING 0

// MSVC CRT file enumeration via glob()
#include <glob.h>

struct xr_find_state
{
    glob_t g;
    size_t idx;
};

inline intptr_t _findfirst(const char* pattern, _finddata_t* out)
{
    xr_find_state* st = new xr_find_state();
    st->idx = 0;
    char pat[4096];
    strncpy(pat, xr_conv_path(pattern), sizeof(pat) - 1);
    pat[sizeof(pat) - 1] = 0;
    size_t pl = strlen(pat);
    if (pl >= 3 && strcmp(pat + pl - 3, "*.*") == 0)
        pat[pl - 1] = 0; // turn trailing *.* into *
    if (glob(pat, GLOB_PERIOD, nullptr, &st->g) != 0 || st->g.gl_pathc == 0)
    {
        globfree(&st->g);
        delete st;
        return -1;
    }
    const char* path = st->g.gl_pathv[st->idx++];
    strncpy(out->name, path, sizeof(out->name) - 1);
    out->name[sizeof(out->name) - 1] = 0;
    struct stat fs;
    if (stat(path, &fs) == 0)
    {
        out->size = (uint64_t)fs.st_size;
        out->time_write = fs.st_mtime;
        out->time_create = fs.st_ctime;
        out->time_access = fs.st_atime;
        out->attrib = S_ISDIR(fs.st_mode) ? _A_SUBDIR : _A_NORMAL;
    }
    return (intptr_t)st;
}

inline int _findnext(intptr_t handle, _finddata_t* out)
{
    xr_find_state* st = (xr_find_state*)handle;
    if (st->idx >= st->g.gl_pathc)
        return -1;
    const char* path = st->g.gl_pathv[st->idx++];
    const char* base = strrchr(path, '/');
    base = base ? base + 1 : path;
    strncpy(out->name, base, sizeof(out->name) - 1);
    out->name[sizeof(out->name) - 1] = 0;
    struct stat fs;
    if (stat(path, &fs) == 0)
    {
        out->size = (uint64_t)fs.st_size;
        out->time_write = fs.st_mtime;
        out->time_create = fs.st_ctime;
        out->time_access = fs.st_atime;
        out->attrib = S_ISDIR(fs.st_mode) ? _A_SUBDIR : _A_NORMAL;
    }
    return 0;
}

inline int _findclose(intptr_t handle)
{
    xr_find_state* st = (xr_find_state*)handle;
    globfree(&st->g);
    delete st;
    return 0;
}

typedef time_t __time64_t;
inline __time64_t _time64(__time64_t* t)
{
    time_t r = time(nullptr);
    if (t)
        *t = r;
    return r;
}
inline struct tm* _localtime64(const __time64_t* t)
{
    static thread_local struct tm result;
    return localtime_r(t, &result);
}
#define GetProcessHeap() ((HANDLE)0)
inline BOOL CopyFile(LPCSTR src, LPCSTR dst, BOOL)
{
    int in = open(xr_conv_path(src), O_RDONLY);
    if (in < 0)
        return FALSE;
    int out = open(xr_conv_path(dst), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0)
    {
        close(in);
        return FALSE;
    }
    char buf[65536];
    ssize_t n;
    BOOL ok = TRUE;
    while ((n = read(in, buf, sizeof(buf))) > 0)
        if (write(out, buf, n) != n)
        {
            ok = FALSE;
            break;
        }
    close(in);
    close(out);
    return ok;
}

// VFS paths use backslash separators internally; translate to '/'
// at every real filesystem boundary.
inline const char* xr_conv_path(const char* path)
{
    if (!path)
        return path;
    static thread_local char buf[4096];
    strncpy(buf, path, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = 0;
    for (char* p = buf; *p; ++p)
        if (*p == '\\')
            *p = '/';
    return buf;
}

// Windows handle/type stragglers used by render-facing headers
typedef void* HMONITOR;
typedef void* HWND;
typedef void* HMENU;
typedef uint32_t UINT32;
typedef uint16_t UINT16;


#ifndef D3DCOLOR_XRGB
#define D3DCOLOR_XRGB(r, g, b) (0xff000000u | ((uint32_t)(r) << 16) | ((uint32_t)(g) << 8) | (uint32_t)(b))
#define D3DCOLOR_ARGB(a, r, g, b) (((uint32_t)(a) << 24) | ((uint32_t)(r) << 16) | ((uint32_t)(g) << 8) | (uint32_t)(b))
#endif

typedef unsigned char* LPBYTE;
typedef const unsigned char* LPCBYTE;

// MSVC floating-point classification
#define _FPCLASS_SNAN 0x0001
#define _FPCLASS_QNAN 0x0002
#define _FPCLASS_NINF 0x0004
#define _FPCLASS_NN   0x0008
#define _FPCLASS_ND   0x0010
#define _FPCLASS_NZ   0x0020
#define _FPCLASS_PZ   0x0040
#define _FPCLASS_PD   0x0080
#define _FPCLASS_PINF 0x0100
inline int xr_fpclass(double x)
{
    int c = fpclassify(x);
    switch (c)
    {
    case FP_NAN: return signbit(x) ? _FPCLASS_SNAN : _FPCLASS_QNAN;
    case FP_INFINITE: return signbit(x) ? _FPCLASS_NINF : _FPCLASS_PINF;
    case FP_ZERO: return signbit(x) ? _FPCLASS_NZ : _FPCLASS_PZ;
    case FP_SUBNORMAL: return signbit(x) ? _FPCLASS_ND : _FPCLASS_PD;
    default: return signbit(x) ? _FPCLASS_NN : _FPCLASS_PD;
    }
}
#define _fpclass xr_fpclass
#define _copysign copysign

#if !defined(_MSC_VER)
# ifndef __forceinline
#  define __forceinline inline __attribute__((always_inline))
# endif
#endif

inline unsigned long GetCurrentThreadId()
{
    return (unsigned long)pthread_self();
}

inline unsigned int GetTickCount()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned int)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

inline HMODULE LoadLibraryA(LPCSTR path)
{
    return dlopen(path, RTLD_NOW | RTLD_GLOBAL);
}
#define LoadLibrary LoadLibraryA
inline FARPROC GetProcAddress_compat(HMODULE mod, LPCSTR name)
{
    return (FARPROC)dlsym(mod, name);
}
#ifndef GetProcAddress
#define GetProcAddress GetProcAddress_compat
#endif
inline void FreeLibrary_compat(HMODULE mod)
{
    if (mod) dlclose(mod);
}
#ifndef FreeLibrary
#define FreeLibrary FreeLibrary_compat
#endif
class IUnknown
{
public:
    virtual ~IUnknown() {}
    virtual long QueryInterface(const GUID&, void**) { return 0; }
    virtual unsigned long AddRef() { return 1; }
    virtual unsigned long Release() { return 0; }
};

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

#ifndef _unlink
#define _unlink unlink
#endif

typedef void* HBITMAP;
typedef void* HPEN;
typedef void* HGDIOBJ;
typedef int HKL_unused_0;

#define EXCEPTION_EXECUTE_HANDLER     1
#define EXCEPTION_CONTINUE_SEARCH     0
#define EXCEPTION_CONTINUE_EXECUTION  (-1)
#define EXCEPTION_STACK_OVERFLOW      0xC00000FDL
#define EXCEPTION_ACCESS_VIOLATION    0xC0000005L
inline void _resetstkoflw() {}
inline void _declspec_dummy() {}
#define _declspec(x)
#define __declspec(x)
#define DLL_PROCESS_ATTACH 1
#define DLL_PROCESS_DETACH 0
#define DLL_THREAD_ATTACH  2
#define DLL_THREAD_DETACH  3
#define MB_OK             0x00000000
#define MB_OKCANCEL       0x00000001
#define MB_ICONEXCLAMATION 0x00000030
#define MB_ICONERROR      0x00000010
#define MB_YESNO          0x00000004

#ifndef GetModuleFileNameA
inline DWORD GetModuleFileNameA(HMODULE, LPSTR buf, DWORD bufsize)
{
    ssize_t n = readlink("/proc/self/exe", buf, bufsize - 1);
    if (n < 0)
        return 0;
    buf[n] = 0;
    return (DWORD)n;
}
#define GetModuleFileName GetModuleFileNameA
#endif

#define ERROR_ALREADY_EXISTS 183
inline void* CreateMutex(void*, BOOL, LPCSTR)
{
    return nullptr;
}
struct STARTUPINFO
{
    DWORD cb;
};
struct PROCESS_INFORMATION
{
    void* hProcess;
    void* hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
};
inline void* CreateDialog(void*, void*, void*, void*)
{
    return nullptr;
}
inline void* GetDlgItem(void*, int)
{
    return nullptr;
}
#define IDD_STARTUP 0
#define IDC_STATIC_LOGO 0
