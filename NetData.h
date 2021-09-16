#pragma once
#include "DxLib.h"



class NetData {
	
private:
	static const int CDATA_SIZE = 2;
	BYTE cData[CDATA_SIZE];		// 通信テスト自分用
	BYTE cGetData[CDATA_SIZE];	// 通信テスト受け取り用
	int port;
	int delayMax;	// 通信ディレイの最大値
public:
	int playerSide;	// 1Pか2Pか
	IPDATA ip;	// 自分のIP、取得したIP
	int UDPNetHandle;        // ネットワークハンドル
	int waitTime;	// これが一定時間たったら次へ
	boolean connectOn;	// 接続完了のサイン

	// コンストラクタ
	NetData::NetData() {
		for (int i = 0; i < CDATA_SIZE; i++) {
			cData[i] = 0;
			cGetData[i] = 0;
		}

		playerSide = 0;
		ip = { 0,0,0,0 };
		
		waitTime = 0;
		connectOn = false;
		
		port = 0;
		delayMax = 1;
		UDPNetHandle = 0;//MakeUDPSocket(-1);	// ハンドル作成
	}

	// デストラクタ
	~NetData() {
		if (UDPNetHandle != 0) DeleteUDPNetHandle();
	}

	// 1P2Pを設定
	void setPlayerSide(int side) {
		playerSide = side;
	}

	// ポート番号でハンドルを作成
	void MakeUDPNetHandle() {
		UDPNetHandle = MakeUDPSocket(port);
	}

	// ＵＤＰソケットハンドルの削除
	void DeleteUDPNetHandle() {
		DeleteUDPSocket(UDPNetHandle);
		UDPNetHandle = 0;
	}

	void setIp(IPDATA getIp) {
		ip = getIp;
	}

	void setPort(int getPort) {
		port = getPort;
	}

	IPDATA getIp() {
		return ip;
	}

	int getPort() {
		return port;
	}

	int getDelayMax() {
		return delayMax;
	}

	void setDelayMax(int delay) {
		// 最大・最小値
		if (delay > 20)delay = 20;
		else if (delay < 2)delay = 2;
		delayMax = delay;
	}

	// 接続前に使用する
	void connect_Reset() {
		for (int i = 0; i < CDATA_SIZE; i++) {
			cData[i] = 0;
			cGetData[i] = 0;
		}
		waitTime = 0;
		connectOn = false;
	}

	// 接続テスト
	// 成功時は1が返ってくる
	int connect_Test() {
		int rNum = 0;
		
		if (playerSide == 1)rNum = connect_Host();
		else if (playerSide == 2)rNum = connect_Client();

		return rNum;
	}

	int connect_Host() {
		// ホスト
		// 0が接続待ち(テスト表示用)
		if(cData[0] < 2)cData[0] = 1;
		// UDPデータ受け取り待機
		int cnwr_ret = CheckNetWorkRecvUDP(UDPNetHandle);
		// データを受信成功したら
		if (cnwr_ret && cnwr_ret != -1) {

			// 先にデータ受け取り
			NetWorkRecvUDP(UDPNetHandle, &ip, NULL, cGetData, sizeof(cGetData), FALSE);	// IP取得する

			// 相手が待機中なら送り返す
			if(cGetData[1] > 0)cData[0] = 2;
			// ネットワークテスト
			//NetWorkSendUDP(UDPNetHandle, ip, 7600, cData, sizeof(cData));	// 返す
			NetWorkSendUDP(UDPNetHandle, ip, port, cData, sizeof(cData));	// 返す
			
			
			
			// 接続成功したら //
			if (cData[0] == 2 && cGetData[1] == 2)
			{
				//DrawFormatString(420, SCREEN_H - 240, Cr, "%d,%d %d,%d", cData[0], cData[1], cGetData[0], cGetData[1]);
				DrawFormatString(420, SCREEN_H - 260, Cr, "！接続に成功！");
				connectOn = true;
			}
		}
		else {
			DrawFormatString(420, SCREEN_H - 260, Cr, "接続待機中");
		}

		// 成功中はカウント
		if (connectOn)waitTime++;
		// 経過したら次へ
		if (waitTime > 60)return 1;

		return 0;
	}

	int connect_Client() {
		if (cData[1] < 2)cData[1] = 1;
		// クライアント
		// 数字が送られてきたら2Pも進める
		if (cGetData[0] > 0)cData[1] = 2;
		// 文字列の送信
		// ネットワークテスト
		//NetWorkSendUDP(UDPNetHandle, ip, 7500, cData, sizeof(cData));
		NetWorkSendUDP(UDPNetHandle, ip, port, cData, sizeof(cData));

		// UDPを接続
		int cnwr_ret = CheckNetWorkRecvUDP(UDPNetHandle);
		// 受信した上でエラーじゃない
		if (cnwr_ret && cnwr_ret != -1) {

			// データを受信 IPアドレス、ポートは知ってるのでいらない
			NetWorkRecvUDP(UDPNetHandle, NULL, NULL, cGetData, sizeof(cGetData), FALSE);

			// 接続成功したら //
			if (cGetData[0] == 2 && cData[1] == 2)
			{
				//DrawFormatString(420, SCREEN_H - 240, Cr, "%d,%d %d,%d", cData[0], cData[1], cGetData[0], cGetData[1]);
				DrawFormatString(420, SCREEN_H - 260, Cr, "！接続に成功！");
				connectOn = true;
			}
		}
		else {
			DrawFormatString(420, SCREEN_H - 260, Cr, "接続待機中");
		}

		// 成功中はカウント
		if (connectOn)waitTime++;
		// 経過したら次へ
		if (waitTime > 60)return 1;

		return 0;
	}

	// ネットデータ内の要素を表示
	void DataDraw(int x, int y) {
		DrawBox(x - 1, y - 1, x + 281, y + 101, Cr, false);
		DrawFormatString(x, y, GetColor(255, 255, 255), "player..%d", playerSide);
		DrawFormatString(x, y + 20, GetColor(255, 255, 255), "udpHandle..%d", UDPNetHandle);
		DrawFormatString(x, y + 40, GetColor(255, 255, 255), "port..%d", port);
		DrawFormatString(x, y + 60, GetColor(255, 255, 255), "ip..%d.%d.%d.%d", ip.d1, ip.d2, ip.d3, ip.d4);
		DrawFormatString(x, y + 80, GetColor(255, 255, 255), "delay..%d", delayMax);
		//DrawFormatString(x, y + 80, GetColor(255, 255, 255), "cGetData..%d, %d", cGetData[0], cGetData[1]);
	}
	
};