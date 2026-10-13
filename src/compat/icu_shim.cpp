// ICU 65 entry points mapped onto the system ICU (renamed _78 symbols).
#include <cstdint>

struct UConverter;
struct UErrorCode_t;
typedef int32_t UErrorCode;
typedef uint16_t UChar;

extern "C" UConverter* ucnv_open_78(const char*, UErrorCode*);
extern "C" void ucnv_close_78(UConverter*);
extern "C" int32_t ucnv_toUChars_78(UConverter*, UChar*, int32_t, const char*, int32_t, UErrorCode*);
extern "C" int32_t ucnv_fromUChars_78(UConverter*, char*, int32_t, const UChar*, int32_t, UErrorCode*);

extern "C" UConverter* ucnv_open_65(const char* name, UErrorCode* err)
{
    return ucnv_open_78(name, err);
}

extern "C" void ucnv_close_65(UConverter* cnv)
{
    ucnv_close_78(cnv);
}

extern "C" int32_t ucnv_toUChars_65(UConverter* cnv, UChar* dest, int32_t destCap, const char* src, int32_t srcLen, UErrorCode* err)
{
    return ucnv_toUChars_78(cnv, dest, destCap, src, srcLen, err);
}

extern "C" int32_t ucnv_fromUChars_65(UConverter* cnv, char* dest, int32_t destCap, const UChar* src, int32_t srcLen, UErrorCode* err)
{
    return ucnv_fromUChars_78(cnv, dest, destCap, src, srcLen, err);
}
