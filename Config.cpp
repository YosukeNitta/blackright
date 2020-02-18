#include "pch.h"
#include "MainSystem.h"
static int Load_1;
static int P1_Key, P2_Key, Gamen;
static int inputUP, inputDOWN;
static int TimeStop, kettei;

static int Load1, Load2;

// 選択ポインタ
// 0でメニュー 1:ボタン設定 2:パッド方向設定
static int SPoint;
// ポインタ縦
static int TPoint;
// ポインタ横
static int YPoint;

static int SPMax;
static int TPMax;

// 画像
static int cursor;

static void Update();
static void Draw();

// コンフィグのメイン関数
int Config()
{
	if(Load_1 == 0){

		// 選択ポインタの位置初期化
		SPoint = 0;
		TPoint = 0;
		YPoint = 0;
		// 選択項目
		SPMax = 5;
		TPMax = 5;
		// 画像読み込み
		cursor = LoadGraph("OB/cursor.png");

		// 選択項目の数を取得
		kettei = 0;
		kettei += 1;

		Load_1 = 1;
	}

	if (SPoint == 0) {
		// バックスペースで終了
		if (P_BInput(2) == 1)
		{
			// おしっぱを拒否
			P1_BCheck();
			// 終了、メニューへ
			MainSystem::Instance().SetNextMode("Menu");
		}
	}
	else if (SPoint == 1) {
		// バックスペースで終了
		if (P_BInput(2) == 1)
		{
			// おしっぱを拒否
			P1_BCheck();
			SPoint = 0;
		}
	}

	// ループ、プログラム実行開始
	{
		Update();

		Draw();

		// 終了
		if ((SPoint == 0) && (P_BInput(2) == 1)) {
			// おしっぱを拒否
			P1_BCheck();
			// 終了、メニューへ
			MainSystem::Instance().SetNextMode("Menu");
		}

	}//全体のループ

	return 0;
}

void Update() {

	// 入力状態ゲット
	P1_Key = P_Input(1);
	P2_Key = P_Input(2);

	// キー入力準備
	if (P1_Key) {
		kettei += 1;
	}
	else {
		kettei = 0;
	}

	// 範囲の限定
	if (SPoint < 0)SPoint = SPMax - 1;
	if (SPoint > SPMax - 1)SPoint = 0;

	// 項目移動
	if (SPoint == 0) {
		if (P_BInput(108) == 1) {
			TPoint--;
		}
		else if (P_BInput(102) == 1) {
			TPoint++;
		}

		if (TPoint < 0)TPoint = TPMax - 1;
		else if (TPoint > TPMax - 1)TPoint = 0;

		// 決定
		if (P_BInput(1) == 1) {
			SPoint = TPoint + 1;
		}
	}
	
	// でばいす
	if (SPoint == 1) {
		// デバイス
		if (InputDevice_Update() == 1) {
			// 終了、メニューへ
			MainSystem::Instance().SetNextMode("Menu");
		}
		InputDevice_Draw();

		// バックスペースで終了
		if (P_BInput(2) == 1)
		{
			// おしっぱを拒否
			P1_BCheck();
			SPoint = 0;
		}
	}
	// ウィンドウ変更
	else if (SPoint == 2) {
		// おしっぱを拒否
		P1_BCheck();
		SPoint = 0;
	}
	// FPS
	else if (SPoint == 3) {
		// おしっぱを拒否
		P1_BCheck();
		SPoint = 0;
	}
	// BGM
	else if (SPoint == 4) {
		// おしっぱを拒否
		P1_BCheck();
		SPoint = 0;
	}
	// 終了
	else if (SPoint == TPMax) {
		// おしっぱを拒否
		P1_BCheck();
		P2_BCheck();
		// 終了、メニューへ
		MainSystem::Instance().SetNextMode("Menu");
	}
}

void Draw() {
	// 描画
	{
		// テスト
		DrawFormatString(SCREEN_W - 80, 20, Cr, "S:%d", SPoint);
		

		if (SPoint == 0){
			// カーソル
			DrawString(0, (20 * TPoint), "■", Cr);

			DrawString(SCREEN_W - 80, 40, "メニュー", Cr);

			//DrawString(20, 0, "ボタン設定", Cr);
			DrawString(20, 0, "ゲームパッド割り当て", Cr);
			
			DrawString(20, 20, "ウィンドウサイズ変更", Cr);
			int fps = GetPrivateProfileInt("Config", "windowmode", 1, "./config.ini");
			if(fps == 0)DrawString(220, 20, "フルスクリーン", Cr);
			else { DrawString(220, 20, "ウィンドウモード", Cr); }

			DrawString(20, 40, "フレームレート表示", Cr);

			DrawString(20, ((TPMax - 1) * 20), "メニューに戻る", Cr);
		}
		else if(SPoint == 1){
			DrawString(SCREEN_W - 80, 40, "ボタン設定", Cr);
		}
		else if (SPoint == 1){
			DrawString(SCREEN_W - 80, 40, "パッド方向", Cr);
		}
		/*
		if (SPoint == 1){
			DrawString(0, 0, "キーボード:1P", Cr);

			int yp = 22;
			int yp2 = yp * 4;
			// キーボード
			DrawString(0, yp * 1, "↑", Cr);
			DrawString(0, yp * 2, "↓", Cr);
			DrawString(0, yp * 3, "←", Cr);
			DrawString(0, yp * 4, "→", Cr);

			
			DrawString(0, yp * 1 + yp2, "Ａ(弱)", Cr);
			DrawString(0, yp * 2 + yp2, "Ｂ(中)", Cr);
			DrawString(0, yp * 3 + yp2, "Ｃ(強)", Cr);
			DrawString(0, yp * 4 + yp2, "Ｄ(特殊)", Cr);
			DrawString(0, yp * 5 + yp2, "Ａ+Ｂ+Ｃ(3ゲージ)", Cr);
			DrawString(0, yp * 6 + yp2, "Ａ+Ｂ(投げ)", Cr);
			DrawString(0, yp * 7 + yp2, "Ｐ(ポーズ)", Cr);
			DrawString(0, yp * 8 + yp2, "ＦＮ１(録画・ポジションリセット)", Cr);
			DrawString(0, yp * 9 + yp2, "ＦＮ２(再生)", Cr);
		}
		DrawString(0, 400, "上下キーで項目を変更できます", Cr);
		*/
	}
}

void Load_Config()
{
	Load_1 = 0;
	InputDevice_Start();	// 初期化
}