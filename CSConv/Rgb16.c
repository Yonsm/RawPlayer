


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB16 to YV12.
VOID Rgb16ToYv12(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
				 PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	WORD w;
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

	iSrcXDif = iSrcXStride - (uWidth * 2);
	iDstYDif = iDstYStride - uWidth;
	iDstUvDif = iDstUvStride - (uWidth / 2);

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			w = ((PWORD) pbSrcX)[0];
			sb = b = _GetRgb16B(w);
			sg = g = _GetRgb16G(w);
			sr = r = _GetRgb16R(w);
			pbDstY[0] = _MakeY(r, g, b);

			w = ((PWORD) pbSrcX)[1];
			sb += b = _GetRgb16B(w);
			sg += g = _GetRgb16G(w);
			sr += r = _GetRgb16R(w);
			pbDstY[1] = _MakeY(r, g, b);

			w = ((PWORD) (pbSrcX + iSrcXStride))[0];
			sb += b = _GetRgb16B(w);
			sg += g = _GetRgb16G(w);
			sr += r = _GetRgb16R(w);
			pbDstY[iDstYStride + 0] = _MakeY(r, g, b);

			w = ((PWORD) (pbSrcX + iSrcXStride))[1];
			sb += b = _GetRgb16B(w);
			sg += g = _GetRgb16G(w);
			sr += r = _GetRgb16R(w);
			pbDstY[iDstYStride + 1] = _MakeY(r, g, b);

			pbDstU[0] = _MakeU(sr, sg, sb, 2);
			pbDstV[0] = _MakeV(sr, sg, sb, 2);

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
// Convert RGB16 to YUY2.
VOID Rgb16ToYuy2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	WORD w;
	INT iSrcXDif;
	INT iDstXDif;
	UINT r, g, b, sr, sg, sb;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 2);
	iDstXDif = iDstXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			w = ((PWORD) pbSrcX)[0];
			sb = b = _GetRgb16B(w);
			sg = g = _GetRgb16G(w);
			sr = r = _GetRgb16R(w);
			pbDstX[0] = _MakeY(r, g, b);

			w = ((PWORD) pbSrcX)[1];
			sb += b = _GetRgb16B(w);
			sg += g = _GetRgb16G(w);
			sr += r = _GetRgb16R(w);
			pbDstX[2] = _MakeY(r, g, b);

			pbDstX[1] = _MakeU(sr, sg, sb, 1);
			pbDstX[3] = _MakeV(sr, sg, sb, 1);

			pbSrcX += 4;
			pbDstX += 4;
		}

		pbSrcX += iSrcXDif;
		pbDstX += iDstXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB16 to YVYU.
VOID Rgb16ToYvyu(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	WORD w;
	INT iSrcXDif;
	INT iDstXDif;
	UINT r, g, b, sr, sg, sb;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 2);
	iDstXDif = iDstXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			w = ((PWORD) pbSrcX)[0];
			sb = b = _GetRgb16B(w);
			sg = g = _GetRgb16G(w);
			sr = r = _GetRgb16R(w);
			pbDstX[0] = _MakeY(r, g, b);

			w = ((PWORD) pbSrcX)[1];
			sb += b = _GetRgb16B(w);
			sg += g = _GetRgb16G(w);
			sr += r = _GetRgb16R(w);
			pbDstX[2] = _MakeY(r, g, b);

			pbDstX[3] = _MakeU(sr, sg, sb, 1);
			pbDstX[1] = _MakeV(sr, sg, sb, 1);

			pbSrcX += 4;
			pbDstX += 4;
		}

		pbSrcX += iSrcXDif;
		pbDstX += iDstXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB16 to UYVY.
VOID Rgb16ToUyvy(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	WORD w;
	INT iSrcXDif;
	INT iDstXDif;
	UINT r, g, b, sr, sg, sb;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 2);
	iDstXDif = iDstXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			w = ((PWORD) pbSrcX)[0];
			sb = b = _GetRgb16B(w);
			sg = g = _GetRgb16G(w);
			sr = r = _GetRgb16R(w);
			pbDstX[1] = _MakeY(r, g, b);

			w = ((PWORD) pbSrcX)[1];
			sb += b = _GetRgb16B(w);
			sg += g = _GetRgb16G(w);
			sr += r = _GetRgb16R(w);
			pbDstX[3] = _MakeY(r, g, b);

			pbDstX[0] = _MakeU(sr, sg, sb, 1);
			pbDstX[2] = _MakeV(sr, sg, sb, 1);

			pbSrcX += 4;
			pbDstX += 4;
		}

		pbSrcX += iSrcXDif;
		pbDstX += iDstXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB16 to VYUY.
VOID Rgb16ToVyuy(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	WORD w;
	INT iSrcXDif;
	INT iDstXDif;
	UINT r, g, b, sr, sg, sb;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 2);
	iDstXDif = iDstXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			w = ((PWORD) pbSrcX)[0];
			sb = b = _GetRgb16B(w);
			sg = g = _GetRgb16G(w);
			sr = r = _GetRgb16R(w);
			pbDstX[1] = _MakeY(r, g, b);

			w = ((PWORD) pbSrcX)[1];
			sb += b = _GetRgb16B(w);
			sg += g = _GetRgb16G(w);
			sr += r = _GetRgb16R(w);
			pbDstX[3] = _MakeY(r, g, b);

			pbDstX[2] = _MakeU(sr, sg, sb, 1);
			pbDstX[0] = _MakeV(sr, sg, sb, 1);

			pbSrcX += 4;
			pbDstX += 4;
		}

		pbSrcX += iSrcXDif;
		pbDstX += iDstXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB16 to RGB32.
VOID Rgb16ToRgb32(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	WORD w;
	INT iDstXDif;
	INT iSrcXDif;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 4);
	iSrcXDif = iSrcXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth; x; x--)
		{
			w = *((PWORD) pbSrcX);

			pbDstX[0] = _GetRgb16B(w);
			pbDstX[1] = _GetRgb16G(w);
			pbDstX[2] = _GetRgb16R(w);
			pbDstX[3] = 0;

			pbDstX += 4;
			pbSrcX += 2;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB16 to RGB24.
VOID Rgb16ToRgb24(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	WORD w;
	INT iDstXDif;
	INT iSrcXDif;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 3);
	iSrcXDif = iSrcXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth; x; x--)
		{
			w = *((PWORD) pbSrcX);

			pbDstX[0] = _GetRgb16B(w);
			pbDstX[1] = _GetRgb16G(w);
			pbDstX[2] = _GetRgb16R(w);

			pbDstX += 3;
			pbSrcX += 2;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB16 to RGB16.
VOID Rgb16ToRgb16(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopy(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert RGB16 to RGB15.
VOID Rgb16ToRgb15(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	WORD w;
	INT iDstXDif;
	INT iSrcXDif;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcXDif = iSrcXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth; x; x--)
		{
			w = *((PWORD) pbSrcX);
			*((PWORD) pbDstX) = (w & 0x001F) | ((w >> 1) & 0xFFE0);

			pbDstX += 2;
			pbSrcX += 2;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
