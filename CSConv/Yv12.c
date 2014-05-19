


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YV12.
VOID Yv12ToYv12(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
				PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				UINT uWidth, INT iHeight)
{
	LineCopy(pbDstY, iDstYStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopy(pbDstU, iDstUvStride, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopy(pbDstV, iDstUvStride, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
}

VOID Yv12ToYV12(PBYTE pbDstX, INT iDstXStride,
				PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				UINT uWidth, INT iHeight)
{
	UINT uPixel = iDstXStride * abs(iHeight);
	LineCopy(pbDstX, iDstXStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopy(pbDstX + uPixel, iDstXStride / 2, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopy(pbDstX + uPixel + (uPixel / 4), iDstXStride / 2, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
}

VOID Yv12ToI420(PBYTE pbDstX, INT iDstXStride,
				PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				UINT uWidth, INT iHeight)
{
	UINT uPixel = iDstXStride * abs(iHeight);
	LineCopy(pbDstX, iDstXStride, pbSrcY, iSrcYStride, uWidth, iHeight);
	LineCopy(pbDstX + uPixel, iDstXStride / 2, pbSrcU, iSrcUvStride, uWidth / 2, iHeight / 2);
	LineCopy(pbDstX + uPixel + (uPixel / 4), iDstXStride / 2, pbSrcV, iSrcUvStride, uWidth / 2, iHeight / 2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YUY2.
VOID Yv12ToYuy2(PBYTE pbDstX, INT iDstXStride,
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

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			pbDstX[0] = pbSrcY[0];
			pbDstX[1] = pbSrcU[0];
			pbDstX[2] = pbSrcY[1];
			pbDstX[3] = pbSrcV[0];

			pbDstX[iDstXStride + 0] = pbSrcY[iSrcYStride + 0];
			pbDstX[iDstXStride + 1] = pbSrcU[0];
			pbDstX[iDstXStride + 2] = pbSrcY[iSrcYStride + 1];
			pbDstX[iDstXStride + 3] = pbSrcV[0];

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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to YVYU.
VOID Yv12ToYvyu(PBYTE pbDstX, INT iDstXStride,
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

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			pbDstX[0] = pbSrcY[0];
			pbDstX[1] = pbSrcV[0];
			pbDstX[2] = pbSrcY[1];
			pbDstX[3] = pbSrcU[0];

			pbDstX[iDstXStride + 0] = pbSrcY[iSrcYStride + 0];
			pbDstX[iDstXStride + 1] = pbSrcV[0];
			pbDstX[iDstXStride + 2] = pbSrcY[iSrcYStride + 1];
			pbDstX[iDstXStride + 3] = pbSrcU[0];

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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to UYVY.
VOID Yv12ToUyvy(PBYTE pbDstX, INT iDstXStride,
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

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			pbDstX[0] = pbSrcU[0];
			pbDstX[1] = pbSrcY[0];
			pbDstX[2] = pbSrcV[0];
			pbDstX[3] = pbSrcY[1];

			pbDstX[iDstXStride + 0] = pbSrcU[0];
			pbDstX[iDstXStride + 1] = pbSrcY[iSrcYStride + 0];
			pbDstX[iDstXStride + 2] = pbSrcV[0];
			pbDstX[iDstXStride + 3] = pbSrcY[iSrcYStride + 1];

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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to VYUY.
VOID Yv12ToVyuy(PBYTE pbDstX, INT iDstXStride,
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

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			pbDstX[0] = pbSrcV[0];
			pbDstX[1] = pbSrcY[0];
			pbDstX[2] = pbSrcU[0];
			pbDstX[3] = pbSrcY[1];

			pbDstX[iDstXStride + 0] = pbSrcV[0];
			pbDstX[iDstXStride + 1] = pbSrcY[iSrcYStride + 0];
			pbDstX[iDstXStride + 2] = pbSrcU[0];
			pbDstX[iDstXStride + 3] = pbSrcY[iSrcYStride + 1];

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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB32.
VOID Yv12ToRgb32(PBYTE pbDstX, INT iDstXStride,
				 PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				 UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 4);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcU[0]];
			guv = g_iGUTab[pbSrcU[0]] + g_iGVTab[pbSrcV[0]];
			rv = g_iRVTab[pbSrcV[0]];

			yy = g_iYYTab[pbSrcY[0]];
			pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);
			pbDstX[3] = 0;

			yy = g_iYYTab[pbSrcY[1]];
			pbDstX[4] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[5] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[6] = _Clip((yy + rv) >> SCALEBITS_OUT);
			pbDstX[7] = 0;

			yy = g_iYYTab[pbSrcY[iSrcYStride]];
			pbDstX[iDstXStride + 0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 2] = _Clip((yy + rv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 3] = 0;

			yy = g_iYYTab[pbSrcY[iSrcYStride + 1]];
			pbDstX[iDstXStride + 4] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 5] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 6] = _Clip((yy + rv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 7] = 0;

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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB24.
VOID Yv12ToRgb24(PBYTE pbDstX, INT iDstXStride,
				 PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				 UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 3);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
#ifdef _NOTABLE
			bu = 0x81 * (pbSrcU[0] - 128);
			rv = 0x66 * (pbSrcV[0] - 128);
			guv = 0x19 * (pbSrcU[0] - 128) + 0x34 * (pbSrcV[0] - 128);

			yy = 0x4A * (pbSrcY[0] - 16);
			pbDstX[0] = _Clip((yy + bu) >> 8);
			pbDstX[1] = _Clip((yy + guv) >> 8);
			pbDstX[2] = _Clip((yy + rv) >> 8);

			yy = 0x4A * (pbSrcY[1] - 16);
			pbDstX[3] = _Clip((yy + bu) >> 8);
			pbDstX[4] = _Clip((yy + guv) >> 8);
			pbDstX[5] = _Clip((yy + rv) >> 8);

			yy = 0x4A * (pbSrcY[iSrcYStride + 0] - 16);
			pbDstX[iDstXStride + 0] = _Clip((yy + bu) >> 8);
			pbDstX[iDstXStride + 1] = _Clip((yy + guv) >> 8);
			pbDstX[iDstXStride + 2] = _Clip((yy + rv) >> 8);

			yy = 0x4A * (pbSrcY[iSrcYStride + 1] - 16);
			pbDstX[iDstXStride + 3] = _Clip((yy + bu) >> 8);
			pbDstX[iDstXStride + 4] = _Clip((yy + guv) >> 8);
			pbDstX[iDstXStride + 5] = _Clip((yy + rv) >> 8);
#else
			bu = g_iBUTab[pbSrcU[0]];
			guv = g_iGUTab[pbSrcU[0]] + g_iGVTab[pbSrcV[0]];
			rv = g_iRVTab[pbSrcV[0]];

			yy = g_iYYTab[pbSrcY[0]];
			pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);

			yy = g_iYYTab[pbSrcY[1]];
			pbDstX[3] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[4] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[5] = _Clip((yy + rv) >> SCALEBITS_OUT);

			yy = g_iYYTab[pbSrcY[iSrcYStride]];
			pbDstX[iDstXStride + 0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 1] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 2] = _Clip((yy + rv) >> SCALEBITS_OUT);

			yy = g_iYYTab[pbSrcY[iSrcYStride + 1]];
			pbDstX[iDstXStride + 3] = _Clip((yy + bu) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 4] = _Clip((yy - guv) >> SCALEBITS_OUT);
			pbDstX[iDstXStride + 5] = _Clip((yy + rv) >> SCALEBITS_OUT);
#endif

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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB16.
VOID Yv12ToRgb16(PBYTE pbDstX, INT iDstXStride,
				 PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				 UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv, r, g, b;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcU[0]];
			guv = g_iGUTab[pbSrcU[0]] + g_iGVTab[pbSrcV[0]];
			rv = g_iRVTab[pbSrcV[0]];

			yy = g_iYYTab[pbSrcY[0]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) pbDstX)[0] = _MakeRgb16(r, g, b);

			yy = g_iYYTab[pbSrcY[1]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) pbDstX)[1] = _MakeRgb16(r, g, b);

			yy = g_iYYTab[pbSrcY[iSrcYStride]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) (pbDstX + iDstXStride))[0] = _MakeRgb16(r, g, b);

			yy = g_iYYTab[pbSrcY[iSrcYStride + 1]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) (pbDstX + iDstXStride))[1] = _MakeRgb16(r, g, b);

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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YV12 to RGB15.
VOID Yv12ToRgb15(PBYTE pbDstX, INT iDstXStride,
				 PBYTE pbSrcY, PBYTE pbSrcU, PBYTE pbSrcV, INT iSrcYStride, INT iSrcUvStride,
				 UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcYDif;
	INT iSrcUvDif;
	INT yy, bu, guv, rv, r, g, b;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbDstX += (iHeight - 1) * iDstXStride;
		iDstXStride = -iDstXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcYDif = iSrcYStride - uWidth;
	iSrcUvDif = iSrcUvStride - (uWidth / 2);

	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			bu = g_iBUTab[pbSrcU[0]];
			guv = g_iGUTab[pbSrcU[0]] + g_iGVTab[pbSrcV[0]];
			rv = g_iRVTab[pbSrcV[0]];

			yy = g_iYYTab[pbSrcY[0]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) pbDstX)[0] = _MakeRgb15(r, g, b);

			yy = g_iYYTab[pbSrcY[1]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) pbDstX)[1] = _MakeRgb15(r, g, b);

			yy = g_iYYTab[pbSrcY[iSrcYStride]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) (pbDstX + iDstXStride))[0] = _MakeRgb15(r, g, b);

			yy = g_iYYTab[pbSrcY[iSrcYStride + 1]];
			b = _Clip(((yy + bu) >> SCALEBITS_OUT));
			g = _Clip(((yy - guv) >> SCALEBITS_OUT));
			r = _Clip(((yy + rv) >> SCALEBITS_OUT));
			((PWORD) (pbDstX + iDstXStride))[1] = _MakeRgb15(r, g, b);

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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
