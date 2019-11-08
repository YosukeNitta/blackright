//==================================================================================
//
//			数値計算の管理部
//
//==================================================================================
#pragma warning(error:4456)

//インクルード部--------------------------------------------
#include "pch.h"

//デファイン部----------------------------------------------


//ﾃﾞｰﾀ定義部------------------------------------------------

static double hosei;
static int getPower;	// ゲージ回収、多すぎると減少
static int gTime;	// ガーポ、当身の保存時間

//内部関数宣言部--------------------------------------------

static void GuardPoint(int i);
static boolean GuardCheck(int n);
static void GuardParam(int i);
static void HitParam(int i);

//プログラム部----------------------------------------------

void HelperDamCheck()
{

	// 1P攻撃
	for (int i = 0; i < HM; i++){
		for (int j = 0; j < ATT_MAX; j++){
			for (int k = 0; k < HIT_MAX; k++){
				//当たり判定チェック、コピペの > の部分をイコールにする
				if(Box_Touch(H1[i].XAtt[j], H1[i].YAtt[j], H1[i].WAtt[j], H1[i].HAtt[j],
					P2.XHit[k], P2.YHit[k], P2.WHit[k], P2.HHit[k]) == true)
				{
					// アーマー、超必殺以外は受けない、地上
					if ((H1[i].HMoveHit > 0) && (P2.D.armor > 0) && (H1[i].attF != 2)
						&& (P2.SFlg != 2)){

						//---------------
						// 共通設定
						//---------------
						if (GuardCheck(i))
							// [ガードの設定]
						{
							P2.time = 0;
							P2.StopTime = 0;
							GuardParam(i);
						}
						// [ヒットの設定]
						else
						{
							// [1Pのヒット確認用]
							H1[i].HMoveHit--;
							// [スパキャン猶予]
							P1.scTime = SC_TIME;
							P1.A.ncTime = NC_TIME;
							// [ダメージ補正]
							P2.Life -= H1[i].A.damage;

							P2.colorCTime = 8;
							P2.colorC[0] = 255, P2.colorC[1] = 0, P2.colorC[2] = 0;

							// [ゲージ回収]
							P1.Power += H1[i].HGetPow;
							P2.Power += H1[i].HGivePow;

							// [ヒットSE]
							SEStart(H1[i].HHitSE);

							// [ヒットエフェクト]
							int posX;
							{ posX = 20 + GetRand(10); }
							EffStart(H1[i].hitEff, (int)P2.XPos, (int)P2.YPos, posX, -(P2.ySize / 1.5),
								H1[i].heSizeX, H1[i].heSizeY, P2.muki);

							// [ヒットストップ、のけぞり時間]
							P2.StopTime = H1[i].HHitStop;
							H1[i].stopTime = H1[i].HSSelf;
							if (P2.SFlg != 2){
								P2.D.nokezori = H1[i].HG_HitTime;
							}
							else{ P2.D.nokezori = H1[i].HA_HitTime; }

							// アーマー減少
							P2.D.armor--;
						}

						
					}
					// P1 攻撃ヒットチェック
					// ヒットフラグがオフで、相手が無敵じゃない
					// スイッチがオン
					else if ((H1[i].HMoveHit > 0) && (H1[i].var) &&
						(
						(P2.Muteki == 0) ||
						(P2.Muteki > 0 && P2.mutekiF == 1 && H1[i].attF != 1) ||
						(P2.Muteki > 0 && P2.mutekiF == 2 && H1[i].attF != 2) ||
						(P2.Muteki > 0 && P2.mutekiF == 3 && H1[i].attF != 3)
						)
					   )
					{
						// ガーポ・当身用
						gTime = P2.time;

						//---------------
						// 共通設定
						//---------------
						P2.time = 0;
						P2.StopTime = 0;
						//***************
						//
						// ヒットorガード
						//
						//***************
						// ガードの設定
						if (GuardCheck(i))
						// [ガードの設定]
						{
							if ((P2.Name == SYUICHI) && ((P2.stateno >= 600) && (P2.stateno < 820))) {
								if (P2.stateno == 600) {
									P2.time = gTime;
									GuardPoint(i);
								}
							}
							else { GuardParam(i); }
						}
						// [ガード時設定終了]
						// [ヒットの設定]
						else
						{
							HitParam(i);
						
						}// ヒット時設定終了
						
						// 共通設定
						// [ダメージ元に戻す]
						H1[i].A.damage = 0;

					}// ヒットorガード判断終了

				}// ifチェック終了
			}// 2P 喰らい判定チェック
		}// 攻撃判定チェック
	}// ヘルパーチェック


	//********************
	// ヘルパー同士の判定
	//********************

	for (int i = 0; i < HM; i++){
		for (int l = 0; l < HM; l++){
			for (int j = 0; j < ATT_MAX; j++){
				for (int k = 0; k < HIT_MAX; k++){
					//当たり判定チェック、コピペの > の部分をイコールにする
					if (Box_Touch(H1[i].XAtt[j], H1[i].YAtt[j], H1[i].WAtt[j], H1[i].HAtt[j],
						H2[l].XAtt[k], H2[l].YAtt[k], H2[l].WAtt[k], H2[l].HAtt[k]) == true)
					{

						// P1 攻撃ヒットチェック
						// ヒットフラグがオフで、相手が無敵じゃない
						// スイッチがオン
						// お互い飛び道具
						if (
							(H1[i].HMoveHit > 0) && (H1[i].var) && (H1[i].attF == 3) &&
							(H2[l].HMoveHit > 0) && (H2[l].var) && (H2[l].attF == 3)
							)
						{

							// [ヒットの設定]

							// [サウンド]
							SEStart(H1[i].HHitSE);
							// [ヒットエフェクト]
							int dPos, dist;
							if (H1[i].XPos > H2[l].XPos){
								dist = (int)((H1[i].XPos - H2[l].XPos) / 2);
								dPos = H1[i].XPos - dist;
							}
							else{ 
								dist = (int)((H2[l].XPos - H1[i].XPos) / 2); 
								dPos = H2[l].XPos - dist;
							}
							EffStart(1, dPos, (int)H1[i].YPos, 0, -25,
								0.5, 0.5, false);

							// 共通設定
							// [1Pのヒット確認用]
							H1[i].HMoveHit--, H2[l].HMoveHit--;
							// [ダメージ元に戻す]
							H1[i].A.damage = 0, H2[l].A.damage = 0;
							// [スパキャン猶予]
							//P1.scTime = SC_TIME;

						}// ヒット時設定終了

						
					}// ifチェック終了
				}// 2P 喰らい判定チェック
			}// 攻撃判定チェック
		}// 2P ヘルパー
	}// ヘルパーチェック


	// 最後に値を渡す
	if (P1.PSide == 1){
		Get_PSet(P1, P2);
		Get_HSet(H1, H2);
	}
	if (P1.PSide == 2){
		Get_PSet(P2, P1);
		Get_HSet(H2, H1);	
	}
	Get_SSet(S);

}

void GuardPoint(int i)
{
	// [ゲージ回収]
	P1.Power += H1[i].HGetPow / 2;
	P2.Power += H1[i].HGivePow / 2;

	// SE
	if (P2.D.nokezori >= 18)SEStart(17);
	else { SEStart(16); }

	// [ヒットエフェクト]
	int posX, posY;
	int defPosY;	// 基準位置
	defPosY = (int)P2.YPos;

	if (P1.YPos < P2.YPos - 50)defPosY = (int)P2.YPos - 50;

	//P1.GuardF; ガード方向で座標かえる
	if ((P1.GuardF == 3 || P1.GuardF == 13) && (defPosY == (int)P2.YPos)) {
		posY = P2.ySize / 2;
	}
	else { posY = P2.ySize; }

	// [1Pのヒット確認用]
	H1[i].HMoveHit = 0;

	// エフェクト開始
	{ posX = 20 + GetRand(10); }
	{
		EffStart(11, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			0.2, 0.6, P2.muki);
	}

	// ヒットストップ、のけぞり時間
	H1[i].stopTime = H1[i].HSSelf;

	if (P2.StopTime < 0)P2.StopTime = 0;

	// [ガード数カウント]
	P1.A.guardCount += 1;	// カウント
}

void GuardParam(int i)
{

	// [ 振り向き ]
	{
		if (P2.XPos < P1.XPos){
			if (P2.muki == 1)P2.muki = 0;
		}
		if (P1.XPos < P2.XPos){
			if (P2.muki == 0)P2.muki = 1;
		}
	}

	// [ゲージ回収]
	P1.Power += H1[i].HGetPow / 2;
	P2.Power += H1[i].HGivePow / 2;

	// [ダメージ計算]
	P2.Life -= (H1[i].A.damage + H1[i].A.hosyo) / 10;
	if ((P2.Life <= 0) && (P2.aGauge > 0))P2.Life = 1;	// 削り殺し防止

	// [ガードゲージ減少]
	//if (P2.SFlg == 2)P2.aGauge -= H1[i].A.damage / 8;	// 空中ガードは増加
	//else{ P1.aGauge -= H1[i].A.damage / 10; }
	
	boolean gotoCrash = false;
	// [ガードゲージ減少]

	if (P2.aGauge > 0 && P2.SFlg == 2) 
	{
		int n = 0;
		// 削り量決定
		if (P2.SFlg == 2){
			n = H1[i].A.damage + H1[i].A.hosyo + 20;
		}
		else{
			//n = (int)((float)(H1[i].A.damage + H1[i].A.hosyo) * 0.55);
		}

		// 初段
		if (P1.A.guardCount == 0){
			if (n > 250)n = 250;
			else if (n < 50)n = 50;
		}
		// ガード中
		else{
			switch (P1.A.guardCount){
			case 1:
				n = ((float)n * 0.95);  break;
			case 2:
				n = ((float)n * 0.90);  break;
			default:
				n = ((float)n * 0.85);  break;
			}
			// 最大値制限
			if (n > 200)n = 200;
			else if (n < 20)n = 20;
		}
		// 数値変更
		P2.aGauge -= n;
		TestText(n, 1);

		// クラッシュしたら
		if (P2.aGauge <= 0){
			gotoCrash = true;
			P1.A.guardCount = 0;
			H1[i].H_GX = -2.0;
			H1[i].H_AX = -1.1;
			H1[i].H_GY = 0.0;
			H1[i].H_AY = -5.0 / 2.0;
			P2.D.yaccel = 0.14;
		}
	}

	// ガークラではない
	if (!gotoCrash){
		// [アニメ設定]
		// 硬直に移行
		if (P2.SFlg == 0)P2.stateno = 50;
		if (P2.SFlg == 1 || (P2.keyAtt[2] > 0))P2.stateno = 51;
		if (P2.SFlg == 2)P2.stateno = 52;

		// [のけぞり時間]
		P2.D.nokezori = H1[i].HG_GuardTime;
		P2.StopTime = H1[i].HHitStop;
	}
	// ガークラ
	else{
		// 硬直に移行
		if (P2.SFlg == 0)P2.stateno = 1000;
		if (P2.SFlg == 1)P2.stateno = 1020;
		if (P2.SFlg == 2)P2.stateno = 1030;

		// ガード時間
		P2.D.nokezori = 70;
	}

	H1[i].stopTime = H1[i].HSSelf;

	// [サウンド]
	SEStart(H1[i].HGuardSE);
	// [ヒットエフェクト]
	int posX, posY;
	int defPosY;	// 基準位置
	defPosY = (int)P2.YPos;

	if (P1.YPos < P2.YPos - 50)defPosY = (int)P2.YPos - 50;

	//P1.GuardF; ガード方向で座標かえる
	if (H1[i].HGuardF == 3 || H1[i].HGuardF == 13){
		posY = P2.ySize / 2;
	}
	else{ posY = P2.ySize; }
	{ posX = 20 + GetRand(10); }

	EffStart(11, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
		0.2, 0.6, P2.muki);

	// [1Pのヒット確認用]
	H1[i].HMoveHit = 0;

	// [ノックバック距離]
	if (((P2.XPos <= GAMENHAJI) || (P2.XPos >= (STAGE_WIDTH - GAMENHAJI))) &&
		(P1.SFlg != 2)){
		// 空中で喰らっている
		if (P2.SFlg == 2){
			P2.XVel = H1[i].H_AX, P2.YVel = H1[i].H_AY;
		}
		else{ P2.XVel = H1[i].H_GX, P2.YVel = H1[i].H_GY; }
	}
	else{
		// 空中で喰らっている
		if (P2.SFlg == 2)P2.XVel = H1[i].H_AX, P2.YVel = H1[i].H_AY;
		else{ P2.XVel = H1[i].H_GX, P2.YVel = H1[i].H_GY; }
	}
	
	if (!gotoCrash){
		// 空ガ調整
		if (P2.SFlg == 2){
			if (P2.YVel < -1.2)P2.YVel = -1.2;
			else if (P2.YVel > -0.2)P2.YVel = -0.2;

			if (P2.XVel < -2)P2.XVel = -2;
		}
	}
	// [ガード数カウント]
	if (!gotoCrash)
		P1.A.guardCount += 1;	// カウント

	// [スパキャン猶予]
	P1.scTime = SC_TIME;

	// トレモ表示用
	AttackSpeed(P1.time);
	Frame_Check();
}

void HitParam(int i)
{
	// [ゲージ回収]
	// ゲージ補正があるか、回収しすぎなら、ゲージ増加を減らす
	if (P1.A.gaugeHosei || getPower >= 1000){
		P1.Power += H1[i].HGetPow / 5;
		if (P1.stateno != 520)P1.Power += P1.addG;
	}
	else if (getPower >= 500){
		P1.Power += (int)(H1[i].HGetPow * 0.8);
		if (P1.stateno != 520)P1.Power += P1.addG;
	}
	else{ 
		P1.Power += H1[i].HGetPow; 
		if (P1.stateno != 520)P1.Power += P1.addG * 5;
	}
	P1.addG = 0;

	// 相手ゲージ
	if (getPower >= 500){
		P2.Power += (int)(H1[i].HGivePow * 0.8);
	}
	else{ P2.Power += H1[i].HGivePow; }

	// ゲージ回収量計測
	if (P1.HitCount == 0){
		getPower = 0;
	}
	getPower += P1.GetPow;


	hosei = (P1.A.cl_max - ((P1.HitCount - 1) * P1.A.comboLate)) * P1.A.hosei_K;

	// [ダメージ補正]
	if (P1.HitCount == 0){	// 1Hit
		P2.Life -= H1[i].A.damage;
		// カウンターヒット
		if (P2.D.counter){
			// フェイタル追加ダメ
			if (P2.D.fatal)P2.Life -= 20;
			P2.Life -= (int)H1[i].A.damage * 0.2;
			if (H1[i].HHitStop > 0)H1[i].HHitStop += 2;
			if (H1[i].HHitStop > 0){
				H1[i].HG_HitTime += 2;
				H1[i].HA_HitTime += 2;
			}
			P2.colorCTime = 5;
			P2.colorC[0] = 255, P2.colorC[1] = 0, P2.colorC[2] = 0;
		}
	}
	else if (P1.HitCount >= 1){
		// ダメが減りすぎ
		if (hosei < P1.A.cl_min){
			P2.Life -= (int)(H1[i].A.damage *P1.A.cl_min);
		}
		else{
			P2.Life -= (int)(H1[i].A.damage *hosei);
		}
	}
	// [保障ダメージ]
	P2.Life -= H1[i].A.hosyo;



	// [アニメ設定]
	// アニメ変更ない場合もタイムを0に
	P2.time = 0;
	// ロック技はそのまま適用
	if ((H1[i].HHitAnim >= 1060) && (H1[i].HHitAnim <= 1069)){
		P2.stateno = H1[i].HHitAnim;
	}
	// 普通の喰らい
	else{
		// 空中喰らいを判断
		if ((P2.SFlg == 2) || (H1[i].H_GY < 0)){
			P2.stateno = 1030;
		}
		// 普通の喰らいで立ち
		else if (P2.SFlg == 0){
			if (H1[i].HHitAnim == 1000)P2.stateno = 1000;
			else{ P2.stateno = 1010; }
		}
		// 普通の喰らいでしゃがみ
		else if (P2.SFlg == 1){
			P2.stateno = 1020;
		}
	}


	// [サウンド]
	SEStart(H1[i].HHitSE);
	// [ヒットエフェクト]
	int posX, posY;

	//P1.GuardF; ガード方向で座標かえる
	if (H1[i].HGuardF == 3 || H1[i].HGuardF == 13){
		posY = P2.ySize / 2;
	}
	else{ posY = P2.ySize; }

	{ posX = 20 + GetRand(10); }
	EffStart(10, (int)P2.XPos, (int)P2.YPos, posX, -(posY / 1.5),
		0.2, 0.2, P2.muki);
	for (int i = 0; i < 6; i++){
		float rand = GetRand(4);
		rand = (rand * 0.01) + 0.07;
		EffStart(9, (int)P2.XPos, (int)P2.YPos, posX, -(posY / 1.5),
			rand, rand, P2.muki);
	}
	EffStart(H1[i].hitEff, (int)P2.XPos, (int)P2.YPos, posX, -(posY / 1.5),
		H1[i].heSizeX, H1[i].heSizeY, P2.muki);
	
	// [ノックバック距離]
	if (((P2.XPos <= GAMENHAJI) || (P2.XPos >= (STAGE_WIDTH - GAMENHAJI))) &&
		(P1.SFlg != 2)){
		// 空中で喰らっている
		if (P2.SFlg == 2){

			P2.XVel = H1[i].H_AX;
			P2.YVel = H1[i].H_AY;
			// ヒット数増加で重力
			//if (P1.HitCount > 20){
				//P2.YVel = (H1[i].H_AY + ((float)0.25 * (P1.HitCount - 10)));
			//}
			//else { P2.YVel = H1[i].H_AY; }


		}
		else{ P2.XVel = H1[i].H_GX, P2.YVel = H1[i].H_GY; }
	}
	else{
		// 空中で喰らっている
		if (P2.SFlg == 2){
			P2.XVel = H1[i].H_AX;
			/*
			// ヒット数増加で重力
			if (P1.HitCount > 20){
			P2.YVel = (H1[i].H_AY + ((float)0.25 * (P1.HitCount - 10)));
			}
			*/
			//else
			{ P2.YVel = H1[i].H_AY; }

		}
		else{ P2.XVel = H1[i].H_GX, P2.YVel = H1[i].H_GY; }
	}

	// [ロック中はヒットバックなし]
	if ((P2.stateno >= 1060) && (P2.stateno <= 1069)){
		P2.XVel = 0;
		P2.YVel = 0;
	}

	// [重力]
	P2.D.yaccel = GRAVITY;

	// [のけぞり時間]
	if (P2.SFlg != 2){
		P2.D.nokezori = H1[i].HG_HitTime;
	}
	else{ P2.D.nokezori = H1[i].HA_HitTime; }
	P2.StopTime = H1[i].HHitStop;
	H1[i].stopTime = H1[i].HSSelf;

	P2.fall = H1[i].fallF;

	// [スパキャン猶予]
	P1.scTime = SC_TIME;

	// [アーマー削除]
	if (P2.D.armor > 0)P2.D.armor = 0;

	// [ヒット数カウント]
	if (H1[i].A.damage > 0)
		P1.HitCount += 1;

	// [1Pのヒット確認用]
	H1[i].HMoveHit = 0;
	
	// [2Pのコントロールを奪う]
	P2.ctrl = 0;

	// [バウンド設定]
	P2.D.bound_xvel = -1.3;
	P2.D.bound_yvel = -3.0;
	P2.D.wall_xvel = 0.4;
	P2.D.wall_yvel = -6.4;

	// 床バウンド
	P2.D.bound = H1[i].A.boundLevel;
	if (P2.D.bound > 2)P2.D.bound = 2;

	if (P2.D.boundCount > 0){
		// 全て小バウンド
		if ((H1[i].A.boundLevel > 0) && (H1[i].A.boundLevel != 3)){
			P2.D.bound = 1;
		}
	}
	// 速度戻す
	if (P2.D.bound == 0){
		P2.D.bound_xvel = -1.3;
		P2.D.bound_yvel = -3.0;
	}

	// 壁バウンド
	// 大きかったらなし
	if (P2.D.wall <= 2){
		P2.D.wall = H1[i].A.wallLevel;
	}
	// 速度戻す
	if (P2.D.wall == 0){
		P2.D.wall_xvel = -0.4;
		P2.D.wall_yvel = -6.4;
	}

	// バウンドとりあえず0
	//P2.D.bound = 0;
	
	// トレモ表示用
	AttackSpeed(P1.time);
	Frame_Check();
	Beat_Check(P2.OldState, P1.PSide);

}


boolean GuardCheck(int n)
{
	// トレモガード適用
	if ((S.TraningFlg) && (S.TSwitch[4] > 0) && (S.TSwitch[2] < 3) && (P1.PSide == 1)){
		if ((S.TSwitch[4] == 1) ||
			((S.TSwitch[4] == 2) && (GetRand(1) == 0))){
			P2.keyAtt[4] = 1;
			if ((H1[n].HGuardF == 3) || (H1[n].HGuardF == 13))P2.keyAtt[2] = 1;
		}
	}

	boolean gu = false;
	boolean airGuard = false;

	// 向きが同じなら6入力でも空中ガード可能
	if ((P2.SFlg == 2) && (P2.keyAtt[6] != 0) && (P2.aGauge > 0)){
		if (P2.muki == 0){
			if (P1.XPos < P2.XPos)airGuard = true;
		}
		else if (P2.muki == 1){
			if (P2.XPos < P1.XPos)airGuard = true;
		}
	}

	if ((P2.SFlg == 2) && (P2.keyAtt[4] != 0) && (P2.aGauge > 0)){
		airGuard = true;
	}


	if (
		// 初回ガード(ジャンプ移行ではない、例外として着地硬直はガード可)　|| 空中ガード判定 || 連続ガード
		((P2.ctrl || P2.stateno == 47) && ((P2.keyAtt[4] != 0) || ((P2.D.selfTime > 0) && (P2.keyAtt[6] != 0)))
		&& (P2.SFlg != 2) && (P2.stateno != 40) ||
		(P2.ctrl) && (airGuard) || (P2.stateno >= 50 && P2.stateno <= 59))
		)
	{
		if (
			(H1[n].HGuardF == 1) ||

			// 中段
			((H1[n].HGuardF == 2)
			&& (P2.SFlg == 0 || P2.SFlg == 2)) ||

			(H1[n].HGuardF == 3)
			&& (((P2.SFlg == 1) || (P2.keyAtt[2] > 0)) || (P2.SFlg == 2)) ||

			(H1[n].HGuardF == 4)
			&& ((P2.SFlg == 0) || ((P2.SFlg == 1) || (P2.keyAtt[2] > 0)))
			)
			gu = true;
	}

	// ガードポイント
	if ((!gu) && (P2.Name == SYUICHI)) {
		if ((P2.stateno == 600) &&
			((gTime >= 1) && (gTime < 50))) {
			if ((H1[n].HGuardF == 1) || (H1[n].HGuardF == 2))
				gu = true;
		}
	}

	return gu;
	////////////////////////
	
}

void GetP_HelDamage(Player GP1, Player GP2)
{

	P1 = GP1;
	P2 = GP2;
}

void GetH_HelDamage(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}

void GetS_HelDamage(System_t GS)
{
	S = GS;
}

