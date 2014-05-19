


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>
#include "ChildWnd.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CExportDlg 类
class CExportDlg
{
private:
	static HWND m_hWnd;
	static HWND m_hParent;
	static CChildWnd *m_pWnd;

public:	
	static BOOL m_bOneFile;
	static DWORD m_dwFromFrame;
	static DWORD m_dwToFrame;
	static COLORSPACE m_csColorSpace;
	static TCHAR m_tzFileName[MAX_PATH];

public:
	// 导出
	static BOOL Show(CChildWnd *pWnd);

private:
	// 打开文件对话框回调函数
	static UINT_PTR CALLBACK ExportDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 初始化对话框
	static VOID OnInitDialog();

	// 通知消息
	static UINT OnNotify(WPARAM wParam, LPARAM lParam);

	// 类型改变
	static VOID OnTypeChange(LPOPENFILENAME pOfn);

	// 调整大小
	static VOID OnSize();

	// 命令处理
	static VOID OnCommand(WPARAM wParam, LPARAM lParam = 0);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
