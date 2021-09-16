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
// クラス作ったのでいらない

//bool Update();	//FPS更新
//void Draw();	//FPSを表示
//void Wait();	//

/**********
* Key
**********/

void GetInput();

int P_Input(int n);
// ボタン入力チェック
int P1_BCheck();
int P2_BCheck();
// P1P2のBCheckを実行
void Key_Setup();
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

// network関連
void Key_SetPlayerSide(int n);
int Key_GetPlayerSide();

void Key_SetNetOn(boolean flg);
boolean Key_GetNetOn();

void Key_SetupSendKey();
// 操作を1フレーム分戻す
void Key_Return1Frame();
void Key_Return1VFrame();
void set_stopVB(boolean flg);

void Key_SetGKeyLog(BYTE gKeyLog[KEYLOG_MAX]);
void Key_SetSendKey(BYTE myKeyLog[KEYLOG_MAX]);
// ネット対戦キー設定準備
int Key_NetButtonUpdate();
// ネット対戦キー設定決定
int Key_NetButtonEnter();

// P1_Bの値セット
void setP1_B(int num, int value);
void setP2_B(int num, int value);

// 0.なし 1.再生 2.録画
int Replay_Mode(int num);
void Replay_End();
void Replay_Setting(int n1, int n2, int c1, int c2, int stageNum);
int Replay_ReserveKey();
void Replay_EnterSetting(int n1, int n2, int c1, int c2, int stageNum);
void Key_GetString(std::string st);
// リプレイの進行値を返す
int GetRTimeCount();

void InputDevice_Start();
int InputDevice_Update();
void InputDevice_Draw();

// セットされているボタンを返す
int Key_ReturnButton(int player, int button);
// 入力した値を割り当てる
void Key_SetButton(int player, int button, int input);
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
// 乱数 (999までの数で確率を指定)
boolean random(int num);
// 判定重なってるかチェック
bool Box_Touch(int x1, int y1, int w1, int h1,
	int x2, int y2, int w2, int h2);

//-------------
// Def
//-------------
void GetDef();
// ゲーム内のキャラ数を返す
int Character_Max();
// 登録番号の名前を返す
Name_t Character_Name(int num);

#endif