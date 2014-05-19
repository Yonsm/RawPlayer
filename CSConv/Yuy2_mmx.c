


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"

#ifdef _MMX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YV12.
VOID Yuy2ToYv12_mmx(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
					PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstYDif;
	INT iDstUvDif;
	M64 m0, m1, m2, m3, m4, m5, m6, m7;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 2);
	iDstYDif = iDstYStride - uWidth;
	iDstUvDif = iDstUvStride - (uWidth / 2);

	m7 = g_mWord00FF;
	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 8; x; x--)
		{
			m0 = ((PM64) pbSrcX)[0];
			m1 = ((PM64) pbSrcX)[1];
			m2 = ((PM64) (pbSrcX + iSrcXStride))[0];
			m3 = ((PM64) (pbSrcX + iSrcXStride))[1];

			m4 = m0;
			m5 = m2;

			m4 = _mm_srli_pi16(m4, 8);
			m5 = _mm_srli_pi16(m5, 8);

			m4 = _mm_and_si64(m4, m7);
			m5 = _mm_and_si64(m5, m7);

			m4 = _mm_add_pi16(m4, m5);

			m5 = m1;
			m6 = m3;

			m5 = _mm_srli_pi16(m5, 8);
			m6 = _mm_srli_pi16(m6, 8);
			m5 = _mm_and_si64(m5, m7);
			m6 = _mm_and_si64(m6, m7);

			m5 = _mm_add_pi16(m5, m6);

			m4 = _mm_add_pi16(m4, g_mWord0001);
			m5 = _mm_add_pi16(m5, g_mWord0001);

			m4 = _mm_srli_pi16(m4, 1);
			m5 = _mm_srli_pi16(m5, 1);

			m0 = _mm_and_si64(m0, m7);
			m1 = _mm_and_si64(m1, m7);
			m2 = _mm_and_si64(m2, m7);
			m3 = _mm_and_si64(m3, m7);

			m0 = _mm_packs_pu16(m0, m1);
			m2 = _mm_packs_pu16(m2, m3);

			((PM64) pbDstY)[0] = m0;
			((PM64) (pbDstY + iDstYStride))[0] = m2;

			m4 = _mm_packs_pu16(m4, m5);
			m5 = m4;

			m4 = _mm_srli_si64(m4, 8);

			m5 = _mm_and_si64(m5, m7);
			m4 = _mm_and_si64(m4, m7);

			m5 = _mm_packs_pu16(m5, m5);
			m4 = _mm_packs_pu16(m4, m4);

			((PDWORD) pbDstU)[0] = _mm_cvtsi64_si32(m5);
			((PDWORD) pbDstV)[0] = _mm_cvtsi64_si32(m4);

			pbSrcX += 16;
			pbDstY += 8;
			pbDstU += 4;
			pbDstV += 4;
		}

		for (x = (uWidth & 7) / 2; x; x--)
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

	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YUY2.
VOID Yuy2ToYuy2_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopy_mmx(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YVYU.
VOID Yuy2ToYvyu_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to UYVY.
VOID Yuy2ToUyvy_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	PM64 p, q;
	M64 m0, m1, m2, m3, m4, m5, m6, m7;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	uWidth *= 2;
	for (y = iHeight; y; y--)
	{
		p = (PM64) pbSrcX;
		q = (PM64) pbDstX;
		for (x = uWidth / 32; x; x--)
		{
			m0 = p[0]; m1 = p[1]; m2 = p[2]; m3 = p[3];
			m4 = m0; m5 = m1; m6 = m2; m7 = m3;

			m0 = _mm_slli_pi16(m0, 8);
			m1 = _mm_slli_pi16(m1, 8);
			m2 = _mm_slli_pi16(m2, 8);
			m3 = _mm_slli_pi16(m3, 8);

			m4 = _mm_srli_pi16(m4, 8);
			m5 = _mm_srli_pi16(m5, 8);
			m6 = _mm_srli_pi16(m6, 8);
			m7 = _mm_srli_pi16(m7, 8);

			m0 = _mm_or_si64(m0, m4);
			m1 = _mm_or_si64(m1, m5);
			m2 = _mm_or_si64(m2, m6);
			m3 = _mm_or_si64(m3, m7);

			q[0] = m0; q[1] = m1; q[2] = m2; q[3] = m3;
			p += 4;
			q += 4;
		}

		for (x = uWidth & 31; x; x--)
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

	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to VYUY.
VOID Yuy2ToVyuy_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB32.
VOID Yuy2ToRgb32_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;

	M64 y0, y1, u0, v0, uv_temp1, uv_temp2, mz;
	M64 r0, g0, b0, r1, g1, b1;
	M64 rgb0, rgb1, rgb2, rgb3;
	M64 bu0, gu0, gv0, rv0, bu1, rv1, guv0, guv1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 4);
	iSrcXDif = iSrcXStride - (uWidth * 2);

	mz = _mm_setzero_si64();
	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 8; x; x--)
		{
			y0 = ((PM64) pbSrcX)[0];
			y1 = ((PM64) pbSrcX)[1];

			u0 = y0;
			v0 = y1;

			y0 = _mm_and_si64(y0, g_mWord00FF);
			y1 = _mm_and_si64(y1, g_mWord00FF);

			u0 = _mm_srli_pi16(u0, 8);
			v0 = _mm_srli_pi16(v0, 8);

			uv_temp1 = _mm_srli_pi32(u0, 16);
			u0 = _mm_slli_pi32(u0, 16);
			u0 = _mm_srli_pi32(u0, 16);

			uv_temp2 = _mm_srli_pi32(v0, 16);
			v0 = _mm_slli_pi32(v0, 16);
			v0 = _mm_srli_pi32(v0, 16);

			u0 = _mm_packs_pi32(u0, v0);
			v0 = _mm_packs_pi32(uv_temp1, uv_temp2);

			// Calculate coefficient.
			u0 = _mm_subs_pi16(u0, g_mSub80);
			v0 = _mm_subs_pi16(v0, g_mSub80);

			gu0 = _mm_mullo_pi16(u0, g_mUGMul);
			gv0 = _mm_mullo_pi16(v0, g_mVGMul);
			bu0 = _mm_mullo_pi16(u0, g_mUBMul);
			rv0 = _mm_mullo_pi16(v0, g_mVRMul);

			guv0 = _mm_adds_pi16(gu0, gv0);

			guv1 = _mm_unpackhi_pi16(guv0, guv0);		// guv3 guv3 guv2 guv2
			guv0 = _mm_unpacklo_pi16(guv0, guv0);		// guv1 guv1 guv0 guv0

			bu1 = _mm_unpackhi_pi16(bu0, bu0);			// bu3 bu3 bu2 bu2
			bu0 = _mm_unpacklo_pi16(bu0, bu0);			// bu1 bu1 bu0 bu0
			rv1 = _mm_unpackhi_pi16(rv0, rv0);			// rv3 rv3 rv2 rv2
			rv0 = _mm_unpacklo_pi16(rv0, rv0);			// rv1 rv1 rv0 rv0

			// Process for row 0.
			y1 = _mm_subs_pi16(y1, g_mSub10);
			y0 = _mm_subs_pi16(y0, g_mSub10);
			y1 = _mm_mullo_pi16(y1, g_mYYMul);
			y0 = _mm_mullo_pi16(y0, g_mYYMul);

			g1 = _mm_subs_pi16(y1, guv1);				// g7 g6 g5 g4
			g0 = _mm_subs_pi16(y0, guv0);				// g3 g2 g1 g0
			g1 = _mm_srai_pi16(g1, SCALEBITS);
			g0 = _mm_srai_pi16(g0, SCALEBITS);
			g0 = _mm_packs_pu16(g0, g1);				// g7 g6 ...g1 g0

			b1 = _mm_adds_pi16(y1, bu1);
			b0 = _mm_adds_pi16(y0, bu0);
			b1 = _mm_srai_pi16(b1, SCALEBITS);
			b0 = _mm_srai_pi16(b0, SCALEBITS);
			b0 = _mm_packs_pu16(b0, b1);

			r1 = _mm_adds_pi16(y1, rv1);
			r0 = _mm_adds_pi16(y0, rv0);
			r1 = _mm_srai_pi16(r1, SCALEBITS);
			r0 = _mm_srai_pi16(r0, SCALEBITS);
			r0 = _mm_packs_pu16(r0, r1);

			r1 = _mm_unpackhi_pi8(b0, r0);				// r7 b7 r6 b6 r5 b5 r4 b4
			r0 = _mm_unpacklo_pi8(b0, r0);				// r3 b3 r2 b2 r1 b1 r0 b0

			g1 = _mm_unpackhi_pi8(g0, mz);				// 0 g7 0 g6 0 g5 0 g4
			g0 = _mm_unpacklo_pi8(g0, mz);				// 0 g3 0 g2 0 g1 0 g0

			rgb0 = _mm_unpacklo_pi8(r0, g0);			// 0 r1 g1 b1 0 r0 g0 b0
			rgb1 = _mm_unpackhi_pi8(r0, g0);			// 0 r3 g3 b3 0 r2 g2 b2
			rgb2 = _mm_unpacklo_pi8(r1, g1);			// 0 r5 g5 b5 0 r4 g4 b4
			rgb3 = _mm_unpackhi_pi8(r1, g1);			// 0 r7 g7 b7 0 r6 g6 b6

			// Write out row 0.
			((PM64) pbDstX)[0] = rgb0;
			((PM64) pbDstX)[1] = rgb1;
			((PM64) pbDstX)[2] = rgb2;
			((PM64) pbDstX)[3] = rgb3;


			pbDstX += 32;
			pbSrcX += 16;
		}

		for (x = (uWidth & 7) / 2; x; x--)
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

	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB24.
VOID Yuy2ToRgb24_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;

	M64 y0, y1, u0, v0, uv_temp1, uv_temp2, mz;
	M64 r0, g0, b0, r1, g1, b1;
	M64 rgb0, rgb1, rgb2, rgb3;
	M64 bu0, gu0, gv0, rv0, bu1, rv1, guv0, guv1;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 3);
	iSrcXDif = iSrcXStride - (uWidth * 2);

	mz = _mm_setzero_si64();
	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 8; x; x--)
		{
			y0 = ((PM64) pbSrcX)[0];
			y1 = ((PM64) pbSrcX)[1];

			u0 = y0;
			v0 = y1;

			y0 = _mm_and_si64(y0, g_mWord00FF);
			y1 = _mm_and_si64(y1, g_mWord00FF);

			u0 = _mm_srli_pi16(u0, 8);
			v0 = _mm_srli_pi16(v0, 8);

			uv_temp1 = _mm_srli_pi32(u0, 16);
			u0 = _mm_slli_pi32(u0, 16);
			u0 = _mm_srli_pi32(u0, 16);

			uv_temp2 = _mm_srli_pi32(v0, 16);
			v0 = _mm_slli_pi32(v0, 16);
			v0 = _mm_srli_pi32(v0, 16);

			u0 = _mm_packs_pi32(u0, v0);
			v0 = _mm_packs_pi32(uv_temp1, uv_temp2);
			// Calculate coefficient.
			u0 = _mm_subs_pi16(u0, g_mSub80);
			v0 = _mm_subs_pi16(v0, g_mSub80);

			gu0 = _mm_mullo_pi16(u0, g_mUGMul);
			gv0 = _mm_mullo_pi16(v0, g_mVGMul);
			bu0 = _mm_mullo_pi16(u0, g_mUBMul);
			rv0 = _mm_mullo_pi16(v0, g_mVRMul);

			guv0 = _mm_adds_pi16(gu0, gv0);

			guv1 = _mm_unpackhi_pi16(guv0, guv0);		// guv3 guv3 guv2 guv2
			guv0 = _mm_unpacklo_pi16(guv0, guv0);		// guv1 guv1 guv0 guv0

			bu1 = _mm_unpackhi_pi16(bu0, bu0);			// bu3 bu3 bu2 bu2
			bu0 = _mm_unpacklo_pi16(bu0, bu0);			// bu1 bu1 bu0 bu0
			rv1 = _mm_unpackhi_pi16(rv0, rv0);			// rv3 rv3 rv2 rv2
			rv0 = _mm_unpacklo_pi16(rv0, rv0);			// rv1 rv1 rv0 rv0

			// Process for row 0.
			y1 = _mm_subs_pi16(y1, g_mSub10);
			y0 = _mm_subs_pi16(y0, g_mSub10);
			y1 = _mm_mullo_pi16(y1, g_mYYMul);
			y0 = _mm_mullo_pi16(y0, g_mYYMul);

			g1 = _mm_subs_pi16(y1, guv1);				// g7 g6 g5 g4
			g0 = _mm_subs_pi16(y0, guv0);				// g3 g2 g1 g0
			g1 = _mm_srai_pi16(g1, SCALEBITS);
			g0 = _mm_srai_pi16(g0, SCALEBITS);
			g0 = _mm_packs_pu16(g0, g1);				// g7 g6 ...g1 g0

			b1 = _mm_adds_pi16(y1, bu1);
			b0 = _mm_adds_pi16(y0, bu0);
			b1 = _mm_srai_pi16(b1, SCALEBITS);
			b0 = _mm_srai_pi16(b0, SCALEBITS);
			b0 = _mm_packs_pu16(b0, b1);

			r1 = _mm_adds_pi16(y1, rv1);
			r0 = _mm_adds_pi16(y0, rv0);
			r1 = _mm_srai_pi16(r1, SCALEBITS);
			r0 = _mm_srai_pi16(r0, SCALEBITS);
			r0 = _mm_packs_pu16(r0, r1);

			r1 = _mm_unpackhi_pi8(b0, r0);				// r7 b7 r6 b6 r5 b5 r4 b4
			r0 = _mm_unpacklo_pi8(b0, r0);				// r3 b3 r2 b2 r1 b1 r0 b0

			g1 = _mm_unpackhi_pi8(g0, mz);				// 0 g7 0 g6 0 g5 0 g4
			g0 = _mm_unpacklo_pi8(g0, mz);				// 0 g3 0 g2 0 g1 0 g0

			rgb0 = _mm_unpacklo_pi8(r0, g0);			// 0 r1 g1 b1 0 r0 g0 b0
			rgb1 = _mm_unpackhi_pi8(r0, g0);			// 0 r3 g3 b3 0 r2 g2 b2
			rgb2 = _mm_unpacklo_pi8(r1, g1);			// 0 r5 g5 b5 0 r4 g4 b4
			rgb3 = _mm_unpackhi_pi8(r1, g1);			// 0 r7 g7 b7 0 r6 g6 b6

			// Write out row 0.
			*((PDWORD) (pbDstX + 0)) = _mm_cvtsi64_si32(rgb0); rgb0 = _mm_srli_si64(rgb0, 32);
			*((PDWORD) (pbDstX + 3)) = _mm_cvtsi64_si32(rgb0);
			*((PDWORD) (pbDstX + 6)) = _mm_cvtsi64_si32(rgb1); rgb1 = _mm_srli_si64(rgb1, 32);
			*((PDWORD) (pbDstX + 9)) = _mm_cvtsi64_si32(rgb1);
			*((PDWORD) (pbDstX + 12)) = _mm_cvtsi64_si32(rgb2); rgb2 = _mm_srli_si64(rgb2, 32);
			*((PDWORD) (pbDstX + 15)) = _mm_cvtsi64_si32(rgb2);
			*((PDWORD) (pbDstX + 18)) = _mm_cvtsi64_si32(rgb3); rgb3 = _mm_srli_si64(rgb3, 32);
			*((PDWORD) (pbDstX + 21)) = _mm_cvtsi64_si32(rgb3);

			pbDstX += 24;
			pbSrcX += 16;
		}

		for (x = (uWidth & 7) / 2; x; x--)
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

	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB16.
VOID Yuy2ToRgb16_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB15.
VOID Yuy2ToRgb15_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
