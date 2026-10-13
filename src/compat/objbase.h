#pragma once
// COM base header compat: no COM on Linux
#ifndef _WIN32
typedef long HRESULT;

#ifndef XRAY_COMPAT_GUID_DEFINED
#define XRAY_COMPAT_GUID_DEFINED
struct GUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
};
typedef const GUID& REFGUID;
typedef const GUID& REFIID;
typedef const GUID& REFCLSID;
typedef GUID IID;
typedef GUID CLSID;
#endif

inline bool operator==(const GUID& a, const GUID& b)
{
    return memcmp(&a, &b, sizeof(GUID)) == 0;
}
inline bool operator!=(const GUID& a, const GUID& b)
{
    return !(a == b);
}

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    extern const GUID name
#define IsEqualGUID(a, b) ((a) == (b))

#define COINIT_MULTITHREADED 0
#define COINIT_APARTMENTTHREADED 0x2
#define CLSCTX_INPROC_SERVER 1
#define CLSCTX_ALL 23

inline HRESULT CoInitialize(void*) { return 0; }
inline HRESULT CoInitializeEx(void*, unsigned long) { return 0; }
inline void CoUninitialize() {}
inline HRESULT CoCreateInstance(const GUID&, void*, unsigned long, const GUID&, void**)
{
    return (HRESULT)0x80004005L;
}
#endif
