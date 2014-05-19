


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "Main.h"
#include "AboutDlg.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define IDC_CheckUpdate				1226										// 检查更新命令标识
#define VAL_AboutDlg_Width			400											// 对话框宽度
#define VAL_AboutDlg_Height			275											// 对话框高度
#define IDT_AboutDlg_Close			1534										// 关闭时钟标识
#define COLOR_AboutDlg_Link			0x00FF0000									// 超链接颜色
#define COLOR_AboutDlg_HotLink		0x000000FF									// 鼠标热点超链接颜色
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAboutDlg 类静态变量
CAboutDlg::DIALOGINFO CAboutDlg::m_diDialog =
{
	{DS_CENTER | WS_POPUP | WS_BORDER | WS_VISIBLE, 0, 0, 0, 0,
		(VAL_AboutDlg_Width * 2 / 3), (VAL_AboutDlg_Height * 2 / 3)},			// 对话框模板
};

CAboutDlg::LINEINFO CAboutDlg::m_liLines[] =
{
	{{0, 1, 88, 74}, 0x00FF9966},												// 蓝线组
	{{60, 116, VAL_AboutDlg_Width, 132}, 0x00FF9966},							// 蓝线组
	{{60, 175, VAL_AboutDlg_Width, 176}, 0x00000000},							// 一根黑线
};

CAboutDlg::RECTINFO CAboutDlg::m_riRects[] =
{
	{{0, 75, VAL_AboutDlg_Width, 111}, 0x00FF9966},								// 蓝色矩形
	{{0, 111, VAL_AboutDlg_Width, 115}, 0x00CC3333},							// 深蓝色矩形
	{{60, 47, 116, 103}, 0x00CC3333},											// 图标外框深蓝色矩形
	{{64, 51, 112, 99}, 0x00FFFFFF},											// 图标外框白色矩形
	{{68, 55, 86, 73}, RGB(0xEE, 0xEE, 0xEE + VER_Major % 16)},					// 图标背景淡灰色矩形
	{{90, 55, 108, 73}, RGB(0xEE, 0xEE, 0xEE + VER_Minor % 16)},				// 图标背景淡灰色矩形
	{{68, 79, 86, 95}, RGB(0xEE, 0xEE, 0xEE + VER_Release % 16)},				// 图标背景淡灰色矩形
	{{90, 79, 108, 95}, RGB(0xEE, 0xEE, 0xEE + VER_Build % 16)},				// 图标背景淡灰色矩形
};

CAboutDlg::TEXTINFO CAboutDlg::m_tiTexts[] =
{
	{60, 162, TEXT(""), 0x00000000},											// 状态信息
	{60, 140, TEXT(""), 0x00BBBBBB},											// 版本信息
	{230, 140, TEXT(""), 0x00BBBBBB},											// 编译信息
	{60, 240, TEXT(""), 0x00000000},											// 描述信息
	{60, 255, TEXT(""), 0x00000000},											// 版权信息
	{125, 95, TEXT(""), 0x00FFFFFF},

#ifdef _BETA
	{270, 40, TEXT("BETA"), 0x00AAAAAA},										// 测试版本
#endif
};

CAboutDlg::FONTTEXTINFO CAboutDlg::m_ftiTexts[] =
{

	{125, 32, STR_Corporation, 0x00FFAAAA, {14, 0, 0, 0, 400, 1,
	0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, TEXT("Arial")}},						// 公司名称

	{125, 40, STR_AppName, 0x00000000, {36, 0, 0, 0, 800, 1,
		0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, TEXT("Arial")}},					// 程序名称
};

CAboutDlg::LINKINFO CAboutDlg::m_liLinks[] =
{
	{{60, 185, 0, 185 + 13}, TEXT(""), TEXT(""), (PCTSTR) IDC_CheckUpdate},		// 检查更新
	{{60, 200, 0, 200 + 13}, TEXT(""), TEXT(""), STR_WebUrl},					// 访问网站
	{{60, 215, 0, 215 + 13}, TEXT(""), TEXT(""), STR_EmailUrl},					// 写信给作者
};

HWND CAboutDlg::m_hWnd = NULL;
BOOL CAboutDlg::m_bSplash = FALSE;
INT CAboutDlg::m_iLinkIndex = -1;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 显示关于对话框
VOID CAboutDlg::Show(HWND hParent)
{
	if (m_hWnd == NULL)
	{
		DialogBoxIndirect(g_hInst, (LPDLGTEMPLATE) &m_diDialog, hParent, (DLGPROC) AboutDlgProc);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 以闪屏方式显示关于对话框
VOID CAboutDlg::Splash(UINT uTimer, HWND hParent)
{
	if (uTimer && (m_hWnd == NULL))
	{
		// 创建无模式对话框
		CreateDialogIndirect(g_hInst, (LPDLGTEMPLATE) &m_diDialog, hParent, (DLGPROC) AboutDlgProc);

		// 如果指定了自动关闭
		if (uTimer != -1)
		{
			SetTimer(m_hWnd, IDT_AboutDlg_Close, uTimer * 1000, NULL);
		}

		// 设置当前窗口以闪屏方式显示的标记
		m_bSplash = TRUE;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 关闭窗口关于对话框
VOID CAboutDlg::Close(UINT uTimer)
{
	// 如果是立即关闭
	if (uTimer)
	{
		SetTimer(m_hWnd, IDT_AboutDlg_Close, uTimer, NULL);
	}
	else
	{
		// 设置 BitLive 通知窗口
		CBitLive::SetNotify(CMainWnd::m_hWnd);

		// 删除定时器
		KillTimer(m_hWnd, IDT_AboutDlg_Close);

		// 如果当前窗口以闪屏方式显示
		if (m_bSplash)
		{
			DestroyWindow(m_hWnd);
			m_bSplash = FALSE;
		}
		else
		{
			EndDialog(m_hWnd, 0);
		}

		m_hWnd = NULL;
		m_tiTexts[0].tzText[0] = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 设置状态信息
VOID CAboutDlg::SetStatusText(PCTSTR ptzText)
{
	HDC hMem;
	RECT rtRect;

	// 状态信息矩形
	rtRect.left = m_tiTexts[0].x;
	rtRect.top = m_tiTexts[0].y;
	rtRect.right = VAL_AboutDlg_Width;
	rtRect.bottom = rtRect.top + _RectHeight(m_liLinks[0].rtRect);

	// 获取状态信息
	if (ptzText)
	{
		_StrToStr(m_tiTexts[0].tzText, ptzText);
	}
	else
	{
		m_tiTexts[0].tzText[0] = 0;
	}

	// 绘制状态信息
	hMem = GetDC(m_hWnd);
	SetTextColor(hMem, m_tiTexts[0].crColor);
	FillRect(hMem, &rtRect, (HBRUSH) GetStockObject(WHITE_BRUSH));
	SelectObject(hMem, (HFONT) GetStockObject(DEFAULT_GUI_FONT));
	TextOut(hMem, m_tiTexts[0].x, m_tiTexts[0].y, m_tiTexts[0].tzText, lstrlen(m_tiTexts[0].tzText));
	ReleaseDC(m_hWnd, hMem);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 关于对话框回调函数
INT_PTR CALLBACK CAboutDlg::AboutDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT i;
	POINT ptPoint;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		// 初始化变量
		m_hWnd = hWnd;
		m_iLinkIndex = NULL;

		// 设置鼠标指针
		SetClassLongPtr(m_hWnd, GCLP_HCURSOR, 0);
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		// 设置对话框风格
		if (GetParent(m_hWnd) == NULL)
		{
			SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, (GetWindowLongPtr(m_hWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW));
		}
		SetWindowPos(m_hWnd, GetParent(m_hWnd) ? 0 : HWND_TOPMOST, 0, 0, VAL_AboutDlg_Width, VAL_AboutDlg_Height, SWP_NOMOVE);

		// 获取语言相关字符串
		wsprintf(m_tiTexts[1].tzText, LNG_Version, STR_VersionStamp);
		wsprintf(m_tiTexts[2].tzText, LNG_Build, STR_BuilStamp);
		_StrToStr(m_tiTexts[3].tzText, LNG_Description);
		_StrToStr(m_tiTexts[4].tzText, LNG_Copyright);
		_StrToStr(m_liLinks[0].tzTitle, LNG_CheckUpdate);

		_StrToStr(m_liLinks[1].tzTitle, LNG_VisitWeb);
		_StrToStr(m_liLinks[2].tzTitle, LNG_WriteEmail);
		_StrToStr(m_liLinks[0].tzTip, LNG_CheckUpdateTip);
		_StrToStr(m_liLinks[1].tzTip, LNG_VisitWebTip);
		_StrToStr(m_liLinks[2].tzTip, LNG_WriteEmailTip);

		// 获取 CPU 特性
		CRawImage::GetCpuFeature(m_tiTexts[5].tzText);

		break;

	case WM_ERASEBKGND:
		OnPaint((HDC) wParam);
		return TRUE;

	case WM_ACTIVATEAPP:
		// 激活程序
		GetCursorPos(&ptPoint);
		MapWindowPoints(NULL, m_hWnd, &ptPoint, 1);
		SendMessage(m_hWnd, WM_MOUSEMOVE, 0, MAKELONG(ptPoint.x, ptPoint.y));
		break;

	case WM_MOUSEMOVE:
		// 如果鼠标所在位置的超链接改变
		ptPoint.x = LOWORD(lParam);
		ptPoint.y = HIWORD(lParam);
		i = GetLinkIndex(ptPoint);
		if (i != m_iLinkIndex)
		{
			m_iLinkIndex = i;
			PaintLinks();
		}
		break;

	case WM_LBUTTONDOWN:
		// 如果在图标框内，拖动对话框
		ptPoint.x = LOWORD(lParam);
		ptPoint.y = HIWORD(lParam);
		if (PtInRect(&m_riRects[2].rtRect, ptPoint))
		{
			ReleaseCapture();
			PostMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}
		break;

	case WM_LBUTTONUP:
		// 如果鼠标在超链接内
		if (m_iLinkIndex != -1)
		{
			// 如果 HIWORD(ptzUrl) 为 0，表示命令，否则表示超链接
			if (HIWORD(m_liLinks[m_iLinkIndex].ptzUrl))
			{
				ShellExecute(NULL, NULL, m_liLinks[m_iLinkIndex].ptzUrl, NULL, NULL, SW_NORMAL);
			}
			else
			{
				PostMessage(m_hWnd, WM_COMMAND, (WPARAM) m_liLinks[m_iLinkIndex].ptzUrl, 0);
			}
		}
		else
		{
			// 关闭对话框
			Close();
		}
		break;

	case WM_COMMAND:
		// 命令消息
		OnCommand(wParam, lParam);
		break;

	case WM_TIMER:
	case WM_RBUTTONUP:
		// 关闭对话框
		Close();
		break;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 绘制对话框
VOID CAboutDlg::OnPaint(HDC hDC)
{
	INT i;
	INT j;
	HDC hMem;
	HPEN hPen;
	HFONT hFont;
	HBRUSH hBrush;
	POINT ptPoint;
	HBITMAP hBitmap;
	static RECT s_rtRect = {0, 0, VAL_AboutDlg_Width, VAL_AboutDlg_Height};

	// 开始绘制
	hMem = CreateCompatibleDC(hDC);
	hBitmap = (HBITMAP) SelectObject(hMem, CreateCompatibleBitmap(hDC, VAL_AboutDlg_Width, VAL_AboutDlg_Height));
	FillRect(hMem, &s_rtRect, (HBRUSH) GetStockObject(WHITE_BRUSH));

	// 绘制水平线组
	for (i = 0; i < _NumOf(m_liLines); i++)
	{
		hPen = (HPEN) SelectObject(hMem, CreatePen(PS_SOLID, 0, m_liLines[i].crColor));
		for (j = m_liLines[i].rtRect.top; j < m_liLines[i].rtRect.bottom; j += 2)
		{
			MoveToEx(hMem, m_liLines[i].rtRect.left, j, NULL);
			LineTo(hMem, m_liLines[i].rtRect.right, j);
		}
		DeleteObject(SelectObject(hMem, hPen));
	}

	// 绘制矩形区域
	for (i = 0; i < _NumOf(m_riRects); i++)
	{
		hBrush = CreateSolidBrush(m_riRects[i].crColor);
		FillRect(hMem, &m_riRects[i].rtRect, hBrush);
		DeleteObject(hBrush);
	}

	// 绘制图标
	DrawIcon(hMem,
		m_riRects[2].rtRect.left + (_RectWidth(m_riRects[2].rtRect) - 32) / 2,
		m_riRects[2].rtRect.top + (_RectHeight(m_riRects[2].rtRect) - 32) / 2,
		LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_Main)));

	// 绘制超链接
	GetCursorPos(&ptPoint);
	MapWindowPoints(NULL, m_hWnd, &ptPoint, 1);
	m_iLinkIndex = GetLinkIndex(ptPoint);
	PaintLinks(hMem);

	// 使用对话框默认字体、透明背景绘制文本
	SetBkMode(hMem, TRANSPARENT);
	SelectObject(hMem, (HFONT) GetStockObject(DEFAULT_GUI_FONT));
	for (i = 0; i < _NumOf(m_tiTexts); i++)
	{
		SetTextColor(hMem, m_tiTexts[i].crColor);
		TextOut(hMem, m_tiTexts[i].x, m_tiTexts[i].y, m_tiTexts[i].tzText, lstrlen(m_tiTexts[i].tzText));
	}

	// 绘制指定字体的文本
	for (i = 0; i < _NumOf(m_ftiTexts); i++)
	{
		hFont = (HFONT) SelectObject(hMem, CreateFontIndirect(&m_ftiTexts[i].lfFont));
		SetTextColor(hMem, m_ftiTexts[i].crColor);
		TextOut(hMem, m_ftiTexts[i].x, m_ftiTexts[i].y, m_ftiTexts[i].ptzText, lstrlen(m_ftiTexts[i].ptzText));
		DeleteObject(SelectObject(hMem, hFont));
	}

	// 结束绘制
	BitBlt(hDC, 0, 0, VAL_AboutDlg_Width, VAL_AboutDlg_Height, hMem, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(hMem, hBitmap));
	DeleteDC(hMem);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 命令处理
VOID CAboutDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDOK:
	case IDCANCEL:
		// 关闭对话框
		Close();
		break;

	case IDC_CheckUpdate:
		// 检查更新
		CBitLive::Live(m_hWnd);
		break;

	case IDC_BitLive:
		// 响应更新消息
		switch (HIWORD(wParam))
		{
		case BLL_Check:
		case BLL_CheckErr:
		case BLL_News:
		case BLL_Update:
		case BLL_Progress:
		case BLL_UpdateEnd:
			SetStatusText((PTSTR) lParam);
			break;

		case BLL_UpToDate:
			SetStatusText((PTSTR) lParam);
			CBitLive::Cancel();
			break;

		case BLL_NewsBox:
			MessageBox(m_hWnd, (PTSTR) lParam, INI_BitLive, MB_ICONINFORMATION);
			break;

		case BLL_UpdateErr:
			MessageBox(m_hWnd, (PTSTR) lParam, INI_BitLive, MB_ICONSTOP);
			break;

		case BLL_Query:
		case BLL_Confirm:
			if (MessageBox(m_hWnd, (PTSTR) lParam, INI_BitLive, MB_ICONQUESTION | MB_YESNO) == IDNO)
			{
				CBitLive::Cancel();
			}
			break;

		case BLL_Exit:
			Close();
			PostQuitMessage(0);
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 获取指定点的超链接
INT CAboutDlg::GetLinkIndex(POINT ptPoint)
{
	INT i;

	for (i = 0; i < _NumOf(m_liLinks); i++)
	{
		if (PtInRect(&m_liLinks[i].rtRect, ptPoint))
		{
			return i;
		}
	}

	return PtInRect(&m_riRects[2].rtRect, ptPoint) ? -2 : -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 绘制超链接
VOID CAboutDlg::PaintLinks(HDC hMem)
{
	INT i;
	BOOL bGetDC;
	HFONT hFont;
	LOGFONT lfFont;
	PTSTR ptzCursor;

	if (hMem == NULL)
	{
		hMem = GetDC(m_hWnd);
		bGetDC = TRUE;
	}
	else
	{
		bGetDC = FALSE;
	}

	GetObject((HFONT) GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lfFont);
	for (i = 0; i < _NumOf(m_liLinks); i++)
	{
		lfFont.lfUnderline = (i == m_iLinkIndex);
		hFont = (HFONT) SelectObject(hMem, CreateFontIndirect(&lfFont));
		SetTextColor(hMem, lfFont.lfUnderline ? COLOR_AboutDlg_HotLink : COLOR_AboutDlg_Link);

		// 计算矩形框，绘制文本
		m_liLinks[i].rtRect.bottom = m_liLinks[i].rtRect.top +
			DrawText(hMem, m_liLinks[i].tzTitle, -1, &m_liLinks[i].rtRect,
			DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);
		DrawText(hMem, m_liLinks[i].tzTitle, -1, &m_liLinks[i].rtRect, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);

		DeleteObject(SelectObject(hMem, hFont));
	}

	if ((m_iLinkIndex >= 0))
	{
		SetStatusText(m_liLinks[m_iLinkIndex].tzTip);

#if (_WINVER >= 0x0401)
		ptzCursor = IDC_HAND;
#else
		ptzCursor = _WinVerAboveEqual(4, 1) ? IDC_HAND : IDC_CROSS;
#endif
	}
	else
	{
		SetStatusText();
		ptzCursor = (m_iLinkIndex == -2) ? IDC_SIZEALL : IDC_ARROW;
	}

	_ExIf(bGetDC, ReleaseDC(m_hWnd, hMem));

	SetCursor(LoadCursor(NULL, ptzCursor));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
