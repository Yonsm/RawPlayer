


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YV12 with de-interlacing.
VOID Yv12ToYv12DI(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
				  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				  UINT uWidth, INT iHeight)
{
	LineCopyDI(pbDstY, iDstYStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopyDI(pbDstU, iDstUvStride, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopyDI(pbDstV, iDstUvStride, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
}

VOID Yv12ToYV12DI(PBYTE pbDstX, INT iDstXStride,
				  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				  UINT uWidth, INT iHeight)
{
	UINT uPixel = iDstXStride * abs(iHeight);
	LineCopyDI(pbDstX, iDstXStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopyDI(pbDstX + uPixel, iDstXStride / 2, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopyDI(pbDstX + uPixel + (uPixel / 4), iDstXStride / 2, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
}

VOID Yv12ToI420DI(PBYTE pbDstX, INT iDstXStride,
				  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				  UINT uWidth, INT iHeight)
{
	UINT uPixel = iDstXStride * abs(iHeight);
	LineCopyDI(pbDstX, iDstXStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopyDI(pbDstX + uPixel, iDstXStride / 2, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopyDI(pbDstX + uPixel + (uPixel / 4), iDstXStride / 2, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YUY2 with de-interlacing.
VOID Yv12ToYuy2DI(PBYTE pbDstX, INT iDstXStride,
				  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				  UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (x = uWidth / 2; x; x--)
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
		for (x = uWidth / 2; x; x--)
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

	for (x = uWidth / 2; x; x--)
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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YVYU with de-interlacing.
VOID Yv12ToYvyuDI(PBYTE pbDstX, INT iDstXStride,
				  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				  UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (x = uWidth / 2; x; x--)
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
		for (x = uWidth / 2; x; x--)
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

	for (x = uWidth / 2; x; x--)
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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to UYVY with de-interlacing.
VOID Yv12ToUyvyDI(PBYTE pbDstX, INT iDstXStride,
				  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				  UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (x = uWidth / 2; x; x--)
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
		for (x = uWidth / 2; x; x--)
		{
			pbDstX[0] = ((pbSrcU[0] + pbSrcU[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			pbDstX[1] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			pbDstX[2] = ((pbSrcV[0] + pbSrcV[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcV[iSrcUvStride + 0] + 1) / 2;
			pbDstX[3] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

			pbDstX[iDstXStride + 0] = pbDstX[0];
			pbDstX[iDstXStride + 1] = ((pbSrcY[iSrcYStride + 0] + pbSrcY[iSrcYStride * 3 + 0] + 1) / 2 +
				pbSrcY[iSrcYStride * 2 + 0] + 1) / 2;
			pbDstX[iDstXStride + 2] = pbDstX[2];
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

	for (x = uWidth / 2; x; x--)
	{
		pbDstX[0] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;
		pbDstX[1] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		pbDstX[2] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;
		pbDstX[3] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		pbDstX[iDstXStride + 0] = pbDstX[0];
		pbDstX[iDstXStride + 1] = pbSrcY[iSrcYStride * 2 + 0];
		pbDstX[iDstXStride + 2] = pbDstX[2];
		pbDstX[iDstXStride + 3] = pbSrcY[iSrcYStride * 2 + 1];

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to VYUY with de-interlacing.
VOID Yv12ToVyuyDI(PBYTE pbDstX, INT iDstXStride,
				  PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				  UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (x = uWidth / 2; x; x--)
	{
		pbDstX[0] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;
		pbDstX[1] = pbSrcY[0];
		pbDstX[2] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;
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
		for (x = uWidth / 2; x; x--)
		{
			pbDstX[0] = ((pbSrcV[0] + pbSrcV[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcV[iSrcUvStride + 0] + 1) / 2;
			pbDstX[1] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			pbDstX[2] = ((pbSrcU[0] + pbSrcU[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			pbDstX[3] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

			pbDstX[iDstXStride + 0] = pbDstX[0];
			pbDstX[iDstXStride + 1] = ((pbSrcY[iSrcYStride + 0] + pbSrcY[iSrcYStride * 3 + 0] + 1) / 2 +
				pbSrcY[iSrcYStride * 2 + 0] + 1) / 2;
			pbDstX[iDstXStride + 2] = pbDstX[2];
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

	for (x = uWidth / 2; x; x--)
	{
		pbDstX[0] = (pbSrcV[0] + pbSrcV[iSrcUvStride + 0] + 1) / 2;
		pbDstX[1] = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		pbDstX[2] = (pbSrcU[0] + pbSrcU[iSrcUvStride + 0] + 1) / 2;
		pbDstX[3] = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		pbDstX[iDstXStride + 0] = pbDstX[0];
		pbDstX[iDstXStride + 1] = pbSrcY[iSrcYStride * 2 + 0];
		pbDstX[iDstXStride + 2] = pbDstX[2];
		pbDstX[iDstXStride + 3] = pbSrcY[iSrcYStride * 2 + 1];

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}
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

VOID Yv12ToRgb32DI(PBYTE pbDstX, INT iDstXStride,
				   PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				   UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv;
	BYTE y1, y2, y3, y4, u, v;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 4);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (x = uWidth / 2; x; x--)
	{
		y1 = pbSrcY[0];
		y2 = pbSrcY[1];
		y3 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		y4 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb32(y1, y2, y3, y4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 8;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	pbDstX += iDstXDif + iDstXStride;
	pbSrcY += iSrcYDif;
	pbSrcU += iSrcUvDif - iSrcUvStride;
	pbSrcV += iSrcUvDif - iSrcUvStride;

	for (y = (iHeight / 2) - 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			y1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			y2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
			y3 = ((pbSrcY[iSrcYStride + 0] + pbSrcY[iSrcYStride * 3 + 0] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2;
			y4 = ((pbSrcY[iSrcYStride + 1] + pbSrcY[iSrcYStride * 3 + 1] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2;
			u = ((pbSrcU[0] + pbSrcU[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			v = ((pbSrcV[0] + pbSrcV[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcV[iSrcUvStride + 0] + 1) / 2;

			_Yv12ToRgb32(y1, y2, y3, y4, u, v);

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

	for (x = uWidth / 2; x; x--)
	{
		y3 = pbSrcY[iSrcYStride * 2 + 0];
		y4 = pbSrcY[iSrcYStride * 2 + 1];
		y1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		y2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb32(y1, y2, y3, y4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 8;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}
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

VOID Yv12ToRgb24DI(PBYTE pbDstX, INT iDstXStride,
				   PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				   UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv;
	BYTE y1, y2, y3, y4, u, v;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 3);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (x = uWidth / 2; x; x--)
	{
		y1 = pbSrcY[0];
		y2 = pbSrcY[1];
		y3 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		y4 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb24(y1, y2, y3, y4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 6;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	pbDstX += iDstXDif + iDstXStride;
	pbSrcY += iSrcYDif;
	pbSrcU += iSrcUvDif - iSrcUvStride;
	pbSrcV += iSrcUvDif - iSrcUvStride;

	for (y = (iHeight / 2) - 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			y1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			y2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
			y3 = ((pbSrcY[iSrcYStride + 0] + pbSrcY[iSrcYStride * 3 + 0] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2;
			y4 = ((pbSrcY[iSrcYStride + 1] + pbSrcY[iSrcYStride * 3 + 1] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2;

			u = ((pbSrcU[0] + pbSrcU[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			v = ((pbSrcV[0] + pbSrcV[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcV[iSrcUvStride + 0] + 1) / 2;

			_Yv12ToRgb24(y1, y2, y3, y4, u, v);

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

	for (x = uWidth / 2; x; x--)
	{
		y3 = pbSrcY[iSrcYStride * 2 + 0];
		y4 = pbSrcY[iSrcYStride * 2 + 1];
		y1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		y2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb24(y1, y2, y3, y4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 6;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB16 with de-interlacing.
#define _Yv12ToRgb16(Y1, Y2, Y3, Y4, U, V)									\
{																			\
	bu = g_iBUTab[U];														\
	guv = g_iGUTab[U] + g_iGVTab[V];										\
	rv = g_iRVTab[V];														\
	yy = g_iYYTab[Y1];														\
	b = _Clip((yy + bu) >> SCALEBITS_OUT);									\
	g = _Clip((yy - guv) >> SCALEBITS_OUT);									\
	r = _Clip((yy + rv) >> SCALEBITS_OUT);									\
	((PWORD) pbDstX)[0] = _MakeRgb16(r, g, b);								\
	yy = g_iYYTab[Y2];														\
	b = _Clip((yy + bu) >> SCALEBITS_OUT);									\
	g = _Clip((yy - guv) >> SCALEBITS_OUT);									\
	r = _Clip((yy + rv) >> SCALEBITS_OUT);									\
	((PWORD) pbDstX)[1] = _MakeRgb16(r, g, b);								\
	yy = g_iYYTab[Y3];														\
	b = _Clip((yy + bu) >> SCALEBITS_OUT);									\
	g = _Clip((yy - guv) >> SCALEBITS_OUT);									\
	r = _Clip((yy + rv) >> SCALEBITS_OUT);									\
	((PWORD) (pbDstX + iDstXStride))[0] = _MakeRgb16(r, g, b);				\
	yy = g_iYYTab[Y4];														\
	b = _Clip((yy + bu) >> SCALEBITS_OUT);									\
	g = _Clip((yy - guv) >> SCALEBITS_OUT);									\
	r = _Clip((yy + rv) >> SCALEBITS_OUT);									\
	((PWORD) (pbDstX + iDstXStride))[1] = _MakeRgb16(r, g, b);				\
}

VOID Yv12ToRgb16DI(PBYTE pbDstX, INT iDstXStride,
				   PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				   UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv;
	BYTE y1, y2, y3, y4, u, v, r, g, b;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (x = uWidth / 2; x; x--)
	{
		y1 = pbSrcY[0];
		y2 = pbSrcY[1];
		y3 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		y4 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb16(y1, y2, y3, y4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	pbDstX += iDstXDif + iDstXStride;
	pbSrcY += iSrcYDif;
	pbSrcU += iSrcUvDif - iSrcUvStride;
	pbSrcV += iSrcUvDif - iSrcUvStride;

	for (y = (iHeight / 2) - 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			y1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			y2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
			y3 = ((pbSrcY[iSrcYStride + 0] + pbSrcY[iSrcYStride * 3 + 0] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2;
			y4 = ((pbSrcY[iSrcYStride + 1] + pbSrcY[iSrcYStride * 3 + 1] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2;

			u = ((pbSrcU[0] + pbSrcU[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			v = ((pbSrcV[0] + pbSrcV[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcV[iSrcUvStride + 0] + 1) / 2;

			_Yv12ToRgb16(y1, y2, y3, y4, u, v);

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

	for (x = uWidth / 2; x; x--)
	{
		y3 = pbSrcY[iSrcYStride * 2 + 0];
		y4 = pbSrcY[iSrcYStride * 2 + 1];
		y1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		y2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb16(y1, y2, y3, y4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB15 with de-interlacing.
#define _Yv12ToRgb15(Y1, Y2, Y3, Y4, U, V)									\
{																			\
	bu = g_iBUTab[U];														\
	guv = g_iGUTab[U] + g_iGVTab[V];										\
	rv = g_iRVTab[V];														\
	yy = g_iYYTab[Y1];														\
	b = _Clip((yy + bu) >> SCALEBITS_OUT);									\
	g = _Clip((yy - guv) >> SCALEBITS_OUT);									\
	r = _Clip((yy + rv) >> SCALEBITS_OUT);									\
	((PWORD) pbDstX)[0] = _MakeRgb15(r, g, b);								\
	yy = g_iYYTab[Y2];														\
	b = _Clip((yy + bu) >> SCALEBITS_OUT);									\
	g = _Clip((yy - guv) >> SCALEBITS_OUT);									\
	r = _Clip((yy + rv) >> SCALEBITS_OUT);									\
	((PWORD) pbDstX)[1] = _MakeRgb15(r, g, b);								\
	yy = g_iYYTab[Y3];														\
	b = _Clip((yy + bu) >> SCALEBITS_OUT);									\
	g = _Clip((yy - guv) >> SCALEBITS_OUT);									\
	r = _Clip((yy + rv) >> SCALEBITS_OUT);									\
	((PWORD) (pbDstX + iDstXStride))[0] = _MakeRgb15(r, g, b);				\
	yy = g_iYYTab[Y4];														\
	b = _Clip((yy + bu) >> SCALEBITS_OUT);									\
	g = _Clip((yy - guv) >> SCALEBITS_OUT);									\
	r = _Clip((yy + rv) >> SCALEBITS_OUT);									\
	((PWORD) (pbDstX + iDstXStride))[1] = _MakeRgb15(r, g, b);				\
}

VOID Yv12ToRgb15DI(PBYTE pbDstX, INT iDstXStride,
				   PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				   UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv;
	BYTE y1, y2, y3, y4, u, v, r, g, b;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (x = uWidth / 2; x; x--)
	{
		y1 = pbSrcY[0];
		y2 = pbSrcY[1];
		y3 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		y4 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb15(y1, y2, y3, y4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}

	pbDstX += iDstXDif + iDstXStride;
	pbSrcY += iSrcYDif;
	pbSrcU += iSrcUvDif - iSrcUvStride;
	pbSrcV += iSrcUvDif - iSrcUvStride;

	for (y = (iHeight / 2) - 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			y1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
			y2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;
			y3 = ((pbSrcY[iSrcYStride + 0] + pbSrcY[iSrcYStride * 3 + 0] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2;
			y4 = ((pbSrcY[iSrcYStride + 1] + pbSrcY[iSrcYStride * 3 + 1] + 1) / 2 + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2;

			u = ((pbSrcU[0] + pbSrcU[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcU[iSrcUvStride + 0] + 1) / 2;
			v = ((pbSrcV[0] + pbSrcV[iSrcUvStride * 2 + 0] + 1) / 2 + pbSrcV[iSrcUvStride + 0] + 1) / 2;

			_Yv12ToRgb15(y1, y2, y3, y4, u, v);

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

	for (x = uWidth / 2; x; x--)
	{
		y3 = pbSrcY[iSrcYStride * 2 + 0];
		y4 = pbSrcY[iSrcYStride * 2 + 1];
		y1 = ((pbSrcY[0] + pbSrcY[iSrcYStride * 2 + 0] + 1) / 2 + pbSrcY[iSrcYStride + 0] + 1) / 2;
		y2 = ((pbSrcY[1] + pbSrcY[iSrcYStride * 2 + 1] + 1) / 2 + pbSrcY[iSrcYStride + 1] + 1) / 2;

		_Yv12ToRgb15(y1, y2, y3, y4, pbSrcU[0], pbSrcV[0]);

		pbDstX += 4;
		pbSrcY += 2;
		pbSrcU++;
		pbSrcV++;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
