//===============================================================================
//
//			格闘ゲーム　　　描画ヘッダ部
//
//===============================================================================
#include "SSet.h"
#include "PSet.h"


#ifndef DEF_GRAPH_H	//一度定義されていたら、定義しない
#define DEF_GRAPH_H


/****
* MainDraw
****/
void MainDraw();
void GetStageNum(int num, int stage);
void GetP_MainDraw(Player GP1, Player GP2);
void GetS_MainDraw(System_t GS);
void GetH_MainDraw(Helper GH1[], Helper GH2[]);
void MainDraw_GetPort(int n, int pside);
void MainDraw_GetName(int n, int pside);
void afterimageSetting(int side, int time, int r, int g, int b);

/****
* ObjectDraw
****/
void ObjectDraw();
void GetP_ObjectDraw(Player GP1, Player GP2);
void GetS_ObjectDraw(System_t GS);
void GetH_ObjectDraw(Helper GH1[], Helper GH2[]);
void TestText(int i, int pos);
void TestChar(char ch[], int size);
void AttackSpeed(int i);
// ダメージ
void Damage_Check(int i);
void Frame_Check();
void Beat_Check(int state, int pside);
void ObjectDraw_KeyDisplay();
void AddGaugeDraw(int power, int side);
void SetBlack(int b);

/****
* Effect
****/
// num は呼び出す画像
int EffStart(int Effnum, int EffXP, int EffYP, int basePosX, int basePosY,
	float SizeX, float SizeY, boolean turn);
// 後ろに表示
int EffStartB(int Effnum, int EffXP, int EffYP, int basePosX, int basePosY,
	float SizeX, float SizeY, boolean turn);
void EffDraw();
void EffDrawB();
// 簡易表示
void EasyEffDraw(int num1, int time, int EffXP, int EffYP, int basePosX, int basePosY,
	float SizeX, float SizeY, boolean turn);
void GetP_Effect(Player GP1, Player GP2);
void GetS_Effect(System_t GS);
void GetH_Effect(Helper GH1[], Helper GH2[]);

#endif
