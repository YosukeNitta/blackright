
#pragma once
#include <windows.h>
#include <memory>

namespace gmlib
{

/**
�X���b�h�������N���X�̃C���^�[�t�F�[�X�B
*/
class IThread
{
protected:
	/**
	�Ăяo�����X���b�h����莞�Ԃ������f���܂��B
	�������Astop()�y��stopAndWait()���Ă΂ꂽ�ꍇ�͑����ɍĊJ���܂��B
	�܂��Astop()�y��stopAndWait()���Ă΂ꂽ��́A���̃��\�b�h�͖���������܂��B
	@param	milliseconds	���f���鎞�Ԃ��w��B0xFFFFFFFF���w�肳�ꂽ�ꍇ�͖������ƂȂ�܂��B
	*/
	virtual void sleep(DWORD milliseconds) = 0;

public:
	/**
	�f�X�g���N�^�B
	�X���b�h���I������܂őҋ@���܂��B
	*/
	virtual ~IThread(){}

	/**
	�X���b�h���N������B
	�Ȍケ�̃X���b�h�͗L����ԂƂȂ�AisValid()��true��Ԃ��悤�ɂȂ�B
	�����Ȃ�ꍇ�ł��A2�x�N�����邱�Ƃ͂ł��܂���B
	2�x�ڈȍ~�̂��̊֐��Ăяo���͖�������܂��B
	*/
	virtual void start() = 0;

	/**
	�X���b�h����~����悤���߂���B���̊֐��͎��ۂ̒�~��҂����ɏ������I������B
	�Ȍケ�̃X���b�h�͖�����ԂƂȂ�AisValid()��false��Ԃ��悤�ɂȂ�B
	*/
	virtual void stop() = 0;
	
	/**
	�X���b�h����~����悤���߂��A���ۂɒ�~����܂ő҂B
	�Ȍケ�̃X���b�h�͖�����ԂƂȂ�AisValid()��false��Ԃ��悤�ɂȂ�B
	*/
	virtual void stopAndWait() = 0;

	/**
	�X���b�h���N�����Ă����Ԃ��ǂ������擾����B
	@return	�N�����Ă���ꍇ��true��Ԃ��B
	*/
	virtual bool isRunning()const = 0;

	/**
	�X���b�h���L����Ԃ��ǂ������擾����B
	�L����ԂȂ̂́Astart()���Ă΂�Ă���Astop()���Ă΂��܂ł̊Ԃł���B
	@return	�L����Ԃ̏ꍇ��true��Ԃ��B
	*/
	virtual bool isValid()const = 0;

	/**
	start()�ɂ���ċN�������X���b�h�ɁA���ۂɏ�����������e�ł��B�I�[�o�[���C�h���Ă��������B
	*/
	virtual void run() = 0;
};

typedef std::tr1::shared_ptr<IThread> SThread;

}//namespace gmlib