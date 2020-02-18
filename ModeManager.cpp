#include "pch.h"
//#include <DxLib.h>
#include <memory>
#include "ModeManager.h"

//	�f�X�g���N�^
ModeManager::~ModeManager()
{
	// ���[�h�z������ׂč폜
	for (auto scene : _modes) {
		delete scene.second;
	}
}

//	�C���X�^���X���擾
ModeManager& ModeManager::Instance() {
	static std::unique_ptr<ModeManager> instance(new ModeManager);

	return *instance;
}

void ModeManager::AddMode(std::string modeName, ModeData* scene)
{
	if (_modes.count(modeName))
		return;
	_modes[modeName] = scene;
}

//	�J�n�V�[����ݒ�
void ModeManager::StartMode(std::string modeName) {

	if (!_modes.count(modeName))
		return;

	_currentMode = _modes[modeName];
}

// ���[�h�ύX
void ModeManager::LoadMode(std::string modeName)
{
	if (!_modes.count(modeName))
		return;

	_nextMode = _modes[modeName];	// ���̃��[�h��ݒ�
	_state = ModeState::Release;	// ���[�h�̏�Ԃ������[�X�ɂ���
}

void ModeManager::Update() 
{
	ModeUpdate();
}

//	�V�[���̍X�V����
void ModeManager::ModeUpdate()
{
	if (!_currentMode)
		return;
	//	������
	if (_state == ModeState::Initialize) {
		_currentMode->Initialize();
		_state = ModeState::Update;
	}

	//	�X�V����
	_currentMode->Update();

	//	�������
	if (_state == ModeState::Release) {
		_currentMode->Release();
		NextMode();	// �����Ń��[�h�ؑ�
		_state = ModeState::Initialize;
	}
}

//	���̃V�[���ɐ؂�ւ���
void ModeManager::NextMode()
{
	_currentMode = _nextMode;	// �؂�ւ�
}
