//===============================================================================
//
//			�i���Q�[���@�@�@�`��w�b�_��
//
//===============================================================================

#ifndef DEF_SOUND_H	//��x��`����Ă�����A��`���Ȃ�
#define DEF_SOUND_H

int BGMStart(int num);
int SEStart(int num2);
void StopSE(int num);
int VOStart(int character, int num, int percent);
void PVoiceSet(int pside, int name);
int PVOStart(int pside, int num, int percent);
void StopSound(int num);

#endif
