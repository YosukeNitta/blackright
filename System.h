//===============================================================================
//
//			格闘ゲーム　　　描画ヘッダ部
//
//===============================================================================
#include "SSet.h"
#include "PSet.h"
#include "vector"
#include <string>

#ifndef DEF_SYSTEM_H	//一度定義されていたら、定義しない
#define DEF_SYSTEM_H

class Def_c{
public:
	int charMax;
	std::vector<Name_t>names;

};

/**********
* FPS
**********/
bool Update();	//FPS更新
void Draw();	//FPSを表示
void Wait();	//

/**********
* Key
**********/

void GetInput();

int P_Input(int n);
// ボタン入力チェック
int P1_BCheck();
int P2_BCheck();
void InputSend();		// 入力をStateに送る

// ボタン入力判断
int P1_BInput(int P1_BNum);
int P2_BInput(int P2_BNum);
int PH_BInput(int PSide, int P_Num);
int P_BInput(int P_Num);
boolean KInput(int num);

int P1_BConfig();

int P1_BSet(int num);
int P2_BSet(int num);

void P2_Auto(int i);
void P2Ctrl();
void P2Ctrl2();
void P2Ctrl3(int num[8]);
void P2Ctrl3_2();
void P2_AI(Player GP1, Player GP2);
void Stop_P2_B();

void GetPH_Key(Player GP);
void GetP_Key(Player GP1, Player GP2);
void GetS_Key(System_t GS);

void Get_Network(boolean net);
void Network_Setting(int side, IPDATA ip, int port, int handle);
void NB_Reset();


void End_Network();

// 0.なし 1.再生 2.録画
int Replay_Mode(int num);
void Replay_End();
void Replay_Setting(int n1, int n2, int c1, int c2, int stageNum);
int Replay_ReserveKey();
void Replay_EnterSetting(int n1, int n2, int c1, int c2, int stageNum);
void Key_GetString(std::string st);

/**********
* Load
**********/

int LoadAllFile();

//-------
// Order
//-------
// 描画ブレンドモードをアルファブレンド（透明）にする
void SetAlpha(int n);

// 描画ブレンドモードをノーブレンドにする
void NoBlend();

// 簡易暗転表示
void Anten(int num);
boolean AntenCheck();

// フェードアウト
void Fadeout(int num);
// フェードアウト確認
boolean FadeoutCheck();
// スクショ
void ScreenShot();
// フォント変更
static void SetFont();
// 画面を保存(スクショとは別)
void SaveScreen();
// 保存画面を表示
void Draw_SaveScreen();
// 判定重なってるかチェック
bool Box_Touch(int x1, int y1, int w1, int h1,
	int x2, int y2, int w2, int h2);

//-------
// Connect
//-------
// ネットワーク取得
void Connect_GetCn(boolean net);
// ネットワーク確認
boolean Connect_CheckCn();
// プレイヤーサイド確認
void Connect_GetSide(int num);
int Connect_CheckSide();
// ポート確認
void Connect_GetPort(int num);
int Connect_CheckPort();
// ハンドル取得
void Connect_GetNetHandle(int num);
int Connect_CheckNetHandle();
// IP取得
void Connect_GetIp(IPDATA num);
IPDATA Connect_CheckIp();

// ディレイ
int Connect_CheckDelay();
void Delay_Check();
void Delay_Draw();
boolean Delay_IfEnd();

// 操作関連
void Connect_Get_GetData(BYTE num, BYTE num1, BYTE num2);
BYTE Connect_CheckGetData(int num1, int num2);

void Connect_Load();
void Connect_SetGameMode();
boolean Connect_WaitGameMode();

//-------------
// Def
//-------------
void GetDef();
// ゲーム内のキャラ数を返す
int Character_Max();
// 登録番号の名前を返す
Name_t Character_Name(int num);

#endif