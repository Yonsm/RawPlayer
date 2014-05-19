


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to YV12.
VOID Rgb24ToYv12(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
				 PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstYDif;
	INT iDstUvDif;
	UINT r, g, b, sr, sg, sb;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 3);
	iDstYDif = iDstYStride - uWidth;
	iDstUvDif = iDstUvStride - (uWidth / 2);

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			sb = b = pbSrcX[0];
			sg = g = pbSrcX[1];
			sr = r = pbSrcX[2];
			pbDstY[0] = _MakeY(r, g, b);

			sb += b = pbSrcX[3];
			sg += g = pbSrcX[4];
			sr += r = pbSrcX[5];
			pbDstY[1] = _MakeY(r, g, b);

			sb += b = pbSrcX[iSrcXStride + 0];
			sg += g = pbSrcX[iSrcXStride + 1];
			sr += r = pbSrcX[iSrcXStride + 2];
			pbDstY[iDstYStride + 0] = _MakeY(r, g, b);

			sb += b = pbSrcX[iSrcXStride + 3];
			sg += g = pbSrcX[iSrcXStride + 4];
			sr += r = pbSrcX[iSrcXStride + 5];
			pbDstY[iDstYStride + 1] = _MakeY(r, g, b);

			pbDstU[0] = _MakeU(sr, sg, sb, 2);
			pbDstV[0] = _MakeV(sr, sg, sb, 2);

			pbSrcX += 6;
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
// Convert RGB24 to YUY2.
VOID Rgb24ToYuy2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstXDif;
	UINT r, g, b, sr, sg, sb;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 3);
	iDstXDif = iDstXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			sb = b = pbSrcX[0];
			sg = g = pbSrcX[1];
			sr = r = pbSrcX[2];
			pbDstX[0] = _MakeY(r, g, b);

			sb += b = pbSrcX[3];
			sg += g = pbSrcX[4];
			sr += r = pbSrcX[5];
			pbDstX[2] = _MakeY(r, g, b);

			pbDstX[1] = _MakeU(sr, sg, sb, 1);
			pbDstX[3] = _MakeV(sr, sg, sb, 1);

			pbSrcX += 6;
			pbDstX += 4;
		}

		pbSrcX += iSrcXDif;
		pbDstX += iDstXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to YVYU.
VOID Rgb24ToYvyu(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstXDif;
	UINT r, g, b, sr, sg, sb;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 3);
	iDstXDif = iDstXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			sb = b = pbSrcX[0];
			sg = g = pbSrcX[1];
			sr = r = pbSrcX[2];
			pbDstX[0] = _MakeY(r, g, b);

			sb += b = pbSrcX[3];
			sg += g = pbSrcX[4];
			sr += r = pbSrcX[5];
			pbDstX[2] = _MakeY(r, g, b);

			pbDstX[3] = _MakeU(sr, sg, sb, 1);
			pbDstX[1] = _MakeV(sr, sg, sb, 1);

			pbSrcX += 6;
			pbDstX += 4;
		}

		pbSrcX += iSrcXDif;
		pbDstX += iDstXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to UYVY.
VOID Rgb24ToUyvy(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstXDif;
	UINT r, g, b, sr, sg, sb;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 3);
	iDstXDif = iDstXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			sb = b = pbSrcX[0];
			sg = g = pbSrcX[1];
			sr = r = pbSrcX[2];
			pbDstX[1] = _MakeY(r, g, b);

			sb += b = pbSrcX[3];
			sg += g = pbSrcX[4];
			sr += r = pbSrcX[5];
			pbDstX[3] = _MakeY(r, g, b);

			pbDstX[0] = _MakeU(sr, sg, sb, 1);
			pbDstX[2] = _MakeV(sr, sg, sb, 1);

			pbSrcX += 6;
			pbDstX += 4;
		}

		pbSrcX += iSrcXDif;
		pbDstX += iDstXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to VYUY.
VOID Rgb24ToVyuy(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstXDif;
	UINT r, g, b, sr, sg, sb;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 3);
	iDstXDif = iDstXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			sb = b = pbSrcX[0];
			sg = g = pbSrcX[1];
			sr = r = pbSrcX[2];
			pbDstX[1] = _MakeY(r, g, b);

			sb += b = pbSrcX[3];
			sg += g = pbSrcX[4];
			sr += r = pbSrcX[5];
			pbDstX[3] = _MakeY(r, g, b);

			pbDstX[2] = _MakeU(sr, sg, sb, 1);
			pbDstX[0] = _MakeV(sr, sg, sb, 1);

			pbSrcX += 6;
			pbDstX += 4;
		}

		pbSrcX += iSrcXDif;
		pbDstX += iDstXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to RGB32.
VOID Rgb24ToRgb32(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 4);
	iSrcXDif = iSrcXStride - (uWidth * 3);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth; x; x--)
		{
			/*pbDstX[0] = pbSrcX[0];
			pbDstX[1] = pbSrcX[1];
			pbDstX[2] = pbSrcX[2];
			pbDstX[3] = 0;*/

			//*((PDWORD) pbDstX) = *((PDWORD) pbSrcX) & 0x00FFFFFF;

			((PWORD) pbDstX)[0] = ((PWORD) pbSrcX)[0];
			((PWORD) pbDstX)[1] = (WORD) pbSrcX[2];

			pbDstX += 4;
			pbSrcX += 3;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to RGB24.
VOID Rgb24ToRgb24(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopy(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 3, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to RGB16.
VOID Rgb24ToRgb16(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcXDif = iSrcXStride - (uWidth * 3);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth; x; x--)
		{
			*((PWORD) pbDstX) = _MakeRgb16(pbSrcX[2], pbSrcX[1], pbSrcX[0]);

			pbDstX += 2;
			pbSrcX += 3;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB24 to RGB15.
VOID Rgb24ToRgb15(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcXDif = iSrcXStride - (uWidth * 3);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth; x; x--)
		{
			*((PWORD) pbDstX) = _MakeRgb15(pbSrcX[2], pbSrcX[1], pbSrcX[0]);

			pbDstX += 2;
			pbSrcX += 3;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
