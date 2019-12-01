#pragma once

#include "SData.h"
using namespace modeData;

// public で前のクラスのものも使える
class Menu : public ModeData {

public:
	int Menu::Mode();
	// メニュー描画
	void Menu::Draw();

	int Menu::End();
	int Menu::Load_Reload();
	void Menu::Load_1second();
};
