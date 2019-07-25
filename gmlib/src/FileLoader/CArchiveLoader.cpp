#include "stdafx.h"

#include "CArchiveLoader.h"
#include "CFileLoader.h"
#include <stdio.h>
#include <vector>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#define _CRT_DISABLE_PERFCRIT_LOCKS
using namespace std;

namespace gmlib
{

CArchiveLoader *CArchiveLoader::createInstance(const char *archiveFileName)
{
	int n = ::MultiByteToWideChar(CP_ACP, 0, archiveFileName, -1, NULL, 0);
	if (0 == n)
	{
		//	�ϊ��s�\�����񂾂����Ǝv����.
		return 0;
	}
	WCHAR *p = new WCHAR [n];
	n = ::MultiByteToWideChar(CP_ACP, 0, archiveFileName, -1, p, n);
	if (0 == n)
	{
		//	�ϊ��s�\�����񂾂����Ǝv����.
		delete [] p;
		return 0;
	}
	CArchiveLoader *ret = CArchiveLoader::createInstance(p);
	delete [] p;
	return ret;
}

CArchiveLoader *CArchiveLoader::createInstance(const WCHAR *archiveFileName)
{
	wstring name;

	//	���΃p�X�Ȃ�΁A��΃p�X�֕ύX.
	if (::PathIsRelativeW(archiveFileName))
	{
		DWORD size = ::GetCurrentDirectoryW(0, NULL);
		WCHAR *p = new WCHAR [size];
		::GetCurrentDirectoryW(size, p);
		name = wstring(p);
		delete [] p;

		//	�����`�F�b�N��ǉ�.
		//	"c:"�ȂǁA���[�g�f�B���N�g����GetCurrentDirectory���g�����ꍇ�̂݁A�����Ɋ���'\\'�����Ă��邽��.
		if (!name.empty() && '\\' != name[name.size() - 1])
		{
			name += '\\';
		}
		name += archiveFileName;
	}
	else
	{
		name = archiveFileName;
	}

	vector<byte> buffer;

	//	�w�b�_�ǂݍ���.
	CFileLoader loader;
	if (0 != loader.load(buffer, archiveFileName, 0, 8))
	{
		return NULL;
	}

	//	�}�W�b�N�i���o�[���`�F�b�N.
	if ('\0' != buffer[0] || 'A' != buffer[1] || 'R' != buffer[2] || 'C' != buffer[3])
	{
		return NULL;
	}

	int headerSize;
	headerSize = *(int*)&buffer[4];

	//	�w�b�_�[��(����A�}�W�b�N�i���o�[�ƃw�b�_�[�T�C�Y�̈������������)��ǂݍ���.
	if (0 != loader.load(buffer, archiveFileName, 8, headerSize - 8))
	{
		return NULL;
	}

	//	���.
	int fileNum = *(int*)&buffer[0];
	vector<wstring>	filePaths;
	vector<int>		fileSizes;
	filePaths.reserve(fileNum);
	fileSizes.reserve(fileNum);

	DWORD i = 4;
	for(int n = 0; n < fileNum; ++n)
	{
		int length = *(int*)&buffer[i];
		i += sizeof(int);
		
		filePaths.push_back(wstring((const WCHAR*)&buffer[i], 0, length / sizeof(WCHAR)));
		i += length;
		
		fileSizes.push_back(*(int*)&buffer[i]);
		i += sizeof(int);
	}

	//	�t�@�C���w�b�_�̃��X�g���쐬����.
	FileHeaderList headers;

	//	�t�@�C���擪�ւ̃I�t�Z�b�g�l.
	//	�����l�̓w�b�_�[�T�C�Y��.
	DWORD offset = headerSize;
	for(int n = 0; n < fileNum; ++n)
	{
		FileHeader h;
		h.dwIndex = n;
		h.dwFileOffset = offset;
		h.dwFileSize = fileSizes[n];

		headers.insert(pair<wstring, FileHeader>(filePaths[n], h));

		offset += fileSizes[n];
	}

	CArchiveLoader *p = new CArchiveLoader;
	p->m_archiveFileName = name;
	p->m_headers = headers;
	return p;
}


LRESULT CArchiveLoader::load(vector<byte> &out, const WCHAR *filename, DWORD offset, DWORD size)const
{
	out.swap(vector<byte>());
	wstring name = filename;

	//	�p�X���̐��K��.
	{
		int n = 0;
		while(-1 != (n = name.find('/', n)))
		{
			name = name.replace(n, 1, L"\\");
		}
		vector<WCHAR> p(name.size() + 1);
		if (!::PathCanonicalizeW(&p[0], name.c_str()))
		{
			return 1;
		}
		name = wstring(&p[0]);
	}

	//	��΃p�X�ł̓��[�h�ł��Ȃ�.
	if (!::PathIsRelativeW(name.c_str()))
	{
		return 2;
	}

	//	�t�@�C�����Ō���.
	FileHeaderList::const_iterator it = m_headers.find(name);
	if (m_headers.end() == it)
	{
		return 3;
	}

	//	�����Ƀq�b�g�����t�@�C���̃w�b�_�����Q��.
	const FileHeader &header = it->second;

	//	�I�t�Z�b�g���t�@�C���T�C�Y�ȏ�Ȃ�G���[.
	if (header.dwFileSize < offset)
	{
		return 4;
	}

	if (0 == size)
	{
		size = (header.dwFileSize - offset);
	}
	else if ((header.dwFileSize - offset) < size)
	{
		size = (header.dwFileSize - offset);
	}

	vector<byte> buffer;
	if (0 < header.dwFileSize)
	{
		CFileLoader loader;
		if (0 != loader.load(buffer, m_archiveFileName.c_str(), header.dwFileOffset + offset, size))
		{
			return 5;
		}
	}
	else
	{
		buffer.resize(0);
	}

	//	����Ƀf�[�^���擾�ł����̂ŁAswap���g��out�Ɣz��Q�Ƃ�����.
	out.swap(buffer);
	return 0;
}


}//namespace gmlib