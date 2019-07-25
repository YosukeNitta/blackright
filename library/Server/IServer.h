
#pragma once
#include <winsock2.h>
#include <windows.h>
#include <memory>

#pragma comment(lib, "ws2_32.lib")

namespace gmlib
{

class IServer
{

public:
	enum
	{
		SE_CREATE = 1,	//	SOCKET�̐����Ɏ��s���܂����B���\�[�X���N�����Ă���\��������܂��B
		SE_BIND,		//	bind�Ɏ��s���܂����B�w�肵���|�[�g�ԍ������Ɏg���Ă���\��������܂��B
		SE_LISTEN,		//	listen�Ɏ��s���܂����B���\�[�X���N�����Ă���\��������܂��B
	};

public:
	virtual ~IServer(){}

	/**
	�ڑ��҂��󂯂��J�n���܂��B
	@param	port	�҂��󂯃|�[�g�ԍ�
	@return			�G���[�R�[�h�BSE_XXXX
	*/
	virtual LRESULT start(WORD port) = 0;

	/**
	�ڑ��҂��󂯂��~���܂��B
	*/
	virtual void stop() = 0;

	/**
	�ڑ����m�����ꂽSOCKET���擾���܂��B
	@param	*addr	�ڑ���̏����i�[����\���̂ւ̃|�C���^�BNULL���w�肵���ꍇ�͎擾���Ȃ��B
	@return			�ڑ����m�����ꂽSOCKET�B
	*/
	virtual SOCKET accept(sockaddr_in *addr = NULL) = 0;
};


typedef std::tr1::shared_ptr<IServer> SServer;

}//namespace gmlib