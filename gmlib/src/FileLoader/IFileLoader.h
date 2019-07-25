

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
	�f�X�g���N�^.
	*/
	virtual ~IFileLoader(){};

	/**
	�t�@�C�������[�h����.
	@param	out			���[�h�����f�[�^���i�[����z��ւ̎Q��. ���[�h�Ɏ��s�����ꍇ�͋�ɂȂ�܂�.
	@param	filename	�t�@�C����.
	@param	offset		�ǂݍ��݊J�n�ʒu�I�t�Z�b�g. �t�@�C���T�C�Y�����傫���l���w�肳��Ă����ꍇ�A�G���[�ƂȂ�.
	@param	size		�ǂݍ��ރT�C�Y����. 0���w�肵���ꍇ�̓t�@�C���̏I�[�܂œǂݍ���.
	@return				�G���[�R�[�h.
	*/
	virtual LRESULT load(std::vector<byte> &out, const WCHAR *filename, DWORD offset = 0, DWORD size = 0)const = 0;

};

typedef std::tr1::shared_ptr<IFileLoader> SFileLoader;

}//namespace gmlib