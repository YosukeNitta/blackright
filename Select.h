#pragma once

#include "SData.h"


// public �őO�̃N���X�̂��̂��g����
class Select : public MData {

public:
	int Select::Mode();
	// ���j���[�`��
	void Select::Draw();

	void Select::Load_Reload();
	void Select::Load_1second();
	// ���
	void Release();
};

