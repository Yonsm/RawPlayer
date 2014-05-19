


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>
#include "RawPlayer.h"

#pragma warning(disable: 4312)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CChildWnd 类
class CChildWnd: public CRawPlayer
{
public:
	HWND m_hWnd;						// 客户窗口句柄
	HWND m_hSeekBar;					// 定位栏窗口句柄
	HWND m_hPlayWnd;					// 播放窗口句柄

	CChildWnd *m_pNext;					// 循环链表中下一个对象的指针
	static CChildWnd *m_pHead;			// 最后创建的对象

private:
	UINT m_uEditBox;					// 编辑窗口位置
	HWND m_hEditBox;					// 编辑窗口句柄
	HWND m_hToolbar;					// 工具栏窗口句柄	
	HWND m_hStatusBar;					// 状态栏窗口句柄	
	HIMAGELIST m_hImageLists[3];		// 工具栏图像列表句柄	

	BOOL m_bCancel;						// 用户取消导出操作
	BOOL m_bExporting;					// 正在导出

	BOOL m_bPrevPlaying;				// 拖动定位条之前是否播放
	BOOL m_bStartOfScroll;				// 是否开始拖动定位条

public:
	// 构造函数
	CChildWnd();

	// 析构函数
	~CChildWnd();

	// 创建窗口
	HWND Create();

public:
	// 子窗口回调函数
	static LRESULT CALLBACK ClildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 播放窗口回调函数
	static LRESULT CALLBACK PlayWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 工具栏窗口回调函数
	static LRESULT CALLBACK ToolbarWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 编辑窗口回调函数
	static LRESULT CALLBACK EditBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	// 窗口被创建
	VOID OnCreate();

	// 大小改变
	VOID OnSize(WPARAM wParam = SIZE_RESTORED, LPARAM lParam = 0);

	// 通知消息
	static VOID OnNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 点击状态栏
	VOID OnClickStatusBar(UINT uIndex);

	// 命令处理
	VOID OnCommand(WPARAM wParam, LPARAM lParam = 0);

	// 导出
	VOID OnFileExport();

	// 弹出通道菜单
	VOID OnMenuPopup();

	// 通道
	VOID OnPlayChannel(UINT uCmd);

	// 缩放
	VOID OnPlayZoom(UINT uCmd, BOOL bCascade = TRUE);

	// 设置格式
	BOOL OnChangeFormat();

	// 创建工具栏
	VOID CreateToolbar();

	// 设置工具栏图片
	HIMAGELIST SetToolbarBitmap(UINT uMsg, HBITMAP hBitmap);

	// 调整状态栏大小及位置
	VOID AdjustStatusBar(WPARAM wParam = SIZE_RESTORED, LPARAM lParam = 0);

	// 设置播放窗口尺寸
	VOID SetVideoSize(DWORD dwWidth, DWORD dwHeight);

private:
	// 导出回调函数
	BOOL ExportProc();
	static DWORD vExportProc(CChildWnd *pThis);

public:
	inline BOOL SetStatusText(PCTSTR ptzText, INT iIndex = 0) {return (BOOL) SendMessage(m_hStatusBar, SB_SETTEXT, iIndex ? iIndex : SBT_NOBORDERS, (LPARAM) ptzText);}
	inline BOOL SetStatusText(INT iValue = 0, INT iIndex = 0) {TCHAR tzText[32]; wsprintf(tzText, TEXT("%d"), iValue); return SetStatusText(tzText, iIndex);}

public:
	inline static CChildWnd *GetWnd(HWND hWnd)
	{
		return (CChildWnd *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	inline static VOID SetStatusText(HWND hWnd, PCTSTR ptzText, INT iIndex = 0)
	{
		SendDlgItemMessage(hWnd, IDC_StatusBar, SB_SETTEXT, iIndex ? iIndex : SBT_NOBORDERS, (LPARAM) ptzText);
	}

	inline BOOL operator==(CChildWnd &cwWnd)
	{
		return (m_csColorSpace == cwWnd.m_csColorSpace) &&
			(m_uWidth == cwWnd.m_uWidth) &&
			(m_iHeight == cwWnd.m_iHeight) &&
			(m_iFrameStep == cwWnd.m_iFrameStep) &&
			(m_uFrameRate == cwWnd.m_uFrameRate);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
