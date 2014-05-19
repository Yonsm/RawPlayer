


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"

#ifdef _SSE2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to YV12.
VOID UyvyToYv12_sse2(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
					 PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstYDif;
	INT iDstUvDif;
	M128I xi0, xi1, xi2, xi3, xi4, xi5, xi7;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 2);
	iDstYDif = iDstYStride - uWidth;
	iDstUvDif = iDstUvStride - (uWidth / 2);

	xi7 = g_xiWord00FF;
	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 16; x; x--)
		{
			_mm_prefetch(pbSrcX + 32, _MM_HINT_NTA);
			_mm_prefetch(pbSrcX + iSrcXStride + 32, _MM_HINT_NTA);

			xi0 = _mm_loadu_si128(((PM128I) pbSrcX) + 0);
			xi1 = _mm_loadu_si128(((PM128I) pbSrcX) + 1);
			xi2 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride)) + 0);
			xi3 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride)) + 1);

			xi4 = xi0;
			xi5 = xi1;

			xi4 = _mm_avg_epu8(xi4, xi2);
			xi5 = _mm_avg_epu8(xi5, xi3);
			xi4 = _mm_and_si128(xi4, xi7);
			xi5 = _mm_and_si128(xi5, xi7);

			xi0 = _mm_srli_epi16(xi0, 8);
			xi1 = _mm_srli_epi16(xi1, 8);
			xi2 = _mm_srli_epi16(xi2, 8);
			xi3 = _mm_srli_epi16(xi3, 8);

			xi0 = _mm_and_si128(xi0, xi7);
			xi1 = _mm_and_si128(xi1, xi7);
			xi2 = _mm_and_si128(xi2, xi7);
			xi3 = _mm_and_si128(xi3, xi7);

			xi0 = _mm_packus_epi16(xi0, xi1);
			xi2 = _mm_packus_epi16(xi2, xi3);

			_mm_storeu_si128((PM128I) pbDstY, xi0);
			_mm_storeu_si128((PM128I) (pbDstY + iDstYStride), xi2);

			xi4 = _mm_packus_epi16(xi4, xi5);
			xi5 = xi4;

			xi4 = _mm_srli_si128(xi4, 1);

			xi5 = _mm_and_si128(xi5, xi7);
			xi4 = _mm_and_si128(xi4, xi7);

			xi5 = _mm_packus_epi16(xi5, xi5);
			xi4 = _mm_packus_epi16(xi4, xi4);

			_mm_storel_epi64((PM128I) pbDstU, xi5);
			_mm_storel_epi64((PM128I) pbDstV, xi4);

			pbSrcX += 32;
			pbDstY += 16;
			pbDstU += 8;
			pbDstV += 8;
		}

		for (x = (uWidth & 15) / 2; x; x--)
		{
			pbDstY[0] = pbSrcX[1];
			pbDstU[0] = (pbSrcX[0] + pbSrcX[iSrcXStride + 0] + 1) / 2;
			pbDstY[1] = pbSrcX[3];
			pbDstV[0] = (pbSrcX[2] + pbSrcX[iSrcXStride + 2] + 1) / 2;

			pbDstY[iDstYStride + 0] = pbSrcX[iSrcXStride + 1];
			pbDstY[iDstYStride + 1] = pbSrcX[iSrcXStride + 3];

			pbSrcX += 4;
			pbDstY += 2;
			pbDstU++;
			pbDstV++;
		}

		pbSrcX += iSrcXDif + iSrcXStride;
		pbDstY += iDstYDif + iDstYStride;
		pbDstU += iDstUvDif;
		pbDstV += iDstUvDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to YUY2.
VOID UyvyToYuy2_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	Yuy2ToUyvy_sse2(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to YVYU.
VOID UyvyToYvyu_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to UYVY.
VOID UyvyToUyvy_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopy_sse2(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to VYUY.
VOID UyvyToVyuy_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to RGB32.
VOID UyvyToRgb32_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to RGB24.
VOID UyvyToRgb24_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to RGB16.
VOID UyvyToRgb16_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to RGB15.
VOID UyvyToRgb15_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
