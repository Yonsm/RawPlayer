


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YVYU to YV12.
VOID YvyuToYv12(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
				PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	Yuy2ToYv12(pbDstY, pbDstV, pbDstU, iDstYStride, iDstUvStride, pbSrcX, iSrcXStride, uWidth, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YVYU to YUY2.
VOID YvyuToYuy2(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	Yuy2ToYvyu(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YVYU to YVYU.
VOID YvyuToYvyu(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopy(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YVYU to UYVY.
VOID YvyuToUyvy(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
			pbDstX[0] = pbSrcX[3];
			pbDstX[1] = pbSrcX[0];
			pbDstX[2] = pbSrcX[1];
			pbDstX[3] = pbSrcX[2];

			pbDstX += 4;
			pbSrcX += 4;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YVYU to VYUY.
VOID YvyuToVyuy(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	Yuy2ToUyvy(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YVYU to RGB32.
VOID YvyuToRgb32(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
			bu = g_iBUTab[pbSrcX[3]];
			guv = g_iGUTab[pbSrcX[3]] + g_iGVTab[pbSrcX[1]];
			rv = g_iRVTab[pbSrcX[1]];

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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YVYU to RGB24.
VOID YvyuToRgb24(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
			bu = g_iBUTab[pbSrcX[3]];
			guv = g_iGUTab[pbSrcX[3]] + g_iGVTab[pbSrcX[1]];
			rv = g_iRVTab[pbSrcX[1]];

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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YVYU to RGB16.
VOID YvyuToRgb16(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
			bu = g_iBUTab[pbSrcX[3]];
			guv = g_iGUTab[pbSrcX[3]] + g_iGVTab[pbSrcX[1]];
			rv = g_iRVTab[pbSrcX[1]];

			yy = g_iYYTab[pbSrcX[0]];
			b[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			g[0] = _Clip((yy - guv) >> SCALEBITS_OUT);
			r[0] = _Clip((yy + rv) >> SCALEBITS_OUT);
			((PWORD) pbDstX)[0] = _MakeRgb16(r[0], g[0], b[0]);

			yy = g_iYYTab[pbSrcX[2]];
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
// Convert YVYU to RGB15.
VOID YvyuToRgb15(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
			bu = g_iBUTab[pbSrcX[3]];
			guv = g_iGUTab[pbSrcX[3]] + g_iGVTab[pbSrcX[1]];
			rv = g_iRVTab[pbSrcX[1]];

			yy = g_iYYTab[pbSrcX[0]];
			b[0] = _Clip((yy + bu) >> SCALEBITS_OUT);
			g[0] = _Clip((yy - guv) >> SCALEBITS_OUT);
			r[0] = _Clip((yy + rv) >> SCALEBITS_OUT);
			((PWORD) pbDstX)[0] = _MakeRgb15(r[0], g[0], b[0]);

			yy = g_iYYTab[pbSrcX[2]];
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
