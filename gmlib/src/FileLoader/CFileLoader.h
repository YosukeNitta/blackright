#pragma once

#include "IFileLoader.h"

namespace gmlib
{

class CFileLoader : public IFileLoader
{
public:
	virtual LRESULT load(std::vector<byte> &out, const WCHAR *filename, DWORD offset = 0, DWORD size = 0)const;
};

}//namespace gmlib