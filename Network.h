#pragma once

#include "SData.h"

// public �őO�̃N���X�̂��̂��g����
class Network : public MData {

public:
	int Mode();
	// ���j���[�`��
	void Draw();

	int End();
	void Load_Reload();
	void Load_1second();
	// ���
	void Release();
};
