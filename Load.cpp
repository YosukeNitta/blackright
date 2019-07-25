#include "pch.h"

int LoadAllFile()
{
	BGMStart(0);
	SEStart(0);
	VOStart(0, 0, 0);
	EffStart(0, 0, 0, 0, 0, 0, 0, false);
	
	fLoad_State();
	CharLoad();
	Load_Pause();
	Connect_Load();

	return 0;
}