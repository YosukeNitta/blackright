#include "stdafx.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include "IPathManager.h"
#include <vector>

namespace gmlib
{

LRESULT IPathManager::normalizePath(std::wstring &path)
{
	//	パスをコピー.
	std::wstring tmp = path;

	//	全ての'/'を、'\\'に置き換えることで、パス記法に統一性を持たせる.
	int n = 0;
	while(-1 != (n = tmp.find('/', n)))
	{
		tmp = tmp.replace(n, 1, L"\\");
	}

	//	不要な"."や".."などの文字を含むパス名を、これらの文字を含まないパス名に変換する.
	std::vector<WCHAR> p(tmp.size() + 1);
	if (!::PathCanonicalizeW(&p[0], tmp.c_str()))
	{
		return 1;
	}
	path = std::wstring(&p[0]);
	return 0;
}

}//namespace gmlib