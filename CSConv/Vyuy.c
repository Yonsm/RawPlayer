


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert VYUY to YV12.
VOID VyuyToYv12(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
				PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UyvyToYv12(pbDstY, pbDstV, pbDstU, iDstYStride, iDstUvStride, pbSrcX, iSrcXStride, uWidth, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert VYUY to YUY2.
VOID VyuyToYuy2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UyvyToYvyu(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert VYUY to YVYU.
VOID VyuyToYvyu(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	Yuy2ToUyvy(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert VYUY to UYVY.
VOID VyuyToUyvy(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UyvyToVyuy(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert VYUY to VYUY.
VOID VyuyToVyuy(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopy(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert VYUY to RGB32.
VOID VyuyToRgb32(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
			bu = g_iBUTab[pbSrcX[2]];
			guv = g_iGUTab[pbSrcX[2]] + g_iGVTab[pbSrcX[0]];
			rv = g_iRVTab[pbSrcX[0]];

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
// Convert VYUY to RGB24.
VOID VyuyToRgb24(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
			bu = g_iBUTab[pbSrcX[2]];
			guv = g_iGUTab[pbSrcX[2]] + g_iGVTab[pbSrcX[0]];
			rv = g_iRVTab[pbSrcX[0]];

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
// Convert VYUY to RGB16.
VOID VyuyToRgb16(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
			bu = g_iBUTab[pbSrcX[2]];
			guv = g_iGUTab[pbSrcX[2]] + g_iGVTab[pbSrcX[0]];
			rv = g_iRVTab[pbSrcX[0]];

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
// Convert VYUY to RGB15.
VOID VyuyToRgb15(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
			bu = g_iBUTab[pbSrcX[2]];
			guv = g_iGUTab[pbSrcX[2]] + g_iGVTab[pbSrcX[0]];
			rv = g_iRVTab[pbSrcX[0]];

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
