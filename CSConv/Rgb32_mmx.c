


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"

#ifdef _MMX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB32 to YV12.
VOID Rgb32ToYv12_mmx(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
					 PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstYDif;
	INT iDstUvDif;

	M64 rgb0, rgb1;
	M64 mz;
	M64 y0, y1;
	M64 uv0, uv1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 4);
	iDstYDif = iDstYStride - uWidth;
	iDstUvDif = iDstUvStride - (uWidth / 2);


	mz = _mm_setzero_si64();
	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			// Calculate Y for pixel 0 of each line.
			rgb0 = _mm_cvtsi32_si64(*((PDWORD) pbSrcX));				// [ - | 0 r g b]
			rgb1 = _mm_cvtsi32_si64(*((PDWORD) (pbSrcX + iSrcXStride)));
			rgb0 = _mm_unpacklo_pi8(rgb0, mz);							// [ 0 r | g b]
			rgb1 = _mm_unpacklo_pi8(rgb1, mz); 

			uv0 = _mm_add_pi16(rgb0, rgb1);

			rgb0 = _mm_madd_pi16(rgb0, g_mMulToY);						// [ 0+r | g+b]
			rgb1 = _mm_madd_pi16(rgb1, g_mMulToY);
			y0 = _mm_add_pi32(rgb0, _mm_srli_si64(rgb0, 32));			// y = r + (g+b), low 32 is y;
			y1 = _mm_add_pi32(rgb1, _mm_srli_si64(rgb1, 32)); 

			*((PDWORD) pbDstY) = _mm_cvtsi64_si32(_mm_srli_pi32(y0, 8)) + 16;
			*((PDWORD) (pbDstY + iDstYStride)) = _mm_cvtsi64_si32(_mm_srli_pi32(y1, 8)) + 16;

			// Calculate Y for pixel 1 of each line.
			rgb0 = _mm_cvtsi32_si64(*((PDWORD) (pbSrcX + 4)));			// [ - | 0 r g b]
			rgb1 = _mm_cvtsi32_si64(*((PDWORD) (pbSrcX + 4 + iSrcXStride)));
			rgb0 = _mm_unpacklo_pi8(rgb0, mz);							// [ 0 r | g b]
			rgb1 = _mm_unpacklo_pi8(rgb1, mz); 

			uv0 = _mm_add_pi16(uv0, rgb0);
			uv0 = _mm_add_pi16(uv0, rgb1);

			rgb0 = _mm_madd_pi16(rgb0, g_mMulToY);						// [ 0+r | g+b]
			rgb1 = _mm_madd_pi16(rgb1, g_mMulToY);
			y0 = _mm_add_pi32(rgb0, _mm_srli_si64(rgb0, 32));			// y = r + (g+b), low 32 is y;
			y1 = _mm_add_pi32(rgb1, _mm_srli_si64(rgb1, 32)); 

			*((PDWORD) (pbDstY + 1)) = _mm_cvtsi64_si32(_mm_srli_pi32(y0, 8)) + 16;
			*((PDWORD) (pbDstY + iDstYStride + 1)) = _mm_cvtsi64_si32(_mm_srli_pi32(y1, 8)) + 16;

			// Calculate U/V.
			uv1 = _mm_madd_pi16(uv0, g_mMulToV);						// [ 0+r | g+b]
			uv0 = _mm_madd_pi16(uv0, g_mMulToU);						// [ 0+r | g+b]

			uv0 = _mm_add_pi32(uv0, _mm_srli_si64(uv0, 32));			// u = r + (g+b), low 32 is u;			
			uv1 = _mm_add_pi32(uv1, _mm_srli_si64(uv1, 32));			// v = r + (g+b), low 32 is v;			

			*((PDWORD) pbDstU) = _mm_cvtsi64_si32(_mm_srli_pi32(uv0, 10)) + 128;
			*((PDWORD) pbDstV) = _mm_cvtsi64_si32(_mm_srli_pi32(uv1, 10)) + 128;

			pbSrcX += 8;
			pbDstY += 2;
			pbDstU++;
			pbDstV++;
		}

		pbSrcX += iSrcXDif + iSrcXStride;
		pbDstY += iDstYDif + iDstYStride;
		pbDstU += iDstUvDif;
		pbDstV += iDstUvDif;
	}

	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB32 to YUY2.
VOID Rgb32ToYuy2_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB32 to YVYU.
VOID Rgb32ToYvyu_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB32 to UYVY.
VOID Rgb32ToUyvy_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB32 to VYUY.
VOID Rgb32ToVyuy_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB32 to RGB32.
VOID Rgb32ToRgb32_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB32 to RGB24.
VOID Rgb32ToRgb24_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB32 to RGB16.
VOID Rgb32ToRgb16_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB32 to RGB15.
VOID Rgb32ToRgb15_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
