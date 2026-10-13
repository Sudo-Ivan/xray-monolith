#pragma once
// Minimal CxImage/CxMemFile shim: MP screenshot JPEG encode is a no-op on Linux.
#include <cstdio>
#include <cstring>
#include <cstdlib>

typedef unsigned char BYTE;
#define CXIMAGE_FORMAT_JPG 3

class CxMemFile
{
public:
    CxMemFile(BYTE* buf, size_t capacity) : m_buf(buf), m_cap(capacity), m_pos(0) {}
    size_t Write(const void* data, size_t size)
    {
        size_t n = size < m_cap - m_pos ? size : m_cap - m_pos;
        memcpy(m_buf + m_pos, data, n);
        m_pos += n;
        return n;
    }
    long Tell() { return (long)m_pos; }
    bool Seek(long, int) { return false; }
private:
    BYTE* m_buf; size_t m_cap; size_t m_pos;
};

class CxImage
{
public:
    bool CreateFromArray(BYTE*, unsigned, unsigned, unsigned, unsigned, bool) { return true; }
    void SetJpegQuality(int) {}
    bool Encode(CxMemFile*, unsigned) { return false; }
    bool Decode(CxMemFile*, unsigned) { return false; }
};
