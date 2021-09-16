//==================================================================================
//
//			共通命令の管理部
//
//========================================================================

//インクルード部--------------------------------------------
#include "pch.h"
#include <iostream>	// 日付で使用
#include <string>

//デファイン部----------------------------------------------



//ﾃﾞｰﾀ定義部------------------------------------------------


//内部関数宣言部--------------------------------------------
static int anten;
static int fadeout;
static int GHandle;
//プログラム部----------------------------------------------

// 描画ブレンドモードをアルファブレンド（透明）にする
void SetAlpha(int n)
{
	
	if (n < 0)n=0;
	else if (n > 255)n = 255;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, n);
}

// 描画ブレンドモードをノーブレンドにする
void NoBlend()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 暗転機能
void Anten(int num)
{
	if (num == 255)anten = 255;

	// 黒い四角を表示
	if (anten > 0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, anten);
		DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		anten -= 20;
	}
	else{
		anten = 0;
	}
}

// 暗転確認
boolean AntenCheck()
{
	if (anten <= 0)return false;
	return true;
}

// フェードアウト
void Fadeout(int num)
{
	if (num == 0)anten = 0;

	// 黒い四角を表示
	if (anten > 0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, anten);
		DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		anten += 20;
	}
	else{
		anten = 0;
	}
}

// フェードアウト確認
boolean FadeoutCheck()
{
	if (fadeout >= 255)return false;
	return true;
}

// スクリーンショット取得
void ScreenShot()
{
	SEStart(28);
	// 日付取得
	DATEDATA Date;
	// 現在時刻を得る
	GetDateTime(&Date);

	std::string str;	// 文字列(これが画像名)
	std::string year = std::to_string(Date.Year);
	std::string mon = std::to_string(Date.Mon);
	std::string day = std::to_string(Date.Day);
	std::string hour = std::to_string(Date.Hour);
	std::string min = std::to_string(Date.Min);
	std::string sec = std::to_string(Date.Sec);
	std::string fn = ".bmp";
	if (mon.size() == 1)mon = "0" + std::to_string(Date.Mon);	// 0をつける
	if (day.size() == 1)day = "0" + std::to_string(Date.Day);	// 0をつける
	if (hour.size() == 1)hour = "0" + std::to_string(Date.Hour);	// 0をつける
	if (min.size() == 1)min = "0" + std::to_string(Date.Min);	// 0をつける
	if (sec.size() == 1)sec = "0" + std::to_string(Date.Sec);	// 0をつける

	str = "SS_" + year + "_" + mon + day + "_" + hour + min + sec + fn;
	SaveDrawScreen(0, 0, SCREEN_W, SCREEN_H, str.c_str());
}

void SetFont()
{
	// ********** フォントのロード **********
	LPCSTR font_path = "ob/PixelMplus-20130602/PixelMplus10-Regular.ttf"; // 読み込むフォントファイルのパス
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);
	}

	// ********** ここから読み込んだフォントを利用できる **********
	SetFontSize(20);	// 見やすさのため、フォントサイズを大きく
	ChangeFont("PixelMplus10", DX_CHARSET_DEFAULT);

	// ********** フォントのアンロード **********
	if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
	}
	else {
		MessageBox(NULL, "remove failure", "", MB_OK);
	}
}

bool Box_Touch(int x1, int y1, int w1, int h1,
	int x2, int y2, int w2, int h2)
{
	// 横サイズが0以下なら
	if ((w1 == 0) || (w2 == 0))return false;

	// xチェック
	if ((x1 >= x2 && x1 <= x2 + w2) ||
		(x2 >= x1 && x2 <= x1 + w1))
	{
		// yチェック
		if (((y1 >= y2 && y1 <= y2 + h2) ||
			(y2 >= y1 && y2 <= y1 + h1)))
		{
			return true;	// 接触
		}
	}
	return false;
}

void SaveScreen()
{
	// 画面のサイズと同じサイズのグラフィックを作成
	GHandle = MakeGraph(SCREEN_W, SCREEN_H);

	// 画面データの取りこみ
	GetDrawScreenGraph(0, 0, SCREEN_W, SCREEN_H, GHandle);
}

void Draw_SaveScreen()
{
	if(GHandle != 0)
		DrawGraph(0, 0, GHandle, false);
}

// 乱数 (999までの数で確率を指定)
boolean random(int num)
{
	boolean f;
	if (GetRand(999) < num)f = true;
	else { f = false; }

	return f;
}