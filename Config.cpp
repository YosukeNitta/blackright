#include "pch.h"
#include "MainSystem.h"
static int Load_1;
static int P1_Key, P2_Key, Gamen;
static int inputUP, inputDOWN;
static int TimeStop, kettei;

static int Load1, Load2;

// �I���|�C���^
// 0�Ń��j���[ 1:�{�^���ݒ� 2:�p�b�h�����ݒ�
static int SPoint;
// �|�C���^�c
static int TPoint;
// �|�C���^��
static int YPoint;

static int SPMax;
static int TPMax;

// �摜
static int cursor;

static void Update();
static void Draw();

// �R���t�B�O�̃��C���֐�
int Config()
{
	if(Load_1 == 0){

		// �I���|�C���^�̈ʒu������
		SPoint = 0;
		TPoint = 0;
		YPoint = 0;
		// �I������
		SPMax = 5;
		TPMax = 5;
		// �摜�ǂݍ���
		cursor = LoadGraph("OB/cursor.png");

		// �I�����ڂ̐����擾
		kettei = 0;
		kettei += 1;

		Load_1 = 1;
	}

	if (SPoint == 0) {
		// �o�b�N�X�y�[�X�ŏI��
		if (P_BInput(2) == 1)
		{
			// �������ς�����
			P1_BCheck();
			// �I���A���j���[��
			MainSystem::Instance().SetNextMode("Menu");
		}
	}
	else if (SPoint == 1) {
		// �o�b�N�X�y�[�X�ŏI��
		if (P_BInput(2) == 1)
		{
			// �������ς�����
			P1_BCheck();
			SPoint = 0;
		}
	}

	// ���[�v�A�v���O�������s�J�n
	{
		Update();

		Draw();

		// �I��
		if ((SPoint == 0) && (P_BInput(2) == 1)) {
			// �������ς�����
			P1_BCheck();
			// �I���A���j���[��
			MainSystem::Instance().SetNextMode("Menu");
		}

	}//�S�̂̃��[�v

	return 0;
}

void Update() {

	// ���͏�ԃQ�b�g
	P1_Key = P_Input(1);
	P2_Key = P_Input(2);

	// �L�[���͏���
	if (P1_Key) {
		kettei += 1;
	}
	else {
		kettei = 0;
	}

	// �͈͂̌���
	if (SPoint < 0)SPoint = SPMax - 1;
	if (SPoint > SPMax - 1)SPoint = 0;

	// ���ڈړ�
	if (SPoint == 0) {
		if (P_BInput(108) == 1) {
			TPoint--;
		}
		else if (P_BInput(102) == 1) {
			TPoint++;
		}

		if (TPoint < 0)TPoint = TPMax - 1;
		else if (TPoint > TPMax - 1)TPoint = 0;

		// ����
		if (P_BInput(1) == 1) {
			SPoint = TPoint + 1;
		}
	}
	
	// �ł΂���
	if (SPoint == 1) {
		// �f�o�C�X
		if (InputDevice_Update() == 1) {
			// �I���A���j���[��
			MainSystem::Instance().SetNextMode("Menu");
		}
		InputDevice_Draw();

		// �o�b�N�X�y�[�X�ŏI��
		if (P_BInput(2) == 1)
		{
			// �������ς�����
			P1_BCheck();
			SPoint = 0;
		}
	}
	// �E�B���h�E�ύX
	else if (SPoint == 2) {
		// �������ς�����
		P1_BCheck();
		SPoint = 0;
	}
	// FPS
	else if (SPoint == 3) {
		// �������ς�����
		P1_BCheck();
		SPoint = 0;
	}
	// BGM
	else if (SPoint == 4) {
		// �������ς�����
		P1_BCheck();
		SPoint = 0;
	}
	// �I��
	else if (SPoint == TPMax) {
		// �������ς�����
		P1_BCheck();
		P2_BCheck();
		// �I���A���j���[��
		MainSystem::Instance().SetNextMode("Menu");
	}
}

void Draw() {
	// �`��
	{
		// �e�X�g
		DrawFormatString(SCREEN_W - 80, 20, Cr, "S:%d", SPoint);
		

		if (SPoint == 0){
			// �J�[�\��
			DrawString(0, (20 * TPoint), "��", Cr);

			DrawString(SCREEN_W - 80, 40, "���j���[", Cr);

			//DrawString(20, 0, "�{�^���ݒ�", Cr);
			DrawString(20, 0, "�Q�[���p�b�h���蓖��", Cr);
			
			DrawString(20, 20, "�E�B���h�E�T�C�Y�ύX", Cr);
			int fps = GetPrivateProfileInt("Config", "windowmode", 1, "./config.ini");
			if(fps == 0)DrawString(220, 20, "�t���X�N���[��", Cr);
			else { DrawString(220, 20, "�E�B���h�E���[�h", Cr); }

			DrawString(20, 40, "�t���[�����[�g�\��", Cr);

			DrawString(20, ((TPMax - 1) * 20), "���j���[�ɖ߂�", Cr);
		}
		else if(SPoint == 1){
			DrawString(SCREEN_W - 80, 40, "�{�^���ݒ�", Cr);
		}
		else if (SPoint == 1){
			DrawString(SCREEN_W - 80, 40, "�p�b�h����", Cr);
		}
		/*
		if (SPoint == 1){
			DrawString(0, 0, "�L�[�{�[�h:1P", Cr);

			int yp = 22;
			int yp2 = yp * 4;
			// �L�[�{�[�h
			DrawString(0, yp * 1, "��", Cr);
			DrawString(0, yp * 2, "��", Cr);
			DrawString(0, yp * 3, "��", Cr);
			DrawString(0, yp * 4, "��", Cr);

			
			DrawString(0, yp * 1 + yp2, "�`(��)", Cr);
			DrawString(0, yp * 2 + yp2, "�a(��)", Cr);
			DrawString(0, yp * 3 + yp2, "�b(��)", Cr);
			DrawString(0, yp * 4 + yp2, "�c(����)", Cr);
			DrawString(0, yp * 5 + yp2, "�`+�a+�b(3�Q�[�W)", Cr);
			DrawString(0, yp * 6 + yp2, "�`+�a(����)", Cr);
			DrawString(0, yp * 7 + yp2, "�o(�|�[�Y)", Cr);
			DrawString(0, yp * 8 + yp2, "�e�m�P(�^��E�|�W�V�������Z�b�g)", Cr);
			DrawString(0, yp * 9 + yp2, "�e�m�Q(�Đ�)", Cr);
		}
		DrawString(0, 400, "�㉺�L�[�ō��ڂ�ύX�ł��܂�", Cr);
		*/
	}
}

void Load_Config()
{
	Load_1 = 0;
	InputDevice_Start();	// ������
}