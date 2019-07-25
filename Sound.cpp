#include "pch.h"

using namespace std;

#define	BGM_MAX		(20)
#define	SE_MAX		(50)

static vector<int> bgm;
static vector<int> se;
static vector<vector<int>> voice;	// �e�X�g�p
static vector<int> pVoice[2];	// �e�X�g�p

//static int se[SE_MAX];
static boolean LoadB = false;
static boolean LoadS = false;
static boolean LoadV = false;
static int volumeB, volumeS;
static int setB, setS;
static int n_bgm;	// ����BGM
static boolean noSound;	// �T�E���honoff
static int noSoundSw = 0;

int BGMStart(int num)
{

	// BGM�����[�h
	if (LoadB == 0)
	{
		// ��̂��쐬
		for (int i = 0; i < BGM_MAX; i++){
			bgm.push_back(0);
		}

		// �񓯊��ǂݍ��ݐݒ�ɕύX
		SetUseASyncLoadFlag(TRUE);

		for (int i = 0; i < STAGE_MAX; i++){
			bgm[i + 1] = LoadSoundMem("music/01.mp3");	// �X�e�[�W�� 
			//if(i == 2)bgm[i + 1] = LoadSoundMem("music/03.mp3");	// �X�e�[�W�� 
		}

		bgm[11] = LoadSoundMem("music/menu.mp3");	// ���j���[
		bgm[12] = LoadSoundMem("music/select.mp3");	// �L�����Z��
		//bgm[13] = LoadSoundMem("music/versus.mp3");	// �ΐ�O
		//bgm[14] = LoadSoundMem("music/win.mp3");		// ������� 
		//bgm[15] = LoadSoundMem("music/versus.mp3");	// �ΐ�O

		// �񓯊��ǂݍ��ݐݒ�ɕύX
		SetUseASyncLoadFlag(FALSE);

		// ���ʓǂݍ���
		volumeB = GetPrivateProfileInt("Config", "BGM", 10, "config.ini");
		
		// ���ʐݒ�
		setB = (int)(volumeB * 23);
		//if (setB > 255)setB = 255;
		//else if (setB < 0)setB = 0;
		// ���̂����
		max(0,min(255,setB));	// �E�ɍ��̐��l��
		// ���ʂ̐ݒ�
		for (int i = 0; i < BGM_MAX; i++){
			ChangeVolumeSoundMem(setB, bgm[i]);
		}

		n_bgm = 0;
		LoadB = 1;
	}

	for (int i = 0; i < BGM_MAX; i++){
		// ����BGM���~�߂�
		StopSoundMem(bgm[i]);
	}
	//ChangeVolumeSoundMem(setB, bgm[num]);
	if (!noSound){
		// BGM�ŏ����痬���Ȃ�
		if (n_bgm == num)PlaySoundMem(bgm[num], DX_PLAYTYPE_LOOP, false);	//�o�b�N�O���E���h�ōĐ�;
		else{ PlaySoundMem(bgm[num], DX_PLAYTYPE_LOOP); }	//�o�b�N�O���E���h�ōĐ�
	}

	n_bgm = num;

	return 0;
}

int SEStart(int num2)
{
	// BGM�����[�h
	if (LoadS == 0)
	{
		// ��̂��쐬
		for (int i = 0; i < SE_MAX; i++){
			se.push_back(0);
		}

		// �񓯊��ǂݍ��ݐݒ�ɕύX
		SetUseASyncLoadFlag(TRUE);

		se[1] = LoadSoundMem("sound/01_swing.wav"); // �T�E���h�����[�h
		se[2] = LoadSoundMem("sound/02_swing.wav");
		se[3] = LoadSoundMem("sound/03_swing.wav");
		se[4] = LoadSoundMem("sound/04_step.wav"); // �_�b�V��������
		se[5] = LoadSoundMem("sound/05_bstep.wav"); // �o�N�X�e
		se[6] = LoadSoundMem("sound/06_chaku.wav"); // ���n
		se[7] = LoadSoundMem("sound/07_walk.wav"); // ������
		se[8] = LoadSoundMem("sound/08_down.wav"); // �_�E��
		se[9] = LoadSoundMem("sound/09_catch.wav"); // �͂�

		se[10] = LoadSoundMem("sound/10_hit.wav"); // �q�b�g1
		se[11] = LoadSoundMem("sound/11_hit.wav"); // �q�b�g2
		se[12] = LoadSoundMem("sound/12_hit.wav"); // �q�b�g3
		se[13] = LoadSoundMem("sound/13_slash.wav"); // �q�b�g�a��
		se[14] = LoadSoundMem("sound/14_hitH.wav"); // �q�b�g3
		se[15] = LoadSoundMem("sound/15_slash2.wav"); // �q�b�g�a��2

		se[16] = LoadSoundMem("sound/16_guard.wav"); // �K�[�h1
		se[17] = LoadSoundMem("sound/17_guard.wav"); // �K�[�h2
		se[18] = LoadSoundMem("sound/18_guard.wav"); // �K�[�h3
		se[19] = LoadSoundMem("sound/19_push.wav"); // �v�b�V���K�[�h

		se[21] = LoadSoundMem("sound/21_swingA.wav"); // �����U��
		se[22] = LoadSoundMem("sound/22_spin.wav"); // �����]
		se[23] = LoadSoundMem("sound/23_wind.wav"); // ���i���j
		//se[24] = LoadSoundMem("sound/24_power.wav"); // �p���[�Q�[�W����
		se[25] = LoadSoundMem("sound/25_super.wav"); // �K�E�Z�J�b�g�C��
		se[26] = LoadSoundMem("sound/26_sliding.wav");// �X��
		se[27] = LoadSoundMem("sound/27_turn.wav"); // �u���߂�
		se[28] = LoadSoundMem("sound/28_catch.wav");// �L���b�`
		se[29] = LoadSoundMem("sound/29_tReject.wav");// ��������
		se[30] = LoadSoundMem("sound/30_freeze.wav");// ���ɔėp��������
		se[31] = LoadSoundMem("sound/31_gauge.wav");// �Q�[�W�A�N�V����
		se[32] = LoadSoundMem("sound/32_throwB.wav");// �u�A�C���v�C���g���u������
		se[33] = LoadSoundMem("sound/33_crash.wav");// �K�[�h�N���b�V��
		se[34] = LoadSoundMem("sound/35_enter.wav");// �K�[�h�N���b�V��

		se[35] = LoadSoundMem("sound/35_enter.wav"); // ����
		se[36] = LoadSoundMem("sound/36_cursor.wav"); // �J�[�\���ړ�
		se[37] = LoadSoundMem("sound/37_cancel.wav"); // �L�����Z��
		se[38] = LoadSoundMem("sound/38_bomb.wav"); // �L�����Z��
		se[39] = LoadSoundMem("sound/39_aura.wav"); // �L�����Z��

		// �񓯊��ǂݍ��ݐݒ�ɕύX
		SetUseASyncLoadFlag(FALSE);

		volumeS = GetPrivateProfileInt("Config", "SE", 10, "config.ini");

		// SE��
		setS = (int)(volumeS * 25);
		if (setS > 255)setS = 255;
		else if (setS < 0)setS = 0;
		// ���ʂ̐ݒ�
		for (int i = 0; i < SE_MAX; i++){
			ChangeVolumeSoundMem(setS, se[i]);
		}

		LoadS = 1;
	}

	// ����炷
	if ((se[num2] != 0) && (!noSound)){
		PlaySoundMem(se[num2], DX_PLAYTYPE_BACK);	//�o�b�N�O���E���h�ōĐ�
	}

	return 0;
}

int VOStart(int ch, int num, int percent)
{
	// BGM�����[�h
	if (LoadV == 0)
	{
		// ��̂��쐬
		for (int n = 0; n < CHARACTERS_MAX; n++){
			voice.push_back(vector<int>());
			for (int i = 0; i < SE_MAX; i++){
				voice[n].push_back(0);
			}
		}
		// ��̂��쐬
		for (int n = 0; n < 2; n++){
			for (int i = 0; i < SE_MAX; i++){
				pVoice[n].push_back(0);
			}
		}

		int homo;
		homo = EIN;
		voice[homo][1] = LoadSoundMem("sound/ein/01.wav"); // �ӂ�
		voice[homo][2] = LoadSoundMem("sound/ein/02.wav"); // �͂�
		voice[homo][3] = LoadSoundMem("sound/ein/03.wav"); // ������
		voice[homo][10] = LoadSoundMem("sound/ein/10.wav"); // ����
		voice[homo][11] = LoadSoundMem("sound/ein/11.wav"); // �����X�J
		voice[homo][15] = LoadSoundMem("sound/ein/15.wav"); // ������
		voice[homo][16] = LoadSoundMem("sound/ein/16.wav"); // ���炦
		voice[homo][17] = LoadSoundMem("sound/ein/17.wav"); // �����
		voice[homo][18] = LoadSoundMem("sound/ein/18.wav"); // ������
		voice[homo][19] = LoadSoundMem("sound/ein/19.wav"); // ������
		voice[homo][20] = LoadSoundMem("sound/ein/20.wav"); // �K��
		voice[homo][21] = LoadSoundMem("sound/ein/21.wav"); // �������[
		voice[homo][22] = LoadSoundMem("sound/ein/22.wav"); // �������Ȃ�
		voice[homo][23] = LoadSoundMem("sound/ein/23.wav"); // ����
		voice[homo][25] = LoadSoundMem("sound/ein/25.wav"); // ����
		voice[homo][26] = LoadSoundMem("sound/ein/26.wav"); // �f���[�b
		voice[homo][31] = LoadSoundMem("sound/ein/31.wav"); // ��_��
		voice[homo][32] = LoadSoundMem("sound/ein/32.wav"); // 
		voice[homo][34] = LoadSoundMem("sound/ein/34.wav"); // �܂��ǂ�����
		voice[homo][35] = LoadSoundMem("sound/ein/35.wav"); // �܂��ǂ�����

		homo = BOUNCER;
		voice[homo][1] = LoadSoundMem("sound/bouncer/01.wav"); // �ӂ�
		voice[homo][2] = LoadSoundMem("sound/bouncer/02.wav"); // �͂�
		voice[homo][3] = LoadSoundMem("sound/bouncer/03.wav"); // ������
		voice[homo][4] = LoadSoundMem("sound/bouncer/04.wav"); // �����ʂ�
		voice[homo][5] = LoadSoundMem("sound/bouncer/05.wav"); // �����
		voice[homo][6] = LoadSoundMem("sound/bouncer/06.wav"); // �ǂ��[
		voice[homo][10] = LoadSoundMem("sound/bouncer/10.wav"); // ����
		voice[homo][11] = LoadSoundMem("sound/bouncer/11.wav"); // �����X�J
		voice[homo][12] = LoadSoundMem("sound/bouncer/12.wav"); // �S�͂ł���
		voice[homo][13] = LoadSoundMem("sound/bouncer/13.wav"); // �㓙
		voice[homo][20] = LoadSoundMem("sound/bouncer/20.wav"); // �S�J
		voice[homo][25] = LoadSoundMem("sound/bouncer/25.wav"); // �f���[�b
		voice[homo][21] = LoadSoundMem("sound/bouncer/21.wav"); // ������Ⴀ����
		voice[homo][31] = LoadSoundMem("sound/bouncer/31.wav"); // ��_��
		voice[homo][32] = LoadSoundMem("sound/bouncer/32.wav"); // 
		voice[homo][34] = LoadSoundMem("sound/bouncer/34.wav"); // �܂��ǂ�����
		voice[homo][35] = LoadSoundMem("sound/bouncer/35.wav"); // �܂��ǂ�����

		int size = setS*1.2;
		if (size > 255)size = 255;

		// ���ʂ̐ݒ�
		for (int n = 0; n < CHARACTERS_MAX; n++){
			for (int i = 0; i < SE_MAX; i++){
				ChangeVolumeSoundMem(setS*1.5, voice[n][i]);
			}
		}

		LoadV = 1;
	}

	int per = GetRand(percent);

	for (int i = 0; i < SE_MAX; i++){
		// ����SE�����~�߂�
		StopSoundMem(voice[ch][num]);
	}
	
	// ����SE�����~�߂�
	StopSoundMem(voice[ch][num]);

	// ����炷
	if (per == 0){
		PlaySoundMem(voice[ch][num], DX_PLAYTYPE_BACK);	//�o�b�N�O���E���h�ōĐ�
	}

	return 0;
}

void PVoiceSet(int pside, int name)
{
	// ���ʂ̐ݒ�
	for (int i = 0; i < SE_MAX; i++){
		pVoice[pside - 1][i] = voice[name][i];
	}

	if (name == EIN){
		pVoice[pside - 1][1] = LoadSoundMem("sound/ein/01.wav"); // �ӂ�
		pVoice[pside - 1][2] = LoadSoundMem("sound/ein/02.wav"); // �͂�
		pVoice[pside - 1][3] = LoadSoundMem("sound/ein/03.wav"); // ������
		pVoice[pside - 1][10] = LoadSoundMem("sound/ein/10.wav"); // ����
		pVoice[pside - 1][11] = LoadSoundMem("sound/ein/11.wav"); // �����X�J
		pVoice[pside - 1][15] = LoadSoundMem("sound/ein/15.wav"); // ������
		pVoice[pside - 1][16] = LoadSoundMem("sound/ein/16.wav"); // ���炦
		pVoice[pside - 1][17] = LoadSoundMem("sound/ein/17.wav"); // �����
		pVoice[pside - 1][18] = LoadSoundMem("sound/ein/18.wav"); // ������
		pVoice[pside - 1][19] = LoadSoundMem("sound/ein/19.wav"); // ������
		pVoice[pside - 1][20] = LoadSoundMem("sound/ein/20.wav"); // �K��
		pVoice[pside - 1][21] = LoadSoundMem("sound/ein/21.wav"); // �������[
		pVoice[pside - 1][22] = LoadSoundMem("sound/ein/22.wav"); // �������Ȃ�
		pVoice[pside - 1][23] = LoadSoundMem("sound/ein/23.wav"); // ����
		pVoice[pside - 1][25] = LoadSoundMem("sound/ein/25.wav"); // ����
		pVoice[pside - 1][26] = LoadSoundMem("sound/ein/26.wav"); // �f���[�b
		pVoice[pside - 1][31] = LoadSoundMem("sound/ein/31.wav"); // ��_��
		pVoice[pside - 1][32] = LoadSoundMem("sound/ein/32.wav"); // 
		pVoice[pside - 1][34] = LoadSoundMem("sound/ein/34.wav"); // �܂��ǂ�����
		pVoice[pside - 1][35] = LoadSoundMem("sound/ein/35.wav"); // �܂��ǂ�����
	}

	if (name == BOUNCER){
		pVoice[pside - 1][1] = LoadSoundMem("sound/bouncer/01.wav"); // �ӂ�
		pVoice[pside - 1][2] = LoadSoundMem("sound/bouncer/02.wav"); // �͂�
		pVoice[pside - 1][3] = LoadSoundMem("sound/bouncer/03.wav"); // ������
		pVoice[pside - 1][4] = LoadSoundMem("sound/bouncer/04.wav"); // �����ʂ�
		pVoice[pside - 1][5] = LoadSoundMem("sound/bouncer/05.wav"); // �����
		pVoice[pside - 1][6] = LoadSoundMem("sound/bouncer/06.wav"); // �ǂ��[
		pVoice[pside - 1][10] = LoadSoundMem("sound/bouncer/10.wav"); // ����
		pVoice[pside - 1][11] = LoadSoundMem("sound/bouncer/11.wav"); // �����X�J
		pVoice[pside - 1][12] = LoadSoundMem("sound/bouncer/12.wav"); // �S�͂ł���
		pVoice[pside - 1][13] = LoadSoundMem("sound/bouncer/13.wav"); // �㓙
		pVoice[pside - 1][20] = LoadSoundMem("sound/bouncer/20.wav"); // �S�J
		pVoice[pside - 1][25] = LoadSoundMem("sound/bouncer/25.wav"); // �f���[�b
		pVoice[pside - 1][21] = LoadSoundMem("sound/bouncer/21.wav"); // ������Ⴀ����
		pVoice[pside - 1][31] = LoadSoundMem("sound/bouncer/31.wav"); // ��_��
		pVoice[pside - 1][32] = LoadSoundMem("sound/bouncer/32.wav"); // 
		pVoice[pside - 1][34] = LoadSoundMem("sound/bouncer/34.wav"); // �܂��ǂ�����
		pVoice[pside - 1][35] = LoadSoundMem("sound/bouncer/35.wav"); // �܂��ǂ�����
	}

	// ���ʂ̐ݒ�
	for (int i = 0; i < SE_MAX; i++){
		ChangeVolumeSoundMem(setS*1.5, pVoice[pside - 1][i]);
	}

}


void StopSE(int num)
{
	for (int i = 0; i < SE_MAX; i++){
		// �S�Ă�SE���~�߂�
		StopSoundMem(se[num]);
	}
}

int PVOStart(int pside, int num, int percent)
{
	int per = GetRand(percent);

	for (int i = 0; i < SE_MAX; i++){
		// ����SE�����~�߂�
		StopSoundMem(pVoice[pside - 1][i]);
	}

	// ����炷
	if (per == 0){
		PlaySoundMem(pVoice[pside-1][num], DX_PLAYTYPE_BACK);	//�o�b�N�O���E���h�ōĐ�
	}

	return 0;
}

void StopSound(int num)
{
	if (num > 0)noSoundSw++;
	else{ noSoundSw = 0; }

	if (noSoundSw == 1){
		if (noSound)noSound = false;
		else{ noSound = true; }
	}
}