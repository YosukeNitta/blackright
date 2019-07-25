//==================================================================================
//
//			キャラクターの管理部
//
//========================================================================

//インクルード部--------------------------------------------
#include "pch.h"

//デファイン部----------------------------------------------

// #include "Define.h" にて、関数をセット
static int N1, N2;
static int pict[2][PICT_MAX][IMAGE2_MAX];

//ﾃﾞｰﾀ定義部------------------------------------------------



//内部関数宣言部--------------------------------------------

// #include "Setting.h" にてセット


//プログラム部----------------------------------------------


//対戦の計算メイン関数

//-------------------------------------------------------------------------------
//	定義　int CharMove( void )
//
//	説明　バトルモードの1P, 2Pの値を設定する、このモードの要
//
//	引数　int (可変)  引数の値で次の画面が決まる
//
//	戻り値　int メニューを決める変数
//-------------------------------------------------------------------------------



// 画像番号
static int CharGraph;


// キャラ固有の数値をセットする
int Player_State()
{
	/*********
	* 能力設定 
	*********/
	// 読み込み
	if (P1.Name > 0)load_status(P1);

	// ステートに数値を送る
	if (P1.PSide == 1)Get_P1Set(P1);
	if (P1.PSide == 2)Get_P2Set(P1);
		
	return 0;
}


// ヘルパーに画像をもってくる
int Helper_Graph()
{
	{
		for (int i = 0; i < HM; i++){
			if (H1[i].var){
				// N1, N2 を取得
				// H1[i].HImage = P1.GraphImage[H1[i].aNum1][H1[i].aNum2];
				H1[i].Image = pict[P1.PSide - 1][H1[i].aNum1][H1[i].aNum2];

				// 画像サイズ取得 P1,2
				GetGraphSize(H1[i].Image, &H1[i].HGraphW, &H1[i].HGraphH);

				// 時間計測
				//H1[i].time += 1;
				//H1[i].HAnimTime += 1;			
			}
		}
	}

	// P1とP2のどっちに送るか判断
	if (P1.PSide == 1)Get_HSet(H1, H2);
	if (P1.PSide == 2)Get_HSet(H2, H1);

	// 終了
	return 0;

}

void Player_GraphLoad()
{
	// 代入用
	int i, j;

	// Playerの数値を渡し、パレットと画像とPICを読み込む
	GetP1_Pict(P1);	// プレイヤーデータを渡す
	PalletSet();
	GetPict(P1.PSide);	// ok
	for (i = 0; i < PICT_MAX; i++)
	{
		for (j = 0; j < IMAGE2_MAX; j++)
		{
			// キャラの画像を登録
			pict[P1.PSide - 1][i][j] = GraphSet(i, j, P1.PSide);
		}
	}
	
	
	//ここまで

	// ステートに数値を送る
	if (P1.PSide == 1)Get_P1Set(P1);
	if (P1.PSide == 2)Get_P2Set(P1);
	// ロード終了
}

int Player_ImageGet(Player GP1)
{
	int img = pict[GP1.PSide - 1][GP1.aNum1][GP1.aNum2];

	return img;
}

void GetP1_Player(Player GP1)
{
	P1 = GP1;
}

void GetS_Player(System_t GS)
{
	S = GS;
}


void Get_N12(int N_1, int N_2)
{
	N1 = N_1;
	N2 = N_2;
}

void GetH_Player(Helper GH1[], Helper GH2[])
{
	int i;
	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}
}

