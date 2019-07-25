//==================================================================================
//
//			ステート準備の管理部
//
//==================================================================================

//インクルード部--------------------------------------------
#include "pch.h"
#include <vector>
using namespace std;
//デファイン部----------------------------------------------

// #include "Define.h" にて、関数をセット


//データ定義部------------------------------------------------

static int PSide;
static boolean load_1;

class Command{
public:
	Command(){

	}
};

static vector<Command>charCmd;

//内部関数宣言部--------------------------------------------

typedef struct {
	// コマンド成立の可否
	int Cmd[20];
	// コマンド成立時間
	int cmdOK[20]; 
	// コマンド毎の受付時間
	int Time[20];
	// コマンド入力のフラグ
	int Flg[20];

}Command_t;

// コマンドの変数用意
static Command_t C[2];


//プログラム部----------------------------------------------

int InPAD(int num);
int InCOM(int num);

static boolean checkThrow();
static boolean check3Gauge();

static int Def_Cmd();
static int EinCmd();
static int Char2_Cmd();
static int Char3_Cmd();
static int Char4_Cmd();
static int Char5_Cmd();

//-------------------------------------------------------------------------------
//
// アインのコマンド技を準備する
// 特殊・必殺技
//
//-------------------------------------------------------------------------------

int CharCmd()
{
	if (!load_1){
		for (int i = 0; i < Character_Max(); i++){
			charCmd.push_back(Command());
		}
	}

	// コマンド入力を取得
	for (int i = 0; i < 20; i++){
		P1.cmd[i] = C[P1.PSide - 1].Cmd[i];
	}

	// キャラ毎のコマンド
	switch (P1.Name){
	case EIN:
		EinCmd();
		break;
	case CORNEL:
		Char2_Cmd();
		break;
	case BOUNCER:
		Char3_Cmd();
		break;
	case HELIOS:
		Char4_Cmd();
		break;
	case HYDE:
		Char5_Cmd();
		break;
	default:
		Def_Cmd();
		break;
	}

	// ステートの結果を返す
	if (P1.PSide == 1)Get_P1Set(P1);
	if (P1.PSide == 2)Get_P2Set(P1);

	return 0;
}

int EinCmd()
{

	/********
	/
	/ [Ｄアクション]
	/
	/*******/

	//  6B
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]) && (P1.Senkou[3] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 225;
		}
	}


	// Dアクション、Dを押していて操作可能
	if ((P1.ctrl) && (P1.button[4] == 1)){
		// 地上にいて、ヘルパー[2]が存在しない
		if ((!H1[2].var) && (P1.SFlg != 2)){
			P1.stateno = 620;
		}
		// 空中設置
		else if ((!H1[2].var) && (P1.SFlg == 2)){
			P1.stateno = 622;
		}
		// ヘルパー[2]がある
		if (H1[2].var){
			if (P1.SFlg != 2){
				P1.stateno = 625;
			}
			else if (P1.SFlg == 2){
				P1.stateno = 626;
			}
		}
	}
	

	/********
	/
	/ [必殺技・ゲージ技・投げ]
	/
	/*******/
	// [ブーメラン投げ]
	// ヘルパー１を出していない
	if ((P1.ctrl) && (InCOM(1)) && (!H1[1].var)){
		if (P1.SFlg != 2){		// 地上
			// ABC全て
			if (P1.Senkou[1] > 0 || P1.Senkou[2] > 0 || P1.Senkou[3] > 0){
				P1.stateno = 600;
				if (P1.Senkou[1] > P1.Senkou[2]){
					if (P1.Senkou[1] > P1.Senkou[3])P1.Var[6] = 1;
					else{ P1.Var[6] = 3; }
				}
				else if (P1.Senkou[2] > P1.Senkou[3])P1.Var[6] = 2;
				else{ P1.Var[6] = 3; }
			}
		}
		else if (P1.SFlg == 2){	// 空中
			// ABC全て
			if (P1.Senkou[1] > 0 || P1.Senkou[2] > 0 || P1.Senkou[3] > 0){
				P1.stateno = 605;
				if (P1.Senkou[1] >= P1.Senkou[2]){
					if (P1.Senkou[1] >= P1.Senkou[3])P1.Var[6] = 1;
					else{ P1.Var[6] = 3; }
				}
				else if (P1.Senkou[2] >= P1.Senkou[3])P1.Var[6] = 2;
				else{ P1.Var[6] = 3; }
			}
		}
	}
	// 操作可能時に 214 を入力
	// [ 凪払い ]
	if ((P1.ctrl) && (InCOM(2))){
		if (P1.SFlg != 2){		// 地上
			if (P1.Senkou[1] > 0)P1.stateno = 610;
			if (P1.Senkou[2] > 0)P1.stateno = 611;
		}
	}
	//[EX技]
	// [大ブーメラン]
	if ((P1.ctrl) && (InCOM(1)) && (P1.Power >= 500)){
		if (P1.SFlg != 2){		// 地上
			if (P1.Senkou[4] > 0)P1.stateno = 700;
		}
	}

	// [ 二段切り ]
	if ((P1.ctrl) && (InCOM(2)) && (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// 地上
			if (P1.Senkou[3] > 0)P1.stateno = 800;
		}	
		else if (P1.SFlg == 2){		// 地上
			if (P1.Senkou[3] > 0)P1.stateno = 830;
		}
	}
	
	// [ 真空ブーメラン ]
	if ((P1.ctrl) && (InCOM(3)) && (P1.Power >= 1000) 
		&& (H1[2].var) && (H1[2].stateno == 2020)){
		if (P1.SFlg != 2){		// 地上
			if (P1.button[4] > 0)P1.stateno = 820;
		}
	}

	// EXアクション
	// A..ゲージ増加
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Var[30] == 0)){
		if (P1.SFlg != 2){		// 地上
			if (P1.button[1] > 0)P1.stateno = 550;	// ゲージ増加
			if (P1.button[3] > 0)P1.stateno = 552;	// 分身
		}
	}
	// B..バースト
	// 停止時間が0、1060～1069ではない
	if ((P1.cmd[3]) && (P1.Var[30] == 0) && (P1.StopTime == 0) &&
		(P1.stateno < 1060 || P1.stateno >= 1070) && (P1.Life > 0) &&
		(P1.stateno >= 1000) && (P2.stateno < 800 || P2.stateno >= 900)){
			{		// どこでも
				if (P1.button[2] > 0)P1.stateno = 551;	// ゲージ増加
			}
	}

	//  投げ
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}

	//  [ ３ゲージ ]
	if (check3Gauge()){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 850;	// 超必
		}
	}

	// コマンドを記憶


	return 0;
}


// コーネル
int Char2_Cmd()
{
	
	/********
	/
	/ [Ｄアクション・特殊技]
	/
	/*******/
	// Dアクション、Dを押していて操作可能
	if ((P1.ctrl) && (P1.Senkou[4] > 0)){
		// 地上にいる
		if (P1.SFlg != 2){
			if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]))P1.stateno = 603;
			else if (P1.keyAtt[2])P1.stateno = 604;
			else{ P1.stateno = 600; }
		}
		// 空中キック
		else if (P1.SFlg == 2 && (P1.YPos < GROUND - 5)){
			P1.stateno = 430;
		}
	}

	//  6B
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]) && (P1.Senkou[2] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 215;
		}
	}

	//  3C
	if ((P1.keyAtt[6]) && (P1.keyAtt[2]) && (P1.Senkou[3] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 321;
		}
	}


	/********
	/
	/ [必殺技・ゲージ技・投げ]
	/
	/*******/

	// [スピナー]
	if ((P1.ctrl) && (P1.cmd[1]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 630;
		}
	}
	// [スピナー]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 610;
		}
	}
	// [アッパー]
	if ((P1.ctrl) && (P1.cmd[4]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 620;
		}
	}
	// [エンハ]
	/*
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Senkou[4] > 0)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 650;
		}
	}
	*/

	//==============
	// ゲージと投げ
	//==============
	
	// [ 打ち上げ ]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[4] > 0) && (P1.Power >= 500)){
		if (P1.SFlg != 2){		// どこでも
			P1.stateno = 700;
		}
	}
	// [ スマッシュ ]
	if ((P1.ctrl) && (P1.cmd[1]) && (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// 地上
			if (P1.Senkou[3] > 0)P1.stateno = 800;
		}
	}
	// [ ウィングアッパー ]
	if ((P1.ctrl) && (P1.cmd[4]) && (P1.Power >= 1000)){
		{		// どこでも
			if (P1.Senkou[3] > 0)P1.stateno = 820;
		}
	}
	// [ ハイドロストーム ]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Power >= 2000) && (!H1[1].var)){
		if (P1.SFlg != 2){		// 地上
			if (P1.Senkou[3] > 0)P1.stateno = 810;
		}
	}

	// EXアクション
	// A..ゲージ増加
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Var[30] == 0)){
		if (P1.SFlg != 2){		// 地上
			if (P1.button[1] > 0)P1.stateno = 550;	// ゲージ増加
			if (P1.button[3] > 0)P1.stateno = 552;	// 分身
		}
	}
	// B..バースト
	// 停止時間が0、1060～1069ではない
	if ((P1.cmd[3]) && (P1.Var[30] == 0) && (P1.StopTime == 0) &&
		(P1.stateno < 1060 || P1.stateno >= 1070) && (P1.Life > 0) &&
		(P1.stateno >= 1000) && (P2.stateno < 800 || P2.stateno >= 900)){
			{		// どこでも
				if (P1.button[2] > 0)P1.stateno = 551;	// ゲージ増加
			}
	}

	//  投げ
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}

	//  [ ３ゲージ ]
	if (check3Gauge()){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 850;	// 超必
		}
	}


	return 0;
}

// バウンサー
int Char3_Cmd()
{

	/********
	/
	/ [Ｄアクション・特殊技]
	/
	/*******/
	//  6B
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]) && (P1.Senkou[2] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 215;
		}
	}
	//  6B
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]) && (P1.Senkou[3] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 225;
		}
	}
	//  3C
	if ((P1.keyAtt[6]) && (P1.keyAtt[2]) && (P1.Senkou[3] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 235;
		}
	}
	//  J2C
	if ((P1.keyAtt[2]) && (P1.Senkou[3] > 0) && (P1.ctrl)){
		if ((P1.SFlg == 2)){		// 地上
			P1.stateno = 430;
		}
	}
	// Dアクション、Dを押していて操作可能
	if ((P1.ctrl) && (P1.Senkou[4] > 0)){
		// 地上にいる
		if (P1.SFlg != 2){
			P1.stateno = 600;
		}
	}
	/********
	/
	/ [必殺技・ゲージ技・投げ]
	/
	/*******/
	// [グランド]
	if ((P1.ctrl) && (P1.cmd[1]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 620;
		}
	}
	// [スピナー]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 610;
		}
	}
	

	//[EX技]
	// [大ブーメラン]
	if ((P1.ctrl) && (P1.cmd[1]) && (P1.Power >= 500)){
		if (P1.SFlg != 2){		// 地上
			if (P1.Senkou[3] > 0)P1.stateno = 625;
		}
	}
	// [0.5投げ]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[3] > 0) 
		&& (P1.Power >= 500)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 610;
			P1.Var[15] = 1;
		}
	}
	

	// EXアクション
	// A..ゲージ増加
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Var[30] == 0)){
		if (P1.SFlg != 2){		// 地上
			if (P1.button[1] > 0)P1.stateno = 550;	// ゲージ増加
			if (P1.button[3] > 0)P1.stateno = 552;	// 分身
		}
	}
	// B..バースト
	// 停止時間が0、1060～1069ではない
	if ((P1.cmd[3]) && (P1.Var[30] == 0) && (P1.StopTime == 0) &&
		(P1.stateno < 1060 || P1.stateno >= 1070) && (P1.Life > 0) &&
		(P1.stateno >= 1000) && (P2.stateno < 800 || P2.stateno >= 900)){
			{		// どこでも
				if (P1.button[2] > 0)P1.stateno = 551;	// ゲージ増加
			}
	}

	//  投げ
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}

	//  [ １ゲージ ]
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.Senkou[3] > 0) && (P1.ctrl)
		&& (P1.Power >= 1000 || P1.Var[13] > 0)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 800;	// 超必
		}
	}

	//  [ ３ゲージ ]
	//  [ １ゲージ ]
	if ((P1.Senkou[4] > 0) && (P1.cmd[3]) && (P1.ctrl)
		&& (P1.Power >= 3000)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 850;	// 超必
		}
	}


	return 0;
}

// ヘリオス
int Char4_Cmd()
{

	/********
	/
	/ [Ｄアクション・特殊技]
	/
	/*******/
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (P1.Senkou[1] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 205;
		}
	}
	//  6B
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (P1.Senkou[2] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 215;
		}
	}
	// Dアクション、Dを押していて操作可能
	if ((P1.ctrl) && (P1.Senkou[4] > 0 || P1.button[4] > 0) && (P1.Var[11] < 3000)){
		// 地上にいる
		if (P1.SFlg != 2){
			P1.stateno = 600;
		}
	}
	/********
	/
	/ [必殺技・ゲージ技・投げ]
	/
	/*******/
	// [ブーメラン投げ]
	// ヘルパー１を出していない
	if ((P1.ctrl) && (P1.cmd[1]) && (!H1[0].var)){
		if (P1.SFlg != 2){		// 地上
			if (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)P1.stateno = 630;
		}
	}
	if ((P1.ctrl) && (P1.cmd[1]) && (!H1[0].var) && (P1.Var[13] == 0)){
		if (P1.SFlg != 2){		// 地上
			if (P1.Senkou[4] > 0){
				// 2本以上
				if (P1.Var[11] >= 2000)
					P1.stateno = 635;
				else{ P1.stateno = 630; }
			}
		}
	}

	// [214]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0 || P1.Senkou[3] > 0)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 610;
			if (P1.Senkou[2] > 0)P1.stateno = 611;
		}
		if (P1.SFlg == 2){
			if (P1.YPos < GROUND - 20)P1.stateno = 616;
		}
	}

	// [２１４Ｄ]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[4] > 0)){
		if (P1.SFlg != 2){		// 地上
			if (P1.Var[11] >= 1000)
				P1.stateno = 615;
			else{ 
				P1.stateno = 611;
			}
		}
		if (P1.SFlg == 2){
			if (P1.YPos < GROUND - 20){
				if (P1.Var[11] >= 1000)
					P1.stateno = 617;
				else{ P1.stateno = 616; }
			}
		}
	}

	// [６２３ＡorＢorＣ]
	if ((P1.ctrl) && (P1.cmd[4]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0 || P1.Senkou[3] > 0)){
		if (P1.SFlg != 2){
			P1.stateno = 620;
		}
	}
	// [６２３Ｄ]
	if ((P1.ctrl) && (P1.cmd[4]) && (P1.Senkou[4] > 0)){
		if (P1.SFlg != 2){		// 地上
			if(P1.Var[11] >= 1000)
				P1.stateno = 625;
			else{ P1.stateno = 620; }
		}
	}

	// [ 1ゲージ ]
	if ((P1.ctrl) && (P1.cmd[1]) && (P1.Power >= 1000) && (!H1[2].var)){
		if (P1.SFlg != 2){		// 地上
			if (P1.Senkou[3] > 0)P1.stateno = 820;
		}
	}
	// [ 1ゲージ ]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// 地上
			if (P1.Senkou[3] > 0)P1.stateno = 800;
		}
	}
	// [ 22D ]
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// 地上
			if (P1.Senkou[4] > 0)P1.stateno = 830;
		}
	}


	//  投げ
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}

	//  [ ３ゲージ ]
	if (check3Gauge()){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 850;	// 超必
		}
	}

	return 0;
}

// ハイド
int Char5_Cmd()
{

	/********
	/
	/ [Ｄアクション・特殊技]
	/
	/*******/
	// Dアクション、Dを押していて操作可能
	if (P1.ctrl){
		if (P1.Senkou[4] > 0){
			// 地上にいる
			if (P1.SFlg != 2){
				if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]))P1.stateno = 605;	// 6D
				else if (P1.keyAtt[2])P1.stateno = 610;	// 2D
				else{ P1.stateno = 600; }	// 5D
			}
			// 空中キック
			else if (P1.SFlg == 2 && (P1.YPos < GROUND - 5)){
				if (P1.keyAtt[2])P1.stateno = 630;	// J2D
				else{P1.stateno = 620;}	// JD
			}
		}
	}

	/********
	/
	/ [必殺技・ゲージ技・投げ]
	/
	/*******/

	// EXアクション
	// A..ゲージ増加
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Var[30] == 0)){
		if (P1.SFlg != 2){		// 地上
			if (P1.button[1] > 0)P1.stateno = 550;	// ゲージ増加
			if (P1.button[3] > 0)P1.stateno = 552;	// 分身
		}
	}
	// B..バースト
	// 停止時間が0、1060～1069ではない
	if ((P1.cmd[3]) && (P1.Var[30] == 0) && (P1.StopTime == 0) &&
		(P1.stateno < 1060 || P1.stateno >= 1070) && (P1.Life > 0) &&
		(P1.stateno >= 1000) && (P2.stateno < 800 || P2.stateno >= 900)){
			{		// どこでも
				if (P1.button[2] > 0)P1.stateno = 551;	// ゲージ増加
			}
	}

	//  投げ
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}
	/*
	//  [ ３ゲージ ]
	if (check3Gauge()){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 850;	// 超必
		}
	}
	*/
	return 0;
}

// 汎用
int Def_Cmd()
{

	/********
	/
	/ [Ｄアクション・特殊技]
	/
	/*******/

	/********
	/
	/ [必殺技・ゲージ技・投げ]
	/
	/*******/
	//  投げ
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}
	//  [ ３ゲージ ]
	if (check3Gauge()){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 850;	// 超必
		}
	}

	return 0;
}


int Ein_ComSet()
{
	int i;

	if (P1.PSide == 1)i = 0;
	if (P1.PSide == 2)i = 1;

	// 入力スタート
	// 1( 236 )
	{
		// 2 方向
		if ((P1.keyPos == 2)){
			if (C[i].Flg[1] == 0)
				C[i].Flg[1] = 1;
				// 入力スタート
				C[i].Time[1] = 17;
		}
		// 3 方向
		if ((C[i].Time[1] != 0) && (C[i].Flg[1] == 1)){
			if (P1.keyPos == 3)C[i].Flg[1] = 2;
		}
		// 6 方向
		if ((C[i].Time[1] != 0) && (C[i].Flg[1] == 2)){
			if (P1.keyPos == 6)C[i].Flg[1] = 3;
		}
		// 236 成立
		if (C[i].Flg[1] == 3){
			C[i].Flg[1] = 0;
			C[i].cmdOK[1] = C[i].Time[1];
		}
	}

	// 2( 214 )
	{
		// 2 方向
		if ((P1.keyPos == 2)){
			if (C[i].Flg[2] == 0)
				C[i].Flg[2] = 1;
			// 入力スタート
			C[i].Time[2] = 17;
		}
		// 1 方向
		if ((C[i].Time[2] != 0) && (C[i].Flg[2] == 1)){
			if (P1.keyPos == 1)C[i].Flg[2] = 2;
		}
		// 4 方向
		if ((C[i].Time[2] != 0) && (C[i].Flg[2] == 2)){
			if (P1.keyPos == 4)C[i].Flg[2] = 3;
		}
		// 214 成立
		if (C[i].Flg[2] == 3){
			C[i].Flg[2] = 0;
			C[i].cmdOK[2] = C[i].Time[2];
		}
	}

	// 3( 22 )
	// 2 方向
	// 最初の２入力のみ
	if ((P1.button[102] == 2) && (P1.keyPos == 2)){
	
		if (C[i].Flg[3] == 0)
			C[i].Flg[3] = 1;
		// 入力スタート
		C[i].Time[3] = 20;
	}
	// 2 方向
	if ((C[i].Time[3] != 0) && (C[i].Flg[3] == 1)){
		if ((P1.keyPos == 2) && (P1.button[102] == 1))
			C[i].Flg[3] = 2;
	}
	// 22 成立
	if (C[i].Flg[3] == 2){
		C[i].Flg[3] = 0;
		C[i].cmdOK[3] = C[i].Time[3];
	}


	// 4( 623 )
	{
		// 6 方向
		if ((P1.keyPos == 6)){
			if (C[i].Flg[4] == 0)
				C[i].Flg[4] = 1;
				// 入力スタート
				C[i].Time[4] = 17;
		}
		// 2 方向
		if ((C[i].Time[4] != 0) && (C[i].Flg[4] == 1)){
			if (P1.keyPos == 2)C[i].Flg[4] = 2;
		}
		// 3 方向
		if ((C[i].Time[4] != 0) && (C[i].Flg[4] == 2)){
			if (P1.keyPos == 3)C[i].Flg[4] = 3;
		}
		// 623 成立
		if (C[i].Flg[4] == 3){
			C[i].Flg[4] = 0;
			C[i].cmdOK[4] = C[i].Time[4];
		}
	}

	

	//--------------
	// 時間カウント
	//--------------
	for (int j = 0; j < 20; j++){
		//--------------
		// 猶予増加
		//--------------
		// 喰らい・硬直
		if ((C[i].cmdOK[j] == C[i].Time[j]) && (C[i].cmdOK[j] > 0)){
			if (P1.HFlg == 1)C[i].cmdOK[j] += 15;
			else if ((P1.stateno >= 200) && (P1.stateno <= 999))C[i].cmdOK[j] += 5;
		}

		// 入力猶予
		if (C[i].Time[j] != 0){
			C[i].Time[j] -= 1;
		}
		// コマンド完成
		if (C[i].cmdOK[j] != 0){
			C[i].cmdOK[j] -= 1;
		}
		if (C[i].Time[j] == 0){
			C[i].Flg[j] = 0;
		}
		// コマンドのオン・オフ
		if (C[i].cmdOK[j] > 0){
			C[i].Cmd[j] = 1;
		}
		else{ C[i].Cmd[j] = 0; }
	}
	



	return 0;
}


// コマンドのフラグを確認
int Ein_ComCheck(int num)
{
	int n, i;
	if (P1.PSide == 1)i = 0;
	if (P1.PSide == 2)i = 1;

	n = C[i].Cmd[num];

	

	return n;
}


//-------------------------------------------------------------------------------
//
// 変数の受け取り
//
//-------------------------------------------------------------------------------

void GetP_EinCmd(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetPH_EinCmd(Player GP1)
{
	P1 = GP1;
}

void GetS_EinCmd(System_t GS)
{
	S = GS;
}


void GetH_EinCmd(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}

//-------------------------------------------------------------------------------
//
// 入力確認
//
//-------------------------------------------------------------------------------

int InPAD(int num)
{
	int i;

	if (P1.PSide == 1)
		i = P1_BInput(num);

	if (P1.PSide == 2)
		i = P2_BInput(num);

	return i;
}

int InCOM(int num)
{
	return Ein_ComCheck(num);
}

void Traning_P2Command(int command[20])
{
	for (int i = 0; i < 20; i++){
		if (command[i] > 0){
			C[1].Cmd[i] = 1;
			C[1].cmdOK[i] = 5;
		}
	}
}

static boolean checkThrow()
{
	// 投げ
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) &&
		(P1.ctrl || ((P1.stateno >= 200 && P1.stateno < 500) && P1.time <= 1 && P2.HFlg == 0 &&
		(P2.stateno < 50 && P2.stateno > 52)))){
		return true;
	}
	return false;
}

static boolean check3Gauge()
{
	//  [ ３ゲージ ]
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.Senkou[3] > 0) && (P1.ctrl)
		&& (P1.Power >= 3000)){
		return true;
	}
	return false;
}