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
		DWORD			dwIndex;		//	ファイル番号.
		DWORD			dwFileOffset;	//	ファイル先頭オフセット値.
		DWORD			dwFileSize;		//	ファイルサイズ.
	}
	FileHeader;

	typedef std::map<std::wstring, FileHeader>	FileHeaderList;

	std::wstring	m_archiveFileName;	//	マウントしたアーカイブファイル. 絶対パス.
	FileHeaderList	m_headers;

	CArchiveLoader(){}

public:

	static CArchiveLoader *createInstance(const char *archiveFileName);
	static CArchiveLoader *createInstance(const WCHAR *archiveFileName);
	virtual LRESULT load(std::vector<byte> &out, const WCHAR *filename, DWORD offset = 0, DWORD size = 0)const;
};

}//namespace gmlib