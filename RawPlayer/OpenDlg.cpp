


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "Main.h"
#include "OpenDlg.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COpenDlg 类静态变量
HWND COpenDlg::m_hWnd = NULL;
HWND COpenDlg::m_hParent = NULL;
RECT COpenDlg::m_rtPreview = {0};
HWND COpenDlg::m_hToolbar = NULL;
CChildWnd *COpenDlg::m_pWnd = NULL;
HMENU COpenDlg::m_hFormatMenu = NULL;
BOOL COpenDlg::m_bFilterChange = FALSE;
HIMAGELIST COpenDlg::m_hImageLists[3] = {0};
RAWIMAGE COpenDlg::m_riFormat = {CS_YV12, 320, 240, 320, 80, 1, 30};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 常见视频尺寸
const TCHAR c_tzFormats[][8] = 
{
	TEXT("CIF"), TEXT("QCIF"), TEXT("SUBQCIF"),
	TEXT("SIF"), TEXT("QSIF"),
	TEXT("VGA"), TEXT("QVGA"), TEXT("QQVGA"),
	TEXT("VCDNTSC"), TEXT("VCDPAL"),
	TEXT("DVDNTSC"), TEXT("DVDPAL"),
};
const UINT c_uFormats[][2] = 
{
	{352, 288}, {176, 144}, {128, 96},
	{320, 240}, {160, 120},
	{640, 480}, {320, 240}, {160, 120},
	{352, 240}, {352, 288},
	{720, 480}, {720, 576},
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 打开文件。如果传入时 ptzFileName 包含文件名称，将尝试使用历史格式打开文件；返回时，ptzFileName 包含文件名称。
HRESULT COpenDlg::Open(PTSTR ptzFileName)
{
	UINT i;
	PTSTR p;
	PTSTR ptzEnd;
	TCHAR tzFilter[1024];

	// 创建对象
	_Assert(ptzFileName);
	m_pWnd = new CChildWnd;
	_ReturnValIf(m_pWnd == NULL, ERROR_OUTOFMEMORY);

	m_pWnd->Stop();

	// 读取默认参数
	CIni::GetStruct(INI_HistroyFormat, &m_riFormat, sizeof(RAWIMAGE));

	if (ptzFileName[0] == 0)
	{
		// 获取当前活动窗口的文件名
		CChildWnd *pWnd = CClientWnd::GetChildWnd();
		if (pWnd)
		{
			lstrcpy(ptzFileName, pWnd->m_tzFileName);
			m_riFormat = *pWnd;
		}
	}
	// 读取记录的历史格式
	else if (CIni::GetStruct(INI_HistroyFormat, ptzFileName, &m_riFormat, sizeof(RAWIMAGE)))
	{
		// 如果打开文件成功
		if (m_pWnd->Open(ptzFileName, &m_riFormat))
		{
			return ERROR_SUCCESS;
		}
		else
		{
			_SafeDelete(m_pWnd);
			return ERROR_INVALID_DATA;
		}
	}
	else
	{
		// 猜测格式
		GetSize(ptzFileName);
	}

	// 获取过滤器
	ptzEnd = &tzFilter[_NumOf(tzFilter)];
	CopyMemory(tzFilter, LNG_ExportFilter, sizeof(tzFilter));
	for (p = tzFilter, i = 0; p < ptzEnd; i++)
	{
		if (i == CS_ALL * 2)
		{
			*p = 0;
			break;
		}
		p += lstrlen(p) + 1;
	}

	// 设置 OFNEX 结构
	OFNEX ofneFileEx = {0};
	ofneFileEx.ofnFile.lStructSize = (_WinVerMajor >= 5) ? SIZE_OFNEX : SIZE_OFN;
	ofneFileEx.ofnFile.hwndOwner = CMainWnd::m_hWnd;
	ofneFileEx.ofnFile.lpstrFile = ptzFileName;
	ofneFileEx.ofnFile.nMaxFile = MAX_PATH;
	ofneFileEx.ofnFile.lpstrFilter = tzFilter;
	ofneFileEx.ofnFile.nFilterIndex = m_riFormat.m_csColorSpace + 1;
	ofneFileEx.ofnFile.lpstrDefExt = EXT_Yuv;
	ofneFileEx.ofnFile.hInstance = g_hInst;
	ofneFileEx.ofnFile.lpfnHook = (LPOFNHOOKPROC) OpenDlgProc;
	ofneFileEx.ofnFile.lpTemplateName = MAKEINTRESOURCE(IDD_Open);
	ofneFileEx.ofnFile.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY |
		OFN_ENABLEHOOK | OFN_ENABLETEMPLATE | OFN_EXPLORER;

	// 显示打开文件对话框
	if (GetOpenFileName(&ofneFileEx.ofnFile))
	{
		return ERROR_SUCCESS;
	}
	else
	{
		// 取消
		_SafeDelete(m_pWnd);
		return ERROR_CANCELLED;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 打开文件对话框回调函数
UINT_PTR CALLBACK COpenDlg::OpenDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT i;
	POINT ptPoint;

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

	case WM_PAINT:
		OnPaint();
		break;

	case WM_LBUTTONDOWN:
		if (CanPreview())
		{
			ptPoint.x = LOWORD(lParam);
			ptPoint.y = HIWORD(lParam);
			if (PtInRect(&m_rtPreview, ptPoint))
			{
				m_pWnd->StepFrame((wParam & MK_CONTROL) ? -1 : 1);
			}
		}
		break;

	case WM_NCDESTROY:
		for (i = 0; i < _NumOf(m_hImageLists); i++)
		{
			ImageList_Destroy(m_hImageLists[i]);
		}
		if (m_hFormatMenu)
		{
			DestroyMenu(m_hFormatMenu);
			m_hFormatMenu = NULL;
		}
		break;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 初始化对话框
VOID COpenDlg::OnInitDialog()
{
	UINT i;	
	const UDACCEL ACCEL_Resolution[] = {{0, 2}, {2, 20}, {4, 40}, {6, 100}};
	const TBBUTTON TBB_Button[] =
	{
		{2,		IDM_Play_Play,			TBSTATE_ENABLED,	TBSTYLE_CHECK,		0,	0},
		{3,		IDM_Play_GotoStart,		TBSTATE_ENABLED,	TBSTYLE_BUTTON,		0,	0},
		{4,		IDM_Play_StepBackward,	TBSTATE_ENABLED,	TBSTYLE_BUTTON,		0,	0},
		{5,		IDM_Play_StepForward,	TBSTATE_ENABLED,	TBSTYLE_BUTTON,		0,	0},
		{6,		IDM_Play_GotoEnd,		TBSTATE_ENABLED,	TBSTYLE_BUTTON,		0,	0},
	};

	// 设置总帧数、宽度、高度和帧率
	SetDlgItemInt(m_hWnd, IDC_Open_FrameStep, 0, FALSE);
	SetDlgItemInt(m_hWnd, IDC_Open_Width, m_riFormat.m_uWidth, FALSE);
	SetDlgItemInt(m_hWnd, IDC_Open_Height, m_riFormat.m_iHeight, TRUE);
	SetDlgItemInt(m_hWnd, IDC_Open_FrameRate, m_riFormat.m_uFrameRate, FALSE);	

	// 设置宽度、高度和帧率上下控件
	SendDlgItemMessage(m_hWnd, IDC_Open_WidthSpin, UDM_SETRANGE, 0, MAKELONG(MAX_Resolution, MIN_Resolution));
	SendDlgItemMessage(m_hWnd, IDC_Open_HeightSpin, UDM_SETRANGE, 0, MAKELONG(MAX_Resolution, MIN_Resolution));
	SendDlgItemMessage(m_hWnd, IDC_Open_FrameRateSpin, UDM_SETRANGE, 0, MAKELONG(MAX_FrameRate, 1));
	SendDlgItemMessage(m_hWnd, IDC_Open_FrameStepSpin, UDM_SETRANGE, 0, MAKELONG(0, 0));
	SendDlgItemMessage(m_hWnd, IDC_Open_WidthSpin, UDM_SETACCEL, _NumOf(ACCEL_Resolution), (LPARAM) &ACCEL_Resolution);
	SendDlgItemMessage(m_hWnd, IDC_Open_HeightSpin, UDM_SETACCEL, _NumOf(ACCEL_Resolution), (LPARAM) &ACCEL_Resolution);	

	// 设置预览复选框状态
	i = CIni::GetInt(INI_OpenPeview, TRUE);
	CheckDlgButton(m_hWnd, IDC_Open_Preview, i ? BST_CHECKED : BST_UNCHECKED);

	// 翻译对话框
	CLanguage::TranslateDialog(m_hWnd, MAKEINTRESOURCE(IDD_Open));

	// 设置播放窗口
	m_pWnd->SetPlayWnd(m_hWnd);

	// 创建工具栏
	m_hToolbar = CUtility::CreateToolbar(WS_VISIBLE | WS_CHILD | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_WRAPABLE | 
		CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER,
		m_hWnd, MAKEINTRESOURCE(IDB_Toolbar), m_hImageLists, _NumOf(TBB_Button), TBB_Button);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 通知消息
UINT COpenDlg::OnNotify(WPARAM wParam, LPARAM lParam)
{
	RECT rtRect;
	BOOL bPreview;
	RAWIMAGE riFormat;
	TCHAR tzFileName[MAX_PATH];	

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

	case CDN_SELCHANGE:
		// 获取文件名
		tzFileName[0] = 0;
		SendMessage(m_hParent, CDM_GETFILEPATH, MAX_PATH, (LPARAM) tzFileName);

		// 更改过滤器
		if (m_bFilterChange)
		{
			m_bFilterChange = FALSE;
		}
		// 设置记录的历史格式
		else if (CIni::GetStruct(INI_HistroyFormat, tzFileName, &riFormat, sizeof(RAWIMAGE)))
		{
			if (riFormat.m_csColorSpace != m_riFormat.m_csColorSpace)
			{
				CopyMemory(&m_riFormat, &riFormat, sizeof(RAWIMAGE));
				SendDlgItemMessage(m_hParent, 0x0470, CB_SETCURSEL, m_riFormat.m_csColorSpace, 0);
			}
			CopyMemory(&m_riFormat, &riFormat, sizeof(RAWIMAGE));
		}
		else
		{
			// 猜测尺寸
			GetSize(tzFileName);
		}

		// 打开文件
		m_pWnd->Open(tzFileName, &m_riFormat);

		// 设置校验过的格式		
		SetDlgItemInt(m_hWnd, IDC_Open_Width, m_riFormat.m_uWidth, FALSE);
		SetDlgItemInt(m_hWnd, IDC_Open_Height, m_riFormat.m_iHeight, TRUE);
		SetDlgItemInt(m_hWnd, IDC_Open_FrameRate, m_riFormat.m_uFrameRate, FALSE);
		SetDlgItemInt(m_hWnd, IDC_Open_FrameStep, m_riFormat.m_iFrameStep, TRUE);
		OnCommand(IDC_RawVideo_Seek);
		break;

	case CDN_FILEOK:
		// 验证格式是否正确
		SetFocus(m_hToolbar);
		if (m_pWnd->m_uTotalFrame)
		{
			m_pWnd->Stop();

			// 保存设置			
			bPreview = IsPreviewChecked();
			CIni::SetInt(INI_OpenPeview, bPreview);

			// 记录历史格式
			m_riFormat = *m_pWnd;
			CIni::SetStruct(INI_HistroyFormat, &m_riFormat, sizeof(RAWIMAGE));
			CIni::SetStruct(INI_HistroyFormat, m_pWnd->m_tzFileName, &m_riFormat, sizeof(RAWIMAGE));
			break;
		}
		else
		{
			SetWindowLongPtr(m_hWnd, DWLP_MSGRESULT, S_FALSE);
			return S_FALSE;
		}
		break;

	case TTN_NEEDTEXT:
		// 显示工具栏提示
		GetMenuString(CMainWnd::m_hMenu, (UINT) wParam, ((LPTOOLTIPTEXT) lParam)->szText, 80, MF_BYCOMMAND);
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 类型改变
VOID COpenDlg::OnTypeChange(LPOPENFILENAME pOfn)
{
	UINT i;
	PTSTR p;
	UINT uMax;	
	TCHAR tzExt[16];

	// 获取选择类型
	p = (PTSTR) pOfn->lpstrFilter;
	i = pOfn->nFilterIndex;
	_ExIf(i > 0, i--);
	m_riFormat.m_csColorSpace = (COLORSPACE) i;

	// 设置默认扩展名
	uMax = m_riFormat.m_csColorSpace * 2 + 1;		
	for (i = 0; i < uMax; i++, p += lstrlen(p) + 1);
	lstrcpyn(tzExt, p + 2, _NumOf(tzExt));
	p = _StrChr(tzExt, ';');
	_ExIf(p, *p = 0);
	SendMessage(m_hParent, CDM_SETDEFEXT, 0, (LPARAM) ((tzExt[0] != '*') ? tzExt : NULL));

	m_bFilterChange = TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 调整尺寸
VOID COpenDlg::OnSize()
{
	HWND hWnd;
	RECT rtRect;
	DWORD dwValue;

	// 调整边框
	hWnd = GetDlgItem(m_hWnd, IDC_Open_PreviewFrame);
	GetWindowRect(hWnd, &m_rtPreview);
	MapWindowPoints(NULL, m_hWnd, (PPOINT) &m_rtPreview, 2);
	GetWindowRect(GetDlgItem(m_hWnd, IDC_Open_FormatFrame), &rtRect);
	dwValue = _RectWidth(rtRect) + 32;		
	GetClientRect(m_hParent, &rtRect);
	m_rtPreview.right = m_rtPreview.left + _RectWidth(rtRect) - dwValue;
	MoveWindow(hWnd, m_rtPreview.left, m_rtPreview.top, _RectWidth(m_rtPreview), _RectHeight(m_rtPreview), TRUE);

	// 调整状态文本
	hWnd = GetDlgItem(m_hWnd, IDC_Open_StatusText);
	GetWindowRect(hWnd, &rtRect);
	MapWindowPoints(NULL, m_hWnd, (PPOINT) &rtRect, 2);
	MoveWindow(hWnd, 
		m_rtPreview.right - _RectWidth(rtRect) - 10, rtRect.top, _RectWidth(rtRect), _RectHeight(rtRect), TRUE);

	// 调整预览框位置
	m_rtPreview.top += 10;
	InflateRect(&m_rtPreview, -10, -10);
	m_rtPreview.left += 35;	

	// 调整工具栏
	MoveWindow(m_hToolbar, m_rtPreview.left - 33, m_rtPreview.top + 3, 25, _RectHeight(m_rtPreview) - 6, TRUE);

	OnCommand(IDC_RawVideo_Change);

	// 刷新预览
	InvalidateRect(m_hWnd, &m_rtPreview, TRUE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 命令处理
VOID COpenDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	INT i;
	RECT rtRect;
	BOOL bPreview;
	PUINT puValue;
	const UINT c_uFormats[][2] = 
	{
		{352, 288}, {176, 144}, {128, 96},
		{320, 240}, {160, 120},
		{640, 480}, {320, 240}, {160, 120},
		{352, 240}, {352, 288},
		{720, 480}, {720, 576},
	};

	_ExIf((HWND) lParam == m_hToolbar, SetFocus(m_hToolbar));
	switch (LOWORD(wParam))
	{
	case IDC_Open_MulSignText:
		if (m_hFormatMenu == NULL)
		{
			m_hFormatMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_Menu_Format));
		}
		GetWindowRect(GetDlgItem(m_hWnd, IDC_Open_MulSignText), &rtRect);
		TrackPopupMenu(GetSubMenu(m_hFormatMenu, 0), TPM_TOPALIGN, rtRect.left, rtRect.top, 0, m_hWnd, NULL);
		break;

	case IDM_Format_CIF:
	case IDM_Format_QCIF:
	case IDM_Format_SUBQCIF:
	case IDM_Format_SIF:
	case IDM_Format_QSIF:
	case IDM_Format_VGA:
	case IDM_Format_QVGA:
	case IDM_Format_QQVGA:
	case IDM_Format_VCDNTSC:
	case IDM_Format_VCDPAL:
	case IDM_Format_DVDNTSC:
	case IDM_Format_DVDPAL:
		i = LOWORD(wParam) - IDM_Format_CIF;
		SetDlgItemInt(m_hWnd, IDC_Open_Width, c_uFormats[i][0], FALSE);
		SetDlgItemInt(m_hWnd, IDC_Open_Height, c_uFormats[i][1], TRUE);
		OnCommand(MAKELONG(IDC_Open_Width, EN_KILLFOCUS));
		OnCommand(MAKELONG(IDC_Open_Height, EN_KILLFOCUS));
		break;

	case IDC_Open_Preview:
		// 切换预览
		if (HIWORD(wParam) == BN_CLICKED)
		{
			IsPreviewChecked() ? InvalidateRect(m_hWnd, &m_rtPreview, TRUE) : m_pWnd->Stop();
			OnCommand(IDC_RawVideo_Seek);
		}
		break;

	case IDC_Open_Width:
	case IDC_Open_Height:
	case IDC_Open_FrameRate:
	case IDC_Open_FrameStep:
		if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			switch (LOWORD(wParam))
			{
			case IDC_Open_Width:
				puValue = &m_riFormat.m_uWidth;
				break;

			case IDC_Open_Height:
				puValue = (PUINT) &m_riFormat.m_iHeight;
				break;

			case IDC_Open_FrameRate:
				puValue = &m_riFormat.m_uFrameRate;
				break;

			case IDC_Open_FrameStep:
				puValue = (PUINT) &m_riFormat.m_iFrameStep;
				break;
			}

			// 限制尺寸范围
			*puValue = GetDlgItemInt(m_hWnd, LOWORD(wParam), NULL, TRUE);
			m_pWnd->Open(m_pWnd->m_tzFileName, &m_riFormat);
			SetDlgItemInt(m_hWnd, LOWORD(wParam), *puValue, TRUE);
		}
		break;

	case IDM_Play_Play:
		m_pWnd->TogglePlay();
		break;

	case IDM_Play_GotoStart:
		m_pWnd->Stop();
		m_pWnd->Seek(0);
		break;

	case IDM_Play_GotoEnd:
		m_pWnd->Stop();
		m_pWnd->Seek(MAX_Frame);
		break;

	case IDM_Play_StepBackward:
		m_pWnd->Stop();
		m_pWnd->Step(-1);
		break;

	case IDM_Play_StepForward:
		m_pWnd->Stop();
		m_pWnd->Step(1);
		break;

	case IDC_RawVideo_Play:
		SendMessage(m_hToolbar, TB_CHECKBUTTON, IDM_Play_Play, lParam);
		break;

	case IDC_RawVideo_Change:
		bPreview = CanPreview();
		for (i = IDM_Play_Play; i <= IDM_Play_GotoEnd; i++)
		{
			SendMessage(m_hToolbar, TB_ENABLEBUTTON, i, MAKELONG(bPreview, 0));
		}

		i = m_pWnd->m_uTotalFrame ? (m_pWnd->m_uTotalFrame - 1) : 0;
		SendDlgItemMessage(m_hWnd, IDC_Open_FrameStepSpin, UDM_SETRANGE, 0, MAKELONG(i, -i));		
		InvalidateRect(GetDlgItem(m_hWnd, IDC_Open_FrameStepSpin), NULL, FALSE);

		// 计算位置和尺寸
		CopyRect(&rtRect, &m_rtPreview);
		InflateRect(&rtRect, -5, -5);
		i = MulDiv(m_pWnd->m_uWidth, _RectHeight(m_rtPreview), m_pWnd->m_iHeight);
		if (i > _RectWidth(rtRect))
		{
			i = MulDiv(m_pWnd->m_iHeight, _RectWidth(rtRect), m_pWnd->m_uWidth);
			if (_RectHeight(rtRect) > i)
			{
				rtRect.top += (_RectHeight(rtRect) - i) / 2;
				rtRect.bottom = rtRect.top + i;
			}
		}
		else
		{
			rtRect.left += (_RectWidth(rtRect) - i) / 2;
			rtRect.right = rtRect.left + i;
		}
		m_pWnd->SetDrawRect(&rtRect);

		SetDlgItemInt(m_hWnd, IDC_Open_TotalFrame, m_pWnd->m_uTotalFrame, FALSE);
		InvalidateRect(m_hWnd, &m_rtPreview, TRUE);
		break;

	case IDC_RawVideo_Seek:
		if (CanPreview())
		{
			SetDlgItemInt(m_hWnd, IDC_Open_StatusText, m_pWnd->m_uCurFrame + 1, FALSE);
		}
		else
		{
			SetDlgItemText(m_hWnd, IDC_Open_StatusText, NULL);
		}
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 绘制
VOID COpenDlg::OnPaint()
{
	HDC hDC;
	HPEN hPen;
	BOOL bPreview;
	HBRUSH hBrush;
	PAINTSTRUCT psPaint;

	// 绘制预览区域
	hDC = BeginPaint(m_hWnd, &psPaint);

	// 判断是否预览图像
	bPreview = IsPreviewChecked();

	// 绘制预览边框
	hPen = (HPEN) SelectObject(hDC, CreatePen(PS_SOLID, 1, RGB(160, 160, 160)));	
	hBrush = (HBRUSH) SelectObject(hDC, bPreview ? GetStockObject(WHITE_BRUSH) : GetSysColorBrush(COLOR_3DFACE));
	RoundRect(hDC, m_rtPreview.left, m_rtPreview.top, m_rtPreview.right, m_rtPreview.bottom, 8, 8);	
	SelectObject(hDC, hBrush);
	hBrush = (HBRUSH) SelectObject(hDC, GetSysColorBrush(COLOR_3DFACE));
	RoundRect(hDC, m_rtPreview.left - 36, m_rtPreview.top, m_rtPreview.left - 7, m_rtPreview.bottom, 8, 8);
	DeleteObject(SelectObject(hDC, hPen));
	SelectObject(hDC, hBrush);

	// 绘制图像
	if (bPreview && m_pWnd->m_uTotalFrame)
	{
		m_pWnd->Draw(hDC);
	}

	// 结束绘制
	EndPaint(m_hWnd, &psPaint);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 根据文件名获取视频大小
BOOL COpenDlg::GetSize(PCTSTR ptzFileName)
{
	INT i;
	PTSTR p;
	TCHAR tzFileName[MAX_PATH];

	// 获取文件名称
	lstrcpy(tzFileName, ptzFileName);
	CharUpper(tzFileName);
	p = _StrRChr(tzFileName, '\\');
	_ExIfElse(p == NULL, return(FALSE), p++);

	// 根据格式字符串确定大小
	for (i = _NumOf(c_tzFormats) - 1; i >= 0; i--)
	{
		if (_StrStr(p, c_tzFormats[i]))
		{
			m_riFormat.m_uWidth = c_uFormats[i][0];
			m_riFormat.m_iHeight = c_uFormats[i][1];
			return TRUE;
		}
	}

	// 根据数字确定大小
	for (; *p; p++)
	{
		if ((*p >= '0') && (*p <= '9'))
		{
			for (i = _StrToInt(p); (*p >= '0') && (*p <= '9'); p++);
			if (p[0] && (p[1] >= '0') && (p[1] <= '9'))
			{
				m_riFormat.m_uWidth = i;
				m_riFormat.m_iHeight = _StrToInt(p + 1);
				return TRUE;
			}
		}
	}

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

