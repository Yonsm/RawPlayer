


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "Main.h"
#include "BitLive.h"
#include <WinInet.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define _BLPostMsg(t, l)		PostMessage(m_hNotify, WM_COMMAND, MAKELONG(IDC_BitLive, t), (LPARAM) (l))
#define _BLSendMsg(t, l)		SendMessage(m_hNotify, WM_COMMAND, MAKELONG(IDC_BitLive, t), (LPARAM) (l))
#define _BLGetInt(f, k, d)		GetPrivateProfileInt(INI_BitLive, k, d, f)
#define _BLGetString(f, k, t)	GetPrivateProfileString(INI_BitLive, k, NULL, t, 1024, f)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBitLive 类静态变量
HWND CBitLive::m_hNotify = NULL;
BOOL CBitLive::m_bCancel = NULL;
HANDLE CBitLive::m_hThread = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 获取提示文本
BOOL CBitLive::GetPromptText(PCTSTR ptzFileName, PCTSTR ptzKeyName, PTSTR ptzText)
{
	PTSTR p;
	TCHAR tzKeyName[MAX_PATH];

	// 获取语言相关文本，如果没有则尝试获取默认文本
	wsprintf(tzKeyName, TEXT("%s.%u"), ptzKeyName, CIni::GetInt(INI_Language, GetUserDefaultLangID()));
	if (_BLGetString(ptzFileName, tzKeyName, ptzText) || _BLGetString(ptzFileName, ptzKeyName, ptzText))
	{
		// 替换换行符
		for (p = ptzText; *p; p++)
		{
			_ExIf(*p == '`', *p = '\n');
		}

		return TRUE;
	}

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 检查更新
BOOL WINAPI CBitLive::Check()
{
	BOOL bResult;
	DWORD dwBuild;
	TCHAR tzUrl[1024];
	TCHAR tzText[1024];
	TCHAR tzUpdateFile[MAX_PATH];
	TCHAR tzBitLiveFile[MAX_PATH];

	_Try
	{
		tzBitLiveFile[0] = 0;
		_LeaveIf(CIni::GetString(INI_BitLive, tzUrl, 1024,
			STR_WebUrl TEXT("BitLive/") STR_AppName TEXT(".blv")) < 2);

		// 获取文件
		_BLSendMsg(BLL_Check, LNG_BitLiveCheck);
		GetTempPath(MAX_PATH, tzBitLiveFile);
		GetTempFileName(tzBitLiveFile, STR_AppName, 0, tzBitLiveFile);
		if ((GetFile(tzBitLiveFile, tzUrl, NULL, FALSE) == 0) ||
			(_BLGetString(tzBitLiveFile, INI_UpdateUrl, tzUrl) == 0))
		{
			// 检查失败
			_BLSendMsg(BLL_CheckErr, LNG_BitLiveCheckErr);
			_Leave;
		}

		// 检测更新
		dwBuild = _BLGetInt(tzBitLiveFile, INI_UpdateBuild, 0);
		if (dwBuild <= VER_Build)
		{
			_BLSendMsg(BLL_UpToDate, LNG_BitLiveUpToDate);
			_LeaveIf(m_bCancel);
		}
		else
		{
			// 询问是否更新
			if (_BLGetInt(tzBitLiveFile, INI_UpdateBox, TRUE))
			{
				GetPromptText(tzBitLiveFile, INI_UpdateText, tzText);
				_BLSendMsg(BLL_Query, tzText);
			}
			if (m_bCancel == FALSE)
			{
				// 获取更新
				_BLSendMsg(BLL_Update, LNG_BitLiveUpdate);
				GetModuleFileName(NULL, tzUpdateFile, MAX_PATH);
				wsprintf(_StrEnd(tzUpdateFile) - _LenOf(EXT_Exe), TEXT(".%u") EXT_Exe, dwBuild);
				bResult = (GetFile(tzUpdateFile, tzUrl) == _BLGetInt(tzBitLiveFile, INI_UpdateSize, -1));
				if (bResult)
				{
					// 确认更新
					wsprintf(tzText, LNG_BitLiveConfirm, tzUpdateFile);
					_BLSendMsg(BLL_Confirm, tzText);
					if (m_bCancel == FALSE)
					{
						bResult = Update(tzUpdateFile, _BLGetInt(tzBitLiveFile, INI_UpdateReplace, FALSE));
						if (bResult && _BLGetInt(tzBitLiveFile, INI_UpdateExit, FALSE))
						{
							// 退出程序
							_BLPostMsg(BLL_Exit, 0);
						}
					}
				}

				if (bResult == FALSE)
				{
					// 更新失败
					_BLSendMsg(BLL_UpdateErr, LNG_BitLiveUpdateErr);
					DeleteFile(tzUpdateFile);
				}

				_BLSendMsg(BLL_UpdateEnd, (bResult ? LNG_Ready : LNG_BitLiveUpdateErr));

				_Leave;
			}
		}

		// 显示新闻文本
		GetPromptText(tzBitLiveFile, INI_NewsText, tzText);
		if (_BLGetInt(tzBitLiveFile, INI_NewsBox, 0))
		{
			_BLSendMsg(BLL_NewsBox, tzText);
		}
		_BLSendMsg(BLL_News, tzText);
	}
	_Finally
	{
		if (tzBitLiveFile[0])
		{
			// 删除临时文件
			DeleteFile(tzBitLiveFile);
		}

		// 关闭线程句柄
		_SafeCloseHandle(m_hThread);

		return TRUE;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 获取文件
DWORD CBitLive::GetFile(PCTSTR ptzFileName, PCTSTR ptzUrl, PCTSTR ptzHeader, BOOL bNotify)
{
	DWORD dwSize;
	HANDLE hFile;
	DWORD dwRecv;
	DWORD dwTotal;
	HINTERNET hInet;
	HINTERNET hHttp;
	TCHAR tzLen[32];
	BYTE bBuffer[4096];
	TCHAR tzText[MAX_PATH];

	// 建立会话
	dwRecv = 0;
	hInet = InternetOpen(STR_AppName, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInet)
	{
		// 打开链接
		hHttp = InternetOpenUrl(hInet, ptzUrl, ptzHeader, lstrlen(ptzHeader), INTERNET_FLAG_RELOAD, 0);
		if (hHttp)
		{
			// 获取大小
			dwSize = _LenOf(tzLen);
			if (HttpQueryInfo(hHttp, HTTP_QUERY_CONTENT_LENGTH, tzLen, &dwSize, NULL))
			{
				dwTotal = _StrToInt(tzLen);
			}
			else
			{
				dwTotal = -1;
			}

			// 创建文件
			hFile = _CreateFileForWrite(ptzFileName);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				// 获取数据
				while ((m_bCancel == FALSE) && InternetReadFile(hHttp, bBuffer, sizeof(bBuffer), &dwSize) && dwSize)
				{
					WriteFile(hFile, bBuffer, dwSize, &dwSize, NULL);
					dwRecv += dwSize;
					if (bNotify)
					{
						// 生成进度提示文本
						if (dwTotal == -1)
						{
							wsprintf(tzText, LNG_BitLiveRecieve, dwRecv);
						}
						else
						{
							wsprintf(tzText, LNG_BitLiveRecvPercent, dwRecv, (dwRecv * 100 / dwTotal));
						}

						// 进度通知
						_BLSendMsg(BLL_Progress, tzText);
					}
				}

				CloseHandle(hFile);
			}

			InternetCloseHandle(hHttp);
		}

		InternetCloseHandle(hInet);
	}

	return dwRecv;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
BOOL CBitLive::Update(PCTSTR ptzFileName, BOOL bReplaceMode)
{
	DWORD dwSize;
	HANDLE hFile;
	TCHAR tzText[1024];
	TCHAR tzExePath[MAX_PATH];
	TCHAR tzBatFile[MAX_PATH];

	if (bReplaceMode)
	{
		GetModuleFileName(NULL, tzExePath, MAX_PATH);
		lstrcpy(tzBatFile, tzExePath);
		lstrcpy(_StrRChr(tzBatFile, '\\'), TEXT("\\BitLive") EXT_Bat);

		hFile = _CreateFileForWrite(tzBatFile);
		_ReturnValIf(hFile == INVALID_HANDLE_VALUE, FALSE);

		dwSize = wsprintf(tzText,
				TEXT(":REPEAT\r\n")
				TEXT("DEL \"%s\"\r\n")
				TEXT("IF EXIST \"%s\" GOTO REPEAT\r\n")
				TEXT("MOVE \"%s\" \"%s\"\r\n")
				TEXT("START%s\"%s\"\r\n")
				TEXT("DEL %%0\r\n"),
				tzExePath, tzExePath, ptzFileName, tzExePath, (_WinVerMajor >= 5) ? TEXT(" \"\" ") : TEXT(" "), tzExePath);

#ifdef _UNICODE
		CHAR szText[1024];
		dwSize = _StrToAStr(szText, tzText) - 1;
		WriteFile(hFile, szText, dwSize, &dwSize, 0);
#else
		WriteFile(hFile, tzText, dwSize, &dwSize, 0);
#endif

		CloseHandle(hFile);

		ptzFileName = tzBatFile;
	}

	// 直接执行
	return (_ShellOpen(ptzFileName, NULL, bReplaceMode ? SW_HIDE : SW_SHOWNORMAL) > (HINSTANCE) 32);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
