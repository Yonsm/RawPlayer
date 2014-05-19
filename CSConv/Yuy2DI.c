


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"
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

VOID Yuy2ToYv12DI(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
				  PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstYDif;
	INT iDstUvDif;
	INT iSrcXDif;
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

	for (x = uWidth / 2; x; x--)
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

	for (y = (iHeight / 2) - 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
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

			Yuy2ToYv12(Src00, Src01, Src02, Src03, Src10, Src11, Src12, Src13);

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

	for (x = uWidth / 2; x; x--)
	{
		Src00 = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
		Src01 = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
		Src02 = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
		Src03 = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;

		Yuy2ToYv12(Src00, Src01, Src02, Src03,
			pbSrcX[iSrcXStride * 2 + 0], pbSrcX[iSrcXStride * 2 + 1], 
			pbSrcX[iSrcXStride * 2 + 2], pbSrcX[iSrcXStride * 2 + 3]);

		pbSrcX += 4;
		pbDstY += 2;
		pbDstU++;
		pbDstV++;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YUY2 with de-interlacing.
VOID Yuy2ToYuy2DI(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopyDI(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to YVYU with de-interlacing.
VOID Yuy2ToYvyuDI(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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

	for (x = uWidth / 2; x; x--)
	{
		pbDstX[0] = pbSrcX[0];
		pbDstX[1] = pbSrcX[3];
		pbDstX[2] = pbSrcX[2];
		pbDstX[3] = pbSrcX[1];

		pbDstX += 4;
		pbSrcX += 4;
	}

	pbDstX += iDstXDif;
	pbSrcX += iSrcXDif - iSrcXStride;
	for (y = iHeight - 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			pbDstX[0] = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
			pbDstX[1] = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;
			pbDstX[2] = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
			pbDstX[3] = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
			pbDstX += 4;
			pbSrcX += 4;
		}
		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}

	pbSrcX += iSrcXStride;
	for (x = uWidth / 2; x; x--)
	{
		pbDstX[0] = pbSrcX[0];
		pbDstX[1] = pbSrcX[3];
		pbDstX[2] = pbSrcX[2];
		pbDstX[3] = pbSrcX[1];

		pbDstX += 4;
		pbSrcX += 4;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to UYVY with de-interlacing.
VOID Yuy2ToUyvyDI(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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

	for (x = uWidth / 2; x; x--)
	{
		pbDstX[0] = pbSrcX[1];
		pbDstX[1] = pbSrcX[0];
		pbDstX[2] = pbSrcX[3];
		pbDstX[3] = pbSrcX[2];

		pbDstX += 4;
		pbSrcX += 4;
	}

	pbDstX += iDstXDif;
	pbSrcX += iSrcXDif - iSrcXStride;
	for (y = iHeight - 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			pbDstX[0] = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
			pbDstX[1] = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
			pbDstX[2] = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;
			pbDstX[3] = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
			pbDstX += 4;
			pbSrcX += 4;
		}
		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}

	pbSrcX += iSrcXStride;
	for (x = uWidth / 2; x; x--)
	{
		pbDstX[0] = pbSrcX[1];
		pbDstX[1] = pbSrcX[0];
		pbDstX[2] = pbSrcX[3];
		pbDstX[3] = pbSrcX[2];

		pbDstX += 4;
		pbSrcX += 4;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to VYUY with de-interlacing.
VOID Yuy2ToVyuyDI(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
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
	pbSrcX += iSrcXDif - iSrcXStride;
	for (y = iHeight - 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			pbDstX[0] = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;
			pbDstX[1] = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
			pbDstX[2] = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
			pbDstX[3] = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
			pbDstX += 4;
			pbSrcX += 4;
		}
		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}

	pbSrcX += iSrcXStride;
	for (x = uWidth / 2; x; x--)
	{
		pbDstX[0] = pbSrcX[3];
		pbDstX[1] = pbSrcX[0];
		pbDstX[2] = pbSrcX[1];
		pbDstX[3] = pbSrcX[2];

		pbDstX += 4;
		pbSrcX += 4;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB32 with de-interlacing.
#define Yuy2ToRGB32(S0, S1, S2, S3)					\
{													\
	bu = g_iBUTab[S1];								\
	guv = g_iGUTab[S1] + g_iGVTab[S3];				\
	rv = g_iRVTab[S3];								\
	yy = g_iYYTab[S0];								\
	pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);	\
	pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);	\
	pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);	\
	pbDstX[3] = 0;									\
	yy = g_iYYTab[S2];								\
	pbDstX[4] = _Clip((yy + bu) >> SCALEBITS_OUT);	\
	pbDstX[5] = _Clip((yy - guv) >> SCALEBITS_OUT);	\
	pbDstX[6] = _Clip((yy + rv) >> SCALEBITS_OUT);	\
	pbDstX[7] = 0;									\
}
VOID Yuy2ToRgb32DI(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;
	BYTE Src0, Src1, Src2, Src3;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 4);
	iSrcXDif = iSrcXStride - (uWidth * 2);

	for (x = uWidth / 2; x; x--)
	{
		Yuy2ToRGB32(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3]);
		pbDstX += 8;
		pbSrcX += 4;
	}
	pbDstX += iDstXDif;

	for (y = iHeight - 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			Src0 = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
			Src1 = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
			Src2 = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
			Src3 = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;

			Yuy2ToRGB32(Src0, Src1, Src2, Src3);

			pbDstX += 8;
			pbSrcX += 4;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}

	pbSrcX += iSrcXDif;
	for (x = uWidth / 2; x; x--)
	{
		Yuy2ToRGB32(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3]);
		pbDstX += 8;
		pbSrcX += 4;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB24 with de-interlacing.
#define Yuy2ToRGB24(S0, S1, S2, S3)						\
{														\
	bu = g_iBUTab[S1];									\
	guv = g_iGUTab[S1] + g_iGVTab[S3];					\
	rv = g_iRVTab[S3];									\
	yy = g_iYYTab[S0];									\
	pbDstX[0] = _Clip((yy + bu) >> SCALEBITS_OUT);		\
	pbDstX[1] = _Clip((yy - guv) >> SCALEBITS_OUT);		\
	pbDstX[2] = _Clip((yy + rv) >> SCALEBITS_OUT);		\
	yy = g_iYYTab[S2];									\
	pbDstX[3] = _Clip((yy + bu) >> SCALEBITS_OUT);		\
	pbDstX[4] = _Clip((yy - guv) >> SCALEBITS_OUT);		\
	pbDstX[5] = _Clip((yy + rv) >> SCALEBITS_OUT);		\
}

VOID Yuy2ToRgb24DI(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;
	BYTE Src0, Src1, Src2, Src3;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 3);
	iSrcXDif = iSrcXStride - (uWidth * 2);

	for (x = uWidth / 2; x; x--)
	{
		Yuy2ToRGB24(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3]);
		pbDstX += 6;
		pbSrcX += 4;
	}
	pbDstX += iDstXDif;

	for (y = iHeight - 2; y; y--)
	{
		for (x = uWidth / 2; x; x--)
		{
			Src0 = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
			Src1 = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
			Src2 = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
			Src3 = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;

			Yuy2ToRGB24(Src0, Src1, Src2, Src3);

			pbDstX += 6;
			pbSrcX += 4;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}

	pbSrcX += iSrcXDif;
	for (x = uWidth / 2; x; x--)
	{
		Yuy2ToRGB24(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3]);
		pbDstX += 6;
		pbSrcX += 4;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB16 with de-interlacing.
#define Yuy2ToRGB16(S0, S1, S2, S3)							\
{															\
	bu = g_iBUTab[S1];										\
	guv = g_iGUTab[S1] + g_iGVTab[S3];						\
	rv = g_iRVTab[S3];										\
	yy = g_iYYTab[S0];										\
	b[0] = _Clip((yy + bu) >> SCALEBITS_OUT);				\
	g[0] = _Clip((yy - guv) >> SCALEBITS_OUT);				\
	r[0] = _Clip((yy + rv) >> SCALEBITS_OUT);				\
	((PWORD) pbDstX)[0] = _MakeRgb16(r[0], g[0], b[0]);		\
	yy = g_iYYTab[S2];										\
	b[0] = _Clip((yy + bu) >> SCALEBITS_OUT);				\
	g[0] = _Clip((yy - guv) >> SCALEBITS_OUT);				\
	r[0] = _Clip((yy + rv) >> SCALEBITS_OUT);				\
	((PWORD) pbDstX)[1] = _MakeRgb16(r[0], g[0], b[0]);		\
}

VOID Yuy2ToRgb16DI(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;
	INT r[2], g[2], b[2];
	BYTE Src0, Src1, Src2, Src3;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcXDif = iSrcXStride - (uWidth * 2);

	r[0] = r[1] = g[0] = g[1] = b[0] = b[1] = 0;
	for (x = uWidth / 2; x; x--)
	{
		Yuy2ToRGB16(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3]);
		pbDstX += 4;
		pbSrcX += 4;
	}
	pbDstX += iDstXDif;

	for (y = iHeight - 2; y; y--)
	{
		r[0] = r[1] = g[0] = g[1] = b[0] = b[1] = 0;
		for (x = uWidth / 2; x; x--)
		{
			Src0 = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
			Src1 = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
			Src2 = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
			Src3 = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;

			Yuy2ToRGB16(Src0, Src1, Src2, Src3);

			pbDstX += 4;
			pbSrcX += 4;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}

	pbSrcX += iSrcXDif;
	r[0] = r[1] = g[0] = g[1] = b[0] = b[1] = 0;
	for (x = uWidth / 2; x; x--)
	{
		Yuy2ToRGB16(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3]);
		pbDstX += 4;
		pbSrcX += 4;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert YUY2 to RGB15 with de-interlacing.
#define Yuy2ToRGB15(S0, S1, S2, S3)							\
{															\
	bu = g_iBUTab[S1];										\
	guv = g_iGUTab[S1] + g_iGVTab[S3];						\
	rv = g_iRVTab[S3];										\
	yy = g_iYYTab[S0];										\
	b[0] = _Clip((yy + bu) >> SCALEBITS_OUT);				\
	g[0] = _Clip((yy - guv) >> SCALEBITS_OUT);				\
	r[0] = _Clip((yy + rv) >> SCALEBITS_OUT);				\
	((PWORD) pbDstX)[0] = _MakeRgb15(r[0], g[0], b[0]);		\
	yy = g_iYYTab[S2];										\
	b[0] = _Clip((yy + bu) >> SCALEBITS_OUT);				\
	g[0] = _Clip((yy - guv) >> SCALEBITS_OUT);				\
	r[0] = _Clip((yy + rv) >> SCALEBITS_OUT);				\
	((PWORD) pbDstX)[1] = _MakeRgb15(r[0], g[0], b[0]);		\
}

VOID Yuy2ToRgb15DI(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iDstXDif;
	INT iSrcXDif;
	INT yy, bu, guv, rv;
	INT r[2], g[2], b[2];
	BYTE Src0, Src1, Src2, Src3;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iDstXDif = iDstXStride - (uWidth * 2);
	iSrcXDif = iSrcXStride - (uWidth * 2);

	r[0] = r[1] = g[0] = g[1] = b[0] = b[1] = 0;
	for (x = uWidth / 2; x; x--)
	{
		Yuy2ToRGB15(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3]);
		pbDstX += 4;
		pbSrcX += 4;
	}
	pbDstX += iDstXDif;

	for (y = iHeight - 2; y; y--)
	{
		r[0] = r[1] = g[0] = g[1] = b[0] = b[1] = 0;
		for (x = uWidth / 2; x; x--)
		{
			Src0 = ((pbSrcX[0] + pbSrcX[iSrcXStride * 2 + 0] + 1) / 2 + pbSrcX[iSrcXStride + 0] + 1) / 2;
			Src1 = ((pbSrcX[1] + pbSrcX[iSrcXStride * 2 + 1] + 1) / 2 + pbSrcX[iSrcXStride + 1] + 1) / 2;
			Src2 = ((pbSrcX[2] + pbSrcX[iSrcXStride * 2 + 2] + 1) / 2 + pbSrcX[iSrcXStride + 2] + 1) / 2;
			Src3 = ((pbSrcX[3] + pbSrcX[iSrcXStride * 2 + 3] + 1) / 2 + pbSrcX[iSrcXStride + 3] + 1) / 2;

			Yuy2ToRGB15(Src0, Src1, Src2, Src3);

			pbDstX += 4;
			pbSrcX += 4;
		}

		pbDstX += iDstXDif;
		pbSrcX += iSrcXDif;
	}

	pbSrcX += iSrcXDif;
	r[0] = r[1] = g[0] = g[1] = b[0] = b[1] = 0;
	for (x = uWidth / 2; x; x--)
	{
		Yuy2ToRGB15(pbSrcX[0], pbSrcX[1], pbSrcX[2], pbSrcX[3]);
		pbDstX += 4;
		pbSrcX += 4;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
