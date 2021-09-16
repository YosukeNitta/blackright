#include "pch.h"

int LoadAllFile()
{
	StageLoad();	// ステージ画像	.. ステージ最大数を設定している
	BGMStart(0);
	SEStart(0);
	VOStart(0, 0, 0);
	EffStart(0, 0, 0, 0, 0, 0, 0, false);

	fLoad_State();
	CharLoad();
	Load_Pause();

	return 0;
}