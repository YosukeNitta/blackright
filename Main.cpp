// �i���Q�[���̃v���O����
//
#include <stdio.h>	// �t�@�C������ɕK�v�ȃw�b�_�t�@�C��
#include <string.h>	// �����񑀍�ɕK�v�ȃw�b�_�t�@�C��

#include "pch.h"
// pch //
/*
#include "DxLib.h"
#include "Char.h"
#include "Graph.h"
#include "Math.h"
#include "Mode.h"
#include "Sound.h"

#include "System.h"
#include "Define.h"
*/
/////////

#include "Menu.h"
#include "Select.h"
#include "Replay.h"
#include "Arcade.h"
#include "Fps.h"
#include "WindowInfo.h"

// vector���g��
#include <vector>
using namespace std;

#pragma warning(disable : 4996)	// fscanf�̃G���[�\��������

#pragma region	���[�J���ϐ�
static int GameMode;	// ���݂̃��[�h�A���j���[�ŏ�����
static int LoadMode;			// ���̃��[�h�̏���������������
static int sw;			// �X�N�V��
static int sw2;
static int sw2_2;
static int window;			// �E�B���h�E

static boolean screenMode;		// �X�N���[���̃��[�h
static boolean drawFPS;			// FPS�\��
static int modeCount = 0;	// ���[�h�ɂȂ��Ă���̌o�߃t���[��

static int sd = 0;	// �Q�[���i�s���x
static boolean wait;	// �M�ё����҂��āA���[�h�𓮂����Ȃ�
//static int stop = 0;
#pragma endregion

// �e�퍀��
static Menu men;
static Select sel;
static Replay rep;
static Arcade arc;
static Fps fps;

class MainSystem{

private:
	int gameMode;	// ���[�h�ԍ��A���j���[�ŏ�����
	//ModeData mode;	// ���ۂ̃Q�[�����[�h
public:
	// �R���X�g���N�^
	MainSystem::MainSystem()
	{
		gameMode = 0;
	}

	// ���C�����\�b�h(����Ɋe�탂�[�h)
	void MainMove()
	{
		// �ύX���ꂽ�ꍇ�A���[�h������
		if (LoadMode == 0){
			if (GameMode == GameScene(MenuScene))
			{
				///Load_Menu();
				men.Load_First();
			}
			else if (GameMode == GameScene(SelectScene)){
				sel.Load_First();
				Load_State();	// �摜�ǂݍ��ݏ������A
				// �����ɖ߂�x�ɗL��
			}
			else if (GameMode == GameScene(VersusScene)){
				Load_Versus();
			}
			//else if (GameMode == GameConfig){
			//Load_Config();
			//}
			else if (GameMode == GameScene(ResultScene)){
				Load_Result();
			}
			else if (GameMode == GameScene(NetworkScene)){
				Load_Network();
			}
			else if (GameMode == GameScene(ReplayScene)){
				rep.Load_First();
			}
			else if (GameMode == GameScene(ArcadeScene)){
				arc.Load_First();
			}
			//BGMStart(0);	// BGM���~
			LoadMode = 1;
			modeCount = 0;	// �I�����C���p
		}

		// namespace modeData

		

		//�Q�[�����[�h�̖��O�Ń��[�h��I��
		switch (GameMode)
		{
		case GameScene(MenuScene):
			//Menu();
			men.All();
			break;
		case GameScene(TitleScene):
			Title();
			break;
		case GameScene(SelectScene):
			sel.All();
			break;
		case GameScene(VersusScene):
			// ���݂�versus�ɂȂ�����J�n
			if ((Connect_CheckCn()) && (Connect_CheckGetData(0, 9) == GameScene(SelectScene))
				&& (modeCount < 2)){
				break;
			}
			else{ modeCount = 2; }
			Versus();
			break;
			//case GameConfig:
			//Config();
			//break;
		case GameScene(ResultScene):
			Result();
			break;
		case GameScene(NetworkScene):
			Network();
			break;
		case GameScene(ReplayScene):
			rep.All();
			break;
		case GameScene(ArcadeScene):
			arc.All();
			break;
		default:
			GameMode = 0;
			break;
		}
	}

	void MainDraw()
	{
			// ���݂�FPS��`��
		// FPS�\����ON�̎��̂�
	}
};
static MainSystem sy;

//�����֐��錾��--------------------------------------------
void Screen_Setting();
void Game_Processing();
void Game_Draw();
void Game_Update();
void Game_End();
void Replay_DrawData();

//�v���O������----------------------------------------------
#pragma region ���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//��ʐݒ�
	sy:MainSystem();
	Screen_Setting();

	// �c�w���C�u���������������A�G���[���N�����璼���ɏI��
	if (DxLib_Init() == -1){ return -1; }

	// �����keyconfig�ǂݍ���
	P1_BConfig();

	{
		// �񓯊��ǂݍ��ݐݒ�ɕύX
		//SetUseASyncLoadFlag(TRUE);

		// �d���t�@�C����ǂݍ���
		GetDef();
		LoadAllFile();
		
		// �����ǂݍ��ݐݒ�ɕύX
		//SetUseASyncLoadFlag(FALSE);
	}

	// �N����ŏ��̃��[�h
	GameMode = GameScene(TitleScene);
	
	// �����g������
	//GameMode = GameScene(SelectScene);
	//TraningSwitch(true);
	//BattleMode(false);
	// ctrl + K + U or C

	// �`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	///-------------
	//
	// ���C�����[�v
	//
	//--------------
	// while( ����ʕ`��, ���b�Z�[�W����(�K�v�炵��), ��ʏ�����, ESC���͊m�F, �Q�[�����[�h��0����Ȃ� )
	while (ProcessMessage() == 0 && ScreenFlip() == 0 && (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
		&& (GameMode != 0))
	{
		//----------
		// �K�{����
		//----------
		{
			// �Q�[���i�s���m�[�}���Ȃ��ʃN���A
			if (sd == 0){
				if (ClsDrawScreen() != 0)break;
			}
			// FPS�X�V
			fps.Update();	// OK
			// ����
			Game_Processing();
		}
		//------
		// �`��
		//------
		{
			//update
			//draw
			//filp
			//wait
			
			Game_Draw();
			//Replay_DrawData();
			fps.Wait();		// 1�t���[���o�߂���܂őҋ@
		}
		//------------
		// ��ʂ̍X�V
		//------------
		{
			Game_Update();
			
		}
	}
	// ���C�����[�v�I��

	// �I������
	Game_End();

	return 0;				// �\�t�g�̏I�� 

}//main�I��
#pragma endregion

void ModeChange(int num)
{
	// ���[�h�ύX
	GameMode = num;
	//WaitSwitch
	// ���̃��[�h�̏���
	LoadMode = 0;
}
/////////////////////////////////////////////////////////////////////////////�����܂�

// �Q�[�����[�h�擾
int CheckGameMode()
{
	return GameMode;
}

// ��ʐݒ�
void Screen_Setting()
{
	//
	// �t�H���g�ǂݍ���
	//

	// ini�ł̐ݒ��ǂݍ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
	// ���߂̖��O�A�ϐ����A�Ȃ��ꍇ�̒l�A�ꏊ
	screenMode = GetPrivateProfileInt("Config", "windowmode", 1, "./config.ini");	// ./���K�v(�J�����g�t�H���_)
	drawFPS = GetPrivateProfileInt("Config", "drawfps", 1, "./config.ini");

	// �^�C�g����ύX
	SetMainWindowText("blackright");

	// �t���X�N���[�����[�h���̉𑜓x���[�h��4:3���g�p���郂�[�h�ɐݒ�
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);

	// �t���X�N���[�����[�h���̉�ʂ̊g�僂�[�h���ŋߓ_���[�h�ɐݒ�
	SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);

	//�E�B���h�E���[�h��
	ChangeWindowMode(screenMode);

	// ��ʃ��[�h�̐ݒ�A�T�C�Y�𑪂�
	SetGraphMode(WindowInfo::width, WindowInfo::height, 16);

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);	// �O���t�B�b�N�n���h���̃��Z�b�g�h�~
														// �E�B���h�E�T�C�Y�ύX���ɏ����Ȃ��悤�ɂ���

	// ��A�N�e�B�u��Ԃł�����(�������Ȃ��ƔM�тł��Ȃ�)
	SetAlwaysRunFlag(true);

	// ��ʃT�C�Y��ύX�\��
	SetWindowSizeChangeEnableFlag(true);

	//= �ϐ��ݒ� ========
	//wait = false;	// �ΐ푊���҂�
}

// ���́EFPS�X�V����
#pragma region
void Game_Processing()
{
	
	// ����
	GetInput();	// �R���g���[���������Ă��邩�m�F

	// �q�����Ă�Ȃ�
	if (Connect_CheckCn()){
		Delay_Check();
		Connect_SetGameMode();
		// Wait������Ȃ�~�߂�
		if (Connect_WaitGameMode())sd = 1;
		else{ sd = 0; }
	}

	

	// �{�^�����̓`�F�b�N�A�X���[���͂Ȃ�
	// 0�`-n�͑��삷��
	if (sd <= 0){
		P1_BCheck();
		P2_BCheck();
	}

	// �X�N�V���̊m�F
	if (CheckHitKey(KEY_INPUT_F1) != 0)sw++;
	else { sw = 0; }

	//Release���[�h���̂ݕ\���������Ƃ��Ȃ񂩂� #ifndef _DEBUG ��
	//#ifdef _DEBUG
	// ���̊m�F
	if (CheckHitKey(KEY_INPUT_F2) != 0)StopSound(1);
	else{ StopSound(0); }
	//#endif

	// ���[�h������
	if (GetASyncLoadNum() == 0){
		// ��{
		if (sd == 0){
			sy.MainMove();
			//sd = 1;
		}
		// ��~
		else if (sd == 1){
			sd = 0;
		}
		// �čs��(2�{��)
		else if(sd == -1){
			sy.MainMove();
			P1_BCheck();
			P2_BCheck();
			sy.MainMove();
			sd = 0;
		}
	}

	/*
	if (stop == 0)stop = 1;
	else if (stop == 1) {
		stop = 0;
		sd = 1;
	}
	*/
}
#pragma endregion

void Game_Draw()
{
	// �ǂݍ��ݐ���\��
	if (GetASyncLoadNum() != 0){
		int amari = GetASyncLoadNum() % 5;	// 5���X�V
		// �񓯊��ǂݍ��݂̐���`��
		DrawFormatString(0, SCREEN_H - 20, Cr, "�c��t�@�C�� %d", GetASyncLoadNum() - amari);
	}

	// �O�̉摜��\��
	//if (sd == 1)SaveScreen();
	//else{ Draw_SaveScreen(); }
	// �ҋ@�ł��Ă��邩(�l�b�g�ΐ�p)
	if (sd == 1)DrawString(SCREEN_W -100, 20, "���[�h�ҋ@���c",Cr);

	//sy.MainDraw();	// FPS����`��
	if (drawFPS) { fps.Draw(SCREEN_W - 80, 0); }
}

/// <summary>
/// ���̍X�V
/// </summary>
void Game_Update()
{
	// �l�b�g���[�N�ɐڑ���
	if (Connect_CheckCn()){
		DrawString(0, SCREEN_H - 20, "�ڑ���", Cr);
		Delay_Draw();	// �f�B���C�\��
		DrawFormatString(0, SCREEN_H - 60, Cr, "side..%d", Connect_CheckSide());
		DrawFormatString(0, SCREEN_H - 40, Cr, "port..%d", Connect_CheckPort());
	}
	// �X�N���[���V���b�g
	if (sw == 1){
		ScreenShot();
	}

	//�E�B���h�E���[�h��
	if (CheckHitKey(KEY_INPUT_F4) != 0)sw2++;
	else { sw2 = 0; }
	if (CheckHitKey(KEY_INPUT_F4) != 0) {
		// �ŏ��̂�
		if (sw2 == 1) {
			if (sw2_2 == 0) {
				ChangeWindowMode(0);
				sw2_2 = 1;
			}
			else if (sw2_2 == 1) {
				ChangeWindowMode(1);
				sw2_2 = 0;
			}
		}
	}
}

// �I������
void Game_End()
{
	BGMStart(0);
	// �l�b�g���[�N�ؒf
	if (Connect_CheckCn())End_Network();
	// ���v���C�I��
	if (Replay_Mode(-1) > 0)Replay_End();
	// �g�����ݒ�ۑ�
	Tlist_Save();
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
}

void Replay_DrawData()
{
	DrawFormatString(SCREEN_W - 100, SCREEN_H - 20, GetColor(10,10,255), "���..%d", Replay_Mode(-1));
}