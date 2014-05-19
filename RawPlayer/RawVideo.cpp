


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "Main.h"
#include "RawVideo.h"

#define _NOBUFFERING
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 构造函数
CRawVideo::CRawVideo()
{
	m_hFile = NULL;
	m_uliSize.QuadPart = 0;
	m_pbRead = NULL;
	m_pbConvert = NULL;
	m_tzFileName[0] = NULL;

	m_uFrameSize = 0;
	m_uTotalFrame = 0;
	m_dwSectorSize = 512;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 析构函数
CRawVideo::~CRawVideo()
{
	Destroy();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 打开文件
BOOL CRawVideo::Open(PCTSTR ptzFileName)
{
	// 销毁
	Destroy();

	// 打开文件
#ifdef _NOBUFFERING
	m_hFile = CreateFile(ptzFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
#else
	m_hFile = CreateFile(ptzFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
#endif
	_ReturnValIf(m_hFile == INVALID_HANDLE_VALUE, FALSE);

	// 获取文件大小
	m_uliSize.LowPart = ::GetFileSize(m_hFile, &m_uliSize.HighPart);
	if ((m_uliSize.LowPart == INVALID_FILE_SIZE) && (GetLastError() != NO_ERROR))
	{
		m_uliSize.QuadPart = 0;
		_SafeCloseValidHandle(m_hFile);
		return FALSE;
	}

	if (m_uliSize.QuadPart == 0)
	{
		return FALSE;
	}

	// 打开成功
	lstrcpy(m_tzFileName, ptzFileName);
	Update();

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 销毁数据
VOID CRawVideo::Destroy()
{
	// 释放内存
	_SafeAlignFree(m_pbRead);
	_SafeAlignFree(m_pbConvert);

	// 关闭文件
	_SafeCloseValidHandle(m_hFile);
	m_uliSize.QuadPart = 0;
	//m_tzFileName[0] = 0;

	// 重置参数
	/*CRawImage::Reset();
	m_uFrameSize = 0;
	m_uTotalFrame = 0;*/
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
VOID CRawVideo::Update()
{
	// 更新
	CRawImage::Update();

	// 分配内存
	_SafeAlignFree(m_pbRead);
	_SafeAlignFree(m_pbConvert);

#ifdef _NOBUFFERING
	m_dwSectorSize = CUtility::GetSectorSize(m_tzFileName);
	m_pbRead = (PBYTE) _AlignAlloc(m_uFrameSize + m_dwSectorSize * 2, m_dwSectorSize);
#else
	m_pbRead = (PBYTE) _AlignAlloc(m_uFrameSize, 16);
#endif

	DWORD dwConvert = (max(160 * 120 * 4, (m_uWidth + 4) * (m_uHeight + 4) * 4));
	m_pbConvert = (PBYTE) _AlignAlloc(dwConvert, 16);

	_Assert(m_pbRead && m_pbConvert);

	// 计算总帧数
	m_uTotalFrame = m_uFrameSize ? ((DWORD) (m_uliSize.QuadPart / m_uFrameSize)) : 0;

	// 设置帧幅
	if (m_uTotalFrame <= 1)
	{
		m_iFrameStep = 0;
	}
	else if (m_iFrameStep == 0)
	{
		m_iFrameStep = 1;
	}
	else if (m_iFrameStep >= (INT) m_uTotalFrame)
	{
		m_iFrameStep = m_uTotalFrame - 1;
	}
	else if (m_iFrameStep <= -((INT) m_uTotalFrame))
	{
		m_iFrameStep = 1 - (INT) m_uTotalFrame;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 读取
PBYTE CRawVideo::Read(UINT uIndex)
{
	DWORD dwSize;
	ULARGE_INTEGER uliPos;

	// 读取数据
	_ReturnValIf(uIndex >= m_uTotalFrame, NULL);

	uliPos.QuadPart = uIndex * m_uFrameSize;

#ifdef _NOBUFFERING

	ULARGE_INTEGER uliAglinPos;

	uliAglinPos.QuadPart = (uliPos.QuadPart / m_dwSectorSize) * m_dwSectorSize;
	DWORD dwRead = (2 + (m_uFrameSize / m_dwSectorSize)) * m_dwSectorSize;

	SetFilePointer(m_hFile, uliAglinPos.LowPart, (PLONG) &uliAglinPos.HighPart, FILE_BEGIN);
	return ReadFile(m_hFile, m_pbRead, dwRead, &dwSize, NULL) ? 
		(m_pbRead + (DWORD) (uliPos.QuadPart - uliAglinPos.QuadPart)) : NULL;

#else

	SetFilePointer(m_hFile, uliPos.LowPart, (PLONG) &uliPos.HighPart, FILE_BEGIN);
	return ReadFile(m_hFile, m_pbRead, m_uFrameSize, &dwSize, NULL) ? m_pbRead : NULL;

#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
