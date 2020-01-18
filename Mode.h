//===============================================================================
//
//			格闘ゲーム　　　描画ヘッダ部
//
//===============================================================================
#include "SSet.h"
#include "PSet.h"
//#include "vector"

#ifndef DEF_MODE_H	//一度定義されていたら、定義しない
#define DEF_MODE_H

//*****
// Main
//*****
void ModeChange(int num);

//int Key_Save(void);
//int Key_Load(void);
//int OutNew(int num, int num2);
//int OutSend(int i);

// ゲームモード取得
int CheckGameMode();
//void Main_GetNetwork(boolean net);
//boolean Main_CheckNetwork();

/****
* Menu
****/
//int Menu();
//int Load_Menu();


/****
* Select
****/
//int Select();

int Player_Name_1();
int Player_Name_2();
void Select_SetName(int p1, int p2);

//void Load_Select();
int BattleMode(int f);
void Select_GetName(std::vector<Name_t>GN);
void GetA_Select(Air GA, int name);
void GetPI_Select(Pict GPI[IMAGE2_MAX], int name);
void Select_ReturnCursor();

/****
* Versus
****/
int Versus();		//対戦

void PauseSetting(int n1);
void Load_Versus();
void Versus_bgmNum(int num);
void Versus_TurboMode();
boolean Versus_GetTurboMode();

/****
* Pause
****/
int Pause();
void GetP_Pause(Player GP1, Player GP2);
void GetS_Pause(System_t GS);
void Load_Pause();
void BackGraph();
void raw_set();

/****
* Config
****/
int Config();
void Load_Config();

//=======
// Title
//=======
int Title();

//=======
// NetWork
//=======
int Network();
int Load_Network();

//=======
// Arcade
//=======
// -1..確認のみ　0or1..変更
boolean Arcade_Switch(int n);
void Arcade_GetData(int gName, int gColor);
void Arcade_BattleCount(int n);
void Arcade_Setting();

#endif