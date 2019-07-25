#include "SData.h"
using namespace modeData;

#ifndef DEF_ARCADE_H	//一度定義されていたら、定義しない 
#define DEF_ARCADE_H

// public で前のクラスのものも使える
class Arcade : public ModeData {

public:
	int Arcade::Mode();
	// メニュー描画
	void Arcade::Draw();

	int Arcade::Load_Reload();
	void Arcade::Load_1second();
};

#endif