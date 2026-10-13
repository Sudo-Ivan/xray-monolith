#pragma once
// MSVC <direct.h> compat for POSIX systems.
#ifndef _WIN32
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#else
#include_next <direct.h>
#endif
