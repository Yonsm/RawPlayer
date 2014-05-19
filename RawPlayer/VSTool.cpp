


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "Main.h"
#include "VSTool.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 枚举版本并操作
BOOL CVSTool::Operate(BOOL bDotNet, INT iCommand)
{
	DWORD i;
	HKEY hKey;
	BOOL bResult;
	LONG lResult;
	DWORD dwSize;
	HKEY hToolKey;
	FILETIME ftTime;
	TCHAR tzName[MAX_PATH];
	TCHAR tzToolKey[MAX_PATH];

	// 打开键值
	lResult = RegOpenKeyEx(HKEY_CURRENT_USER, bDotNet ? TEXT("Software\\Microsoft\\VisualStudio") : TEXT("Software\\Microsoft\\Devstudio"), 0, KEY_ALL_ACCESS, &hKey);
	if (lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}

	// 枚举版本循环
	for (i = 0, bResult = FALSE; (bResult == FALSE) || (iCommand != 0); i++)
	{
		// 枚举版本
		dwSize = MAX_PATH;
		lResult = RegEnumKeyEx(hKey, i, tzName, &dwSize, NULL, NULL, NULL, &ftTime);
		if (lResult != ERROR_SUCCESS)
		{
			break;
		}

		// 打开工具菜单键值
		wsprintf(tzToolKey, TEXT("%s\\%s"), tzName, bDotNet ? TEXT("External Tools") : TEXT("Tools"));
		lResult = RegOpenKeyEx(hKey, tzToolKey, 0, KEY_ALL_ACCESS, &hToolKey);
		if (lResult == ERROR_SUCCESS)
		{
			_ExIf(Operate(hToolKey, bDotNet, iCommand), bResult = TRUE);
			RegCloseKey(hToolKey);
		}
	}

	RegCloseKey(hKey);

	return bResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 针对已打开的工具键操作
BOOL CVSTool::Operate(HKEY hToolKey, BOOL bDotNet, INT iCommand)
{
	BOOL bResult;
	LONG lResult;
	DWORD dwSize;
	DWORD dwIndex;
	DWORD dwNumTools;

	// 查询工具数量
	bResult = FALSE;
	dwSize = sizeof(dwNumTools);
	lResult = RegQueryValueEx(hToolKey, bDotNet ? TEXT("ToolNumKeys") : TEXT("NumTools"), NULL, NULL, (PBYTE) &dwNumTools, &dwSize);
	if (lResult == ERROR_SUCCESS)
	{
		// 获取工具编号
		dwIndex = Get(hToolKey, bDotNet, dwNumTools);
		if (dwIndex < dwNumTools)
		{
			if (iCommand < 0)
			{
				// 删除工具
				bResult = Remove(hToolKey, bDotNet, dwIndex, dwNumTools);
			}
			else if (iCommand == 0)
			{
				// 工具存在
				bResult = TRUE;
			}
			else if (iCommand > 0)
			{
				// 更新工具
				bResult = Update(hToolKey, bDotNet, dwIndex);
			}
		}
		else if (iCommand > 0)
		{
			// 添加工具
			bResult = Add(hToolKey, dwNumTools, bDotNet, iCommand);
		}
	}

	return bResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 获取工具编号
DWORD CVSTool::Get(HKEY hToolKey, BOOL bDotNet, DWORD dwNumTools)
{
	DWORD i;
	DWORD dwSize;
	LONG lResult;
	TCHAR tzName[MAX_PATH];
	TCHAR tzValue[MAX_PATH];

	// 判断是否已经存在
	for (i = 0; i < dwNumTools; i++)
	{
		dwSize = sizeof(tzValue);
		wsprintf(tzName, bDotNet ? TEXT("ToolTitle%u") : TEXT("MenuName#%u"), i);
		lResult = RegQueryValueEx(hToolKey, tzName, NULL, NULL, (PBYTE) tzValue, &dwSize);
		if ((lResult == ERROR_SUCCESS) && (lstrcmpi(tzValue, STR_AppName) == 0))
		{
			break;
		}
	}

	return i;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 添加工具
BOOL CVSTool::Add(HKEY hToolKey, DWORD dwNumTools, BOOL bDotNet, INT iCommand)
{
	LONG lResult;
	DWORD dwValue;
	TCHAR tzName[MAX_PATH];
	TCHAR tzValue[MAX_PATH];

	// 添加工具
	dwValue = bDotNet ? 17 : 1;
	wsprintf(tzName, bDotNet ? TEXT("ToolOpt%u") : TEXT("GUITool#%u"), dwNumTools);
	RegSetValueEx(hToolKey, tzName, 0, REG_DWORD, (PBYTE) &dwValue, sizeof(DWORD));

	GetModuleFileName(NULL, tzValue, MAX_PATH);
	wsprintf(tzName, bDotNet ? TEXT("ToolCmd%u") : TEXT("PathName#%u"), dwNumTools);
	RegSetValueEx(hToolKey, tzName, 0, REG_SZ, (PBYTE) tzValue, _StrSize(tzValue));

	wsprintf(tzName, bDotNet ? TEXT("ToolTitle%u") : TEXT("MenuName#%u"), dwNumTools);
	RegSetValueEx(hToolKey, tzName, 0, REG_SZ, (PBYTE) STR_AppName, sizeof(STR_AppName));

	dwNumTools++;
	lResult = RegSetValueEx(hToolKey, bDotNet ? TEXT("ToolNumKeys") : TEXT("NumTools"), 0, REG_DWORD, (PBYTE) &dwNumTools, sizeof(DWORD));
	return (lResult == ERROR_SUCCESS);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 更新工具
BOOL CVSTool::Update(HKEY hToolKey, BOOL bDotNet, DWORD dwIndex)
{
	LONG lResult;
	TCHAR tzName[MAX_PATH];
	TCHAR tzValue[MAX_PATH];

	GetModuleFileName(NULL, tzValue, MAX_PATH);
	wsprintf(tzName, bDotNet ? TEXT("ToolCmd%u") : TEXT("PathName#%u"), dwIndex);
	lResult = RegSetValueEx(hToolKey, tzName, 0, REG_SZ, (PBYTE) tzValue, _StrSize(tzValue));
	return (lResult == ERROR_SUCCESS);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 删除工具
BOOL CVSTool::Remove(HKEY hToolKey, BOOL bDotNet, DWORD dwIndex, DWORD dwNumTools)
{
	LONG lResult;
	TCHAR tzName[MAX_PATH];

	// 删除工具
	wsprintf(tzName, bDotNet ? TEXT("ToolOpt%u") : TEXT("GUITool#%u"), dwIndex);
	RegDeleteValue(hToolKey, tzName);

	wsprintf(tzName, bDotNet ? TEXT("ToolCmd%u") : TEXT("PathName#%u"), dwIndex);
	RegDeleteValue(hToolKey, tzName);

	wsprintf(tzName, bDotNet ? TEXT("ToolTitle%u") : TEXT("MenuName#%u"), dwIndex);
	RegDeleteValue(hToolKey, tzName);

	dwNumTools--;
	lResult = RegSetValueEx(hToolKey, bDotNet ? TEXT("ToolNumKeys") : TEXT("NumTools"), 0, REG_DWORD, (PBYTE) &dwNumTools, sizeof(DWORD));

	return (lResult == ERROR_SUCCESS);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
