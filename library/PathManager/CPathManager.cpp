#include "pch.h" 

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include "CPathManager.h"

namespace gmlib
{

LRESULT CPathManager::pushPath(const WCHAR *pszPath)
{
	std::wstring path;
	if (::PathIsRelativeW(pszPath))
	{
		//	相対パスの場合、カレントパスと連結する.
		path = getPath();
		path += pszPath;
	}
	else
	{
		//	絶対パスの場合は、カレントパスは無視する.
		path = pszPath;
	}

	//	パスの正規化を行う.
	if (0 != normalizePath(path))
	{
		return 1;
	}
	//	末尾には'\\'が必ずあるようにしておく.
	if (!path.empty() && '\\' != path[path.size() - 1])
	{
		path += '\\';
	}

	//	スタックにパスを登録.
	m_paths.push_back(path);
	return 0;
}

LRESULT CPathManager::popPath()
{
	if (m_paths.empty())
	{
		return 1;
	}
	m_paths.pop_back();
	return 0;
}

const WCHAR *CPathManager::getPath()const
{
	const static WCHAR tmp[] = L"";
	if (m_paths.empty())
		return tmp;
	return m_paths.back().c_str();
}


}//namespace gmlib