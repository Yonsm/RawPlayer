


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMainWnd 类
class CMainWnd
{
public:
	static HWND m_hWnd;						// 主窗口句柄
	static HMENU m_hMenu;					// 主菜单句柄

public:
	// 创建主窗口
	static HWND Create();

	// 显示主窗口，并建立消息循环，等待运行完成
	static VOID Show(PCTSTR ptzCmdLine = NULL, INT iCmdShow = SW_NORMAL);

private:
	// 回调函数
	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 窗口被创建
	static VOID OnCreate();

	// 大小改变
	static VOID OnSize(WPARAM wParam = SIZE_RESTORED, LPARAM lParam = 0);

	// 弹出菜单初始化
	static VOID OnMenuPopup(WPARAM wParam, LPARAM lParam);

	// 菜单项被选择
	static VOID OnMenuSelect(UINT uCmd);

	// 命令处理
	static VOID OnCommand(WPARAM wParam, LPARAM lParam = 0);

	// 窗口销毁
	static VOID OnDestroy();

	// 打开文件
	static VOID OnFileOpen(PCTSTR ptzFileName = NULL);

	// 切换查看菜单状态
	static VOID OnViewMenu(UINT uCmd = -1, BOOL bInitialize = TRUE);

	// 改变语言
	static VOID OnViewLanguage(UINT uLang);

	// 创建或删除快捷方式
	static VOID OnHelpShortcut(UINT uCmd);

	// 最小化导系统托盘
	static VOID OnTrayIcon(BOOL bEnable = TRUE);

public:
	// 判断菜单项是否被选择
	inline static BOOL IsMenuChecked(UINT uCmd)
	{
		return ((MF_CHECKED & GetMenuState(m_hMenu, uCmd, MF_BYCOMMAND)) == MF_CHECKED);
	}

	// 使菜单和工具栏命令有效或无效
	inline static BOOL EnableCommand(UINT uCmd, BOOL bEnable = TRUE)
	{
		return EnableMenuItem(m_hMenu, uCmd, bEnable ? MF_ENABLED : MF_GRAYED);
	}

	// 使菜单和工具栏命令选择或不选择
	inline static BOOL CheckCommand(UINT uCmd, BOOL bChecked = TRUE)
	{
		return CheckMenuItem(m_hMenu, uCmd, bChecked ? MF_CHECKED : MF_UNCHECKED);
	}

	// 使菜单和工具栏命令单选选择
	inline static BOOL CheckRadioCommand(UINT uFirst, UINT uLast, UINT uCmd)
	{
		return CheckMenuRadioItem(m_hMenu, uFirst, uLast, uCmd, MF_BYCOMMAND);
	}

	// 消息框
	inline static INT MsgBox(PCTSTR ptzText = NULL, PCTSTR ptzCaption = STR_AppName, UINT uType = MB_ICONINFORMATION)
	{
		return MessageBox(m_hWnd, ptzText, ptzCaption, uType);
	}

	// 错误提示框
	inline static INT ErrorBox(PCTSTR ptzText = NULL, PCTSTR ptzCaption = STR_AppName)
	{
		return MsgBox(ptzText, ptzCaption, MB_ICONSTOP);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
