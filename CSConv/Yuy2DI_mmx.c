


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"

#ifdef _MMX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YV12 with de-interlacing.
#define Yuy2ToYv12(S00, S01, S02, S03, S10, S11, S12, S13)	\
{															\
	pbDstY[0] = S00;										\
	pbDstU[0] = (S01 + S11 + 1) / 2;						\
	pbDstY[1] = S02;										\
	pbDstV[0] = (S03 + S13 + 1) / 2;						\
	pbDstY[iDstYStride + 0] = S10;							\
	pbDstY[iDstYStride + 1] = S12;							\
}

VOID Yuy2ToYv12DI_mmx(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride, 
				PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstYDif;
	INT iDstUvDif;
	M64 m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, mMASK, mMaskAvg;
	BYTE bSrc00, bSrc01, bSrc02, bSrc03, bSrc10, bSrc11, bSrc12, bSrc13;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 2);
	iDstYDif = iDstYStride - uWidth;
	iDstUvDif = iDstUvStride - (uWidth / 2);

	mMASK = g_mWord00FF;
	mMaskAvg = g_mMaskAvg;

	for (x = uWidth / 8; x; x--)
	{
		m0 = ((PM64) pbSrcX)[0];
		m1 = ((PM64) pbSrcX)[1];
		m2 = ((PM64) (pbSrcX + iSrcXStride))[0];
		m3 = ((PM64) (pbSrcX + iSrcXStride))[1];
		m4 = ((PM64) (pbSrcX + iSrcXStride * 2))[0];
		m5 = ((PM64) (pbSrcX + iSrcXStride * 2))[1];

		m8 = m0;
		m9 = m1;

		m0 = _mm_and_si64(m0, mMaskAvg);
		m1 = _mm_and_si64(m1, mMaskAvg);
		m2 = _mm_and_si64(m2, mMaskAvg);
		m3 = _mm_and_si64(m3, mMaskAvg);
		m4 = _mm_and_si64(m4, mMaskAvg);
		m5 = _mm_and_si64(m5, mMaskAvg);
		m0 = _mm_srli_si64(m0, 1);
		m1 = _mm_srli_si64(m1, 1);
		m2 = _mm_srli_si64(m2, 1);
		m3 = _mm_srli_si64(m3, 1);
		m4 = _mm_srli_si64(m4, 1);
		m5 = _mm_srli_si64(m5, 1);
		m4 = _mm_adds_pu8(m4, m0);
		m5 = _mm_adds_pu8(m5, m1);
		m4 = _mm_and_si64(m4, mMaskAvg);
		m5 = _mm_and_si64(m5, mMaskAvg);
		m4 = _mm_srli_si64(m4, 1);
		m5 = _mm_srli_si64(m5, 1);
		m4 = _mm_adds_pu8(m4, m2);
		m5 = _mm_adds_pu8(m5, m3);

		m2 = m4;
		m3 = m5;
		m4 = _mm_and_si64(m4, mMaskAvg);
		m5 = _mm_and_si64(m5, mMaskAvg);
		m4 = _mm_srli_si64(m4, 1);
		m5 = _mm_srli_si64(m5, 1);
		m4 = _mm_adds_pu8(m4, m0);
		m5 = _mm_adds_pu8(m5, m1);


		m4 = _mm_srli_pi16(m4, 8);
		m5 = _mm_srli_pi16(m5, 8);

		m8 = _mm_and_si64(m8, mMASK);
		m9 = _mm_and_si64(m9, mMASK);
		m2 = _mm_and_si64(m2, mMASK);
		m3 = _mm_and_si64(m3, mMASK);

		m8 = _mm_packs_pu16(m8, m8);
		m2 = _mm_packs_pu16(m2, m3);

		((PM64) pbDstY)[0] = m8;
		((PM64) (pbDstY + iDstYStride))[0] = m2;

		m4 = _mm_packs_pu16(m4, m5);
		m5 = m4;

		m4 = _mm_srli_pi16(m4, 8);

		m5 = _mm_and_si64(m5, mMASK);
		m4 = _mm_and_si64(m4, mMASK);

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
		bSrc10 = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
		bSrc11 = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
		bSrc12 = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
		bSrc13 = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;
		Yuy2ToYv12(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3], bSrc10, bSrc11, bSrc12, bSrc13);
		pbSrcX += 4;
		pbDstY += 2;
		pbDstU++;
		pbDstV++;
	}

	pbSrcX += iSrcXDif;
	pbDstY += iDstYDif + iDstYStride;
	pbDstU += iDstUvDif;
	pbDstV += iDstUvDif;
	for (y = iHeight / 2 - 2; y; y--)
	{
		for (x = uWidth / 8; x; x--)
		{
			m0 = ((PM64) pbSrcX)[0];
			m1 = ((PM64) pbSrcX)[1];
			m2 = ((PM64) (pbSrcX + iSrcXStride))[0];
			m3 = ((PM64) (pbSrcX + iSrcXStride))[1];
			m4 = ((PM64) (pbSrcX + iSrcXStride * 2))[0];
			m5 = ((PM64) (pbSrcX + iSrcXStride * 2))[1];
			m6 = ((PM64) (pbSrcX + iSrcXStride * 3))[0];
			m7 = ((PM64) (pbSrcX + iSrcXStride * 3))[1];

			m0 = _mm_and_si64(m0, mMaskAvg);
			m1 = _mm_and_si64(m1, mMaskAvg);
			m2 = _mm_and_si64(m2, mMaskAvg);
			m3 = _mm_and_si64(m3, mMaskAvg);
			m4 = _mm_and_si64(m4, mMaskAvg);
			m5 = _mm_and_si64(m5, mMaskAvg);
			m6 = _mm_and_si64(m6, mMaskAvg);
			m7 = _mm_and_si64(m7, mMaskAvg);
			m0 = _mm_srli_si64(m0, 1);
			m1 = _mm_srli_si64(m1, 1);
			m2 = _mm_srli_si64(m2, 1);
			m3 = _mm_srli_si64(m3, 1);
			m4 = _mm_srli_si64(m4, 1);
			m5 = _mm_srli_si64(m5, 1);
			m6 = _mm_srli_si64(m6, 1);
			m7 = _mm_srli_si64(m7, 1);
			m0 = _mm_adds_pu8(m4, m0);
			m1 = _mm_adds_pu8(m5, m1);
			m0 = _mm_and_si64(m0, mMaskAvg);
			m1 = _mm_and_si64(m1, mMaskAvg);
			m0 = _mm_srli_si64(m0, 1);
			m1 = _mm_srli_si64(m1, 1);
			m0 = _mm_adds_pu8(m0, m2);
			m1 = _mm_adds_pu8(m1, m3);

			m6 = _mm_adds_pu8(m6, m2);
			m7 = _mm_adds_pu8(m7, m3);
			m6 = _mm_and_si64(m6, mMaskAvg);
			m7 = _mm_and_si64(m7, mMaskAvg);
			m6 = _mm_srli_si64(m6, 1);
			m7 = _mm_srli_si64(m7, 1);
			m6 = _mm_adds_pu8(m6, m4);
			m7 = _mm_adds_pu8(m7, m5);
			m2 = m0;
			m3 = m1;
			m8 = m6;
			m9 = m7;

			m0 = _mm_and_si64(m0, mMaskAvg);
			m1 = _mm_and_si64(m1, mMaskAvg);
			m6 = _mm_and_si64(m6, mMaskAvg);
			m7 = _mm_and_si64(m7, mMaskAvg);
			m0 = _mm_srli_si64(m0, 1);
			m1 = _mm_srli_si64(m1, 1);
			m6 = _mm_srli_si64(m6, 1);
			m7 = _mm_srli_si64(m7, 1);
			m0 = _mm_adds_pu8(m0, m6);
			m1 = _mm_adds_pu8(m1, m7);

			m0 = _mm_srli_pi16(m0, 8);
			m1 = _mm_srli_pi16(m1, 8);

			m2 = _mm_and_si64(m2, mMASK);
			m3 = _mm_and_si64(m3, mMASK);
			m8 = _mm_and_si64(m8, mMASK);
			m9 = _mm_and_si64(m9, mMASK);

			m2 = _mm_packs_pu16(m2, m3);
			m8 = _mm_packs_pu16(m8, m9);

			((PM64) pbDstY)[0] = m2;
			((PM64) (pbDstY + iDstYStride))[0] = m8;

			m0 = _mm_packs_pu16(m0, m1);
			m1 = m0;

			m0 = _mm_srli_pi16(m0, 8);

			m1 = _mm_and_si64(m1, mMASK);
			m0 = _mm_and_si64(m0, mMASK);

			m1 = _mm_packs_pu16(m1, m1);
			m0 = _mm_packs_pu16(m0, m0);

			((PDWORD) pbDstU)[0] = _mm_cvtsi64_si32(m1);
			((PDWORD) pbDstV)[0] = _mm_cvtsi64_si32(m0);

			pbSrcX += 16;
			pbDstY += 8;
			pbDstU += 4;
			pbDstV += 4;
		}

		for (x = (uWidth & 7) / 2; x; x--)
		{
			bSrc00 = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
			bSrc01 = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
			bSrc02 = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
			bSrc03 = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;
			
			bSrc10 = ((pbSrcX[iSrcXStride + 0] + pbSrcX[iSrcXStride * 3 + 0] + 1) / 2 +
				pbSrcX[iSrcXStride * 2 + 0] + 1) / 2;
			bSrc11 = ((pbSrcX[iSrcXStride + 1] + pbSrcX[iSrcXStride * 3 + 1] + 1) / 2 +
				pbSrcX[iSrcXStride * 2 + 1] + 1) / 2;
			bSrc12 = ((pbSrcX[iSrcXStride + 2] + pbSrcX[iSrcXStride * 3 + 2] + 1) / 2 +
				pbSrcX[iSrcXStride * 2 + 2] + 1) / 2;
			bSrc13 = ((pbSrcX[iSrcXStride + 3] + pbSrcX[iSrcXStride * 3 + 3] + 1) / 2 +
				pbSrcX[iSrcXStride * 2 + 3] + 1) / 2;
			
			Yuy2ToYv12(bSrc10, bSrc11, bSrc12, bSrc13, bSrc10, bSrc11, bSrc12, bSrc13);

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
	for (x = uWidth / 8; x; x--)
	{
		m0 = ((PM64) pbSrcX)[0];
		m1 = ((PM64) pbSrcX)[1];
		m2 = ((PM64) (pbSrcX + iSrcXStride))[0];
		m3 = ((PM64) (pbSrcX + iSrcXStride))[1];
		m4 = ((PM64) (pbSrcX + iSrcXStride * 2))[0];
		m5 = ((PM64) (pbSrcX + iSrcXStride * 2))[1];
		m8 = m4;
		m9 = m5;

		m0 = _mm_and_si64(m0, mMaskAvg);
		m1 = _mm_and_si64(m1, mMaskAvg);
		m2 = _mm_and_si64(m2, mMaskAvg);
		m3 = _mm_and_si64(m3, mMaskAvg);
		m4 = _mm_and_si64(m4, mMaskAvg);
		m5 = _mm_and_si64(m5, mMaskAvg);
		m0 = _mm_srli_si64(m0, 1);
		m1 = _mm_srli_si64(m1, 1);
		m2 = _mm_srli_si64(m2, 1);
		m3 = _mm_srli_si64(m3, 1);
		m4 = _mm_srli_si64(m4, 1);
		m5 = _mm_srli_si64(m5, 1);
		m0 = _mm_adds_pu8(m4, m0);
		m1 = _mm_adds_pu8(m5, m1);
		m0 = _mm_and_si64(m0, mMaskAvg);
		m1 = _mm_and_si64(m1, mMaskAvg);
		m0 = _mm_srli_si64(m0, 1);
		m1 = _mm_srli_si64(m1, 1);
		m0 = _mm_adds_pu8(m0, m2);
		m1 = _mm_adds_pu8(m1, m3);
		m2 = m0;
		m3 = m1;
		m0 = _mm_and_si64(m0, mMaskAvg);
		m1 = _mm_and_si64(m1, mMaskAvg);
		m0 = _mm_srli_si64(m0, 1);
		m1 = _mm_srli_si64(m1, 1);
		m0 = _mm_adds_pu8(m4, m0);
		m1 = _mm_adds_pu8(m5, m1);
		m0 = _mm_srli_pi16(m0, 8);
		m1 = _mm_srli_pi16(m1, 8);

		m2 = _mm_and_si64(m2, mMASK);
		m3 = _mm_and_si64(m3, mMASK);
		m8 = _mm_and_si64(m8, mMASK);
		m9 = _mm_and_si64(m9, mMASK);

		m2 = _mm_packs_pu16(m2, m3);
		m8 = _mm_packs_pu16(m8, m9);

		((PM64) pbDstY)[0] = m2;
		((PM64) (pbDstY + iDstYStride))[0] = m8;

		m0 = _mm_packs_pu16(m0, m1);
		m1 = m0;

		m0 = _mm_srli_pi16(m0, 8);

		m1 = _mm_and_si64(m1, mMASK);
		m0 = _mm_and_si64(m0, mMASK);

		m1 = _mm_packs_pu16(m1, m1);
		m0 = _mm_packs_pu16(m0, m0);

		((PDWORD) pbDstU)[0] = _mm_cvtsi64_si32(m1);
		((PDWORD) pbDstV)[0] = _mm_cvtsi64_si32(m0);

		pbSrcX += 16;
		pbDstY += 8;
		pbDstU += 4;
		pbDstV += 4;
	}

	for (x = (uWidth & 7) / 2; x; x--)
	{
		bSrc10 = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
		bSrc11 = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
		bSrc12 = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
		bSrc13 = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;
		Yuy2ToYv12(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3], bSrc10, bSrc11, bSrc12, bSrc13);
		pbSrcX += 4;
		pbDstY += 2;
		pbDstU++;
		pbDstV++;
	}
	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YUY2 with de-interlacing.
VOID Yuy2ToYuy2DI_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopyDI_mmx(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YVYU with de-interlacing.
VOID Yuy2ToYvyuDI_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to UYVY with de-interlacing.
VOID Yuy2ToUyvyDI_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	PBYTE p, q;
	M64 xi00, xi01, xi02, xi03, xi04, xi05, xi06, xi07, mMaskAvg;
	M64 xi10, xi11, xi12, xi13;
	M64 xi20, xi21, xi22, xi23;
	
	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	uWidth *= 2;
	p = pbSrcX;
	q = pbDstX;
	mMaskAvg = g_mMaskAvg;

	for (x = uWidth / 32; x; x--)
	{
		xi00 = *(PM64) (p + 0);
		xi01 = *(PM64) (p + 8);
		xi02 = *(PM64) (p + 16);
		xi03 = *(PM64) (p + 24);

		xi04 = xi00;
		xi05 = xi01;
		xi06 = xi02;
		xi07 = xi03;

		xi00 = _mm_slli_pi16(xi00, 8);
		xi01 = _mm_slli_pi16(xi01, 8);
		xi02 = _mm_slli_pi16(xi02, 8);
		xi03 = _mm_slli_pi16(xi03, 8);

		xi04 = _mm_srli_pi16(xi04, 8);
		xi05 = _mm_srli_pi16(xi05, 8);
		xi06 = _mm_srli_pi16(xi06, 8);
		xi07 = _mm_srli_pi16(xi07, 8);

		xi00 = _mm_or_si64(xi00, xi04);
		xi01 = _mm_or_si64(xi01, xi05);
		xi02 = _mm_or_si64(xi02, xi06);
		xi03 = _mm_or_si64(xi03, xi07);

		*(PM64) (q + 0) = xi00;
		*(PM64) (q + 8) = xi01;
		*(PM64) (q + 16) = xi02;
		*(PM64) (q + 24) = xi03;

		p += 32;
		q += 32;
	}

	for (x = uWidth & 31; x; x--)
	{
		q[0] = p[1];
		q[1] = p[0];
		q[2] = p[3];
		q[3] = p[2];

		p += 4;
		q += 4;
	}
	pbDstX += iDstXStride;
	for (y = iHeight - 2 ; y; y--)
	{
		p = pbSrcX;
		q = pbDstX;
		for (x = uWidth / 32; x; x--)
		{
			xi00 = *(PM64) (p + 0);
			xi01 = *(PM64) (p + 8);
			xi02 = *(PM64) (p + 16);
			xi03 = *(PM64) (p + 24);

			xi10 = *(PM64) (p + iSrcXStride + 0);
			xi11 = *(PM64) (p + iSrcXStride + 8);
			xi12 = *(PM64) (p + iSrcXStride + 16);
			xi13 = *(PM64) (p + iSrcXStride + 24);

			xi20 = *(PM64) (p + iSrcXStride * 2 + 0);
			xi21 = *(PM64) (p + iSrcXStride * 2 + 8);
			xi22 = *(PM64) (p + iSrcXStride * 2 + 16);
			xi23 = *(PM64) (p + iSrcXStride * 2 + 24);

			xi00 = _mm_and_si64(xi00, mMaskAvg);
			xi01 = _mm_and_si64(xi01, mMaskAvg);
			xi02 = _mm_and_si64(xi02, mMaskAvg);
			xi03 = _mm_and_si64(xi03, mMaskAvg);
			xi10 = _mm_and_si64(xi10, mMaskAvg);
			xi11 = _mm_and_si64(xi11, mMaskAvg);
			xi12 = _mm_and_si64(xi12, mMaskAvg);
			xi13 = _mm_and_si64(xi13, mMaskAvg);
			xi20 = _mm_and_si64(xi10, mMaskAvg);
			xi21 = _mm_and_si64(xi11, mMaskAvg);
			xi22 = _mm_and_si64(xi12, mMaskAvg);
			xi23 = _mm_and_si64(xi13, mMaskAvg);
			xi00 = _mm_srli_si64(xi00, 1);
			xi01 = _mm_srli_si64(xi01, 1);
			xi02 = _mm_srli_si64(xi02, 1);
			xi03 = _mm_srli_si64(xi03, 1);
			xi10 = _mm_srli_si64(xi10, 1);
			xi11 = _mm_srli_si64(xi11, 1);
			xi12 = _mm_srli_si64(xi12, 1);
			xi13 = _mm_srli_si64(xi13, 1);
			xi20 = _mm_srli_si64(xi20, 1);
			xi21 = _mm_srli_si64(xi21, 1);
			xi22 = _mm_srli_si64(xi22, 1);
			xi23 = _mm_srli_si64(xi23, 1);
			xi00 = _mm_adds_pu8(xi00, xi20);
			xi01 = _mm_adds_pu8(xi01, xi21);
			xi02 = _mm_adds_pu8(xi02, xi22);
			xi03 = _mm_adds_pu8(xi03, xi23);
			xi00 = _mm_and_si64(xi00, mMaskAvg);
			xi01 = _mm_and_si64(xi01, mMaskAvg);
			xi02 = _mm_and_si64(xi02, mMaskAvg);
			xi03 = _mm_and_si64(xi03, mMaskAvg);
			xi00 = _mm_srli_si64(xi00, 1);
			xi01 = _mm_srli_si64(xi01, 1);
			xi02 = _mm_srli_si64(xi02, 1);
			xi03 = _mm_srli_si64(xi03, 1);
			xi00 = _mm_adds_pu8(xi00, xi10);
			xi01 = _mm_adds_pu8(xi01, xi11);
			xi02 = _mm_adds_pu8(xi02, xi12);
			xi03 = _mm_adds_pu8(xi03, xi13);

			xi04 = xi00;
			xi05 = xi01;
			xi06 = xi02;
			xi07 = xi03;

			xi00 = _mm_slli_pi16(xi00, 8);
			xi01 = _mm_slli_pi16(xi01, 8);
			xi02 = _mm_slli_pi16(xi02, 8);
			xi03 = _mm_slli_pi16(xi03, 8);

			xi04 = _mm_srli_pi16(xi04, 8);
			xi05 = _mm_srli_pi16(xi05, 8);
			xi06 = _mm_srli_pi16(xi06, 8);
			xi07 = _mm_srli_pi16(xi07, 8);

			xi00 = _mm_or_si64(xi00, xi04);
			xi01 = _mm_or_si64(xi01, xi05);
			xi02 = _mm_or_si64(xi02, xi06);
			xi03 = _mm_or_si64(xi03, xi07);

			*(PM64) (q + 0) = xi00;
			*(PM64) (q + 8) = xi01;
			*(PM64) (q + 16) = xi02;
			*(PM64) (q + 24) = xi03;

			p += 32;
			q += 32;
		}

		for (x = uWidth & 31; x; x--)
		{
			q[0] = ((p[1] + p[iSrcXStride * 2 + 1] + 1) / 2 + p[iSrcXStride + 1] + 1) / 2;
			q[1] = ((p[0] + p[iSrcXStride * 2 + 0] + 1) / 2 + p[iSrcXStride + 0] + 1) / 2;
			q[2] = ((p[3] + p[iSrcXStride * 2 + 3] + 1) / 2 + p[iSrcXStride + 3] + 1) / 2;
			q[3] = ((p[2] + p[iSrcXStride * 2 + 2] + 1) / 2 + p[iSrcXStride + 2] + 1) / 2;

			p += 4;
			q += 4;
		}
		pbSrcX += iSrcXStride;
		pbDstX += iDstXStride;
	}
	p = pbSrcX + iSrcXStride;
	q = pbDstX;
	for (x = uWidth / 32; x; x--)
	{
		xi00 = *(PM64) (p + 0);
		xi01 = *(PM64) (p + 8);
		xi02 = *(PM64) (p + 16);
		xi03 = *(PM64) (p + 24);

		xi04 = xi00;
		xi05 = xi01;
		xi06 = xi02;
		xi07 = xi03;

		xi00 = _mm_slli_pi16(xi00, 8);
		xi01 = _mm_slli_pi16(xi01, 8);
		xi02 = _mm_slli_pi16(xi02, 8);
		xi03 = _mm_slli_pi16(xi03, 8);

		xi04 = _mm_srli_pi16(xi04, 8);
		xi05 = _mm_srli_pi16(xi05, 8);
		xi06 = _mm_srli_pi16(xi06, 8);
		xi07 = _mm_srli_pi16(xi07, 8);

		xi00 = _mm_or_si64(xi00, xi04);
		xi01 = _mm_or_si64(xi01, xi05);
		xi02 = _mm_or_si64(xi02, xi06);
		xi03 = _mm_or_si64(xi03, xi07);

		*(PM64) (q + 0) = xi00;
		*(PM64) (q + 8) = xi01;
		*(PM64) (q + 16) = xi02;
		*(PM64) (q + 24) = xi03;

		p += 32;
		q += 32;
	}

	for (x = uWidth & 31; x; x--)
	{
		q[0] = p[1];
		q[1] = p[0];
		q[2] = p[3];
		q[3] = p[2];

		p += 4;
		q += 4;
	}
	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to VYUY with de-interlacing.
VOID Yuy2ToVyuyDI_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB32 with de-interlacing.
#define _Yuy2ToRgb32_mmx								\
{														\
	y0 = _mm_and_si64(y0, g_mWord00FF);					\
	y1 = _mm_and_si64(y1, g_mWord00FF);					\
	u0 = _mm_srli_pi16(u0, 8);							\
	v0 = _mm_srli_pi16(v0, 8);							\
	uv_temp1 = _mm_srli_pi32(u0, 16);					\
	u0 = _mm_slli_pi32(u0, 16);							\
	u0 = _mm_srli_pi32(u0, 16);							\
	uv_temp2 = _mm_srli_pi32(v0, 16);					\
	v0 = _mm_slli_pi32(v0, 16);							\
	v0 = _mm_srli_pi32(v0, 16);							\
	u0 = _mm_packs_pi32(u0, v0);							\
	v0 = _mm_packs_pi32(uv_temp1, uv_temp2);				\
	u0 = _mm_subs_pi16(u0, g_mSub80);					\
	v0 = _mm_subs_pi16(v0, g_mSub80);					\
	gu0 = _mm_mullo_pi16(u0, g_mUGMul);					\
	gv0 = _mm_mullo_pi16(v0, g_mVGMul);					\
	bu0 = _mm_mullo_pi16(u0, g_mUBMul);					\
	rv0 = _mm_mullo_pi16(v0, g_mVRMul);					\
	guv0 = _mm_adds_pi16(gu0, gv0);						\
	guv1 = _mm_unpackhi_pi16(guv0, guv0);				\
	guv0 = _mm_unpacklo_pi16(guv0, guv0);				\
	bu1 = _mm_unpackhi_pi16(bu0, bu0);					\
	bu0 = _mm_unpacklo_pi16(bu0, bu0);					\
	rv1 = _mm_unpackhi_pi16(rv0, rv0);					\
	rv0 = _mm_unpacklo_pi16(rv0, rv0);					\
	y1 = _mm_subs_pi16(y1, g_mSub10);					\
	y0 = _mm_subs_pi16(y0, g_mSub10);					\
	y1 = _mm_mullo_pi16(y1, g_mYYMul);					\
	y0 = _mm_mullo_pi16(y0, g_mYYMul);					\
	g1 = _mm_subs_pi16(y1, guv1);						\
	g0 = _mm_subs_pi16(y0, guv0);						\
	g1 = _mm_srai_pi16(g1, SCALEBITS);					\
	g0 = _mm_srai_pi16(g0, SCALEBITS);					\
	g0 = _mm_packs_pu16(g0, g1);						\
	b1 = _mm_adds_pi16(y1, bu1);						\
	b0 = _mm_adds_pi16(y0, bu0);						\
	b1 = _mm_srai_pi16(b1, SCALEBITS);					\
	b0 = _mm_srai_pi16(b0, SCALEBITS);					\
	b0 = _mm_packs_pu16(b0, b1);						\
	r1 = _mm_adds_pi16(y1, rv1);						\
	r0 = _mm_adds_pi16(y0, rv0);						\
	r1 = _mm_srai_pi16(r1, SCALEBITS);					\
	r0 = _mm_srai_pi16(r0, SCALEBITS);					\
	r0 = _mm_packs_pu16(r0, r1);						\
	r1 = _mm_unpackhi_pi8(b0, r0);						\
	r0 = _mm_unpacklo_pi8(b0, r0);						\
	g1 = _mm_unpackhi_pi8(g0, mz);						\
	g0 = _mm_unpacklo_pi8(g0, mz);						\
	rgb0 = _mm_unpacklo_pi8(r0, g0);					\
	rgb1 = _mm_unpackhi_pi8(r0, g0);					\
	rgb2 = _mm_unpacklo_pi8(r1, g1);					\
	rgb3 = _mm_unpackhi_pi8(r1, g1);					\
	((PM64) pbDstX)[0] = rgb0;							\
	((PM64) pbDstX)[1] = rgb1;							\
	((PM64) pbDstX)[2] = rgb2;							\
	((PM64) pbDstX)[3] = rgb3;							\
	pbDstX += 32;										\
	pbSrcX += 16;										\
}
VOID Yuy2ToRgb32DI_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;
	M64 xi10, xi11;
	M64 xi20, xi21;

	M64 y0, y1, u0, v0, uv_temp1, uv_temp2, mz, mMaskAvg;
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
	mMaskAvg = g_mMaskAvg;

	mz = _mm_setzero_si64();
	for (x = uWidth / 8; x; x--)
	{
		y0 = *(PM64) pbSrcX;
		y1 = *(PM64) (pbSrcX + 8);

		u0 = y0;
		v0 = y1;
		_Yuy2ToRgb32_mmx
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
	pbSrcX -= iSrcXStride;
	for (y = iHeight - 2 ; y; y--)
	{
		for (x = uWidth / 8; x; x--)
		{
			y0 = *(PM64) (pbSrcX + 0);
			y1 = *(PM64) (pbSrcX + 8);

			xi10 = *(PM64) (pbSrcX + iSrcXStride + 0);
			xi11 = *(PM64) (pbSrcX + iSrcXStride + 8);

			xi20 = *(PM64) (pbSrcX + iSrcXStride * 2 + 0);
			xi21 = *(PM64) (pbSrcX + iSrcXStride * 2 + 8);

			y0 = _mm_and_si64(y0, mMaskAvg);
			y1 = _mm_and_si64(y1, mMaskAvg);
			xi10 = _mm_and_si64(xi10, mMaskAvg);
			xi11 = _mm_and_si64(xi11, mMaskAvg);
			xi20 = _mm_and_si64(xi20, mMaskAvg);
			xi21 = _mm_and_si64(xi21, mMaskAvg);
			y0 = _mm_srli_si64(y0, 1);
			y1 = _mm_srli_si64(y1, 1);
			xi10 = _mm_srli_si64(xi10, 1);
			xi11 = _mm_srli_si64(xi11, 1);
			xi20 = _mm_srli_si64(xi20, 1);
			xi21 = _mm_srli_si64(xi21, 1);
			y0 = _mm_adds_pu8(y0, xi20);
			y1 = _mm_adds_pu8(y1, xi21);
			y0 = _mm_and_si64(y0, mMaskAvg);
			y1 = _mm_and_si64(y1, mMaskAvg);
			y0 = _mm_srli_si64(y0, 1);
			y1 = _mm_srli_si64(y1, 1);
			y0 = _mm_adds_pu8(y0, xi10);
			y1 = _mm_adds_pu8(y1, xi11);

			u0 = y0;
			v0 = y1;

			_Yuy2ToRgb32_mmx
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
	pbSrcX += iSrcXDif;
	for (x = uWidth / 8; x; x--)
	{
		y0 = *(PM64) pbSrcX;
		y1 = *(PM64) (pbSrcX + 8);

		u0 = y0;
		v0 = y1;

		_Yuy2ToRgb32_mmx
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

	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB24 with de-interlacing.
#define _Yuy2ToRgb24_mmx								\
{														\
	y0 = _mm_and_si64(y0, g_mWord00FF);					\
	y1 = _mm_and_si64(y1, g_mWord00FF);					\
	u0 = _mm_srli_pi16(u0, 8);							\
	v0 = _mm_srli_pi16(v0, 8);							\
	uv_temp1 = _mm_srli_pi32(u0, 16);					\
	u0 = _mm_slli_pi32(u0, 16);							\
	u0 = _mm_srli_pi32(u0, 16);							\
	uv_temp2 = _mm_srli_pi32(v0, 16);					\
	v0 = _mm_slli_pi32(v0, 16);							\
	v0 = _mm_srli_pi32(v0, 16);							\
	u0 = _mm_packs_pi32(u0, v0);							\
	v0 = _mm_packs_pi32(uv_temp1, uv_temp2);				\
	u0 = _mm_subs_pi16(u0, g_mSub80);					\
	v0 = _mm_subs_pi16(v0, g_mSub80);					\
	gu0 = _mm_mullo_pi16(u0, g_mUGMul);					\
	gv0 = _mm_mullo_pi16(v0, g_mVGMul);					\
	bu0 = _mm_mullo_pi16(u0, g_mUBMul);					\
	rv0 = _mm_mullo_pi16(v0, g_mVRMul);					\
	guv0 = _mm_adds_pi16(gu0, gv0);						\
	guv1 = _mm_unpackhi_pi16(guv0, guv0);				\
	guv0 = _mm_unpacklo_pi16(guv0, guv0);				\
	bu1 = _mm_unpackhi_pi16(bu0, bu0);					\
	bu0 = _mm_unpacklo_pi16(bu0, bu0);					\
	rv1 = _mm_unpackhi_pi16(rv0, rv0);					\
	rv0 = _mm_unpacklo_pi16(rv0, rv0);					\
	y1 = _mm_subs_pi16(y1, g_mSub10);					\
	y0 = _mm_subs_pi16(y0, g_mSub10);					\
	y1 = _mm_mullo_pi16(y1, g_mYYMul);					\
	y0 = _mm_mullo_pi16(y0, g_mYYMul);					\
	g1 = _mm_subs_pi16(y1, guv1);						\
	g0 = _mm_subs_pi16(y0, guv0);						\
	g1 = _mm_srai_pi16(g1, SCALEBITS);					\
	g0 = _mm_srai_pi16(g0, SCALEBITS);					\
	g0 = _mm_packs_pu16(g0, g1);						\
	b1 = _mm_adds_pi16(y1, bu1);						\
	b0 = _mm_adds_pi16(y0, bu0);						\
	b1 = _mm_srai_pi16(b1, SCALEBITS);					\
	b0 = _mm_srai_pi16(b0, SCALEBITS);					\
	b0 = _mm_packs_pu16(b0, b1);						\
	r1 = _mm_adds_pi16(y1, rv1);						\
	r0 = _mm_adds_pi16(y0, rv0);						\
	r1 = _mm_srai_pi16(r1, SCALEBITS);					\
	r0 = _mm_srai_pi16(r0, SCALEBITS);					\
	r0 = _mm_packs_pu16(r0, r1);						\
	r1 = _mm_unpackhi_pi8(b0, r0);						\
	r0 = _mm_unpacklo_pi8(b0, r0);						\
	g1 = _mm_unpackhi_pi8(g0, mz);						\
	g0 = _mm_unpacklo_pi8(g0, mz);						\
	rgb0 = _mm_unpacklo_pi8(r0, g0);					\
	rgb1 = _mm_unpackhi_pi8(r0, g0);					\
	rgb2 = _mm_unpacklo_pi8(r1, g1);					\
	rgb3 = _mm_unpackhi_pi8(r1, g1);					\
	*((PDWORD) (pbDstX + 0)) = _mm_cvtsi64_si32(rgb0);	\
	rgb0 = _mm_srli_si64(rgb0, 32);						\
	*((PDWORD) (pbDstX + 3)) = _mm_cvtsi64_si32(rgb0);	\
	*((PDWORD) (pbDstX + 6)) = _mm_cvtsi64_si32(rgb1);	\
	rgb1 = _mm_srli_si64(rgb1, 32);						\
	*((PDWORD) (pbDstX + 9)) = _mm_cvtsi64_si32(rgb1);	\
	*((PDWORD) (pbDstX + 12)) = _mm_cvtsi64_si32(rgb2);	\
	rgb2 = _mm_srli_si64(rgb2, 32);						\
	*((PDWORD) (pbDstX + 15)) = _mm_cvtsi64_si32(rgb2);	\
	*((PDWORD) (pbDstX + 18)) = _mm_cvtsi64_si32(rgb3);	\
	rgb3 = _mm_srli_si64(rgb3, 32);						\
	*((PDWORD) (pbDstX + 21)) = _mm_cvtsi64_si32(rgb3);	\
	pbDstX += 24;										\
	pbSrcX += 16;										\
}

VOID Yuy2ToRgb24DI_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;
	M64 xi10, xi11;
	M64 xi20, xi21;
	
	M64 y0, y1, u0, v0, uv_temp1, uv_temp2, mz, mMaskAvg;
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
	mMaskAvg = g_mMaskAvg;

	mz = _mm_setzero_si64();
	for (x = uWidth / 8; x; x--)
	{
		y0 = *(PM64) pbSrcX;
		y1 = *(PM64) (pbSrcX + 8);

		u0 = y0;
		v0 = y1;
		_Yuy2ToRgb24_mmx
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
	pbSrcX -= iSrcXStride;
	for (y = iHeight - 2 ; y; y--)
	{
		for (x = uWidth / 8; x; x--)
		{
			y0 = *(PM64) (pbSrcX + 0);
			y1 = *(PM64) (pbSrcX + 8);

			xi10 = *(PM64) (pbSrcX + iSrcXStride + 0);
			xi11 = *(PM64) (pbSrcX + iSrcXStride + 8);

			xi20 = *(PM64) (pbSrcX + iSrcXStride * 2 + 0);
			xi21 = *(PM64) (pbSrcX + iSrcXStride * 2 + 8);

			y0 = _mm_and_si64(y0, mMaskAvg);
			y1 = _mm_and_si64(y1, mMaskAvg);
			xi10 = _mm_and_si64(xi10, mMaskAvg);
			xi11 = _mm_and_si64(xi11, mMaskAvg);
			xi20 = _mm_and_si64(xi20, mMaskAvg);
			xi21 = _mm_and_si64(xi21, mMaskAvg);
			y0 = _mm_srli_si64(y0, 1);
			y1 = _mm_srli_si64(y1, 1);
			xi10 = _mm_srli_si64(xi10, 1);
			xi11 = _mm_srli_si64(xi11, 1);
			xi20 = _mm_srli_si64(xi20, 1);
			xi21 = _mm_srli_si64(xi21, 1);
			y0 = _mm_adds_pu8(y0, xi20);
			y1 = _mm_adds_pu8(y1, xi21);
			y0 = _mm_and_si64(y0, mMaskAvg);
			y1 = _mm_and_si64(y1, mMaskAvg);
			y0 = _mm_srli_si64(y0, 1);
			y1 = _mm_srli_si64(y1, 1);
			y0 = _mm_adds_pu8(y0, xi10);
			y1 = _mm_adds_pu8(y1, xi11);

			u0 = y0;
			v0 = y1;

			_Yuy2ToRgb24_mmx
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
	pbSrcX += iSrcXDif;
	for (x = uWidth / 8; x; x--)
	{
		y0 = *(PM64) pbSrcX;
		y1 = *(PM64) (pbSrcX + 8);

		u0 = y0;
		v0 = y1;

		_Yuy2ToRgb24_mmx
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
	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB16 with de-interlacing.
VOID Yuy2ToRgb16DI_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB15 with de-interlacing.
VOID Yuy2ToRgb15DI_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
