// MonitorList.cpp — monitor enumeration for the vid_monitor dropdown.
//
// Linux port: enumerates displays via SDL2 (SDL_GetNumVideoDisplays,
// SDL_GetDisplayBounds, SDL_GetDisplayName). HMONITOR carries
// (SDL display index + 1), 0/NULL means "Auto".

#include "stdafx.h"
#include "MonitorList.h"

#if !defined(_WIN32) && !defined(_WIN64)

ENGINE_API xr_token* vid_monitor_token = nullptr;
ENGINE_API xr_string vid_monitor_name = "Auto";
ENGINE_API volatile long g_monitor_list_dirty = 0;

static xr_vector<HMONITOR> s_monitor_handles;

void fill_vid_monitor_list()
{
    if (vid_monitor_token != nullptr)
        return;

    int n = SDL_GetNumVideoDisplays();
    if (n <= 0)
    {
        Msg("! vid_monitor: SDL reported no displays, list will be Auto-only");
        n = 0;
    }

    vid_monitor_token = xr_alloc<xr_token>(n + 2);

    vid_monitor_token[0].name = xr_strdup("Auto");
    vid_monitor_token[0].id = 0;
    s_monitor_handles.push_back(NULL);

    for (int i = 0; i < n; ++i)
    {
        xr_string label;
        const char* sdl_name = SDL_GetDisplayName(i);
        if (sdl_name && sdl_name[0])
        {
            label = sdl_name;
        }
        else
        {
            SDL_Rect b = {};
            SDL_GetDisplayBounds(i, &b);
            char buf[64];
            snprintf(buf, sizeof(buf), "Monitor %d (%dx%d)", i + 1, b.w, b.h);
            label = buf;
        }

        vid_monitor_token[i + 1].name = xr_strdup(label.c_str());
        vid_monitor_token[i + 1].id = i + 1;
        s_monitor_handles.push_back((HMONITOR)(uintptr_t)(i + 1));
    }

    vid_monitor_token[n + 1].name = nullptr;
    vid_monitor_token[n + 1].id = -1;

    Msg("* vid_monitor: enumerated %d monitor(s)", n);
}

void free_vid_monitor_list()
{
    if (!vid_monitor_token)
        return;

    for (int i = 0; vid_monitor_token[i].name; ++i)
        xr_free(vid_monitor_token[i].name);

    xr_free(vid_monitor_token);
    vid_monitor_token = nullptr;
    s_monitor_handles.clear();
}

HMONITOR ResolveSelectedMonitor()
{
    const xr_string& name = vid_monitor_name;

    if (name.empty() || name == "Auto")
        return NULL;

    if (!vid_monitor_token)
        return NULL;

    for (u32 i = 1; vid_monitor_token[i].name; ++i)
    {
        if (xr_strcmp(vid_monitor_token[i].name, name.c_str()) == 0)
        {
            if (i < static_cast<u32>(s_monitor_handles.size()))
                return s_monitor_handles[i];
        }
    }

    Msg("! vid_monitor: '%s' not found on this system, using Auto", name.c_str());
    return NULL;
}

void refresh_vid_monitor_list()
{
    Msg("* vid_monitor: live-refresh triggered");
    free_vid_monitor_list();
    fill_vid_monitor_list();
}

#endif // !_WIN32
