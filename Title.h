#pragma once

#include "SData.h"

// public で前のクラスのものも使える
class Title : public MData {

public:
	int Title::Mode();
	// メニュー描画
	void Title::Draw();

	void Title::Load_Reload();
	void Title::Load_1second();

	// 解放
	void Release();
};