#pragma once
// COM base header compat: no COM on Linux
#ifndef _WIN32
typedef long HRESULT;
#define COINIT_MULTITHREADED 0
#define COINIT_APARTMENTTHREADED 0x2
inline HRESULT CoInitialize(void*) { return 0; }
inline HRESULT CoInitializeEx(void*, unsigned long) { return 0; }
inline void CoUninitialize() {}
#endif
