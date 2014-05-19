


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 头文件
#include "Define.h"

#include <Windows.h>
#include <CommCtrl.h>
#include <ShlObj.h>
#include <Malloc.h>
#include <AviRiff.h>
#include <MMSystem.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 保持与 Visual C++ 6.0 兼容
#if (_MSC_VER < 1300)
typedef LPCSTR							PCSTR;
typedef LPCSTR							PCSTR;
typedef LPSTR							PSTR;
typedef LPTSTR							PTSTR;
typedef LPCTSTR							PCTSTR;
typedef LPVOID							PVOID;
#if defined(_WIN64)
typedef LONGLONG						LONG_PTR;
#else
typedef LONG							LONG_PTR;
#endif
#endif

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL					(WM_MOUSELAST + 1)
#define WHEEL_DELTA						120
#endif
#ifndef MIIM_STRING
#define MIIM_STRING						0x00000040
#endif
#ifndef IDC_HAND
#define IDC_HAND						MAKEINTRESOURCE(32649)
#endif
#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE				0x0040
#endif
#ifndef CSIDL_MYPICTURES
#define CSIDL_MYPICTURES				0x0027
#endif
#ifndef GetClassLongPtr
#define GetClassLongPtr					GetClassLong
#define SetClassLongPtr					SetClassLong
#define GCLP_HCURSOR					GCL_HCURSOR
#endif
#ifndef GetWindowLongPtr
#define GetWindowLongPtr				GetWindowLong
#define SetWindowLongPtr				SetWindowLong
#define GWLP_USERDATA					GWL_USERDATA
#define GWLP_WNDPROC					GWL_WNDPROC
#define DWLP_MSGRESULT					DWL_MSGRESULT
#define GCLP_HBRBACKGROUND				GCL_HBRBACKGROUND
#endif
#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES			-1
#endif
#ifndef VK_OEM_PLUS
#define VK_OEM_PLUS						0xBB
#define VK_OEM_COMMA					0xBC
#define VK_OEM_MINUS					0xBD
#define VK_OEM_PERIOD					0xBE
#endif
#ifndef IS_INTRESOURCE
#define IS_INTRESOURCE(r)				(((ULONG) (r) >> 16) == 0)
#endif
#ifndef CDSIZEOF_STRUCT
#define CDSIZEOF_STRUCT(s, m)			(((INT) ((PBYTE) (&((s *) 0)->m) - ((PBYTE) ((s *) 0)))) + sizeof(((s *) 0)->m))
#endif
#ifndef LVM_SORTITEMSEX
#define LVM_SORTITEMSEX					(LVM_FIRST + 81)
#define ListView_SortItemsEx(h, f, p)	(BOOL) SNDMSG((h), LVM_SORTITEMSEX, (WPARAM) (p), (LPARAM) (f))
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 头文件
#include "Macro.h"
#include "Resource.h"

#include "MainWnd.h"
#include "OpenDlg.h"
#include "ExportDlg.h"
#include "ChildWnd.h"
#include "ClientWnd.h"
#include "AboutDlg.h"

#include "Ini.h"
#include "VSTool.h"
#include "BitLive.h"
#include "Utility.h"
#include "Shortcut.h"
#include "Language.h"
#include "RecentFile.h"

#include "RawPlayer.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 全局变量
extern HINSTANCE g_hInst;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INI 键名
#define INI_Language			TEXT("Language")
#define INI_WindowLeft			TEXT("WindowLeft")
#define INI_WindowTop			TEXT("WindowTop")
#define INI_WindowWidth			TEXT("WindowWidth")
#define INI_WindowHeight		TEXT("WindowHeight")
#define INI_WindowState			TEXT("WindowState")
#define INI_RecentFile			TEXT("RecentFile")
#define INI_MaxRecentFile		TEXT("MaxRecentFile")
#define INI_Toolbar				TEXT("Toolbar")
#define INI_StatusBar			TEXT("StatusBar")
#define INI_AlwaysOnTop			TEXT("AlwaysOnTop")
#define INI_MinToTray			TEXT("MinToTray")

#define INI_BitLive				TEXT("BitLive")
#define INI_UpdateBuild			TEXT("UpdateBuild")
#define INI_UpdateUrl			TEXT("UpdateUrl")
#define INI_UpdateBox			TEXT("UpdateBox")
#define INI_UpdateText			TEXT("UpdateText")
#define INI_UpdateSize			TEXT("UpdateSize")
#define INI_UpdateReplace		TEXT("UpdateReplace")
#define INI_UpdateExit			TEXT("UpdateExit")
#define INI_NewsBox				TEXT("NewsBox")
#define INI_NewsText			TEXT("NewsText")

#define INI_OpenPeview			TEXT("OpenPreview")
#define INI_PicturesFolder		TEXT("PicturesFolder")
#define INI_ClientColor			TEXT("ClientColor")
#define INI_ShowSplash			TEXT("ShowSplash")
#define INI_ShowOpen			TEXT("ShowOpen")

#define INI_HistroyFormat		TEXT("HistroyFormat")
#define INI_ExportFormat		TEXT("ExportFormat")
#define INI_ExportRange			TEXT("ExportRange")
#define INI_PlayOnOpen			TEXT("PlayOnOpen")
#define INI_MaxOnOpen			TEXT("MaxOnOpen")
#define INI_DirectDraw			TEXT("DirectDraw")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 语言相关字字符串
#ifdef _CHS

#define LNG_Ready				_Lang(100, "就绪。")
#define LNG_Version				_Lang(101, "版本: %s")
#define LNG_Build				_Lang(102, "编译: %s")

#define	LNG_CheckUpdate			_Lang(103, "检查更新")
#define LNG_CancelUpdate		_Lang(104, "取消更新")
#define	LNG_VisitWeb			_Lang(105, "访问网站")
#define	LNG_WriteEmail			_Lang(106, "给作者写信")

#define LNG_CheckUpdateTip		_Lang(107, "在 Internet 上检查本程序的更新版本。")
#define LNG_VisitWebTip			_TLang(108, TEXT("访问 ") STR_Web TEXT("。"))
#define LNG_WriteEmailTip		_TLang(109, TEXT("写信给 ") STR_Email TEXT("。"))

#define LNG_BitLiveCheck		_Lang(110, "开始检查更新...")
#define LNG_BitLiveUpdate		_Lang(111, "开始获取更新...")
#define LNG_BitLiveUpToDate		_Lang(112, "您所使用的已经是最新版本。")
#define LNG_BitLiveCheckErr		_Lang(113, "检查更新过程中出现错误。")
#define LNG_BitLiveUpdateErr	_Lang(114, "更新未能成功完成。")
#define LNG_BitLiveRecieve		_Lang(115, "已收到 %u 字节...")
#define LNG_BitLiveRecvPercent	_Lang(116, "已收到 %u 字节 (%u%%)...")
#define LNG_BitLiveConfirm		_LangEx(117, "新版本已经下载完成，并保存为:\n\n    %s\n\n现在要应用更新吗?")

#else

#define LNG_Ready				_Lang(100, "Ready.")
#define LNG_Version				_Lang(101, "Ver: %s")
#define LNG_Build				_Lang(102, "Build: %s")

#define	LNG_CheckUpdate			_Lang(103, "Check Update")
#define	LNG_CancelUpdate		_Lang(104, "Cancel Update")
#define	LNG_VisitWeb			_Lang(105, "Visit Web Site")
#define	LNG_WriteEmail			_Lang(106, "Email To Author")

#define LNG_CheckUpdateTip		_Lang(107, "Check new version via Internet.")
#define LNG_VisitWebTip			_TLang(108, TEXT("Visit ") STR_Web TEXT("."))
#define LNG_WriteEmailTip		_TLang(109, TEXT("Email to ") STR_Email TEXT("."))

#define LNG_BitLiveCheck		_Lang(110, "Begin BitLive...")
#define LNG_BitLiveUpdate		_Lang(111, "Begin LiveUpdate...")
#define LNG_BitLiveUpToDate		_Lang(112, "The current version is up to date.")
#define LNG_BitLiveCheckErr		_Lang(113, "Failed to check update.")
#define LNG_BitLiveUpdateErr	_Lang(114, "LiveUpdate completed unsuccessfully.")
#define LNG_BitLiveRecieve		_Lang(115, "Recieved %u bytes ...")
#define LNG_BitLiveRecvPercent	_Lang(116, "Recieved %u bytes (%u%%) ...")
#define LNG_BitLiveConfirm		_LangEx(117, "The update version has been downloaded successfully:\n\n    %s\n\nApply this update now?")

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 语言相关字符串
#ifdef _CHS
#define LNG_ExportFilter		_LangEx(200, "YV12 文件 (*.yv12;*.yuv)\0*.yv12;*.yuv\0I420 文件 (*.i420;*.yuv)\0*.i420;*.yuv\0YUY2 文件 (*.yuy2;*.yuv)\0*.yuy2;*.yuv\0YVYU 文件 (*.yvyu;*.yuv)\0*.yvyu;*.yuv\0UYVY 文件 (*.uyvy;*.yuv)\0*.uyvy;*.yuv\0VYUY 文件 (*.vyuy;*.yuv)\0*.vyuy;*.yuv\0RGB32 文件 (*.rgb32;*.rgb)\0*.rgb32;*.rgb\0RGB24 文件 (*.rgb24;*.rgb)\0*.rgb24;*.rgb\0RGB16 文件 (*.rgb16;*.rgb)\0*.rgb16;*.rgb\0RGB15 文件 (*.rgb15;*.rgb)\0*.rgb15;*.rgb\0所有文件 (*.*)\0*.*\0位图文件 (*.bmp)\0*.bmp\0")
#define LNG_BrowseFolder		_Lang(201, "请选择文件夹的位置。")

#define LNG_OpenErr				_Lang(300, "无法打开文件 %s。")
#define ERR_HelpErr				_Lang(301, "打开帮助文件失败，请重新安装 MVPlayer。")
#define LNG_CreateShortcutErr	_Lang(302, "创建快捷方式失败。")
#define LNG_RemoveShortcutErr	_Lang(303, "删除快捷方式失败。")

#define LNG_ExportErr			_Lang(304, "导出操作未能成功完成。")
#define LNG_AddToVSErr			_Lang(305, "创建快捷方式失败，可能没有安装 Microsoft Visual Studio。")
#define LNG_RemoveFromVSErr		_Lang(306, "从 Microsoft Visual Studio 中具菜单中删除快捷方式失败。")
#define LNG_InputErr			_Lang(307, "不能接受您的输入。")
#define LNG_UnexpectedErr		_Lang(308, "出现了未知错误。")
#define LNG_DirectDrawErr		_Lang(309, "DirectDraw 出现错误: %#08X，或许是你的显卡不支持 YUV 表面。")

#define LNG_CreateShortcutMsg	_Lang(400, "已经成功创建快捷方式。")
#define LNG_RemoveShortcutMsg	_Lang(401, "已经成功删除快捷方式。")
#define LNG_ConvertMsg			_Lang(402, "正在转换第 %u 帧 (按 Esc 取消)...")
#define LNG_ExportMsg			_Lang(403, "导出操作成功完成，总共耗时 %u 毫秒。")
#define LNG_AddToVSMsg			_LangEx(404, "已经在 Microsoft Visual Studio 工具菜单中创建快捷方式。\n\n请确保 Microsoft Visual Studio 5.x/6.x 没有运行，否则可能无法保存快捷方式。")
#define LNG_RemoveFromVSMsg		_LangEx(405, "已经从 Microsoft Visual Studio 工具菜单中删除快捷方式。\n\n请确保 Microsoft Visual Studio 5.x/6.x 没有运行，否则可能无法删除快捷方式。")

#define LNG_ColorSpaceMsg		_LangEx(406, "请键入色系 (%s)，按 Enter 确认，Esc 取消。")
#define LNG_FrameWidthMsg		_LangEx(407, "请键入宽度 (%d - %d)，按 Enter 确认，Esc 取消。")
#define LNG_FrameHeightMsg		_LangEx(408, "请键入高度 (-%d - -%d, %d - %d)，按 Enter 确认，Esc 取消。")
#define LNG_FrameRateMsg		_LangEx(409, "请键入帧率 (%d - %d)，按 Enter 确认，Esc 取消。")
#define LNG_FrameStepMsg		_LangEx(410, "请键入帧幅 (%d - %d)，按 Enter 确认，Esc 取消。")
#define LNG_FrameIndexeMsg		_LangEx(411, "请键入要转跳到的帧 (%d - %d)，按 Enter 确认，Esc 取消。")

#else // _CHS

#define LNG_ExportFilter		_LangEx(200, "YV12 Files (*.yv12;*.yuv)\0*.yv12;*.yuv\0I420 Files (*.i420;*.yuv)\0*.i420;*.yuv\0YUY2 Files (*.yuy2;*.yuv)\0*.yuy2;*.yuv\0YVYU Files (*.yvyu;*.yuv)\0*.yvyu;*.yuv\0UYVY Files (*.uyvy;*.yuv)\0*.uyvy;*.yuv\0VYUY Files (*.vyuy;*.yuv)\0*.vyuy;*.yuv\0RGB32 Files (*.rgb32;*.rgb)\0*.rgb32;*.rgb\0RGB24 Files (*.rgb24;*.rgb)\0*.rgb24;*.rgb\0RGB16 Files (*.rgb16;*.rgb)\0*.rgb16;*.rgb\0RGB15 Files (*.rgb15;*.rgb)\0*.rgb15;*.rgb\0All Files (*.*)\0*.*\0Bitmap Files (*.bmp)\0*.bmp\0")
#define LNG_BrowseFolder		_Lang(201, "Please select folder location.")

#define LNG_OpenErr				_Lang(300, "Failed to open file %s.")
#define ERR_HelpErr				_Lang(301, "Failed to open help file. Please re-install MVPlayer.")
#define LNG_CreateShortcutErr	_Lang(302, "Failed to create shortcut.")
#define LNG_RemoveShortcutErr	_Lang(303, "Failed to remove shortcut.")

#define LNG_ExportErr			_Lang(304, "Export operation complete unsuccessfully.")
#define LNG_AddToVSErr			_Lang(305, "Failed to create shortcut. Probably Microsoft Visual Studio is not installed.")
#define LNG_RemoveFromVSErr		_Lang(306, "Failed to remove shortcut from Microsoft Visual Studio.")
#define LNG_InputErr			_Lang(307, "Could not accept your input.")
#define LNG_UnexpectedErr		_Lang(308, "An unexpected error occurred.")
#define LNG_DirectDrawErr		_Lang(309, "DirectDraw error occurred: %#08X. Maybe your video card could not support YUV surface.")

#define LNG_CreateShortcutMsg	_Lang(400, "Shortcut has been created successfully.")
#define LNG_RemoveShortcutMsg	_Lang(401, "Shortcut has been removed successfully.")
#define LNG_ConvertMsg			_Lang(402, "Converting %u (Press Esc to cancel)...")
#define LNG_ExportMsg			_Lang(403, "Export operation completed in %u ms.")
#define LNG_AddToVSMsg			_LangEx(404, "Shortcut has been created successfully.\n\nBe sure that Microsoft Visual Studio 5.x/6.x is not running, otherwise the shortcut may not be saved.\n\n")
#define LNG_RemoveFromVSMsg		_LangEx(405, "Shortcut has been removed successfully.\n\nBe sure that Microsoft Visual Studio 5.x/6.x is not running, otherwise the shortcut may not be removed.\n\n")

#define LNG_ColorSpaceMsg		_LangEx(406, "Input color space (%s). Enter to confirm, Esc to cancel.")
#define LNG_FrameWidthMsg		_LangEx(407, "Input frame width (%d - %d). Enter to confirm, Esc to cancel.")
#define LNG_FrameHeightMsg		_LangEx(408, "Input frame height (-%d - -%d, %d - %d). Enter to confirm, Esc to cancel.")
#define LNG_FrameRateMsg		_LangEx(409, "Input frame rate (%d - %d). Enter to confirm, Esc to cancel.")
#define LNG_FrameStepMsg		_LangEx(410, "Input frame step (%d - %d). Enter to confirm, Esc to cancel.")
#define LNG_FrameIndexeMsg		_LangEx(411, "Input frame index that you want to go (%d - %d). Enter to confirm, Esc to cancel.")

#endif // _CHS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
