//インクルード部--------------------------------------------
#include "pch.h"
#include "Arcade.h"
using namespace modeData;

#include <string>	// stringに使う
#include <vector>
using namespace std;	// vecterで使用

static int playerChar;
static int battleCount;	// CPU撃破数
static vector<int> port;	// これに全てのキャラの判定
static vector<int> enemyName;	// これに全てのキャラの判定
static int name, color;
static int arcadeFlg;

int Arcade::Mode(void)
{
	Get_Color(color, 1);		// カラー決定
	Get_Color(0, 2);		// カラー決定
	if (enemyName[battleCount] == CORNEL)GetStageNum(2, 2);	// ステージ受け取り;
	else{ GetStageNum(0, 0); }	// ステージ受け取り
	Versus_bgmNum(0 + 1);
	Select_SetName(name, enemyName[battleCount]);

	if ((P1_BInput(1) == 1) || (P1_BInput(3) == 1)){
		if (battleCount >= Character_Max() - 1)ModeChange(GameScene(MenuScene));
		else{ ModeChange(GameScene(VersusScene)); }
	}

	return 0;
}
// なんどもお。
int Arcade::Load_Reload()
{
	Anten(255);
	// キャラセレBGM
	BGMStart(12);

	return 0;
}
// 画像のロードなど
void Arcade::Load_1second()
{
	battleCount = 0;
	Name_t name;
	string str, str2, fn;
	int r, g, b;
	int handle;
	str2 = "char/";
	fn = "/pal/port.bmp";

	// ポートレート取得
	for (int i = 0; i < Character_Max(); i++){
		port.push_back(0);
		name = Character_Name(i);
		str = str2 + name.nameC + fn;

		// 画像の読み込み
		handle = LoadSoftImage(str.c_str());
		// パレットの色を取得する
		GetPaletteSoftImage(handle, 0, &r, &g, &b, 0);
		// 透明色決定
		SetTransColor(r, g, b);

		// 画像生成
		port[i] = LoadGraph(str.c_str());
	}
	SetTransColor(0, 0, 0);
	// 使い終わったら解放
	DeleteSoftImage(handle);
}
void Arcade::Draw(void)
{
	DrawFormatString(0, 0, Cr, "%dwin", battleCount);
	if(battleCount >= Character_Max() - 1)DrawFormatString(0, 0, Cr, "%dwin", battleCount);

	for (int i = 0; i < Character_Max() - 1; i++){
		if (battleCount > i){
			SetDrawBlendMode(DX_BLENDMODE_ADD, 128); //加算合成
		}
		DrawGraph(i * 70, 20, port[enemyName[i] - 1], true);
		NoBlend();
	}
}

// -1..確認のみ　0or1..変更
boolean Arcade_Switch(int n)
{
	if (n == 1)arcadeFlg = true;
	else if(n == 0){ arcadeFlg = false; }

	return arcadeFlg;
}

// -1..確認のみ　0or1..変更
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
		// チェック
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