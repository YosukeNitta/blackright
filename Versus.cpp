//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include "Versus.h"
#include "MainSystem.h"
static boolean Load_1;
static int Num;
static int ModePause, PauseNum;
static int GHandle;
static int bgmNum = 1;

static int gameTime;	// 1�`60�Ń��[�v
static boolean turboMode;

/**************
* �ΐ���
**************/

int Versus::Mode() 
{
	if (!Load_1) {
		/*
		* �V�X�e������
		*/
		// �̗́A�ʒu�����������Ă���
		CharLoad();
		// �|�[�Y���I�t
		ModePause = 0;
		// BGM��炷
		//BGMStart(1);

		//Load_1 = true;
	}

	// �����J�n
	{
		//--------------
		// ���ԃJ�E���g
		//--------------
		{
			gameTime++;
			if (gameTime > 60)
				gameTime = 1;
		}

		/*********
		* ���Z����
		*********/
		{
			if (ModePause == 0) {
				StopCmd(false);	// �R�}���h�󂯕t��
				Num = CharMove();	// ���W�Ƃ����v�Z����A���ꂪ Versus �̃��C��
				// �^�[�{���[�h
				if (turboMode) {
					if (gameTime == 12 || gameTime == 24 || gameTime == 36 ||
						gameTime == 48 || gameTime == 60) { //|| gameTime == 60){
						//StopCmd(true);	// �R�}���h���󂯕t���Ȃ�
						P1_BCheck();
						P2_BCheck();
						CharMove();
					}
				}

				// 1���Ԃ��Ă�����I��
				if (Num == 1) {
					CharLoad();
					MainSystem::Instance().SetNextMode("Menu");	// ���j���[��
				}
			}// ModePause 
		}

		//---------
		// BGM�Đ������[�h����
		//---------
		if (!Load_1) {
			// BGM��炷
			BGMStart(bgmNum);
			Load_1 = true;
		}

		/*********
		* �`�揈��
		*********/
		if (Load_1 == 1)	// ���[�h�������Ă���
		{
			// �v�Z�Ɏg���l�� State �œn���Ă�����

			// �퓬��
			if (!ModePause)
			{
				// �L�����A�w�i�`��A���C�t�o�[
				MainDraw();
				// ���̑�(�G�t�F�N�g�A������)
				ObjectDraw();
			}

			if (ModePause)
			{
				// �|�[�Y�O�̉摜��\��
				DrawGraph(0, 0, GHandle, false);
				// �|�[�Y���
				// �Ԃ��Ă����l�Ń��[�h�ύX
				PauseNum = Pause();
			}
			if (PauseNum != 0) {
				switch (PauseNum) {
				case 1:
					ModePause = 0, PauseNum = 0;
					MainSystem::Instance().SetNextMode("Versus");
					// �X�e�[�^�X�߂�
					break;
				case 2:
					ModePause = 0, PauseNum = 0;
					MainSystem::Instance().SetNextMode("Select");
					break;
				case 3:
					ModePause = 0, PauseNum = 0;
					MainSystem::Instance().SetNextMode("Menu");
					break;
				case 4:
					ModePause = 0, PauseNum = 0;
					// ���s
					break;
				case SceneNum(ReplayScene):	// ���v���C
					ModePause = 0, PauseNum = 0;
					MainSystem::Instance().SetNextMode("Replay");
					// ���s
					break;
				}
			}
		}	// Load_1
		else {
			DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);	// �^����
		}
	}// ���C�����[�v�I��


	//�����I��
	return 0;
}

void Versus::Draw()
{
	
}

void Versus::Load_Reload() 
{
	Load_1 = false;
}


void Versus::Load_1second() 
{
}

void Versus::Release(void)
{
}

void PauseSetting(int n1)
{
	ModePause = n1;

	// �L�����A�w�i�`��A���C�t�o�[
	MainDraw();
	// ���̑�(�G�t�F�N�g�A������)
	ObjectDraw();

	// ��ʂ̃T�C�Y�Ɠ����T�C�Y�̃O���t�B�b�N���쐬
	GHandle = MakeGraph(SCREEN_W, SCREEN_H);

	// ��ʃf�[�^�̎�肱��
	GetDrawScreenGraph(0, 0, SCREEN_W, SCREEN_H, GHandle);
}

void Load_Versus()
{
	Load_1 = false;
}

void Versus_bgmNum(int num)
{
	bgmNum = num;
}

boolean Versus_GetTurboMode(){
	return turboMode;
}

void Versus_TurboMode()
{
	if (!turboMode)
		turboMode = true;
	else{
		turboMode = false;
	}
}