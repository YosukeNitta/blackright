//�C���N���[�h��--------------------------------------------
#include "pch.h"

// ���j���[����
#include "Replay.h"
using namespace modeData;

// �t�@�C��������
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <string>	// string�Ɏg��
#include <vector>

using namespace std;	// vecter�Ŏg�p

void Replay_GetFileData();
void Replay_GetPort(int num);
int Replay_Delete();

//�ް���`��------------------------------------------------
static int yPos = 0, xPos = 0;
static int fileCount = 0;
static int deleteSwitch = 0;
static vector<string> repName;	// ����ɑS�ẴL�����̔���
static vector<int> r_port;	// ����ɑS�ẴL�����̔���
static vector<int> r_cn1;	// ����ɑS�ẴL�����̔���
static vector<int> r_cn2;	// ����ɑS�ẴL�����̔���
static int r_name[2], r_color[2], r_stage;

// 
void ReadReplayFile()
{
	// ����Ȃ�폜
	int rSize = repName.size();
	if (rSize > 0){
		for (int i = 0; i < rSize; i++){
			repName.pop_back();    //  �����f�[�^���폜
			r_cn1.pop_back();    //  �����f�[�^���폜
			r_cn2.pop_back();    //  �����f�[�^���폜
		}
	}

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	TCHAR *target = "replay/\\*.rep"; 

	hFind = FindFirstFile(target, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			repName.push_back(FindFileData.cFileName);
			r_cn1.push_back(0);
			r_cn2.push_back(0);
			fileCount++;
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}

	for (int i = 0; i < repName.size(); i++){
		Replay_GetPort(i);
	}
}

int Replay::Mode()
{
	if (P_BInput(2) == 1){
		SEStart(37);
		ModeChange(GameScene(MenuScene));	// �L�����Z����
	}
	// �L�����A�J���[�A�X�e�[�W�@��ǂݍ���
	else if (((P_BInput(1) == 1) || (P_BInput(3) == 1)) && (repName.size() > 0)){
		Replay_GetFileData();

		Get_Color(r_color[0], 1);		// �J���[����
		Get_Color(r_color[1], 2);		// �J���[����
		GetStageNum(r_stage, r_stage);	// �X�e�[�W�󂯎��
		Versus_bgmNum(r_stage + 1);
		Select_SetName(r_name[0], r_name[1]);

		TraningSwitch(false);
		AISwitch(false);
		BattleMode(0);
		WinCount(0);

		Replay_End();
		Key_GetString(repName[yPos + (xPos * 7)]);
		Replay_Mode(1);
		
		ModeChange(GameScene(VersusScene));
	}
	// �L�����A�J���[�A�X�e�[�W�@��ǂݍ���
	else if ((P_BInput(4) == 1) && (repName.size() > 0)){
		SEStart(38);
		Replay_Delete();
	}

	if (repName.size() > 0){
		if ((P_BInput(108) == 1)){
			yPos--;
			SEStart(36);
		}
		else if ((P_BInput(102) == 1)){
			yPos++;
			SEStart(36);
		}

		if (yPos < 0){
			yPos = 6;
			if (((xPos * 7) + yPos) > repName.size() - 1)
				yPos = (repName.size() % 7) - 1;
		}
		if ((yPos > 6) ||
			(((xPos * 7) + yPos) > repName.size() - 1)
			){
			yPos = 0;
		}

		
	}
	if (repName.size() > 7){
		if ((P_BInput(104) == 1)){
			xPos--;
			SEStart(36);
		}
		else if ((P_BInput(106) == 1)){
			xPos++;
			SEStart(36);
		}

		if ((xPos * 7) < 0){
			xPos = repName.size() / 7;
		}
		if ((xPos * 7) >(repName.size() - 1)){
			xPos = 0;
		}
	}

	return 0;
}

// ���j���[�`��
void Replay::Draw()
{
	// �J�[�\���`��
	{
		int minus = 0;
		if (xPos > 0)minus = xPos * 6;

		// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
		// �J�[�\���`��
		DrawBox(0, (yPos * 60) + 20, SCREEN_W, (yPos * 60) + 80, GetColor(20, 20, 250), true);
		NoBlend();
	}

	DrawString(0,0,"���v���C", Cr);
	DrawString(10, SCREEN_H - 20, "D..�t�@�C���폜  S..�Đ��I��", Cr);
	DrawFormatString(100, 0, Cr, "%d / %d", yPos + (xPos * 7) + 1, repName.size());
	for (int i = 0; i < 7; i++){
		if ((i + (xPos * 7)) < repName.size()){
			DrawFormatString(20, 20 + (60 * i), Cr, "%s", repName[i + (xPos * 7)].c_str());
			DrawGraph(320, 20 + (60 * i), r_port[r_cn1[i + (xPos * 7)]], true);
			DrawGraph(380, 20 + (60 * i), r_port[r_cn2[i + (xPos * 7)]], true);
		}
	}
	//DrawFormatString(420, 20, Cr, "%s", repName[yPos].c_str());
	//DrawFormatString(0, SCREEN_H - 20, Cr, "�t�@�C����%d", repName.size());
}

int Replay::Load_Reload()
{
	fileCount = 0;
	// rep�t�@�C�����[�h
	ReadReplayFile();

	for (int i = 0; i < 2; i++){
		r_name[i] = 0;
		r_color[i] = 0;
	}
	r_stage = 0;

	// BGM�ǂݍ���
	BGMStart(11);

	return 0;
}

void Replay::Load_1second()
{
	Name_t name;
	string str, str2, fn;
	int r, g, b;
	int handle;
	str2 = "char/";
	fn = "/pal/port.bmp";

	// �|�[�g���[�g�擾
	for (int i = 0; i < Character_Max(); i++){
		r_port.push_back(0);
		name = Character_Name(i);
		str = str2 + name.nameC + fn;

		// �摜�̓ǂݍ���
		handle = LoadSoftImage(str.c_str());
		// �p���b�g�̐F���擾����
		GetPaletteSoftImage(handle, 0, &r, &g, &b, 0);
		// �����F����
		SetTransColor(r, g, b);

		// �摜����
		r_port[i] = LoadGraph(str.c_str());
	}
	SetTransColor(0, 0, 0);
	// �g���I���������
	DeleteSoftImage(handle);
}

void Replay_EnterSetting(int n1, int n2, int c1, int c2, int stageNum)
{
	r_name[0] = n1, r_name[1] = n2;
	r_color[0] = c1, r_color[1] = c2;
	r_stage = stageNum;
}

void Replay_GetFileData()
{
	// ����Z�b�e�B���O //

	int i, fp;
	int anum;	// Name�̏ꏊ
	anum = 0;

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// input��inputc�ɕ������͂���
	boolean iflg = false;	// ���O���̓t���O
	char t_setting[8] = { "setting" };

	string str = "replay/" + repName[yPos + (xPos * 7)];

	// �t�@�C�����J�� //
	fp = FileRead_open(str.c_str());//�t�@�C���ǂݍ���
	if (fp == NULL){			// �G���[���N��������
		printfDx("error repfile\n");// ������\��
		return;
	}
	// �t�@�C���|�C���^��擪�Ɉړ�����
	FileRead_seek(fp, 0, SEEK_SET);

	// 1�s�ڂ̂݊J���ĕ���
	while (1){
		//=========================
		// ��{���[�v
		// ���̃��[�v�I����ɉ���
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1�����擾����

			// ���s�œǂݍ��ݏI��
			if (input[i] == '\n'){
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				break;
			}

			// ������ɂ���
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == ';'){ //�J���}�����s�Ȃ�
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				break;	// i��1���₵�ă��[�v�E�o�i�������j
			}

		}	// ��{���[�v�I��


		// ���O��������
		if (iflg){
			switch (anum)
			{
			case 0:
				r_name[0] = atoi(inputc); break;
			case 1:
				r_name[1] = atoi(inputc); break;
			case 2:
				r_color[0] = atoi(inputc); break;
			case 3:
				r_color[1] = atoi(inputc); break;
			case 4:
				r_stage = atoi(inputc); break;
			default:
				break;
			}
			anum++;	// �ŏ���0�ɂȂ�
		}

		if (strcmp(inputc, t_setting) == 0){
			iflg = true;
		}

		if (input[i] == '\n')goto EXFILE;
	}

	// �t�@�C�������
EXFILE:
	FileRead_close(fp);
	fp = NULL;
}

void Replay_GetPort(int num)
{
	// ����Z�b�e�B���O //

	int i, fp;
	int anum;	// Name�̏ꏊ
	anum = 0;

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// input��inputc�ɕ������͂���
	boolean iflg = false;	// ���O���̓t���O
	char t_setting[8] = { "setting" };

	string str = "replay/" + repName[num];

	// �t�@�C�����J�� //
	fp = FileRead_open(str.c_str());//�t�@�C���ǂݍ���
	if (fp == NULL){			// �G���[���N��������
		printfDx("error repfile\n");// ������\��
		return;
	}
	// �t�@�C���|�C���^��擪�Ɉړ�����
	FileRead_seek(fp, 0, SEEK_SET);

	// 1�s�ڂ̂݊J���ĕ���
	while (1){
		//=========================
		// ��{���[�v
		// ���̃��[�v�I����ɉ���
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1�����擾����

			// ���s�œǂݍ��ݏI��
			if (input[i] == '\n'){
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				break;
			}

			// ������ɂ���
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == ';'){ //�J���}�����s�Ȃ�
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				break;	// i��1���₵�ă��[�v�E�o�i�������j
			}

		}	// ��{���[�v�I��


		// ���O��������
		if (iflg){
			switch (anum)
			{
			case 0:
				r_cn1[num] = atoi(inputc) - 1; break;
			case 1:
				r_cn2[num] = atoi(inputc) - 1; break;
			default:
				break;
			}
			anum++;	// �ŏ���0�ɂȂ�
		}

		if (strcmp(inputc, t_setting) == 0){
			iflg = true;
		}

		if (input[i] == '\n'){
			goto EXFILE;
		}
	}

	// �t�@�C�������
EXFILE:
	FileRead_close(fp);
	fp = NULL;
}

int Replay_Delete()
{
	string str = "replay/" + repName[yPos + (xPos * 7)];

	if ((remove(str.c_str()) == 0) && (repName.size() > 0)){
		repName.erase(repName.begin() + yPos + (xPos * 7));    //  �����f�[�^���폜
		r_cn1.erase(r_cn1.begin() + yPos + (xPos * 7));    //  �����f�[�^���폜
		r_cn2.erase(r_cn2.begin() + yPos + (xPos * 7));    //  �����f�[�^���폜
	}
	return 0;
}