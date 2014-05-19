


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>
#include "ClientWnd.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 打开文件对话框结构
typedef struct tagOFNEX
{
	OPENFILENAME ofnFile;
	PVOID pvReserved;
	DWORD dwReserved;
	DWORD FlagEx;
}
OFNEX, *POFNEX;

#define SIZE_OFN		CDSIZEOF_STRUCT(OPENFILENAME, lpTemplateName)
#define SIZE_OFNEX		((sizeof(OPENFILENAME) <= SIZE_OFN) ? sizeof(OFNEX) : sizeof(OPENFILENAME))
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COpenDlg 类
class COpenDlg
{
private:
	static HWND m_hWnd;
	static HWND m_hParent;
	static HWND m_hToolbar;
	static RECT m_rtPreview;
	static RAWIMAGE m_riFormat;
	static HMENU m_hFormatMenu;
	static BOOL m_bFilterChange;	
	static HIMAGELIST m_hImageLists[3];

public:
	static CChildWnd *m_pWnd;

public:
	// 打开文件
	static HRESULT Open(PTSTR ptzFileName);

private:
	// 打开文件对话框回调函数
	static UINT_PTR CALLBACK OpenDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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

	// 绘制
	static VOID OnPaint();

	// 根据文件名获取视频大小
	static BOOL GetSize(PCTSTR ptzFileName);

private:
	// 是否选择要预览
	inline static BOOL IsPreviewChecked() {return IsDlgButtonChecked(m_hWnd, IDC_Open_Preview) == BST_CHECKED;}

	// 是否可以预览
	inline static BOOL CanPreview() {return m_pWnd->m_uTotalFrame && IsPreviewChecked();}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
