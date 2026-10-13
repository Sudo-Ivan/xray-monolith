#pragma once
// MSVC <io.h> compat for POSIX systems.
// The low-level I/O functions it declares live in unistd.h/fcntl.h here.
#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
#else
#include_next <io.h>
#endif
