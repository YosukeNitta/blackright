#include "stdafx.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include "IPathManager.h"
#include <vector>

namespace gmlib
{

LRESULT IPathManager::normalizePath(std::wstring &path)
{
	//	�p�X���R�s�[.
	std::wstring tmp = path;

	//	�S�Ă�'/'���A'\\'�ɒu�������邱�ƂŁA�p�X�L�@�ɓ��ꐫ����������.
	int n = 0;
	while(-1 != (n = tmp.find('/', n)))
	{
		tmp = tmp.replace(n, 1, L"\\");
	}

	//	�s�v��"."��".."�Ȃǂ̕������܂ރp�X�����A�����̕������܂܂Ȃ��p�X���ɕϊ�����.
	std::vector<WCHAR> p(tmp.size() + 1);
	if (!::PathCanonicalizeW(&p[0], tmp.c_str()))
	{
		return 1;
	}
	path = std::wstring(&p[0]);
	return 0;
}

}//namespace gmlib