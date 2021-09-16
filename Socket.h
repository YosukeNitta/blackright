#pragma once
#include "DxLib.h"
#include "NetData.h"

// 通信用クラス
class Socket {
protected:
	// ポート番号等の通信に必要なデータ
	static const int DATALOG_MAX = KEYLOG_MAX + 5;	// 記録する最大データ
													// +3は時間2つとゲームモード
													// あとping計測用のランダム値
													// 自分のnowTimeも渡す
	// 
	// 500バイト程度に抑えたほうがいいらしい
	// 12 * 10 = 120 で快適
	// 20 * 14 の 280バイトが限度か
	//

	static const int SEND_SIZE = 10;	// 送受信するキーログのサイズ
										// 10フレーム分取得する
	static const int ALLLOG_SIZE = 60;	// 全体ログのサイズ
										// 送信データ × これのサイズ
	//static const int DELAY_MAX = 6;	// これ以上差があると止まる
										// 相手がディレイ / 2 まで近づくと再開
	static const int DELAY_MIDDLETIME = 4;	// ディレイ処理を始める時間
	static const int DISCONNECT_TIME = 300;	// 切断する時間
											// これを過ぎると通信終了する
	static const int SOCKET_WAITTIME = 250;	// ソケットの待機時間

	boolean load_f;	// 最初のみロード
	int disconnectTime;	// 通信停止中の時間

	BYTE myGameMode;	// ゲームモード
	BYTE p2GameMode;	// 取得したゲームモード
	BYTE waitMode;	// モード変更の待機	0..待機しない 1..待機 2..ロード時待機
					// キー入力を待っており、ゲーム進行はisKeyUpdateが行う
	BYTE waitTime[2];	// ソケットの待機時間 [1]が60になる度に[0]にストックされる
	BYTE p2WaitTime[2];	// ソケットの番号1～60;
	BYTE p2MaxWait[2];	// 一番大きい待機値

	BYTE nowWaitTime[2];	// 実際の待機時間 2Pのキー取得に合わせて進む
							// これと同じ待機番号を反映させる

	int getData;	// 取得データ確認用(テスト)
	int getRecvData;	// 取得回数(テスト)
	boolean isChangeP2GameMode;	// 2Pのモード変更があったか
	int gameTime;
	boolean isKeyUpdate;	// ゲーム内のキーが進行した
	
	BYTE randNum;	// 1～100、101～200 から一致するたびに+10ずつしてランダムに設定
	BYTE randBaseNum;	// 10段階の変動値
	BYTE returnRandNum;	// 帰ってきたランダム値
	BYTE p2RandNum;	// 2Pが送ってきたランダム値
	BYTE pingCount;		// かえってくるまで加算し続ける
	BYTE ping;		// 一致した値が帰ってきた時間を表示

	int moreKey;	// もっとゲームを進行できる

public:
	// レバー+4ボタン+スタート
	//  +ゲームモード 
	//  +経過時間(パケット番号)+末尾番号(送ったものが合ってるか証明)
	//static const int KEYLOG_MAX = 9;
	NetData* netData;
	BYTE myKeyLog[KEYLOG_MAX];	// 自分のキーログ、Keyクラスで使われる
	BYTE p2KeyLog[KEYLOG_MAX];	// 取得したキーログ、Keyクラスで使われる

	BYTE mySendData[SEND_SIZE][DATALOG_MAX];	// 送信するデータ
	BYTE p2RecvData[SEND_SIZE][DATALOG_MAX];	// 受信するデータ
	
	BYTE myAllLog[ALLLOG_SIZE][SEND_SIZE][DATALOG_MAX];	// 全体のログ
	BYTE p2AllLog[ALLLOG_SIZE][SEND_SIZE][DATALOG_MAX];	// 2P全体ログ


	// コンストラクタ
	Socket() {
		load_f = false;
		disconnectTime = 0;
		waitTime[0] = 0;
		waitTime[1] = 1;
		p2WaitTime[0] = 0;	// 2Pは受信してから反映
		p2WaitTime[1] = 0;	//
		p2MaxWait[0] = 0;	// 
		p2MaxWait[1] = 0;	//

		nowWaitTime[0] = 0;
		nowWaitTime[1] = 1;

		// 実際に使われるキー
		for (int i = 0;i < KEYLOG_MAX; i++) {
			myKeyLog[i] = 0;
			p2KeyLog[i] = 0;
		}

		// 通信用データ
		for (int i = 0; i < SEND_SIZE; i++) {
			for (int j = 0; j < DATALOG_MAX; j++) {
				mySendData[i][j] = 0;
				p2RecvData[i][j] = 0;
			}
		}

		// 全体ログ
		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				for (int k = 0; k < DATALOG_MAX; k++) {
					myAllLog[i][j][k] = 0;
					p2AllLog[i][j][k] = 0;
				}
			}
		}

		netData = new NetData();

		myGameMode = p2GameMode = SceneNum(NoScene);
		waitMode = 0;

		getData = 0;
		getRecvData = 0;
		isChangeP2GameMode = false;
		gameTime = 0;
		isKeyUpdate = false;

		randNum = GetRand(9) + 1;
		randBaseNum = 0;
		returnRandNum = 0;
		p2RandNum = 255;
		pingCount = 0;	
		ping = 0;

		moreKey = 0;
	}

	// デストラクタ
	~Socket() {
		DeleteUDPSocket(netData->UDPNetHandle);	// ソケット削除
		delete netData;	// ネットデータ削除
	}

	//================
	// getter, setter
	//================

	// get
	boolean getLoad_f() {
		return load_f;
	}
	// set
	void setLoad_f(boolean b) {
		load_f = b;
	}

	// ネットデータ取得
	// Networkクラスから取得する
	/*
	NetData getNetData() {
		return *netData;
	}
	*/
	void setNetData(NetData* n) {
		netData = n;
	}

	int getNoConnectTime() {
		return disconnectTime;
	}

	void setNoConnectTime(int time) {
		disconnectTime = time;
	}

	BYTE getMyGameMode() {
		return p2GameMode;
	}

	void setMyGameMode(int b) {
		// モード番号が違うなら
		if (myGameMode != (BYTE)b) {
			myGameMode = (BYTE)b;
			// 待機時間をを初期化する
			nowWaitTime[0] = 0;
			nowWaitTime[1] = 1;
			waitTime[0] = 0;
			waitTime[1] = 1;
			// モード変更で待つ
			waitMode = 2;
		}
	}

	BYTE getP2GameMode() {
		return p2GameMode;
	}

	void setP2GameMode(int b) {
		p2GameMode = (BYTE)b;
	}

	BYTE getWaitMode() {
		return waitMode;
	}

	void setWaitMode(int f) {
		waitMode = (BYTE)f;
	}

	boolean getIsKeyUpdate() {
		return isKeyUpdate;
	}

	void setIsKeyUpdate(boolean flg) {
		isKeyUpdate = flg;
	}

	int getMoreKey() {
		return moreKey;
	}

	/////////////////////
	// 更新処理
	// 受け渡しは別で行う(先にkeyとmodeを取得しておくこと)
	int update() {

		// 大きさを比べて自分がでかすぎたら止まる
		int nowTime = nowWaitTime[0] * 60 + nowWaitTime[1];
		int myTime = waitTime[0] * 60 + waitTime[1];
		int p2Time = p2WaitTime[0] * 60 + p2WaitTime[1];
		int p2Max = p2MaxWait[0] * 60 + p2MaxWait[1];	// 最大値計測

		// ロード完了時の待機
		//if (waitTime[0] == 0 && waitTime[1] == 1) {
		if(waitMode == 2){
			//waitMode = 2;
			// nowより自分が一定数でかかったらキー入力を止める
			//if (myTime - nowTime > DELAY_MAX) {
			if (myTime - nowTime > netData->getDelayMax()) {
				// 相手より大きいなら
				//if (myTime > p2Time)
				// nowTimeが相手と合わせて進むので、nowTimeと比較した
				waitMode = 1;
			}
			// 一致したらウェイト終了
			else if (p2WaitTime[0] == 0 && p2WaitTime[1] == 1) {
				waitMode = 0;
			}
			
		}
		// ウェイト2(開幕)以外のモード設定
		else {
			// nowより自分が一定数でかかったらキー入力を止める
			//if (myTime - nowTime > DELAY_MAX) {
			if (myTime - nowTime > netData->getDelayMax()) {
				waitMode = 1;
			}
			// キー入力時間が近づいたら再開
			//else if (myTime - p2Time <= DELAY_MAX / 2) {
			//else if (myTime - nowTime <= DELAY_MAX / 2) {
			// キー入力が追いついたら再開
			else if (myTime == nowTime) {
				waitMode = 0;
			}
			
			//2Pの最大値が自分より大きいなら進める
			// *テスト* 
			if (p2Max > myTime) {
				waitMode = 0;
			}
		}

		// 自分の入力キーを通信用データに移動
		moveKeyToData();

		// データ送信
		sendData();
		// データ取得
		recvData();

		// 通信用データをログに移動
		moveDataToLog();	// データをログに移動
		//moveP2DataToLog();	// 〃
		//setP2WaitTime();	// ウェイトタイム決定
							// 分離した
		setP2MaxWait();	// ウェイトタイム最大値を設定する

		// ログのカウントを見てキー決定
		// どちらも待機時間の入力があれば時間更新

		// ランダム値が合ってたらping決定
		enterPing();

		
		// キーを更新する
		keyUpdate();

		// nowWaitがディレイ値よりやや離れたら
		// 可能ならゲームを早送りする
		//if(myTime - nowTime > DELAY_MIDDLETIME)searchKey();	

		// ソケット情報
		// 通常ウェイト以外は進める
		if (waitMode == 0 || waitMode == 2)
		{
			// ソケット番号更新
			waitTime[1]++;
			if (waitTime[1] >= 60) {
				waitTime[1] = 0;
				waitTime[0]++;	// 大きい位
				// リセット
				if (waitTime[0] >= SOCKET_WAITTIME) {
					waitTime[0] = 1;
				}
			}
		}

		// 停止中なら
		// 入力を消去する
		// keyUpdateで消去していたのでなし
		/*
		if (waitMode != 0) {
			// 入力を全て消す
			for (int i = 0; i < KEYLOG_MAX; i++) {
				myKeyLog[i] = 0;
				p2KeyLog[i] = 0;
			}
		}
		*/
		// リプレイ進行値を取得
		getGameTime(0);

		// 一定時間経過で切断させる？
		return 0;
	}

	// 描画
	// 通信中のデータを表示
	void draw();

	// キーログを入力から取得
	void setKeyLog(BYTE key[KEYLOG_MAX]) {
		myKeyLog[0] = 0;
		for (int i = 0; i < KEYLOG_MAX; i++) {
			myKeyLog[i] = key[i];
		}
	}
	// getter
	//BYTE* getGKeyLog() {return p2KeyLog;}

	// 相手側にデータ送信
	int sendData() {
		// テスト
		// 通常ウェイトのみ止める
		// 先行ウェイトは2回に一回送る
		//if (waitMode == 0 || waitMode == 2 ||
		//	((waitMode == 1) && (disconnectTime % 2 == 0)))

		// 常に送信したほうがよさそうだった
		{
			// ネットワークテスト
			/*
			if (netData->playerSide == 1) {
				// 10フレーム分
				NetWorkSendUDP(netData->UDPNetHandle, netData->getIp(), 7600, mySendData, sizeof(mySendData));
			}
			else if (netData->playerSide == 2) {
				// 10フレーム分
				NetWorkSendUDP(netData->UDPNetHandle, netData->getIp(), 7500, mySendData, sizeof(mySendData));
			}
			*/
			// キーログ送信
			NetWorkSendUDP(netData->UDPNetHandle, netData->getIp(), netData->getPort(), mySendData, sizeof(mySendData));
		}

		return 0;
	}

	// 相手のデータ受信
	int recvData() {
		// 受信前に初期化する
		// 通信用データ
		for (int i = 0; i < SEND_SIZE; i++) {
			for (int j = 0; j < DATALOG_MAX; j++) {
				//p2RecvData[i][j] = 0;
			}
		}

		// UDPを接続
		int cnwr_ret = CheckNetWorkRecvUDP(netData->UDPNetHandle);
		// データ取得確認の表示用
		getData = cnwr_ret;
		getRecvData = 0;

		// 終了時は途中でループに入れないようにする
		if (disconnectTime >= DISCONNECT_TIME) {
			return -1;
		}
		
		// データがあるときだけ読み込む？
		while (1) {
			cnwr_ret = CheckNetWorkRecvUDP(netData->UDPNetHandle);
			// 受信した上でエラーじゃない
			if (cnwr_ret && cnwr_ret != -1) {
				// 10フレーム分
				NetWorkRecvUDP(netData->UDPNetHandle, NULL, NULL, p2RecvData, sizeof(p2RecvData), FALSE);
				// データをログに移動
				moveP2DataToLog();
				getRecvData++;
			}
			// エラー
			else {
				break;
			}
		}

		return 0;
	}

	// 自分のキー入力 > 通信用データに移動
	int moveKeyToData() {
		// 相手のランダム値は常に反映する
		mySendData[0][KEYLOG_MAX + 4] = p2RandNum;	

		// 通常ウェイトモードのみ変更しない
		// 自分の送信キーを変えてないため
		if (waitMode == 1)return -1;

		// ☆
		// 通信用データを1列更新する
		// SendDataを1つ空ける
		for (int i = SEND_SIZE - 1; i > 0; i--) {
			for (int j = 0; j < DATALOG_MAX; j++) {
				mySendData[i][j] = mySendData[i-1][j];
			}
		}

		//----------------------------
		// ボタンの入力をデータに移動
		for (int i = 0; i < KEYLOG_MAX; i++) {
			mySendData[0][i] = myKeyLog[i];
		}
		
		// 番号
		mySendData[0][KEYLOG_MAX] = waitTime[0];
		mySendData[0][KEYLOG_MAX + 1] = waitTime[1];
		// モード
		mySendData[0][KEYLOG_MAX + 2] = myGameMode;
		// ping
		mySendData[0][KEYLOG_MAX + 3] = randNum;	// 自分のランダム値
		// p2RandNumはメソッドの最初で変えている
		//
		//----------------------------

		return 0;
	}

	// 通信用データをログに移行する
	int moveDataToLog() {

		// 待機モードなら変えない
		// タイム0,1なら普通に通す
		if (waitMode == 1)return -1;
		
		// ログを1列移動する
		// 追加するログ[0][][]を空ける
		for (int i = ALLLOG_SIZE - 1; i > 0; i--) {
			for (int j = 0; j < SEND_SIZE; j++) {
				for (int k = 0; k < DATALOG_MAX; k++) {
					myAllLog[i][j][k] = myAllLog[i - 1][j][k];
				}
			}
		}
		// 通信用データをログに移動
		// 最新情報の位置に記録
		for (int i = 0; i < SEND_SIZE; i++) {
			for (int j = 0; j < DATALOG_MAX; j++) {
				myAllLog[0][i][j] = mySendData[i][j];
			}
		}

		// モードが変わったら
		if (myAllLog[0][0][KEYLOG_MAX + 2] != myAllLog[1][0][KEYLOG_MAX + 2]) {
			// 古くなった前モードのデータを削除する
			for (int i = 10; i < ALLLOG_SIZE; i++) {
				for (int j = 0; j < SEND_SIZE; j++) {
					for (int k = 0; k < DATALOG_MAX; k++) {
						myAllLog[i][j][k] = 0;
					}
				}
			}
		}

		return 0;
	}

	// 2P
	// 取得データ > ログ
	int moveP2DataToLog() {
		// モードが同じかつ
		// 時間が一緒orそれ以下なら反映させない
		// 無駄なログを入れないための処理
		if (p2AllLog[0][0][KEYLOG_MAX + 2] == p2RecvData[0][KEYLOG_MAX + 2]) {
			if (p2AllLog[0][0][KEYLOG_MAX] == p2RecvData[0][KEYLOG_MAX]) {
				if (p2AllLog[0][0][KEYLOG_MAX + 1] >= p2RecvData[0][KEYLOG_MAX + 1]) {
					return -1;
				}
			}
		}

		// ログを1列移動する
		// 追加するログ[0][][]を空ける
		for (int all = ALLLOG_SIZE - 1; all > 0; all--) {
			for (int send = 0; send < SEND_SIZE; send++) {
				for (int data = 0; data < DATALOG_MAX; data++) {
					p2AllLog[all][send][data] = p2AllLog[all - 1][send][data];
				}
			}
		}
		// 通信用データをログに移動
		// 最新情報の位置に記録
		for (int send = 0; send < SEND_SIZE; send++) {
			for (int data = 0; data < DATALOG_MAX; data++) {
				p2AllLog[0][send][data] = p2RecvData[send][data];
			}
		}

		// ログ削除(現在は移動)

		return 0;
	}

	// 2Pの古いログを削除する
	int deleteP2AllLog() {
		//setP2WaitTime() の内容があった
		// モードが変わったら
		if (isChangeP2GameMode) {
			// 古くなった前モードのデータを削除する
			for (int i = 10; i < ALLLOG_SIZE; i++) {
				for (int j = 0; j < SEND_SIZE; j++) {
					// ゲームモードが今と違うなら
					if (p2AllLog[i][j][KEYLOG_MAX + 2] != p2GameMode) {
						for (int k = 0; k < DATALOG_MAX; k++) {
							p2AllLog[i][j][k] = 0;	// 情報を削除する
						}
					}
				}
			}
			isChangeP2GameMode = false;
		}

		return 0;
	}

	// 最大ウェイトタイムを設定する
	// ウェイトをキー決定時に設定したので
	// こっちは最大値を常に測る
	int setP2MaxWait() {
		// 最大値初期化
		p2MaxWait[0] = 0;
		p2MaxWait[1] = 0;
		// 待機時間の最大値取得
		for (int i = 1; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// 時間内で見つけたら、
				if ((p2WaitTime[0] <= p2AllLog[i][j][KEYLOG_MAX]) &&
					(p2AllLog[i][j][KEYLOG_MAX + 2] == myGameMode)) {	// ゲームモードが同じなら
					//p2WaitTime[0] = p2AllLog[i][j][KEYLOG_MAX];
					p2MaxWait[0] = p2AllLog[i][j][KEYLOG_MAX];
					// さらに大きいものを見つける
					if (p2WaitTime[1] < p2AllLog[i][j][KEYLOG_MAX + 1]) {
						//p2WaitTime[1] = p2AllLog[i][j][KEYLOG_MAX + 1];
						p2MaxWait[1] = p2AllLog[i][j][KEYLOG_MAX + 1];
					}
				}
			}
		}

		return 0;
	}

	// ログ > キー反映　これが実際の操作になる
	boolean moveLogToKey() {
		boolean flg = false;

		// 最新情報の位置に記録
		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// ゲームモードが一致したら
				if (myAllLog[i][j][KEYLOG_MAX + 2] == myGameMode) {
					// 1ケタ目のソケット
					if (myAllLog[i][j][KEYLOG_MAX] == nowWaitTime[0]) {
						// 2ケタ目
						if (myAllLog[i][j][KEYLOG_MAX + 1] == nowWaitTime[1]) {
							// キーにログデータをセット
							for (int num = 0; num < KEYLOG_MAX; num++) {
								myKeyLog[num] = myAllLog[i][j][num];
							}
							flg = true;
							break;
						}
					}
				}
			}
		}

		return flg;
	}

	// ログ > キー反映
	boolean moveP2LogToKey() {
		boolean flg = false;

		// 最新情報の位置に記録
		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// ゲームモードが一致したら
				if (p2AllLog[i][j][KEYLOG_MAX + 2] == myGameMode) {
					// 1ケタ目のウェイト
					if (p2AllLog[i][j][KEYLOG_MAX] == nowWaitTime[0]) {
						// 2ケタ目のウェイト
						if (p2AllLog[i][j][KEYLOG_MAX + 1] == nowWaitTime[1]) {
							// キーにログデータをセット
							for (int num = 0; num < KEYLOG_MAX; num++) {
								p2KeyLog[num] = p2AllLog[i][j][num];
							}
							// 各種設定
							{
								// 時間を設定
								//if (p2AllLog[i][j][0] > 0)
								p2WaitTime[0] = p2AllLog[i][j][KEYLOG_MAX];
								//if (p2AllLog[i][j][KEYLOG_MAX + 1] > 0)
								p2WaitTime[1] = p2AllLog[i][j][KEYLOG_MAX + 1];
								
								// モードがちがったら変える
								if (p2GameMode != p2AllLog[i][j][KEYLOG_MAX + 2]) {
									p2GameMode = p2AllLog[i][j][KEYLOG_MAX + 2];
									isChangeP2GameMode = true;	// 変更した
									// モード変更されたなら
									deleteP2AllLog();	// 古いログを削除
								}
								//returnRandNum = p2AllLog[i][j][KEYLOG_MAX + 4];	// 相手が送ってきた自分のランダム値
								p2RandNum = p2AllLog[i][j][KEYLOG_MAX + 3];	// 相手のランダム値
							}
							flg = true;
							break;
						}
					}
				}
			}
		}

		return flg;
	}

	void getGameTime(int num);

	// pingを設定する
	void enterPing() {
		boolean flg = false;
		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// ランダム値が一致したら
				if (p2AllLog[i][j][KEYLOG_MAX + 4] == randNum) {
					flg = true;
					returnRandNum = p2AllLog[i][j][KEYLOG_MAX + 4];
				}
			}
		}

		pingCount++;
		// 一致したらping決定
		if (flg) {
			if (returnRandNum == randNum) {
				ping = pingCount;
				pingCount = 0;	// 値を初期化

				//====================
				// 次のランダム値決定
				//====================
				setRandNum();
			}
		}
	}

	// 次のランダム値決定
	void setRandNum() {
		// 基準値増加
		randBaseNum += 10;
		if (randBaseNum >= 100)
			randBaseNum = 0;
		// 決定
		randNum = randBaseNum + GetRand(9) + 1;
	}

	// キーを実際に進行する＆現在時間を進める
	// 
	int keyUpdate() {
		
		// キー進行の許可を決定する
		isKeyUpdate = false;
		boolean flgP1, flgP2;
		flgP2 = moveP2LogToKey();
		flgP1 = moveLogToKey();
		// キー入力成功したらウェイトタイムを進める
		// 1P,2Pのログに時間と合うキーがあるなら
		if (flgP2 && flgP1) {
			// ソケット番号更新
			nowWaitTime[1]++;
			if (nowWaitTime[1] >= 60) {
				nowWaitTime[1] = 0;
				nowWaitTime[0]++;	// 大きい位
				// リセット
				if (nowWaitTime[0] >= SOCKET_WAITTIME) {
					nowWaitTime[0] = 1;
				}
			}
			isKeyUpdate = true;	// キー進行の許可
			disconnectTime = 0;
			return 0;
		}
		else {
			// 見つからなかった
			// ここで入力を無くす
			disconnectTime++;	// 接続失敗していると増えていく

			for (int j = 0; j < KEYLOG_MAX; j++) {
				myKeyLog[j] = 0;
				p2KeyLog[j] = 0;
			}
		}

		return -1;
	}

	int searchKey() {
		// 2回目以降はやらない
		if (moreKey == 1) {
			moreKey = 0;
			return moreKey;
		}

		// 1P2Pの成立フラグ
		boolean p1Flg, p2Flg;
		p1Flg = p2Flg = false;

		// 最新情報の位置に記録
		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// ゲームモードが一致したら
				if (myAllLog[i][j][KEYLOG_MAX + 2] == myGameMode) {
					// 1ケタ目のソケット
					if (myAllLog[i][j][KEYLOG_MAX] == nowWaitTime[0]) {
						// 2ケタ目
						if (myAllLog[i][j][KEYLOG_MAX + 1] == nowWaitTime[1]) {
							p1Flg = true;
							break;
						}
					}
				}
			}
		}

		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// ゲームモードが一致したら
				if (p2AllLog[i][j][KEYLOG_MAX + 2] == myGameMode) {
					// 1ケタ目のソケット
					if (p2AllLog[i][j][KEYLOG_MAX] == nowWaitTime[0]) {
						// 2ケタ目
						if (p2AllLog[i][j][KEYLOG_MAX + 1] == nowWaitTime[1]) {
							p2Flg = true;
							break;
						}
					}
				}
			}
		}

		if (p1Flg && p2Flg) {
			moreKey = 1;
		}

		return moreKey;
	}

	// 一定条件で切断
	boolean isEndConnect() {
		boolean isEnd = false;
		// 指定した時間経過で切断
		if (disconnectTime > DISCONNECT_TIME)isEnd = true;
		return isEnd;
	}
};
