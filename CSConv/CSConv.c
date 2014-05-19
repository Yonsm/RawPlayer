


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header.
#include "CSConv.h"
#include <String.h>

#ifdef __INTEL_COMPILER
#pragma warning(disable: 1011 964)
#endif

#if (MSC_VER < 1300)
#pragma warning(disable: 4305)
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Global variable.
INT g_iYYTab[256] = {0};
INT g_iBUTab[256] = {0};
INT g_iGUTab[256] = {0};
INT g_iGVTab[256] = {0};
INT g_iRVTab[256] = {0};

#ifdef _MMX
M64 g_mSub10 = {0x0010001000100010};
M64 g_mSub80 = {0x0080008000800080};
M64 g_mYYMul = {0x004A004A004A004A};
M64 g_mUGMul = {0x0019001900190019};
M64 g_mVGMul = {0x0034003400340034};
M64 g_mUBMul = {0x0081008100810081};
M64 g_mVRMul = {0x0066006600660066};

M64 g_mWord0001 = {0x0001000100010001};
M64 g_mWord00FF = {0x00FF00FF00FF00FF};
M64 g_mWordFF00 = {0xFF00FF00FF00FF00};

M64 g_mMulToY = {0x0000004200810019};
M64 g_mMulToU = {0x0000FFDAFFB60070};
M64 g_mMulToV = {0x00000070FFA2FFEE};
M64 g_mMask24 = {0x0000000000FFFFFF};

M64 g_mMaskAvg = {0xFEFEFEFEFEFEFEFE};
#endif

#ifdef _SSE2
#ifdef __INTEL_COMPILER
M128I g_xiSub10;
M128I g_xiSub80;
M128I g_xiYYMul;
M128I g_xiUGMul;
M128I g_xiVGMul;
M128I g_xiUBMul;
M128I g_xiVRMul;

M128I g_xiWord00FF;
M128I g_xiWordFF00;

M128I g_xiMulToY;
M128I g_xiMulToU;
M128I g_xiMulToV;
M128I g_xiMask0;
M128I g_xiMask1;
M128I g_xiMask2;
M128I g_xiMask3;
#else
M128I g_xiSub10 = {0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00};
M128I g_xiSub80 = {0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00};
M128I g_xiYYMul = {0x4A, 0x00, 0x4A, 0x00, 0x4A, 0x00, 0x4A, 0x00, 0x4A, 0x00, 0x4A, 0x00, 0x4A, 0x00, 0x4A, 0x00};
M128I g_xiUGMul = {0x19, 0x00, 0x19, 0x00, 0x19, 0x00, 0x19, 0x00, 0x19, 0x00, 0x19, 0x00, 0x19, 0x00, 0x19, 0x00};
M128I g_xiVGMul = {0x34, 0x00, 0x34, 0x00, 0x34, 0x00, 0x34, 0x00, 0x34, 0x00, 0x34, 0x00, 0x34, 0x00, 0x34, 0x00};
M128I g_xiUBMul = {0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00};
M128I g_xiVRMul = {0x66, 0x00, 0x66, 0x00, 0x66, 0x00, 0x66, 0x00, 0x66, 0x00, 0x66, 0x00, 0x66, 0x00, 0x66, 0x00};

M128I g_xiWord00FF = {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00};
M128I g_xiWordFF00 = {0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};

M128I g_xiMulToY = {0x19, 0x00, 0x81, 0x00, 0x42, 0x00, 0x00, 0x00, 0x19, 0x00, 0x81, 0x00, 0x42, 0x00, 0x00, 0x00};
M128I g_xiMulToU = {0x70, 0x00, 0xB6, 0xFF, 0xDA, 0xFF, 0x00, 0x00, 0x70, 0x00, 0xB6, 0xFF, 0xDA, 0xFF, 0x00, 0x00};
M128I g_xiMulToV = {0xEE, 0xFF, 0xA2, 0xFF, 0x70, 0x00, 0x00, 0x00, 0xEE, 0xFF, 0xA2, 0xFF, 0x70, 0x00, 0x00, 0x00};

M128I g_xiMask0 = {0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
M128I g_xiMask1 = {0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
M128I g_xiMask2 = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
M128I g_xiMask3 = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
#endif
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get CPU feature.
#ifdef _M_IX86
__inline DWORD GetCpuId(DWORD dwFlag) {__asm mov eax, dwFlag __asm cpuid}
__inline DWORD GetCpuFx(DWORD dwFlag) {__asm mov eax, dwFlag __asm cpuid __asm mov eax, edx}
DWORD GetCpuFeature()
{
	DWORD dwExt;
	DWORD dwFeature;

	__try
	{
		// Get standard features.
		dwFeature = GetCpuFx(1) & CPU_MASK;

		// Get AMD specials.
		if (GetCpuId(0x80000000) == 0x80000000)
		{
			// Get extended features.
			dwExt = GetCpuFx(0x80000001);
			dwFeature |= (dwExt & CPU_K3D);
			dwFeature |= (dwExt & CPU_K3DX);
			dwFeature |= (dwExt & CPU_KMMX);
		}

		return dwFeature;
	}
	__except (1)
	{
		return 0;
	}
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize color space conversion. Return CPU feature.
DWORD InitCSConv()
{
	UINT i;
	DWORD dwCpuFeature;

	// Get CPU feature.
#if defined(_M_IX86)
	dwCpuFeature = GetCpuFeature();
#elif defined(_M_X64)
	dwCpuFeature = CPU_SSE2;
#else
	dwCpuFeature = 0;
#endif

	// Initialize color space data.
	for (i = 0; i < 256; i++)
	{
		g_iYYTab[i] = _FixOut(Y_Y_OUT) * (i - Y_ADD_OUT);
		g_iBUTab[i] = _FixOut(B_U_OUT) * (i - U_ADD_OUT);
		g_iGUTab[i] = _FixOut(G_U_OUT) * (i - U_ADD_OUT);
		g_iGVTab[i] = _FixOut(G_V_OUT) * (i - V_ADD_OUT);
		g_iRVTab[i] = _FixOut(R_V_OUT) * (i - V_ADD_OUT);
	}

#if defined(_SSE2) && defined(__INTEL_COMPILER)
	g_xiSub10 = _mm_set1_epi16(0x0010);
	g_xiSub80 = _mm_set1_epi16(0x0080);
	g_xiYYMul = _mm_set1_epi16(0x004A);
	g_xiUGMul = _mm_set1_epi16(0x0019);
	g_xiVGMul = _mm_set1_epi16(0x0034);
	g_xiUBMul = _mm_set1_epi16(0x0081);
	g_xiVRMul = _mm_set1_epi16(0x0066);

	g_xiWord00FF = _mm_set1_epi16(0x00FF);
	g_xiWordFF00 = _mm_set1_epi16(0xFF00);

	g_xiMulToY = _mm_set_epi32(0x00000042, 0x00810019, 0x00000042, 0x00810019);
	g_xiMulToU = _mm_set_epi32(0x0000FFDA, 0xFFB60070, 0x0000FFDA, 0xFFB60070);
	g_xiMulToV = _mm_set_epi32(0x00000070, 0xFFA2FFEE, 0x00000070, 0xFFA2FFEE);

	g_xiMask0 = _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00FFFFFF);
	g_xiMask1 = _mm_set_epi32(0x00000000, 0x00000000, 0x0000FFFF, 0xFF000000);
	g_xiMask2 = _mm_set_epi32(0x00000000, 0x000000FF, 0xFFFF0000, 0x00000000);
	g_xiMask3 = _mm_set_epi32(0x00000000, 0xFFFFFF00, 0x00000000, 0x00000000);
#endif

	return dwCpuFeature;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy memory line by line.
VOID LineCopy(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines)
{
	INT y;

	if (iLines < 0)
	{
		iLines = -iLines;
		pbSrc += (iLines - 1) * iSrcStride;
		iSrcStride = -iSrcStride;
	}

	for (y = iLines; y; y--)
	{
		memcpy(pbDst, pbSrc, uRows);
		pbSrc += iSrcStride;
		pbDst += iDstStride;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy memory line by line.
#ifdef _MMX
VOID LineCopy_mmx(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines)
{
	UINT x;
	INT y;
	PM64 p, q;
	M64 m0, m1, m2, m3, m4, m5, m6, m7;

	if (iLines < 0)
	{
		iLines = -iLines;
		pbSrc += (iLines - 1) * iSrcStride;
		iSrcStride = -iSrcStride;
	}

	for (y = iLines; y; y--)
	{
		p = (PM64) pbSrc;
		q = (PM64) pbDst;
		for (x = uRows / 64; x; x--)
		{
			m0 = p[0]; m1 = p[1]; m2 = p[2]; m3 = p[3]; m4 = p[4]; m5 = p[5]; m6 = p[6]; m7 = p[7];
			q[0] = m0; q[1] = m1; q[2] = m2; q[3] = m3; q[4] = m4; q[5] = m5; q[6] = m6; q[7] = m7;

			p += 8;
			q += 8;
		}
		memcpy(q, p, uRows & 63);

		pbSrc += iSrcStride;
		pbDst += iDstStride;
	}

	_mm_empty();
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy memory line by line.
#ifdef _SSE2
VOID LineCopy_sse2(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines)
{
	UINT x;
	INT y;
	PM128I p, q;
	M128I xi0, xi1, xi2, xi3, xi4, xi5, xi6, xi7;

	if (iLines < 0)
	{
		iLines = -iLines;
		pbSrc += (iLines - 1) * iSrcStride;
		iSrcStride = -iSrcStride;
	}

	for (y = iLines; y; y--)
	{
		p = (PM128I) pbSrc;
		q = (PM128I) pbDst;
		for (x = uRows / 128; x; x--)
		{
			_mm_prefetch((PBYTE) (p + 8), _MM_HINT_NTA);

			xi0 = _mm_loadu_si128(p + 0);
			xi1 = _mm_loadu_si128(p + 1);
			xi2 = _mm_loadu_si128(p + 2);
			xi3 = _mm_loadu_si128(p + 3);
			xi4 = _mm_loadu_si128(p + 4);
			xi5 = _mm_loadu_si128(p + 5);
			xi6 = _mm_loadu_si128(p + 6);
			xi7 = _mm_loadu_si128(p + 7);

			_mm_storeu_si128(q + 0, xi0);
			_mm_storeu_si128(q + 1, xi1);
			_mm_storeu_si128(q + 2, xi2);
			_mm_storeu_si128(q + 3, xi3);
			_mm_storeu_si128(q + 4, xi4);
			_mm_storeu_si128(q + 5, xi5);
			_mm_storeu_si128(q + 6, xi6);
			_mm_storeu_si128(q + 7, xi7);

			p += 8;
			q += 8;
		}
		memcpy(q, p, uRows & 127);

		pbSrc += iSrcStride;
		pbDst += iDstStride;
	}
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy memory line by line with de-interlacing.
VOID LineCopyDI(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines)
{
	UINT x;
	INT y;

	if (iLines < 0)
	{
		iLines = -iLines;
		pbSrc += (iLines - 1) * iSrcStride;
		iSrcStride = -iSrcStride;
	}

	memcpy(pbDst, pbSrc, uRows);
	pbDst += iDstStride;

	for (y = iLines - 2; y; y--)
	{
		for (x = 0; x < uRows; x++)
		{
			pbDst[x] = ((pbSrc[x] + pbSrc[iSrcStride * 2 + x] + 1) / 2 + pbSrc[iSrcStride + x] + 1) / 2;
		}

		pbSrc += iSrcStride;
		pbDst += iDstStride;
	}

	pbSrc += iSrcStride;
	memcpy(pbDst, pbSrc, uRows);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy memory line by line with de-interlacing.
#ifdef _MMX
VOID LineCopyDI_mmx(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines)
{
	UINT x;
	INT y;
	PBYTE p, q; 
	M64 m0,m1,m2,m3, mMaskAvg;

	p = pbSrc;
	q = pbDst;
	mMaskAvg = g_mMaskAvg;

	if (iLines < 0)
	{
		iLines = -iLines;
		pbSrc += (iLines - 1) * iSrcStride;
		p = pbSrc;
		iSrcStride = -iSrcStride;
	}

	memcpy(pbDst, pbSrc, uRows);
	pbDst += iDstStride;
	q = pbDst;

	for (y = iLines - 2; y; y--)
	{
		for (x = uRows / 8; x; x--)
		{
			m1 = *(PM64) p;
			m2 = *(PM64) (p + iSrcStride);
			m3 = *(PM64) (p + iSrcStride * 2);

			m1 = _mm_and_si64(m1, mMaskAvg);
			m2 = _mm_and_si64(m2, mMaskAvg);
			m3 = _mm_and_si64(m3, mMaskAvg);
			m1 = _mm_srli_si64(m1, 1);
			m2 = _mm_srli_si64(m2, 1);
			m3 = _mm_srli_si64(m3, 1);
			m0 = _mm_adds_pu8(m1, m3);
			m0 = _mm_and_si64(m0, mMaskAvg);
			m0 = _mm_srli_si64(m0, 1);
			m0 = _mm_adds_pu8(m0, m2);

			*(PM64) q = m0;
			q += 8;
			p += 8;
		}

		for (x = (uRows & 7); x; x--)
		{
			q[0] = ((p[0] + p[iSrcStride * 2 + 0] + 1) / 2 + p[iSrcStride + 0] + 1) / 2;
			q++;
			p++;
		}

		pbSrc += iSrcStride;
		pbDst += iDstStride;
		p = pbSrc;
		q = pbDst;
	}

	pbSrc += iSrcStride;
	memcpy(pbDst, pbSrc, uRows);

	_mm_empty();
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy memory line by line with de-interlacing.
#ifdef _XMM
VOID LineCopyDI_xmm(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines)
{
	UINT x;
	INT y;
	PBYTE p, q; 
	M64 m0;

	p = pbSrc;
	q = pbDst;

	if (iLines < 0)
	{
		iLines = -iLines;
		pbSrc += (iLines - 1) * iSrcStride;
		p = pbSrc;
		iSrcStride = -iSrcStride;
	}

	memcpy(pbDst, pbSrc, uRows);
	pbDst += iDstStride;
	q = pbDst;

	for (y = iLines - 2; y; y--)
	{
		for (x = uRows / 8; x; x--)
		{
			_mm_prefetch(p + 8, _MM_HINT_NTA);
			_mm_prefetch(p + iSrcStride + 8, _MM_HINT_NTA);
			_mm_prefetch(p + iSrcStride * 2 + 8, _MM_HINT_NTA);

			m0 = *(PM64) p;
			m0 = _mm_avg_pu8(m0, *((PM64) (p + iSrcStride * 2)));
			m0 = _mm_avg_pu8(m0, *((PM64) (p + iSrcStride)));

			*(PM64) q = m0;
			q += 8;
			p += 8;
		}

		for (x = (uRows & 7); x; x--)
		{
			q[0] = ((p[0] + p[iSrcStride * 2 + 0] + 1) / 2 + p[iSrcStride + 0] + 1) / 2;
			q++;
			p++;
		}

		pbSrc += iSrcStride;
		pbDst += iDstStride;
		p = pbSrc;
		q = pbDst;
	}

	pbSrc += iSrcStride;
	memcpy(pbDst, pbSrc, uRows);

	_mm_empty();
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy memory line by line with de-interlacing.
#ifdef _SSE2
VOID LineCopyDI_sse2(PBYTE pbDst, INT iDstStride, PBYTE pbSrc, INT iSrcStride, UINT uRows, INT iLines)
{
	UINT x;
	INT y;
	PBYTE p, q;
	M128I xi0, xi1, xi2, xi3;

	p = pbSrc;
	q = pbDst;

	if (iLines < 0)
	{
		iLines = -iLines;
		pbSrc += (iLines - 1) * iSrcStride;
		p = pbSrc;
		iSrcStride = -iSrcStride;
	}

	memcpy(pbDst, pbSrc, uRows);
	pbDst += iDstStride;
	q = pbDst;

	for (y = iLines - 2; y; y--)
	{
		for (x = uRows / 16; x; x--)
		{
			_mm_prefetch(p + 16, _MM_HINT_NTA);
			_mm_prefetch(p + iSrcStride + 16, _MM_HINT_NTA);
			_mm_prefetch(p + iSrcStride * 2 + 16, _MM_HINT_NTA);
			xi1 = _mm_loadu_si128((PM128I) p);
			xi2 = _mm_loadu_si128((PM128I) (p + iSrcStride));
			xi3 = _mm_loadu_si128((PM128I) (p + iSrcStride * 2));
			xi0 = _mm_avg_epu8(xi1, xi3);
			xi0 = _mm_avg_epu8(xi0, xi2);
			_mm_storeu_si128((PM128I) q, xi0);
			q += 16;
			p += 16;
		}

		for (x = (uRows & 15); x; x--)
		{
			q[0] = ((p[0] + p[iSrcStride * 2 + 0] + 1) / 2 + p[iSrcStride + 0] + 1) / 2;
			q++;
			p++;
		}

		pbSrc += iSrcStride;
		pbDst += iDstStride;
		p = pbSrc;
		q = pbDst;
	}

	pbSrc += iSrcStride;
	memcpy(pbDst, pbSrc, uRows);
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
