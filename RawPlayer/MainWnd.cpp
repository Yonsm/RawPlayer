


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "Main.h"
#include "MainWnd.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMainWnd 类静态变量
HWND CMainWnd::m_hWnd = NULL;
HMENU CMainWnd::m_hMenu = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 创建主窗口
HWND CMainWnd::Create()
{
	WNDCLASSEX wceClass;

	// 注册主窗口类
	wceClass.cbSize = sizeof(WNDCLASSEX);
	wceClass.style = CS_VREDRAW | CS_HREDRAW;
	wceClass.lpfnWndProc = MainWndProc;
	wceClass.cbClsExtra = 0;
	wceClass.cbWndExtra = 0;
	wceClass.hInstance = g_hInst;
	wceClass.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_Main));
	wceClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wceClass.hbrBackground = (HBRUSH) COLOR_BTNSHADOW;
	wceClass.lpszMenuName = MAKEINTRESOURCE(IDR_Menu);
	wceClass.lpszClassName = STR_AppName;
	wceClass.hIconSm = NULL;
	RegisterClassEx(&wceClass);

	// 创建主窗口
	return CreateWindowEx(WS_EX_ACCEPTFILES | WS_EX_APPWINDOW, STR_AppName, STR_AppName, WS_OVERLAPPEDWINDOW,
		CIni::GetInt(INI_WindowLeft, CW_USEDEFAULT),
		CIni::GetInt(INI_WindowTop, CW_USEDEFAULT),
		CIni::GetInt(INI_WindowWidth, CW_USEDEFAULT),
		CIni::GetInt(INI_WindowHeight, CW_USEDEFAULT),
		NULL, NULL, g_hInst, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 显示主窗口，并建立消息循环，等待运行完成
VOID CMainWnd::Show(PCTSTR ptzCmdLine, INT iCmdShow)
{
	MSG msgMsg;
	HACCEL hAccel;

	// 显示主窗口
	if (iCmdShow <= SW_NORMAL)
	{
		iCmdShow = _Bound(CIni::GetInt(INI_WindowState), SW_NORMAL, SW_MAX);
	}
	ShowWindow(m_hWnd, iCmdShow);

	// 载入菜单加速键
	hAccel = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDR_Accel));

	// 处理命令行
	if (ptzCmdLine && ptzCmdLine[0])
	{
		OnFileOpen(ptzCmdLine);
	}
	else if (CIni::GetInt(INI_ShowOpen))
	{
		PostMessage(m_hWnd, WM_COMMAND, IDM_File_Open, 0);
	}

	// 消息循环
	while (GetMessage(&msgMsg, NULL, 0, 0))
	{
		if ((TranslateMDISysAccel(CClientWnd::m_hWnd, &msgMsg) == 0) &&
			(TranslateAccelerator(m_hWnd, hAccel, &msgMsg) == 0))
		{
			TranslateMessage(&msgMsg);
			DispatchMessage(&msgMsg);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 回调函数
LRESULT CALLBACK CMainWnd::MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CChildWnd *pWnd;
	TCHAR tzTemp[MAX_PATH];

	switch (uMsg)
	{
	case WM_CREATE:
		// 窗口被创建
		m_hWnd = hWnd;
		m_hMenu = GetMenu(m_hWnd);
		OnCreate();
		break;

	case WM_SIZE:
		// 尺寸改变
		OnSize(wParam, lParam);
		return 0;

	case WM_WINDOWPOSCHANGED:
		// 位置改变
		for (pWnd = CChildWnd::m_pHead; pWnd; pWnd = pWnd->m_pNext)
		{
			if (pWnd->m_hWnd)
			{
				pWnd->SetDrawRect();
			}
		}
		break;

	case WM_INITMENUPOPUP:
		// 弹出菜单初始化
		OnMenuPopup(wParam, lParam);
		break;

	case WM_MENUSELECT:
		// 菜单项被选择
		OnMenuSelect(LOWORD(wParam));
		break;

	case WM_EXITMENULOOP:
		// 退出菜单循环
		CClientWnd::SetStatusText(LNG_Ready);
		break;

	case WM_COMMAND:
		// 命令处理
		OnCommand(wParam, lParam);
		break;

	case WM_COPYDATA:
		// 本程序的其它实例通知打开文件
		if (((PCOPYDATASTRUCT) lParam)->dwData == IDM_File_Open)
		{
			static TCHAR s_tzFileName[MAX_PATH];

			lstrcpyn(s_tzFileName, (PTSTR) ((PCOPYDATASTRUCT) lParam)->lpData, MAX_PATH);
			PostMessage(m_hWnd, WM_COMMAND, IDM_File_Open, (LPARAM) s_tzFileName);
		}
		break;

	case WM_HOTKEY:
		// 热键消息
		if ((m_hWnd != GetForegroundWindow()) && (wParam == IDC_TrayIcon))
		{
			OnTrayIcon(FALSE);
		}
		break;

	case WM_DROPFILES:
		// 拖动文件
		DragQueryFile((HDROP) wParam, 0, tzTemp, MAX_PATH);
		DragFinish((HDROP) wParam);
		OnFileOpen(tzTemp);
		break;

	case WM_HELP:
		// 帮助
		OnCommand(IDM_Help_Content);
		break;

	case WM_DESTROY:
		// 窗口被销毁
		OnDestroy();
		break;
	}

	return DefFrameProc(hWnd, CClientWnd::m_hWnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 窗口被创建
VOID CMainWnd::OnCreate()
{
#ifdef _MAKELANG
	OnViewLanguage(IDM_View_Default);
#else
	// 列出语言
	if (CLanguage::List(GetSubMenu(GetSubMenu(m_hMenu, 2), 12)) != IDM_View_Default)
	{
		// 翻译菜单
		CLanguage::TranslateMenu(m_hMenu, MAKEINTRESOURCE(IDR_Menu));
	}
#endif

	// 创建
	CClientWnd::Create();

	// 设置菜单项状态
	OnViewMenu();

	// 注册热键
	RegisterHotKey(m_hWnd, IDC_TrayIcon, MOD_WIN, 'V');

	// 读取新近文件列表
	CRecentFile::Load(GetSubMenu(m_hMenu, 0), 4);

	// 设置同步状态
	//CRawVideo::SetSync(CIni::GetInt(INI_Synchronize));
	//CheckCommand(IDM_Play_Synchronize, CRawPlayer::m_bSync);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 大小改变
VOID CMainWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	if (wParam == SIZE_MINIMIZED)
	{
		if (IsMenuChecked(IDM_View_MinToTray))
		{
			// 添加系统托盘图标，隐藏主窗口
			OnTrayIcon();
		}
	}
	else
	{
		MoveWindow(CClientWnd::m_hWnd, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		_ExIf(CClientWnd::IsScrabble(), CClientWnd::Scrabble());
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 弹出菜单初始化
VOID CMainWnd::OnMenuPopup(WPARAM wParam, LPARAM lParam)
{
	HWND hWnd;
	DWORD dwSize;
	TCHAR tzTemp[512];
	TCHAR tzPath[512];

	switch (GetMenuItemID((HMENU) wParam, 0))
	{
	case IDM_Help_Desktop:
		// 判断快捷方式是否存在
		CheckCommand(IDM_Help_Desktop, CShortcut::Exist(CSIDL_DESKTOP));
		CheckCommand(IDM_Help_StartMenu, CShortcut::Exist(CSIDL_STARTMENU));
		CheckCommand(IDM_Help_ProgramMenu, CShortcut::Exist(CSIDL_PROGRAMS));
		CheckCommand(IDM_Help_QuickLaunch, CShortcut::Exist(CSIDL_APPDATA));
		CheckCommand(IDM_Help_VisualStudio, CVSTool::Exist());
		break;

	case IDM_Play_Play:
		hWnd = CClientWnd::GetActive();
		_ExIf(hWnd, SendMessage(hWnd, WM_INITMENUPOPUP, wParam, lParam));
		break;

	case IDM_View_Toolbar:
		GetModuleFileName(NULL, tzTemp, MAX_PATH);
		wsprintf(tzPath, TEXT("\"%s\" \"%%1\""), tzTemp);
		dwSize = _NumOf(tzTemp);
		SHGetValue(HKEY_CLASSES_ROOT, STR_AppName TEXT("\\shell\\open\\command"), NULL, NULL, tzTemp, &dwSize);
		CheckCommand(IDM_View_AssociateFile, lstrcmpi(tzTemp, tzPath) == 0);			
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 菜单项被选择
VOID CMainWnd::OnMenuSelect(UINT uCmd)
{
	// 如果是语言菜单项
	if ((uCmd > IDM_View_Default) && (uCmd < IDM_View_Default + 50))
	{
		CLanguage::GetDescription(m_hMenu, uCmd);
	}
	else
	{
		// 如果是新近文件菜单项
		if ((uCmd > IDM_File_Recent) && (uCmd < IDM_File_Recent + 50))
		{
			uCmd = IDM_File_Recent;
		}
		CLanguage::TranslateString(uCmd);
	}

	// 在状态栏显示相应的菜单项提示
	CClientWnd::SetStatusText(CLanguage::m_tzText[0] ? CLanguage::m_tzText : LNG_Ready);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 命令处理
VOID CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	PTSTR p;
	DWORD dwSize;
	CChildWnd *pWnd;	
	TCHAR tzTemp[512];
	TCHAR tzPath[512];

	if ((LOWORD(wParam) >= IDM_View_Default) && (LOWORD(wParam) < IDM_View_Default + 50))
	{
		// 改变语言
		if (IsMenuChecked(LOWORD(wParam)) == FALSE)
		{
			OnViewLanguage(LOWORD(wParam));
		}
		return;
	}
	else if ((LOWORD(wParam) >= IDM_File_Recent) && (LOWORD(wParam) < IDM_File_Recent + 10))
	{
		// 打开新近文件
		GetMenuString(m_hMenu, LOWORD(wParam), tzTemp, _NumOf(tzTemp), MF_BYCOMMAND);
		OnFileOpen(tzTemp + 4);
		return;
	}

	switch (LOWORD(wParam))
	{
	case IDM_File_Open:
		OnFileOpen((PTSTR) lParam);
		break;

	case IDM_File_Exit:
		PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		break;

	case IDM_View_Toolbar:
	case IDM_View_StatusBar:
	case IDM_View_AlwaysOnTop:
	case IDM_View_MinToTray:
	case IDM_View_ShowSplash:
	case IDM_View_ShowOpen:
	case IDM_View_PlayOnOpen:
		// 查看选项
		OnViewMenu(LOWORD(wParam), FALSE);
		break;

	case IDM_View_AssociateFile:
		GetModuleFileName(NULL, tzPath, MAX_PATH);
		wsprintf(tzTemp, TEXT("\"%s\" \"%%1\""), tzPath);
		dwSize = _NumOf(tzPath);
		SHGetValue(HKEY_CLASSES_ROOT, STR_AppName TEXT("\\shell\\open\\command"), NULL, NULL, tzPath, &dwSize);
		if (lstrcmpi(tzTemp, tzPath) == 0)
		{
			SHDeleteKey(HKEY_CLASSES_ROOT, STR_AppName);

			for (p = STR_ExtList; *p; p += lstrlen(p) + 1)
			{
				SHDeleteKey(HKEY_CLASSES_ROOT, p);
			}
		}
		else
		{
			SHSetValue(HKEY_CLASSES_ROOT, 
				STR_AppName TEXT("\\shell\\open\\command"), NULL, REG_SZ, tzTemp, _StrSize(tzTemp));

			GetModuleFileName(NULL, tzPath, MAX_PATH);
			wsprintf(tzTemp, TEXT("\"%s\",1"), tzPath);
			SHSetValue(HKEY_CLASSES_ROOT, STR_AppName TEXT("\\DefaultIcon"), NULL, REG_SZ, tzTemp, _StrSize(tzTemp));

			for (p = STR_ExtList; *p; p += lstrlen(p) + 1)
			{
				SHSetValue(HKEY_CLASSES_ROOT, p, NULL, REG_SZ, STR_AppName, sizeof(STR_AppName));
			}
		}
		break;

	case IDM_Window_Cascade:
		SendMessage(CClientWnd::m_hWnd, WM_MDICASCADE, 0, 0);
		return;

	case IDM_Window_TitleHorz:
		SendMessage(CClientWnd::m_hWnd, WM_MDITILE, MDITILE_HORIZONTAL, 0);
		break;

	case IDM_Window_TitleVert:
		SendMessage(CClientWnd::m_hWnd, WM_MDITILE, MDITILE_VERTICAL, 0);
		break;

	case IDM_Window_ArrangeIcons:
		SendMessage(CClientWnd::m_hWnd, WM_MDIICONARRANGE, 0, 0);
		break;

	case IDM_Window_CloseAll:
		CClientWnd::CloseAll();
		break;

	case IDM_Help_Content:
		// 查看帮助内容
		GetModuleFileName(NULL, tzTemp, MAX_PATH);
		lstrcpy(_StrEnd(tzTemp) - _LenOf(EXT_Chm), EXT_Chm);
		if (ShellExecute(NULL, NULL, tzTemp, NULL, NULL, SW_SHOW) <= (HINSTANCE) 32)
		{
			ErrorBox(ERR_HelpErr);
		}
		break;

	case IDM_Help_Desktop:
	case IDM_Help_StartMenu:
	case IDM_Help_ProgramMenu:
	case IDM_Help_QuickLaunch:
	case IDM_Help_VisualStudio:
		// 创建或删除快捷方式
		OnHelpShortcut(LOWORD(wParam));
		break;

	case IDM_Help_About:
		// 显示关于对话框
		CAboutDlg::Show(m_hWnd);
		break;

	case IDC_TrayIcon:
		// 系统托盘消息
		if ((lParam == WM_LBUTTONUP) || (lParam == WM_RBUTTONUP))
		{
			OnTrayIcon(FALSE);
		}
		break;

	default:
		pWnd = CClientWnd::GetChildWnd();
		_ExIf(pWnd, pWnd->OnCommand(wParam, lParam));
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 窗口销毁
VOID CMainWnd::OnDestroy()
{
	WINDOWPLACEMENT wpWindow;

	// 保存窗口位置信息
	wpWindow.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_hWnd, &wpWindow);
	CIni::SetInt(INI_WindowLeft, wpWindow.rcNormalPosition.left);
	CIni::SetInt(INI_WindowTop, wpWindow.rcNormalPosition.top);
	CIni::SetInt(INI_WindowWidth, _RectWidth(wpWindow.rcNormalPosition));
	CIni::SetInt(INI_WindowHeight, _RectHeight(wpWindow.rcNormalPosition));
	CIni::SetInt(INI_WindowState, wpWindow.showCmd);

	// 保存同步状态
	//CIni::SetInt(INI_Synchronize, CRawPlayer::m_bSync);

	// 保存新近文件列表
	CRecentFile::Save();

	// 销毁相关数据
	CLanguage::Destroy();

	// 退出程序
	PostQuitMessage(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 打开文件
VOID CMainWnd::OnFileOpen(PCTSTR ptzFileName)
{
	TCHAR tzFileName[MAX_PATH];
	TCHAR tzOpenError[MAX_PATH * 2];
	static BOOL s_bShowOpen = FALSE;

	if (s_bShowOpen == FALSE)
	{
		s_bShowOpen = TRUE;
		if (CRawPlayer::m_bSync)
		{
			CheckCommand(IDM_Play_Synchronize, CRawPlayer::ToggleSync());
		}

		if (ptzFileName)
		{
			CUtility::TrimQuotMark(tzFileName, ptzFileName);
		}
		else
		{
			tzFileName[0] = 0;
		}

		if (COpenDlg::Open(tzFileName) != ERROR_CANCELLED)
		{
			// 结果处理
			if (COpenDlg::m_pWnd && CClientWnd::Load(COpenDlg::m_pWnd))
			{
				// 添加到新近文件列表
				CRecentFile::Add(tzFileName);
			}
			else
			{
				// 打开文件错误
				wsprintf(tzOpenError, LNG_OpenErr, tzFileName);
				ErrorBox(tzOpenError);
			}
		}

		s_bShowOpen = FALSE;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 切换查看菜单状态
VOID CMainWnd::OnViewMenu(UINT uCmd, BOOL bInitialize)
{
	UINT i;
	BOOL bChecked;
	const PTSTR STR_KeyNames[] =
	{
		INI_Toolbar,
		INI_StatusBar,
		INI_AlwaysOnTop,
		INI_MinToTray,
		INI_ShowSplash,
		INI_ShowOpen,
		INI_PlayOnOpen
	};

	// 获取 INI 键名索引号
	i = uCmd - IDM_View_Toolbar;

	// 如果是有效命令
	if (i < _NumOf(STR_KeyNames))
	{
		// 切换菜单项
		bChecked = !IsMenuChecked(uCmd);
		CheckCommand(uCmd, bChecked);

		// 如果不是初始化指定的切换，记录到 INI 中
		if (bInitialize == FALSE)
		{
			CIni::SetInt(STR_KeyNames[i], bChecked);
		}

		switch (uCmd)
		{
		case IDM_View_Toolbar:
		case IDM_View_StatusBar:
			CClientWnd::ToggleBar();
			break;

		case IDM_View_AlwaysOnTop:
			// 总在最前
			SetWindowPos(m_hWnd, bChecked ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			return;

		case IDM_View_MinToTray:
			// 最小化到系统托盘
			if (bChecked && (bInitialize == FALSE))
			{
				CloseWindow(m_hWnd);
			}
			return;

		default:
			return;
		}
	}
	// 如果是初始化菜单项
	else if (bInitialize)
	{
		// 初始化菜单项
		for (i = 0; i < _NumOf(STR_KeyNames); i++)
		{
			// 默认是否选择
			bChecked = IsMenuChecked(IDM_View_Toolbar + i);

			// 如果默认选择与 INI 中不一样
			if (bChecked != (BOOL) CIni::GetInt(STR_KeyNames[i], bChecked))
			{
				// 切换菜单项
				OnViewMenu(IDM_View_Toolbar + i, TRUE);
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 改变语言
VOID CMainWnd::OnViewLanguage(UINT uLang)
{
#ifdef _MAKELANG
	// 保存常规字符串
	LNG_Description; LNG_Copyright; LNG_Ready; LNG_Version; LNG_Build;
	LNG_CheckUpdate; LNG_CancelUpdate; LNG_VisitWeb; LNG_WriteEmail;
	LNG_CheckUpdateTip; LNG_VisitWebTip; LNG_WriteEmailTip;
	LNG_BitLiveCheck; LNG_BitLiveUpdate; LNG_BitLiveUpToDate; LNG_BitLiveCheckErr; LNG_BitLiveUpdateErr;
	LNG_BitLiveRecieve; LNG_BitLiveRecvPercent; LNG_BitLiveConfirm;

	LNG_ExportFilter; LNG_BrowseFolder;
	LNG_OpenErr; ERR_HelpErr; LNG_CreateShortcutErr; LNG_RemoveShortcutErr;
	LNG_ExportErr; LNG_AddToVSErr; LNG_RemoveFromVSErr; LNG_InputErr; LNG_UnexpectedErr; LNG_DirectDrawErr;
	LNG_CreateShortcutMsg; LNG_RemoveShortcutMsg; LNG_ConvertMsg; LNG_ExportMsg; LNG_AddToVSMsg; LNG_RemoveFromVSMsg;
	LNG_ColorSpaceMsg; LNG_FrameWidthMsg; LNG_FrameHeightMsg; LNG_FrameRateMsg; LNG_FrameStepMsg; LNG_FrameIndexeMsg;
	
	// 保存资源字符串
	for (INT i = IDM_File_Open; i <= IDM_Help_About; i++)
	{
		CLanguage::TranslateString(i);
	}

	// 保存菜单
	CLanguage::TranslateMenu(m_hMenu, MAKEINTRESOURCE(IDR_Menu));

	// 保存对话框字符串
	PostMessage(m_hWnd, WM_COMMAND, IDM_File_Open, 0);

#else

	// 设置语言
	CLanguage::Set(m_hMenu, uLang);

	// 翻译菜单
	OnCommand(IDM_Window_Cascade);
	CLanguage::TranslateMenu(m_hMenu, MAKEINTRESOURCE(IDR_Menu));
	DrawMenuBar(m_hWnd);

	// 设置状态栏
	OnMenuSelect(uLang);

#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 创建或删除快捷方式
VOID CMainWnd::OnHelpShortcut(UINT uCmd)
{
	INT iFolder;

	switch (uCmd)
	{
	case IDM_Help_Desktop:
		iFolder = CSIDL_DESKTOP;
		break;

	case IDM_Help_StartMenu:
		iFolder = CSIDL_STARTMENU;
		break;

	case IDM_Help_ProgramMenu:
		iFolder = CSIDL_PROGRAMS;
		break;

	case IDM_Help_QuickLaunch:
		iFolder = CSIDL_APPDATA;
		break;

	default:
		// 操作在 Visual Studio 中的快捷方式
		if (IsMenuChecked(IDM_Help_VisualStudio))
		{
			if (CVSTool::Remove())
			{
				MsgBox(LNG_RemoveFromVSMsg);
			}
			else
			{
				ErrorBox(LNG_RemoveFromVSErr);
			}
		}
		else
		{
			if (CVSTool::Add())
			{
				MsgBox(LNG_AddToVSMsg);
			}
			else
			{
				ErrorBox(LNG_AddToVSErr);
			}
		}
		return;
	}

	if (IsMenuChecked(uCmd))
	{
		if (CShortcut::Remove(iFolder))
		{
			MsgBox(LNG_RemoveShortcutMsg);
		}
		else
		{
			ErrorBox(LNG_RemoveShortcutErr);
		}
	}
	else
	{
		if (CShortcut::Create(iFolder))
		{
			MsgBox(LNG_CreateShortcutMsg);
		}
		else
		{
			ErrorBox(LNG_CreateShortcutErr);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 最小化到系统托盘
VOID CMainWnd::OnTrayIcon(BOOL bEnable)
{
	NOTIFYICONDATA nidTray = {0};
	nidTray.cbSize = sizeof(NOTIFYICONDATA);
	nidTray.hWnd = m_hWnd;
	nidTray.uID = IDC_TrayIcon;

	if (bEnable)
	{
		// 添加系统托盘图标，隐藏主窗口
		nidTray.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nidTray.uCallbackMessage = WM_COMMAND;
		nidTray.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_Main));
		GetWindowText(m_hWnd, nidTray.szTip, _NumOf(nidTray.szTip));
		Shell_NotifyIcon(NIM_ADD, &nidTray);
		ShowWindow(m_hWnd, SW_HIDE);
	}
	else
	{
		// 删除系统托盘图标，恢复主窗口
		Shell_NotifyIcon(NIM_DELETE, &nidTray);
		ShowWindow(m_hWnd, SW_RESTORE);
		SetForegroundWindow(m_hWnd);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
