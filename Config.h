#pragma once

#include "SData.h"

// public で前のクラスのものも使える
class Config : public MData {

public:
	int Config::Mode();
	// メニュー描画
	void Config::Draw();

	void Config::Load_Reload();
	void Config::Load_1second();
	// 解放
	void Release();
};
