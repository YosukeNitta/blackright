#include "SData.h"
using namespace modeData;

#ifndef DEF_REPLAY_H	//��x��`����Ă�����A��`���Ȃ� 
#define DEF_REPLAY_H

// public �őO�̃N���X�̂��̂��g����
class Replay : public ModeData {

public:
	int Replay::Mode();
	// ���j���[�`��
	void Replay::Draw();

	int Replay::Load_Reload();
	void Replay::Load_1second();
};

#endif