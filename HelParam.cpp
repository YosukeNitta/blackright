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
void HelParam(void)
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

	//=============
	// キャラ別事前設定
	//=============
	if (P1.Var[11] > 3000)P1.Var[11] = 3000;	// シャインゲージ
	if (P1.Var[11] < 0)P1.Var[11] = 0;

	// D突進カウント
	if (P2.stateno < 1000)P1.Var[3] = 0;

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
			// 最初にロック！
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
			if (P1.AnimTime <= 3)P1.XVel = P1.C.runF[0] * 0.5; //速度を足す;
			// SEを鳴らす
			if ((P1.time == 14) || (P1.time == 31))SEStart(4);
			if (P1.time > ANIMELEM)P1.time = 0;


			break;
		case 35:	// バックステップ
			P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
			P1.ignoreG = true;

			// 無敵
			if (P1.time <= 4){
				P1.Muteki = 1, P1.mutekiF = 0;
			}

			// SEを鳴らす
			if (P1.time == 1)SEStart(5);

			VelAdd(0, P1.C.yAccel);

			//速度を足す
			if (P1.time == 0)VelSet(P1.C.runB[0], P1.C.runB[1]);


			// 終了
			if ((P1.YPos >= GROUND) && (P1.time > 2)){
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

			// SEを鳴らす
			if (AnimElem(9))SEStart(3);

			// 全体フレームを超えたら立ちへ
			if (P1.time >= P1.animElem[P1.stateno]){
				P1.time = 0;
				P1.stateno = 0;
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
			//[ 通常技の自動振り向き ]
			if (P1.time == 0){
				if ((P1.XPos < P2.XPos) && (P1.muki == 1)){
					P1.muki = 0;
				}
				else if ((P1.XPos > P2.XPos) && (P1.muki == 0)){
					P1.muki = 1;
				}
			}
			// 先行入力消す
			if (P1.time <= 1)P1.Senkou[1] = 0;

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
			if (P1.time >= 1){

				// [ダメージ]
				Damage(46, 0);

				// [ゲージ] 
				Power(30);
				// [ヒットストップ・のけぞり時間]
				HitTime(5, 13, 17, 11);
				// [ノックバック]
				HitVel(-3.8, 0, -1.6, -4.8);

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
			// ヒット時チェーン
			if ((P1.CFlg) && (P1.time >= 1)){
				// [ジャンプキャンセル]
				if (P1.keyAtt[8]){		// 先行入力効かせてみる
					P1.stateno = 40, P1.More = 1,
						P1.time = 0, P1.A.damage = 0;
				}
				// [通常技]
				if (P1.Senkou[3] > 0){
					if (key(2)){
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
			// 205：立ちA
			//********************
		case 205:	//立ちA
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// ヒット数セット 
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			// 位置をずらす
			if (P1.time == 2){
				PosAdd(4, 0);
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(82, 0);

				// [ゲージ] 
				Power(94);
				HitTime(8, 18, 20, 14);

				// [ノックバック]
				HitVel(-2.6, 0, -1.4, -5);
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
				if ((P1.CFlg) && (P1.time >= 1)){
					// [ジャンプキャンセル]
					if (P1.K_Senkou[8]){		// 先行入力効かせてみる
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [通常技]
					if (P1.Senkou[3] > 0){
						if (key(2)){
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else{
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// [必殺技・ゲージ技]
					SCancel();
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
			// ヒット数セット 
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(80, 0);

				// [ゲージ] 
				Power(98);
				HitTime(8, 18, 20, 14);

				// [ノックバック]
				HitVel(-3.4, 0, -1.6, -5);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1000;	// 上段喰らい
				// エフェクト
				HitEff(3, 0.6, 0.6);
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
					if (P1.Senkou[3] > 0){
						// 下要素を確認
						if (key(2)){
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
						if ((key(6)) && (!key(2)) && (!key(8))){
							P1.stateno = 215, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else if (key(2) && P1.Var[1] == 0){
							P1.stateno = 310, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							P1.Var[1]++;
						}
					}
					else if (P1.Senkou[1] > 0){
						// 下要素を確認
						if (key(6) && (P1.Var[20] == 0)){
							P1.stateno = 205, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// [必殺技・ゲージ技]
					SCancel();
				}
			}

			break;
			//********************
			// 215：2B
			//********************
		case 215:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);

			// 位置をずらす
			if (P1.time == 0){
				VelAdd(5.2, 0);
			}

			// SEを鳴らす
			if (P1.time == 1)SEStart(3);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [ダメージ]
				Damage(100, 0);
				// [ゲージ] 
				Power(120);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 22, 24, 18);
				// [ノックバック]
				HitVel(-3.8, 0, -1.8, -4.0);
				// [ガード属性]
				P1.GuardF = 2;


				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// 下段喰らい
				P1.A.forceKurai = true;	// 喰らい姿勢を強制

				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1)){
					/*
					// [通常技]
					if (P1.Senkou[3] > 0){
						// 下要素を確認
						if (!key(2)){
							P1.stateno = 220, P1.More = 1,
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
			// 220：立ちC
			//********************
		case 220:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SEを鳴らす
			if (P1.time == 1)SEStart(3);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(105, 0);
				// [ゲージ] 
				Power(128);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 22, 24, 18);
				// [ノックバック]
				HitVel(-4.0, 0, -2.2, -4);
				// [ガード属性]
				P1.GuardF = 1;


				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// 下段喰らい

				// エフェクト
				HitEff(3, 0.6, 0.6);
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
			// 300：しゃがみA
			//********************
		case 300:
			P1.ctrl = 0, P1.SFlg = 1;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			//[ 通常技の自動振り向き ]
			if (P1.time == 0){
				if ((P1.XPos < P2.XPos) && (P1.muki == 1)){
					P1.muki = 0;
				}
				else if ((P1.XPos > P2.XPos) && (P1.muki == 0)){
					P1.muki = 1;
				}
			}
			// 先行入力消す
			if (P1.time <= 1)P1.Senkou[1] = 0;
			// SEを鳴らす
			if (P1.time == 1)SEStart(1);
			//  全体フレーム、14を超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(44, 0);

				// [ゲージ] 
				P1.GetPow = 24, P1.GivePow = 12;

				// [ヒットストップ・のけぞり時間]
				HitTime(5, 12, 16, 10);

				// [ノックバック]
				HitVel(-4.4, 0, -1.9, -4.8);
				P1.GuardF = 3;

				P1.HitAnim = 1010;	// 下段喰らい
				P1.HitSE = 10;
			}
			else {
				DamReset();
			}
			// キャンセル
			if ((P1.CFlg) && (P1.time >= 1)){
				if (P1.Senkou[3] > 0){
					if (key(2)){
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
					else{
						P1.stateno = 200, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				// [必殺技・ゲージ技]
				SCancel();
			}
			// キャンセル、連打キャンセル
			if ((P1.button[1] == 1) && (P1.time >= 9) && (P1.stateno == 300)){
				// 下要素が入ってるやん！
				if (key(2)){
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
			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(75, 0);

				// [ゲージ] 
				Power(86);

				HitTime(8, 18, 20, 14);

				// [ノックバック]
				HitVel(-3.4, 0, -1.8, -4.6);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1010;	// 下段喰らい
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 11;
			}
			else{
				DamReset();
			}
			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1)){
					// [通常技]
					if (P1.Senkou[3] > 0){		// 先行入力効かせてみる
						if (key(2)){
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
						if (!key(2) && P1.Var[1] == 0){
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
			// 320：2Ｃ
			//********************
		case 320:
			P1.ctrl = 0, P1.SFlg = 1;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);

			// SEを鳴らす
			if (P1.time == 1)SEStart(3);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(100, 0);
				// [ゲージ] 
				Power(120);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 32, 34, 18);
				// [ノックバック]
				HitVel(-1.8, -3.6, -1.8, -3.4);
				GuardVel(-3.8, -1.8);
				// [ガード属性]
				P1.GuardF = 3;


				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// 下段喰らい

				P1.A.boundLevel = 1;
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 1)){
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
			if (P1.time == 1){
				SEStart(1);
			}
			// 先行入力消す
			if (P1.time <= 1)P1.Senkou[1] = 0;
			// ヒット数セット
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
				HitTime(5, 14, 18, 12);
				// [ノックバック]
				HitVel(-4.2, 0, -1.2, -4.0);
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
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					if (P1.Senkou[3] > 0){		// 先行入力効かせてみる
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (P1.Senkou[2] > 0){		// 先行入力効かせてみる
						P1.stateno = 410, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					/*
					if (P1.Senkou[1] > 0){		// 先行入力効かせてみる
						P1.stateno = 400, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					*/
					// [空中必殺技]
					ACancel();
				}

				// [ジャンプキャンセル]
				if ((P1.CFlg) && (P1.time >= 1) && (P1.AirJump > 0)){
					if (P1.K_Senkou[8] >= 1){		// 先行入力効かせてみる
						P1.AirJump -= 1;
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM){
				P1.ctrl = 1, P1.More = 1,
					P1.stateno = 46, P1.time = 0;
			}
			break;
			//********************
			// 410：ジャンプB
			//********************
		case 410:
			P1.ctrl = 0, P1.SFlg = 2;
			// SE
			if (P1.time == 1)SEStart(1);

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(75, 0);

				// [ゲージ] 
				Power(80);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 16, 18, 14);
				// [ノックバック]
				HitVel(-4.6, 0, -1.4, -4.0);
				P1.GuardF = 2;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				// エフェクト
				HitEff(3, 0.6, 0.6);
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
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					if (P1.Senkou[3] > 0){		// 先行入力効かせてみる
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}

					// [空中必殺技]
					ACancel();
				}

				// [ジャンプキャンセル]
				if ((P1.CFlg) && (P1.time >= 1) && (P1.AirJump > 0)){
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
			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 5フレ
			if (P1.time >= 1){

				// [ダメージ]
				Damage(95, 0);

				// [ゲージ] 
				Power(100);

				// [ヒットストップ・のけぞり時間]
				HitTime(8, 17, 40, 14);

				// [ノックバック]
				HitVel(-4.8, 0, -3.2, 6.9);
				P1.GuardF = 2;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;
				P1.A.boundLevel = 1;
			}
			else {
				DamReset();
			}
			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [空中必殺技]
					ACancel();
				}

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


			// SE
			if (P1.time == 1)SEStart(2);
			/// ヒット数セット、
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
			// もし距離が離れなかったら


			// ヒット音
			if (P1.time == 7)P1.HitSE = 11;

			// 2P無敵
			if (P1.time < 33)P2.Muteki = 1, P2.mutekiF = 0;


			// ヒット数セット
			if (P1.time == 34)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 1フレ
			if (P1.time == 34){

				// [攻撃判定セット]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [ダメージ]
				Damage(0, 180);
				P1.A.hosei_K = 0.5;
				// [ゲージ] 
				Power(300);
				HitTime(6, 45, 45, 0);
				// [ノックバック]
				HitVel(-2.6, -3.4, 0, 0);
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
			if (P1.time == 4)	// 投げのタイミングが被ってない
			{
				DamReset();
				P1.MoveHit = 1;	// １回
			}

			
			//if (P1.time == 1)DamReset();
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
				Damage(170, 10);
				// [ゲージ] 
				Power(300);

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
			// 600 チャージ1
			//********************
		case 600:	// Aブメ
			P1.ctrl = 0, P1.SFlg = 0;

			P1.Var[11] += 15;	// 20
			if (P1.Var[11] > 3000)P1.Var[11] = 3000;

			if (P1.time == 0)SEStart(39);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 601, P1.ctrl = 0, P1.More = 1;

			break;
			//********************
			// チャージ2
			//********************
		case 601:
			P1.ctrl = 0, P1.SFlg = 0;

			P1.Var[11] += 16;	// 22
			if (P1.Var[11] > 3000)P1.Var[11] = 3000;

			// ループ
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 601, P1.ctrl = 1, P1.More = 1;

			// D離し or ゲージMAX で解除
			if (P1.button[4] == 0 || P1.Var[11] >= 3000){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				StopSE(39);
			}
			break;
			//********************
			// 610 スピナー
			//********************
		case 610:
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
			}

			if (P1.time == 0){
				EffStart(16, P1.XPos, P1.YPos, -10, -(P1.ySize / 1.5),
					0.04, 0.25, P1.muki);
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 5){
				P1.XVel = 13;
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(110, 0);
				// [ゲージ] 
				Power(135);
				HitTime(4, 30, 32, 24);
				// [ノックバック]
				HitVel(-2.6, -4.5, -2.8, -3.2);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 0.2;
				P1.A.boundLevel = 0;
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
			// 611 Bスピナー
			//********************
		case 611:
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
			}

			if (P1.time == 0){
				EffStart(16, P1.XPos, P1.YPos, -10, -(P1.ySize / 1.5),
					0.04, 0.25, P1.muki);
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 5){
				P1.XVel = 20;
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(105, 0);
				// [ゲージ] 
				Power(130);
				HitTime(4, 30, 32, 24);
				// [ノックバック]
				HitVel(-2.6, -4.5, -2.8, -3.2);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 0.2;
				if (P1.HitCount == 0){
					P1.A.boundLevel = 2;
					P1.A.bound_xvel = -0.8;
					P1.A.bound_yvel = -3.2;
				}
				else{ P1.A.boundLevel = 0; }

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
			// 615 スピナー
			//********************
		case 615:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.touchF = false;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
			}

			if (P1.time == 0){
				EffStart(16, P1.XPos, P1.YPos, -10, -(P1.ySize / 1.5),
					0.04, 0.25, P1.muki);
			}

			// ゲージ消費
			if (P1.time == 1){
				P1.Var[11] -= 1000;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 5){
				P1.XVel = 20;
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(110, 20);
				// [ゲージ] 
				Power(170);
				HitTime(4, 50, 60, 26);
				// [ノックバック]
				//HitVel(0.0, -8.2, 0.0, -6.6);
				HitVel(0.0, -7.0, 0.0, -6.4);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;
				P1.A.yaccel = 0.30;
			}
			else {
				DamReset();
			}

			/*
			// キャンセル
			if (P1.StopTime == 0){
				// ヒット時キャンセル
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [ゲージ技]
					//HCancel();
				}
			}
			*/

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				P1.touchF = true;
			}
			break;
			//********************
			// 616：ジャンプD
			//********************
		case 616:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(3);

			if (P1.time == 0)VelSet(0, -0.1);
			if (P1.time == 8)VelSet(6.0, 8);

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 5フレ
			if (P1.time >= 1){

				// [ダメージ]
				Damage(110, 0);
				// [ゲージ] 
				Power(140);

				// [ヒットストップ・のけぞり時間]
				HitTime(8, 68, 60, 14);

				// [ノックバック]
				HitVel(-3.0, -1.8, -3.4, 6.0);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 2;
				P1.A.boundLevel = 3;
				P1.A.bound_yvel = -3.2;
				P1.HitAnim = 1000;
				P1.HitSE = 12;
				P1.A.hosei_K = 0.8;
			}
			else {
				DamReset();
			}
			if (Chakuchi()){
				P1.stateno = 619, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break; 
			//********************
			// 617：Ｄ突進（空）
			//********************
		case 617:
			P1.ctrl = 0, P1.SFlg = 2;
			//P1.A.gaugeHosei = true;
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(3);
			// ゲージ消費
			if (P1.time == 1){
				P1.Var[11] -= 1000;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			if (P1.time == 0)VelSet(0, -0.1);
			if (AnimElem(2))VelSet(8.2, 8.1);

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 5フレ
			if (P1.time >= 1){

				// [ダメージ]
				Damage(110, 10);
				// [ゲージ] 
				Power(190);

				// [ヒットストップ・のけぞり時間]
				HitTime(8, 68, 60, 18);

				// [ノックバック]
				HitVel(-2.2, -2.0, -3.2, 6.0);
				P1.GuardF = 2;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.A.boundLevel = 3;
				P1.A.bound_xvel = -0.8;
				P1.A.bound_yvel = -4.2;
				P1.HitAnim = 1000;
				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			if (Chakuchi()){
				P1.stateno = 618, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;
			//********************
			// 618：空Ｄ突進追い打ち
			//********************
		case 618:
			P1.ctrl = 0, P1.SFlg = 1;
			P1.YPos = GROUND;
			P1.ignoreG = false;
			VelSet(0, 0);

			// SEを鳴らす
			if (P1.time == 1)SEStart(3);

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// ヒット時キャンセル
			if ((P1.CFlg) && (P1.time >= 1)){
				// [必殺技・ゲージ技]
				P1.Var[3]++;
			}

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(80, 30);
				// [ゲージ] 
				Power(110);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 32, 34, 18);

				// ヒットカウント
				if (P1.Var[3] == 0){
					// [ノックバック]
					HitVel(0.0, -5.4, 0.0, -5.0);
				}
				else{ HitVel(-2.0, -3.0, -2.0, -2.8); }
				
				GuardVel(-3.8, -1.8);
				// [ガード属性]
				P1.GuardF = 1;

				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// 下段喰らい

				P1.A.boundLevel = 0;
				// エフェクト
				HitEff(3, 0.8, 0.8);
				P1.HitSE = 13;
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
			break;
			//********************
			// 619：ジャンプD着地
			//********************
		case 619:
			P1.SFlg = 0, P1.A.damage = 0, P1.ctrl = 0;
			P1.YPos = GROUND;
			P1.ignoreG = false;
			VelSet(0, 0);
			// SEを鳴らす
			if (P1.time == 1)SEStart(6);
			// 最初にロック！
			if (P1.time == 0)P1.Lock = 1;

			// キャンセル
			if (P1.StopTime == 0){
				// ヒット時キャンセル
				if (P2.stateno >= 1000){
					// [ゲージ技]
					HCancel();
				}
			}

			// 前ジャンプ の 全体フレーム
			if (P1.time >= ANIMELEM)P1.More = 1, P1.stateno = 0,
				P1.time = 0, P1.Lock = 0;
			break;
			//********************
			// 620 昇竜
			//********************
		case 620:	
			P1.ctrl = 0, P1.SFlg = 0;
			P1.D.counter = true;
			P1.D.fatal = true;

			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				//P1.mutekiF = 0;
			}
			//if(P1.time <= 8)P1.Muteki = 1;

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 7)
				EffStart(20, P1.XPos + 17, P1.YPos - 10, 0, 0,
				0.50, 0.50, P1.muki);

			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [ダメージ]
				Damage(110, 0);
				// [ゲージ] 
				Power(145);
				HitTime(8, 24, 24, 18);
				// [ノックバック]
				HitVel(-2.6, -3.2, -2.2, -3.0);
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
					//HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 625 Ｄ昇竜
			//********************
		case 625:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.D.counter = true;
			P1.D.fatal = true;

			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.mutekiF = 0;
			}
			if (P1.time <= 10)P1.Muteki = 3;

			// ゲージ消費
			if (P1.time == 1){
				P1.Var[11] -= 1000;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			// ヒット数セット
			if (P1.time == 8 || P1.time == 10){
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 7)
				EffStart(20, P1.XPos + 18, P1.YPos - 10, 0, 0,
				0.54, 0.51, P1.muki);

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(70, 10);
				// [ゲージ] 
				Power(80);
				HitTime(8, 24, 40, 18);
				// [ノックバック]
				HitVel(-2.6, -3.4, -2.2, -3.2);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;	// 地上受け身可能
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
					//HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;

			//********************
			// 630 ブメ投げ A
			//********************
		case 630:	// Aブメ
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす
			if (P1.time == 1){
				SEStart(2);
			}
			// ヘルパー呼び出し.[0]
			if (P1.time == 13){
				HelperReset(0);
				H1[0].var = true;
				H1[0].time = 0;
				H1[0].stateno = 2000;
			}

			// ヒット時キャンセル
			if (P1.StopTime == 0){
				if ((H1[0].var == false) && (P1.time >= 12) && (P1.scTime > 0)){
					// [ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;


			break;
			//********************
			// 635 ブメ投げ A
			//********************
		case 635:	// Aブメ
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす
			if (P1.time == 0){
				SEStart(2);
			}
			// ゲージ消費
			if (P1.time == 1){
				P1.Var[11] -= 2000;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			// ヘルパー呼び出し.(ノーマル波動)
			if (P1.time == 12){
				HelperReset(1);
				H1[0].var = true;
				H1[0].time = 0;
				H1[0].stateno = 2010;
			}
			/*
			// ヘルパー呼び出し(EX波動)
			if (P1.time == 21){
				HelperReset(0);
				H1[0].var = true;
				H1[0].time = 0;
				H1[0].stateno = 2010; 
			}
			*/

			// ヒット時キャンセル
			if (P1.StopTime == 0){
				if ((H1[0].var == false) && (P1.time >= 12) && (P1.scTime > 0)){
					// [ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;


			break;
			//********************
			// 700 大ブメ[5]
			//********************
		case 700:
			P1.ctrl = 0, P1.SFlg = 0;
			// ゲージ消費
			if (P1.time == 1)P1.Power -= 500;
			// SEを鳴らす
			if (P1.time == 2){
				SEStart(22);
			}
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

			// 無敵
			if (P1.time <= 9)P1.Muteki = 1, P1.mutekiF = 0;
			else { P1.Muteki = 0; }

			if (P1.time == 1){
				EffStart(16, P1.XPos, P1.YPos, -10, -(P1.ySize / 1.5),
					0.04, 0.25, P1.muki);
			}
			
			// 3456 9
			// 前進
			if (P1.time <= 5){
				P1.XVel = 10.0;
			}

			// ヒット数セット	14フレ～
			if (AnimElem(3) || AnimElem(4) ||
				AnimElem(5) || AnimElem(6) ||
				AnimElem(9)){
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット

			if (P1.time <= 1){
				DamReset();
			}
			if ((P1.time >= 7) && (P1.time < 24)){

				// [ダメージ]
				Damage(70, 5);
				// [ゲージ] 
				Power(0, 100);
				HitTime(6, 24, 24, 22);
				// [ノックバック]
				HitVel(-3.2, 0, -4.4, -2.0);
				if (P2.SFlg != 2){
					P1.HitAnim = 1000;	// 地上
				}
				else if (P2.SFlg == 2){
					P1.HitAnim = 1060;	// 地上へ
				}
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;
				// エフェクト
				HitEff(3, 0.6, 0.6);
			}
			// ダメージセット
			else if (P1.time >= 24){

				// [ダメージ]
				Damage(35, 30);
				// [ゲージ] 
				Power(0, 100);

				HitTime(10, 100, 100, 12);
				// [ノックバック]
				HitVel(-5.0, -3.6, -4.8, -3.2);

				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 14;
				// エフェクト
				HitEff(3, 0.8, 0.8);
			}

			// 当たったら変更
			if ((P1.CFlg) && (P1.time < 24) && (P2.stateno == 1060) && (P2.SFlg == 0)){
				P2.YPos = GROUND;
				
				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 40;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 40;
				if (P1.muki == 0){
					P2.XPos = P1.XPos + 40;
				}
				else if (P1.muki == 1){
					P2.XPos = P1.XPos - 40;
				}
				
			}

			// 立ち の 全体フレーム、54を超えたらリセット
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}

			break;
			//********************
			// 820 飛び道具
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
			}
			// ヘルパー変更.[2]、発射前なら変更
			if (P1.time == 6){
				HelperReset(2);
				H1[2].var = true;
				H1[2].XPos = P1.XPos;
				H1[2].time = 0;
				H1[2].stateno = 2020;
			}


			// 終了・34
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 830 フルチャージ
			//********************
		case 830:
			P1.ctrl = 0, P1.SFlg = 0;
			// ゲージ消費
			if (P1.time == 0){
				P1.Power -= 1000;
				S.StopTime = 10;
				S.Anten = 25;
				SEStart(25);
				// 中段エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}
			// ヘルパー変更.[2]、発射前なら変更
			if (P1.time == 15){
				P1.Var[11] += 2000;
				if (P1.Var[11] > 3000)P1.Var[11] = 3000;
				// 中段エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.50, 0.50, P1.PSide);
				SEStart(31);
			}

			// 終了・34
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 850 飛び道具
			//********************
		case 850:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			// ゲージ消費
			if (P1.time == 0){
				P1.Power -= 3000;
				S.StopTime = 15;
				S.noGauge = 15;
				S.Anten = 25;
				SEStart(25);
				// 中段エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}

			// 前進
			if (P1.time == 1){
				VelSet(12.8, 0);
				afterimageSetting(P1.PSide, 180, 155, 155, 255);
			}

			// 無敵
			if (P1.time <= 5){
				P1.mutekiF = 0;
				P1.Muteki = 1;
			}

			// ヒット数セット	14フレ～
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット、持続 2 フレーム？
			if (P1.time >= 1){


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
				HitEff(3, 0.6, 0.6);
			}
			else {
				DamReset();
			}
			// 当たったら変更
			if ((P1.CFlg) && (P2.stateno >= 1000) && (P1.time >= 1)){
				P1.stateno = 851;
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

			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				afterimageSetting(P1.PSide, 0, 255, 255, 255);
			}
			break;
			//********************
			// 851 3ゲージヒット時
			//********************
		case 851:
			P1.ctrl = 0;
			P1.A.gaugeHosei = true;
			
			if (P1.time == 23 - 1){
				VelSet(-2.4, -12);
				P1.SFlg = 2;
			}
			else if(P1.time == 0){
				P1.SFlg = 0;
			}

			if (P1.time == 23){
				//VelSet(-2.4, -5);
				//P1.SFlg = 2;
			}

			// ヒット数セット	14フレ～
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット、持続 2 フレーム？
			if (P1.time >= 1){

				// [ダメージ]
				Damage(0, 0);

				// [ゲージ] 
				P1.GetPow = 0, P1.GivePow = 0;
				HitTime(5, 120, 120, 8);

				P1.G_GuardTime = 8;
				// [ノックバック]
				HitVel(-1, 0, 0, 0);
				P1.GuardF = 1;
				P1.HitAnim = 1000;
				P1.HitSE = 11;
			}
			else {
				DamReset();
			}

			// 終了
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 852, P1.ctrl = 1;
				P1.More = 1;
				VelSet(0, 0);
			}
			break;
			//********************
			// 852 3ゲージ攻撃
			//********************
		case 852:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.A.gaugeHosei = true;

			if (P1.time == 9)
			{
				afterimageSetting(P1.PSide, 0, 255, 255, 255);
			}

			//
			if (P1.time == 6){
				int posY = P2.ySize;
				int posX = 20 + GetRand(10);
				for (int i = 0; i < 3; i++){
					float rand = GetRand(4);
					rand = (rand * 0.01) + 0.07;
					EffStart(9, (int)P2.XPos, (int)P2.YPos, posX, -(posY / 1.5),
						rand, rand, P2.muki);
				}
				EffStart(16, P2.XPos, P2.YPos - ((P2.GraphH * P2.GSize) / 2), 0, -1,
					0.40, 0.08, P2.muki);
			}

			if (P1.time >= 37 - 1){
				P1.ignoreG = false;
			}
			else{ P1.ignoreG = true; }
			
			if (P1.time == 37){
				VelSet(-1, -3);
			}

			// 攻撃判定
			if (P1.time == 0 || P1.time == 2 || P1.time == 4 || P1.time == 6){
				P1.MoveHit = 1;	// １回
				P1.A.kill = 0;
				if (P1.time == 6)P1.A.kill = 1;
			}
			//  暫定エフェクト
			if (P1.time <= 5){
				int x, y;
				x = (int)P2.XPos + GetRand(200) - 100;
				y = (int)P2.YPos - 10 - GetRand(170);
				EffStart(10, x, y, 0, -1,
					0.2, 0.2, P1.muki);	// ヒット
				for (int i = 0; i < 3; i++){
					float rand = GetRand(4);
					rand = (rand * 0.01) + 0.07;
					EffStart(9, x, y, 0, -1,
						rand, rand, P2.muki);
				}
				EffStart(1, x, y, 0, -1,
					0.6, 0.6, P1.muki);
			}

			// ダメージセット
			if ((P1.time >= 0) && (P1.time <= 9)){

				// [ダメージ]
				Damage(100, 20);
				// [ゲージ] 
				Power(0, 150);

				HitTime(18, 200, 200, 15);
				P1.HSSelf = 6;
				// [ノックバック]
				HitVel(-3.2, -6.2, -3.2, -6.2);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.A.boundLevel = 1;
				P1.HitAnim = 1030;	// 空中喰らい(確定ダウン)
				P1.HitSE = 13;
				if (P1.time == 6){
					P1.HitSE = 14;
					Damage(0, 150);
				}
				// エフェクト
				HitEff(0, 0.6, 0.6);

			}
			else{ DamReset(); }

			// 着地
			if (Chakuchi()){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 0;
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
					// 2000 小ブメ （地上版）
					//********************
				case 2000:
					EasyEffDraw(8, H1[i].HAnimTime, H1[i].XPos, H1[i].YPos,
						0, 0,
						0.5, 0.5, H1[i].muki);

					//***************
					//  基本設定
					//***************
					// 座標・速度設定
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;

						// 速度なかったら設定
						H_VelSet(i, 11.8, 0);

						
						if (P1.stateno == 635)H_PosSet(i, 20, -112);
						else{ H_PosSet(i, 68, -150); }
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
						H_HitTime(i, 8, 24, 24, 18);
						H1[i].HSSelf = 0;

						H1[i].fallF = 2;

						// [ヒット時のアニメ]
						H1[i].HHitAnim = 1000;
						// [ノックバック]
						H1[i].H_GX = -2.8, H1[i].H_GY = 0;
						H1[i].H_AX = -1.6, H1[i].H_AY = -3.2;
					}


					H_Move(i);

					//***************
					//  行動内容
					//***************
					// アニメをループさせる
					if (H1[i].HAnimTime >= 4)H1[i].HAnimTime = 0;
					// 終了条件、端に到達or地面に当たるorヒットor本体被ダメ
					if ((H1[i].XPos < (S.ScroolX)) ||
						(H1[i].XPos >(S.ScroolX + SCREEN_W)) ||
						(H1[i].YPos > GROUND) ||
						(H1[i].HMoveHit == 0) ||
						(P1.stateno >= 1000)
						)
					{
						HelperReset(i);
					}

					break;

					//********************
					// 2010 小ブメ （地上版）
					//********************
				case 2010:
					EasyEffDraw(8, H1[i].HAnimTime, H1[i].XPos, H1[i].YPos,
						0, 0,
						0.7, 0.7, H1[i].muki);

					//***************
					//  基本設定
					//***************
					// 座標・速度設定
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;

						// 速度なかったら設定
						H_VelSet(i, 10.2, 0);

						H_PosSet(i, 20, -112);
						P1.Var[12] = H1[0].XPos;	// 座標
						P1.Var[13] = 0;				// ヒット数設定
					}
					if (H1[i].time >= 3)H_VelSet(i, 10.2, 0);

					// ヒット数・ダメージセット
					if (H1[i].time == 2){
						H1[i].HMoveHit = 1;
						//H1[i].A.damage = 110;
						H1[i].A.hosyo = 5;
						H1[i].A.damage = 45;
					}
					// ダメージ位置セット
					if (H_triggerT(i, 0, 1)){

						//[ 攻撃判定セット ]
						//H2_AttSet(i, 0, 50, 30, -20, -36);

						// [ヒット・ガードサウンド]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [ゲージ] 
						H1[i].HGetPow = 150, H1[i].HGivePow = 75;
						//[ ガード条件 ]
						H1[i].HGuardF = 1;
						// [ヒット硬直]
						H_HitTime(i, 8, 32, 32, 18);
						H1[i].HSSelf = 6;

						H1[i].fallF = 2;

						// [ヒット時のアニメ]
						H1[i].HHitAnim = 1000;
						// [ノックバック]
						H1[i].H_GX = -2.2, H1[i].H_GY = -4.6;
						H1[i].H_AX = -1.4, H1[i].H_AY = -4.4;
					}

					// 攻撃がヒットしたら、
					if ((H1[i].HMoveHit == 0) && (H1[i].stopTime == 0) && (H1[i].time > 2))
					{
						H1[i].time = 1;	// 0
						H1[i].HXVel = 0.0;
						P1.Var[13] += 1;
					}
					if (H1[i].stopTime == 0){
						H_Move(i);
					}
					//H_Move(i);

					//***************
					//  行動内容
					//***************
					// アニメをループさせる
					if (H1[i].HAnimTime >= 4)H1[i].HAnimTime = 0;
					// 終了条件、端に到達or地面に当たるorヒットor本体被ダメ
					if (//(H1[i].XPos < (S.ScroolX + GAMENHAJI)) ||
						//(H1[i].XPos >(S.ScroolX + SCREEN_W - GAMENHAJI)) ||
						(H1[i].XPos < (S.ScroolX)) ||			// 外に出る
						(H1[i].XPos >(S.ScroolX + SCREEN_W)) ||
						(P1.Var[13] > 2) ||						// 3ヒット以上
						(H1[i].XPos > P1.Var[12] + 420 ||		// 進みすぎ
						H1[i].XPos < P1.Var[12] - 420) ||
						(P1.stateno >= 1000)
						)
					{
						// 設定リセットして消す
						P1.Var[12] = 0;
						P1.Var[13] = 0;
						HelperReset(i);
					}

					break;
					//********************
					// 2020 真空ブーメラン
					//********************
				case 2020:

					//***************
					//  基本設定
					//***************
					H1[i].YPos = GROUND;
					H1[i].Alpha = true;	//透明化
					P1.A.gaugeHosei = true;

					//SE
					if (H1[i].time == 1)SEStart(23);


					// 座標・速度設定
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H1[i].HXVel = 21.0;
						H1[i].attF = 1;
					}

					// ヒット数・ダメージセット
					if (H1[i].time == 1){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 170;
						H1[i].A.hosyo = 60;
					}
					

					// ダメージ位置セット、2フレ以降
					if (H1[i].time >= 1){


						// [ヒット・ガードサウンド]
						H1[i].HHitSE = 14, H1[i].HGuardSE = 17;
						// [ゲージ] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 300;

						//[ ガード条件 ]
						H1[i].HGuardF = 1;
						H1[i].fallF = 1;

						// [ヒット硬直]
						H_HitTime(i, 8, 32, 60, 14);
						H1[i].HSSelf = 2;
						// [ヒット時のアニメ]
						H1[i].HHitAnim = 1030;
						// [ノックバック]
						H1[i].H_GX = -3.2, H1[i].H_GY = -3.2;
						H1[i].H_AX = -2.8, H1[i].H_AY = -2.8;
						//
						H1[i].A.boundLevel = 1;
						// エフェクト
						h_HitEff(i, 3, 0.5, 0.5);

					}

					if (H1[i].stopTime == 0){
						H_Move(i);
					}

					// エフェクトとして描画
					EasyEffDraw(21, H1[i].HAnimTime, (int)H1[i].XPos, (int)H1[i].YPos, 0, 0, 0.7, 1, H1[i].muki);

					//***************
					//  行動内容
					//***************

					// アニメをループさせる
					if (H1[i].HAnimTime >= 8)H1[i].HAnimTime = 0;
					// 終了条件、端到達
					if ((H1[i].XPos < (S.ScroolX + GAMENHAJI)) ||
						(H1[i].XPos >(S.ScroolX + SCREEN_W - GAMENHAJI))
						)
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
	if (P1.PSide == 1)Get_PSet(P1, P2), Get_HSet(H1, H2);
	if (P1.PSide == 2)Get_PSet(P2, P1), Get_HSet(H2, H1);

}//StateParam終了




//-------------------------------------------------------------------------------
//
// 受け取り用
//
//-------------------------------------------------------------------------------

// 順番を逆にする
void GetP_HelParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_HelParam(System_t GS)
{
	S = GS;
}

void GetH_HelParam(Helper GH1[], Helper GH2[])
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



//-------------------------------------------------------------------------------
//
// キャンセル設定
//
//-------------------------------------------------------------------------------

// 必殺技キャンセル
void SCancel()
{
	// Ｄ
	if ((P1.Senkou[4] > 0 || P1.button[4] > 0) && (P1.Var[11] < 3000)
		&& (P1.SFlg != 2)){		// 先行入力効かせてみる
		P1.stateno = 600, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 236 + 攻撃
	if ((P1.cmd[1]) && (!H1[0].var)){		// 画面にヘルパー１がない
		if ((P1.Senkou[1] > 0) ||
			(P1.Senkou[2] > 0)){
			P1.stateno = 630, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	// 214 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0))
		&& (P1.cmd[2])){		// 先行入力効かせてみる
		if (P1.SFlg != 2){
			P1.stateno = 610, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
			if (P1.Senkou[2] > 0)P1.stateno = 611;
		}
	}
	// 623 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[4])){		// 先行入力効かせてみる
		if (P1.SFlg != 2){
			P1.stateno = 620, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	
	// 236 + D
	if ((P1.cmd[1]) && (!H1[0].var) && (P1.Senkou[4] > 0) && (P1.Var[13] == 0)){		// 画面にヘルパー１がない
		if (P1.Var[11] >= 2000){
			P1.stateno = 635, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
		else {
			P1.stateno = 630, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	// 214 + D
	if ((P1.Senkou[4] > 0) 
		&& (P1.cmd[2])){		// 先行入力効かせてみる
		if (P1.Var[11] >= 1000){
			P1.stateno = 615, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		}
		else {
			P1.stateno = 610, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		}
	}
	// 623 + D
	if ((P1.Senkou[4] > 0)
		&& (P1.cmd[4])){		// 先行入力効かせてみる
		if (P1.Var[11] >= 1000){
			P1.stateno = 625, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		}
		else {
			P1.stateno = 620, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		}
	}

	HCancel();
	//最後に判定を消す
	DamReset();
}

// 必殺技キャンセル
void HCancel()
{
	// [超必]
	//  236+C		[飛び道具]
	if ((P1.Senkou[3] > 0) && (P1.cmd[1])
		&& (P1.Power >= 1000) && (!H1[2].var)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 820, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	//  214+C		[二段切り]
	if ((P1.Senkou[3] > 0) && (P1.cmd[2])
		&& (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 800, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	//  22+D		[チャージ]
	if ((P1.Senkou[4] > 0) && (P1.cmd[3])
		&& (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// 地上
			P1.stateno = 830, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	// [ ハードパンチ ]
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
	//DamReset();

}

// 空中必キャンセル
void ACancel()
{
	// 214 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[2]) && (P1.YPos < GROUND - 20)){		// 先行入力効かせてみる
		if (P1.SFlg == 2){
			P1.stateno = 616, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	// 214 + D
	if (((P1.Senkou[4] > 0 || P1.button[4] == 1))
		&& (P1.cmd[2]) && (P1.YPos < GROUND - 20)){		// 先行入力効かせてみる
		if (P1.Var[11] >= 1000){
			P1.stateno = 617, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
		else{
			P1.stateno = 616, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	//最後に判定を消す
	//DamReset();
}
