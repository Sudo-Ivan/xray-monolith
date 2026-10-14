#include "stdafx.h"
#pragma hdrstop


// #include "rt_lzo.h"
#include <lzo/lzo1x.h>


__declspec(thread) unsigned char rtc_wrkmem[LZO1X_1_MEM_COMPRESS] __attribute__((aligned(16)));


void rtc_initialize()
{
	VERIFY(lzo_init() == LZO_E_OK);
}

u32 rtc_csize(u32 in)
{
	VERIFY(in);
	return in + in / 64 + 16 + 3;
}

u32 rtc_compress(void* dst, u32 dst_len, const void* src, u32 src_len)
{
	lzo_uint out_size = dst_len;
	int r = lzo1x_1_compress(
		(const lzo_byte*)src, (lzo_uint)src_len,
		(lzo_byte*)dst, (lzo_uintp)&out_size,
		rtc_wrkmem);
	VERIFY(r == LZO_E_OK);
	return out_size;
}

u32 rtc_decompress(void* dst, u32 dst_len, const void* src, u32 src_len)
{
	lzo_uint out_size = dst_len;
	int r = lzo1x_decompress(
		(const lzo_byte*)src, (lzo_uint)src_len,
		(lzo_byte*)dst, (lzo_uintp)&out_size,
		rtc_wrkmem);
	VERIFY(r == LZO_E_OK);
	return out_size;
}
