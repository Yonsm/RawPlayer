


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "Main.h"
#include "ExportDlg.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CExportDlg 类静态变量
HWND CExportDlg::m_hWnd = NULL;
HWND CExportDlg::m_hParent = NULL;
CChildWnd *CExportDlg::m_pWnd = NULL;
BOOL CExportDlg::m_bOneFile = FALSE;
DWORD CExportDlg::m_dwFromFrame = 0;
DWORD CExportDlg::m_dwToFrame = 0;
TCHAR CExportDlg::m_tzFileName[MAX_PATH] ={0};
COLORSPACE CExportDlg::m_csColorSpace = CS_YV12;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出
BOOL CExportDlg::Show(CChildWnd *pWnd)
{
	UINT i;
	PTSTR p;
	OFNEX ofneFileEx;
	TCHAR tzFilter[1024];

	// 检查
	_Assert(pWnd);
	m_pWnd = pWnd;

	// 获取文件名
	lstrcpy(m_tzFileName, m_pWnd->m_tzFileName);
	p = _StrRChr(m_tzFileName, '.');
	_ExIf(p, *p = 0);

	// 获取导出类型
	i = CIni::GetInt(INI_ExportFormat, CS_BMP);
	_ExIf(i >= CS_MAX, i = CS_BMP);

	// 设置 OFNEX 结构	
	_ZeroMem(ofneFileEx);
	CopyMemory(tzFilter, LNG_ExportFilter, sizeof(tzFilter));
	ofneFileEx.ofnFile.lStructSize = (_WinVerMajor >= 5) ? SIZE_OFNEX : SIZE_OFN;
	ofneFileEx.ofnFile.hwndOwner = CMainWnd::m_hWnd;
	ofneFileEx.ofnFile.lpstrFile = m_tzFileName;
	ofneFileEx.ofnFile.nMaxFile = MAX_PATH;
	ofneFileEx.ofnFile.lpstrFilter = tzFilter;
	ofneFileEx.ofnFile.nFilterIndex = i + 1;
	ofneFileEx.ofnFile.lpstrDefExt = EXT_Bmp;
	ofneFileEx.ofnFile.hInstance = g_hInst;
	ofneFileEx.ofnFile.lpfnHook = (LPOFNHOOKPROC) ExportDlgProc;
	ofneFileEx.ofnFile.lpTemplateName = MAKEINTRESOURCE(IDD_Export);
	ofneFileEx.ofnFile.Flags = OFN_ENABLESIZING | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY |
		OFN_ENABLEHOOK | OFN_ENABLETEMPLATE | OFN_EXPLORER;

	// 显示打开文件对话框
	return GetSaveFileName(&ofneFileEx.ofnFile);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 打开文件对话框回调函数
UINT_PTR CALLBACK CExportDlg::ExportDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		m_hWnd = hWnd;
		m_hParent = GetParent(m_hWnd);
		OnInitDialog();
		break;

	case WM_NOTIFY:
		return OnNotify(wParam, lParam);

	case WM_SIZE:
		OnSize();
		break;

	case WM_COMMAND:
		OnCommand(wParam, lParam);
		break;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 初始化对话框
VOID CExportDlg::OnInitDialog()
{
	// 翻译对话框
	CLanguage::TranslateDialog(m_hWnd, MAKEINTRESOURCE(IDD_Export));

	OnCommand(IDC_Export_AllFrames);
	UINT i = CIni::GetInt(INI_ExportRange, IDC_Export_CurrentFrame);
	i = _Bound(i, IDC_Export_CurrentFrame, IDC_Export_FrameRange);
	CheckRadioButton(m_hWnd, IDC_Export_CurrentFrame, IDC_Export_FrameRange, i);
	OnCommand(i);

	SendDlgItemMessage(m_hWnd, IDC_Export_FromSpin, UDM_SETRANGE, 0, MAKELONG(m_pWnd->m_uTotalFrame, 1));
	SendDlgItemMessage(m_hWnd, IDC_Export_ToSpin, UDM_SETRANGE, 0, MAKELONG(m_pWnd->m_uTotalFrame, 1));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 通知消息
UINT CExportDlg::OnNotify(WPARAM wParam, LPARAM lParam)
{
	UINT i;
	RECT rtRect;

	switch (((LPNMHDR) lParam)->code)
	{
	case CDN_INITDONE:
		// 调整父窗口位置，使其居中与屏幕中心
		GetWindowRect(m_hParent, &rtRect);
		MoveWindow(m_hParent, 
			(GetSystemMetrics(SM_CXSCREEN) - _RectWidth(rtRect) - 20) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - _RectHeight(rtRect)) / 2,
			_RectWidth(rtRect), _RectHeight(rtRect), TRUE);

		// 重新调整窗口大小
		GetClientRect(m_hParent, &rtRect);
		MoveWindow(m_hWnd, 0, 0, _RectWidth(rtRect), _RectHeight(rtRect), TRUE);
		//break;

	case CDN_TYPECHANGE:
		OnTypeChange(((LPOFNOTIFY) lParam)->lpOFN);
		break;

	case CDN_FILEOK:
		m_csColorSpace = (COLORSPACE) (((LPOFNOTIFY) lParam)->lpOFN->nFilterIndex - 1);
		CIni::SetInt(INI_ExportFormat, m_csColorSpace);

		m_bOneFile = (IsDlgButtonChecked(m_hWnd, IDC_Export_OneFile) == BST_CHECKED);
		m_dwFromFrame = GetDlgItemInt(m_hWnd, IDC_Export_From, NULL, FALSE);
		m_dwToFrame = GetDlgItemInt(m_hWnd, IDC_Export_To, NULL, FALSE);
		m_dwFromFrame--;
		m_dwToFrame--;
		if (m_dwFromFrame > m_dwToFrame)
		{
			i = m_dwToFrame;
			m_dwToFrame = m_dwFromFrame;
			m_dwFromFrame = i;
		}

		for (i = IDC_Export_CurrentFrame; i <= IDC_Export_FrameRange; i++)
		{
			if (SendDlgItemMessage(m_hWnd, i, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				CIni::SetInt(INI_ExportRange, i);
				break;
			}
		}
		break;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 类型改变
VOID CExportDlg::OnTypeChange(LPOPENFILENAME pOfn)
{
	UINT i;
	PTSTR p;
	UINT uMax;	
	UINT uIndex;
	TCHAR tzExt[16];

	// 获取选择类型
	p = (PTSTR) pOfn->lpstrFilter;
	uIndex = pOfn->nFilterIndex;
	_ReturnIf(uIndex == 0);
	uIndex--;

	// 设置默认扩展名
	uMax = uIndex * 2 + 1;		
	for (i = 0; i < uMax; i++, p += lstrlen(p) + 1);
	lstrcpyn(tzExt, p + 2, _NumOf(tzExt));
	p = _StrChr(tzExt, ';');
	_ExIf(p, *p = 0);
	SendMessage(m_hParent, CDM_SETDEFEXT, 0, (LPARAM) ((tzExt[0] != '*') ? tzExt : NULL));

	// 设置是否多文件导出
	CheckRadioButton(m_hWnd, IDC_Export_MultiFiles, IDC_Export_OneFile, 
		(uIndex == CS_BMP) ? IDC_Export_MultiFiles : IDC_Export_OneFile);
	EnableWindow(GetDlgItem(m_hWnd, IDC_Export_OneFile), (uIndex != CS_BMP));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 调整尺寸
VOID CExportDlg::OnSize()
{
	HWND hWnd;
	RECT rtRect;
	RECT rtRange;
	DWORD dwValue;

	// 调整边框
	hWnd = GetDlgItem(m_hWnd, IDC_Export_FormatFrame);
	GetWindowRect(hWnd, &rtRange);
	MapWindowPoints(NULL, m_hWnd, (PPOINT) &rtRange, 2);
	GetWindowRect(GetDlgItem(m_hWnd, IDC_Export_RangeFrame), &rtRect);
	dwValue = _RectWidth(rtRect) + 32;		
	GetClientRect(m_hParent, &rtRect);
	rtRange.right = rtRange.left + _RectWidth(rtRect) - dwValue;
	MoveWindow(hWnd, rtRange.left, rtRange.top, _RectWidth(rtRange), _RectHeight(rtRange), TRUE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 命令处理
VOID CExportDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_Export_CurrentFrame:
	case IDC_Export_AllFrames:
		if (LOWORD(wParam) == IDC_Export_CurrentFrame)
		{
			m_dwFromFrame = 1 + m_pWnd->m_uCurFrame;
			m_dwToFrame = m_dwFromFrame;
		}
		else
		{
			m_dwFromFrame = 1;
			m_dwToFrame = m_pWnd->m_uTotalFrame;
		}
		SetDlgItemInt(m_hWnd, IDC_Export_From, m_dwFromFrame, FALSE);
		SetDlgItemInt(m_hWnd, IDC_Export_To, m_dwToFrame, FALSE);
		break;

	case IDC_Export_FrameRange:
		break;

	case IDC_Export_From:
	case IDC_Export_To:
		if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			m_dwFromFrame = GetDlgItemInt(m_hWnd, LOWORD(wParam), NULL, FALSE);
			if (m_dwFromFrame == 0)
			{
				m_dwFromFrame = 1;
			}
			else if (m_dwFromFrame > m_pWnd->m_uTotalFrame)
			{
				m_dwFromFrame = m_pWnd->m_uTotalFrame;
			}
			else
			{
				return;
			}
			SetDlgItemInt(m_hWnd, LOWORD(wParam), m_dwFromFrame, FALSE);
		}

	default :
		return;
	}

	EnableWindow(GetDlgItem(m_hWnd, IDC_Export_From), LOWORD(wParam) == IDC_Export_FrameRange);
	EnableWindow(GetDlgItem(m_hWnd, IDC_Export_To), LOWORD(wParam) == IDC_Export_FrameRange);
	InvalidateRect(GetDlgItem(m_hWnd, IDC_Export_FromSpin), NULL, FALSE);
	InvalidateRect(GetDlgItem(m_hWnd, IDC_Export_ToSpin), NULL, FALSE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
