#pragma once
// MSVC <mmsystem.h> compat: multimedia API not needed on Linux
#ifndef _WIN32
#include <ctime>
inline unsigned int timeGetTime() { return (unsigned int)(clock() * 1000 / CLOCKS_PER_SEC); }
#endif
