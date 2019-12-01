#pragma once

#include "SData.h"
using namespace modeData;

// public で前のクラスのものも使える
class Replay : public ModeData {

public:
	int Replay::Mode();
	// メニュー描画
	void Replay::Draw();

	int Replay::Load_Reload();
	void Replay::Load_1second();
};
