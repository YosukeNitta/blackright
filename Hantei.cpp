//==================================================================================
//
//			数値計算の管理部
//
//========================================================================

//インクルード部--------------------------------------------
#include "pch.h"

//デファイン部----------------------------------------------



//ﾃﾞｰﾀ定義部------------------------------------------------


//内部関数宣言部--------------------------------------------



//プログラム部----------------------------------------------


void KuraiSet(int i, int W, int H, int X, int Y)
{
	// 喰らい判定テスト
	P1.WHit[i] = W, P1.HHit[i] = H;
	if (P1.muki == 0){
		P1.XHit[i] = (int)P1.XPos + X;
	}
	else if (P1.muki == 1){
		P1.XHit[i] = (int)P1.XPos - X - W;
	}
	P1.YHit[i] = (int)P1.YPos + Y;
}

void KuraiReset(){
	for (int i = 0; i < HIT_MAX; i++){
		P1.XHit[i] = -100, P1.YHit[i] = -100;
		P1.WHit[i] = 0, P1.HHit[i] = 0;
	}

}

//ダメージセット
void AttSet(int i, int W, int H, int X, int Y)
{
	int homo = 0;
	// [攻撃判定セット]
	if (W < 0){
		W = -W;
		homo = 1;
	}
	
	P1.WAtt[i] = W, P1.HAtt[i] = H;
	if (P1.muki == 0){
		P1.XAtt[i] = (int)P1.XPos + X;
	}
	else{
		// マイナスの時だけ WAtt[0] を引く
		P1.XAtt[i] = (int)P1.XPos - X - W;
	}
	// 向きを変えるのは X 方向だけ！
	P1.YAtt[i] = (int)P1.YPos + Y;

	/*
	if (homo == 1){
		if (P1.muki == 0){
			P1.XAtt[i] -= P1.WAtt[i];
		}
		else{
			// マイナスの時だけ WAtt[0] を引く
			P1.XAtt[i] += P1.WAtt[i];
		}
	}
	*/

}

void AttReset()
{
	for (int i = 0; i < ATT_MAX; i++)
	{
		P1.XAtt[i] = -50, P1.YAtt[i] = -50;
		P1.WAtt[i] = 0, P1.HAtt[i] = 0;
	}

}


//====================
// ヘルパー判定
//====================
void H_KuraiSet(int n, int i, int W, int H, int X, int Y)
{
	// 喰らい判定テスト
	H1[n].WHit[i] = W, H1[n].HHit[i] = H;
	if (H1[n].muki == 0){
		H1[n].XHit[i] = (int)H1[n].XPos + X;
	}
	else if (H1[n].muki == 1){
		H1[n].XHit[i] = (int)H1[n].XPos - X - W;
	}
	H1[n].YHit[i] = (int)H1[n].YPos + Y;
}

void H_KuraiReset(){
	for (int n = 0; n < HELPER_MAX; n++){
		for (int i = 0; i < HIT_MAX; i++){
			H1[n].XHit[i] = -50, H1[n].YHit[i] = -50;
			H1[n].WHit[i] = 0, H1[n].HHit[i] = 0;
		}
	}
}


//ダメージセット
void H_AttSet(int n, int i, int W, int H, int X, int Y)
{
	// [攻撃判定セット]
	H1[n].WAtt[i] = W, H1[n].HAtt[i] = H;
	if (H1[n].muki == 0){
		H1[n].XAtt[i] = (int)H1[n].XPos + X;
	}
	else{
		// マイナスの時だけ WAtt[0] を引く
		H1[n].XAtt[i] = (int)H1[n].XPos - X - W;
	}
	// 向きを変えるのは X 方向だけ！
	H1[n].YAtt[i] = (int)H1[n].YPos + Y;

}

void H_AttReset()
{
	for (int n = 0; n < HELPER_MAX; n++){
		for (int i = 0; i < HIT_MAX; i++)
		{
			H1[n].XAtt[i] = -150, H1[n].YAtt[i] = -150;
			H1[n].WAtt[i] = 0, H1[n].HAtt[i] = 0;
		}
	}
}


// 値を返す
Player ReturnPSet()
{
	return P1;
}

Helper ReturnHSet(int i)
{
	return H1[i];
}

// 順番を逆にする
void GetPH_Hantei(Player GP1)
{
	P1 = GP1;
}

void GetS_Hantei(System_t GS)
{
	S = GS;
}

void GetH_Hantei(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}

void GetH1_Hantei(Helper GH1[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
	}

}
