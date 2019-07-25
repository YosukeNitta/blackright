
#pragma once
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <memory>
#include <ws2tcpip.h>
#include <wspiapi.h>

#pragma comment(lib, "ws2_32.lib")

namespace gmlib
{

/**
�\�P�b�g�������N���X�̃C���^�[�t�F�[�X�B
�S�Ẵ��\�b�h�́A�P��̃X���b�h����Ăяo����邱�Ƃ��z�肳��Ă��܂��B
�Ⴆ�΁A�����̃X���b�h���瓯����send���Ă΂ꂽ�ꍇ�̓���͕s��ł��B
�����̃X���b�h����g�������ꍇ�A�������CRITICAL_SECTION�Ȃǂŕی삵�Ă��痘�p���Ă��������B
*/
class ISocket
{
public:
	enum
	{
		STATE_TRY_CONNECTING = 0,	//	�ڑ��v�������Ă����ԁB
		STATE_CONNECTED,			//	�ڑ��v�����������A����M���\�ȏ�ԁB
		STATE_CONNECT_ERROR,		//	�ڑ��v�������s������ԁB
		STATE_CONNECTION_TIMED_OUT,	//	�ڑ��v���Ɏ��Ԃ������肷�������߁A������߂���ԁB
		STATE_RECEIVE_TIMED_OUT,	//	�Ō�Ƀf�[�^����M�������Ԃ���A�w�莞�Ԉȏ�o�߂����̂Őؒf������ԁB
		STATE_DISCONNECTION,		//	�������ؒf���ꂽ��̏�ԁB
	};

	enum
	{
		UPDATE_SEND		= (1 << 0),	//	send���\�b�h�ɂ���đ��M�o�b�t�@�ɓo�^�����f�[�^���A���ۂɑ��M���܂��B
		UPDATE_RECV		= (1 << 1),	//	���ۂɎ�M���s���Arecv()�Ŏ�M�f�[�^���Q�Ƃł���悤�ɂ��܂��B
	};

public:
	/**
	�f�X�g���N�^�B
	*/
	virtual ~ISocket(){}

	/**
	�ڑ����I�����܂��B��M�����f�[�^�͔j������܂���B
	*/
	virtual void close() = 0;
	
	/**
	�ڑ���ɑ��M����f�[�^���A���M�o�b�t�@�ɓo�^���܂��B
	���ۂɑ��M����̂�update(UPDATE_SEND)���Ă΂ꂽ���ł��B
	@param	p	�o�^�f�[�^�ւ̃|�C���^�B
	@param	len	�o�^�f�[�^�̃T�C�Y�B
	*/
	virtual void send(const void *p, int len) = 0;

	/**
	��M�����f�[�^��ێ����Ă����M�o�b�t�@�ւ̎Q�Ƃ��擾���܂��B
	@return	��M�o�b�t�@�ւ̎Q�ƁB
	*/
	virtual std::vector<byte> &recv() = 0;
	
	/**
	��M�����f�[�^��ێ����Ă����M�o�b�t�@�ւ̎Q�Ƃ��擾���܂��B
	@return	��M�o�b�t�@�ւ̎Q�ƁB
	*/
	virtual const std::vector<byte> &recv()const = 0;

	/**
	��Ԃ��X�V���܂��B
	����I(�Ⴆ��FPS60�̃Q�[���Ȃ��1/60�b����)�ɌĂяo���Ă��������B
	@param	flg		UPDATE_XXXX�萔��OR�őg�ݍ��킹�����̂��w�肵�܂��B
	*/
	virtual void update(DWORD flg = UPDATE_SEND | UPDATE_RECV) = 0;

	/**
	���݂̏�Ԃ��擾���܂��B
	@return	���݂̏�ԁBSTATE_XXXX�萔�B
	*/
	virtual int getState()const = 0;

	/**
	SOCKET���擾����B
	@return	SOCKET
	*/
	virtual SOCKET getSocket()const = 0;

	/**
	��M�Ɋւ���^�C���A�E�g���Ԃ�ݒ肵�܂��B
	@param	timeoutReceiving	�w�肳�ꂽ���Ԉȏ�̎��Ԏ�M���s���Ȃ������ꍇ�A�����ؒf������� STATE_RECEIVE_TIMED_OUT �ȏ�ԂɂȂ�܂��B
								0���w�肵���ꍇ�̓^�C���A�E�g�͋N����܂���B
	*/
	virtual void setTimeoutReceiving(DWORD timeoutReceiving) = 0;

	/**
	��M�Ɋւ���^�C���A�E�g���Ԃ��擾���܂��B
	@return	��M�Ɋւ���^�C���A�E�g���ԁB
	*/
	virtual DWORD getTimeoutReceiving()const = 0;
};

typedef std::tr1::shared_ptr<ISocket> SSocket;

}//namespace gmlib