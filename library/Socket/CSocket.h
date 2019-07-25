
#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>
#include "ISocket.h"

namespace gmlib
{

class CSocket : public ISocket
{
	//	�R�s�[�R���X�g���N�^�͋֎~�B
	CSocket(CSocket&);

protected:
	SOCKET				m_socket;
	DWORD				m_timeoutReceiving;		//	�f�[�^�̎�M����莞�Ԉȏ�s���Ȃ������ꍇ�A�ʐM��Q�Ƃ��Đؒf����B���̐ؒf�܂ł̎��Ԑݒ�B
	DWORD				m_timeoutConnecting;	//	�ڑ��̍ۂ̃^�C���A�E�g���ԁB

	DWORD				m_lastTime;	//	�^�C���A�E�g�𑪂邽�߂̍ŏI�X�V���ԁB

	int					m_state;	//	���݂̏�ԁBgetState()�Ŏ擾�ł���B
	std::vector<byte>	m_send;		//	���M�o�b�t�@�B
	std::vector<byte>	m_recv;		//	��M�o�b�t�@�B

	void				*m_shared;	//	�ڑ��p�X���b�h�Ƃ̋��L�I�u�W�F�N�g���w���|�C���^�B	

	/**
	�R���X�g���N�^�B
	@param	address	�ڑ���A�h���X�BIP�������̓h���C�����B
	@param	port	�ڑ���|�[�g�ԍ��B
	@param	timeout	�ڑ��v���̃^�C���A�E�g���ԁB0�Ȃ�Ζ������ɑ҂��܂����A���ۂ�20�b�`30�b���x�Ŏ��s����͂��ł��B
	*/
	CSocket(const char *address, WORD port, DWORD timeoutConnecting);

	/**
	update(DWORD)���ŁAflg��UPDATE_RECV���܂܂�Ă����ꍇ�ɌĂяo����郁�\�b�h�B
	*/
	virtual void updateRecv();

	/**
	update(DWORD)���ŁAflg��UPDATE_SEND���܂܂�Ă����ꍇ�ɌĂяo����郁�\�b�h�B
	*/
	virtual void updateSend();
public:

	/**
	�R���X�g���N�^�B
	SOCKET�̐����Ɛڑ��͎��O�ōs���A�ʐM�Ǘ��݂̂��s���Ă��炤�ꍇ�ɗ��p����B
	@param	socket �ڑ��ς�SOCKET�B
	*/
	CSocket(SOCKET socket);

	/**
	�w�肳�ꂽ�A�h���X�ɐڑ��v���������Ă���CSocket�𐶐����܂��B
	@param	address	�ڑ���A�h���X�BIP�������̓h���C�����B
	@param	port	�ڑ���|�[�g�ԍ��B
	@param	timeout	�ڑ��v���̃^�C���A�E�g���ԁB0�Ȃ�Ζ������ɑ҂��܂����A���ۂ�20�b�`30�b���x�Ŏ��s����͂��ł��B
	@return			�w���ɐڑ��v���������Ă���CSocket�C���X�^���X�ւ̃|�C���^�B
	*/
	static CSocket *connect(const char *address, WORD port, DWORD timeoutConnecting = 0);

	virtual ~CSocket();
	virtual void close();
	virtual void send(const void *p, int len);
	virtual std::vector<byte> &recv();
	virtual const std::vector<byte> &recv()const;
	virtual void update(DWORD flg = UPDATE_SEND | UPDATE_RECV);
	virtual int getState()const;
	virtual SOCKET getSocket()const;
	virtual void setTimeoutReceiving(DWORD timeoutReceiving);
	virtual DWORD getTimeoutReceiving()const;
};

}//namespace gmlib