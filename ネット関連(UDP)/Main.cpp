// �i���Q�[���̃v���O����
//
#include <stdio.h>	// �t�@�C������ɕK�v�ȃw�b�_�t�@�C��
#include <string.h>	// �����񑀍�ɕK�v�ȃw�b�_�t�@�C��

#include "pch.h"

#pragma warning(disable : 4996)	// fscanf�̃G���[�\��������

#define BLOG_MAX	(21)	//�L�[�L�^�̍ő吔

/*
* ���[�J���ϐ�
*/

static int GameMode;	// ���݂̃��[�h�A���j���[�ŏ�����
static int LoadMode;			// ���̃��[�h�̏���������������
static int sw;			// �X�N�V��
static int window;			// �E�B���h�E

static boolean screenMode;		// �X�N���[���̃��[�h
static boolean drawFPS;			// FPS�\��
static boolean network = false;

class MainSystem{

private:
	int gameMode;	// ���݂̃��[�h�A���j���[�ŏ�����
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
			if (GameMode == GameMenu)
			{
				Load_Menu();
			}
			else if (GameMode == GameSelect){
				Load_Select();
				Load_State();	// �摜�ǂݍ��ݏ������A
				// �����ɖ߂�x�ɗL��
			}
			else if (GameMode == GameVersus){
				Load_Versus();
			}
			//else if (GameMode == GameConfig){
				//Load_Config();
			//}
			else if (GameMode == GameResult){
				Load_Result();
			}
			else if (GameMode == GameNetwork){
				Load_Network();
			}
			//BGMStart(0);	// BGM���~
			LoadMode = 1;
		}

		//�Q�[�����[�h�̖��O�Ń��[�h��I��
		switch (GameMode)
		{
		case GameMenu:
			Menu();
			break;
		case GameTitle:
			Title();
			break;
		case GameSelect:
			Select();
			break;
		case GameVersus:
			Versus();
			break;
		//case GameConfig:
			//Config();
			//break;
		case GameResult:
			Result();
			break;
		case GameNetwork:
			Network();
			break;
		default:
			GameMode = 0;
			break;
		}
		/*
		if (GameMode == GameMenu)
		{
			Menu();		//���j���[�A�Ԃ��Ă����l�����̃��[�h
		}
		else if (GameMode == GameTitle)
		{
			Title();
		}
		else if (GameMode == GameSelect){
			Select();	//���[�h�ύX �L�����Z���N�g
		}
		else if (GameMode == GameVersus){
			Versus();	//���[�h�ύX �ΐ�
		}
		else if (GameMode == GameConfig){
			Config();	//���[�h�ύX �R���t�B�O
		}
		else if (GameMode == GameResult){
			Result();	//���[�h�ύX �R���t�B�O
		}
		else if (GameMode == GameNetwork){
			Network();	//���[�h�ύX �R���t�B�O
		}
		else {
			//GameMode = 0;	//�I��
		}
		*/

	}
	void MainDraw()
	{
		if (drawFPS){ Draw(); }		// ���݂�FPS��`��
		// FPS�\����ON�̎��̂�
	}
};
static MainSystem sy;

// �t�H���g�ύX
static void SetFont();	

/////////////////////////////////////////////////////////////////////////////��������X�^�[�g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//
	// �t�H���g�ǂݍ���
	//
	//SetFont();
	

	// ini�ł̐ݒ��ǂݍ���
	// ���߂̖��O�A�ϐ����A�Ȃ��ꍇ�̒l�A�ꏊ
	screenMode = GetPrivateProfileInt("Config", "windowmode", 1, "./config.ini");
	drawFPS = GetPrivateProfileInt("Config", "drawfps", 1, "./config.ini");

	//�E�B���h�E���[�h��
	ChangeWindowMode(screenMode);

	// �^�C�g����ύX
	SetMainWindowText("blackright");

	// ��ʃ��[�h�̐ݒ�A�T�C�Y�𑪂�
	SetGraphMode(SCREEN_W, SCREEN_H, 16);

	// ��A�N�e�B�u��Ԃł�����(�������Ȃ��ƔM�тł��Ȃ�)
	SetAlwaysRunFlag(true);

	// ��ʃT�C�Y��ύX�\��
	//SetWindowSizeChangeEnableFlag(true);

	// �c�w���C�u���������������A�G���[���N�����璼���ɏI��
	if (DxLib_Init() == -1){ return -1; }

	// �����keyconfig�ǂݍ���
	P1_BConfig(0, 0);

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
	GameMode = GameTitle;

	// �����g������
	//GameMode = GameSelect;
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

	// �����ɓ��͊Ǘ�
	
	// while( ����ʕ`��, ���b�Z�[�W����(�K�v�炵��), ��ʏ�����, ESC���͊m�F, �Q�[�����[�h��0����Ȃ� )
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClsDrawScreen() == 0 && (CheckHitKey(KEY_INPUT_ESCAPE) == 0) &&
		(GameMode != 0))
	{
		//----------
		// �K�{����
		//----------
		{
			// FPS�X�V
			Update();	// OK

			GetInput();	// �R���g���[���������Ă��邩�m�F�A�ق�Ƃ͈�x�����ł���

			// �X�N�V���̊m�F
			if (CheckHitKey(KEY_INPUT_F1) != 0)sw++;
			else { sw = 0; }
			// �E�B���h�E�̊m�F
			if (CheckHitKey(KEY_INPUT_F4) != 0)window++;
			else { window = 0; }

			// �{�^�����̓`�F�b�N
			P1_BCheck();
			P2_BCheck();
		}

		//
		if (GetASyncLoadNum() == 0){
			sy.MainMove();
		}
		//------
		// �`��
		//------
		{
			//Game_MainDraw();

			if (GetASyncLoadNum() != 0){
				// �񓯊��ǂݍ��݂̐���`��
				DrawFormatString(0, SCREEN_H - 20, GetColor(255, 255, 255), "�c��t�@�C�� %d", GetASyncLoadNum());
			}
		}

		//------------
		// ��ʂ̍X�V
		//------------
		{
			sy.MainDraw();	// FPS����`��
			Wait();		// 1�t���[���o�߂���܂őҋ@
			// �ǂ�������H

			// �l�b�g���[�N
			if (network)DrawString(0, SCREEN_H - 20, "�ڑ���", Cr);
			// �X�N���[���V���b�g
			if (sw == 1){
				ScreenShot();
			}
			/*
			// �E�B���h�E�ύX
			if (window == 1){
				if (screenMode == false)screenMode = true;
				else{ screenMode = false; }
				ChangeWindowMode(screenMode);
			}
			*/

		}

	}
	// ���C�����[�v�I��

	// ����ݒ�ۑ�
	//Key_Save();
	BGMStart(0);
	// �l�b�g���[�N�ؒf
	if (network)End_Network();
	// �g�����ݒ�ۑ�
	Tlist_Save();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 

}//main�I��

void ModeChange(int num)
{
	// ���[�h�ύX

	GameMode = num;
	// ���̃��[�h�̏���
	LoadMode = 0;
}
/////////////////////////////////////////////////////////////////////////////�����܂�

// �Q�[�����[�h�擾
int CheckGameMode()
{
	return GameMode;
}


void SetFont()
{
	
	// ********** �t�H���g�̃��[�h **********
	LPCSTR font_path = "ob/PixelMplus-20130602/PixelMplus10-Regular.ttf"; // �ǂݍ��ރt�H���g�t�@�C���̃p�X
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
	// �t�H���g�Ǎ��G���[����
	MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
	}
	
	// ********** ��������ǂݍ��񂾃t�H���g�𗘗p�ł��� **********
	SetFontSize(20);	// ���₷���̂��߁A�t�H���g�T�C�Y��傫��
	ChangeFont("PixelMplus10", DX_CHARSET_DEFAULT);
	
	// ********** �t�H���g�̃A�����[�h **********
	if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
	}
	else {
	MessageBox(NULL, "remove failure", "", MB_OK);
	}
	
}

// �l�b�g���[�N�擾
void Main_GetNetwork(boolean net)
{
	network = net;
}

// �l�b�g���[�N�̐ڑ��m�F
boolean Main_CheckNetwork()
{
	return network;
}