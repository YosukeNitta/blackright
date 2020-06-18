#include "pch.h"
#include "MainSystem.h"
#include <memory>

static int sw;			// スクショ

void Game_Processing();
void Game_Update();


//	クラスのインスタンスを取得
MainSystem& MainSystem::Instance()
{
	// memoryを使って何かを行っている？
	static std::unique_ptr<MainSystem> instance(new MainSystem);
	// インスタンス？を返す
	return *instance;
}

// メインメソッド(これに各種モード)
void MainSystem::MainLoop()
{
	///-------------
	//
	// メインループ
	//
	//--------------
	// while( メッセージ処理(必要らしい), ESC入力確認, ゲームモードが0じゃない )
	while (ProcessMessage() == 0 && (CheckHitKey(KEY_INPUT_ESCAPE) == 0))
	{
		//----------
		// 必須処理
		//----------
		{
			// ゲーム進行がノーマルなら画面クリア
			if (ClsDrawScreen() != 0)break;	// 画面初期化

			// 準備
			Game_Processing();

			// モード毎処理		
			// ロード終了しているなら実行
			if (GetASyncLoadNum() == 0) {
				// 実行
				Update();
			}
			// 読み込み数を表示
			else {
				int amari = GetASyncLoadNum() % 5;	// 5ずつ更新
				// 非同期読み込みの数を描画
				DrawFormatString(0, SCREEN_H - 20, Cr, "残りファイル %d", GetASyncLoadNum() - amari);
			}
		}
		//------
		// 描画
		//------
		{
			//update
			//draw
			//filp
			//wait
			// この順番でfps使う

			// FPS更新
			_fps.Update();	// OK
			if (_drawFPS) { _fps.Draw(SCREEN_W - 80, 0); }
			
			//Replay_DrawData();	// デバッグ用
		}
		//------------
		// 画面の更新
		//------------
		{
			Game_Update();
			_fps.Wait();		// 1フレーム経過するまで待機
		}
		ScreenFlip();	// 表示
		
		// ループ終了
		if (CheckEnd())break;
	}
}

void MainSystem::Update()
{
	// ないなら止める
	if (!_currentMode)
		return;

	// モード読み込み
	if (_currentState == 0) {
		_currentMode->Load_First();	// 読み込み
		_currentState = 1;	// 実行モードへ
	}

	// モード進行
	_currentMode->All();

	//	解放処理
	if (_currentState == 2) {
		_currentMode->Release();
		_currentMode->Release();	// 解放処理
		_currentMode = _nextMode;	// 次のモードセット
		_currentState = 0;
	}
}

// 入力・FPS更新処理
#pragma region
void Game_Processing()
{

	// 入力
	GetInput();	// コントローラが入っているか確認

	// 繋がってるなら
	if (Connect_CheckCn()) {
		Delay_Check();
		Connect_SetGameMode();
	}

	// ボタン入力チェック、スロー時はなし
	// 0～-nは操作する
	P1_BCheck();
	P2_BCheck();

	// スクショの確認
	if (CheckHitKey(KEY_INPUT_F1) != 0)sw++;
	else { sw = 0; }

	//Releaseモード時のみ表示したいときなんかは #ifndef _DEBUG で
	//#ifdef _DEBUG
	// 音の確認
	if (CheckHitKey(KEY_INPUT_F2) != 0)StopSound(1);
	else { StopSound(0); }
	//#endif

	// モード毎処理
	// ロード終了しているなら実行
	//if (GetASyncLoadNum() == 0){
	//	MainSystem::Instance().MainMove();
	//}
}
#pragma endregion

/// <summary>
/// 情報の更新
/// </summary>
void Game_Update()
{
	// ネットワークに接続中
	if (Connect_CheckCn()) {
		DrawString(0, SCREEN_H - 20, "接続中", Cr);
		Delay_Draw();	// ディレイ表示
		DrawFormatString(0, SCREEN_H - 60, Cr, "side..%d", Connect_CheckSide());
		DrawFormatString(0, SCREEN_H - 40, Cr, "port..%d", Connect_CheckPort());
	}
	// スクリーンショット
	if (sw == 1) {
		ScreenShot();
	}
}