#pragma once

#include "SData.h"
using namespace modeData;

// public �őO�̃N���X�̂��̂��g����
class Menu : public ModeData {

public:
	int Menu::Mode();
	// ���j���[�`��
	void Menu::Draw();

	int Menu::End();
	int Menu::Load_Reload();
	void Menu::Load_1second();
};
