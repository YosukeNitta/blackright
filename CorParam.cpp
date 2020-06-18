//==================================================================================
//
//			数値計算の管理部
//
//========================================================================

//インクルード部--------------------------------------------
#include "pch.h"
#include "Define2.h"

//デファイン部---------------------------------------------

//ﾃﾞｰﾀ定義部------------------------------------------------


// どっちのプレイヤー側か判断
static int PSide;


//内部関数宣言部--------------------------------------------

static void SCancel();
static void ACancel();
static void HCancel();

//プログラム部----------------------------------------------


/****
ステートの数値等を設定する
****/
void CorParam(void)
{
	//ワンモアを切る
	if (P1.More)P1.More = 0;

	// コマンドに数値を渡す
	//GetPH_EinCmd(P1);

	// ステートが切り替わったら
	if (P1.stateno != P1.OldState){
		GetP_SameParam(P1, P2);
		GetH_SameParam(H1, H2);
		GetS_SameParam(S);
		ChangeState();
	}


	// 1P
	if (P1.StopTime == 0 || P1.time == 0){
		//設定開始
		//--------------------
		// 攻撃判定リセット
		//--------------------
		if (P1.stateno < 200 || P1.stateno > 999){
			DamReset();

		}
		//--------------------
		// 5～6：振り向き
		// 40～47：
		// ジャンプ
		// 50～59：
		// ガード
		//--------------------
		if (((P1.stateno >= 5) && (P1.stateno <= 10)) ||	// 振り向き、しゃがみ移行
			((P1.stateno >= 40) && (P1.stateno <= 47)) ||	// ジャンプ
			((P1.stateno >= 50) && (P1.stateno <= 69)) ||	// ガード・空中ダッシュ・ハイジャンプ　
			(P1.stateno == 520) ||	// ガーキャン
			((P1.stateno >= 550) && (P1.stateno <= 599))	// EXアクション
			){
			GetP_SameParam(P1, P2);
			GetH_SameParam(H1, H2);
			GetS_SameParam(S);
			SameParam();
		}
		//------------
		//
		// 喰らい関連
		//
		//------------
		if ((P1.stateno >= 1000) && (P1.stateno <= 1999)){	// 立ち喰らい
			GetP_SameParam(P1, P2);
			GetH_SameParam(H1, H2);
			GetS_SameParam(S);
			DamParam();
		}

		// チェーン初期化
		if (P1.ctrl){
			P1.Var[1] = 0;
			P1.Var[2] = 0;
			P1.Var[4] = 0;
		}

		switch (P1.stateno){

			//********************
			// 0 立ち
			//********************
		case 0:		// 立ち
			//***************
			//  基本設定
			//***************

			// 立ち設定：操作可否
			P1.SFlg = 0, P1.Lock = 0;
			P1.ctrl = 1;
			P1.YPos = GROUND;
			P1.ignoreG = false;
			if (P1.time == 1){
				VelSet(0, 0);
			}

			// 一応初期化
			DamReset();

			// 立ち の 全体フレーム、54を超えたらリセット
			if (P1.time > ANIMELEM)P1.time = 0;
			
			//***************
			//  行動内容
			//***************

			// 勝利ポーズへ
			if ((P1.time > 30) && (S.roundState == 3) && (P2.stateno == 1070))
				P1.time = 0, P1.More = 1, P1.stateno = 95;

			break;
			//**** 終了 ****//
			//********************
			// 11 しゃがみ
			//********************
		case 11:
			P1.SFlg = 1, P1.ctrl = 1;	//しゃがみ判定に
			VelSet(0, 0);
			// 一応初期化
			DamReset();

			// 投げ無敵付与
			if (P1.Muteki == 1 && P1.time == 0 &&
				P1.mutekiF == 0){
				P1.mutekiF = 2;
				P1.Muteki = 10;
			}
			// 下入力解除で 12 へ
			if (InputPAD(102) == 0){
				P1.time = 0, P1.stateno = 12, P1.More = 1;
			}

			break;
			//********************
			// 12 しゃがみ→立ち
			//********************
		case 12:
			P1.SFlg = 0;	//立ち判定に
			// 最初にロック
			if (P1.time == 0)P1.Lock = 1;
			// 操作が入ったらモーションキャンセル
			if (P1.Key != 0)P1.time = 0, P1.stateno = 0, P1.SFlg = 0, P1.More = 1, P1.Lock = 0;
			if (P1.time > ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.SFlg = 0, P1.More = 1, P1.Lock = 0;
			}

			break;
		case 20:	// 前歩き
			P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
			P1.XVel = P1.C.walkF; //速度を足す
			// SEを鳴らす
			if ((P1.time == 10) || (P1.time == 35))SEStart(7);
			if (P1.time > ANIMELEM)P1.time = 0;

			break;
		case 21:	// 後ろ歩き
			P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
			P1.XVel = P1.C.walkB; //速度を足す
			// SEを鳴らす
			if (P1.time == 20)SEStart(7);
			if (P1.time > ANIMELEM)P1.time = 0;

			break;
		case 30:	// ダッシュ
			P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
			P1.XVel = P1.C.runF[0]; //速度を足す
			
			//if (P1.AnimTime <= 8)P1.XVel = P1.C.runF[0] * (0.1 * (P1.AnimTime + 1)); //速度を足す;
			if (P1.AnimTime <= 8)P1.XVel = P1.C.runF[0] * (0.1 * ((float)P1.AnimTime + 1)); //速度を足す;
			if (P1.AnimTime <= 6)P1.ctrl = 0; //速度を足す
																					 // SEを鳴らす
			if ((P1.time == 14) || (P1.time == 31))SEStart(4);
			if (P1.time > ANIMELEM)P1.time = 0;

			break;
		case 35:	// バックステップ
			P1.SFlg = 0, P1.ctrl = 0, P1.Lock = 0;

			// 無敵
			if (P1.time <= 3){
				P1.Muteki = 1, P1.mutekiF = 0;
			}

			// SEを鳴らす
			if (P1.time == 1)SEStart(5);

			//if ((P1.YPos == GROUND) && (P1.time == 0))P1.YVel = -P1.C.runB[1];
			if(P1.time <= 1)P1.XVel = P1.C.runB[0] * 2.5; //速度を足す

			//-----------------
			// 派生
			//-----------------
			// バクステ
			if ((P1.time >= 1) && (P1.key[4] == 1)){
				P1.stateno = 37;
				P1.time = 0;
				P1.SFlg = 0;
				P1.More = 1;
			}

			// 終了
			if (P1.time >= ANIMELEM){
				P1.stateno = 36;
				P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
			}

			break;

		case 36:	// バクステ硬直
			P1.SFlg = 0, P1.ctrl = 0;

			VelSet(0, 0);

			// 終了
			if (P1.time >= ANIMELEM){
				P1.stateno = 0;
				P1.time = 0;
				P1.SFlg = 0;
			}

			break;

		case 37:	// 派生バクステ
			P1.SFlg = 0, P1.ctrl = 0, P1.Lock = 0;

			// SEを鳴らす
			if (P1.time == 1)SEStart(5);

			//if ((P1.YPos == GROUND) && (P1.time == 0))P1.YVel = -P1.C.runB[1];
			if (P1.time == 2)P1.XVel = P1.C.runB[0] * 2.5; //速度を足す
			else if (P1.time < 2){ P1.XVel = 0; }

			//-----------------
			// 派生
			//-----------------
			// バクステ
			if ((P1.time >= 3) && (P1.key[4] == 1)){
				P1.stateno = 37;
				P1.time = 0;
				P1.SFlg = 0;
				P1.More = 1;
			}

			// 終了
			if (P1.time >= ANIMELEM){
				P1.stateno = 36;
				P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
			}

			break;
		case 90:	// 開幕
			P1.SFlg = 0, P1.ctrl = 0;

			// 最初の位置
			if (P1.time == 0){
				if (P1.muki == 0){
					//初期座標をセット
					P1.XPos = (STAGE_WIDTH / 2) - 366;
				}
				else{ P1.XPos = (STAGE_WIDTH / 2) + 366; }
			}

			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SEを鳴らす
			if (P1.time == 1 || P1.time == 37)SEStart(26);

			// スラ速度セット
			//if (P1.time <= 8 && P1.time > 0)VelSet(9,0);
			//if (P1.time == 14)VelSet(5,0);

			if (P1.time == 4 || P1.time == 40)VelSet(10, 0);
			if (P1.time == 10 || P1.time == 46)VelAdd(2, 0);

			if ((P1.muki == 0) && (P1.XPos > (STAGE_WIDTH / 2) - 120)){
				P1.XPos = (STAGE_WIDTH / 2) - 120;
			}
			else if ((P1.muki == 1) && (P1.XPos < (STAGE_WIDTH / 2) + 120)){
				P1.XPos = (STAGE_WIDTH / 2) + 120;
			}

			// 全体フレームを超えたら立ちへ
			if (P1.time >= P1.animElem[P1.stateno]){
				P1.time = 0;
				P1.stateno = 0;
				// 最初の位置
				if (P1.muki == 0){
					//初期座標をセット
					P1.XPos = (STAGE_WIDTH / 2) - 120;
				}
				else{ P1.XPos = (STAGE_WIDTH / 2) + 120; }
			}

		case 95:	// 勝利
			P1.SFlg = 0, P1.ctrl = 0;

			break;
		case 100:	// ダッシュ
			P1.SFlg = 2;
			// 前ジャンプ の 全体フレーム
			if (P1.time > 1)P1.More = 1, P1.stateno = 0;

			break;
			//********************
			// 200：立ちA
			//********************
		case 200:
			P1.ctrl = 0, P1.SFlg = 0;

			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// 先行入力消す
			// buttonはそのまま
			if (P1.time <= 1)P1.Senkou[1] = 0;
			//[ 通常技の自動振り向き ]
			if (P1.time == 0){
				if ((P1.XPos < P2.XPos) && (P1.muki == 1)){
					P1.muki = 0;
				}
				else if ((P1.XPos > P2.XPos) && (P1.muki == 0)){
					P1.muki = 1;
				}
			}

			// ヒット数セット 
			if (P1.time == 0 && P1.MoveHit == 0){
				P1.MoveHit = 1;	// １回
			}
			// 立ち の 全体フレーム、54を超えたらリセット
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}
			// SEを鳴らす
			if (P1.time == 1){
				SEStart(1);
			}
			// ヒット数セット、発生６フレ
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit == 1)){

				// [ダメージ]
				Damage(46, 0);

				// [ゲージ] 
				Power(40);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 12, 16, 10);
				// [ノックバック]
				HitVel(-3.8, 0, -2.2, -4.6);
				GuardVel(-4.2, -3.4);

				// [ガード属性]
				P1.GuardF = 1;
				// [ヒット時のアニメ]
				P1.HitAnim = 1010;
				// [ヒットサウンド]
				SESet(10, 16);

				// [フォール状態]
				P1.fallF = 1;
			}
			else {
				DamReset();
			}
			// 連打キャンセル
			if (P1.time >= ANIMELEM - 1){
				// 下要素が入ってない
				if ((P1.Senkou[1] > 0) &&
					(P1.button[102] == 0) && (P1.CFlg == 0)){
					P1.More = 1, P1.time = 0, P1.A.damage = 0;
				}
			}
			// ヒット時チェーン
			if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
				// [ジャンプキャンセル]
				if (P1.keyAtt[8]){		// 先行入力効かせてみる
					P1.stateno = 40, P1.More = 1,
						P1.time = 0, P1.A.damage = 0;
				}
				// [通常技]
				if (P1.Senkou[3] > 0){
					// スラ
					if (key(3)){
						P1.stateno = 321, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)){
						P1.stateno = 320, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else{
						P1.stateno = 220, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if (P1.Senkou[2] > 0){		// 先行入力効かせてみる
					// 6B
					if ((key(6)) && (!key(2)) && (!key(8))){
						P1.stateno = 215, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)){
						P1.stateno = 310, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else{
						if (P1.Senkou[1] == 0){
							P1.stateno = 210, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
				}
				else if (P1.Senkou[1] > 0){		
					if (key(2) && (P1.Var[4] == 0)){
						P1.stateno = 300, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
						P1.Var[4]++;
					}
				}
				// [必殺技・ゲージ技]
				SCancel();
			}

			// 投げ
			DelayThrow(1);
			// 3ゲージ
			Delay3Button(850, 3000);

			break;
			
			//********************
			// 210：立ちB
			//********************
		case 210:	//立ちB
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// ヒット数セット 
			if (P1.time == 6){
				P1.MoveHit = 1;	// １回
			}
			if (P1.time == 1)VelSet(2.0, 0);
			// 位置をずらす
			if (AnimElem(2)){
				PosAdd(6, 0);
			}
			if (AnimElem(3)){
				PosAdd(10, 0);
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// ダメージセット
			if ((P1.time >= 3) && (P1.time <= 12)){

				// [ダメージ]
				Damage(82, 0);

				// [ゲージ] 
				Power(94);
				HitTime(8, 18, 20, 14);

				// [ノックバック]
				HitVel(-4.2, 0, -2.0, -4.8);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1000;	// 上段喰らい
				P1.HitSE = 11;
			}
			else {
				DamReset();
			}
			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
					// [ジャンプキャンセル]
					if ((P1.K_Senkou[8]) && (P2.HFlg == 1)) {		// 先行入力効かせてみる
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [通常技]
					if (P1.Senkou[3] > 0){
						
						// スラ
						if (key(3)){
							P1.stateno = 321, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else if (key(2)){
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else{
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					else if (P1.Senkou[2] > 0){
						// 6B
						if ((key(6)) && (!key(2)) && (!key(8))){
							P1.stateno = 215, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						// 下要素を確認
						else if (key(2) && P1.Var[1] == 0){
							P1.stateno = 310, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
							P1.Var[1]++;
						}
					}
					// 先行入力
					else if (P1.Senkou[1] > 0){
						// 連打コンボ
						if (P1.Var[20] == 1){
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// [必殺技・ゲージ技]
					SCancel();
				}
			}
			
			// 投げ
			DelayThrow(2);
			// 3ゲージ
			Delay3Button(850, 3000);

			break;
			//********************
			// 215：6B
			//********************
		case 215:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(0,0);

			// 移動
			if (AnimElem(3))VelAdd(2,0);
			if (AnimElem(6))PosAdd(22, 0);
			if (AnimElem(10))PosAdd(-39, 0);



			// SE
			if (P1.time == 1){
				SEStart(1);
			}
			// ヒット数セット、８フレ
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 33)){

				// [ダメージ]
				Damage(95, 0);

				// [ゲージ] 
				Power(106);
				HitTime(8, 22, 24, 16);

				// [ノックバック]
				HitVel(-4.4, 0, -2.0, -5);
				P1.GuardF = 2;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// 下段喰らい
				P1.A.forceKurai = true;	// 喰らい姿勢を強制
				P1.HitSE = 12;
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 8) && (P1.A.ncTime > 0)){

					// [通常技]
					/*
					if (P1.Senkou[3] > 0){
						if (InputPAD(102) == 0){
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					*/
					// [必殺技・ゲージ技]
					SCancel();
				}
			}

			// 投げ
			DelayThrow(2);
			// 3ゲージ
			Delay3Button(850, 3000);

			break;
			//********************
			// 220：立ちC
			//********************
		case 220:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// 位置をずらす
			if (P1.time == 0){
				PosAdd(6, 0);
			}
			// SEを鳴らす
			if (P1.time == 1)SEStart(3);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;


			// ヒット数セット 発生：26
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			//else{ P1.MoveHit = 0; }

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(100, 0);
				// [ゲージ] 
				Power(120);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 22, 24, 18);
				// [ノックバック]
				HitVel(-3.8, 0, -2.8, -4);
				// [ガード属性]
				P1.GuardF = 1;


				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// 下段喰らい


				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			// [ガード距離]
			P1.GuardDist = 300;

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
					// [必殺技・ゲージ技]
					// 先行入力
					if (P1.Senkou[1] > 0){
						// 連打コンボ
						if (P1.Var[20] == 1){
							P1.stateno = 610, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					SCancel();
				}

			}

			// 3ゲージ
			Delay3Button(850, 3000);

			break;
			//********************
			// 300：しゃがみA
			//********************
		case 300:
			P1.ctrl = 0, P1.SFlg = 1;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// 先行入力消す
			if (P1.time <= 1)P1.Senkou[1] = 0;
			//[ 通常技の自動振り向き ]
			if (P1.time == 0){
				if ((P1.XPos < P2.XPos) && (P1.muki == 1)){
					P1.muki = 0;
				}
				else if ((P1.XPos > P2.XPos) && (P1.muki == 0)){
					P1.muki = 1;
				}
			}
			// SEを鳴らす
			if (P1.time == 1)SEStart(1);
			//  全体フレーム、14を超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;
			// ヒット数セット、発生６フレ
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if ((P1.time >= 1)){

				// [ダメージ]
				Damage(44, 0);

				// [ゲージ] 
				P1.GetPow = 24, P1.GivePow = 12;

				// [ヒットストップ・のけぞり時間]
				HitTime(6, 12, 16, 10);

				// [ノックバック]
				HitVel(-3.4, 0, -1.6, -4.8);
				P1.GuardF = 1;

				P1.HitAnim = 1010;	// 下段喰らい
				P1.HitSE = 10;
			}
			else {
				DamReset();
			}

			// キャンセル
			if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
				if (P1.Senkou[3] > 0){
					// スラ
					if (key(3)){
						P1.stateno = 321, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)){
						P1.stateno = 320, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else{
						P1.stateno = 220, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if (P1.Senkou[2] > 0){		// 先行入力効かせてみる
					// 6B
					if ((key(6)) && (!key(2)) && (!key(8))){
						P1.stateno = 215, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)){
						P1.stateno = 310, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else{
						P1.stateno = 210, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if (P1.Senkou[1] > 0){		// 先行入力効かせてみる
					if (key(2)){
						P1.stateno = 300, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if(P1.Var[4] == 0) {
						P1.stateno = 200, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
						P1.Var[4]++;
					}
				}
				// [必殺技・ゲージ技]
				SCancel();
			}

			// キャンセル、連打キャンセル
			if ((P1.button[1] == 1) && (P1.time >= 9) && (P1.stateno == 300)){
				// 下要素が入ってる
				if (P1.button[102] != 0){
					P1.More = 1, P1.time = 0, P1.A.damage = 0;
				}
			}

			

			// 投げ
			DelayThrow(1);
			

			break;
			//********************
			// 310：しゃがみB
			//********************
		case 310:
			P1.ctrl = 0, P1.SFlg = 1;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// 位置をずらす
			if (P1.time == 5){
				if (P1.muki == 0){
					P1.XPos += 6;
				}
				else{
					P1.XPos -= 6;
				}
			}
			// 位置をずらす
			if (P1.time == 9){
				if (P1.muki == 0){
					P1.XPos -= 6;
				}
				else{
					P1.XPos += 6;
				}
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;
			// ヒット数セット
			if (P1.time == 5){
				P1.MoveHit = 1;	// １回
			}


			// ダメージセット
			if ((P1.time >= 5) && (P1.time <= 10)){
				
				// [ダメージ]
				Damage(80, 0);

				// [ゲージ] 
				Power(88);

				HitTime(8, 18, 20, 14);

				// [ノックバック]
				HitVel(-3.8, 0, -1.8, -4.6);
				P1.GuardF = 3;	// 下段
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1010;	// 下段喰らい
				P1.HitSE = 11;
			}
			else{
				DamReset();
			}
			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
					// [通常技]
					if (P1.Senkou[3] > 0){		// 先行入力効かせてみる
						// スラ
						if (key(3)){
							P1.stateno = 321, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						// 下要素を確認
						else if (key(2)){
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else{
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					else if (P1.Senkou[2] > 0){		// 先行入力効かせてみる
						// 6B
						if ((key(6)) && (!key(2)) && (!key(8))){
							P1.stateno = 215, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						// 下要素を確認
						else if (!key(2) && P1.Var[1] == 0){
							P1.stateno = 210, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							P1.Var[1]++;
						}
					}
					// [必殺技・ゲージ技]
					SCancel();
				}
			}

			// 投げ
			DelayThrow(2);
			// 3ゲージ
			Delay3Button(850, 3000);

			break;
			//********************
			// 320：2Ｃ
			//********************
		case 320:
			P1.ctrl = 0, P1.SFlg = 1;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);

			// 位置をずらす
			if (P1.time == 0){
				PosAdd(5, 0);
			}

			// SEを鳴らす
			if (P1.time == 1)SEStart(3);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// ヒット数セット 10フレ
			if (P1.time == 8){
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 22)){

				// [ダメージ]
				Damage(100, 0);
				// [ゲージ] 
				Power(120);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 22, 24, 18);
				// [ノックバック]
				HitVel(-4.0, 0, -1.8, -4.0);
				// [ガード属性]
				P1.GuardF = 1;


				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// 下段喰らい


				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
					// [ジャンプキャンセル]
					if ((P1.K_Senkou[8]) && (P2.HFlg == 1)){		// 先行入力効かせてみる
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [必殺技・ゲージ技]
					SCancel();
				}
			}

			// 3ゲージ
			Delay3Button(850, 3000);

			break;
			//********************
			// 321：スライディング
			//********************
		case 321:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SEを鳴らす
			if (P1.time == 1)SEStart(26);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;
			
			// スラ速度セット
			//if (P1.time <= 8 && P1.time > 0)VelSet(9,0);
			//if (P1.time == 14)VelSet(5,0);

			if (P1.time == 4)VelSet(10, 0);
			if (P1.time == 10)VelAdd(2.2, 0);

			// ヒット数セット 10フレ
			if (P1.time == 8){
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if ((P1.time >= 8) && (P1.time <= 22)){

				// [ダメージ]
				Damage(100, 0);
				// [ゲージ] 
				Power(120);
				HitTime(10, 40, 40, 18);
				// [ノックバック]
				HitVel(-1.8, -3.6, -2.2, -3.5);
				GuardVel(-3.6, -1.4);
				P1.GuardF = 3;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// 空中喰らい(確定ダウン)
				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			
			// 必キャン
			if (P1.StopTime == 0){
				// ヒット時キャンセル
				if ((P1.CFlg) && (P1.time >= 1)){
					// [必殺技・ゲージ技]
					if(P1.scTime > 0)HCancel();
					if(P2.stateno >= 1000)SCancel();	// ヒット時のみ可能
				}
			}

			//-----------------
			// 派生
			//-----------------
			// スラ
			if ((P1.button[3] == 1) && (P1.time >= ANIMELEM - 16) && (!P1.CFlg)){
				// 下要素が入ってる
				if (key(2)){
					P1.More = 1, P1.time = 0, P1.A.damage = 0;
					P1.stateno = 321;
				}
			}

			// 3ゲージ
			Delay3Button(850, 3000);

			break;
			//********************
			// 400：ジャンプA
			//********************
		case 400:
			P1.ctrl = 0, P1.SFlg = 2;
			if (P1.time == 1)SEStart(1);
			// 先行入力消す
			if (P1.time <= 1)P1.Senkou[1] = 0;
			// ヒット数セット 6フレ
			if (P1.time == 4)P1.MoveHit = 1;	// １回
			// ダメージセット
			if ((P1.time >= 4) && (P1.time <= 6)){

				// [ダメージ]
				Damage(42, 0);
				// [ゲージ] 
				Power(24);

				// [ヒットストップ・のけぞり時間]
				HitTime(5, 12, 16, 10);
				// [ノックバック]
				HitVel(-4.2, 0, -1.4, -4.0);
				// ガード判定
				if (P1.YVel >= 0.0)P1.GuardF = 2;
				else { P1.GuardF = 1; }
				P1.HitAnim = 1000;
				P1.HitSE = 10;
			}
			else {
				DamReset();
			}
			/*
			// 操作可能フレーム
			if (P1.time >= 19)P1.ctrl = 1;
			*/
			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 4) && (P1.scTime > 0)){
					if (P1.Senkou[4] > 0){		// 先行入力効かせてみる
						P1.stateno = 430, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[3] > 0){		// 先行入力効かせてみる
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[2] > 0){		// 先行入力効かせてみる
						P1.stateno = 410, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[1] > 0){		// 先行入力効かせてみる
						P1.stateno = 400, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [空中必殺技]
					ACancel();
				}

				// [ジャンプキャンセル]
				if ((P1.CFlg) && (P1.time >= 4) && (P1.AirJump > 0)){
					if (P1.K_Senkou[8] >= 1){		// 先行入力効かせておく
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}
			/*
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM){
				P1.ctrl = 1, P1.More = 1, 
				P1.stateno = 46, P1.time = 0;
			}
			*/
			// 投げ
			DelayThrow(1);

			break;
			//********************
			// 410：ジャンプB
			//********************
		case 410:
			P1.ctrl = 0, P1.SFlg = 2;
			// SE
			if (P1.time == 1)SEStart(1);
			// ヒット数セット
			if (P1.time == 5)P1.MoveHit = 1;	// １回
			// ダメージセット
			if ((P1.time >= 5) && (P1.time <= 9)){
			
				// [ダメージ]
				Damage(75, 0);

				// [ゲージ] 
				Power(80);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 16, 18, 14);
				// [ノックバック]
				HitVel(-4.6, 0, -1.8, -4.0);
				// ガード判定
				if (P1.YVel >= 0.0)P1.GuardF = 2;
				else { P1.GuardF = 1; }
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				P1.HitSE = 11;
			}
			else{
				DamReset();
			}
			/*
			// 操作可能フレーム
			if (P1.time >= 23){
				P1.ctrl = 1;
			}
			*/
			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 5) && (P1.scTime > 0)){
					if (P1.Senkou[4] > 0){		// 先行入力効かせてみる
						P1.stateno = 430, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[3] > 0){		// 先行入力効かせてみる
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					
					// [空中必殺技]
					ACancel();
				}

				// [ジャンプキャンセル]
				if ((P1.CFlg) && (P1.time >= 5) && (P1.AirJump > 0)){
					if (P1.K_Senkou[8] >= 1){		// 先行入力効かせてみる
						//P1.AirJump -= 1;
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// 投げ
			DelayThrow(2);

			break;
			//********************
			// 420：ジャンプC
			//********************
		case 420:
			P1.ctrl = 0, P1.SFlg = 2;
			// SE
			if (P1.time == 1)SEStart(2);
			// ヒット数セット
			if (P1.time == 6)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 5フレ
			if ((P1.time >= 6) && (P1.time <= 11)){

				// [ダメージ]
				Damage(96, 0);

				// [ゲージ] 
				Power(102);

				// [ヒットストップ・のけぞり時間]
				HitTime(8, 17, 19, 14);

				// [ノックバック]
				HitVel(-4.8, 0, -2.0, -3.0);
				// ガード判定
				if (P1.YVel >= 0.0)P1.GuardF = 2;
				else { P1.GuardF = 1; }
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				P1.HitSE = 12;

			}
			else {
				DamReset();
			}
			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 6) && (P1.scTime > 0)){
					if (P1.Senkou[4] > 0){		// 先行入力効かせてみる
						P1.stateno = 430, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}

					// [空中必殺技]
					ACancel();
				}

			}
			break;
			//********************
			// 430：ジャンプD
			//********************
		case 430:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(3);

			if (P1.time == 0)VelSet(0,-0.8);
			if (P1.time == 12)VelSet(7.6,7);

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 5フレ
			if (P1.time >= 1){

				// [ダメージ]
				if (P1.AirJump == 0)Damage(118, 0);
				else{ Damage(105, 0); }
				// [ゲージ] 
				Power(120);

				// [ヒットストップ・のけぞり時間]
				HitTime(8, 28, 48, 14);

				// [ノックバック]
				//HitVel(-2.8, -3.6, -3.6, 6.0);
				HitVel(-2.8, -3.6, -3.6, 5.2);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 2;
				P1.HitAnim = 1000;
				P1.HitSE = 12;
				// バウンド
				if (P1.HitCount == 0){
					P1.A.boundLevel = 2;
					P1.A.bound_xvel = -0.9;
					P1.A.bound_yvel = -3.6;
				}
				else{
					P1.A.boundLevel = 1;
					P1.A.bound_xvel = -0.9;
					P1.A.bound_yvel = -3.4;
				}
				P1.A.hosei_K = 0.9;
			}
			else {
				//DamReset();
			}
			if (//(P2.stateno >= 1000) && 
				(P1.CFlg == 1) && (P1.time > 1)){
				P1.stateno = 432, P1.More = 1, P1.time = 0;
				P1.SFlg = 2;
				VelSet(0, 0);
				P1.ignoreG = true;
			}

			if (P1.YPos + P1.YVel * 2 >= GROUND){
				P1.stateno = 431, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;
			//********************
			// 431：ジャンプD着地
			//********************
		case 431:	
			P1.SFlg = 0, P1.A.damage = 0, P1.ctrl = 0;
			P1.YPos = GROUND;
			P1.ignoreG = false;
			P1.D.counter = 0;
			VelSet(0, 0);
			// SEを鳴らす
			if (P1.time == 1)SEStart(6);
			// 最初にロック
			if (P1.time == 0)P1.Lock = 1;
			// 前ジャンプ の 全体フレーム
			if (P1.time >= ANIMELEM)P1.More = 1, P1.stateno = 0,
				P1.time = 0, P1.Lock = 0;
			break;
			//********************
			// 432：ジャンプDヒット..後退
			//********************
		case 432:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;

			if (P1.time == 0)VelSet(-2.6, -3.4);

			VelAdd(0, P1.C.yAccel);

			if ((P1.time >= 1) && (P2.HFlg) && (P1.AirJump > 0)){
				if (P1.K_Senkou[8] > 0){
					P1.SFlg = 2;
					P1.stateno = 45;
					P1.time = 0;
					P1.ignoreG = false;
					P1.AirJump -= 1;
					P1.More = 1;
				}
				// [ジャンプキャンセル]
				if ((P1.CFlg) && (P1.time >= 5) && (P1.AirJump > 0)){
					if (P1.K_Senkou[8] >= 1){		// 先行入力効かせてみる
						P1.AirJump -= 1;
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// ヒット時
			if ((P1.time >= 4 + 1) && (P2.HFlg) && (P2.stateno >= 1000)){
				P1.ctrl = 1;
			}

			if ((P1.YPos + P1.YVel * 2 >= GROUND) && (P1.time >= 4)){
				P1.stateno = 47, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				P1.YVel = 0;
				P1.ignoreG = false;
			}

			// キャンセル
			// 応急処置
			/*
			if (P1.StopTime == 0){
				// [ジャンプキャンセル]
				if ((P1.time >= 0) && (P1.AirJump > 0) && (P2.HFlg)){
					if (P1.K_Senkou[8] >= 1){		// 先行入力効かせてみる
						P1.ignoreG = false;
						P1.AirJump -= 1;
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}
			*/
			/*
			if (P1.time >= 20) {
				P1.ctrl = 1, P1.SFlg = 2;
				P1.stateno = 46;
				P1.time = 0;
				P1.ignoreG = false;
			}
			*/
			break;
			//********************
			// 500：地上投げ
			//********************
		case 500:
			P1.ctrl = 0, P1.SFlg = 0;
			// 投げ方向設定
			if (P1.time == 1){
				P1.throwSide = P1.muki;
				P1.A.throwTurn = false;
				if (P1.keyAtt[4]){
					P1.A.throwTurn = true;
				}
				P1.XVel = 0;
			}


			// SE
			if (P1.time == 1)SEStart(2);
			// ヒット数セット、
			// ガード時は無効
			if ((P1.time == 4) && (P2.stateno <= 999) && (P1.XPos - P2.XPos < 170 && P2.XPos - P1.XPos < 170))	// 喰らい・空中状態じゃない
			{
				P1.MoveHit = 1;	// １回
				// どっちも投げタイミングが同じなら、2P負ける
				//if ((P1.stateno == P2.stateno) && (P1.time == P2.time) && (P1.PSide == 2))
				//	P1.MoveHit = 0;
			}

			// ダメージセット、持続 1フレ
			if (P1.time == 4){

				// [ダメージ]
				Damage(0, 0);
				// [ゲージ] 
				Power(0, 0);
				HitTime(0, 3, 3, 0);

				P1.GX = 0, P1.GY = 0;
				P1.GuardF = 0;
				P1.attF = 2;

				P1.HitAnim = 1060;
				P1.HitSE = 9;
				P1.A.hitEff = 2;
			}

			// [ガード距離]
			P1.GuardDist = 0;


			// 当たったら変更
			if ((P1.CFlg) && (P1.time == 8)){
				P1.stateno = 505;
				P1.time = 0, P2.time = 0;
				P1.More = 1;
				P1.A.damage = 0;
				
				P1.StopTime = 6;
				
				// 投げの向きを変更
				if (P1.A.throwTurn){
					if (P1.muki == 0){
						P1.muki = 1, P2.muki = 0;
					}
					else if (P1.muki == 1){
						P1.muki = 0, P2.muki = 1;
					}
				}


			}
			// 全体フレームを超えたらリセット
			if (P1.time >= 32)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// 3ゲージ
			Delay3Button(850, 3000);

			break;
			//********************
			// 505：投げ（演出）
			//********************
		case 505:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.Muteki = 1, P1.mutekiF = 0;


			P1.XVel = 0;
			// 距離をとる
			if (P1.time == 0){

				int dist = 86;
				if (P1.muki == 0){
					P2.XPos = P1.XPos + dist;
					// もし距離が離れなかったら
					if (P2.XPos > STAGE_WIDTH - GAMENHAJI){
						P2.XPos = STAGE_WIDTH - GAMENHAJI;
						P1.XPos = P2.XPos - dist;
					}
				}
				else{
					P2.XPos = P1.XPos - dist;
					if (P2.XPos < GAMENHAJI){
						P2.XPos = GAMENHAJI;
						P1.XPos = P2.XPos + dist;
					}
				}
				
				P2.YVel = 0;
				P2.YPos = GROUND;
			}


			// 距離を離す
			if (AnimElem(8))P1.XVel = 4.2;
			if (AnimElem(9))P1.XVel = 3.2;
			// ヒット音、エフェクト
			if (P1.time == 7){
				SEStart(11);
				EffStart(P1.A.hitEff, (int)P2.XPos, (int)P2.YPos, 20 + GetRand(10), -(P2.ySize / 1.5),
					P1.A.heSizeX, P1.A.heSizeY, P2.muki);
				// 距離を離す
				P2.XVel = -3.5;
			}
			// 2P無敵
			if (P1.time < 33)P2.Muteki = 1, P2.mutekiF = 0;


			// ヒット数セット
			if (P1.time == 34)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 1フレ
			if (P1.time == 34){

				// [攻撃判定セット]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [ダメージ]
				Damage(0, 160);
				P1.A.hosei_K = 0.5;
				// [ゲージ] 
				Power(270);
				HitTime(6, 45, 45, 0);
				// [ノックバック]
				HitVel(-2.6, -3.4, 0, 0);
				P1.HitAnim = 1030;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitSE = 12;
				
			}
			else {
				DamReset();
			}
			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [必殺技・ゲージ技]
					HCancel();
				}
			}

			// 投げ抜け
			ThrowReject();

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			break;
			//********************
			// 510 空中投げ
			//********************
		case 510:	// 空中投げ
			P1.ctrl = 0, P1.SFlg = 2;

			// 投げ方向設定
			if (P1.time == 1){
				P1.throwSide = P1.muki;
				P1.A.throwTurn = false;
				if (P1.keyAtt[4]) {
					P1.A.throwTurn = true;
				}
			}
			// SEを鳴らす
			if (P1.time == 1)SEStart(1);
			// ヒット準備
			// ヒット数セット、
			if (P1.time == 4)	// 投げのタイミングが被ってない
			{
				DamReset();
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット、持続 1フレ
			if (P1.time == 4){

				// [ダメージ]
				Damage(0, 0);
				// [ゲージ] 
				Power(0, 0);


				HitTime(4, 16, 20, 0);
				// [ノックバック]
				HitVel(0, 0, 0, -5);
				P1.GuardF = 0;
				P1.attF = 2;
				// [ガード距離]
				P1.GuardDist = 0;

				P1.HitAnim = 1065;
				P1.HitSE = 9;
				P1.A.hitEff = 2;
			}
			// 当たったら変更
			if ((P1.CFlg) && (P1.time >= 5)){
				P1.stateno = 515;
				P1.time = 0, P2.time = 0;
				P1.More = 1;
				P1.A.damage = 0;
				// 投げの向きを変更
				if (P1.A.throwTurn){
					if (P1.muki == 0){
						P1.muki = 1, P2.muki = 0;
					}
					else if (P1.muki == 1){
						P1.muki = 0, P2.muki = 1;
					}
				}
			}
			break;

			//********************
			// 515 空投げ演出
			//********************
		case 515:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.Muteki = 1, P1.mutekiF = 0;
			P1.A.damage = 0, P1.MoveHit = 1;
			P1.WAtt[0] = 0, P1.HAtt[0] = 0;
			P1.XAtt[0] = 0, P1.YAtt[0] = 0;
			P1.XVel = 0;
			P1.ignoreG = true;

			// バグ防止の浮き
			if ((P1.time == 0) && (P1.YPos + 15 >= GROUND)){
				P1.YPos = GROUND - 15;
			}

			// 距離をとる
			if (P1.time == 0){
				P1.YPos -= 2;
				P2.YPos = P1.YPos - 1;

				int dist = P2.xSize + 5;
				if (P1.muki == 0){
					P2.XPos = P1.XPos + dist;
					// もし距離が離れなかったら
					if (P2.XPos > STAGE_WIDTH - GAMENHAJI){
						P2.XPos = STAGE_WIDTH - GAMENHAJI;
						P1.XPos = P2.XPos - dist;
					}
				}
				else{
					P2.XPos = P1.XPos - dist;
					if (P2.XPos < GAMENHAJI){
						P2.XPos = GAMENHAJI;
						P1.XPos = P2.XPos + dist;
					}
				}
				P1.YVel = -4;
				P2.YVel = -4;
			}
			// 重力加算
			if (P1.time > 16){
				P1.YVel += GRAVITY + 0.05;
			}
			// 無敵終了
			else if (P1.time == 16){
				P1.YVel += GRAVITY;
				P2.YVel += GRAVITY;
			}
			// ついでに無敵時間
			else {
				P2.Muteki = 1, P2.mutekiF = 0;
				P2.time = 0;
				P1.YVel += GRAVITY;
				P2.YVel += GRAVITY;
			}
			// SEを鳴らす
			if (P1.time == 15)SEStart(10);

			// ヒット数セット
			if (P1.time == 15)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 1フレ
			if (P1.time == 16){

				// 床バウンド
				//P1.A.boundLevel = 1;
				// [攻撃判定セット]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [ダメージ]
				Damage(150, 10);
				// [ゲージ] 
				Power(270);

				HitTime(6, 100, 100, 0);
				// [ノックバック]
				HitVel(-4.5, 9, -4.5, 9);
				P1.HitAnim = 1030;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitSE = 11;


			}
			else{
				DamReset();
			}

			// 姿勢変更
			if (P1.time == 17)P2.time = 20;

			// 投げ抜け
			ThrowReject();

			// 全体フレームを超えたらリセット
			if (P1.YPos + P1.YVel * 2 >= GROUND){
				P1.time = 0, P1.stateno = 47,
					P1.YPos = GROUND, P1.YVel = 0,
					P1.ctrl = 0, P1.SFlg = 0;
			}
			break;
			
			//********************
			// 600 スウィフト1
			//********************
		case 600:	// Aブメ
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
				P1.Var[2]++;
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 5){
				P1.XVel = 5.4;
			}

			/*
			if (P1.time == 5){
			P1.XVel = 13;
			}
			*/
			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [ダメージ]
				Damage(80, 0);
				// [ゲージ] 
				Power(90);
				HitTime(8, 24, 24, 22);
				// [ノックバック]
				HitVel(-2.6, 0, -2.2, -2.0);
				P1.A.guard_gx = -5.0;
				P1.HitAnim = 1000;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 2;	// 地上受け身可能
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1)){
					// [ゲージ技]
					HCancel();
					if ((P1.Var[20] > 0) && (P1.Var[2] < 3) && (P1.scTime > 0))SCancel();
					// Ｄ派生技
					if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)){		// 先行入力効かせてみる
						if ((P1.cmd[2]) && (P1.Power >= 500))P1.stateno = 700;
						else if ((P1.keyAtt[6]) && (!P1.keyAtt[8]) && (!P1.keyAtt[2]))P1.stateno = 603;
						else if (P1.keyAtt[2])P1.stateno = 604;
						else{ P1.stateno = 600 + P1.Var[2]; }
						P1.More = 1;
						P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 601 スウィフト2
			//********************
		case 601:	// Aブメ
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = -2.8;
				P1.Var[2]++;
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(86, 0);
				// [ゲージ] 
				Power(95);
				HitTime(8, 24, 24, 22);
				// [ノックバック]
				HitVel(5.6, 0, 2.0, -2.0);
				P1.HitAnim = 1000;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 2;	// 地上受け身可能
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1)){
					// [ゲージ技]
					HCancel();
					if ((P1.Var[20] > 0) && (P1.Var[2] < 3) && (P1.scTime > 0))SCancel();
					// Ｄ派生技
					if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)){		// 先行入力効かせてみる
						if ((P1.cmd[2]) && (P1.Power >= 500))P1.stateno = 700;
						else if ((P1.keyAtt[6]) && (!P1.keyAtt[8]) && (!P1.keyAtt[2]))P1.stateno = 603;
						else if (P1.keyAtt[2])P1.stateno = 604;
						else{ P1.stateno = 600 + P1.Var[2]; }
						P1.More = 1;
						P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// スウィフト3
			//********************
		case 602:
			P1.ctrl = 0, P1.SFlg = 2;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 1.8;
				P1.YVel = -4.5;
				P1.ignoreG = true;
				P1.Var[2]++;
			}

			P1.YVel += 0.3;

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}


			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [ダメージ]
				Damage(85, 5);
				// [ゲージ] 
				Power(100);
				HitTime(8, 32, 38, 18);
				// [ノックバック]
				HitVel(-3.8, -3.8, -3.6, -2.8);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1)){
					/*
					if ((P1.Senkou[4] > 0)){
						//&& (P1.Var[20] > 1)) {		// 先行入力効かせてみる
						P1.stateno = 430, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					*/
					// [ゲージ技]
					ACancel();
				}
			}

			// 着地
			if ((P1.time > 0) && (P1.YVel > 0) && (P1.YPos + P1.YVel * 2 >= GROUND)){
				P1.time = 0, P1.stateno = 625, P1.More = 1,
					P1.ctrl = 0, P1.ignoreG = false;
			}

			break;
			//********************
			// 603 6D(中段)
			//********************
		case 603:	// Aブメ
			P1.ctrl = 0;
			if(P1.time == 0)P1.SFlg = 2;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 1.2;
				P1.YVel = -4.4;
				P1.ignoreG = true;
				P1.Var[2]++;
				P1.Var[3] = 0;
			}

			if(P1.YPos != GROUND)P1.YVel += GRAVITY;

			// ホールド
			/*
			if ((P1.time == 10) && (P1.button[4] > 10)){
				P1.Var[3] = 1;
				VelAdd(-0.2, GRAVITY);
			}
			*/

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(95, 0);
				// [ゲージ] 
				Power(110);
				HitTime(8, 28, 28, 22);
				// [ノックバック]
				HitVel(-3.0, 0, -2.4, -2.0);
				P1.A.guard_gx = -3.8;
				P1.HitAnim = 1000;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 2;	// 地上受け身可能
				// [ガード属性]
				P1.GuardF = 2;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1)){
					// [ゲージ技]
					HCancel();
					if ((P1.Var[20] > 0) && (P1.Var[2] < 3) && (P1.scTime > 0))SCancel();
					// Ｄ派生技
					if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)){		// 先行入力効かせてみる
						if ((P1.cmd[2]) && (P1.Power >= 500))P1.stateno = 700;
					    else if (P1.keyAtt[2])P1.stateno = 604;
						else{ P1.stateno = 600 + P1.Var[2]; }
						P1.More = 1;
						P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// 着地
			if ((P1.time > 0) && (P1.YVel > 0) && (P1.YPos + P1.YVel * 2 >= GROUND)){
				P1.YPos = GROUND;
				P1.YVel = 0;
				P1.XVel = 0;
				P1.ignoreG = false;
				P1.SFlg = 0;
				// フェイント版
				if (P1.Var[3] != 0){
					P1.time = 0, P1.stateno = 605, P1.ctrl = 0;
					P1.SFlg = 1;
					P1.More = 1;
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 604 2D(下段)
			//********************
		case 604:	// Aブメ
			P1.ctrl = 0, P1.SFlg = 1;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = -0.4;
				P1.Var[2]++;
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 12){
				PosAdd(-4, 0);
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(98, 0);
				// [ゲージ] 
				Power(108);
				HitTime(8, 32, 38, 23);
				// [ノックバック]
				HitVel(-2.6, -3.6, -2.4, -3.0);
				P1.A.guard_gx = -4.4;
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 2;
				// [ガード属性]
				P1.GuardF = 3;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1)){
					// [ゲージ技]
					HCancel();
					if ((P1.Var[20] > 0) && (P1.Var[2] < 3) && (P1.scTime > 0))SCancel();
					// Ｄ派生技
					if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)){		// 先行入力効かせてみる
						if ((P1.cmd[2]) && (P1.Power >= 500))P1.stateno = 700;
						else if ((P1.keyAtt[6]) && (!P1.keyAtt[8]) && (!P1.keyAtt[2]))P1.stateno = 603;
						else{ P1.stateno = 600 + P1.Var[2]; }
						P1.More = 1;
						P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 11, P1.ctrl = 1;

			break;
			//********************
			// 605 フェイント
			//********************
		case 605:
			P1.ctrl = 0, P1.SFlg = 1;
			P1.ignoreG = false;
			P1.YPos = GROUND;

			VelSet(0, 0);
			// SEを鳴らす
			if (P1.time == 1){
				SEStart(6);
			}

			// 終了
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 11, P1.ctrl = 1;
			}
			break;
			//********************
			// 610 スピナー
			//********************
		case 610:	
			if (((P1.Senkou[3] > P1.Senkou[1]) || (P1.Senkou[2] > P1.Senkou[1])) &&
				(P1.time == 0)){
				P1.stateno++;
				P1.More = 1, P1.time = 0;
				P1.ctrl = 0;
			}
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
			}
			
			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 5){
				P1.XVel = 13;
			}
			
			/*
			if (P1.time == 5){
				P1.XVel = 13;
			}
			*/

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(100, 0);
				// [ゲージ] 
				Power(120);
				HitTime(2, 53, 60, 24);
				// [ノックバック]
				HitVel(-2.4, -3.5, -2.6, -2.2);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;
				P1.A.wallLevel = 2;

			}
			else {
				DamReset();
			}

			// キャンセル
			if (P1.StopTime == 0){
				// ヒット時キャンセル
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// 先行入力
					if (P1.Senkou[1] > 0){
						// 連打コンボ
						if (P1.Var[20] == 1){
							if (P1.Power >= 1000){
								P1.stateno = 800, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							}
						}
					}
					// [ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			
			break;

			//********************
			// 611 Bスピナー
			//********************
		case 611:
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 0){
				P1.XVel = 0;
			}
			if (P1.time == 1)SEStart(21);

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 6){
				P1.XVel = 17;
			}

			/*
			if (P1.time == 5){
			P1.XVel = 13;
			}
			*/

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(116, 0);
				// [ゲージ] 
				Power(142);
				HitTime(2, 53, 60, 24);
				// [ノックバック]
				HitVel(-2.9, -3.6, -2.8, -2.6);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 0.3;
				P1.A.wall_yvel = -6.6;

			}
			else {
				DamReset();
			}

			// キャンセル
			if (P1.StopTime == 0){
				// ヒット時キャンセル
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;

			//********************
			// 620 アッパー
			//********************
		case 620:
			// ステート変更
			if (((P1.Senkou[3] > P1.Senkou[1]) || (P1.Senkou[2] > P1.Senkou[1])) &&
				(P1.time == 0)){
				P1.stateno++;
				P1.More = 1, P1.time = 0;
				P1.ctrl = 0;
			}
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;

			// SEを鳴らす
			if (P1.time == 0){
				SEStart(21);
				VelSet(0,0);
			}
			if (P1.time == 1){
				VelSet(0, 0);
			}

			if (AnimElem(3)){
				VelSet(1.6, -4.7);
			}

			// アニメ時間
			if (P1.time == ANIMELEM - 1){
				VelSet(0.1, -0.5);
			}
			if (P1.time > ANIMELEM - 1){
				VelAdd(0, 0.4);
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [ダメージ]
				Damage(105, 10);
				// [ゲージ] 
				Power(145);

				HitTime(8, 100, 100, 12);
				// [ノックバック]
				HitVel(-1.8, -5.4, -1.8, -4.8);
				
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;

				P1.HitSE = 13;
			}

			// キャンセル
			if (P1.StopTime == 0){
				// ヒット時キャンセル
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > SC_TIME - 4)){
					// [ゲージ技]
					HCancel();
					// 着地する
					if (P1.stateno != 620){
						P1.SFlg = 0;
						P1.YPos = GROUND;
						VelSet(0, 0);
					}
				}
				// ヒット時キャンセル
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [ゲージ技]
					ACancel();
				}
			}

			// 着地
			if (P1.YPos > GROUND){
				P1.time = 0, P1.stateno = 625, P1.ctrl = 0;
				P1.More = 1;
			}
			break;

			//********************
			// 621 Bアッパー
			//********************
		case 621:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			P1.D.counter = true;
			P1.D.fatal = true;

			// SEを鳴らす
			if (P1.time == 0){
				SEStart(21);
				VelSet(0, 0);
			}
			if (P1.time == 1){
				VelSet(0, 0);
			}

			if (AnimElem(3)){
				VelSet(1.6, -6.1);
			}

			// アニメ時間
			if (P1.time == ANIMELEM - 1){
				VelSet(0.1, -0.5);
			}
			if (P1.time > ANIMELEM - 1){
				VelAdd(0, 0.4);
			}

			// 無敵
			if (P1.time <= 6){
				P1.mutekiF = 0;
				P1.Muteki = 1;
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [ダメージ]
				Damage(120, 0);
				// [ゲージ] 
				Power(150);

				HitTime(8, 100, 100, 12);
				// [ノックバック]
				HitVel(-1.7, -5.4, -1.7, -4.8);

				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 2;	// 地上受け身可能
				// [ガード属性]
				P1.GuardF = 1;

				P1.HitSE = 13;
			}

			// キャンセル
			if (P1.StopTime == 0){
				// ヒット時キャンセル
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > SC_TIME - 4)){
					// [ゲージ技]
					//HCancel();
					// 着地する
					if (P1.stateno != 621){
						P1.SFlg = 0;
						P1.YPos = GROUND;
						VelSet(0, 0);
					}
				}
			}

			// 着地
			if (P1.YPos > GROUND){
				P1.time = 0, P1.stateno = 625, P1.ctrl = 0;
				P1.More = 1;
			}
			break;

			//********************
			// 625 着地
			//********************
		case 625:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.ignoreG = false;
			P1.YPos = GROUND;
			P1.D.counter = true;
			P1.D.fatal = true;

			VelSet(0, 0);
			// SEを鳴らす
			if (P1.time == 1){
				SEStart(6);
			}

			// 終了
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}
			break;
			//********************
			// 630 スウィフト1
			//********************
		case 630:	// Aブメ
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			/*
			if (P1.time == 5){
			P1.XVel = 13;
			}
			*/

			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [ダメージ]
				Damage(100, 0);
				// [ゲージ] 
				Power(120);
				HitTime(8, 26, 28, 22);
				// [ノックバック]
				HitVel(-2.8, -2.6, -2.4, -2.0);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 2;	// 地上受け身可能
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [ゲージ技]
					HCancel();

				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 650 エンハンス
			//********************
		case 650:	// エンハ
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			if (AnimElem(2)){
				//P1.MoveHit = 1;	// １回
				SEStart(22);
			}

			if (P1.time == 33){
				if(P1.Var[20] < 3)P1.Var[20] += 1;
				SEStart(31);
				P1.colorCTime = 10;
				P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 打ち上げ
			//********************
		case 700:	// Aブメ
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);	
				P1.Var[2]++;
			}

			if (AnimElem(2))PosAdd(8, 0);

			// ゲージ消費
			if (P1.time == 1){
				if (P1.Var[20] < 3)P1.Power -= 500;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(80, 20);
				// [ゲージ] 
				Power(0, 90);
				//HitTime(8, 18, 20, 14);
				HitTime(8, 22, 24, 18);
				// [ノックバック]
				HitVel(-0.6, -6.8, -0.6, -6.0);
				GuardVel(-4.8, -1.8);
				//P1.A.guard_gx = -4.6;
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 2;	// 地上受け身可能
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1)){
					// [ジャンプキャンセル]
					if ((P1.K_Senkou[8]) && (P2.HFlg)){		// 先行入力効かせてみる
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// Ｄ派生技
					if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)){		// 先行入力効かせてみる
						if ((P1.keyAtt[6]) && (!P1.keyAtt[8]) && (!P1.keyAtt[2]))P1.stateno = 603;
						else if (P1.keyAtt[2])P1.stateno = 604;
						else{ P1.stateno = 600 + P1.Var[2]; }
						P1.More = 1;
						P1.time = 0, P1.A.damage = 0;
					}
					// [ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 800 ハードパンチ
			//********************
		case 800:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			// ゲージ消費
			if (P1.time == 0){
				P1.Power -= 1000;
				S.StopTime = 10;
				S.Anten = 25;
				SEStart(25);
				// 中段エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
				afterimageSetting(P1.PSide, 30, 128, 128, 250);
			}

			// SEを鳴らす、移動
			if (P1.time == 1){
				SEStart(21);
				P1.XVel = 0;
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 205, P1.colorC[2] = 0;
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}


			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [ダメージ]
				Damage(140, 80);
				// [ゲージ] 
				Power(0, 310);
				HitTime(10, 60, 60, 24);
				// [ノックバック]
				HitVel(-4.2, -3.8, -3.8, -3.4);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 14;
				// エフェクト
				HitEff(1, 1, 1);
				// 地震エフェクト
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}
			else {
				DamReset();
			}


			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}
			break;
			//********************
			// ハイドロストーム
			//********************
		case 810:
			P1.ctrl = 0, P1.SFlg = 0;

			// ゲージ消費
			if (P1.time == 0){
				P1.Power -= 2000;
				S.StopTime = 15;
				S.Anten = 15;
				SEStart(25);
				// 中段エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}
			// ヘルパー変更.[2]、発射前なら変更
			if (P1.time == 45){
				if (!H1[1].var){
					H1[1].var = true;
					H1[1].time = 0;
					H1[1].stateno = 2100;
				}
			}


			// 終了
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 820 EXアッパー
			//********************
		case 820:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			P1.A.gaugeHosei = true;
			P1.D.counter = true;
			P1.D.fatal = true;

			// ゲージ消費
			if (P1.time == 0){
				P1.Power -= 1000;
				afterimageSetting(P1.PSide, 50, 128, 128, 250);
				S.StopTime = 15;
				S.Anten = 15;
				SEStart(25);
				// 中段エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}

			// SEを鳴らす
			if (P1.time == 0){
				SEStart(21);
				VelSet(0, 0);
			}
			if (P1.time == 1){
				VelSet(0, 0);
			}

			if (AnimElem(3)){
				VelSet(3.4, -7.2);
			}

			// アニメ時間
			if (P1.time == ANIMELEM - 1){
				VelSet(0.1, -0.5);
			}
			if (P1.time > ANIMELEM - 1){
				VelAdd(0, 0.4);
			}

			// 無敵
			if (P1.time <= 6){
				P1.mutekiF = 0;
				P1.Muteki = 2;
			}

			if (AnimElem(2) || AnimElem(3) || AnimElem(4)){
				P1.MoveHit = 1;
				// [ノックバック]
				
				if (AnimElem(4)){
					// [ノックバック]
					HitVel(-1.2, -4.2, -1.2, -4.0);
					P1.A.yaccel = 0.2;
				}
				else{ HitVel(-1.2, -6.8, -1.2, -6.8); }
			}
			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(75, 0);
				// [ゲージ] 
				Power(0,70);

				HitTime(8, 100, 100, 12);
				// [ノックバック]
				//HitVel(-1.4, -5.6, -1.4, -5.4);

				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 2;	// 地上受け身可能
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;
			}

			// 叩きつけへ
			if ((P1.CFlg) && (P1.ignoreG) && (P2.stateno >= 1000) && (P1.time >= ANIMELEM)){
				P1.time = 0, P1.stateno = 821, P1.ctrl = 0;
				P1.More = 1;
				P1.A.damage = 0;
			}

			// 着地
			if (P1.YPos > GROUND){
				P1.time = 0, P1.stateno = 625, P1.ctrl = 0;
				P1.More = 1;
			}
			break;
			//********************
			// 821: EX昇竜叩き落とし
			//********************
		case 821:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(3);

			if (P1.time == 0)VelSet(0, -0.2);
			else{ P1.YVel += GRAVITY; }

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 5フレ
			if (P1.time >= 1){

				// [ダメージ]
				Damage(20, 64);
				// [ゲージ] 
				Power(0, 80);

				// [ヒットストップ・のけぞり時間]
				HitTime(10, 28, 48, 18);

				// [ノックバック]
				HitVel(-4.0, -3.6, -4.4, 5.2);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1030;
				P1.HitSE = 14;
				// エフェクト
				HitEff(1, 1, 1);
				// バウンド
				P1.A.boundLevel = 1;
				// 地震エフェクト
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}
			else {
				//DamReset();
			}

			if (P1.YPos + P1.YVel * 2 >= GROUND){
				P1.stateno = 431, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;
			//********************
			// 850 ヴァンパイアキラー
			//********************
		case 850:
			P1.ctrl = 0;
			P1.A.gaugeHosei = true;
			if (P1.time < 28){
				P1.ignoreG = false;
				P1.SFlg = 0;
			}
			else{
				P1.ignoreG = true;
				P1.SFlg = 2;
			}

			// ゲージ消費
			if (P1.time == 0){
				P1.Power -= 3000;
				S.StopTime = 10;
				S.Anten = 25;
				SEStart(25);
				// エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);

				DamReset();
			}

			// 無敵
			/*
			if (P1.time <= 5){
				P1.mutekiF = 0;
				P1.Muteki = 1;
			}
			*/

			// SEを鳴らす
			if (P1.time == 1){
				SEStart(21);
			}
			// SEを鳴らす
			if (P1.time == 28+1){
				SEStart(21);
			}

			if (P1.time == 5){
				P1.XVel = 13;
			}

			if (P1.time == 1 + 28){
				VelSet(0, 0);
			}
			if (P1.time == 7 + 28){
				VelSet(1.6, -6);
			}
			// アニメ時間
			if (P1.time == ANIMELEM - 1){
				VelSet(0.1, -0.5);
			}
			if (P1.time > ANIMELEM - 1){
				VelAdd(0, 0.4);
			}

			if (P1.time == 1 || P1.time == 28+7 || P1.time == 28+9 || P1.time == 28+11){
				P1.MoveHit = 1;
			}
			if (P1.time == 28){
				P1.MoveHit = 0;
			}

			// ダメージセット
			if ((P1.time >= 1) && (P1.time < 28)){

				// [ダメージ]
				Damage(100, 0);
				// [ゲージ] 
				Power(0, 100);
				HitTime(10, 24, 24, 24);
				// [ノックバック]
				HitVel(-1.6, 0, -2.4, -2.0);
				P1.HitAnim = 1060;	// 地上
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;

				P1.A.kill = 0;
			}
			// ダメージセット
			else if ((P1.time >= 28) && (P1.time <= ANIMELEM)){

				// [ダメージ]
				Damage(40, 25);
				// [ゲージ] 
				Power(0, 100);

				HitTime(8, 100, 100, 12);
				// [ノックバック]
				HitVel(-1.6, -6.6, -1.6, -6.2);

				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;

				P1.HitSE = 13;
				P1.A.kill = 0;
			}

			// 当たったら変更
			if ((P1.CFlg) && (P1.time >= 1) && (!P1.ignoreG) && (P2.stateno >= 1000) && (P2.YPos != GROUND)){
				P2.YPos = GROUND;
				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 50;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 50;
				if (P1.muki == 0){
					P2.XPos = P1.XPos + 50;
				}
				else if (P1.muki == 1){
					P2.XPos = P1.XPos - 50;
				}
			}
			

			// 流星キックへ
			if ((P1.CFlg) && (P1.ignoreG) && (P2.stateno >= 1000) && (P1.time == 44)){
				P1.time = 0, P1.stateno = 851, P1.ctrl = 0;
				P1.More = 1;
				P1.A.damage = 0;
			}

			// 終了
			if (P1.time >= ANIMELEM){
				if (P1.YPos + P1.YVel * 2 >= GROUND){
					P1.time = 0, P1.stateno = 621, P1.ctrl = 0;
				}
			}

			break;
			//********************
			// 851 流星キック
			//********************
		case 851:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.touchF = false;
			P1.ignoreG = true;
			P1.A.gaugeHosei = true;
			// ゲージ消費
			if (P1.time == 1+6-1){
				S.StopTime = 15;
				S.Anten = 30;
				S.noGauge = 15;
				SEStart(25);
				// エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize - 50, 0, 0, 0.25, 0.25, P1.PSide);
			}

			if (P1.time == 0)VelSet(-8,-11);
			if (P1.time == 1+6)VelSet(16.0, 12);


			// ヒット数セット 10フレ
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(55, 155);
				// [ゲージ] 
				Power(0, 200);
				HitTime(16, 300, 300, 15);
				// [ノックバック]
				//HitVel(-0.5, -13.8, -0.5, -13.2);
				HitVel(0, -6.0, 0, -5.2);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// 空中喰らい(確定ダウン)
				P1.HitSE = 14;
				//P1.A.yaccel = 0.28;
				// エフェクト
				HitEff(1, 1, 1);
				//P1.A.hosei_K = 0.05;
				P1.A.addMuteki = true;
				P1.A.amTime = 120;

				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;

				P1.A.kill = true;
			}
			else{
				DamReset();
			}

			// 終了
			if (P1.time >= ANIMELEM){
				if (P1.YPos + P1.YVel * 2 >= GROUND){
					P1.time = 0, P1.stateno = 625, P1.ctrl = 0;
					P1.touchF = true;
				}
			}

			break;
			//===============================//
		}// switch終了

		// ステート番号を保存
		P1.OldState = P1.stateno;
		P1.oldState2 = P1.State2;

	}// P1処理終了


	for (int i = 0; i < HM; i++){
		{
			//********************
			//
			// ヘルパー
			//
			//********************

			// オンだったらステートに飛ばす
			if (H1[i].var){
				switch (H1[i].stateno){
					//********************
					// 2000 置かれた大ブメ 
					//********************
				case 2000:

					//***************
					//  基本設定
					//***************	
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H_PosSet(i, -80, 0);
					}
					//***************
					//  行動内容
					//***************

					// 全体フレーム33or被ダメ
					if ((H1[i].time > 33) ||
						(P1.stateno >= 1000)){
						HelperReset(i);
					}
					break;

					//********************
					// 2010 小ブメ （地上版）
					//********************
				case 2010:


					//***************
					//  基本設定
					//***************
					// 座標・速度設定
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;

						// 速度なかったら設定
						if (H1[i].HXVel == 0){
							H_VelSet(i, 6.1, 0);
						}

						H_PosSet(i, 20, -100);
					}

					// ヒット数・ダメージセット
					if (H1[i].time == 0){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 75;
					}
					// ダメージ位置セット
					if (H_triggerT(i, 0, 1)){

						//[ 攻撃判定セット ]
						H_AttSet(i, 0, 50, 30, -20, -50);

						// [ヒット・ガードサウンド]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [ゲージ] 
						H1[i].HGetPow = 40, H1[i].HGivePow = 20;
						//[ ガード条件 ]
						H1[i].HGuardF = 1;
						// [ヒット硬直]
						HI.HHitStop = 4;
						H1[i].HG_HitTime = 20, H1[i].HA_HitTime = 24,
							// [ガード硬直]
							H1[i].HG_GuardTime = 16;

						// [ヒット時のアニメ]
						H1[i].HHitAnim = 1000;
						// [ノックバック]
						H1[i].H_GX = -4.2, H1[i].H_GY = 0;
						H1[i].H_AX = -3, H1[i].H_AY = -5;
					}


					H_Move(i);

					//***************
					//  行動内容
					//***************
					// アニメをループさせる
					if (H1[i].HAnimTime >= 24)H1[i].HAnimTime = 0;
					// 終了条件、端に到達or地面に当たるorヒットor本体被ダメ
					if ((H1[i].XPos < (S.ScroolX + GAMENHAJI)) ||
						(H1[i].XPos >(S.ScroolX + SCREEN_W - GAMENHAJI)) ||
						(H1[i].YPos > GROUND) ||
						(H1[i].HMoveHit == 0) ||
						(P1.stateno >= 1000)
						)
					{
						HelperReset(i);
					}

					break;

					//********************
					// 2020 設置ノーマル 
					//********************
				case 2020:

					//***************
					//  基本設定
					//***************
					// 座標・速度設定
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H1[i].HXVel = 0;

						H_PosSet(i, 20, -100);
					}

					// ヒット数・ダメージセット
					if (H1[i].time == 0){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 75;
					}

					// 位置を変える
					if (H1[i].muki == 0){
						H1[i].XPos += H1[i].HXVel;
					}
					else{ H1[i].XPos -= H1[i].HXVel; }

					//***************
					//  行動内容
					//***************
					// アニメをループさせる
					if (H1[i].HAnimTime >= 24)H1[i].HAnimTime = 0;
					// 終了間際に半透明に
					if (H1[i].time >= 240)H1[i].Alpha = true;
					// 終了条件、時間経過
					if (H1[i].time >= 300){
						HelperReset(i);
					}

					break;
					//********************
					// 2025 派生D (発射)
					//********************
				case 2025:

					//***************
					//  基本設定
					//***************
					// 座標・速度設定
					// 速度は 625 で決める
					// 速度がないときはここで決める
					if (H1[i].time == 0 && H1[i].HXVel == 0){
						H1[i].HXVel = 5.0;
					}
					// 方向転換
					if (H1[i].time == 50 && H1[i].HXVel == 4.5){
						H1[i].HXVel = -5.0;
						SEStart(5);
					}

					// 音を鳴らす
					if (H1[i].time == 3){
						SEStart(5);
					}

					// ヒット数・ダメージセット
					if (H1[i].time == 4){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 75;
					}

					// ダメージ位置セット、5フレ以降
					if (H1[i].time >= 4){

						//[ 攻撃判定セット ]
						H_AttSet(i, 0, 50, 30, -20, -38);

						// [ヒット・ガードサウンド]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [ゲージ] 
						H1[i].HGetPow = 40, H1[i].HGivePow = 20;
						//[ ガード条件 ]
						H1[i].HGuardF = 1;

						// [ヒット硬直]
						HI.HHitStop = 6;
						H1[i].HG_HitTime = 26, H1[i].HA_HitTime = 34,
							// [ガード硬直]
							H1[i].HG_GuardTime = 24;


						// [ヒット時のアニメ]
						H1[i].HHitAnim = 1000;
						// [ノックバック]
						H1[i].H_GX = -3.8, H1[i].H_GY = 0;
						H1[i].H_AX = -2, H1[i].H_AY = -6;
					}

					H_Move(i);

					//***************
					//  行動内容
					//***************
					// アニメをループさせる
					if (H1[i].HAnimTime >= 24)H1[i].HAnimTime = 0;
					// 終了条件、端到達or攻撃ヒット
					if ((H1[i].XPos < (S.ScroolX)) ||
						(H1[i].XPos >(S.ScroolX + SCREEN_W)) ||
						(H1[i].HMoveHit == 0)
						)
					{
						HelperReset(i);
					}
					break;

					//********************
					// 2030 大ブーメラン 
					//********************
				case 2030:

					//***************
					//  基本設定
					//***************
					// 座標設定
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H1[i].HXVel = 8;

						H_PosSet(i, 20, -65);
					}
					// 速度設定
					if (H1[i].time == 1){
						H1[i].HXVel = 8.5;
					}
					if (H1[i].time == 25){
						H1[i].HXVel = -8.5;
					}


					// ヒット数・ダメージセット
					if ((H1[i].time == 1) || (H1[i].time == 25)){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 70;
					}

					// ダメージ位置セット、2フレ以降
					if (H1[i].time >= 1){

						//[ 攻撃判定セット ]
						H_AttSet(i, 0, 80, 60, -40, -108);

						// [ヒット・ガードサウンド]
						H1[i].HHitSE = 12, H1[i].HGuardSE = 17;
						// [ゲージ] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 40;
						//[ ガード条件 ]
						H1[i].HGuardF = 1;

						// [ヒット硬直]
						HI.HHitStop = 6;
						H1[i].HG_HitTime = 40, H1[i].HA_HitTime = 40,
							// [ガード硬直]
							H1[i].HG_GuardTime = 15;

						// [ヒット時のアニメ]
						H1[i].HHitAnim = 1000;
						// [ノックバック]
						H_HitVel(i, -3.7, 0,
							-4.2, -6);

						if (H1[i].time >= 25){
							H_HitVel(i, -4.6, -8,
								4, -8.8);
						}

						// [補正]
						if (P1.CFlg){
							P1.A.hosei_K = 0.7;
						}

					}

					H_Move(i);

					//***************
					//  行動内容
					//***************
					// アニメをループさせる
					if (H1[i].HAnimTime >= 32)H1[i].HAnimTime = 0;
					// 終了条件、時間経過or喰らい
					if ((H1[i].time > 42) || (P1.stateno >= 1000))
					{
						HelperReset(i);
					}
					break;

					//********************
					// 2100 ハイドロ雨
					//********************
				case 2100:

					//***************
					//  基本設定
					//***************
					H1[i].XPos = 0;
					H1[i].YPos = 0;
					H1[i].GSize = 1;
					H1[i].Alpha = 1;	//透明化
					P1.A.gaugeHosei = true;
					if (H1[i].time == 0){
						H1[i].muki = 0;
					}
					
					if(H1[i].time >= 14)S.Anten = 1;

					//SE
					if (H1[i].time == 6 && P1.Var[11] == 0)SEStart(23);

					

					// ヒット数・ダメージセット
					if (H1[i].time % 15 == 0 && (H1[i].time != 0)){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 55;
						H1[i].A.hosyo = 5;
					}
					else{
						H1[i].HMoveHit = 0;
						H1[i].A.damage = 0;
					}

					// ダメージ位置セット、2フレ以降
					if (H1[i].time >= 0){

						//[ 攻撃判定セット ]
						H_AttSet(i, 0, 80, 220, -40, -220);

						// [ヒット・ガードサウンド]
						H1[i].HHitSE = 13, H1[i].HGuardSE = 17;
						// [ゲージ] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 40;
						//[ ガード条件 ]
						H1[i].HGuardF = 1;

						// [ヒット硬直]
						H_HitTime(i, 0, 28, 120, 24);

						// [ヒット時のアニメ]
						H1[i].HHitAnim = 1030;
						// [ノックバック]
						H1[i].H_GX = 0, H1[i].H_GY = 0;
						H1[i].H_AX = 0, H1[i].H_AY = -2.8;
						// エフェクト
						h_HitEff(i, 3, 0.6, 0.6);
					}

					H_Move(i);


					//if (H1[i].time == 24);//H1[i].HAnimTime = 1;
					
					// エフェクトとして描画
					if (H1[i].HAnimTime > 0){
						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							-100 + H1[i].HAnimTime * 4, -500 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);
						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							486 + H1[i].HAnimTime * 4, -500 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);

						// エフェクトとして描画
						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							-100 + H1[i].HAnimTime * 4, -200 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);
						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							-100 + H1[i].HAnimTime * 4, 100 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);

						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							486 + H1[i].HAnimTime * 4, -200 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);
						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							486 + H1[i].HAnimTime * 4, 100 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);
					}

					//***************
					//  行動内容
					//***************

					// 終了条件、2秒経過
					if (H1[i].time >= 120)
					{
						HelperReset(i);
					}
					break;
					//===============================//
					//******************
					// ヘルパー処理終了
					//******************
				}// switch終了
			}// オン・オフ判断終了
		}// 次のヘルパーへ
	}// for終了

	// 値を返す
	Get_SSet(S);
	if (P1.PSide == 1)Get_PSet(P1, P2), Get_HSet(H1, H2);//, Get_SSet(S);
	if (P1.PSide == 2)Get_PSet(P2, P1), Get_HSet(H2, H1);//, Get_SSet(S);

}//StateParam終了




//-------------------------------------------------------------------------------
//
// 受け取り用
//
//-------------------------------------------------------------------------------

// 順番を逆にする
void GetP_CorParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_CorParam(System_t GS)
{
	S = GS;
}

void GetH_CorParam(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}



//-------------------------------------------------------------------------------
//
// 受け取り２
//
//-------------------------------------------------------------------------------


/*
int InputCOM(int num)
{

	return 0;//Ein_ComCheck(num);

}
*/

//-------------------------------------------------------------------------------
//
// 数値初期化
//
//-------------------------------------------------------------------------------



//-------------------------------------------------------------------------------
//
// キャンセル設定
//
//-------------------------------------------------------------------------------

// 必殺技キャンセル
void SCancel()
{
	// Ｄ派生技
	if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)
		&& (P1.SFlg != 2)){		// 先行入力効かせてみる
		if ((P1.keyAtt[6]) && (!P1.keyAtt[8]) && (!P1.keyAtt[2]))P1.stateno = 603;
		else if (P1.keyAtt[2])P1.stateno = 604;
		else{ P1.stateno = 600 + P1.Var[2]; }
			P1.More = 1;
			P1.time = 0, P1.A.damage = 0;
	}
	// 236 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[1])){		// 先行入力効かせてみる
		P1.stateno = 630, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 214 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[2])){		// 先行入力効かせてみる
		P1.stateno = 610, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 623 + B
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[4])){		// 先行入力効かせてみる
		P1.stateno = 620, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 214 + D
	if ((P1.Senkou[4] > 0) && ((P1.Power >= 500) || (P1.Var[20] > 2))
		&& (P1.cmd[2])){		// 先行入力効かせてみる
		P1.stateno = 700, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	HCancel();

	//最後に判定を消す
	DamReset();
	
}

// 必殺技キャンセル
void HCancel()
{
	
	// 214 + C
	if ((P1.Senkou[3] > 0)
		&& (P1.cmd[2]) && (P1.Power >= 2000) && (!H1[1].var)){		// 先行入力効かせてみる
		P1.stateno = 810, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 236 + C
	if ((P1.Senkou[3] > 0)
		&& (P1.cmd[1]) && (P1.Power >= 1000)){		// 先行入力効かせてみる
		P1.stateno = 800, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 623 + C
	if ((P1.Senkou[3] > 0)
		&& (P1.cmd[4]) && (P1.Power >= 1000)){		// 先行入力効かせてみる
		P1.stateno = 820, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	//  [ ３ゲージ ]
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.Senkou[3] > 0)
		&& (P1.Power >= 3000)){		// 地上
			P1.stateno = 850;	// 超必
			P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}

	//最後に判定を消す
	DamReset();

}

// 空中必キャンセル
void ACancel()
{
	/*
	// [必殺技]
	// D (ノーマル設置、地上発射)
	if (P1.Senkou[4] > 0){
		// ヘルパー[2]がある
		if (H1[2].var){
			if (P1.SFlg == 2){
				P1.stateno = 626, P1.More = 1,
					P1.time = 0, P1.A.damage = 0;
			}
		}
		// 設置がない
		else if (!H1[2].var){
			if (P1.SFlg == 2){
				P1.stateno = 622, P1.More = 1,
					P1.time = 0, P1.A.damage = 0;
			}
		}
	}

	// 236 + 攻撃
	if ((InputCOM(1)) && (!H1[1].var)){		// 画面にヘルパー１がない
		if ((P1.Senkou[1] > 0) ||
			(P1.Senkou[2] > 0) ||
			(P1.Senkou[3] > 0)){
			P1.stateno = 605, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	*/

	// [超必]
	//  214+D		[流星キック]
	/*
	if ((P1.Senkou[4] > 0) && (P1.cmd[2])
		&& (P1.Power >= 1000)){
		if (P1.SFlg == 2){		// 地上
			P1.stateno = 820, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	*/
	// 623 + C
	if ((P1.Senkou[3] > 0)
		&& (P1.cmd[4]) && (P1.Power >= 1000)){		// 先行入力効かせてみる
		P1.stateno = 820, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}

	//最後に判定を消す
	DamReset();

	
}

