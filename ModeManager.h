#pragma once
#include "ModeData.h"
#include "SData.h"
#include <unordered_map>

enum ModeState {
	Initialize,	//	������
	Update,	//	�X�V
	Release	//	�Q�[���V�[��
};

//	�V�[���Ǘ��N���X
class ModeManager
{
private:
	ModeManager() : _state(ModeState::Initialize),		// �錾
		_currentMode(nullptr), _nextMode(nullptr) {}
	ModeManager(const ModeManager&) {}
	ModeManager& operator=(const ModeManager&) {}
public:
	//	�f�X�g���N�^
	~ModeManager();

	//	�C���X�^���X���擾
	static ModeManager& Instance();

	//	�V�[����ǉ�����
	void AddMode(std::string modeName, ModeData* scene);

	//	�J�n�V�[����ݒ�
	void StartMode(std::string modeName);

	//	���[�h�ړ�
	void LoadMode(std::string modeName);

	//	�X�V����
	void Update();

private:
	//	�V�[���̍X�V����
	void ModeUpdate();

	//	���̃V�[���̑��ݗL�����m�F
	void NextMode();

private:
	ModeData* _currentMode;	//	���ݎ��s���Ă���V�[��
	ModeData* _nextMode;		//	���Ɏ��s����V�[��
	ModeState _state;			//	�V�[���̏��
	std::unordered_map<std::string, ModeData*> _modes;	// �V�[���ƃL�[�������ɂ܂Ƃ߂�
};
