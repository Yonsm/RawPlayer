


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "Main.h"
#include "ClientWnd.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ClientWnd 类静态变量
HWND CClientWnd::m_hWnd = NULL;
HMENU CClientWnd::m_hPopup = NULL;
UINT CClientWnd::m_uChildNum = 0;
BOOL CClientWnd::m_bScrabble = FALSE;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 创建客户窗口
VOID CClientWnd::Create()
{
	WNDCLASSEX wceClass;
	CLIENTCREATESTRUCT ccsClient;	

	// 创建客户窗口
	m_hPopup = GetSubMenu(CMainWnd::m_hMenu, 1);
	ccsClient.hWindowMenu = GetSubMenu(CMainWnd::m_hMenu, 3);
	ccsClient.idFirstChild = IDM_Window_First;
	m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("MDICLIENT"), NULL, WS_VISIBLE | WS_CHILD, 
		0, 0, 0, 0, CMainWnd::m_hWnd, NULL, g_hInst, &ccsClient);

	// 注册子窗口类
	wceClass.cbSize = sizeof(WNDCLASSEX);
	wceClass.style = CS_VREDRAW | CS_HREDRAW;
	wceClass.lpfnWndProc = CChildWnd::ClildWndProc;
	wceClass.cbClsExtra = 0;
	wceClass.cbWndExtra = 0;
	wceClass.hInstance = g_hInst;
	wceClass.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_File));
	wceClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wceClass.hbrBackground = (HBRUSH) COLOR_BTNSHADOW;
	wceClass.lpszMenuName = NULL;
	wceClass.lpszClassName = STR_AppName TEXT("ChildWnd");
	wceClass.hIconSm = NULL;
	RegisterClassEx(&wceClass);

	// 注册播放窗口类
	wceClass.lpfnWndProc = CChildWnd::PlayWndProc;
	wceClass.hbrBackground = NULL;//(HBRUSH) COLOR_WINDOWTEXT;
	wceClass.lpszClassName = STR_AppName TEXT("PlayWnd");
	RegisterClassEx(&wceClass);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 设置窗口边框
VOID CClientWnd::SetEdge(BOOL bEdge)
{
	DWORD dwExStyle;

	dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if (bEdge)
	{
		_ReturnIf(dwExStyle & WS_EX_CLIENTEDGE);
		dwExStyle |= WS_EX_CLIENTEDGE;
	}
	else
	{
		_ReturnIf((dwExStyle & WS_EX_CLIENTEDGE) == FALSE);
		dwExStyle &= ~WS_EX_CLIENTEDGE;
	}
	SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle);
	SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
		SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 载入文件
BOOL CClientWnd::Load(CChildWnd *pWnd)
{
	HWND hWnd;
	TCHAR tzText[MAX_PATH];

	// 检查文件是否已载入
	_Assert(pWnd);
	hWnd = GetWindow(m_hWnd, GW_CHILD);
	for (hWnd = GetWindow(hWnd, GW_HWNDFIRST); hWnd; hWnd = GetWindow(hWnd, GW_HWNDNEXT))
	{
		// 如果文件已载入
		GetWindowText(hWnd, tzText, MAX_PATH);
		if (lstrcmpi(tzText, pWnd->m_tzFileName) == 0)
		{
			CChildWnd *p = GetChildWnd(hWnd);
			if (p)
			{
				// 如果格式相同
				if (*pWnd == *p)
				{
					delete pWnd;
					SendMessage(m_hWnd, WM_MDIACTIVATE, (WPARAM) hWnd, 0);
					return TRUE;
				}
			}

			// 关闭已打开的文件，然后重新用新的格式创建窗口
			SendMessage(m_hWnd, WM_MDIDESTROY, (WPARAM) hWnd, 0);
			break;
		}
	}

	// 创建子窗口
	if (pWnd->Create())
	{
		OnChildOpenClose(TRUE);
		return TRUE;
	}

	delete pWnd;

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 缩放所有窗口
VOID CClientWnd::OnPlayZoom(UINT uCmd)
{
	RECT rtMain;
	RECT rtClient;
	RECT rtChild;

	SendMessage(m_hWnd, WM_MDICASCADE, 0, 0);
	HWND hWnd = GetWindow(m_hWnd, GW_CHILD);
	for (hWnd = GetWindow(hWnd, GW_HWNDFIRST); hWnd; )
	{
		_Assert(GetChildWnd(hWnd));
		GetChildWnd(hWnd)->OnPlayZoom(uCmd, FALSE);
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}

	if (m_bScrabble)
	{
		GetWindowRect(CMainWnd::m_hWnd, &rtMain);
		GetClientRect(m_hWnd, &rtClient);
		hWnd = GetWindow(GetWindow(m_hWnd, GW_CHILD), GW_HWNDFIRST);
		GetWindowRect(hWnd, &rtChild);

		MoveWindow(CMainWnd::m_hWnd, rtMain.left, rtMain.top, 
			_RectWidth(rtMain) - _RectWidth(rtClient) + _RectWidth(rtChild),
			_RectHeight(rtMain) - _RectHeight(rtClient) + _RectHeight(rtChild), TRUE);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 切换工具栏和状态栏显示
VOID CClientWnd::ToggleBar()
{
	HWND hWnd;

	hWnd = GetWindow(m_hWnd, GW_CHILD);
	for (hWnd = GetWindow(hWnd, GW_HWNDFIRST); hWnd; )
	{
		_Assert(GetChildWnd(hWnd));
		GetChildWnd(hWnd)->OnSize();
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 关闭所有子窗口
VOID CClientWnd::CloseAll()
{
	HWND hWnd;
	HWND hDestroy;

	hWnd = GetWindow(m_hWnd, GW_CHILD);
	for (hWnd = GetWindow(hWnd, GW_HWNDFIRST); hWnd; )
	{
		hDestroy = hWnd;
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
		SendMessage(m_hWnd, WM_MDIDESTROY, (WPARAM) hDestroy, 0);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 使主窗口播放等命令有效或无效
VOID CClientWnd::EnableCommand(BOOL bEnable)
{
	UINT i;

	CMainWnd::EnableCommand(IDM_File_Export, bEnable);
	CMainWnd::EnableCommand(IDM_View_Toolbar, bEnable);
	CMainWnd::EnableCommand(IDM_View_StatusBar, bEnable);
	for (i = IDM_Play_Play; i <= IDM_Play_ZoomDouble; i++)
	{
		CMainWnd::EnableCommand(i, bEnable);
	}
	for (i = IDM_Window_Cascade; i <= IDM_Window_CloseAll; i++)
	{
		CMainWnd::EnableCommand(i, bEnable);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
VOID CClientWnd::OnChildOpenClose(BOOL bOpen)
{
	_ExIf(m_bScrabble, OnPlayScrabble());

	if (bOpen)
	{
		m_uChildNum++;
		if (m_uChildNum == 1)
		{
			EnableCommand();
			CMainWnd::EnableCommand(IDM_Play_Synchronize, FALSE);
			CMainWnd::EnableCommand(IDM_Play_Scrabble, FALSE);
		}
		else if (m_uChildNum == 2)
		{
			CMainWnd::EnableCommand(IDM_Play_Synchronize, TRUE);
			CMainWnd::EnableCommand(IDM_Play_Scrabble, TRUE);
		}
	}
	else
	{
		m_uChildNum--;
		if (m_uChildNum <= 1)
		{
			if (m_uChildNum == 0)
			{
				// 保存子窗口最大化状态
				BOOL bMaximized = (HasEdge() == FALSE);
				CIni::SetInt(INI_MaxOnOpen, bMaximized);
				_ExIf(bMaximized, SetEdge());

				EnableCommand(FALSE);
			}
			CMainWnd::EnableCommand(IDM_Play_Synchronize, FALSE);
			CMainWnd::EnableCommand(IDM_Play_Scrabble, FALSE);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 拼接窗口
VOID CClientWnd::OnPlayScrabble()
{
	// 设置菜单状态
	m_bScrabble = !CMainWnd::IsMenuChecked(IDM_Play_Scrabble);
	CMainWnd::CheckCommand(IDM_Play_Scrabble, m_bScrabble);

	// 同步
	if (CRawPlayer::m_bSync == FALSE)
	{
		SendMessage(CMainWnd::m_hWnd, WM_COMMAND, IDM_Play_Synchronize, 0);
	}

	// 设置主窗口
	if (m_bScrabble)
	{
		OnPlayZoom();
	}

	Scrabble();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 拼接窗口
VOID CClientWnd::Scrabble()
{
	UINT i;
	RECT rtClient;
	DWORD dwWidth;
	DWORD dwStyle;
	CChildWnd *pWnd;

	_ReturnIf(m_uChildNum == 0);

	// 获取客户矩形
	GetClientRect(m_hWnd, &rtClient);
	dwWidth = _RectWidth(rtClient) / m_uChildNum;
	for (pWnd = CChildWnd::m_pHead, i = 0; pWnd; pWnd = pWnd->m_pNext, i++)
	{
		// 设置子窗口
		dwStyle = GetWindowLong(pWnd->m_hWnd, GWL_STYLE);
		if (m_bScrabble)
		{
			dwStyle &= ~WS_CAPTION;
			dwStyle &= ~WS_THICKFRAME;
		}
		else
		{
			dwStyle |= WS_CAPTION | WS_THICKFRAME;
		}
		SetWindowLong(pWnd->m_hWnd, GWL_STYLE, dwStyle);
		if (m_bScrabble == FALSE)
		{
			SetWindowPos(pWnd->m_hWnd, NULL, 0, 0, 0, 0, 
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
		}

		// 设置播放窗口
		dwStyle = GetWindowLong(pWnd->m_hPlayWnd, GWL_EXSTYLE);
		if (m_bScrabble)
		{
			dwStyle &= ~WS_EX_CLIENTEDGE;
		}
		else
		{
			dwStyle |= WS_EX_CLIENTEDGE;
		}
		SetWindowLong(pWnd->m_hPlayWnd, GWL_EXSTYLE, dwStyle);
		if (m_bScrabble == FALSE)
		{
			SetWindowPos(pWnd->m_hPlayWnd, NULL, 0, 0, 0, 0, 
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
		}

		if (m_bScrabble)
		{
			// 移动子窗口
			MoveWindow(pWnd->m_hWnd, dwWidth * i, 0, dwWidth, _RectHeight(rtClient), TRUE);
		}

		if (m_bScrabble)
		{
			RECT rtRect;
			rtRect.left = pWnd->m_uWidth * i / m_uChildNum;
			rtRect.right = rtRect.left + (pWnd->m_uWidth / m_uChildNum);
			rtRect.top = 0;
			rtRect.bottom = pWnd->m_uHeight;
			pWnd->SetSrcRect(&rtRect);
		}
		else
		{
			pWnd->SetSrcRect();
		}
	}

	if (m_bScrabble == FALSE)
	{
		SendMessage(m_hWnd, WM_MDICASCADE, 0, 0);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
