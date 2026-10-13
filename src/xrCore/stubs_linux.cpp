// Symbol stubs for code owned by other engine modules or by xrDebugNew.cpp
// (the Windows crash handler, which is not ported yet).
#include "stdafx.h"
#include "xrDebug.h"
#include "log.h"

// build info normally generated per executable
XRCORE_API u32 build_id = 0;
XRCORE_API LPCSTR build_date = __DATE__;

// modded exes info normally provided by the game binary
int get_modded_exes_version()
{
	return 0;
}

xr_string get_modded_exes_version_string()
{
	return xr_string("linux-port");
}

LPCSTR get_modded_exes_name()
{
	return "xray-monolith-linux";
}

// console markup check lives in xrEngine
enum Console_mark : int;
bool is_console_mark(Console_mark)
{
	return false;
}

// thread spawn hook from xrDebugNew.cpp
void debug_on_thread_spawn()
{
}

// std::string overloads that xrDebug.cpp does not provide
void xrDebug::fail(const char* e1, const std::string& e2, const char* file, int line, const char* function,
                   bool& ignore_always)
{
	backend("assertion failed", e1, e2.c_str(), nullptr, file, line, function, ignore_always);
}

void xrDebug::soft_fail(LPCSTR e1, const std::string& e2, LPCSTR file, int line, LPCSTR function)
{
	static bool ignore_always = false;
	backend("soft assert", e1, e2.c_str(), nullptr, file, line, function, ignore_always);
}
