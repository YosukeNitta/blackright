#pragma once

#pragma warning (disable:4786)
#include "IFileLoader.h"
#include <string>
#include <map>

namespace gmlib
{

class CArchiveLoader : public IFileLoader
{

protected:
	typedef struct
	{
		DWORD			dwIndex;		//	�t�@�C���ԍ�.
		DWORD			dwFileOffset;	//	�t�@�C���擪�I�t�Z�b�g�l.
		DWORD			dwFileSize;		//	�t�@�C���T�C�Y.
	}
	FileHeader;

	typedef std::map<std::wstring, FileHeader>	FileHeaderList;

	std::wstring	m_archiveFileName;	//	�}�E���g�����A�[�J�C�u�t�@�C��. ��΃p�X.
	FileHeaderList	m_headers;

	CArchiveLoader(){}

public:

	static CArchiveLoader *createInstance(const char *archiveFileName);
	static CArchiveLoader *createInstance(const WCHAR *archiveFileName);
	virtual LRESULT load(std::vector<byte> &out, const WCHAR *filename, DWORD offset = 0, DWORD size = 0)const;
};

}//namespace gmlib