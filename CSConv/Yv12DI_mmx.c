


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"

#ifdef _MMX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YV12 with de-interlacing.
VOID Yv12ToYv12DI_mmx(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
					  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					  UINT uWidth, INT iHeight)
{
	LineCopyDI_mmx(pbDstY, iDstYStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopyDI_mmx(pbDstU, iDstUvStride, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopyDI_mmx(pbDstV, iDstUvStride, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
}

VOID Yv12ToYV12DI_mmx(PBYTE pbDstX, INT iDstXStride,
					  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					  UINT uWidth, INT iHeight)
{
	UINT uPixel = iDstXStride * abs(iHeight);
	LineCopyDI_mmx(pbDstX, iDstXStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopyDI_mmx(pbDstX + uPixel, iDstXStride / 2, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopyDI_mmx(pbDstX + uPixel + (uPixel / 4), iDstXStride / 2, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
}

VOID Yv12ToI420DI_mmx(PBYTE pbDstX, INT iDstXStride,
					  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
					  UINT uWidth, INT iHeight)
{
	UINT uPixel = iDstXStride * abs(iHeight);
	LineCopyDI_mmx(pbDstX, iDstXStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopyDI_mmx(pbDstX + uPixel, iDstXStride / 2, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopyDI_mmx(pbDstX + uPixel + (uPixel / 4), iDstXStride / 2, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YUY2 with de-interlacing.
VOID Yv12ToYuy2DI_mmx(PBYTE pbDstX, INT iDstXStride,
				PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	M64 xiU, xiV, xiUv0, xiUv1, 
		xiY01,xiY02,xiY03,xiY11,xiY12,xiY13,
		xiY0,xiY1,xiY2,xiY3,
		xiU1,xiU2,xiU3,xiV1,xiV2,xiV3,
		mMaskAvg;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - 2 * uWidth;
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	mMaskAvg = g_mMaskAvg;
	for (x = uWidth / 16 ; x ; x--)
	{
		xiU1 = *(PM64) pbSrcU;
		xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
		xiU1 = _mm_and_si64(xiU1, mMaskAvg);
		xiU2 = _mm_and_si64(xiU2, mMaskAvg);
		xiU1 = _mm_srli_si64(xiU1, 1);
		xiU2 = _mm_srli_si64(xiU2, 1);
		xiU = _mm_adds_pu8(xiU1, xiU2);
		xiV1 = *(PM64) pbSrcV;
		xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
		xiV1 = _mm_and_si64(xiV1, mMaskAvg);
		xiV2 = _mm_and_si64(xiV2, mMaskAvg);
		xiV1 = _mm_srli_si64(xiV1, 1);
		xiV2 = _mm_srli_si64(xiV2, 1);
		xiV = _mm_adds_pu8(xiV1, xiV2);
		xiUv0 = _mm_unpacklo_pi8(xiU, xiV);
		xiUv1 = _mm_unpackhi_pi8(xiU, xiV);

		xiY01 = *(PM64) pbSrcY;
		xiY11 = *(PM64) (pbSrcY + 8);

		xiY0 = xiY01;
		xiY1 = xiY11;

		xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
		xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

		xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
		xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);

		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY02 = _mm_and_si64(xiY02, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_srli_si64(xiY02, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY2 = _mm_adds_pu8(xiY01, xiY03);
		xiY2 = _mm_and_si64(xiY2, mMaskAvg);
		xiY2 = _mm_srli_si64(xiY2, 1);
		xiY2 = _mm_adds_pu8(xiY2, xiY02);

		xiY11 = _mm_and_si64(xiY11, mMaskAvg);
		xiY12 = _mm_and_si64(xiY12, mMaskAvg);
		xiY13 = _mm_and_si64(xiY13, mMaskAvg);
		xiY11 = _mm_srli_si64(xiY11, 1);
		xiY12 = _mm_srli_si64(xiY12, 1);
		xiY13 = _mm_srli_si64(xiY13, 1);
		xiY3 = _mm_adds_pu8(xiY11, xiY13);
		xiY3 = _mm_and_si64(xiY3, mMaskAvg);
		xiY3 = _mm_srli_si64(xiY3, 1);
		xiY3 = _mm_adds_pu8(xiY3, xiY12);

		*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiY0, xiUv0);
		*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiY0, xiUv0);
		*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiY1, xiUv1);
		*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiY1, xiUv1);

		*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiY2, xiUv0);
		*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiY2, xiUv0);
		*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiY3, xiUv1);
		*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiY3, xiUv1);

		pbDstX += 32;
		pbSrcY += 16;
		pbSrcU += 8;
		pbSrcV += 8;
	}
	for (x = (uWidth & 15) / 2; x; x--)
	{
		pbDstX[0] = pbSrcY[0];
		pbDstX[1] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;
		pbDstX[2] = pbSrcY[1];
		pbDstX[3] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;

		pbDstX[iDstXStride + 0] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		pbDstX[iDstXStride + 1] = pbDstX[1];
		pbDstX[iDstXStride + 2] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
		pbDstX[iDstXStride + 3] = pbDstX[3];

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	pbDstX += iDstXDif + iDstXStride;
	pbSrcY += iSrcYDif ;
	pbSrcU += iSrcUvDif - iSrcUvStride;
	pbSrcV += iSrcUvDif - iSrcUvStride;
	for (y = (iHeight / 2) - 2; y; y--)
	{
		for (x = uWidth / 16 ; x ; x--)
		{
			xiU1 = *(PM64) pbSrcU;
			xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
			xiU3 = *(PM64) (pbSrcU + iSrcUvStride * 2);
			xiU1 = _mm_and_si64(xiU1, mMaskAvg);
			xiU2 = _mm_and_si64(xiU2, mMaskAvg);
			xiU3 = _mm_and_si64(xiU3, mMaskAvg);
			xiU1 = _mm_srli_si64(xiU1, 1);
			xiU2 = _mm_srli_si64(xiU2, 1);
			xiU3 = _mm_srli_si64(xiU3, 1);
			xiU = _mm_adds_pu8(xiU1, xiU3);
			xiU = _mm_and_si64(xiU, mMaskAvg);
			xiU = _mm_srli_si64(xiU, 1);
			xiU = _mm_adds_pu8(xiU, xiU2);

			xiV1 = *(PM64) pbSrcV;
			xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
			xiV3 = *(PM64) (pbSrcV + iSrcUvStride * 2);
			xiV1 = _mm_and_si64(xiV1, mMaskAvg);
			xiV2 = _mm_and_si64(xiV2, mMaskAvg);
			xiV3 = _mm_and_si64(xiV3, mMaskAvg);
			xiV1 = _mm_srli_si64(xiV1, 1);
			xiV2 = _mm_srli_si64(xiV2, 1);
			xiV3 = _mm_srli_si64(xiV3, 1);
			xiV = _mm_adds_pu8(xiV1, xiV3);
			xiV = _mm_and_si64(xiV, mMaskAvg);
			xiV = _mm_srli_si64(xiV, 1);
			xiV = _mm_adds_pu8(xiV, xiV2);

			xiUv0 = _mm_unpacklo_pi8(xiU, xiV);
			xiUv1 = _mm_unpackhi_pi8(xiU, xiV);

			xiY01 = *(PM64) pbSrcY;
			xiY11 = *(PM64) (pbSrcY + 8);

			xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
			xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

			xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
			xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);

			xiY01 = _mm_and_si64(xiY01, mMaskAvg);
			xiY02 = _mm_and_si64(xiY02, mMaskAvg);
			xiY03 = _mm_and_si64(xiY03, mMaskAvg);
			xiY01 = _mm_srli_si64(xiY01, 1);
			xiY02 = _mm_srli_si64(xiY02, 1);
			xiY03 = _mm_srli_si64(xiY03, 1);
			xiY0 = _mm_adds_pu8(xiY01, xiY03);
			xiY0 = _mm_and_si64(xiY0, mMaskAvg);
			xiY0 = _mm_srli_si64(xiY0, 1);
			xiY0 = _mm_adds_pu8(xiY0, xiY02);

			xiY11 = _mm_and_si64(xiY11, mMaskAvg);
			xiY12 = _mm_and_si64(xiY12, mMaskAvg);
			xiY13 = _mm_and_si64(xiY13, mMaskAvg);
			xiY11 = _mm_srli_si64(xiY11, 1);
			xiY12 = _mm_srli_si64(xiY12, 1);
			xiY13 = _mm_srli_si64(xiY13, 1);
			xiY1 = _mm_adds_pu8(xiY11, xiY13);
			xiY1 = _mm_and_si64(xiY1, mMaskAvg);
			xiY1 = _mm_srli_si64(xiY1, 1);
			xiY1 = _mm_adds_pu8(xiY1, xiY12);

			xiY2 = *(PM64) (pbSrcY + iSrcYStride * 3 + 0);
			xiY3 = *(PM64) (pbSrcY + iSrcYStride * 3 + 8);
			xiY2 = _mm_and_si64(xiY2, mMaskAvg);
			xiY3 = _mm_and_si64(xiY3, mMaskAvg);
			xiY2 = _mm_srli_si64(xiY2, 1);
			xiY3 = _mm_srli_si64(xiY3, 1);
			xiY2 = _mm_adds_pu8(xiY02, xiY2);
			xiY2 = _mm_and_si64(xiY2, mMaskAvg);
			xiY2 = _mm_srli_si64(xiY2, 1);
			xiY2 = _mm_adds_pu8(xiY2, xiY03);
			xiY3 = _mm_adds_pu8(xiY12, xiY3);
			xiY3 = _mm_and_si64(xiY3, mMaskAvg);
			xiY3 = _mm_srli_si64(xiY3, 1);
			xiY3 = _mm_adds_pu8(xiY3, xiY13);

			*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiY0, xiUv0);
			*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiY0, xiUv0);
			*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiY1, xiUv1);
			*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiY1, xiUv1);

			*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiY2, xiUv0);
			*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiY2, xiUv0);
			*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiY3, xiUv1);
			*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiY3, xiUv1);

			pbDstX += 32;
			pbSrcY += 16;
			pbSrcU += 8;
			pbSrcV += 8;
		}
		for (x = (uWidth & 15) / 2; x; x--)
		{
			pbDstX[0] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			pbDstX[1] = ((pbSrcU[0] + pbSrcU[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			pbDstX[2] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
			pbDstX[3] = ((pbSrcV[0] + pbSrcV[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcV[iSrcUvStride + 0] + 1) / 2;
			
			pbDstX[iDstXStride + 0] = ((pbSrcY[iSrcYStride + 0] + pbSrcY[iSrcYStride * 3 + 0] + 1) / 2 +
				pbSrcY[iSrcYStride * 2 + 0] + 1) / 2;
			pbDstX[iDstXStride + 1] = pbDstX[1];
			pbDstX[iDstXStride + 2] = ((pbSrcY[iSrcYStride + 1] + pbSrcY[iSrcYStride * 3 + 1] + 1) / 2 +
				pbSrcY[iSrcYStride * 2 + 1] + 1) / 2;
			pbDstX[iDstXStride + 3] = pbDstX[3];

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
	for (x = uWidth / 16 ; x ; x--)
	{
		xiU1 = *(PM64) pbSrcU;
		xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
		xiU1 = _mm_and_si64(xiU1, mMaskAvg);
		xiU2 = _mm_and_si64(xiU2, mMaskAvg);
		xiU1 = _mm_srli_si64(xiU1, 1);
		xiU2 = _mm_srli_si64(xiU2, 1);
		xiU = _mm_adds_pu8(xiU1, xiU2);
		xiV1 = *(PM64) pbSrcV;
		xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
		xiV1 = _mm_and_si64(xiV1, mMaskAvg);
		xiV2 = _mm_and_si64(xiV2, mMaskAvg);
		xiV1 = _mm_srli_si64(xiV1, 1);
		xiV2 = _mm_srli_si64(xiV2, 1);
		xiV = _mm_adds_pu8(xiV1, xiV2);
		xiUv0 = _mm_unpacklo_pi8(xiU, xiV);
		xiUv1 = _mm_unpackhi_pi8(xiU, xiV);

		xiY01 = *(PM64) pbSrcY;
		xiY11 = *(PM64) (pbSrcY + 8);


		xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
		xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

		xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
		xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);
		xiY2 = xiY03;
		xiY3 = xiY13;

		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY02 = _mm_and_si64(xiY02, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_srli_si64(xiY02, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY0 = _mm_adds_pu8(xiY01, xiY03);
		xiY0 = _mm_and_si64(xiY0, mMaskAvg);
		xiY0 = _mm_srli_si64(xiY0, 1);
		xiY0 = _mm_adds_pu8(xiY0, xiY02);

		xiY11 = _mm_and_si64(xiY11, mMaskAvg);
		xiY12 = _mm_and_si64(xiY12, mMaskAvg);
		xiY13 = _mm_and_si64(xiY13, mMaskAvg);
		xiY11 = _mm_srli_si64(xiY11, 1);
		xiY12 = _mm_srli_si64(xiY12, 1);
		xiY13 = _mm_srli_si64(xiY13, 1);
		xiY1 = _mm_adds_pu8(xiY11, xiY13);
		xiY1 = _mm_and_si64(xiY1, mMaskAvg);
		xiY1 = _mm_srli_si64(xiY1, 1);
		xiY1 = _mm_adds_pu8(xiY1, xiY12);

		*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiY0, xiUv0);
		*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiY0, xiUv0);
		*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiY1, xiUv1);
		*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiY1, xiUv1);

		*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiY2, xiUv0);
		*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiY2, xiUv0);
		*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiY3, xiUv1);
		*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiY3, xiUv1);

		pbDstX += 32;
		pbSrcY += 16;
		pbSrcU += 8;
		pbSrcV += 8;
	}
	for (x = (uWidth & 15) / 2; x; x--)
	{
		pbDstX[0] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		pbDstX[1] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;
		pbDstX[2] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
		pbDstX[3] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;

		pbDstX[iDstXStride + 0] = pbSrcY[iSrcYStride * 2 + 0];
		pbDstX[iDstXStride + 1] = pbDstX[1];
		pbDstX[iDstXStride + 2] = pbSrcY[iSrcYStride * 2 + 1];
		pbDstX[iDstXStride + 3] = pbDstX[3];

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YVYU with de-interlacing.
VOID Yv12ToYvyuDI_mmx(PBYTE pbDstX, INT iDstXStride,
				PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	M64 xiU, xiV, xiUv0, xiUv1, 
		xiY01,xiY02,xiY03,xiY11,xiY12,xiY13,
		xiY0,xiY1,xiY2,xiY3,
		xiU1,xiU2,xiU3,xiV1,xiV2,xiV3,
		mMaskAvg;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - 2 * uWidth;
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	mMaskAvg = g_mMaskAvg;
	for (x = uWidth / 16 ; x ; x--)
	{
		xiU1 = *(PM64) pbSrcU;
		xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
		xiU1 = _mm_and_si64(xiU1, mMaskAvg);
		xiU2 = _mm_and_si64(xiU2, mMaskAvg);
		xiU1 = _mm_srli_si64(xiU1, 1);
		xiU2 = _mm_srli_si64(xiU2, 1);
		xiU = _mm_adds_pu8(xiU1, xiU2);
		xiV1 = *(PM64) pbSrcV;
		xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
		xiV1 = _mm_and_si64(xiV1, mMaskAvg);
		xiV2 = _mm_and_si64(xiV2, mMaskAvg);
		xiV1 = _mm_srli_si64(xiV1, 1);
		xiV2 = _mm_srli_si64(xiV2, 1);
		xiV = _mm_adds_pu8(xiV1, xiV2);
		xiUv0 = _mm_unpacklo_pi8(xiV, xiU);
		xiUv1 = _mm_unpackhi_pi8(xiV, xiU);

		xiY01 = *(PM64) pbSrcY;
		xiY11 = *(PM64) (pbSrcY + 8);

		xiY0 = xiY01;
		xiY1 = xiY11;

		xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
		xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

		xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
		xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);

		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY02 = _mm_and_si64(xiY02, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_srli_si64(xiY02, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY2 = _mm_adds_pu8(xiY01, xiY03);
		xiY2 = _mm_and_si64(xiY2, mMaskAvg);
		xiY2 = _mm_srli_si64(xiY2, 1);
		xiY2 = _mm_adds_pu8(xiY2, xiY02);

		xiY11 = _mm_and_si64(xiY11, mMaskAvg);
		xiY12 = _mm_and_si64(xiY12, mMaskAvg);
		xiY13 = _mm_and_si64(xiY13, mMaskAvg);
		xiY11 = _mm_srli_si64(xiY11, 1);
		xiY12 = _mm_srli_si64(xiY12, 1);
		xiY13 = _mm_srli_si64(xiY13, 1);
		xiY3 = _mm_adds_pu8(xiY11, xiY13);
		xiY3 = _mm_and_si64(xiY3, mMaskAvg);
		xiY3 = _mm_srli_si64(xiY3, 1);
		xiY3 = _mm_adds_pu8(xiY3, xiY12);

		*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiY0, xiUv0);
		*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiY0, xiUv0);
		*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiY1, xiUv1);
		*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiY1, xiUv1);

		*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiY2, xiUv0);
		*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiY2, xiUv0);
		*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiY3, xiUv1);
		*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiY3, xiUv1);

		pbDstX += 32;
		pbSrcY += 16;
		pbSrcU += 8;
		pbSrcV += 8;
	}
	for (x = (uWidth & 15) / 2; x; x--)
	{
		pbDstX[0] = pbSrcY[0];
		pbDstX[1] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;
		pbDstX[2] = pbSrcY[1];
		pbDstX[3] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;

		pbDstX[iDstXStride + 0] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		pbDstX[iDstXStride + 1] = pbDstX[1];
		pbDstX[iDstXStride + 2] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
		pbDstX[iDstXStride + 3] = pbDstX[3];

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	pbDstX += iDstXDif + iDstXStride;
	pbSrcY += iSrcYDif ;
	pbSrcU += iSrcUvDif - iSrcUvStride;
	pbSrcV += iSrcUvDif - iSrcUvStride;
	for (y = (iHeight / 2) - 2; y; y--)
	{
		for (x = uWidth / 16 ; x ; x--)
		{
			xiU1 = *(PM64) pbSrcU;
			xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
			xiU3 = *(PM64) (pbSrcU + iSrcUvStride * 2);
			xiU1 = _mm_and_si64(xiU1, mMaskAvg);
			xiU2 = _mm_and_si64(xiU2, mMaskAvg);
			xiU3 = _mm_and_si64(xiU3, mMaskAvg);
			xiU1 = _mm_srli_si64(xiU1, 1);
			xiU2 = _mm_srli_si64(xiU2, 1);
			xiU3 = _mm_srli_si64(xiU3, 1);
			xiU = _mm_adds_pu8(xiU1, xiU3);
			xiU = _mm_and_si64(xiU, mMaskAvg);
			xiU = _mm_srli_si64(xiU, 1);
			xiU = _mm_adds_pu8(xiU, xiU2);

			xiV1 = *(PM64) pbSrcV;
			xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
			xiV3 = *(PM64) (pbSrcV + iSrcUvStride * 2);
			xiV1 = _mm_and_si64(xiV1, mMaskAvg);
			xiV2 = _mm_and_si64(xiV2, mMaskAvg);
			xiV3 = _mm_and_si64(xiV3, mMaskAvg);
			xiV1 = _mm_srli_si64(xiV1, 1);
			xiV2 = _mm_srli_si64(xiV2, 1);
			xiV3 = _mm_srli_si64(xiV3, 1);
			xiV = _mm_adds_pu8(xiV1, xiV3);
			xiV = _mm_and_si64(xiV, mMaskAvg);
			xiV = _mm_srli_si64(xiV, 1);
			xiV = _mm_adds_pu8(xiV, xiV2);

			xiUv0 = _mm_unpacklo_pi8(xiV, xiU);
			xiUv1 = _mm_unpackhi_pi8(xiV, xiU);

			xiY01 = *(PM64) pbSrcY;
			xiY11 = *(PM64) (pbSrcY + 8);

			xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
			xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

			xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
			xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);

			xiY01 = _mm_and_si64(xiY01, mMaskAvg);
			xiY02 = _mm_and_si64(xiY02, mMaskAvg);
			xiY03 = _mm_and_si64(xiY03, mMaskAvg);
			xiY01 = _mm_srli_si64(xiY01, 1);
			xiY02 = _mm_srli_si64(xiY02, 1);
			xiY03 = _mm_srli_si64(xiY03, 1);
			xiY0 = _mm_adds_pu8(xiY01, xiY03);
			xiY0 = _mm_and_si64(xiY0, mMaskAvg);
			xiY0 = _mm_srli_si64(xiY0, 1);
			xiY0 = _mm_adds_pu8(xiY0, xiY02);

			xiY11 = _mm_and_si64(xiY11, mMaskAvg);
			xiY12 = _mm_and_si64(xiY12, mMaskAvg);
			xiY13 = _mm_and_si64(xiY13, mMaskAvg);
			xiY11 = _mm_srli_si64(xiY11, 1);
			xiY12 = _mm_srli_si64(xiY12, 1);
			xiY13 = _mm_srli_si64(xiY13, 1);
			xiY1 = _mm_adds_pu8(xiY11, xiY13);
			xiY1 = _mm_and_si64(xiY1, mMaskAvg);
			xiY1 = _mm_srli_si64(xiY1, 1);
			xiY1 = _mm_adds_pu8(xiY1, xiY12);

			xiY2 = *(PM64) (pbSrcY + iSrcYStride * 3 + 0);
			xiY3 = *(PM64) (pbSrcY + iSrcYStride * 3 + 8);
			xiY2 = _mm_and_si64(xiY2, mMaskAvg);
			xiY3 = _mm_and_si64(xiY3, mMaskAvg);
			xiY2 = _mm_srli_si64(xiY2, 1);
			xiY3 = _mm_srli_si64(xiY3, 1);
			xiY2 = _mm_adds_pu8(xiY02, xiY2);
			xiY2 = _mm_and_si64(xiY2, mMaskAvg);
			xiY2 = _mm_srli_si64(xiY2, 1);
			xiY2 = _mm_adds_pu8(xiY2, xiY03);
			xiY3 = _mm_adds_pu8(xiY12, xiY3);
			xiY3 = _mm_and_si64(xiY3, mMaskAvg);
			xiY3 = _mm_srli_si64(xiY3, 1);
			xiY3 = _mm_adds_pu8(xiY3, xiY13);

			*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiY0, xiUv0);
			*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiY0, xiUv0);
			*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiY1, xiUv1);
			*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiY1, xiUv1);

			*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiY2, xiUv0);
			*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiY2, xiUv0);
			*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiY3, xiUv1);
			*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiY3, xiUv1);

			pbDstX += 32;
			pbSrcY += 16;
			pbSrcU += 8;
			pbSrcV += 8;
		}
		for (x = (uWidth & 15) / 2; x; x--)
		{
			pbDstX[0] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			pbDstX[1] = ((pbSrcV[0] + pbSrcV[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcV[iSrcUvStride + 0] + 1) / 2;
			pbDstX[2] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
			pbDstX[3] = ((pbSrcU[0] + pbSrcU[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			
			pbDstX[iDstXStride + 0] = ((pbSrcY[iSrcYStride + 0] + pbSrcY[iSrcYStride * 3 + 0] + 1) / 2 +
				pbSrcY[iSrcYStride * 2 + 0] + 1) / 2;
			pbDstX[iDstXStride + 1] = pbDstX[1];
			pbDstX[iDstXStride + 2] = ((pbSrcY[iSrcYStride + 1] + pbSrcY[iSrcYStride * 3 + 1] + 1) / 2 +
				pbSrcY[iSrcYStride * 2 + 1] + 1) / 2;
			pbDstX[iDstXStride + 3] = pbDstX[3];

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
	for (x = uWidth / 16 ; x ; x--)
	{
		xiU1 = *(PM64) pbSrcU;
		xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
		xiU1 = _mm_and_si64(xiU1, mMaskAvg);
		xiU2 = _mm_and_si64(xiU2, mMaskAvg);
		xiU1 = _mm_srli_si64(xiU1, 1);
		xiU2 = _mm_srli_si64(xiU2, 1);
		xiU = _mm_adds_pu8(xiU1, xiU2);
		xiV1 = *(PM64) pbSrcV;
		xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
		xiV1 = _mm_and_si64(xiV1, mMaskAvg);
		xiV2 = _mm_and_si64(xiV2, mMaskAvg);
		xiV1 = _mm_srli_si64(xiV1, 1);
		xiV2 = _mm_srli_si64(xiV2, 1);
		xiV = _mm_adds_pu8(xiV1, xiV2);
		xiUv0 = _mm_unpacklo_pi8(xiV, xiU);
		xiUv1 = _mm_unpackhi_pi8(xiV, xiU);

		xiY01 = *(PM64) pbSrcY;
		xiY11 = *(PM64) (pbSrcY + 8);


		xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
		xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

		xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
		xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);
		xiY2 = xiY03;
		xiY3 = xiY13;

		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY02 = _mm_and_si64(xiY02, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_srli_si64(xiY02, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY0 = _mm_adds_pu8(xiY01, xiY03);
		xiY0 = _mm_and_si64(xiY0, mMaskAvg);
		xiY0 = _mm_srli_si64(xiY0, 1);
		xiY0 = _mm_adds_pu8(xiY0, xiY02);

		xiY11 = _mm_and_si64(xiY11, mMaskAvg);
		xiY12 = _mm_and_si64(xiY12, mMaskAvg);
		xiY13 = _mm_and_si64(xiY13, mMaskAvg);
		xiY11 = _mm_srli_si64(xiY11, 1);
		xiY12 = _mm_srli_si64(xiY12, 1);
		xiY13 = _mm_srli_si64(xiY13, 1);
		xiY1 = _mm_adds_pu8(xiY11, xiY13);
		xiY1 = _mm_and_si64(xiY1, mMaskAvg);
		xiY1 = _mm_srli_si64(xiY1, 1);
		xiY1 = _mm_adds_pu8(xiY1, xiY12);

		*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiY0, xiUv0);
		*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiY0, xiUv0);
		*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiY1, xiUv1);
		*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiY1, xiUv1);

		*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiY2, xiUv0);
		*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiY2, xiUv0);
		*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiY3, xiUv1);
		*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiY3, xiUv1);

		pbDstX += 32;
		pbSrcY += 16;
		pbSrcU += 8;
		pbSrcV += 8;
	}
	for (x = (uWidth & 15) / 2; x; x--)
	{
		pbDstX[0] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		pbDstX[1] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;
		pbDstX[2] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
		pbDstX[3] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;

		pbDstX[iDstXStride + 0] = pbSrcY[iSrcYStride * 2 + 0];
		pbDstX[iDstXStride + 1] = pbDstX[1];
		pbDstX[iDstXStride + 2] = pbSrcY[iSrcYStride * 2 + 1];
		pbDstX[iDstXStride + 3] = pbDstX[3];

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to UYVY with de-interlacing.
VOID Yv12ToUyvyDI_mmx(PBYTE pbDstX, INT iDstXStride,
				PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	M64 xiU, xiV, xiUv0, xiUv1, 
		xiY01,xiY02,xiY03,xiY11,xiY12,xiY13,
		xiY0,xiY1,xiY2,xiY3,
		xiU1,xiU2,xiU3,xiV1,xiV2,xiV3,
		mMaskAvg;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - 2 * uWidth;
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	mMaskAvg = g_mMaskAvg;
	for (x = uWidth / 16 ; x ; x--)
	{
		xiU1 = *(PM64) pbSrcU;
		xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
		xiU1 = _mm_and_si64(xiU1, mMaskAvg);
		xiU2 = _mm_and_si64(xiU2, mMaskAvg);
		xiU1 = _mm_srli_si64(xiU1, 1);
		xiU2 = _mm_srli_si64(xiU2, 1);
		xiU = _mm_adds_pu8(xiU1, xiU2);
		xiV1 = *(PM64) pbSrcV;
		xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
		xiV1 = _mm_and_si64(xiV1, mMaskAvg);
		xiV2 = _mm_and_si64(xiV2, mMaskAvg);
		xiV1 = _mm_srli_si64(xiV1, 1);
		xiV2 = _mm_srli_si64(xiV2, 1);
		xiV = _mm_adds_pu8(xiV1, xiV2);
		xiUv0 = _mm_unpacklo_pi8(xiU, xiV);
		xiUv1 = _mm_unpackhi_pi8(xiU, xiV);

		xiY01 = *(PM64) pbSrcY;
		xiY11 = *(PM64) (pbSrcY + 8);

		xiY0 = xiY01;
		xiY1 = xiY11;

		xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
		xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

		xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
		xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);

		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY02 = _mm_and_si64(xiY02, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_srli_si64(xiY02, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY2 = _mm_adds_pu8(xiY01, xiY03);
		xiY2 = _mm_and_si64(xiY2, mMaskAvg);
		xiY2 = _mm_srli_si64(xiY2, 1);
		xiY2 = _mm_adds_pu8(xiY2, xiY02);

		xiY11 = _mm_and_si64(xiY11, mMaskAvg);
		xiY12 = _mm_and_si64(xiY12, mMaskAvg);
		xiY13 = _mm_and_si64(xiY13, mMaskAvg);
		xiY11 = _mm_srli_si64(xiY11, 1);
		xiY12 = _mm_srli_si64(xiY12, 1);
		xiY13 = _mm_srli_si64(xiY13, 1);
		xiY3 = _mm_adds_pu8(xiY11, xiY13);
		xiY3 = _mm_and_si64(xiY3, mMaskAvg);
		xiY3 = _mm_srli_si64(xiY3, 1);
		xiY3 = _mm_adds_pu8(xiY3, xiY12);

		*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiUv0, xiY0);
		*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiUv0, xiY0);
		*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiUv1, xiY1);
		*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiUv1, xiY1);

		*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiUv0, xiY2);
		*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiUv0, xiY2);
		*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiUv1, xiY3);
		*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiUv1, xiY3);

		pbDstX += 32;
		pbSrcY += 16;
		pbSrcU += 8;
		pbSrcV += 8;
	}
	for (x = (uWidth & 15) / 2; x; x--)
	{
		pbDstX[0] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;
		pbDstX[1] = pbSrcY[0];
		pbDstX[2] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;
		pbDstX[3] = pbSrcY[1];

		pbDstX[iDstXStride + 0] = pbDstX[0];
		pbDstX[iDstXStride + 1] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		pbDstX[iDstXStride + 2] = pbDstX[2];
		pbDstX[iDstXStride + 3] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	pbDstX += iDstXDif + iDstXStride;
	pbSrcY += iSrcYDif ;
	pbSrcU += iSrcUvDif - iSrcUvStride;
	pbSrcV += iSrcUvDif - iSrcUvStride;
	for (y = (iHeight / 2) - 2; y; y--)
	{
		for (x = uWidth / 16 ; x ; x--)
		{
			xiU1 = *(PM64) pbSrcU;
			xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
			xiU3 = *(PM64) (pbSrcU + iSrcUvStride * 2);
			xiU1 = _mm_and_si64(xiU1, mMaskAvg);
			xiU2 = _mm_and_si64(xiU2, mMaskAvg);
			xiU3 = _mm_and_si64(xiU3, mMaskAvg);
			xiU1 = _mm_srli_si64(xiU1, 1);
			xiU2 = _mm_srli_si64(xiU2, 1);
			xiU3 = _mm_srli_si64(xiU3, 1);
			xiU = _mm_adds_pu8(xiU1, xiU3);
			xiU = _mm_and_si64(xiU, mMaskAvg);
			xiU = _mm_srli_si64(xiU, 1);
			xiU = _mm_adds_pu8(xiU, xiU2);

			xiV1 = *(PM64) pbSrcV;
			xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
			xiV3 = *(PM64) (pbSrcV + iSrcUvStride * 2);
			xiV1 = _mm_and_si64(xiV1, mMaskAvg);
			xiV2 = _mm_and_si64(xiV2, mMaskAvg);
			xiV3 = _mm_and_si64(xiV3, mMaskAvg);
			xiV1 = _mm_srli_si64(xiV1, 1);
			xiV2 = _mm_srli_si64(xiV2, 1);
			xiV3 = _mm_srli_si64(xiV3, 1);
			xiV = _mm_adds_pu8(xiV1, xiV3);
			xiV = _mm_and_si64(xiV, mMaskAvg);
			xiV = _mm_srli_si64(xiV, 1);
			xiV = _mm_adds_pu8(xiV, xiV2);

			xiUv0 = _mm_unpacklo_pi8(xiU, xiV);
			xiUv1 = _mm_unpackhi_pi8(xiU, xiV);

			xiY01 = *(PM64) pbSrcY;
			xiY11 = *(PM64) (pbSrcY + 8);

			xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
			xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

			xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
			xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);

			xiY01 = _mm_and_si64(xiY01, mMaskAvg);
			xiY02 = _mm_and_si64(xiY02, mMaskAvg);
			xiY03 = _mm_and_si64(xiY03, mMaskAvg);
			xiY01 = _mm_srli_si64(xiY01, 1);
			xiY02 = _mm_srli_si64(xiY02, 1);
			xiY03 = _mm_srli_si64(xiY03, 1);
			xiY0 = _mm_adds_pu8(xiY01, xiY03);
			xiY0 = _mm_and_si64(xiY0, mMaskAvg);
			xiY0 = _mm_srli_si64(xiY0, 1);
			xiY0 = _mm_adds_pu8(xiY0, xiY02);

			xiY11 = _mm_and_si64(xiY11, mMaskAvg);
			xiY12 = _mm_and_si64(xiY12, mMaskAvg);
			xiY13 = _mm_and_si64(xiY13, mMaskAvg);
			xiY11 = _mm_srli_si64(xiY11, 1);
			xiY12 = _mm_srli_si64(xiY12, 1);
			xiY13 = _mm_srli_si64(xiY13, 1);
			xiY1 = _mm_adds_pu8(xiY11, xiY13);
			xiY1 = _mm_and_si64(xiY1, mMaskAvg);
			xiY1 = _mm_srli_si64(xiY1, 1);
			xiY1 = _mm_adds_pu8(xiY1, xiY12);

			xiY2 = *(PM64) (pbSrcY + iSrcYStride * 3 + 0);
			xiY3 = *(PM64) (pbSrcY + iSrcYStride * 3 + 8);
			xiY2 = _mm_and_si64(xiY2, mMaskAvg);
			xiY3 = _mm_and_si64(xiY3, mMaskAvg);
			xiY2 = _mm_srli_si64(xiY2, 1);
			xiY3 = _mm_srli_si64(xiY3, 1);
			xiY2 = _mm_adds_pu8(xiY02, xiY2);
			xiY2 = _mm_and_si64(xiY2, mMaskAvg);
			xiY2 = _mm_srli_si64(xiY2, 1);
			xiY2 = _mm_adds_pu8(xiY2, xiY03);
			xiY3 = _mm_adds_pu8(xiY12, xiY3);
			xiY3 = _mm_and_si64(xiY3, mMaskAvg);
			xiY3 = _mm_srli_si64(xiY3, 1);
			xiY3 = _mm_adds_pu8(xiY3, xiY13);

			*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiUv0, xiY0);
			*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiUv0, xiY0);
			*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiUv1, xiY1);
			*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiUv1, xiY1);

			*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiUv0, xiY2);
			*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiUv0, xiY2);
			*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiUv1, xiY3);
			*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiUv1, xiY3);

			pbDstX += 32;
			pbSrcY += 16;
			pbSrcU += 8;
			pbSrcV += 8;
		}
		for (x = (uWidth & 15) / 2; x; x--)
		{
			pbDstX[0] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			pbDstX[1] = pbSrcY[0];
			pbDstX[2] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;
			pbDstX[3] = pbSrcY[1];

			pbDstX[iDstXStride + 0] = pbDstX[0];
			pbDstX[iDstXStride + 1] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			pbDstX[iDstXStride + 2] = pbDstX[2];
			pbDstX[iDstXStride + 3] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
			
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
	for (x = uWidth / 16 ; x ; x--)
	{
		xiU1 = *(PM64) pbSrcU;
		xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
		xiU1 = _mm_and_si64(xiU1, mMaskAvg);
		xiU2 = _mm_and_si64(xiU2, mMaskAvg);
		xiU1 = _mm_srli_si64(xiU1, 1);
		xiU2 = _mm_srli_si64(xiU2, 1);
		xiU = _mm_adds_pu8(xiU1, xiU2);
		xiV1 = *(PM64) pbSrcV;
		xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
		xiV1 = _mm_and_si64(xiV1, mMaskAvg);
		xiV2 = _mm_and_si64(xiV2, mMaskAvg);
		xiV1 = _mm_srli_si64(xiV1, 1);
		xiV2 = _mm_srli_si64(xiV2, 1);
		xiV = _mm_adds_pu8(xiV1, xiV2);
		xiUv0 = _mm_unpacklo_pi8(xiU, xiV);
		xiUv1 = _mm_unpackhi_pi8(xiU, xiV);

		xiY01 = *(PM64) pbSrcY;
		xiY11 = *(PM64) (pbSrcY + 8);


		xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
		xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

		xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
		xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);
		xiY2 = xiY03;
		xiY3 = xiY13;

		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY02 = _mm_and_si64(xiY02, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_srli_si64(xiY02, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY0 = _mm_adds_pu8(xiY01, xiY03);
		xiY0 = _mm_and_si64(xiY0, mMaskAvg);
		xiY0 = _mm_srli_si64(xiY0, 1);
		xiY0 = _mm_adds_pu8(xiY0, xiY02);

		xiY11 = _mm_and_si64(xiY11, mMaskAvg);
		xiY12 = _mm_and_si64(xiY12, mMaskAvg);
		xiY13 = _mm_and_si64(xiY13, mMaskAvg);
		xiY11 = _mm_srli_si64(xiY11, 1);
		xiY12 = _mm_srli_si64(xiY12, 1);
		xiY13 = _mm_srli_si64(xiY13, 1);
		xiY1 = _mm_adds_pu8(xiY11, xiY13);
		xiY1 = _mm_and_si64(xiY1, mMaskAvg);
		xiY1 = _mm_srli_si64(xiY1, 1);
		xiY1 = _mm_adds_pu8(xiY1, xiY12);

		*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiUv0, xiY0);
		*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiUv0, xiY0);
		*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiUv1, xiY1);
		*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiUv1, xiY1);

		*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiUv0, xiY2);
		*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiUv0, xiY2);
		*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiUv1, xiY3);
		*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiUv1, xiY3);

		pbDstX += 32;
		pbSrcY += 16;
		pbSrcU += 8;
		pbSrcV += 8;
	}
	for (x = (uWidth & 15) / 2; x; x--)
	{
		pbDstX[0] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;
		pbDstX[1] = pbSrcY[0];
		pbDstX[2] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;
		pbDstX[3] = pbSrcY[1];

		pbDstX[iDstXStride + 0] = pbDstX[0];
		pbDstX[iDstXStride + 1] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		pbDstX[iDstXStride + 2] = pbDstX[2];
		pbDstX[iDstXStride + 3] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to VYUY with de-interlacing.
VOID Yv12ToVyuyDI_mmx(PBYTE pbDstX, INT iDstXStride,
				PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	M64 xiU, xiV, xiUv0, xiUv1, 
		xiY01,xiY02,xiY03,xiY11,xiY12,xiY13,
		xiY0,xiY1,xiY2,xiY3,
		xiU1,xiU2,xiU3,xiV1,xiV2,xiV3,
		mMaskAvg;
	
	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - 2 * uWidth;
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	mMaskAvg = g_mMaskAvg;
	for (x = uWidth / 16 ; x ; x--)
	{
		xiU1 = *(PM64) pbSrcU;
		xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
		xiU1 = _mm_and_si64(xiU1, mMaskAvg);
		xiU2 = _mm_and_si64(xiU2, mMaskAvg);
		xiU1 = _mm_srli_si64(xiU1, 1);
		xiU2 = _mm_srli_si64(xiU2, 1);
		xiU = _mm_adds_pu8(xiU1, xiU2);
		xiV1 = *(PM64) pbSrcV;
		xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
		xiV1 = _mm_and_si64(xiV1, mMaskAvg);
		xiV2 = _mm_and_si64(xiV2, mMaskAvg);
		xiV1 = _mm_srli_si64(xiV1, 1);
		xiV2 = _mm_srli_si64(xiV2, 1);
		xiV = _mm_adds_pu8(xiV1, xiV2);
		xiUv0 = _mm_unpacklo_pi8(xiV, xiU);
		xiUv1 = _mm_unpackhi_pi8(xiV, xiU);

		xiY01 = *(PM64) pbSrcY;
		xiY11 = *(PM64) (pbSrcY + 8);

		xiY0 = xiY01;
		xiY1 = xiY11;

		xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
		xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

		xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
		xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);

		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY02 = _mm_and_si64(xiY02, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_srli_si64(xiY02, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY2 = _mm_adds_pu8(xiY01, xiY03);
		xiY2 = _mm_and_si64(xiY2, mMaskAvg);
		xiY2 = _mm_srli_si64(xiY2, 1);
		xiY2 = _mm_adds_pu8(xiY2, xiY02);

		xiY11 = _mm_and_si64(xiY11, mMaskAvg);
		xiY12 = _mm_and_si64(xiY12, mMaskAvg);
		xiY13 = _mm_and_si64(xiY13, mMaskAvg);
		xiY11 = _mm_srli_si64(xiY11, 1);
		xiY12 = _mm_srli_si64(xiY12, 1);
		xiY13 = _mm_srli_si64(xiY13, 1);
		xiY3 = _mm_adds_pu8(xiY11, xiY13);
		xiY3 = _mm_and_si64(xiY3, mMaskAvg);
		xiY3 = _mm_srli_si64(xiY3, 1);
		xiY3 = _mm_adds_pu8(xiY3, xiY12);

		*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiUv0, xiY0);
		*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiUv0, xiY0);
		*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiUv1, xiY1);
		*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiUv1, xiY1);

		*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiUv0, xiY2);
		*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiUv0, xiY2);
		*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiUv1, xiY3);
		*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiUv1, xiY3);

		pbDstX += 32;
		pbSrcY += 16;
		pbSrcU += 8;
		pbSrcV += 8;
	}
	for (x = (uWidth & 15) / 2; x; x--)
	{
		pbDstX[0] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;
		pbDstX[1] = pbSrcY[0];
		pbDstX[2] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;
		pbDstX[3] = pbSrcY[1];

		pbDstX[iDstXStride + 0] = pbDstX[0];
		pbDstX[iDstXStride + 1] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		pbDstX[iDstXStride + 2] = pbDstX[2];
		pbDstX[iDstXStride + 3] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	pbDstX += iDstXDif + iDstXStride;
	pbSrcY += iSrcYDif ;
	pbSrcU += iSrcUvDif - iSrcUvStride;
	pbSrcV += iSrcUvDif - iSrcUvStride;
	for (y = (iHeight / 2) - 2; y; y--)
	{
		for (x = uWidth / 16 ; x ; x--)
		{
			xiU1 = *(PM64) pbSrcU;
			xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
			xiU3 = *(PM64) (pbSrcU + iSrcUvStride * 2);
			xiU1 = _mm_and_si64(xiU1, mMaskAvg);
			xiU2 = _mm_and_si64(xiU2, mMaskAvg);
			xiU3 = _mm_and_si64(xiU3, mMaskAvg);
			xiU1 = _mm_srli_si64(xiU1, 1);
			xiU2 = _mm_srli_si64(xiU2, 1);
			xiU3 = _mm_srli_si64(xiU3, 1);
			xiU = _mm_adds_pu8(xiU1, xiU3);
			xiU = _mm_and_si64(xiU, mMaskAvg);
			xiU = _mm_srli_si64(xiU, 1);
			xiU = _mm_adds_pu8(xiU, xiU2);

			xiV1 = *(PM64) pbSrcV;
			xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
			xiV3 = *(PM64) (pbSrcV + iSrcUvStride * 2);
			xiV1 = _mm_and_si64(xiV1, mMaskAvg);
			xiV2 = _mm_and_si64(xiV2, mMaskAvg);
			xiV3 = _mm_and_si64(xiV3, mMaskAvg);
			xiV1 = _mm_srli_si64(xiV1, 1);
			xiV2 = _mm_srli_si64(xiV2, 1);
			xiV3 = _mm_srli_si64(xiV3, 1);
			xiV = _mm_adds_pu8(xiV1, xiV3);
			xiV = _mm_and_si64(xiV, mMaskAvg);
			xiV = _mm_srli_si64(xiV, 1);
			xiV = _mm_adds_pu8(xiV, xiV2);

			xiUv0 = _mm_unpacklo_pi8(xiV, xiU);
			xiUv1 = _mm_unpackhi_pi8(xiV, xiU);

			xiY01 = *(PM64) pbSrcY;
			xiY11 = *(PM64) (pbSrcY + 8);

			xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
			xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

			xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
			xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);

			xiY01 = _mm_and_si64(xiY01, mMaskAvg);
			xiY02 = _mm_and_si64(xiY02, mMaskAvg);
			xiY03 = _mm_and_si64(xiY03, mMaskAvg);
			xiY01 = _mm_srli_si64(xiY01, 1);
			xiY02 = _mm_srli_si64(xiY02, 1);
			xiY03 = _mm_srli_si64(xiY03, 1);
			xiY0 = _mm_adds_pu8(xiY01, xiY03);
			xiY0 = _mm_and_si64(xiY0, mMaskAvg);
			xiY0 = _mm_srli_si64(xiY0, 1);
			xiY0 = _mm_adds_pu8(xiY0, xiY02);

			xiY11 = _mm_and_si64(xiY11, mMaskAvg);
			xiY12 = _mm_and_si64(xiY12, mMaskAvg);
			xiY13 = _mm_and_si64(xiY13, mMaskAvg);
			xiY11 = _mm_srli_si64(xiY11, 1);
			xiY12 = _mm_srli_si64(xiY12, 1);
			xiY13 = _mm_srli_si64(xiY13, 1);
			xiY1 = _mm_adds_pu8(xiY11, xiY13);
			xiY1 = _mm_and_si64(xiY1, mMaskAvg);
			xiY1 = _mm_srli_si64(xiY1, 1);
			xiY1 = _mm_adds_pu8(xiY1, xiY12);

			xiY2 = *(PM64) (pbSrcY + iSrcYStride * 3 + 0);
			xiY3 = *(PM64) (pbSrcY + iSrcYStride * 3 + 8);
			xiY2 = _mm_and_si64(xiY2, mMaskAvg);
			xiY3 = _mm_and_si64(xiY3, mMaskAvg);
			xiY2 = _mm_srli_si64(xiY2, 1);
			xiY3 = _mm_srli_si64(xiY3, 1);
			xiY2 = _mm_adds_pu8(xiY02, xiY2);
			xiY2 = _mm_and_si64(xiY2, mMaskAvg);
			xiY2 = _mm_srli_si64(xiY2, 1);
			xiY2 = _mm_adds_pu8(xiY2, xiY03);
			xiY3 = _mm_adds_pu8(xiY12, xiY3);
			xiY3 = _mm_and_si64(xiY3, mMaskAvg);
			xiY3 = _mm_srli_si64(xiY3, 1);
			xiY3 = _mm_adds_pu8(xiY3, xiY13);

			*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiUv0, xiY0);
			*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiUv0, xiY0);
			*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiUv1, xiY1);
			*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiUv1, xiY1);

			*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiUv0, xiY2);
			*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiUv0, xiY2);
			*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiUv1, xiY3);
			*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiUv1, xiY3);

			pbDstX += 32;
			pbSrcY += 16;
			pbSrcU += 8;
			pbSrcV += 8;
		}
		for (x = (uWidth & 15) / 2; x; x--)
		{
			pbDstX[0] = ((pbSrcV[0] + pbSrcV[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcV[iSrcUvStride + 0] + 1) / 2;
			pbDstX[1] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			pbDstX[2] = ((pbSrcU[0] + pbSrcU[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			pbDstX[3] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

			pbDstX[iDstXStride + 0] = pbDstX[1];
			pbDstX[iDstXStride + 1] = ((pbSrcY[iSrcYStride + 0] + pbSrcY[iSrcYStride * 3 + 0] + 1) / 2 +
				pbSrcY[iSrcYStride * 2 + 0] + 1) / 2;
			pbDstX[iDstXStride + 2] = pbDstX[3];
			pbDstX[iDstXStride + 3] = ((pbSrcY[iSrcYStride + 1] + pbSrcY[iSrcYStride * 3 + 1] + 1) / 2 +
				pbSrcY[iSrcYStride * 2 + 1] + 1) / 2;
			
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
	for (x = uWidth / 16 ; x ; x--)
	{
		xiU1 = *(PM64) pbSrcU;
		xiU2 = *(PM64) (pbSrcU + iSrcUvStride);
		xiU1 = _mm_and_si64(xiU1, mMaskAvg);
		xiU2 = _mm_and_si64(xiU2, mMaskAvg);
		xiU1 = _mm_srli_si64(xiU1, 1);
		xiU2 = _mm_srli_si64(xiU2, 1);
		xiU = _mm_adds_pu8(xiU1, xiU2);
		xiV1 = *(PM64) pbSrcV;
		xiV2 = *(PM64) (pbSrcV + iSrcUvStride);
		xiV1 = _mm_and_si64(xiV1, mMaskAvg);
		xiV2 = _mm_and_si64(xiV2, mMaskAvg);
		xiV1 = _mm_srli_si64(xiV1, 1);
		xiV2 = _mm_srli_si64(xiV2, 1);
		xiV = _mm_adds_pu8(xiV1, xiV2);
		xiUv0 = _mm_unpacklo_pi8(xiV, xiU);
		xiUv1 = _mm_unpackhi_pi8(xiV, xiU);

		xiY01 = *(PM64) pbSrcY;
		xiY11 = *(PM64) (pbSrcY + 8);


		xiY02 = *(PM64) (pbSrcY + iSrcYStride + 0);
		xiY12 = *(PM64) (pbSrcY + iSrcYStride + 8);

		xiY03 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);
		xiY13 = *(PM64) (pbSrcY + iSrcYStride * 2 + 8);
		xiY2 = xiY03;
		xiY3 = xiY13;

		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY02 = _mm_and_si64(xiY02, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_srli_si64(xiY02, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY0 = _mm_adds_pu8(xiY01, xiY03);
		xiY0 = _mm_and_si64(xiY0, mMaskAvg);
		xiY0 = _mm_srli_si64(xiY0, 1);
		xiY0 = _mm_adds_pu8(xiY0, xiY02);

		xiY11 = _mm_and_si64(xiY11, mMaskAvg);
		xiY12 = _mm_and_si64(xiY12, mMaskAvg);
		xiY13 = _mm_and_si64(xiY13, mMaskAvg);
		xiY11 = _mm_srli_si64(xiY11, 1);
		xiY12 = _mm_srli_si64(xiY12, 1);
		xiY13 = _mm_srli_si64(xiY13, 1);
		xiY1 = _mm_adds_pu8(xiY11, xiY13);
		xiY1 = _mm_and_si64(xiY1, mMaskAvg);
		xiY1 = _mm_srli_si64(xiY1, 1);
		xiY1 = _mm_adds_pu8(xiY1, xiY12);

		*(PM64) (pbDstX + 0) = _mm_unpacklo_pi8(xiUv0, xiY0);
		*(PM64) (pbDstX + 8) = _mm_unpackhi_pi8(xiUv0, xiY0);
		*(PM64) (pbDstX + 16) = _mm_unpacklo_pi8(xiUv1, xiY1);
		*(PM64) (pbDstX + 24) = _mm_unpackhi_pi8(xiUv1, xiY1);

		*(PM64) (pbDstX + iDstXStride + 0) = _mm_unpacklo_pi8(xiUv0, xiY2);
		*(PM64) (pbDstX + iDstXStride + 8) = _mm_unpackhi_pi8(xiUv0, xiY2);
		*(PM64) (pbDstX + iDstXStride + 16) = _mm_unpacklo_pi8(xiUv1, xiY3);
		*(PM64) (pbDstX + iDstXStride + 24) = _mm_unpackhi_pi8(xiUv1, xiY3);

		pbDstX += 32;
		pbSrcY += 16;
		pbSrcU += 8;
		pbSrcV += 8;
	}
	for (x = (uWidth & 15) / 2; x; x--)
	{
		pbDstX[0] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;
		pbDstX[1] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		pbDstX[2] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;
		pbDstX[3] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		pbDstX[iDstXStride + 0] = pbDstX[1];
		pbDstX[iDstXStride + 1] = pbSrcY[iSrcYStride * 2 + 0];
		pbDstX[iDstXStride + 2] = pbDstX[3];
		pbDstX[iDstXStride + 3] = pbSrcY[iSrcYStride * 2 + 1];

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB32 with de-interlacing.
#define _Yv12ToRgb32(Y1, Y2, Y3, Y4, U, V)							\
{																	\
	bu = g_iBUTab[U];												\
	guv = g_iGUTab[U] + g_iGVTab[V];								\
	rv = g_iRVTab[V];												\
	yy = g_iYYTab[Y1];												\
	pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);					\
	pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);					\
	pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);					\
	pbDstX[3] = 0;													\
	yy = g_iYYTab[Y2];												\
	pbDstX[4] = _Clip((yy + bu) >> SCALEBITS_OUT);					\
	pbDstX[5] = _Clip((yy - guv) >> SCALEBITS_OUT);					\
	pbDstX[6] = _Clip((yy + rv) >> SCALEBITS_OUT);					\
	pbDstX[7] = 0;													\
	yy = g_iYYTab[Y3];												\
	pbDstX[iDstXStride + 0] = _Clip((yy + bu) >> SCALEBITS_OUT);	\
	pbDstX[iDstXStride + 1] = _Clip((yy - guv) >> SCALEBITS_OUT);	\
	pbDstX[iDstXStride + 2] = _Clip((yy + rv) >> SCALEBITS_OUT);	\
	pbDstX[iDstXStride + 3] = 0;									\
	yy = g_iYYTab[Y4];												\
	pbDstX[iDstXStride + 4] = _Clip((yy + bu) >> SCALEBITS_OUT);	\
	pbDstX[iDstXStride + 5] = _Clip((yy - guv) >> SCALEBITS_OUT);	\
	pbDstX[iDstXStride + 6] = _Clip((yy + rv) >> SCALEBITS_OUT);	\
	pbDstX[iDstXStride + 7] = 0;									\
}

#define _Yv12ToRgb32_mmx										\
{																\
	u0 = _mm_unpacklo_pi8(u0, mz);								\
	v0 = _mm_unpacklo_pi8(v0, mz);								\
	u0 = _mm_subs_pi16(u0, g_mSub80);							\
	v0 = _mm_subs_pi16(v0, g_mSub80);							\
	gu0 = _mm_mullo_pi16(u0, g_mUGMul);							\
	gv0 = _mm_mullo_pi16(v0, g_mVGMul);							\
	bu0 = _mm_mullo_pi16(u0, g_mUBMul);							\
	rv0 = _mm_mullo_pi16(v0, g_mVRMul);							\
	guv0 = _mm_adds_pi16(gu0, gv0);								\
	guv1 = _mm_unpackhi_pi16(guv0, guv0);						\
	guv0 = _mm_unpacklo_pi16(guv0, guv0);						\
	bu1 = _mm_unpackhi_pi16(bu0, bu0);							\
	bu0 = _mm_unpacklo_pi16(bu0, bu0);							\
	rv1 = _mm_unpackhi_pi16(rv0, rv0);							\
	rv0 = _mm_unpacklo_pi16(rv0, rv0);							\
	y1 = _mm_unpackhi_pi8(xiY00, mz);							\
	y0 = _mm_unpacklo_pi8(xiY00, mz);							\
	y1 = _mm_subs_pi16(y1, g_mSub10);							\
	y0 = _mm_subs_pi16(y0, g_mSub10);							\
	y1 = _mm_mullo_pi16(y1, g_mYYMul);							\
	y0 = _mm_mullo_pi16(y0, g_mYYMul);							\
	g1 = _mm_subs_pi16(y1, guv1);								\
	g0 = _mm_subs_pi16(y0, guv0);								\
	g1 = _mm_srai_pi16(g1, SCALEBITS);							\
	g0 = _mm_srai_pi16(g0, SCALEBITS);							\
	g0 = _mm_packs_pu16(g0, g1);								\
	b1 = _mm_adds_pi16(y1, bu1);								\
	b0 = _mm_adds_pi16(y0, bu0);								\
	b1 = _mm_srai_pi16(b1, SCALEBITS);							\
	b0 = _mm_srai_pi16(b0, SCALEBITS);							\
	b0 = _mm_packs_pu16(b0, b1);								\
	r1 = _mm_adds_pi16(y1, rv1);								\
	r0 = _mm_adds_pi16(y0, rv0);								\
	r1 = _mm_srai_pi16(r1, SCALEBITS);							\
	r0 = _mm_srai_pi16(r0, SCALEBITS);							\
	r0 = _mm_packs_pu16(r0, r1);								\
	r1 = _mm_unpackhi_pi8(b0, r0);								\
	r0 = _mm_unpacklo_pi8(b0, r0);								\
	g1 = _mm_unpackhi_pi8(g0, mz);								\
	g0 = _mm_unpacklo_pi8(g0, mz);								\
	rgb0 = _mm_unpacklo_pi8(r0, g0);							\
	rgb1 = _mm_unpackhi_pi8(r0, g0);							\
	rgb2 = _mm_unpacklo_pi8(r1, g1);							\
	rgb3 = _mm_unpackhi_pi8(r1, g1);							\
	((PM64) pbDstX)[0] = rgb0;									\
	((PM64) pbDstX)[1] = rgb1;									\
	((PM64) pbDstX)[2] = rgb2;									\
	((PM64) pbDstX)[3] = rgb3;									\
	y1 = _mm_unpackhi_pi8(xiY02, mz);							\
	y0 = _mm_unpacklo_pi8(xiY02, mz);							\
	y1 = _mm_subs_pi16(y1, g_mSub10);							\
	y0 = _mm_subs_pi16(y0, g_mSub10);							\
	y1 = _mm_mullo_pi16(y1, g_mYYMul);							\
	y0 = _mm_mullo_pi16(y0, g_mYYMul);							\
	g1 = _mm_subs_pi16(y1, guv1);								\
	g0 = _mm_subs_pi16(y0, guv0);								\
	g1 = _mm_srai_pi16(g1, SCALEBITS);							\
	g0 = _mm_srai_pi16(g0, SCALEBITS);							\
	g0 = _mm_packs_pu16(g0, g1);								\
	b1 = _mm_adds_pi16(y1, bu1);								\
	b0 = _mm_adds_pi16(y0, bu0);								\
	b1 = _mm_srai_pi16(b1, SCALEBITS);							\
	b0 = _mm_srai_pi16(b0, SCALEBITS);							\
	b0 = _mm_packs_pu16(b0, b1);								\
	r1 = _mm_adds_pi16(y1, rv1);								\
	r0 = _mm_adds_pi16(y0, rv0);								\
	r1 = _mm_srai_pi16(r1, SCALEBITS);							\
	r0 = _mm_srai_pi16(r0, SCALEBITS);							\
	r0 = _mm_packs_pu16(r0, r1);								\
	r1 = _mm_unpackhi_pi8(b0, r0);								\
	r0 = _mm_unpacklo_pi8(b0, r0);								\
	g1 = _mm_unpackhi_pi8(g0, mz);								\
	g0 = _mm_unpacklo_pi8(g0, mz);								\
	rgb0 = _mm_unpacklo_pi8(r0, g0);							\
	rgb1 = _mm_unpackhi_pi8(r0, g0);							\
	rgb2 = _mm_unpacklo_pi8(r1, g1);							\
	rgb3 = _mm_unpackhi_pi8(r1, g1);							\
	((PM64) (pbDstX + iDstXStride))[0] = rgb0;					\
	((PM64) (pbDstX + iDstXStride))[1] = rgb1;					\
	((PM64) (pbDstX + iDstXStride))[2] = rgb2;					\
	((PM64) (pbDstX + iDstXStride))[3] = rgb3;					\
}

VOID Yv12ToRgb32DI_mmx(PBYTE pbDstX, INT iDstXStride,
				PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;

	M64 y0, y1, xiY00, xiY01, xiY02, xiY03;
	M64 u0, v0, u1, v1, u2, v2, mz;
	M64 r0, g0, b0, r1, g1, b1;
	M64 rgb0, rgb1, rgb2, rgb3;
	M64 bu0, gu0, gv0, rv0, bu1, rv1, guv0, guv1;
	M64 mMaskAvg;

	INT yy, bu, guv, rv;
	BYTE yy1, yy2, yy3, yy4, u, v;

	mMaskAvg = g_mMaskAvg;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 4);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	mz = _mm_setzero_si64();
	for (x = uWidth / 8 ; x ; x--)
	{
		u1 = _mm_cvtsi32_si64(*((PDWORD) pbSrcU));
		u2 = _mm_cvtsi32_si64(*((PDWORD) pbSrcU + iSrcUvStride));
		u1 = _mm_and_si64(u1, mMaskAvg);
		u2 = _mm_and_si64(u2, mMaskAvg);
		u1 = _mm_srli_si64(u1, 1);
		u2 = _mm_srli_si64(u2, 1);
		u0 = _mm_adds_pu8(u1, u2);

		v1 = _mm_cvtsi32_si64(*((PDWORD) pbSrcV));
		v2 = _mm_cvtsi32_si64(*((PDWORD) pbSrcV + iSrcUvStride));
		v1 = _mm_and_si64(v1, mMaskAvg);
		v2 = _mm_and_si64(v2, mMaskAvg);
		v1 = _mm_srli_si64(v1, 1);
		v2 = _mm_srli_si64(v2, 1);
		v0 = _mm_adds_pu8(v1, v2);

		xiY01 = *((PM64) pbSrcY);
		xiY00 = xiY01;
		xiY02 = *((PM64) (pbSrcY + iSrcYStride));
		xiY03 = *((PM64) (pbSrcY + iSrcYStride * 2));

		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY02 = _mm_and_si64(xiY02, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_srli_si64(xiY02, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY01 = _mm_adds_pu8(xiY01, xiY03);
		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_adds_pu8(xiY01, xiY02);

		_Yv12ToRgb32_mmx;
		
		pbDstX += 32;
		pbSrcY += 8;
		pbSrcU += 4;
		pbSrcV += 4;
	}
	for (x = (uWidth & 7) / 2; x; x--)
	{
		yy1 = pbSrcY[0];
		yy2 = pbSrcY[1];
		yy3 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		yy4 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb32(yy1, yy2, yy3, yy4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 8;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}
	pbDstX += iDstXDif + iDstXStride;
	pbSrcY += iSrcYDif ;
	pbSrcU += iSrcUvDif - iSrcUvStride;
	pbSrcV += iSrcUvDif - iSrcUvStride;
	for (y = (iHeight / 2) - 2; y; y--)
	{
		for (x = uWidth / 8 ; x ; x--)
		{
			u0 = _mm_cvtsi32_si64(*(PDWORD) pbSrcU);
			u1 = _mm_cvtsi32_si64(*(PDWORD) (pbSrcU + iSrcUvStride));
			u2 = _mm_cvtsi32_si64(*(PDWORD) (pbSrcU + iSrcUvStride * 2));
			u0 = _mm_and_si64(u0, mMaskAvg);
			u1 = _mm_and_si64(u1, mMaskAvg);
			u2 = _mm_and_si64(u2, mMaskAvg);
			u0 = _mm_srli_si64(u0, 1);
			u1 = _mm_srli_si64(u1, 1);
			u2 = _mm_srli_si64(u2, 1);
			u0 = _mm_adds_pu8(u0, u2);
			u0 = _mm_and_si64(u0, mMaskAvg);
			u0 = _mm_srli_si64(u0, 1);
			u0 = _mm_adds_pu8(u0, u1);

			v0 = _mm_cvtsi32_si64(*(PDWORD) pbSrcV);
			v1 = _mm_cvtsi32_si64(*(PDWORD) (pbSrcV + iSrcUvStride));
			v2 = _mm_cvtsi32_si64(*(PDWORD) (pbSrcV + iSrcUvStride * 2));
			v0 = _mm_and_si64(v0, mMaskAvg);
			v1 = _mm_and_si64(v1, mMaskAvg);
			v2 = _mm_and_si64(v2, mMaskAvg);
			v0 = _mm_srli_si64(v0, 1);
			v1 = _mm_srli_si64(v1, 1);
			v2 = _mm_srli_si64(v2, 1);
			v0 = _mm_adds_pu8(v0, v2);
			v0 = _mm_and_si64(v0, mMaskAvg);
			v0 = _mm_srli_si64(v0, 1);
			v0 = _mm_adds_pu8(v0, v1);

			xiY00 = *(PM64) pbSrcY;

			xiY01 = *(PM64) (pbSrcY + iSrcYStride + 0);

			xiY02 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);

			xiY00 = _mm_and_si64(xiY00, mMaskAvg);
			xiY01 = _mm_and_si64(xiY01, mMaskAvg);
			xiY02 = _mm_and_si64(xiY02, mMaskAvg);
			xiY00 = _mm_srli_si64(xiY00, 1);
			xiY01 = _mm_srli_si64(xiY01, 1);
			xiY02 = _mm_srli_si64(xiY02, 1);
			xiY00 = _mm_adds_pu8(xiY00, xiY02);
			xiY00 = _mm_and_si64(xiY00, mMaskAvg);
			xiY00 = _mm_srli_si64(xiY00, 1);
			xiY00 = _mm_adds_pu8(xiY00, xiY01);

			xiY03 = *(PM64) (pbSrcY + iSrcYStride * 3 + 0);
			xiY03 = _mm_and_si64(xiY03, mMaskAvg);
			xiY03 = _mm_srli_si64(xiY03, 1);
			xiY03 = _mm_adds_pu8(xiY01, xiY03);
			xiY03 = _mm_and_si64(xiY03, mMaskAvg);
			xiY03 = _mm_srli_si64(xiY03, 1);
			xiY02 = _mm_adds_pu8(xiY02, xiY03);

			_Yv12ToRgb32_mmx;
				
			pbDstX += 32;
			pbSrcY += 8;
			pbSrcU += 4;
			pbSrcV += 4;
		}
		for (x = (uWidth & 15) / 2; x; x--)
		{
			yy1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			yy2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
			yy3 = ((pbSrcY[iSrcYStride + 0] + pbSrcY[iSrcYStride * 3 + 0] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2;
			yy4 = ((pbSrcY[iSrcYStride + 1] + pbSrcY[iSrcYStride * 3 + 1] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2;
			u = ((pbSrcU[0] + pbSrcU[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			v = ((pbSrcV[0] + pbSrcV[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcV[iSrcUvStride + 0] + 1) / 2;

			_Yv12ToRgb32(yy1, yy2, yy3, yy4, u, v);

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
	for (x = uWidth / 8 ; x ; x--)
	{
		u1 = _mm_cvtsi32_si64(*((PDWORD) pbSrcU));
		u2 = _mm_cvtsi32_si64(*((PDWORD) pbSrcU + iSrcUvStride));
		u1 = _mm_and_si64(u1, mMaskAvg);
		u2 = _mm_and_si64(u2, mMaskAvg);
		u1 = _mm_srli_si64(u1, 1);
		u2 = _mm_srli_si64(u2, 1);
		u0 = _mm_adds_pu8(u1, u2);

		v1 = _mm_cvtsi32_si64(*((PDWORD) pbSrcV));
		v2 = _mm_cvtsi32_si64(*((PDWORD) pbSrcV + iSrcUvStride));
		v1 = _mm_and_si64(v1, mMaskAvg);
		v2 = _mm_and_si64(v2, mMaskAvg);
		v1 = _mm_srli_si64(v1, 1);
		v2 = _mm_srli_si64(v2, 1);
		v0 = _mm_adds_pu8(v1, v2);

		xiY00 = *((PM64) pbSrcY);
		xiY01 = *((PM64) (pbSrcY + iSrcYStride));
		xiY03 = *((PM64) (pbSrcY + iSrcYStride * 2));
		xiY02 = xiY03;

		xiY00 = _mm_and_si64(xiY00, mMaskAvg);
		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY00 = _mm_srli_si64(xiY00, 1);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY00 = _mm_adds_pu8(xiY00, xiY03);
		xiY00 = _mm_and_si64(xiY00, mMaskAvg);
		xiY00 = _mm_srli_si64(xiY00, 1);
		xiY00 = _mm_adds_pu8(xiY00, xiY01);
		
		_Yv12ToRgb32_mmx;
			
		pbDstX += 32;
		pbSrcY += 8;
		pbSrcU += 4;
		pbSrcV += 4;
	}
	for (x = (uWidth & 15) / 2; x; x--)
	{
		yy3 = pbSrcY[iSrcYStride * 2 + 0];
		yy4 = pbSrcY[iSrcYStride * 2 + 1];
		yy1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		yy2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb32(yy1, yy2, yy3, yy4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 8;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}
	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB24 with de-interlacing.
#define _Yv12ToRgb24(Y1, Y2, Y3, Y4, U, V)							\
{																	\
	bu = g_iBUTab[U];												\
	guv = g_iGUTab[U] + g_iGVTab[V];								\
	rv = g_iRVTab[V];												\
	yy = g_iYYTab[Y1];												\
	pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);					\
	pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);					\
	pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);					\
	yy = g_iYYTab[Y2];												\
	pbDstX[3] = _Clip((yy + bu) >> SCALEBITS_OUT);					\
	pbDstX[4] = _Clip((yy - guv) >> SCALEBITS_OUT);					\
	pbDstX[5] = _Clip((yy + rv) >> SCALEBITS_OUT);					\
	yy = g_iYYTab[Y3];												\
	pbDstX[iDstXStride + 0] = _Clip((yy + bu) >> SCALEBITS_OUT);	\
	pbDstX[iDstXStride + 1] = _Clip((yy - guv) >> SCALEBITS_OUT);	\
	pbDstX[iDstXStride + 2] = _Clip((yy + rv) >> SCALEBITS_OUT);	\
	yy = g_iYYTab[Y4];												\
	pbDstX[iDstXStride + 3] = _Clip((yy + bu) >> SCALEBITS_OUT);	\
	pbDstX[iDstXStride + 4] = _Clip((yy - guv) >> SCALEBITS_OUT);	\
	pbDstX[iDstXStride + 5] = _Clip((yy + rv) >> SCALEBITS_OUT);	\
}

#define _Yv12ToRgb24_mmx										\
{																\
	u0 = _mm_unpacklo_pi8(u0, mz);								\
	v0 = _mm_unpacklo_pi8(v0, mz);								\
	u0 = _mm_subs_pi16(u0, g_mSub80);							\
	v0 = _mm_subs_pi16(v0, g_mSub80);							\
	gu0 = _mm_mullo_pi16(u0, g_mUGMul);							\
	gv0 = _mm_mullo_pi16(v0, g_mVGMul);							\
	bu0 = _mm_mullo_pi16(u0, g_mUBMul);							\
	rv0 = _mm_mullo_pi16(v0, g_mVRMul);							\
	guv0 = _mm_adds_pi16(gu0, gv0);								\
	guv1 = _mm_unpackhi_pi16(guv0, guv0);						\
	guv0 = _mm_unpacklo_pi16(guv0, guv0);						\
	bu1 = _mm_unpackhi_pi16(bu0, bu0);							\
	bu0 = _mm_unpacklo_pi16(bu0, bu0);							\
	rv1 = _mm_unpackhi_pi16(rv0, rv0);							\
	rv0 = _mm_unpacklo_pi16(rv0, rv0);							\
	y1 = _mm_unpackhi_pi8(xiY00, mz);							\
	y0 = _mm_unpacklo_pi8(xiY00, mz);							\
	y1 = _mm_subs_pi16(y1, g_mSub10);							\
	y0 = _mm_subs_pi16(y0, g_mSub10);							\
	y1 = _mm_mullo_pi16(y1, g_mYYMul);							\
	y0 = _mm_mullo_pi16(y0, g_mYYMul);							\
	g1 = _mm_subs_pi16(y1, guv1);								\
	g0 = _mm_subs_pi16(y0, guv0);								\
	g1 = _mm_srai_pi16(g1, SCALEBITS);							\
	g0 = _mm_srai_pi16(g0, SCALEBITS);							\
	g0 = _mm_packs_pu16(g0, g1);								\
	b1 = _mm_adds_pi16(y1, bu1);								\
	b0 = _mm_adds_pi16(y0, bu0);								\
	b1 = _mm_srai_pi16(b1, SCALEBITS);							\
	b0 = _mm_srai_pi16(b0, SCALEBITS);							\
	b0 = _mm_packs_pu16(b0, b1);								\
	r1 = _mm_adds_pi16(y1, rv1);								\
	r0 = _mm_adds_pi16(y0, rv0);								\
	r1 = _mm_srai_pi16(r1, SCALEBITS);							\
	r0 = _mm_srai_pi16(r0, SCALEBITS);							\
	r0 = _mm_packs_pu16(r0, r1);								\
	r1 = _mm_unpackhi_pi8(b0, r0);								\
	r0 = _mm_unpacklo_pi8(b0, r0);								\
	g1 = _mm_unpackhi_pi8(g0, mz);								\
	g0 = _mm_unpacklo_pi8(g0, mz);								\
	rgb0 = _mm_unpacklo_pi8(r0, g0);							\
	rgb1 = _mm_unpackhi_pi8(r0, g0);							\
	rgb2 = _mm_unpacklo_pi8(r1, g1);							\
	rgb3 = _mm_unpackhi_pi8(r1, g1);							\
	*((PDWORD) (pbDstX + 0)) = _mm_cvtsi64_si32(rgb0);			\
	rgb0 = _mm_srli_si64(rgb0, 32);								\
	*((PDWORD) (pbDstX + 3)) = _mm_cvtsi64_si32(rgb0);			\
	*((PDWORD) (pbDstX + 6)) = _mm_cvtsi64_si32(rgb1);			\
	rgb1 = _mm_srli_si64(rgb1, 32);								\
	*((PDWORD) (pbDstX + 9)) = _mm_cvtsi64_si32(rgb1);			\
	*((PDWORD) (pbDstX + 12)) = _mm_cvtsi64_si32(rgb2);			\
	 rgb2 = _mm_srli_si64(rgb2, 32);							\
	*((PDWORD) (pbDstX + 15)) = _mm_cvtsi64_si32(rgb2);			\
	*((PDWORD) (pbDstX + 18)) = _mm_cvtsi64_si32(rgb3);			\
	rgb3 = _mm_srli_si64(rgb3, 32);								\
	*((PDWORD) (pbDstX + 21)) = _mm_cvtsi64_si32(rgb3);			\
	y1 = _mm_unpackhi_pi8(xiY02, mz);							\
	y0 = _mm_unpacklo_pi8(xiY02, mz);							\
	y1 = _mm_subs_pi16(y1, g_mSub10);							\
	y0 = _mm_subs_pi16(y0, g_mSub10);							\
	y1 = _mm_mullo_pi16(y1, g_mYYMul);							\
	y0 = _mm_mullo_pi16(y0, g_mYYMul);							\
	g1 = _mm_subs_pi16(y1, guv1);								\
	g0 = _mm_subs_pi16(y0, guv0);								\
	g1 = _mm_srai_pi16(g1, SCALEBITS);							\
	g0 = _mm_srai_pi16(g0, SCALEBITS);							\
	g0 = _mm_packs_pu16(g0, g1);								\
	b1 = _mm_adds_pi16(y1, bu1);								\
	b0 = _mm_adds_pi16(y0, bu0);								\
	b1 = _mm_srai_pi16(b1, SCALEBITS);							\
	b0 = _mm_srai_pi16(b0, SCALEBITS);							\
	b0 = _mm_packs_pu16(b0, b1);								\
	r1 = _mm_adds_pi16(y1, rv1);								\
	r0 = _mm_adds_pi16(y0, rv0);								\
	r1 = _mm_srai_pi16(r1, SCALEBITS);							\
	r0 = _mm_srai_pi16(r0, SCALEBITS);							\
	r0 = _mm_packs_pu16(r0, r1);								\
	r1 = _mm_unpackhi_pi8(b0, r0);								\
	r0 = _mm_unpacklo_pi8(b0, r0);								\
	g1 = _mm_unpackhi_pi8(g0, mz);								\
	g0 = _mm_unpacklo_pi8(g0, mz);								\
	rgb0 = _mm_unpacklo_pi8(r0, g0);							\
	rgb1 = _mm_unpackhi_pi8(r0, g0);							\
	rgb2 = _mm_unpacklo_pi8(r1, g1);							\
	rgb3 = _mm_unpackhi_pi8(r1, g1);							\
	*((PDWORD)(pbDstX + iDstXStride + 0))=_mm_cvtsi64_si32(rgb0);	\
	rgb0 = _mm_srli_si64(rgb0, 32);								\
	*((PDWORD)(pbDstX + iDstXStride + 3))=_mm_cvtsi64_si32(rgb0);	\
	*((PDWORD)(pbDstX + iDstXStride + 6))=_mm_cvtsi64_si32(rgb1);	\
	 rgb1 = _mm_srli_si64(rgb1, 32);							\
	*((PDWORD)(pbDstX + iDstXStride + 9))=_mm_cvtsi64_si32(rgb1);	\
	*((PDWORD)(pbDstX + iDstXStride + 12))=_mm_cvtsi64_si32(rgb2);\
	 rgb2 = _mm_srli_si64(rgb2, 32);							\
	*((PDWORD)(pbDstX + iDstXStride + 15))=_mm_cvtsi64_si32(rgb2);\
	*((PDWORD)(pbDstX + iDstXStride + 18))=_mm_cvtsi64_si32(rgb3);\
	rgb3 = _mm_srli_si64(rgb3, 32);								\
	*((PDWORD)(pbDstX + iDstXStride + 21))=_mm_cvtsi64_si32(rgb3);\
}

VOID Yv12ToRgb24DI_mmx(PBYTE pbDstX, INT iDstXStride,
				PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;

	M64 y0, y1, xiY00, xiY01, xiY02, xiY03;
	M64 u0, v0, u1, v1, u2, v2, mz;
	M64 r0, g0, b0, r1, g1, b1;
	M64 rgb0, rgb1, rgb2, rgb3;
	M64 bu0, gu0, gv0, rv0, bu1, rv1, guv0, guv1;
	M64 mMaskAvg;

	INT yy, bu, guv, rv;
	BYTE yy1, yy2, yy3, yy4, u, v;
	
	mMaskAvg = g_mMaskAvg;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 3);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	mz = _mm_setzero_si64();
	for (x = uWidth / 8 ; x ; x--)
	{
		u1 = _mm_cvtsi32_si64(*((PDWORD) pbSrcU));
		u2 = _mm_cvtsi32_si64(*((PDWORD) pbSrcU + iSrcUvStride));
		u1 = _mm_and_si64(u1, mMaskAvg);
		u2 = _mm_and_si64(u2, mMaskAvg);
		u1 = _mm_srli_si64(u1, 1);
		u2 = _mm_srli_si64(u2, 1);
		u0 = _mm_adds_pu8(u1, u2);

		v1 = _mm_cvtsi32_si64(*((PDWORD) pbSrcV));
		v2 = _mm_cvtsi32_si64(*((PDWORD) pbSrcV + iSrcUvStride));
		v1 = _mm_and_si64(v1, mMaskAvg);
		v2 = _mm_and_si64(v2, mMaskAvg);
		v1 = _mm_srli_si64(v1, 1);
		v2 = _mm_srli_si64(v2, 1);
		v0 = _mm_adds_pu8(v1, v2);

		xiY01 = *((PM64) pbSrcY);
		xiY00 = xiY01;
		xiY02 = *((PM64) (pbSrcY + iSrcYStride));
		xiY03 = *((PM64) (pbSrcY + iSrcYStride * 2));

		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY02 = _mm_and_si64(xiY02, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_srli_si64(xiY02, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY01 = _mm_adds_pu8(xiY01, xiY03);
		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY02 = _mm_adds_pu8(xiY01, xiY02);

		_Yv12ToRgb24_mmx;
		
		pbDstX += 24;
		pbSrcY += 8;
		pbSrcU += 4;
		pbSrcV += 4;
	}
	for (x = (uWidth & 7) / 2; x; x--)
	{
		yy1 = pbSrcY[0];
		yy2 = pbSrcY[1];
		yy3 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		yy4 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb24(yy1, yy2, yy3, yy4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 6;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}
	pbDstX += iDstXDif + iDstXStride;
	pbSrcY += iSrcYDif ;
	pbSrcU += iSrcUvDif - iSrcUvStride;
	pbSrcV += iSrcUvDif - iSrcUvStride;
	for (y = (iHeight / 2) - 2; y; y--)
	{
		for (x = uWidth / 8 ; x ; x--)
		{
			u0 = _mm_cvtsi32_si64(*(PDWORD) pbSrcU);
			u1 = _mm_cvtsi32_si64(*(PDWORD) (pbSrcU + iSrcUvStride));
			u2 = _mm_cvtsi32_si64(*(PDWORD) (pbSrcU + iSrcUvStride * 2));
			u0 = _mm_and_si64(u0, mMaskAvg);
			u1 = _mm_and_si64(u1, mMaskAvg);
			u2 = _mm_and_si64(u2, mMaskAvg);
			u0 = _mm_srli_si64(u0, 1);
			u1 = _mm_srli_si64(u1, 1);
			u2 = _mm_srli_si64(u2, 1);
			u0 = _mm_adds_pu8(u0, u2);
			u0 = _mm_and_si64(u0, mMaskAvg);
			u0 = _mm_srli_si64(u0, 1);
			u0 = _mm_adds_pu8(u0, u1);

			v0 = _mm_cvtsi32_si64(*(PDWORD) pbSrcV);
			v1 = _mm_cvtsi32_si64(*(PDWORD) (pbSrcV + iSrcUvStride));
			v2 = _mm_cvtsi32_si64(*(PDWORD) (pbSrcV + iSrcUvStride * 2));
			v0 = _mm_and_si64(v0, mMaskAvg);
			v1 = _mm_and_si64(v1, mMaskAvg);
			v2 = _mm_and_si64(v2, mMaskAvg);
			v0 = _mm_srli_si64(v0, 1);
			v1 = _mm_srli_si64(v1, 1);
			v2 = _mm_srli_si64(v2, 1);
			v0 = _mm_adds_pu8(v0, v2);
			v0 = _mm_and_si64(v0, mMaskAvg);
			v0 = _mm_srli_si64(v0, 1);
			v0 = _mm_adds_pu8(v0, v1);

			xiY00 = *(PM64) pbSrcY;

			xiY01 = *(PM64) (pbSrcY + iSrcYStride + 0);

			xiY02 = *(PM64) (pbSrcY + iSrcYStride * 2 + 0);

			xiY00 = _mm_and_si64(xiY00, mMaskAvg);
			xiY01 = _mm_and_si64(xiY01, mMaskAvg);
			xiY02 = _mm_and_si64(xiY02, mMaskAvg);
			xiY00 = _mm_srli_si64(xiY00, 1);
			xiY01 = _mm_srli_si64(xiY01, 1);
			xiY02 = _mm_srli_si64(xiY02, 1);
			xiY00 = _mm_adds_pu8(xiY00, xiY02);
			xiY00 = _mm_and_si64(xiY00, mMaskAvg);
			xiY00 = _mm_srli_si64(xiY00, 1);
			xiY00 = _mm_adds_pu8(xiY00, xiY01);

			xiY03 = *(PM64) (pbSrcY + iSrcYStride * 3 + 0);
			xiY03 = _mm_and_si64(xiY03, mMaskAvg);
			xiY03 = _mm_srli_si64(xiY03, 1);
			xiY03 = _mm_adds_pu8(xiY01, xiY03);
			xiY03 = _mm_and_si64(xiY03, mMaskAvg);
			xiY03 = _mm_srli_si64(xiY03, 1);
			xiY02 = _mm_adds_pu8(xiY02, xiY03);

			_Yv12ToRgb24_mmx;
				
			pbDstX += 24;
			pbSrcY += 8;
			pbSrcU += 4;
			pbSrcV += 4;
		}
		for (x = (uWidth & 15) / 2; x; x--)
		{
			yy1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			yy2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
			yy3 = ((pbSrcY[iSrcYStride + 0] + pbSrcY[iSrcYStride * 3 + 0] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2;
			yy4 = ((pbSrcY[iSrcYStride + 1] + pbSrcY[iSrcYStride * 3 + 1] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2;
			u = ((pbSrcU[0] + pbSrcU[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			v = ((pbSrcV[0] + pbSrcV[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcV[iSrcUvStride + 0] + 1) / 2;

			_Yv12ToRgb24(yy1, yy2, yy3, yy4, u, v);

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
	for (x = uWidth / 8 ; x ; x--)
	{
		u1 = _mm_cvtsi32_si64(*((PDWORD) pbSrcU));
		u2 = _mm_cvtsi32_si64(*((PDWORD) pbSrcU + iSrcUvStride));
		u1 = _mm_and_si64(u1, mMaskAvg);
		u2 = _mm_and_si64(u2, mMaskAvg);
		u1 = _mm_srli_si64(u1, 1);
		u2 = _mm_srli_si64(u2, 1);
		u0 = _mm_adds_pu8(u1, u2);

		v1 = _mm_cvtsi32_si64(*((PDWORD) pbSrcV));
		v2 = _mm_cvtsi32_si64(*((PDWORD) pbSrcV + iSrcUvStride));
		v1 = _mm_and_si64(v1, mMaskAvg);
		v2 = _mm_and_si64(v2, mMaskAvg);
		v1 = _mm_srli_si64(v1, 1);
		v2 = _mm_srli_si64(v2, 1);
		v0 = _mm_adds_pu8(v1, v2);

		xiY00 = *((PM64) pbSrcY);
		xiY01 = *((PM64) (pbSrcY + iSrcYStride));
		xiY03 = *((PM64) (pbSrcY + iSrcYStride * 2));
		xiY02 = xiY03;

		xiY00 = _mm_and_si64(xiY00, mMaskAvg);
		xiY01 = _mm_and_si64(xiY01, mMaskAvg);
		xiY03 = _mm_and_si64(xiY03, mMaskAvg);
		xiY00 = _mm_srli_si64(xiY00, 1);
		xiY01 = _mm_srli_si64(xiY01, 1);
		xiY03 = _mm_srli_si64(xiY03, 1);
		xiY00 = _mm_adds_pu8(xiY00, xiY03);
		xiY00 = _mm_and_si64(xiY00, mMaskAvg);
		xiY00 = _mm_srli_si64(xiY00, 1);
		xiY00 = _mm_adds_pu8(xiY00, xiY01);

		_Yv12ToRgb24_mmx;
		pbDstX += 24;
		pbSrcY += 8;
		pbSrcU += 4;
		pbSrcV += 4;
	}
	for (x = (uWidth & 15) / 2; x; x--)
	{
		yy3 = pbSrcY[iSrcYStride * 2 + 0];
		yy4 = pbSrcY[iSrcYStride * 2 + 1];
		yy1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		yy2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb24(yy1, yy2, yy3, yy4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 6;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}
	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB16 with de-interlacing.
VOID Yv12ToRgb16DI_mmx(PBYTE pbDstX, INT iDstXStride,
				PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride, UINT uWidth, INT iHeight)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB15 with de-interlacing.
VOID Yv12ToRgb15DI_mmx(PBYTE pbDstX, INT iDstXStride,
				   PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride, UINT uWidth, INT iHeight)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
