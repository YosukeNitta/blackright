// 格闘ゲームのプログラム
//
#include <stdio.h>	// ファイル操作に必要なヘッダファイル
#include <string.h>	// 文字列操作に必要なヘッダファイル

#include "pch.h"

#pragma warning(disable : 4996)	// fscanfのエラー表示を消す

#define BLOG_MAX	(21)	//キー記録の最大数

/*
* ローカル変数
*/

static int GameMode;	// 現在のモード、メニューで初期化
static int LoadMode;			// そのモードの準備を初期化する
static int sw;			// スクショ
static int window;			// ウィンドウ

static boolean screenMode;		// スクリーンのモード
static boolean drawFPS;			// FPS表示
static boolean network = false;

class MainSystem{

private:
	int gameMode;	// 現在のモード、メニューで初期化
public:
	// コンストラクタ
	MainSystem::MainSystem()
	{
		gameMode = 0;
	}

	// メインメソッド(これに各種モード)
	void MainMove()
	{
		// 変更された場合、モード初期化
		if (LoadMode == 0){
			if (GameMode == GameMenu)
			{
				Load_Menu();
			}
			else if (GameMode == GameSelect){
				Load_Select();
				Load_State();	// 画像読み込み初期化、
				// ここに戻る度に有効
			}
			else if (GameMode == GameVersus){
				Load_Versus();
			}
			//else if (GameMode == GameConfig){
				//Load_Config();
			//}
			else if (GameMode == GameResult){
				Load_Result();
			}
			else if (GameMode == GameNetwork){
				Load_Network();
			}
			//BGMStart(0);	// BGMを停止
			LoadMode = 1;
		}

		//ゲームモードの名前でモードを選ぶ
		switch (GameMode)
		{
		case GameMenu:
			Menu();
			break;
		case GameTitle:
			Title();
			break;
		case GameSelect:
			Select();
			break;
		case GameVersus:
			Versus();
			break;
		//case GameConfig:
			//Config();
			//break;
		case GameResult:
			Result();
			break;
		case GameNetwork:
			Network();
			break;
		default:
			GameMode = 0;
			break;
		}
		/*
		if (GameMode == GameMenu)
		{
			Menu();		//メニュー、返ってきた値が次のモード
		}
		else if (GameMode == GameTitle)
		{
			Title();
		}
		else if (GameMode == GameSelect){
			Select();	//モード変更 キャラセレクト
		}
		else if (GameMode == GameVersus){
			Versus();	//モード変更 対戦
		}
		else if (GameMode == GameConfig){
			Config();	//モード変更 コンフィグ
		}
		else if (GameMode == GameResult){
			Result();	//モード変更 コンフィグ
		}
		else if (GameMode == GameNetwork){
			Network();	//モード変更 コンフィグ
		}
		else {
			//GameMode = 0;	//終了
		}
		*/

	}
	void MainDraw()
	{
		if (drawFPS){ Draw(); }		// 現在のFPSを描画
		// FPS表示がONの時のみ
	}
};
static MainSystem sy;

// フォント変更
static void SetFont();	

/////////////////////////////////////////////////////////////////////////////ここからスタート
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//
	// フォント読み込み
	//
	//SetFont();
	

	// iniでの設定を読み込む
	// 命令の名前、変数名、ない場合の値、場所
	screenMode = GetPrivateProfileInt("Config", "windowmode", 1, "./config.ini");
	drawFPS = GetPrivateProfileInt("Config", "drawfps", 1, "./config.ini");

	//ウィンドウモードに
	ChangeWindowMode(screenMode);

	// タイトルを変更
	SetMainWindowText("blackright");

	// 画面モードの設定、サイズを測る
	SetGraphMode(SCREEN_W, SCREEN_H, 16);

	// 非アクティブ状態でも動く(動かさないと熱帯できない)
	SetAlwaysRunFlag(true);

	// 画面サイズを変更可能に
	//SetWindowSizeChangeEnableFlag(true);

	// ＤＸライブラリ初期化処理、エラーが起きたら直ちに終了
	if (DxLib_Init() == -1){ return -1; }

	// これでkeyconfig読み込み
	P1_BConfig(0, 0);

	{
		// 非同期読み込み設定に変更
		//SetUseASyncLoadFlag(TRUE);

		// 重いファイルを読み込む
		GetDef();
		LoadAllFile();

		// 同期読み込み設定に変更
		//SetUseASyncLoadFlag(FALSE);
	}

	// 起動後最初のモード
	GameMode = GameTitle;

	// すぐトレモへ
	//GameMode = GameSelect;
	//TraningSwitch(true);
	//BattleMode(false);
	// ctrl + K + U or C

	// 描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	///-------------
	//
	// メインループ
	//
	//--------------

	// ここに入力管理
	
	// while( 裏画面描画, メッセージ処理(必要らしい), 画面初期化, ESC入力確認, ゲームモードが0じゃない )
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClsDrawScreen() == 0 && (CheckHitKey(KEY_INPUT_ESCAPE) == 0) &&
		(GameMode != 0))
	{
		//----------
		// 必須処理
		//----------
		{
			// FPS更新
			Update();	// OK

			GetInput();	// コントローラが入っているか確認、ほんとは一度だけでいい

			// スクショの確認
			if (CheckHitKey(KEY_INPUT_F1) != 0)sw++;
			else { sw = 0; }
			// ウィンドウの確認
			if (CheckHitKey(KEY_INPUT_F4) != 0)window++;
			else { window = 0; }

			// ボタン入力チェック
			P1_BCheck();
			P2_BCheck();
		}

		//
		if (GetASyncLoadNum() == 0){
			sy.MainMove();
		}
		//------
		// 描画
		//------
		{
			//Game_MainDraw();

			if (GetASyncLoadNum() != 0){
				// 非同期読み込みの数を描画
				DrawFormatString(0, SCREEN_H - 20, GetColor(255, 255, 255), "残りファイル %d", GetASyncLoadNum());
			}
		}

		//------------
		// 画面の更新
		//------------
		{
			sy.MainDraw();	// FPS等を描画
			Wait();		// 1フレーム経過するまで待機
			// どっちが先？

			// ネットワーク
			if (network)DrawString(0, SCREEN_H - 20, "接続中", Cr);
			// スクリーンショット
			if (sw == 1){
				ScreenShot();
			}
			/*
			// ウィンドウ変更
			if (window == 1){
				if (screenMode == false)screenMode = true;
				else{ screenMode = false; }
				ChangeWindowMode(screenMode);
			}
			*/

		}

	}
	// メインループ終了

	// 操作設定保存
	//Key_Save();
	BGMStart(0);
	// ネットワーク切断
	if (network)End_Network();
	// トレモ設定保存
	Tlist_Save();

	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 

}//main終了

void ModeChange(int num)
{
	// モード変更

	GameMode = num;
	// 次のモードの準備
	LoadMode = 0;
}
/////////////////////////////////////////////////////////////////////////////ここまで

// ゲームモード取得
int CheckGameMode()
{
	return GameMode;
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

// ネットワーク取得
void Main_GetNetwork(boolean net)
{
	network = net;
}

// ネットワークの接続確認
boolean Main_CheckNetwork()
{
	return network;
}