#include "stdafx.h"

#include "CVirtualFileLoader.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
using namespace std;

namespace gmlib
{

CVirtualFileLoader::LoaderPair::LoaderPair(SFileLoader _loader, std::wstring _path)
:path(_path)
,loader(_loader)
{
	if (!path.empty() && '/' != path[path.size() - 1] && '\\' != path[path.size() - 1])
	{
		path += '/';
	}
}

LRESULT CVirtualFileLoader::load(std::vector<byte> &out, const WCHAR *filename, DWORD offset, DWORD size)const
{
	if (NULL == filename)
	{
		return 1;
	}

	std::wstring name;
	if (::PathIsRelativeW(filename))
	{
		//	相対パス.
		name = getPath();
		name += filename;

		std::vector< LoaderPair >::const_reverse_iterator it = m_loaders.rbegin();
		while(m_loaders.rend() != it)
		{
			const LoaderPair &pair = (*it);
			if (0 == pair.loader->load(out, (pair.path + name).c_str(), offset, size))
			{
				return 0;
			}
			++it;
		}
	}
	else
	{
		//	絶対パス.
		name = filename;

		//	絶対パスの場合、デフォルトパスを無視する.
		std::vector< LoaderPair >::const_reverse_iterator it = m_loaders.rbegin();
		while(m_loaders.rend() != it)
		{
			const LoaderPair &pair = (*it);
			if (0 == pair.loader->load(out, name.c_str(), offset, size))
			{
				return 0;
			}
			++it;
		}
	}
	return 2;
}

LRESULT CVirtualFileLoader::pushLoader(SFileLoader loader, const char *path)
{
	if (NULL == loader || NULL == path)
	{
		return 1;
	}

	int n = ::MultiByteToWideChar(CP_ACP, 0, path, -1, NULL, 0);
	if (0 == n)
	{
		//	変換不可能文字列だったと思われる.
		return 2;
	}
	WCHAR *p = new WCHAR [n];
	n = ::MultiByteToWideChar(CP_ACP, 0, path, -1, p, n);
	if (0 == n)
	{
		//	変換不可能文字列だったと思われる.
		delete [] p;
		return 3;
	}
	LRESULT ret = pushLoader(loader, p);
	delete [] p;
	return ret;
}

LRESULT CVirtualFileLoader::pushLoader(SFileLoader loader, const WCHAR *path)
{
	if (NULL == loader || NULL == path)
	{
		return 1;
	}
	
	m_loaders.push_back(LoaderPair(loader, path));
	return 0;
}

LRESULT CVirtualFileLoader::popLoader()
{
	if (m_loaders.empty())
	{
		return 1;
	}

	m_loaders.pop_back();
	return 0;
}


}//namespace gmlib