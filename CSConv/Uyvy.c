


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to YV12.
VOID UyvyToYv12(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
				PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstYDif;
	INT iDstUvDif;
	INT iSrcXDif;

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
			pbDstY[0] = pbSrcX[1];
			pbDstU[0] = (pbSrcX[0] + pbSrcX[iSrcXStride + 0] + 1) / 2;
			pbDstY[1] = pbSrcX[3];
			pbDstV[0] = (pbSrcX[2] + pbSrcX[iSrcXStride + 2] + 1) / 2;

			pbDstY[iDstYStride + 0] = pbSrcX[iSrcXStride + 1];
			pbDstY[iDstYStride + 1] = pbSrcX[iSrcXStride + 3];

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
// Convert UYVY to YUY2.
VOID UyvyToYuy2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	Yuy2ToUyvy(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to YVYU.
VOID UyvyToYvyu(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
	iSrcXDif = iSrcXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			pbDstX[0] = pbSrcX[1];
			pbDstX[1] = pbSrcX[2];
			pbDstX[2] = pbSrcX[3];
			pbDstX[3] = pbSrcX[0];

			pbDstX += 4;
			pbSrcX += 4;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to UYVY.
VOID UyvyToUyvy(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopy(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to VYUY.
VOID UyvyToVyuy(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
	iSrcXDif = iSrcXStride - (uWidth * 2);

	for (y = iHeight; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			pbDstX[0] = pbSrcX[2];
			pbDstX[1] = pbSrcX[1];
			pbDstX[2] = pbSrcX[0];
			pbDstX[3] = pbSrcX[3];

			pbDstX += 4;
			pbSrcX += 4;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to RGB32.
VOID UyvyToRgb32(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;

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
		for (x = uWidth / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcX[0]];
			guv = g_iGUTab[pbSrcX[0]] + g_iGVTab[pbSrcX[2]];
			rv = g_iRVTab[pbSrcX[2]];

			yy = g_iYYTab[pbSrcX[1]];
			pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);
			pbDstX[3] = 0;

			yy = g_iYYTab[pbSrcX[3]];
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
// Convert UYVY to RGB24.
VOID UyvyToRgb24(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;

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
		for (x = uWidth / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcX[0]];
			guv = g_iGUTab[pbSrcX[0]] + g_iGVTab[pbSrcX[2]];
			rv = g_iRVTab[pbSrcX[2]];

			yy = g_iYYTab[pbSrcX[1]];
			pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);

			yy = g_iYYTab[pbSrcX[3]];
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
// Convert UYVY to RGB16.
VOID UyvyToRgb16(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;
	INT r[2], g[2], b[2];

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
		r[0] = r[1] = g[0] = g[1] = b[0] = b[1] = 0;
		for (x = uWidth / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcX[0]];
			guv = g_iGUTab[pbSrcX[0]] + g_iGVTab[pbSrcX[2]];
			rv = g_iRVTab[pbSrcX[2]];

			yy = g_iYYTab[pbSrcX[1]];
			b[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			g[0] = _Clip((yy - guv) >> SCALEBITS_OUT);
			r[0] = _Clip((yy + rv) >> SCALEBITS_OUT);
			((PWORD) pbDstX)[0] = _MakeRgb16(r[0], g[0], b[0]);

			yy = g_iYYTab[pbSrcX[3]];
			b[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			g[0] = _Clip((yy - guv) >> SCALEBITS_OUT);
			r[0] = _Clip((yy + rv) >> SCALEBITS_OUT);
			((PWORD) pbDstX)[1] = _MakeRgb16(r[0], g[0], b[0]);

			pbDstX += 4;
			pbSrcX += 4;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to RGB15.
VOID UyvyToRgb15(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;
	INT r[2], g[2], b[2];

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
		r[0] = r[1] = g[0] = g[1] = b[0] = b[1] = 0;
		for (x = uWidth / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcX[0]];
			guv = g_iGUTab[pbSrcX[0]] + g_iGVTab[pbSrcX[2]];
			rv = g_iRVTab[pbSrcX[2]];

			yy = g_iYYTab[pbSrcX[1]];
			b[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			g[0] = _Clip((yy - guv) >> SCALEBITS_OUT);
			r[0] = _Clip((yy + rv) >> SCALEBITS_OUT);
			((PWORD) pbDstX)[0] = _MakeRgb15(r[0], g[0], b[0]);

			yy = g_iYYTab[pbSrcX[3]];
			b[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			g[0] = _Clip((yy - guv) >> SCALEBITS_OUT);
			r[0] = _Clip((yy + rv) >> SCALEBITS_OUT);
			((PWORD) pbDstX)[1] = _MakeRgb15(r[0], g[0], b[0]);

			pbDstX += 4;
			pbSrcX += 4;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
