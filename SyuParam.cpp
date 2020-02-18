//========================================================================
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
void SyuParam(void)
{


	//ワンモアを切る
	if (P1.More)P1.More = 0;

	// ステートが切り替わったら
	if (P1.stateno != P1.OldState) {
		GetP_SameParam(P1, P2);
		GetH_SameParam(H1, H2);
		GetS_SameParam(S);
		ChangeState();
	}

	//=============
	// キャラ別事前設定
	//=============

	if (P1.Var[13] > 0) {
		// カラー変更
		if (P1.colorCTime <= 0) {
			P1.colorC[0] = 255, P1.colorC[1] = 124, P1.colorC[2] = 124;
			P1.colorCTime = 1;
		}
		P1.Var[13]--;
		P1.Power = 0;
		P1.A.gaugeHosei = 1;
	}
	// 1P
	if (P1.StopTime == 0 || P1.time == 0) {
		//設定開始
		//--------------------
		// 攻撃判定リセット
		//--------------------
		if (P1.stateno < 200 || P1.stateno > 999) {
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
			) {
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
		if ((P1.stateno >= 1000) && (P1.stateno <= 1999)) {	// 立ち喰らい
			GetP_SameParam(P1, P2);
			GetH_SameParam(H1, H2);
			GetS_SameParam(S);
			DamParam();
		}

		// チェーン初期化、コマ投げ0.5フラグ
		if (P1.ctrl) {
			P1.Var[2] = 0;
		}
		// 連打スイッチ
		//if (P1.ctrl)P1.Var[20] = 0;

		if ((P1.stateno != 610) && (P1.stateno != 611))P1.Var[15] = 0;
		// アーマーキャンセル
		if (P1.stateno == 225 || P1.stateno == 230 || P1.stateno == 320);
		else { P1.Var[14] = 0; }


		//
		switch (P1.stateno) {

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
			if (P1.time == 1) {
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
				P1.mutekiF == 0) {
				P1.mutekiF = 2;
				P1.Muteki = 10;
			}
			// 下入力解除で 12 へ
			if (InputPAD(102) == 0) {
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
			if (P1.time > ANIMELEM) {
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
		case 30:	// ステップ
			P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
			P1.ignoreG = true;

			// SEを鳴らす
			if (P1.time == 1)SEStart(5);

			VelAdd(0, P1.C.yAccel);

			//速度を足す
			if (P1.time == 0) {
				VelSet(P1.C.runF[0], P1.C.runF[1]);
			}


			// 終了
			if ((P1.YPos + P1.YVel * 2 >= GROUND) && (P1.YVel > 0)) {
				P1.stateno = 0;
				P1.time = 0;
				P1.More = 1;
				P1.SFlg = 0;
				P1.ctrl = 1;
				P1.ignoreG = false, VelSet(0, 0);
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
			if (P1.time >= ANIMELEM) {
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
			if (P1.time <= 9) {
				P1.Muteki = 1, P1.mutekiF = 0;
			}

			// SEを鳴らす
			if (P1.time == 1)SEStart(5);

			VelAdd(0, P1.C.yAccel);

			//速度を足す
			if (P1.time == 0) {
				VelSet(P1.C.runB[0], P1.C.runB[1]);
				if (P1.Var[13] > 0)P1.XVel -= 0.5;
			}

			// 終了
			if ((P1.YPos + P1.YVel * 2 >= GROUND) && (P1.YVel > 0)) {
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
			if (P1.time >= ANIMELEM + 1) {
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
			if (P1.time == 0) {
				// 二段ジャンプ時、ジャンプ力を下げる
				{
					//P1.YPos -= 2;
				}
			}
			// 落下
			if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
			// 地面にいて時間が 0 じゃない時
			if ((P1.YPos > GROUND) && (P1.time != 0)) {
				P1.stateno = 47, P1.More = 1, P1.time = 0;
			}
			break;
		case 42:	// 前ジャンプ
			P1.SFlg = 2, P1.ctrl = 1;
			P1.Muteki = 0;

			P1.XVel = P1.C.jumpF[0]; //速度を足す;

			if (P1.JumpVel == 2)P1.XVel = P1.C.jumpF[1];// += (P1.XVel / 4) + 0.2;	// ダッシュ時
			if (P1.hyJump)P1.XVel = P1.C.jumpF[1];//P1.C.jumpF*1.5; //速度を足す;
			// タイムが 0 のとき、↑に飛ぶ
			if (P1.time == 0) {
				// 二段ジャンプ時、ジャンプ力を下げる
				{
					//P1.YPos -= 2;
				}
			}
			// 落下
			if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
			// 地面にいて時間が 0 じゃない時
			if ((P1.YPos > GROUND) && (P1.time != 0)) {
				P1.stateno = 47, P1.More = 1, P1.time = 0;
			}
			break;
		case 43:	// 後ろジャンプ
			P1.SFlg = 2, P1.ctrl = 1;
			P1.Muteki = 0;

			P1.XVel = P1.C.jumpB; //速度を足す;

			if (P1.hyJump)P1.XVel = P1.C.jumpB*1.4; //速度を足す;
			// タイムが 0 のとき、↑に飛ぶ
			if (P1.time == 0) {
				// 二段ジャンプ時、ジャンプ力を下げる
				{
					//P1.YPos -= 2;
				}
			}
			// 落下
			if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
			// 地面にいて時間が 0 じゃない時
			if ((P1.YPos > GROUND) && (P1.time != 0)) {
				P1.stateno = 47, P1.More = 1, P1.time = 0;
			}
			break;

		case 45:	// 空中ジャンプ
			P1.ctrl = 0, P1.SFlg = 2;
			P1.XVel = 0, P1.YVel = -0.2;


			if (P1.time == 0) {
				// 左入力
				if (P1.keyAtt[4]) {
					P1.JumpVel = -1;
				}
				// 右入力
				else if (P1.keyAtt[6]) {
					P1.JumpVel = 1;
				}
				else { P1.JumpVel = 0; }

				P1.AirJump--, P1.airDash--;
				P1.hyJump = false;

				// 先行入力を消す
				P1.K_Senkou[8] = 0;
			}

			// 振り向き
			if (P1.time == 0) {
				if (P1.XPos < P2.XPos && P1.muki == 1) {
					P1.muki = 0;
					P1.JumpVel = -P1.JumpVel;
				}
				else if (P2.XPos < P1.XPos && P1.muki == 0) {
					P1.muki = 1;
					P1.JumpVel = -P1.JumpVel;
				}
			}

			// ステート変更、2フレを超えた時ジャンプへ
			if (P1.time > 1) {
				//エフェクト
				EffStart(16, (int)P1.XPos, (int)P1.YPos - 20, 0, 0,
					0.24, 0.04, P1.muki);
				// ジャンプフラグをオン
				P1.JumpFlg = 1;
				// ワンモアで次のステートに
				P1.More = 1, P1.time = 0;
				switch (P1.JumpVel) {
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
			if (P1.time >= P1.animElem[P1.stateno]) {
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
			if (P1.time == 0) {
				if ((P1.XPos < P2.XPos) && (P1.muki == 1)) {
					P1.muki = 0;
				}
				else if ((P1.XPos > P2.XPos) && (P1.muki == 0)) {
					P1.muki = 1;
				}
			}
			// 先行入力消す
			if (P1.time <= 1)P1.Senkou[1] = 0;

			// 連打回数カウント
			if (P1.time == 0) {
				P1.Var[2]++;
			}

			// 立ち の 全体フレーム、54を超えたらリセット
			if (P1.time >= ANIMELEM) {
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}
			// SEを鳴らす
			if (P1.time == 1) {
				SEStart(1);
			}

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 1, 2);
			}

			// ヒット数セット、発生６フレ
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)) {

				// [ダメージ]
				Damage(46, 0);

				// [ゲージ] 
				Power(31);
				// [ヒットストップ・のけぞり時間]
				HitTime(6, 12, 16, 10);
				// [ノックバック]
				HitVel(-3.6, 0, -1.6, -4.8);
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
			if ((P1.CFlg) && (P1.time >= 1)) {
				// [ジャンプキャンセル]
				if (P1.keyAtt[8]) {		// 先行入力効かせてみる
					P1.stateno = 40, P1.More = 1,
						P1.time = 0, P1.A.damage = 0;
				}
				// [通常技]
				if (P1.Senkou[3] > 0) {
					// 足払い
					if ((key(6)) && (key(2))) {
						P1.stateno = 325, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// 6C
					else if ((key(6)) && (!key(2)) && (!key(8))) {
						P1.stateno = 225, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)) {
						P1.stateno = 320, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.stateno = 220, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if (P1.Senkou[2] > 0) {		// 先行入力効かせてみる
					// 6B
					if ((key(6)) && (!key(2)) && (!key(8))) {
						P1.stateno = 215, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)) {
						P1.stateno = 310, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.stateno = 210, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				// 先行入力
				else if ((P1.Senkou[1] > 0) && (P1.Var[2] < 5)) {
					if (key(2)) {
						P1.stateno = 300, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}

				// [必殺技・ゲージ技]
				SCancel();
			}
			// キャンセル、連打キャンセル
			if ((P1.button[1] == 1) && (P1.time >= ANIMELEM - 2) && (P1.stateno == 200)) {
				// 下要素が入ってる
				if (!key(2)) {
					P1.More = 1, P1.time = 0, P1.A.damage = 0;
				}
			}

			// 投げ
			DelayThrow(1);
			// 3ゲージ
			Delay3Button(850, 3000);

			break;

			//********************
			// 205：立ちA
			//********************
		case 205:	//立ちA
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// ヒット数セット 
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}
			// 位置をずらす
			if (P1.time == 2) {
				PosAdd(4, 0);
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// ダメージセット
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(90, 0);

				// [ゲージ] 
				Power(99);
				HitTime(8, 18, 20, 16);

				// [ノックバック]
				HitVel(-3.6, 0, -1.4, -5);
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
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
					// [ジャンプキャンセル]
					if (P1.K_Senkou[8]) {		// 先行入力効かせてみる
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [通常技]
					if (P1.Senkou[3] > 0) {
						if (key(2)) {
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else {
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// [必殺技・ゲージ技]
					SCancel();
				}
			}

			// 投げ
			// 2Fに投げをずらし押ししたら
			if ((P1.time == 1) &&
				(P1.button[1] == 2 && P1.button[2] == 1) &&
				(P2.HFlg == 0)
				) {
				P1.stateno = 500, P1.More = 1,
					P1.time = 1, P1.A.damage = 0;
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
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}
			// SE
			if (P1.time == 1)SEStart(1);

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 2, 1);
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)) {

				// [ダメージ]
				Damage(90, 0);

				// [ゲージ] 
				Power(108);
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
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
					// [ジャンプキャンセル]
					if ((P1.K_Senkou[8]) && (P2.HFlg == 1)) {		// 先行入力効かせてみる
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [通常技]
					if (P1.Senkou[3] > 0) {
						// 足払い
						if ((key(6)) && (key(2))) {
							P1.stateno = 325, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else if (key(6)) {
							P1.stateno = 225, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else if (key(2)) {
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else {
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					else if (P1.Senkou[2] > 0) {
						// 下要素を確認
						if (key(2)) {
							P1.stateno = 310, P1.More = 1,
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
			// 215：6B
			//********************
		case 215:
			P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
			P1.ignoreG = true;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// 位置をずらす
			if (P1.time == 0) {
				PosAdd(6, 0);
			}
			// SEを鳴らす
			if (P1.time == 1)SEStart(3);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			
			// ヒット数セット 発生：26
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}
			//else{ P1.MoveHit = 0; }

			if (P1.time >= 5) {
				VelAdd(0, 0.38);
			}

			//速度を足す
			if (P1.time == 5) {
				VelSet(1.2, -5.0);
			}

			// ダメージセット
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(90, 20);
				// [ゲージ] 
				Power(155);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 19, 22, 16);
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

			// 終了
			if ((P1.YPos + P1.YVel * 2 >= GROUND) && (P1.YVel > 0) && (P1.time > 9)) {
				P1.stateno = 216;
				P1.time = 0;
				P1.SFlg = 0;
				P1.ctrl = 0;
			}

			// 投げ
			DelayThrow(2);
			// 3ゲージ
			Delay3Button(850, 3000);

			break;

		case 216:	// 着地硬直
			P1.SFlg = 0, P1.ctrl = 0;

			// SEを鳴らす
			if (P1.time == 1)SEStart(6);
			VelSet(0, 0);
			P1.YPos = GROUND;

			// 終了
			if (P1.time >= ANIMELEM + 1) {
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
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
			if (P1.time == 1) {
				PVOStart(P1.PSide, 3, 0);
			}
			
			// 位置をずらす
			if (P1.time == 0) {
				VelSet(2.2, 0);
			}

			if (P1.time == 7) {
				if (P1.muki == 0) {
					P1.XPos += 9;
				}
				else {
					P1.XPos -= 9;
				}
			}

			// アーマーカウント
			if (P1.time == 0)P1.Var[12] = P1.D.armor;

			// ヒット数セット
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(110, 10);
				// [ゲージ] 
				Power(150);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 24, 25, 22);
				//HitTime(8, 22, 24, 18);
				// [ノックバック]
				HitVel(-4.2, 0, -2.8, -2.8);
				GuardVel(-6.2, -4.0);
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

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
					// [必殺技・ゲージ技]
					SCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

			// 3ゲージ
			Delay3Button(850, 3000);

			break;

			//********************
			// 225：6C
			//********************
		case 225:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SEを鳴らす
			if (P1.time == 1) {
				SEStart(3);
			}

			if (P1.time == 0) {
				if (P1.muki == 0) {
					P1.XPos += 6;
				}
				else {
					P1.XPos -= 6;
				}
			}

			if (P1.time == 9) {
				P1.XVel = 4.2;
			}

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 3, 0);
			}

			// ヒット数セット
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(110, 10);
				// [ゲージ] 
				Power(150);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 22, 22, 20);
				// [ノックバック]
				HitVel(-4.2, 0, -2.8, -2.8);
				GuardVel(-6.4, -4.0);
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

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
					// [必殺技・ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

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
			//[ 通常技の自動振り向き ]
			if (P1.time == 0) {
				if ((P1.XPos < P2.XPos) && (P1.muki == 1)) {
					P1.muki = 0;
				}
				else if ((P1.XPos > P2.XPos) && (P1.muki == 0)) {
					P1.muki = 1;
				}
			}
			// 先行入力消す
			if (P1.time <= 1)P1.Senkou[1] = 0;

			// 連打回数カウント
			if (P1.time == 0) {
				P1.Var[2]++;
			}

			// SEを鳴らす
			if (P1.time == 1)SEStart(1);
			//  全体フレーム、14を超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 1, 2);
			}

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if ((P1.time >= 1) && (P1.MoveHit > 0)) {

				// [ダメージ]
				Damage(45, 0);

				// [ゲージ] 
				Power(30);

				// [ヒットストップ・のけぞり時間]
				HitTime(6, 12, 16, 10);

				// [ノックバック]
				HitVel(-3.6, 0, -1.6, -4.8);


				P1.GuardF = 1;

				P1.HitAnim = 1010;	// 下段喰らい
				P1.HitSE = 10;
			}
			else {
				DamReset();
			}


			// キャンセル
			if ((P1.CFlg) && (P1.time >= 1)) {

				if (P1.Senkou[3] > 0) {
					// 足払い
					if ((key(6)) && (key(2))) {
						P1.stateno = 325, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// 6C
					else if ((key(6)) && (!key(2)) && (!key(8))) {
						P1.stateno = 225, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)) {
						P1.stateno = 320, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.stateno = 220, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if (P1.Senkou[2] > 0) {		// 先行入力効かせてみる
					// 6B
					if ((key(6)) && (!key(2)) && (!key(8))) {
						P1.stateno = 215, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)) {
						P1.stateno = 310, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.stateno = 210, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if ((P1.Senkou[1] > 0) && (P1.Var[2] < 5)) {		// 先行入力効かせてみる
					if (key(2)) {
						P1.stateno = 300, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.stateno = 200, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}

				// [必殺技・ゲージ技]
				SCancel();
			}

			// キャンセル、連打キャンセル
			if ((P1.button[1] == 1) && (P1.time >= ANIMELEM - 2) && (P1.stateno == 300)) {
				// 下要素が入ってる
				if (key(2)) {
					P1.More = 1, P1.time = 0, P1.A.damage = 0;
				}
			}

			// 投げ
			DelayThrow(1);
			// 3ゲージ
			Delay3Button(850, 3000);

			break;
			//********************
			// 310：しゃがみB
			//********************
		case 310:
			P1.ctrl = 0, P1.SFlg = 1;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// 位置をずらす
			if (P1.time == 5) {
				if (P1.muki == 0) {
					P1.XPos += 6;
				}
				else {
					P1.XPos -= 6;
				}
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;
			// ヒット数セット
			if (P1.time == 5) {
				P1.MoveHit = 1;	// １回
			}


			// ダメージセット
			if ((P1.time >= 5) && (P1.time <= 10)) {

				// [ダメージ]
				Damage(85, 0);

				// [ゲージ] 
				Power(95);

				HitTime(8, 18, 20, 14);

				// [ノックバック]
				HitVel(-4.0, 0, -1.8, -4.6);
				P1.GuardF = 3;	// 下段
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
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 5)) {
					// [通常技]
					if (P1.Senkou[3] > 0) {		// 先行入力効かせてみる
						// 足払い
						if ((key(6)) && (key(2))) {
							P1.stateno = 325, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						// スラ
						else if (key(6)) {
							P1.stateno = 225, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						// 下要素を確認
						else if (key(2)) {
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else {
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
			// 320：足払い
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
			if (P1.time == 1) {
				PVOStart(P1.PSide, 3, 0);
			}

			// ヒット数セット 10フレ
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(105, 0);
				// [ゲージ] 
				Power(125);
				HitTime(10, 40, 40, 18);
				// [ノックバック]
				HitVel(-1.8, -4, -2.2, -3.5);
				GuardVel(-4.0, -1.5);

				P1.GuardF = 3;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// 空中喰らい(確定ダウン)
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;
			}
			else { DamReset(); }
			// 応急処置
			if (P1.StopTime == 0) {
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 10)) {
					// [必殺技・ゲージ技]
					SCancel();
				}
			}
			
			// 3ゲージ
			Delay3Button(850, 3000);
			
			break;

			//********************
			// 325：打ち上げ
			//********************
		case 325:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SEを鳴らす
			if (P1.time == 1)SEStart(2);
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 3, 0);
			}

			// ヒット数セット 10フレ
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(110, 0);
				// [ゲージ] 
				Power(132);
				HitTime(8, 30, 30, 18);
				// [ノックバック]
				HitVel(-1.2, -6.5, -1.5, -4.6);
				GuardVel(-3.8, -1.5);

				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// 空中喰らい(確定ダウン)
				P1.HitSE = 12;
			}
			else { DamReset(); }
			// 応急処置
			if (P1.StopTime == 0) {
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 10)) {
					// [ジャンプキャンセル]
					if ((P1.K_Senkou[8]) && (P2.stateno >= 1000)) {		// 先行入力効かせてみる
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
			// 400：ジャンプA
			//********************
		case 400:
			P1.ctrl = 0, P1.SFlg = 2;
			if (P1.time == 1)SEStart(1);
			// 先行入力消す
			if (P1.time <= 1)P1.Senkou[1] = 0;

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 1, 2);
			}

			// ヒット数セット
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(45, 0);
				// [ゲージ] 
				Power(26);

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

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)) {
					if (P1.Senkou[3] > 0) {		// 先行入力効かせてみる
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[2] > 0) {		// 先行入力効かせてみる
						P1.stateno = 410, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[1] > 0) {		// 先行入力効かせてみる
						P1.stateno = 400, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [空中必殺技]
					ACancel();
				}

				// [ジャンプキャンセル]
				if ((P1.CFlg) && (P1.time >= 1) && (P1.AirJump > 0)) {
					if (P1.K_Senkou[8] >= 1) {		// 先行入力効かせてみる
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}
			// 全体フレームを超えたらリセット
			/*
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

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 2, 1);
			}

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;
			// ダメージセット
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(80, 0);

				// [ゲージ] 
				Power(90);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 16, 18, 14);
				// [ノックバック]
				HitVel(-4.6, 0, -1.6, -4.0);
				// ガード判定
				if (P1.YVel >= 0.0)P1.GuardF = 2;
				else { P1.GuardF = 1; }
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
			if (P1.time >= 23){
			P1.ctrl = 1;
			}
			*/
			// キャンセル
			// 応急処置
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)) {
					if (P1.Senkou[3] > 0) {		// 先行入力効かせてみる
						// 6B
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [空中必殺技]
					ACancel();
				}

				// [ジャンプキャンセル]
				if ((P1.CFlg) && (P1.time >= 1) && (P1.AirJump > 0)) {
					if (P1.K_Senkou[8] >= 1) {		// 先行入力効かせてみる
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
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(3);

			if (P1.time == 0)VelSet(0, -0.2);
			if (P1.time == 11)VelSet(0, 7.6);

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 5フレ
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(116, 0);
				// [ゲージ] 
				Power(148);

				// [ヒットストップ・のけぞり時間]
				HitTime(8, 68, 60, 14);

				// [ノックバック]
				HitVel(-1.0, -1.8, -1.2, 6.2);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.A.boundLevel = 1;
				if (P2.SFlg == 2)P1.A.boundLevel = 2;	// 2..1回まで;
				P1.A.bound_yvel = -2.2;
				P1.HitAnim = 1000;
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			if (Chakuchi()) {
				P1.stateno = 421, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
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

			// キャンセル
			if (P1.StopTime == 0) {
				// ヒット時キャンセル
				if (P2.HFlg == 1) {
					// [ゲージ技]
					HCancel();
				}
			}

			// 終了
			if (P1.time >= ANIMELEM) {
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
			if ((P1.YPos + P1.YVel >= GROUND) && (P1.time >= 1)) {
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

			if (P1.time == 0) {
				P1.ignoreG = false;
				afterimageSetting(P1.PSide, 15, 155, 155, 255);
			}
			else { P1.ignoreG = true; }
			// SE
			if (P1.time == 1)SEStart(3);

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 3, 0);
			}

			P1.YVel += GRAVITY;
			if (P1.time == 0) {
				P1.YVel = -3.8;
				if (P1.XVel > P1.C.jumpF[0])P1.XVel = P1.C.jumpF[0];
				else if (P1.XVel < P1.C.jumpB)P1.XVel = P1.C.jumpB;
			}
			if (P1.YVel >= 9.0)P1.YVel = 9.0;

			// ヒット数セット
			if (P1.time == 1)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 5フレ
			if (P1.time >= 1) {

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
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;
				// バウンド
				if (P1.HitCount == 0) {
					P1.A.boundLevel = 1;
					P1.A.bound_xvel = -1.2;
					P1.A.bound_yvel = -3.2;
				}
				else {
					P1.A.boundLevel = 1;
					P1.A.bound_yvel = -2.8;
				}
				P1.A.hosei_K = 0.9;
			}
			else {
				DamReset();
			}

			// 着地
			if (P1.YPos + P1.YVel * 2 >= GROUND) {
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
			// ずらし投げに対応するために1Fから
			if (P1.time == 1) {
				P1.throwSide = P1.muki;
				P1.A.throwTurn = false;
				if (P1.keyAtt[4]) {
					P1.A.throwTurn = true;
				}
				P1.XVel = 0;
			}

			// VO
			if ((P1.time == 9) && (P1.StopTime == 0)) {
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
			if (P1.time == 4) {

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
			if ((P1.CFlg) && (P1.time == 8)) {
				P1.stateno = 505;
				P1.time = 0, P2.time = 0;
				P1.More = 1;
				P1.A.damage = 0;

				P1.StopTime = 6;

				// 投げの向きを変更
				if (P1.A.throwTurn) {
					if (P1.muki == 0) {
						P1.muki = 1, P2.muki = 0;
					}
					else if (P1.muki == 1) {
						P1.muki = 0, P2.muki = 1;
					}
				}


			}
			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

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
			if (P1.time == 0) {
				int dist = 76;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + dist;
					// もし距離が離れなかったら
					if (P2.XPos > STAGE_WIDTH - GAMENHAJI) {
						P2.XPos = STAGE_WIDTH - GAMENHAJI;
						P1.XPos = P2.XPos - dist;
					}
				}
				else {
					P2.XPos = P1.XPos - dist;
					if (P2.XPos < GAMENHAJI) {
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
			if (P1.time < 21)P2.Muteki = 1, P2.mutekiF = 0;
			
			if (P1.time > 25 && P1.time < 42)P2.Muteki = 1, P2.mutekiF = 0;


			// ヒット数セット
			if (P1.time == 22 || P1.time == 43)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 1フレ
			if (P1.time == 22) {

				// [攻撃判定セット]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [ダメージ]
				Damage(0, 80);
				// [ゲージ] 
				Power(140);
				HitTime(2, 45, 45, 0);
				// [ノックバック]
				HitVel(-3.0, 0, 0, 0);
				P1.HitAnim = 1000;
				// [喰らい中の浮き]
				P1.fallF = 0;
				P1.HitSE = 12;
			}
			// ダメージセット、持続 1フレ
			else if (P1.time >= 43) {

				// [攻撃判定セット]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [ダメージ]
				Damage(0, 100);
				P1.A.hosei_K = 0.6;
				// [ゲージ] 
				Power(160);
				HitTime(6, 45, 45, 0);
				// [ノックバック]
				HitVel(-2.6, -3.4, 0, 0);
				P1.HitAnim = 1030;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitSE = 12;
			}
			else { DamReset(); }

			// 応急処置
			if (P1.StopTime == 0) {
				// キャンセル
				if ((P1.CFlg) && (P1.time >= 48) && (P1.scTime > 0)) {
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
			if (P1.time == 1) {
				P1.throwSide = P1.muki;
				P1.A.throwTurn = false;
				if (P1.muki == 0) {
					if (InputPAD(104) >= 1) {
						P1.A.throwTurn = true;
					}
				}
				if (P1.muki == 1) {
					if (InputPAD(106) >= 1) {
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
			if (P1.time == 4) {

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
			if ((P1.CFlg) && (P1.time >= 7)) {
				P1.stateno = 515;
				P1.time = 0, P2.time = 0;
				P1.More = 1;
				P1.A.damage = 0;
				// 投げの向きを変更
				if (P1.A.throwTurn) {
					if (P1.muki == 0) {
						P1.muki = 1, P2.muki = 0;
					}
					else if (P1.muki == 1) {
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
			if ((P1.time == 0) && (P1.YPos + 20 >= GROUND)) {
				P1.YPos = GROUND - 20;
			}

			// 距離をとる
			if (P1.time == 0) {
				P1.YPos -= 2;
				P2.YPos = P1.YPos - 26;
				int dist = P2.xSize + 5;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + dist;
					// もし距離が離れなかったら
					if (P2.XPos > STAGE_WIDTH - GAMENHAJI) {
						P2.XPos = STAGE_WIDTH - GAMENHAJI;
						P1.XPos = P2.XPos - dist;
					}
				}
				else {
					P2.XPos = P1.XPos - dist;
					if (P2.XPos < GAMENHAJI) {
						P2.XPos = GAMENHAJI;
						P1.XPos = P2.XPos + dist;
					}
				}
				P1.YVel = -2.2;
				P2.YVel = -2.2;
			}
			// 重力加算
			if (P1.time > 16) {
				P1.YVel += GRAVITY + 0.05;
			}
			// 無敵終了
			else if (P1.time == 16) {
				P1.YVel += GRAVITY;
				P2.YVel += GRAVITY;
				P1.YVel = -4;
			}
			// ついでに無敵時間
			else {
				P2.Muteki = 1, P2.mutekiF = 0;
				P2.time = 0;
				P1.YVel += GRAVITY / 2;
				P2.YVel += GRAVITY / 2;
			}
			// SEを鳴らす
			if (P1.time == 15)SEStart(10);

			// VO
			if ((P1.time == 9) && (P1.StopTime == 0)) {
				PVOStart(P1.PSide, 10, 0);
			}

			// ヒット数セット
			if (P1.time == 16)P1.MoveHit = 1;	// １回
			// ダメージセット、持続 1フレ
			if (P1.time == 17) {

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
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 11;


			}
			else {
				DamReset();
			}

			// 姿勢変更
			if (P1.time == 18)P2.time = 20;

			// 投げ抜け
			ThrowReject();

			// 全体フレームを超えたらリセット
			if (P1.YPos + P1.YVel * 2 >= GROUND) {
				P1.time = 0, P1.stateno = 47,
					P1.YPos = GROUND, P1.YVel = 0,
					P1.ctrl = 0, P1.SFlg = 0;
			}
			break;

			//********************
			// 600 硬破斬
			//********************
		case 600:
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 0) {
				SEStart(3);
				P1.XVel = 0;
			}

			// ヒット数セット
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
				P1.Var[11] = 0;
			}

			// ガーポの色
			if ((P1.time >= 1) && (P1.time < 50)) {
				P1.colorCTime = 2;
				P1.colorC[0] = 160, P1.colorC[1] = 160, P1.colorC[2] = 255;
			}

			if(P1.time == 50){
				SEStart(31);
				// 中段エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
				P1.Var[11] = 1;	// タメ
			}

			//
			if ((P1.K_Senkou[6] == 1) && (P1.time >= 10) && (P1.time < 50)) {
				P1.stateno = 30;
				P1.time = 0;
				P1.SFlg = 0;
				P1.More = 1;
			}
			// バクステ
			if ((P1.K_Senkou[4] == 1) && (P1.time >= 10) && (P1.time < 50)) {
				P1.stateno = 35;
				P1.time = 0;
				P1.SFlg = 0;
				P1.More = 1;
			}

			// 全体フレームを超えたらリセット
			if ((P1.time >= ANIMELEM) || 
				((P1.button[4] == 0) && (P1.time >= 14) && (P1.time < 50))) {
				P1.time = 0, P1.stateno = 601, P1.ctrl = 1;
				P1.More = 1;
			}

			break;

			//********************
			// 601 硬破斬派生
			//********************
		case 601:
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 0) {
				SEStart(21);
				P1.XVel = 0;
			}

			// ヒット数セット
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(110, 0);
				// [ゲージ] 
				Power(135);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 22, 24, 18);
				// [ノックバック]
				HitVel(-2.4, -3.4, -2.6, -2.6);
				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 2;
				// [ガード属性]
				P1.GuardF = 1;
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 13;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 0.2;
				P1.A.boundLevel = 0;

				// 最大タメ
				if (P1.Var[11] == 1) {
					Damage(170, 100);
					P1.A.hosei_K = 0.6;
					Power(380);
					HitTime(14, 38, 38, 18);
					HitVel(-1.4, -4.0, -1.6, -3.2);
					// エフェクト
					HitEff(3, 0.8, 0.8);
					P1.HitSE = 14;
					P1.GuardF = 0;	// ガー不
					P1.fallF = 1;
					P1.A.wallLevel = 0;
				}
			}
			else {
				DamReset();
			}

			// キャンセル
			if (P1.StopTime == 0) {
				// ヒット時キャンセル
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)) {
					// [ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;

			//********************
			// 610：時つ風(上)
			//********************
		case 610:
			P1.ctrl = 0, P1.SFlg = 0;

			if (P1.time == 0)VelSet(0, 0);

			// SE
			if (P1.time == 1)SEStart(2);

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 611：時つ風(下)
			//********************
		case 611:
			P1.ctrl = 0, P1.SFlg = 0;

			if (P1.time == 0)VelSet(0, 0);

			// SE
			if (P1.time == 1)SEStart(2);

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;


			//********************
			// 615 時つ風(成功)
			//********************
		case 615:
			P1.ctrl = 0, P1.SFlg = 0;
			// SEを鳴らす、移動
			if (P1.time == 1) {
				SEStart(21);
				P1.XVel = 0;
			}

			// ヒット数セット
			if (P1.time == 1 || P1.time == 20) {
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 0) {
				DamReset();
			}

			// ダメージセット
			if (P1.time == 1) {

				// [ダメージ]
				Damage(80, 0);
				// [ゲージ] 
				Power(110);
				// [ヒットストップ・のけぞり時間]
				HitTime(8, 22, 24, 18);
				// [ノックバック]
				HitVel(-2.6, 0, -2.8, 0);
				P1.HitAnim = 1060;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 13;
			}
			// ダメージセット
			else if (P1.time == 20) {

				// [ダメージ]
				Damage(0, 110);
				P1.A.hosei_K = 0.6;

				// [ゲージ] 
				Power(210);

				HitTime(10, 100, 100, 12);
				// [ノックバック]
				HitVel(-4.0, -3.6, -3.8, -3.2);

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
			if ((P1.CFlg) && (P1.time < 5) && (P2.stateno == 1060) && (P2.SFlg == 0)) {
				P2.YPos = GROUND;

				P2.XVel = 0;


				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 100;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 100;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + 100;
				}
				else if (P1.muki == 1) {
					P2.XPos = P1.XPos - 100;
				}
			}

			// キャンセル
			if (P1.StopTime == 0) {
				// ヒット時キャンセル
				if ((P1.CFlg) && (P1.time >= 20) && (P1.scTime > 0)) {
					// [ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;

			//********************
			// 620：兜
			//********************
		case 620:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SEを鳴らす
			if (P1.time == 1)SEStart(3);

			// 位置をずらす
			if (P1.time == 4) {
				VelAdd(6.0, 0);
			}

			// アーマーカウント
			if (P1.time == 0)P1.Var[12] = P1.D.armor;

			// ヒット数セット
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(110, 10);
				// [ゲージ] 
				Power(150);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 27, 27, 23);
				// [ノックバック]
				HitVel(-4.0, 0, -2.0, 3.0);
				GuardVel(-6.2, -4.0);
				// [ガード属性]
				P1.GuardF = 1;


				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// 下段喰らい

				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;

				P1.A.boundLevel = 2;
			}
			else {
				DamReset();
			}

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
					// [必殺技・ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

			break;

			//********************
			// 621：兜
			//********************
		case 621:
			P1.ctrl = 0, P1.SFlg = 0;
			// 慣性
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SEを鳴らす
			if (P1.time == 1)SEStart(3);

			// 位置をずらす
			if (P1.time == 8) {
				VelAdd(10.0, 0);
			}

			// アーマーカウント
			if (P1.time == 0)P1.Var[12] = P1.D.armor;

			// ヒット数セット
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}

			// ダメージセット
			if (P1.time >= 1) {

				// [ダメージ]
				Damage(110, 10);
				// [ゲージ] 
				Power(150);
				// [ヒットストップ・のけぞり時間]
				HitTime(10, 27, 27, 23);
				// [ノックバック]
				HitVel(-4.0, 0, -2.0, 3.1);
				GuardVel(-6.2, -4.0);
				// [ガード属性]
				P1.GuardF = 2;


				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// 下段喰らい
				P1.A.forceKurai = true;	// 喰らい姿勢を強制

				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;

				P1.A.boundLevel = 2;
			}
			else {
				DamReset();
			}

			// キャンセル
			// 応急処置
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
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
			if (P1.time == 0) {
				VelSet(0, 0);
				PosAdd(20, 0);
			}
			// SEを鳴らす
			if (P1.time == 1)SEStart(3);

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 6, 0);
			}

			// ゲージ消費
			if (P1.time == 1) {
				P1.Power -= 500;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			// ヒット数セット
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
			}

			if (P1.time == 13 - 1) {
				if (P1.muki == 0) {
					EffStartB(18, (int)P1.XPos + 140, P1.YPos, 0, -7,
						0.25, 0.06, P1.muki);
				}
				else {
					EffStartB(18, (int)P1.XPos - 140, P1.YPos, 0, -7,
						0.25, 0.06, P1.muki);
				}
			}

			// ダメージセット
			if (P1.time >= 1) {

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
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)) {
					// [必殺技・ゲージ技]
					HCancel();
				}
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

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
			if (P1.time == 0) {
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

			if (AnimElem(20)) {
				SEStart(22);
			}

			// 3456 9
			// 前進
			if (AnimElem(27)) {
				P1.XVel = 10.0;
			}

			// ヒット数セット	14フレ～
			if (AnimElem(2) || AnimElem(6) ||
				AnimElem(10) || AnimElem(13) ||
				AnimElem(17) || AnimElem(30)) {
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット

			if (P1.time < 1) {
				DamReset();
			}
			if ((P1.time >= 1) && (P1.time < 24)) {

				// [ダメージ]
				Damage(60, 4);
				// [ゲージ] 
				Power(0, 70);
				HitTime(0, 60, 60, 22);
				// [ノックバック]
				HitVel(-3.2, 0, -1.4, -2.0);
				// 一発目
				if (P1.time < 14) {
					if (P2.SFlg != 2) {
						P1.HitAnim = 1060;	// 地上
					}
					else if (P2.SFlg == 2) {
						P1.HitAnim = 1060;	// 地上へ
					}
				}
				else {
					P1.HitAnim = 1000;	// 地上
				}
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.A.kill = 0;
			}
			// ダメージセット
			else if (P1.time >= 100) {

				// [ダメージ]
				Damage(35, 40);
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
				P1.A.kill = 1;
				// 地震エフェクト
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}

			// 当たったら変更
			if ((P1.CFlg) && (P1.time < 16) && (P2.stateno == 1060) && (P2.SFlg == 0)) {
				P2.YPos = GROUND;

				P2.XVel = 0;

				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 100;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 100;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + 100;
				}
				else if (P1.muki == 1) {
					P2.XPos = P1.XPos - 100;
				}

			}

			// 立ち の 全体フレーム、54を超えたらリセット
			if (P1.time >= ANIMELEM) {
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}

			break;

			//********************
			// 810：超当身
			//********************
		case 810:
			P1.ctrl = 0, P1.SFlg = 0;

			if (P1.time == 0) {
				VelSet(0, 0);
				P1.Power -= 1000;
			}

			// SE
			if (P1.time == 1)SEStart(2);

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;

			//********************
			// 811 超当身(成功)
			//********************
		case 811:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = 1;

			// ゲージ消費
			if (P1.time == 0) {
				S.StopTime = 10;
				S.Anten = 25;
				SEStart(25);
				// 中段エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}

			// SEを鳴らす、移動
			if (P1.time == 1) {
				SEStart(21);
				//P1.XVel = 10;
				P1.XPos = P2.XPos;
				PosAdd(98, 0);
			}
			// ヒット数セット
			if (P1.time == 1) {
				P1.MoveHit = 1;	// １回
				// エフェクト
				EffStart(4, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize - (P1.GSize * 29.0), 0, 0, 7.0, 0.8, P1.PSide);
			}

			if (P1.time == 0) {
				DamReset();
				P1.touchF = false;
			}

			// 剣を振る
			if (P1.time == 45)SEStart(27);

			// 接触復活
			if (P1.time == 10)P1.touchF = true;

			// ダメージセット
			if (P1.time == 1) {

				// [ダメージ]
				Damage(260, 30);
				// [ゲージ] 
				Power(0, 420);

				HitTime(16, 100, 100, 10);
				// [ノックバック]
				HitVel(0.0, -5.6, 0.0, -5.2);

				P1.HitAnim = 1030;	// 空中喰らい
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 14;
				// エフェクト
				HitEff(0, 0.8, 0.8);
				// 地震エフェクト
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}

			// 向き変更
			if((P1.time >= 1 && P1.time <= 4) && (P2.stateno >= 1000) && (P1.muki != P2.muki) 
				&& (P1.XPos > GAMENHAJI && P1.XPos < STAGE_WIDTH - GAMENHAJI)){
				if (P2.muki == 0)P2.muki = 1;
				else { P2.muki = 0; }
			}

			// 全体フレームを超えたらリセット
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;

			//********************
			// 850 ヴァンパイアキラー
			//********************
		case 850:
			P1.ctrl = 0;
			P1.A.gaugeHosei = true;
			P1.ignoreG = false;
			P1.SFlg = 0;

			// ゲージ消費
			if (P1.time == 0) {
				P1.Power -= 3000;
				S.StopTime = 15;
				S.noGauge = 15;
				S.Anten = 25;
				SEStart(25);
				// エフェクト
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);

				DamReset();
			}

			// SEを鳴らす
			if (P1.time == 1) {
				SEStart(21);
			}

			if (P1.time == 1) {
				P1.MoveHit = 1;
			}
			if (P1.time == 28) {
				P1.MoveHit = 0;
			}

			// ダメージセット
			if ((P1.time >= 1) && (P1.time < 28)) {

				// [ダメージ]
				Damage(100, 0);
				// [ゲージ] 
				Power(0, 100);
				HitTime(4, 24, 24, 14);
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

			// 当たったら変更
			if ((P1.CFlg) && (P1.time >= 1) && (!P1.ignoreG) && (P2.stateno >= 1000) && (P2.YPos != GROUND)) {
				P2.YPos = GROUND;
				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 50;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 50;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + 50;
				}
				else if (P1.muki == 1) {
					P2.XPos = P1.XPos - 50;
				}
			}


			// 流星キックへ
			if ((P1.CFlg) && (P2.HFlg == 1) && (P1.time > 0)) {
				P1.time = 0, P1.stateno = 851, P1.ctrl = 0;
				P1.More = 1;
				P1.A.damage = 0;
			}

			// 終了
			if (P1.time >= ANIMELEM) {
				P1.time = 0, P1.stateno = 0, P1.ctrl = 0;
				
			}

			break;

			//********************
			// 851 3ゲージヒット
			//********************
		case 851:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;

			// 無敵
			if (P1.time <= 9)P1.Muteki = 1, P1.mutekiF = 0;
			else { P1.Muteki = 0; }

			// 3456 9
			// 前進
			if (AnimElem(4) || AnimElem(6) ||
				(AnimElem(9) || (AnimElem(12)))) {
				P1.XVel = 2.2;
			}
			if (AnimElem(20)) {
				P1.XVel = 6.0;
			}

			// ヒット数セット	14フレ～
			if (AnimElem(3) || AnimElem(6) ||
				AnimElem(9) || AnimElem(12) ||
				AnimElem(14)) {
				P1.MoveHit = 1;	// １回
			}
			// ダメージセット

			if (P1.time == 0) {
				DamReset();
			}
			if ((P1.time >= 1) && (P1.time < 30)) {

				// [ダメージ]
				Damage(90, 10);
				// [ゲージ] 
				Power(0, 90);
				HitTime(0, 60, 60, 22);
				// [ノックバック]
				HitVel(-3.4, 0, -0.4, -2.0);
				// 一発目
				if (P1.time < 14) {
					if (P2.SFlg != 2) {
						P1.HitAnim = 1060;	// 地上
					}
					else if (P2.SFlg == 2) {
						P1.HitAnim = 1060;	// 地上へ
					}
				}
				else {
					P1.HitAnim = 1000;	// 地上
				}
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				P1.HitSE = 13;
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.A.kill = 0;

			}
			// ダメージセット
			else if (P1.time >= 58) {

				// [ダメージ]
				Damage(80, 20);
				// [ゲージ] 
				Power(0, 100);
				HitTime(5, 60, 60, 22);
				// [ノックバック]
				HitVel(-0.6, -7.4, -0.8, -7.2);
				P1.A.yaccel = 0.16;	// 浮かせる
				P1.HitAnim = 1030;	// 空中
				// [喰らい中の浮き]
				P1.fallF = 1;
				// [ガード属性]
				P1.GuardF = 1;
				

				P1.HitSE = 13;
				// エフェクト
				HitEff(3, 0.6, 0.6);
				P1.A.kill = 0;

			}

			// 当たったら変更
			if ((P1.CFlg) && (P1.time < 16) && (P2.stateno == 1060) && (P2.SFlg == 0)) {
				P2.YPos = GROUND;

				P2.XVel = 0;

				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 100;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 100;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + 100;
				}
				else if (P1.muki == 1) {
					P2.XPos = P1.XPos - 100;
				}

			}

			// 立ち の 全体フレーム、54を超えたらリセット
			if (P1.time >= ANIMELEM) {
				P1.time = 0, P1.stateno = 852, P1.ctrl = 0;
			}

			break;

			//********************
			// 852：ジャンプC
			//********************
		case 852:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.A.gaugeHosei = true;
			P1.ignoreG = true;

			// SE
			if (P1.time == 47)SEStart(3);

			if (P1.time == 1)VelSet(0.0, -10.2);
			if (P1.time == 15) {
				VelSet(0, 0);
			}
			if (P1.time == 56)VelSet(0, 8.6);

			// ヒット数セット
			if (P1.time == 1 || P1.time == 56)P1.MoveHit = 1;	// １回

			// ダメージセット
			if (P1.time >= 1 && P1.time < 47) {

				// [ダメージ]
				Damage(25, 40);
				// [ゲージ] 
				Power(0, 80);

				// [ヒットストップ・のけぞり時間]
				HitTime(5, 68, 60, 14);

				// [ノックバック]
				HitVel(0.0, -1.8, 0.0, 6.2);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.A.boundLevel = 3;	// 2..1回まで
				P1.A.bound_yvel = -6.2;
				P1.A.bound_xvel = 0.0;
				P1.HitAnim = 1030;
				// エフェクト
				HitEff(1, 0.6, 0.6);
				P1.HitSE = 12;
				P1.A.kill = 0;
				// 地震エフェクト
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}
			else if (P1.time >= 56) {

				// [ダメージ]
				Damage(25, 100);
				// [ゲージ] 
				Power(0, 120);

				// [ヒットストップ・のけぞり時間]
				HitTime(12, 68, 60, 14);

				// [ノックバック]
				HitVel(-1.0, -1.8, -1.2, 6.2);
				P1.GuardF = 1;
				// [喰らい中の浮き]
				P1.fallF = 1;
				P1.A.boundLevel = 1;	// 2..1回まで
				P1.A.bound_yvel = -2.2;
				P1.HitAnim = 1030;
				// エフェクト
				HitEff(3, 1.0, 1.0);
				P1.HitSE = 14;
				P1.A.kill = 1;
				// 地震エフェクト
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}
			else {
				DamReset();
			}
			if (Chakuchi()) {
				P1.stateno = 421, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;

			//===============================//
		}// switch終了

		// ステート番号を保存
		P1.OldState = P1.stateno;
		P1.oldState2 = P1.State2;

	}// P1処理終了


	for (int i = 0; i < HM; i++) {
		{
			//********************
			//
			// ヘルパー
			//
			//********************

			// オンだったらステートに飛ばす
			if (H1[i].var) {
				switch (H1[i].stateno) {
					//********************
					// 2000 置かれた大ブメ 
					//********************
				case 2000:

					//***************
					//  基本設定
					//***************	
					if (H1[i].time == 0) {
						H1[i].muki = P1.muki;
						H_PosSet(i, -80, 0);
					}
					//***************
					//  行動内容
					//***************

					// 全体フレーム33or被ダメ
					if ((H1[i].time > 33) ||
						(P1.stateno >= 1000)) {
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
void GetP_SyuParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_SyuParam(System_t GS)
{
	S = GS;
}

void GetH_SyuParam(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++) {
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
		&& (P1.SFlg != 2)) { //&& (P1.D.armor == 0 || (P1.stateno == 225 || P1.stateno == 320))){		// 先行入力効かせてみる
		{
			P1.stateno = 600, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	// 236 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[1])) {		// 先行入力効かせてみる
		P1.stateno = 620, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		if (P1.Senkou[2] > 0 || P1.Senkou[3] > 0)P1.stateno = 621;
	}

	// 214 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[2])) {		// 先行入力効かせてみる
		P1.stateno = 610, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		if (P1.Senkou[2] > 0 || P1.Senkou[3] > 0)P1.stateno = 611;
	}
	/*
	// 236 + C
	if ((P1.Senkou[3] > 0) && (P1.cmd[1]) && (P1.Power >= 500)) {		// 先行入力効かせてみる
		P1.stateno = 625, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	*/
	HCancel();

	//最後に判定を消す
	DamReset();
}

// 必殺技キャンセル
void HCancel()
{
	//  236+C		[二段切り]
	if ((P1.Senkou[3] > 0) && (P1.cmd[1])
		&& (P1.Power >= 1000)) {
		if (P1.SFlg != 2) {		// 地上
			P1.stateno = 800, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	//  214+C		[超当身]
	if ((P1.Senkou[3] > 0) && (P1.cmd[2])
		&& (P1.Power >= 1000)) {
		if (P1.SFlg != 2) {		// 地上
			P1.stateno = 811, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
			P1.Power -= 1000;
		}
	}

	//  [ ３ゲージ ]
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.Senkou[3] > 0)
		&& (P1.Power >= 3000)) {		// 地上
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

}
