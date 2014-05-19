
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#pragma warning(disable: 4163 4995 4996)
#ifdef __ICL
#pragma warning(disable: 1478)
#endif

//#define _BETA
//#define _CHS
//#define _MAKELANG

#if (defined(_M_AMD64) && !defined(_M_X64))
#define _M_X64
#endif

#if (defined(_M_X64) || defined(_M_IA64))
#define _WINVER					0x0500
#else
#define _WINVER					0x0400
#endif

#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 字符串化
#define _String(v)				#v
#define _DotJoin(a, b, c, d)	_String(a.b.c.d)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 版本信息
#define VER_Major				2
#define VER_Minor				2
#define VER_Release				222 
#define VER_Build				700   
#define VER_Version				MAKELONG(MAKEWORD(VER_Major, VER_Minor), VER_Release)
#define STR_Version				TEXT(_DotJoin(VER_Major, VER_Minor, VER_Release, VER_Build))

#if defined(_M_IA64)
#define STR_VersionStamp		STR_Version TEXT(" (IA64)")
#elif defined(_M_X64)
#define STR_VersionStamp		STR_Version TEXT(" (X64)")
#elif defined(_UNICODE)
#define STR_VersionStamp		STR_Version TEXT(" (X86U)")
#else
#define STR_VersionStamp		STR_Version TEXT(" (X86)")
#endif

#define STR_BuildDate			TEXT(__DATE__)
#define STR_BuildTime			TEXT(__TIME__)
#define STR_BuilStamp			TEXT(__DATE__) TEXT(" ") TEXT(__TIME__)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 程序信息
#define STR_AppName				TEXT("RawPlayer")
#define STR_Corporation			TEXT("Yonsm.NET")
#define STR_Author				TEXT("Yonsm")
#define STR_Web					TEXT("WWW.Yonsm.NET")
#define STR_Email				TEXT("Yonsm@msn.com")
#define STR_WebUrl				TEXT("http://www.yonsm.net")
#define STR_EmailUrl			TEXT("mailto:") STR_Email TEXT("?Subject=") STR_AppName
#define STR_Description			TEXT("Raw Video Player")
#define STR_Copyright			TEXT("Copyright (C) 2004-2010 ") STR_Corporation TEXT(". All rights reserved.")

#ifdef _CHS
#define LNG_Description			_TLang(10, TEXT("原始视频播放器"))
#define LNG_Copyright			_TLang(11, TEXT("版权所有 (C) 2004-2010 ") STR_Corporation TEXT("，保留所有权利"))
#else
#define LNG_Description			_TLang(10, STR_Description)
#define LNG_Copyright			_TLang(11, STR_Copyright)
#endif

#define STR_ExtList				TEXT(".yuv\0.yv12\0.i420\0.yuy2\0.yvyu\0.uyvy\0.vyuy\0.rgb32\0.rgb24\0.rgb16\0.rgb15\0.rgb\0")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 扩展名
#define EXT_Bat					TEXT(".bat")
#define EXT_Chm					TEXT(".chm")
#define EXT_Dll					TEXT(".dll")
#define EXT_Exe					TEXT(".exe")
#define EXT_Ini					TEXT(".ini")
#define EXT_Lnk					TEXT(".lnk")
#define EXT_Lng					TEXT(".lng")
#define EXT_Bmp					TEXT(".bmp")
#define EXT_Jpg					TEXT(".jpg")
#define EXT_Yuv					TEXT(".yuv")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
