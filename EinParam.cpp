//==================================================================================
//
//			数値計算の管理部
//
//========================================================================

//インクルード部--------------------------------------------
#include "pch.h"
#include "Define2.h"

//デファイン部----------------------------------------------

// #include "Define2.h" にて、関数をセット

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
void EinParam(void)
{

	//ワンモアを切る
	if (P1.More)P1.More = 0;

	// コマンドに数値を渡す
	GetPH_EinCmd(P1);

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
		if (P1.ctrl)P1.Var[1] = 0;

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


				
				// 投げ無敵付与
				/*
				if (P1.Muteki == 10 && P1.time == 0 &&
					P1.mutekiF == 0){
					P1.mutekiF = 2;
					P1.Muteki = 10;
				}
				*/

				// 立ち の 全体フレームを超えたらリセット
				if (P1.time >= P1.animElem[P1.stateno])P1.time = 0;

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
				// 最初にロック！
				if (P1.time == 0)P1.Lock = 1;
				// 操作が入ったらモーションキャンセル
				if (P1.Key != 0)P1.time = 0, P1.stateno = 0, P1.SFlg = 0, P1.More = 1, P1.Lock = 0;
				if (P1.time >= P1.animElem[P1.stateno]){
					P1.time = 0, P1.stateno = 0, P1.SFlg = 0, P1.More = 1, P1.Lock = 0;
				}
			
				break;
			case 20:	// 前歩き
				P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
				P1.XVel = P1.C.walkF; //速度を足す
				// SEを鳴らす
				if ((P1.time == 10) || (P1.time == 35))SEStart(7);
				if (P1.time >= ANIMELEM)P1.time = 0;
				break;
			case 21:	// 後ろ歩き
				P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
				P1.XVel = P1.C.walkB; //速度を足す
				// SEを鳴らす
				if (P1.time == 20)SEStart(7);
				if (P1.time >= P1.animElem[P1.stateno])P1.time = 0;

				break;
			case 30:	// ダッシュ
				P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
				P1.XVel = P1.C.runF[0]; //速度を足す
				if (P1.AnimTime <= 8)P1.XVel = P1.C.runF[0] * (0.1 * (P1.AnimTime + 1)); //速度を足す;
				// SEを鳴らす
				if ((P1.time == 20) || (P1.time == 40))SEStart(4);
				if (P1.time >= P1.animElem[P1.stateno])P1.time = 0;

				
				break;
			case 35:	// バックステップ
				P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
				P1.ignoreG = true;

				// 無敵
				if (P1.time <= 5){
					P1.Muteki = 1, P1.mutekiF = 0;
				}

				// SEを鳴らす
				if (P1.time == 1)SEStart(5);

				VelAdd(0, P1.C.yAccel);

				//速度を足す
				if (P1.time == 0)VelSet(P1.C.runB[0], P1.C.runB[1]);
				

				// 終了
				if ((P1.YPos >= GROUND) && (P1.time > 1)){
					P1.stateno = 36;
					P1.time = 0;
					P1.SFlg = 0;
					P1.ctrl = 0;
				}

				break;
			case 36:	// 着地硬直
				P1.SFlg = 0, P1.ctrl = 0;

				// SEを鳴らす
				if (P1.time == 1)SEStart(6);
				VelSet(0, 0);
				P1.YPos = GROUND;

				// 終了
				if (P1.time >= ANIMELEM + 1){
					P1.stateno = 0;
					P1.time = 0;
					P1.ctrl = 1;
				}

				break;

			case 90:	// 開幕
				P1.SFlg = 0, P1.ctrl = 0;
				/*
				if (P1.time == 7)SEStart(32);
				if (AnimElem(13))SEStart(28);
				*/

				// 音を鳴らす
				if (P1.time == 15){
					SEStart(2);
				}

				// 全体フレームを超えたら立ちへ
				if (P1.time >= P1.animElem[P1.stateno]){
					P1.time = 0;
					P1.stateno = 0;
				}
				break;

			case 95:	// ラウンド取得
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
				// 全体フレーム
				if (P1.time >= ANIMELEM){
					P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				}
				// SEを鳴らす
				if (P1.time == 1){
					SEStart(1);
				}

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 1, 2);
				}

				// ヒット数セット
				if (P1.time == 3){
					P1.MoveHit = 1;	// １回
				}

				// ダメージセット、持続 2 フレーム？
				if (P1.time >= 1){

					// [ダメージ]
					Damage(45, 0);

					// [ゲージ] 
					P1.GetPow = 25, P1.GivePow = 12;
					// [ヒットストップ・のけぞり時間]
					HitTime(6, 12, 16, 10);
					// [ノックバック]
					HitVel(-3.6, 0, -1.5, -4.8);

					// [ガード属性]
					P1.GuardF = 1;
					// [喰らい中の浮き]
					P1.fallF = 1;
					// [ヒット時のアニメ]
					P1.HitAnim = 1000;
					// [ヒットサウンド]
					SESet(10, 16);
				}
				else {
					DamReset();
				}
				

				// ヒット時チェーン
				if ((P1.CFlg) && (P1.time >= 5)){
					// [ジャンプキャンセル]
					if (P1.keyAtt[8]){		// 先行入力効かせてみる
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [通常技]
					if (P1.Senkou[3] > 0){
						// 6C
						if ((key(6)) && (!key(2)) && (!key(8))){
							P1.stateno = 225, P1.More = 1,
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
						if (key(2)){
							P1.stateno = 310, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else{
							P1.stateno = 210, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// 先行入力
					else if (P1.Senkou[1] > 0){
						if (key(2)){
							P1.stateno = 300, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else{
							P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// [必殺技・ゲージ技]
					SCancel();

				}
				// キャンセル、連打キャンセル
				if ((P1.button[1] == 1) && (P1.time >= 9) && (P1.stateno == 200)){
					if (!key(2)){
						P1.More = 1, P1.time = 0, P1.A.damage = 0;
					}
				}

				break;
			//********************
			// 210：立ちB
			//********************
			case 210:	//立ちB
				P1.ctrl = 0, P1.SFlg = 0;
				// 慣性
				if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
				// SE
				if (P1.time == 1){
					SEStart(1);
				}
				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 2, 1);
				}

				// ヒット数セット、８フレ
				if (P1.time == 6){
					P1.MoveHit = 1;	// １回
				}
				// ダメージセット
				if ((P1.time >= 6) && (P1.time <= 10)){

					// [ダメージ]
					Damage(76, 0);

					// [ゲージ] 
					Power(88);
					HitTime(8, 18, 20, 16);

					// [ノックバック]
					HitVel(-2.8, 0, -1.7, -4.0);
					P1.GuardF = 1;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitAnim = 1000;	// 上段喰らい
					P1.HitSE = 11;
				}
				else {
					DamReset();
				}
				// 全体フレームを超えたらリセット
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

				// キャンセル
				// 応急処置
				if (P1.StopTime == 0){
					if ((P1.CFlg) && (P1.time >= 8)){

						// [通常技]
						if (P1.Senkou[3] > 0){
							// 6C
							if ((key(6)) && (!key(2)) && (!key(8))){
								P1.stateno = 225, P1.More = 1,
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
						else if (P1.Senkou[2] > 0){
							// 下要素を確認
							if (key(2) && P1.Var[1] == 0){
								P1.stateno = 310, P1.More = 1,
									P1.time = 0, P1.A.damage = 0;
								P1.Var[1] ++;
							}
						}
						// [必殺技・ゲージ技]
						SCancel();
					}
				}
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
					PosAdd(1, 0);
				}
				// SEを鳴らす
				if (P1.time == 1)SEStart(3);

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 3, 0);
				}

				// 全体フレームを超えたらリセット
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;


				// ヒット数セット 発生：26
				if (P1.time == 9){
					P1.MoveHit = 1;	// １回
				}
				//else{ P1.MoveHit = 0; }

				// ダメージセット
				if ((P1.time >= 9) && (P1.time <= 18)){

					// [ダメージ]
					Damage(96, 0);
					// [ゲージ] 
					Power(114);
					// [ヒットストップ・のけぞり時間]
					HitTime(8, 22, 24, 18);
					// [ノックバック]
					HitVel(-4.0, 0, -2.0, -4);
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

				// キャンセル
				// 応急処置
				if (P1.StopTime == 0){
					if ((P1.CFlg) && (P1.time >= 1)){
						/*
						// [通常技]
						if (P1.Senkou[3] > 0){
							// 下要素を確認
							if (key(2)){
								P1.stateno = 320, P1.More = 1,
									P1.time = 0, P1.A.damage = 0;
							}
						}
						*/
						// [必殺技・ゲージ技]
						SCancel();
					}

				}
				break;
			//********************
			// 225：6C
			//********************
			case 225:
				P1.ctrl = 0, P1.SFlg = 0;
				// 慣性
				if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
				
				// 位置をずらす
				if (P1.time == 0){
					PosAdd(5, 0);
				}
				// SEを鳴らす
				if (P1.time == 1)SEStart(3);
				// 全体フレームを超えたらリセット
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 3, 0);
				}

				// ヒット数セット 発生：26
				if (P1.time == 1){
					P1.MoveHit = 1;	// １回
				}
				// ダメージセット
				if ((P1.time >= 1)){

					// [ダメージ]
					Damage(106, 0);
					// [ゲージ] 
					Power(128);
					// [ヒットストップ・のけぞり時間]
					HitTime(8, 22, 24, 18);
					// [ノックバック]
					HitVel(-4.8, 0, -2.8, -4);
					// [ガード属性]
					P1.GuardF = 2;
					

					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitAnim = 1020;	// 下段喰らい
					P1.A.forceKurai = true;	// 屈喰らいに

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
					if ((P1.CFlg) && (P1.time >= 27)){

						// [必殺技・ゲージ技]
						SCancel();

					}
				}
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
				
				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 1, 2);
				}
				
				// ヒット数セット、発生６フレ
				if (P1.time == 1){
					P1.MoveHit = 1;	// １回
				}
				// ダメージセット
				if (P1.time >= 1){

					// [ダメージ]
					Damage(42, 0);

					// [ゲージ] 
					Power(24);

					// [ヒットストップ・のけぞり時間]
					HitTime(6, 12, 16, 10);
	
					// [ノックバック]
					HitVel(-3.1, 0, -1.6, -4.8);
					P1.GuardF = 3;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitAnim = 1010;	// 下段喰らい
					P1.HitSE = 10;
				}
				else {
					DamReset();
				}
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 6)){
					if (P1.Senkou[3] > 0){
						// 6C
						if ((key(6)) && (!key(2)) && (!key(8))){
							P1.stateno = 225, P1.More = 1,
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
						if (key(2)){
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
							P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else{
							P1.stateno = 200, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// [必殺技・ゲージ技]
					SCancel();
				}// キャンセル、連打キャンセル
				if ((P1.button[1] == 1) && (P1.time >= 9) && (P1.stateno == 300)){
					// 下要素が入ってるやん！
					if (P1.keyAtt[2]){
						P1.More = 1, P1.time = 0, P1.A.damage = 0;
					}
				}

				break;
			//********************
			// 310：しゃがみB
			//********************
			case 310:
				P1.ctrl = 0, P1.SFlg = 1;
				// 慣性
				if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
				// SE
				if (P1.time == 1)SEStart(1);
				// 全体フレームを超えたらリセット
				if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 2, 1);
				}

				// ヒット数セット
				if (P1.time == 1){
					P1.MoveHit = 1;	// １回
				}

				
				// ダメージセット
				if (P1.time >= 1){

					// [ダメージ]
					Damage(75, 0);

					// [ゲージ] 
					Power(86);

					HitTime(8, 18, 20, 16);

					// [ノックバック]
					HitVel(-3.2, 0, -1.5, -4.6);
					P1.GuardF = 1;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitAnim = 1010;	// 下段喰らい
					P1.HitSE = 11;
				}
				else {
					DamReset();
				}

				// キャンセル
				// 応急処置
				if (P1.StopTime == 0){
					if ((P1.CFlg) && (P1.time >= 1)){
						// [ジャンプキャンセル]
						if ((P1.K_Senkou[8]) && (P2.HFlg == 1)) {		// 先行入力効かせてみる
							P1.stateno = 40, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						// [通常技]
						if (P1.Senkou[3] > 0){		// 先行入力効かせてみる
							// 6C
							if ((key(6)) && (!key(2)) && (!key(8))){
								P1.stateno = 225, P1.More = 1,
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
							// 下要素を確認
							if ((!key(2)) && (P1.Var[1] == 0)){
								P1.stateno = 210, P1.More = 1,
									P1.time = 0, P1.A.damage = 0;
								P1.Var[1]++;
							}
						}
						
						// [必殺技・ゲージ技]
						SCancel();
					}
				}


				break;
			//********************
			// 320：しゃがみC
			//********************
			case 320:
				P1.ctrl = 0, P1.SFlg = 0;
				// 慣性
				if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
				// SEを鳴らす
				if (P1.time == 1)SEStart(2);
				// 全体フレームを超えたらリセット
				if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 3, 0);
				}

				// ヒット数セット 10フレ
				if (P1.time == 1){
					P1.MoveHit = 1;	// １回
				}
				// ダメージセット
				if (P1.time >= 1){

					// [ダメージ]
					Damage(98, 0);
					// [ゲージ] 
					Power(118);
					HitTime(8, 40, 40, 18);
					// [ノックバック]
					HitVel(-1.8, -4, -2.2, -3.5);
					GuardVel(-3.8, -1.5);

					P1.GuardF = 3;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitAnim = 1030;	// 空中喰らい(確定ダウン)
					P1.HitSE = 12;
				}
				else{ DamReset(); }
				// 応急処置
				if (P1.StopTime == 0){
					// キャンセル
					if ((P1.CFlg) && (P1.time >= 10)){
						// [必殺技・ゲージ技]
						SCancel();
					}
				}
				break;
			//********************
			// 400：ジャンプA
			//********************
			case 400:
				P1.ctrl = 0, P1.SFlg = 2;
				if (P1.time == 1)SEStart(1);

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 1, 2);
				}
				// 先行入力消す
				if (P1.time <= 1)P1.Senkou[1] = 0;
				// ヒット数セット 6フレ
				if (P1.time == 4)P1.MoveHit = 1;	// １回
				// ダメージセット
				if ((P1.time >= 4) && (P1.time <= 10)){

					// [ダメージ]
					Damage(40, 0);
					// [ゲージ] 
					Power(30, 15);

					// [ヒットストップ・のけぞり時間]
					HitTime(5, 14, 18, 12);
					// [ノックバック]
					HitVel(-4.6, 0, -1.4, -4.2);
					// ガード判定
					if (P1.YVel >= 0.0)P1.GuardF = 2;
					else{ P1.GuardF = 1; }
					//P1.GuardF = 1;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitAnim = 1000;
					P1.HitSE = 10;
				}
				else {
					DamReset();
				}
				// 操作可能フレーム
				//if (P1.time >= 19)P1.ctrl = 1;
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					if (P1.Senkou[3] > 0){		// 先行入力効かせてみる
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (P1.Senkou[2] > 0){		// 先行入力効かせてみる
						P1.stateno = 410, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}

					// [空中必殺技]
					ACancel();

				}
				/*
				// 全体フレームを超えたらリセット
				if (P1.time >= ANIMELEM){
					P1.ctrl = 1, P1.More = 1,
						P1.stateno = 46, P1.time = 0;
				}
				*/
				break;
			//********************
			// 410：ジャンプB
			//********************
			case 410:
				P1.ctrl = 0, P1.SFlg = 2;
				// SE
				if (P1.time == 1)SEStart(1);

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 2, 1);
				}

				// ヒット数セット
				if (P1.time == 2)P1.MoveHit = 1;	// １回
				// ダメージセット
				if (P1.time >= 2){

					// [ダメージ]
					Damage(70, 0);

					// [ゲージ] 
					Power(80);
					// [ヒットストップ・のけぞり時間]
					HitTime(8, 16, 18, 14);
					// [ノックバック]
					HitVel(-4.6, 0,  -1.6, -4.6);
					P1.GuardF = 2;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitAnim = 1000;
					P1.HitSE = 11;
				}
				else {
					DamReset();
				}
				/*
				// 操作可能フレーム
				if (P1.time >= 24){
					P1.ctrl = 1;
				}
				*/
				// キャンセル
				// 応急処置
				if (P1.StopTime == 0){
					if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
						if (P1.Senkou[3] > 0){		// 先行入力効かせてみる
							P1.stateno = 420, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}

						// [空中必殺技]
						ACancel();
					}

					// [ジャンプキャンセル]
					if ((P1.CFlg) && (P1.time >= 6) && (P1.AirJump > 0)){
						if (P1.K_Senkou[8] >= 1){		// 先行入力効かせてみる
							P1.stateno = 45, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
				}
				break;
			//********************
			// 420：ジャンプC
			//********************
			case 420:
				P1.ctrl = 0, P1.SFlg = 2;
				// SE
				if (P1.time == 1)SEStart(2);

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 3, 0);
				}

				// ヒット数セット
				if (P1.time == 7)P1.MoveHit = 1;	// １回
				// ダメージセット、持続 5フレ
				if ((P1.time >= 7) && (P1.time <= 11)){

					// [ダメージ]
					Damage(95, 0);

						// [ゲージ] 
					Power(100);

					// [ヒットストップ・のけぞり時間]
					HitTime(8, 18, 40, 16);

					// [ノックバック]
					if (P1.HitCount > 10)
					HitVel(-4.8, 0, -3.8, 4.5);
					else { HitVel(-4.8, 0, -3.8, 6.5); }

					P1.GuardF = 2;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitAnim = 1000;
					P1.HitSE = 12;
					P1.A.boundLevel = 1;
				}
				else {
					//DamReset();
				}
				// [キャンセル]
				if ((P1.CFlg) && (P1.time >= 9)){
					// [空中必殺技]
					ACancel();
				}

				break;
			//********************
			// 500：地上投げ
			//********************
			case 500:
				P1.ctrl = 0, P1.SFlg = 0;
				// 投げ方向設定
				if (P1.time == 0){
					P1.throwSide = P1.muki;
					P1.A.throwTurn = false;
					if (P1.keyAtt[4]){
						P1.A.throwTurn = true;
					}
					P1.XVel = 0;
				}

				if ((P1.time == 9) && (P1.StopTime == 0))PVOStart(P1.PSide, 11, 0);

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

					int dist = 80;
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

					// 近いなら座標ずらす
					if (P2.XPos < GAMENHAJI){
						P2.XPos = GAMENHAJI;
						P1.XPos = P2.XPos + 80;
					}
					if (P2.XPos > STAGE_WIDTH - GAMENHAJI){
						P2.XPos = STAGE_WIDTH - GAMENHAJI;
						P1.XPos = P2.XPos - 80;
					}

					P2.YVel = 0;
					P2.YPos = GROUND;
				}


				// 距離を離す
				if (P1.time == 3)P2.XVel = -2.5;
				if (P1.time == 4)P2.XVel = -2.2;
				// ヒット音
				//if(P1.time == 7)SEStart(11);
				// ヒット音、エフェクト
				if (P1.time == 4){
					SEStart(11);
					EffStart(P1.A.hitEff, (int)P2.XPos, (int)P2.YPos, 20 + GetRand(10), -(P2.ySize / 1.5),
						P1.A.heSizeX, P1.A.heSizeY, P2.muki);
				}

				// 2P無敵
				if (P1.time < 39)P2.Muteki = 1, P2.mutekiF = 0;

				// VO
				if ((P1.time == 35) && (P1.StopTime == 0)){
					PVOStart(P1.PSide, 10, 0);
				}

				// ヒット数セット
				if (P1.time == 40)P1.MoveHit = 1;	// １回
				// ダメージセット、持続 1フレ
				if (P1.time == 40){

					// [攻撃判定セット]
					ExAtt(P1.PSide, 0, 100, 90, 20, -100);

					// [ダメージ]
					Damage(0, 180);
					P1.A.hosei_K = 0.5;
					// [ゲージ] 
					Power(300);
					HitTime(6, 45, 45, 0);
					// [ノックバック]
					HitVel(-2, -3.4, 0, 0);
					P1.HitAnim = 1030;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitSE = 12;
					P1.A.boundLevel = 1;
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
				if (P1.time == 0){
					P1.throwSide = P1.muki;
					P1.A.throwTurn = false;
					if (P1.muki == 0){
						if (InputPAD(104) >= 1){
							P1.A.throwTurn = true;
						}
					}
					if (P1.muki == 1){
						if (InputPAD(106) >= 1){
							P1.A.throwTurn = true;
						}
					}
				}
				// SEを鳴らす
				if (P1.time == 1)SEStart(1);
				// ヒット準備
				// ヒット数セット、
				if (P1.time == 4)
				{
					P1.MoveHit = 1;	// １回
				}

				if ((P1.time == 9) && (P1.StopTime == 0))PVOStart(P1.PSide, 11, 0);

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
				if ((P1.CFlg) && (P1.time >= 7)){
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
				P1.XVel = 0;
				P1.ignoreG = true;
				
				// バグ防止の浮き
				if ((P1.time == 0) && (P1.YPos+20 >= GROUND)){
					P1.YPos = GROUND - 20;
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

					P1.YVel = 0;
					P2.YVel = 0;
				}
				// 重力加算
				if (AnimElem(5)){
					P1.YVel = -5;
				}
				// 重力
				else if (P1.State2 >= 4){
					P1.YVel += 0.5;
				}
				
				// ついでに無敵時間
				if (P1.State2 < 3){
					P2.Muteki = 1, P2.mutekiF = 0;
					P2.time = 0;
					//P1.YVel += GRAVITY;
					//P2.YVel += GRAVITY;
				}
				// SEを鳴らす
				if (P1.time == 15)SEStart(10);

				// VO
				if ((P1.time == 9) && (P1.StopTime == 0)){
					PVOStart(P1.PSide, 10, 0);
				}

				// ヒット数セット
				if (P1.time == 1)P1.MoveHit = 1;	// １回
				// ダメージセット、持続 1フレ
				if (P1.time >= 1){

					// 床バウンド
					//P1.A.boundLevel = 1;
					// [攻撃判定セット]
					if (P1.time == 17)
					ExAtt(P1.PSide, 0, 100, 90, 20, -100);

					// [ダメージ]
					Damage(170, 10);
					// [ゲージ] 
					Power(300);

					HitTime(0, 100, 100, 0);
					// [ノックバック]
					HitVel(-4.8, 3.8, -4.8, 3.8);
					P1.HitAnim = 1030;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitSE = 11;
					
				}
				else{
					DamReset();
				}
				// 相手の姿勢を後半のに
				if (P1.CFlg && P2.stateno == 1030 && P2.time < 20)P2.time = 20;

				// 投げ抜け
				ThrowReject();

				// 着地
				if (P1.YPos + P1.YVel * 2 >= GROUND){
					P1.time = 0, P1.stateno = 47,
					P1.ctrl = 0, P1.SFlg = 0;
					P1.YPos = GROUND;
					P1.ignoreG = false;
				}
				break;
				
			//********************
			// 600 ブメ投げ A
			//********************
			case 600:	// Aブメ
				P1.ctrl = 0, P1.SFlg = 0;
				// SEを鳴らす
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 15, 0);


				// ヘルパー呼び出し.[0]
				if (P1.time == 0){
					HelperReset(0);
					H1[0].var = true;
					H1[0].time = 0;
					H1[0].stateno = 2000;
				}

				// ヘルパー設定準備.[1]	飛び道具
				if (P1.time == 0){
					HelperReset(1);
					H1[1].var = false;

					if (P1.Var[6] == 1)H1[1].HXVel = 9.8;
					else if (P1.Var[6] == 2)H1[1].HXVel = 13.8;
					else if (P1.Var[6] == 3){
						H_VelSet(1, 6.6, -6);
					}
				}
				// 呼び出し.[1]	飛び道具
				else if (P1.time == 12){
					H1[1].var = true;
					H1[1].time = 0;
					H1[1].stateno = 2010;
				}

				// ヒット時キャンセル
				if (P1.StopTime == 0){
					if ((H1[1].var == false) && (P1.time >= 12) && (P1.scTime > 0)){
						// [ゲージ技]
						HCancel();
					}
				}

				// 全体フレームを超えたらリセット
				if (P1.time >= 48)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;


				break;
			//********************
			// 605 空中ブメ投げ
			//********************
			case 605:	// Aブメ
				P1.ctrl = 0, P1.SFlg = 2;
				P1.ignoreG = true;
				// 速度をつける
				if (P1.time == 0){
					P1.XVel = -1.8;
					P1.YVel = -3;
				}
				// SEを鳴らす
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 15, 0);


				// ヘルパー設定準備.[1]	飛び道具
				if (P1.time == 0){
					HelperReset(1);
					H1[1].var = false;

					if (P1.Var[6] == 1)H_VelSet(1, 6.2, 6.2);
					else if (P1.Var[6] == 2)H_VelSet(1, 8.9, 8.9);
					else if (P1.Var[6] == 3){
						H_VelSet(1, 9.5, 0);
					}
				}
				// 呼び出し.[1]	飛び道具
				else if (P1.time == 12){
					H1[1].var = true;
					H1[1].time = 0;
					H1[1].stateno = 2010;
				}

				// 重力緩和
				VelAdd(0, P1.C.yAccel-0.2);
				
				// ヒット時キャンセル
				if (P1.StopTime == 0){
					if ((H1[1].var == false) && (P1.time >= 12) && (P1.scTime > 0)){
						// [ゲージ技]
						ACancel();
					}
				}

				// 全体フレームを超えたらリセット
				if (P1.YPos + P1.YVel * 2 >= GROUND)P1.time = 0, P1.stateno = 606,
											P1.ctrl = 0, P1.SFlg = 0;

				break;

				//********************
				// 606 着地硬直
				//********************
			case 606:
				P1.ctrl = 0, P1.SFlg = 0;
				// 速度をつける
				VelSet(0, 0);
				P1.YPos = GROUND;
				
				// SEを鳴らす
				if (P1.time == 1){
					SEStart(6);
				}



				// 全体フレームを超えたらリセット
				if (P1.time >= 17)P1.stateno = 11, P1.SFlg = 1, P1.ctrl = 1;

				break;
			//********************
			// 610 凪払い A
			//********************
			case 610:	// A凪払い
				P1.ctrl = 0, P1.SFlg = 0;
				// SEを鳴らす、移動
				if (P1.time == 0){
					SEStart(21);
					P1.XVel = 4;
				}
				// 全体フレームを超えたらリセット
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				// ヒット数セット
				if (P1.time == 12){
					P1.MoveHit = 1;	// １回
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 16, 0);

				// ダメージセット
				if (P1.time >= 1){

					// [ダメージ]
					Damage(100, 0);
			
					// [ゲージ] 
					Power(120);
					HitTime(6, 53, 60, 18);
					
					// [ノックバック]
					HitVel(-2.8, -4.7, -2.6, -3.5);
					GuardVel(-3.8 ,-2.6 * 1.2);
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

				// ヒット時キャンセル
				if (P1.StopTime == 0){
					if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
						// [ゲージ技]
						HCancel();
					}
				}

				break;
			//********************
			// 611 凪払い B
			//********************
			case 611:
				P1.ctrl = 0, P1.SFlg = 0;
				// SEを鳴らす、移動
				if (P1.time == 1){
					SEStart(21);
					P1.XVel = 10;
				}
				// 全体フレームを超えたらリセット
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				// ヒット数セット
				if (P1.time == 14){
					P1.MoveHit = 1;	// １回
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 16, 0);

				// ダメージセット
				if (P1.time >= 1){
		
					// [攻撃判定セット]
					AttSet(0, 120, 100, 69, -100);

					// [ダメージ]
					Damage(105, 0);

					// [ゲージ] 
					Power(130);
					HitTime(8, 53, 60, 16);

					// [ノックバック]
					HitVel(-3.2, -4.8, -3.1, -3.4);
					P1.HitAnim = 1030;	// 空中喰らい
					// [喰らい中の浮き]
					P1.fallF = 1;
					// [ガード属性]
					P1.GuardF = 1;

					if (P2.SFlg == 2){	
						P1.A.wall_xvel = 0.4;
						P1.A.wall_yvel = -5.4;
					}
					else{ 
						P1.A.wall_xvel = 0.4;
						P1.A.wall_yvel = -5.8;
					}
					P1.A.wallLevel = 2;
					// SE
					P1.HitSE = 13;

				}
				else {
					DamReset();
				}

				// ヒット時キャンセル
				if (P1.StopTime == 0){
					if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
						// [ゲージ技]
						HCancel();
					}
				}

				break;
			//********************
			// 620 設置（ノーマル）
			//********************
			case 620:
				P1.ctrl = 0, P1.SFlg = 0;
				// SEを鳴らす
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 17, 0);

				// ヘルパー呼び出し.[1]
				if (P1.time == 0){
					HelperReset(0);
					H1[0].var = true;
					H1[0].time = 0;
					H1[0].stateno = 2000;
				}
				// ヘルパー呼び出し.[2]	設置
				if (P1.time == 12){
					HelperReset(2);
					H1[2].var = true;
					H1[2].time = 0;
					H1[2].stateno = 2020;
				}

				// ヘルパー削除
				if (P1.time >= 23){
					HelperReset(0);
					H1[0].var = false;
				}
				// 全体フレームを超えたらリセット
				if (P1.time >= 36)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

				break;
			//********************
			// 622 設置（空中）
			//********************
			case 622:
				P1.ctrl = 0, P1.SFlg = 2;
				P1.ignoreG = true;
				// 速度をつける
				if (P1.time == 0){
					P1.XVel = -2;
					P1.YVel = -2.8;
				}
				/*
				if (AnimElem(2)){
					VelSet(-1.1, 0);
				}
				*/
				// SEを鳴らす
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 17, 0);

				// ヘルパー呼び出し.[2]	設置
				if (P1.time == 12){
					HelperReset(2);
					H1[2].var = true;
					H1[2].time = 0;
					H1[2].HYVel = 5;
					H1[2].stateno = 2020;
				}
				// 重力緩和
				VelAdd(0, P1.C.yAccel-0.2);

				// 着地
				if (P1.YPos + P1.YVel * 2 >= GROUND)P1.time = 0, P1.stateno = 606, P1.ctrl = 0;

				break;
			//********************
			// 625 発射
			//********************
			case 625:
				P1.ctrl = 0, P1.SFlg = 0;
				// SEを鳴らす
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 18, 0);

				// ヘルパー変更.[2]、発射前なら変更
				if (P1.time == 0){
					if ((H1[2].var) && (H1[2].stateno == 2020)){
						//HelperReset(2);
						H1[2].var = true;
						H1[2].time = 0;
						H1[2].stateno = 2025;

						// レバー方向で速度変更
						if (P1.keyAtt[6])H1[2].HXVel = 9.2;
						else if (P1.keyAtt[4])H1[2].HXVel = -6.4;
					}
				}
				

				// 終了・19F
				if (P1.time >= 19)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

				break;
			//********************
			// 626 空中発射
			//********************
			case 626:
				P1.ctrl = 0, P1.SFlg = 2;
				// SEを鳴らす
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 18, 0);

				// ヘルパー変更.[2]、発射前なら変更
				if ((H1[2].var) && (H1[2].stateno == 2020)){
					//HelperReset(2);
					H1[2].var = true;
					H1[2].time = 0;
					H1[2].stateno = 2025;

					// レバー方向で速度変更
					if (P1.keyAtt[6])H1[2].HXVel = 9.2;
					else if (P1.keyAtt[4])H1[2].HXVel = -6.4;
				}
				// 時間経過でジャンプ降りに
				if (P1.time >= ANIMELEM)P1.ctrl = 1;

				// 着地で終了
				if (P1.YPos + P1.YVel * 2 >= GROUND)P1.time = 0, P1.stateno = 47, P1.More = 1;

				break;
			//********************
			// 700 大ブメ[5]
			//********************
			case 700:
				P1.ctrl = 0, P1.SFlg = 0;
				P1.A.gaugeHosei = true;
				// ゲージ消費
				if (P1.time == 1){
					P1.Power -= 500;
					SEStart(31);
					P1.colorCTime = 20;
					P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
				}
				// SEを鳴らす
				if (P1.time == 2){
					SEStart(22);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 19, 0);

				// ヘルパー呼び出し.[5]
				if (P1.time == 4){
					HelperReset(5);
					H1[5].var = true;
					H1[5].time = 0;
					H1[5].stateno = 2030;
				}
				// 終了・58F
				if (P1.time >= 58)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

				break;
			//********************
			// 800 二段切り
			//********************
			case 800:
				P1.ctrl = 0, P1.SFlg = 0;
				P1.A.gaugeHosei = true;
				P1.D.counter = true;
				P1.D.fatal = true;

				// ゲージ消費
				if (P1.time == 0){
					P1.Power -= 1000;
					S.StopTime = 10;
					S.Anten = 25;
					SEStart(25);
					// 中段エフェクト
					EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
				}

				// 前進
				if (P1.time == 1){
					VelSet(12.6,0);
				}
				if (P1.time <= 16){
					VelAdd(0.4, 0);
				}
				else{ P1.XVel = 0; }
				// SEを鳴らす
				if (P1.time == 7){
					SEStart(21);
				}

				if ((P1.time == 0) && (P1.StopTime == 0))PVOStart(P1.PSide, 20, 0);

				// 無敵
				if (P1.time <= 14)P1.Muteki = 1, P1.mutekiF = 0;
				else { P1.Muteki = 0; }

				// ヒット数セット	14フレ～
				if (P1.time == 13){
					P1.MoveHit = 1;	// １回
				}
				// ダメージセット、持続 2 フレーム？
				if (P1.time >= 1){
	
					// [攻撃判定セット]
					AttSet(0, 55, 115, 25, -115);

					// [ダメージ]
					Damage(0, 0);

					// [ゲージ] 
					P1.GetPow = 0, P1.GivePow = 0;
					HitTime(5, 10, 8, 8);

					P1.G_GuardTime = 8;
					// [ノックバック]
					HitVel(0, 0, 0, 0);
					P1.GuardF = 1;
					P1.HitAnim = 1060;
					P1.HitSE = 13;
				}
				else {
					DamReset();
				}
				// 当たったら変更
				if ((P1.CFlg) && (P2.stateno >= 1000)&& (P1.time >= 13)){
					P1.stateno = 801;
					P1.time = 0, P2.time = 0;
					P1.More = 1, P1.A.damage = 0;
					P1.XVel = 0;
					if (P1.muki == 0){
						P2.XPos = P1.XPos + 100;
					}
					else {
						P2.XPos = P1.XPos - 100;
					}
					// 相手側
					P2.YPos = GROUND;
				}

				// 立ち の 全体フレーム、54を超えたらリセット
				if (P1.time >= ANIMELEM){
					P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				}

				break;
			//********************
			// 801 二段切り：演出
			//********************
			case 801:
				P1.ctrl = 0, P1.SFlg = 0;
				P1.Muteki = 1, P1.mutekiF = 0;
				// 立ち の 全体フレーム、113を超えたらリセット
				if (P1.time > 113){
					P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				}
				// SEを鳴らす
				if (P1.time == 11){
					SEStart(22);
				}

				if ((P1.time == 42) && (P1.StopTime == 0))PVOStart(P1.PSide, 21, 0);

				// 2P無敵
				if (P1.time < 62){
					P2.Muteki = 1, P2.mutekiF = 0;
				}

				// ヒット数セット	14フレ～
				if (P1.time == 62){
					P1.MoveHit = 1;	// １回
				}
				// ダメージセット、持続 2 フレーム？
				if (P1.time == 62){

					// [攻撃判定セット]
					ExAtt(P1.PSide, 0, 60, 35, 25, -165);

					// [ダメージ]
					Damage(160, 80);
					// [ゲージ] 
					Power(0, 240);
					P1.HitStop = 2, 
						P1.G_HitTime = 90, P1.A_HitTime = 20;
					P1.G_GuardTime = 0;
					// [ノックバック]
					HitVel(-3.4, -4.1, -3.4, -4.1);
					P1.GuardF = 1;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitAnim = 1000;
					P1.HitSE = 13;
					// エフェクト
					HitEff(1, 1, 1);
					// 地震エフェクト
					P1.A.quakeTime = 3;
					P1.A.quakeY = 2;
				}
				else {
					DamReset();
				}
				break;
			//********************
			// 820 真空ブーメラン
			//********************
			case 820:
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
					// ヒット数設定
					P1.Var[11] = 0;
				}

				if ((P1.time == 0) && (P1.StopTime == 0))PVOStart(P1.PSide, 22, 0);

				// ヘルパー変更.[2]、発射前なら変更
				if (P1.time == 4){
					if ((H1[2].var) && (H1[2].stateno == 2020)){
						H1[2].var = true;
						H1[2].time = 5;

						if (P2.XPos < H1[2].XPos)
							H1[2].muki = 1;
						else if (P2.XPos > H1[2].XPos)
							H1[2].muki = 0;

						H1[2].YPos += 100;
						H1[2].stateno = 2100;
						P1.Var[12] = H1[2].XPos;	// 座標
						P1.Var[13] = 0;	// 時間リセット
					}
				}


				// 終了・34
				if (P1.time >= 34)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

				break;
				//********************
				// 830：落葉切り
				//********************
			case 830:
				P1.ctrl = 0, P1.SFlg = 2;
				P1.ignoreG = true;
				P1.A.gaugeHosei = true;

				// ゲージ消費
				if (P1.time == 0){
					P1.Power -= 1000;
					S.StopTime = 10;
					S.Anten = 25;
					SEStart(25);
					// 中段エフェクト
					EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
				}

				if ((P1.time == 0) && (P1.StopTime == 0))PVOStart(P1.PSide, 23, 0);

				// SE
				if (P1.time == 1)SEStart(3);

				if (P1.time == 0)VelSet(0, -0.1);
				if (P1.time == 5)VelSet(0, 12);
				if (P1.time > 5)VelAdd(0, GRAVITY);

				// ヒット数セット
				if (P1.time == 1)P1.MoveHit = 1;	// １回
				// ダメージセット、持続 5フレ
				if (P1.time >= 1){

					// [ダメージ]
					Damage(190, 30);
					// [ゲージ] 
					Power(0, 330);

					// [ヒットストップ・のけぞり時間]
					HitTime(10, 68, 60, 14);

					// [ノックバック]
					HitVel(-1.4, -1.8, -0.4, 12.0);
					P1.GuardF = 1;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.A.boundLevel = 2;
					P1.A.bound_xvel = -1.6;
					P1.A.bound_yvel = -3.4;
					P1.HitAnim = 1000;
					P1.HitSE = 14;
					// エフェクト
					HitEff(1, 1, 1);
				}
				else {
					//DamReset();
				}
				if (P1.YPos + P1.YVel * 2 >= GROUND){
					P1.stateno = 831, P1.More = 1, P1.time = 0;
					P1.SFlg = 0;
					VelSet(0, 0);
					P1.ignoreG = false;
				}

				break;
				//********************
				// 831：ジャンプD着地
				//********************
			case 831:
				P1.SFlg = 0, P1.A.damage = 0, P1.ctrl = 0;
				P1.YPos = GROUND;
				P1.ignoreG = false;
				VelSet(0, 0);
				if (P1.time == 0){
					SEStart(8);
					EffStartB(16, P1.XPos, P1.YPos, 0, -1,
						0.20, 0.04, P1.muki);
				}

				// ヒット時スパキャン
				if (P1.StopTime == 0){
					if ((P1.time >= 0) && (P2.stateno >= 1000)){
						// [ゲージ技]
						HCancel();
					}
				}

				// SEを鳴らす
				if (P1.time == 1)SEStart(6);
				// 最初にロック！
				if (P1.time == 0)P1.Lock = 1;
				// 前ジャンプ の 全体フレーム
				if (P1.time >= ANIMELEM)P1.More = 1, P1.stateno = 0,
					P1.time = 0, P1.Lock = 0;
				break;
			//********************
			// 850 裂空ブーメラン（3ゲージ）
			//********************
			case 850:
				P1.ctrl = 0, P1.SFlg = 0;
				P1.A.gaugeHosei = true;
				// ゲージ消費
				if (P1.time == 1){
					P1.Power -= 3000;
					S.StopTime = 10;
					S.Anten = 30;
					SEStart(25);
					// 中段エフェクト
					EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
				}

				if ((P1.time == 0) && (P1.StopTime == 0))PVOStart(P1.PSide, 25, 0);

				// 無敵
				/*
				if (P1.time <= 2){
					P1.mutekiF = 0;
					P1.Muteki = 1;
				}
				*/

				// ヒット数セット 10フレ
				if (P1.time == 1){
					P1.MoveHit = 1;	// １回
				}
				// ダメージセット
				if ((P1.time >= 1) && (P1.time <= 10)){

					// [ダメージ]
					Damage(0, 0);
					// [ゲージ] 
					Power(0, 0);
					HitTime(12, 300, 300, 15);
					// [ノックバック]
					//HitVel(-0.5, -14.8, -0.5, -14.0);
					HitVel(-0.5, -7.6, -0.5, -7.2);
					GuardVel(-3.2, -2.8);
					P1.GuardF = 1;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.HitAnim = 1030;	// 空中喰らい(確定ダウン)
					P1.HitSE = 12;
					P1.A.yaccel = 0.2;
					P1.A.addMuteki = true;
					P1.A.amTime = 30;
				}

				if ((P1.CFlg) && (P2.stateno >= 1000) && (P1.time > 0)){
					P1.stateno = 851, P1.time = 0;
					P1.A.addMuteki = false;
				}

				// 終了
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				break;
				//********************
				// 851 ３ゲージ演出
				//********************
			case 851:
				P1.ctrl = 0, P1.SFlg = 0;
				P1.Muteki = 1, P1.mutekiF = 0;

				// ヒット数セット 10フレ
				if (P1.time == 1){
					SEStart(23);	// 風
					S.Anten = 80;

					P1.Var[15] = (int)P1.XPos;
					if (P1.muki == 0){
						P1.Var[15] += 110;
					}
					else{ P1.Var[15] -= 110; }
				}
				// 攻撃判定
				if (AnimElem(18) || AnimElem(19) || AnimElem(20)){
					P1.MoveHit = 1;	// １回
				}
				if (AnimElem(21)){
					P1.MoveHit = 1;
				}

				if ((P1.State2 != P1.oldState2) && (P1.State2 == 14)){
					S.noGauge = 15;
					S.StopTime = 15;
					SEStart(25);
				}

				if ((P1.State2 != P1.oldState2) && (P1.State2 == 16)){
					EffStart(6, P1.XPos, (int)P1.YPos, 110, -160,
						1.2, 0.9, P1.muki);
				}

				if ((P1.State2 != P1.oldState2) && (P1.State2 == 18)){
					VelSet(-12, 0);
					PVOStart(P1.PSide, 26, 0);
				}

				// ヘルパー戻りブメ
				if ((P1.State2 != P1.oldState2) && (P1.State2 == 33)){
					HelperReset(0);
					H1[0].var = true;
					H1[0].time = 0;
					H1[0].stateno = 2150;

					SEStart(27);
				}

				// キャッチ音
				if ((P1.State2 != P1.oldState2) && (P1.State2 == 35)){
					SEStart(28);
				}
				
				// ダメージセット
				if ((P1.State2 >= 17) && (P1.State2 <= 22)){

					// [ダメージ]
					Damage(90, 15);
					// [ゲージ] 
					Power(0, 150);
					
					HitTime(19, 200, 200, 15);
					P1.HSSelf = 0;
					// [ノックバック]
					HitVel(-2.6, -6.2, -2.6, -6.2);
					P1.GuardF = 1;
					// [喰らい中の浮き]
					P1.fallF = 1;
					P1.A.boundLevel = 1;
					P1.HitAnim = 1030;	// 空中喰らい(確定ダウン)
					P1.HitSE = 13;
					if (AnimElem(21)){
						P1.HitSE = 14;
						Damage(0, 165);
					}
					// エフェクト
					HitEff(3, 0.6, 0.6);

				}
				else{ DamReset(); }

				// 終了
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
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

					// 全体フレーム33or他のステートへ
					if ((H1[i].time > 33) || 
						(P1.stateno != 600 && P1.stateno != 620)){
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
						H1[i].A.damage = 80;
					}
					// ダメージ位置セット
					if (H_triggerT(i, 0, 1)){

						//[ 攻撃判定セット ]
						//H2_AttSet(i, 0, 50, 30, -20, -36);

						// [ヒット・ガードサウンド]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [ゲージ] 
						H1[i].HGetPow = 100, H1[i].HGivePow = 50;
						//[ ガード条件 ]
						H1[i].HGuardF = 1;
						// [ヒット硬直]
						H_HitTime(i, 6, 24, 26, 16);
						H1[i].HSSelf = 0;

						H1[i].fallF = 2;

						// [ヒット時のアニメ]
						H1[i].HHitAnim = 1000;
						// [ノックバック]
						H1[i].H_GX = -2.5, H1[i].H_GY = 0;
						H1[i].H_AX = -1.6, H1[i].H_AY = -3.2;
					}


					H_Move(i);

					//***************
					//  行動内容
					//***************
					// アニメをループさせる
					if (H1[i].HAnimTime >= 16)H1[i].HAnimTime = 0;
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
						H1[i].HMoveHit = 0;
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
					if (H1[i].HAnimTime >= 40)H1[i].HAnimTime = 0;
					// 終了間際に半透明に
					if (H1[i].time >= 300)H1[i].Alpha = true;
					// 終了条件、時間経過
					if (H1[i].time >= 360){
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
					H1[i].Alpha = false;
					if (H1[i].time == 0)H1[i].muki = P1.muki;

					// 座標・速度設定
					// 速度は 625 で決める
					// 速度がないときはここで決める
					if (H1[i].time == 0 && H1[i].HXVel == 0){
						H1[i].HXVel = 5.4;
					}
					// 方向転換
					if ((H1[i].HXVel < 0) &&
						((H1[i].muki == 0 && H1[i].XPos < (S.ScroolX + 20)) ||
						(H1[i].muki == 1 && H1[i].XPos >(S.ScroolX + SCREEN_W - 20)))){
						H1[i].HXVel = -H1[i].HXVel - (H1[i].HXVel * 0.2);
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
						//H2_AttSet(i, 0, 50, 30, -20, -38);

						// [ヒット・ガードサウンド]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [ゲージ] 
						H1[i].HGetPow = 50, H1[i].HGivePow = 25;
						//[ ガード条件 ]
						H1[i].HGuardF = 1;

						// [ヒット硬直]
						H_HitTime(i, 6, 20, 34, 20);
						H1[i].HSSelf = 0;

						H1[i].fallF = 2;

						// [ヒット時のアニメ]
						H1[i].HHitAnim = 1000;
						// [ノックバック]
						H1[i].H_GX = -1.9, H1[i].H_GY = 0;
						H1[i].H_AX = -1.2, H1[i].H_AY = -3;
					}

					// バウンド
					if (H1[i].YPos + H1[i].HYVel >= GROUND){
						H1[i].HYVel = -H1[i].HYVel;
						SEStart(5);
					}
					H_Move(i);

					//***************
					//  行動内容
					//***************
					// アニメをループさせる
					if (H1[i].HAnimTime >= 24)H1[i].HAnimTime = 0;
					// 終了条件、その向きで端到達or攻撃ヒット
					if ((H1[i].XPos < 0 || H1[i].XPos > STAGE_WIDTH) ||
						(H1[i].muki == 1 && H1[i].XPos < (S.ScroolX)) ||
						(H1[i].muki == 0 && H1[i].XPos >(S.ScroolX + SCREEN_W)) ||
						(H1[i].HMoveHit == 0 && H1[i].time >= 4) ||
						(P1.stateno >= 1000)
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

						H_PosSet(i, 0, -100);
					}
					// 速度設定
					if (H1[i].time == 0){
						H1[i].HXVel = 12.0;
					}
					// 速度設定
					if (H1[i].time == 16){
						H1[i].HXVel = 1.4;
					}
					if (H1[i].time == 31){
						H1[i].HXVel = -13.4;
					}

					
					// ヒット数・ダメージセット
					if ((H1[i].time == 1) || (H1[i].time == 16) ||
						(H1[i].time == 31)){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 65;
					}

					// ダメージ位置セット、2フレ以降
					if (H1[i].time >= 1){

						//[ 攻撃判定セット ]
						//H2_AttSet(i, 0, 80, 60, -40, -108);

						// [ヒット・ガードサウンド]
						H1[i].HHitSE = 12, H1[i].HGuardSE = 17;
						// [ゲージ] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 42;
						//[ ガード条件 ]
						H1[i].HGuardF = 1;

						// [ヒット硬直]
						H_HitTime(i, 6, 36, 40, 16);
						H1[i].HSSelf = 0;

						// [ヒット時のアニメ]
						H1[i].HHitAnim = 1000;

						// [ノックバック]
						H_HitVel(i, 
							-1.9, 0, -2.1, -3);
						if (H1[i].time >= 16){
							H_HitVel(i, 0.6, 0,
								0.5, -1.6);
						}
						if (H1[i].time >= 25){
							H_HitVel(i, 2.1, -4.1, 
										2, -4.3);
						}

						// [補正]
						if (H1[i].time >= 31)P1.A.hosei_K = 0.4;

					}

					H_Move(i);

					//***************
					//  行動内容
					//***************
					// アニメをループさせる
					if (H1[i].HAnimTime >= P1.animElem[H1[i].stateno])H1[i].HAnimTime = 0;
					// 終了条件、時間経過or喰らい
					if ((H1[i].time > 44) || (P1.stateno >= 1000))
					{
						HelperReset(i);
					}
					break;

				//********************
				// 2100 真空ブーメラン
				//********************
				case 2100:
					
					//***************
					//  基本設定
					//***************
					H1[i].HYVel = 0;
					if (H1[i].YPos >= GROUND)H1[i].YPos = GROUND;
					H1[i].Alpha = true;	//透明化
					P1.A.gaugeHosei = true;
					P1.Var[13]++;	// 時間加算

					//SE
					if (H1[i].time == 6 && P1.Var[11] == 0)SEStart(23);


					// 座標・速度設定
					// 速度がないときはここで決める
					if (H1[i].time == 5 && H1[i].HXVel == 0){
						H1[i].HXVel = 0;// 1.0;
					}
					if (H1[i].time == 7){
						H1[i].HXVel = 0;//5.0;
					}

					// ヒット数・ダメージセット
					if (H1[i].time == 6){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 54;
						H1[i].A.hosyo = 6;
					}
					
					// ダメージ位置セット、2フレ以降
					if (H1[i].time >= 6){


						// [ヒット・ガードサウンド]
						H1[i].HHitSE = 13, H1[i].HGuardSE = 17;
						// [ゲージ] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 80;

						//[ ガード条件 ]
						H1[i].HGuardF = 1;
						H1[i].fallF = 1;

						// [ヒット硬直]
						//H_HitTime(i, 6, 32, 60, 14);
						H_HitTime(i, 6, 32, 60, 14);
						H1[i].HSSelf = 1;
						// [ヒット時のアニメ]
						H1[i].HHitAnim = 1030;
						// [ノックバック]
						H1[i].H_GX = 0, H1[i].H_GY = -2;
						H1[i].H_AX = 0, H1[i].H_AY = -2.8;
						// エフェクト
						h_HitEff(i, 3, 0.6, 0.6);
					}
					// 攻撃がヒットしたら、
					if (H1[i].HMoveHit == 0 && H1[i].time >= 6 && H1[i].stopTime == 0)
					{
						H1[i].time = 3;	// 0
						H1[i].HXVel = 0;
						P1.Var[11] += 1;
					}

					if (H1[i].stopTime == 0){
						H_Move(i);
					}
					
					// エフェクトとして描画
					EasyEffDraw(5, H1[i].HAnimTime, (int)H1[i].XPos, (int)H1[i].YPos, 0, 0, 0.55, 0.55, H1[i].muki);

					//***************
					//  行動内容
					//***************

					// アニメをループさせる
					if (H1[i].HAnimTime >= 10)H1[i].HAnimTime = 0;
					// 終了条件、端到達or５回ヒット
					if (//(H1[i].XPos < (S.ScroolX - H1[i].WAtt[0])) ||
						//(H1[i].XPos > (S.ScroolX + SCREEN_W + H1[i].WAtt[0])) ||
						(H1[i].XPos < GAMENHAJI) ||
						(H1[i].XPos >(STAGE_WIDTH - GAMENHAJI)) ||
						(P1.Var[11] >= 5) ||
						//(P1.stateno >= 1000) ||
						(H1[i].time > 300)
						//|| (H1[i].XPos > P1.Var[12] + 290 || 
						//H1[i].XPos < P1.Var[12] - 290)
						)
					{
						P1.Var[11] = 0;
						P1.Var[12] = 0;
						HelperReset(i);
					}
					break;
					
					//********************
					// 2150 裂空戻りブメ 
					//********************
				case 2150:

					//***************
					//  基本設定
					//***************	
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H_PosSet(i, 18, -160);
					}
					//***************
					//  行動内容
					//***************

					// 全体フレームor被ダメ
					if ((H1[i].time > P1.animElem[H1[i].stateno]) ||
						(P1.stateno >= 1000)){
						HelperReset(i);
					}
					break;

					//********************
					// 2552 分身 
					//********************
				case 2552:
					//***************
					//  基本設定
					//***************	
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H_PosSet(i, 0, 0);
					}
					//***************
					//  行動内容
					//***************
					//H1[i].HImage = P1.Image;

					// ステートが新しくなったら
					//if (H1[i].stateno != H1[i].oldState){
						//H1[i].Reset();
					//}
					if(H1[i].time == 0)H1[i].Reset();

					// 全体フレームor被ダメ
					if (H1[i].time > 2000){
						HelperReset(i);
					}
					break;

					//===============================//
				//******************
				// ヘルパー処理終了
				//******************

				// ステート番号を保存
				H1[i].oldState = H1[i].stateno;

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
void GetP_EinParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_EinParam(System_t GS)
{
	S = GS;
}

void GetH_EinParam(Helper GH1[], Helper GH2[])
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


//-------------------------------------------------------------------------------
//
// 数値初期化
//
//-------------------------------------------------------------------------------




// 必殺技キャンセル
void SCancel()
{


	// [必殺技]
	// D (ノーマル設置、地上発射)
	if (P1.Senkou[4] > 0){
		// 地上にいて、ヘルパー[2]が存在しない
		if ((!H1[2].var) && (P1.SFlg != 2)){
			P1.stateno = 620, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
		// ヘルパー[2]がある
		if (H1[2].var){
			if (P1.SFlg != 2){
				P1.stateno = 625, P1.More = 1,
					P1.time = 0, P1.A.damage = 0;
			}
			else if (P1.SFlg == 2){
				P1.stateno = 626, P1.More = 1,
					P1.time = 0, P1.A.damage = 0;
			}
		}
	}

	//  投げ
	if ((P1_BInput(1) == 1) && (P1_BInput(4) == 1) && (P1.ctrl)){
		if (P1.SFlg != 2){		// 立ち
			P1.stateno = 500;	// 投げ
		}
		else {
			P1.stateno = 510;	// 空投げ
		}
	}
	// 236 + 攻撃
	if ((P1.cmd[1]) && (!H1[1].var)){		// 画面にヘルパー１がない
		if ((P1.Senkou[1] > 0) ||
			(P1.Senkou[2] > 0) ||
			(P1.Senkou[3] > 0)){
			P1.stateno = 600, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
			if (P1.Senkou[1] >= P1.Senkou[2]){
				if (P1.Senkou[1] >= P1.Senkou[3])P1.Var[6] = 1;
				else{ P1.Var[6] = 3; }
			}
			else if (P1.Senkou[2] >= P1.Senkou[3])P1.Var[6] = 2;
			else{ P1.Var[6] = 3; }
		}
	}

	// 214 + AorB
	if ((P1.Senkou[2] > 0)
		&& (P1.cmd[2])){		// 先行入力効かせてみる
		P1.stateno = 611, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	else if ((P1.Senkou[1] > 0)
		&& (P1.cmd[2])){		// 先行入力効かせてみる
		P1.stateno = 610, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// [EX技] 0.5ゲージ
	if ((P1.Senkou[4] > 0)
		&& (P1.cmd[1]) && (P1.Power >= 500)){
		P1.stateno = 700, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}

	// Hさせて頂く。
	HCancel();

	//最後に判定を消す
	DamReset();
}


// 超必キャンセル
void HCancel()
{

	// [超必]
	//  214+C		[二段切り]
	if ((P1.Senkou[3] > 0) && (P1.cmd[2])
		&& (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 800, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	// [ 真空ブーメラン ]
	if ((P1.Senkou[4] > 0) && (P1.cmd[3])
		&& (P1.Power >= 1000)
		&& (H1[2].var) && (H1[2].stateno == 2020)){
		P1.stateno = 820, P1.More = 1,
			P1.time = 0;
	}

	// [ 裂空ブーメラン ]
	if ((P1.Senkou[1] > 0 && P1.Senkou[2] > 0 && P1.Senkou[3] > 0) && (P1.Power >= 3000)){
		P1.stateno = 850, P1.More = 1;
		P1.time = 0;
	}
	// EXアクション
	// A..ゲージ増加
	if ((P1.Senkou[1] > 0) && (P1.cmd[3]) && (P1.Var[30] == 0)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 550;
			P1.More = 1;
			P1.time = 0;// ゲージ増加
		}
	}

	//最後に判定を消す
	DamReset();
}

// 空中必キャンセル
void ACancel()
{
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
	if ((P1.cmd[1]) && (!H1[1].var)){		// 画面にヘルパー１がない
		if ((P1.Senkou[1] > 0) ||
			(P1.Senkou[2] > 0) ||
			(P1.Senkou[3] > 0)){
			P1.stateno = 605, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
			if (P1.Senkou[1] >= P1.Senkou[2]){
				if (P1.Senkou[1] >= P1.Senkou[3])P1.Var[6] = 1;
				else{ P1.Var[6] = 3; }
			}
			else if (P1.Senkou[2] >= P1.Senkou[3])P1.Var[6] = 2;
			else{ P1.Var[6] = 3; }
		}
	}

	// [超必]
	//  214+C		[二段切り]
	if ((P1.Senkou[3] > 0) && (P1.cmd[2])
		&& (P1.Power >= 1000)){
		if (P1.SFlg == 2){		// 地上
			P1.stateno = 830, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	//最後に判定を消す
	DamReset();
}