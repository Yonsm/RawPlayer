


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"

#ifdef _SSE2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YV12.
VOID Yv12ToYv12_sse2(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
					 PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					 UINT uWidth, INT iHeight)
{
	LineCopy_sse2(pbDstY, iDstYStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopy_sse2(pbDstU, iDstUvStride, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopy_sse2(pbDstV, iDstUvStride, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
}

VOID Yv12ToYV12_sse2(PBYTE pbDstX, INT iDstXStride,
					 PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					 UINT uWidth, INT iHeight)
{
	UINT uPixel = iDstXStride * abs(iHeight);
	LineCopy_sse2(pbDstX, iDstXStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopy_sse2(pbDstX + uPixel, iDstXStride / 2, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopy_sse2(pbDstX + uPixel + (uPixel / 4), iDstXStride / 2, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
}

VOID Yv12ToI420_sse2(PBYTE pbDstX, INT iDstXStride,
					 PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					 UINT uWidth, INT iHeight)
{
	UINT uPixel = iDstXStride * abs(iHeight);
	LineCopy_sse2(pbDstX, iDstXStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopy_sse2(pbDstX + uPixel, iDstXStride / 2, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopy_sse2(pbDstX + uPixel + (uPixel / 4), iDstXStride / 2, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YUY2.
VOID Yv12ToYuy2_sse2(PBYTE pbDstX, INT iDstXStride,
					 PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					 UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	M128I xiY0, xiY1, xiY2, xiY3, xiU, xiV, xiUV0, xiUV1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - 2 * uWidth;
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 32; x; x--)
		{
			_mm_prefetch(pbSrcV + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcU + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + 32, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + iSrcYStride + 32, _MM_HINT_NTA);

			xiU = _mm_loadu_si128((PM128I) pbSrcU);
			xiV = _mm_loadu_si128((PM128I) pbSrcV);
			xiUV0 = _mm_unpacklo_epi8(xiU, xiV);
			xiUV1 = _mm_unpackhi_epi8(xiU, xiV);

			xiY0 = _mm_loadu_si128(((PM128I) pbSrcY) + 0);
			xiY1 = _mm_loadu_si128(((PM128I) pbSrcY) + 1);
			xiY2 = _mm_loadu_si128(((PM128I) (pbSrcY + iSrcYStride)) + 0);
			xiY3 = _mm_loadu_si128(((PM128I) (pbSrcY + iSrcYStride)) + 1);

			_mm_storeu_si128(((PM128I) pbDstX) + 0, _mm_unpacklo_epi8(xiY0, xiUV0));
			_mm_storeu_si128(((PM128I) pbDstX) + 1, _mm_unpackhi_epi8(xiY0, xiUV0));
			_mm_storeu_si128(((PM128I) pbDstX) + 2, _mm_unpacklo_epi8(xiY1, xiUV1));
			_mm_storeu_si128(((PM128I) pbDstX) + 3, _mm_unpackhi_epi8(xiY1, xiUV1));

			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 0, _mm_unpacklo_epi8(xiY2, xiUV0));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 1, _mm_unpackhi_epi8(xiY2, xiUV0));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 2, _mm_unpacklo_epi8(xiY3, xiUV1));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 3, _mm_unpackhi_epi8(xiY3, xiUV1));

			pbDstX += 64;
			pbSrcY += 32;
			pbSrcU += 16;
			pbSrcV += 16;
		}

		for (x = (uWidth & 31) / 2; x; x--)
		{
			pbDstX[0] = pbSrcY[0];
			pbDstX[1] = pbSrcU[0];
			pbDstX[2] = pbSrcY[1];
			pbDstX[3] = pbSrcV[0];

			pbDstX[iDstXStride + 0] = pbSrcY[iSrcYStride + 0];
			pbDstX[iDstXStride + 1] = pbSrcU[0];
			pbDstX[iDstXStride + 2] = pbSrcY[iSrcYStride + 1];
			pbDstX[iDstXStride + 3] = pbSrcV[0];

			pbDstX += 4;
			pbSrcY += 2;
			pbSrcU++;
			pbSrcV++;
		}

		pbDstX += iDstXDif + iDstXStride;
		pbSrcY += iSrcYDif + iSrcYStride;
		pbSrcU += iSrcUvDif;
		pbSrcV += iSrcUvDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YVYU.
VOID Yv12ToYvyu_sse2(PBYTE pbDstX, INT iDstXStride,
					 PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					 UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	M128I xiY0, xiY1, xiY2, xiY3, xiU, xiV, xiVU0, xiVU1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - 2 * uWidth;
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 32; x; x--)
		{
			_mm_prefetch(pbSrcV + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcU + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + 32, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + iSrcYStride + 32, _MM_HINT_NTA);

			xiU = _mm_loadu_si128((PM128I) pbSrcU);
			xiV = _mm_loadu_si128((PM128I) pbSrcV);
			xiVU0 = _mm_unpacklo_epi8(xiV, xiU);
			xiVU1 = _mm_unpackhi_epi8(xiV, xiU);

			xiY0 = _mm_loadu_si128(((PM128I) pbSrcY) + 0);
			xiY1 = _mm_loadu_si128(((PM128I) pbSrcY) + 1);
			xiY2 = _mm_loadu_si128(((PM128I) (pbSrcY + iSrcYStride)) + 0);
			xiY3 = _mm_loadu_si128(((PM128I) (pbSrcY + iSrcYStride)) + 1);

			_mm_storeu_si128(((PM128I) pbDstX) + 0, _mm_unpacklo_epi8(xiY0, xiVU0));
			_mm_storeu_si128(((PM128I) pbDstX) + 1, _mm_unpackhi_epi8(xiY0, xiVU0));
			_mm_storeu_si128(((PM128I) pbDstX) + 2, _mm_unpacklo_epi8(xiY1, xiVU1));
			_mm_storeu_si128(((PM128I) pbDstX) + 3, _mm_unpackhi_epi8(xiY1, xiVU1));

			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 0, _mm_unpacklo_epi8(xiY2, xiVU0));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 1, _mm_unpackhi_epi8(xiY2, xiVU0));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 2, _mm_unpacklo_epi8(xiY3, xiVU1));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 3, _mm_unpackhi_epi8(xiY3, xiVU1));

			pbDstX += 64;
			pbSrcY += 32;
			pbSrcU += 16;
			pbSrcV += 16;
		}

		for (x = (uWidth & 31) / 2; x; x--)
		{
			pbDstX[0] = pbSrcY[0];
			pbDstX[1] = pbSrcV[0];
			pbDstX[2] = pbSrcY[1];
			pbDstX[3] = pbSrcU[0];

			pbDstX[iDstXStride + 0] = pbSrcY[iSrcYStride + 0];
			pbDstX[iDstXStride + 1] = pbSrcV[0];
			pbDstX[iDstXStride + 2] = pbSrcY[iSrcYStride + 1];
			pbDstX[iDstXStride + 3] = pbSrcU[0];

			pbDstX += 4;
			pbSrcY += 2;
			pbSrcU++;
			pbSrcV++;
		}

		pbDstX += iDstXDif + iDstXStride;
		pbSrcY += iSrcYDif + iSrcYStride;
		pbSrcU += iSrcUvDif;
		pbSrcV += iSrcUvDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to UYVY.
VOID Yv12ToUyvy_sse2(PBYTE pbDstX, INT iDstXStride,
					 PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					 UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	M128I xiY0, xiY1, xiY2, xiY3, xiU, xiV, xiUV0, xiUV1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - 2 * uWidth;
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 32; x; x--)
		{
			_mm_prefetch(pbSrcV + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcU + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + 32, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + iSrcYStride + 32, _MM_HINT_NTA);

			xiU = _mm_loadu_si128((PM128I) pbSrcU);
			xiV = _mm_loadu_si128((PM128I) pbSrcV);
			xiUV0 = _mm_unpacklo_epi8(xiU, xiV);
			xiUV1 = _mm_unpackhi_epi8(xiU, xiV);

			xiY0 = _mm_loadu_si128(((PM128I) pbSrcY) + 0);
			xiY1 = _mm_loadu_si128(((PM128I) pbSrcY) + 1);
			xiY2 = _mm_loadu_si128(((PM128I) (pbSrcY + iSrcYStride)) + 0);
			xiY3 = _mm_loadu_si128(((PM128I) (pbSrcY + iSrcYStride)) + 1);

			_mm_storeu_si128(((PM128I) pbDstX) + 0, _mm_unpacklo_epi8(xiUV0, xiY0));
			_mm_storeu_si128(((PM128I) pbDstX) + 1, _mm_unpackhi_epi8(xiUV0, xiY0));
			_mm_storeu_si128(((PM128I) pbDstX) + 2, _mm_unpacklo_epi8(xiUV1, xiY1));
			_mm_storeu_si128(((PM128I) pbDstX) + 3, _mm_unpackhi_epi8(xiUV1, xiY1));

			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 0, _mm_unpacklo_epi8(xiUV0, xiY2));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 1, _mm_unpackhi_epi8(xiUV0, xiY2));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 2, _mm_unpacklo_epi8(xiUV1, xiY3));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 3, _mm_unpackhi_epi8(xiUV1, xiY3));

			pbDstX += 64;
			pbSrcY += 32;
			pbSrcU += 16;
			pbSrcV += 16;
		}

		for (x = (uWidth & 31) / 2; x; x--)
		{
			pbDstX[0] = pbSrcU[0];
			pbDstX[1] = pbSrcY[0];
			pbDstX[2] = pbSrcV[0];
			pbDstX[3] = pbSrcY[1];

			pbDstX[iDstXStride + 0] = pbSrcU[0];
			pbDstX[iDstXStride + 1] = pbSrcY[iSrcYStride + 0];
			pbDstX[iDstXStride + 2] = pbSrcV[0];
			pbDstX[iDstXStride + 3] = pbSrcY[iSrcYStride + 1];

			pbDstX += 4;
			pbSrcY += 2;
			pbSrcU++;
			pbSrcV++;
		}

		pbDstX += iDstXDif + iDstXStride;
		pbSrcY += iSrcYDif + iSrcYStride;
		pbSrcU += iSrcUvDif;
		pbSrcV += iSrcUvDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to VYUY.
VOID Yv12ToVyuy_sse2(PBYTE pbDstX, INT iDstXStride,
					 PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					 UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	M128I xiY0, xiY1, xiY2, xiY3, xiU, xiV, xiVU0, xiVU1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - 2 * uWidth;
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 32; x; x--)
		{
			_mm_prefetch(pbSrcV + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcU + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + 32, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + iSrcYStride + 32, _MM_HINT_NTA);

			xiU = _mm_loadu_si128((PM128I) pbSrcU);
			xiV = _mm_loadu_si128((PM128I) pbSrcV);
			xiVU0 = _mm_unpacklo_epi8(xiV, xiU);
			xiVU1 = _mm_unpackhi_epi8(xiV, xiU);

			xiY0 = _mm_loadu_si128(((PM128I) pbSrcY) + 0);
			xiY1 = _mm_loadu_si128(((PM128I) pbSrcY) + 1);
			xiY2 = _mm_loadu_si128(((PM128I) (pbSrcY + iSrcYStride)) + 0);
			xiY3 = _mm_loadu_si128(((PM128I) (pbSrcY + iSrcYStride)) + 1);

			_mm_storeu_si128(((PM128I) pbDstX) + 0, _mm_unpacklo_epi8(xiVU0, xiY0));
			_mm_storeu_si128(((PM128I) pbDstX) + 1, _mm_unpackhi_epi8(xiVU0, xiY0));
			_mm_storeu_si128(((PM128I) pbDstX) + 2, _mm_unpacklo_epi8(xiVU1, xiY1));
			_mm_storeu_si128(((PM128I) pbDstX) + 3, _mm_unpackhi_epi8(xiVU1, xiY1));

			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 0, _mm_unpacklo_epi8(xiVU0, xiY2));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 1, _mm_unpackhi_epi8(xiVU0, xiY2));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 2, _mm_unpacklo_epi8(xiVU1, xiY3));
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 3, _mm_unpackhi_epi8(xiVU1, xiY3));

			pbDstX += 64;
			pbSrcY += 32;
			pbSrcU += 16;
			pbSrcV += 16;
		}

		for (x = (uWidth & 31) / 2; x; x--)
		{
			pbDstX[0] = pbSrcV[0];
			pbDstX[1] = pbSrcY[0];
			pbDstX[2] = pbSrcU[0];
			pbDstX[3] = pbSrcY[1];

			pbDstX[iDstXStride + 0] = pbSrcV[0];
			pbDstX[iDstXStride + 1] = pbSrcY[iSrcYStride + 0];
			pbDstX[iDstXStride + 2] = pbSrcU[0];
			pbDstX[iDstXStride + 3] = pbSrcY[iSrcYStride + 1];

			pbDstX += 4;
			pbSrcY += 2;
			pbSrcU++;
			pbSrcV++;
		}


		pbDstX += iDstXDif + iDstXStride;
		pbSrcY += iSrcYDif + iSrcYStride;
		pbSrcU += iSrcUvDif;
		pbSrcV += iSrcUvDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB32.
VOID Yv12ToRgb32_sse2(PBYTE pbDstX, INT iDstXStride,
					  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					  UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv;

	M128I y0, y1, u0, v0, mz;
	M128I r0, g0, b0, r1, g1, b1;
	M128I rgb0, rgb1, rgb2, rgb3;
	M128I bu0, gu0, gv0, rv0, bu1, rv1, guv0, guv1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 4);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	mz = _mm_setzero_si128();
	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 16; x; x--)
		{
			_mm_prefetch(pbSrcV + 8, _MM_HINT_NTA);
			_mm_prefetch(pbSrcU + 8, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + iSrcYStride + 16, _MM_HINT_NTA);

			// Calculate coefficient.
			u0 = _mm_loadl_epi64((PM128I) pbSrcU);		// [	| u3 u2 u1 u0]
			v0 = _mm_loadl_epi64((PM128I) pbSrcV);		// [	| v3 v2 v1 v0]

			u0 = _mm_unpacklo_epi8(u0, mz);				// u3 u2 u1 u0
			v0 = _mm_unpacklo_epi8(v0, mz);				// v3 v2 v1 v0;
			u0 = _mm_subs_epi16(u0, g_xiSub80);
			v0 = _mm_subs_epi16(v0, g_xiSub80);

			gu0 = _mm_mullo_epi16(u0, g_xiUGMul);
			gv0 = _mm_mullo_epi16(v0, g_xiVGMul);
			bu0 = _mm_mullo_epi16(u0, g_xiUBMul);
			rv0 = _mm_mullo_epi16(v0, g_xiVRMul);

			guv0 = _mm_adds_epi16(gu0, gv0);

			guv1 = _mm_unpackhi_epi16(guv0, guv0);		// guv3 guv3 guv2 guv2
			guv0 = _mm_unpacklo_epi16(guv0, guv0);		// guv1 guv1 guv0 guv0
			bu1 = _mm_unpackhi_epi16(bu0, bu0);			// bu3 bu3 bu2 bu2
			bu0 = _mm_unpacklo_epi16(bu0, bu0);			// bu1 bu1 bu0 bu0
			rv1 = _mm_unpackhi_epi16(rv0, rv0);			// rv3 rv3 rv2 rv2
			rv0 = _mm_unpacklo_epi16(rv0, rv0);			// rv1 rv1 rv0 rv0

			// Process for row 0.
			y0 = _mm_loadu_si128((PM128I) pbSrcY);		// [YYYY | YYYY]; row 0
			y1 = _mm_unpackhi_epi8(y0, mz);				// y7 y6 y5 y4
			y0 = _mm_unpacklo_epi8(y0, mz);				// y3 y2 y1 y0
			y1 = _mm_subs_epi16(y1, g_xiSub10);
			y0 = _mm_subs_epi16(y0, g_xiSub10);
			y1 = _mm_mullo_epi16(y1, g_xiYYMul);
			y0 = _mm_mullo_epi16(y0, g_xiYYMul);

			g1 = _mm_subs_epi16(y1, guv1);				// g7 g6 g5 g4
			g0 = _mm_subs_epi16(y0, guv0);				// g3 g2 g1 g0
			g1 = _mm_srai_epi16(g1, SCALEBITS);
			g0 = _mm_srai_epi16(g0, SCALEBITS);
			g0 = _mm_packus_epi16(g0, g1);				// g7 g6 ...g1 g0

			b1 = _mm_adds_epi16(y1, bu1);
			b0 = _mm_adds_epi16(y0, bu0);
			b1 = _mm_srai_epi16(b1, SCALEBITS);
			b0 = _mm_srai_epi16(b0, SCALEBITS);
			b0 = _mm_packus_epi16(b0, b1);

			r1 = _mm_adds_epi16(y1, rv1);
			r0 = _mm_adds_epi16(y0, rv0);
			r1 = _mm_srai_epi16(r1, SCALEBITS);
			r0 = _mm_srai_epi16(r0, SCALEBITS);
			r0 = _mm_packus_epi16(r0, r1);

			r1 = _mm_unpackhi_epi8(b0, r0);				// r7 b7 r6 b6 r5 b5 r4 b4
			r0 = _mm_unpacklo_epi8(b0, r0);				// r3 b3 r2 b2 r1 b1 r0 b0

			g1 = _mm_unpackhi_epi8(g0, mz);				// 0 g7 0 g6 0 g5 0 g4
			g0 = _mm_unpacklo_epi8(g0, mz);				// 0 g3 0 g2 0 g1 0 g0

			rgb0 = _mm_unpacklo_epi8(r0, g0);			// 0 r1 g1 b1 0 r0 g0 b0
			rgb1 = _mm_unpackhi_epi8(r0, g0);			// 0 r3 g3 b3 0 r2 g2 b2
			rgb2 = _mm_unpacklo_epi8(r1, g1);			// 0 r5 g5 b5 0 r4 g4 b4
			rgb3 = _mm_unpackhi_epi8(r1, g1);			// 0 r7 g7 b7 0 r6 g6 b6

			// Write out row 0.
			_mm_storeu_si128(((PM128I) pbDstX) + 0, rgb0);
			_mm_storeu_si128(((PM128I) pbDstX) + 1, rgb1);
			_mm_storeu_si128(((PM128I) pbDstX) + 2, rgb2);
			_mm_storeu_si128(((PM128I) pbDstX) + 3, rgb3);

			// Process for row 1.
			y0 = _mm_loadu_si128((PM128I) (pbSrcY + iSrcYStride));		// [YYYY | YYYY]; row 1
			y1 = _mm_unpackhi_epi8(y0, mz);				// y7 y6 y5 y4
			y0 = _mm_unpacklo_epi8(y0, mz);				// y3 y2 y1 y0
			y1 = _mm_subs_epi16(y1, g_xiSub10);
			y0 = _mm_subs_epi16(y0, g_xiSub10);
			y1 = _mm_mullo_epi16(y1, g_xiYYMul);
			y0 = _mm_mullo_epi16(y0, g_xiYYMul);

			g1 = _mm_subs_epi16(y1, guv1);				// g7 g6 g5 g4
			g0 = _mm_subs_epi16(y0, guv0);				// g3 g2 g1 g0
			g1 = _mm_srai_epi16(g1, SCALEBITS);
			g0 = _mm_srai_epi16(g0, SCALEBITS);
			g0 = _mm_packus_epi16(g0, g1);				// g7 g6 ...g1 g0

			b1 = _mm_adds_epi16(y1, bu1);
			b0 = _mm_adds_epi16(y0, bu0);
			b1 = _mm_srai_epi16(b1, SCALEBITS);
			b0 = _mm_srai_epi16(b0, SCALEBITS);
			b0 = _mm_packus_epi16(b0, b1);

			r1 = _mm_adds_epi16(y1, rv1);
			r0 = _mm_adds_epi16(y0, rv0);
			r1 = _mm_srai_epi16(r1, SCALEBITS);
			r0 = _mm_srai_epi16(r0, SCALEBITS);
			r0 = _mm_packus_epi16(r0, r1);

			r1 = _mm_unpackhi_epi8(b0, r0);				// r7 b7 r6 b6 r5 b5 r4 b4
			r0 = _mm_unpacklo_epi8(b0, r0);				// r3 b3 r2 b2 r1 b1 r0 b0

			g1 = _mm_unpackhi_epi8(g0, mz);				// 0 g7 0 g6 0 g5 0 g4
			g0 = _mm_unpacklo_epi8(g0, mz);				// 0 g3 0 g2 0 g1 0 g0

			rgb0 = _mm_unpacklo_epi8(r0, g0);			// 0 r1 g1 b1 0 r0 g0 b0
			rgb1 = _mm_unpackhi_epi8(r0, g0);			// 0 r3 g3 b3 0 r2 g2 b2
			rgb2 = _mm_unpacklo_epi8(r1, g1);			// 0 r5 g5 b5 0 r4 g4 b4
			rgb3 = _mm_unpackhi_epi8(r1, g1);			// 0 r7 g7 b7 0 r6 g6 b6

			// Write out row 1.
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 0, rgb0);
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 1, rgb1);
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 2, rgb2);
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 3, rgb3);

			pbDstX += 64;
			pbSrcY += 16;
			pbSrcU += 8;
			pbSrcV += 8;
		}

		for (x = (uWidth & 7) / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcU[0]];
			guv = g_iGUTab[pbSrcU[0]] + g_iGVTab[pbSrcV[0]];
			rv = g_iRVTab[pbSrcV[0]];

			yy = g_iYYTab[pbSrcY[0]];
			pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);
			pbDstX[3] = 0;

			yy = g_iYYTab[pbSrcY[1]];
			pbDstX[4] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[5] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[6] = _Clip((yy + rv) >> SCALEBITS_OUT);
			pbDstX[7] = 0;

			yy = g_iYYTab[pbSrcY[iSrcYStride]];
			pbDstX[iDstXStride + 0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 2] = _Clip((yy + rv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 3] = 0;

			yy = g_iYYTab[pbSrcY[iSrcYStride + 1]];
			pbDstX[iDstXStride + 4] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 5] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 6] = _Clip((yy + rv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 7] = 0;

			pbDstX += 8;
			pbSrcY += 2;
			pbSrcU++;
			pbSrcV++;
		}

		pbDstX += iDstXDif + iDstXStride;
		pbSrcY += iSrcYDif + iSrcYStride;
		pbSrcU += iSrcUvDif;
		pbSrcV += iSrcUvDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB24.
VOID Yv12ToRgb24_sse2(PBYTE pbDstX, INT iDstXStride,
					  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					  UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv;

	M128I y0, y1, u0, v0, mz;
	M128I r0, g0, b0, r1, g1, b1;
	M128I rgb0, rgb1, rgb2, rgb3;
	M128I bu0, gu0, gv0, rv0, bu1, rv1, guv0, guv1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 3);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	mz = _mm_setzero_si128();
	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 16; x; x--)
		{
			_mm_prefetch(pbSrcV + 8, _MM_HINT_NTA);
			_mm_prefetch(pbSrcU + 8, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + iSrcYStride + 16, _MM_HINT_NTA);

			// Calculate coefficient.
			u0 = _mm_loadl_epi64((PM128I) pbSrcU);		// [	| u3 u2 u1 u0]
			v0 = _mm_loadl_epi64((PM128I) pbSrcV);		// [	| v3 v2 v1 v0]

			u0 = _mm_unpacklo_epi8(u0, mz);				// u3 u2 u1 u0
			v0 = _mm_unpacklo_epi8(v0, mz);				// v3 v2 v1 v0;
			u0 = _mm_subs_epi16(u0, g_xiSub80);
			v0 = _mm_subs_epi16(v0, g_xiSub80);

			gu0 = _mm_mullo_epi16(u0, g_xiUGMul);
			gv0 = _mm_mullo_epi16(v0, g_xiVGMul);
			bu0 = _mm_mullo_epi16(u0, g_xiUBMul);
			rv0 = _mm_mullo_epi16(v0, g_xiVRMul);

			guv0 = _mm_adds_epi16(gu0, gv0);

			guv1 = _mm_unpackhi_epi16(guv0, guv0);		// guv3 guv3 guv2 guv2
			guv0 = _mm_unpacklo_epi16(guv0, guv0);		// guv1 guv1 guv0 guv0
			bu1 = _mm_unpackhi_epi16(bu0, bu0);			// bu3 bu3 bu2 bu2
			bu0 = _mm_unpacklo_epi16(bu0, bu0);			// bu1 bu1 bu0 bu0
			rv1 = _mm_unpackhi_epi16(rv0, rv0);			// rv3 rv3 rv2 rv2
			rv0 = _mm_unpacklo_epi16(rv0, rv0);			// rv1 rv1 rv0 rv0

			// Process for row 0.
			y0 = _mm_loadu_si128((PM128I) pbSrcY);		// [YYYY | YYYY]; row 0
			y1 = _mm_unpackhi_epi8(y0, mz);				// y7 y6 y5 y4
			y0 = _mm_unpacklo_epi8(y0, mz);				// y3 y2 y1 y0
			y1 = _mm_subs_epi16(y1, g_xiSub10);
			y0 = _mm_subs_epi16(y0, g_xiSub10);
			y1 = _mm_mullo_epi16(y1, g_xiYYMul);
			y0 = _mm_mullo_epi16(y0, g_xiYYMul);

			g1 = _mm_subs_epi16(y1, guv1);				// g7 g6 g5 g4
			g0 = _mm_subs_epi16(y0, guv0);				// g3 g2 g1 g0
			g1 = _mm_srai_epi16(g1, SCALEBITS);
			g0 = _mm_srai_epi16(g0, SCALEBITS);
			g0 = _mm_packus_epi16(g0, g1);				// g7 g6 ...g1 g0

			b1 = _mm_adds_epi16(y1, bu1);
			b0 = _mm_adds_epi16(y0, bu0);
			b1 = _mm_srai_epi16(b1, SCALEBITS);
			b0 = _mm_srai_epi16(b0, SCALEBITS);
			b0 = _mm_packus_epi16(b0, b1);

			r1 = _mm_adds_epi16(y1, rv1);
			r0 = _mm_adds_epi16(y0, rv0);
			r1 = _mm_srai_epi16(r1, SCALEBITS);
			r0 = _mm_srai_epi16(r0, SCALEBITS);
			r0 = _mm_packus_epi16(r0, r1);

			r1 = _mm_unpackhi_epi8(b0, r0);				// r7 b7 r6 b6 r5 b5 r4 b4
			r0 = _mm_unpacklo_epi8(b0, r0);				// r3 b3 r2 b2 r1 b1 r0 b0

			g1 = _mm_unpackhi_epi8(g0, mz);				// 0 g7 0 g6 0 g5 0 g4
			g0 = _mm_unpacklo_epi8(g0, mz);				// 0 g3 0 g2 0 g1 0 g0

			rgb0 = _mm_unpacklo_epi8(r0, g0);			// 0 r1 g1 b1 0 r0 g0 b0
			rgb1 = _mm_unpackhi_epi8(r0, g0);			// 0 r3 g3 b3 0 r2 g2 b2
			rgb2 = _mm_unpacklo_epi8(r1, g1);			// 0 r5 g5 b5 0 r4 g4 b4
			rgb3 = _mm_unpackhi_epi8(r1, g1);			// 0 r7 g7 b7 0 r6 g6 b6

			// Write out row 0.
			*((PDWORD) (pbDstX + 0)) = _mm_cvtsi128_si32(rgb0); rgb0 = _mm_srli_si128(rgb0, 4);
			*((PDWORD) (pbDstX + 3)) = _mm_cvtsi128_si32(rgb0); rgb0 = _mm_srli_si128(rgb0, 4);
			*((PDWORD) (pbDstX + 6)) = _mm_cvtsi128_si32(rgb0); rgb0 = _mm_srli_si128(rgb0, 4);
			*((PDWORD) (pbDstX + 9)) = _mm_cvtsi128_si32(rgb0);

			*((PDWORD) (pbDstX + 12)) = _mm_cvtsi128_si32(rgb1); rgb1 = _mm_srli_si128(rgb1, 4);
			*((PDWORD) (pbDstX + 15)) = _mm_cvtsi128_si32(rgb1); rgb1 = _mm_srli_si128(rgb1, 4);
			*((PDWORD) (pbDstX + 18)) = _mm_cvtsi128_si32(rgb1); rgb1 = _mm_srli_si128(rgb1, 4);
			*((PDWORD) (pbDstX + 21)) = _mm_cvtsi128_si32(rgb1);

			*((PDWORD) (pbDstX + 24)) = _mm_cvtsi128_si32(rgb2); rgb2 = _mm_srli_si128(rgb2, 4);
			*((PDWORD) (pbDstX + 27)) = _mm_cvtsi128_si32(rgb2); rgb2 = _mm_srli_si128(rgb2, 4);
			*((PDWORD) (pbDstX + 30)) = _mm_cvtsi128_si32(rgb2); rgb2 = _mm_srli_si128(rgb2, 4);
			*((PDWORD) (pbDstX + 33)) = _mm_cvtsi128_si32(rgb2);

			*((PDWORD) (pbDstX + 36)) = _mm_cvtsi128_si32(rgb3); rgb3 = _mm_srli_si128(rgb3, 4);
			*((PDWORD) (pbDstX + 39)) = _mm_cvtsi128_si32(rgb3); rgb3 = _mm_srli_si128(rgb3, 4);
			*((PDWORD) (pbDstX + 42)) = _mm_cvtsi128_si32(rgb3); rgb3 = _mm_srli_si128(rgb3, 4);
			*((PDWORD) (pbDstX + 45)) = _mm_cvtsi128_si32(rgb3);

			// Process for row 1.
			y0 = _mm_loadu_si128((PM128I) (pbSrcY + iSrcYStride));		// [YYYY | YYYY]; row 1
			y1 = _mm_unpackhi_epi8(y0, mz);				// y7 y6 y5 y4
			y0 = _mm_unpacklo_epi8(y0, mz);				// y3 y2 y1 y0
			y1 = _mm_subs_epi16(y1, g_xiSub10);
			y0 = _mm_subs_epi16(y0, g_xiSub10);
			y1 = _mm_mullo_epi16(y1, g_xiYYMul);
			y0 = _mm_mullo_epi16(y0, g_xiYYMul);

			g1 = _mm_subs_epi16(y1, guv1);				// g7 g6 g5 g4
			g0 = _mm_subs_epi16(y0, guv0);				// g3 g2 g1 g0
			g1 = _mm_srai_epi16(g1, SCALEBITS);
			g0 = _mm_srai_epi16(g0, SCALEBITS);
			g0 = _mm_packus_epi16(g0, g1);				// g7 g6 ...g1 g0

			b1 = _mm_adds_epi16(y1, bu1);
			b0 = _mm_adds_epi16(y0, bu0);
			b1 = _mm_srai_epi16(b1, SCALEBITS);
			b0 = _mm_srai_epi16(b0, SCALEBITS);
			b0 = _mm_packus_epi16(b0, b1);

			r1 = _mm_adds_epi16(y1, rv1);
			r0 = _mm_adds_epi16(y0, rv0);
			r1 = _mm_srai_epi16(r1, SCALEBITS);
			r0 = _mm_srai_epi16(r0, SCALEBITS);
			r0 = _mm_packus_epi16(r0, r1);

			r1 = _mm_unpackhi_epi8(b0, r0);				// r7 b7 r6 b6 r5 b5 r4 b4
			r0 = _mm_unpacklo_epi8(b0, r0);				// r3 b3 r2 b2 r1 b1 r0 b0

			g1 = _mm_unpackhi_epi8(g0, mz);				// 0 g7 0 g6 0 g5 0 g4
			g0 = _mm_unpacklo_epi8(g0, mz);				// 0 g3 0 g2 0 g1 0 g0

			rgb0 = _mm_unpacklo_epi8(r0, g0);			// 0 r1 g1 b1 0 r0 g0 b0
			rgb1 = _mm_unpackhi_epi8(r0, g0);			// 0 r3 g3 b3 0 r2 g2 b2
			rgb2 = _mm_unpacklo_epi8(r1, g1);			// 0 r5 g5 b5 0 r4 g4 b4
			rgb3 = _mm_unpackhi_epi8(r1, g1);			// 0 r7 g7 b7 0 r6 g6 b6

			// Write out row 1.
			*((PDWORD) (pbDstX + iDstXStride + 0)) = _mm_cvtsi128_si32(rgb0); rgb0 = _mm_srli_si128(rgb0, 4);
			*((PDWORD) (pbDstX + iDstXStride + 3)) = _mm_cvtsi128_si32(rgb0); rgb0 = _mm_srli_si128(rgb0, 4);
			*((PDWORD) (pbDstX + iDstXStride + 6)) = _mm_cvtsi128_si32(rgb0); rgb0 = _mm_srli_si128(rgb0, 4);
			*((PDWORD) (pbDstX + iDstXStride + 9)) = _mm_cvtsi128_si32(rgb0);

			*((PDWORD) (pbDstX + iDstXStride + 12)) = _mm_cvtsi128_si32(rgb1); rgb1 = _mm_srli_si128(rgb1, 4);
			*((PDWORD) (pbDstX + iDstXStride + 15)) = _mm_cvtsi128_si32(rgb1); rgb1 = _mm_srli_si128(rgb1, 4);
			*((PDWORD) (pbDstX + iDstXStride + 18)) = _mm_cvtsi128_si32(rgb1); rgb1 = _mm_srli_si128(rgb1, 4);
			*((PDWORD) (pbDstX + iDstXStride + 21)) = _mm_cvtsi128_si32(rgb1);

			*((PDWORD) (pbDstX + iDstXStride + 24)) = _mm_cvtsi128_si32(rgb2); rgb2 = _mm_srli_si128(rgb2, 4);
			*((PDWORD) (pbDstX + iDstXStride + 27)) = _mm_cvtsi128_si32(rgb2); rgb2 = _mm_srli_si128(rgb2, 4);
			*((PDWORD) (pbDstX + iDstXStride + 30)) = _mm_cvtsi128_si32(rgb2); rgb2 = _mm_srli_si128(rgb2, 4);
			*((PDWORD) (pbDstX + iDstXStride + 33)) = _mm_cvtsi128_si32(rgb2);

			*((PDWORD) (pbDstX + iDstXStride + 36)) = _mm_cvtsi128_si32(rgb3); rgb3 = _mm_srli_si128(rgb3, 4);
			*((PDWORD) (pbDstX + iDstXStride + 39)) = _mm_cvtsi128_si32(rgb3); rgb3 = _mm_srli_si128(rgb3, 4);
			*((PDWORD) (pbDstX + iDstXStride + 42)) = _mm_cvtsi128_si32(rgb3); rgb3 = _mm_srli_si128(rgb3, 4);
			*((PDWORD) (pbDstX + iDstXStride + 45)) = _mm_cvtsi128_si32(rgb3);

			pbDstX += 48;
			pbSrcY += 16;
			pbSrcU += 8;
			pbSrcV += 8;
		}

		for (x = (uWidth & 15) / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcU[0]];
			guv = g_iGUTab[pbSrcU[0]] + g_iGVTab[pbSrcV[0]];
			rv = g_iRVTab[pbSrcV[0]];

			yy = g_iYYTab[pbSrcY[0]];
			pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);

			yy = g_iYYTab[pbSrcY[1]];
			pbDstX[3] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[4] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[5] = _Clip((yy + rv) >> SCALEBITS_OUT);

			yy = g_iYYTab[pbSrcY[iSrcYStride]];
			pbDstX[iDstXStride + 0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 2] = _Clip((yy + rv) >> SCALEBITS_OUT);

			yy = g_iYYTab[pbSrcY[iSrcYStride + 1]];
			pbDstX[iDstXStride + 3] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 4] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 5] = _Clip((yy + rv) >> SCALEBITS_OUT);

			pbDstX += 6;
			pbSrcY += 2;
			pbSrcU++;
			pbSrcV++;
		}

		pbDstX += iDstXDif + iDstXStride;
		pbSrcY += iSrcYDif + iSrcYStride;
		pbSrcU += iSrcUvDif;
		pbSrcV += iSrcUvDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB16.
VOID Yv12ToRgb16_sse2(PBYTE pbDstX, INT iDstXStride,
					  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					  UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv, r, g, b;

	M128I y0, y1, u0, v0, mz;
	M128I r0, g0, b0, r1, g1, b1;
	M128I bu0, gu0, gv0, rv0, bu1, rv1, guv0, guv1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	mz = _mm_setzero_si128();
	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 16; x; x--)
		{
			_mm_prefetch(pbSrcV + 8, _MM_HINT_NTA);
			_mm_prefetch(pbSrcU + 8, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + iSrcYStride + 16, _MM_HINT_NTA);

			// Calculate coefficient.
			u0 = _mm_loadl_epi64((PM128I) pbSrcU);		// [	| u3 u2 u1 u0]
			v0 = _mm_loadl_epi64((PM128I) pbSrcV);		// [	| v3 v2 v1 v0]

			u0 = _mm_unpacklo_epi8(u0, mz);				// u3 u2 u1 u0
			v0 = _mm_unpacklo_epi8(v0, mz);				// v3 v2 v1 v0
			u0 = _mm_subs_epi16(u0, g_xiSub80);
			v0 = _mm_subs_epi16(v0, g_xiSub80);

			gu0 = _mm_mullo_epi16(u0, g_xiUGMul);
			gv0 = _mm_mullo_epi16(v0, g_xiVGMul);
			bu0 = _mm_mullo_epi16(u0, g_xiUBMul);
			rv0 = _mm_mullo_epi16(v0, g_xiVRMul);

			guv0 = _mm_adds_epi16(gu0, gv0);

			guv1 = _mm_unpackhi_epi16(guv0, guv0);		// guv3 guv3 guv2 guv2
			guv0 = _mm_unpacklo_epi16(guv0, guv0);		// guv1 guv1 guv0 guv0

			bu1 = _mm_unpackhi_epi16(bu0, bu0);			// bu3 bu3 bu2 bu2
			bu0 = _mm_unpacklo_epi16(bu0, bu0);			// bu1 bu1 bu0 bu0
			rv1 = _mm_unpackhi_epi16(rv0, rv0);			// rv3 rv3 rv2 rv2
			rv0 = _mm_unpacklo_epi16(rv0, rv0);			// rv1 rv1 rv0 rv0

			// Process for row 0.
			y0 = _mm_loadu_si128((PM128I) pbSrcY);		// [YYYY | YYYY]; row 0

			y1 = _mm_unpackhi_epi8(y0, mz);				// y7 y6 y5 y4
			y0 = _mm_unpacklo_epi8(y0, mz);				// y3 y2 y1 y0
			y1 = _mm_subs_epi16(y1, g_xiSub10);
			y0 = _mm_subs_epi16(y0, g_xiSub10);
			y1 = _mm_mullo_epi16(y1, g_xiYYMul);
			y0 = _mm_mullo_epi16(y0, g_xiYYMul);

			b1 = _mm_adds_epi16(y1, bu1);
			b0 = _mm_adds_epi16(y0, bu0);
			b1 = _mm_srai_epi16(b1, SCALEBITS);			// 8 bits (0 - 7)
			b0 = _mm_srai_epi16(b0, SCALEBITS);

			b1 = _mm_packus_epi16(b1, mz);				// 0 0 0 0 b7 b6 b5 b4
			b0 = _mm_packus_epi16(b0, mz);				// 0 0 0 0 b3 b2 b1 b0
			b1 = _mm_unpacklo_epi8(b1, mz);				// 0 b7 0b 6 0 b5 0 b4
			b0 = _mm_unpacklo_epi8(b0, mz);

			b1 = _mm_srli_epi16(b1, 3);
			b0 = _mm_srli_epi16(b0, 3);					// 5 bits (0 - 4)

			g1 = _mm_subs_epi16(y1, guv1);				// g7 g6 g5 g4
			g0 = _mm_subs_epi16(y0, guv0);				// g3 g2 g1 g0
			g1 = _mm_srai_epi16(g1, SCALEBITS);			// 8 bits (0 - 7)
			g0 = _mm_srai_epi16(g0, SCALEBITS);

			g1 = _mm_packus_epi16(g1, mz);				// 0 0 0 0 g7 g6 g5 g4
			g0 = _mm_packus_epi16(g0, mz);				// 0 0 0 0 g3 g2 g1 g0
			g1 = _mm_unpacklo_epi8(g1, mz);				// 0 g7 0 g6 0 g5 0 g4
			g0 = _mm_unpacklo_epi8(g0, mz);

			g1 = _mm_srli_epi16(g1, 2);					// 6 bits (0 - 5)
			g0 = _mm_srli_epi16(g0, 2);
			g1 = _mm_slli_epi16(g1, 5);					// 6 bits (5 - 10)
			g0 = _mm_slli_epi16(g0, 5);					// 6 bits (5 - 10)

			r1 = _mm_adds_epi16(y1, rv1);
			r0 = _mm_adds_epi16(y0, rv0);
			r1 = _mm_srai_epi16(r1, SCALEBITS);
			r0 = _mm_srai_epi16(r0, SCALEBITS);

			r1 = _mm_packus_epi16(r1, mz);				// 0 0 0 0 r7 r6 r5 r4
			r0 = _mm_packus_epi16(r0, mz);				// 0 0 0 0 r3 r2 r1 r0
			r1 = _mm_unpacklo_epi8(r1, mz);				// 0 r7 0 r6 0 r5 0 r4
			r0 = _mm_unpacklo_epi8(r0, mz);

			r1 = _mm_srli_epi16(r1, 3);					// 5 bits (0 - 4)
			r0 = _mm_srli_epi16(r0, 3);
			r1 = _mm_slli_epi16(r1, 11);				// 5 bits (11 - 15)
			r0 = _mm_slli_epi16(r0, 11);				// 5 bits (11 - 15)

			// Combine RGB.
			b0 = _mm_or_si128(g0, b0);
			b0 = _mm_or_si128(r0, b0);					// 16 bits rgb
			b1 = _mm_or_si128(g1, b1);
			b1 = _mm_or_si128(r1, b1);					// 16 bits rgb

			// Write out row 0.
			_mm_storeu_si128(((PM128I) pbDstX) + 0, b0);
			_mm_storeu_si128(((PM128I) pbDstX) + 1, b1);

			// Process for row 1.
			y0 = _mm_loadu_si128((PM128I) (pbSrcY + iSrcYStride));		// [YYYY | YYYY]; row 0
			y1 = _mm_unpackhi_epi8(y0, mz);				// y7 y6 y5 y4
			y0 = _mm_unpacklo_epi8(y0, mz);				// y3 y2 y1 y0
			y1 = _mm_subs_epi16(y1, g_xiSub10);
			y0 = _mm_subs_epi16(y0, g_xiSub10);
			y1 = _mm_mullo_epi16(y1, g_xiYYMul);
			y0 = _mm_mullo_epi16(y0, g_xiYYMul);

			b1 = _mm_adds_epi16(y1, bu1);
			b0 = _mm_adds_epi16(y0, bu0);
			b1 = _mm_srai_epi16(b1, SCALEBITS);			// 8 bits (0 - 7)
			b0 = _mm_srai_epi16(b0, SCALEBITS);

			b1 = _mm_packus_epi16(b1, mz);				// 0 0 0 0 b7 b6 b5 b4
			b0 = _mm_packus_epi16(b0, mz);				// 0 0 0 0 b3 b2 b1 b0
			b1 = _mm_unpacklo_epi8(b1, mz);				// 0 b7 0b 6 0 b5 0 b4
			b0 = _mm_unpacklo_epi8(b0, mz);

			b1 = _mm_srli_epi16(b1, 3);
			b0 = _mm_srli_epi16(b0, 3);					// 5 bits (0 - 4)

			g1 = _mm_subs_epi16(y1, guv1);				// g7 g6 g5 g4
			g0 = _mm_subs_epi16(y0, guv0);				// g3 g2 g1 g0
			g1 = _mm_srai_epi16(g1, SCALEBITS);			// 8 bits (0 - 7)
			g0 = _mm_srai_epi16(g0, SCALEBITS);

			g1 = _mm_packus_epi16(g1, mz);				// 0 0 0 0 g7 g6 g5 g4
			g0 = _mm_packus_epi16(g0, mz);				// 0 0 0 0 g3 g2 g1 g0
			g1 = _mm_unpacklo_epi8(g1, mz);				// 0 g7 0 g6 0 g5 0 g4
			g0 = _mm_unpacklo_epi8(g0, mz);

			g1 = _mm_srli_epi16(g1, 2);					// 6 bits (0 - 5)
			g0 = _mm_srli_epi16(g0, 2);
			g1 = _mm_slli_epi16(g1, 5);					// 6 bits (5 - 10)
			g0 = _mm_slli_epi16(g0, 5);					// 6 bits (5 - 10)

			r1 = _mm_adds_epi16(y1, rv1);
			r0 = _mm_adds_epi16(y0, rv0);
			r1 = _mm_srai_epi16(r1, SCALEBITS);
			r0 = _mm_srai_epi16(r0, SCALEBITS);

			r1 = _mm_packus_epi16(r1, mz);				// 0 0 0 0 r7 r6 r5 r4
			r0 = _mm_packus_epi16(r0, mz);				// 0 0 0 0 r3 r2 r1 r0
			r1 = _mm_unpacklo_epi8(r1, mz);				// 0 r7 0 r6 0 r5 0 r4
			r0 = _mm_unpacklo_epi8(r0, mz);

			r1 = _mm_srli_epi16(r1, 3);					// 5 bits (0 - 4)
			r0 = _mm_srli_epi16(r0, 3);
			r1 = _mm_slli_epi16(r1, 11);				// 5 bits (11 - 15)
			r0 = _mm_slli_epi16(r0, 11);				// 5 bits (11 - 15)

			// Combine RGB.
			b0 = _mm_or_si128(g0, b0);
			b0 = _mm_or_si128(r0, b0);					// 16 bits rgb
			b1 = _mm_or_si128(g1, b1);
			b1 = _mm_or_si128(r1, b1);					// 16 bits rgb

			// Write out row 1.
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 0, b0);
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 1, b1);

			pbDstX += 32;
			pbSrcY += 16;
			pbSrcU += 8;
			pbSrcV += 8;
		}

		for (x = (uWidth & 15) / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcU[0]];
			guv = g_iGUTab[pbSrcU[0]] + g_iGVTab[pbSrcV[0]];
			rv = g_iRVTab[pbSrcV[0]];

			yy = g_iYYTab[pbSrcY[0]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) pbDstX)[0] = _MakeRgb16(r, g, b);

			yy = g_iYYTab[pbSrcY[1]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) pbDstX)[1] = _MakeRgb16(r, g, b);

			yy = g_iYYTab[pbSrcY[iSrcYStride]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) (pbDstX + iDstXStride))[0] = _MakeRgb16(r, g, b);

			yy = g_iYYTab[pbSrcY[iSrcYStride + 1]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) (pbDstX + iDstXStride))[1] = _MakeRgb16(r, g, b);

			pbDstX += 4;
			pbSrcY += 2;
			pbSrcU++;
			pbSrcV++;
		}


		pbDstX += iDstXDif + iDstXStride;
		pbSrcY += iSrcYDif + iSrcYStride;
		pbSrcU += iSrcUvDif;
		pbSrcV += iSrcUvDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB15.
VOID Yv12ToRgb15_sse2(PBYTE pbDstX, INT iDstXStride,
					  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					  UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv, r, g, b;

	M128I y0, y1, u0, v0, mz;
	M128I r0, g0, b0, r1, g1, b1;
	M128I bu0, gu0, gv0, rv0, bu1, rv1, guv0, guv1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	mz = _mm_setzero_si128();
	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 16; x; x--)
		{
			_mm_prefetch(pbSrcV + 8, _MM_HINT_NTA);
			_mm_prefetch(pbSrcU + 8, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcY + iSrcYStride + 16, _MM_HINT_NTA);

			// Calculate coefficient.
			u0 = _mm_loadl_epi64((PM128I) pbSrcU);		// [	| u3 u2 u1 u0]
			v0 = _mm_loadl_epi64((PM128I) pbSrcV);		// [	| v3 v2 v1 v0]

			u0 = _mm_unpacklo_epi8(u0, mz);				// u3 u2 u1 u0
			v0 = _mm_unpacklo_epi8(v0, mz);				// v3 v2 v1 v0
			u0 = _mm_subs_epi16(u0, g_xiSub80);
			v0 = _mm_subs_epi16(v0, g_xiSub80);

			gu0 = _mm_mullo_epi16(u0, g_xiUGMul);
			gv0 = _mm_mullo_epi16(v0, g_xiVGMul);
			bu0 = _mm_mullo_epi16(u0, g_xiUBMul);
			rv0 = _mm_mullo_epi16(v0, g_xiVRMul);

			guv0 = _mm_adds_epi16(gu0, gv0);

			guv1 = _mm_unpackhi_epi16(guv0, guv0);		// guv3 guv3 guv2 guv2
			guv0 = _mm_unpacklo_epi16(guv0, guv0);		// guv1 guv1 guv0 guv0

			bu1 = _mm_unpackhi_epi16(bu0, bu0);			// bu3 bu3 bu2 bu2
			bu0 = _mm_unpacklo_epi16(bu0, bu0);			// bu1 bu1 bu0 bu0
			rv1 = _mm_unpackhi_epi16(rv0, rv0);			// rv3 rv3 rv2 rv2
			rv0 = _mm_unpacklo_epi16(rv0, rv0);			// rv1 rv1 rv0 rv0

			// Process for row 0.
			y0 = _mm_loadu_si128((PM128I) pbSrcY);		// [YYYY | YYYY]; row 0

			y1 = _mm_unpackhi_epi8(y0, mz);				// y7 y6 y5 y4
			y0 = _mm_unpacklo_epi8(y0, mz);				// y3 y2 y1 y0
			y1 = _mm_subs_epi16(y1, g_xiSub10);
			y0 = _mm_subs_epi16(y0, g_xiSub10);
			y1 = _mm_mullo_epi16(y1, g_xiYYMul);
			y0 = _mm_mullo_epi16(y0, g_xiYYMul);

			b1 = _mm_adds_epi16(y1, bu1);
			b0 = _mm_adds_epi16(y0, bu0);
			b1 = _mm_srai_epi16(b1, SCALEBITS);			// 8 bits (0 - 7)
			b0 = _mm_srai_epi16(b0, SCALEBITS);

			b1 = _mm_packus_epi16(b1, mz);				// 0 0 0 0 b7 b6 b5 b4
			b0 = _mm_packus_epi16(b0, mz);				// 0 0 0 0 b3 b2 b1 b0
			b1 = _mm_unpacklo_epi8(b1, mz);				// 0 b7 0b 6 0 b5 0 b4
			b0 = _mm_unpacklo_epi8(b0, mz);

			b1 = _mm_srli_epi16(b1, 3);
			b0 = _mm_srli_epi16(b0, 3);					// 5 bits (0 - 4)

			g1 = _mm_subs_epi16(y1, guv1);				// g7 g6 g5 g4
			g0 = _mm_subs_epi16(y0, guv0);				// g3 g2 g1 g0
			g1 = _mm_srai_epi16(g1, SCALEBITS);			// 8 bits (0 - 7)
			g0 = _mm_srai_epi16(g0, SCALEBITS);

			g1 = _mm_packus_epi16(g1, mz);				// 0 0 0 0 g7 g6 g5 g4
			g0 = _mm_packus_epi16(g0, mz);				// 0 0 0 0 g3 g2 g1 g0
			g1 = _mm_unpacklo_epi8(g1, mz);				// 0 g7 0 g6 0 g5 0 g4
			g0 = _mm_unpacklo_epi8(g0, mz);

			g1 = _mm_srli_epi16(g1, 3);					// 5 bits (0 - 4)
			g0 = _mm_srli_epi16(g0, 3);
			g1 = _mm_slli_epi16(g1, 5);					// 5 bits (5 - 9)
			g0 = _mm_slli_epi16(g0, 5);					// 5 bits (5 - 9)

			r1 = _mm_adds_epi16(y1, rv1);
			r0 = _mm_adds_epi16(y0, rv0);
			r1 = _mm_srai_epi16(r1, SCALEBITS);
			r0 = _mm_srai_epi16(r0, SCALEBITS);

			r1 = _mm_packus_epi16(r1, mz);				// 0 0 0 0 r7 r6 r5 r4
			r0 = _mm_packus_epi16(r0, mz);				// 0 0 0 0 r3 r2 r1 r0
			r1 = _mm_unpacklo_epi8(r1, mz);				// 0 r7 0 r6 0 r5 0 r4
			r0 = _mm_unpacklo_epi8(r0, mz);

			r1 = _mm_srli_epi16(r1, 3);					// 5 bits (0 - 4)
			r0 = _mm_srli_epi16(r0, 3);
			r1 = _mm_slli_epi16(r1, 10);				// 5 bits (10 - 14)
			r0 = _mm_slli_epi16(r0, 10);				// 5 bits (10 - 14)

			// Combine RGB.
			b0 = _mm_or_si128(g0, b0);
			b0 = _mm_or_si128(r0, b0);					// 16 bits rgb
			b1 = _mm_or_si128(g1, b1);
			b1 = _mm_or_si128(r1, b1);					// 16 bits rgb


			// Write out row 0.
			_mm_storeu_si128(((PM128I) pbDstX) + 0, b0);
			_mm_storeu_si128(((PM128I) pbDstX) + 1, b1);

			// Process for row 1.
			y0 = _mm_loadu_si128((PM128I) (pbSrcY + iSrcYStride));		// [YYYY | YYYY]; row 0

			y1 = _mm_unpackhi_epi8(y0, mz);				// y7 y6 y5 y4
			y0 = _mm_unpacklo_epi8(y0, mz);				// y3 y2 y1 y0
			y1 = _mm_subs_epi16(y1, g_xiSub10);
			y0 = _mm_subs_epi16(y0, g_xiSub10);
			y1 = _mm_mullo_epi16(y1, g_xiYYMul);
			y0 = _mm_mullo_epi16(y0, g_xiYYMul);

			b1 = _mm_adds_epi16(y1, bu1);
			b0 = _mm_adds_epi16(y0, bu0);
			b1 = _mm_srai_epi16(b1, SCALEBITS);			// 8 bits (0 - 7)
			b0 = _mm_srai_epi16(b0, SCALEBITS);

			b1 = _mm_packus_epi16(b1, mz);				// 0 0 0 0 b7 b6 b5 b4
			b0 = _mm_packus_epi16(b0, mz);				// 0 0 0 0 b3 b2 b1 b0
			b1 = _mm_unpacklo_epi8(b1, mz);				// 0 b7 0b 6 0 b5 0 b4
			b0 = _mm_unpacklo_epi8(b0, mz);

			b1 = _mm_srli_epi16(b1, 3);
			b0 = _mm_srli_epi16(b0, 3);					// 5 bits (0 - 4)

			g1 = _mm_subs_epi16(y1, guv1);				// g7 g6 g5 g4
			g0 = _mm_subs_epi16(y0, guv0);				// g3 g2 g1 g0
			g1 = _mm_srai_epi16(g1, SCALEBITS);			// 8 bits (0 - 7)
			g0 = _mm_srai_epi16(g0, SCALEBITS);

			g1 = _mm_packus_epi16(g1, mz);				// 0 0 0 0 g7 g6 g5 g4
			g0 = _mm_packus_epi16(g0, mz);				// 0 0 0 0 g3 g2 g1 g0
			g1 = _mm_unpacklo_epi8(g1, mz);				// 0 g7 0 g6 0 g5 0 g4
			g0 = _mm_unpacklo_epi8(g0, mz);

			g1 = _mm_srli_epi16(g1, 3);					// 5 bits (0 - 4)
			g0 = _mm_srli_epi16(g0, 3);
			g1 = _mm_slli_epi16(g1, 5);					// 5 bits (5 - 9)
			g0 = _mm_slli_epi16(g0, 5);					// 5 bits (5 - 9)

			r1 = _mm_adds_epi16(y1, rv1);
			r0 = _mm_adds_epi16(y0, rv0);
			r1 = _mm_srai_epi16(r1, SCALEBITS);
			r0 = _mm_srai_epi16(r0, SCALEBITS);

			r1 = _mm_packus_epi16(r1, mz);				// 0 0 0 0 r7 r6 r5 r4
			r0 = _mm_packus_epi16(r0, mz);				// 0 0 0 0 r3 r2 r1 r0
			r1 = _mm_unpacklo_epi8(r1, mz);				// 0 r7 0 r6 0 r5 0 r4
			r0 = _mm_unpacklo_epi8(r0, mz);

			r1 = _mm_srli_epi16(r1, 3);					// 5 bits (0 - 4)
			r0 = _mm_srli_epi16(r0, 3);
			r1 = _mm_slli_epi16(r1, 10);				// 5 bits (10 - 14)
			r0 = _mm_slli_epi16(r0, 10);				// 5 bits (10 - 14)

			// Combine RGB.
			b0 = _mm_or_si128(g0, b0);
			b0 = _mm_or_si128(r0, b0);					// 16 bits rgb
			b1 = _mm_or_si128(g1, b1);
			b1 = _mm_or_si128(r1, b1);					// 16 bits rgb

			// Write out row 1.
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 0, b0);
			_mm_storeu_si128(((PM128I) (pbDstX + iDstXStride)) + 1, b1);

			pbDstX += 32;
			pbSrcY += 16;
			pbSrcU += 8;
			pbSrcV += 8;
		}

		for (x = (uWidth & 15) / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcU[0]];
			guv = g_iGUTab[pbSrcU[0]] + g_iGVTab[pbSrcV[0]];
			rv = g_iRVTab[pbSrcV[0]];

			yy = g_iYYTab[pbSrcY[0]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) pbDstX)[0] = _MakeRgb15(r, g, b);

			yy = g_iYYTab[pbSrcY[1]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) pbDstX)[1] = _MakeRgb15(r, g, b);

			yy = g_iYYTab[pbSrcY[iSrcYStride]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) (pbDstX + iDstXStride))[0] = _MakeRgb15(r, g, b);

			yy = g_iYYTab[pbSrcY[iSrcYStride + 1]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) (pbDstX + iDstXStride))[1] = _MakeRgb15(r, g, b);

			pbDstX += 4;
			pbSrcY += 2;
			pbSrcU++;
			pbSrcV++;
		}

		pbDstX += iDstXDif + iDstXStride;
		pbSrcY += iSrcYDif + iSrcYStride;
		pbSrcU += iSrcUvDif;
		pbSrcV += iSrcUvDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
