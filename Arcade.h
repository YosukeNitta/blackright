#pragma once

#include "SData.h"
using namespace modeData;


// public で前のクラスのものも使える
class Arcade : public ModeData {

public:
	int Arcade::Mode();
	// メニュー描画
	void Arcade::Draw();

	int Arcade::Load_Reload();
	void Arcade::Load_1second();
};
