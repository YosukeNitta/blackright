#include "pch.h"
#include "MainSystem.h"
#include <memory>

static int sw;			// �X�N�V��

void Game_Processing();
void Game_Update();


//	�N���X�̃C���X�^���X���擾
MainSystem& MainSystem::Instance()
{
	// memory���g���ĉ������s���Ă���H
	static std::unique_ptr<MainSystem> instance(new MainSystem);
	// �C���X�^���X�H��Ԃ�
	return *instance;
}

// ���C�����\�b�h(����Ɋe�탂�[�h)
void MainSystem::MainLoop()
{
	///-------------
	//
	// ���C�����[�v
	//
	//--------------
	// while( ���b�Z�[�W����(�K�v�炵��), ESC���͊m�F, �Q�[�����[�h��0����Ȃ� )
	while (ProcessMessage() == 0 && (CheckHitKey(KEY_INPUT_ESCAPE) == 0))
	{
		//----------
		// �K�{����
		//----------
		{
			// �Q�[���i�s���m�[�}���Ȃ��ʃN���A
			if (ClsDrawScreen() != 0)break;	// ��ʏ�����

			// ����
			Game_Processing();

			// ���[�h������		
			// ���[�h�I�����Ă���Ȃ���s
			if (GetASyncLoadNum() == 0) {
				// ���s
				Update();
			}
			// �ǂݍ��ݐ���\��
			else {
				int amari = GetASyncLoadNum() % 5;	// 5���X�V
				// �񓯊��ǂݍ��݂̐���`��
				DrawFormatString(0, SCREEN_H - 20, Cr, "�c��t�@�C�� %d", GetASyncLoadNum() - amari);
			}
		}
		//------
		// �`��
		//------
		{
			//update
			//draw
			//filp
			//wait
			// ���̏��Ԃ�fps�g��

			// FPS�X�V
			_fps.Update();	// OK
			if (_drawFPS) { _fps.Draw(SCREEN_W - 80, 0); }
			
			//Replay_DrawData();	// �f�o�b�O�p
		}
		//------------
		// ��ʂ̍X�V
		//------------
		{
			Game_Update();
			_fps.Wait();		// 1�t���[���o�߂���܂őҋ@
		}
		ScreenFlip();	// �\��
		
		// ���[�v�I��
		if (CheckEnd())break;
	}
}

void MainSystem::Update()
{
	// �Ȃ��Ȃ�~�߂�
	if (!_currentMode)
		return;

	// ���[�h�ǂݍ���
	if (_currentState == 0) {
		_currentMode->Load_First();	// �ǂݍ���
		_currentState = 1;	// ���s���[�h��
	}

	// ���[�h�i�s
	_currentMode->All();

	//	�������
	if (_currentState == 2) {
		_currentMode->Release();
		_currentMode->Release();	// �������
		_currentMode = _nextMode;	// ���̃��[�h�Z�b�g
		_currentState = 0;
	}
}

// ���́EFPS�X�V����
#pragma region
void Game_Processing()
{

	// ����
	GetInput();	// �R���g���[���������Ă��邩�m�F

	// �q�����Ă�Ȃ�
	if (Connect_CheckCn()) {
		Delay_Check();
		Connect_SetGameMode();
	}

	// �{�^�����̓`�F�b�N�A�X���[���͂Ȃ�
	// 0�`-n�͑��삷��
	P1_BCheck();
	P2_BCheck();

	// �X�N�V���̊m�F
	if (CheckHitKey(KEY_INPUT_F1) != 0)sw++;
	else { sw = 0; }

	//Release���[�h���̂ݕ\���������Ƃ��Ȃ񂩂� #ifndef _DEBUG ��
	//#ifdef _DEBUG
	// ���̊m�F
	if (CheckHitKey(KEY_INPUT_F2) != 0)StopSound(1);
	else { StopSound(0); }
	//#endif

	// ���[�h������
	// ���[�h�I�����Ă���Ȃ���s
	//if (GetASyncLoadNum() == 0){
	//	MainSystem::Instance().MainMove();
	//}
}
#pragma endregion

/// <summary>
/// ���̍X�V
/// </summary>
void Game_Update()
{
	// �l�b�g���[�N�ɐڑ���
	if (Connect_CheckCn()) {
		DrawString(0, SCREEN_H - 20, "�ڑ���", Cr);
		Delay_Draw();	// �f�B���C�\��
		DrawFormatString(0, SCREEN_H - 60, Cr, "side..%d", Connect_CheckSide());
		DrawFormatString(0, SCREEN_H - 40, Cr, "port..%d", Connect_CheckPort());
	}
	// �X�N���[���V���b�g
	if (sw == 1) {
		ScreenShot();
	}
}