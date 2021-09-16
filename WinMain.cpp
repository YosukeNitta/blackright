// �i���Q�[���̃v���O����
//
#include <stdio.h>	// �t�@�C������ɕK�v�ȃw�b�_�t�@�C��
#include <string.h>	// �����񑀍�ɕK�v�ȃw�b�_�t�@�C��

#include "pch.h"
/////////

#include "Fps.h"
#include "WindowInfo.h"
#include "MainSystem.h"

// vector���g��
#include <vector>
using namespace std;

#pragma warning(disable : 4996)	// fscanf�̃G���[�\��������

#pragma region	���[�J���ϐ�
static int window;			// �E�B���h�E

static boolean screenMode;		// �X�N���[���̃��[�h
static int modeCount = 0;	// ���[�h�ɂȂ��Ă���̌o�߃t���[��

static boolean wait;	// �M�ё����҂��āA���[�h�𓮂����Ȃ�
//static int stop = 0;

#pragma endregion

// �e�퍀��
//static Fps fps;

// ���C���V�X�e���N���X
//static MainSystem sy;

//�����֐��錾��--------------------------------------------
void Screen_Setting();

void Game_End();
void File_Loading();
void Replay_DrawData();


//�v���O������----------------------------------------------
#pragma region ���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//��ʐݒ�
	MainSystem::Instance();
	Screen_Setting();

	// �c�w���C�u���������������A�G���[���N�����璼���ɏI��
	if (DxLib_Init() == -1){ return -1; }

	// �����keyconfig�ǂݍ���
	P1_BConfig();

	// �t�@�C�������[�h
	File_Loading();

	// �`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	// �J�n
	MainSystem::Instance().MainLoop();

	// �I������
	Game_End();

	return 0;				// �\�t�g�̏I�� 

}//main�I��
#pragma endregion

/////////////////////////////////////////////////////////////////////////////�����܂�


// ��ʐݒ�
void Screen_Setting()
{
	//
	// �t�H���g�ǂݍ���
	//

	// ini�ł̐ݒ��ǂݍ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
	// ���߂̖��O�A�ϐ����A�Ȃ��ꍇ�̒l�A�ꏊ
	screenMode = GetPrivateProfileInt("Config", "windowmode", 1, "./config.ini");	// ./���K�v(�J�����g�t�H���_)

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

	//SetChangeScreenModeGraphicsSystemResetFlag(FALSE);	// �O���t�B�b�N�n���h���̃��Z�b�g�h�~
														// �E�B���h�E�T�C�Y�ύX���ɏ����Ȃ��悤�ɂ���

	// ��A�N�e�B�u��Ԃł�����(�������Ȃ��ƔM�тł��Ȃ�)
	SetAlwaysRunFlag(true);

	// ��ʃT�C�Y��ύX�\��
	SetWindowSizeChangeEnableFlag(true);

	// ���d�N��
	// �l�b�g���[�N�e�X�g
	//SetDoubleStartValidFlag(TRUE);

	//= �ϐ��ݒ� ========
	//wait = false;	// �ΐ푊���҂�
}

// �I������
void Game_End()
{
	BGMStart(0);
	// �l�b�g���[�N�ؒf

	// ���v���C�I��
	if (Replay_Mode(-1) > 0)Replay_End();
	// �g�����ݒ�ۑ�
	Tlist_Save();
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
}

// �t�@�C���̃��[�h
void File_Loading() {
	// �L�����N�^�[�t�@�C���ǂݍ���

	// �񓯊��ǂݍ��ݐݒ�ɕύXz 
	//SetUseASyncLoadFlag(TRUE);
	// �d���t�@�C����ǂݍ���
	GetDef();
	LoadAllFile();
	// �����ǂݍ��ݐݒ�ɕύX
	//SetUseASyncLoadFlag(FALSE);
}

void Replay_DrawData()
{
	DrawFormatString(SCREEN_W - 100, SCREEN_H - 20, GetColor(10,10,255), "���..%d", Replay_Mode(-1));
}
