// 格闘ゲームのプログラム
//
#include <stdio.h>	// ファイル操作に必要なヘッダファイル
#include <string.h>	// 文字列操作に必要なヘッダファイル

#include "pch.h"
// pch //
/*
#include "DxLib.h"
#include "Char.h"
#include "Graph.h"
#include "Math.h"
#include "Mode.h"
#include "Sound.h"

#include "System.h"
#include "Define.h"
*/
/////////

#include "Menu.h"
#include "Select.h"
#include "Replay.h"
#include "Arcade.h"
#include "Fps.h"
#include "WindowInfo.h"

// vectorを使う
#include <vector>
using namespace std;

#pragma warning(disable : 4996)	// fscanfのエラー表示を消す

#pragma region	ローカル変数
static int GameMode;	// 現在のモード、メニューで初期化
static int LoadMode;			// そのモードの準備を初期化する
static int sw;			// スクショ
static int sw2;
static int sw2_2;
static int window;			// ウィンドウ

static boolean screenMode;		// スクリーンのモード
static boolean drawFPS;			// FPS表示
static int modeCount = 0;	// モードになってからの経過フレーム

static int sd = 0;	// ゲーム進行速度
static boolean wait;	// 熱帯相手を待って、モードを動かさない
//static int stop = 0;
#pragma endregion

// 各種項目
static Menu men;
static Select sel;
static Replay rep;
static Arcade arc;
static Fps fps;

class MainSystem{

private:
	int gameMode;	// モード番号、メニューで初期化
	//ModeData mode;	// 実際のゲームモード
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
			if (GameMode == GameScene(MenuScene))
			{
				///Load_Menu();
				men.Load_First();
			}
			else if (GameMode == GameScene(SelectScene)){
				sel.Load_First();
				Load_State();	// 画像読み込み初期化、
				// ここに戻る度に有効
			}
			else if (GameMode == GameScene(VersusScene)){
				Load_Versus();
			}
			//else if (GameMode == GameConfig){
			//Load_Config();
			//}
			else if (GameMode == GameScene(ResultScene)){
				Load_Result();
			}
			else if (GameMode == GameScene(NetworkScene)){
				Load_Network();
			}
			else if (GameMode == GameScene(ReplayScene)){
				rep.Load_First();
			}
			else if (GameMode == GameScene(ArcadeScene)){
				arc.Load_First();
			}
			//BGMStart(0);	// BGMを停止
			LoadMode = 1;
			modeCount = 0;	// オンライン用
		}

		// namespace modeData

		

		//ゲームモードの名前でモードを選ぶ
		switch (GameMode)
		{
		case GameScene(MenuScene):
			//Menu();
			men.All();
			break;
		case GameScene(TitleScene):
			Title();
			break;
		case GameScene(SelectScene):
			sel.All();
			break;
		case GameScene(VersusScene):
			// お互いversusになったら開始
			if ((Connect_CheckCn()) && (Connect_CheckGetData(0, 9) == GameScene(SelectScene))
				&& (modeCount < 2)){
				break;
			}
			else{ modeCount = 2; }
			Versus();
			break;
			//case GameConfig:
			//Config();
			//break;
		case GameScene(ResultScene):
			Result();
			break;
		case GameScene(NetworkScene):
			Network();
			break;
		case GameScene(ReplayScene):
			rep.All();
			break;
		case GameScene(ArcadeScene):
			arc.All();
			break;
		default:
			GameMode = 0;
			break;
		}
	}

	void MainDraw()
	{
			// 現在のFPSを描画
		// FPS表示がONの時のみ
	}
};
static MainSystem sy;

//内部関数宣言部--------------------------------------------
void Screen_Setting();
void Game_Processing();
void Game_Draw();
void Game_Update();
void Game_End();
void Replay_DrawData();

//プログラム部----------------------------------------------
#pragma region メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//画面設定
	sy:MainSystem();
	Screen_Setting();

	// ＤＸライブラリ初期化処理、エラーが起きたら直ちに終了
	if (DxLib_Init() == -1){ return -1; }

	// これでkeyconfig読み込み
	P1_BConfig();

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
	GameMode = GameScene(TitleScene);
	
	// すぐトレモへ
	//GameMode = GameScene(SelectScene);
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
	// while( 裏画面描画, メッセージ処理(必要らしい), 画面初期化, ESC入力確認, ゲームモードが0じゃない )
	while (ProcessMessage() == 0 && ScreenFlip() == 0 && (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
		&& (GameMode != 0))
	{
		//----------
		// 必須処理
		//----------
		{
			// ゲーム進行がノーマルなら画面クリア
			if (sd == 0){
				if (ClsDrawScreen() != 0)break;
			}
			// FPS更新
			fps.Update();	// OK
			// 準備
			Game_Processing();
		}
		//------
		// 描画
		//------
		{
			//update
			//draw
			//filp
			//wait
			
			Game_Draw();
			//Replay_DrawData();
			fps.Wait();		// 1フレーム経過するまで待機
		}
		//------------
		// 画面の更新
		//------------
		{
			Game_Update();
			
		}
	}
	// メインループ終了

	// 終了処理
	Game_End();

	return 0;				// ソフトの終了 

}//main終了
#pragma endregion

void ModeChange(int num)
{
	// モード変更
	GameMode = num;
	//WaitSwitch
	// 次のモードの準備
	LoadMode = 0;
}
/////////////////////////////////////////////////////////////////////////////ここまで

// ゲームモード取得
int CheckGameMode()
{
	return GameMode;
}

// 画面設定
void Screen_Setting()
{
	//
	// フォント読み込み
	//

	// iniでの設定を読み込む                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
	// 命令の名前、変数名、ない場合の値、場所
	screenMode = GetPrivateProfileInt("Config", "windowmode", 1, "./config.ini");	// ./が必要(カレントフォルダ)
	drawFPS = GetPrivateProfileInt("Config", "drawfps", 1, "./config.ini");

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

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);	// グラフィックハンドルのリセット防止
														// ウィンドウサイズ変更時に消えないようにする

	// 非アクティブ状態でも動く(動かさないと熱帯できない)
	SetAlwaysRunFlag(true);

	// 画面サイズを変更可能に
	SetWindowSizeChangeEnableFlag(true);

	//= 変数設定 ========
	//wait = false;	// 対戦相手を待つ
}

// 入力・FPS更新処理
#pragma region
void Game_Processing()
{
	
	// 入力
	GetInput();	// コントローラが入っているか確認

	// 繋がってるなら
	if (Connect_CheckCn()){
		Delay_Check();
		Connect_SetGameMode();
		// Waitがあるなら止める
		if (Connect_WaitGameMode())sd = 1;
		else{ sd = 0; }
	}

	

	// ボタン入力チェック、スロー時はなし
	// 0～-nは操作する
	if (sd <= 0){
		P1_BCheck();
		P2_BCheck();
	}

	// スクショの確認
	if (CheckHitKey(KEY_INPUT_F1) != 0)sw++;
	else { sw = 0; }

	//Releaseモード時のみ表示したいときなんかは #ifndef _DEBUG で
	//#ifdef _DEBUG
	// 音の確認
	if (CheckHitKey(KEY_INPUT_F2) != 0)StopSound(1);
	else{ StopSound(0); }
	//#endif

	// モード毎処理
	if (GetASyncLoadNum() == 0){
		// 基本
		if (sd == 0){
			sy.MainMove();
			//sd = 1;
		}
		// 停止
		else if (sd == 1){
			sd = 0;
		}
		// 再行動(2倍速)
		else if(sd == -1){
			sy.MainMove();
			P1_BCheck();
			P2_BCheck();
			sy.MainMove();
			sd = 0;
		}
	}

	/*
	if (stop == 0)stop = 1;
	else if (stop == 1) {
		stop = 0;
		sd = 1;
	}
	*/
}
#pragma endregion

void Game_Draw()
{
	// 読み込み数を表示
	if (GetASyncLoadNum() != 0){
		int amari = GetASyncLoadNum() % 5;	// 5ずつ更新
		// 非同期読み込みの数を描画
		DrawFormatString(0, SCREEN_H - 20, Cr, "残りファイル %d", GetASyncLoadNum() - amari);
	}

	// 前の画像を表示
	//if (sd == 1)SaveScreen();
	//else{ Draw_SaveScreen(); }
	// 待機できているか(ネット対戦用)
	if (sd == 1)DrawString(SCREEN_W -100, 20, "モード待機中…",Cr);

	//sy.MainDraw();	// FPS等を描画
	if (drawFPS) { fps.Draw(SCREEN_W - 80, 0); }
}

/// <summary>
/// 情報の更新
/// </summary>
void Game_Update()
{
	// ネットワークに接続中
	if (Connect_CheckCn()){
		DrawString(0, SCREEN_H - 20, "接続中", Cr);
		Delay_Draw();	// ディレイ表示
		DrawFormatString(0, SCREEN_H - 60, Cr, "side..%d", Connect_CheckSide());
		DrawFormatString(0, SCREEN_H - 40, Cr, "port..%d", Connect_CheckPort());
	}
	// スクリーンショット
	if (sw == 1){
		ScreenShot();
	}

	//ウィンドウモードに
	if (CheckHitKey(KEY_INPUT_F4) != 0)sw2++;
	else { sw2 = 0; }
	if (CheckHitKey(KEY_INPUT_F4) != 0) {
		// 最初のみ
		if (sw2 == 1) {
			if (sw2_2 == 0) {
				ChangeWindowMode(0);
				sw2_2 = 1;
			}
			else if (sw2_2 == 1) {
				ChangeWindowMode(1);
				sw2_2 = 0;
			}
		}
	}
}

// 終了処理
void Game_End()
{
	BGMStart(0);
	// ネットワーク切断
	if (Connect_CheckCn())End_Network();
	// リプレイ終了
	if (Replay_Mode(-1) > 0)Replay_End();
	// トレモ設定保存
	Tlist_Save();
	DxLib_End();				// ＤＸライブラリ使用の終了処理
}

void Replay_DrawData()
{
	DrawFormatString(SCREEN_W - 100, SCREEN_H - 20, GetColor(10,10,255), "状態..%d", Replay_Mode(-1));
}