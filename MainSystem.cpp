#include "pch.h"
#include "MainSystem.h"
#include <memory>

static int sw;			// スクショ
static NetData* netData;
static boolean netOn = false;	// ネットワークの開始
static BYTE keyLog[KEYLOG_MAX];	// キーの取得
static int GHandle;	// 停止前の画像
static int wait_StopTime;	// ウェイト1になっている時間
//static void Game_Processing();
static void Game_Update();


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
	// 開始前
	_socket = new Socket();
	// 画面のサイズと同じサイズのグラフィックを作成
	GHandle = MakeGraph(SCREEN_W, SCREEN_H);

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

			// てすと用
			if (netOn) {
				_socket->draw();	// 情報表示
				// 特定の条件で終了
				// 一定時間接続がないか、モードセレクトでネットワークを切る
				if (_socket->isEndConnect()) {
					delete _socket;
					netOn = false;
					Key_SetNetOn(false);
					MainSystem::Instance().EndMainSystem();	// 終了
				}
			}

			_fps.Wait();		// 1フレーム経過するまで待機
		}
		ScreenFlip();	// 表示
		
		// ループ終了
		if (CheckEnd()) {
			// 起動しているならネットワークを終了する
			if (netOn) {
				delete _socket;
				netOn = false;
				Key_SetNetOn(false);
				MainSystem::Instance().EndMainSystem();	// 終了
			}
			break;
		}
	}
	// ループ終了時、熱帯を切っておく
	if (netOn) {
		// 特定の条件で終了
		// 一定時間接続がないか、モードセレクトでネットワークを切る
		if (_socket->isEndConnect()) {
			delete _socket;
			netOn = false;
			Key_SetNetOn(false);
		}
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

	{
		// ネット対戦がオンでキー入力の待機状態なら待つ
		// 画面表示を変更する
		if ((netOn) && (!_socket->getIsKeyUpdate())) {
			
			// 取り込んだゲーム画面表示
			DrawGraph(0, 0, GHandle, false);

			// 通常ウェイトモードなら
			// 開幕(2)は行わない
			if (_socket->getWaitMode() == 1) {
				//==========================
				// 暗くする
				//==========================
				// 少し暗くする
				SetAlpha(188);
				// 同じなら解除
				DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
				NoBlend();
			}
		}
		else {
			//==========================
			// 暗くする
			//==========================

			// 進行する
			_currentMode->All();

			// 復活
			// 再進行するなら
			/*
			if (_socket->getMoreKey() == 1) {
				_socket->keyUpdate();
				_socket->searchKey();	// これで再進行を判断(ループ終了)

				_currentMode->All();

				// ピンク色の線を表示する
				SetAlpha(128);
				// 同じなら解除
				DrawBox(0, SCREEN_H / 2 - 15, SCREEN_W, SCREEN_H / 2 + 15, GetColor(255, 0, 255), true);
				NoBlend();
			}
			*/
			// 復活〆

			// 最後に画面データの取りこみ
			GetDrawScreenGraph(0, 0, SCREEN_W, SCREEN_H, GHandle);
		}
	}

	//	解放処理
	if (_currentState == 2) {
		_currentMode->Release();
		_currentMode->Release();	// 解放処理
		_currentMode = _nextMode;	// 次のモードセット
		_currentState = 0;
		// 番号セット
		if (_currentMode == _modes["Versus"]) {
			_modeNum = SceneNum(VersusScene);
		}
		// 番号セット
		else if (_currentMode == _modes["Select"]) {
			_modeNum = SceneNum(SelectScene);
		}
		// 番号セット
		else if (_currentMode == _modes["Result"]) {
			_modeNum = SceneNum(ResultScene);
		}
		// 番号セット
		else if (_currentMode == _modes["Menu"]) {
			_modeNum = SceneNum(MenuScene);
		}
		// シーンなし
		else {
			_modeNum = SceneNum(NoScene);
		}
	}
}

// 入力・FPS更新処理
#pragma region
void MainSystem::Game_Processing()
{
	if (_socket->getWaitMode() == 1) {
		set_stopVB(true);
	}
	else {
		set_stopVB(false);
	}
	// 入力
	GetInput();	// コントローラが入っているか確認
	// ボタン入力チェック、スロー時はなし
	// 0～-nは操作する
	Key_Setup();

	// 通常ウェイトが1フレ以上かかるなら操作を戻す(待ってる間に自分の入力だけ進んでしまう)
	// 1フレあけないと多分-1されてしまう
	//if (_socket->getWaitMode() == 1)Key_Return1Frame();

	// 繋がってるなら
	// !net!
	if (netOn) {
		
		// ここから強制終了の可能性あり
		// netDataをセットする
		if (!_socket->getLoad_f()) 
		{
			_socket->setNetData(netData);
			_socket->setLoad_f(true);	// ロード完了
		}
		
		// ゲームモード取得
		_socket->setMyGameMode(_modeNum);
		// ネット用のキー設定開始
		// ここで自分のキー入力を取得(keyLogにセット)
		Key_NetButtonUpdate();
		// myKeyを送信
		_socket->setKeyLog(keyLog);
		// socketで更新処理を行い、2pKeyを取得
		_socket->update();

		// キーにソケットで取得したKeyを渡す
		// ここでKeyに入力を保存
		Key_SetGKeyLog(_socket->p2KeyLog);	// 2Pのキー入力
		Key_SetSendKey(_socket->myKeyLog);	// 自分のキー入力
		// 1P、2Pのキーを決定
		// 入力を反映する
		Key_NetButtonEnter();
		
		/*
		// ウェイトモードが1になってる時間を計測
		if (_socket->getWaitMode() == 1){
			wait_StopTime++;
		}	
		else {
			wait_StopTime = 0;
		}
		*/
		// 通常ウェイトが1フレ以上かかるなら操作を戻す(待ってる間に自分の入力だけ進んでしまう)
		// 1フレあけないと多分-1されてしまう
		
	}
	
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
	if (netOn) {
		//DrawString(0, SCREEN_H - 20, "接続中", Cr);
		// ネットデータ表示
		//netData->DataDraw(120, 100);
		
	}
	// スクリーンショット
	if (sw == 1) {
		ScreenShot();
	}
}

//////////////////////////////////////////////////////////////////////////////
// 取得する
void Main_GetNetData(NetData* net) {
	netData = net;
}

// 0で
void ChangeNetOn(int n) {
	if (n == 1) netOn = true;
	else { netOn = false; }
}

boolean GetNetOn() {
	return netOn;
}

// keyからキーログを取得、socketで使う
void Main_SetKeyLog(BYTE key[KEYLOG_MAX]) {
	for (int i = 0; i < KEYLOG_MAX; i++) {
		keyLog[i] = key[i];
	}
}