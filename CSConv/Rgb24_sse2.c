


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"

#ifdef _SSE2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to YV12.
VOID Rgb24ToYv12_sse2(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
					  PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstYDif;
	INT iDstUvDif;

	M128I rgb0, rgb1, rgb2, rgb3;
	M128I mz;
	M128I y0, y1;
	M128I uv0, uv1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 3);
	iDstYDif = iDstYStride - uWidth;
	iDstUvDif = iDstUvStride - (uWidth / 2);

	mz = _mm_setzero_si128();

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 4; x; x--)
		{
			_mm_prefetch(pbSrcX + 12, _MM_HINT_NTA);
			_mm_prefetch(pbSrcX + iSrcXStride + 12, _MM_HINT_NTA);

			// Calculate Y for pixel 0/1 of each line.
			rgb2 = _mm_loadu_si128(((PM128I) pbSrcX));					// [ r | gbrgbrgb]
			rgb0 = _mm_and_si128(rgb2, g_xiMask0);						// [ 0 | 0 ..0 rgb]
			rgb1 = _mm_and_si128(rgb2, g_xiMask1);						// [ 0 | 0..0 rgb 000]
			rgb0 = _mm_or_si128(rgb0, _mm_slli_si128(rgb1, 1));			// { 0 | 0..0 rgb 0rgb}
			rgb1 = _mm_and_si128(rgb2, g_xiMask2);
			rgb0 = _mm_or_si128(rgb0, _mm_slli_si128(rgb1, 2));
			rgb1 = _mm_and_si128(rgb2, g_xiMask3);
			rgb2 = _mm_or_si128(rgb0, _mm_slli_si128(rgb1, 3));			// [ 0 r g b 0 r g b | 0 r g b 0 r g b ]

			rgb3 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride)));
			rgb0 = _mm_and_si128(rgb3, g_xiMask0);						// [ 0 | 0..0 rgb]
			rgb1 = _mm_and_si128(rgb3, g_xiMask1);						// [ 0 | 0..0 rgb 000]
			rgb0 = _mm_or_si128(rgb0, _mm_slli_si128(rgb1, 1));			// { 0 | 0..0 rgb 0rgb}
			rgb1 = _mm_and_si128(rgb3, g_xiMask2);						// 
			rgb0 = _mm_or_si128(rgb0, _mm_slli_si128(rgb1, 2));			// 
			rgb1 = _mm_and_si128(rgb3, g_xiMask3);						// 
			rgb3 = _mm_or_si128(rgb0, _mm_slli_si128(rgb1, 3));			// [ 0 r g b 0 r g b | 0 r g b 0 r g b ]

			rgb0 = _mm_unpacklo_epi8(rgb2, mz);							// [ 0 0 0 r 0 g 0 b | 0 0 0 r 0 g 0 b]
			rgb1 = _mm_unpacklo_epi8(rgb3, mz); 

			uv0 = _mm_add_epi16(rgb0, rgb1);
			uv1 = _mm_srli_si128(uv0, 8);
			uv0 = _mm_add_epi16(uv0, uv1);

			rgb0 = _mm_madd_epi16(rgb0, g_xiMulToY);					// [ 0+r | g+b | 0+r | g+b]
			rgb1 = _mm_madd_epi16(rgb1, g_xiMulToY);
			y0 = _mm_add_epi32(rgb0, _mm_srli_epi64(rgb0, 32));			// y = r + (g+b), low 32 is y;
			y1 = _mm_add_epi32(rgb1, _mm_srli_epi64(rgb1, 32)); 

			rgb0 = _mm_srli_epi32(y0, 8);
			rgb1 = _mm_srli_epi32(y1, 8);
			*((PDWORD) pbDstY) = _mm_cvtsi128_si32(rgb0) + 16;
			*((PDWORD) (pbDstY + iDstYStride)) = _mm_cvtsi128_si32(rgb1) + 16;

			y0 = _mm_srli_si128(rgb0, 8);
			y1 = _mm_srli_si128(rgb1, 8);
			*((PDWORD) (pbDstY + 1)) = _mm_cvtsi128_si32(y0) + 16;
			*((PDWORD) (pbDstY + 1 + iDstYStride)) = _mm_cvtsi128_si32(y1) + 16;

			// Calculate U/V.
			uv1 = _mm_madd_epi16(uv0, g_xiMulToV);						// [ 0+r | g+b]
			uv0 = _mm_madd_epi16(uv0, g_xiMulToU);						// [ 0+r | g+b]

			uv0 = _mm_add_epi32(uv0, _mm_srli_epi64(uv0, 32));			// u = r + (g+b), low 32 is u;			
			uv1 = _mm_add_epi32(uv1, _mm_srli_epi64(uv1, 32));			// v = r + (g+b), low 32 is v;			

			*((PDWORD) pbDstU) = _mm_cvtsi128_si32(_mm_srli_epi32(uv0, 10)) + 128;
			*((PDWORD) pbDstV) = _mm_cvtsi128_si32(_mm_srli_epi32(uv1, 10)) + 128;


			// Calculate Y for pixel 2/3 of each line.
			rgb0 = _mm_unpackhi_epi8(rgb2, mz);							// [ 0 0 0 r 0 g 0 b| 0 0 0 r 0 g 0 b]
			rgb1 = _mm_unpackhi_epi8(rgb3, mz); 

			uv0 = _mm_add_epi16(rgb0, rgb1);
			uv1 = _mm_srli_si128(uv0, 8);
			uv0 = _mm_add_epi16(uv0, uv1);

			rgb0 = _mm_madd_epi16(rgb0, g_xiMulToY);					// [ 0+r | g+b | 0+r | g+b]
			rgb1 = _mm_madd_epi16(rgb1, g_xiMulToY);
			y0 = _mm_add_epi32(rgb0, _mm_srli_epi64(rgb0, 32));			// y = r + (g+b), low 32 is y;
			y1 = _mm_add_epi32(rgb1, _mm_srli_epi64(rgb1, 32)); 

			pbDstY += 2;
			rgb0 = _mm_srli_epi32(y0, 8);
			rgb1 = _mm_srli_epi32(y1, 8);
			*((PDWORD) pbDstY) = _mm_cvtsi128_si32(rgb0) + 16;
			*((PDWORD) (pbDstY + iDstYStride)) = _mm_cvtsi128_si32(rgb1) + 16;

			y0 = _mm_srli_si128(rgb0, 8);
			y1 = _mm_srli_si128(rgb1, 8);
			*((PDWORD) (pbDstY + 1)) = _mm_cvtsi128_si32(y0) + 16;
			*((PDWORD) (pbDstY + 1 + iDstYStride)) = _mm_cvtsi128_si32(y1) + 16;

			// Calculate U/V.
			uv1 = _mm_madd_epi16(uv0, g_xiMulToV);						// [ 0+r | g+b]
			uv0 = _mm_madd_epi16(uv0, g_xiMulToU);						// [ 0+r | g+b]

			uv0 = _mm_add_epi32(uv0, _mm_srli_epi64(uv0, 32));			// u = r + (g+b), low 32 is u;			
			uv1 = _mm_add_epi32(uv1, _mm_srli_epi64(uv1, 32));			// v = r + (g+b), low 32 is v;			

			*((PDWORD) (pbDstU+1)) = _mm_cvtsi128_si32(_mm_srli_epi32(uv0, 10)) + 128;
			*((PDWORD) (pbDstV+1)) = _mm_cvtsi128_si32(_mm_srli_epi32(uv1, 10)) + 128;

			pbSrcX += 12;
			pbDstY += 2;
			pbDstU += 2;
			pbDstV += 2;
		}

		pbSrcX += iSrcXDif + iSrcXStride;
		pbDstY += iDstYDif + iDstYStride;
		pbDstU += iDstUvDif;
		pbDstV += iDstUvDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to YUY2.
VOID Rgb24ToYuy2_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to YVYU.
VOID Rgb24ToYvyu_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to UYVY.
VOID Rgb24ToUyvy_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to VYUY.
VOID Rgb24ToVyuy_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to RGB32.
VOID Rgb24ToRgb32_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to RGB24.
VOID Rgb24ToRgb24_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to RGB16.
VOID Rgb24ToRgb16_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to RGB15.
VOID Rgb24ToRgb15_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
