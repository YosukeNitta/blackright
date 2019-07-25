

#pragma once
#include <windows.h>
#include <vector>
#include <memory>

namespace gmlib
{

class IFileLoader
{
public:
	/**
	デストラクタ.
	*/
	virtual ~IFileLoader(){};

	/**
	ファイルをロードする.
	@param	out			ロードしたデータを格納する配列への参照. ロードに失敗した場合は空になります.
	@param	filename	ファイル名.
	@param	offset		読み込み開始位置オフセット. ファイルサイズよりも大きい値が指定されていた場合、エラーとなる.
	@param	size		読み込むサイズ制限. 0を指定した場合はファイルの終端まで読み込む.
	@return				エラーコード.
	*/
	virtual LRESULT load(std::vector<byte> &out, const WCHAR *filename, DWORD offset = 0, DWORD size = 0)const = 0;

};

typedef std::tr1::shared_ptr<IFileLoader> SFileLoader;

}//namespace gmlib