


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 消息: 通知窗口将收到 WM_COMMAND 消息
// 参数: LOWORD(wParam) 为 IDC_BitLive，HIWORD(wParam) 为消息级别，lParam 为提示文本
// 返回: 调用 CBitLive::Cancel() 中止后继操作
#define IDC_BitLive			51426

#define BLL_Check			0	// 开始检查
#define BLL_CheckErr		1	// 检查失败
#define BLL_UpToDate		2	// 没有发现新版本

#define BLL_NewsBox			3	// 新闻消息框
#define BLL_News			4	// 新闻文本

#define BLL_Query			5	// 发现新版本，询问是否下载
#define BLL_Update			6	// 开始下载更新
#define BLL_Progress		7	// 下载进度
#define BLL_Confirm			8	// 确认应用更新
#define BLL_Exit			9	// 请求退出 (lParam 为 0)
#define BLL_UpdateErr		10	// 更新失败
#define BLL_UpdateEnd		11	// 更新结束
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBitLive 类
class CBitLive
{
private:
	static HWND m_hNotify;			// 通知消息窗口句柄
	static BOOL m_bCancel;			// 取消标识
	static HANDLE m_hThread;		// 更新线程句柄

public:
	// 开始检查更新
	inline static VOID Live(HWND hNotify)
	{
		DWORD dwThread;

		if (m_hThread == NULL)
		{
			SetNotify(hNotify);
			m_bCancel = FALSE;
			m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) Check, NULL, 0, &dwThread);
		}
	}

	// 设置通知消息窗口
	inline static VOID SetNotify(HWND hNotify)
	{
		m_hNotify = hNotify;
	}

	// 取消检查更新
	inline static VOID Cancel()
	{
		m_bCancel = TRUE;
	}

	// 终止检查更新
	inline static VOID Destroy()
	{
		if (m_hThread)
		{
			TerminateThread(m_hThread, 0);
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
	}

private:
	// 获取提示文本
	static BOOL GetPromptText(PCTSTR ptzFileName, PCTSTR ptzKeyName, PTSTR ptzText);

	// 检查更新
	static BOOL WINAPI Check();

	// 获取文件
	static DWORD GetFile(PCTSTR ptzFileName, PCTSTR ptzUrl, PCTSTR ptzHeader = NULL, BOOL bNotify = TRUE);

	// 更新
	static BOOL Update(PCTSTR ptzFileName, BOOL bReplaceMode);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
