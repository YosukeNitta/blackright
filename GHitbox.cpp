//==================================================================================
//
//			数値計算の管理部
//
//========================================================================

//インクルード部--------------------------------------------
#include "pch.h"
#include "vector"

using namespace std;

//デファイン部----------------------------------------------


//ﾃﾞｰﾀ定義部------------------------------------------------
static Air air[2][STATE_MAX];

//static Air air2[CHARACTERS_MAX][STATE_MAX];	// 保存用
static vector<vector<Air>>air2;	// これに全てのキャラの判定

static Pict pic[2][PICT_MAX][IMAGE2_MAX];

typedef struct{
	int attFlg;	// 命令で判定をつける
	int attSide;	// プレイヤーの方向
	int i, w, h, x, y;
}ExSet;

ExSet ex;

static vector<Name_t>N;

//内部関数宣言部--------------------------------------------


//プログラム部----------------------------------------------




void GHitbox(){


	// 最初に判定初期化
	GetPH_Hantei(P1);
	KuraiReset();
	AttReset();
	P1 = ReturnPSet();

	GetPH_Hantei(P1);	// 数値を判定に渡す


	//=================
	// boxセッティング
	//=================
	int num = 0;
	int bTime = 0;
	int side = P1.PSide - 1;

	// ANumが存在するならスタート
	if (air[side][P1.stateno].ANum > -1){
		for (num = 0; num < STATE2_MAX-1; num++){
			bTime += air[side][P1.stateno].B[num].time;
			// 時間が上回っている＆次がある
			if ((bTime <= P1.time) &&
				(air[side][P1.stateno].B[num + 1].time > -2)){
			}
			//
			else{ break; }
		}
		
		for (int n2 = 0; n2 < HIT_MAX; n2++){
			//if (air[side][P1.stateno].B[num].h[0])TestText(1);
			//else { TestText(0); } あった
			// 判定があるなら描画
			if (air[side][P1.stateno].B[num].h[n2]){
				KuraiSet(n2,
					(int)((air[side][P1.stateno].B[num].hw[n2] - air[side][P1.stateno].B[num].hx[n2]) * P1.GSize),
					(int)((air[side][P1.stateno].B[num].hh[n2] - air[side][P1.stateno].B[num].hy[n2]) * P1.GSize),
					(int)(air[side][P1.stateno].B[num].hx[n2] * P1.GSize), (int)(air[side][P1.stateno].B[num].hy[n2] * P1.GSize));
			}
			if (air[side][P1.stateno].B[num].a[n2]){
				AttSet(n2,
					(int)((air[side][P1.stateno].B[num].aw[n2] - air[side][P1.stateno].B[num].ax[n2]) * P1.GSize),
					(int)((air[side][P1.stateno].B[num].ah[n2] - air[side][P1.stateno].B[num].ay[n2]) * P1.GSize),
					(int)(air[side][P1.stateno].B[num].ax[n2] * P1.GSize), (int)(air[side][P1.stateno].B[num].ay[n2] * P1.GSize));
			}
		} // n2 終了

	}

	//========================
	// もし判定追加があったら
	//========================
	if ((ex.attFlg > 0) && (ex.attSide == P1.PSide)){
		AttSet(ex.i, ex.w, ex.h, ex.x, ex.y);
		ex.attFlg --;
	}
	
	// 値を返す
	P1 = ReturnPSet();


	// P1に設定
	P1.State2 = num;
	P1.aNum1 = air[side][P1.stateno].B[num].num1;
	P1.aNum2 = air[side][P1.stateno].B[num].num2;
	
	P1.aHV = air[side][P1.stateno].B[num].HV;
	
	if (P1.aHV == 0){
		P1.aPosX = air[side][P1.stateno].B[num].posX;
	}
	else if (P1.aHV != 0){
		P1.aPosX = -air[side][P1.stateno].B[num].posX;
	}
	P1.aPosY = air[side][P1.stateno].B[num].posY;

	P1.sPosX = pic[side][P1.aNum1][P1.aNum2].posX;
	P1.sPosY = pic[side][P1.aNum1][P1.aNum2].posY;
	
	

	// 値を渡す
	if (P1.PSide == 1)Get_P1Set(P1);
	if (P1.PSide == 2)Get_P2Set(P1);

}


void HelperKurai(){

	Helper H;

	
	// 最初に判定初期化
	GetH1_Hantei(H1);
	H_KuraiReset();
	H_AttReset();
	for (int i = 0; i < HELPER_MAX; i++){
		H1[i] = ReturnHSet(i);
	}

	GetH1_Hantei(H1);	// 数値を判定に渡す
	

	//=================
	// boxセッティング
	//=================
	int num, bTime;
	int side;
	side = P1.PSide - 1;
	
	// ヘルパー全てに行う
	for (int hn = 0; hn < HM; hn++){
	// あるなら
	if (H1[hn].var){
	// 変換する
	H = H1[hn];
	bTime = 0;
	// ANumが存在するならスタート
	if (air[side][H.stateno].ANum > -1){
		for (num = 0; num < STATE2_MAX - 1; num++){
			bTime += air[side][H.stateno].B[num].time;
			// 時間が上回っている＆次がある
			if ((bTime <= H.HAnimTime) &&
				(air[side][H.stateno].B[num + 1].time > -2)){
			}
			//
			else{ break; }
		}


		for (int n2 = 0; n2 < HIT_MAX; n2++){
			// 判定があるなら描画
			if (air[side][H.stateno].B[num].h[n2]){
				H_KuraiSet(hn, n2,
					(int)((air[side][H.stateno].B[num].hw[n2] - air[side][H.stateno].B[num].hx[n2]) * H.GSize),
					(int)((air[side][H.stateno].B[num].hh[n2] - air[side][H.stateno].B[num].hy[n2]) * H.GSize),
					(int)(air[side][H.stateno].B[num].hx[n2] * H.GSize), (int)(air[side][H.stateno].B[num].hy[n2] * H.GSize));
			}
			// なくてデフォ判定があるなら
			else if (air[side][H.stateno].def && air[side][H.stateno].defh[n2]){
				H_KuraiSet(hn, n2,
					(int)((air[side][H.stateno].defhw[n2] - air[side][H.stateno].defhx[n2]) * H.GSize),
					(int)((air[side][H.stateno].defhh[n2] - air[side][H.stateno].defhy[n2]) * H.GSize),
					(int)(air[side][H.stateno].defhx[n2] * H.GSize), (int)(air[side][H.stateno].defhy[n2] * H.GSize));
			}
			// 攻撃判定
			if (air[side][H.stateno].B[num].a[n2]){
				H_AttSet(hn, n2,
					(int)((air[side][H.stateno].B[num].aw[n2] - air[side][H.stateno].B[num].ax[n2]) * H.GSize),
					(int)((air[side][H.stateno].B[num].ah[n2] - air[side][H.stateno].B[num].ay[n2]) * H.GSize),
					(int)(air[side][H.stateno].B[num].ax[n2] * H.GSize), (int)(air[side][H.stateno].B[num].ay[n2] * H.GSize));
			}
		} // n2 終了
		H = ReturnHSet(hn);

		// Hに設定
		H.state2 = num;
		H.aNum1 = air[side][H.stateno].B[num].num1;
		H.aNum2 = air[side][H.stateno].B[num].num2;
		H.aPosX = air[side][H.stateno].B[num].posX;
		H.aPosY = air[side][H.stateno].B[num].posY;
		H.aHV = air[side][H.stateno].B[num].HV;
		H.sPosX = pic[side][H.aNum1][H.aNum2].posX;
		H.sPosY = pic[side][H.aNum1][H.aNum2].posY;

		// 元に戻す
		H1[hn] = H;

	}	// if終了


	}	// var if 終了
	
	}	// for終了


	// 値を返す
	for (int i = 0; i < HELPER_MAX; i++){
		//H1[i] = ReturnHSet(i);
	}

	

	

	// 値を渡す
	if (P1.PSide == 1)Get_HSet(H1, H2);
	else if (P1.PSide == 2)Get_HSet(H2, H1);
}


void GetP_GHitbox(Player GP1)
{
	P1 = GP1;
}

void GetS_GHitbox(System_t GS)
{
	S = GS;
}

void GetH_GHitbox(Helper GH1[], Helper GH2[])
{
	int i;
	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}


}

void GetA_GHitbox(Air GA[STATE_MAX], int side)
{
	// AIR2作成
	air2.push_back(std::vector<Air>());
	air2[side].reserve(STATE_MAX);	// 容量を確保
	for (int j = 0; j < STATE_MAX; j++){
		air2[side].push_back(Air());
	}
	
	// AIR変数準備 //
	// 初期化＆代入
	for (int n = 0; n < STATE_MAX; n++){
		air2[side][n].setup();
		air2[side][n] = GA[n];
	}
}

void GetPI_GHitbox(Pict GPI[PICT_MAX][IMAGE2_MAX], int side)
{
	for (int a = 0; a < PICT_MAX; a++){
		for (int b = 0; b < IMAGE2_MAX; b++){
			pic[side-1][a][b] = GPI[a][b];
		}
	}
}

void GetN_GHitBox(vector<Name_t>GN)
{
	for (int i = 0; i < Character_Max(); i++) {
		N.push_back(GN[i]);
		//N[i] = GN[i];
	}
}

Player GetAnimElem(Player GP1)
{
	for (int n = 0; n < Character_Max(); n++) {
		// セットスタート
		if (GP1.Name == N[n].name) {
			for (int n1 = 0; n1 < STATE_MAX; n1++) {
				GP1.animElem[n1] = air2[n][n1].aTime;
			}
			break;
		}
	}
	

	return GP1;
}

void load_GHitBox()
{
	// AIR変数準備 //
	// 初期化＆移植
	for (int n = 0; n < STATE_MAX; n++){
		air[P1.PSide - 1][n].setup();
		air[P1.PSide - 1][n] = air2[P1.Name - 1][n];
	}
}

void ExAtt(int PSide, int i, int W, int H, int X, int Y)
{
	ex.i = i;
	ex.w = W;
	ex.h = H;
	ex.x = X;
	ex.y = Y;
	ex.attSide = PSide;
	ex.attFlg = 2;
}