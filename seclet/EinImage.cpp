//==================================================================================
//
//			数値計算の管理部
//
//==================================================================================

//インクルード部--------------------------------------------
#include "pch.h"

//デファイン部----------------------------------------------

//ﾃﾞｰﾀ定義部------------------------------------------------



//内部関数宣言部--------------------------------------------

// #include "Setting.h" にてセット

// 縦番号を決めてから、これに送る
static int m_row, m_column;
// 送る画像の縦番号
static int i;
// 送る画像の横番号（画像ファイルの番号）　未使用
static int r;

//プログラム部----------------------------------------------


//データ操作系関数

//-------------------------------------------------------------------------------
//	定義　int EinImage
//
//	説明　ここで画像も準備したほうがいいかも、ステート・時間　
//  に合わせて呼ぶ画像を決める（横番号・縦番号）　のほうがいいかも
//
//	引数　
//
//	戻り値　int 画像の縦番号
//-------------------------------------------------------------------------------



int EinImage(int Stateno, int time)
{
	// 横
	// 基本はステートと同じ
	r = Stateno;	
	// 縦、これに i を入れる
	m_column = 0;	


	// P1 ステート確認
	switch (Stateno)
	{
	case 0:	//立ち	54 フレーム
		// 二重スイッチ 時間確認
		if (time >= 0){	
			i = 0;	//15
		}
		if (time >= 15){
			i = 1;	// 7
		}
		if (time >= 22){
			i = 2;	// 6
		}
		if (time >= 28){
			i = 3;	// 9
		}
		if (time >= 37){
			i = 4;	// 3
		}
		if (time >= 40){
			i = 5;	// 6
		}
		if (time >= 46){
			i = 6;	// 8
		}
	
		break;

	case 5:	// 立ち振り向き	8 フレ
		if (time >= 0){
			i = 0;	// 4
		}
		if (time >= 4){
			i = 1;	// 4
		}
		break;

	case 6:	// しゃがみ振り向き	8 フレ
		if(time >= 0){
			r = 5;
			i = 2;	// 4
		}
		if (time >= 4){
			i = 3;	// 4
		}
		break;

	case 10:	//立ち→しゃがみ	11F
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 5){
			i = 1;	// 6
		}
		break;


	case 11:	// しゃがみ	0～F
		if (time >= 0){
			i = 0;	//1～
		}
		break;


	case 12:	//しゃがみ→立ち	8
		if (time >= 0){
			i = 1;	// 3
		}
		if (time >= 3){
			i = 0;	// 5
		}
		break;


	case 20:	// 前歩き	47
		if (time >= 0){
			i = 0;	// 8
		}
		if (time >= 8){
			i = 1;	// 5
		}
		if (time >= 13){
			i = 2;	// 4
		}
		if (time >= 17){
			i = 3;	// 5
		}
		if (time >= 22){
			i = 4;	// 9
		}
		if (time >= 31){
			i = 5;	// 5
		}
		if (time >= 36){
			i = 6;	// 4
		}
		if (time >= 40){
			i = 7;	// 7
		}
		break;

	case 21:	// 後ろ歩き		48
		if (time >= 0){
			i = 0;	// 11
		}
		if (time >= 11){
			i = 1;	// 9
		}
		if (time >= 20){
			i = 2;	// 7
		}
		if (time >= 27){
			i = 3;	// 5
		}
		if (time >= 32){
			i = 4;	// 7
		}
		if (time >= 39){
			i = 5;	// 9
		}

		break;


	case 30:	//前ダッシュ	42
		if (time >= 0){
			i = 0;	// 3
		}
		if (time >= 3){
			i = 1;	// 5
		}
		if (time >= 8){
			i = 2;	// 4
		}
		if (time >= 12){
			i = 3;	// 5
		}
		if (time >= 17){
			i = 4;	// 7
		}
		if (time >= 24){
			i = 3;	// 5
		}
		if (time >= 29){
			i = 2;	// 4
		}
		if (time >= 33){
			i = 1;	// 4
		}
		if (time >= 37){
			i = 0;	// 5
		}
		break;


	case 35:	// バックステップ	9～
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 5){
			i = 1;	// 4～
		}
		break;

	case 40:	// ジャンプ開始		3
		if (time >= 0){
			i = 0;	// 3
		}
		break;

	case 41:	// 垂直ジャンプ		27～(24～ループ
		if (time >= 0){
			i = 0;	// 9
		}
		if (time >= 9){
			i = 1;	// 3
		}
		if (time >= 12){
			i = 2;	// 5
		}
		if (time >= 17){
			i = 3;	// 4
		}
		if (time >= 21){
			i = 4;	// 3
		}
		if (time >= 24){
			i = 5;	// 3
		}

		if (time >= 27){
			i = 4;	// 3
		}
		if (time >= 30){
			i = 5;	// 3
		}
		break;

	case 42:	// 前ジャンプ
		if (time >= 0){
			i = 0;	// 9
		}
		if (time >= 9){
			i = 1;	// 3
		}
		if (time >= 12){
			i = 2;	// 5
		}
		if (time >= 17){
			i = 3;	// 4
		}
		if (time >= 21){
			i = 4;	// 3
		}
		if (time >= 24){
			i = 5;	// 3
		}

		if (time >= 27){
			i = 4;	// 3
		}
		if (time >= 30){
			i = 5;	// 3
		}
		break;

	case 43:	// 後ろジャンプ		23～(18～ループ)
		if (time >= 0){
			i = 0;	// 18
		}
		if (time >= 18){
			i = 1;	// 4
		}
		if (time >= 22){
			i = 2;	// 4
		}

		if (time >= 26){
			i = 1;	// 4
		}
		if (time >= 30){
			i = 2;	// 4
		}

		break;

	case 45:	// 空中ジャンプ		2
		if (time >= 0){
			i = 0;	// 9
		}
		if (time >= 9){
			i = 1;	// 3
		}
		break;

	case 46:	// ジャンプ状態		6～
		if (time >= 0){
			r = 41;
			i = 4;	// 9
		}
		if (time >= 3){
			r = 41;
			i = 5;	// 3
		}
		break;

	case 47:	// 着地		2
		if (time >= 0){
			i = 0;	// 2
		}
		break;

	case 50:	// ガード(立)	4
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		break;
	case 51:	// ガード(立)	1～
		if (time >= 0){
			i = 1;	// 2
		}
		break;
	case 52:	// 解除(立)	4
		if (time >= 0){
			i = 1;	// 2
		}
		if (time >= 2){
			i = 0;	// 2
		}
		break;
	case 53:	// ガード(屈)	4
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		break;
	case 54:	// ガード(屈)	1
		if (time >= 0){
			i = 1;	// 2
		}
		break;
	case 55:	// 解除(屈)	4
		if (time >= 0){
			i = 1;	// 2
		}
		if (time >= 2){
			i = 0;	// 2
		}
		break;
	case 56:	// ガード(空)	4
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		break;
	case 57:	// ガード(空)	1
		if (time >= 0){
			i = 1;	// 2
		}
		break;
	case 58:	// 解除(空)	4
		if (time >= 0){
			i = 1;	// 2
		}
		if (time >= 2){
			i = 0;	// 2
		}
		break;
	case 95:	//勝利	87 フレーム
		// 二重スイッチ 時間確認
		if (time >= 0){
			i = 0;	// 4
		}
		if (time >= 4){
			i = 1;	//14
		}
		if (time >= 18){
			i = 2;	// 7
		}
		if (time >= 25){
			i = 3;	// 5
		}
		if (time >= 30){
			i = 5;	// 5
		}
		if (time >= 35){
			i = 6;	// 46
		}
		if (time >= 81){
			i = 5;	// 2
		}
		if (time >= 83){
			i = 4;	// 2
		}
		if (time >= 85){
			i = 7;	// 2
		}
		if (time >= 87){
			i = 8;	// 2
		}
	
		break;

	case 200:	// 立ちA	14
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		if (time >= 4){
			i = 2;	// 3
		}
		if (time >= 7){
			i = 3;	// 3
		}
		if (time >= 10){
			i = 1;	// 1
		}
		if (time >= 11){
			i = 0;	// 2
		}
		break;

	case 210:	// 立ちB	25
		if (time >= 0){
			i = 0;	// 1
		}
		if (time >= 1){
			i = 1;	// 2
		}
		if (time >= 3){
			i = 2;	// 2
		}
		if (time >= 5){
			i = 3;	// 2
		}
		if (time >= 7){
			i = 4;	// 2
		}
		if (time >= 9){
			i = 5;	// 5
		}
		if (time >= 14){
			i = 6;	// 3
		}
		if (time >= 17){
			i = 7;	// 3
		}
		if (time >= 20){
			i = 8;	// 2
		}
		if (time >= 22){
			i = 9;	// 2
		}
		if (time >= 24){
			i = 0;	// 1
		}
		break;
	case 220:	// 立ちC	51
		if (time >= 0){
			i = 0;	// 1
		}
		if (time >= 1){
			i = 1;	// 7
		}
		if (time >= 8){
			i = 2;	// 6
		}
		if (time >= 14){
			i = 3;	// 4
		}
		if (time >= 18){
			i = 4;	// 4
		}
		if (time >= 22){
			i = 5;	// 2
		}
		if (time >= 24){
			i = 6;	// 2
		}
		if (time >= 26){
			i = 7;	// 2
		}
		if (time >= 28){
			i = 8;	// 2
		}
		if (time >= 30){
			i = 9;	// 9
		}
		if (time >= 39){
			i = 6;	// 7
		}
		if (time >= 46){
			i = 10;	// 2
		}
		if (time >= 48){
			i = 0;	// 2
		}
		break;
	case 300:	// しゃがみA	14
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 3
		}
		if (time >= 5){
			i = 2;	// 3
		}
		if (time >= 8){
			i = 3;	// 3
		}
		if (time >= 11){
			i = 1;	// 2
		}
		if (time >= 13){
			i = 0;	// 1
		}
		break;

	case 310:	// しゃがみB	28
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 4
		}
		if (time >= 6){
			i = 2;	// 3
		}
		if (time >= 9){
			i = 3;	// 3
		}
		if (time >= 12){
			i = 4;	// 4
		}
		if (time >= 16){
			i = 5;	// 6
		}
		if (time >= 22){
			i = 6;	// 4
		}
		if (time >= 26){
			i = 0;	// 2
		}
		break;
	case 320:	// しゃがみC	33
		if (time >= 0){
			i = 0;	// 4
		}
		if (time >= 4){
			i = 1;	// 3
		}
		if (time >= 7){
			i = 2;	// 2
		}
		if (time >= 9){
			i = 3;	// 4
		}
		if (time >= 13){
			i = 4;	// 5
		}
		if (time >= 18){
			i = 5;	// 3
		}
		if (time >= 21){
			i = 6;	// 5
		}
		if (time >= 26){
			i = 1;	// 4
		}
		if (time >= 30){
			i = 0;	// 3
		}
		break;
	case 400:	// ジャンプA	19
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 3
		}
		if (time >= 5){
			i = 2;	// 10
		}
		if (time >= 14){
			i = 3;	// 2
		}
		if (time >= 16){
			i = 1;	// 2
		}
		if (time >= 18){
			i = 0;	// 1
		}
		break;

	case 410:	// ジャンプB	27
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 3
		}
		if (time >= 5){
			i = 2;	// 2
		}
		if (time >= 7){
			i = 3;	// 2
		}
		if (time >= 9){
			i = 4;	// 7
		}
		if (time >= 16){
			i = 5;	// 4
		}
		if (time >= 20){
			i = 6;	// 6
		}
		if (time >= 26){
			i = 0;	// 1
		}
		break;

	case 420:	// ジャンプC	22
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 4
		}
		if (time >= 6){
			i = 2;	// 2
		}
		if (time >= 8){
			i = 3;	// 2
		}
		if (time >= 10){
			i = 4;	// 6
		}
		if (time >= 16){
			i = 5;	// 5
		}
		if (time >= 21){
			i = 0;	// 1
		}
		break;

	case 500:	// 投げ		32
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 6
		}
		if (time >= 8){
			i = 2;	// 14
		}
		if (time >= 22){
			i = 3;	// 8
		}
		if (time >= 30){
			i = 0;	// 2
		}
		break;
	case 505:	// 投げ		24
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 3
		}
		if (time >= 5){
			i = 2;	// 3
		}
		if (time >= 8){
			i = 3;	// 1
		}
		if (time >= 9){
			i = 4;	// 1
		}
		if (time >= 10){
			i = 5;	// 9
		}
		if (time >= 19){
			i = 2;	// 4
		}
		break;

	case 506:	// 投げ、打撃部分	46
		if (time >= 0){
			i = 1;	// 3
		}
		if (time >= 3){
			i = 0;	// 10
		}
		if (time >= 12){
			i = 1;	// 2
		}
		if (time >= 14){
			i = 2;	// 2
		}
		if (time >= 16){
			i = 3;	// 2
		}
		if (time >= 18){
			i = 4;	// 2
		}
		if (time >= 20){
			i = 5;	// 13
		}
		if (time >= 33){
			i = 6;	// 3
		}
		if (time >= 36){
			i = 7;	// 6
		}
		if (time >= 42){
			i = 8;	// 1
		}
		break;

	case 510:	// 空中投げ		22～
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 6
		}
		if (time >= 8){
			i = 2;	// 14
		}
		break;

	case 515:	// 空中投げヒット	33～
		if (time >= 0){
			i = 0;	// 3
		}
		if (time >= 3){
			i = 1;	// 11
		}
		if (time >= 14){
			i = 2;	// 3
		}
		if (time >= 17){
			i = 3;	// 3
		}
		if (time >= 20){
			i = 4;	// 6
		}
		if (time >= 26){
			i = 5;	// 6
		}
		if (time >= 32){
			i = 0;	// 1～
		}
		break;

	case 600:	// ブメ投げ (A)	45
		if (time >= 0){
			i = 0;	// 11
		}
		if (time >= 11){
			i = 1;	// 2
		}
		if (time >= 13){
			i = 2;	// 4
		}
		if (time >= 17){
			i = 3;	// 10
		}
		if (time >= 27){
			i = 4;	// 6
		}
		if (time >= 33){
			i = 5;	// 7
		}
		if (time >= 40){
			i = 6;	// 7
		}
		if (time >= 47){
			i = 7;	// 1
		}

		break;

	case 605:	// 空中ブメ投げ	45
		if (time >= 0){
			i = 0;	// 11
		}
		if (time >= 11){
			i = 1;	// 2
		}
		if (time >= 13){
			i = 2;	// 3
		}
		if (time >= 16){
			i = 3;	// 10
		}

		break;

	case 606:	//着地硬直	6F
		if (time >= 0){
			r = 10;
			i = 1;	// 6
		}
		if (time >= 2){
			r = 11;
			i = 0;	// 5
		}
		if (time >= 10){
			r = 10;
			i = 1;	// 6
		}
		if (time >= 14){
			r = 10;
			i = 0;	// 6
		}
		break;

	case 610:	// 凪払い (A)	42
		if (time >= 0){
			i = 0;	// 8
		}
		if (time >= 8){
			i = 1;	// 3
		}
		if (time >= 11){
			i = 2;	// 2
		}
		if (time >= 13){
			i = 3;	// 2
		}
		if (time >= 15){
			i = 4;	// 1
		}
		if (time >= 16){
			i = 5;	// 12
		}
		if (time >= 28){
			i = 6;	// 6
		}
		if (time >= 34){
			i = 7;	// 6
		}
		if (time >= 40){
			i = 8;	// 2
		}
		break;
	case 611:	// 凪払い (B)	45
		if (time >= 0){
			i = 0;	// 9
		}
		if (time >= 9){
			i = 1;	// 3
		}
		if (time >= 12){
			i = 2;	// 2
		}
		if (time >= 14){
			i = 3;	// 2
		}
		if (time >= 16){
			i = 4;	// 1
		}
		if (time >= 17){
			i = 5;	// 12
		}
		if (time >= 29){
			i = 6;	// 6
		}
		if (time >= 35){
			i = 7;	// 6
		}
		if (time >= 41){
			i = 8;	// 2
		}
		break;

	case 620:	// ブメ設置		36
		if (time >= 0){
			i = 0;	// 7
		}
		if (time >= 7){
			i = 1;	// 3
		}
		if (time >= 10){
			i = 2;	// 3
		}
		if (time >= 13){
			i = 3;	// 5
		}
		if (time >= 18){
			i = 4;	// 5
		}
		if (time >= 23){
			i = 5;	// 6
		}
		if (time >= 29){
			i = 6;	// 6
		}
		if (time >= 35){
			i = 7;	// 1
		}

		break;

	case 622:	// 空中設置	45
		if (time >= 0){
			i = 0;	// 11
			r = 605;
		}
		if (time >= 11){
			i = 1;	// 2
		}
		if (time >= 13){
			i = 2;	// 3
		}
		if (time >= 16){
			i = 3;	// 10
		}

		break;

	case 625:	// ブメ発射		19
		if (time >= 0){
			i = 0;	// 1
			r = 200;
		}
		if (time >= 1){
			i = 0;	// 3
			r = 625;
		}
		if (time >= 4){
			i = 1;	// 10
			r = 625;
		}
		if (time >= 14){
			i = 0;	// 3
			r = 625;
		}
		if (time >= 17){
			i = 0;	// 2
			r = 200;
		}
		break;

	case 626:	// 空中発射		15～
		if (time >= 0){
			i = 0;	// 1
			r = 400;
		}
		if (time >= 1){
			i = 2;	// 3
			r = 625;	// ブメ発射
		}
		if (time >= 4){
			i = 3;	// 9
			r = 625;
		}
		if (time >= 13){
			i = 2;	// 1
			r = 625;
		}
		if (time >= 14){
			i = 0;	// 1～
			r = 400;
		}
		break;

	case 700:	// 大ブメ		58
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		if (time >= 4){
			i = 2;	// 4
		}
		if (time >= 8){
			r = 600;
			i = 2;	// 5
		}
		if (time >= 13){
			r = 600;
			i = 3;	// 25
		}
		if (time >= 38){
			r = 600;
			i = 2;	// 7
		}
		if (time >= 45){
			i = 2;	// 6
		}
		if (time >= 51){
			r = 210;
			i = 6;	// 2
		}
		if (time >= 53){
			r = 210;
			i = 7;	// 3
		}
		if (time >= 56){
			r = 610;
			i = 6;	// 2
		}

		break;

	case 800:	// 二段切り	48
		if (time >= 0){
			i = 0;	// 10
		}
		if (time >= 10){
			i = 1;	// 3
		}
		if (time >= 13){
			i = 2;	// 23
		}
		if (time >= 29){
			i = 3;	// 10
		}
		if (time >= 39){
			i = 4;	// 9
		}
		break;

	case 801:	// 二段切り2	113
		if (time >= 0){
			i = 0;	// 10
		}


		if (time >= 10){	// ループ
			i = 1;	// 2
		}
		if (time >= 12){
			i = 2;	// 2
		}
		if (time >= 14){
			i = 3;	// 2
		}
		if (time >= 16){
			i = 4;	// 2
		}
		if (time >= 18){
			i = 1;	// 2
		}
		if (time >= 20){
			i = 2;	// 2
		}
		if (time >= 22){
			i = 3;	// 2
		}
		if (time >= 24){
			i = 4;	// 2
		}
		if (time >= 26){
			i = 1;	// 2
		}
		if (time >= 28){	// ループ終了
			i = 2;	// 2
		}

		if (time >= 30){
			i = 5;	// 25
		}
		if (time >= 55){
			i = 6;	// 3
		}
		if (time >= 58){
			i = 7;	// 2
		}
		if (time >= 60){
			i = 8;	// 2
		}
		if (time >= 62){
			i = 9;	// 2
		}
		if (time >= 64){
			i = 10;	// 2
		}
		if (time >= 66){
			i = 11;	// 30
		}
		if (time >= 96){
			i = 12;	// 6
		}
		if (time >= 102){
			i = 13;	// 5
		}
		if (time >= 107){
			i = 14;	// 3
		}
		if (time >= 110){	// 終了
			i = 15;	// 2
		}
		break;

	case 820:	// 真空ブーメラン	34
		if (time >= 0){
			i = 0;	// 7
			r = 820;
		}
		if (time >= 7){
			i = 0;	// 3
			r = 625;
		}
		if (time >= 10){
			i = 1;	// 18
			r = 625;
		}
		if (time >= 28){
			i = 0;	// 4
			r = 625;
		}
		if (time >= 32){
			i = 0;	// 2
			r = 200;
		}
		break;


	case 1000:	// 上段やられ	1～
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 4){
			i = 1;	// 1～
		}
		break;

	case 1005:	// 上段戻り		1～
		if (time >= 0){
			i = 0;	// 1～
		}
		break;
	case 1010:	// 下段やられ	1～
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 4){
			i = 1;	// 1～
		}
		break;

	case 1015:	// 段戻り		1～
		if (time >= 0){
			i = 0;	// 1～
		}
		break;

	case 1020:	// しゃがみやられ	1～
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 4){
			i = 1;	// 1～
		}
		break;

	case 1025:	// しゃがみ戻り		1～
		if (time >= 0){
			i = 0;	// 1～
		}
		break;
	case 1030:	// 空中やられ	11～
		if (time >= 0){
			i = 0;	// 8
		}
		if (time >= 7){
			i = 1;	// 4～
		}
		if (time >= 11){
			i = 2;	// 1～
		}
		break;

	case 1035:	// 空中戻り		1～
		if (time >= 0){
			i = 2;	// 1～
		}
		break;

	case 1040:	// 床バウンド	1～
		if (time >= 0){
			r = 1030;
			i = 2;	// 1～
		}
		break;

	case 1050:	// ダウン		4～
		if (time >= 0){
			i = 0;	// 3
		}
		if (time >= 3){
			i = 1;	// 1～
		}
		break;

	case 1055:	// 起き上がり	28
		if (time >= 0){
			i = 0;	// 6
		}
		if (time >= 6){
			i = 1;	// 9
		}
		if (time >= 15){
			i = 2;	// 5
		}
		if (time >= 20){
			i = 3;	// 4
		}
		if (time >= 24){
			i = 4;	// 4
		}
		break;

	case 1060:	// 上段やられ	1～
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 4){
			i = 1;	// 1～
		}
		break;

	case 1065:	// 空中やられ	11～
		if (time >= 0){
			r = 1030;
			i = 0;	// 8
		}
		
		if (time >= 7){
			i = 1;	// 4～
		}
		if (time >= 11){
			i = 2;	// 1～
		}
		
		break;


	case 1070:	// 敗北		1～
		if (time >= 0){
			i = 0;	// 1
		}
		break;

	case 1085:	// 空中受け身		27～(24～ループ
		if (time >= 0){
			r = 41;
			i = 0;	// 9
		}
		if (time >= 9){
			i = 1;	// 3
		}
		if (time >= 12){
			i = 2;	// 5
		}
		if (time >= 17){
			i = 3;	// 4
		}
		if (time >= 21){
			i = 4;	// 3
		}
		if (time >= 24){
			i = 5;	// 3
		}

		if (time >= 27){
			i = 4;	// 3
		}
		if (time >= 30){
			i = 5;	// 3
		}
		break;

	case 2000:	// 大ブメ置き	1～
		if (time >= 0){
			i = 0;	// 1～
		}
		break;

	case 2010:	// 小ブメ	24～
		if (time >= 0){
			i = 0;	// 3～
		}
		if (time >= 3){
			i = 1;	// 3～
		}
		if (time >= 6){
			i = 2;	// 3～
		}
		if (time >= 9){
			i = 3;	// 3～
		}
		if (time >= 12){
			i = 4;	// 3～
		}
		if (time >= 15){
			i = 5;	// 3～
		}
		if (time >= 18){
			i = 6;	// 3～
		}
		if (time >= 21){
			i = 7;	// 3～
		}

		break;

	case 2020:	// 設置	24～
		if (time >= 0){
			i = 0;	// 3～
		}
		if (time >= 3){
			i = 1;	// 3～
		}
		if (time >= 6){
			i = 2;	// 3～
		}
		if (time >= 9){
			i = 3;	// 3～
		}
		if (time >= 12){
			i = 4;	// 3～
		}
		if (time >= 15){
			i = 5;	// 3～
		}
		if (time >= 18){
			i = 6;	// 3～
		}
		if (time >= 21){
			i = 7;	// 3～
		}
		r = 2010;	// 小ブメ

		break;

	case 2025:	// 設置	24～
		if (time >= 0){
			i = 0;	// 3～
		}
		if (time >= 3){
			i = 1;	// 3～
		}
		if (time >= 6){
			i = 2;	// 3～
		}
		if (time >= 9){
			i = 3;	// 3～
		}
		if (time >= 12){
			i = 4;	// 3～
		}
		if (time >= 15){
			i = 5;	// 3～
		}
		if (time >= 18){
			i = 6;	// 3～
		}
		if (time >= 21){
			i = 7;	// 3～
		}
		r = 2010;	// 小ブメ

		break;

	case 2030:	// 大ブメ	32～
		if (time >= 0){
			i = 0;	// 4～
		}
		if (time >= 4){
			i = 1;	// 4～
		}
		if (time >= 8){
			i = 2;	// 4～
		}
		if (time >= 12){
			i = 3;	// 4～
		}
		if (time >= 16){
			i = 4;	// 4～
		}
		if (time >= 20){
			i = 5;	// 4～
		}
		if (time >= 24){
			i = 6;	// 4～
		}
		if (time >= 28){
			i = 7;	// 4～
		}

		break;

	case 2100:	// 大ブメ	32～
		if (time >= 0){
			i = 0;	// 3～
		}
		if (time >= 3){
			i = 1;	// 3～
		}
		if (time >= 6){
			i = 2;	// 3～
		}
		if (time >= 9){
			i = 3;	// 3～
		}
		if (time >= 12){
			i = 4;	// 3～
		}

		break;

	default: 
		if (time >= 0){
			i = 0;	// 1
		}
		break;

		break;	// i の準備終了

	} // 画像セット終わり

	// ステと同じじゃない場合、r を横に
	if (m_row != r){
		m_row = r;
	}
	// i を縦にセット
	m_column = i;
	Get_N12(m_row, m_column);
	return 0;


}// 終了

