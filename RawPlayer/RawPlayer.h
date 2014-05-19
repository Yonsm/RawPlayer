


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <DDraw.h>
#include <Windows.h>
#include "RawVideo.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define IDC_RawVideo_Change		11344
#define IDC_RawVideo_Play		11345
#define IDC_RawVideo_Seek		11346
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 去除通道
#define CH_WIPEY				0x00000001
#define CH_WIPEU				0x00000010
#define CH_WIPEV				0x00000100
#define CH_WIPER				0x00010000
#define CH_WIPEG				0x00100000
#define CH_WIPEB				0x01000000

#define NeedWipeYUV(d)			(LOWORD(d))
#define NeedWipeRGB(d)			(HIWORD(d))
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRawPlayer 类
class CRawPlayer: public CRawVideo
{
public:
	static BOOL m_bSync;				// 是否同步播放
	static BOOL m_bPlaying;				// 是否正在同步播放	

	static CRawPlayer *m_pHead;			// 最后创建的对象

	CRawPlayer *m_pNext;				// 循环链表中下一个对象的指针

	HWND m_hPlayWnd;					// 播放窗口	
	DWORD m_uCurFrame;					// 当前帧索引
	DWORD m_dwWipeChannel;				// 去除通道
	MMRESULT m_mrTimer;					// 播放定时器
	CRawImage m_riDst;					// 目标格式

	BOOL m_bDrawing;					// 正在绘制

	RECT m_rtSrc;						// 绘制源矩形
	RECT m_rtDraw;						// 绘制矩形
	RECT m_rtDirectDraw;				// 绘制矩形

	LPDIRECTDRAW m_pDDraw;				// DirectDraw 指针
	DDSURFACEDESC m_sdSurface;			// DirectDraw 表面描述
	LPDIRECTDRAWSURFACE m_pSurface;		// DirectDraw 表面指针
	LPDIRECTDRAWCLIPPER m_pClipper;		// DirectDraw 剪裁器指针
	LPDIRECTDRAWSURFACE m_pPrimary;		// DirectDraw 主表面指针

	HRESULT (CRawPlayer::*m_pDraw)(HDC hDC);	// 绘制函数指针

public:
	// 构造函数
	CRawPlayer();

	// 析构函数
	~CRawPlayer();

public:
	// 打开
	BOOL Open(PCTSTR ptzFileName, PRAWIMAGE priFormat = NULL);

	// 销毁
	VOID Destroy();

	// 保存为位图文件
	BOOL SaveBitmap(UINT uIndex, PCTSTR ptzFileName);

private:
	// 用 GDI 绘制
	HRESULT Paint(HDC hDC = NULL);

	// 初始化 DirectDraw
	HRESULT DDInit();

	// 用 DirectDraw 渲染
	HRESULT Render(HDC hDC = NULL);

	// 释放 DirectDraw
	HRESULT DDRelease();

	// 检查差异
	HRESULT CheckDiff(PBYTE pbData);

public:
	// 使用 DirectDraw 方式
	HRESULT EnableDDraw(BOOL bEnable = TRUE, BOOL bStop = TRUE);

public:
	// 开始播放
	BOOL Play();

	// 停止播放
	BOOL Stop();

	// 定位
	VOID Seek(INT iIndex = 0);

	// 跳步
	VOID Step(INT iMultiple = 1);

	// 帧跳
	VOID StepFrame(INT iFrame = 1);

	// 设置当前帧
	DWORD SetCurFrame(INT iIndex);

private:
	// 播放过程
	static VOID CALLBACK PlayProc(UINT uID, UINT uMsg, CRawPlayer *pThis, DWORD dwParam1, DWORD dwParam2);

	// 单步播放过程
	static VOID CALLBACK SyncPlayProc(UINT uID, UINT uMsg, CRawPlayer *pThis, DWORD dwParam1, DWORD dwParam2);

public:
	// 绘制
	inline VOID Draw(HDC hDC = NULL) {if (m_bDrawing == FALSE) {m_bDrawing = TRUE; (this->*m_pDraw)(hDC); m_bDrawing = FALSE;}}
	inline BOOL IsDDraw() {return m_pDraw == &CRawPlayer::Render;}

	// 播放
	inline BOOL IsPlaying() {return m_bSync ? m_bPlaying : m_mrTimer;}
	inline BOOL TogglePlay() {return IsPlaying() ? Stop() : Play();}
	inline VOID Restart() {SetCurFrame((m_iFrameStep < 0) ? MAX_Frame : 0);}

	// 播放设置
	inline VOID SetPlayWnd(HWND hWnd) {m_hPlayWnd = hWnd; SetDrawRect();}
	inline VOID SetSrcRect(LPCRECT prtRect = NULL) {prtRect ? CopyRect(&m_rtSrc, prtRect) : SetRect(&m_rtSrc, 0, 0, m_uWidth, m_uHeight);}
	inline VOID SetDrawRect(LPCRECT prtRect = NULL) {prtRect ? CopyRect(&m_rtDraw, prtRect) : GetClientRect(m_hPlayWnd, &m_rtDraw); CopyRect(&m_rtDirectDraw, &m_rtDraw); MapWindowPoints(m_hPlayWnd, NULL, (PPOINT) &m_rtDirectDraw, 2);}
	
	// 去除通道
	inline DWORD GetWipeChannel() {return m_dwWipeChannel;}
	inline VOID SetWipeChannel(DWORD dwMask, BOOL bWipe) {bWipe ? m_dwWipeChannel |= dwMask : m_dwWipeChannel &= ~dwMask; m_dwWipeChannel ? EnableDDraw(FALSE) : EnableDDraw(); Draw();}

	// 同步播放
	inline static BOOL SetSync(BOOL bSync) {BOOL bPrev = m_bSync; m_bSync = TRUE; if (m_pHead) m_pHead->Stop(); m_bSync = bSync; return bPrev;}
	
	inline static BOOL ToggleSync()	
	{
		BOOL bPrev = m_bSync;
		m_bSync = TRUE;
		if (m_pHead) m_pHead->Stop();
		m_bSync = !bPrev;

		for (CRawPlayer *p = m_pHead; p; p = p->m_pNext)
		{
			p->SetCurFrame(p->m_uCurFrame + 0);
			p->Draw();
		}
		return m_bSync;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
