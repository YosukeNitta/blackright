#include "pch.h"
#include <iostream>	// ���t�擾�p
#include <string>
#pragma warning(disable : 4996)	// fscanf�̃G���[�\��������

#define CO_MAX (4 + BUTTON_MAX)	// �㉺���E�A8�{�^��

using namespace std;

//===================================
// [ �l�b�g�ΐ� ] //
static boolean network = false;
static IPDATA Ip;
static int Port;
static int network_side = 0;	// 1,1P 2,2P

static const int PUT_MAX = 10;	// �L�^����t���[���̍ő�
static const int DATA_MAX = 10;

static int P1_NB[PAD_MAX]; 

static BYTE gData[DATA_MAX];	// ����(����Ȃ̂Œx�点��K�v�͂Ȃ�)

static BYTE getData[PUT_MAX][DATA_MAX];	// �擾�������͏��(10F�O�܂Ŏ擾)
static BYTE myData[DATA_MAX];	// �����̓��͏��(10F�O�܂Ŏ擾)
//static BYTE myData[DATA_MAX];	// �����̓��͏��()

static BYTE getMyData[DATA_MAX];	// �����̓��͏��
// �����̓���
static BYTE dataLog[PUT_MAX][DATA_MAX];	// ����(�m�肵�����̂�����)
//static BYTE gDataLog[PUT_MAX][DATA_MAX];	// ����(����Ȃ̂Œx�点��K�v�͂Ȃ�)

static BYTE netD[DATA_MAX];	// ���肩��̎󂯎��p

static int net_time = 1;

// [ �p�P�b�g���X ] //
static int lostKey = 0;

// [ ���v���C ] //
static int replayMode = 0;	// 0.�Ȃ� 1.�Đ� 2.�^��
static int rTimeCount;
static int ifp;	// �ǂݍ��݂Ŏg�p
static FILE *fp;
static int r_color[2], r_name[2], r_stage;	// ���v���C�p�f�[�^
static string gString;
static int r_key[2][8];

//===================================
// [ UDP ] //
static int UDPNetHandle;        // �l�b�g���[�N�n���h��
static int NetHandle, LostHandle;    // �l�b�g���[�N�n���h��
//static int DataLength;            // ��M�f�[�^�ʕۑ��p�ϐ�

//===================================
// [ �R���g���[���ݒ� ]
static int device[2];	// 0..1P 1..2P
static int d_Input[2][3];	// 0..L 1..R 2..1

// 1P�A2P�̃p�b�h�ڑ�
static int m_P_Input[2];

// �{�^���̗v�f�i���ۂɎg�����́j�A���ꂪbutton�ɔt��
static vector<int> P1_B;
static vector<int> P2_B;

// �L�[�{�[�h�ł�
static int enter[2];

// ����̉�
//static int ctrl[2];
static int button3[8];

//static int mOld_B[PAD_MAX];
static vector<int> mOld_B;

// �������ɔC����
static vector<int> key;
static vector<int> pad;
static int co_k[2][CO_MAX];
static int co_p[2][CO_MAX];

static int Load_1;

static void NumSetting();
static int load_config();

static void EnterData();

static void KeySetting_Network();	
static void Send_NetKey();
static void Get_NetKey();
int Replay_LoadKey();

// �p�b�h���ڑ�����Ă��邩�m�F
void GetInput()
{
	// �錾
	{
		// P1
		int i;
		if (device[0] == 1) {
			i = GetJoypadInputState(DX_INPUT_PAD1);
			m_P_Input[0] = i;
		}
		else if (device[0] == 2) {
			i = GetJoypadInputState(DX_INPUT_PAD2);
			m_P_Input[0] = i;
		}
		else { m_P_Input[0] = 0; }
		
		
		// P2
		int j;
		//j = GetJoypadInputState(DX_INPUT_PAD2);
		//m_P_Input[1] = j; // j����
		if (device[1] == 1) {
			j = GetJoypadInputState(DX_INPUT_PAD1);
			m_P_Input[1] = j;
		}
		else if (device[1] == 2) {
			j = GetJoypadInputState(DX_INPUT_PAD2);
			m_P_Input[1] = j;
		}
		else { m_P_Input[1] = 0; }


		/*
		if (j != 0) {
			m_P_Input[0] = j;	// i����
			m_P_Input[1] = i;	// i����
		}
		*/
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
	if (CheckHitKey(KEY_INPUT_RETURN) == 1)
	{
		enter[0] += 1;
	}
	else { enter[0] = 0; }
	if (CheckHitKey(KEY_INPUT_BACK) == 1)
	{
		enter[1] += 1;
	}
	else { enter[1] = 0; }


	DINPUT_JOYSTATE input;

	GetJoypadDirectInputState(m_P_Input[0], &input);

	if (!network){	//
		// P1�� ��v�f ��������Ă�����
		
		if (
			((input.POV[0] == 0) || (input.POV[0] == 4500) || (input.POV[0] == 31500)) ||
			((m_P_Input[0] & pad[co_p[0][0]]) != 0) ||
			(CheckHitKey(key[co_k[0][0]]) == 1)
			)
		{
			P1_B[108] += 1;
		}
		else{
			P1_B[108] = 0;
		}
		
		// ��
		if (
			((input.POV[0] == 18000) || (input.POV[0] == 13500) || (input.POV[0] == 22500)) ||
			((m_P_Input[0] & pad[co_p[0][1]]) != 0) ||
			(CheckHitKey(key[co_k[0][1]]) == 1)){
			P1_B[102] += 1;
		}
		else{
			P1_B[102] = 0;
		}
		// ��
		if (
			((input.POV[0] == 27000) || (input.POV[0] == 22500) || (input.POV[0] == 31500)) ||
			((m_P_Input[0] & pad[co_p[0][2]]) != 0) ||
			(CheckHitKey(key[co_k[0][2]]) == 1)){
			P1_B[104] += 1;
		}
		else{
			P1_B[104] = 0;
		}
		// �E
		if (
			((input.POV[0] == 9000) || (input.POV[0] == 4500) || (input.POV[0] == 13500)) ||
			((m_P_Input[0] & pad[co_p[0][3]]) != 0) ||
			(CheckHitKey(key[co_k[0][3]]) == 1)){
			P1_B[106] += 1;
			P1_B[104] = 0;
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
	else if (network){	// networkside���g�킸�A���ʂɂ���
		// P1�� ��v�f ��������Ă�����
		if (
			((m_P_Input[0] & pad[co_p[0][0]]) != 0) ||
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
			P1_NB[104] = 0;
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
		{
			P1_NB[7] = 0;
		}

		// 8�{�^��  �g����(1)
		{
			P1_NB[8] = 0;
		}
		// 9�{�^��  �g����(2)
		{
			P1_NB[9] = 0;
		}
	}	// 1
	return 0;	// �I�����I��I
}

// �{�^�����̓`�F�b�N
// �{�^���z�u��������ꍇ�͂�����

int P2_BCheck()
{
	DINPUT_JOYSTATE input;

	GetJoypadDirectInputState(m_P_Input[1], &input);

	// �ʏ��2P����
	if (!network){
		// P1�� ��v�f ��������Ă�����
		if (
			((input.POV[1] == 0) || (input.POV[1] == 4500) || (input.POV[1] == 31500)) ||
			((m_P_Input[1] & pad[co_p[1][0]]) != 0) ||
			(CheckHitKey(key[co_k[1][0]]) == 1)
			)
		{
			P2_B[108] += 1;
		}
		else{
			P2_B[108] = 0;
		}
		// ��
		if (
			((input.POV[1] == 18000) || (input.POV[1] == 13500) || (input.POV[1] == 22500)) ||
			((m_P_Input[1] & pad[co_p[1][1]]) != 0) ||
			(CheckHitKey(key[co_k[1][1]]) == 1)){
			P2_B[102] += 1;
		}
		else{
			P2_B[102] = 0;
		}
		// ��
		if (
			((input.POV[1] == 27000) || (input.POV[1] == 22500) || (input.POV[1] == 31500)) ||
			((m_P_Input[1] & pad[co_p[1][2]]) != 0) ||
			(CheckHitKey(key[co_k[1][2]]) == 1)){
			P2_B[104] += 1;
		}
		else{
			P2_B[104] = 0;
		}
		// �E
		if (
			((input.POV[1] == 9000) || (input.POV[1] == 4500) || (input.POV[1] == 13500)) ||
			((m_P_Input[1] & pad[co_p[1][3]]) != 0) ||
			(CheckHitKey(key[co_k[1][3]]) == 1)){
			P2_B[106] += 1;
			P2_B[104] = 0;
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
		KeySetting_Network();
	}// �l�b�g���[�N����


	return 0;	// �I�����I��I
}

void InputSend()
{
	// �Đ����[�h
	if (replayMode == 1){
		Replay_LoadKey();
	}
	// P1,P2�̕ϐ��ɕۑ�
	for (int i = 0; i < PAD_MAX; i++){
		P1.button[i] = P1_B[i];
		P2.button[i] = P2_B[i];
	}

	//
	Player P[2];
	P[0] = P1, P[1] = P2;

	// ���o�T����
	//ctrl[0] = P1.ctrl;

	// �e���L�[�ʒu���f�Amuki���t���ƍ��E���ς��i��ɉE�����j
	for (int i = 0; i < 2; i++)
	{
		// ��s���͂̐ݒ�A�Ƃ��6�t��
		for (int j = 1; j < BUTTON_MAX; j++){
			// �L��������ʂ������Ă�Ƃ��͐�s���͂�����
			if (S.StopTime == 0){
				if (P[i].StopTime == 0){
					if (P[i].Senkou[j] > 0)P[i].Senkou[j] -= 1;
					if (P[i].rbutton[j] > 0)P[i].rbutton[j] -= 1;
				}
			}

			// ���o�T(�K�[�h�A�N���オ��)
			if ((P[i].stateno >= 50 && P[i].stateno < 60) ||
				(P[i].stateno == 1055)){
				if (P[i].button[j] == 1)P[i].rbutton[j] = 6;
			}
			else{ if (P[i].button[j] == 1)P[i].rbutton[j] = 1; }

			// ���͂����u�Ԃɐ�s����6�t���ǉ�
			// �������ςŗP�\���L�т�
			if (P[i].button[j] == 1)P[i].Senkou[j] = 6;
		}

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
	// 1P�D��
	{
		if (P1_B[P_Num] != 0)P_InputNum = P1_B[P_Num];
		else if (P2_B[P_Num] != 0)P_InputNum = P2_B[P_Num];

		// ����
		if (P_Num == 1 && (enter[0] > 0))P_InputNum = enter[0];
		// �L�����Z��
		if (P_Num == 2 && (enter[1] > 0))P_InputNum = enter[1];
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
		if (m_P_Input[1] & pad[i])watasu = 1;
	}

	return watasu;
}


int P1_BConfig()
{
	// �ȈՃL�[����
	if (Load_1 == 0){
		NumSetting();
		load_config();
		P1_B.resize(PAD_MAX);
		P2_B.resize(PAD_MAX);
		mOld_B.resize(PAD_MAX);

		device[0] = 1;	// �b��
		device[1] = 2;

		Load_1 = 0;
	}

	// �R���t�B�O�ɉf�鐔�l
	return 0;
}

void NumSetting()
{
	// ��̂��쐬
	key.reserve(PAD_MAX);
	pad.reserve(PAD_MAX);
	for (int i = 0; i < PAD_MAX; i++){
		key.push_back(0);
		pad.push_back(0);
	}

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

	key[108] = 0;

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

	pad[108] = 0;
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
	string str;
	char str2[10];
	// �n�C�X�R�A�Z�[�u�p�t�@�C�����J��
	fp = fopen("keyconfig.ini", "rt");

	// �J���Ȃ������ꍇ�͉��������ɏI��
	if (fp == NULL){ printfDx("error keyconfig\n"); return 0; }

	// �C���f�[�^�̓ǂݍ���
	for (int i = 0; i < CO_MAX; i++)
	{
		str2[0] = '\0';
		fscanf(fp, "P1 Key%d = %s\n",
			//&i, &co_k[0][i]);
			&i, str2);
		co_k[0][i] = atoi(str2);
	}
	for (int i = 0; i < CO_MAX; i++)
	{
		str2[0] = '\0';
		fscanf(fp, "P2 Key%d = %s\n",
			&i, str2);
		co_k[1][i] = atoi(str2);
	}

	for (int i = 0; i < CO_MAX; i++)
	{
		str2[0] = '\0';
		// �p�b�h�P
		fscanf(fp, "P1 Pad%d = %s\n",
			&i, str2);
		co_p[0][i] = atoi(str2);
	}
	for (int i = 0; i < CO_MAX; i++)
	{
		str2[0] = '\0';
		// �p�b�h�Q
		fscanf(fp, "P2 Pad%d = %s\n",
			&i, str2);
		co_p[1][i] = atoi(str2);
	}

	// -1��p��
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < CO_MAX; j++){
			if (co_k[i][j] == -1)co_k[i][j] = 108;
			if (co_p[i][j] == -1)co_p[i][j] = 108;
		}
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

// �l�b�g���[�N�̃L�[������S�čs��
void KeySetting_Network()
{
	// �l�b�g�^�C��
	// 1�`10�����Ă���ɍ����������̃L�[�𔽉f����
	net_time++;
	if (net_time > 10)net_time = 1;
	
	// NB��0�`255�܂�(BYTE�ɑ������̂ŁAbyte��-128�`127)
	for (int i = 0; i < PAD_MAX; i++){
		if (P1_NB[i] > 120)P1_NB[i] = 120;
	}	

	// �f�[�^���q�����Ă�
	{
		// �����̑��M�f�[�^����
		Connect_GetCn(true);
		if (Connect_CheckSide() > 0){
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
			myData[9] = CheckGameMode();
		}
		//
		/////////////////////////////////////////////

		{
			// ��M�f�[�^������
			for (int i = 0; i < DATA_MAX; i++){
				gData[i] = 0;	// �R�}���h
				getData[0][i] = 0;
				getMyData[i] = 0;
				netD[i] = 0;
			}
			// UDP
			///////////////////////
			int cnwr_ret = CheckNetWorkRecvUDP(UDPNetHandle);
			// �ڑ��������Ă���
			if (cnwr_ret && cnwr_ret != -1){
				// �f�[�^�󂯎��
				if (Connect_CheckSide() == 1){
					Send_NetKey();
					Get_NetKey();
					//Send_YourKey();
					//Get_MyKey();
				}
				else if (Connect_CheckSide() == 2){
					Get_NetKey();
					Send_NetKey();
				}
			}
			// �ڑ����Ȃ��Ȃ��Ă���
			else{
				for (int i = 0; i < PAD_MAX; i++){
					P1_NB[i]--;
					if (P1_NB[i] < 0)P1_NB[i] = 0;
				}
			}
			///////////////////////
			
			// �����Ƒ���̃f�[�^�����ɐi�߂�

			///////////////////////
		}

		// �{�^������
		// Data��gData������
		EnterData();

		// �����̃f�[�^�����ɐi�߂�
		for (int data = 0; data < DATA_MAX; data++){
			// ����
			dataLog[9][data] = dataLog[8][data];
			dataLog[8][data] = dataLog[7][data];
			dataLog[7][data] = dataLog[6][data];
			dataLog[6][data] = dataLog[5][data];
			dataLog[5][data] = dataLog[4][data];
			dataLog[4][data] = dataLog[3][data];
			dataLog[3][data] = dataLog[2][data];
			dataLog[2][data] = dataLog[1][data];
			dataLog[1][data] = dataLog[0][data];
		}
	}	// �S�̏I��
}

// �{�^�����Z�b�g
void NB_Reset()
{
	for (int num = 0; num < DATA_MAX; num++){
		gData[num] = 0;	// 1�񂾂��s��
	}

	for (int i = 0; i < PUT_MAX; i++){
		for (int num = 0; num < DATA_MAX; num++){
			dataLog[i][num] = 0;
		}
	}

	for (int i = 0; i < PAD_MAX; i++){
		P1_NB[i] = 0;
	}

	//delayNum[0] = 0;
	//delayNum[1] = 0;
}

// �l�b�g���[�N�̏����擾
void Network_Setting(int side, IPDATA ip, int port, int handle)
{
	//network_side = side;
	network_side = Connect_CheckSide();
	Ip = Connect_CheckIp();
	Port = Connect_CheckPort();
	UDPNetHandle = Connect_CheckNetHandle();
}

// �����̃L�[�𑗂�
void Send_NetKey()
{
	//===============================
	// �p�^�[��2:���ړI(�����΍�Ȃ�)
	//
	// 10F�O�܂ł̋L�^�ƈꏏ�ɑ���
	// �ŐV���O�Ɏ����̃L�[���͂�����
	for (int data = 0; data < DATA_MAX; data++){
		dataLog[0][data] = myData[data];
	}
	// ���M(�}�C�i�X�Ȃ�G���[�A�l�ɂ���Č������Ⴄ)
	while (true){
		if (NetWorkSendUDP(UDPNetHandle, Ip, Port, dataLog, sizeof(dataLog)) == 0)
			break;
	}
	// �����̃f�[�^���M����
	// ���ɐi�߂�̂�
	// �ʂł���Ă��炤
	//===============================
}

// ����̃L�[���擾
void Get_NetKey()
{
	//===============================
	// �p�^�[��2:���ړI�Ȏ󂯎��(10F�O���擾�E�����΍�Ȃ�)
	if (CheckNetWorkRecvUDP(UDPNetHandle) == TRUE){	// �����Ƃ������󂯎��
		while (true){
			if (NetWorkRecvUDP(UDPNetHandle, NULL, NULL, getData, sizeof(getData), FALSE) == 0)
				break;
		}
	}
	Connect_Get_GetData(getData[0][9], 0, 9);	// �ŐV�̃Q�[�����[�h����

	/*
	if (getData[0][0] == 0){
		lostKey++;
		StopTime_Add(1);
	}
	else if (getData[0][0] > 0){
		if (lostKey > 0){
			// �����ŌJ��Ԃ�����
			for (int d = 0; d < DATA_MAX; d++){
				gData[d] = getData[0][d];	// �����E�p�P�b�g���X�C�ɂ������ꂿ�Ⴄ
				Data[d] = myData[d];	// �����Y���C�ɂ������ړ��ꂿ�Ⴄ(������)
			}
			lostKey--;
		}
	}
	*/
	
	if (lostKey <= 0){
		for (int d = 0; d < DATA_MAX; d++){
			gData[d] = getData[0][d];	// �����E�p�P�b�g���X�C�ɂ������ꂿ�Ⴄ
		}
	}
}

// Data�ɓ��ꂽ�L�[��P1_B�ɔ��f������(�ŏI�i�K)
// �����̑����x�点��
void EnterData()
{
	int n = 1;	// �f�t�H��2
	// �f�B���C�̒l(1�`9����������)
	// 0����Ȃ��悤�ɁA�ʐM�x���Ȃ��͂��肦�Ȃ�
	//if ((Connect_CheckDelay() > 0) && (Connect_CheckDelay() < PUT_MAX))n = Connect_CheckDelay();
	//else if (Connect_CheckDelay() >= PUT_MAX)n = 9;

	if (Connect_CheckSide() == 1){
		// 1P
		P1_B[1] = dataLog[n][1];
		P1_B[2] = dataLog[n][2];
		P1_B[3] = dataLog[n][3];
		P1_B[4] = dataLog[n][4];
		P1_B[102] = dataLog[n][5];
		P1_B[104] = dataLog[n][6];
		P1_B[106] = dataLog[n][7];
		P1_B[108] = dataLog[n][8];
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
	else if (Connect_CheckSide() == 2){
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
		P2_B[1] = dataLog[n][1];
		P2_B[2] = dataLog[n][2];
		P2_B[3] = dataLog[n][3];
		P2_B[4] = dataLog[n][4];
		P2_B[102] = dataLog[n][5];
		P2_B[104] = dataLog[n][6];
		P2_B[106] = dataLog[n][7];
		P2_B[108] = dataLog[n][8];
	}
}

// �l�b�g���[�N�I��
void End_Network(){

	//[ UDP ]
	// �t�c�o�\�P�b�g�n���h���̍폜
	DeleteUDPSocket(UDPNetHandle);
}

//----------------
//
// ���v���C�L�[�ۑ�
//
//----------------
// �^��
int Replay_ReserveKey()
{
	// �I�[�v���ł��Ȃ������炱���ŏI��
	if (fp == NULL){ return FALSE; }

	// [sizeof]�̓������T�C�Y�𒲂ׂ�
	//fprintf(fp, "\n");		// �Ȃ񂩍ŏ��̍s���������̂ł���g��
	{
		// �ŏ��̂�
		if (rTimeCount == 0)fprintf(fp, "[setting]%d,%d,%d,%d,%d\n", 
			r_name[0], r_name[1], r_color[0], r_color[1], r_stage);

		fprintf(fp, "[%d]%d,%d,%d,%d,%d,%d,%d,%d,\n", 
			rTimeCount,
			P1_B[108], P1_B[102], P1_B[104], P1_B[106],
			P1_B[1], P1_B[2], P1_B[3], P1_B[4]);
		fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d,\n",
			P2_B[108], P2_B[102], P2_B[104], P2_B[106],
			P2_B[1], P2_B[2], P2_B[3], P2_B[4]);
	}
	rTimeCount++;


	/* �ǂݍ��݃f�[�^�̊m�F */
	return TRUE;
}


int Replay_LoadKey()
{
	// ����Z�b�e�B���O //

	int i;
	int count;	// ����̏ꏊ(�ŏ���[ ] 2��)
	int nCount;	// ���s�̃J�E���g
	boolean iflg;
	count = -1, nCount = 0;

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// input��inputc�ɕ������͂���

	// �͂��߂�
	if (rTimeCount == 0){
		// �t�@�C���|�C���^��擪�Ɉړ�����
		FileRead_seek(ifp, 0, SEEK_SET);
		//�ŏ���1�s�ǂݔ�΂�
		while (FileRead_getc(ifp) != '\n'); // FileRead_getc:�ꕶ���ǂݏo��;
	}

	// �t�@�C�����J�� //
	while (1){
		//=========================
		// ��{���[�v
		// ���̃��[�v�I����ɉ���
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(ifp);//1�����擾����

			// ���s�œǂݍ��ݏI��
			if (input[i] == '\n'){
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				nCount++;
				count = 0;
				break;
			}

			// ������ɂ���
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == ';'){ //�J���}�����s�Ȃ�
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				break;	// i��1���₵�ă��[�v�E�o�i�������j
			}

			// �ǂݍ��ݏI��
			if (input[i] == EOF){//�t�@�C���̏I���Ȃ�
				FileRead_close(ifp);
				ModeChange(SceneNum(ReplayScene));
				goto EXLOAD;//�I��
			}
		}	// ��{���[�v�I��

		// �ŏ��ɐ��������ɂ��āA�����������Ă���ύX����
		iflg = false;
		{
			if (inputc[0] == '0')iflg = true;
			else if (inputc[0] == '1')iflg = true;
			else if (inputc[0] == '2')iflg = true;
			else if (inputc[0] == '3')iflg = true;
			else if (inputc[0] == '4')iflg = true;
			else if (inputc[0] == '5')iflg = true;
			else if (inputc[0] == '6')iflg = true;
			else if (inputc[0] == '7')iflg = true;
			else if (inputc[0] == '8')iflg = true;
			else if (inputc[0] == '9')iflg = true;
		}

		// 0�`7�ő��
		if (iflg){
			if ((count >= 0) && (count <= 7)){
				r_key[nCount][count] = atoi(inputc);
			}
			count++;
		}

		// 2����s����
		if (nCount == 2){
			goto EXLOAD;
		}
	}
EXLOAD:

	int pa1, pa2;
	pa1 = P1_B[7];
	pa2 = P2_B[7];

	// P1,P2�̕ϐ��ɕۑ�
	for (int i = 0; i < PAD_MAX; i++){
		P1_B[i] = 0;
		P2_B[i] = 0;
	}

	P1_B[7] = pa1;
	P2_B[7] = pa2;

	// �������
	P1_B[108] = r_key[0][0];
	P1_B[102] = r_key[0][1];
	P1_B[104] = r_key[0][2];
	P1_B[106] = r_key[0][3];
	P1_B[1] = r_key[0][4];
	P1_B[2] = r_key[0][5];
	P1_B[3] = r_key[0][6];
	P1_B[4] = r_key[0][7];

	P2_B[108] = r_key[1][0];
	P2_B[102] = r_key[1][1];
	P2_B[104] = r_key[1][2];
	P2_B[106] = r_key[1][3];
	P2_B[1] = r_key[1][4];
	P2_B[2] = r_key[1][5];
	P2_B[3] = r_key[1][6];
	P2_B[4] = r_key[1][7];

	/* �ǂݍ��݃f�[�^�̊m�F */
	rTimeCount++;
	return 0;
}

// 0.�Ȃ� 1.�Đ� 2.�^�� ����ȊO(-1��)�A��Ԋm�F
int Replay_Mode(int num)
{
	// 0�`2
	if ((num >= 0) && (num <= 2)){
		replayMode = num;
		rTimeCount = 0;	// �����ŏ�����

		// �Đ�
		if (replayMode == 1){
			string str;	// ������(���ꂪ���O)
			str = "replay/" + gString;
			//strcpy(fileHomo, str.c_str());

			// (�Q�Ԗڂ̈�����"wb"��'w'�́u�������݁v�̈Ӗ�
			// 'b'�̓o�C�i���̈Ӗ�(�t�̓e�L�X�g))
			ifp = FileRead_open(str.c_str());//�t�@�C���ǂݍ��� c_str��char�ɂł���H
			if (ifp == NULL){			// �G���[���N��������
				printfDx("�Đ��ł��܂���ł���\n");// ������\��
				return 0;
			}
		}
		//�^��
		else if (replayMode == 2){
			// ���t�擾
			DATEDATA Date;
			// ���ݎ����𓾂�
			GetDateTime(&Date);

			string str;	// ������(���ꂪ���O)
			string year = std::to_string(Date.Year);
			string mon = std::to_string(Date.Mon);
			string day = std::to_string(Date.Day);
			string hour = std::to_string(Date.Hour);
			string min = std::to_string(Date.Min);
			if (mon.size() == 1)mon = "0" + std::to_string(Date.Mon);	// 0������
			if (day.size() == 1)day = "0" + std::to_string(Date.Day);	// 0������
			if (hour.size() == 1)hour = "0" + std::to_string(Date.Hour);	// 0������
			if (min.size() == 1)min = "0" + std::to_string(Date.Min);	// 0������

			str = "replay/" + year + mon + day + "_" + hour + min + ".rep";

			// (�Q�Ԗڂ̈�����"wb"��'w'�́u�������݁v�̈Ӗ�
			// 'b'�̓o�C�i���̈Ӗ�(�t�̓e�L�X�g))
			if ((fopen_s(&fp, str.c_str(), "wt")) != 0)
				printfDx("�ۑ��ł��܂���ł���\n");
		}
	}

	return replayMode;
}

void Key_GetString(string st)
{
	gString = st;
}

//
// ���v���C
//

void Replay_End()
{
	// �I�[�v���ł�����
	if (fp != NULL){ 
		// �t�@�C�������
		fclose(fp);
	}
	if (ifp != NULL){
		// �t�@�C�������
		FileRead_close(ifp);
	}
	fp = NULL;	// �t�@�C������ɂ���
	ifp = NULL;	// �t�@�C������ɂ���
	gString = "0";
	replayMode = 0;
}

void Replay_Setting(int n1, int n2, int c1, int c2, int stageNum)
{
	r_name[0] = n1, r_name[1] = n2;
	r_color[0] = c1, r_color[1] = c2;
	r_stage = stageNum;
}


void InputDevice_Start() {
	device[0] = -1;
	device[1] = -1;
}

int InputDevice_Update() {
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT) != 0) {
		d_Input[0][0]++;
	}
	else { d_Input[0][0] = 0; }
	
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT) != 0) {
		d_Input[0][1]++;
	}
	else { d_Input[0][1] = 0; }
	
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1) != 0) {
		d_Input[0][2]++;
	}
	else { d_Input[0][2] = 0; }

	// 2P
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_LEFT) != 0) {
		d_Input[1][0]++;
	}
	else { d_Input[1][0] = 0; }

	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_RIGHT) != 0) {
		d_Input[1][1]++;
	}
	else { d_Input[1][1] = 0; }

	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_1) != 0) {
		d_Input[1][2]++;
	}
	else { d_Input[1][2] = 0; }


	// ���E

	// ���󂢂Ă�
	if (device[0] == -1) {
		if (d_Input[0][0] == 1) {
			if(device[0] != 1 && device[1] != 1)
				device[0] = 1;
			// ���ɂ���ꍇ
			else if (device[1] == 1)
				device[1] = -1;
		}
		else if (d_Input[1][0] == 1) {
			if (device[0] != 2 && device[1] != 2)
			device[0] = 2;
			// ���ɂ���ꍇ
			else if (device[1] == 2)
				device[1] = -1;
		}
	}

	if (device[1] == -1) {
		if (d_Input[0][1] == 1) {
			if (device[0] != 1 && device[1] != 1)
				device[1] = 1;
			// ���ɂ���ꍇ
			else if (device[0] == 1)
				device[0] = -1;
		}
		else if (d_Input[1][1] == 1) {
			if (device[0] != 2 && device[1] != 2)
				device[1] = 2;
			// ���ɂ���ꍇ
			else if (device[0] == 2)
				device[0] = -1;
		}
	}

	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1) != 0) {
		return 1; //���S����
	}
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_1) != 0) {
		return 1; //���S����
	}

	/*
	{
		// ����
		if (device[0] == GetJoypadInputState(DX_INPUT_PAD1) ||
			device[1] == GetJoypadInputState(DX_INPUT_PAD1)) {

		}
		if (device[0] == GetJoypadInputState(DX_INPUT_PAD2) ||
			device[1] == GetJoypadInputState(DX_INPUT_PAD2)) {
			if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_1) != 0) {
				return 1; //���S����
			}
		}
	}
	*/

	return 0;
}

void InputDevice_Draw() {
	if (device[0] == 1){
		DrawString(20,60,"PAD1", Cr);
	}
	else if (device[0] == 2) {
		DrawString(20, 60, "PAD2", Cr);
	}
	else {
		DrawString(20, 60, "NO INPUT", Cr);
	}

	if (device[1] == 1) {
		DrawString(SCREEN_W - 60, 60, "PAD1", Cr);
	}
	else if (device[1] == 2) {
		DrawString(SCREEN_W - 60, 60, "PAD2", Cr);
	}
	else {
		DrawString(SCREEN_W - 100, 60, "NO INPUT", Cr);
	}

	// ����
	// �ǂ���ɂ��Ȃ�
	if (device[0] != 1 && device[1] != 1)
		if (GetJoypadNum() >= 1)DrawString(SCREEN_W / 2 - 40, 50, "PAD1", Cr);
	if (device[0] != 2 && device[1] != 2)
		if (GetJoypadNum() >= 2)
		DrawString(SCREEN_W / 2 - 40, 70, "PAD2", Cr);
}