#pragma once

#include "SData.h"

// public �őO�̃N���X�̂��̂��g����
class Config : public MData {

public:
	int Config::Mode();
	// ���j���[�`��
	void Config::Draw();

	void Config::Load_Reload();
	void Config::Load_1second();
	// ���
	void Release();
};
