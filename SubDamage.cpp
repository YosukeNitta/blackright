//==================================================================================
//
//			数値計算の管理部
//
//==================================================================================

//インクルード部--------------------------------------------
#include "pch.h"

//デファイン部----------------------------------------------



//ﾃﾞｰﾀ定義部------------------------------------------------
static void GuardParam();
static void GuardPoint();
static void HitParam();
static boolean GuardCheck();
static void DamageEnter();

//内部関数宣言部--------------------------------------------

static int posX;	// エフェクトの位置
//static int velY;	// 移動速度
static double hosei;	// 補正です。
static double knockBack;	// 画面端ノックバック
static int getPower;	// ゲージ回収、多すぎると減少
static int hit, guard;	// ヒット、ガード判定
static Player P[2], hozon[2];
static boolean touch[2];
static int gTime;	// ガーポ、当身の保存時間
//プログラム部----------------------------------------------

void DamageCheck()
{

	//
	touch[0] = 0, touch[1] = 0;

	//[ 通常投げ抜け設定 ]
	// どっちも投げならなし
	if ((P1.stateno == 500 && P2.stateno == 500) ||
		(P1.stateno == 510 && P2.stateno == 510)){
		// どちらも動作前
		if (P1.time <= 6 && P2.time <= 6){
			P1.D.counter = 0;
			P1.rejectF = 1;
			P2.D.counter = 0;
			P2.rejectF = 1;
			P1.Senkou[1] = 5, P1.Senkou[2] = 5;
			P2.Senkou[1] = 5, P2.Senkou[2] = 5;
		}
		// 2P抜け失敗
		else if (P1.time <= 6 && P2.time > 6){
			P2.D.counter = 0;
			P2.rejectF = 0;
			P2.Senkou[1] = 0, P2.Senkou[2] = 0;
		}
		// 1P抜け失敗
		else if (P2.time <= 6 && P1.time > 6){
			P1.D.counter = 0;
			P1.rejectF = 0;
			P1.Senkou[1] = 0, P1.Senkou[2] = 0;
		}
	}

	P[0] = P1, P[1] = P2;

	// 自動ガード
	for (int i = 0; i < 2; i++){
		if (P[i].D.selfTime > 0){
			P[i].D.selfTime--;
		}
		if (P[i].HFlg == 1){
			P[i].D.selfTime = 6;
		}
	}
	
	// ヒット判定
	for (int p = 0; p < 2; p++){
		if (p == 0){
			P1 = P[0], P2 = P[1];
		}
		else{
			P1 = P[1], P2 = P[0];
		}


		// 1P攻撃
		for (int i = 0; i < HIT_MAX; i++){
			for (int j = 0; j < ATT_MAX; j++){
				//当たり判定チェック、コピペの > の部分をイコールにする
				/*
				if (
					((P1.XAtt[j] >= P2.XHit[i] && P1.XAtt[j] <= P2.XHit[i] + P2.WHit[i]) ||
					(P2.XHit[i] >= P1.XAtt[j] && P2.XHit[i] <= P1.XAtt[j] + P1.WAtt[j])) &&
					((P1.YAtt[j] >= P2.YHit[i] && P1.YAtt[j] <= P2.YHit[i] + P2.HHit[i]) ||
					(P2.YHit[i] >= P1.YAtt[j] && P2.YHit[i] <= P1.YAtt[j] + P1.HAtt[j]))
					)
					*/
				if (Box_Touch(P1.XAtt[j], P1.YAtt[j], P1.WAtt[j], P1.HAtt[j],
					P2.XHit[i], P2.YHit[i], P2.WHit[i], P2.HHit[i]) == true)
				{
					// ここじゃない
					touch[p] = 1;
					//break;
				}// 1P 攻撃判定チェック 終了
			}// 攻撃判定チェック2
		}// 全て終了

		if (p == 0){
			P[0] = P1, P[1] = P2;
		}
		else{
			P[0] = P2, P[1] = P1;
		}

	}// 1P.2P終了

	P1 = P[0], P2 = P[1];

	// 同時に攻撃
	if ((touch[0] == 1) && (touch[1] == 1)){
		// 打撃
		if ((P1.attF == 1) && (P2.attF == 1)){
			/*
			if ((P1.MoveHit > 0) && (P2.MoveHit > 0)){
				touch[0] = 0;
				touch[1] = 0;
				P1.MoveHit--;
				P2.MoveHit--;
				P1.StopTime = 6;
				P2.StopTime = 6;
				SEStart(17);
				int dist = 0;
				if (P1.XPos < P2.XPos)dist = P2.XPos - P1.XPos;
				else{ dist = -(P1.XPos - P2.XPos); }
				dist /= 2;
				EffStart(12, (int)P1.XPos + dist, (int)P1.YPos, 0, -(P1.ySize / 1.5),
					0.5, 0.5, 0);
			}
			*/
		}
		// 投げ
		else if ((P1.attF == 2) && (P2.attF == 2)){
			// 両方 コマ投げなら何も起こらない
			if (P1.stateno > 510 && P2.stateno > 510){
				touch[0] = 0;
				touch[1] = 0;
			}
			// 1P コマ
			else if (P1.stateno > 510 && P2.stateno <= 510){
				touch[1] = 0;
			}// 2P コマ
			else if (P1.stateno <= 510 && P2.stateno > 510){
				touch[0] = 0;
			}
			// 同時の場合、1Pが投げた扱い
			else if (P1.stateno <= 510 && P2.stateno <= 510){
				//if (GetRand(2) == 0){
					touch[1] = 0;
				//}
			}
		}
		// 1Pが打撃
		else if (P1.attF != 2 && P2.attF == 2){
			// コマ投げが勝つ
			if ((P2.stateno < 500) || (P2.stateno > 510)){
				touch[0] = 0;
			}
			// 打撃が勝つ
			else{ touch[0] = 0; }
		}
		// 2Pが打撃
		else if (P1.attF == 2 && P2.attF != 2){
			// コマ投げが勝つ
			if ((P1.stateno < 500) || (P1.stateno > 510)){
				touch[1] = 0;
			}
			else{ touch[1] = 0; }
		}
	}
	
	// 1P攻撃
	if (touch[0] == 1){
		DamageEnter();
	}
	// 2P攻撃
	if (touch[1] == 1){
		hozon[0] = P1, hozon[1] = P2;
		P2 = hozon[0], P1 = hozon[1];
		DamageEnter();
		hozon[0] = P2, hozon[1] = P1;
		P1 = hozon[0], P2 = hozon[1];
	}

	Get_PSet(P1, P2);
	Get_SSet(S);
	
}

void DamageEnter()
{
	// アーマー、超必殺・投げ以外は受けない、地上限定
	// ここじゃない
	if ((P1.MoveHit > 0) && (P2.D.armor > 0) && ((P1.stateno < 800) && (P1.stateno != 520)) && (P1.attF != 2)
		&& (P2.SFlg != 2 || P2.stateno == 30)){

		//---------------
		// 共通設定
		//---------------
		// [キャンセル可能に]
		P1.CFlg = 1;
		// [1Pのヒット確認用]
		P1.MoveHit--;
		// [スパキャン猶予]
		P1.scTime = SC_TIME;

		if (GuardCheck())
			// [ガードの設定]
		{
			P2.time = 0;
			P2.StopTime = 0;
			GuardParam();
		}
		// [スパアマ時ヒットの設定]
		else
		{
			// [キャンセル可能に]
			P1.CFlg = 1;
			// [1Pのヒット確認用]
			P1.MoveHit--;
			// [スパキャン猶予]
			P1.scTime = SC_TIME;
			P1.A.ncTime = NC_TIME;
			// [ダメージ補正]
			P2.Life -= P1.A.damage;
			// [保障ダメージ]
			P2.Life -= P1.A.hosyo;

			P2.colorCTime = 8;
			P2.colorC[0] = 255, P2.colorC[1] = 0, P2.colorC[2] = 0;

			// [ゲージ回収]
			P1.Power += P1.GetPow;
			P2.Power += P1.GivePow;

			// [ヒットSE]
			SEStart(P1.HitSE);

			// [ヒットエフェクト]
			int posX;
			{ posX = 20 + GetRand(10); }
			EffStart(P1.A.hitEff, (int)P2.XPos, (int)P2.YPos, posX, -(P2.ySize / 1.5),
				P1.A.heSizeX, P1.A.heSizeY, P2.muki);

			// [ヒットストップ、のけぞり時間]
			P2.StopTime = P1.HitStop;
			P1.StopTime = P1.HSSelf;
			/*
			if (P2.SFlg != 2){
				P2.D.nokezori = P1.G_HitTime;
			}
			else{ P2.D.nokezori = P1.A_HitTime; }
			*/
			// アーマー減少
			P2.D.armor--;
		}
	}	// アーマー終了

	// ここじゃない
	// P1 攻撃ヒットチェック
	// ダメージがあり、ヒットフラグがオフで、相手が無敵じゃない
	else if ((P1.MoveHit > 0) &&
		(
		(P2.Muteki == 0 && P1.attF != 2) ||
		(P2.Muteki > 0 && P2.mutekiF == 1 && P1.attF != 1) ||
		(P2.Muteki > 0 && P2.mutekiF == 2 && P1.attF != 2) ||
		(P2.Muteki > 0 && P2.mutekiF == 3 && P1.attF != 3) ||
		// 投げ 地上、空中
		(P2.Muteki == 0 && P1.SFlg != 2 && P2.SFlg != 2 && P1.attF == 2) ||	
		(P2.Muteki == 0 && P1.SFlg == 2 && P2.SFlg == 2 && P1.attF == 2)
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
		// [キャンセル可能に]
		P1.CFlg = 1;
		// [1Pのヒット確認用]
		P1.MoveHit--;
		// [スパキャン猶予]
		P1.scTime = SC_TIME;
		P1.A.ncTime = NC_TIME;
		//***************
		//
		// ヒットorガード
		//
		//***************

		// ガードの設定

		if (GuardCheck())
		// [ガードの設定]
		{
			if ((P2.Name == SYUICHI) && ((P2.stateno >= 600) && (P2.stateno < 700))) {
				if (P2.stateno == 600){
					P2.time = gTime;
					GuardPoint();
				}
				if ((P2.stateno == 610) || (P2.stateno == 611)) {
					P2.time = 0;
					P2.stateno = 615;
					P1.StopTime = 10, P2.StopTime = 10;
					P1.CFlg = 0;
					SEStart(19);
				}
			}
			else { GuardParam(); }
		}
		// [ヒットの設定]
		else
		{
			HitParam();
		}
		
	}// 接触条件終了

}

void GuardPoint()
{
	// [ゲージ回収]
	P1.Power += P1.GetPow / 2;
	P2.Power += P1.GivePow / 2;

	// SE
	if (P2.D.nokezori >= 18)SEStart(17);
	else { SEStart(16); }

	// [ヒットエフェクト]
	int posX, posY;
	int defPosY;	// 基準位置
	float effSize = 0.0;
	defPosY = (int)P2.YPos;
	if (P1.YPos < P2.YPos - 50)defPosY = (int)P2.YPos - 50;
	if (P2.D.nokezori >= 20)effSize = 0.1;

	//P1.GuardF; ガード方向で座標かえる
	if ((P1.GuardF == 3 || P1.GuardF == 13) && (defPosY == (int)P2.YPos)) {
		posY = P2.ySize / 2;
	}
	else { posY = P2.ySize; }

	// エフェクト開始
	{ posX = 20 + GetRand(10); }
	{
		EffStart(11, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			0.2, 0.6 + effSize, P2.muki);
	}

	// ヒットストップ、のけぞり時間
	P2.StopTime = P1.HitStop;
	P1.StopTime = P1.HSSelf;
	if (P1.StopTime < 0)P1.StopTime = 0;
	if (P2.StopTime < 0)P2.StopTime = 0;

	// [ガード数カウント]
	P1.A.guardCount += 1;	// カウント
}

void GuardParam()
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

	// [削り]
	int kezuri;
	kezuri = (P1.A.damage + P1.A.hosyo) / 10;

	if (P1.stateno >= 600){
		P2.Life -= kezuri;
	}
	else if ((P1.stateno >= 220 && P1.stateno < 300) || 
			(P1.stateno >= 320 && P1.stateno < 400)){
		if (kezuri >= 8)kezuri = 8;
		P2.Life -= kezuri;
	}
	/*
	// 通常技削り　ダメは最大4
	else if (
		(P1.stateno > 210 && P1.stateno < 300) ||
		(P1.stateno > 310 && P1.stateno < 400)){
		if (kezuri > 4)kezuri = 4;
		P2.Life -= kezuri;
	}
	*/
	if (P2.Life <= 0)P2.Life = 1;	// 削り殺し防止

	// [ゲージ回収]
	P1.Power += P1.GetPow / 2;
	P2.Power += P1.GivePow / 2;

	boolean gotoCrash = false;

	// [ガードゲェジ減少]
	if (P2.aGauge > 0){
		int n = 0;
		// 削り量決定
		if (P2.SFlg == 2){
			n = (P1.A.damage + P1.A.hosyo) * 1.1 + 20;
		}
		else{
			n = (int)((float)(P1.A.damage + P1.A.hosyo) * 0.55);
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

		//int n = 0;
		/*
		// 削り量決定
		if (P2.SFlg == 2){
			n = P1.A.damage + P1.A.hosyo;
		}
		else{
			n = (P1.A.damage + P1.A.hosyo) / 4;
		}

		// 初段
		if (P2.A.guardCount == 0){
			if (n > 200)n = 200;
			else if (n < 1)n = 1;
			P2.aGauge -= n;
		}
		// ガード中
		else{
			if (n > 100)n = 100;
			else if (n < 1)n = 1;
			P2.aGauge -= n;
		}
		*/

		// クラッシュしたら
		if (P2.aGauge <= 0){
			gotoCrash = true;
			P1.A.guardCount = 0;
			P1.A.guard_gx = -2.0;
			P1.A.guard_ax = -1.1;
			P1.GY = 0.0;
			P1.AY = -5.0;
			P2.D.yaccel = 0.14;
			P2.Life -= 50;
			P1.CFlg = 0;
			if (P2.Life <= 0)P2.Life = 1;	// 削り殺し防止
		}
	}

	// [アドゲージ増加]
	//P1.addG += (P1.A.damage + P1.A.hosyo) / 12;

	// ガークラではない
	if (!gotoCrash){
		// [アニメ設定]
		if (P2.SFlg == 0)P2.stateno = 50;
		if (P2.SFlg == 1 || (P2.keyAtt[2] > 0))P2.stateno = 51;
		if (P2.SFlg == 2)P2.stateno = 52;

		// ガード時間
		P2.D.nokezori = P1.G_GuardTime;
		if ((P2.SFlg == 2) && (P1.A.A_GuardTime > 0))P2.D.nokezori = P1.A.A_GuardTime;
	}
	else{
		// [アニメ設定]
		if (P2.SFlg == 0)P2.stateno = 1000;
		if (P2.SFlg == 1)P2.stateno = 1020;
		if (P2.SFlg == 2)P2.stateno = 1030;

		// ガード時間
		P2.D.nokezori = 70;
	}

	// SE
	if (P2.D.nokezori >= 18)SEStart(17);
	else{ SEStart(16); }

	// [ヒットエフェクト]
	int posX, posY;
	int defPosY;	// 基準位置
	float effSize = 0.0;
	defPosY = (int)P2.YPos;
	if (P1.YPos < P2.YPos - 50)defPosY = (int)P2.YPos - 50;
	if (P2.D.nokezori >= 20)effSize = 0.1;

	//P1.GuardF; ガード方向で座標かえる
	if ((P1.GuardF == 3 || P1.GuardF == 13) && (defPosY == (int)P2.YPos)){
		posY = P2.ySize / 2;
	}
	else{ posY = P2.ySize; }

	// エフェクト開始
	{ posX = 20 + GetRand(10); }
	if (!gotoCrash){
		EffStart(11, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			0.2, 0.6 + effSize, P2.muki);
	}
	else{
		EffStart(15, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			0.4, 0.4 + effSize, P2.muki);
	}

	// ヒットストップ、のけぞり時間
	P2.StopTime = P1.HitStop;
	P1.StopTime = P1.HSSelf;
	if (P1.StopTime < 0)P1.StopTime = 0;
	if (P2.StopTime < 0)P2.StopTime = 0;

	// ガード時間

	// [ヒットバック　画面端にいて、1Pが地上]
	if (((P2.XPos <= GAMENHAJI) || (P2.XPos >= (STAGE_WIDTH - GAMENHAJI))) &&
		(P1.SFlg != 2)){
		// 空中で喰らっている
		if (P2.SFlg == 2){
			P1.A.hbVelX = P1.A.guard_gx;
			P2.XVel = P1.A.guard_ax;
			P2.YVel = P1.AY / 2.0;
		}
		else{ 
			//P1.A.hbVelX = P1.A.guard_gx + (P1.A.guard_gx * 0.1), 
			P2.XVel = P1.A.guard_gx + (P1.A.guard_gx * 0.1);
			P2.YVel = P1.GY; 
		}
		//else{ P1.XVel = P1.GX + (P1.GX * 0.1), P2.XVel = P1.GX, P2.YVel = P1.GY; }
		// 引き寄せ技ガード時、近づかないように
		if (P1.A.hbVelX > 0)P1.A.hbVelX = 0;
	}
	// [中央喰らい]
	else{
		// 空中で喰らっている
		if (P2.SFlg == 2){
			//P2.XVel = P1.AX * 1.5;
			P2.XVel = P1.A.guard_ax;
			P2.YVel = P1.AY / 2.0;
		}
		// 無理矢理ガード時は距離離す
		else{ 
			P2.XVel = P1.A.guard_gx + (P1.A.guard_gx * 0.1);
			P2.YVel = P1.GY; 
		}
	}
	// 空中ガードの速度、適度に
	if (!gotoCrash){
		if (P2.SFlg == 2){
			if (P2.YVel < -1.2)P2.YVel = -1.2;
			else if (P2.YVel > -0.2)P2.YVel = -0.2;

			if (P2.XVel < -2)P2.XVel = -2;
		}
	}

	// [ガード数カウント]
	if (!gotoCrash)
		P1.A.guardCount += 1;	// カウント

	//i = HIT_MAX + 1;
	//break;


	// トレモ表示用
	AttackSpeed(P1.time);
	Frame_Check();

}

void HitParam()
{
	
	hosei = (P1.A.cl_max - ((P1.HitCount - 1) * P1.A.comboLate)) * P1.A.hosei_K;

	// [ダメージ補正]
	if (P1.HitCount == 0){			// 初段ダメージ
		P2.Life -= P1.A.damage;
		
		// カウンターヒット
		if (P2.D.counter){		
			// フェイタル追加ダメ
			if(P2.D.fatal)P2.Life -= 20;
			// 投げ以外ならダメ増加
			if ((P1.A.damage + P1.A.hosyo) > 200){
				P2.Life -= 40;
			}
			else{ P2.Life -= (P1.A.damage + P1.A.hosyo) * 0.2; }

			if (P1.HitStop > 0){
				P1.G_HitTime += 2;
				P1.A_HitTime += 2;
			}
			P2.colorCTime = 5;
			P2.colorC[0] = 255, P2.colorC[1] = 0, P2.colorC[2] = 0;
		}
		
	}
	else if (P1.HitCount >= 1){		// 初段以降
		// ダメが減少しすぎ
		if (hosei < P1.A.cl_min)
		{
			// 強制補正のが小さい
			if (P1.A.hosei_K < P1.A.cl_min){
				P2.Life -= (int)(P1.A.damage * P1.A.hosei_K);
			}
			// 最低ダメージ
			else{
				P2.Life -= (int)(P1.A.damage * P1.A.cl_min);
			}
		}
		else
		{ 
			P2.Life -= (int)(P1.A.damage * hosei);
		}
	}
	
	/*
	else if (P1.HitCount >= 11){
		P2.Life -= (int)((P1.A.damage * 0.15) * P1.A.hosei_K);
	}
	*/
	// [保障ダメージ]
	P2.Life -= P1.A.hosyo;
	// kill
	if ((!P1.A.kill) && (P2.Life <= 0))P2.Life = 1;

	// [ゲージ回収]
	// 回収表示
	AddGaugeDraw(P1.addG, P1.PSide);

	// ゲージ補正があるか、回収しすぎなら、ゲージ増加を減らす
	if (P1.A.gaugeHosei || getPower >= 1000){
		P1.Power += (int)(P1.GetPow * 0.2);
		if (P1.stateno != 520)P1.Power += P1.addG * 5;
	}
	else if (getPower >= 500){
		P1.Power += (int)(P1.GetPow * 0.8);
		if (P1.stateno != 520)P1.Power += P1.addG * 5;
	}
	else{ 
		P1.Power += P1.GetPow; 
		if(P1.stateno != 520)P1.Power += P1.addG * 5;
	}
	if(P1.stateno != 520)P1.addG = 0;

	// 相手ゲージ
	if (getPower >= 500){
		P2.Power += (int)(P1.GivePow * 0.8);
	}
	else{ P2.Power += P1.GivePow; }

	// ゲージ回収量計測
	if (P1.HitCount == 0){
		getPower = 0;
	}
	getPower += P1.GetPow;


	// [投げ抜け可否]
	// 操作可能か着地なら
	if (P2.ctrl || P2.stateno == 47)P2.rejectF = true;
	// 相手も投げなら、何もしない
	else if ((P2.stateno == 500 || P2.stateno == 510)){

	}
	// どれでもない
	else{ P2.rejectF = false; }


	// [アニメ設定]
	// ロック技はそのまま
	if ((P1.HitAnim >= 1060) && (P1.HitAnim <= 1069)){
		P2.stateno = P1.HitAnim;
	}
	else{
		// 強制喰らいなら姿勢変更
		if (P1.A.forceKurai){
			if (P2.SFlg == 2){ P2.stateno = 1030; }
			else { P2.stateno = P1.HitAnim; }
		}
		// 空中喰らいを判断
		else if ((P2.SFlg == 2) || (P1.GY < 0)){
			P2.stateno = 1030;
		}
		// 普通の喰らいで立ち
		else if (P2.SFlg == 0){
			if (P1.HitAnim == 1000)P2.stateno = 1000;
			else{ P2.stateno = 1010; }
		}
		// 普通の喰らいでしゃがみ
		else if (P2.SFlg == 1){
			P2.stateno = 1020;
		}

	}

	// [ヒットSE]
	SEStart(P1.HitSE);

	// [ヒットエフェクト]
	int posX, posY;
	int defPosY;	// 基準位置
	//if (P2.muki == 1)posX = -20 - GetRand(10);
	//else
	defPosY = (int)P2.YPos;
	if (P1.YPos < P2.YPos - 50)defPosY = (int)P2.YPos - 50;

	//P1.GuardF; ガード方向で座標かえる
	if ((P1.GuardF == 3 || P1.GuardF == 13) && (defPosY == (int)P2.YPos)){
		posY = P2.ySize / 2;
	}
	else{ posY = P2.ySize; }

	{ posX = 20 + GetRand(10); }
	if (P1.A.hitEff != 0){
		EffStart(10, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			0.2, 0.2, P2.muki);
		for (int i = 0; i < 6; i++){
			float rand = GetRand(4);
			rand = (rand * 0.01) + 0.07;
			EffStart(9, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
				rand, rand, P2.muki);
		}
		EffStart(P1.A.hitEff, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			P1.A.heSizeX, P1.A.heSizeY, P2.muki);
	}

	// [ヒットストップ、のけぞり時間]
	P2.StopTime = P1.HitStop;
	P1.StopTime = P1.HSSelf;
	if (P1.StopTime < 0)P1.StopTime = 0;
	if (P2.StopTime < 0)P2.StopTime = 0;

	if (P2.SFlg != 2){
		P2.D.nokezori = P1.G_HitTime;
	}
	else{ P2.D.nokezori = P1.A_HitTime; }

	// 永パ防止
	if ((P2.SFlg == 2) && (P1.HitCount >= 20))knockBack += 1;
	if (P1.HitCount <= 1)knockBack = 0;

	// [ヒットバック　画面端にいて、1Pが地上]
	if ((P2.XPos <= GAMENHAJI) || (P2.XPos >= (STAGE_WIDTH - GAMENHAJI)))
	{
		if (P1.SFlg != 2)P1.A.hbVelX = P1.GX;
		// 空中で喰らっている
		if (P2.SFlg == 2){
			P2.XVel = P1.AX;
			// 超必殺は固定
			if (P1.stateno >= 800)P2.YVel = P1.AY;
			else{ P2.YVel = P1.AY + (knockBack * 0.1); }
			/*
			// ヒット数増加で重力
			if (P1.HitCount > 20){
				P2.YVel = (P1.AY + ((float)0.25 * (P1.HitCount - 10)));
			}
			//else { P2.YVel = P1.AY; }
			*/
		}
		// 画面端で2P地上
		else{ 
			P2.XVel = P1.GX;
			P2.YVel = P1.GY; 
		}
		// 引き寄せ技でも前進しない
		if (P1.A.hbVelX > 0)P1.A.hbVelX = 0;
	}
	else{
		// 空中で喰らっている
		if (P2.SFlg == 2){
			P2.XVel = P1.AX;
			P2.YVel = P1.AY + (knockBack * 0.1);
		}
		else{ 
			P2.XVel = P1.GX, P2.YVel = P1.GY; 
		}
	}

	// [ロック中はヒットバックなし]
	if ((P1.HitAnim >= 1060) && (P1.HitAnim <= 1069)){
		P1.A.hbVelX = 0;
		P2.XVel = 0;
		P2.YVel = 0;
	}

	// [重力]
	P2.D.yaccel = P1.A.yaccel;

	// [ヒット数カウント]
	if ((P1.A.damage > 0) || (P1.A.hosyo > 0))
		P1.HitCount += 1;	// ダメージがある時のみカウント

	// [2Pのコントロールを奪う]
	P2.ctrl = 0;
	// [フォール判定]
	P2.fall = P1.fallF;


	// [バウンド設定]
	P2.D.bound_xvel = P1.A.bound_xvel;
	P2.D.bound_yvel = P1.A.bound_yvel;
	P2.D.wall_xvel = P1.A.wall_xvel;
	P2.D.wall_yvel = P1.A.wall_yvel;
	
	P2.D.bound = P1.A.boundLevel;
	if (P2.D.bound > 2)P2.D.bound = 2;

	if (P2.D.boundCount > 0){
		// 全て小バウンド
		if ((P1.A.boundLevel > 0) && (P1.A.boundLevel != 3)){
			P2.D.bound = 1;
		}
	}
	// 速度戻す
	if (P2.D.bound == 0){
		P2.D.bound_xvel = -1.3;
		P2.D.bound_yvel = -3.0;
	}
	
	// 大きかったらなし
	if (P2.D.wall <= 2){
		P2.D.wall = P1.A.wallLevel;
	}
	// 速度戻す
	if (P2.D.wall == 0){
		P2.D.wall_xvel = -0.4;
		P2.D.wall_yvel = -6.4;
	}
	
	// [アーマー削除]
	if(P2.D.armor > 0)P2.D.armor = 0;

	// [無敵付加]	一部の技用（ロックがめんどい時とか）
	if (P1.A.addMuteki){
		P2.Muteki = P1.A.amTime;
		P2.mutekiF = 0;
		P1.A.addMuteki = false;
		P1.A.amTime = 0;
	}

	// [地震エフェクト]
	if (P1.A.quakeTime > 0){
		S.quakeTime = P1.A.quakeTime;
		S.quakeY = P1.A.quakeY;
	}
	
	// トレモ表示用
	AttackSpeed(P1.time);
	Frame_Check();
	Beat_Check(P2.OldState, P1.PSide);
}

boolean GuardCheck()
{
	// トレモガード適用
	if ((S.TraningFlg) && (S.TSwitch[4] > 0) && (S.TSwitch[2] < 3) && (P1.PSide == 1)){
		if ((S.TSwitch[4] == 1) || 
			((S.TSwitch[4] == 2) && (GetRand(1) == 0))){
			P2.keyAtt[4] = 1;	// ガードするかランダム
			if ((P1.GuardF == 3) || (P1.GuardF == 13))P2.keyAtt[2] = 1;
		}

		// ガード
		if (P2.stateno >= 50 && P2.stateno < 60){
			if ((P1.GuardF == 3) || (P1.GuardF == 13)){
				P2.keyAtt[2] = 1;
				P2.button[102] = 1;
			}
		}
	}

	// ガード実行
	boolean gu = false;
	// 空中ガード可否
	boolean airGuard = false;

	// 向きが同じなら6入力でも空中ガード可能
	if ((P2.SFlg == 2) && (P2.keyAtt[6] != 0) && (P2.aGauge > 0) && (P2.stateno != 60 && P2.stateno != 65)){
		if (P2.muki == 0){
			if (P1.XPos < P2.XPos)airGuard = true;
		}
		else if (P2.muki == 1){
			if (P2.XPos < P1.XPos)airGuard = true;
		}
	}

	// 空ガ条件
	if ((P2.SFlg == 2) && (P2.keyAtt[4] != 0) && (P2.aGauge > 0) && 
		(P2.stateno != 60 && P2.stateno != 65)){
		airGuard = true;
	}

	if (
		// 初回ガード(ジャンプ移行ではない、例外として着地硬直はガード可) &&
		// 空中ガード判定 || 連続ガード || 起き上がりめくり＆相手が空中
		// 相手が空中ではない &&
		// ジャンプ移行以外 || 操作可能で空ガok || 既にガード中
		(
		(P2.ctrl || P2.stateno == 47) && 
		((P2.keyAtt[4] != 0) || ((P2.D.selfTime > 0) && (P2.keyAtt[6] != 0) && (P1.SFlg == 2))) && 
		(P2.SFlg != 2) && 
		(P2.stateno != 40) || (P2.ctrl) && (airGuard) || (P2.stateno >= 50 && P2.stateno <= 59)
		)
	   )
		{
		// ガードを認識、ガード方向確認開始
		if (
			(P1.GuardF == 1) ||
			
			// 中段
			((P1.GuardF == 2)
			&& ((P2.SFlg == 0 && (P2.keyAtt[2] == 0)) || P2.SFlg == 2)) ||	// 下を入れていない
			// 下段
			(P1.GuardF == 3)
			&& (((P2.SFlg == 1) || (P2.keyAtt[2] > 0)) || (P2.SFlg == 2)) || 
			// 空ガ不可
			(P1.GuardF == 4)
			&& ((P2.SFlg == 0) || ((P2.SFlg == 1) || (P2.keyAtt[2] > 0)))
			)
			gu = true;
	}
	// ガードポイント
	if ((!gu) && (P2.Name == SYUICHI)) {
		if ((P2.stateno == 600) && 
			((gTime >= 2) && (gTime < 30))) {
			if((P1.GuardF == 1) || (P1.GuardF == 2))
			gu = true;
		}
		if ((P2.stateno == 610) &&
			((gTime >= 2) && (gTime <= 17))) {
			if ((P1.GuardF == 1) || (P1.GuardF == 2))
				gu = true;
		}
		if ((P2.stateno == 611) &&
			((gTime >= 2) && (gTime <= 17))) {
			if ((P1.GuardF == 3) || (P1.GuardF == 13))
				gu = true;
		}
	}

	return gu;	// ガードの可否
}

void GetP_SubDamage(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetH_SubDamage(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}

void GetS_SubDamage(System_t GS)
{
	S = GS;
}
