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

void GetSoundData();

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

		GetSoundData();
		/*
		se[1] = LoadSoundMem("sound/01_swing.wav"); // �T�E���h�����[�h
		*/

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

	//if (se > se.size)return 0;

	// ����炷
	if ((se[num2] != 0) && (!noSound)){
		PlaySoundMem(se[num2], DX_PLAYTYPE_BACK);	//�o�b�N�O���E���h�ōĐ�
	}

	return 0;
}

int VOStart(int ch, int num, int percent)
{
	/*

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

	*/
	return 0;
}

void PVoiceSet(int pside, int name)
{
	/*
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
	*/
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
	/*
	int per = GetRand(percent);

	for (int i = 0; i < SE_MAX; i++){
		// ����SE�����~�߂�
		StopSoundMem(pVoice[pside - 1][i]);
	}

	// ����炷
	if (per == 0){
		PlaySoundMem(pVoice[pside-1][num], DX_PLAYTYPE_BACK);	//�o�b�N�O���E���h�ōĐ�
	}
	*/

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

void GetSoundData()
{
	// ����Z�b�e�B���O //

	int i, fp;
	int anum;	// Name�̏ꏊ
	int num;	// �Z�b�g�ԍ�
	anum = -1;

	char fname[15] = { "sound/list.txt" };
	
	// ���O�ǂݍ���
	string setName;
	string fn1 = "sound/";
	
	int input[NAME_MAX];
	char inputc[NAME_MAX];	// input��inputc�ɕ������͂���
	boolean iflg = false;	// ���O���̓t���O
	boolean next = false;

	// �t�@�C�����J�� //
	fp = FileRead_open(fname);//�t�@�C���ǂݍ���
	if (fp == NULL) {			// �G���[���N��������
		printfDx("error name\n");// ������\��
		return;
	}

	//�ŏ���1�s�ǂݔ�΂�
	while (FileRead_getc(fp) != '\n'); // FileRead_getc:�ꕶ���ǂݏo��

	while (1) {
		//=========================
		// ��{���[�v
		// ���̃��[�v�I����ɉ���
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1�����擾����

			// �X���b�V���ŉ��s
			if (inputc[i] == '/') {					//�X���b�V���������
				while (FileRead_getc(fp) != '\n');	//���s�܂Ń��[�v
				i = -1;//�J�E���^���ŏ��ɖ߂��� (�߂����Ƃ���0�ɂȂ�)
				continue;
			}

			// ���s�Ŏ��̍s�ɐi��
			if (input[i] == '\n') {
				for (int j = 0; j < NAME_MAX; j++) {
					inputc[j] = input[j] = NULL;
					//inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				}
				i = -1;
				break;
			}

			// ������ɂ���
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == ';' || input[i] == '=') { //�J���}�����s�Ȃ�
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				break;	// i��1���₵�ă��[�v�E�o�i�������j
			}

			// �ǂݍ��ݏI��
			if (input[i] == EOF) {//�t�@�C���̏I���Ȃ�
				goto EXFILE;//�I��
			}
		}	// ��{���[�v�I��



		// �ŏ��ɐ��������ɂ��āA�����������Ă���ύX����
		/*
		iflg = false;
		{
			if ((inputc[0] != '\0') && (inputc[0] != '\n') && (inputc[0] != NULL)
				&& (anum < Character_Max()))iflg = true;
		}
		*/

		// �ŏ��ɐ��������ɂ��āA�����������Ă���ύX����
		iflg = false;

		// ������Z�b�g�Ɉڍs���Ă��Ȃ�
		if(!next)
		{
			if (inputc[0] == '0')iflg = true;
			else if (inputc[0] == '1')iflg = true;
			else if (inputc[0] == '2')iflg = true;
			else if (inputc[0] == '3')iflg = true;
			else if (inputc[0] == '4')iflg = true;
			else if (inputc[0] == '5')iflg = true;
			else if (inputc[0] == '6')iflg = true;
			else if (inputc[0] == '7')iflg = true;
			else if (inputc[0] == '8')iflg = true;
			else if (inputc[0] == '9')iflg = true;
		}

		// ���O��������
		if (iflg) {
			num = atoi(inputc);	// �ԍ����Z�b�g
			next = true;	// ���̏���
		}

		// ������Z�b�g
		if (next && (inputc[0] != '\0' || inputc[0] != NULL || inputc[0] != '\n')) {
			//strcpy_s(buf, inputc);
			setName = fn1 + inputc;
			se[num] = LoadSoundMem(setName.c_str());
			next = false;
		}

		if (input[i] == EOF) {//�t�@�C���̏I���Ȃ�
			goto EXFILE;//�I��
		}
	}

	// �t�@�C�������
EXFILE:
	FileRead_close(fp);

	// Pict��Name�𑗂�
	//GetN_Pict(N);
}