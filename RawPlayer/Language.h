


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define _Lang(n, a)				CLanguage::TranslateText(TEXT(#n), TEXT(a))
#define _TLang(n, t)			CLanguage::TranslateText(TEXT(#n), t)
#define _LangEx(n, a)			CLanguage::TranslateTextEx(TEXT(#n), TEXT(a))
#define _TLangEx(n, t)			CLanguage::TranslateTextEx(TEXT(#n), t)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLanguage 类
class CLanguage
{
public:
	static UINT m_uLang;					// 语言菜单号
	static TCHAR m_tzText[1024];			// 临时字符串

private:
	static UINT m_uMax;						// 语言最大号
	static HFONT m_hFont;					// 对话框字体
	static TCHAR m_tzFileName[MAX_PATH];	// 语言文件名称

public:
	// 初始化语言
	static LANGID Initialize();

	// 列出语言
	static UINT List(HMENU hMenu);

	// 设置语言
	static VOID Set(HMENU hMenu, UINT uLang);

	// 获取语言描述
	static PTSTR GetDescription(HMENU hMenu, UINT uLang);

	// 获取常规字符串
	static PTSTR TranslateText(PCTSTR ptzName, PCTSTR ptzDefault = NULL);

	// 获取常规字符串，并替换特殊字符
	static PTSTR TranslateTextEx(PCTSTR ptzName, PCTSTR ptzDefault = NULL);

	// 获取资源字符串
	static PTSTR TranslateString(UINT uResID);

	// 更新菜单
	static VOID TranslateMenu(HMENU hMenu, PCTSTR ptzResName);

	// 更新对话框
	static VOID TranslateDialog(HWND hWnd, PCTSTR ptzResName);

public:
	// 销毁
	inline static VOID Destroy()
	{
		if (m_hFont)
		{
			DeleteObject(m_hFont);
		}
	}

private:
	// 设置菜单字符串
	static BOOL SetMenuString(HMENU hMenu, UINT uItemID, PCTSTR ptzString, BOOL bByPosition = FALSE);

	// 从语言文件中更新菜单
	static VOID UpdateMenuFromLanguage(HMENU hMenu, PCTSTR ptzSection, PCTSTR ptzPath = TEXT(""));

	// 从资源中更新菜单
	static VOID UpdateMenuFromResource(HMENU hMenu, PCTSTR ptzResName);

	// 从语言文件中更新对话框
	static BOOL CALLBACK UpdateDialogFromLanguage(HWND hWnd, PCTSTR ptzSection);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
