#include "pch.h"

int LoadAllFile()
{
	StageLoad();	// �X�e�[�W�摜	.. �X�e�[�W�ő吔��ݒ肵�Ă���
	BGMStart(0);
	SEStart(0);
	VOStart(0, 0, 0);
	EffStart(0, 0, 0, 0, 0, 0, 0, false);

	fLoad_State();
	CharLoad();
	Load_Pause();

	return 0;
}