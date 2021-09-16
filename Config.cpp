#include "pch.h"
#include "Config.h"
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

// �p�b�h����
static const int pad_max = 32;
static int padInput[pad_max];



// �摜
static int cursor;

static void Update();
//static void Draw();

// �R���t�B�O�̃��C���֐�
int Config::Mode()
{
	if(Load_1 == 0){

		// �I���|�C���^�̈ʒu������
		SPoint = 0;
		TPoint = 0;
		YPoint = 0;
		// �I������
		SPMax = 5;
		TPMax = 2;
		// �摜�ǂݍ���
		cursor = LoadGraph("OB/cursor.png");

		// �I�����ڂ̐����擾
		kettei = 0;
		kettei += 1;

		Load_1 = 1;
	}

	if (SPoint == 0) {
		// �ǂ������ŏI��
		if ((P_BInput(2) == 1) ||
			(TPoint == 1 && P_BInput(1) == 1))
		{
			// �������ς�����
			P1_BCheck();
			SEStart(37);
			// �I���A���j���[��
			MainSystem::Instance().SetNextMode("Menu");
		}
	}
	else if (SPoint == 1) {
		/*
		// �o�b�N�X�y�[�X�ŏI��
		if (P_BInput(2) == 1)
		{
			// �������ς�����
			P1_BCheck();
			SPoint = 0;
		}
		*/
	}

	// ���[�v�A�v���O�������s�J�n
	{
		Update();

		Draw();

		// �I��
		if ((SPoint == 0) && (P_BInput(2) == 1)) {
			// �������ς�����
			P1_BCheck();
			SEStart(37);
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
			SEStart(36);
		}
		else if (P_BInput(102) == 1) {
			TPoint++;
			SEStart(36);
		}

		if (TPoint < 0)TPoint = TPMax - 1;
		else if (TPoint > TPMax - 1)TPoint = 0;

		// ����
		if (P_BInput(1) == 1) {
			SPoint = TPoint + 1;
			SEStart(35);
		}
	}
	
	// �{�^���ݒ�
	if (SPoint == 1) {
		// �f�o�C�X
		/*
		if (InputDevice_Update() == 1) {
			// �I���A���j���[��
			MainSystem::Instance().SetNextMode("Menu");
		}
		*/
		// �f�o�C�X1P2P�\��
		//InputDevice_Draw();

		if (P_BInput(108) == 1) {
			TPoint--;
			SEStart(36);
		}
		else if (P_BInput(102) == 1) {
			TPoint++;
			SEStart(36);
		}
		// �I���̍��ڂ�+1
		if (TPoint < 0)TPoint = 9 + 1 - 1;
		else if (TPoint > 9 + 1 - 1)TPoint = 0;

		if (P_BInput(104) == 1) {
			YPoint--;
			SEStart(36);
		}
		else if (P_BInput(106) == 1) {
			YPoint++;
			SEStart(36);
		}
		if (YPoint < 0)YPoint = 2 - 1;
		else if (YPoint > 2 - 1)YPoint = 0;

		// �{�^������
		// 1P
		// �͈͓��Ȃ�{�^���Z�b�g
		if (TPoint < 9) {
			//1P
			if (YPoint == 0) {
				for (int i = 4; i < pad_max; i++) {
					// �������͂���
					if ((P_Input(1) & padInput[i]) != 0) {
						int setNum;	// �\���p
						setNum = Key_ReturnButton(0, 4 + TPoint);
						
						// �O�̃L�[�ƈႤ�Ȃ�
						if (padInput[i] != setNum) {
							// �v���C���[�A �{�^���A���e
							Key_SetButton(0, 4 + TPoint, padInput[i]);
							SEStart(35);
						}
						
					}
				}
			}
			// 2P
			else if (YPoint == 1) {
				for (int i = 1; i < pad_max; i++) {
					// �������͂���
					if ((P_Input(2) & padInput[i]) != 0) {
						int setNum;	// �\���p
						setNum = Key_ReturnButton(1, 4 + TPoint);
						
						// �O�̃L�[�ƈႤ�Ȃ�
						if (padInput[i] != setNum) {
							// �v���C���[�A �{�^���A���e
							Key_SetButton(1, 4 + TPoint, padInput[i]);
							SEStart(35);
						}
						
					}
				}
			}
		}

		// �o�b�N�X�y�[�X�ŏI��
		// ���ڌ���ł��I��
		if (CheckHitKey(KEY_INPUT_BACK) == 1 || 
			(TPoint == 9 && (P_BInput(1) == 1 || P_BInput(2) == 1)))
		{
			// �������ς�����
			P1_BCheck();
			SPoint = 0;
			SEStart(37);
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

void Config::Draw() {
	// �`��
	{
		// �e�X�g
		DrawFormatString(SCREEN_W - 80, 20, Cr, "S:%d", SPoint);
		

		if (SPoint == 0){
			// �p�b�h�ڑ�
			DrawFormatString(SCREEN_W - 120, 0 + 60, GetColor(255, 255, 255), "�p�b�h�ڑ��� %d", GetJoypadNum());

			// �J�[�\��
			DrawString(0, (20 * TPoint), "��", Cr);

			DrawString(SCREEN_W - 80, 40, "���j���[", Cr);

			//DrawString(20, 0, "�{�^���ݒ�", Cr);
			DrawString(20, 0, "�{�^���ݒ�", Cr);
			
			/*
			DrawString(20, 20, "�E�B���h�E�T�C�Y�ύX", Cr);
			int window = GetPrivateProfileInt("Config", "windowmode", 1, "./config.ini");
			if(window == 0)DrawString(220, 20, "�t���X�N���[��", Cr);
			else { DrawString(220, 20, "�E�B���h�E���[�h", Cr); }

			DrawString(20, 40, "�t���[�����[�g�\��", Cr);
			int fps = GetPrivateProfileInt("Config", "drawfps ", 1, "./config.ini");
			if (fps == 0)DrawString(220, 40, "���Ȃ�", Cr);
			else { DrawString(220, 40, "����", Cr); }
			*/

			DrawString(20, ((TPMax - 1) * 20), "���j���[�ɖ߂�", Cr);

			DrawString(0, 400, "Enter:����  Backspace:�L�����Z��", Cr);
		}
		else if (SPoint == 1){
			DrawString(SCREEN_W - 100, 40, "�{�^���ݒ�", Cr);
		}
		else if (SPoint == 2){
			DrawString(SCREEN_W - 80, 40, "�p�b�h����", Cr);
		}
		
		if (SPoint == 1){
			int xp = 20;
			int yp = 80;

			// �J�[�\��
			DrawString(0, (20 * TPoint) + yp, "��", Cr);

			if(YPoint == 0)DrawString(0, 0, "�Q�[���p�b�h:1P", Cr);
			else if (YPoint == 1)DrawString(0, 0, "�Q�[���p�b�h:2P", Cr);
			
			// �L�[�{�[�h
			DrawString(xp, yp + 20 * 0, "�`(��)", Cr);
			DrawString(xp, yp + 20 * 1, "�a(��)", Cr);
			DrawString(xp, yp + 20 * 2, "�b(��)", Cr);
			DrawString(xp, yp + 20 * 3, "�c(����)", Cr);
			DrawString(xp, yp + 20 * 4, "�`+�a+�b(3�Q�[�W)", Cr);
			DrawString(xp, yp + 20 * 5, "�`+�a(����)", Cr);
			DrawString(xp, yp + 20 * 6, "�o(�|�[�Y)", Cr);
			DrawString(xp, yp + 20 * 7, "�e�m�P(�^��E�ʒu���Z�b�g)", Cr);
			DrawString(xp, yp + 20 * 8, "�e�m�Q(�Đ�)", Cr);

			for (int i = 0; i < 9; i++) {
				int setNum;	// �\���p
				setNum = Key_ReturnButton(YPoint, i + 4);
				// �\���p�ɖ|��
				{
					if (setNum == padInput[4]) setNum = 1;
					if (setNum == padInput[5]) setNum = 2;
					if (setNum == padInput[6]) setNum = 3;
					if (setNum == padInput[7]) setNum = 4;
					if (setNum == padInput[8]) setNum = 5;
					if (setNum == padInput[9]) setNum = 6;
					if (setNum == padInput[10]) setNum = 7;
					if (setNum == padInput[11]) setNum = 8;
					if (setNum == padInput[12]) setNum = 9;
					if (setNum == padInput[13]) setNum = 10;
					if (setNum == padInput[14]) setNum = 11;
					if (setNum == padInput[15]) setNum = 12;
					if (setNum == padInput[16]) setNum = 13;
					if (setNum == padInput[17]) setNum = 14;
				}
				DrawFormatString(xp + 240, yp + 20 * i, Cr, "%d", setNum);
			}
			DrawString(xp, yp + 20 * 9, "�I��", Cr);
			DrawString(0, 400, "���E�ŃQ�[���p�b�h��ύX���܂�", Cr);
			DrawString(0, 420, "Backspace:�L�����Z��", Cr);
		}
		
		
	}
}

void Config::Load_Reload()
{
	// �Đڑ�
	ReSetupJoypad();
}

void Config::Load_1second()
{
	Load_1 = 0;
	InputDevice_Start();	// ������

	// �p�b�h
	padInput[0] = 0;
	padInput[1] = 0;
	padInput[2] = 0;
	padInput[3] = 0;
	
	padInput[1 + 3] = PAD_INPUT_1;
	padInput[2 + 3] = PAD_INPUT_2;
	padInput[3 + 3] = PAD_INPUT_3;
	padInput[4 + 3] = PAD_INPUT_4;
	padInput[5 + 3] = PAD_INPUT_5;
	padInput[6 + 3] = PAD_INPUT_6;
	padInput[7 + 3] = PAD_INPUT_7;
	padInput[8 + 3] = PAD_INPUT_8;
	padInput[9 + 3] = PAD_INPUT_9;
	padInput[10 + 3] = PAD_INPUT_10;
	padInput[11 + 3] = PAD_INPUT_11;
	padInput[12 + 3] = PAD_INPUT_12;
	padInput[13 + 3] = PAD_INPUT_13;
	padInput[14 + 3] = PAD_INPUT_14;
	padInput[15 + 3] = PAD_INPUT_15;
	padInput[16 + 3] = PAD_INPUT_16;
	padInput[17 + 3] = PAD_INPUT_17;
	padInput[18 + 3] = PAD_INPUT_18;
	padInput[19 + 3] = PAD_INPUT_19;
	padInput[20 + 3] = PAD_INPUT_20;
	padInput[21 + 3] = PAD_INPUT_21;
	padInput[22 + 3] = PAD_INPUT_22;
	padInput[23 + 3] = PAD_INPUT_23;
	padInput[24 + 3] = PAD_INPUT_24;
	padInput[25 + 3] = PAD_INPUT_25;
	padInput[26 + 3] = PAD_INPUT_26;
	padInput[27 + 3] = PAD_INPUT_27;
	padInput[28 + 3] = PAD_INPUT_28;
}

void Config::Release(void)
{

}