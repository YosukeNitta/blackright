#pragma once

#include "SData.h"
using namespace modeData;


// public �őO�̃N���X�̂��̂��g����
class Arcade : public ModeData {

public:
	int Arcade::Mode();
	// ���j���[�`��
	void Arcade::Draw();

	int Arcade::Load_Reload();
	void Arcade::Load_1second();
};
