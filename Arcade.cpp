//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include "Arcade.h"
using namespace modeData;

#include <string>	// string�Ɏg��
#include <vector>
using namespace std;	// vecter�Ŏg�p

static int playerChar;
static int battleCount;	// CPU���j��
static vector<int> port;	// ����ɑS�ẴL�����̔���
static vector<int> enemyName;	// ����ɑS�ẴL�����̔���
static int name, color;
static int arcadeFlg;

int Arcade::Mode(void)
{
	Get_Color(color, 1);		// �J���[����
	Get_Color(0, 2);		// �J���[����
	if (enemyName[battleCount] == CORNEL)GetStageNum(2, 2);	// �X�e�[�W�󂯎��;
	else{ GetStageNum(0, 0); }	// �X�e�[�W�󂯎��
	Versus_bgmNum(0 + 1);
	Select_SetName(name, enemyName[battleCount]);

	if ((P1_BInput(1) == 1) || (P1_BInput(3) == 1)){
		if (battleCount >= Character_Max() - 1)ModeChange(GameScene(MenuScene));
		else{ ModeChange(GameScene(VersusScene)); }
	}

	return 0;
}
// �Ȃ�ǂ����B
int Arcade::Load_Reload()
{
	Anten(255);
	// �L�����Z��BGM
	BGMStart(12);

	return 0;
}
// �摜�̃��[�h�Ȃ�
void Arcade::Load_1second()
{
	battleCount = 0;
	Name_t name;
	string str, str2, fn;
	int r, g, b;
	int handle;
	str2 = "char/";
	fn = "/pal/port.bmp";

	// �|�[�g���[�g�擾
	for (int i = 0; i < Character_Max(); i++){
		port.push_back(0);
		name = Character_Name(i);
		str = str2 + name.nameC + fn;

		// �摜�̓ǂݍ���
		handle = LoadSoftImage(str.c_str());
		// �p���b�g�̐F���擾����
		GetPaletteSoftImage(handle, 0, &r, &g, &b, 0);
		// �����F����
		SetTransColor(r, g, b);

		// �摜����
		port[i] = LoadGraph(str.c_str());
	}
	SetTransColor(0, 0, 0);
	// �g���I���������
	DeleteSoftImage(handle);
}
void Arcade::Draw(void)
{
	DrawFormatString(0, 0, Cr, "%dwin", battleCount);
	if(battleCount >= Character_Max() - 1)DrawFormatString(0, 0, Cr, "%dwin", battleCount);

	for (int i = 0; i < Character_Max() - 1; i++){
		if (battleCount > i){
			SetDrawBlendMode(DX_BLENDMODE_ADD, 128); //���Z����
		}
		DrawGraph(i * 70, 20, port[enemyName[i] - 1], true);
		NoBlend();
	}
}

// -1..�m�F�̂݁@0or1..�ύX
boolean Arcade_Switch(int n)
{
	if (n == 1)arcadeFlg = true;
	else if(n == 0){ arcadeFlg = false; }

	return arcadeFlg;
}

// -1..�m�F�̂݁@0or1..�ύX
void Arcade_GetData(int gName, int gColor)
{
	name = gName;
	color = gColor;
}

// 
void Arcade_BattleCount(int n)
{
	if (n == 1)battleCount += 1;
}

// 
void Arcade_Setting()
{
	battleCount = 0;
	boolean ef = false;
	if (enemyName.size() > 0)ef = true;
	int plus = 1;

	for (int i = 0; i < Character_Max(); i++){
		if(!ef)enemyName.push_back(0);
		if ((i + 1) == name)plus = 2;
		enemyName[i] = plus + i;
	}
	vector<int> randCh;
	boolean flg = false;

	for (int i = 0; i < Character_Max() - 1; i++){
		randCh.push_back(0);
		flg = false;
		// �`�F�b�N
		while (!flg){
			randCh[i] = enemyName[GetRand(Character_Max() - 2)];
			flg = true;
			for (int j = 0; j < i; j++){
				if (randCh[i] == randCh[j])flg = false;
			}
		}
	}
	for (int i = 0; i < Character_Max() - 1; i++){
		enemyName[i] = randCh[i];
	}
}