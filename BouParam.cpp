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
void BouParam(void)
{


	//ワンモアを切る
	if (P1.More)P1.More = 0;

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
	P1.Var[11] = P1.D.armor;	// アーマー数表示
	if (P1.Var[13] > 0){
		// カラー変更
		if (P1.colorCTime <= 0){
			P1.colorC[0] = 255, P1.colorC[1] = 124, P1.colorC[2] = 124;
			P1.colorCTime = 1;
		}
		P1.Var[13]--;
		P1.Power = 0;
		P1.A.gaugeHosei = 1;
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
		if (
			((P1.stateno >= 5) && (P1.stateno <= 10)) ||	// 振り向き、しゃがみ移行
			(P1.stateno == 40) ||
			((P1.stateno >= 46) && (P1.stateno <= 47)) ||	// ジャンプ
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

		// チェーン初期化、コマ投げ0.5フラグ
		if (P1.ctrl){
			P1.Var[1] = 0;
			P1.Var[2] = 0;
		}
		// 連打スイッチ
		//if (P1.ctrl)P1.Var[20] = 0;

		if((P1.stateno != 610) && (P1.stateno != 611))P1.Var[15] = 0;
		// アーマーキャンセル
		if (P1.stateno == 225 || P1.stateno == 230 || P1.stateno == 320);
		else{ P1.Var[14] = 0; }
		

		//
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
			if (P1.Var[13] > 0)P1.XVel += 0.4;
			// SEを鳴らす
			if ((P1.time == 10) || (P1.time == 35))SEStart(7);
			if (P1.time > ANIMELEM)P1.time = 0;

			break;
		case 21:	// 後ろ歩き
			P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
			P1.XVel = P1.C.walkB; //速度を足す
			if (P1.Var[13] > 0)P1.XVel -= 0.4;
			// SEを鳴らす
			if (P1.time == 20)SEStart(7);
			if (P1.time > ANIMELEM)P1.time = 0;

			break;
		case 30:	// ステップ
			P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
			P1.ignoreG = true;

			// ゲージ増加
			if (P1.time == 0)P1.Power += 25;

			// SEを鳴らす
			if (P1.time == 1)SEStart(5);

			VelAdd(0, P1.C.yAccel);

			//速度を足す
			if (P1.time == 0){
				VelSet(P1.C.runF[0], P1.C.runF[1]);
				if (P1.Var[13] > 0)P1.XVel += 0.6;
			}
			

			// 終了
			if ((P1.YPos + P1.YVel * 2 >= GROUND) && (P1.YVel > 0)){
				/*
				P1.stateno = 31;
				P1.time = 0;
				P1.More = 1;
				P1.SFlg = 0;
				P1.ctrl = 0;
				*/
				P1.stateno = 0;
				P1.time = 0;
				P1.More = 1;
				P1.SFlg = 0;
				P1.ctrl = 1;
				P1.ignoreG = false, VelSet(0,0);
				P1.YPos = GROUND;
				SEStart(6);
			}

			break;
		case 31:	// 着地硬直
			P1.SFlg = 0, P1.ctrl = 0;

			// SEを鳴らす
			if (P1.time == 0)SEStart(6);
			VelSet(0, 0);
			P1.YPos = GROUND;

			// 終了
			if (P1.time >= ANIMELEM){
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
				P1.More = 1;
			}

			break;
		case 35:	// バックステップ
			P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
			P1.ignoreG = true;

			// 無敵
			if (P1.time <= 9){
				P1.Muteki = 1, P1.mutekiF = 0;
			}

			// SEを鳴らす
			if (P1.time == 1)SEStart(5);

			VelAdd(0, P1.C.yAccel);

			//速度を足す
			if (P1.time == 0){
				VelSet(P1.C.runB[0], P1.C.runB[1]);
				if (P1.Var[13] > 0)P1.XVel -= 0.5;
			}

			// 終了
			if ((P1.YPos + P1.YVel * 2 >= GROUND) && (P1.YVel > 0)){
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

		//
		case 41:	// 垂直
			P1.SFlg = 2, P1.ctrl = 1;
			P1.Muteki = 0;
			P1.XVel = 0;	// 垂直は左右に飛ばない
			// タイムが 0 のとき、↑に飛ぶ
			if (P1.time == 0){
				// 二段ジャンプ時、ジャンプ力を下げる
				{
					//P1.YPos -= 2;
				}
			}
			// 落下
			if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
			// 地面にいて時間が 0 じゃない時
			if ((P1.YPos > GROUND) && (P1.time != 0)){
				P1.stateno = 47, P1.More = 1, P1.time = 0;
			}
			break;
		case 42:	// 前ジャンプ
			P1.SFlg = 2, P1.ctrl = 1;
			P1.Muteki = 0;
			
			P1.XVel = P1.C.jumpF[0]; //速度を足す;
			if (P1.AirJump != P1.C.airJump)P1.XVel = P1.C.jumpF[1];//P1.C.jumpF[0] * 1.2;

			if (P1.JumpVel == 2)P1.XVel = P1.C.jumpF[1];// += (P1.XVel / 4) + 0.2;	// ダッシュ時
			if (P1.hyJump)P1.XVel = P1.C.jumpF[1];//P1.C.jumpF*1.5; //速度を足す;
			// タイムが 0 のとき、↑に飛ぶ
			if (P1.time == 0){
				// 二段ジャンプ時、ジャンプ力を下げる
				{
					//P1.YPos -= 2;
				}
			}
			// 落下
			if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
			// 地面にいて時間が 0 じゃない時
			if ((P1.YPos > GROUND) && (P1.time != 0)){
				P1.stateno = 47, P1.More = 1, P1.time = 0;
			}
			break;
		case 43:	// 後ろジャンプ
			P1.SFlg = 2, P1.ctrl = 1;
			P1.Muteki = 0;
			
			P1.XVel = P1.C.jumpB; //速度を足す;
			if (P1.AirJump != P1.C.airJump)P1.XVel = P1.C.jumpB * 1.2;

			if (P1.hyJump)P1.XVel = P1.C.jumpB*1.4; //速度を足す;
			// タイムが 0 のとき、↑に飛ぶ
			if (P1.time == 0){
				// 二段ジャンプ時、ジャンプ力を下げる
				{
					//P1.YPos -= 2;
				}
			}
			// 落下
			if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
			// 地面にいて時間が 0 じゃない時
			if ((P1.YPos > GROUND) && (P1.time != 0)){
				P1.stateno = 47, P1.More = 1, P1.time = 0;
			}
			break;

		case 45:	// 空中ジャンプ
			P1.ctrl = 0, P1.SFlg = 2;
			P1.XVel = 0, P1.YVel = -0.2;


			if (P1.time == 0){
				// 左入力
				if (P1.keyAtt[4]){
					P1.JumpVel = -1;
				}
				// 右入力
				else if (P1.keyAtt[6]){
					P1.JumpVel = 1;
				}
				else{ P1.JumpVel = 0; }

				P1.AirJump--, P1.airDash--;
				P1.hyJump = false;

				// 先行入力を消す
				P1.K_Senkou[8] = 0;
			}

			// 振り向き
			if (P1.time == 0){
				if (P1.XPos < P2.XPos && P1.muki == 1){
					P1.muki = 0;
					P1.JumpVel = -P1.JumpVel;
				}
				else if (P2.XPos < P1.XPos && P1.muki == 0){
					P1.muki = 1;
					P1.JumpVel = -P1.JumpVel;
				}
			}

			// ステート変更、2フレを超えた時ジャンプへ
			if (P1.time > 1){
				//エフェクト
				EffStart(16, (int)P1.XPos, (int)P1.YPos - 20, 0, 0,
					0.24, 0.04, P1.muki);
				// ジャンプフラグをオン
				P1.JumpFlg = 1;
				// ワンモアで次のステートに
				P1.More = 1, P1.time = 0;
				switch (P1.JumpVel){
				case 0:
					P1.stateno = 41;
					break;
				case 1:
					P1.stateno = 42;
					break;
				case -1:
					P1.stateno = 43;
					break;
				}
				P1.YVel = P1.C.jumpA + 0.8;
			}
			break;	// ジャンプ準備終了

		//
		case 90:	// 開幕
			P1.SFlg = 0, P1.ctrl = 0;

			if (P1.time == 15 - 1)SEStart(9);

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
			
			// 連打回数カウント
			if (P1.time == 0){
				P1.Var[2]++;
			}

			// 立ち の 全体フレーム、54を超えたらリセット
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

			// ヒット数セット、発生６フレ
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [ダメージ]
				Damage(48, 0);

				// [ゲージ] 
				Power(32);
				// [ヒットストップ・のけぞり時間]
				HitTime(5, 14, 18, 12);
				// [ノックバック]
				HitVel(-3.4, 0, -1.6, -4.8);
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
					// 6B
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
				// 先行入力
				else if ((P1.Senkou[1] > 0) && (P1.Var[2] < 3)){
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
			if ((P1.button[1] == 1) && (P1.time >= ANIMELEM - 2) && (P1.stateno == 200)){
				// 下要素が入ってるやん！
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
			// ヒット数セット 
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			// SE
			if (P1.time == 1)SEStart(1);

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 2, 1);
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [ダメージ]
				Damage(100, 0);

				// [ゲージ] 
				Power(120);
				HitTime(8, 18, 22, 16);

				// [ノックバック]
				HitVel(-2.6, 0, -1.7, -4.4);
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
					if ((P1.K_Senkou[8]) && (P2.HFlg == 1)) {		// 先行入力効かせてみる
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [通常技]
					if (P1.Senkou[3] > 0){
						// 6B
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
					// [必殺技・ゲージ技]
					SCancel();
				}
			}

			break;
			//********************
			// 215：6B
			//********************
		case 215:
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

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 3, 0);
			}

			// ヒット数セット 発生：26
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			//else{ P1.MoveHit = 0; }

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(90, 20);
				// [ゲージ] 
				Power(155);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 22, 24, 20);	// 18
				// [ノックバック]
				HitVel(-3.8, 0, -2.8, -4);
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
			// 220：立ちC
			//********************
		case 220:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SEを鳴らす
			if (P1.time == 1)SEStart(3);

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 3, 0);
			}

			// アーマーカウント
			if (P1.time == 0)P1.Var[12] = P1.D.armor;
			
			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(110, 10);
				// [ゲージ] 
				Power(150);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 25, 25, 23);
				// [ノックバック]
				HitVel(-4.2, 0, -2.8, -2.8);
				GuardVel(-6.2, -4.0);
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
					// [必殺技・ゲージ技]
					SCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

			break;
			
			
			//********************
			// 225：6C
			//********************
		case 225:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if (P1.time == 0)VelSet(0,0);
			// 位置をずらす
			if (P1.time == 0){
				PosAdd(6, 0);
				P1.Var[14] = 0;
			}
			// SEを鳴らす
			if (P1.time == 1)SEStart(3);

			// VO
			if ((P1.time == 0) && (P1.StopTime == 0)){
				PVOStart(P1.PSide, 25, 0);
			}

			// アーマーカウント
			if (P1.time == 0)P1.Var[12] = P1.D.armor;

			// アーマー
			if (P1.time == 0){
				P1.D.armor += 3;	// アーマー
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [ダメージ]
				Damage(270, 60);
				// [ゲージ] 
				Power(460);
				// [ヒットストップ・のけぞり時間]
				HitTime(12, 60, 60, 18);
				P1.A.A_GuardTime = 16;
				// [ノックバック]
				HitVel(-5.6, -4.2, -5.6, -4);
				GuardVel(-3.8, -2.6);
				// [ガード属性]
				P1.GuardF = 1;

				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 1.2;
				P1.A.wall_yvel = -6.8;
				P1.HitAnim = 1030;	// 下段喰らい

				P1.HitSE = 14;
				// エフェクト
				HitEff(1, 1, 1);
			}
			else {
				DamReset();
			}

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((!P1.CFlg) && (P1.time == 18)){
					// [通常技]
					if (P1.button[3] >= 18){
						// タメC
						{
							P1.stateno = 230, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
				}
				if ((P1.CFlg) && (P1.time >= 1)){
					// Ｄ派生技
					if ((P1.Senkou[4] > 0) && (P1.stateno != 600)// && (P1.Var[14] == 0)
						&& (P1.SFlg != 2) && (P1.D.armor == 0 || (P1.stateno == 225 || P1.stateno == 320))){		// 先行入力効かせてみる
						if (P1.stateno == 225){
							P1.D.armor -= 3;
							if (P1.D.armor <= 0)P1.D.armor = 0;
							P1.Var[14] = 1;
						}
						else if (P1.stateno == 320){
							P1.D.armor -= 2;
							if (P1.D.armor <= 0)P1.D.armor = 0;
							P1.Var[14] = 1;
						}

						// もう一度チェック
						//if (P1.D.armor == 0)
						{
							P1.stateno = 600, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
				}
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [必殺技・ゲージ技]
					HCancel();
					// [必殺技]
					if (P2.stateno >= 1000){
						SCancel();
					}
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 226, P1.ctrl = 0, P1.More = 1;
			}

			if ((P1.stateno != 225) && (P1.D.armor > 0)){
				if ((P1.Var[14] == 0) && (P1.stateno != 230)){
					P1.D.armor-=3;	// アーマーはがす;
					if (P1.Var[12] < P1.D.armor)P1.D.armor--;
				}
			}

			break;
			//********************
			// 226：硬直
			//********************
		case 226:
			P1.SFlg = 0, P1.ctrl = 0;
			P1.D.counter = 0;
			VelSet(0, 0);
			P1.YPos = GROUND;

			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P2.stateno >= 1000)){
					// [必殺技・ゲージ技]
					SCancel();
				}
			}

			// 終了
			if (P1.time >= ANIMELEM){
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
			}

			break;
			//********************
			// 230：H6C
			//********************
		case 230:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if (P1.time == 0)VelSet(0, 0);
			// 位置をずらす
			if (P1.time == 0){
				P1.Var[14] = 0;
			}

			// アーマーカウント
			if (P1.time == 0){
				P1.Var[12] = P1.D.armor;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 125, P1.colorC[2] = 125;
			}
			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [ダメージ]
				Damage(280, 70);
				// [ゲージ] 
				Power(460);
				// [ヒットストップ・のけぞり時間]
				HitTime(18, 300, 300, 20);
				P1.A.A_GuardTime = 18;
				// [ノックバック]
				HitVel(-7.0, -4.8, -7.0, -4.6);
				GuardVel(-4.0, -3.0);
				// [ガード属性]
				P1.GuardF = 1;

				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;

				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 1.2;
				P1.A.wall_yvel = -7.2;
				P1.A.yaccel = 0.30;
				P1.HitAnim = 1030;	// 下段喰らい

				P1.HitSE = 14;
				// エフェクト
				HitEff(1, 1, 1);
			}
			else {
				DamReset();
			}

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1)){
					// Ｄ派生技
					if ((P1.Senkou[4] > 0) && (P1.stateno != 600)// && (P1.Var[14] == 0)
						&& (P1.SFlg != 2) && (P1.D.armor == 0 || (P1.stateno == 225 || P1.stateno == 320))){		// 先行入力効かせてみる
						if (P1.stateno == 225){
							P1.D.armor -= 2;
							if (P1.D.armor <= 0)P1.D.armor = 0;
							P1.Var[14] = 1;
						}
						else if (P1.stateno == 320){
							P1.D.armor -= 1;
							if (P1.D.armor <= 0)P1.D.armor = 0;
							P1.Var[14] = 1;
						}

						// もう一度チェック
						//if (P1.D.armor == 0)
						{
							P1.stateno = 600, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
				}
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [必殺技・ゲージ技]
					HCancel();
					// [必殺技]
					if (P2.stateno >= 1000){
						SCancel();
					}
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 231, P1.ctrl = 0, P1.More = 1;
			}

			if ((P1.stateno != 230) && (P1.D.armor > 0)){
				if (P1.Var[14] == 0){
					P1.D.armor -= 3;	// アーマーはがす;
				}
			}

			break;
			//********************
			// 231：硬直
			//********************
		case 231:
			P1.SFlg = 0, P1.ctrl = 0;
			P1.D.counter = 1;
			VelSet(0, 0);
			P1.YPos = GROUND;

			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P2.stateno >= 1000)){
					// [必殺技・ゲージ技]
					SCancel();
				}
			}

			// 終了
			if (P1.time >= ANIMELEM){
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
			}

			break;

			//********************
			// 235：グランド
			//********************
		case 235:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if (P1.time == 0){
				VelSet(0, 0);
				PosAdd(20, 0);
			}
			// SEを鳴らす
			if (P1.time == 1)SEStart(3);

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 6, 0);
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// エフェクト
			if (P1.time == 16 - 1){
				SEStart(9);
				if (P1.muki == 0){
					EffStartB(18, (int)P1.XPos + 140, (int)P1.YPos, 0, -7,
						0.55, 0.06, P1.muki);
				}
				else {
					EffStartB(18, (int)P1.XPos - 140, (int)P1.YPos, 0, -7,
						0.55, 0.06, P1.muki);
				}
			}

			if (P1.time == 38){
				SEStart(9);
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(120, 0);
				// [ゲージ] 
				Power(180);
				// [ヒットストップ・のけぞり時間]
				HitTime(0, 18, 22, 16);
				// [ノックバック]
				HitVel(-2.0, -2.8, -2.4, -2.6);
				// [ガード属性]
				P1.GuardF = 3;

				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.A.boundLevel = 0;
				P1.HitAnim = 1020;	// 下段喰らい

				P1.HitSE = 12;
			}
			else {
				DamReset();
			}

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [必殺技・ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

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

			// 連打回数カウント
			if (P1.time == 0){
				P1.Var[2]++;
			}

			// SEを鳴らす
			if (P1.time == 1)SEStart(1);
			//  全体フレーム、14を超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 1, 2);
			}

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [ダメージ]
				Damage(47, 0);

				// [ゲージ] 
				Power(32);

				// [ヒットストップ・のけぞり時間]
				HitTime(5, 14, 18, 12);

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
			if ((P1.CFlg) && (P1.time >= 1)){
				
				if (P1.Senkou[3] > 0){
					// 6B
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
				else if ((P1.Senkou[1] > 0) && (P1.Var[2] < 3)){		// 先行入力効かせてみる
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
			if ((P1.button[1] == 1) && (P1.time >= ANIMELEM - 2) && (P1.stateno == 300)){
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
					P1.XPos += 5;
				}
				else{
					P1.XPos -= 5;
				}
			}
			// 位置をずらす
			if (P1.time == 9){
				if (P1.muki == 0){
					P1.XPos -= 5;
				}
				else{
					P1.XPos += 5;
				}
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 2, 1);
			}

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [ダメージ]
				Damage(100, 0);

				// [ゲージ] 
				Power(120);


				HitTime(8, 40, 40, 14);
				// [ノックバック]
				HitVel(-2.0, -3.8, -2.4, -3.6);
				GuardVel(-3.2, -2.4*1.5);
				P1.GuardF = 3;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// 空中喰らい(確定ダウン)
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
						// 6B
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
						// 6B
						if ((key(6)) && (!key(2)) && (!key(8))){
							P1.stateno = 215, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
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
				P1.Var[14] = 0;
				PosAdd(5, 0);
			}

			// SEを鳴らす
			if (P1.time == 1)SEStart(3);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 0, P1.More = 1, P1.stateno = 325, P1.time = 0;

			// VO
			if ((P1.time == 1) && (P1.StopTime == 0)){
				PVOStart(P1.PSide, 4, 0);
			}

			if (P1.time == 22)P1.XVel = 17.4;

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;
			}
			if (P1.time == 24){
				P1.MoveHit = 1;
			}

			// アーマーカウント
			if (P1.time == 0)P1.Var[12] = P1.D.armor;

			// アーマーセット
			if (P1.time == 3){
				P1.D.armor+=2;	// アーマー
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(108, 0);
				// [ゲージ] 
				Power(110);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 40, 40, 18);
				// [ノックバック]
				HitVel(-3.8, 0, -1.8, -3.4);
				if (P1.time >= 24){
					HitVel(-2.8, -4.0, -1.8, -3.4);
					// 高度を下げる
					if (P1.HitCount > 9)
						HitVel(-2.8, -4.0, -1.8, -2.8);
				}
				GuardVel(-6.4, -3);
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
				if ((P1.CFlg) && (P1.time >= 1)){
					// Ｄ派生技
					if ((P1.Senkou[4] > 0) && (P1.stateno != 600)// && (P1.Var[14] == 0)
						&& (P1.SFlg != 2) && (P1.D.armor == 0 || (P1.stateno == 225 || P1.stateno == 320))){		// 先行入力効かせてみる
						if (P1.stateno == 225){
							P1.D.armor -= 2;
							if (P1.D.armor <= 0)P1.D.armor = 0;
							P1.Var[14] = 1;
						}
						else if (P1.stateno == 320){
							P1.D.armor -= 2;
							if (P1.D.armor <= 0)P1.D.armor = 0;
							P1.Var[14] = 1;
						}

						// もう一度チェック
						//if (P1.D.armor == 0)
						{
							P1.stateno = 600, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
				}
				// キャンセル
				
				if ((P1.CFlg) && (P1.time >= 1) && (P1.MoveHit == 0) && (P1.scTime > 0)){
					// [必殺技・ゲージ技]
					HCancel();
					// [必殺技]
					if (P2.stateno >= 1000){
						SCancel();
					}
				}
			}

			if ((P1.stateno != 320) && (P1.D.armor > 0)){
				if (P1.Var[14] == 0)P1.D.armor-=2;	// アーマーはがす;
			}

			break;
			//********************
			// 325：硬直
			//********************
		case 325:
			P1.SFlg = 0, P1.ctrl = 0;
			P1.D.counter = 0;
			VelSet(0, 0);
			P1.YPos = GROUND;

			// 応急処置
			if (P1.StopTime == 0){
				// キャンセル
				if ((P2.stateno >= 1000)){
					// [必殺技・ゲージ技]
					SCancel();
				}
			}

			// 終了
			if (P1.time >= ANIMELEM){
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
			}

			break;
			//********************
			// 400：ジャンプA
			//********************
		case 400:
			P1.ctrl = 0, P1.SFlg = 2;
			if (P1.time == 1)SEStart(1);
			// 先行入力消す
			if (P1.time <= 1)P1.Senkou[1] = 0;

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 1, 2);
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(45, 0);
				// [ゲージ] 
				Power(26);

				// [ヒットストップ・のけぞり時間]
				HitTime(5, 12, 16, 10);
				// [ノックバック]
				HitVel(-4.2, 0, -1.4, -4.0);
				P1.GuardF = 1;
				P1.HitAnim = 1000;
				P1.HitSE = 10;
			}
			else {
				DamReset();
			}

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					if (P1.Senkou[3] > 0){		// 先行入力効かせてみる
						// 6C
						if (key(2)){
							P1.stateno = 430, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else {
							P1.stateno = 420, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
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
			}
			// 全体フレームを超えたらリセット
			/*
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
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [ダメージ]
				Damage(86, 10);

				// [ゲージ] 
				Power(124);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 16, 50, 14);
				// [ノックバック]
				HitVel(-4.6, 0, -2.6, 4.4);
				P1.GuardF = 2;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				P1.HitSE = 11;
				P1.A.boundLevel = 1;
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
						// 6B
						if (key(2)){
							P1.stateno = 430, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else {
							P1.stateno = 420, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
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
			P1.ignoreG = true;
			P1.YVel += P1.C.yAccel;
			// SE
			if (P1.time == 1)SEStart(2);

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 3, 0);
			}

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 5フレ
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [ダメージ]
				Damage(60, 90);

				// [ゲージ] 
				Power(170);

				// [ヒットストップ・のけぞり時間]
				HitTime(10, 60, 62, 24);

				// [ノックバック]
				HitVel(-1.8, 5.0, -2.4, 5.0);
				P1.GuardF = 2;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.A.boundLevel = 2;
				P1.A.bound_xvel = -1.0;
				P1.A.bound_yvel = -8.2;
				// バウンド
				if (P2.D.boundCount > 0){
					P1.A.boundLevel = 1;
					P1.A.bound_yvel = -5.6;
				}
				P1.HitAnim = 1030;
				P1.HitSE = 12;

			}
			else {
				DamReset();
			}

			if (P1.time == 18){
				//P1.XVel = P1.XVel / 2;
				
				if (P1.keyAtt[4] > 0){
					//P1.XVel = P1.XVel / 2;
					P1.XVel = -1.2;
					if (P1.XVel > 0)P1.XVel = -P1.XVel;
				}
				else if (P1.keyAtt[6] > 0){
					//P1.XVel = P1.XVel / 2;
					P1.XVel = 1.2;
					if (P1.XVel < 0)P1.XVel = -P1.XVel;
				}
				else{
					P1.XVel = 0;
				}
				P1.YVel = -2.6;
			}

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [空中必殺技]
					ACancel();
				}

			}

			if (P1.time >= ANIMELEM) P1.stateno = 425, P1.time = 0, P1.More = 1, P1.ctrl = 0;

			// 終了
			if (P1.YPos + P1.YVel >= GROUND){
				P1.stateno = 47;
				P1.time = 0;
				P1.ctrl = 1;
			}

			break;
			//********************
			// 421：硬直
			//********************
		case 421:
			P1.SFlg = 0, P1.ctrl = 0;
			P1.ignoreG = false;
			P1.D.counter = 0;

			// SEを鳴らす
			if (P1.time == 1)SEStart(6);

			VelSet(0, 0);
			P1.YPos = GROUND;

			// 終了
			if (P1.time >= ANIMELEM){
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
			}

			break;
			//********************
			// 425：JC2
			//********************
		case 425:
			P1.SFlg = 2, P1.ctrl = 0;
			P1.ignoreG = true;
			P1.D.counter = 0;
			P1.YVel += P1.C.yAccel;

			// 終了
			if ((P1.YPos + P1.YVel >= GROUND) && (P1.time >= 1)){
				//P1.stateno = 421;
				P1.stateno = 47;
				P1.time = 0;
				P1.ctrl = 1;
			}

			break;
			//********************
			// 430：ジャンプD
			//********************
		case 430:
			P1.ctrl = 0, P1.SFlg = 2;

			if (P1.time == 0){
				P1.ignoreG = false;
				afterimageSetting(P1.PSide, 15, 155, 155, 255);
			}
			else{ P1.ignoreG = true; }
			// SE
			if (P1.time == 1)SEStart(3);

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 3, 0);
			}

			P1.YVel += GRAVITY;
			if (P1.time == 0){
				P1.YVel = -3.8;
				if (P1.XVel > P1.C.jumpF[0])P1.XVel = P1.C.jumpF[0];
				else if (P1.XVel < P1.C.jumpB)P1.XVel = P1.C.jumpB;
			}
			if (P1.YVel >= 9.0)P1.YVel = 9.0;

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 5フレ
			if (P1.time >= 1){

				// [ダメージ]
				Damage(110, 0);
				// [ゲージ] 
				Power(130);

				// [ヒットストップ・のけぞり時間]
				HitTime(8, 60, 60, 16);

				// [ノックバック]
				HitVel(-2.2, -3.6, -2.4, 5.2);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				P1.HitSE = 12;
				// バウンド
				if (P1.HitCount == 0){
					P1.A.boundLevel = 1;
					P1.A.bound_xvel = -1.2;
					P1.A.bound_yvel = -3.2;
				}
				else{
					P1.A.boundLevel = 1;
					P1.A.bound_yvel = -2.8;
				}
				P1.A.hosei_K = 0.9;
			}
			else {
				DamReset();
			}

			// 着地
			if (P1.YPos + P1.YVel * 2 >= GROUND){
				P1.stateno = 421, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
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

			// VO
			if ((P1.time == 9) && (P1.StopTime == 0)){
				PVOStart(P1.PSide, 11, 0);
			}

			// SE
			if (P1.time == 1)SEStart(2);
			// ヒット数セット、
			// ガード時は無効
			if ((P1.time == 4) && (P2.stateno <= 999) && (P1.XPos - P2.XPos < 160 && P2.XPos - P1.XPos < 160))	// 喰らい・空中状態じゃない
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
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

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
			//if (AnimElem(8))P1.XVel = 4.2;
			//if (AnimElem(9))P1.XVel = 3.2;
			// ヒット音
			if (P1.time == 7)P1.HitSE = 11;

			// 2P無敵
			if (P1.time < 33)P2.Muteki = 1, P2.mutekiF = 0;


			// VO
			if ((P1.time == 30) && (P1.StopTime == 0)){
				PVOStart(P1.PSide, 10, 0);
			}

			// ヒット数セット
			if (P1.time == 34)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 1フレ
			if (P1.time == 34){

				// [攻撃判定セット]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [ダメージ]
				Damage(0, 180);
				// [ゲージ] 
				Power(300);
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
			if ((P1.time == 4))	// 投げのタイミングが被ってない
			{
				P1.MoveHit = 1;	// １回
				/*
				// どっちも投げタイミングが同じなら、2P負ける
				if ((P1.stateno == P2.stateno) && (P1.time == P2.time) && (P1.PSide == 2))
					P1.MoveHit = 0;
					*/
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
			if ((P1.time == 0) && (P1.YPos + 20 >= GROUND)){
				P1.YPos = GROUND - 20;
			}

			// 距離をとる
			if (P1.time == 0){
				P1.YPos -= 2;
				P2.YPos = P1.YPos - 26;
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
				P1.YVel = -2.2;
				P2.YVel = -2.2;
			}
			// 重力加算
			if (P1.time > 16){
				P1.YVel += GRAVITY + 0.05;
			}
			// 無敵終了
			else if (P1.time == 16){
				P1.YVel += GRAVITY;
				P2.YVel += GRAVITY;
				P1.YVel = -4;
			}
			// ついでに無敵時間
			else {
				P2.Muteki = 1, P2.mutekiF = 0;
				P2.time = 0;
				P1.YVel += GRAVITY/2;
				P2.YVel += GRAVITY/2;
			}
			// SEを鳴らす
			if (P1.time == 15)SEStart(10);

			// VO
			if ((P1.time == 9) && (P1.StopTime == 0)){
				PVOStart(P1.PSide, 10, 0);
			}

			// ヒット数セット
			if (P1.time == 16)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 1フレ
			if (P1.time == 17){

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
				HitVel(-4.8, 9, -4.8, 9);
				P1.HitAnim = 1030;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitSE = 11;


			}
			else{
				DamReset();
			}

			// 姿勢変更
			if (P1.time == 18)P2.time = 20;

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
			// 600 アサルト
			//********************
		case 600:	
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
			}

			if (P1.time == 13 - 1)SEStart(9);
			if (P1.time == 33 - 1)SEStart(9);

			// VO
			if (P1.time == ANIMELEM - 12){
				PVOStart(P1.PSide, 13, 0);
			}

			if (P1.D.armor == 0)P1.Var[16] = 0;
			else{ P1.Var[16] = 1; }

			// アーマー付与
			if (P1.time == ANIMELEM - 2){
				if (P1.Var[16] == 0){
					P1.D.armor = 1;
					EffStartB(16, P1.XPos, P1.YPos, -10, -(P1.ySize / 1.5),
						0.25, 0.25, P1.muki);
				}
				else {
					P1.Power += 100;
					EffStartB(16, P1.XPos, P1.YPos, -10, -(P1.ySize / 1.5),
						0.25, 0.25, P1.muki);
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// アサルト2
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
			// 610：コマ投げ
			//********************
		case 610:
			P1.ctrl = 0, P1.SFlg = 0;

			if (P1.time == 0)VelSet(0,0);

			// SE
			if (P1.time == 1)SEStart(2);
			// ヒット数セット、
			// ガード時は無効
			if ((P1.time == 9 - 2) && (P2.stateno <= 999) 
				&& ((P1.XPos - P2.XPos < 220) && (P2.XPos - P1.XPos < 220)))	// 喰らい・空中状態じゃない
			{
				P1.MoveHit = 1;	// １回
				// どっちも投げタイミングが同じなら、2P負ける
				//if ((P1.stateno == P2.stateno) && (P1.time == P2.time) && (P1.PSide == 2))
				//	P1.MoveHit = 0;
			}

			// 投げ無敵
			if (P1.Var[15] != 1){
				if ((P1.D.counter) || (P1.time == 0)){
					P1.mutekiF = 2;
					P1.Muteki = 1;
				}
			}
			// 0.5に移行
			if (P1.Var[15] == 1){
				if ((P1.D.counter) || (P1.time == 0)){
					P1.mutekiF = 1;
					P1.Muteki = 1;
				}
				// ゲージ消費
				if (P1.time == 1){
					P1.Power -= 500;
					SEStart(31);
					P1.colorCTime = 20;
					P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
				}
			}
			
			// ダメージセット、持続 1フレ
			if (P1.time == 5){

				// [ダメージ]
				Damage(0, 0);
				// [ゲージ] 
				Power(0, 0);
				HitTime(0, 3, 3, 0);

				P1.A.hitEff = 2;

				P1.GX = 0, P1.GY = 0;
				P1.GuardF = 0;
				P1.attF = 2;

				P1.HitAnim = 1060;
				P1.HitSE = 29;
			}

			// [ガード距離]
			P1.GuardDist = 0;


			// 当たったら変更
			if ((P1.CFlg) && (P1.time == 9)){
				P1.stateno = 611;
				P1.time = 0, P2.time = 0;
				P1.More = 1;
				P1.A.damage = 0;
				// 投げの向きを変更
				if (P1.muki == 0){
					P1.muki = 0, P2.muki = 1;
				}
				else if (P1.muki == 1){
					P1.muki = 1, P2.muki = 0;
				}


			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 611：投げ（演出）
			//********************
		case 611:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.Muteki = 2, P1.mutekiF = 0;
			P1.ignoreG = 1;
			// 距離をとる
			if (P1.time == 0){
				if (P1.muki == 0){
					P2.XPos = P1.XPos + 86;
				}
				else{
					P2.XPos = P1.XPos - 86;
				}
				P1.XVel = 0;
				P2.YVel = 0;
				P2.YPos = GROUND;
			}
			// もし距離が離れなかったら


			// 距離を離す
			if (P1.time == 26)VelSet(3.2, -4.8);
			if (P1.time > 26){
				P1.YVel += P1.C.yAccel;
				if (P1.XVel > 1)P1.XVel *= 0.9;
			}
			//if (AnimElem(9))P1.XVel = 3.2;
			// ヒット音
			if (P1.time == 7)P1.HitSE = 11;

			// 2P無敵
			if (P1.time < 25 - 1)P2.Muteki = 1, P2.mutekiF = 0;

			// VO
			if (P1.time == 21){
				PVOStart(P1.PSide, 6, 0);
			}

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 1フレ
			if (P1.time >= 1){

				// [ダメージ]
				Damage(0, 230);
				// [ゲージ] 
				Power(330);
				HitTime(10, 45, 45, 0);
				// [ノックバック]
				HitVel(-3.4, -5.4, 0, 0);

				if (P1.Var[15] == 1){
					Damage(0, 260);
					Power(0,190);
				}
				P1.HitAnim = 1030;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitSE = 14;
				HitEff(1, 1.0, 1.0);
				P1.A.boundLevel = 1;
			}
			else{ DamReset(); }

			// 全体フレームを超えたらリセット
			if ((P1.time > 0) && (P1.YVel > 0) && (P1.YPos + (P1.YVel * 2) > GROUND)){
				P1.time = 0, P1.stateno = 612, 
				P1.ctrl = 0, P1.More = 1;
			}
			break;
			//********************
			// 612：硬直
			//********************
		case 612:
			P1.SFlg = 0, P1.ctrl = 0;
			P1.Muteki = 1, P1.mutekiF = 0;
			P1.ignoreG = false;

			// SEを鳴らす
			if (P1.time == 1)SEStart(6);

			VelSet(0, 0);
			P1.YPos = GROUND;

			// 終了
			if (P1.time >= ANIMELEM){
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
			}

			break;
			
			//********************
			// 620：グランド
			//********************
		case 620:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if (P1.time == 0){
				VelSet(0, 0);
				PosAdd(20, 0);
			}
			// SEを鳴らす
			if (P1.time == 1)SEStart(3);

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 6, 0);
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(120, 0);
				// [ゲージ] 
				Power(180);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 24, 28, 22);
				// [ノックバック]
				HitVel(-4.4, 0, -2.2, 3.6);
				// [ガード属性]
				P1.GuardF = 3;


				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.A.boundLevel = 3;
				P1.HitAnim = 1020;	// 下段喰らい

				P1.HitSE = 12;
			}
			else {
				DamReset();
			}

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [必殺技・ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

			break;

			//********************
			// 625：グランド
			//********************
		case 625:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			// 慣性
			if (P1.time == 0){
				VelSet(0, 0);
				PosAdd(20, 0);
			}
			// SEを鳴らす
			if (P1.time == 1)SEStart(3);

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 6, 0);
			}

			// ゲージ消費
			if (P1.time == 1){
				P1.Power -= 500;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			// ヒット数セット
			if (P1.time == 1){
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 13 - 1){
				if (P1.muki == 0){
					EffStartB(18, (int)P1.XPos + 140, P1.YPos, 0, -7,
						0.25, 0.06, P1.muki);
				}
				else {
					EffStartB(18, (int)P1.XPos - 140, P1.YPos, 0, -7,
						0.25, 0.06, P1.muki);
				}
			}

			// ダメージセット
			if (P1.time >= 1){

				// [ダメージ]
				Damage(130, 10);
				// [ゲージ] 
				Power(0, 100);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 24, 28, 22);
				// [ノックバック]
				HitVel(-2.2, -3.4, -1.4, 3.4);
				GuardVel(-4.4, -2.4);
				// [ガード属性]
				P1.GuardF = 3;


				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// 下段喰らい
				P1.fall = 1;
				P1.A.boundLevel = 3;
				P1.HitSE = 12;
			}
			else {
				DamReset();
			}

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [必殺技・ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

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
			// 800 ハードパンチ
			//********************
		case 800:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;

			if (P1.time == 0)VelSet(0, 0);

			// 強化中
			if (P1.time == 0 && P1.Var[13] > 0){
				P1.time = 0, P1.stateno = 810, P1.ctrl = 1;
				P1.More = 1;
				break;
			}
			if (P1.StopTime == 0){
				// ゲージ消費
				if (P1.time == 0){
					P1.Power -= 1000;
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

			// VO
			if ((P1.time == 0) && (P1.StopTime == 0)){
				PVOStart(P1.PSide, 20, 0);
			}

			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [ダメージ]
				Damage(160, 90);
				// [ゲージ] 
				Power(0, 320);
				HitTime(10, 60, 60, 24);
				// [ノックバック]
				HitVel(-5.4, -4.8, -5.6, -4.4);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;

				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 14;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 0.6;
				P1.A.wall_yvel = -7.2;
				// エフェクト
				HitEff(1, 1, 1);
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
			// 810 ハードパンチ(強化版)
			//********************
		case 810:
			P1.ctrl = 0, P1.SFlg = 0;

			if (P1.time == 0)VelSet(0, 0);


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

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 20, 0);
			}

			// ダメージセット
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [ダメージ]
				Damage(150, 100);
				// [ゲージ] 
				Power(0, 320);
				HitTime(10, 60, 60, 24);
				// [ノックバック]
				HitVel(-9.4, -5.8, -9.6, -5.4);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;

				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 14;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 0.8;
				P1.A.wall_yvel = -7.2;
				// エフェクト
				HitEff(1, 1, 1);
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
			// 850 3ゲージ
			//********************
		case 850:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			// SEを鳴らす、移動
			if (P1.time == 0){
				P1.XVel = 0;
				P1.Power -= 3000;
				S.StopTime = 15;
				S.noGauge = 15;
				S.Anten = 25;
				SEStart(25);
				// 中段エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}

			// VO
			if ((P1.time == 0) && (P1.StopTime == 0)){
				PVOStart(P1.PSide, 12, 0);
			}

			if (P1.time == 13 - 1)SEStart(9);

			// アーマー付与
			if (P1.time == 16 - 1){
				EffStartB(16, P1.XPos, P1.YPos, -10, -(P1.ySize / 1.5),
					0.25, 0.25, P1.muki);
				P1.Var[13] = 599;	// 強化
				afterimageSetting(P1.PSide, P1.Var[13], 255, 0, 255);
				P1.D.armor = 3;
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
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
void GetP_BouParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_BouParam(System_t GS)
{
	S = GS;
}

void GetH_BouParam(Helper GH1[], Helper GH2[])
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
	if ((P1.Senkou[4] > 0) && (P1.stateno != 600) && (P1.Var[14] == 0)
		&& (P1.SFlg != 2)){ //&& (P1.D.armor == 0 || (P1.stateno == 225 || P1.stateno == 320))){		// 先行入力効かせてみる
		if (P1.stateno == 225){
			P1.D.armor -= 2;
			if (P1.D.armor <= 0)P1.D.armor = 0;
			P1.Var[14] = 1;
		}
		else if (P1.stateno == 320){
			P1.D.armor -= 1;
			if (P1.D.armor <= 0)P1.D.armor = 0;
			P1.Var[14] = 1;
		}
		// もう一度チェック
		//if (P1.D.armor == 0)
		{
			P1.stateno = 600, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	// 236 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0))
		&& (P1.cmd[1])){		// 先行入力効かせてみる
		P1.stateno = 620, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 236 + C
	if ((P1.Senkou[3] > 0) && (P1.cmd[1]) && (P1.Power >= 500)){		// 先行入力効かせてみる
		P1.stateno = 625, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	HCancel();

	//最後に判定を消す
	DamReset();
}

// 必殺技キャンセル
void HCancel()
{
	// [ ハードパンチ ]
	if ((P1.Senkou[1] > 0 && P1.Senkou[2] > 0 && P1.Senkou[3] > 0) && (P1.Power >= 1000 || P1.Var[13] > 0)){
		P1.stateno = 800, P1.More = 1;
		P1.time = 0;
	}
	// [ ３ゲージ ]
	if ((P1.Senkou[4] > 0) && (P1.cmd[3]) && (P1.Power >= 3000) && (P1.SFlg != 2) && (P1.Var[13] <= 0)){
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

}
