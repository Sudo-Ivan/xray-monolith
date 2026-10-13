// SDL2-backed Win32 windowing shims for the Linux port.
// HWND stores SDL_Window*. HMONITOR stores (SDL display index + 1).

#include "stdafx.h"

#if !defined(_WIN32) && !defined(_WIN64)

#include "win_sdl.h"
#include <locale.h>

static SDL_Window* g_main_window = nullptr;

void xray_sdl_set_main_window(SDL_Window* w)
{
    g_main_window = w;
}

HWND xray_sdl_main_window()
{
    return (HWND)g_main_window;
}

static SDL_Window* sdl_win(HWND h)
{
    return (SDL_Window*)h;
}

bool GetClientRect(HWND w, RECT* rc)
{
    if (!w || !rc)
        return false;
    int ww = 0, hh = 0;
    SDL_GetWindowSize(sdl_win(w), &ww, &hh);
    rc->left = 0;
    rc->top = 0;
    rc->right = ww;
    rc->bottom = hh;
    return true;
}

bool GetWindowRect(HWND w, RECT* rc)
{
    if (!w || !rc)
        return false;
    int x = 0, y = 0, ww = 0, hh = 0;
    SDL_GetWindowPosition(sdl_win(w), &x, &y);
    SDL_GetWindowSize(sdl_win(w), &ww, &hh);
    rc->left = x;
    rc->top = y;
    rc->right = x + ww;
    rc->bottom = y + hh;
    return true;
}

int MapWindowPoints(HWND from, HWND to, LPPOINT pts, UINT count)
{
    int fx = 0, fy = 0, tx = 0, ty = 0;
    if (from)
        SDL_GetWindowPosition(sdl_win(from), &fx, &fy);
    if (to)
        SDL_GetWindowPosition(sdl_win(to), &tx, &ty);
    for (UINT i = 0; i < count; ++i)
    {
        pts[i].x += fx - tx;
        pts[i].y += fy - ty;
    }
    return 0;
}

int MapWindowPoints(HWND from, HWND to, RECT* rc, UINT)
{
    POINT p[2] = {{rc->left, rc->top}, {rc->right, rc->bottom}};
    int r = MapWindowPoints(from, to, p, 2);
    rc->left = p[0].x;
    rc->top = p[0].y;
    rc->right = p[1].x;
    rc->bottom = p[1].y;
    return r;
}

int ShowCursor(BOOL show)
{
    static int counter = 0;
    counter += show ? 1 : -1;
    SDL_ShowCursor(counter >= 0 ? SDL_ENABLE : SDL_DISABLE);
    return counter;
}

bool ClipCursor(const RECT* rc)
{
    SDL_Window* w = g_main_window;
    if (!w)
        return false;
    if (rc)
    {
        SDL_SetWindowGrab(w, SDL_TRUE);
        SDL_Rect r = {rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top};
        // confine relative to window client area
        r.x = 0;
        r.y = 0;
        int ww = 0, hh = 0;
        SDL_GetWindowSize(w, &ww, &hh);
        r.w = ww;
        r.h = hh;
        SDL_SetWindowMouseRect(w, &r);
    }
    else
    {
        SDL_SetWindowMouseRect(w, nullptr);
        SDL_SetWindowGrab(w, SDL_FALSE);
    }
    return true;
}

bool GetCursorPos(LPPOINT p)
{
    if (!p)
        return false;
    int x = 0, y = 0;
    SDL_GetMouseState(&x, &y);
    int wx = 0, wy = 0;
    if (g_main_window)
        SDL_GetWindowPosition(g_main_window, &wx, &wy);
    p->x = x + wx;
    p->y = y + wy;
    return true;
}

bool SetCursorPos(int x, int y)
{
    if (!g_main_window)
        return false;
    int wx = 0, wy = 0;
    SDL_GetWindowPosition(g_main_window, &wx, &wy);
    SDL_WarpMouseInWindow(g_main_window, x - wx, y - wy);
    return true;
}

HWND SetForegroundWindow(HWND w)
{
    if (w)
        SDL_RaiseWindow(sdl_win(w));
    return w;
}

HWND SetActiveWindow(HWND w)
{
    if (w)
        SDL_RaiseWindow(sdl_win(w));
    return w;
}

HWND GetActiveWindow()
{
    return (HWND)g_main_window;
}

long GetWindowLong(HWND, int)
{
    return 0;
}

LONG_PTR SetWindowLongPtr(HWND, int, LONG_PTR v)
{
    return v;
}

bool SetWindowPos(HWND w, HWND, int x, int y, int ww, int hh, UINT flags)
{
    SDL_Window* win = sdl_win(w);
    if (!win)
        return false;
    if (!(flags & SWP_NOMOVE))
        SDL_SetWindowPosition(win, x, y);
    if (!(flags & SWP_NOSIZE))
        SDL_SetWindowSize(win, ww, hh);
    if (flags & SWP_SHOWWINDOW)
        SDL_ShowWindow(win);
    if (flags & SWP_HIDEWINDOW)
        SDL_HideWindow(win);
    return true;
}

bool MoveWindow(HWND w, int x, int y, int ww, int hh, BOOL)
{
    return SetWindowPos(w, HWND_TOP, x, y, ww, hh, 0);
}

bool GetWindowInfo(HWND w, WINDOWINFO* info)
{
    if (!w || !info)
        return false;
    info->cbSize = sizeof(WINDOWINFO);
    GetWindowRect(w, &info->rcWindow);
    GetClientRect(w, &info->rcClient);
    info->dwStyle = 0;
    info->dwExStyle = 0;
    return true;
}

bool IsIconic(HWND w)
{
    return w && (SDL_GetWindowFlags(sdl_win(w)) & SDL_WINDOW_MINIMIZED);
}

bool GetMonitorInfoA(HMONITOR mon, MONITORINFO* mi)
{
    if (!mi)
        return false;
    mi->cbSize = sizeof(MONITORINFO);
    SDL_Rect bounds;
    int idx = mon ? ((int)(uintptr_t)mon - 1) : 0;
    if (SDL_GetDisplayBounds(idx, &bounds) != 0)
        idx = 0;
    if (SDL_GetDisplayBounds(idx, &bounds) != 0)
        return false;
    mi->rcMonitor.left = bounds.x;
    mi->rcMonitor.top = bounds.y;
    mi->rcMonitor.right = bounds.x + bounds.w;
    mi->rcMonitor.bottom = bounds.y + bounds.h;
    mi->rcWork = mi->rcMonitor;
    mi->dwFlags = (idx == 0) ? MONITORINFOF_PRIMARY : 0;
    return true;
}

HMONITOR MonitorFromWindow(HWND w, u32)
{
    int idx = 0;
    if (w)
        idx = SDL_GetWindowDisplayIndex(sdl_win(w));
    if (idx < 0)
        idx = 0;
    return (HMONITOR)(uintptr_t)(idx + 1);
}

int GetSystemMetrics(int idx)
{
    switch (idx)
    {
    case SM_SWAPBUTTON:
        return 0;
    case SM_XVIRTUALSCREEN:
    case SM_YVIRTUALSCREEN:
        return 0;
    case SM_CXVIRTUALSCREEN:
    case SM_CYVIRTUALSCREEN:
    {
        SDL_Rect r;
        if (SDL_GetDisplayBounds(0, &r) == 0)
            return idx == SM_CXVIRTUALSCREEN ? r.w : r.h;
        return 0;
    }
    }
    return 0;
}

int ShowWindow(HWND w, int cmd)
{
    if (!w)
        return 0;
    switch (cmd)
    {
    case 0: SDL_HideWindow(sdl_win(w)); break;
    case 5: SDL_ShowWindow(sdl_win(w)); break;
    case 6: SDL_MinimizeWindow(sdl_win(w)); break;
    case 3: SDL_MaximizeWindow(sdl_win(w)); break;
    case 9: SDL_RestoreWindow(sdl_win(w)); break;
    default: SDL_ShowWindow(sdl_win(w)); break;
    }
    return 1;
}

bool DestroyWindow(HWND w)
{
    if (w)
        SDL_DestroyWindow(sdl_win(w));
    return true;
}

bool AdjustWindowRect(RECT*, u32, BOOL)
{
    return true;
}

bool SetRect(RECT* rc, int l, int t, int r, int b)
{
    if (!rc)
        return false;
    rc->left = l;
    rc->top = t;
    rc->right = r;
    rc->bottom = b;
    return true;
}

bool ScreenToClient(HWND w, LPPOINT p)
{
    if (!p)
        return false;
    int wx = 0, wy = 0;
    if (w)
        SDL_GetWindowPosition(sdl_win(w), &wx, &wy);
    p->x -= wx;
    p->y -= wy;
    return true;
}

bool ClientToScreen(HWND w, LPPOINT p)
{
    if (!p)
        return false;
    int wx = 0, wy = 0;
    if (w)
        SDL_GetWindowPosition(sdl_win(w), &wx, &wy);
    p->x += wx;
    p->y += wy;
    return true;
}

short GetKeyState(int vk)
{
    switch (vk)
    {
    case VK_CAPITAL:
        return (SDL_GetModState() & KMOD_CAPS) ? 1 : 0;
    case VK_NUMLOCK:
        return (SDL_GetModState() & KMOD_NUM) ? 1 : 0;
    case VK_SHIFT:
        return (SDL_GetModState() & KMOD_SHIFT) ? -32768 : 0;
    case VK_CONTROL:
        return (SDL_GetModState() & KMOD_CTRL) ? -32768 : 0;
    case VK_MENU:
        return (SDL_GetModState() & KMOD_ALT) ? -32768 : 0;
    }
    return 0;
}

static void* g_keyboard_layout = (void*)0x409;
void* GetKeyboardLayout(u32)
{
    return g_keyboard_layout;
}

void* ActivateKeyboardLayout(void* hkl, u32)
{
    void* prev = g_keyboard_layout;
    g_keyboard_layout = hkl;
    return prev;
}

HMODULE GetModuleHandleA(LPCSTR)
{
    return (HMODULE)(uintptr_t)1;
}

LRESULT SendMessage(HWND, u32, WPARAM, LPARAM)
{
    return 0;
}

bool PostMessage(HWND, u32, WPARAM, LPARAM)
{
    return false;
}

static bool g_quit = false;
void xray_sdl_request_quit()
{
    g_quit = true;
}

void PostQuitMessage(int)
{
    g_quit = true;
}

bool PeekMessageA(MSG* msg, HWND, u32, u32, u32 remove)
{
    if (msg)
        msg->message = g_quit ? WM_QUIT : 0;
    return g_quit;
}

LRESULT TranslateMessage(const MSG*)
{
    return 0;
}

LRESULT DispatchMessageA(const MSG*)
{
    return 0;
}

LRESULT DefWindowProcA(HWND, u32, WPARAM, LPARAM)
{
    return 0;
}

LRESULT CallWindowProcA(void*, HWND, u32, WPARAM, LPARAM)
{
    return 0;
}

HWND CreateWindowExA(u32, LPCSTR, LPCSTR title, u32 style, int x, int y, int w, int h, HWND, void*, HINSTANCE, LPVOID)
{
    u32 flags = SDL_WINDOW_SHOWN;
    if (style & WS_POPUP)
        flags |= SDL_WINDOW_BORDERLESS;
    if (x == (int)CW_USEDEFAULT)
        x = SDL_WINDOWPOS_UNDEFINED;
    if (y == (int)CW_USEDEFAULT)
        y = SDL_WINDOWPOS_UNDEFINED;
    SDL_Window* win = SDL_CreateWindow(title ? title : "X-Ray Engine", x, y, w, h, flags);
    if (win && !g_main_window)
        g_main_window = win;
    return (HWND)win;
}

bool RegisterClassA(const WNDCLASS*)
{
    return true;
}

void* GetStockObject(int)
{
    return nullptr;
}

void* LoadIconA(HINSTANCE, LPCSTR)
{
    return nullptr;
}

static void* g_arrow_cursor = nullptr;
void* LoadCursorA(HINSTANCE, LPCSTR)
{
    if (!g_arrow_cursor)
        g_arrow_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    return g_arrow_cursor;
}

void* SetCursor(void* c)
{
    SDL_SetCursor((SDL_Cursor*)c);
    return c;
}

HWND SetCapture(HWND w)
{
    return w;
}

bool ReleaseCapture()
{
    return true;
}

HWND GetCapture()
{
    return nullptr;
}

HWND GetFocus()
{
    return (HWND)g_main_window;
}

HWND SetFocus(HWND w)
{
    return w;
}

void* GetDC(HWND)
{
    return nullptr;
}

int ReleaseDC(HWND, void*)
{
    return 0;
}

u32 GetWindowThreadProcessId(HWND, u32*)
{
    return (u32)getpid();
}

void InvalidateRect(HWND, const RECT*, BOOL) {}
void UpdateWindow(HWND) {}
void ValidateRect(HWND, const RECT*) {}

bool BringWindowToTop(HWND w)
{
    if (w)
        SDL_RaiseWindow(sdl_win(w));
    return true;
}

bool SetWindowTextA(HWND w, LPCSTR text)
{
    if (w)
        SDL_SetWindowTitle(sdl_win(w), text);
    return true;
}

bool RedrawWindow(HWND, const RECT*, void*, u32)
{
    return true;
}

bool FlashWindow(HWND, BOOL)
{
    return true;
}

#endif // !_WIN32

HMONITOR MonitorFromPoint(POINT pt, u32 flags)
{
    int n = SDL_GetNumVideoDisplays();
    for (int i = 0; i < n; ++i)
    {
        SDL_Rect b;
        if (SDL_GetDisplayBounds(i, &b) == 0)
        {
            if (pt.x >= b.x && pt.x < b.x + b.w && pt.y >= b.y && pt.y < b.y + b.h)
                return (HMONITOR)(uintptr_t)(i + 1);
        }
    }
    if (flags == MONITOR_DEFAULTTONULL)
        return nullptr;
    return (HMONITOR)(uintptr_t)1;
}

int GetLocaleInfoA(LCID, u32, LPSTR buf, int bufsize)
{
    if (buf && bufsize >= 4)
    {
        // report US code page 1251 only when the host locale is Cyrillic;
        // engine uses this for ImGui font selection - keep it ASCII-safe
        const char* loc = setlocale(LC_ALL, nullptr);
        int cp = 1251;
        if (loc && (strstr(loc, "ru") || strstr(loc, "uk") || strstr(loc, "be")))
            cp = 1251;
        else
            cp = 1252;
        snprintf(buf, bufsize, "%d", cp);
        return (int)strlen(buf);
    }
    return 0;
}

void DisableProcessWindowsGhosting()
{
}

bool SystemParametersInfoA(u32, u32, void*, u32)
{
    return true;
}

HWND GetDesktopWindow()
{
    return nullptr;
}

bool EnumDisplaySettingsA(LPCSTR, u32 mode_num, DEVMODE* dm)
{
    if (!dm)
        return false;
    memset(dm, 0, sizeof(*dm));
    if (mode_num == ENUM_CURRENT_SETTINGS || mode_num == (u32)-1)
    {
        SDL_DisplayMode m;
        if (SDL_GetDesktopDisplayMode(0, &m) == 0)
        {
            dm->dmPelsWidth = m.w;
            dm->dmPelsHeight = m.h;
            dm->dmBitsPerPel = SDL_BITSPERPIXEL(m.format);
            dm->dmDisplayFrequency = (u32)m.refresh_rate;
            return true;
        }
    }
    // enumerated modes
    int n = SDL_GetNumDisplayModes(0);
    if (mode_num >= (u32)n)
        return false;
    SDL_DisplayMode m;
    if (SDL_GetDisplayMode(0, (int)mode_num, &m) == 0)
    {
        dm->dmPelsWidth = m.w;
        dm->dmPelsHeight = m.h;
        dm->dmBitsPerPel = SDL_BITSPERPIXEL(m.format);
        dm->dmDisplayFrequency = (u32)m.refresh_rate;
        return true;
    }
    return false;
}

long ChangeDisplaySettingsA(DEVMODE* dm, u32)
{
    if (!dm)
        return DISP_CHANGE_FAILED;
    SDL_DisplayMode target;
    target.format = SDL_PIXELFORMAT_RGB888;
    target.w = (int)dm->dmPelsWidth;
    target.h = (int)dm->dmPelsHeight;
    target.refresh_rate = (int)dm->dmDisplayFrequency;
    target.driverdata = nullptr;
    if (SDL_SetWindowDisplayMode((SDL_Window*)xray_sdl_main_window(), &target) != 0)
        return DISP_CHANGE_FAILED;
    return DISP_CHANGE_SUCCESSFUL;
}

bool EnumDisplayDevicesA(LPCSTR, u32, void*, u32)
{
    return false;
}

u32 timeGetTime()
{
    return SDL_GetTicks();
}
