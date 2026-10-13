// SDL2-backed DirectInput shim for the Linux port.
//
// Implements the buffered-read semantics of IDirectInputDevice8 on top of
// SDL events. The device message pump (device.cpp) calls
// xray_input_feed_sdl_event() for every SDL_Event; keyboard and mouse
// events are translated to DIK scancodes and DIMOFS_* offsets and pushed
// onto per-device queues that GetDeviceData drains.

#include "stdafx.h"

#if !defined(_WIN32) && !defined(_WIN64)

#include <SDL.h>
#include <deque>
#include <mutex>

#include "xr_input.h"

// ---------------- scancode translation ----------------

static const u8 s_sdl_to_dik[SDL_NUM_SCANCODES] = {};

// Build the table once - SDL scancodes are USB-HID usages, DIK are PC/AT
// set-1 scancodes. The mapping below covers the keys the engine uses.
static const struct
{
    SDL_Scancode sdl;
    u8 dik;
} s_sdl_dik_map[] = {
    {SDL_SCANCODE_A, DIK_A},
    {SDL_SCANCODE_B, DIK_B},
    {SDL_SCANCODE_C, DIK_C},
    {SDL_SCANCODE_D, DIK_D},
    {SDL_SCANCODE_E, DIK_E},
    {SDL_SCANCODE_F, DIK_F},
    {SDL_SCANCODE_G, DIK_G},
    {SDL_SCANCODE_H, DIK_H},
    {SDL_SCANCODE_I, DIK_I},
    {SDL_SCANCODE_J, DIK_J},
    {SDL_SCANCODE_K, DIK_K},
    {SDL_SCANCODE_L, DIK_L},
    {SDL_SCANCODE_M, DIK_M},
    {SDL_SCANCODE_N, DIK_N},
    {SDL_SCANCODE_O, DIK_O},
    {SDL_SCANCODE_P, DIK_P},
    {SDL_SCANCODE_Q, DIK_Q},
    {SDL_SCANCODE_R, DIK_R},
    {SDL_SCANCODE_S, DIK_S},
    {SDL_SCANCODE_T, DIK_T},
    {SDL_SCANCODE_U, DIK_U},
    {SDL_SCANCODE_V, DIK_V},
    {SDL_SCANCODE_W, DIK_W},
    {SDL_SCANCODE_X, DIK_X},
    {SDL_SCANCODE_Y, DIK_Y},
    {SDL_SCANCODE_Z, DIK_Z},
    {SDL_SCANCODE_1, DIK_1},
    {SDL_SCANCODE_2, DIK_2},
    {SDL_SCANCODE_3, DIK_3},
    {SDL_SCANCODE_4, DIK_4},
    {SDL_SCANCODE_5, DIK_5},
    {SDL_SCANCODE_6, DIK_6},
    {SDL_SCANCODE_7, DIK_7},
    {SDL_SCANCODE_8, DIK_8},
    {SDL_SCANCODE_9, DIK_9},
    {SDL_SCANCODE_0, DIK_0},
    {SDL_SCANCODE_RETURN, DIK_RETURN},
    {SDL_SCANCODE_ESCAPE, DIK_ESCAPE},
    {SDL_SCANCODE_BACKSPACE, DIK_BACK},
    {SDL_SCANCODE_TAB, DIK_TAB},
    {SDL_SCANCODE_SPACE, DIK_SPACE},
    {SDL_SCANCODE_MINUS, DIK_MINUS},
    {SDL_SCANCODE_EQUALS, DIK_EQUALS},
    {SDL_SCANCODE_LEFTBRACKET, DIK_LBRACKET},
    {SDL_SCANCODE_RIGHTBRACKET, DIK_RBRACKET},
    {SDL_SCANCODE_BACKSLASH, DIK_BACKSLASH},
    {SDL_SCANCODE_NONUSHASH, DIK_BACKSLASH},
    {SDL_SCANCODE_SEMICOLON, DIK_SEMICOLON},
    {SDL_SCANCODE_APOSTROPHE, DIK_APOSTROPHE},
    {SDL_SCANCODE_GRAVE, DIK_GRAVE},
    {SDL_SCANCODE_COMMA, DIK_COMMA},
    {SDL_SCANCODE_PERIOD, DIK_PERIOD},
    {SDL_SCANCODE_SLASH, DIK_SLASH},
    {SDL_SCANCODE_CAPSLOCK, DIK_CAPITAL},
    {SDL_SCANCODE_F1, DIK_F1},
    {SDL_SCANCODE_F2, DIK_F2},
    {SDL_SCANCODE_F3, DIK_F3},
    {SDL_SCANCODE_F4, DIK_F4},
    {SDL_SCANCODE_F5, DIK_F5},
    {SDL_SCANCODE_F6, DIK_F6},
    {SDL_SCANCODE_F7, DIK_F7},
    {SDL_SCANCODE_F8, DIK_F8},
    {SDL_SCANCODE_F9, DIK_F9},
    {SDL_SCANCODE_F10, DIK_F10},
    {SDL_SCANCODE_F11, DIK_F11},
    {SDL_SCANCODE_F12, DIK_F12},
    {SDL_SCANCODE_PRINTSCREEN, DIK_SYSRQ},
    {SDL_SCANCODE_SCROLLLOCK, DIK_SCROLL},
    {SDL_SCANCODE_PAUSE, DIK_PAUSE},
    {SDL_SCANCODE_INSERT, DIK_INSERT},
    {SDL_SCANCODE_HOME, DIK_HOME},
    {SDL_SCANCODE_PAGEUP, DIK_PRIOR},
    {SDL_SCANCODE_DELETE, DIK_DELETE},
    {SDL_SCANCODE_END, DIK_END},
    {SDL_SCANCODE_PAGEDOWN, DIK_NEXT},
    {SDL_SCANCODE_RIGHT, DIK_RIGHT},
    {SDL_SCANCODE_LEFT, DIK_LEFT},
    {SDL_SCANCODE_DOWN, DIK_DOWN},
    {SDL_SCANCODE_UP, DIK_UP},
    {SDL_SCANCODE_NUMLOCKCLEAR, DIK_NUMLOCK},
    {SDL_SCANCODE_KP_DIVIDE, DIK_DIVIDE},
    {SDL_SCANCODE_KP_MULTIPLY, DIK_MULTIPLY},
    {SDL_SCANCODE_KP_MINUS, DIK_SUBTRACT},
    {SDL_SCANCODE_KP_PLUS, DIK_ADD},
    {SDL_SCANCODE_KP_ENTER, DIK_NUMPADENTER},
    {SDL_SCANCODE_KP_1, DIK_NUMPAD1},
    {SDL_SCANCODE_KP_2, DIK_NUMPAD2},
    {SDL_SCANCODE_KP_3, DIK_NUMPAD3},
    {SDL_SCANCODE_KP_4, DIK_NUMPAD4},
    {SDL_SCANCODE_KP_5, DIK_NUMPAD5},
    {SDL_SCANCODE_KP_6, DIK_NUMPAD6},
    {SDL_SCANCODE_KP_7, DIK_NUMPAD7},
    {SDL_SCANCODE_KP_8, DIK_NUMPAD8},
    {SDL_SCANCODE_KP_9, DIK_NUMPAD9},
    {SDL_SCANCODE_KP_0, DIK_NUMPAD0},
    {SDL_SCANCODE_KP_PERIOD, DIK_DECIMAL},
    {SDL_SCANCODE_KP_COMMA, DIK_NUMPADCOMMA},
    {SDL_SCANCODE_NONUSBACKSLASH, DIK_OEM_102},
    {SDL_SCANCODE_APPLICATION, DIK_APPS},
    {SDL_SCANCODE_POWER, DIK_POWER},
    {SDL_SCANCODE_KP_EQUALS, DIK_NUMPADEQUALS},
    {SDL_SCANCODE_F13, DIK_F13},
    {SDL_SCANCODE_F14, DIK_F14},
    {SDL_SCANCODE_F15, DIK_F15},
    {SDL_SCANCODE_MUTE, DIK_MUTE},
    {SDL_SCANCODE_VOLUMEDOWN, DIK_VOLUMEDOWN},
    {SDL_SCANCODE_VOLUMEUP, DIK_VOLUMEUP},
    {SDL_SCANCODE_LCTRL, DIK_LCONTROL},
    {SDL_SCANCODE_LSHIFT, DIK_LSHIFT},
    {SDL_SCANCODE_LALT, DIK_LMENU},
    {SDL_SCANCODE_LGUI, DIK_LWIN},
    {SDL_SCANCODE_RCTRL, DIK_RCONTROL},
    {SDL_SCANCODE_RSHIFT, DIK_RSHIFT},
    {SDL_SCANCODE_RALT, DIK_RMENU},
    {SDL_SCANCODE_RGUI, DIK_RWIN},
    {SDL_SCANCODE_AUDIONEXT, DIK_NEXTTRACK},
    {SDL_SCANCODE_AUDIOPREV, DIK_PREVTRACK},
    {SDL_SCANCODE_AUDIOSTOP, DIK_MEDIASTOP},
    {SDL_SCANCODE_AUDIOPLAY, DIK_PLAYPAUSE},
    {SDL_SCANCODE_MEDIASELECT, DIK_MEDIASELECT},
    {SDL_SCANCODE_CALCULATOR, DIK_CALCULATOR},
    {SDL_SCANCODE_MAIL, DIK_MAIL},
    {SDL_SCANCODE_AC_SEARCH, DIK_WEBSEARCH},
    {SDL_SCANCODE_AC_HOME, DIK_WEBHOME},
    {SDL_SCANCODE_AC_BACK, DIK_WEBBACK},
    {SDL_SCANCODE_AC_FORWARD, DIK_WEBFORWARD},
    {SDL_SCANCODE_AC_STOP, DIK_WEBSTOP},
    {SDL_SCANCODE_AC_REFRESH, DIK_WEBREFRESH},
    {SDL_SCANCODE_AC_BOOKMARKS, DIK_WEBFAVORITES},
};

static u8 sdl_scancode_to_dik(SDL_Scancode sc)
{
    for (size_t i = 0; i < sizeof(s_sdl_dik_map) / sizeof(s_sdl_dik_map[0]); ++i)
    {
        if (s_sdl_dik_map[i].sdl == sc)
            return s_sdl_dik_map[i].dik;
    }
    return 0;
}

// ---------------- event queues ----------------

struct SDiEventQueue
{
    std::deque<DIDEVICEOBJECTDATA> items;
    std::mutex lock;

    void push(u32 ofs, u32 data)
    {
        DIDEVICEOBJECTDATA od;
        od.dwOfs = ofs;
        od.dwData = data;
        od.dwTimeStamp = GetTickCount();
        od.dwSequence = 0;
        od.uAppData = 0;
        std::lock_guard<std::mutex> g(lock);
        if (items.size() > 4096)
            items.pop_front();
        items.push_back(od);
    }

    DWORD drain(DIDEVICEOBJECTDATA* out, DWORD max)
    {
        std::lock_guard<std::mutex> g(lock);
        DWORD n = 0;
        while (n < max && !items.empty())
        {
            out[n++] = items.front();
            items.pop_front();
        }
        return n;
    }
};

static SDiEventQueue g_kb_queue;
static SDiEventQueue g_mouse_queue;

// ---------------- device shim ----------------

class CSdlDiDevice final : public IDirectInputDevice8
{
public:
    explicit CSdlDiDevice(bool is_keyboard) : m_is_keyboard(is_keyboard), m_acquired(false) {}

    HRESULT SetDataFormat(const DIDATAFORMAT*) override
    {
        return DI_OK;
    }

    HRESULT SetCooperativeLevel(void*, u32) override
    {
        return DI_OK;
    }

    HRESULT SetProperty(const GUID*, const DIPROPHEADER*) override
    {
        return DI_OK;
    }

    HRESULT GetProperty(const GUID* prop, DIPROPHEADER* hdr) override
    {
        if (!hdr)
            return DIERR_INVALIDPARAM;
        // DIPROP_KEYNAME - return the SDL name for the key at dwOfs
        if (hdr->dwSize >= sizeof(DIPROPHEADER) + sizeof(wchar_t))
        {
            wchar_t* dest = (wchar_t*)((char*)hdr + sizeof(DIPROPHEADER));
            const u32 dik = hdr->dwObj;
            const SDL_Scancode sc = dik_to_sdl_scancode(dik);
            const char* name = sc ? SDL_GetScancodeName(sc) : "";
            if (sc)
            {
                size_t n = strlen(name);
                if (n > 255)
                    n = 255;
                for (size_t i = 0; i < n; ++i)
                    dest[i] = (wchar_t)(unsigned char)name[i];
                dest[n] = 0;
            }
            else
            {
                dest[0] = 0;
            }
        }
        return DI_OK;
    }

    HRESULT Acquire() override
    {
        m_acquired = true;
        return DI_OK;
    }

    HRESULT Unacquire() override
    {
        m_acquired = false;
        return DI_OK;
    }

    HRESULT GetDeviceData(u32, DIDEVICEOBJECTDATA* out, DWORD* count, u32) override
    {
        if (!count || !out)
            return DIERR_INVALIDPARAM;
        if (!m_acquired)
        {
            *count = 0;
            return DIERR_NOTACQUIRED;
        }
        SDiEventQueue& q = m_is_keyboard ? g_kb_queue : g_mouse_queue;
        *count = q.drain(out, *count);
        return DI_OK;
    }

    unsigned long Release() override { return 0; }

private:
    bool m_is_keyboard;
    bool m_acquired;

    static SDL_Scancode dik_to_sdl_scancode(u32 dik)
    {
        for (size_t i = 0; i < sizeof(s_sdl_dik_map) / sizeof(s_sdl_dik_map[0]); ++i)
        {
            if (s_sdl_dik_map[i].dik == dik)
                return s_sdl_dik_map[i].sdl;
        }
        return SDL_SCANCODE_UNKNOWN;
    }
};

class CSdlDirectInput final : public IDirectInput8
{
public:
    HRESULT CreateDevice(GUID guid, IDirectInputDevice8** device, void*)
    {
        *device = new CSdlDiDevice(&guid == &GUID_SysKeyboard);
        return DI_OK;
    }

    unsigned long Release() override { return 0; }
};

const GUID GUID_SysKeyboard = {0x6f1d2b61, 0xd5a0, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
const GUID GUID_SysMouse = {0x6f1d2b60, 0xd5a0, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
const GUID GUID_XAxis = {0xa36d02e0, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
const GUID GUID_YAxis = {0xa36d02e1, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
const GUID GUID_ZAxis = {0xa36d02e2, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};

DIDATAFORMAT c_dfDIKeyboard = {};
DIDATAFORMAT c_dfDIMouse2 = {};

extern "C" HRESULT DirectInput8Create(HINSTANCE, DWORD, const void*, void** out, void*)
{
    *out = new CSdlDirectInput();
    return DI_OK;
}

// ---------------- SDL event feed ----------------

extern void xray_input_feed_sdl_event(const SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
        const u8 dik = sdl_scancode_to_dik(e.key.keysym.scancode);
        if (dik == 0)
            break;
        g_kb_queue.push(dik, (e.type == SDL_KEYDOWN) ? 0x80 : 0);
        break;
    }
    case SDL_MOUSEMOTION:
        if (e.motion.xrel != 0)
            g_mouse_queue.push(DIMOFS_X, (u32)e.motion.xrel);
        if (e.motion.yrel != 0)
            g_mouse_queue.push(DIMOFS_Y, (u32)e.motion.yrel);
        break;
    case SDL_MOUSEWHEEL:
        if (e.wheel.y != 0)
            g_mouse_queue.push(DIMOFS_Z, (u32)e.wheel.y);
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
        u32 ofs = 0;
        switch (e.button.button)
        {
        case SDL_BUTTON_LEFT: ofs = DIMOFS_BUTTON0; break;
        case SDL_BUTTON_RIGHT: ofs = DIMOFS_BUTTON1; break;
        case SDL_BUTTON_MIDDLE: ofs = DIMOFS_BUTTON2; break;
        case SDL_BUTTON_X1: ofs = DIMOFS_BUTTON3; break;
        case SDL_BUTTON_X2: ofs = DIMOFS_BUTTON4; break;
        default: return;
        }
        g_mouse_queue.push(ofs, (e.type == SDL_MOUSEBUTTONDOWN) ? 0x80 : 0);
        break;
    }
    default:
        break;
    }
}

#endif // !_WIN32
