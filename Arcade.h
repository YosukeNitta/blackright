#include "SData.h"
using namespace modeData;

#ifndef DEF_ARCADE_H	//��x��`����Ă�����A��`���Ȃ� 
#define DEF_ARCADE_H

// public �őO�̃N���X�̂��̂��g����
class Arcade : public ModeData {

public:
	int Arcade::Mode();
	// ���j���[�`��
	void Arcade::Draw();

	int Arcade::Load_Reload();
	void Arcade::Load_1second();
};

#endif