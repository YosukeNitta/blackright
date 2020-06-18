#pragma once
#include <unordered_map>
#include "SData.h"
#include "Fps.h"

#include "Title.h"
#include "Menu.h"
#include "Select.h"
#include "Replay.h"
#include "Arcade.h"
#include "Versus.h"
#include "Result.h"
#include "Network.h"

class MainSystem {

private:
	std::unordered_map<std::string, MData*> _modes;	// �V�[���ƃL�[�������ɂ܂Ƃ߂�
	MData* _currentMode;	//	���ݎ��s���Ă���V�[��
	MData* _nextMode;	//	���ݎ��s���Ă���V�[��
	int _currentState = 0;	// 0�ŊJ�n 1��Update 2�Ń����[�X
	bool _endMainSystem = false;
	Fps _fps;
	boolean _drawFPS;			// FPS�\��

	// �V���O���g���N���X�Ȃ̂ŃR���X�g���N�^������B��
	// �R���X�g���N�^
	MainSystem() {
		_modes["Title"] = new Title;
		_modes["Menu"] = new Menu;
		_modes["Select"] = new Select;
		_modes["Arcade"] = new Arcade;
		_modes["Versus"] = new Versus;
		_modes["Result"] = new Result;
		_modes["Replay"] = new Replay;
		_modes["Network"] = new Network;
		_currentMode = _modes["Title"];	// �������[�h����
		_nextMode = _currentMode;
		//_currentMode->Load_First();	// �ǂݍ���

		// �ݒ�
		_drawFPS = GetPrivateProfileInt("Config", "drawfps", 1, "./config.ini");
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
};