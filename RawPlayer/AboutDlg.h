


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAboutDlg 类
class CAboutDlg
{
private:
	// 对话框模板
	typedef struct tagDIALOGINFO
	{
		DLGTEMPLATE dtDialog;							// 对话框
		WORD wNoMenu;									// 没有对话框菜单
		WORD wNoClass;									// 没有对话框类名
		WORD wNoCaption;								// 没有对话框标题
		WORD wEnd[5];									// 对话框模板结束
	}
	DIALOGINFO;

	// 水平线组
	typedef struct tagLINEINFO
	{
		RECT rtRect;									// 位置
		COLORREF crColor;								// 颜色
	}
	LINEINFO;

	// 矩形
	typedef struct tagRECTINFO
	{
		RECT rtRect;									// 位置
		COLORREF crColor;								// 颜色
	}
	RECTINFO;

	// 文本
	typedef struct tagTEXTINFO
	{
		INT x;											// X 坐标
		INT y;											// Y 坐标
		TCHAR tzText[64];								// 文本
		COLORREF crColor;								// 颜色
	}
	TEXTINFO;

	// 指定字体的文本
	typedef struct tagFONTTEXTINFO
	{
		INT x;											// X 坐标
		INT y;											// Y 坐标
		PCTSTR ptzText;									// 文本
		COLORREF crColor;								// 颜色
		LOGFONT lfFont;									// 字体
	}
	FONTTEXTINFO;

	// 超链接
	typedef struct tagLINKINFO
	{
		RECT rtRect;									// 位置
		TCHAR tzTitle[32];								// 标题
		TCHAR tzTip[64];								// 提示字符串
		PCTSTR ptzUrl;									// 超链接目标 (HIWORD(ptzUrl) 为 0 表示命令消息)
	}
	LINKINFO;

public:
	static HWND m_hWnd;									// 窗口句柄

private:
	static BOOL m_bSplash;								// 是否以闪屏方式显示
	static INT m_iLinkIndex;							// 当前超链接

	static DIALOGINFO m_diDialog;						// 对话框模板
	static LINEINFO m_liLines[];						// 水平线组
	static RECTINFO m_riRects[];						// 矩形
	static TEXTINFO m_tiTexts[];						// 文本
	static FONTTEXTINFO m_ftiTexts[];					// 指定字体的文本
	static LINKINFO m_liLinks[];						// 超链接

public:
	// 显示关于对话框
	static VOID Show(HWND hParent = NULL);

	// 以闪屏方式显示关于对话框
	static VOID Splash(UINT uTimer = 1, HWND hParent = NULL);

	// 关闭窗口关于对话框
	static VOID Close(UINT uTimer = 0);

	// 设置状态信息
	static VOID SetStatusText(PCTSTR ptzText = NULL);

private:
	// 关于对话框回调函数
	static INT_PTR CALLBACK AboutDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 绘制对话框
	static VOID OnPaint(HDC hDC);

	// 命令处理
	static VOID OnCommand(WPARAM wParam, LPARAM lParam = 0);

	// 获取指定点的超链接
	static INT GetLinkIndex(POINT ptPoint);

	// 绘制超链接
	static VOID PaintLinks(HDC hDC = NULL);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
