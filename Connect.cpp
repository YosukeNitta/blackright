#include "pch.h"

static const int PUT_MAX = 10;	// �L�^����t���[���̍ő�
static const int DATA_MAX = 10;

// �ڑ�
static boolean network = false;
// 0.�Ȃ� 1.�z�X�g 2.�N���C�A���g
static int connectSide = 0;
// �����IP�A�h���X
static IPDATA Ip;
// �l�b�g���[�N�n���h��
static int NetHandle = 0;
// �|�[�g�ԍ�
static int Port = 0;

// �ҋ@
static boolean wait;

// [ �ʐM���x�v�� ] //
static int delay = 0;	// �͂��܂ŉ��t���[����
static int delayTime = 0;
static int delayTime_P1 = 0;
static BYTE delayNum[2];	// �����̓��͏�񂪂ǂ��܂ł�������
static int gameMode[2];	// �Q�[�����[�h(0:���� 1:����)

// [ ���� ] //
// �z��P�Ƀt���[�����̓��́A�z��Q�ɂ��̃L�[�ƃ��[�h������
// 0..�J�E���g(�g���ĂȂ�) 1�`8..�L�[ 9..����̃��[�h
static BYTE getData[PUT_MAX][DATA_MAX];

//===========================
// �f�[�^���Ƃ�
//===========================
// �l�b�g���[�N�擾
void Connect_GetCn(boolean net)
{
	network = net;
}
// �l�b�g���[�N�m�F
boolean Connect_CheckCn()
{
	return network;
}

// �v���C���[�T�C�h�擾
void Connect_GetSide(int num)
{
	connectSide = num;
}
// �v���C���[�T�C�h�m�F
int Connect_CheckSide()
{
	return connectSide;
}

// �|�[�g�擾
void Connect_GetPort(int num)
{
	Port = num;
}
// �|�[�g�m�F
int Connect_CheckPort()
{
	return Port;
}

// �n���h���擾
void Connect_GetNetHandle(int num)
{
	NetHandle = num;
}\
// �n���h���m�F
int Connect_CheckNetHandle()
{
	return NetHandle;
}

// IP�擾
void Connect_GetIp(IPDATA num)
{
	Ip = num;
}
// IP�m�F
IPDATA Connect_CheckIp()
{
	return Ip;
}

//===========================
// �f�B���C�ݒ�
//===========================
// �f�B���C���x�`�F�b�N
void Delay_Check(){

	// 1P > 2P	�ڑ��m�F
	// 1P < 2P	2P�`�F�b�N�J�n
	// 1P > 2P	1P�`�F�b�N�J�n�A2P��M���̌o�ߎ��Ԍ���
	// 1P < 2P	2P�o�ߎ��Ԍ���
	// 1P > 2P	1P�󂯎�������Ƃ�񍐂�������
	//
	if (delayTime < 180){
		// 1P
		if (Connect_CheckSide() == 1){
			if (delayNum[1] == 100){
				delayNum[0] = delayTime;	// �����̃f�B���C�^�C��
			}
			else{
				delayNum[0] = 100;
			}

			// 1..���M
			NetWorkSendUDP(NetHandle, Ip, Port, delayNum, sizeof(delayNum));

			// 2..�����̑��xor����̏�����󂯎��
			if (CheckNetWorkRecvUDP(NetHandle) == TRUE){	// �����Ƃ������󂯎��
				NetWorkRecvUDP(NetHandle, NULL, NULL, delayNum, sizeof(delayNum), FALSE);
			}
			// �����Num�ɐ����������Ă���A������f�B���C�ɂ���

		}
		// 2P
		else if (Connect_CheckSide() == 2){
			if (delayNum[0] == 100){
				delayNum[1] = delayTime;	// �����̃f�B���C�^�C��
			}
			else{
				delayNum[1] = 100;
			}

			// 1..�����̑��M���x����
			NetWorkSendUDP(NetHandle, Ip, Port, delayNum, sizeof(delayNum));

			// �@ ���M�m�莞�̃f�B���C�^�C�����擾
			if (CheckNetWorkRecvUDP(NetHandle) == TRUE){	// �����Ƃ������󂯎��
				NetWorkRecvUDP(NetHandle, NULL, NULL, delayNum, sizeof(delayNum), FALSE);
			}

		}

		// �f�B���C��0(�܂�����ĂȂ�)
		if (delay == 0){
			// ���肪1�ȏ���������Ă���
			if ((Connect_CheckSide() == 1) && (delayNum[1] > 0) && (delayNum[1] != 100)){
				delay = delayNum[1];	// �����DT��
				delayNum[1] = 0;		// ����������(����𑗂�Ȃ��悤��)
			}
			else if ((Connect_CheckSide() == 2) && (delayNum[0] > 0) && (delayNum[0] != 100)){
				delay = delayNum[0];
				delayNum[0] = 0;
				// delayNum(�ŏ��Ɏ󂯎�����Ƃ��̃f�B���C�^�C��);	// �����DT
			}
		}

		delayTime++;	// �f�B���C���ԃv���X(10�b�͑҂�)	
	}
}

// �f�B���C��`�悷��
void Delay_Draw(){
	DrawFormatString(0, 0, GetColor(255, 255, 255), "delay %d", delay);
}

// �f�B���C�`�F�b�N���I�����Ă邩
boolean Delay_IfEnd(){
	if (delayTime >= 179)return true;
	return false;
}

// �f�B���C���l�𑗂�
int Connect_CheckDelay(){
	return delay;
}

// �ϐ��̏�����
void Connect_Load(){
	delayNum[0] = -2;
	delayNum[1] = -2;
	gameMode[0] = 0;
	gameMode[1] = 0;
	Ip.d1 = 0;
	Ip.d2 = 0;
	Ip.d3 = 0;
	Ip.d4 = 0;
	for (int i = 0; i < PUT_MAX; i++){
		for (int j = 0; j < DATA_MAX; j++){
			getData[i][j] = 0;
		}
	}
}

BYTE Connect_CheckGetData(int num1, int num2){
	return getData[num1][num2];
}

//
void Connect_Get_GetData(BYTE num, BYTE num1, BYTE num2){
	getData[num1][num2] = num;
}

void Connect_SetGameMode()
{
	gameMode[0] = CheckGameMode();
	// 1�ȏ�Ȃ�
	if (getData[0][9] > 0){
		gameMode[1] = getData[0][9];
	}
}

boolean Connect_WaitGameMode()
{
	// 1P��2P��������ꍇ
	if (gameMode[0] != gameMode[1]){

		// �Z���N�g�ɐ�ɐi��
		if (gameMode[0] == GameScene(SelectScene)){
			if (gameMode[1] == GameScene(NetworkScene))return true;
			if (gameMode[1] == GameScene(ResultScene))return true;
		}
		// �o�[�T�X
		else if (gameMode[0] == GameScene(VersusScene)){
			if (gameMode[1] == GameScene(SelectScene))return true;
		}
		// ���U���g
		else if (gameMode[0] == GameScene(ResultScene)){
			if (gameMode[1] == GameScene(VersusScene))return true;
		}
	}
	return false;
}

/*
// �l�b�g���[�N�̏����擾
void Set_Network(int side, IPDATA ip, int port, int handle)
{
	connectSide = side;
	Ip = ip;
	Port = port;
	NetHandle = handle;
}

// �l�b�g���[�N�I��
void End_Network(){

	//[ UDP ]
	// �t�c�o�\�P�b�g�n���h���̍폜
	DeleteUDPSocket(NetHandle);
}

*/