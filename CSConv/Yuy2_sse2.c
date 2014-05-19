


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"

#ifdef _SSE2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YV12.
VOID Yuy2ToYv12_sse2(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
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
			xi4 = _mm_srli_epi16(xi4, 8);
			xi5 = _mm_srli_epi16(xi5, 8);

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
			pbDstY[0] = pbSrcX[0];
			pbDstU[0] = (pbSrcX[1] + pbSrcX[iSrcXStride + 1] + 1) / 2;
			pbDstY[1] = pbSrcX[2];
			pbDstV[0] = (pbSrcX[3] + pbSrcX[iSrcXStride + 3] + 1) / 2;

			pbDstY[iDstYStride + 0] = pbSrcX[iSrcXStride + 0];
			pbDstY[iDstYStride + 1] = pbSrcX[iSrcXStride + 2];

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
// Convert YUY2 to YUY2.
VOID Yuy2ToYuy2_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopy_sse2(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YVYU.
VOID Yuy2ToYvyu_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to UYVY.
VOID Yuy2ToUyvy_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	PM128I p, q;
	M128I xi0, xi1, xi2, xi3, xi4, xi5, xi6, xi7;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	uWidth *= 2;
	for (y = iHeight; y; y--)
	{
		p = (PM128I) pbSrcX;
		q = (PM128I) pbDstX;
		for (x = uWidth / 64; x; x--)
		{
			xi0 = _mm_loadu_si128(p + 0);
			xi1 = _mm_loadu_si128(p + 1);
			xi2 = _mm_loadu_si128(p + 2);
			xi3 = _mm_loadu_si128(p + 3);

			xi4 = xi0;
			xi5 = xi1;
			xi6 = xi2;
			xi7 = xi3;

			xi0 = _mm_slli_epi16(xi0, 8);
			xi1 = _mm_slli_epi16(xi1, 8);
			xi2 = _mm_slli_epi16(xi2, 8);
			xi3 = _mm_slli_epi16(xi3, 8);

			xi4 = _mm_srli_epi16(xi4, 8);
			xi5 = _mm_srli_epi16(xi5, 8);
			xi6 = _mm_srli_epi16(xi6, 8);
			xi7 = _mm_srli_epi16(xi7, 8);

			xi0 = _mm_or_si128(xi0, xi4);
			xi1 = _mm_or_si128(xi1, xi5);
			xi2 = _mm_or_si128(xi2, xi6);
			xi3 = _mm_or_si128(xi3, xi7);

			_mm_storeu_si128(q + 0, xi0);
			_mm_storeu_si128(q + 1, xi1);
			_mm_storeu_si128(q + 2, xi2);
			_mm_storeu_si128(q + 3, xi3);

			p += 4;
			q += 4;
		}

		for (x = uWidth & 63; x; x--)
		{
			((PBYTE) q)[0] = ((PBYTE) p)[1];
			((PBYTE) q)[1] = ((PBYTE) p)[0];
			((PBYTE) q)[2] = ((PBYTE) p)[3];
			((PBYTE) q)[3] = ((PBYTE) p)[2];

			((PBYTE) p) += 4;
			((PBYTE) q) += 4;
		}

		pbSrcX += iSrcXStride;
		pbDstX += iDstXStride;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to VYUY.
VOID Yuy2ToVyuy_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB32.
VOID Yuy2ToRgb32_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;

	M128I y0, y1, u0, v0, uv_temp, mz;
	M128I r0, g0, b0, r1, g1, b1;
	M128I rgb0, rgb1, rgb2, rgb3;
	M128I bu0, gu0, gv0, rv0, bu1, rv1, guv0, guv1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 4);
	iSrcXDif = iSrcXStride - (uWidth * 2);

	mz = _mm_setzero_si128();
	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 16; x; x--)
		{
			_mm_prefetch(pbSrcX + 32, _MM_HINT_NTA);

			y0 = _mm_loadu_si128((PM128I) pbSrcX);
			y1 = _mm_loadu_si128((PM128I) (pbSrcX + 16));

			u0 = y0;
			v0 = y1;

			y0 = _mm_and_si128(y0, g_xiWord00FF);
			y1 = _mm_and_si128(y1, g_xiWord00FF);

			u0 = _mm_srli_epi16(u0, 8);
			v0 = _mm_srli_epi16(v0, 8);

			u0 = _mm_shufflelo_epi16(u0, 0xD8);
			u0 = _mm_shufflehi_epi16(u0, 0xD8);
			u0 = _mm_shuffle_epi32(u0, 0xD8);

			v0 = _mm_shufflelo_epi16(v0, 0xD8);
			v0 = _mm_shufflehi_epi16(v0, 0xD8);
			v0 = _mm_shuffle_epi32(v0, 0xD8);

			uv_temp = u0;
			u0 = _mm_unpacklo_epi64(u0, v0);
			v0 = _mm_unpackhi_epi64(uv_temp, v0);

			// Calculate coefficient.
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


			pbDstX += 64;
			pbSrcX += 32;
		}

		for (x = (uWidth & 15) / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcX[1]];
			guv = g_iGUTab[pbSrcX[1]] + g_iGVTab[pbSrcX[3]];
			rv = g_iRVTab[pbSrcX[3]];

			yy = g_iYYTab[pbSrcX[0]];
			pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);
			pbDstX[3] = 0;

			yy = g_iYYTab[pbSrcX[2]];
			pbDstX[4] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[5] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[6] = _Clip((yy + rv) >> SCALEBITS_OUT);
			pbDstX[7] = 0;

			pbDstX += 8;
			pbSrcX += 4;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB24.
VOID Yuy2ToRgb24_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;

	M128I y0, y1, u0, v0, uv_temp, mz;
	M128I r0, g0, b0, r1, g1, b1;
	M128I rgb0, rgb1, rgb2, rgb3;
	M128I bu0, gu0, gv0, rv0, bu1, rv1, guv0, guv1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 3);
	iSrcXDif = iSrcXStride - (uWidth * 2);

	mz = _mm_setzero_si128();
	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 16; x; x--)
		{
			_mm_prefetch(pbSrcX + 32, _MM_HINT_NTA);

			y0 = _mm_loadu_si128((PM128I) pbSrcX);
			y1 = _mm_loadu_si128((PM128I) (pbSrcX + 16));

			u0 = y0;
			v0 = y1;

			y0 = _mm_and_si128(y0, g_xiWord00FF);
			y1 = _mm_and_si128(y1, g_xiWord00FF);

			u0 = _mm_srli_epi16(u0, 8);
			v0 = _mm_srli_epi16(v0, 8);

			u0 = _mm_shufflelo_epi16(u0, 0xD8);
			u0 = _mm_shufflehi_epi16(u0, 0xD8);
			u0 = _mm_shuffle_epi32(u0, 0xD8);

			v0 = _mm_shufflelo_epi16(v0, 0xD8);
			v0 = _mm_shufflehi_epi16(v0, 0xD8);
			v0 = _mm_shuffle_epi32(v0, 0xD8);

			uv_temp = u0;
			u0 = _mm_unpacklo_epi64(u0, v0);
			v0 = _mm_unpackhi_epi64(uv_temp, v0);
			// Calculate coefficient.
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


			pbDstX += 48;
			pbSrcX += 32;
		}

		for (x = (uWidth & 15) / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcX[1]];
			guv = g_iGUTab[pbSrcX[1]] + g_iGVTab[pbSrcX[3]];
			rv = g_iRVTab[pbSrcX[3]];

			yy = g_iYYTab[pbSrcX[0]];
			pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);

			yy = g_iYYTab[pbSrcX[2]];
			pbDstX[3] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[4] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[5] = _Clip((yy + rv) >> SCALEBITS_OUT);

			pbDstX += 6;
			pbSrcX += 4;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB16.
VOID Yuy2ToRgb16_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB15.
VOID Yuy2ToRgb15_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
