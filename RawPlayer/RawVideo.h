


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>
#include "RawImage.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRawVideo 类
class CRawVideo: public CRawImage
{
public:
	HANDLE m_hFile;						// 文件句柄
	PBYTE m_pbRead;						// 文件缓冲区
	PBYTE m_pbConvert;					// 转换缓冲区
	ULARGE_INTEGER m_uliSize;			// 文件大小
	TCHAR m_tzFileName[MAX_PATH];		// 文件名称
	DWORD m_dwSectorSize;				// 文件所在的磁盘的扇区大小

	UINT m_uTotalFrame;					// 总帧数

public:
	// 构造函数
	CRawVideo();

	// 析构函数
	~CRawVideo();

public:
	// 打开文件
	BOOL Open(PCTSTR ptzFileName);

	// 读取
	PBYTE Read(UINT uIndex);

	// 销毁
	VOID Destroy();

private:
	// 更新
	VOID Update();

public:
	// 转换
	inline PBYTE Convert(UINT uIndex, CRawImage &riDst)
	{
		PBYTE p = Read(uIndex);
		if (p)
		{
			p = CRawImage::Convert(riDst, m_pbConvert, p);
		}
		return p;
	}

	// 转换
	inline PBYTE Convert(UINT uIndex, COLORSPACE csDst)
	{
		PBYTE p = Read(uIndex);
		if (p)
		{
			p = CRawImage::Convert(csDst, m_pbConvert, p);
		}
		return p;
	}

	// 转换
	inline PBYTE Convert(UINT uIndex, CRawImage &riDst, PBYTE pbDst)
	{
		PBYTE p = Read(uIndex);
		if (p)
		{
			CRawImage::Convert(riDst, pbDst, p);
		}
		return p;
	}

	// 转换
	inline PBYTE Convert(UINT uIndex, COLORSPACE csDst, PBYTE pbDst)
	{
		PBYTE p = Read(uIndex);
		if (p)
		{
			p = CRawImage::Convert(csDst, pbDst, p);
		}
		return p;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
