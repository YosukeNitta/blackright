//�C���N���[�h��--------------------------------------------
#include "pch.h"

//#include <list>
//using namespace std;
#include "Menu.h"
#include "MainSystem.h"

static int* test1;
static int* test2;
static int* test3;
//static boolean load_1 = false;
//static boolean load_f = false; //��x�̂�
static int P1_Key;
static int inputUP, inputDOWN;
static int TimeStop, kettei;
static int modeG[8];
static int config;	// �R���t�B�O�摜
static int back, mode[8];
// ���݂̑I��ԍ�, �I�����ڂ̐�
static int SPoint = 0;
static const int SentakuNum = 8;
static int next;	// ���̍��ڂւ̃X�N���[��
static int gameTime;	// ����
static int anten;	// �I�����̈Ó]
//static void Draw();

//static 
int Menu::Mode()
{
	// ���[�v�A�v���O�������s�J�n
	{

		//*********
		// �K�{����
		//*********

		if (P_BInput(108) == 1){
			inputUP += 1;
		}
		else{
			inputUP = 0;
		}
		// ���L�[
		if (P_BInput(102) == 1){
			inputDOWN += 1;
		}
		else{
			inputDOWN = 0;
		}

		// �L�[���͂ɉ����ď���
		// �J�[�\���ړ�
		// ��L�[�̓��͎��ԁA���莞�͓����Ȃ�
		if (kettei == 0){
			if (KInput(P_BInput(108))){
				SPoint--, SEStart(36);
				next = 3;
			}
			if (KInput(P_BInput(102))){
				SPoint++, SEStart(36);
				next = -3;
			}
			if (SPoint < 0)SPoint = SentakuNum - 1;
			if (SPoint > SentakuNum - 1)SPoint = 0;
		}

		// �L�[����
		// ����L�[������
		if ((P_BInput(1) == 1 || P_BInput(3) == 1) && (kettei == 0))
		{
			kettei = 1;
			SEStart(35);
		}

		// ���肵�Ă���J�E���g���Z
		if (kettei == 1){
			TimeStop += 1;
		}
		else{
			TimeStop = 0;
		}

		// �X�N���[���ύX
		if (next > 0){
			next -= 1;
		}
		else if(next < 0){
			next += 1;
		}

		// �Q�[���^�C�����Z
		gameTime++;
		if (gameTime > 300)gameTime = 300;


		// �^�[�{���[�h�ύX
		/*
		if (P_BInput(4) == 1) {
			if (Versus_GetTurboMode()) {
				SEStart(35);
			}
			else { SEStart(38); }
			Versus_TurboMode();
		}
		*/
		
		
		// 1 �b�o�߂����Ή�
		// ARCADE
		if ((kettei == 1) && (TimeStop > 10) && (SPoint == 0)) {
			TraningSwitch(false);
			AISwitch(true);
			Arcade_Switch(1);
			BattleMode(3);
			WinCount(0);
			//ModeChange(SceneNum(SelectScene));	// �L�����Z����
			MainSystem::Instance().SetNextMode("Select");
		}
		// �ΐ�
		if ((kettei == 1) && (TimeStop > 10) && (SPoint == 1)) {
			TraningSwitch(false);
			AISwitch(false);
			Arcade_Switch(0);
			BattleMode(0);
			WinCount(0);
			//ModeChange(SceneNum(SelectScene));	// �L�����Z����
			MainSystem::Instance().SetNextMode("Select");
		}
		// VS.AI
		if ((kettei == 1) && (TimeStop > 10) && (SPoint == 2)) {
			TraningSwitch(false);
			AISwitch(true);
			Arcade_Switch(0);
			BattleMode(2);
			WinCount(0);
			//ModeChange(SceneNum(SelectScene));	// �L�����Z����
			MainSystem::Instance().SetNextMode("Select");
		}
		// �g����
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 3)) {
			TraningSwitch(true);
			AISwitch(false);
			BattleMode(1);
			//ModeChange(SceneNum(SelectScene));	// �g�����N�����Ă���L�����Z����
			MainSystem::Instance().SetNextMode("Select");
		}
		// �l�b�g���[�N
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 4)) {
			MainSystem::Instance().SetNextMode("Network");
		}
		// ���v���C�Đ�
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 5)) {
			//ModeChange(SceneNum(ReplayScene));	// 
			MainSystem::Instance().SetNextMode("Replay");
		}
		// �R���t�B�O
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 6)) {
			MainSystem::Instance().SetNextMode("Config");
		}
		// �I��
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 7)) {
			//ModeChange(4);
			//ModeChange(0);	// �I��
			MainSystem::Instance().EndMainSystem();
		}

		/*
		// F3�Ńl�b�g�ΐ폀��
		if (CheckHitKey(KEY_INPUT_F3) != 0) {
			MainSystem::Instance().SetNextMode("Network");
		}

		// F4�Ńl�b�g�ΐ폀��
		if (CheckHitKey(KEY_INPUT_F4) != 0) {
			MainSystem::Instance().SetNextMode("Config");
		}
		*/

		// �^�C�g���֖߂�
		if (P_BInput(2) == 1) {
			MainSystem::Instance().SetNextMode("Title");
			SEStart(49);
			BGMStart(0);
		}
		// �l�b�g���[�N�Ɍq���ł��Ȃ�I��
		//if (Connect_CheckCn())ModeChange(0);	// �I��


	}//�S�̂��I��

	return 0;
}

// ���j���[�`��
void Menu::Draw()
{

	// �w�i
	DrawGraph(0, 0, back, true);
	
	DrawBox(0, SCREEN_H - 40, SCREEN_W, SCREEN_H - 20, GetColor(0, 0, 0), true);
	int drawX =  80;
	int drawY = SCREEN_H - 40 + 1;
	switch (SPoint){
	case 0:
		DrawString(drawX, drawY, "CPU��|���ď����������[�h�ł�", Cr);
		break;
	case 1:
		DrawString(drawX, drawY, "�v���C���[�Ƒΐ킵�܂�", Cr);
		break;
	case 2:
		DrawString(drawX, drawY, "CPU�Ƒΐ킵�܂�", Cr);
		break;
	case 3:
		DrawString(drawX, drawY, "�l�X�ȏ󋵂ł̓������m�F�ł��܂�", Cr);
		break;
	case 4:
		DrawString(drawX, drawY, "�l�b�g�ΐ�(������)", Cr);
		break;
	case 5:
		DrawString(drawX, drawY, "�L�^�������v���C���Đ����܂�", Cr);
		break;
	case 6:
		DrawString(drawX, drawY, "�e��ݒ���s���܂�", Cr);
		break;
	case 7:
		DrawString(drawX, drawY, "�Q�[�����I�����܂�", Cr);
		break;
	}
	
	// �^�[�{���[�h�\��
	/*
	if (Versus_GetTurboMode()){
		DrawString(2, 2, "Turbo", Cr);
	}
	else
	{
		DrawString(2, 2, "Normal", Cr);
	}
	*/

	{
		// �摜�\��
		/*
		for (int i = 0; i < SentakuNum; i++){
			SetDrawBright(128, 128, 128);	// ��{�͈Â�
			if (i == SPoint)SetDrawBright(250, 250, 255);	// �I��ł�̂𖾂邭
			DrawGraph(25, 33 * i, mode[i], true);
			SetDrawBright(255, 255, 255);
		}
		*/
		// �摜�\��
		int b[3], f[3];
		b[0] = SPoint - 1;
		b[1] = SPoint - 2;
		b[2] = SPoint - 3;
		
		f[0] = SPoint + 1;
		f[1] = SPoint + 2;
		f[2] = SPoint + 3;

		// �ʒu����
		if (b[0] < 0)b[0] = SentakuNum - 1;
		if (f[0] >= SentakuNum)f[0] = 0;

		// 
		if (b[1] < 0){
			b[1] = SentakuNum - 1;
			if (b[0] == SentakuNum - 1)b[1] = SentakuNum - 2;
		}
		if (f[1] >= SentakuNum){
			f[1] = 0;
			if (f[0] == 0)f[1] = 1;
		}

		// 
		if (b[2] < 0) {
			b[2] = SentakuNum - 1;
			if (b[0] == SentakuNum - 1)b[2] = SentakuNum - 2;
		}
		if (f[2] >= SentakuNum) {
			f[2] = 0;
			if (f[0] == 0)f[2] = 1;
		}

		for (int i = 0; i < 3; i++){
			int xpos = -5;
			// �摜�\���ʒu�ݒ�
			xpos += (int)(gameTime * 1.0);
			if (xpos > 25)
				xpos = 25;

			SetDrawBright(128, 128, 128);	// ��{�͈Â�
			//DrawGraph(xpos, 22 * 0 + next, mode[b[2]], true);
			DrawGraph(xpos, 30 * 1 + next + 3, mode[b[1]], true);
			DrawGraph(xpos, 30 * 3 + next, mode[b[0]], true);

			DrawGraph(xpos, 30 * 7 + next, mode[f[0]], true);
			DrawGraph(xpos, 30 * 9 + next, mode[f[1]], true);
			//DrawGraph(xpos, 22 * 11 + next, mode[f[2]], true);

			// �ړ����I�������
			if(next == 0)SetDrawBright(250, 250, 255);	// �I��ł�̂𖾂邭			
			DrawGraph(xpos, 30 * 5 + next, mode[SPoint], true);
			
			SetDrawBright(255, 255, 255);
		}

		// �Ó]
		if (AntenCheck()){
			Anten(0);
		}

		// ���V�[���ڍs�̈Ó]
		if ((kettei == 1) && (TimeStop <= 11)) {
			int num = TimeStop * 25;
			if (num > 255) num = 255;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, num);
			DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

int Menu::End() {
	DeleteGraph(back);
	for (int i = 0; i < 7; i++) {
		DeleteGraph(mode[i]);
	}

	return 0;
}

void Menu::Load_Reload(){
	Anten(255);

	// BGM�ǂݍ���
	BGMStart(11);
	kettei = 0;

	// ����
	gameTime = 0;
	// �Ó]
	anten = 0;

	// ���v���C�I��(�O�̂���)
	Replay_End();
}


void Menu::Load_1second(){

	// �摜
	back = LoadGraph("ob/titleB.png");
	
	int setMode[7];	// ����p
	LoadDivGraph("ob/menu.png", 7, 1, 7, 160, 33, setMode);
	config = LoadGraph("ob/config.png");

	for(int i=0; i<7; i++){
		mode[i] = setMode[i];
	}

	// ���[�h1��2���t�ɂ���
	int hozon = mode[1];
	mode[1] = mode[2];
	mode[2] = hozon;

	/*
	hozon = mode[4];
	mode[4] = mode[5];
	mode[5] = hozon;
	*/
	
	// �摜
	mode[7] = mode[6];
	
	// �摜�ύX
	mode[6] = config;
}

void Menu::Release(void)
{
}