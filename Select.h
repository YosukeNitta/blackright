#pragma once

#include "SData.h"


// public で前のクラスのものも使える
class Select : public MData {

public:
	int Select::Mode();
	// メニュー描画
	void Select::Draw();

	void Select::Load_Reload();
	void Select::Load_1second();
	// 解放
	void Release();
};

