#include "SData.h"
using namespace modeData;

#ifndef DEF_MENU_H	//��x��`����Ă�����A��`���Ȃ� 
#define DEF_MENU_H

// public �őO�̃N���X�̂��̂��g����
class Menu : public ModeData {

public:
	int Menu::Mode();
	// ���j���[�`��
	void Menu::Draw();

	int Menu::Load_Reload();
	void Menu::Load_1second();
};

#endif