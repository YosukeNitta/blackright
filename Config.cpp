#include "pch.h"
#include "Config.h"
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

// パッド入力
static const int pad_max = 32;
static int padInput[pad_max];



// 画像
static int cursor;

static void Update();
//static void Draw();

// コンフィグのメイン関数
int Config::Mode()
{
	if(Load_1 == 0){

		// 選択ポインタの位置初期化
		SPoint = 0;
		TPoint = 0;
		YPoint = 0;
		// 選択項目
		SPMax = 5;
		TPMax = 2;
		// 画像読み込み
		cursor = LoadGraph("OB/cursor.png");

		// 選択項目の数を取得
		kettei = 0;
		kettei += 1;

		Load_1 = 1;
	}

	if (SPoint == 0) {
		// どっちかで終了
		if ((P_BInput(2) == 1) ||
			(TPoint == 1 && P_BInput(1) == 1))
		{
			// おしっぱを拒否
			P1_BCheck();
			SEStart(37);
			// 終了、メニューへ
			MainSystem::Instance().SetNextMode("Menu");
		}
	}
	else if (SPoint == 1) {
		/*
		// バックスペースで終了
		if (P_BInput(2) == 1)
		{
			// おしっぱを拒否
			P1_BCheck();
			SPoint = 0;
		}
		*/
	}

	// ループ、プログラム実行開始
	{
		Update();

		Draw();

		// 終了
		if ((SPoint == 0) && (P_BInput(2) == 1)) {
			// おしっぱを拒否
			P1_BCheck();
			SEStart(37);
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
			SEStart(36);
		}
		else if (P_BInput(102) == 1) {
			TPoint++;
			SEStart(36);
		}

		if (TPoint < 0)TPoint = TPMax - 1;
		else if (TPoint > TPMax - 1)TPoint = 0;

		// 決定
		if (P_BInput(1) == 1) {
			SPoint = TPoint + 1;
			SEStart(35);
		}
	}
	
	// ボタン設定
	if (SPoint == 1) {
		// デバイス
		/*
		if (InputDevice_Update() == 1) {
			// 終了、メニューへ
			MainSystem::Instance().SetNextMode("Menu");
		}
		*/
		// デバイス1P2P表示
		//InputDevice_Draw();

		if (P_BInput(108) == 1) {
			TPoint--;
			SEStart(36);
		}
		else if (P_BInput(102) == 1) {
			TPoint++;
			SEStart(36);
		}
		// 終了の項目が+1
		if (TPoint < 0)TPoint = 9 + 1 - 1;
		else if (TPoint > 9 + 1 - 1)TPoint = 0;

		if (P_BInput(104) == 1) {
			YPoint--;
			SEStart(36);
		}
		else if (P_BInput(106) == 1) {
			YPoint++;
			SEStart(36);
		}
		if (YPoint < 0)YPoint = 2 - 1;
		else if (YPoint > 2 - 1)YPoint = 0;

		// ボタン決定
		// 1P
		// 範囲内ならボタンセット
		if (TPoint < 9) {
			//1P
			if (YPoint == 0) {
				for (int i = 4; i < pad_max; i++) {
					// 何か入力した
					if ((P_Input(1) & padInput[i]) != 0) {
						int setNum;	// 表示用
						setNum = Key_ReturnButton(0, 4 + TPoint);
						
						// 前のキーと違うなら
						if (padInput[i] != setNum) {
							// プレイヤー、 ボタン、内容
							Key_SetButton(0, 4 + TPoint, padInput[i]);
							SEStart(35);
						}
						
					}
				}
			}
			// 2P
			else if (YPoint == 1) {
				for (int i = 1; i < pad_max; i++) {
					// 何か入力した
					if ((P_Input(2) & padInput[i]) != 0) {
						int setNum;	// 表示用
						setNum = Key_ReturnButton(1, 4 + TPoint);
						
						// 前のキーと違うなら
						if (padInput[i] != setNum) {
							// プレイヤー、 ボタン、内容
							Key_SetButton(1, 4 + TPoint, padInput[i]);
							SEStart(35);
						}
						
					}
				}
			}
		}

		// バックスペースで終了
		// 項目決定でも終了
		if (CheckHitKey(KEY_INPUT_BACK) == 1 || 
			(TPoint == 9 && (P_BInput(1) == 1 || P_BInput(2) == 1)))
		{
			// おしっぱを拒否
			P1_BCheck();
			SPoint = 0;
			SEStart(37);
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

void Config::Draw() {
	// 描画
	{
		// テスト
		DrawFormatString(SCREEN_W - 80, 20, Cr, "S:%d", SPoint);
		

		if (SPoint == 0){
			// パッド接続
			DrawFormatString(SCREEN_W - 120, 0 + 60, GetColor(255, 255, 255), "パッド接続数 %d", GetJoypadNum());

			// カーソル
			DrawString(0, (20 * TPoint), "■", Cr);

			DrawString(SCREEN_W - 80, 40, "メニュー", Cr);

			//DrawString(20, 0, "ボタン設定", Cr);
			DrawString(20, 0, "ボタン設定", Cr);
			
			/*
			DrawString(20, 20, "ウィンドウサイズ変更", Cr);
			int window = GetPrivateProfileInt("Config", "windowmode", 1, "./config.ini");
			if(window == 0)DrawString(220, 20, "フルスクリーン", Cr);
			else { DrawString(220, 20, "ウィンドウモード", Cr); }

			DrawString(20, 40, "フレームレート表示", Cr);
			int fps = GetPrivateProfileInt("Config", "drawfps ", 1, "./config.ini");
			if (fps == 0)DrawString(220, 40, "しない", Cr);
			else { DrawString(220, 40, "する", Cr); }
			*/

			DrawString(20, ((TPMax - 1) * 20), "メニューに戻る", Cr);

			DrawString(0, 400, "Enter:決定  Backspace:キャンセル", Cr);
		}
		else if (SPoint == 1){
			DrawString(SCREEN_W - 100, 40, "ボタン設定", Cr);
		}
		else if (SPoint == 2){
			DrawString(SCREEN_W - 80, 40, "パッド方向", Cr);
		}
		
		if (SPoint == 1){
			int xp = 20;
			int yp = 80;

			// カーソル
			DrawString(0, (20 * TPoint) + yp, "■", Cr);

			if(YPoint == 0)DrawString(0, 0, "ゲームパッド:1P", Cr);
			else if (YPoint == 1)DrawString(0, 0, "ゲームパッド:2P", Cr);
			
			// キーボード
			DrawString(xp, yp + 20 * 0, "Ａ(弱)", Cr);
			DrawString(xp, yp + 20 * 1, "Ｂ(中)", Cr);
			DrawString(xp, yp + 20 * 2, "Ｃ(強)", Cr);
			DrawString(xp, yp + 20 * 3, "Ｄ(特殊)", Cr);
			DrawString(xp, yp + 20 * 4, "Ａ+Ｂ+Ｃ(3ゲージ)", Cr);
			DrawString(xp, yp + 20 * 5, "Ａ+Ｂ(投げ)", Cr);
			DrawString(xp, yp + 20 * 6, "Ｐ(ポーズ)", Cr);
			DrawString(xp, yp + 20 * 7, "ＦＮ１(録画・位置リセット)", Cr);
			DrawString(xp, yp + 20 * 8, "ＦＮ２(再生)", Cr);

			for (int i = 0; i < 9; i++) {
				int setNum;	// 表示用
				setNum = Key_ReturnButton(YPoint, i + 4);
				// 表示用に翻訳
				{
					if (setNum == padInput[4]) setNum = 1;
					if (setNum == padInput[5]) setNum = 2;
					if (setNum == padInput[6]) setNum = 3;
					if (setNum == padInput[7]) setNum = 4;
					if (setNum == padInput[8]) setNum = 5;
					if (setNum == padInput[9]) setNum = 6;
					if (setNum == padInput[10]) setNum = 7;
					if (setNum == padInput[11]) setNum = 8;
					if (setNum == padInput[12]) setNum = 9;
					if (setNum == padInput[13]) setNum = 10;
					if (setNum == padInput[14]) setNum = 11;
					if (setNum == padInput[15]) setNum = 12;
					if (setNum == padInput[16]) setNum = 13;
					if (setNum == padInput[17]) setNum = 14;
				}
				DrawFormatString(xp + 240, yp + 20 * i, Cr, "%d", setNum);
			}
			DrawString(xp, yp + 20 * 9, "終了", Cr);
			DrawString(0, 400, "左右でゲームパッドを変更します", Cr);
			DrawString(0, 420, "Backspace:キャンセル", Cr);
		}
		
		
	}
}

void Config::Load_Reload()
{
	// 再接続
	ReSetupJoypad();
}

void Config::Load_1second()
{
	Load_1 = 0;
	InputDevice_Start();	// 初期化

	// パッド
	padInput[0] = 0;
	padInput[1] = 0;
	padInput[2] = 0;
	padInput[3] = 0;
	
	padInput[1 + 3] = PAD_INPUT_1;
	padInput[2 + 3] = PAD_INPUT_2;
	padInput[3 + 3] = PAD_INPUT_3;
	padInput[4 + 3] = PAD_INPUT_4;
	padInput[5 + 3] = PAD_INPUT_5;
	padInput[6 + 3] = PAD_INPUT_6;
	padInput[7 + 3] = PAD_INPUT_7;
	padInput[8 + 3] = PAD_INPUT_8;
	padInput[9 + 3] = PAD_INPUT_9;
	padInput[10 + 3] = PAD_INPUT_10;
	padInput[11 + 3] = PAD_INPUT_11;
	padInput[12 + 3] = PAD_INPUT_12;
	padInput[13 + 3] = PAD_INPUT_13;
	padInput[14 + 3] = PAD_INPUT_14;
	padInput[15 + 3] = PAD_INPUT_15;
	padInput[16 + 3] = PAD_INPUT_16;
	padInput[17 + 3] = PAD_INPUT_17;
	padInput[18 + 3] = PAD_INPUT_18;
	padInput[19 + 3] = PAD_INPUT_19;
	padInput[20 + 3] = PAD_INPUT_20;
	padInput[21 + 3] = PAD_INPUT_21;
	padInput[22 + 3] = PAD_INPUT_22;
	padInput[23 + 3] = PAD_INPUT_23;
	padInput[24 + 3] = PAD_INPUT_24;
	padInput[25 + 3] = PAD_INPUT_25;
	padInput[26 + 3] = PAD_INPUT_26;
	padInput[27 + 3] = PAD_INPUT_27;
	padInput[28 + 3] = PAD_INPUT_28;
}

void Config::Release(void)
{

}