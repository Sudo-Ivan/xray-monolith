#pragma once
// <windows.h> compat shim for non-Windows builds.
// Win32 types, constants and shims come from the Common platform layer
// grafted from OpenXRay (see PORTING.md).
#ifndef _WIN32

#include "../Common/Platform.hpp"

// extra Win32 types not covered by PlatformLinux.inl
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
typedef unsigned char BYTE;
typedef BYTE* PBYTE;
typedef int* PINT;
typedef unsigned int* PUINT;
typedef void* LPVOID;
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
typedef int INT;
typedef int INT_PTR;
typedef unsigned int DWORD32;
typedef unsigned long long DWORD64;
typedef long long LONG64;
typedef unsigned long DWORDLONG;
typedef void VOID;

#ifndef DECLARE_HANDLE
#define DECLARE_HANDLE(name) typedef void* name
#endif

#endif // _WIN32
