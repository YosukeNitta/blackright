#pragma once

#include "SData.h"
using namespace modeData;

// public �őO�̃N���X�̂��̂��g����
class Replay : public ModeData {

public:
	int Replay::Mode();
	// ���j���[�`��
	void Replay::Draw();

	int Replay::Load_Reload();
	void Replay::Load_1second();
};
