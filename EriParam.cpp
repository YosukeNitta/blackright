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
void EriParam(void)
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
		// 40～47：
		// ジャンプ
		// 50～59：
		// ガード
		//--------------------
		if (((P1.stateno >= 40) && (P1.stateno <= 47)) ||	// ジャンプ
			((P1.stateno >= 50) && (P1.stateno <= 69))){	// ガード・空中ダッシュ・ハイジャンプ　
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
			GetS_SameParam(S);
			GetP_SameParam(P1, P2);
			DamParam();
		}

		// チェーン初期化
		if (P1.ctrl)P1.Var[1] = 0;
		// 連打スイッチ
		if (P1.ctrl)P1.Var[20] = 0;

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
			// 5 振り向き
			//********************
		case 5:

			P1.SFlg = 0, P1.ctrl = 1;
			VelSet(0, 0);
			if (P1.time == 0){
				P1.Lock = 1;
			}
			if (P1.time >= 8){
				P1.Lock = 0;
			}

			break;
			//********************
			// 6 振り向き（しゃがみ）
			//********************
		case 6:

			P1.SFlg = 1, P1.ctrl = 1;
			VelSet(0, 0);
			if (P1.time == 0){
				P1.Lock = 1;
			}
			if (P1.time >= 8){
				P1.Lock = 0;
				P1.More = 1, P1.time = 0, P1.stateno = 11;
			}

			break;
			//********************
			// 10 立ち→しゃがみ
			//********************
		case 10:
			P1.SFlg = 1;	//しゃがみ判定に
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// しゃがみから立ちへ 

			if (InputPAD(102) == 0){
				P1.time = 0, P1.stateno = 12, P1.More = 1;
			}


			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 11, P1.More = 1;

			break;
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
			// SEを鳴らす
			if ((P1.time == 14) || (P1.time == 31))SEStart(4);
			if (P1.time > ANIMELEM)P1.time = 0;


			break;
		case 35:	// バックステップ
			P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
			P1.ignoreG = true;

			// 無敵
			if (P1.time <= 3){
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
				HitTime(8, 13, 20, 11);
				// [ノックバック]
				HitVel(-2.8, 0, -1.4, -4.8);

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
			// 205：立ちA
			//********************
		case 205:	//立ちA
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
			// ヒット数セット 
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			if (P1.time == 1)VelSet(2.0, 0);
			// 位置をずらす
			if (P1.time == 2){
				PosAdd(6, 0);
			}
			if (P1.time == 2 + 4){
				PosAdd(8, 0);
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
				HitTime(8, 18, 22, 14);

				// [ノックバック]
				HitVel(-2.6, 0, -1.7, -5);
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
				if ((P1.CFlg) && (P1.time >= 6)){
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
				HitTime(8, 18, 22, 14);

				// [ノックバック]
				HitVel(-2.6, 0, -1.7, -5);
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
						// 下要素を確認
						if (key(6)){
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
					else if (P1.Senkou[2] > 0){
						// 下要素を確認
						if (key(2) && P1.Var[1] == 0){
							P1.stateno = 310, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							P1.Var[1]++;
						}
					}
					else if (P1.Senkou[1] > 0){
						// 下要素を確認
						if (key(6)){
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
				HitTime(10, 22, 28, 18);
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
				if ((P1.CFlg) && (P1.time >= 1)){
					// [必殺技・ゲージ技]
					SCancel();
				}

			}
			break;
			//********************
			// 225：2Ｃ
			//********************
		case 225:
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
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [ダメージ]
				Damage(100, 0);
				// [ゲージ] 
				Power(120);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 22, 28, 18);
				// [ノックバック]
				HitVel(-3.8, 0, -1.8, -4.0);
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
				if ((P1.CFlg) && (P1.time >= 1)){
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
				HitTime(6, 12, 20, 10);

				// [ノックバック]
				HitVel(-3.0, 0, -1.6, -4.8);
				P1.GuardF = 1;

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
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(75, 0);

				// [ゲージ] 
				Power(86);

				HitTime(8, 18, 22, 14);

				// [ノックバック]
				HitVel(-3, 0, -1.8, -4.6);
				P1.GuardF = 3;
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

			// 位置をずらす
			if (P1.time == 0){
				PosAdd(5, 0);
			}

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
				HitTime(10, 32, 38, 18);
				// [ノックバック]
				HitVel(-1.8, -4.0, -1.8, -4.0);
				// [ガード属性]
				P1.GuardF = 1;


				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// 下段喰らい


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
				HitTime(6, 12, 18, 10);
				// [ノックバック]
				HitVel(-4.2, 0, -1.4, -4.0);
				P1.GuardF = 1;
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
					if (P1.Senkou[2] > 0){		// 先行入力効かせてみる
						P1.stateno = 410, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
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
				HitTime(8, 16, 20, 14);
				// [ノックバック]
				HitVel(-4.6, 0, -1.8, -4.0);
				P1.GuardF = 2;
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
						P1.AirJump -= 1;
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
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(2);

			if (P1.time == 0)VelSet(0, -0.8);
			if (P1.time == 10)VelSet(0, 10);

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 5フレ
			if (P1.time >= 1){

				// [ダメージ]
				Damage(105, 0);

				// [ゲージ] 
				Power(110);

				// [ヒットストップ・のけぞり時間]
				HitTime(8, 28, 42, 8);

				// [ノックバック]
				HitVel(-2.8, -4.2, -3.2, 6.4);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1000;
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

			if (P1.YPos + P1.YVel * 2 >= GROUND){
				P1.stateno = 421, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;
			//********************
			// 431：ジャンプD着地
			//********************
		case 421:
			P1.SFlg = 0, P1.A.damage = 0, P1.ctrl = 0;
			P1.YPos = GROUND;
			P1.ignoreG = false;
			P1.D.counter = 0;
			VelSet(0, 0);
			// SEを鳴らす
			if (P1.time == 1)SEStart(6);
			// 最初にロック！
			if (P1.time == 0)P1.Lock = 1;
			// 前ジャンプ の 全体フレーム
			if (P1.time >= ANIMELEM + 4)P1.More = 1, P1.stateno = 0,
				P1.time = 0, P1.Lock = 0;
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
			// ヒット数セット、
			// ガード時は無効
			if ((P1.time == 5) && (P2.stateno <= 999) && (P1.XPos - P2.XPos < 170 && P2.XPos - P1.XPos < 170))	// 喰らい・空中状態じゃない
			{
				P1.MoveHit = 1;	// １回
				// どっちも投げタイミングが同じなら、2P負ける
				//if ((P1.stateno == P2.stateno) && (P1.time == P2.time) && (P1.PSide == 2))
				//	P1.MoveHit = 0;
			}

			// ダメージセット、持続 1フレ
			if (P1.time == 5){

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


			// 距離を離す
			if (AnimElem(4))P1.XVel = 4.2;
			if (AnimElem(4))P1.XVel = 3.2;
			// ヒット音
			if (P1.time == 7)P1.HitSE = 11;

			// 2P無敵
			if (P1.time < 7)P2.Muteki = 1, P2.mutekiF = 0;


			// ヒット数セット
			if (P1.time == 8)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 1フレ
			if (P1.time == 8){

				// [攻撃判定セット]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [ダメージ]
				Damage(0, 70);
				// [ゲージ] 
				Power(100, 150);
				HitTime(6, 55, 55, 0);
				// [ノックバック]
				HitVel(0, -9.4, 0, 0);
				P1.HitAnim = 1030;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitSE = 12;
				P1.A.hosei_K = 0.3;
			}
			else {
				DamReset();
			}

			// 投げ抜け
			if ((P2.Senkou[1] > 0 && P2.Senkou[2] > 0)
				&& (P1.time < TR_TIME) && (P2.rejectF)){
				P1.stateno = 55, P2.stateno = 55;
				P1.time = 0, P2.time = 0;

				if (P1.throwSide == 0){
					P1.muki = 0, P2.muki = 1;
					P2.XPos = P1.XPos + 40;
				}
				else{
					P1.muki = 1, P2.muki = 0;
					P2.XPos = P1.XPos - 40;
				}
				P1.YPos = GROUND, P2.YPos = GROUND;
				P1.XVel = -8, P2.XVel = -8;
				P1.YVel = 0, P2.YVel = 0;
				P1.More = 1, P2.More = 1;

				SEStart(29);
				EffStart(12, P2.XPos, (int)P2.YPos, 0, -(P2.ySize / 1.5),
					0.4, 0.4, false);
			}

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
			if ((P1.time == 5))	// 投げのタイミングが被ってない
			{
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット、持続 1フレ
			if (P1.time == 5){

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
				Damage(140, 10);
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
			if ((P2.Senkou[1] > 0 && P2.Senkou[2] > 0)
				&& (P1.time < TR_TIME) && (P2.rejectF)){
				P1.stateno = 56, P2.stateno = 56;
				P1.time = 0, P2.time = 0;

				if (P1.throwSide == 0){
					P1.muki = 0, P2.muki = 1;
					P2.XPos = P1.XPos + 40;
				}
				else{
					P1.muki = 1, P2.muki = 0;
					P2.XPos = P1.XPos - 40;
				}
				P1.XVel = -2, P2.XVel = -2;
				P1.YVel = -3, P2.YVel = -3;
				P1.ignoreG = false, P2.ignoreG = false;
				P1.D.yaccel = 0.35, P2.D.yaccel = 0.35;
				P1.fall = 0, P2.fall = 0;

				P2.YPos = P1.YPos;
				P1.More = 1, P2.More = 1;

				SEStart(29);
				EffStart(12, P2.XPos, (int)P2.YPos, 0, -(P2.ySize / 1.5),
					0.4, 0.4, false);
			}

			// 全体フレームを超えたらリセット
			if (P1.YPos + P1.YVel * 2 >= GROUND){
				P1.time = 0, P1.stateno = 47,
					P1.YPos = GROUND, P1.YVel = 0,
					P1.ctrl = 0, P1.SFlg = 0;
			}
			break;
			//********************
			// 520：ガーキャン
			//********************
		case 520:	//立ちB
			P1.ctrl = 0, P1.SFlg = 0;
			// SE
			if (P1.StopTime == 0){
				if (P1.time == 0){
					SEStart(30);

					P1.Power -= 1000;
					VelSet(0, 0);

					S.StopTime = 10;
					S.Anten = 10;
					P2.CFlg = 0;

					P1.colorCTime = 10;
					P1.colorC[0] = 100, P1.colorC[1] = 100, P1.colorC[2] = 255;
				}
			}

			// 攻撃出るまで無敵
			if (P1.D.counter == 1){
				P1.Muteki = 3;
				P1.mutekiF = 0;
			}

			// ヒット数セット、８フレ
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 1)VelSet(2.8, 0);
			// 位置をずらす
			if (P1.time == 3){
				PosAdd(5, 0);
			}

			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [ダメージ]
				Damage(0, 0);

				// [ゲージ] 
				P1.GetPow = 0, P1.GivePow = 0;
				HitTime(10, 20, 20, 18);

				// [ノックバック]
				HitVel(-2.1, -2.6, -1.7, -3);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 2;
				P1.HitAnim = 1000;	// 上段喰らい
				P1.HitSE = 11;
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 600 凪払い B
			//********************
		case 600:
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 1){
				SEStart(21);
			}
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if (P1.time >= 1){

				// [攻撃判定セット]
				AttSet(0, 120, 100, 69, -100);

				// [ダメージ]
				Damage(105, 0);

				// [ゲージ] 
				Power(120);
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
			// スウィフト2
			//********************
		case 601:
			P1.ctrl = 0, P1.SFlg = 2;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 1.8;
				P1.YVel = -4.5;
				P1.ignoreG = true;
			}

			P1.YVel += 0.3;

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}


			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [ダメージ]
				Damage(105, 0);
				// [ゲージ] 
				Power(120);
				HitTime(10, 32, 38, 18);
				// [ノックバック]
				HitVel(-3.4, -2.5, -3.5, -2.5);
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



			// 着地
			if ((P1.time > 0) && (P1.YVel > 0) && (P1.YPos + P1.YVel * 2 >= GROUND)){
				P1.time = 0, P1.stateno = 621, P1.More = 1,
					P1.ctrl = 0, P1.ignoreG = false;
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
			if ((P1.time >= 1) && (P1.time <= 20)){

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
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;

			// SEを鳴らす
			if (P1.time == 0){
				SEStart(21);
				VelSet(0, 0);
			}
			if (P1.time == 1){
				VelSet(0, 0);
			}

			if (AnimElem(3)){
				VelSet(1, -6);
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
				Damage(95, 10);
				// [ゲージ] 
				Power(126);

				HitTime(8, 100, 100, 12);
				// [ノックバック]
				HitVel(-1.7, -5.4, -1.7, -4.8);

				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;

				P1.HitSE = 13;
			}

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1)){ //&& (P2.stateno >= 1000)
				}
			}

			// 着地
			if (P1.YPos > GROUND){
				P1.time = 0, P1.stateno = 621, P1.ctrl = 0;
			}
			break;

			//********************
			// 621 着地
			//********************
		case 621:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.ignoreG = false;
			VelSet(0, 0);
			// SEを鳴らす
			if (P1.time == 1){
				SEStart(6);
				P1.YPos = GROUND;
			}

			// 終了
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
			// 800 ハードパンチ
			//********************
		case 800:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			P1.touchF = false;

			if (P1.time == 12)VelAdd(180, 0);
			

			if (P1.StopTime == 0){
				// ゲージ消費
				if (P1.time == 0){
					P1.Power -= 3000;
					S.StopTime = 10;
					S.Anten = 25;
					SEStart(25);
					// 中段エフェクト
					EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
				}
			}

			// SEを鳴らす、移動
			if (P1.time == 1){
				SEStart(21);
				// カラー変更
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
				Damage(250, 350);
				// [ゲージ] 
				Power(0, 600);
				HitTime(8, 600, 600, 24);
				// [ノックバック]
				HitVel(0, -5.8, 0, -5.4);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 14;
				P1.A.yaccel = 0.11;
				// エフェクト
				HitEff(1, 1, 1);
			}
			else {
				DamReset();
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				P1.touchF = true;
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
void GetP_EriParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_EriParam(System_t GS)
{
	S = GS;
}

void GetH_EriParam(Helper GH1[], Helper GH2[])
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
	// 214 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0))
		&& (P1.cmd[2])){		// 先行入力効かせてみる
		P1.stateno = 600, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}

	HCancel();
	//最後に判定を消す
	DamReset();
}

// 必殺技キャンセル
void HCancel()
{
	//  [ ３ゲージ ]
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.Senkou[3] > 0)
		&& (P1.Power == 3000)){		// 地上
		P1.stateno = 800;	// 超必
		P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	//最後に判定を消す
	DamReset();

}

// 空中必キャンセル
void ACancel()
{

}