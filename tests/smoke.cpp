// Smoke test: links libxrcore.a and exercises basic runtime pieces.
// Exits 0 on success.
#include "stdafx.h"

int main()
{
    Memory._initialize(FALSE);

    xr_string s = "zone";
    Msg("smoke: %s len=%d", s.c_str(), (int)s.size());

    // exercise the win32->posix file layer
    DWORD attr = GetFileAttributes(".");
    Msg("attr('.')=0x%x", attr);

    string_path cwd;
    GetCurrentDirectory(sizeof(cwd), cwd);
    Msg("cwd=%s", cwd);

    Memory._destroy();
    return 0;
}
