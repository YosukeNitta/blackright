#pragma once

#include "SData.h"

// public �őO�̃N���X�̂��̂��g����
class Versus : public MData {

public:
	int Mode();
	// ���j���[�`��
	void Draw();

	void Load_Reload();
	void Load_1second();
	// ���
	void Release();
};
