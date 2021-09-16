// 格闘ゲームのプログラム
//
#include <stdio.h>	// ファイル操作に必要なヘッダファイル
#include <string.h>	// 文字列操作に必要なヘッダファイル

#include "pch.h"
/////////

#include "Fps.h"
#include "WindowInfo.h"
#include "MainSystem.h"

// vectorを使う
#include <vector>
using namespace std;

#pragma warning(disable : 4996)	// fscanfのエラー表示を消す

#pragma region	ローカル変数
static int window;			// ウィンドウ

static boolean screenMode;		// スクリーンのモード
static int modeCount = 0;	// モードになってからの経過フレーム

static boolean wait;	// 熱帯相手を待って、モードを動かさない
//static int stop = 0;

#pragma endregion

// 各種項目
//static Fps fps;

// メインシステムクラス
//static MainSystem sy;

//内部関数宣言部--------------------------------------------
void Screen_Setting();

void Game_End();
void File_Loading();
void Replay_DrawData();


//プログラム部----------------------------------------------
#pragma region メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//画面設定
	MainSystem::Instance();
	Screen_Setting();

	// ＤＸライブラリ初期化処理、エラーが起きたら直ちに終了
	if (DxLib_Init() == -1){ return -1; }

	// これでkeyconfig読み込み
	P1_BConfig();

	// ファイルをロード
	File_Loading();

	// 描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// 開始
	MainSystem::Instance().MainLoop();

	// 終了処理
	Game_End();

	return 0;				// ソフトの終了 

}//main終了
#pragma endregion

/////////////////////////////////////////////////////////////////////////////ここまで


// 画面設定
void Screen_Setting()
{
	//
	// フォント読み込み
	//

	// iniでの設定を読み込む                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
	// 命令の名前、変数名、ない場合の値、場所
	screenMode = GetPrivateProfileInt("Config", "windowmode", 1, "./config.ini");	// ./が必要(カレントフォルダ)

	// タイトルを変更
	SetMainWindowText("blackright");

	// フルスクリーンモード時の解像度モードを4:3を使用するモードに設定
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);

	// フルスクリーンモード時の画面の拡大モードを最近点モードに設定
	SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);

	//ウィンドウモードに
	ChangeWindowMode(screenMode);

	// 画面モードの設定、サイズを測る
	SetGraphMode(WindowInfo::width, WindowInfo::height, 16);

	//SetChangeScreenModeGraphicsSystemResetFlag(FALSE);	// グラフィックハンドルのリセット防止
														// ウィンドウサイズ変更時に消えないようにする

	// 非アクティブ状態でも動く(動かさないと熱帯できない)
	SetAlwaysRunFlag(true);

	// 画面サイズを変更可能に
	SetWindowSizeChangeEnableFlag(true);

	// 多重起動
	// ネットワークテスト
	//SetDoubleStartValidFlag(TRUE);

	//= 変数設定 ========
	//wait = false;	// 対戦相手を待つ
}

// 終了処理
void Game_End()
{
	BGMStart(0);
	// ネットワーク切断

	// リプレイ終了
	if (Replay_Mode(-1) > 0)Replay_End();
	// トレモ設定保存
	Tlist_Save();
	DxLib_End();				// ＤＸライブラリ使用の終了処理
}

// ファイルのロード
void File_Loading() {
	// キャラクターファイル読み込み

	// 非同期読み込み設定に変更z 
	//SetUseASyncLoadFlag(TRUE);
	// 重いファイルを読み込む
	GetDef();
	LoadAllFile();
	// 同期読み込み設定に変更
	//SetUseASyncLoadFlag(FALSE);
}

void Replay_DrawData()
{
	DrawFormatString(SCREEN_W - 100, SCREEN_H - 20, GetColor(10,10,255), "状態..%d", Replay_Mode(-1));
}
