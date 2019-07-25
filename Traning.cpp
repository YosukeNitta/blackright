//==================================================================================
//
//			数値計算の管理部
//
//========================================================================

//インクルード部--------------------------------------------
#include "pch.h"

//デファイン部----------------------------------------------

// #include "Define.h" にて、関数をセット



//ﾃﾞｰﾀ定義部------------------------------------------------


//内部関数宣言部--------------------------------------------
static const int REC_TIME = 600;
// 0.通常 1.準備 2.録画中 3.再生中
static int button[REC_TIME][8];
static int uButton[REC_TIME][8];
static int rTime;	// レコーディング時間
static int rTime_Max;	// レコーディングした時間

static boolean turn = false;	// レコーディング開始時の向き
static boolean reversal = false, oldHFlg = false;

static Player sP2;	// セットするP2
static int oldSwitch;	// 前のスイッチ
static int onCmd[20];

//プログラム部----------------------------------------------


//-------------------------------------------------------------------------------
//	定義　int CharMove( void )
//
//	説明　バトルモードの1P, 2Pの値を設定する、このモードの要
//
//	引数　int (可変)  引数の値で次の画面が決まる
//
//	戻り値　int メニューを決める変数
//-------------------------------------------------------------------------------


void SetTraning()
{
	//================================================
	// 体力
	if (S.T_Life == 0){
		if (P1.stateno < 1000 && (P1.ctrl)){
			if (P1.Life < P1.C.lifeMax){ P1.Life += 16; }
			if (P1.Life > P1.C.lifeMax)P1.Life = P1.C.lifeMax;
		}
		if (P2.stateno < 1000 && (P2.ctrl)){
			if (P2.Life < P2.C.lifeMax){ P2.Life += 16; }
			if (P2.Life > P2.C.lifeMax)P2.Life = P2.C.lifeMax;
		}

		P1.LifeC = LIFECOUNT_MAX, P2.LifeC = LIFECOUNT_MAX;	// ライフカウント常に最大
	}

	//================================================
	// ゲージ
	if (S.T_Power == 0){
		if ((P2.HFlg == 0) && (P1.stateno < 200)){
			if (P1.Power > POWER_MAX)P1.Power = POWER_MAX;
			else if (P1.Power < POWER_MAX){ P1.Power = POWER_MAX; }
		}
		if ((P1.HFlg == 0) && (P2.stateno < 200)){
			if (P2.Power > POWER_MAX)P2.Power = POWER_MAX;
			else if (P2.Power < POWER_MAX){ P2.Power = POWER_MAX; }
		}
	}
	else if (S.T_Power == 1){
		if ((P2.HFlg == 0) && (P1.stateno < 200) && (P1.ctrl)){
			if (P1.Power > 1000)P1.Power = 1000;
			else if (P1.Power < 1000){ P1.Power = 1000; }
		}
		if ((P1.HFlg == 0) && (P2.stateno < 200) && (P2.ctrl)){
			if (P2.Power > 1000)P2.Power = 1000;
			else if (P2.Power < 1000){ P2.Power = 1000; }
		}
	}
	else if (S.T_Power == 2){
		if ((P2.HFlg == 0) && (P1.stateno < 200) && (P1.ctrl)){
			if (P1.Power > 2000)P1.Power = 2000;
			else if (P1.Power < 2000){ P1.Power = 2000; }
		}
		if ((P1.HFlg == 0) && (P2.stateno < 200) && (P2.ctrl)){
			if (P2.Power > 2000)P2.Power = 2000;
			else if (P2.Power < 2000){ P2.Power = 2000; }
		}
	}

	//================================================
	// ガードゲージ
	if (S.T_GGauge == 0){
		if (P1.ctrl == 1){
			if (P1.aGauge <= 0)P1.aGauge = GUARD_MAX;
			if (P1.GRecovery != 0)P1.GRecovery = 0;
		}
		if (P2.ctrl == 1){
			if (P2.aGauge <= 0)P2.aGauge = GUARD_MAX;
			if (P2.GRecovery != 0)P2.GRecovery = 0;
		}
	}

	//================================================
	// アドゲージ
	if (S.T_AddGauge == 1){
		if (P1.ctrl == 1 && P2.ctrl == 1){
			P1.addG = ADD_MAX;
			P2.addG = ADD_MAX;
		}
	}

	//================================================
	// カウンター
	if (S.T_Counter == 1){
		if (P1.HitCount == 0 && P2.stateno < 1000){
			P2.D.counter = 1;
		}
		if (P2.HitCount == 0 && P1.stateno < 1000){
			P1.D.counter = 1;
		}
	}

	//================================================
	// 状態	（立屈空・プレイヤー・AI）

	// 操作を停止
	if (S.TSwitch[2] != 3)Stop_P2_B();

	if (S.TSwitch[2] == 0);	// 立ち
	else if (S.TSwitch[2] == 1){	// 屈
		P2_Auto(102);
	}
	else if (S.TSwitch[2] == 2){	// 空
		// 地上にいたらジャンプ
		if (P2.SFlg != 2)P2_Auto(108);
	}

	// AI
	if (S.TSwitch[2] == 4)S.AI = true;
	else{ S.AI = false; }

	

	//================================================
	// リセット
	if ((P1.button[8] == 1) && (P1.keyAtt[2] > 0) && (S.TSwitch[7] == 0)){
		// 位置決定
		if (P1.button[104] > 0)FirstPosSet(-1);
		else if (P1.button[106] > 0)FirstPosSet(1);
		else{ FirstPosSet(0); }
		ModeChange(GameScene(VersusScene));	// 初期化
	}

	//================================================
	//--------------------
	// ダミーのアクション
	//--------------------
	if ((S.TSwitch[2] < 3) && (P2.stateno != 1055))// 立屈空のとき
	{

	//[ 受け身 ]
		if (S.TSwitch[3] == 0){
			if ((P2.stateno >= 1030) && (P2.stateno < 1080)){
				P2_Auto(1);
			}
		}
		else if (S.TSwitch[3] == 1){
			if ((P2.stateno >= 1030) && (P2.stateno < 1080)){
				P2_Auto(1);
				if (P2.muki == 0)P2_Auto(106);
				else { P2_Auto(104); }
			}
		}
		else if (S.TSwitch[3] == 2){
			if ((P2.stateno >= 1030) && (P2.stateno < 1080)){
				P2_Auto(1);
				if (P2.muki == 0)P2_Auto(104);
				else { P2_Auto(106); }
			}
		}
	
	//[ ガード ]
	// 1.する
		/*
		if (S.TSwitch[4] == 1){
			
			if ((P1.stateno >= 200) && (P1.stateno <= 999) //&& (P1.WAtt[0] > 0 || P1.WAtt[1] > 0) 
				&& ((P2.XPos - P1.XPos < 300) &&
				(P1.XPos - P2.XPos < 300))){
				if (P1.XPos < P2.XPos)P2_Auto(106);
				else if (P1.XPos > P2.XPos)P2_Auto(104);

				if (P1.XPos == P2.XPos){
					if (P2.muki == 0)P2_Auto(104);
					else { P2_Auto(106); }
				}

				// 屈ガ条件
				if ((P1.stateno >= 300) && (P1.stateno <= 399))P2_Auto(102);
			}
			// 屈ガ条件
			if ((P2.stateno >= 54))P2_Auto(102);

			
			for (int i = 0; i < HELPER_MAX; i++){
				// ヘルパー
				if ((H1[i].var) && 
					(H1[i].HMoveHit > 0 && H1[i].A.damage > 0) &&
					(H1[i].WHit > 0)
					){
					((H1[i].XPos - P2.XPos < 105) &&
						(P2.XPos - H1[i].XPos < 105)) ||
					if (P1.XPos < P2.XPos)P2_Auto(106);
					else if (P1.XPos > P2.XPos)P2_Auto(104);

					if (P1.XPos == P2.XPos){
						if (P2.muki == 0)P2_Auto(104);
						else { P2_Auto(106); }
					}

				}
			}
			
		}
		*/
	
		//[ 投げ抜け ]
		// 1.する
		if (S.T_ThrowReject == 1){

			if ((P1.stateno == 505 || P1.stateno == 515) && P1.time <= 1){
				P2_Auto(1), 
				P2_Auto(2);
			}

		}
	//================================================
	//[ レコーディング ]
		// スイッチのオン・オフ
		if (P1_BInput(8) == 1){
			if ((P1.keyAtt[2] == 0)){
				//  録画準備
				if (S.TSwitch[7] == 0)S.TSwitch[7] = 1;
				// 録画へ
				else if (S.TSwitch[7] == 1){
					S.TSwitch[7] = 2;
				}
				else if (S.TSwitch[7] == 2){
					S.TSwitch[7] = 0;
					rTime_Max = rTime;
				}
				// 再生終了
				else if (S.TSwitch[7] == 3){
					S.TSwitch[7] = 0;
				}
				SEStart(36);
			}
		}

		// リバサ確認
		if ((oldHFlg && P2.HFlg == 0) && (S.TSwitch[8] == 2) && 
			(P2.stateno > 59 || P2.stateno < 50))
			reversal = true;

		// 再生、リバサでも自動で
		if (P1_BInput(9) == 1 || reversal){
			// 再生へ
			if (S.TSwitch[7] == 0){
				S.TSwitch[7] = 3;
				SEStart(36);
				//if (reversal);
			}
			// 基本に戻る
			else if (S.TSwitch[7] == 1){
				S.TSwitch[7] = 0;
				SEStart(36);
			}
			// 録画中止
			else if (S.TSwitch[7] == 2){
				S.TSwitch[7] = 0;
				// 数値初期化
				for (int i = 0; i < 8; i++){
					button[rTime][i] = 0;
					rTime_Max = 0;
				}
				SEStart(37);
			}
			// もう一度再生
			else if (S.TSwitch[7] == 3){
				rTime = 0;
				SEStart(36);
				//if (reversal);
			}

			// リバサフラグ終了
			reversal = false;
		}

		// 喰らい状態確認
		//oldHFlg = P2.HFlg;
		if ((P2.HFlg == 1) || (P2.stateno >= 50 && P2.stateno <= 52))oldHFlg = 1;
		else{ oldHFlg = 0; }
		// 2P操作開始
		if (S.TSwitch[7] == 1){
			P2Ctrl();
		}
		//else{ P2_AI(P1, P2); }

	}// ダミーのアクション終了

	// 再生
	if (S.TSwitch[7] == 3){

		// 最初に
		if (rTime == 0){
			// リバサなら
			if (S.TSwitch[8] == 2){
				// コマンド移す
				Traning_P2Command(onCmd);
				// キー4～7どれか入力
				for (int n = 0; n < REC_TIME; n++){
					// 初期化
					for (int i = 0; i < 8; i++){
						uButton[n][i] = 0;
					}
					for (int i = 4; i < 8; i++){
						if (button[n][i] > 0)uButton[rTime][i] = 1;
					}
				}
			}
			// それ以外
			else{
				for (int n = 0; n < REC_TIME; n++){
					for (int i = 0; i < 8; i++){
						uButton[n][i] = button[n][i];
					}
				}
				// 向き
				if (P2.muki != turn){
					// 数値初期化
					int hozon = 0;
					for (int i = 0; i < REC_TIME; i++){
						hozon = uButton[i][2];
						uButton[i][2] = uButton[i][3];
						uButton[i][3] = hozon;
					}
				}
			}
		}
		// 行動させる
		P2Ctrl3(uButton[rTime]);
		P2Ctrl3_2();

		// 再生終了
		if (rTime >= rTime_Max){
			//S.TSwitch[7] = 0;	// 元はこれ
			// 通常orリバサ
			if (S.TSwitch[8] == 0 || S.TSwitch[8] == 2 || reversal){
				S.TSwitch[7] = 0;
			}
			// 連続
			else if (S.TSwitch[8] == 1){
				rTime = -1;
				S.TSwitch[7] = 3;
			}
		}
	}

	// 録画
	if (S.TSwitch[7] == 2){

		// 向き
		if (rTime == 0){
			if (!P2.muki)turn = false;
			else{ turn = true; }
		}

		P2Ctrl();

		for (int i = 0; i < 4; i++){
			if (P2_BInput(i + 1) > 0){
				button[rTime][i + 4] = 1;
			}
			else{ button[rTime][i + 4] = 0; }
		}
		// 上下左右
		if (P2_BInput(108) > 0){
			button[rTime][0] = 1;
		}
		else{ button[rTime][0] = 0; }

		if (P2_BInput(102) > 0){
			button[rTime][1] = 1;
		}
		else{ button[rTime][1] = 0; }

		if (P2_BInput(104) > 0){
			button[rTime][2] = 1;
		}
		else{ button[rTime][2] = 0; }

		if (P2_BInput(106) > 0){
			button[rTime][3] = 1;
		}
		else{ button[rTime][3] = 0; }

		if (rTime == 0){
			for (int i = 0; i < 20; i++){
				onCmd[i] = 0;
			}
		}
		for (int i = 0; i < 20; i++){
			if (P2.cmd[i] > 0)onCmd[i] = 1;
		}
	}

	// 準備時はrTimeを0に
	if (S.TSwitch[7] == 0 || S.TSwitch[7] == 1){
		rTime = 0;
	}
	if (S.TSwitch[7] == 2 || S.TSwitch[7] == 3){
		rTime++;
	}
	// 限界時間でニュートラルに戻る
	if (rTime > REC_TIME - 1){
		rTime_Max = REC_TIME - 1;
		rTime = 0;
		S.TSwitch[7] = 0;
	}
	
	// トレモでの表示
	TestText(rTime, 0);

	// 録画終了時、前のPを記憶
	if (oldSwitch == 2 && S.TSwitch[7] != 2)sP2 = P2;
	// 記憶
	oldSwitch = S.TSwitch[7];

	

	//================================================
	// 返す
	Get_PSet(P1, P2);
	Get_SSet(S);
}


void GetP_Traning(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}


void GetS_Traning(System_t GS)
{
	S = GS;
}

void GetH_Traning(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}