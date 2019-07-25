#include "SData.h"
using namespace modeData;

#ifndef DEF_REPLAY_H	//一度定義されていたら、定義しない 
#define DEF_REPLAY_H

// public で前のクラスのものも使える
class Replay : public ModeData {

public:
	int Replay::Mode();
	// メニュー描画
	void Replay::Draw();

	int Replay::Load_Reload();
	void Replay::Load_1second();
};

#endif