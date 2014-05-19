


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include "Define.h"
#include "Macro.h"
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#ifdef _UNICODE
#define CF_TTEXT					CF_UNICODETEXT
#define STR_GetFileAttributesEx		"GetFileAttributesExW"
#else
#define CF_TTEXT					CF_TEXT
#define STR_GetFileAttributesEx		"GetFileAttributesExA"
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUtility 类
class CUtility
{
public:
	// 获取特殊文件夹路径
	static BOOL GetFolder(INT iFolder, PTSTR ptzFolder);

	// 获取图片文件夹
	static PTSTR GetPicturesFolder(PTSTR ptzFolder, BOOL bIniFirst = TRUE);

	// 浏览获取文件夹
	static BOOL BrowseFolder(PTSTR ptzFolder, HWND hParent = NULL);

	// 去除末尾的反斜杠
	inline static PTSTR TrimBackslash(PTSTR ptzString)
	{
		PTSTR p = _StrEnd(ptzString) - 1;
		_ExIf(*p == '\\', *p = 0);
		return ptzString;
	}

	// 取引号内的字符串
	inline static PTSTR TrimQuotMark(PTSTR ptzDst, PCTSTR ptzSrc)
	{
		if (ptzSrc)
		{
			PTSTR p, q;
			for (p = (PTSTR) ptzSrc; *p == '"'; p++);
			for (q = (PTSTR) _StrEnd(ptzSrc) - 1; (*q == '"') && (q > p); q--);
			lstrcpyn(ptzDst, p, (INT) (q - p + 2));
		}
		else
		{
			ptzDst[0] = 0;
		}

		return ptzDst;
	}

	// 判断是否是目录
	inline static BOOL IsDirectory(PTSTR ptzString)
	{
		WIN32_FILE_ATTRIBUTE_DATA fadAttrib;

#if (_WINVER > 0x0400)
		return GetFileAttributesEx(ptzString, GetFileExInfoStandard, &fadAttrib) &&
			(fadAttrib.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
#else
		typedef BOOL (WINAPI* GFAEPROC)(PCTSTR, GET_FILEEX_INFO_LEVELS, PVOID);
		GFAEPROC fpGfae = (GFAEPROC) GetProcAddress(GetModuleHandle(TEXT("KERNEL32")), STR_GetFileAttributesEx);
		if (fpGfae)
		{
			return fpGfae(ptzString, GetFileExInfoStandard, &fadAttrib) &&
				(fadAttrib.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
		}
		else
		{
			DWORD dwAttrib = GetFileAttributes(ptzString);
			return (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
		}
#endif
	}

	// 获取扇区大小
	inline static DWORD GetSectorSize(PCTSTR ptzPath)
	{
		PTSTR p;
		DWORD dwFree;
		DWORD dwTotal;
		DWORD dwSector;
		DWORD dwSectorSize;
		TCHAR tzRootPath[MAX_PATH];

		// 获取扇区大小
		lstrcpy(tzRootPath, ptzPath);
		p = tzRootPath;
		if (tzRootPath[0] == '\\')
		{
			p = _StrChr(p + 2, '\\') + 1;
		}
		p = _StrChr(p, '\\');
		_ExIf(p, *(p + 1) = 0);
		GetDiskFreeSpace(tzRootPath, &dwSector, &dwSectorSize, &dwFree, &dwTotal);
		return dwSectorSize;
	}

private:
	// 浏览文件夹回调函数
	static INT CALLBACK BrowseFolderProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

public:
	// 设置到剪贴板数据
	static BOOL SetClipboard(UINT uFormat, PVOID pvData, UINT uSize);

	// 获取剪贴板数据
	static PVOID GetClipboard(UINT uFormat);

	// 设置到剪贴板文本
	inline static BOOL SetClipboardText(PCTSTR ptzText, UINT uSize = -1)
	{
		return SetClipboard(CF_TTEXT, (PVOID) ptzText, uSize == -1 ? _StrSize(ptzText) : uSize);
	}

	// 获取剪贴板文本
	inline static PTSTR GetClipboardText()
	{
		return (PTSTR) GetClipboard(CF_TTEXT);
	}

	// 剪贴板数据是否为文本格式
	inline static BOOL IsClipboardText()
	{
		return (IsClipboardFormatAvailable(CF_TEXT) ||
			IsClipboardFormatAvailable(CF_OEMTEXT) ||
			IsClipboardFormatAvailable(CF_UNICODETEXT));
	}

public:
	// 创建工具栏位图
	static HBITMAP CreateToolbarBitmap(HBITMAP hBitmap, BOOL bDisabled = FALSE);

	// 创建工具栏
	static HWND CreateToolbar(DWORD dwStyle, HWND hParent, PCTSTR ptzBitmapName, HIMAGELIST *phImageList,
		UINT uButtons, LPCTBBUTTON ptbbButtons);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
