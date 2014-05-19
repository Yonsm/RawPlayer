


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"

#ifdef _SSE2
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

VOID Yuy2ToYv12DI_sse2(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
					   PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstYDif;
	INT iDstUvDif;
	M128I xi0, xi1, xi2, xi3, xi4, xi5, xi6, xi7, xiMASK;
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

	xiMASK = g_xiWord00FF;
	for (x = uWidth / 16; x; x--)
	{
		_mm_prefetch(pbSrcX + 32, _MM_HINT_NTA);
		_mm_prefetch(pbSrcX + iSrcXStride + 32, _MM_HINT_NTA);
		_mm_prefetch(pbSrcX + iSrcXStride * 2 + 32, _MM_HINT_NTA);

		xi0 = _mm_loadu_si128(((PM128I) pbSrcX) + 0);
		xi1 = _mm_loadu_si128(((PM128I) pbSrcX) + 1);
		xi2 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride)) + 0);
		xi3 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride)) + 1);
		xi4 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride * 2)) + 0);
		xi5 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride * 2)) + 1);


		xi4 = _mm_avg_epu8(xi4, xi0);
		xi5 = _mm_avg_epu8(xi5, xi1);
		xi4 = _mm_avg_epu8(xi4, xi2);
		xi5 = _mm_avg_epu8(xi5, xi3);
		xi2 = xi4;
		xi3 = xi5;
		xi4 = _mm_avg_epu8(xi4, xi0);
		xi5 = _mm_avg_epu8(xi5, xi1);
		xi4 = _mm_srli_epi16(xi4, 8);
		xi5 = _mm_srli_epi16(xi5, 8);

		xi0 = _mm_and_si128(xi0, xiMASK);
		xi1 = _mm_and_si128(xi1, xiMASK);
		xi2 = _mm_and_si128(xi2, xiMASK);
		xi3 = _mm_and_si128(xi3, xiMASK);

		xi0 = _mm_packus_epi16(xi0, xi1);
		xi2 = _mm_packus_epi16(xi2, xi3);

		_mm_storeu_si128((PM128I) pbDstY, xi0);
		_mm_storeu_si128((PM128I) (pbDstY + iDstYStride), xi2);

		xi4 = _mm_packus_epi16(xi4, xi5);
		xi5 = xi4;

		xi4 = _mm_srli_si128(xi4, 1);

		xi5 = _mm_and_si128(xi5, xiMASK);
		xi4 = _mm_and_si128(xi4, xiMASK);

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
		for (x = uWidth / 16; x; x--)
		{
			_mm_prefetch(pbSrcX + 32, _MM_HINT_NTA);
			_mm_prefetch(pbSrcX + iSrcXStride + 32, _MM_HINT_NTA);
			_mm_prefetch(pbSrcX + iSrcXStride * 2 + 32, _MM_HINT_NTA);
			_mm_prefetch(pbSrcX + iSrcXStride * 3 + 32, _MM_HINT_NTA);

			xi0 = _mm_loadu_si128(((PM128I) pbSrcX) + 0);
			xi1 = _mm_loadu_si128(((PM128I) pbSrcX) + 1);
			xi2 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride)) + 0);
			xi3 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride)) + 1);
			xi4 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride * 2)) + 0);// xi0;
			xi5 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride * 2)) + 1);// xi1;
			xi6 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride * 3)) + 0);// xi0;
			xi7 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride * 3)) + 1);// xi1;

			xi0 = _mm_avg_epu8(xi4, xi0);
			xi1 = _mm_avg_epu8(xi5, xi1);
			xi0 = _mm_avg_epu8(xi0, xi2);
			xi1 = _mm_avg_epu8(xi1, xi3);
			xi6 = _mm_avg_epu8(xi6, xi2);
			xi7 = _mm_avg_epu8(xi7, xi3);
			xi6 = _mm_avg_epu8(xi6, xi4);
			xi7 = _mm_avg_epu8(xi7, xi5);

			xi2 = xi0;
			xi3 = xi1;

			xi0 = _mm_avg_epu8(xi0, xi6);
			xi1 = _mm_avg_epu8(xi1, xi7);

			xi0 = _mm_srli_epi16(xi0, 8);
			xi1 = _mm_srli_epi16(xi1, 8);

			xi2 = _mm_and_si128(xi2, xiMASK);
			xi3 = _mm_and_si128(xi3, xiMASK);
			xi6 = _mm_and_si128(xi6, xiMASK);
			xi7 = _mm_and_si128(xi7, xiMASK);

			xi2 = _mm_packus_epi16(xi2, xi3);
			xi6 = _mm_packus_epi16(xi6, xi7);

			_mm_storeu_si128((PM128I) pbDstY, xi2);
			_mm_storeu_si128((PM128I) (pbDstY + iDstYStride), xi6);

			xi0 = _mm_packus_epi16(xi0, xi1);
			xi1 = xi0;

			xi0 = _mm_srli_si128(xi0, 1);

			xi1 = _mm_and_si128(xi1, xiMASK);
			xi0 = _mm_and_si128(xi0, xiMASK);

			xi1 = _mm_packus_epi16(xi1, xi1);
			xi0 = _mm_packus_epi16(xi0, xi0);

			_mm_storel_epi64((PM128I) pbDstU, xi1);
			_mm_storel_epi64((PM128I) pbDstV, xi0);

			pbSrcX += 32;
			pbDstY += 16;
			pbDstU += 8;
			pbDstV += 8;
		}

		for (x = (uWidth & 15) / 2; x; x--)
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
	for (x = uWidth / 16; x; x--)
	{
		_mm_prefetch(pbSrcX + iSrcXStride + 32, _MM_HINT_NTA);
		_mm_prefetch(pbSrcX + iSrcXStride * 2 + 32, _MM_HINT_NTA);

		xi0 = _mm_loadu_si128(((PM128I) pbSrcX) + 0);
		xi1 = _mm_loadu_si128(((PM128I) pbSrcX) + 1);
		xi2 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride)) + 0);
		xi3 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride)) + 1);
		xi4 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride * 2)) + 0);
		xi5 = _mm_loadu_si128(((PM128I) (pbSrcX + iSrcXStride * 2)) + 1);

		xi0 = _mm_avg_epu8(xi4, xi0);
		xi1 = _mm_avg_epu8(xi5, xi1);
		xi0 = _mm_avg_epu8(xi0, xi2);
		xi1 = _mm_avg_epu8(xi1, xi3);
		xi2 = xi0;
		xi3 = xi1;
		xi0 = _mm_avg_epu8(xi4, xi0);
		xi1 = _mm_avg_epu8(xi5, xi1);
		xi0 = _mm_srli_epi16(xi0, 8);
		xi1 = _mm_srli_epi16(xi1, 8);

		xi2 = _mm_and_si128(xi2, xiMASK);
		xi3 = _mm_and_si128(xi3, xiMASK);
		xi4 = _mm_and_si128(xi4, xiMASK);
		xi5 = _mm_and_si128(xi5, xiMASK);

		xi2 = _mm_packus_epi16(xi2, xi3);
		xi4 = _mm_packus_epi16(xi4, xi5);

		_mm_storeu_si128((PM128I) pbDstY, xi2);
		_mm_storeu_si128((PM128I) (pbDstY + iDstYStride), xi4);

		xi0 = _mm_packus_epi16(xi0, xi1);
		xi1 = xi0;

		xi0 = _mm_srli_si128(xi0, 1);

		xi1 = _mm_and_si128(xi1, xiMASK);
		xi0 = _mm_and_si128(xi0, xiMASK);

		xi1 = _mm_packus_epi16(xi1, xi1);
		xi0 = _mm_packus_epi16(xi0, xi0);

		_mm_storel_epi64((PM128I) pbDstU, xi1);
		_mm_storel_epi64((PM128I) pbDstV, xi0);

		pbSrcX += 32;
		pbDstY += 16;
		pbDstU += 8;
		pbDstV += 8;
	}

	for (x = (uWidth & 15) / 2; x; x--)
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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YUY2 with de-interlacing.
VOID Yuy2ToYuy2DI_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopyDI_sse2(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YVYU with de-interlacing.
VOID Yuy2ToYvyuDI_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to UYVY with de-interlacing.
VOID Yuy2ToUyvyDI_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	PBYTE p, q;
	M128I xi00, xi01, xi02, xi03, xi04, xi05, xi06, xi07;
	M128I xi10, xi11, xi12, xi13;
	M128I xi20, xi21, xi22, xi23;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	uWidth *= 2;
	p = pbSrcX;
	q = pbDstX;
	for (x = uWidth / 64; x; x--)
	{
		_mm_prefetch(pbSrcX + 64, _MM_HINT_NTA);
		xi00 = _mm_loadu_si128((PM128I) (p + 0));
		xi01 = _mm_loadu_si128((PM128I) (p + 16));
		xi02 = _mm_loadu_si128((PM128I) (p + 32));
		xi03 = _mm_loadu_si128((PM128I) (p + 48));

		xi04 = xi00;
		xi05 = xi01;
		xi06 = xi02;
		xi07 = xi03;

		xi00 = _mm_slli_epi16(xi00, 8);
		xi01 = _mm_slli_epi16(xi01, 8);
		xi02 = _mm_slli_epi16(xi02, 8);
		xi03 = _mm_slli_epi16(xi03, 8);

		xi04 = _mm_srli_epi16(xi04, 8);
		xi05 = _mm_srli_epi16(xi05, 8);
		xi06 = _mm_srli_epi16(xi06, 8);
		xi07 = _mm_srli_epi16(xi07, 8);

		xi00 = _mm_or_si128(xi00, xi04);
		xi01 = _mm_or_si128(xi01, xi05);
		xi02 = _mm_or_si128(xi02, xi06);
		xi03 = _mm_or_si128(xi03, xi07);

		_mm_storeu_si128((PM128I) (q + 0), xi00);
		_mm_storeu_si128((PM128I) (q + 16), xi01);
		_mm_storeu_si128((PM128I) (q + 32), xi02);
		_mm_storeu_si128((PM128I) (q + 48), xi03);

		p += 64;
		q += 64;
	}

	for (x = uWidth & 63; x; x--)
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
		for (x = uWidth / 64; x; x--)
		{
			_mm_prefetch(pbSrcX + 64, _MM_HINT_NTA);
			_mm_prefetch(pbSrcX + iSrcXStride + 64, _MM_HINT_NTA);
			_mm_prefetch(pbSrcX + iSrcXStride * 2 + 64, _MM_HINT_NTA);

			xi00 = _mm_loadu_si128((PM128I) (p + 0));
			xi01 = _mm_loadu_si128((PM128I) (p + 16));
			xi02 = _mm_loadu_si128((PM128I) (p + 32));
			xi03 = _mm_loadu_si128((PM128I) (p + 48));

			xi10 = _mm_loadu_si128((PM128I) (p + iSrcXStride + 0));
			xi11 = _mm_loadu_si128((PM128I) (p + iSrcXStride + 16));
			xi12 = _mm_loadu_si128((PM128I) (p + iSrcXStride + 32));
			xi13 = _mm_loadu_si128((PM128I) (p + iSrcXStride + 48));

			xi20 = _mm_loadu_si128((PM128I) (p + iSrcXStride * 2 + 0));
			xi21 = _mm_loadu_si128((PM128I) (p + iSrcXStride * 2 + 16));
			xi22 = _mm_loadu_si128((PM128I) (p + iSrcXStride * 2 + 32));
			xi23 = _mm_loadu_si128((PM128I) (p + iSrcXStride * 2 + 48));

			xi00 = _mm_avg_epu8(xi00, xi20);
			xi01 = _mm_avg_epu8(xi01, xi21);
			xi02 = _mm_avg_epu8(xi02, xi22);
			xi03 = _mm_avg_epu8(xi03, xi23);

			xi00 = _mm_avg_epu8(xi00, xi10);
			xi01 = _mm_avg_epu8(xi01, xi11);
			xi02 = _mm_avg_epu8(xi02, xi12);
			xi03 = _mm_avg_epu8(xi03, xi13);

			xi04 = xi00;
			xi05 = xi01;
			xi06 = xi02;
			xi07 = xi03;

			xi00 = _mm_slli_epi16(xi00, 8);
			xi01 = _mm_slli_epi16(xi01, 8);
			xi02 = _mm_slli_epi16(xi02, 8);
			xi03 = _mm_slli_epi16(xi03, 8);

			xi04 = _mm_srli_epi16(xi04, 8);
			xi05 = _mm_srli_epi16(xi05, 8);
			xi06 = _mm_srli_epi16(xi06, 8);
			xi07 = _mm_srli_epi16(xi07, 8);

			xi00 = _mm_or_si128(xi00, xi04);
			xi01 = _mm_or_si128(xi01, xi05);
			xi02 = _mm_or_si128(xi02, xi06);
			xi03 = _mm_or_si128(xi03, xi07);

			_mm_storeu_si128((PM128I) (q + 0), xi00);
			_mm_storeu_si128((PM128I) (q + 16), xi01);
			_mm_storeu_si128((PM128I) (q + 32), xi02);
			_mm_storeu_si128((PM128I) (q + 48), xi03);

			p += 64;
			q += 64;
		}

		for (x = uWidth & 63; x; x--)
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
	for (x = uWidth / 64; x; x--)
	{
		_mm_prefetch(pbSrcX + 64, _MM_HINT_NTA);
		xi00 = _mm_loadu_si128((PM128I) (p + 0));
		xi01 = _mm_loadu_si128((PM128I) (p + 16));
		xi02 = _mm_loadu_si128((PM128I) (p + 32));
		xi03 = _mm_loadu_si128((PM128I) (p + 48));

		xi04 = xi00;
		xi05 = xi01;
		xi06 = xi02;
		xi07 = xi03;

		xi00 = _mm_slli_epi16(xi00, 8);
		xi01 = _mm_slli_epi16(xi01, 8);
		xi02 = _mm_slli_epi16(xi02, 8);
		xi03 = _mm_slli_epi16(xi03, 8);

		xi04 = _mm_srli_epi16(xi04, 8);
		xi05 = _mm_srli_epi16(xi05, 8);
		xi06 = _mm_srli_epi16(xi06, 8);
		xi07 = _mm_srli_epi16(xi07, 8);

		xi00 = _mm_or_si128(xi00, xi04);
		xi01 = _mm_or_si128(xi01, xi05);
		xi02 = _mm_or_si128(xi02, xi06);
		xi03 = _mm_or_si128(xi03, xi07);

		_mm_storeu_si128((PM128I) (q + 0), xi00);
		_mm_storeu_si128((PM128I) (q + 16), xi01);
		_mm_storeu_si128((PM128I) (q + 32), xi02);
		_mm_storeu_si128((PM128I) (q + 48), xi03);

		p += 64;
		q += 64;
	}

	for (x = uWidth & 63; x; x--)
	{
		q[0] = p[1];
		q[1] = p[0];
		q[2] = p[3];
		q[3] = p[2];

		p += 4;
		q += 4;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to VYUY with de-interlacing.
VOID Yuy2ToVyuyDI_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB32 with de-interlacing.
VOID Yuy2ToRgb32DI_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB24 with de-interlacing.
VOID Yuy2ToRgb24DI_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB16 with de-interlacing.
VOID Yuy2ToRgb16DI_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB15 with de-interlacing.
VOID Yuy2ToRgb15DI_sse2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
