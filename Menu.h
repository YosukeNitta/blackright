#include "SData.h"
using namespace modeData;

#ifndef DEF_MENU_H	//一度定義されていたら、定義しない 
#define DEF_MENU_H

// public で前のクラスのものも使える
class Menu : public ModeData {

public:
	int Menu::Mode();
	// メニュー描画
	void Menu::Draw();

	int Menu::Load_Reload();
	void Menu::Load_1second();
};

#endif