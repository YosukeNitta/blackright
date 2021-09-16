#pragma once
#include <unordered_map>
#include "SData.h"
#include "Fps.h"

#include "Title.h"
#include "Menu.h"
#include "Select.h"
#include "Config.h"
#include "Replay.h"
#include "Arcade.h"
#include "Versus.h"
#include "Result.h"
#include "Network.h"

#include "Socket.h"

/*

����̒��Ń��[�h�@�\�����s����H

*/
class MainSystem {
protected:
	
public:
	std::unordered_map<std::string, MData*> _modes;	// �V�[���ƃL�[�������ɂ܂Ƃ߂�
	MData* _currentMode;	//	���ݎ��s���Ă���V�[��
	MData* _nextMode;	//	���Ɏ��s����V�[��
	int _currentState = 0;	// 0�ŊJ�n 1��Update 2�Ń����[�X
	bool _endMainSystem = false;
	Fps _fps;
	boolean _drawFPS;			// FPS�\��
	Socket* _socket;		// �ʐM�p�N���X
	int _modeNum;	// currentMode�̔ԍ�

	// �V���O���g���N���X�Ȃ̂ŃR���X�g���N�^������B��
	// �R���X�g���N�^
	MainSystem() {
		_modes["Title"] = new Title;
		_modes["Menu"] = new Menu;
		_modes["Select"] = new Select;
		_modes["Config"] = new Config;
		_modes["Arcade"] = new Arcade;
		_modes["Versus"] = new Versus;
		_modes["Result"] = new Result;
		_modes["Replay"] = new Replay;
		_modes["Network"] = new Network;
		_currentMode = _modes["Title"];	// �������[�h����
		_nextMode = _currentMode;	// �������[�h��ݒ肵�Ă���
		//_currentMode->Load_First();	// �ǂݍ���

		// �ݒ�
		_drawFPS = GetPrivateProfileInt("Config", "drawfps", 1, "./config.ini");
		//_socket();
		_modeNum = SceneNum(NoScene);
	}
	//MainSystem(const MainSystem&) {}
	//MainSystem& operator=(const MainSystem&) {}
public:
	

	//	�C���X�^���X���擾
	static MainSystem& Instance();

	// ���C�����\�b�h(����Ɋe�탂�[�h)
	void MainLoop();

	void Update();

	void MainDraw()
	{

	}

	void SetNextMode(std::string next)
	{
		// �T���ĂȂ��Ȃ�I��
		if (!_modes.count(next))
			return;

		_nextMode = _modes[next];
		_currentState = 2;
	}

	// ������I������
	void EndMainSystem()
	{
		_endMainSystem = true;
	}

	// ������I������
	bool CheckEnd()
	{
		return _endMainSystem;
	}

	void Game_Processing();

	//======================
	// �l�b�g���[�N�p
	//======================
	// �|�C���^�n��
	Socket GetSocket() {
		return *_socket;
	}

	// �|�C���^�擾
	void SetSocket(Socket* soc) {
		_socket = soc;
	}
};

// �l�b�g���[�N�̃I���I�t
boolean GetNetOn();