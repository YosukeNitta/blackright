#include "stdafx.h"

#include "CFileLoader.h"
#include <string>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#define _CRT_DISABLE_PERFCRIT_LOCKS

namespace gmlib
{

LRESULT CFileLoader::load(std::vector<byte> &out, const WCHAR *filename, DWORD offset, DWORD size)const
{
	out.swap(std::vector<byte>());
	std::wstring name = filename;

	//	�p�X���̐��K��.
	{
		int n = 0;
		while(-1 != (n = name.find('/', n)))
		{
			name = name.replace(n, 1, L"\\");
		}
		std::vector<WCHAR> p(name.size() + 1);
		if (!::PathCanonicalizeW(&p[0], name.c_str()))
		{
			//	������(�����O�ł͂Ȃ�)�̃p�X����260�ȏゾ�ƃG���[�ƂȂ���悤.
			return 1;
		}
		name = std::wstring(&p[0]);
	}

	FILE *fp = NULL;
	if (0 != _wfopen_s(&fp, name.c_str(), L"rb"))
	{
		return 2;
	}
	fseek(fp, 0, SEEK_END);
	DWORD fileSize = ftell(fp);
	if (fileSize < offset)
	{
		fclose(fp);
		return 3;
	}

	if (0 == size)
	{
		size = (fileSize - offset);
	}
	else if ((fileSize - offset) < size)
	{
		size = (fileSize - offset);
	}
	fseek(fp, offset, SEEK_SET);
	
	//	�ǂݍ��݃o�b�t�@���m��.
	std::vector<byte> buffer(size);

	if (0 < size)
	{
		DWORD readSize = fread(&buffer[0], 1, size, fp);
		if (readSize != size)
		{
			fclose(fp);
			return 4;
		}
	}
	fclose(fp);

	//	����Ƀf�[�^���擾�ł����̂ŁAswap���g��out�Ɣz��Q�Ƃ�����.
	out.swap(buffer);

	return 0;
}



}//namespace gmlib