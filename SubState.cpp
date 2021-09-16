//==================================================================================
//
//			ステート準備の管理部
//
//==================================================================================

//インクルード部--------------------------------------------
#include "pch.h"
#include "SData.h"
using namespace stateData;

//デファイン部----------------------------------------------

// #include "Define.h" にて、関数をセット


//ﾃﾞｰﾀ定義部------------------------------------------------

//[ ダッシュ ]
typedef struct {
	int Image;		// 現在の画像
	float GSize;	// 描画サイズ
	int XPos, YPos;
	int XSize, YSize;
	int Num1, Num2;	// 画像番号 縦と横
	int Time;		// 時間

}Dash_t;

// エフェクトの変数用意
static Dash_t D[2];
static Player P[2];

// 大体ダッシュ関連
static int m_DashFlg[2];
static int m_dashTime[2];
static boolean m_dMuki[2];
static int m_KeyL[2], m_KeyR[2];

static int m_OldState[2];

static const int DASH_TIME = 13;

// ハイジャンプ猶予
static int hjFlg[2];


//内部関数宣言部--------------------------------------------

static void P_Dash();
static Player Neutral(Player p1);

//プログラム部----------------------------------------------

//データ操作系関数

//-------------------------------------------------------------------------------
//	定義　int CharMove( void )
//
//	説明　バトルモードの1P, 2Pの値を設定する、このモードの要
//
//	引数　int (可変)  引数の値で次の画面が決まる
//
//	戻り値　ないんじゃない？（適当）
//-------------------------------------------------------------------------------


//ステート番号決定
void EnterState()
{	
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){
		// 関数への代入用
		Player EP;
		if (i == 0)EP = P[1];
		else{ EP = P[0]; }

		/************
		* しゃがみ(11) or 立ち(0)判定
		************/
		P[i] = Neutral(P[i]);
	}
	P1 = P[0], P2 = P[1];
	

	/********
	* 振り向き
	********/

	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){
		// 相手
		Player EP = P[1];
		if (i == 1)EP = P[0];

		// １フレ前の向き記憶
		P[i].oldMuki = P[i].muki;

		// ダッシュ中なら振り向かない
		if ((P[i].DashFlg == 1) && (P[i].ctrl))P[i].Turn = false;
		else { P[i].Turn = true; }
		//else { P[i].Turn = true; }

		// 地上にいる、コントロール可能
		// 逆を向いていたら
		// 1P
		if ((P[i].SFlg != 2) && (P[i].ctrl) && (P[i].Turn) && (P[i].stateno < 1000)){
			if ((P[i].XPos < EP.XPos) && (P[i].muki == 1)){
				P[i].muki = 0;
				P[i].XVel = -P[i].XVel;
				if (P[i].SFlg == 1)P[i].stateno = 6;
				else{ P[i].stateno = 5; }
			}
			if ((P[i].XPos > EP.XPos) && (P[i].muki == 0)){
				P[i].muki = 1;
				P[i].XVel = -P[i].XVel;
				if (P[i].SFlg == 1)P[i].stateno = 6;
				else{ P[i].stateno = 5; }
			}
		}
	}
	P1 = P[0], P2 = P[1];

	/********
	* 歩き＆ダッシュ
	********/

	// ダッシュ設定
	P_Dash();
	{
		P[0] = P1, P[1] = P2;

		for (int i = 0; i < 2; i++){

			// ダッシュに変化するか判断
			//P[i].DashFlg = m_DashFlg[i];
			if (m_DashFlg[i] > 0)P[i].DashFlg = true;
			else{ P[i].DashFlg = false; }
		
			// 地上にいてコントロール可能、上下入力がない、ダッシュフラグがオフの時
			// 1P 
			if ((P[i].SFlg == 0) && (P[i].ctrl) && (P[i].button[102] == 0)){
				if (P[i].DashFlg){
					if (P[i].button[104] != 0){
						if (P[i].muki == 0){
							P[i].stateno = 35; // 左右を見て判断
							P[i].DashFlg = 0, m_DashFlg[i] = 0;	// バクステ停止
						}
						else {
							P[i].stateno = 30;
						}
					}
					if (P[i].button[106] != 0){
						if (P[i].muki == 0){
							P[i].stateno = 30;
						}
						else {
							P[i].stateno = 35; // 左右を見て判断
							P[i].DashFlg = 0, m_DashFlg[i] = 0;	// バクステ停止
						}
					}
				}
				else{
					if (P[i].button[104] != 0){
						if (P[i].muki == 0)P[i].stateno = 21; 	// 前後歩き
						else{ P[i].stateno = 20; }

					}
					if (P[i].button[106] != 0){
						if (P[i].muki == 0)P[i].stateno = 20; 	// 前後歩き
						else{ P[i].stateno = 21; }
					}
				}
			}

			// ダッシュフラグ中 & ダッシュじゃない & 時間切れなら中止
			if ((m_DashFlg[i] > 0) && m_dashTime[i] == 0 && P[i].stateno != 30){
				// おしっぱで消えない
				if (m_DashFlg[i] == 1 && m_KeyL[i] == 0)m_DashFlg[i] = 0;
				else if (m_DashFlg[i] == 2 && m_KeyR[i] == 0)m_DashFlg[i] = 0;
			}
			// ダッシュがステップなら
			if (P[i].stateno == 30 && P[i].SFlg == 2){
				m_DashFlg[i] = 0;
			}
			// ダッシュ中に他の行動したなら
			if ((P[i].stateno != 30) && (m_OldState[i] == 30)){
				P[i].DashFlg = 0, m_DashFlg[i] = 0;	// 停止
			}

			m_OldState[i] = P[i].stateno;

		}	// for終了
		P1 = P[0], P2 = P[1];
	}

	
	/********
	* ガード
	********/

	int dist1, dist2;
	dist1 = (int)P1.XPos - (int)P2.XPos;
	dist2 = (int)P2.XPos - (int)P1.XPos;

	// 1P 
	/*
	// 操作可能、相手が攻撃ステート、↓のは応急処置
	if ((P1.ctrl) && (P2.stateno >= 200) && (P2.stateno <= 999) &&
		((dist1 < P2.GuardDist) && (dist2 < P2.GuardDist))){
		// 向きとレバーがOKのとき
		if (((P1.XPos <= P2.XPos) && (P1_BInput(104) != 0)) ||
			((P1.XPos >= P2.XPos) && (P1_BInput(106) != 0))){
	
			if (P1.SFlg == 2){
				P1.stateno = 56;
				// 空中にいる
			}
			else if (P1_BInput(102) != 0){
				P1.stateno = 53;
				// + 下要素
			}

			else{
				P1.stateno = 50;
				// それ以外
			}
		}
	}
	

	// ヘルパーのガード
	for (int i = 0; i < HELPER_MAX; i++){
		if ((P1.ctrl) && ((H2[i].XPos - P1.XPos <= 150) &&
			(H2[i].XPos - P1.XPos >= -150))
			&& (H2[i].WAtt[0] > 0 && H2[i].HAtt[0] > 0)){
			// 向きとレバーがOKのとき
			if (((P1.XPos <= P2.XPos) && (P1_BInput(104) != 0)) ||
				((P1.XPos >= P2.XPos) && (P1_BInput(106) != 0))){

				if (P1.SFlg == 2){
					P1.stateno = 56;
					// 空中にいる
				}
				else if (P1_BInput(102) != 0){
					P1.stateno = 53;
					// + 下要素
				}

				else{
					P1.stateno = 50;
					// それ以外
				}
			}
		}
	}
	*/

	/********
	* ジャンプ
	********/
	Player P[2];
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){
		// 地に足が着いている場合のみ上入力を見る
		// 1P
		if ((P[i].ctrl) && (P[i].SFlg != 2) && (P[i].button[108] != 0) &&
			(P[i].stateno != 40)){
			P[i].stateno = 40;
		}

		// ハイジャンプ
		if ((hjFlg[i] > 0) && (P[i].StopTime == 0))hjFlg[i]--;
		if (P[i].button[102] == 1){
			hjFlg[i] = 9;
		}
		if ((hjFlg[i] > 0) && (P[i].SFlg != 2) && (P[i].button[108] != 0)){
			P[i].hyJump = true;
			//P[i].AirJump--;
		}

		// 二段ジャンプ、ジャンプ回数がある
		// 地上より少し高くないとできない
		if ((P[i].ctrl) && (P[i].SFlg == 2) && (P[i].button[108] == 1) &&
			(P[i].YPos < GROUND - 16) && (P[i].AirJump > 0)){	// 前制限10
			P[i].stateno = 45;
		}

		// 空ダ時間減少
		if (P[i].airDashTime > 0)P[i].airDashTime -= 1;
		
		// 空中ダッシュ
		// 前制限20
		if ((P[i].ctrl) && (P[i].SFlg == 2) && (P[i].DashFlg && P[i].K_Senkou[6] > 0) &&
			((P[i].YVel >= 0 && (P[i].YPos + (P[i].YVel * 2) < GROUND - 22)) || (P[i].YVel < 0 && P[i].YPos < GROUND - 134)) &&
			(P[i].airDash > 0)){
			P[i].stateno = 60;
			P[i].AirJump--, P[i].airDash--;
			P[i].airDashTime = 20;
			P[i].time = 0;
			P[i].DashFlg = 0, m_DashFlg[i] = 0;	//　ダッシュ停止
		}
		else if ((P[i].ctrl) && (P[i].SFlg == 2) && (P[i].DashFlg && P[i].keyAtt[4] > 0) &&// keyatt or senkou
			((P[i].YVel >= 0 && (P[i].YPos + (P[i].YVel * 2) < GROUND - 22)) || (P[i].YVel < 0 && P[i].YPos < GROUND - 134)) &&
			(P[i].airDash > 0)){
			P[i].stateno = 65; 
			P[i].AirJump--, P[i].airDash--;
			P[i].DashFlg = 0, m_DashFlg[i] = 0;	//　ダッシュ停止
			P[i].airDashTime = 20;
			P[i].time = 0;
		}

		// ジャンプ回数設定、2回まで
		if ((P[i].SFlg != 2) && (P[i].YPos == GROUND) && (P[i].stateno != 40)){
			P[i].AirJump = P[i].C.airJump;
			P[i].airDash = P[i].C.airDash;
			P[i].hyJump = false;
		}


		//  [ プッシュガード ]
		if ((P[i].button[4] > 0) &&  (P[i].ctrl) && (P[i].SFlg != 2)
			&& (P[i].aGauge > 0) && (P[i].keyAtt[4] > 0)) {
			//if(P[i].SFlg == 2)P[i].stateno = 59;
			if (P[i].keyAtt[2] > 0)P[i].stateno = 58;
			else { P[i].stateno = 57; }
			P[i].ctrl = 0;
			P[i].D.nokezori = 0;
		}
		
	}
	P1 = P[0], P2 = P[1];

	/********
	* 値をStateに渡す
	********/
	Get_PSet(P1, P2);
	Get_SSet(S);
	Get_HSet(H1, H2);
}

void EnterAttack()
{

	/********
	* 通常技（共通）
	********/

	P[0] = P1, P[1] = P2;
	Helper h[2][HM];
	for (int n = 0; n < HM; n++){
		h[0][n] = H1[n];
		h[1][n] = H2[n];
	}

	for (int i = 0; i < 2; i++){
		// 空ダ限定　先行入力
		if ((P[i].stateno == 60 || P[i].stateno == 65)){
			for (int n = 1; n < 5; n++){
				if (P[i].Senkou[n] > 0){
					P[i].rbutton[n] = 1;
					P[i].Senkou[n] = 5;
				}
			}
		}

		// 操作可能な時
		// 1P
		if (P[i].ctrl){
			if ((P[i].SFlg == 2) && (P[i].YPos < GROUND)){		// 空中
				// A.B.C を判断
				if (P[i].Senkou[3] > 0)P[i].stateno = 420;
				else if (P[i].Senkou[2] > 0)P[i].stateno = 410;
				else if (P[i].Senkou[1] > 0)P[i].stateno = 400;
			}
			else if ((P[i].SFlg != 2) && (P[i].button[102] > 0)){	// しゃがみ
				if (P[i].Senkou[3] > 0)P[i].stateno = 320;
				else if (P[i].Senkou[2] > 0)P[i].stateno = 310;
				else if (P[i].Senkou[1] > 0)P[i].stateno = 300;
			}
			else if ((P[i].SFlg != 2) && (P[i].button[102] == 0)){	// 立ち
				if (P[i].Senkou[3] > 0)P[i].stateno = 220;
				else if (P[i].Senkou[2] > 0)P[i].stateno = 210;
				else if (P[i].Senkou[1] > 0)P[i].stateno = 200;
			}
		}
		

		// ガーキャン
		if ((P[i].stateno == 50 || P[i].stateno == 51 || 
			 P[i].stateno == 53 || P[i].stateno == 54)
			//&& (P[i].aGauge >= (GUARD_MAX / 2)) 
			&& (P[i].Power >= 1000)
			&& (P[i].keyAtt[6] > 0)
			&& ((P[i].rbutton[1] > 0) && (P[i].rbutton[2] > 0))
			){
			P[i].stateno = 520;
			P[i].time = 0;
			P[i].ctrl = 0;
		}

	}
	P1 = P[0], P2 = P[1];
	
	
	for (int n = 0; n < HM; n++){
		H1[n] = h[0][n];
		H2[n] = h[1][n];
	}
	

	// 値を返す
	Get_PSet(P1, P2);
	Get_SSet(S);
	Get_HSet(H1, H2);
}

// ダッシュを可能にするフラグ
void P_Dash()
{

	Player GP[2];
	GP[0] = P1, GP[1] = P2;

	for (int i = 0; i < 2; i++){

		// 方向入力を判断
		{
			// 右入力＆上下要素がない
			if ((GP[i].button[106] != 0)
				&& ((GP[i].SFlg == 2) || (GP[i].keyAtt[2] == 0))){//&& (GP[i].keyAtt[8] == 0)){
				m_KeyR[i] += 1;
			}
			else{
				m_KeyR[i] = 0;
			}

			// 左
			if ((GP[i].button[104] != 0)
				&& ((GP[i].SFlg == 2) || (GP[i].keyAtt[2] == 0))){//&& (GP[i].keyAtt[8] == 0)){
				m_KeyL[i] += 1;
			}
			else{
				m_KeyL[i] = 0;
			}
		}

		// 入力猶予があって、 キー入力を開始した瞬間
		// ←
		if ((m_KeyL[i] == 1) && (m_dMuki[i] == 0) && (m_dashTime[i] > 0)){
			m_DashFlg[i] = 1;
		}
		// →
		if ((m_KeyR[i] == 1) && (m_dMuki[i] == 1) && (m_dashTime[i] > 0)){
			m_DashFlg[i] = 2;
		}


		// キー入力を終了して、ダッシュフラグがオンの時
		//if (((m_KeyR[i] == 0) && (m_KeyL[i] == 0)) && (m_DashFlg[i] = 1))m_DashFlg[i] = 0;

		// 左右どちらかに入力 & 受付猶予が0
		if ((m_KeyL[i] == 1) && (m_dashTime[i] == 0)){
			m_dashTime[i] = DASH_TIME;	// 受付猶予の設定
			m_dMuki[i] = 0;
		}
		if ((m_KeyR[i] == 1) && (m_dashTime[i] == 0)){
			m_dashTime[i] = DASH_TIME;	// 受付猶予の設定
			m_dMuki[i] = 1;
		}

		// ダッシュタイムを減らしていく
		if (m_dashTime[i] != 0)m_dashTime[i] -= 1;
	}
}

Player Neutral(Player p1){
	
	/*
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){
	// 地上にいてコントロール可能な時、2方向に入力
	// 違ったら立ちに
	// 1P
	if ((P[i].ctrl) && (P[i].SFlg != 2) && (P[i].Lock == 0)){
	// 下入力していて、ステート11じゃない
	if ((P[i].button[102] != 0) && (P[i].stateno != 11)){
	P[i].stateno = 10;
	}
	// ロック！中は立ちステートに戻らない
	else if ((P[i].button[102] == 0) && (P[i].SFlg == 0)){
	P[i].stateno = 0;
	}
	}
	}
	P1 = P[0], P2 = P[1];
	*/

	// 地上にいてコントロール可能な時、2方向に入力
	// 違ったら立ちに
	// 1P
	if ((p1.ctrl) && (p1.SFlg != 2) && (p1.Lock == 0)){
		// 下入力していて、ステート11じゃない
		if ((p1.button[102] != 0) && (p1.stateno != 11)){
			p1.stateno = 10;
		}
		// ロック！中は立ちステートに戻らない
		else if ((p1.button[102] == 0) && (p1.SFlg == 0)){
			p1.stateno = 0;
		}
	}

	return p1;
}

void SubState_DashReset()
{
	for (int i = 0; i < 2; i++){
		m_KeyR[i] = 0;
		m_KeyL[i] = 0;
		m_DashFlg[i] = 0;
		m_dashTime[i] = 0;
		m_dMuki[i] = 0;
		hjFlg[i] = 0;
	}
}

void GetP_SubState(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_SubState(System_t GS)
{
	S = GS;
}

void GetH_SubState(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}