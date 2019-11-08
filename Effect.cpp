//インクルード部--------------------------------------------
#include "pch.h"
#include <math.h>
#include <vector>
using namespace std;

#define EFF_MAX	(30)	//エフェクトの最大数
static const int BE_MAX = 10;	//後ろエフェクトの最大数
static const int E_MAXTIME = 300;	//エフェクト表示時間

// 画像の配列
static int EffSet[EFF_MAX][EFF_MAX];
static int LoadE;
static int xadd, yadd;

//[ エフェクト ]
typedef struct {
	int Image;		// 現在の画像
	
	float SizeX,SizeY;	// 描画サイズ
	float defX, defY;	// 最初のサイズ(実際に表示されるサイズではない)
	int alpha;		// 透過

	int XPos, YPos;	// 画像の表示位置
	int XAdd, YAdd;	// 座標を移動させる
	int XSize, YSize;// 画像サイズ
	int Num1, Num2;	// 画像番号 縦と横
	int Time;		// 時間
	boolean turn;
	int RGB[3];		// 変えると色が変わる
	float rot;		// 角度

	void Draw(){
		DrawRotaGraph3(XPos + XAdd - (int)S.ScroolX, YPos + YAdd - (int)S.ScroolY + S.quakeY,
			XSize / 2, YSize / 2,
			SizeX, SizeY, rot, Image, true, turn);
	}
}Effect_t;

// エフェクトの変数用意
static Effect_t E[EFF_MAX];
//static vector<Effect_t> E;
// 簡易表示エフェクト
static Effect_t E2[EFF_MAX];
static Effect_t BE[BE_MAX];

//表示フラグ
static boolean E2drawFlg;
static Effect_t EffImage(Effect_t e);
static Effect_t EffSetting(int Effnum, int EffXP, int EffYP, int basePosX, int basePosY,
	float SizeX, float SizeY, boolean turn);
static Effect_t EffectDraw(Effect_t e);

// num は呼び出す画像
int EffStart(int Effnum, int EffXP, int EffYP, int basePosX, int basePosY, 
				float SizeX, float SizeY, boolean turn)
{
	// 画像をロード
	if (LoadE == 0)
	{
		xadd = 0, yadd = 0;
		SetTransColor(0, 0, 0);
		// 非同期読み込み設定に変更
		SetUseASyncLoadFlag(TRUE);

		EffSet[0][0] = LoadGraph("ob/Effect/03_slash.png");
		LoadDivGraph("ob/Effect/01_hit.png", 5, 5, 1, 320, 320, EffSet[1]);
		LoadDivGraph("ob/Effect/02_hit2.png", 5, 5, 1, 320, 320, EffSet[2]);
		EffSet[3][0] = LoadGraph("ob/Effect/03_slash.png");
		EffSet[4][0] = LoadGraph("ob/Effect/04_slash.png");
		LoadDivGraph("ob/Effect/05.png", 5, 5, 1, 400, 700, EffSet[5]);
		LoadDivGraph("ob/Effect/06_air.png", 6, 3, 2, 400, 400, EffSet[6]);
		EffSet[7][0] = LoadGraph("ob/Effect/07_hydro.png");
		EffSet[8][0] = LoadGraph("ob/Effect/08_shot.png");
		EffSet[9][0] = LoadGraph("ob/Effect/10_right.png");

		EffSet[10][0] = LoadGraph("ob/Effect/15_wave.png");
		LoadDivGraph("ob/Effect/11_guard.png", 10, 5, 2, 500, 500, EffSet[11]);
		LoadDivGraph("ob/Effect/12_guard2.png", 10, 5, 2, 500, 500, EffSet[12]);
		EffSet[15][0] = LoadGraph("ob/Effect/15_wave.png");
		EffSet[16][0] = LoadGraph("ob/Effect/16_wave.png");
		EffSet[18][0] = LoadGraph("ob/Effect/18_wave2.png");
		EffSet[19][0] = LoadGraph("ob/Effect/19_wave.png");
		EffSet[17][0] = LoadGraph("ob/Effect/17_right.png");
		EffSet[20][0] = LoadGraph("ob/Effect/20_fire.png");
		EffSet[21][0] = LoadGraph("ob/Effect/21_fire2.png");

		// 非同期読み込み設定に変更
		SetUseASyncLoadFlag(FALSE);

		LoadE = 1;
	}

	for (int i = 0; i < EFF_MAX; i++){
		// 画像が入っていなかったら登録
		if (E[i].Image == 0){
			E[i] = EffSetting(Effnum, EffXP, EffYP, basePosX, basePosY,
				SizeX, SizeY, turn);

			//e.push_back(Effect_t());
			//e[i] = EffSetting(Effnum, EffXP, EffYP, basePosX, basePosY,
				//SizeX, SizeY, turn);
			break;	// 登録終了
		}

	}
	
	return 0;
}

// num は呼び出す画像
int EffStartB(int Effnum, int EffXP, int EffYP, int basePosX, int basePosY,
				float SizeX, float SizeY, boolean turn)
{
	for (int i = 0; i < BE_MAX; i++){
		// 画像が入っていなかったら登録
		if (BE[i].Image == 0){
			BE[i] = EffSetting(Effnum, EffXP, EffYP, basePosX, basePosY,
				SizeX, SizeY, turn);
			break;	// 登録終了
		}

	}

	return 0;
}

void EffDraw()
{
	

	// 描画ブレンドモードをアルファブレンド（透明）にする
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	for (int i = 0; i < EFF_MAX; i++){

		// エフェクトが入っていたら描画
		if (E[i].Image != 0){
			E[i] = EffectDraw(E[i]);
		}

		// E2描画
		if (E2drawFlg){
			if (E2[i].Image != 0){
				// 描画ブレンドモードをアルファブレンド（透明）にする
				if(E2[i].alpha == 0)SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
				else if (E2[i].alpha != 0){
					SetDrawBlendMode(DX_BLENDMODE_ADD, E2[i].alpha);
				}

				E2[i].Draw();

				if (E[i].alpha != 0){
					// 描画ブレンドモードをノーブレンドにする
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}

				E2[i].Image = 0;
			}
		}
		
	}
	// easyDraw終了
	E2drawFlg = false;

	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void EffDrawB()
{
	for (int i = 0; i < BE_MAX; i++){
		// エフェクトが入っていたら描画
		if (BE[i].Image != 0){
			BE[i] = EffectDraw(BE[i]);
		}
	}
}

Effect_t EffImage(Effect_t e)
{
	int i, time;
	time = e.Time;

	// P1 ステート確認
	switch (e.Num1)
	{
	case 0:	//空
		i = 19;	// 2
		break;
	case 1:	//ヒット１
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		if (time >= 4){
			i = 2;	// 2
		}
		if (time >= 6){
			i = 3;	// 2
		}
		if (time >= 8){
			i = 4;	// 2
		}
		if (time >= 10){
			i = 19;	// 2
		}
		break;

	case 2:	//ヒット２
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		if (time >= 4){
			i = 2;	// 2
		}
		if (time >= 6){
			i = 3;	// 2
		}
		if (time >= 8){
			i = 4;	// 2
		}
		if (time >= 10){
			i = 19;	// 2
		}
		break;

		// スラッシュ
	case 3:
		i = 0;
		if (time == 0){
			e.rot = GetRand(180);
			e.SizeX -= (float)(e.defX * 0.7);
			e.SizeY -= (float)(e.defY * 0.7);
		}
		if (time < 12){
			e.SizeX += (float)(e.defX / 10.0);
			e.SizeY += (float)(e.defY / 10.0);
			//e.SizeX -= (float)(e.defX / 10.0);
			//e.SizeY -= (float)(e.defY / 10.0);
		}
		else { i = 19; }

		break;

		// スラッシュ(真横)
	case 4:
		i = 0;
		if (time == 0) {
			//e.rot = 15.0;
			e.SizeX -= (float)(e.defX * 0.7);
			e.SizeY -= (float)(e.defY * 0.7);
		}
		
		if (time < 12) {
			e.SizeX += (float)(e.defX / 10.0);
			e.SizeY += (float)(e.defY / 10.0);
			//e.SizeX -= (float)(e.defX / 10.0);
			//e.SizeY -= (float)(e.defY / 10.0);
		}
		else if (time < 20) {
			e.SizeX -= (float)(e.defX / 10.0);
			e.SizeY -= (float)(e.defY / 10.0);
			//e.SizeX -= (float)(e.defX / 10.0);
			//e.SizeY -= (float)(e.defY / 10.0);
		}
		else { i = 19; }

		break;

	case 5:	//真空
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		if (time >= 4){
			i = 2;	// 2
		}
		if (time >= 6){
			i = 3;	// 2
		}
		if (time >= 8){
			i = 4;	// 2
		}
		e.Image = EffSet[e.Num1][i];
		// 画像サイズ取得
		GetGraphSize(e.Image, &e.XSize, &e.YSize);
		yadd = -(e.YSize / 2) + 2;
		break;

	case 6:	// 3ゲージ風
		if (time >= 0){
			i = 5;	// 5
			xadd = -103;
		}
		if (time >= 5){
			i = 4;	// 3
			xadd = -95;
		}
		if (time >= 8){
			i = 3;	// 3
			xadd = -71;
		}
		if (time >= 11){
			i = 2;	// 3
			xadd = -24;
		}
		if (time >= 14){
			i = 1;	// 3
			xadd = 5;
		}
		if (time >= 17){
			i = 0;	// 3
			xadd = 28;
		}
		
		if (time >= 20){
			i = 0;	// 3
			xadd = 42;
		}
		if (time >= 23){
			i = 1;	// 2
			xadd = 93;
		}
		if (time >= 25){
			i = 2;	// 2
			xadd = 139;
		}
		if (time >= 27){
			i = 3;	// 2
			xadd = 172;
		}
		if (time >= 29){
			i = 4;	// 4
			xadd = 215;
		}
		if (time >= 33){
			i = 5;	// 4
			xadd = 231;
		}
		if (time >= 37){
			i = 19;	// end
			xadd = 0;
		}
		break;

	case 7:	// コーネル：雨
		if (time >= 0){
			i = 0;	// 5
			xadd = -103;
		}
		/*
		if (time >= 10){
			i = 19;	// 2
		}
		*/
		break;
		
	case 8:	// ヘリオス：ショット
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 10){
			i = 19;	// 5
		}
		break;

	case 9:	// 光2(移動)
		i = 0;
		if (time == 0){
			e.alpha = 250;
			e.rot = GetRand(360);
			xadd = sin(e.rot) * 200;
			yadd = cos(e.rot) * 200;
		}
		else if (time == 1){
			e.alpha = 240;
			xadd = sin(e.rot) * 300;
			yadd = cos(e.rot) * 300;
		}
		else if (time == 2){
			e.alpha = 220;
			xadd = sin(e.rot) * 500;
			yadd = cos(e.rot) * 500;
		}
		else if (time == 3){
			e.alpha = 200;
			xadd = sin(e.rot) * 700;
			yadd = cos(e.rot) * 700;
		}
		else if (time == 4){
			e.alpha = 180;
			xadd = sin(e.rot) * 900;
			yadd = cos(e.rot) * 900;
			e.SizeX -= (float)(e.defX * 0.1);
			e.SizeY -= (float)(e.defY * 0.1);
		}
		else if (time == 5){
			e.alpha = 160;
			xadd = sin(e.rot) * 1100;
			yadd = cos(e.rot) * 1100;
			e.SizeX -= (float)(e.defX * 0.1);
			e.SizeY -= (float)(e.defY * 0.1);
		}
		else if (time == 6){
			e.alpha = 140;
			xadd = sin(e.rot) * 1400;
			yadd = cos(e.rot) * 1400;
			e.SizeX -= (float)(e.defX * 0.1);
			e.SizeY -= (float)(e.defY * 0.1);
		}
		else if (time == 7){
			e.alpha = 130;
			xadd = sin(e.rot) * 1900;
			yadd = cos(e.rot) * 1900;
			e.SizeX -= (float)(e.defX * 0.1);
			e.SizeY -= (float)(e.defY * 0.1);
		}
		else if (time == 8){
			e.alpha = 128;
			xadd = sin(e.rot) * 2500;
			yadd = cos(e.rot) * 2500;
			e.SizeX -= (float)(e.defX * 0.1);
			e.SizeY -= (float)(e.defY * 0.1);
		}
		else { i = 19; }
		break;

	case 10:	// 光
		i = 0;
		if (time == 0){
			e.alpha = 194;
		}
		else if (time == 1){
			e.alpha = 128;
			e.SizeX += (float)(e.defX * 0.1);
			e.SizeY += (float)(e.defY * 0.1);
		}
		else if (time == 2){
			e.alpha = 64;
			e.SizeX += (float)(e.defX * 0.2);
			e.SizeY += (float)(e.defY * 0.2);
		}
		else if (time == 3){
			e.alpha = 32;
			e.SizeX += (float)(e.defX * 0.4);
			e.SizeY += (float)(e.defY * 0.4);
		}
		else { i = 19; }
		break;
	
	case 11:	//ガード１
		if (time >= 0){
			i = 0;	// 1
		}
		if (time >= 1){
			i = 1;	// 1
		}
		if (time >= 2){
			i = 2;	// 1
		}
		if (time >= 3){
			i = 3;	// 1
		}
		if (time >= 4){
			i = 4;	// 1
		}
		if (time >= 5){
			i = 5;	// 1
		}
		if (time >= 6){
			i = 6;	// 1
		}
		if (time >= 7){
			i = 7;	// 2
		}
		if (time >= 9){
			i = 1;	// 2
		}
		if (time >= 11){
			i = 19;	// end
		}
		break;

	case 12:	//ガード２
		if (time >= 0){
			i = 0;	// 1
		}
		if (time >= 1){
			i = 1;	// 1
		}
		if (time >= 2){
			i = 2;	// 1
		}
		if (time >= 3){
			i = 3;	// 1
		}
		if (time >= 4){
			i = 4;	// 1
		}
		if (time >= 5){
			i = 5;	// 1
		}
		if (time >= 6){
			i = 6;	// 1
		}
		if (time >= 7){
			i = 7;	// 2
		}
		if (time >= 9){
			i = 1;	// 2
		}
		if (time >= 11){
			i = 19;	// end
		}
		break;
	// 衝撃波
	case 15:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX += (float)(e.defX / 10.0);
			e.SizeY += (float)(e.defY / 10.0);
		}

		if (e.alpha > 0) {
			e.alpha -= 25;
		}
		else { i = 19; }
		break;
	// 衝撃波
	case 16:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX+=(float)(e.defX/10.0);
			e.SizeY+=(float)(e.defY/10.0);
		}
		
		if (e.alpha > 0) {
			e.alpha -= 25;
		}
		else { i = 19; }

		break;

		// 光
	case 17:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX += (float)(e.defX / 12.0);
			e.SizeY += (float)(e.defY / 12.0);
		}

		if (e.alpha > 0) {
			e.alpha -= 25;
		}
		else { i = 19; }

		break;

		// 青衝撃波
	case 18:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX += (float)(e.defX / 10.0);
			e.SizeY += (float)(e.defY / 10.0);
		}

		if (e.alpha > 0) {
			e.alpha -= 25;
		}
		else { i = 19; }

		break;
		
		// 衝撃波
	case 19:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX += (float)(e.defX / 10.0);
			e.SizeY += (float)(e.defY / 10.0);
		}

		if (e.alpha > 0) {
			e.alpha -= 25;
		}
		else { i = 19; }

		break;

		// 光
	case 20:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX += (float)(e.defX / 12.0);
			e.SizeY += (float)(e.defY / 12.0);
		}
		

		if (e.alpha > 0) {
			e.alpha -= 18;
		}		
		else { i = 19; }
		break;

	case 21:	// ヘリオス：ショット2
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 10){
			i = 19;	// 5
		}
		break;
	default:
		i = time / 2;
		break;
	
	} // 画像セット終わり
	
	e.Num2 = i;
	return e;
}

// 簡易表示
void EasyEffDraw(int num1, int time, int EffXP, int EffYP, int basePosX, int basePosY,
	float SizeX, float SizeY, boolean turn)
{
	for (int i = 0; i < EFF_MAX; i++){
		// 画像が入っていなかったら登録
		if (E2[i].Image == 0){

			// 時間を初期化
			E2[i].Time = time, E2[i].alpha = 0;
			E2[i].XAdd = 0, E2[i].YAdd = 0;
			E2[i].RGB[0] = 255, E2[i].RGB[1] = 255, E2[i].RGB[2] = 255;

			// 描画サイズ
			E2[i].SizeX = SizeX;
			E2[i].SizeY = SizeY;
			E2[i].defX = SizeX;
			E2[i].defY = SizeY;

			// 画像番号取得
			E2[i].Num1 = num1;
			E2[i] = EffImage(E2[i]);	// Num2その他

			E2[i].XAdd = (int)(xadd * SizeX), E2[i].YAdd = (int)(yadd * SizeY);
			if (turn){
				E2[i].XAdd = -E2[i].XAdd;
			}
			xadd = 0, yadd = 0;

			// 画像を読み込み
			E2[i].Image = EffSet[E2[i].Num1][E2[i].Num2];
			
			// 画像サイズ取得
			GetGraphSize(E2[i].Image, &E2[i].XSize, &E2[i].YSize);
			
			E2[i].turn = turn;

			// 画像の位置
			if (turn)basePosX = -basePosX;	// 反対にする
			E2[i].XPos = EffXP + basePosX;// -(int)S.ScroolX;
			E2[i].YPos = EffYP + basePosY; //- (int)((E[i].YSize * E[i].SizeY) / 2);

			break;	// 登録終了
		}

	}
	E2drawFlg = true;

}

// 画像表示
Effect_t EffectDraw(Effect_t e)
{
	// 横画像取得
	e = EffImage(e);	// Num2その他

	// 画像を読み込み
	e.Image = EffSet[e.Num1][e.Num2];

	// 座標調整
	e.XAdd = (int)(xadd * e.SizeX);
	e.YAdd = (int)(yadd * e.SizeY);
	if (e.turn) {
		e.XAdd = -e.XAdd;
	}
	xadd = 0, yadd = 0;


	// 描画ブレンドモードをアルファブレンド（透明）にする
	if (e.alpha == 0)SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	else if (e.alpha != 0) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, e.alpha);
	}

	e.Draw();

	if (e.alpha != 0) {
		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	// 時間経過
	e.Time += 1;

	// 画像削除して再利用できるようにする
	if (e.Time >= E_MAXTIME)e.Image = 0;

	return e;
}

// 画像登録
Effect_t EffSetting(int Effnum, int EffXP, int EffYP, int basePosX, int basePosY,
	float SizeX, float SizeY, boolean turn)
{
	Effect_t e;

	// 時間を初期化
	e.Time = 0, e.alpha = 0;
	e.XAdd = 0, e.YAdd = 0;
	e.rot = 0;
	e.RGB[0] = 255, e.RGB[1] = 255, e.RGB[2] = 255;

	// 描画サイズ
	e.SizeX = SizeX;
	e.SizeY = SizeY;
	e.defX = SizeX;
	e.defY = SizeY;

	// 画像番号取得
	e.Num1 = Effnum;
	e = EffImage(e);	// Num2その他

	e.XAdd = (int)(xadd * SizeX), e.YAdd = (int)(yadd * SizeY);
	if (turn){
		e.XAdd = -e.XAdd;
	}
	xadd = 0, yadd = 0;

	// 画像を読み込み
	e.Image = EffSet[e.Num1][e.Num2];

	// 画像サイズ取得
	GetGraphSize(e.Image, &e.XSize, &e.YSize);

	e.turn = turn;

	// 画像の位置
	if (turn)basePosX = -basePosX;	// 反対にする
	e.XPos = EffXP + basePosX;// -(int)S.ScroolX;
	e.YPos = EffYP + basePosY; //- (int)((e.YSize * e.SizeY) / 2);

	return e;	// 登録終了
}


void GetP_Effect(Player GP1, Player GP2)
{
	P2 = GP2;
}

void GetS_Effect(System_t GS)
{
	S = GS;
}

void GetH_Effect(Helper GH1[], Helper GH2[])
{
	int i;
	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}
}