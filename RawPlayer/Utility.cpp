


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "Main.h"
#include "Utility.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 获取特殊文件夹路径
BOOL CUtility::GetFolder(INT iFolder, PTSTR ptzFolder)
{
	BOOL bResult;
	LPMALLOC pMalloc;
	LPITEMIDLIST pIdl;

	_Assert(ptzFolder);
	bResult = FALSE;
	if (SHGetMalloc(&pMalloc) == S_OK)
	{
		if (SHGetSpecialFolderLocation(CMainWnd::m_hWnd, iFolder, &pIdl) == S_OK)
		{
			bResult = SHGetPathFromIDList(pIdl, ptzFolder);
			pMalloc->Free((PVOID) pIdl);
		}
		pMalloc->Release();
	}
	return bResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 获取图片文件夹
PTSTR CUtility::GetPicturesFolder(PTSTR ptzFolder, BOOL bIniFirst)
{
	_Assert(ptzFolder);

	if ((bIniFirst == FALSE) || (CIni::GetString(INI_PicturesFolder, ptzFolder) == 0))
	{
		if (GetFolder(CSIDL_MYPICTURES, ptzFolder) == FALSE)
		{
			if (GetFolder(CSIDL_PERSONAL, ptzFolder) == FALSE)
			{
				lstrcpy(ptzFolder, TEXT("C:"));
			}
			else
			{
				TrimBackslash(ptzFolder);
			}
			lstrcat(ptzFolder, TEXT("\\My Pictures"));
		}
	}

	return ptzFolder;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 浏览获取文件夹
BOOL CUtility::BrowseFolder(PTSTR ptzFolder, HWND hParent)
{
	BOOL bResult;
	LPMALLOC pMalloc;
	BROWSEINFO biBrowse;
	LPITEMIDLIST piilItem;

	bResult = FALSE;
	_Assert(ptzFolder);
	_ZeroMem(biBrowse);
	biBrowse.hwndOwner = hParent;
	biBrowse.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
	biBrowse.lpszTitle = LNG_BrowseFolder;
	biBrowse.lpfn = BrowseFolderProc;
	biBrowse.lParam = (LPARAM) ptzFolder;
	piilItem = SHBrowseForFolder(&biBrowse);
	if (piilItem)
	{
		bResult = SHGetPathFromIDList(piilItem, ptzFolder);

		if (SUCCEEDED(SHGetMalloc(&pMalloc)))
		{
			pMalloc->Free((PVOID) piilItem);
			pMalloc->Release();
		}
	}

	return bResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 浏览文件夹回调函数
INT CALLBACK CUtility::BrowseFolderProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 设置到剪贴板数据
BOOL CUtility::SetClipboard(UINT uFormat, PVOID pvData, UINT uSize)
{
	PVOID p;
	BOOL bResult;
	HGLOBAL hGlobal;

	bResult = FALSE;
	if (OpenClipboard(CMainWnd::m_hWnd))
	{
		EmptyClipboard();
		hGlobal = GlobalAlloc(GMEM_MOVEABLE, uSize);
		if (hGlobal)
		{
			p = GlobalLock(hGlobal);
			CopyMemory(p, pvData, uSize);
			GlobalUnlock(p);
			if (SetClipboardData(uFormat, hGlobal))
			{
				bResult = TRUE;
			}
			else
			{
				GlobalFree(hGlobal);
			}
		}
		CloseClipboard();
	}
	return bResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 设置到剪贴板数据
PVOID CUtility::GetClipboard(UINT uFormat)
{
	SIZE_T sztSize;
	PVOID pvResult;
	HGLOBAL hGlobal;
	PVOID pvClipboard;

	pvResult = NULL;
	if (OpenClipboard(CMainWnd::m_hWnd))
	{
		hGlobal = GetClipboardData(uFormat);
		if (hGlobal)
		{
			sztSize = GlobalSize(hGlobal);
			pvResult = _HeapAlloc(sztSize);
			if (pvResult)
			{
				pvClipboard = GlobalLock(hGlobal);
				CopyMemory(pvResult, pvClipboard, sztSize);
				GlobalUnlock(pvClipboard);
			}
		}
		CloseClipboard();
	}
	return pvResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 创建工具栏位图
HBITMAP CUtility::CreateToolbarBitmap(HBITMAP hBitmap, BOOL bDisabled)
{
	LONG i;
	LONG j;
	HDC hDC;
	PBYTE p;
	PBYTE pbBits;
	HBITMAP hResult;
	BITMAP bmBitmap;
	BITMAPINFOHEADER bihInfo;

	// 获取位图信息
	_ReturnValIf(GetObject(hBitmap, sizeof(BITMAP), &bmBitmap) == 0, NULL);

	// 设置位图信息头
	_ZeroMem(bihInfo);
	bihInfo.biSize = sizeof(BITMAPINFOHEADER);
	bihInfo.biWidth = bmBitmap.bmWidth;
	bihInfo.biHeight = bmBitmap.bmHeight;
	bihInfo.biPlanes = 1;
	bihInfo.biBitCount = 24;
	bihInfo.biSizeImage = _DibSize24(bihInfo.biWidth, bihInfo.biHeight);

	// 分配内存
	pbBits = (PBYTE) _HeapAlloc(bihInfo.biSizeImage);
	_ReturnValIf(pbBits == NULL, NULL);

	// 获取 DIB 数据
	hResult = NULL;
	hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	if (GetDIBits(hDC, hBitmap, 0, bihInfo.biHeight, pbBits, (PBITMAPINFO) &bihInfo, DIB_RGB_COLORS))
	{
		for (j = 0; j < bihInfo.biHeight; j++)
		{
			p = _DibBits24(pbBits, bihInfo.biWidth, 0, j);
			for (i = 0; i < bihInfo.biWidth; i++, p += 3)
			{
				if ((p[0] != 0xFF) || (p[1] != 0x00) && (p[2] != 0xFF))
				{
					if (bDisabled)
					{
						// 处理为灰度图象
						p[0] = p[1] = p[2] = (11 * p[0] + 59 * p[1] + 30 * p[2]) / 100;
						//p[0] = p[1] = p[2] = (p[0] + p[1] + p[2]) / 3;
					}
					else
					{
						// 减少图像亮度
						_ExIfElse(p[0] > 20, p[0] -= 20, p[0] = 0);
						_ExIfElse(p[1] > 20, p[1] -= 20, p[1] = 0);
						_ExIfElse(p[2] > 20, p[2] -= 20, p[2] = 0);
					}
				}
			}
		}
		// 创建位图
		hResult = CreateDIBitmap(hDC, &bihInfo, CBM_INIT, pbBits, (PBITMAPINFO) &bihInfo, DIB_RGB_COLORS);
	}

	// 释放
	DeleteDC(hDC);
	_HeapFree(pbBits);

	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 创建工具栏
HWND CUtility::CreateToolbar(DWORD dwStyle, HWND hParent, PCTSTR ptzBitmapName, HIMAGELIST *phImageList,
				   UINT uButtons, LPCTBBUTTON ptbbButtons)
{
	UINT i;
	HWND hToolbar;
	HBITMAP hBitmaps[3];
	const UINT MSG_ImageList[] = {TB_SETIMAGELIST, TB_SETHOTIMAGELIST, TB_SETDISABLEDIMAGELIST};

	// 创建工具栏
	hToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, dwStyle, 0, 0, 0, 0, 
		hParent, (HMENU) IDC_Toolbar, g_hInst, NULL);

	// 设置按钮结构大小
	SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);

	// 设置工具栏按钮位图
	hBitmaps[0] = LoadBitmap(g_hInst, ptzBitmapName);
	hBitmaps[1] = CreateToolbarBitmap(hBitmaps[0]);
	hBitmaps[2] = CreateToolbarBitmap(hBitmaps[0], TRUE);
	for (i = 0; i < 3; i++)
	{
		phImageList[i] = ImageList_Create(16, 16, ILC_COLOR16 | ILC_MASK, 1, 1);
		ImageList_AddMasked(phImageList[i], hBitmaps[i], RGB(0xFF, 0x00, 0xFF));
		DeleteObject(hBitmaps[i]);
		SendMessage(hToolbar, MSG_ImageList[i], 0, (LPARAM) phImageList[i]);
	}

	// 添加工具栏按钮
	SendMessage(hToolbar, TB_ADDBUTTONS, uButtons, (LPARAM) ptbbButtons);

	return hToolbar;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
