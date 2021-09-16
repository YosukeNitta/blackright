//インクルード部--------------------------------------------
#include "pch.h"
#include "Versus.h"
#include "MainSystem.h"
static boolean Load_1;
static int Num;
static int ModePause, PauseNum;
static int GHandle;
static int bgmNum = 1;

static int gameTime;	// 1～60でループ
static boolean turboMode;
static int _anten;	// 暗転(最初)
static int _timeStop;	// 暗転
static int _moveScene;	// シーン移動

/**************
* 対戦画面
**************/

int Versus::Mode() 
{
	if (!Load_1) {
		/*
		* システム処理
		*/
		// 体力、位置を初期化しておく
		CharLoad();
		// ポーズをオフ
		ModePause = 0;
		// BGMを鳴らす
		//BGMStart(1);

		//Load_1 = true;
	}

	// 処理開始
	{
		//--------------
		// 時間カウント
		//--------------
		{
			gameTime++;
			if (gameTime > 60)
				gameTime = 1;
		}

		/*********
		* 演算処理
		*********/
		{
			if (ModePause == 0) {
				StopCmd(false);	// コマンド受け付け
				Num = CharMove();	// 座標とかを計算する、これが Versus のメイン
				/*
				// ターボモード
				if (turboMode) {
					if (gameTime == 12 || gameTime == 24 || gameTime == 36 ||
						gameTime == 48 || gameTime == 60) { //|| gameTime == 60){
						//StopCmd(true);	// コマンドを受け付けない
						P1_BCheck();
						P2_BCheck();
						CharMove();
					}
				}
				*/

				// 1が返ってきたら終了
				if (Num == 1) {
					CharLoad();
					MainSystem::Instance().SetNextMode("Menu");	// メニューへ
				}
			}// ModePause 
		}

		//---------
		// BGM再生＆ロード完了
		//---------
		if (!Load_1) {
			// BGMを鳴らす
			BGMStart(bgmNum);
			Load_1 = true;
		}

		/*********
		* 描画処理
		*********/
		if (Load_1 == 1)	// ロード完了してたら
		{
			// 計算に使う値は State で渡しておこう

			// 戦闘中
			if (!ModePause)
			{
				// キャラ、背景描画、ライフバー
				MainDraw();
				// その他(エフェクト、文字等)
				ObjectDraw();
			}

			if (ModePause)
			{
				// ポーズ前の画像を表示
				DrawGraph(0, 0, GHandle, false);
				// ポーズ画面
				// 返ってきた値でモード変更
				PauseNum = Pause();
				// ポーズが終了された
				if (PauseNum != 0) {
					// グラフィックをメモリ上から削除する
					DeleteGraph(GHandle);
				}
			}
			// ポーズ返り値で変更
			if (PauseNum != 0) {
				switch (PauseNum) {
				case 1:
					ModePause = 0, PauseNum = 0;
					_moveScene = 1;
					//MainSystem::Instance().SetNextMode("Versus");
					// ステータス戻す
					break;
				case 2:
					ModePause = 0, PauseNum = 0;
					_moveScene = 2;
					//MainSystem::Instance().SetNextMode("Select");
					break;
				case 3:
					ModePause = 0, PauseNum = 0;
					_moveScene = 3;
					//MainSystem::Instance().SetNextMode("Menu");
					break;
				case 4:	// 対戦画面に戻る
					ModePause = 0, PauseNum = 0;
					// 続行
					break;
				case SceneNum(ReplayScene):	// リプレイ
					ModePause = 0, PauseNum = 0;
					_moveScene = SceneNum(ReplayScene);
					//MainSystem::Instance().SetNextMode("Replay");
					// 続行
					break;
				}
			}
		}	// Load_1
		else {
			DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);	// 真っ黒
		}

		// シーン変更
		if (_moveScene) {
			// 暗転を進める
			_timeStop++;

			// 終了時の暗転
			if (_timeStop > 0) {
				int num = _timeStop * 25;
				if (num > 255) num = 255;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, num);
				DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			// 移動する
			if (_timeStop > 11) {
				switch (_moveScene) {
				case 1:
					MainSystem::Instance().SetNextMode("Versus");
					break;
				case 2:
					MainSystem::Instance().SetNextMode("Select");
					break;
				case 3:
					MainSystem::Instance().SetNextMode("Menu");
					break;
				case SceneNum(ReplayScene):	// リプレイ
					MainSystem::Instance().SetNextMode("Replay");
					// 続行
					break;
				}
			}
		}
		// 開始時の暗転
		else if(_anten > 0){
			// 終了時の暗転
			if (_anten > 0) {
				int num = _anten * 25;
				if (num > 255) num = 255;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, num);
				DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			_anten--;
		}
		
	}// メインループ終了


	//処理終了
	return 0;
}

void Versus::Draw()
{
	
}

void Versus::Load_Reload() 
{
	Load_1 = false;
	_anten = 11;
	_moveScene = 0;	// 移行シーン
	_timeStop = 0;	// 終了時の暗転
}


void Versus::Load_1second() 
{
}

void Versus::Release(void)
{
}

void PauseSetting(int n1)
{
	ModePause = n1;

	// キャラ、背景描画、ライフバー
	MainDraw();
	// その他(エフェクト、文字等)
	ObjectDraw();

	// 画面のサイズと同じサイズのグラフィックを作成
	GHandle = MakeGraph(SCREEN_W, SCREEN_H);

	// 画面データの取りこみ
	GetDrawScreenGraph(0, 0, SCREEN_W, SCREEN_H, GHandle);
}

void Load_Versus()
{
	Load_1 = false;
}

void Versus_bgmNum(int num)
{
	bgmNum = num;
}

boolean Versus_GetTurboMode(){
	return turboMode;
}

void Versus_TurboMode()
{
	if (!turboMode)
		turboMode = true;
	else{
		turboMode = false;
	}
}