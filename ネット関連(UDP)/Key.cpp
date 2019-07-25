#include "pch.h"
#pragma warning(disable : 4996)	// fscanf�̃G���[�\��������

#define CO_MAX (4 + BUTTON_MAX)	// �㉺���E�A8�{�^��

// �l�b�g�ΐ�
static boolean network = false;
static IPDATA Ip;
static int Port;
static int network_side = 0;	// 1,1P 2,2P

static const int PUT_MAX = 10;	// �L�^����t���[���̍ő�
static const int DATA_MAX = 10;
static BYTE Data[DATA_MAX];	// ����(�m�肵�����̂�����)
static BYTE gData[DATA_MAX];	// ����(����Ȃ̂Œx�点��K�v�͂Ȃ�)

static BYTE getData[DATA_MAX];	// �擾�������͏��
static BYTE myData[DATA_MAX];	// �����̓��͏��
static BYTE getMyData[DATA_MAX];	// �����̓��͏��

static BYTE dataLog[PUT_MAX][DATA_MAX];	// ����(�m�肵�����̂�����)
//static BYTE gDataLog[PUT_MAX][DATA_MAX];	// ����(����Ȃ̂Œx�点��K�v�͂Ȃ�)

static BYTE netD[DATA_MAX];	// ���肩��̎󂯎��p

static int net_time = 1;

static int UDPNetHandle;        // �l�b�g���[�N�n���h��

static int NetHandle, LostHandle;    // �l�b�g���[�N�n���h��
static int DataLength;            // ��M�f�[�^�ʕۑ��p�ϐ�

// 1P�A2P�̃p�b�h�ڑ�
static int m_P_Input[2];

// �{�^���̗v�f�i���ۂɎg�����́j
static int P1_B[PAD_MAX], P2_B[PAD_MAX];
static int P1_NB[PAD_MAX], P2_NB[PAD_MAX];
// �{�^���̏���
static int P1_I[PAD_MAX], P2_I[PAD_MAX];

// ����̉�
static int ctrl[2];

static int button3[8];

static int mOld_B[PAD_MAX];

// �������ɔC����
static int key[PAD_MAX];
static int pad[PAD_MAX];
static int co_k[2][CO_MAX];
static int co_p[2][CO_MAX];

static int Load_1;

static void NumSetting();
static int load_config();

static void Send_NetKey();
static void Get_NetKey();
static void Send_YourKey();
static void Get_MyKey();

// �p�b�h���ڑ�����Ă��邩�m�F
void GetInput()
{
	// �錾


	{
		// P1
		int i;
		i = GetJoypadInputState(DX_INPUT_PAD1);
		m_P_Input[0] = i;	// i����

		// P2
		int j;
		j = GetJoypadInputState(DX_INPUT_PAD2);
		m_P_Input[1] = j; // j����
	}

}


// �ڑ��̂݊m�F
int P_Input(int n)
{
	return m_P_Input[n - 1];
}


// �{�^�����̓`�F�b�N
// �{�^���z�u��������ꍇ�͂�����

int P1_BCheck()
{
	if (!network){	//
		// P1�� ��v�f ��������Ă�����
		if (((m_P_Input[0] & pad[co_p[0][0]]) != 0) ||
			(CheckHitKey(key[co_k[0][0]]) == 1)){
			P1_B[108] += 1;
		}
		else{
			P1_B[108] = 0;
		}
		// ��
		if (((m_P_Input[0] & pad[co_p[0][1]]) != 0) ||
			(CheckHitKey(key[co_k[0][1]]) == 1)){
			P1_B[102] += 1;
		}
		else{
			P1_B[102] = 0;
		}
		// ��
		if (((m_P_Input[0] & pad[co_p[0][2]]) != 0) ||
			(CheckHitKey(key[co_k[0][2]]) == 1)){
			P1_B[104] += 1;
		}
		else{
			P1_B[104] = 0;
		}
		// �E
		if (((m_P_Input[0] & pad[co_p[0][3]]) != 0) ||
			(CheckHitKey(key[co_k[0][3]]) == 1)){
			P1_B[106] += 1;
		}
		else{
			P1_B[106] = 0;
		}


		// 1�{�^��	A
		if (
			((m_P_Input[0] & pad[co_p[0][4]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][4]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P1_B[1] ++;
		}
		else{
			P1_B[1] = 0;
		}
		// 2�{�^��	B
		if (
			((m_P_Input[0] & pad[co_p[0][5]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][5]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)
			){
			P1_B[2] += 1;
		}
		else{
			P1_B[2] = 0;
		}
		// 3�{�^��	C
		if (
			((m_P_Input[0] & pad[co_p[0][6]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||

			(CheckHitKey(key[co_k[0][6]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)
			){
			P1_B[3] += 1;
		}
		else{
			P1_B[3] = 0;
		}
		// 4�{�^��	D
		if (((m_P_Input[0] & pad[co_p[0][7]]) != 0) ||
			(CheckHitKey(key[co_k[0][7]]) == 1)
			){
			P1_B[4] += 1;
		}
		else{
			P1_B[4] = 0;
		}
		// 5�{�^��	A+B+C
		if (((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)){
			P1_B[5] += 1;
		}
		else{
			P1_B[5] = 0;
		}
		// 6�{�^��	A+B (����)
		if (((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P1_B[6] += 1;
		}
		else{
			P1_B[6] = 0;
		}
		// 7�{�^��	PAUSE
		if (((m_P_Input[0] & pad[co_p[0][10]]) != 0) ||
			(CheckHitKey(key[co_k[0][10]]) == 1)){
			P1_B[7] += 1;
		}
		else{
			P1_B[7] = 0;
		}
		// 8�{�^��  �g����(1)
		if (((m_P_Input[0] & pad[co_p[0][11]]) != 0) ||
			(CheckHitKey(key[co_k[0][11]]) == 1)){
			P1_B[8] += 1;
		}
		else{
			P1_B[8] = 0;
		}
		// 9�{�^��  �g����(2)
		if (((m_P_Input[0] & pad[co_p[0][12]]) != 0) ||
			(CheckHitKey(key[co_k[0][12]]) == 1)){
			P1_B[9] += 1;
		}
		else{
			P1_B[9] = 0;
		}
	}	// 0
	else if (network_side == 1){	//
		// P1�� ��v�f ��������Ă�����
		if (((m_P_Input[0] & pad[co_p[0][0]]) != 0) ||
			(CheckHitKey(key[co_k[0][0]]) == 1)){
			P1_NB[108] += 1;
		}
		else{
			P1_NB[108] = 0;
		}
		// ��
		if (((m_P_Input[0] & pad[co_p[0][1]]) != 0) ||
			(CheckHitKey(key[co_k[0][1]]) == 1)){
			P1_NB[102] += 1;
		}
		else{
			P1_NB[102] = 0;
		}
		// ��
		if (((m_P_Input[0] & pad[co_p[0][2]]) != 0) ||
			(CheckHitKey(key[co_k[0][2]]) == 1)){
			P1_NB[104] += 1;
		}
		else{
			P1_NB[104] = 0;
		}
		// �E
		if (((m_P_Input[0] & pad[co_p[0][3]]) != 0) ||
			(CheckHitKey(key[co_k[0][3]]) == 1)){
			P1_NB[106] += 1;
		}
		else{
			P1_NB[106] = 0;
		}

		// 1�{�^��	A
		if (
			((m_P_Input[0] & pad[co_p[0][4]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][4]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P1_NB[1] += 1;
		}
		else{
			P1_NB[1] = 0;
		}
		// 2�{�^��	B
		if (
			((m_P_Input[0] & pad[co_p[0][5]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][5]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)
			){
			P1_NB[2] += 1;
		}
		else{
			P1_NB[2] = 0;
		}
		// 3�{�^��	C
		if (
			((m_P_Input[0] & pad[co_p[0][6]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||

			(CheckHitKey(key[co_k[0][6]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)
			){
			P1_NB[3] += 1;
		}
		else{
			P1_NB[3] = 0;
		}
		// 4�{�^��	D
		if (((m_P_Input[0] & pad[co_p[0][7]]) != 0) ||
			(CheckHitKey(key[co_k[0][7]]) == 1)
			){
			P1_NB[4] += 1;
		}
		else{
			P1_NB[4] = 0;
		}
		// 5�{�^��	A+B (����)
		if (((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)){
			P1_NB[5] += 1;
		}
		else{
			P1_NB[5] = 0;
		}
		// 6�{�^��	A+B+C
		if (((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P1_NB[6] += 1;
		}
		else{
			P1_NB[6] = 0;
		}
		// 7�{�^��	PAUSE
		if (((m_P_Input[0] & pad[co_p[0][10]]) != 0) ||
			(CheckHitKey(key[co_k[0][10]]) == 1)){
			P1_NB[7] += 1;
		}
		else{
			P1_NB[7] = 0;
		}
		// 8�{�^��  �g����(1)
		if (((m_P_Input[0] & pad[co_p[0][11]]) != 0) ||
			(CheckHitKey(key[co_k[0][11]]) == 1)){
			P1_NB[8] += 1;
		}
		else{
			P1_NB[8] = 0;
		}
		// 9�{�^��  �g����(2)
		if (((m_P_Input[0] & pad[co_p[0][12]]) != 0) ||
			(CheckHitKey(key[co_k[0][12]]) == 1)){
			P1_NB[9] += 1;
		}
		else{
			P1_NB[9] = 0;
		}
	}	// 1
	else if (network_side == 2){
		// P1�� ��v�f ��������Ă�����
		if (((m_P_Input[0] & pad[co_p[0][0]]) != 0) ||
			(CheckHitKey(key[co_k[0][0]]) == 1)){
			P2_NB[108] += 1;
		}
		else{
			P2_NB[108] = 0;
		}
		// ��
		if (((m_P_Input[0] & pad[co_p[0][1]]) != 0) ||
			(CheckHitKey(key[co_k[0][1]]) == 1)){
			P2_NB[102] += 1;
		}
		else{
			P2_NB[102] = 0;
		}
		// ��
		if (((m_P_Input[0] & pad[co_p[0][2]]) != 0) ||
			(CheckHitKey(key[co_k[0][2]]) == 1)){
			P2_NB[104] += 1;
		}
		else{
			P2_NB[104] = 0;
		}
		// �E
		if (((m_P_Input[0] & pad[co_p[0][3]]) != 0) ||
			(CheckHitKey(key[co_k[0][3]]) == 1)){
			P2_NB[106] += 1;
		}
		else{
			P2_NB[106] = 0;
		}

		// 1�{�^��	A
		if (
			((m_P_Input[0] & pad[co_p[0][4]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][4]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P2_NB[1] += 1;
		}
		else{
			P2_NB[1] = 0;
		}
		// 2�{�^��	B
		if (
			((m_P_Input[0] & pad[co_p[0][5]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][5]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)
			){
			P2_NB[2] += 1;
		}
		else{
			P2_NB[2] = 0;
		}
		// 3�{�^��	C
		if (
			((m_P_Input[0] & pad[co_p[0][6]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||

			(CheckHitKey(key[co_k[0][6]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)
			){
			P2_NB[3] += 1;
		}
		else{
			P2_NB[3] = 0;
		}
		// 4�{�^��	D
		if (((m_P_Input[0] & pad[co_p[0][7]]) != 0) ||
			(CheckHitKey(key[co_k[0][7]]) == 1)
			){
			P2_NB[4] += 1;
		}
		else{
			P2_NB[4] = 0;
		}
		// 5�{�^��	A+B (����)
		if (((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)){
			P2_NB[5] += 1;
		}
		else{
			P2_NB[5] = 0;
		}
		// 6�{�^��	A+B+C
		if (((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P2_NB[6] += 1;
		}
		else{
			P2_NB[6] = 0;
		}
		// 7�{�^��	PAUSE
		if (((m_P_Input[0] & pad[co_p[0][10]]) != 0) ||
			(CheckHitKey(key[co_k[0][10]]) == 1)){
			P2_NB[7] += 1;
		}
		else{
			P2_NB[7] = 0;
		}
		// 8�{�^��  �g����(1)
		if (((m_P_Input[0] & pad[co_p[0][11]]) != 0) ||
			(CheckHitKey(key[co_k[0][11]]) == 1)){
			P2_NB[8] += 1;
		}
		else{
			P2_NB[8] = 0;
		}
		// 9�{�^��  �g����(2)
		if (((m_P_Input[0] & pad[co_p[0][12]]) != 0) ||
			(CheckHitKey(key[co_k[0][12]]) == 1)){
			P2_NB[9] += 1;
		}
		else{
			P2_NB[9] = 0;
		}
	}// 2


	return 0;	// �I�����I��I
}

// �{�^�����̓`�F�b�N
// �{�^���z�u��������ꍇ�͂�����

int P2_BCheck()
{
	// �ʏ��2P����
	if (!network){
		// P1�� ��v�f ��������Ă�����
		if (((m_P_Input[1] & pad[co_p[1][0]]) != 0) ||
			(CheckHitKey(key[co_k[1][0]]) == 1)){
			P2_B[108] += 1;
		}
		else{
			P2_B[108] = 0;
		}
		// ��
		if (((m_P_Input[1] & pad[co_p[1][1]]) != 0) ||
			(CheckHitKey(key[co_k[1][1]]) == 1)){
			P2_B[102] += 1;
		}
		else{
			P2_B[102] = 0;
		}
		// ��
		if (((m_P_Input[1] & pad[co_p[1][2]]) != 0) ||
			(CheckHitKey(key[co_k[1][2]]) == 1)){
			P2_B[104] += 1;
		}
		else{
			P2_B[104] = 0;
		}
		// �E
		if (((m_P_Input[1] & pad[co_p[1][3]]) != 0) ||
			(CheckHitKey(key[co_k[1][3]]) == 1)){
			P2_B[106] += 1;
		}
		else{
			P2_B[106] = 0;
		}

		// 1�{�^��	A
		if (
			((m_P_Input[1] & pad[co_p[1][4]]) != 0) ||
			((m_P_Input[1] & pad[co_p[1][8]]) != 0) ||
			((m_P_Input[1] & pad[co_p[1][9]]) != 0) ||

			(CheckHitKey(key[co_k[1][4]]) == 1) ||
			(CheckHitKey(key[co_k[1][8]]) == 1) ||
			(CheckHitKey(key[co_k[1][9]]) == 1)){
			P2_B[1] ++;
		}
		else{
			P2_B[1] = 0;
		}
		// 2�{�^��	B
		if (
			((m_P_Input[1] & pad[co_p[1][5]]) != 0) ||
			((m_P_Input[1] & pad[co_p[1][8]]) != 0) ||
			((m_P_Input[1] & pad[co_p[1][9]]) != 0) ||

			(CheckHitKey(key[co_k[1][5]]) == 1) ||
			(CheckHitKey(key[co_k[1][8]]) == 1) ||
			(CheckHitKey(key[co_k[1][9]]) == 1)
			){
			P2_B[2] += 1;
		}
		else{
			P2_B[2] = 0;
		}
		// 3�{�^��	C
		if (
			((m_P_Input[1] & pad[co_p[1][6]]) != 0) ||
			((m_P_Input[1] & pad[co_p[1][8]]) != 0) ||

			(CheckHitKey(key[co_k[1][6]]) == 1) ||
			(CheckHitKey(key[co_k[1][8]]) == 1)
			){
			P2_B[3] += 1;
		}
		else{
			P2_B[3] = 0;
		}
		// 4�{�^��	D
		if (((m_P_Input[1] & pad[co_p[1][7]]) != 0) ||
			(CheckHitKey(key[co_k[1][7]]) == 1)
			){
			P2_B[4] += 1;
		}
		else{
			P2_B[4] = 0;
		}
		// 5�{�^��	A+B (����)
		if (((m_P_Input[1] & pad[co_p[1][8]]) != 0) ||
			(CheckHitKey(key[co_k[1][8]]) == 1)){
			P2_B[5] += 1;
		}
		else{
			P2_B[5] = 0;
		}
		// 6�{�^��	A+B+C
		if (((m_P_Input[1] & pad[co_p[1][9]]) != 0) ||
			(CheckHitKey(key[co_k[1][9]]) == 1)){
			P2_B[6] += 1;
		}
		else{
			P2_B[6] = 0;
		}
		// 7�{�^��	PAUSE
		if (((m_P_Input[1] & pad[co_p[1][10]]) != 0) ||
			(CheckHitKey(key[co_k[1][10]]) == 1)){
			P2_B[7] += 1;
		}
		else{
			P2_B[7] = 0;
		}
		// 8�{�^��  �g����(1)
		if (((m_P_Input[1] & pad[co_p[1][11]]) != 0) ||
			(CheckHitKey(key[co_k[1][11]]) == 1)){
			P2_B[8] += 1;
		}
		else{
			P2_B[8] = 0;
		}
		// 9�{�^��  �g����(2)
		if (((m_P_Input[1] & pad[co_p[1][12]]) != 0) ||
			(CheckHitKey(key[co_k[1][12]]) == 1)){
			P2_B[9] += 1;
		}
		else{
			P2_B[9] = 0;
		}
	}	// �l�b�g���[�N�Ɍq�����Ă��Ȃ��ꍇ�̂ݍs��


	// �l�b�g���[�N����
	else if (network){
		net_time++;
		if (net_time > 10)net_time = 1;

		// �f�[�^���q�����Ă�
		{
			// �����̑��M�f�[�^����
			Main_GetNetwork(true);
			if (network_side == 1){
				for (int i = 0; i < DATA_MAX; i++){
					if (myData[i] > 60)myData[i] = 60;
				}
				myData[0] = net_time;
				myData[1] = P1_NB[1];
				myData[2] = P1_NB[2];
				myData[3] = P1_NB[3];
				myData[4] = P1_NB[4];
				myData[5] = P1_NB[102];
				myData[6] = P1_NB[104];
				myData[7] = P1_NB[106];
				myData[8] = P1_NB[108];
				myData[9] = P1_NB[7];
			}
			else if (network_side == 2){
				for (int i = 0; i < DATA_MAX; i++){
					if (myData[i] > 60)myData[i] = 60;
				}
				myData[0] = net_time;
				myData[1] = P2_NB[1];
				myData[2] = P2_NB[2];
				myData[3] = P2_NB[3];
				myData[4] = P2_NB[4];
				myData[5] = P2_NB[102];
				myData[6] = P2_NB[104];
				myData[7] = P2_NB[106];
				myData[8] = P2_NB[108];
				myData[9] = P2_NB[7];
			}
			//
			/////////////////////////////////////////////

			// �O�̃L�[������͌Ăяo��
			/*
			for (int i = 0; i < 10; i++){
				// ���͂Ȃ����ɉߋ��̃f�[�^�ɂ͂���ꍇ
				if (Data[i][3] == 0){
					if (Data[i + 10][2] > 0){
						Data[i][3] = Data[i + 10][2];
						if (Data[i + 20][1] > 0){
							Data[i][3] = Data[i + 20][1];
						}
					}
				}
			}
			*/
			/////////////////////////////////////////////

		{
			// ��M�f�[�^������
			for (int i = 0; i < DATA_MAX; i++){
				Data[i] = 0;
				gData[i] = 0;	// �R�}���h
				//myData[i] = 0;
				getData[i] = 0;
				getMyData[i] = 0;

				netD[i] = 0;
			}
			// UDP
			///////////////////////
			int cnwr_ret = CheckNetWorkRecvUDP(UDPNetHandle);
			if (cnwr_ret && cnwr_ret != -1){
				// �f�[�^�󂯎��
				if (network_side == 1){
					Send_NetKey();
					Get_NetKey();
					//Send_YourKey();
					//Get_MyKey();
				}
				else if (network_side == 2){
					Get_NetKey();
					Send_NetKey();
					//Get_MyKey();
					//Send_YourKey();
				}
			}
			else{
				//Main_GetNetwork(false);
				if (network_side == 1){
					for (int i = 0; i < PAD_MAX; i++){
						P1_NB[i]--;
						if (P1_NB[i] < 0)P1_NB[i] = 0;
					}
				}
				else if (network_side == 2){
					for (int i = 0; i < PAD_MAX; i++){
						P2_NB[i]--;
						if (P2_NB[i] < 0)P2_NB[i] = 0;
					}
				}
			}
			///////////////////////

			for (int data = 0; data < DATA_MAX; data++){
				dataLog[0][data] = myData[data];
			}

			// �����Ƒ���̃f�[�^�擾����
			// ���ɐi�߂�
			for (int i = 0; i < PUT_MAX - 1; i++){
				for (int data = 0; data < DATA_MAX; data++){
					// 1P
					dataLog[i+1][data] = dataLog[i][data];
					// 2P
					//gDataLog[i + 1][data] = gDataLog[i][data];
				}
			}

			//�擾��������̃L�[�Ǝ��Ԃ������Ȃ�i�߂�
			for (int i = 0; i < PUT_MAX; i++){
				// ���O�Ǝ��Ԃ�����
				if (getData[0] == dataLog[i][0]){
					for (int data = 0; data < DATA_MAX; data++){
						Data[data] = dataLog[i][data];
						gData[data] = getData[data];
					}
				}

			}

			///////////////////////
		}


		// �{�^������
		if (network_side == 1){
			// 1P
			P1_B[1] = Data[1];
			P1_B[2] = Data[2];
			P1_B[3] = Data[3];
			P1_B[4] = Data[4];
			P1_B[102] = Data[5];
			P1_B[104] = Data[6];
			P1_B[106] = Data[7];
			P1_B[108] = Data[8];
			//P1_B[7] = Data3[9];
			// 2P
			P2_B[1] = gData[1];
			P2_B[2] = gData[2];
			P2_B[3] = gData[3];
			P2_B[4] = gData[4];
			P2_B[102] = gData[5];
			P2_B[104] = gData[6];
			P2_B[106] = gData[7];
			P2_B[108] = gData[8];
		}
		else if (network_side == 2){
			// 1P
			P1_B[1] = gData[1];
			P1_B[2] = gData[2];
			P1_B[3] = gData[3];
			P1_B[4] = gData[4];
			P1_B[102] = gData[5];
			P1_B[104] = gData[6];
			P1_B[106] = gData[7];
			P1_B[108] = gData[8];
			// 2P
			P2_B[1] = Data[1];
			P2_B[2] = Data[2];
			P2_B[3] = Data[3];
			P2_B[4] = Data[4];
			P2_B[102] = Data[5];
			P2_B[104] = Data[6];
			P2_B[106] = Data[7];
			P2_B[108] = Data[8];
		}



		}	// �S�̏I��
	}// �l�b�g���[�N����


	return 0;	// �I�����I��I
}

void InputSend()
{
	// P1,P2�̕ϐ��ɕۑ�
	for (int i = 0; i < PAD_MAX; i++){
		P1.button[i] = P1_B[i];
		P2.button[i] = P2_B[i];
	}


	// ��s���͂̐ݒ�A�Ƃ��6�t��
	for (int i = 1; i < BUTTON_MAX; i++){

		// �L��������ʂ������Ă�Ƃ��͐�s���͂�����
		if (S.StopTime == 0){
			if (P1.StopTime == 0){
				if (P1.Senkou[i] > 0)P1.Senkou[i] -= 1;
				if (P1.rbutton[i] > 0)P1.rbutton[i] -= 1;
			}
			if (P2.StopTime == 0){
				if (P2.Senkou[i] > 0)P2.Senkou[i] -= 1;
				if (P2.rbutton[i] > 0)P2.rbutton[i] -= 1;
			}
		}

		// ���o�T(�K�[�h�A�N���オ��)
		if ((P1.stateno >= 50 && P1.stateno < 60) ||
			(P1.stateno == 1055)){
			if (P1.button[i] == 1)P1.rbutton[i] = 6;
		}
		else{ if (P1.button[i] == 1)P1.rbutton[i] = 1; }
		if ((P2.stateno >= 50 && P2.stateno < 60) ||
			(P2.stateno == 1055)){
			if (P2.button[i] == 1)P2.rbutton[i] = 6;
		}
		else{ if (P2.button[i] == 1)P2.rbutton[i] = 1; }

		// ���͂����u�Ԃɐ�s����6�t���ǉ�
		// �������ςŗP�\���L�т�
		//if (P1.button[i] >= 1 && P1.button[i] <= 5)P1.Senkou[i] = 6;
		//if (P2.button[i] >= 1 && P2.button[i] <= 5)P2.Senkou[i] = 6;
		if (P1.button[i] == 1)P1.Senkou[i] = 6;
		if (P2.button[i] == 1)P2.Senkou[i] = 6;
	}

	// ���o�T����
	ctrl[0] = P1.ctrl;
	ctrl[1] = P2.ctrl;

	// �e���L�[�ʒu���f�Amuki���t���ƍ��E���ς��i��ɉE�����j

	Player P[2];
	P[0] = P1, P[1] = P2;

	for (int i = 0; i < 2; i++)
	{

		// �܂��͏�
		if (P[i].button[108] > 0){
			if (P[i].button[104] > 0)P[i].keyPos = 7;
			else if (P[i].button[106] > 0)P[i].keyPos = 9;
			else { P[i].keyPos = 8; }
		}
		// ���ɉ�
		else if (P[i].button[102] > 0){
			if (P[i].button[104] > 0)P[i].keyPos = 1;
			else if (P[i].button[106] > 0)P[i].keyPos = 3;
			else { P[i].keyPos = 2; }
		}
		// �����č�
		else if (P[i].button[104] > 0)P[i].keyPos = 4;
		// ����ɉE
		else if (P[i].button[106] > 0)P[i].keyPos = 6;
		// �g�h���Ƀj���[�g����
		else{ P[i].keyPos = 5; }

		// ���Ƌt�ɂ���
		if (P[i].muki == 1){
			if (P[i].keyPos == 1)P[i].keyPos = 3;
			else if (P[i].keyPos == 3)P[i].keyPos = 1;
			else if (P[i].keyPos == 4)P[i].keyPos = 6;
			else if (P[i].keyPos == 6)P[i].keyPos = 4;
			else if (P[i].keyPos == 7)P[i].keyPos = 9;
			else if (P[i].keyPos == 9)P[i].keyPos = 7;
		}


		//**************
		// �L�[��������
		//**************

		// �܂��͏㉺
		if (P[i].button[108] > 0)P[i].keyAtt[8] = 1;
		else{ P[i].keyAtt[8] = 0; }
		if (P[i].button[102] > 0)P[i].keyAtt[2] = 1;
		else{ P[i].keyAtt[2] = 0; }

		// ���E
		if (P[i].muki == 0){
			if (P[i].button[104] > 0)P[i].keyAtt[4] = 1;
			else{ P[i].keyAtt[4] = 0; }
			if (P[i].button[106] > 0)P[i].keyAtt[6] = 1;
			else{ P[i].keyAtt[6] = 0; }
		}
		else{
			if (P[i].button[106] > 0)P[i].keyAtt[4] = 1;
			else{ P[i].keyAtt[4] = 0; }
			if (P[i].button[104] > 0)P[i].keyAtt[6] = 1;
			else{ P[i].keyAtt[6] = 0; }
		}


		// �΂�
		if (P[i].keyAtt[2] > 0){
			if (P[i].keyAtt[4] > 0)P[i].keyAtt[1] = 1;
			else{ P[i].keyAtt[1] = 0; }
			if (P[i].keyAtt[6] > 0)P[i].keyAtt[3] = 1;
			else{ P[i].keyAtt[3] = 0; }
		}
		else{
			P[i].keyAtt[1] = 0;
			P[i].keyAtt[3] = 0;
		}

		if (P[i].keyAtt[8] > 0){
			if (P[i].keyAtt[4] > 0)P[i].keyAtt[7] = 1;
			else{ P[i].keyAtt[7] = 0; }
			if (P[i].keyAtt[6] > 0)P[i].keyAtt[9] = 1;
			else{ P[i].keyAtt[9] = 0; }
		}
		else{
			P[i].keyAtt[7] = 0;
			P[i].keyAtt[9] = 0;
		}

		// [Key] ��s����
		for (int j = 1; j < 9 + 1; j++){
			if (P[i].keyAtt[j] == 1){
				P[i].key[j] ++;
			}
			else{
				P[i].key[j] = 0;
			}

			// �L��������ʂ������Ă�Ƃ��͐�s���͂�����
			if (S.StopTime == 0){
				if (P[i].StopTime == 0){
					if (P[i].K_Senkou[j] > 0)P[i].K_Senkou[j] -= 1;
					if (P[i].K_Senkou[j] < 0)P[i].K_Senkou[j] = 0;
				}
			}

			// ���͂����u�Ԃɐ�s���͂T�t���ǉ�
			if (P[i].key[j] == 1)P[i].K_Senkou[j] = 6;
		}

	}
	P1 = P[0], P2 = P[1];


	Get_PSSet(P1, P2, S);		// �l��Ԃ�

}


// 1P���̃{�^�����͔��f
int P1_BInput(int P1_BNum)
{

	// �������ꂽ�l��Ԃ�
	return P1_B[P1_BNum];

}

// 2P��
int P2_BInput(int P2_BNum)
{

	// �������ꂽ�l��Ԃ�
	return P2_B[P2_BNum];

}

// �L�[���Ăяo��
int PH_BInput(int PSide, int P_Num)
{
	int n = 0;
	if (PSide == 1){
		n = P1_B[P_Num];
	}
	else if (PSide == 2){
		n = P2_B[P_Num];
	}
	return n;
}

// �ǂ��炩�̃L�[�����
int P_BInput(int P_Num)
{
	// ����ɐ��l���Ԃ�����
	int P_InputNum = 0;

	{
		if (P1_B[P_Num] != 0)P_InputNum = P1_B[P_Num];
		if (P2_B[P_Num] != 0)P_InputNum = P2_B[P_Num];
	}

	// �������ꂽ�l��Ԃ�
	return P_InputNum;

}

// �L�[�̓�����σ`�F�b�N
// [���_]�����j���[����߂莞�ɂ������ς��Ă���ƁA���̂܂ܗ����
boolean KInput(int n)
{
	boolean key;

	// �傫�����Ȃ����`�F�b�N
	if (n == 1 || (n >= 30 && n % 5 == 0))key = true;
	else{ key = false; }

	return key;
}

//================================================




int P1_BSet(int num)
{
	int watasu = 0;

	if (((m_P_Input[0] & pad[co_p[0][num]]) != 0)
		//(CheckHitKey(key[co_k[0][num]]) == 1) 
		){
		watasu = 1;
	}

	return watasu;
}

int P2_BSet(int num)
{
	int watasu = 0;

	for (int i = 1; i < PAD_MAX; i++){
		if (P2_I[num] == i){
			if (m_P_Input[1] & pad[i])watasu = 1;
		}
	}

	return watasu;
}


int P1_BConfig(int num1, int num2)
{
	// �ȈՃL�[����
	if (Load_1 == 0){
		NumSetting();
		load_config();
		Load_1 = 0;
	}

	P1_I[num1] = num2;

	// �R���t�B�O�ɉf�鐔�l
	return num2;
}

int P2_BConfig(int num1, int num2)
{
	P2_I[num1] = num2;

	// �R���t�B�O�ɉf�鐔�l
	return num2;
}

void NumSetting()
{

	key[48] = KEY_INPUT_0;	// �O�L�[
	key[49] = KEY_INPUT_1;	// �P�L�[
	key[50] = KEY_INPUT_2;	// �Q�L�[
	key[51] = KEY_INPUT_3;	// �R�L�[
	key[52] = KEY_INPUT_4;	// �S�L�[
	key[53] = KEY_INPUT_5;	// �T�L�[
	key[54] = KEY_INPUT_6;	// �U�L�[
	key[55] = KEY_INPUT_7;	// �V�L�[
	key[56] = KEY_INPUT_8;	// �W�L�[
	key[57] = KEY_INPUT_9;	// �X�L�[

	key[65] = KEY_INPUT_A;	// �`�L�[
	key[66] = KEY_INPUT_B;	// �a�L�[
	key[67] = KEY_INPUT_C;	// �b�L�[
	key[68] = KEY_INPUT_D;	// �c�L�[
	key[69] = KEY_INPUT_E;	// �d�L�[
	key[70] = KEY_INPUT_F;	// �e�L�[
	key[71] = KEY_INPUT_G;	// �f�L�[
	key[72] = KEY_INPUT_H;	// �g�L�[
	key[73] = KEY_INPUT_I;	// �h�L�[
	key[74] = KEY_INPUT_J;	// �i�L�[
	key[75] = KEY_INPUT_K;	// �j�L�[
	key[76] = KEY_INPUT_L;	// �k�L�[
	key[77] = KEY_INPUT_M;	// �l�L�[
	key[78] = KEY_INPUT_N;	// �m�L�[
	key[79] = KEY_INPUT_O;	// �n�L�[
	key[80] = KEY_INPUT_P;	// �o�L�[
	key[81] = KEY_INPUT_Q;	// �p�L�[
	key[82] = KEY_INPUT_R;	// �q�L�[
	key[83] = KEY_INPUT_S;	// �r�L�[
	key[84] = KEY_INPUT_T;	// �s�L�[
	key[85] = KEY_INPUT_U;	// �t�L�[
	key[86] = KEY_INPUT_V;	// �u�L�[
	key[87] = KEY_INPUT_W;	// �v�L�[
	key[88] = KEY_INPUT_X;	// �w�L�[
	key[89] = KEY_INPUT_Y;	// �x�L�[
	key[90] = KEY_INPUT_Z;	// �y�L�[

	key[96] = KEY_INPUT_NUMPAD0;	// �e���L�[�O
	key[97] = KEY_INPUT_NUMPAD1;	// �e���L�[�P
	key[98] = KEY_INPUT_NUMPAD2;	// �e���L�[�Q
	key[99] = KEY_INPUT_NUMPAD3;	// �e���L�[�R
	key[100] = KEY_INPUT_NUMPAD4;	// �e���L�[�S
	key[101] = KEY_INPUT_NUMPAD5;	// �e���L�[�T
	key[102] = KEY_INPUT_NUMPAD6;	// �e���L�[�U
	key[103] = KEY_INPUT_NUMPAD7;	// �e���L�[�V
	key[104] = KEY_INPUT_NUMPAD8;	// �e���L�[�W
	key[105] = KEY_INPUT_NUMPAD9;	// �e���L�[�X

	key[32] = KEY_INPUT_SPACE;	// �X�y�[�X�L�[

	key[37] = KEY_INPUT_LEFT;	// ���L�[
	key[38] = KEY_INPUT_UP;		// ��L�[
	key[39] = KEY_INPUT_RIGHT;	// �E�L�[
	key[40] = KEY_INPUT_DOWN;	// ���L�[



	pad[4] = PAD_INPUT_1;
	pad[5] = PAD_INPUT_2;
	pad[6] = PAD_INPUT_3;
	pad[7] = PAD_INPUT_4;
	pad[8] = PAD_INPUT_5;
	pad[9] = PAD_INPUT_6;
	pad[10] = PAD_INPUT_7;
	pad[11] = PAD_INPUT_8;
	pad[12] = PAD_INPUT_9;
	pad[13] = PAD_INPUT_10;
	pad[14] = PAD_INPUT_11;
	pad[15] = PAD_INPUT_12;
	pad[16] = PAD_INPUT_13;
	pad[17] = PAD_INPUT_14;
	pad[18] = PAD_INPUT_15;
	pad[19] = PAD_INPUT_16;
	pad[20] = PAD_INPUT_17;
	pad[21] = PAD_INPUT_18;
	pad[22] = PAD_INPUT_19;
	pad[23] = PAD_INPUT_20;
	pad[24] = PAD_INPUT_21;
	pad[25] = PAD_INPUT_22;
	pad[26] = PAD_INPUT_23;
	pad[27] = PAD_INPUT_24;
	pad[28] = PAD_INPUT_25;
	pad[29] = PAD_INPUT_26;
	pad[30] = PAD_INPUT_27;
	pad[31] = PAD_INPUT_28;

	pad[0] = PAD_INPUT_UP;
	pad[1] = PAD_INPUT_DOWN;
	pad[2] = PAD_INPUT_LEFT;
	pad[3] = PAD_INPUT_RIGHT;

}


int Send_PI(int num, int Side)
{
	int re;

	if (Side == 1)re = P1_I[num];
	if (Side == 2)re = P2_I[num];

	return re;
}


//----------------
//
// �g�����p�ݒ�
//
//----------------

// ����ɓ���
void P2_Auto(int i)
{
	P2_B[i] = 1;
}

// 2P�𑀍�
void P2Ctrl()
{
	// 2P�p�b�h�m�F
	// m_P_Input[1] = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	for (int i = 0; i < PAD_MAX; i++){
		mOld_B[i] = P1_B[i];	// 1P�̑�����L��
		P2_B[i] = mOld_B[i];	// 2P�̑����1P�̂�K�p
		P1_B[i] = 0;	// 1P�𓮂��Ȃ�����A�K�{
	}
}

void P2Ctrl2()
{
	for (int i = 0; i < PAD_MAX; i++){

		P1_B[i] = mOld_B[i];	// �I�����1P�̑���𕜊�
	}
}

void P2Ctrl3(int num[8])
{
	for (int i = 0; i < 8; i++){
		if (num[i] > 0){
			button3[i] += 1;
		}
		else{ button3[i] = 0; }
	}
}

void P2Ctrl3_2()
{
	P2_B[108] = button3[0];
	P2_B[102] = button3[1];
	P2_B[104] = button3[2];
	P2_B[106] = button3[3];
	P2_B[1] = button3[4];
	P2_B[2] = button3[5];
	P2_B[3] = button3[6];
	P2_B[4] = button3[7];
}

// �Ȉ�AI
void P2_AI(Player GP1, Player GP2)
{
	m_P_Input[1] = 0;	// 2P�����~
	Player p[2];
	p[0] = GP1, p[1] = GP2;
	int rS = 0, lS = 0;
	int xDist = P2.XPos - P1.XPos;
	if (xDist < 0)xDist = -xDist;

	// P1,P2�̕ϐ��ɕۑ�
	for (int i = 0; i < PAD_MAX; i++){
		P2.button[i] = 0;
		P2_B[i] = 0;
	}

	// �O����
	if ((xDist > 180) && (P2.ctrl) && (P2.ctrl) && (P2.stateno != 20)){
		if (GetRand(999) < 100){
			P2.stateno = 20;
		}
	}

	if ((rS > 0) && (P2.muki == 0))P2_B[106]++;
	else if ((rS > 0) && (P2.muki == 1))P2_B[104]++;
	if ((lS > 0) && (P2.muki == 0))P2_B[104]++;
	else if ((lS > 0) && (P2.muki == 0))P2_B[106]++;


	P2_B[7] = 0;	// �|�[�Y�͐�΂�0
}

void Stop_P2_B()
{
	// P1,P2�̕ϐ��ɕۑ�
	for (int i = 0; i < PAD_MAX; i++){
		P2_B[i] = 0;
	}
}


//----------------
//
// �L�[�R���t�B�O���[�h�p
//
//----------------
int load_config()
{
	FILE *fp;

	// �n�C�X�R�A�Z�[�u�p�t�@�C�����J��
	fp = fopen("keyconfig.ini", "rt");

	// �J���Ȃ������ꍇ�͉��������ɏI��
	if (fp == NULL){ printfDx("read error"); return 0; }


	// �C���f�[�^�̓ǂݍ���
	for (int i = 0; i < CO_MAX; i++)
	{
		fscanf(fp, "P1 Key%d = %d\n",
			&i, &co_k[0][i]);
	}
	for (int i = 0; i < CO_MAX; i++)
	{
		fscanf(fp, "P2 Key%d = %d\n",
			&i, &co_k[1][i]);
	}

	for (int i = 0; i < CO_MAX; i++)
	{
		// �p�b�h�P
		fscanf(fp, "P1 Pad%d = %d\n",
			&i, &co_p[0][i]
			);
	}
	for (int i = 0; i < CO_MAX; i++)
	{
		// �p�b�h�Q
		fscanf(fp, "P2 Pad%d = %d\n",
			&i, &co_p[1][i]
			);
	}


	// �t�@�C�������
	fclose(fp);


	return 0;
}


//----------------
//
// ������
//
//----------------

void GetPH_Key(Player GP)
{
	P1 = GP;
}


void GetS_Key(System_t GS)
{
	S = GS;
}

void GetP_Key(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}


//----------------
//
// �l�b�g���[�N
//
//----------------
void Get_Network(boolean net)
{
	network = net;

}

// �{�^�����Z�b�g
void NB_Reset()
{
	for (int num = 0; num < DATA_MAX; num++){
		Data[num] = 0;
		gData[num] = 0;	// 1�񂾂��s��
	}
	for (int i = 0; i < PAD_MAX; i++){
		P1_NB[i] = 0;
		P2_NB[i] = 0;
	}
}

// �l�b�g���[�N�̏����擾
void Network_Setting(int side, IPDATA ip, int port, int handle)
{
	network_side = side;
	Ip = ip;
	Port = port;
	UDPNetHandle = handle;
	//NetHandle = handle;
}

// �����̃L�[�𑗂�
void Send_NetKey()
{
	// ����
	//for (int i = 0; i < 4; i++){
		NetWorkSendUDP(UDPNetHandle, Ip, Port, myData, sizeof(myData));
	//}
}

// ����̃L�[���擾
void Get_NetKey()
{
	// [ UDP ]
	//for (int i = 0; i < 4; i++){
		while(NetWorkRecvUDP(UDPNetHandle, NULL, NULL, netD, sizeof(netD), FALSE) == 0);	// �ꐶ�҂�
		for (int d = 0; d < DATA_MAX; d++){
			if (netD[d] > 0)getData[d] = netD[d];
		}
	//}
	
}

// �擾��������̃L�[�𑗂�
void Send_YourKey()
{
	// ����
	//for (int i = 0; i < 4; i++){
		NetWorkSendUDP(UDPNetHandle, Ip, Port, gData, sizeof(gData));
	//}
}

// �����̃L�[���擾(������Data�Ɍ���)
void Get_MyKey()
{
	// [ UDP ]
	//for (int i = 0; i < 4; i++){
		while(NetWorkRecvUDP(UDPNetHandle, NULL, NULL, netD, sizeof(netD), FALSE) == 0);
		for (int d = 0; d < DATA_MAX; d++){
			if (netD[d] > 0)getMyData[d] = netD[d];
		}
	//}

}

// �l�b�g���[�N�I��
void End_Network(){

	//[ UDP ]
	// �t�c�o�\�P�b�g�n���h���̍폜
	DeleteUDPSocket(UDPNetHandle);
}