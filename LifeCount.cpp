//==================================================================================
//
//			数値計算の管理部
//
//========================================================================

//インクルード部--------------------------------------------
#include "pch.h"


//デファイン部----------------------------------------------





//ﾃﾞｰﾀ定義部------------------------------------------------

static int Load_1;
static boolean Load_1data = false;
static int mOldPower[2];
static int oldAdd[2];
static int mPlaySnd;
static Player mP[2];

static int time = 0;
static int recoverTime[2];
//内部関数宣言部--------------------------------------------

//パワー量の設定
void PowerSet();


//プログラム部----------------------------------------------


//-------------------------------------------------------------------------------
//	定義　void LifeCount( void )
//
//	説明　ライフカウンターの設定を行う
//
//	引数　void
//
//	戻り値　void
//-------------------------------------------------------------------------------

void LifeCount()
{
	// 一度のみロード
	if (!Load_1data) {
		recoverTime[0] = 0;
		recoverTime[1] = 0;
		Load_1data = true;
	}

	// ゲージ管理
	PowerSet();
	if (P1.Life <= 0){
		P1.Life = 0;
		P1.ctrl = 0;
		// stateがおかしかったら変更
		if (P1.stateno < 1000)P1.stateno = 1030;
	}
	if (P2.Life <= 0){
		P2.Life = 0;
		P2.ctrl = 0;
		if (P2.stateno < 1000)P2.stateno = 1030;
	}


	// どちらかがやられたら
	if (((P1.Life <= 0) || (P2.Life <= 0)) && (Load_1 == 0)){
		S.StopTime = 20;
		S.roundState = 2;
		S.roundTime = 0;
		// 1P
		if (P1.Life <= 0){
			P1.LifeC -= 1;
			P1.Life = 0;
		}
		// 2P
		if (P2.Life <= 0){
			P2.LifeC -= 1;
			P2.Life = 0;
		}


		for (int i = 0; i < PAD_MAX; i++){
			P1.button[i] = 0;
			P2.button[i] = 0;
		}

		// 決着がついた
		if (P1.LifeC == 0 || P2.LifeC == 0){
			if (S.roundState != 3){
				S.roundState = 3;
				if (P1.LifeC == 0)VOStart(P1.Name, 35, 0);
				if (P2.LifeC == 0)VOStart(P2.Name, 35, 0);
			}
		}
		// まだ終わってない
		else{
			// ヘルパー削除
			for (int i = 0; i < HM; i++){
				H1[i].var = false;
				H1[i].A.damage = 0;
				H2[i].var = false;
				H2[i].A.damage = 0;

				for (int j = 0; j < HIT_MAX; j++){
					H1[i].XAtt[j] = -1;
					H1[i].YAtt[j] = -1;
					H1[i].WAtt[j] = -1;
					H1[i].HAtt[j] = -1;

					H2[i].XAtt[j] = -1;
					H2[i].YAtt[j] = -1;
					H2[i].WAtt[j] = -1;
					H2[i].HAtt[j] = -1;
				}
			}
		}
		// ロードを停止
		Load_1 = 1;
	}

	// 終了アクション
	/*
	// 勝利ポーズになったら、対戦時にキャラセレに戻る
	if (P1.stateno == 95){
		if (P1.time >= 80)
		{
			S.roundState = 0;
			S.roundTime = 0;
			Load_1 = 0;
			CharLoad();
			if (!S.TraningFlg)ModeChange(2);
			else{
				ModeChange(3);
			}
		}
	}
	if (P2.stateno == 95){
		if (P2.time >= 80)
		{
			S.roundState = 0;
			S.roundTime = 0;
			Load_1 = 0;
			CharLoad();
			if (!S.TraningFlg)ModeChange(2);
			else{
				ModeChange(3);
			}
		}
	}
	*/
	// 即しボタン
	if (CheckHitKey(KEY_INPUT_F3) != 0)P2.Life = 0;

	Get_PSSet(P1, P2, S);
	Get_HSet(H1, H2);
}

void GetPS_LifeCount(Player GP1, Player GP2, System_t GS)
{
	P1 = GP1;
	P2 = GP2;
	S = GS;

	
}

void GetH_LifeCount(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}
}

void PowerSet()
{
	Player P[2];
	P[0] = P1, P[1] = P2;

	for (int i = 0; i < 2; i++){
		//--------------
		// パワー量制限
		//--------------
		// 最大値
		if (P[i].Power > POWER_MAX){
			P[i].Power = POWER_MAX;
		}
		// 最低値
		if (P[i].Power < 0){
			P[i].Power = 0;
		}

	}
	P1 = P[0], P2 = P[1];

	//------------------------
	// １ゲージ増加のサウンド
	//------------------------
	// 計算用P変数
	/*
	mP[0] = P1, mP[1] = P2;

	// 1・2P
	for (int p = 0; p < 2; p++){
		// パワーがMAX
		if ((mOldPower[p] < POWER_MAX) &&
			(mP[p].Power == POWER_MAX)){
			mPlaySnd = 1;
		}
	}
	if (mPlaySnd != 0){
		SEStart(24);
		mPlaySnd = 0;
	}
	// パワー記憶
	mOldPower[0] = P1.Power;
	mOldPower[1] = P2.Power;
	*/

	//-------------
	// ガードリカバリ
	//-------------
	
	// アド減らす
	if (time <= 120){
		time++;
	}
	else{ time = 1; }

	// 個人での時間計測
	P[0] = P1, P[1] = P2;
	for (int side = 0; side < 2; side++) {
		// 喰らいではない
		// ガードでもない
		if ((P[side].HFlg == 0) &&
			(P[side].stateno < 50 || P[side].stateno > 59)) {
			// 時間経過
			recoverTime[side]++;
			// ガードゲージ
			// 一定時間で回復
			if (recoverTime[side] % 10 == 0) {
				if (P[side].aGauge > 0) {
					P[side].aGauge += 2;
				}
			}
			// 1フレでリセット
			if (recoverTime[side] > 60)
				recoverTime[side] = 1;
		}
	}
	P1 = P[0], P2 = P[1];

	// ガークラ
	// 1フレ前はゲージがあった
	if (P1.aGauge <= 0 && oldAdd[0] > 0){
		SEStart(33);
		S.StopTime = 15;
		S.Anten = 15;
		P2.button[1] = 0;
		P2.button[2] = 0;
		P2.button[3] = 0;
		P2.button[4] = 0;
		P2.Senkou[1] = 0;
		P2.Senkou[2] = 0;
		P2.Senkou[3] = 0;
		P2.Senkou[4] = 0;
		P2.rbutton[1] = 0;
		P2.rbutton[2] = 0;
		P2.rbutton[3] = 0;
		P2.rbutton[4] = 0;
	}
	if (P2.aGauge <= 0 && oldAdd[1] > 0){
		SEStart(33);
		S.StopTime = 15;
		S.Anten = 15;
		P1.button[1] = 0;
		P1.button[2] = 0;
		P1.button[3] = 0;
		P1.button[4] = 0;
		P1.Senkou[1] = 0;
		P1.Senkou[2] = 0;
		P1.Senkou[3] = 0;
		P1.Senkou[4] = 0;
		P1.rbutton[1] = 0;
		P1.rbutton[2] = 0;
		P1.rbutton[3] = 0;
		P1.rbutton[4] = 0;
	}
	oldAdd[0] = P1.aGauge, oldAdd[1] = P2.aGauge;

	if (time % 10 == 0){
		if (P1.aGauge <= 0){
			P1.GRecovery+=20;
		}
		if (P2.aGauge <= 0){
			P2.GRecovery+=20;
		}
	}
	
	// ガードゲージ
	/*
	// 一定時間で回復
	if (recoverTime[0] % 10 == 0) {
		if (P1.aGauge > 0) {
			P1.aGauge += 2;
		}
	}
	if(recoverTime[1] % 10 == 0){
		if (P2.aGauge > 0) {
			P2.aGauge += 2;
		}
	}
	*/
	/*
	if (time % 60 == 0){
		if (P1.aGauge > 0){
			P1.aGauge += 5;
		}
		if (P2.aGauge > 0){
			P2.aGauge += 5;
		}
	}
	*/

	// ガードゲージ復活
	if (P1.GRecovery >= GUARD_MAX){
		P1.aGauge = GUARD_MAX;
		P1.GRecovery = 0;
	}
	if (P2.GRecovery >= GUARD_MAX){
		P2.aGauge = GUARD_MAX;
		P2.GRecovery = 0;
	}

	// 最低値
	if (P1.aGauge < 0){
		P1.aGauge = 0;
	}
	if (P2.aGauge < 0){
		P2.aGauge = 0;
	}

	// 最高値
	if (P1.aGauge > GUARD_MAX){
		P1.aGauge = GUARD_MAX;
	}
	if (P2.aGauge > GUARD_MAX){
		P2.aGauge = GUARD_MAX;
	}

	//=======
	// アドゲージ版
	//=======
	if (time % 40 == 0){
		if (P1.addG > 0){
			P1.addG --;
		}
		if (P2.addG > 0){
			P2.addG --;
		}
	}

	// 最低値
	if (P1.addG < 0){
		P1.addG = 0;
	}
	if (P2.addG < 0){
		P2.addG = 0;
	}

	// 最高値
	if (P1.addG > ADD_MAX){
		P1.addG = ADD_MAX;
	}
	if (P2.addG > ADD_MAX){
		P2.addG = ADD_MAX;
	}

}

void load_LifeCount()
{
	Load_1 = 0;
}