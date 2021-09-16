#include "pch.h"
#include "MainSystem.h"
#include <memory>

static int sw;			// �X�N�V��
static NetData* netData;
static boolean netOn = false;	// �l�b�g���[�N�̊J�n
static BYTE keyLog[KEYLOG_MAX];	// �L�[�̎擾
static int GHandle;	// ��~�O�̉摜
static int wait_StopTime;	// �E�F�C�g1�ɂȂ��Ă��鎞��
//static void Game_Processing();
static void Game_Update();


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
	// �J�n�O
	_socket = new Socket();
	// ��ʂ̃T�C�Y�Ɠ����T�C�Y�̃O���t�B�b�N���쐬
	GHandle = MakeGraph(SCREEN_W, SCREEN_H);

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

			// �Ă��Ɨp
			if (netOn) {
				_socket->draw();	// ���\��
				// ����̏����ŏI��
				// ��莞�Ԑڑ����Ȃ����A���[�h�Z���N�g�Ńl�b�g���[�N��؂�
				if (_socket->isEndConnect()) {
					delete _socket;
					netOn = false;
					Key_SetNetOn(false);
					MainSystem::Instance().EndMainSystem();	// �I��
				}
			}

			_fps.Wait();		// 1�t���[���o�߂���܂őҋ@
		}
		ScreenFlip();	// �\��
		
		// ���[�v�I��
		if (CheckEnd()) {
			// �N�����Ă���Ȃ�l�b�g���[�N���I������
			if (netOn) {
				delete _socket;
				netOn = false;
				Key_SetNetOn(false);
				MainSystem::Instance().EndMainSystem();	// �I��
			}
			break;
		}
	}
	// ���[�v�I�����A�M�т�؂��Ă���
	if (netOn) {
		// ����̏����ŏI��
		// ��莞�Ԑڑ����Ȃ����A���[�h�Z���N�g�Ńl�b�g���[�N��؂�
		if (_socket->isEndConnect()) {
			delete _socket;
			netOn = false;
			Key_SetNetOn(false);
		}
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

	{
		// �l�b�g�ΐ킪�I���ŃL�[���͂̑ҋ@��ԂȂ�҂�
		// ��ʕ\����ύX����
		if ((netOn) && (!_socket->getIsKeyUpdate())) {
			
			// ��荞�񂾃Q�[����ʕ\��
			DrawGraph(0, 0, GHandle, false);

			// �ʏ�E�F�C�g���[�h�Ȃ�
			// �J��(2)�͍s��Ȃ�
			if (_socket->getWaitMode() == 1) {
				//==========================
				// �Â�����
				//==========================
				// �����Â�����
				SetAlpha(188);
				// �����Ȃ����
				DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
				NoBlend();
			}
		}
		else {
			//==========================
			// �Â�����
			//==========================

			// �i�s����
			_currentMode->All();

			// ����
			// �Đi�s����Ȃ�
			/*
			if (_socket->getMoreKey() == 1) {
				_socket->keyUpdate();
				_socket->searchKey();	// ����ōĐi�s�𔻒f(���[�v�I��)

				_currentMode->All();

				// �s���N�F�̐���\������
				SetAlpha(128);
				// �����Ȃ����
				DrawBox(0, SCREEN_H / 2 - 15, SCREEN_W, SCREEN_H / 2 + 15, GetColor(255, 0, 255), true);
				NoBlend();
			}
			*/
			// �����Y

			// �Ō�ɉ�ʃf�[�^�̎�肱��
			GetDrawScreenGraph(0, 0, SCREEN_W, SCREEN_H, GHandle);
		}
	}

	//	�������
	if (_currentState == 2) {
		_currentMode->Release();
		_currentMode->Release();	// �������
		_currentMode = _nextMode;	// ���̃��[�h�Z�b�g
		_currentState = 0;
		// �ԍ��Z�b�g
		if (_currentMode == _modes["Versus"]) {
			_modeNum = SceneNum(VersusScene);
		}
		// �ԍ��Z�b�g
		else if (_currentMode == _modes["Select"]) {
			_modeNum = SceneNum(SelectScene);
		}
		// �ԍ��Z�b�g
		else if (_currentMode == _modes["Result"]) {
			_modeNum = SceneNum(ResultScene);
		}
		// �ԍ��Z�b�g
		else if (_currentMode == _modes["Menu"]) {
			_modeNum = SceneNum(MenuScene);
		}
		// �V�[���Ȃ�
		else {
			_modeNum = SceneNum(NoScene);
		}
	}
}

// ���́EFPS�X�V����
#pragma region
void MainSystem::Game_Processing()
{
	if (_socket->getWaitMode() == 1) {
		set_stopVB(true);
	}
	else {
		set_stopVB(false);
	}
	// ����
	GetInput();	// �R���g���[���������Ă��邩�m�F
	// �{�^�����̓`�F�b�N�A�X���[���͂Ȃ�
	// 0�`-n�͑��삷��
	Key_Setup();

	// �ʏ�E�F�C�g��1�t���ȏォ����Ȃ瑀���߂�(�҂��Ă�ԂɎ����̓��͂����i��ł��܂�)
	// 1�t�������Ȃ��Ƒ���-1����Ă��܂�
	//if (_socket->getWaitMode() == 1)Key_Return1Frame();

	// �q�����Ă�Ȃ�
	// !net!
	if (netOn) {
		
		// �������狭���I���̉\������
		// netData���Z�b�g����
		if (!_socket->getLoad_f()) 
		{
			_socket->setNetData(netData);
			_socket->setLoad_f(true);	// ���[�h����
		}
		
		// �Q�[�����[�h�擾
		_socket->setMyGameMode(_modeNum);
		// �l�b�g�p�̃L�[�ݒ�J�n
		// �����Ŏ����̃L�[���͂��擾(keyLog�ɃZ�b�g)
		Key_NetButtonUpdate();
		// myKey�𑗐M
		_socket->setKeyLog(keyLog);
		// socket�ōX�V�������s���A2pKey���擾
		_socket->update();

		// �L�[�Ƀ\�P�b�g�Ŏ擾����Key��n��
		// ������Key�ɓ��͂�ۑ�
		Key_SetGKeyLog(_socket->p2KeyLog);	// 2P�̃L�[����
		Key_SetSendKey(_socket->myKeyLog);	// �����̃L�[����
		// 1P�A2P�̃L�[������
		// ���͂𔽉f����
		Key_NetButtonEnter();
		
		/*
		// �E�F�C�g���[�h��1�ɂȂ��Ă鎞�Ԃ��v��
		if (_socket->getWaitMode() == 1){
			wait_StopTime++;
		}	
		else {
			wait_StopTime = 0;
		}
		*/
		// �ʏ�E�F�C�g��1�t���ȏォ����Ȃ瑀���߂�(�҂��Ă�ԂɎ����̓��͂����i��ł��܂�)
		// 1�t�������Ȃ��Ƒ���-1����Ă��܂�
		
	}
	
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
	if (netOn) {
		//DrawString(0, SCREEN_H - 20, "�ڑ���", Cr);
		// �l�b�g�f�[�^�\��
		//netData->DataDraw(120, 100);
		
	}
	// �X�N���[���V���b�g
	if (sw == 1) {
		ScreenShot();
	}
}

//////////////////////////////////////////////////////////////////////////////
// �擾����
void Main_GetNetData(NetData* net) {
	netData = net;
}

// 0��
void ChangeNetOn(int n) {
	if (n == 1) netOn = true;
	else { netOn = false; }
}

boolean GetNetOn() {
	return netOn;
}

// key����L�[���O���擾�Asocket�Ŏg��
void Main_SetKeyLog(BYTE key[KEYLOG_MAX]) {
	for (int i = 0; i < KEYLOG_MAX; i++) {
		keyLog[i] = key[i];
	}
}