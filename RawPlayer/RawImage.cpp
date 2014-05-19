


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "Main.h"
#include "RawImage.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 转换函数
const PVOID c_pvCSConv[CS_ALL][CS_ALL] = 
{
	{Yv12ToYv12, Yv12ToYv12, Yv12ToYuy2, Yv12ToYvyu, Yv12ToUyvy, Yv12ToVyuy, Yv12ToRgb32, Yv12ToRgb24, Yv12ToRgb16, Yv12ToRgb15},
	{Yv12ToYv12, Yv12ToYv12, Yv12ToYuy2, Yv12ToYvyu, Yv12ToUyvy, Yv12ToVyuy, Yv12ToRgb32, Yv12ToRgb24, Yv12ToRgb16, Yv12ToRgb15},
	{Yuy2ToYv12, Yuy2ToYv12, Yuy2ToYuy2, Yuy2ToYvyu, Yuy2ToUyvy, Yuy2ToVyuy, Yuy2ToRgb32, Yuy2ToRgb24, Yuy2ToRgb16, Yuy2ToRgb15},
	{YvyuToYv12, YvyuToYv12, YvyuToYuy2, YvyuToYvyu, YvyuToUyvy, YvyuToVyuy, YvyuToRgb32, YvyuToRgb24, YvyuToRgb16, YvyuToRgb15},
	{UyvyToYv12, UyvyToYv12, UyvyToYuy2, UyvyToYvyu, UyvyToUyvy, UyvyToVyuy, UyvyToRgb32, UyvyToRgb24, UyvyToRgb16, UyvyToRgb15},
	{VyuyToYv12, VyuyToYv12, VyuyToYuy2, VyuyToYvyu, VyuyToUyvy, VyuyToVyuy, VyuyToRgb32, VyuyToRgb24, VyuyToRgb16, VyuyToRgb15},
	{Rgb32ToYv12, Rgb32ToYv12, Rgb32ToYuy2, Rgb32ToYvyu, Rgb32ToUyvy, Rgb32ToVyuy, Rgb32ToRgb32, Rgb32ToRgb24, Rgb32ToRgb16, Rgb32ToRgb15},
	{Rgb24ToYv12, Rgb24ToYv12, Rgb24ToYuy2, Rgb24ToYvyu, Rgb24ToUyvy, Rgb24ToVyuy, Rgb24ToRgb32, Rgb24ToRgb24, Rgb24ToRgb16, Rgb24ToRgb15},
	{Rgb16ToYv12, Rgb16ToYv12, Rgb16ToYuy2, Rgb16ToYvyu, Rgb16ToUyvy, Rgb16ToVyuy, Rgb16ToRgb32, Rgb16ToRgb24, Rgb16ToRgb16, Rgb16ToRgb15},
	{Rgb15ToYv12, Rgb15ToYv12, Rgb15ToYuy2, Rgb15ToYvyu, Rgb15ToUyvy, Rgb15ToVyuy, Rgb15ToRgb32, Rgb15ToRgb24, Rgb15ToRgb16, Rgb15ToRgb15}
};

#ifdef _M_IX86
const PVOID c_pvCSConv_mmx[CS_ALL][CS_ALL] = 
{
	{Yv12ToYv12_mmx, Yv12ToYv12_mmx, Yv12ToYuy2_mmx, Yv12ToYvyu_mmx, Yv12ToUyvy_mmx, Yv12ToVyuy_mmx, Yv12ToRgb32_mmx, Yv12ToRgb24_mmx, Yv12ToRgb16_mmx, Yv12ToRgb15_mmx},
	{Yv12ToYv12_mmx, Yv12ToYv12_mmx, Yv12ToYuy2_mmx, Yv12ToYvyu_mmx, Yv12ToUyvy_mmx, Yv12ToVyuy_mmx, Yv12ToRgb32_mmx, Yv12ToRgb24_mmx, Yv12ToRgb16_mmx, Yv12ToRgb15_mmx},
	{Yuy2ToYv12_mmx, Yuy2ToYv12_mmx, Yuy2ToYuy2_mmx, Yuy2ToYvyu, Yuy2ToUyvy_mmx, Yuy2ToVyuy, Yuy2ToRgb32_mmx, Yuy2ToRgb24_mmx, Yuy2ToRgb16, Yuy2ToRgb15},
	{YvyuToYv12, YvyuToYv12, YvyuToYuy2, YvyuToYvyu, YvyuToUyvy, YvyuToVyuy, YvyuToRgb32, YvyuToRgb24, YvyuToRgb16, YvyuToRgb15},
	{UyvyToYv12_mmx, UyvyToYv12_mmx, UyvyToYuy2_mmx, UyvyToYvyu, UyvyToUyvy_mmx, UyvyToVyuy, UyvyToRgb32, UyvyToRgb24, UyvyToRgb16, UyvyToRgb15},
	{VyuyToYv12, VyuyToYv12, VyuyToYuy2, VyuyToYvyu, VyuyToUyvy, VyuyToVyuy, VyuyToRgb32, VyuyToRgb24, VyuyToRgb16, VyuyToRgb15},
	{Rgb32ToYv12_mmx, Rgb32ToYv12_mmx, Rgb32ToYuy2, Rgb32ToYvyu, Rgb32ToUyvy, Rgb32ToVyuy, Rgb32ToRgb32, Rgb32ToRgb24, Rgb32ToRgb16, Rgb32ToRgb15},
	{Rgb24ToYv12_mmx, Rgb24ToYv12_mmx, Rgb24ToYuy2, Rgb24ToYvyu, Rgb24ToUyvy, Rgb24ToVyuy, Rgb24ToRgb32, Rgb24ToRgb24, Rgb24ToRgb16, Rgb24ToRgb15},
	{Rgb16ToYv12, Rgb16ToYv12, Rgb16ToYuy2, Rgb16ToYvyu, Rgb16ToUyvy, Rgb16ToVyuy, Rgb16ToRgb32, Rgb16ToRgb24, Rgb16ToRgb16, Rgb16ToRgb15},
	{Rgb15ToYv12, Rgb15ToYv12, Rgb15ToYuy2, Rgb15ToYvyu, Rgb15ToUyvy, Rgb15ToVyuy, Rgb15ToRgb32, Rgb15ToRgb24, Rgb15ToRgb16, Rgb15ToRgb15}
};
#endif

const PVOID c_pvCSConv_sse2[CS_ALL][CS_ALL] = 
{
	{Yv12ToYv12_sse2, Yv12ToYv12_sse2, Yv12ToYuy2_sse2, Yv12ToYvyu_sse2, Yv12ToUyvy_sse2, Yv12ToVyuy_sse2, Yv12ToRgb32_sse2, Yv12ToRgb24_sse2, Yv12ToRgb16_sse2, Yv12ToRgb15_sse2},
	{Yv12ToYv12_sse2, Yv12ToYv12_sse2, Yv12ToYuy2_sse2, Yv12ToYvyu_sse2, Yv12ToUyvy_sse2, Yv12ToVyuy_sse2, Yv12ToRgb32_sse2, Yv12ToRgb24_sse2, Yv12ToRgb16_sse2, Yv12ToRgb15_sse2},
	{Yuy2ToYv12_sse2, Yuy2ToYv12_sse2, Yuy2ToYuy2_sse2, Yuy2ToYvyu, Yuy2ToUyvy_sse2, Yuy2ToVyuy, Yuy2ToRgb32_sse2, Yuy2ToRgb24_sse2, Yuy2ToRgb16, Yuy2ToRgb15},
	{YvyuToYv12, YvyuToYv12, YvyuToYuy2, YvyuToYvyu, YvyuToUyvy, YvyuToVyuy, YvyuToRgb32, YvyuToRgb24, YvyuToRgb16, YvyuToRgb15},
	{UyvyToYv12_sse2, UyvyToYv12_sse2, UyvyToYuy2_sse2, UyvyToYvyu, UyvyToUyvy_sse2, UyvyToVyuy, UyvyToRgb32, UyvyToRgb24, UyvyToRgb16, UyvyToRgb15},
	{VyuyToYv12, VyuyToYv12, VyuyToYuy2, VyuyToYvyu, VyuyToUyvy, VyuyToVyuy, VyuyToRgb32, VyuyToRgb24, VyuyToRgb16, VyuyToRgb15},
	{Rgb32ToYv12_sse2, Rgb32ToYv12_sse2, Rgb32ToYuy2, Rgb32ToYvyu, Rgb32ToUyvy, Rgb32ToVyuy, Rgb32ToRgb32, Rgb32ToRgb24, Rgb32ToRgb16, Rgb32ToRgb15},
	{Rgb24ToYv12_sse2, Rgb24ToYv12_sse2, Rgb24ToYuy2, Rgb24ToYvyu, Rgb24ToUyvy, Rgb24ToVyuy, Rgb24ToRgb32, Rgb24ToRgb24, Rgb24ToRgb16, Rgb24ToRgb15},
	{Rgb16ToYv12, Rgb16ToYv12, Rgb16ToYuy2, Rgb16ToYvyu, Rgb16ToUyvy, Rgb16ToVyuy, Rgb16ToRgb32, Rgb16ToRgb24, Rgb16ToRgb16, Rgb16ToRgb15},
	{Rgb15ToYv12, Rgb15ToYv12, Rgb15ToYuy2, Rgb15ToYvyu, Rgb15ToUyvy, Rgb15ToVyuy, Rgb15ToRgb32, Rgb15ToRgb24, Rgb15ToRgb16, Rgb15ToRgb15}
};

const PVOID c_pvCSConvDI[CS_ALL][CS_ALL] = 
{
	{Yv12ToYv12DI, Yv12ToYv12DI, Yv12ToYuy2DI, Yv12ToYvyuDI, Yv12ToUyvyDI, Yv12ToVyuyDI, Yv12ToRgb32DI, Yv12ToRgb24DI, Yv12ToRgb16DI, Yv12ToRgb15DI},
	{Yv12ToYv12DI, Yv12ToYv12DI, Yv12ToYuy2DI, Yv12ToYvyuDI, Yv12ToUyvyDI, Yv12ToVyuyDI, Yv12ToRgb32DI, Yv12ToRgb24DI, Yv12ToRgb16DI, Yv12ToRgb15DI},
	{Yuy2ToYv12DI, Yuy2ToYv12DI, Yuy2ToYuy2DI, Yuy2ToYvyu, Yuy2ToUyvyDI, Yuy2ToVyuy, Yuy2ToRgb32, Yuy2ToRgb24, Yuy2ToRgb16, Yuy2ToRgb15},
	{YvyuToYv12, YvyuToYv12, YvyuToYuy2, YvyuToYvyu, YvyuToUyvy, YvyuToVyuy, YvyuToRgb32, YvyuToRgb24, YvyuToRgb16, YvyuToRgb15},
	{UyvyToYv12DI, UyvyToYv12DI, UyvyToYuy2DI, UyvyToYvyu, UyvyToUyvyDI, UyvyToVyuy, UyvyToRgb32, UyvyToRgb24, UyvyToRgb16, UyvyToRgb15},
	{VyuyToYv12, VyuyToYv12, VyuyToYuy2, VyuyToYvyu, VyuyToUyvy, VyuyToVyuy, VyuyToRgb32, VyuyToRgb24, VyuyToRgb16, VyuyToRgb15},
	{Rgb32ToYv12, Rgb32ToYv12, Rgb32ToYuy2, Rgb32ToYvyu, Rgb32ToUyvy, Rgb32ToVyuy, Rgb32ToRgb32, Rgb32ToRgb24, Rgb32ToRgb16, Rgb32ToRgb15},
	{Rgb24ToYv12, Rgb24ToYv12, Rgb24ToYuy2, Rgb24ToYvyu, Rgb24ToUyvy, Rgb24ToVyuy, Rgb24ToRgb32, Rgb24ToRgb24, Rgb24ToRgb16, Rgb24ToRgb15},
	{Rgb16ToYv12, Rgb16ToYv12, Rgb16ToYuy2, Rgb16ToYvyu, Rgb16ToUyvy, Rgb16ToVyuy, Rgb16ToRgb32, Rgb16ToRgb24, Rgb16ToRgb16, Rgb16ToRgb15},
	{Rgb15ToYv12, Rgb15ToYv12, Rgb15ToYuy2, Rgb15ToYvyu, Rgb15ToUyvy, Rgb15ToVyuy, Rgb15ToRgb32, Rgb15ToRgb24, Rgb15ToRgb16, Rgb15ToRgb15}
};

#ifdef _M_IX86
const PVOID c_pvCSConvDI_mmx[CS_ALL][CS_ALL] = 
{
	{Yv12ToYv12DI_mmx, Yv12ToYv12DI_mmx, Yv12ToYuy2DI_mmx, Yv12ToYvyuDI_mmx, Yv12ToUyvyDI_mmx, Yv12ToVyuyDI_mmx, Yv12ToRgb32DI_mmx, Yv12ToRgb24DI_mmx, Yv12ToRgb16DI_mmx, Yv12ToRgb15DI_mmx},
	{Yv12ToYv12DI_mmx, Yv12ToYv12DI_mmx, Yv12ToYuy2DI_mmx, Yv12ToYvyuDI_mmx, Yv12ToUyvyDI_mmx, Yv12ToVyuyDI_mmx, Yv12ToRgb32DI_mmx, Yv12ToRgb24DI_mmx, Yv12ToRgb16DI_mmx, Yv12ToRgb15DI_mmx},
	{Yuy2ToYv12DI_mmx, Yuy2ToYv12DI_mmx, Yuy2ToYuy2DI_mmx, Yuy2ToYvyu, Yuy2ToUyvyDI_mmx, Yuy2ToVyuy, Yuy2ToRgb32, Yuy2ToRgb24, Yuy2ToRgb16, Yuy2ToRgb15},
	{YvyuToYv12, YvyuToYv12, YvyuToYuy2, YvyuToYvyu, YvyuToUyvy, YvyuToVyuy, YvyuToRgb32, YvyuToRgb24, YvyuToRgb16, YvyuToRgb15},
	{UyvyToYv12DI_mmx, UyvyToYv12DI_mmx, UyvyToYuy2DI_mmx, UyvyToYvyu, UyvyToUyvyDI_mmx, UyvyToVyuy, UyvyToRgb32, UyvyToRgb24, UyvyToRgb16, UyvyToRgb15},
	{VyuyToYv12, VyuyToYv12, VyuyToYuy2, VyuyToYvyu, VyuyToUyvy, VyuyToVyuy, VyuyToRgb32, VyuyToRgb24, VyuyToRgb16, VyuyToRgb15},
	{Rgb32ToYv12, Rgb32ToYv12, Rgb32ToYuy2, Rgb32ToYvyu, Rgb32ToUyvy, Rgb32ToVyuy, Rgb32ToRgb32, Rgb32ToRgb24, Rgb32ToRgb16, Rgb32ToRgb15},
	{Rgb24ToYv12, Rgb24ToYv12, Rgb24ToYuy2, Rgb24ToYvyu, Rgb24ToUyvy, Rgb24ToVyuy, Rgb24ToRgb32, Rgb24ToRgb24, Rgb24ToRgb16, Rgb24ToRgb15},
	{Rgb16ToYv12, Rgb16ToYv12, Rgb16ToYuy2, Rgb16ToYvyu, Rgb16ToUyvy, Rgb16ToVyuy, Rgb16ToRgb32, Rgb16ToRgb24, Rgb16ToRgb16, Rgb16ToRgb15},
	{Rgb15ToYv12, Rgb15ToYv12, Rgb15ToYuy2, Rgb15ToYvyu, Rgb15ToUyvy, Rgb15ToVyuy, Rgb15ToRgb32, Rgb15ToRgb24, Rgb15ToRgb16, Rgb15ToRgb15}
};

const PVOID c_pvCSConvDI_xmm[CS_ALL][CS_ALL] = 
{
	{Yv12ToYv12DI_xmm, Yv12ToYv12DI_xmm, Yv12ToYuy2DI_xmm, Yv12ToYvyuDI_xmm, Yv12ToUyvyDI_xmm, Yv12ToVyuyDI_xmm, Yv12ToRgb32DI_xmm, Yv12ToRgb24DI_xmm, Yv12ToRgb16DI_xmm, Yv12ToRgb15DI_xmm},
	{Yv12ToYv12DI_xmm, Yv12ToYv12DI_xmm, Yv12ToYuy2DI_xmm, Yv12ToYvyuDI_xmm, Yv12ToUyvyDI_xmm, Yv12ToVyuyDI_xmm, Yv12ToRgb32DI_xmm, Yv12ToRgb24DI_xmm, Yv12ToRgb16DI_xmm, Yv12ToRgb15DI_xmm},
	{Yuy2ToYv12DI_xmm, Yuy2ToYv12DI_xmm, Yuy2ToYuy2DI_xmm, Yuy2ToYvyu, Yuy2ToUyvyDI_xmm, Yuy2ToVyuy, Yuy2ToRgb32, Yuy2ToRgb24, Yuy2ToRgb16, Yuy2ToRgb15},
	{YvyuToYv12, YvyuToYv12, YvyuToYuy2, YvyuToYvyu, YvyuToUyvy, YvyuToVyuy, YvyuToRgb32, YvyuToRgb24, YvyuToRgb16, YvyuToRgb15},
	{UyvyToYv12DI_xmm, UyvyToYv12DI_xmm, UyvyToYuy2DI_xmm, UyvyToYvyu, UyvyToUyvyDI_xmm, UyvyToVyuy, UyvyToRgb32, UyvyToRgb24, UyvyToRgb16, UyvyToRgb15},
	{VyuyToYv12, VyuyToYv12, VyuyToYuy2, VyuyToYvyu, VyuyToUyvy, VyuyToVyuy, VyuyToRgb32, VyuyToRgb24, VyuyToRgb16, VyuyToRgb15},
	{Rgb32ToYv12, Rgb32ToYv12, Rgb32ToYuy2, Rgb32ToYvyu, Rgb32ToUyvy, Rgb32ToVyuy, Rgb32ToRgb32, Rgb32ToRgb24, Rgb32ToRgb16, Rgb32ToRgb15},
	{Rgb24ToYv12, Rgb24ToYv12, Rgb24ToYuy2, Rgb24ToYvyu, Rgb24ToUyvy, Rgb24ToVyuy, Rgb24ToRgb32, Rgb24ToRgb24, Rgb24ToRgb16, Rgb24ToRgb15},
	{Rgb16ToYv12, Rgb16ToYv12, Rgb16ToYuy2, Rgb16ToYvyu, Rgb16ToUyvy, Rgb16ToVyuy, Rgb16ToRgb32, Rgb16ToRgb24, Rgb16ToRgb16, Rgb16ToRgb15},
	{Rgb15ToYv12, Rgb15ToYv12, Rgb15ToYuy2, Rgb15ToYvyu, Rgb15ToUyvy, Rgb15ToVyuy, Rgb15ToRgb32, Rgb15ToRgb24, Rgb15ToRgb16, Rgb15ToRgb15}
};
#endif

const PVOID c_pvCSConvDI_sse2[CS_ALL][CS_ALL] = 
{
	{Yv12ToYv12DI_sse2, Yv12ToYv12DI_sse2, Yv12ToYuy2DI_sse2, Yv12ToYvyuDI_sse2, Yv12ToUyvyDI_sse2, Yv12ToVyuyDI_sse2, Yv12ToRgb32DI_sse2, Yv12ToRgb24DI_sse2, Yv12ToRgb16DI_sse2, Yv12ToRgb15DI_sse2},
	{Yv12ToYv12DI_sse2, Yv12ToYv12DI_sse2, Yv12ToYuy2DI_sse2, Yv12ToYvyuDI_sse2, Yv12ToUyvyDI_sse2, Yv12ToVyuyDI_sse2, Yv12ToRgb32DI_sse2, Yv12ToRgb24DI_sse2, Yv12ToRgb16DI_sse2, Yv12ToRgb15DI_sse2},
	{Yuy2ToYv12DI_sse2, Yuy2ToYv12DI_sse2, Yuy2ToYuy2DI_sse2, Yuy2ToYvyu, Yuy2ToUyvyDI_sse2, Yuy2ToVyuy, Yuy2ToRgb32, Yuy2ToRgb24, Yuy2ToRgb16, Yuy2ToRgb15},
	{YvyuToYv12, YvyuToYv12, YvyuToYuy2, YvyuToYvyu, YvyuToUyvy, YvyuToVyuy, YvyuToRgb32, YvyuToRgb24, YvyuToRgb16, YvyuToRgb15},
	{UyvyToYv12DI_sse2, UyvyToYv12DI_sse2, UyvyToYuy2DI_sse2, UyvyToYvyu, UyvyToUyvyDI_sse2, UyvyToVyuy, UyvyToRgb32, UyvyToRgb24, UyvyToRgb16, UyvyToRgb15},
	{VyuyToYv12, VyuyToYv12, VyuyToYuy2, VyuyToYvyu, VyuyToUyvy, VyuyToVyuy, VyuyToRgb32, VyuyToRgb24, VyuyToRgb16, VyuyToRgb15},
	{Rgb32ToYv12, Rgb32ToYv12, Rgb32ToYuy2, Rgb32ToYvyu, Rgb32ToUyvy, Rgb32ToVyuy, Rgb32ToRgb32, Rgb32ToRgb24, Rgb32ToRgb16, Rgb32ToRgb15},
	{Rgb24ToYv12, Rgb24ToYv12, Rgb24ToYuy2, Rgb24ToYvyu, Rgb24ToUyvy, Rgb24ToVyuy, Rgb24ToRgb32, Rgb24ToRgb24, Rgb24ToRgb16, Rgb24ToRgb15},
	{Rgb16ToYv12, Rgb16ToYv12, Rgb16ToYuy2, Rgb16ToYvyu, Rgb16ToUyvy, Rgb16ToVyuy, Rgb16ToRgb32, Rgb16ToRgb24, Rgb16ToRgb16, Rgb16ToRgb15},
	{Rgb15ToYv12, Rgb15ToYv12, Rgb15ToYuy2, Rgb15ToYvyu, Rgb15ToUyvy, Rgb15ToVyuy, Rgb15ToRgb32, Rgb15ToRgb24, Rgb15ToRgb16, Rgb15ToRgb15}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRawImage 成员变量
DWORD CRawImage::m_dwCpuFeature = 0;
const PVOID (*CRawImage::m_pvCSConv)[CS_ALL] = c_pvCSConv;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 初始化
DWORD CRawImage::Initialize()
{
	// 初始化色度空间转换库
	if (m_dwCpuFeature == 0)
	{
		m_dwCpuFeature = InitCSConv();
	}

	// 选择转换函数
	switch (CIni::GetInt(TEXT("CSConv")))
	{
	case 1:
		m_pvCSConv = c_pvCSConv;
		break;

	case 2:
		m_pvCSConv = c_pvCSConvDI;
		break;

#ifdef _M_IX86
	case 3:
		m_pvCSConv = c_pvCSConv_mmx;
		break;

	case 4:
		m_pvCSConv = c_pvCSConvDI_mmx;
		break;

	//case 5:
	//	m_pvCSConv = c_pvCSConv_xmm;
	//	break;

	case 6:
		m_pvCSConv = c_pvCSConvDI_xmm;
		break;
#endif

	case 7:
		m_pvCSConv = c_pvCSConv_sse2;
		break;

	case 8:
		m_pvCSConv = c_pvCSConvDI_sse2;
		break;

	default:
		if (m_dwCpuFeature & CPU_SSE2)
		{
			m_pvCSConv = c_pvCSConv_sse2;
		}
#ifdef _M_IX86
		else if (m_dwCpuFeature & CPU_MMX)
		{
			m_pvCSConv = c_pvCSConv_mmx;
		}
#endif
		else
		{
			m_pvCSConv = c_pvCSConv;
		}
		break;
	}

	return m_dwCpuFeature;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 获取 CPU 特性
DWORD CRawImage::GetCpuFeature(PTSTR ptzText)
{
	Initialize();

	if (ptzText)
	{
		lstrcpy(ptzText, TEXT("CPU:"));
		_ExIf(m_dwCpuFeature & CPU_K3D, lstrcat(ptzText, TEXT(" 3DNOW")));
		_ExIf(m_dwCpuFeature & CPU_MMX, lstrcat(ptzText, TEXT(" MMX")));
		_ExIf(m_dwCpuFeature & CPU_SSE, lstrcat(ptzText, TEXT(" SSE")));
		_ExIf(m_dwCpuFeature & CPU_SSE2, lstrcat(ptzText, TEXT(" SSE2")));
	}

	return m_dwCpuFeature;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
VOID CRawImage::Update()
{
	// 校验色度空间
	_ExIf(m_csColorSpace >= CS_ALL, m_csColorSpace = CS_YV12);

	// 校验宽度
	m_uWidth = _Bound(m_uWidth, MIN_Resolution, MAX_Resolution);
	m_uWidth = (m_uWidth + 1) & ~1;

	// 校验高度
	m_uHeight = abs(m_iHeight);
	m_uHeight = _Bound(m_uHeight, MIN_Resolution, MAX_Resolution);
	m_iHeight = (m_iHeight + 1) & ~1;
	m_iHeight = (m_iHeight < 0) ? (-((INT) m_uHeight)) : m_uHeight;
	
	// 校验帧率
	m_uFrameRate = _Bound(m_uFrameRate, 1, MAX_FrameRate);

	// 设置跨度
	GetStride();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 转换
PBYTE CRawImage::Convert(CRawImage &riDst, PBYTE pbDst, PBYTE pbSrc)
{
	PVOID pvCSConv;
	PBYTE pbDstU, pbDstV, pbSrcU, pbSrcV;

	// 获取转换函数
	pvCSConv = m_pvCSConv[m_csColorSpace][riDst.m_csColorSpace];

	if ((m_csColorSpace == CS_YV12) || (m_csColorSpace == CS_I420))
	{
		// 获取源 UV 分量
		GetUV(pbSrc, &pbSrcU, &pbSrcV);

		if ((riDst.m_csColorSpace == CS_YV12) || (riDst.m_csColorSpace == CS_I420))
		{
			// 获取目标 UV 分量
			riDst.GetUV(pbDst, &pbDstU, &pbDstV);

			// 转换
			((FLATTOFLAT *) pvCSConv)(pbDst, pbDstU, pbDstV, riDst.m_uXStride, riDst.m_uUvStride, 
				pbSrc, pbSrcU, pbSrcV, m_uXStride, m_uUvStride, riDst.m_uWidth, riDst.m_iHeight);
		}
		else
		{
			// 转换
			((FLATTOPACK *) pvCSConv)(pbDst, riDst.m_uXStride, 
				pbSrc, pbSrcU, pbSrcV, m_uXStride, m_uUvStride, riDst.m_uWidth, riDst.m_iHeight);
		}
	}
	else
	{
		if ((riDst.m_csColorSpace == CS_YV12) || (riDst.m_csColorSpace == CS_I420))
		{
			// 获取目标 UV 分量
			riDst.GetUV(pbDst, &pbDstU, &pbDstV);

			// 转换
			((PACKTOFLAT *) pvCSConv)(pbDst, pbDstU, pbDstV, riDst.m_uXStride, riDst.m_uUvStride, 
				pbSrc, m_uXStride, riDst.m_uWidth, riDst.m_iHeight);
		}
		else
		{
			// 转换
			((PACKTOPACK *) pvCSConv)(pbDst, riDst.m_uXStride, pbSrc, m_uXStride, riDst.m_uWidth, riDst.m_iHeight);
		}
	}

	return pbDst;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 获取跨度
VOID CRawImage::GetStride()
{
	// 设置跨度
	switch (m_csColorSpace)
	{
	case CS_YV12:
	case CS_I420:
		m_uXStride = m_uWidth;
		m_uUvStride = m_uWidth / 2;
		break;

	case CS_YUY2:
	case CS_YVYU:
	case CS_UYVY:
	case CS_VYUY:
		m_uXStride = m_uWidth * 2;
		m_uUvStride = 0;
		break;

	case CS_RGB32:
		m_uXStride = m_uWidth * 4;
		m_uUvStride = 0;
		break;

	case CS_RGB24:
		m_uXStride = _DibPitch24(m_uWidth);;
		m_uUvStride = 0;
		break;

	case CS_RGB16:
	case CS_RGB15:
		m_uXStride = _DibPitch(m_uWidth, 16);
		m_uUvStride = 0;
		break;

	default:
		m_uXStride = 0;
		m_uUvStride = 0;
		break;
	}

	// 设置帧大小
	m_uFrameSize = (m_uXStride + m_uUvStride) * m_uHeight;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 获取 UV 分量
VOID CRawImage::GetUV(PBYTE pbX, PBYTE *ppbU, PBYTE *ppbV)
{
	_Assert(ppbU && ppbV);

	if (m_csColorSpace == CS_YV12)
	{
		*ppbV = pbX + m_uXStride * m_uHeight;
		*ppbU = *ppbV + m_uUvStride * m_uHeight / 2;
	}
	else if (m_csColorSpace == CS_I420)
	{
		*ppbU = pbX + m_uXStride * m_uHeight;
		*ppbV = *ppbU + m_uUvStride * m_uHeight / 2;
	}
	else
	{
		*ppbU = *ppbV = NULL;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
