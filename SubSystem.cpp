//==================================================================================
//
//			補助計算の管理部
//
//==================================================================================

//インクルード部--------------------------------------------
#include "pch.h"

//デファイン部----------------------------------------------

//#include "Define.h" で設定

//ﾃﾞｰﾀ定義部------------------------------------------------

static void TH_Set(Player GP1, Player GP2);

//内部関数宣言部--------------------------------------------

//#include "Setting.h" で設定

// 離れるべき距離
static double Distance, DisPoint;
// スクロール量
static double ScXPoint, ScYPoint;
// スクロール用のキャラ速度
static double P1_ScX, P2_ScX;

// 1フレーム前の向き
static int OldMuki[2];
static double m_oldXPos[2], m_oldYPos[2];

static Player mP1, mP2;

// 過去の座標から、ずらす量を計算
double xp[2], yp[2];

// 左側のキャラ、右側のキャラ、キャラ間の距離
static double leftP, rightP, PDist;

//プログラム部----------------------------------------------

//==================================================================================
//
//			ここでは クラス・State だけでは見づらい処理を持ってきている
//
//==================================================================================

void TouchHantei()
{
	TH_Set(P1, P2);
	Get_PSSet(P1, P2, S);
}

void TH_Set(Player GP1, Player GP2)
{

	Player P[2];
	P[0] = GP1, P[1] = GP2;

	for (int i = 0; i < 2; i++){
		//P1
		P[i].X_UL = (int)P[i].XPos - (P[i].xSize / 2);	//左上のX座標
		// 空中か地上かで判定変える
		if (P[i].SFlg == 2){
			P[i].Y_UL = (int)P[i].YPos - P[i].ySize - (20 * 2);		// 空中の時は判定小さく
		}
		else{
			P[i].Y_UL = (int)P[i].YPos - P[i].ySize;		//左上のY座標、10*2 は画像の足元
		}

		if (!P[i].touchF){
			P[i].X_UL = 0 - (i+1) * 500;
			P[i].Y_UL = 0 - (i+1) * 500;
		}
	}

	GetP_SubSystem(P[0],P[1]);
}

int SetScrool()
{
	//--------------------
	//
	// Xスクロール
	//
	//--------------------

	/*
	//----------------
	//
	// パターン１：少し動き続ける
	// 離れる条件：画面の4/5の距離より近くなる
	// 近づく条件：どちらかが近づく距離にいる
	//
	//----------------
	// 次に備えScXを今の位置に
	ScXPoint = S.ScroolX;

	double mDist = GAMENHAJI * 3;		// カメラ移動する条件の距離
	double mSDist = mDist * 2;			// カメラが接近する条件

	// 1P
	if (P1.XPos < S.ScroolX + mDist){
		ScXPoint = P1.XPos - mDist;
	}
	else if (P1.XPos > S.ScroolX + SCREEN_W - mDist){
		ScXPoint = P1.XPos + mDist - SCREEN_W;
	}
	// 2P
	if (P2.XPos < S.ScroolX + mDist){
		ScXPoint = P2.XPos - mDist;
	}
	else if (P2.XPos > S.ScroolX + SCREEN_W - mDist){
		ScXPoint = P2.XPos + mDist - SCREEN_W;
	}
	
	// 距離が離れすぎたら、スクロールしない
	if (PDist >= SCREEN_W - (mDist * 2))ScXPoint = S.ScroolX;
	

	// 1Pか2Pが画面端にいた場合、スクロールしない
	if ((SCREEN_W - mSDist <= P1.XPos - P2.XPos) ||
		(SCREEN_W - mSDist <= P2.XPos - P1.XPos))
	{
		ScXPoint = S.ScroolX;
	}
	*/


	/*
	//----------------
	//
	// パターン２：同じ距離をキープ
	// 離れる条件：1Pと2Pの距離
	//
	//----------------
	// 次に備えScXを今の位置に
	ScXPoint = S.ScroolX;

	if (P1.XPos <= P2.XPos){
		leftP = P1.XPos;
		rightP = P2.XPos;
	}
	else if (P2.XPos < P1.XPos){
		leftP = P2.XPos;
		rightP = P1.XPos;
	}
	PDist = rightP - leftP;
	if (PDist >= SCREEN_W - (GAMENHAJI * 2)){}
	else{ ScXPoint = leftP - (SCREEN_W / 2) + (PDist / 2); }
	*/

	
	//----------------
	//
	// パターン３：同じ距離をキープ
	// 離れる条件：1Pと2Pの距離
	//
	//----------------
	// 次に備えScXを今の位置に
	ScXPoint = S.ScroolX;
	
	double mDist = GAMENHAJI * 4;		// カメラ移動する条件の距離
	double mSDist = mDist * 2;			// カメラが接近する条件

	// 距離を測る、1Pと2Pの左右を決める
	if (P1.XPos <= P2.XPos){
	leftP = P1.XPos;
	rightP = P2.XPos;
	}
	else if (P2.XPos < P1.XPos){
	leftP = P2.XPos;
	rightP = P1.XPos;
	}
	// キャラ間の距離
	PDist = rightP - leftP;

	// 端端じゃないとき、距離調整
	if (PDist < SCREEN_W - (GAMENHAJI * 2)){
		// 1P
		if (P1.XPos < S.ScroolX + mDist){
			ScXPoint = P1.XPos - mDist;
		}
		else if (P1.XPos > S.ScroolX + SCREEN_W - mDist){
			ScXPoint = P1.XPos + mDist - SCREEN_W;
		}
		// 2P
		if (P2.XPos < S.ScroolX + mDist){
			ScXPoint = P2.XPos - mDist;
		}
		else if (P2.XPos > S.ScroolX + SCREEN_W - mDist){
			ScXPoint = P2.XPos + mDist - SCREEN_W;
		}
	}

	// キャラ間の距離が端々より大きい時、変化なし
	if (PDist >= SCREEN_W - (GAMENHAJI * 2)){}	// 640 - 70 = 570
	// やや遠い時、同じ距離に置く
	else if (PDist >= SCREEN_W - mSDist){ 
		ScXPoint = leftP - (SCREEN_W / 2) + (PDist / 2); // leftP - 360 + PDist
	}
	// 近い
	else {}

	/*
	// 1P
	if (P1.XPos < S.ScroolX + mDist){
		ScXPoint = P1.XPos - mDist;
	}
	else if (P1.XPos > S.ScroolX + SCREEN_W - mDist){
		ScXPoint = P1.XPos + mDist - SCREEN_W;
	}
	// 2P
	if (P2.XPos < S.ScroolX + mDist){
		ScXPoint = P2.XPos - mDist;
	}
	else if (P2.XPos > S.ScroolX + SCREEN_W - mDist){
		ScXPoint = P2.XPos + mDist - SCREEN_W;
	}

	// 距離が離れすぎたら、スクロールしない
	if (PDist >= SCREEN_W - (mDist * 2))ScXPoint = S.ScroolX;
	*/

	//--------------------
	//
	// Y軸スクロール
	//
	//--------------------
	// Y方向に、画面の 3/4 以上進んだら
	if ((P1.YPos < (S.ScroolY + (SCREEN_H / 2.2))) ||
		(P2.YPos < (S.ScroolY + (SCREEN_H / 2.2)))){
		if (P1.YPos < P2.YPos){
			//ScYPoint -= P1_ScY;		// 画面を上に
			ScYPoint = P1.YPos - 210;
		}
		else if (P1.YPos > P2.YPos){
			ScYPoint = P2.YPos - 210;
		}
		else{ ScYPoint = P1.YPos - 210; }
	}
	else {
		if (P1.YPos < P2.YPos){
			//ScYPoint += P1_ScY;		// 下
			ScYPoint = P1.YPos - 210;
		}
		else if (P1.YPos > P2.YPos){
			ScYPoint = P2.YPos - 210;
		}
		else{ ScYPoint = P1.YPos - 210; }
	}



	//--------------------
	// 修正
	//--------------------
	// スクロール量加算(noScroolがなければ)
	if (!S.noScroolX){
		// スクロールしすぎなら
		if (S.ScroolX - ScXPoint > 100)S.ScroolX -= 100;
		else if (ScXPoint - S.ScroolX > 100)S.ScroolX += 100;
		else{ S.ScroolX = ScXPoint; }	// 通常
	}
	if (!S.noScroolY){
		S.ScroolY = ScYPoint;
	}

	// スクロールステージを超えていたら戻す
	if ((STAGE_WIDTH - SCREEN_W) < (S.ScroolX)){
		S.ScroolX = STAGE_WIDTH - SCREEN_W;
	}
	else if (S.ScroolX < 0){
		S.ScroolX = 0;
	}
	// Y
	if ((STAGE_HEIGHT - SCREEN_H) < (S.ScroolY)){
		S.ScroolY = STAGE_HEIGHT - SCREEN_H;
	}
	else if (S.ScroolY < 0){
		S.ScroolY = 0;
	}
	
	// 次に備えScXを今の位置に
	ScXPoint = S.ScroolX;

	// Sを返す
	Get_SSet(S);

	return 0;
}

// キャラの接触チェック
int TouchCheck()
{
	// ほも
	if (S.roundState != 0){
	Player P[2];
	P[0] = P1, P[1] = P2;

	for (int i = 0; i < 2; i++){

		//X軸 端処理
		if (P[i].XPos <= (S.ScroolX + GAMENHAJI)){
			P[i].XPos = S.ScroolX + GAMENHAJI;
		}
		else if (P[i].XPos >= (S.ScroolX + SCREEN_W - GAMENHAJI)){
			P[i].XPos = S.ScroolX + (SCREEN_W - GAMENHAJI);
		}

	}
	// 戻ってくる
	P1 = P[0], P2 = P[1];

	double m_X[2];
	m_X[0] = P1.XPos - m_oldXPos[0];
	m_X[1] = P2.XPos - m_oldXPos[1];
	TH_Set(P1, P2);

	//当たり判定チェック、コピペの > の部分をイコールにする
	if ((((P1.X_UL >= P2.X_UL && P1.X_UL <= P2.X_UL + P2.xSize) ||
		(P2.X_UL >= P1.X_UL && P2.X_UL <= P1.X_UL + P1.xSize)) &&
		((P1.Y_UL >= P2.Y_UL && P1.Y_UL <= P2.Y_UL + P2.ySize) ||
		(P2.Y_UL >= P1.Y_UL && P2.Y_UL <= P1.Y_UL + P1.ySize))))
	{
		// ここからスタート
		// まずは離す距離を測ってみよう
		Distance = (double)((P1.xSize / 2.0) + (P2.xSize / 2.0));

		// この Distance を使って離す量を測る
		// 1Pが2Pより左 のとき
		if (P1.XPos < P2.XPos){
			DisPoint = Distance - (P2.XPos - P1.XPos);
			// 次に   2Pが1Pより左 のとき
		}
		else if (P1.XPos > P2.XPos){
			DisPoint = Distance - (P1.XPos - P2.XPos);
			// 最後に Xの距離が同じ とき
		}
		else if (P1.XPos == P2.XPos){
			DisPoint = Distance;
		}

		double a, a2;

		//------------
		// 高さが同じ
		//------------
		if (P1.YPos == P2.YPos){
			
			// 当たってたら 速度 をチェック
			// 1P のが速い
			if (P1.XVel > P2.XVel){
				if (P1.XPos <= P2.XPos){
					P1.XPos -= (DisPoint / 2), P2.XPos += (DisPoint / 2);
					a = P1.XPos + m_X[0] / 2;
					a2 = P2.XPos + m_X[1] / 2;
					if (a <= P1.XPos)P1.XPos = a;
					if (a2 >= P2.XPos)P2.XPos = a2;
				}
				else{
					P1.XPos += (DisPoint / 2), P2.XPos -= (DisPoint / 2);
					a = P1.XPos + m_X[0] / 2;
					a2 = P2.XPos + m_X[1] / 2;
					if (a >= P1.XPos)P1.XPos = a;
					if (a2 <= P2.XPos)P2.XPos = a2;
				}
			}
			// 2P
			else if (P1.XVel < P2.XVel){
				if (P2.XPos <= P1.XPos){
					P1.XPos += (DisPoint / 2), P2.XPos -= (DisPoint / 2);
					a = P1.XPos + m_X[0] / 2;
					a2 = P2.XPos + m_X[1] / 2;
					if (a >= P1.XPos)P1.XPos = a;
					if (a2 <= P2.XPos)P2.XPos = a2;
				}
				else{
					P1.XPos -= (DisPoint / 2), P2.XPos += (DisPoint / 2);
					a = P1.XPos + m_X[0] / 2;
					a2 = P2.XPos + m_X[1] / 2;
					if (a <= P1.XPos)P1.XPos = a;
					if (a2 >= P2.XPos)P2.XPos = a2;
				}
			}
			// 同じ速さ
			else if (P1.XVel == P2.XVel){
				if (P1.XPos <= P2.XPos){
					P1.XPos -= (DisPoint / 2), P2.XPos += (DisPoint / 2);
				}
				else{
					P1.XPos += (DisPoint / 2), P2.XPos -= (DisPoint / 2);
				}
			}

			
			// X座標が 同じ
			if (P1.XPos == P2.XPos){
				// どっちかが左端
				if ((P1.XPos <= GAMENHAJI) || (P2.XPos <= GAMENHAJI)){
					// 1Pが右向き
					if (P1.muki == 0){
						P2.XPos += DisPoint;
					}
					// 2Pが右向き
					else if (P2.muki == 0){
						P1.XPos += DisPoint;
					}
				}
				// どっちかが右端
				else if ((P1.XPos >= (STAGE_WIDTH - GAMENHAJI)) ||
					(P2.XPos >= (STAGE_WIDTH - GAMENHAJI))){

					// 1Pが右向き
					if (P1.muki == 1){
						P2.XPos -= DisPoint;
					}
					// 2Pが右向き
					else if (P2.muki == 1){
						P1.XPos -= DisPoint;
					}
				}
			}// X座標が同じ場合 

		}// 高さ同じの調整終了

		/**
		* 高さが違う場合
		**/
		else if (P1.YPos != P2.YPos){
			
			// 画面端
			// 1P 左端
			if (P1.XPos <= GAMENHAJI){
			P1.XPos = GAMENHAJI;
			}
			// 2P 左端
			else if (P2.XPos <= GAMENHAJI){
			P2.XPos = GAMENHAJI;
			}
			// 1P 右端
			if (P1.XPos >= (STAGE_WIDTH - GAMENHAJI)){
			P1.XPos = (STAGE_WIDTH - GAMENHAJI);
			}
			// 2P 右端
			else if (P2.XPos >= (STAGE_WIDTH - GAMENHAJI)){
			P2.XPos = (STAGE_WIDTH - GAMENHAJI);
			}
			

			// 1P 左
			if (P1.XPos < P2.XPos){
				P1.XPos -= (DisPoint / 2), P2.XPos += (DisPoint / 2);
			}
			// 1P 右
			if (P1.XPos > P2.XPos){
				P1.XPos += (DisPoint / 2), P2.XPos -= (DisPoint / 2);
			}

			// X座標が 同じ
			if (P1.XPos == P2.XPos){
				// どっちかが左端
				if ((P1.XPos <= GAMENHAJI) || (P2.XPos <= GAMENHAJI)){
					// 1Pが右向き
					if (P1.muki == 0){
						P2.XPos += DisPoint;
					}
					// 2Pが右向き
					else if (P2.muki == 0){
						P1.XPos += DisPoint;
					}
				}
				// どっちかが右端
				else if ((P1.XPos >= (STAGE_WIDTH - GAMENHAJI)) ||
					(P2.XPos >= (STAGE_WIDTH - GAMENHAJI))){

					// 1Pが右向き
					if (P1.muki == 1){
						P2.XPos -= DisPoint;
					}
					// 2Pが右向き
					else if (P2.muki == 1){
						P1.XPos -= DisPoint;
					}
				}
			}// X座標が同じ場合 
		}// 高さが違う場合 
	}//if終了

	// 測った値を送る
	Get_PSet(P1, P2);

	} // ほも終了
	// 終了
	return 0;
}


void PosHosei()
{
	// ほも
	if (S.roundState != 0){

	// 座標記録
	m_oldXPos[0] = P1.XPos, m_oldXPos[1] = P2.XPos;
	m_oldYPos[0] = P1.YPos, m_oldYPos[1] = P2.YPos;
	
	Player P[2];
	P[0] = P1, P[1] = P2;

	for (int i = 0; i < 2; i++){

		//X軸 端処理
		if (P[i].XPos <= (S.ScroolX + GAMENHAJI)){
			P[i].XPos = (double)S.ScroolX + GAMENHAJI;
		}
		else if (P[i].XPos >= (S.ScroolX + SCREEN_W - GAMENHAJI)){
			P[i].XPos = (double)S.ScroolX + (SCREEN_W - GAMENHAJI);
		}
	
	}
	// 戻ってくる
	P1 = P[0], P2 = P[1];

	
	TH_Set(P1, P2);
	//当たり判定チェック、コピペの > の部分をイコールにする
	if (((P1.X_UL >= P2.X_UL && P1.X_UL <= P2.X_UL + P2.xSize) ||
		(P2.X_UL >= P1.X_UL && P2.X_UL <= P1.X_UL + P1.xSize)) &&
		((P1.Y_UL >= P2.Y_UL && P1.Y_UL <= P2.Y_UL + P2.ySize) ||
		(P2.Y_UL >= P1.Y_UL && P2.Y_UL <= P1.Y_UL + P1.ySize)))
	{
		// 修正するなら
		{
			// 過去の座標から、ずらす量を計算
			xp[0] = P1.XPos - m_oldXPos[0];
			xp[1] = P2.XPos - m_oldXPos[1];
			yp[0] = P1.YPos - m_oldYPos[0];
			yp[1] = P2.YPos - m_oldYPos[1];

			// この Distance を使って離す量を測る
			// 1Pが2Pより左 のとき
			if (P1.XPos < P2.XPos){
				DisPoint = Distance - (P2.XPos - P1.XPos);
				// 次に   2Pが1Pより左 のとき
			}
			else if (P1.XPos > P2.XPos){
				DisPoint = Distance - (P1.XPos - P2.XPos);
				// 最後に Xの距離が同じ とき
			}
			else if (P1.XPos == P2.XPos){
				DisPoint = Distance;
			}

			//------------
			// 高さが同じ
			//------------
			if (P1.YPos == P2.YPos){

				// 1P 左端
				if (P1.XPos <= GAMENHAJI){
					P2.XPos += DisPoint;
				}
				// 1P 右端
				else if (P1.XPos >= (STAGE_WIDTH - GAMENHAJI)){
					P2.XPos -= DisPoint;
				}
				// 2P 左端
				if (P2.XPos <= GAMENHAJI){
					P1.XPos += DisPoint;
				}
				// 2P 右端
				else if (P2.XPos >= (STAGE_WIDTH - GAMENHAJI)){
					P1.XPos -= DisPoint;
				}

			}// 高さ同じの調整終了
			//------------------
			// 高さが違う場合
			//------------------
			else if (P1.YPos != P2.YPos){
				// 画面端
				// 1P 左端
				if (P1.XPos <= (double)GAMENHAJI){
					P1.XPos = (double)GAMENHAJI;
				}
				// 2P 左端
				else if (P2.XPos <= (double)GAMENHAJI){
					P2.XPos = (double)GAMENHAJI;
				}
				// 1P 右端
				if (P1.XPos >= (double)(STAGE_WIDTH - GAMENHAJI)){
					P1.XPos = (double)(STAGE_WIDTH - GAMENHAJI);
				}
				// 2P 右端
				else if (P2.XPos >= (double)(STAGE_WIDTH - GAMENHAJI)){
					P2.XPos = (double)(STAGE_WIDTH - GAMENHAJI);
				}
				/*
					// 1P下
					if (P1.YPos > P2.YPos){
						// 1Pが右向き
						if (P1.XPos <= GAMENHAJI){
							P2.XPos += DisPoint;
						}
						else if (P1.XPos >= (STAGE_WIDTH - GAMENHAJI)){
							P2.XPos -= DisPoint;
						}
					}
					// 2P下
					else if (P2.YPos > P1.YPos){
						// 1Pが左向き
						if (P2.XPos <= GAMENHAJI){
							P1.XPos += DisPoint;
						}
						else if (P2.XPos >= (STAGE_WIDTH - GAMENHAJI)){
							P1.XPos -= DisPoint;
						}
					}
					*/
			}// 高さが違う場合 
			
		}
	}
	

	// 測った値を送る
	Get_PSet(P1, P2);

	}	// roundstate 終了
}

void VelPosSet()
{
	// 向きを記憶
	// 向きが変わっても１フレだけは速度が変わらない
	OldMuki[0] = P1.muki;
	OldMuki[1] = P2.muki;

	// 移動前の座標記録
	m_oldXPos[0] = P1.XPos, m_oldXPos[1] = P2.XPos;
	m_oldYPos[0] = P1.YPos, m_oldYPos[1] = P2.YPos;

	Player P[2];
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){
		// 座標の位置に速度を足す
		// 1P
		if (P[i].StopTime == 0){

			if (P[i].muki == 0){
				if (P[i].muki == OldMuki[i]){
					P[i].XPos += P[i].XVel * 2;
					P[i].XPos += P[i].A.hbVelX * 2;
					P[i].XPos += P[i].D.addVelX * 2;
				}
				else { 
					P[i].XPos -= P[i].XVel * 2; 
					P[i].XPos -= P[i].A.hbVelX * 2;
					P[i].XPos -= P[i].D.addVelX * 2;
				}
			}
			// 右側
			else if(P[i].muki == 1){
				if (P[i].muki == OldMuki[i]){
					P[i].XPos -= P[i].XVel * 2;
					P[i].XPos -= P[i].A.hbVelX * 2;
					P[i].XPos -= P[i].D.addVelX * 2;
				}
				else { 
					P[i].XPos += P[i].XVel * 2; 
					P[i].XPos += P[i].A.hbVelX * 2;
					P[i].XPos += P[i].D.addVelX * 2;
				}
			}
			//==============
			// Y 速度調整
			//==============

			// 重力無視じゃない
			if (!P[i].ignoreG){

				// ガ－ドor喰らい以外
				if (P[i].stateno < 1000){
					//&& P[i].time > 0
					// 空中
					if (P[i].SFlg == 2){
						// ガード
						if (P[i].stateno >= 50 && P[i].stateno <= 59 && P[i].time > 0){
							P[i].YVel += GRAVITY;
						}
						// 通常時は個別の重力がかかる
						else{ 
							// 空ダしてない
							if (!P[i].airDashTime)
							P[i].YVel += P[i].C.yAccel; 
						}
					}
				}

				// 喰らい
				if (P[i].stateno >= 1000 && P[i].time > 0){
					if (P[i].SFlg == 2){
						// 受け身　壁バウンド等
						if (P[i].stateno == 1085){
							P[i].YVel += GRAVITY;
						}
						// 通常喰らい
						else{
							P[i].YVel += P[i].D.yaccel;
						}
					}
				}

			}// ignoreG終了

			// Y速度分 移動
			P[i].YPos += (P[i].YVel * 2);
			P[i].YPos += (P[i].A.hbVelY * 2);
		}


	}
	P1 = P[0], P2 = P[1];

	// 向きを記憶
	// 向きが変わっても１フレだけは速度が変わらない
	//OldMuki[0] = P1.muki;
	//OldMuki[1] = P2.muki;

	// 測った値を送る
	Get_PSSet(P1, P2, S);
}

void VelXYAdd()
{
	Player P[2];
	P[0] = P1, P[1] = P2;
	
	for (int i = 0; i < 2; i++){
		if (P[i].StopTime == 0){

			//==============
			// X 速度調整
			//==============
			
			// はやすぎる
			//if (P[i].XVel > 25)P[i].XVel = 25;
			//else if (P[i].XVel < -25)P[i].XVel = -25;
			P[i].XVel = max(-25, min(25, P[i].XVel));
			P[i].A.hbVelX = max(-25, min(25, P[i].A.hbVelX));
			if (P[i].SFlg != 2){
				if (P[i].XVel > 0){
					//P[i].XVel -= 0.5;
					P[i].XVel = P[i].XVel * 0.82;	// 0.82は摩擦
				}
				else if (P[i].XVel < 0){
					P[i].XVel = P[i].XVel * 0.82;
				}
				// 喰らいorガード途中に画面端に到達
				// テスト導入
				if (((P[i].XPos <= GAMENHAJI) || (P[i].XPos >= STAGE_WIDTH - GAMENHAJI)) && (P[i].XVel < 0) && 
					((P[i].stateno >= 50 && P[i].stateno < 60) || (P1.stateno == 1000)	// ガードor地上喰らい
					|| (P[i].stateno == 1010) || (P[i].stateno == 1020))){
					if (i == 0){
						//if(P[1].A.hbVelX != 0.0)
						P[1].A.hbVelX = P[i].XVel;
					}
					else{ 
						//if (P[].A.hbVelX != 0.0)
						P[0].A.hbVelX = P[i].XVel;
					}
					P[i].XVel = 0;
				}

				// Velが少ない場合、0にする(MUGENは1)
				if (P[i].XVel < 1 && P[i].XVel > -1)P[i].XVel = 0;	// やりすぎた場合
				
			}
			
			// ヒットバック
			if (P[i].A.hbVelX > 0){
				P[i].A.hbVelX = P[i].A.hbVelX * 0.82;	// 0.82は摩擦
			}
			else if (P[i].A.hbVelX < 0){
				P[i].A.hbVelX = P[i].A.hbVelX * 0.82;
			}
			if ((P[i].A.hbVelX < 1) && (P[i].A.hbVelX > -1))P[i].A.hbVelX = 0;	// やりすぎた場合

			// アドガバック
			if (P[i].D.addVelX > 0){
				P[i].D.addVelX = P[i].D.addVelX * 0.82;	// 0.82は摩擦
			}
			else if (P[i].D.addVelX < 0){
				P[i].D.addVelX = P[i].D.addVelX * 0.82;
			}
			if ((P[i].D.addVelX < 1) && (P[i].D.addVelX > -1))P[i].D.addVelX = 0;	// やりすぎた場合

			//==============
			// Y 速度調整
			//==============
			// ヒットバック
			if (P[i].A.hbVelY > 0){
				P[i].A.hbVelY = P[i].A.hbVelY * 0.82;	// 0.82は摩擦
			}
			else if (P[i].A.hbVelY < 0){
				P[i].A.hbVelY = P[i].A.hbVelY * 0.82;
			}
			if ((P[i].A.hbVelY < 1) && (P[i].A.hbVelY > -1))P[i].A.hbVelY = 0;	// やりすぎた場合

			// 重力をかけて、落下や着地をする
			// ignoreG があるときは、空中にいてもかからない
			/*
			// 重力無視じゃない
			if (!P[i].ignoreG){

				// ガ－ドor喰らい ではない
				if (P[i].stateno < 1000 && P[i].HFlg == 0){
					//&& P[i].time > 0
					// 1P 地面より下で着地ステートへ
					if (P[i].YPos > GROUND){
						P[i].YVel = 0, P[i].YPos = GROUND;
						P[i].stateno = 47, P[i].time = 0;
					}
					
					// 空中
					if (P[i].SFlg == 2){
						// 移動中は個別の重力がかかる
						P[i].YVel += P[i].C.yAccel;
					}
				}

				// ガードor喰らい
				if (P[i].stateno >= 1000 && P[i].time > 0){
					if (P[i].SFlg == 2){
						// 喰らい中は重さ共通
						P[i].YVel += GRAVITY;
					}
				}

			}// ignoreG終了
			*/
			// はやすぎる
			if (P[i].YVel > 18)P[i].YVel = 18;
			// 重力無視じゃない
			if (!P[i].ignoreG){

				// ガ－ドor喰らい ではない 速度が下
				if (P[i].stateno < 1000 && P[i].HFlg == 0 && P[i].YVel > 0){
					//&& P[i].time > 0
					// 1P 地面より下で着地ステートへ
					if (P[i].YPos > GROUND){
						P[i].YVel = 0, P[i].YPos = GROUND;
						P[i].stateno = 47, P[i].time = 0;
					}

				}

			}// ignoreG終了

		}// 処理終了
	}// ループ
	
	P1 = P[0], P2 = P[1];
	
	// 測った値を送る
	Get_PSet(P1, P2);
}

void GetP_SubSystem(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_SubSystem(System_t GS)
{
	S = GS;
}
