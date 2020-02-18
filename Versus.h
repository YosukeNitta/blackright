#pragma once

#include "SData.h"

// public で前のクラスのものも使える
class Versus : public MData {

public:
	int Mode();
	// メニュー描画
	void Draw();

	void Load_Reload();
	void Load_1second();
	// 解放
	void Release();
};
