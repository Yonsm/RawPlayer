


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>

#define INI_Main TEXT("Main")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CIni 类
class CIni
{
public:
	// INI 文件名
	static TCHAR m_tzIniName[MAX_PATH];

public:
	// 设置文件名
	inline static VOID SetIniName()
	{
		lstrcpy(m_tzIniName + GetModuleFileName(NULL, m_tzIniName, MAX_PATH) - 4, TEXT(".ini"));
	}

	// 设置文件名
	inline static VOID SetIniName(PCTSTR ptzIniName)
	{
		lstrcpy(m_tzIniName, ptzIniName);
	}

public:
	// 获取整数
	inline static UINT GetInt(PCTSTR ptzSecName, PCTSTR ptzKeyName, INT iDefault = 0)
	{
		return GetPrivateProfileInt(ptzSecName, ptzKeyName, iDefault, m_tzIniName);
	}

	// 设置整数
	inline static BOOL SetInt(PCTSTR ptzSecName, PCTSTR ptzKeyName, INT iValue = 0)
	{
		TCHAR tzString[16];

		wsprintf(tzString, TEXT("%d"), iValue);
		return WritePrivateProfileString(ptzSecName, ptzKeyName, tzString, m_tzIniName);
	}

	// 获取字符串
	inline static DWORD GetString(PCTSTR ptzSecName, PCTSTR ptzKeyName, PTSTR ptzString,
		DWORD dwSize = MAX_PATH, PCTSTR ptzDefault = NULL)
	{
		return GetPrivateProfileString(ptzSecName, ptzKeyName, ptzDefault, ptzString, dwSize, m_tzIniName);
	}

	// 设置字符串
	inline static BOOL SetString(PCTSTR ptzSecName, PCTSTR ptzKeyName, PCTSTR ptzString)
	{
		return WritePrivateProfileString(ptzSecName, ptzKeyName, ptzString, m_tzIniName);
	}

	// 获取结构
	inline static BOOL GetStruct(PCTSTR ptzSecName, PCTSTR ptzKeyName, PVOID pvStruct, UINT uSize)
	{
		return GetPrivateProfileStruct(ptzSecName, ptzKeyName, pvStruct, uSize, m_tzIniName);
	}

	// 设置结构
	inline static BOOL SetStruct(PCTSTR ptzSecName, PCTSTR ptzKeyName, PVOID pvStruct, UINT uSize)
	{
		return WritePrivateProfileStruct(ptzSecName, ptzKeyName, pvStruct, uSize, m_tzIniName);
	}

	// 获取节
	inline static DWORD GetSection(PCTSTR ptzSecName, PTSTR ptzBuffer, DWORD dwSize)
	{
		return GetPrivateProfileSection(ptzSecName, ptzBuffer, dwSize, m_tzIniName);
	}

	// 设置节
	inline static DWORD SetSection(PCTSTR ptzSecName, PCTSTR ptzString)
	{
		return WritePrivateProfileSection(ptzSecName, ptzString, m_tzIniName);
	}

	// 获取节名
	inline static DWORD GetSectionNames(PTSTR ptzBuffer, DWORD dwSize)
	{
		return GetPrivateProfileSectionNames(ptzBuffer, dwSize, m_tzIniName);
	}

public:
	// 获取整数
	inline static UINT GetInt(PCTSTR ptzKeyName, INT iDefault = 0)
	{
		return GetInt(INI_Main, ptzKeyName, iDefault);
	}

	// 设置整数
	inline static BOOL SetInt(PCTSTR ptzKeyName, INT iValue = 0)
	{
		return SetInt(INI_Main, ptzKeyName, iValue);
	}

	// 获取字符串
	inline static DWORD GetString(PCTSTR ptzKeyName, PTSTR ptzString, DWORD dwSize = MAX_PATH, PCTSTR ptzDefault = NULL)
	{
		return GetString(INI_Main, ptzKeyName, ptzString, dwSize, ptzDefault);
	}

	// 设置字符串
	inline static BOOL SetString(PCTSTR ptzKeyName, PCTSTR ptzString = NULL)
	{
		return SetString(INI_Main, ptzKeyName, ptzString);
	}

	// 获取结构
	inline static BOOL GetStruct(PCTSTR ptzKeyName, PVOID pvStruct, UINT uSize)
	{
		return GetStruct(INI_Main, ptzKeyName, pvStruct, uSize);
	}

	// 设置结构
	inline static BOOL SetStruct(PCTSTR ptzKeyName, PVOID pvStruct, UINT uSize)
	{
		return SetStruct(INI_Main, ptzKeyName, pvStruct, uSize);
	}

	// 获取节
	inline static DWORD GetSection(PTSTR ptzBuffer, DWORD dwSize)
	{
		return GetSection(INI_Main, ptzBuffer, dwSize);
	}

	// 设置节
	inline static DWORD SetSection(PCTSTR ptzString)
	{
		return SetSection(INI_Main, ptzString);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
