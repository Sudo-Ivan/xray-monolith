#pragma once

// Win32 windowing shim backed by SDL2.
// HWND carries an SDL_Window*, HMONITOR carries an SDL display index + 1.
// Implemented in src/xrEngine/win_sdl.cpp.

#if !defined(_WIN32) && !defined(_WIN64)

#include <SDL.h>

// window styles
#define WS_BORDER          0x00800000
#define WS_CAPTION         0x00C00000
#define WS_CHILD           0x40000000
#define WS_CLIPSIBLINGS    0x04000000
#define WS_DISABLED        0x08000000
#define WS_DLGFRAME        0x00400000
#define WS_HSCROLL         0x00100000
#define WS_ICONIC          0x20000000
#define WS_MAXIMIZE        0x01000000
#define WS_MAXIMIZEBOX     0x00010000
#define WS_MINIMIZE        0x20000000
#define WS_MINIMIZEBOX     0x00020000
#define WS_OVERLAPPED      0x00000000
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
#define WS_POPUP           0x80000000
#define WS_POPUPWINDOW     (WS_POPUP | WS_BORDER | WS_SYSMENU)
#define WS_SIZEBOX         0x00040000
#define WS_SYSMENU         0x00080000
#define WS_THICKFRAME      0x00040000
#define WS_TILED           WS_OVERLAPPED
#define WS_VISIBLE         0x10000000
#define WS_VSCROLL         0x00200000
#define WS_EX_TOPMOST      0x00000008

// window longs
#define GWL_STYLE          (-16)
#define GWL_EXSTYLE        (-20)
#define GWL_ID             (-12)
#define GWL_WNDPROC        (-4)

// SetWindowPos
#define HWND_TOP           ((HWND)0)
#define HWND_BOTTOM        ((HWND)1)
#define HWND_TOPMOST       ((HWND)-1)
#define HWND_NOTOPMOST     ((HWND)-2)
#define SWP_NOSIZE         0x0001
#define SWP_NOMOVE         0x0002
#define SWP_NOZORDER       0x0004
#define SWP_NOACTIVATE     0x0010
#define SWP_FRAMECHANGED   0x0020
#define SWP_SHOWWINDOW     0x0040
#define SWP_HIDEWINDOW     0x0080

// messages (subset used by the engine)
#define WM_NULL            0x0000
#define WM_CREATE          0x0001
#define WM_DESTROY         0x0002
#define WM_MOVE            0x0003
#define WM_SIZE            0x0005
#define WM_ACTIVATE        0x0006
#define WM_SETFOCUS        0x0007
#define WM_KILLFOCUS       0x0008
#define WM_ENABLE          0x000A
#define WM_SETREDRAW       0x000B
#define WM_SETTEXT         0x000C
#define WM_GETTEXT         0x000D
#define WM_GETTEXTLENGTH   0x000E
#define WM_PAINT           0x000F
#define WM_CLOSE           0x0010
#define WM_QUIT            0x0012
#define WM_ERASEBKGND      0x0014
#define WM_SYSCOLORCHANGE  0x0015
#define WM_SHOWWINDOW      0x0018
#define WM_ACTIVATEAPP     0x001C
#define WM_SETCURSOR       0x0020
#define WM_GETMINMAXINFO   0x0024
#define WM_DISPLAYCHANGE   0x007E
#define WM_NCCREATE        0x0081
#define WM_NCDESTROY       0x0082
#define WM_NCCALCSIZE      0x0083
#define WM_NCHITTEST       0x0084
#define WM_NCPAINT         0x0085
#define WM_NCACTIVATE      0x0086
#define WM_GETDLGCODE      0x0087
#define WM_KEYDOWN         0x0100
#define WM_KEYUP           0x0101
#define WM_CHAR            0x0102
#define WM_DEADCHAR        0x0103
#define WM_SYSKEYDOWN      0x0104
#define WM_SYSKEYUP        0x0105
#define WM_SYSCHAR         0x0106
#define WM_SYSDEADCHAR     0x0107
#define WM_UNICHAR         0x0109
#define WM_SYSCOMMAND      0x0112
#define WM_TIMER           0x0113
#define WM_HSCROLL         0x0114
#define WM_VSCROLL         0x0115
#define WM_INITMENU        0x0116
#define WM_INITMENUPOPUP   0x0117
#define WM_INPUT           0x00FF
#define WM_MOUSEFIRST      0x0200
#define WM_MOUSEMOVE       0x0200
#define WM_LBUTTONDOWN     0x0201
#define WM_LBUTTONUP       0x0202
#define WM_LBUTTONDBLCLK   0x0203
#define WM_RBUTTONDOWN     0x0204
#define WM_RBUTTONUP       0x0205
#define WM_RBUTTONDBLCLK   0x0206
#define WM_MBUTTONDOWN     0x0207
#define WM_MBUTTONUP       0x0208
#define WM_MBUTTONDBLCLK   0x0209
#define WM_MOUSEWHEEL      0x020A
#define WM_ENTERSIZEMOVE   0x0231
#define WM_EXITSIZEMOVE    0x0232
#define WM_HOTKEY          0x0312
#define WM_INPUTLANGCHANGE 0x0051

// syscmd
#define SC_SIZE            0xF000
#define SC_MOVE            0xF010
#define SC_MINIMIZE        0xF020
#define SC_MAXIMIZE        0xF030
#define SC_NEXTWINDOW      0xF040
#define SC_PREVWINDOW      0xF050
#define SC_CLOSE           0xF060
#define SC_VSCROLL         0xF070
#define SC_HSCROLL         0xF080
#define SC_MOUSEMENU       0xF090
#define SC_KEYMENU         0xF100
#define SC_ARRANGE         0xF110
#define SC_RESTORE         0xF120
#define SC_TASKLIST        0xF130
#define SC_SCREENSAVE      0xF140
#define SC_HOTKEY          0xF150
#define SC_MONITORPOWER    0xF170
#define SC_CONTEXTHELP     0xF180
#define SC_SEPARATOR       0xF00F

// window info / monitor info
struct WINDOWINFO
{
    u32 cbSize;
    RECT rcWindow;
    RECT rcClient;
    u32 dwStyle;
    u32 dwExStyle;
    u32 dwWindowStatus;
    UINT cxWindowBorders;
    UINT cyWindowBorders;
    u16 atomWindowType;
    u16 wCreatorVersion;
};

struct MONITORINFO
{
    u32 cbSize;
    RECT rcMonitor;
    RECT rcWork;
    u32 dwFlags;
};
typedef MONITORINFO* LPMONITORINFO;

#define MONITORINFOF_PRIMARY 0x00000001
#define CW_USEDEFAULT        ((int)0x80000000)
#define SM_SWAPBUTTON        23
#define SM_XVIRTUALSCREEN    76
#define SM_YVIRTUALSCREEN    77
#define SM_CXVIRTUALSCREEN   78
#define SM_CYVIRTUALSCREEN   79

// misc
#define IDC_ARROW          ((LPCSTR)0x7F00)
#define IDC_CROSS          ((LPCSTR)0x7F03)
#define IDC_IBEAM          ((LPCSTR)0x7F01)
#define IDC_WAIT           ((LPCSTR)0x7F02)
#define IDI_APPLICATION    ((LPCSTR)0x7F00)
#define IDI_ICON1          ((LPCSTR)0x7F00)
#define BLACK_BRUSH        4
#define GRAY_BRUSH         2
#define WHITE_BRUSH        0
#define MAKEINTRESOURCE(i) ((LPCSTR)(u32)(i))

// window class
typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);
struct WNDCLASS
{
    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    void* hIcon;
    void* hCursor;
    void* hbrBackground;
    LPCSTR lpszMenuName;
    LPCSTR lpszClassName;
};

// PeekMessage / MSG
struct tagMSG
{
    HWND hwnd;
    u32 message;
    WPARAM wParam;
    LPARAM lParam;
    u32 time;
    POINT pt;
};
typedef tagMSG MSG;
typedef MSG* LPMSG;
#define PM_REMOVE   0x0001
#define PM_NOREMOVE 0x0000

// virtual keys used
#define VK_SHIFT    0x10
#define VK_CONTROL  0x11
#define VK_MENU     0x12
#define VK_CAPITAL  0x14
#define VK_NUMLOCK  0x90

// API backed by SDL2 (implementation in src/xrEngine/win_sdl.cpp)
HWND xray_sdl_main_window();
bool GetClientRect(HWND, RECT*);
bool GetWindowRect(HWND, RECT*);
int MapWindowPoints(HWND from, HWND to, LPPOINT pts, UINT count);
int MapWindowPoints(HWND from, HWND to, RECT* rc, UINT count);
int ShowCursor(BOOL show);
bool ClipCursor(const RECT*);
bool GetCursorPos(LPPOINT);
bool SetCursorPos(int x, int y);
HWND SetForegroundWindow(HWND);
HWND SetActiveWindow(HWND);
HWND GetActiveWindow();
long GetWindowLong(HWND, int idx);
LONG_PTR SetWindowLongPtr(HWND, int idx, LONG_PTR v);
#define SetWindowLong SetWindowLongPtr
bool SetWindowPos(HWND, HWND, int x, int y, int w, int h, UINT flags);
bool MoveWindow(HWND, int x, int y, int w, int h, BOOL repaint);
bool GetWindowInfo(HWND, WINDOWINFO*);
bool IsIconic(HWND);
bool GetMonitorInfoA(HMONITOR, MONITORINFO*);
HMONITOR MonitorFromWindow(HWND, u32 flags);
int GetSystemMetrics(int idx);
int ShowWindow(HWND, int cmd);
bool DestroyWindow(HWND);
bool AdjustWindowRect(RECT*, u32 style, BOOL menu);
bool SetRect(RECT*, int l, int t, int r, int b);
bool ScreenToClient(HWND, LPPOINT);
bool ClientToScreen(HWND, LPPOINT);
short GetKeyState(int vk);
void* GetKeyboardLayout(u32);
void* ActivateKeyboardLayout(void* hkl, u32 flags);
HMODULE GetModuleHandleA(LPCSTR);
#define GetModuleHandle GetModuleHandleA
LRESULT SendMessage(HWND, u32 msg, WPARAM, LPARAM);
bool PostMessage(HWND, u32 msg, WPARAM, LPARAM);
void PostQuitMessage(int code);
bool PeekMessageA(MSG* msg, HWND, u32, u32, u32 remove);
#define PeekMessage PeekMessageA
LRESULT TranslateMessage(const MSG*);
LRESULT DispatchMessageA(const MSG*);
#define DispatchMessage DispatchMessageA
LRESULT DefWindowProcA(HWND, u32, WPARAM, LPARAM);
#define DefWindowProc DefWindowProcA
LRESULT CallWindowProcA(void* prev, HWND, u32, WPARAM, LPARAM);
#define CallWindowProc CallWindowProcA
HWND CreateWindowExA(u32 ex, LPCSTR cls, LPCSTR title, u32 style, int x, int y, int w, int h, HWND parent, void* menu, HINSTANCE, LPVOID);
#define CreateWindowEx CreateWindowExA
#define CreateWindowA(cls, title, style, x, y, w, h, parent, menu, inst, param) CreateWindowExA(0, cls, title, style, x, y, w, h, parent, menu, inst, param)
#define CreateWindow CreateWindowA
bool RegisterClassA(const WNDCLASS*);
#define RegisterClass RegisterClassA
void* GetStockObject(int);
void* LoadIconA(HINSTANCE, LPCSTR);
#define LoadIcon LoadIconA
void* LoadCursorA(HINSTANCE, LPCSTR);
#define LoadCursor LoadCursorA
void* SetCursor(void* c);
HWND SetCapture(HWND);
bool ReleaseCapture();
HWND GetCapture();
HWND GetFocus();
HWND SetFocus(HWND);
void* GetDC(HWND);
int ReleaseDC(HWND, void*);
u32 GetWindowThreadProcessId(HWND, u32*);
void UpdateWindow(HWND);
void ValidateRect(HWND, const RECT*);
bool BringWindowToTop(HWND);
bool SetWindowTextA(HWND, LPCSTR);
#define SetWindowText SetWindowTextA
bool RedrawWindow(HWND, const RECT*, void*, u32);
bool FlashWindow(HWND, BOOL invert);

#endif // !_WIN32

// additional compat decls for non-Windows builds
#if !defined(_WIN32) && !defined(_WIN64)

typedef void* HKL;
typedef u32 LCID;

#define HKL_NEXT        ((HKL)0x00000001)
#define HKL_PREV        ((HKL)-1)
#define SORT_DEFAULT    0x0
#define LANGIDFROMLCID(lcid) ((u16)(lcid))
#define PRIMARYLANGID(lgid) ((u16)(lgid) & 0x3ff)
#define MAKELANGID(p,s) ((((u16)(s)) << 10) | (u16)(p))
#define MAKELCID(lgid,srt) ((LCID)(((u32)((u16)(srt))) << 16) | ((u32)((u16)(lgid))))
#define LOCALE_RETURN_NUMBER            0x20000000
#define LOCALE_IDEFAULTANSICODEPAGE     0x1004
#define MB_PRECOMPOSED                  0x00000001
#define MONITOR_DEFAULTTONULL           0x00000000
#define MONITOR_DEFAULTTOPRIMARY        0x00000001
#define MONITOR_DEFAULTTONEAREST        0x00000002
#define E_NOTIMPL                       0x80004001L
#define CP_ACP                          0
#define CP_OEMCP                        1
#define CP_UTF8                         65001

HMONITOR MonitorFromPoint(POINT pt, u32 flags);
int GetLocaleInfoA(LCID lcid, u32 lctype, LPSTR buf, int bufsize);
void DisableProcessWindowsGhosting();

// accessibility structures
struct STICKYKEYS
{
    u32 cbSize;
    u32 dwFlags;
};
struct TOGGLEKEYS
{
    u32 cbSize;
    u32 dwFlags;
};
struct FILTERKEYS
{
    u32 cbSize;
    u32 dwFlags;
    u32 iWaitMSec;
    u32 iDelayMSec;
    u32 iRepeatMSec;
    u32 iBounceMSec;
};
#define SPI_GETSTICKYKEYS     0x003A
#define SPI_SETSTICKYKEYS     0x003B
#define SPI_GETFILTERKEYS     0x0032
#define SPI_SETFILTERKEYS     0x0033
#define SPI_GETTOGGLEKEYS     0x0034
#define SPI_SETTOGGLEKEYS     0x0035
#define SPI_GETSCREENSAVEACTIVE 0x0010
#define SPI_SETSCREENSAVEACTIVE 0x0011
#define SKF_STICKYKEYSON    0x00000001
#define SKF_AVAILABLE       0x00000002
#define SKF_HOTKEYACTIVE    0x00000004
#define SKF_CONFIRMHOTKEY   0x00000008
#define SKF_HOTKEYSOUND     0x00000010
#define SKF_INDICATOR       0x00000020
#define SKF_AUDIBLEFEEDBACK 0x00000040
#define TKF_TOGGLEKEYSON    0x00000001
#define TKF_AVAILABLE       0x00000002
#define TKF_HOTKEYACTIVE    0x00000004
#define TKF_CONFIRMHOTKEY   0x00000008
#define TKF_HOTKEYSOUND     0x00000010
#define TKF_INDICATOR       0x00000020
#define FKF_FILTERKEYSON    0x00000001
#define FKF_AVAILABLE       0x00000002
#define FKF_HOTKEYACTIVE    0x00000004
#define FKF_CONFIRMHOTKEY   0x00000008
#define FKF_HOTKEYSOUND     0x00000010
#define FKF_INDICATOR       0x00000020
#define WM_COMMAND          0x0111
#define LOWORD(l)           ((u16)((u32)(l) & 0xffff))
#define HIWORD(l)           ((u16)(((u32)(l) >> 16) & 0xffff))
#define LOBYTE(w)           ((u8)((w) & 0xff))
#define HIBYTE(w)           ((u8)(((w) >> 8) & 0xff))
#define MAKELONG(a,b)       ((long)(((u16)(a)) | (((u32)((u16)(b))) << 16)))
#define MAKELPARAM(l,h)     ((LPARAM)MAKELONG(l,h))
#define MAKEWPARAM(l,h)     ((WPARAM)MAKELONG(l,h))
#define MAKELCID(lgid,srt)  ((LCID)(((u32)((u16)(srt))) << 16) | ((u32)((u16)(lgid))))

bool SystemParametersInfoA(u32 action, u32 param, void* pvParam, u32 fWinIni);
#define SystemParametersInfo SystemParametersInfoA

#endif

// more windowing compat
#if !defined(_WIN32) && !defined(_WIN64)

#define WA_INACTIVE       0
#define WA_ACTIVE         1
#define WA_CLICKACTIVE    2
#define WS_ACTIVECAPTION  0x0001

#define ENUM_CURRENT_SETTINGS ((u32)-1)
#define ENUM_REGISTRY_SETTINGS ((u32)-2)
#define CDS_UPDATEREGISTRY  0x00000001
#define CDS_FULLSCREEN      0x00000004
#define DISP_CHANGE_SUCCESSFUL 0
#define DISP_CHANGE_FAILED  -1

struct DEVMODE
{
    char dmDeviceName[32];
    u16 dmSpecVersion;
    u16 dmDriverVersion;
    u16 dmSize;
    u16 dmDriverExtra;
    u32 dmFields;
    union {
        struct {
            s16 dmOrientation;
            s16 dmPaperSize;
            s16 dmPaperLength;
            s16 dmPaperWidth;
            s16 dmScale;
            s16 dmCopies;
            s16 dmDefaultSource;
            s16 dmPrintQuality;
        };
        struct {
            POINT dmPosition;
            u32 dmDisplayOrientation;
            u32 dmDisplayFixedOutput;
        };
    };
    s16 dmColor;
    s16 dmDuplex;
    s16 dmYResolution;
    s16 dmTTOption;
    s16 dmCollate;
    char dmFormName[32];
    u16 dmLogPixels;
    u32 dmBitsPerPel;
    u32 dmPelsWidth;
    u32 dmPelsHeight;
    union {
        u32 dmDisplayFlags;
        u32 dmNup;
    };
    u32 dmDisplayFrequency;
    u32 dmICMMethod;
    u32 dmICMIntent;
    u32 dmMediaType;
    u32 dmDitherType;
    u32 dmReserved1;
    u32 dmReserved2;
    u32 dmPanningWidth;
    u32 dmPanningHeight;
};

#define DM_BITSPERPEL     0x00040000
#define DM_PELSWIDTH      0x00080000
#define DM_PELSHEIGHT     0x00100000
#define DM_DISPLAYFLAGS   0x00200000
#define DM_DISPLAYFREQUENCY 0x00400000
#define DM_POSITION       0x00000020

HWND GetDesktopWindow();
bool EnumDisplaySettingsA(LPCSTR dev, u32 mode_num, DEVMODE* dm);
#define EnumDisplaySettings EnumDisplaySettingsA
long ChangeDisplaySettingsA(DEVMODE* dm, u32 flags);
#define ChangeDisplaySettings ChangeDisplaySettingsA
bool EnumDisplayDevicesA(LPCSTR dev, u32 num, void* dd, u32 flags);
#define EnumDisplayDevices EnumDisplayDevicesA
u32 timeGetTime();

#define IDCANCEL    2
#define IDOK        1
#define IDYES       6
#define IDNO        7
#define IDABORT     3
#define IDRETRY     4
#define IDIGNORE    5

#endif
