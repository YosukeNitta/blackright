#include "pch.h"
#include "MainSystem.h"
#include <memory>

//	�N���X�̃C���X�^���X���擾
MainSystem& MainSystem::Instance()
{
	// memory���g���ĉ������s���Ă���H
	static std::unique_ptr<MainSystem> instance(new MainSystem);
	// �C���X�^���X�H��Ԃ�
	return *instance;
}

// ���C�����\�b�h(����Ɋe�탂�[�h)
void MainSystem::MainMove()
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