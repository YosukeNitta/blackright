#include "SData.h"
using namespace modeData;

#ifndef DEF_SELECT_H	//��x��`����Ă�����A��`���Ȃ� 
#define DEF_SELECT_H

// public �őO�̃N���X�̂��̂��g����
class Select : public ModeData {

public:
	int Select::Mode();
	// ���j���[�`��
	void Select::Draw();

	int Select::Load_Reload();
	void Select::Load_1second();
};

#endif