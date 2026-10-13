// Smoke test: links libxrcore.a, mounts a gamedata directory through
// LocatorAPI, and lists the files the VFS sees.
// Usage: xrcore_smoke [path-to-gamedata]
#include "stdafx.h"

int main(int argc, char** argv)
{
    const char* gamedata = argc > 1 ? argv[1] : "gamedata";

    fprintf(stderr, "[smoke] Core._initialize\n");
    Core._initialize("xrcore_smoke", nullptr, FALSE);
    fprintf(stderr, "[smoke] FS._initialize\n");

    FS._initialize(CLocatorAPI::flTargetFolderOnly, gamedata);
    fprintf(stderr, "[smoke] FS.file_list\n");

    FS_FileSet files;
    int n = FS.file_list(files, "$target_folder$", FS_ListFiles);
    fprintf(stderr, "[smoke] file_list -> %d\n", n);
    Msg("VFS mounted %s: %d files", gamedata, n);

    int shown = 0;
    for (FS_FileSetIt it = files.begin(); it != files.end() && shown < 10; ++it, ++shown)
        Msg("  %s (%ld bytes)", it->name.c_str(), it->size);

    FS._destroy();
    Core._destroy();
    return n > 0 ? 0 : 1;
}
