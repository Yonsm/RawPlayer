


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#pragma once
#include <StdLib.h>

#ifdef __cplusplus
extern "C"
{
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Platform specials.
#if (defined(_M_AMD64) && !defined(_M_X64))
#define _M_X64
#endif

#if defined(_M_IX86)
#define _MMX
#define _XMM
#define _SSE
#define _SSE2
#elif defined(_M_X64)
#define _SSE2
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Intrinsics header.
#ifdef _MMX
#include <MMIntrin.h>
typedef __m64 M64, *PM64;
#endif

#ifdef _SSE
#include <XMMIntrin.h>
typedef __m128 M128, *PM128;
#endif

#ifdef _SSE2
#include <EMMIntrin.h>
typedef __m128d M128D, *P128D;
typedef __m128i M128I, *PM128I;
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Portable type.
#ifndef VOID
#define VOID void
typedef void *PVOID;
typedef float FLOAT, *PFLOAT;
typedef double DOUBLE, *PDOUBLE;

typedef int INT, *PINT;
typedef char CHAR, *PCHAR, *PSTR;
typedef short SHORT, *PSHORT;
typedef long LONG, BOOL, *PLONG, *PBOOL;

typedef unsigned int UINT, *PUINT;
typedef unsigned char UCHAR, BYTE, *PUCHAR, *PBYTE;
typedef unsigned short USHORT, WORD, WCHAR, *PUSHORT, *PWORD, *PWCHAR, *PWSTR;
typedef unsigned long ULONG, DWORD, *PULONG, *PDWORD;
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPU feature.
#define	CPU_MMX			0x00800000
#define	CPU_SSE			0x02000000
#define	CPU_SSE2		0x04000000

#define	CPU_TSC			0x00000010
#define	CPU_CMOV		0x00008000
#define	CPU_FXSR		0x01000000
#define	CPU_HTT			0x10000000

#define CPU_MASK		(CPU_MMX | CPU_SSE | CPU_SSE2 | CPU_TSC | CPU_CMOV | CPU_FXSR | CPU_FXSR | CPU_HTT)

#define CPU_K3D			0x80000000
#define CPU_K3DX		0x40000000
#define CPU_KMMX		0x00400000
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize color space conversion. Return CPU feature.
DWORD InitCSConv();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Color space conversion function type.
typedef VOID (FLATTOFLAT)(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
						  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride, 
						  UINT uWidth, INT iHeight);

typedef VOID (FLATTOPACK)(PBYTE pbDstX, INT iDstXStride,
						  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride, 
						  UINT uWidth, INT iHeight);

typedef VOID (PACKTOFLAT)(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
						  PBYTE pbSrcX, INT iSrcXStride,
						  UINT uWidth, INT iHeight);

typedef VOID (PACKTOPACK)(PBYTE pbDstX, INT iDstXStride,
						  PBYTE pbSrcX, INT iSrcXStride,
						  UINT uWidth, INT iHeight);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// YV12 conversion.
FLATTOFLAT Yv12ToYv12;
FLATTOPACK Yv12ToYV12;
FLATTOPACK Yv12ToI420;
FLATTOPACK Yv12ToYuy2;
FLATTOPACK Yv12ToYvyu;
FLATTOPACK Yv12ToUyvy;
FLATTOPACK Yv12ToVyuy;
FLATTOPACK Yv12ToRgb32;
FLATTOPACK Yv12ToRgb24;
FLATTOPACK Yv12ToRgb16;
FLATTOPACK Yv12ToRgb15;

// YUY2 conversion.
PACKTOFLAT Yuy2ToYv12;
PACKTOPACK Yuy2ToYuy2;
PACKTOPACK Yuy2ToYvyu;
PACKTOPACK Yuy2ToUyvy;
PACKTOPACK Yuy2ToVyuy;
PACKTOPACK Yuy2ToRgb32;
PACKTOPACK Yuy2ToRgb24;
PACKTOPACK Yuy2ToRgb16;
PACKTOPACK Yuy2ToRgb15;

// YVYU conversion.
PACKTOFLAT YvyuToYv12;
PACKTOPACK YvyuToYuy2;
PACKTOPACK YvyuToYvyu;
PACKTOPACK YvyuToUyvy;
PACKTOPACK YvyuToVyuy;
PACKTOPACK YvyuToRgb32;
PACKTOPACK YvyuToRgb24;
PACKTOPACK YvyuToRgb16;
PACKTOPACK YvyuToRgb15;

// UYVY conversion.
PACKTOFLAT UyvyToYv12;
PACKTOPACK UyvyToYuy2;
PACKTOPACK UyvyToYvyu;
PACKTOPACK UyvyToUyvy;
PACKTOPACK UyvyToVyuy;
PACKTOPACK UyvyToRgb32;
PACKTOPACK UyvyToRgb24;
PACKTOPACK UyvyToRgb16;
PACKTOPACK UyvyToRgb15;

// VYUY conversion.
PACKTOFLAT VyuyToYv12;
PACKTOPACK VyuyToYuy2;
PACKTOPACK VyuyToYvyu;
PACKTOPACK VyuyToUyvy;
PACKTOPACK VyuyToVyuy;
PACKTOPACK VyuyToRgb32;
PACKTOPACK VyuyToRgb24;
PACKTOPACK VyuyToRgb16;
PACKTOPACK VyuyToRgb15;

// RGB32 conversion.
PACKTOFLAT Rgb32ToYv12;
PACKTOPACK Rgb32ToYuy2;
PACKTOPACK Rgb32ToYvyu;
PACKTOPACK Rgb32ToUyvy;
PACKTOPACK Rgb32ToVyuy;
PACKTOPACK Rgb32ToRgb32;
PACKTOPACK Rgb32ToRgb24;
PACKTOPACK Rgb32ToRgb16;
PACKTOPACK Rgb32ToRgb15;

// RGB24 conversion.
PACKTOFLAT Rgb24ToYv12;
PACKTOPACK Rgb24ToYuy2;
PACKTOPACK Rgb24ToYvyu;
PACKTOPACK Rgb24ToUyvy;
PACKTOPACK Rgb24ToVyuy;
PACKTOPACK Rgb24ToRgb32;
PACKTOPACK Rgb24ToRgb24;
PACKTOPACK Rgb24ToRgb16;
PACKTOPACK Rgb24ToRgb15;

// RGB16 conversion.
PACKTOFLAT Rgb16ToYv12;
PACKTOPACK Rgb16ToYuy2;
PACKTOPACK Rgb16ToYvyu;
PACKTOPACK Rgb16ToUyvy;
PACKTOPACK Rgb16ToVyuy;
PACKTOPACK Rgb16ToRgb32;
PACKTOPACK Rgb16ToRgb24;
PACKTOPACK Rgb16ToRgb16;
PACKTOPACK Rgb16ToRgb15;

// RGB15 conversion.
PACKTOFLAT Rgb15ToYv12;
PACKTOPACK Rgb15ToYuy2;
PACKTOPACK Rgb15ToYvyu;
PACKTOPACK Rgb15ToUyvy;
PACKTOPACK Rgb15ToVyuy;
PACKTOPACK Rgb15ToRgb32;
PACKTOPACK Rgb15ToRgb24;
PACKTOPACK Rgb15ToRgb16;
PACKTOPACK Rgb15ToRgb15;

// YV12 conversion with de-interlacing.
FLATTOFLAT Yv12ToYv12DI;
FLATTOPACK Yv12ToYV12DI;
FLATTOPACK Yv12ToI420DI;
FLATTOPACK Yv12ToYuy2DI;
FLATTOPACK Yv12ToYvyuDI;
FLATTOPACK Yv12ToUyvyDI;
FLATTOPACK Yv12ToVyuyDI;
FLATTOPACK Yv12ToRgb32DI;
FLATTOPACK Yv12ToRgb24DI;
FLATTOPACK Yv12ToRgb16DI;
FLATTOPACK Yv12ToRgb15DI;

// YUY2 conversion with de-interlacing.
PACKTOFLAT Yuy2ToYv12DI;
PACKTOPACK Yuy2ToYuy2DI;
PACKTOPACK Yuy2ToYvyuDI;
PACKTOPACK Yuy2ToUyvyDI;
PACKTOPACK Yuy2ToVyuyDI;
PACKTOPACK Yuy2ToRgb32DI;
PACKTOPACK Yuy2ToRgb24DI;
PACKTOPACK Yuy2ToRgb16DI;
PACKTOPACK Yuy2ToRgb15DI;

// UYVY conversion with de-interlacing.
PACKTOFLAT UyvyToYv12DI;
PACKTOPACK UyvyToYuy2DI;
PACKTOPACK UyvyToYvyuDI;
PACKTOPACK UyvyToUyvyDI;
PACKTOPACK UyvyToVyuyDI;
PACKTOPACK UyvyToRgb32DI;
PACKTOPACK UyvyToRgb24DI;
PACKTOPACK UyvyToRgb16DI;
PACKTOPACK UyvyToRgb15DI;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// YV12 conversion.
#ifdef _MMX
FLATTOFLAT Yv12ToYv12_mmx;
FLATTOPACK Yv12ToYV12_mmx;
FLATTOPACK Yv12ToI420_mmx;
FLATTOPACK Yv12ToYuy2_mmx;
FLATTOPACK Yv12ToYvyu_mmx;
FLATTOPACK Yv12ToUyvy_mmx;
FLATTOPACK Yv12ToVyuy_mmx;
FLATTOPACK Yv12ToRgb32_mmx;
FLATTOPACK Yv12ToRgb24_mmx;
FLATTOPACK Yv12ToRgb16_mmx;
FLATTOPACK Yv12ToRgb15_mmx;

// YUY2 conversion.
PACKTOFLAT Yuy2ToYv12_mmx;
PACKTOPACK Yuy2ToYuy2_mmx;
//PACKTOPACK Yuy2ToYvyu_mmx;
PACKTOPACK Yuy2ToUyvy_mmx;
//PACKTOPACK Yuy2ToVyuy_mmx;
PACKTOPACK Yuy2ToRgb32_mmx;
PACKTOPACK Yuy2ToRgb24_mmx;
//PACKTOPACK Yuy2ToRgb16_mmx;
//PACKTOPACK Yuy2ToRgb15_mmx;

// YVYU conversion.
//PACKTOFLAT YvyuToYv12_mmx;
//PACKTOPACK YvyuToYuy2_mmx;
//PACKTOPACK YvyuToYvyu_mmx;
//PACKTOPACK YvyuToUyvy_mmx;
//PACKTOPACK YvyuToVyuy_mmx;
//PACKTOPACK YvyuToRgb32_mmx;
//PACKTOPACK YvyuToRgb24_mmx;
//PACKTOPACK YvyuToRgb16_mmx;
//PACKTOPACK YvyuToRgb15_mmx;

// UYVY conversion.
PACKTOFLAT UyvyToYv12_mmx;
PACKTOPACK UyvyToYuy2_mmx;
//PACKTOPACK UyvyToYvyu_mmx;
PACKTOPACK UyvyToUyvy_mmx;
//PACKTOPACK UyvyToVyuy_mmx;
//PACKTOPACK UyvyToRgb32_mmx;
//PACKTOPACK UyvyToRgb24_mmx;
//PACKTOPACK UyvyToRgb16_mmx;
//PACKTOPACK UyvyToRgb15_mmx;

// VYUY conversion.
//PACKTOFLAT VyuyToYv12_mmx;
//PACKTOPACK VyuyToYuy2_mmx;
//PACKTOPACK VyuyToYvyu_mmx;
//PACKTOPACK VyuyToUyvy_mmx;
//PACKTOPACK VyuyToVyuy_mmx;
//PACKTOPACK VyuyToRgb32_mmx;
//PACKTOPACK VyuyToRgb24_mmx;
//PACKTOPACK VyuyToRgb16_mmx;
//PACKTOPACK VyuyToRgb15_mmx;

// RGB32 conversion.
PACKTOFLAT Rgb32ToYv12_mmx;
//PACKTOPACK Rgb32ToYuy2_mmx;
//PACKTOPACK Rgb32ToYvyu_mmx;
//PACKTOPACK Rgb32ToUyvy_mmx;
//PACKTOPACK Rgb32ToVyuy_mmx;
//PACKTOPACK Rgb32ToRgb32_mmx;
//PACKTOPACK Rgb32ToRgb24_mmx;
//PACKTOPACK Rgb32ToRgb16_mmx;
//PACKTOPACK Rgb32ToRgb15_mmx;

// RGB24 conversion.
PACKTOFLAT Rgb24ToYv12_mmx;
//PACKTOPACK Rgb24ToYuy2_mmx;
//PACKTOPACK Rgb24ToYvyu_mmx;
//PACKTOPACK Rgb24ToUyvy_mmx;
//PACKTOPACK Rgb24ToVyuy_mmx;
//PACKTOPACK Rgb24ToRgb32_mmx;
//PACKTOPACK Rgb24ToRgb24_mmx;
//PACKTOPACK Rgb24ToRgb16_mmx;
//PACKTOPACK Rgb24ToRgb15_mmx;

// RGB16 conversion.
//PACKTOFLAT Rgb16ToYv12_mmx;
//PACKTOPACK Rgb16ToYuy2_mmx;
//PACKTOPACK Rgb16ToYvyu_mmx;
//PACKTOPACK Rgb16ToUyvy_mmx;
//PACKTOPACK Rgb16ToVyuy_mmx;
//PACKTOPACK Rgb16ToRgb32_mmx;
//PACKTOPACK Rgb16ToRgb24_mmx;
//PACKTOPACK Rgb16ToRgb16_mmx;
//PACKTOPACK Rgb16ToRgb15_mmx;

// RGB15 conversion.
//PACKTOFLAT Rgb15ToYv12_mmx;
//PACKTOPACK Rgb15ToYuy2_mmx;
//PACKTOPACK Rgb15ToYvyu_mmx;
//PACKTOPACK Rgb15ToUyvy_mmx;
//PACKTOPACK Rgb15ToVyuy_mmx;
//PACKTOPACK Rgb15ToRgb32_mmx;
//PACKTOPACK Rgb15ToRgb24_mmx;
//PACKTOPACK Rgb15ToRgb16_mmx;
//PACKTOPACK Rgb15ToRgb15_mmx;

// YV12 conversion with de-interlacing.
FLATTOFLAT Yv12ToYv12DI_mmx;
FLATTOPACK Yv12ToYV12DI_mmx;
FLATTOPACK Yv12ToI420DI_mmx;
FLATTOPACK Yv12ToYuy2DI_mmx;
FLATTOPACK Yv12ToYvyuDI_mmx;
FLATTOPACK Yv12ToUyvyDI_mmx;
FLATTOPACK Yv12ToVyuyDI_mmx;
FLATTOPACK Yv12ToRgb32DI_mmx;
FLATTOPACK Yv12ToRgb24DI_mmx;
FLATTOPACK Yv12ToRgb16DI_mmx;
FLATTOPACK Yv12ToRgb15DI_mmx;

// YUY2 conversion with de-interlacing.
PACKTOFLAT Yuy2ToYv12DI_mmx;
PACKTOPACK Yuy2ToYuy2DI_mmx;
PACKTOPACK Yuy2ToYvyuDI_mmx;
PACKTOPACK Yuy2ToUyvyDI_mmx;
PACKTOPACK Yuy2ToVyuyDI_mmx;
PACKTOPACK Yuy2ToRgb32DI_mmx;
PACKTOPACK Yuy2ToRgb24DI_mmx;
PACKTOPACK Yuy2ToRgb16DI_mmx;
PACKTOPACK Yuy2ToRgb15DI_mmx;

// UYVY conversion with de-interlacing.
PACKTOFLAT UyvyToYv12DI_mmx;
PACKTOPACK UyvyToYuy2DI_mmx;
PACKTOPACK UyvyToYvyuDI_mmx;
PACKTOPACK UyvyToUyvyDI_mmx;
PACKTOPACK UyvyToVyuyDI_mmx;
PACKTOPACK UyvyToRgb32DI_mmx;
PACKTOPACK UyvyToRgb24DI_mmx;
PACKTOPACK UyvyToRgb16DI_mmx;
PACKTOPACK UyvyToRgb15DI_mmx;
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// YV12 conversion with de-interlacing.
#ifdef _XMM
FLATTOFLAT Yv12ToYv12DI_xmm;
FLATTOPACK Yv12ToYV12DI_xmm;
FLATTOPACK Yv12ToI420DI_xmm;
FLATTOPACK Yv12ToYuy2DI_xmm;
FLATTOPACK Yv12ToYvyuDI_xmm;
FLATTOPACK Yv12ToUyvyDI_xmm;
FLATTOPACK Yv12ToVyuyDI_xmm;
FLATTOPACK Yv12ToRgb32DI_xmm;
FLATTOPACK Yv12ToRgb24DI_xmm;
FLATTOPACK Yv12ToRgb16DI_xmm;
FLATTOPACK Yv12ToRgb15DI_xmm;

// YUY2 conversion with de-interlacing.
PACKTOFLAT Yuy2ToYv12DI_xmm;
PACKTOPACK Yuy2ToYuy2DI_xmm;
PACKTOPACK Yuy2ToYvyuDI_xmm;
PACKTOPACK Yuy2ToUyvyDI_xmm;
PACKTOPACK Yuy2ToVyuyDI_xmm;
PACKTOPACK Yuy2ToRgb32DI_xmm;
PACKTOPACK Yuy2ToRgb24DI_xmm;
PACKTOPACK Yuy2ToRgb16DI_xmm;
PACKTOPACK Yuy2ToRgb15DI_xmm;

// UYVY conversion with de-interlacing.
PACKTOFLAT UyvyToYv12DI_xmm;
PACKTOPACK UyvyToYuy2DI_xmm;
PACKTOPACK UyvyToYvyuDI_xmm;
PACKTOPACK UyvyToUyvyDI_xmm;
PACKTOPACK UyvyToVyuyDI_xmm;
PACKTOPACK UyvyToRgb32DI_xmm;
PACKTOPACK UyvyToRgb24DI_xmm;
PACKTOPACK UyvyToRgb16DI_xmm;
PACKTOPACK UyvyToRgb15DI_xmm;
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// YV12 conversion.
#ifdef _SSE2
FLATTOFLAT Yv12ToYv12_sse2;
FLATTOPACK Yv12ToYV12_sse2;
FLATTOPACK Yv12ToI420_sse2;
FLATTOPACK Yv12ToYuy2_sse2;
FLATTOPACK Yv12ToYvyu_sse2;
FLATTOPACK Yv12ToUyvy_sse2;
FLATTOPACK Yv12ToVyuy_sse2;
FLATTOPACK Yv12ToRgb32_sse2;
FLATTOPACK Yv12ToRgb24_sse2;
FLATTOPACK Yv12ToRgb16_sse2;
FLATTOPACK Yv12ToRgb15_sse2;

// YUY2 conversion.
PACKTOFLAT Yuy2ToYv12_sse2;
PACKTOPACK Yuy2ToYuy2_sse2;
//PACKTOPACK Yuy2ToYvyu_sse2;
PACKTOPACK Yuy2ToUyvy_sse2;
//PACKTOPACK Yuy2ToVyuy_sse2;
PACKTOPACK Yuy2ToRgb32_sse2;
PACKTOPACK Yuy2ToRgb24_sse2;
//PACKTOPACK Yuy2ToRgb16_sse2;
//PACKTOPACK Yuy2ToRgb15_sse2;

// YVYU conversion.
//PACKTOFLAT YvyuToYv12_sse2;
//PACKTOPACK YvyuToYuy2_sse2;
//PACKTOPACK YvyuToYvyu_sse2;
//PACKTOPACK YvyuToUyvy_sse2;
//PACKTOPACK YvyuToVyuy_sse2;
//PACKTOPACK YvyuToRgb32_sse2;
//PACKTOPACK YvyuToRgb24_sse2;
//PACKTOPACK YvyuToRgb16_sse2;
//PACKTOPACK YvyuToRgb15_sse2;

// UYVY conversion.
PACKTOFLAT UyvyToYv12_sse2;
PACKTOPACK UyvyToYuy2_sse2;
//PACKTOPACK UyvyToYvyu_sse2;
PACKTOPACK UyvyToUyvy_sse2;
//PACKTOPACK UyvyToVyuy_sse2;
//PACKTOPACK UyvyToRgb32_sse2;
//PACKTOPACK UyvyToRgb24_sse2;
//PACKTOPACK UyvyToRgb16_sse2;
//PACKTOPACK UyvyToRgb15_sse2;

// VYUY conversion.
//PACKTOFLAT VyuyToYv12_sse2;
//PACKTOPACK VyuyToYuy2_sse2;
//PACKTOPACK VyuyToYvyu_sse2;
//PACKTOPACK VyuyToUyvy_sse2;
//PACKTOPACK VyuyToVyuy_sse2;
//PACKTOPACK VyuyToRgb32_sse2;
//PACKTOPACK VyuyToRgb24_sse2;
//PACKTOPACK VyuyToRgb16_sse2;
//PACKTOPACK VyuyToRgb15_sse2;

// RGB32 conversion.
PACKTOFLAT Rgb32ToYv12_sse2;
//PACKTOPACK Rgb32ToYuy2_sse2;
//PACKTOPACK Rgb32ToYvyu_sse2;
//PACKTOPACK Rgb32ToUyvy_sse2;
//PACKTOPACK Rgb32ToVyuy_sse2;
//PACKTOPACK Rgb32ToRgb32_sse2;
//PACKTOPACK Rgb32ToRgb24_sse2;
//PACKTOPACK Rgb32ToRgb16_sse2;
//PACKTOPACK Rgb32ToRgb15_sse2;

// RGB24 conversion.
PACKTOFLAT Rgb24ToYv12_sse2;
//PACKTOPACK Rgb24ToYuy2_sse2;
//PACKTOPACK Rgb24ToYvyu_sse2;
//PACKTOPACK Rgb24ToUyvy_sse2;
//PACKTOPACK Rgb24ToVyuy_sse2;
//PACKTOPACK Rgb24ToRgb32_sse2;
//PACKTOPACK Rgb24ToRgb24_sse2;
//PACKTOPACK Rgb24ToRgb16_sse2;
//PACKTOPACK Rgb24ToRgb15_sse2;

// RGB16 conversion.
//PACKTOFLAT Rgb16ToYv12_sse2;
//PACKTOPACK Rgb16ToYuy2_sse2;
//PACKTOPACK Rgb16ToYvyu_sse2;
//PACKTOPACK Rgb16ToUyvy_sse2;
//PACKTOPACK Rgb16ToVyuy_sse2;
//PACKTOPACK Rgb16ToRgb32_sse2;
//PACKTOPACK Rgb16ToRgb24_sse2;
//PACKTOPACK Rgb16ToRgb16_sse2;
//PACKTOPACK Rgb16ToRgb15_sse2;

// RGB15 conversion.
//PACKTOFLAT Rgb15ToYv12_sse2;
//PACKTOPACK Rgb15ToYuy2_sse2;
//PACKTOPACK Rgb15ToYvyu_sse2;
//PACKTOPACK Rgb15ToUyvy_sse2;
//PACKTOPACK Rgb15ToVyuy_sse2;
//PACKTOPACK Rgb15ToRgb32_sse2;
//PACKTOPACK Rgb15ToRgb24_sse2;
//PACKTOPACK Rgb15ToRgb16_sse2;
//PACKTOPACK Rgb15ToRgb15_sse2;

// YV12 conversion with de-interlacing.
FLATTOFLAT Yv12ToYv12DI_sse2;
FLATTOPACK Yv12ToYV12DI_sse2;
FLATTOPACK Yv12ToI420DI_sse2;
FLATTOPACK Yv12ToYuy2DI_sse2;
FLATTOPACK Yv12ToYvyuDI_sse2;
FLATTOPACK Yv12ToUyvyDI_sse2;
FLATTOPACK Yv12ToVyuyDI_sse2;
FLATTOPACK Yv12ToRgb32DI_sse2;
FLATTOPACK Yv12ToRgb24DI_sse2;
FLATTOPACK Yv12ToRgb16DI_sse2;
FLATTOPACK Yv12ToRgb15DI_sse2;

// YUY2 conversion with de-interlacing.
PACKTOFLAT Yuy2ToYv12DI_sse2;
PACKTOPACK Yuy2ToYuy2DI_sse2;
PACKTOPACK Yuy2ToYvyuDI_sse2;
PACKTOPACK Yuy2ToUyvyDI_sse2;
PACKTOPACK Yuy2ToVyuyDI_sse2;
PACKTOPACK Yuy2ToRgb32DI_sse2;
PACKTOPACK Yuy2ToRgb24DI_sse2;
PACKTOPACK Yuy2ToRgb16DI_sse2;
PACKTOPACK Yuy2ToRgb15DI_sse2;

// UYVY conversion with de-interlacing.
PACKTOFLAT UyvyToYv12DI_sse2;
PACKTOPACK UyvyToYuy2DI_sse2;
PACKTOPACK UyvyToYvyuDI_sse2;
PACKTOPACK UyvyToUyvyDI_sse2;
PACKTOPACK UyvyToVyuyDI_sse2;
PACKTOPACK UyvyToRgb32DI_sse2;
PACKTOPACK UyvyToRgb24DI_sse2;
PACKTOPACK UyvyToRgb16DI_sse2;
PACKTOPACK UyvyToRgb15DI_sse2;
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macros.
#define _Clip(x)				(BYTE) (((x) < 0) ? 0 : (((x) > 255) ? 255 : (x)))

#ifdef _M_IX86
#define _NotImplement()			__asm int 3
#else
#define _NotImplement()
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// For YUV to RGB conversion.
#define Y_R_IN					0.257
#define Y_G_IN					0.504
#define Y_B_IN					0.098

#define U_R_IN					0.148
#define U_G_IN					0.291
#define U_B_IN					0.439

#define V_R_IN					0.439
#define V_G_IN					0.368
#define V_B_IN					0.071

#define Y_Y_OUT					1.164
#define B_U_OUT					2.018
#define G_U_OUT					0.391
#define G_V_OUT					0.813
#define R_V_OUT					1.596

#define Y_ADD_IN				16
#define U_ADD_IN				128
#define V_ADD_IN				128

#define Y_ADD_OUT				16
#define U_ADD_OUT				128
#define V_ADD_OUT				128

#define SCALEBITS				6
#define SCALEBITS_IN			8
#define SCALEBITS_OUT			13

#define _FixIn(x)				((USHORT) ((x) * (1L << SCALEBITS_IN) + 0.5))
#define _FixOut(x)				((USHORT) ((x) * (1L << SCALEBITS_OUT) + 0.5))

#define _GetRgb15B(x)			(((x) << 3) & 0xF8)
#define _GetRgb15G(x)			(((x) >> 2) & 0xF8)
#define _GetRgb15R(x)			(((x) >> 7) & 0xF8)
#define _GetRgb16B(x)			(((x) << 3) & 0xF8)
#define _GetRgb16G(x)			(((x) >> 3) & 0xFC)
#define _GetRgb16R(x)			(((x) >> 8) & 0xF8)

#define _MakeRgb15(r, g, b)		((((r) << 7) & 0x7C00) | (((g) << 2) & 0x03E0) | (((b) >> 3) & 0x001F))
#define _MakeRgb16(r, g, b)		((((r) << 8) & 0xF800) | (((g) << 3) & 0x07E0) | (((b) >> 3) & 0x001F))

#define _MakeY(r, g, b)			((BYTE) ((_FixIn(Y_R_IN) * (r) + _FixIn(Y_G_IN) * (g) + _FixIn(Y_B_IN) * (b)) >> SCALEBITS_IN) + Y_ADD_IN)
#define _MakeU(r, g, b, n)		((BYTE) ((_FixIn(U_B_IN) * (b) - _FixIn(U_G_IN) * (g) - _FixIn(U_R_IN) * (r)) >> (SCALEBITS_IN + n)) + U_ADD_IN)
#define _MakeV(r, g, b, n)		((BYTE) ((_FixIn(V_R_IN) * (r) - _FixIn(V_G_IN) * (g) - _FixIn(V_B_IN) * (b)) >> (SCALEBITS_IN + n)) + V_ADD_IN)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Global variable.
extern INT g_iYYTab[256];
extern INT g_iBUTab[256];
extern INT g_iGUTab[256];
extern INT g_iGVTab[256];
extern INT g_iRVTab[256];

#ifdef _MMX
extern M64 g_mSub10;
extern M64 g_mSub80;
extern M64 g_mYYMul;
extern M64 g_mUGMul;
extern M64 g_mVGMul;
extern M64 g_mUBMul;
extern M64 g_mVRMul;
extern M64 g_mWord0001;
extern M64 g_mWord00FF;
extern M64 g_mWordFF00;

extern M64 g_mMulToY;
extern M64 g_mMulToU;
extern M64 g_mMulToV;
extern M64 g_mMask24;

extern M64 g_mMaskAvg;
#endif

#ifdef _SSE2
extern M128I g_xiSub10;
extern M128I g_xiSub80;
extern M128I g_xiYYMul;
extern M128I g_xiUGMul;
extern M128I g_xiVGMul;
extern M128I g_xiUBMul;
extern M128I g_xiVRMul;

extern M128I g_xiWord00FF;
extern M128I g_xiWordFF00;

extern M128I g_xiMulToY;
extern M128I g_xiMulToU;
extern M128I g_xiMulToV;
extern M128I g_xiMask0;
extern M128I g_xiMask1;
extern M128I g_xiMask2;
extern M128I g_xiMask3;
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy memory line by line.
VOID LineCopy(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines);
VOID LineCopyDI(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines);
#ifdef _MMX
VOID LineCopy_mmx(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines);
VOID LineCopyDI_mmx(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines);
#endif
#ifdef _XMM
VOID LineCopyDI_xmm(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines);
#endif
#ifdef _SSE2
VOID LineCopy_sse2(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines);
VOID LineCopyDI_sse2(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#ifdef __cplusplus
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
