


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"

#ifdef _XMM
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

VOID Yuy2ToYv12DI_xmm(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
					  PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstYDif;
	INT iDstUvDif;
	M64 m0, m1, m2, m3, m4, m5, m6, m7, mMASK;
	BYTE Src00, Src01, Src02, Src03, Src10, Src11, Src12, Src13;

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
	for (x = uWidth / 8; x; x--)
	{
		_mm_prefetch(pbSrcX + 16, _MM_HINT_NTA);
		_mm_prefetch(pbSrcX + iSrcXStride + 16, _MM_HINT_NTA);
		_mm_prefetch(pbSrcX + iSrcXStride * 2 + 16, _MM_HINT_NTA);

		m0 = ((PM64) pbSrcX)[0];
		m1 = ((PM64) pbSrcX)[1];
		m2 = ((PM64) (pbSrcX + iSrcXStride))[0];
		m3 = ((PM64) (pbSrcX + iSrcXStride))[1];
		m4 = ((PM64) (pbSrcX + iSrcXStride * 2))[0];
		m5 = ((PM64) (pbSrcX + iSrcXStride * 2))[1];


		m4 = _mm_avg_pu8(m4, m0);
		m5 = _mm_avg_pu8(m5, m1);
		m4 = _mm_avg_pu8(m4, m2);
		m5 = _mm_avg_pu8(m5, m3);
		m2 = m4;
		m3 = m5;
		m4 = _mm_avg_pu8(m4, m0);
		m5 = _mm_avg_pu8(m5, m1);
		m4 = _mm_srli_pi16(m4, 8);
		m5 = _mm_srli_pi16(m5, 8);

		m0 = _mm_and_si64(m0, mMASK);
		m1 = _mm_and_si64(m1, mMASK);
		m2 = _mm_and_si64(m2, mMASK);
		m3 = _mm_and_si64(m3, mMASK);

		m0 = _mm_packs_pu16(m0, m1);
		m2 = _mm_packs_pu16(m2, m3);

		((PM64) pbDstY)[0] = m0;
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
		Src10 = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
		Src11 = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
		Src12 = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
		Src13 = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;
		Yuy2ToYv12(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3], Src10, Src11, Src12, Src13);
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
			_mm_prefetch(pbSrcX + iSrcXStride + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcX + iSrcXStride * 2 + 16, _MM_HINT_NTA);
			_mm_prefetch(pbSrcX + iSrcXStride * 3 + 16, _MM_HINT_NTA);

			m0 = ((PM64) pbSrcX)[0];
			m1 = ((PM64) pbSrcX)[1];
			m2 = ((PM64) (pbSrcX + iSrcXStride))[0];
			m3 = ((PM64) (pbSrcX + iSrcXStride))[1];
			m4 = ((PM64) (pbSrcX + iSrcXStride * 2))[0];
			m5 = ((PM64) (pbSrcX + iSrcXStride * 2))[1];
			m6 = ((PM64) (pbSrcX + iSrcXStride * 3))[0];
			m7 = ((PM64) (pbSrcX + iSrcXStride * 3))[1];

			m0 = _mm_avg_pu8(m4, m0);
			m1 = _mm_avg_pu8(m5, m1);
			m0 = _mm_avg_pu8(m0, m2);
			m1 = _mm_avg_pu8(m1, m3);
			m6 = _mm_avg_pu8(m6, m2);
			m7 = _mm_avg_pu8(m7, m3);
			m6 = _mm_avg_pu8(m6, m4);
			m7 = _mm_avg_pu8(m7, m5);

			m2 = m0;
			m3 = m1;

			m0 = _mm_avg_pu8(m0, m6);
			m1 = _mm_avg_pu8(m1, m7);

			m0 = _mm_srli_pi16(m0, 8);
			m1 = _mm_srli_pi16(m1, 8);

			m2 = _mm_and_si64(m2, mMASK);
			m3 = _mm_and_si64(m3, mMASK);
			m6 = _mm_and_si64(m6, mMASK);
			m7 = _mm_and_si64(m7, mMASK);

			m2 = _mm_packs_pu16(m2, m3);
			m6 = _mm_packs_pu16(m6, m7);

			((PM64) pbDstY)[0] = m2;
			((PM64) (pbDstY + iDstYStride))[0] = m6;

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
			Src00 = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
			Src01 = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
			Src02 = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
			Src03 = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;
			
			Src10 = ((pbSrcX[iSrcXStride + 0] + pbSrcX[iSrcXStride * 3 + 0] + 1) / 2 +
				pbSrcX[iSrcXStride * 2 + 0] + 1) / 2;
			Src11 = ((pbSrcX[iSrcXStride + 1] + pbSrcX[iSrcXStride * 3 + 1] + 1) / 2 +
				pbSrcX[iSrcXStride * 2 + 1] + 1) / 2;
			Src12 = ((pbSrcX[iSrcXStride + 2] + pbSrcX[iSrcXStride * 3 + 2] + 1) / 2 +
				pbSrcX[iSrcXStride * 2 + 2] + 1) / 2;
			Src13 = ((pbSrcX[iSrcXStride + 3] + pbSrcX[iSrcXStride * 3 + 3] + 1) / 2 +
				pbSrcX[iSrcXStride * 2 + 3] + 1) / 2;
			
			Yuy2ToYv12(Src10, Src11, Src12, Src13, Src10, Src11, Src12, Src13);

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
		_mm_prefetch(pbSrcX + iSrcXStride + 16, _MM_HINT_NTA);
		_mm_prefetch(pbSrcX + iSrcXStride * 2 + 16, _MM_HINT_NTA);

		m0 = ((PM64) pbSrcX)[0];
		m1 = ((PM64) pbSrcX)[1];
		m2 = ((PM64) (pbSrcX + iSrcXStride))[0];
		m3 = ((PM64) (pbSrcX + iSrcXStride))[1];
		m4 = ((PM64) (pbSrcX + iSrcXStride * 2))[0];
		m5 = ((PM64) (pbSrcX + iSrcXStride * 2))[1];

		m0 = _mm_avg_pu8(m4, m0);
		m1 = _mm_avg_pu8(m5, m1);
		m0 = _mm_avg_pu8(m0, m2);
		m1 = _mm_avg_pu8(m1, m3);
		m2 = m0;
		m3 = m1;
		m0 = _mm_avg_pu8(m4, m0);
		m1 = _mm_avg_pu8(m5, m1);
		m0 = _mm_srli_pi16(m0, 8);
		m1 = _mm_srli_pi16(m1, 8);

		m2 = _mm_and_si64(m2, mMASK);
		m3 = _mm_and_si64(m3, mMASK);
		m4 = _mm_and_si64(m4, mMASK);
		m5 = _mm_and_si64(m5, mMASK);

		m2 = _mm_packs_pu16(m2, m3);
		m4 = _mm_packs_pu16(m4, m5);

		((PM64) pbDstY)[0] = m2;
		((PM64) (pbDstY + iDstYStride))[0] = m4;

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
		Src10 = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
		Src11 = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
		Src12 = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
		Src13 = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;
		Yuy2ToYv12(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3], Src10, Src11, Src12, Src13);
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
VOID Yuy2ToYuy2DI_xmm(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopyDI_xmm(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YVYU with de-interlacing.
VOID Yuy2ToYvyuDI_xmm(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to UYVY with de-interlacing.
VOID Yuy2ToUyvyDI_xmm(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	PBYTE p, q;
	M64 xi00, xi01, xi02, xi03, xi04, xi05, xi06, xi07;
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
	for (x = uWidth / 32; x; x--)
	{
		_mm_prefetch(pbSrcX + 32, _MM_HINT_NTA);
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
			_mm_prefetch(pbSrcX + 32, _MM_HINT_NTA);
			_mm_prefetch(pbSrcX + iSrcXStride + 32, _MM_HINT_NTA);
			_mm_prefetch(pbSrcX + iSrcXStride * 2 + 32, _MM_HINT_NTA);

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

			xi00 = _mm_avg_pu8(xi00, xi20);
			xi01 = _mm_avg_pu8(xi01, xi21);
			xi02 = _mm_avg_pu8(xi02, xi22);
			xi03 = _mm_avg_pu8(xi03, xi23);

			xi00 = _mm_avg_pu8(xi00, xi10);
			xi01 = _mm_avg_pu8(xi01, xi11);
			xi02 = _mm_avg_pu8(xi02, xi12);
			xi03 = _mm_avg_pu8(xi03, xi13);

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
		_mm_prefetch(pbSrcX + 32, _MM_HINT_NTA);
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
VOID Yuy2ToVyuyDI_xmm(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB32 with de-interlacing.
VOID Yuy2ToRgb32DI_xmm(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB24 with de-interlacing.
VOID Yuy2ToRgb24DI_xmm(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB16 with de-interlacing.
VOID Yuy2ToRgb16DI_xmm(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB15 with de-interlacing.
VOID Yuy2ToRgb15DI_xmm(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
