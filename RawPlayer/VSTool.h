


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CVSTool 类
class CVSTool
{
public:
	// 判断工具是否已经存在
	inline static BOOL Exist()
	{
		return Operate(FALSE, 0) || Operate(TRUE, 0);
	}

	// 添加工具
	inline static BOOL Add()
	{
		BOOL bResult = Operate(FALSE, 1);
		bResult = Operate(TRUE, 1) | bResult;
		return bResult;
	}

	// 删除工具
	inline static BOOL Remove()
	{
		BOOL bResult = Operate(FALSE, -1);
		bResult = Operate(TRUE, -1) | bResult;
		return bResult;
	}

private:
	// 枚举版本并操作
	static BOOL Operate(BOOL bDotNet, INT iCommand);

	// 针对已打开的工具键操作
	static BOOL Operate(HKEY hToolKey, BOOL bDotNet, INT iCommand);

	// 获取工具编号
	static DWORD Get(HKEY hToolKey, BOOL bDotNet, DWORD dwNumTools);

	// 添加工具
	static BOOL Add(HKEY hToolKey, DWORD dwNumTools, BOOL bDotNet, INT iCommand);

	// 更新工具
	static BOOL Update(HKEY hToolKey, BOOL bDotNet, DWORD dwIndex);

	// 删除工具
	static BOOL Remove(HKEY hToolKey, BOOL bDotNet, DWORD dwIndex, DWORD dwNumTools);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
