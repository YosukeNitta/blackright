//インクルード部--------------------------------------------
#include "pch.h"
#include "Title.h"
#include "MainSystem.h"

static int titleG;
//static int tGLogo;
static int fill;
static boolean load;
static int timeStop;
static int kettei;
static int alpha = 255;	// 透明度
static boolean change = true;

//メニュー画面
int Title::Mode()
{
	

	/*
	// ロード
	if (!load)
	{
		fill = 255;
		load = true;
	}

	// 塗りつぶし
	if (fill > 0){
		fill -= 6;
	}
	if (fill < 0){
		fill = 0;
	}
	*/

	if (change) {
		if (alpha > 205)alpha -= 3;
		alpha += 6;
	}
	else {
		if (alpha < 50)alpha += 3;
		alpha -= 6;
	}

	if (alpha < 0) {
		change = true;
		alpha = 0;
	}
	else if (alpha > 255) {
		change = false;
		alpha = 255;
	}

	// 描画
	{
		// 画像
		//DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(220,220,220), true);
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawGraph(0, 0, titleG, true);
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//DrawGraph(0, 0, tGLogo, true);

		SetAlpha(alpha);
		DrawString(240, 400, "PRESS START BUTTON", Cr);
		NoBlend();
	}

	// キー入力
	// 決定キー時処理
	if ((P_BInput(1) == 1 || P_BInput(3) == 1 || P_BInput(7) == 1) && (kettei == 0))
	{
		kettei = 1;
		SEStart(35);
	}

	// 決定してたらカウント加算
	if (kettei == 1) {
		timeStop += 1;
	}
	else {
		timeStop = 0;
	}


	// キー入力で終了
	if (timeStop >= 10) {
		load = false;
		//ModeChange(SceneNum(MenuScene));
		MainSystem::Instance().SetNextMode("Menu");
		kettei = 0;
		
	}
	return 0;	//どこにも当たらなかったら終了

}	//メニュー終了

void Title::Draw()
{

}

void Title::Load_1second()
{
	//titleG = LoadGraph("ob/title.png");
}

void Title::Load_Reload()
{
	titleG = LoadGraph("ob/title.png");
}


void Title::Release(void)
{
	// 終了処理
	DeleteGraph(titleG);
	//titleG = LoadGraph("ob/title.png");
}