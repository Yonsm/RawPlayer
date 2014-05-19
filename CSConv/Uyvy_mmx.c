


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"

#ifdef _MMX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to YV12.
VOID UyvyToYv12_mmx(PBYTE pbDstY, PBYTE pbDstU, PBYTE pbDstV, INT iDstYStride, INT iDstUvStride,
					PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	UINT x;
	INT y;
	INT iSrcXDif;
	INT iDstYDif;
	INT iDstUvDif;
	M64 m0, m1, m2, m3, m4, m5, m6, m7;

	if (iHeight < 0)
	{
		iHeight = -iHeight;
		pbSrcX += (iHeight - 1) * iSrcXStride;
		iSrcXStride = -iSrcXStride;
	}

	iSrcXDif = iSrcXStride - (uWidth * 2);
	iDstYDif = iDstYStride - uWidth;
	iDstUvDif = iDstUvStride - (uWidth / 2);

	m7 = g_mWord00FF;
	for (y = iHeight / 2; y; y--)
	{
		for (x = uWidth / 8; x; x--)
		{
			m0 = ((PM64) pbSrcX)[0];
			m1 = ((PM64) pbSrcX)[1];
			m2 = ((PM64) (pbSrcX + iSrcXStride))[0];
			m3 = ((PM64) (pbSrcX + iSrcXStride))[1];

			m4 = m0;
			m5 = m2;

			m4 = _mm_and_si64(m4, m7);
			m5 = _mm_and_si64(m5, m7);

			m4 = _mm_add_pi16(m4, m5);

			m5 = m1;
			m6 = m3;

			m5 = _mm_and_si64(m5, m7);
			m6 = _mm_and_si64(m6, m7);

			m5 = _mm_add_pi16(m5, m6);

			m4 = _mm_add_pi16(m4, g_mWord0001);
			m5 = _mm_add_pi16(m5, g_mWord0001);

			m4 = _mm_srli_pi16(m4, 1);
			m5 = _mm_srli_pi16(m5, 1);

			m0 = _mm_srli_pi16(m0, 8);
			m1 = _mm_srli_pi16(m1, 8);
			m2 = _mm_srli_pi16(m2, 8);
			m3 = _mm_srli_pi16(m3, 8);

			m0 = _mm_and_si64(m0, m7);
			m1 = _mm_and_si64(m1, m7);
			m2 = _mm_and_si64(m2, m7);
			m3 = _mm_and_si64(m3, m7);

			m0 = _mm_packs_pu16(m0, m1);
			m2 = _mm_packs_pu16(m2, m3);

			((PM64) pbDstY)[0] = m0;
			((PM64) (pbDstY + iDstYStride))[0] = m2;

			m4 = _mm_packs_pu16(m4, m5);
			m5 = m4;

			m4 = _mm_srli_si64(m4, 8);

			m5 = _mm_and_si64(m5, m7);
			m4 = _mm_and_si64(m4, m7);

			m5 = _mm_packs_pu16(m5, m5);
			m4 = _mm_packs_pu16(m4, m4);

			((PDWORD) pbDstU)[0] = _mm_cvtsi64_si32(m5);
			((PDWORD) pbDstV)[0] = _mm_cvtsi64_si32(m4);

			pbSrcX += 16;
			pbDstY += 8;
			pbDstU += 4;
			pbDstV += 4;
		}

		for (x = (uWidth & 7) / 2; x; x--)
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

	_mm_empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to YUY2.
VOID UyvyToYuy2_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	Yuy2ToUyvy_mmx(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to YVYU.
VOID UyvyToYvyu_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to UYVY.
VOID UyvyToUyvy_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	LineCopy_mmx(pbDstX, iDstXStride, pbSrcX, iSrcXStride, uWidth * 2, iHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to VYUY.
VOID UyvyToVyuy_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to RGB32.
VOID UyvyToRgb32_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to RGB24.
VOID UyvyToRgb24_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to RGB16.
VOID UyvyToRgb16_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert UYVY to RGB15.
VOID UyvyToRgb15_mmx(PBYTE pbDstX, INT iDstXStride, PBYTE pbSrcX, INT iSrcXStride, UINT uWidth, INT iHeight)
{
	_NotImplement();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
