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
		//	���΃p�X�̏ꍇ�A�J�����g�p�X�ƘA������.
		path = getPath();
		path += pszPath;
	}
	else
	{
		//	��΃p�X�̏ꍇ�́A�J�����g�p�X�͖�������.
		path = pszPath;
	}

	//	�p�X�̐��K�����s��.
	if (0 != normalizePath(path))
	{
		return 1;
	}
	//	�����ɂ�'\\'���K������悤�ɂ��Ă���.
	if (!path.empty() && '\\' != path[path.size() - 1])
	{
		path += '\\';
	}

	//	�X�^�b�N�Ƀp�X��o�^.
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