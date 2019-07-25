#include "SData.h"
using namespace modeData;

#ifndef DEF_SELECT_H	//一度定義されていたら、定義しない 
#define DEF_SELECT_H

// public で前のクラスのものも使える
class Select : public ModeData {

public:
	int Select::Mode();
	// メニュー描画
	void Select::Draw();

	int Select::Load_Reload();
	void Select::Load_1second();
};

#endif