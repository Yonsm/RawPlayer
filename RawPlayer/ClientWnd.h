


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>
#include "ChildWnd.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CClientWnd 类
class CClientWnd
{
public:
	static HWND m_hWnd;						// 客户窗口句柄
	static HMENU m_hPopup;					// 弹出菜单句柄

private:
	static UINT m_uChildNum;				// 子窗口数目
	static BOOL m_bScrabble;				// 是否处于拼凑状态

public:
	// 创建客户窗口
	static VOID Create();

	// 设置窗口边框
	static VOID SetEdge(BOOL bEdge = TRUE);

	// 载入文件
	static BOOL Load(CChildWnd *pWnd);

	// 缩放所有窗口
	static VOID OnPlayZoom(UINT uCmd = IDM_Play_ZoomActual);

	// 切换工具栏和状态栏显示
	static VOID ToggleBar();

	// 关闭所有子窗口
	static VOID CloseAll();

	// 使主窗口播放等命令有效或无效
	static VOID EnableCommand(BOOL bEnable = TRUE);

	// 子窗口关闭
	static VOID OnChildOpenClose(BOOL bOpen = FALSE);

	// 拼接窗口
	static VOID OnPlayScrabble();

	// 拼接窗口
	static VOID Scrabble();

public:
	// 获取信息
	inline static UINT GetChildCount() {return m_uChildNum;}
	inline static BOOL IsScrabble() {return m_bScrabble;}
	inline static BOOL HasEdge() {return (GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_CLIENTEDGE);}

	// 获取子窗口信息
	inline static HWND GetActive() {return (HWND) SendMessage(m_hWnd, WM_MDIGETACTIVE, 0, 0);}
	inline static CChildWnd *GetChildWnd(HWND hWnd = GetActive()) {return CChildWnd::GetWnd(hWnd);}
	inline static VOID SetStatusText(PCTSTR ptzText, INT iIndex = 0, HWND hWnd = GetActive()) {CChildWnd::SetStatusText(hWnd, ptzText, iIndex);}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
