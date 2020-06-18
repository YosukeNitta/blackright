#pragma once
#include "DxLib.h"

class NetData {

public:
	IPDATA Ip, P2Ip;	// 自分のIP、取得したIP
	int Port;
	int UDPNetHandle;        // ネットワークハンドル
	
	// コンストラクタ
	NetData() {
		NetData::Port = 0;
		NetData::UDPNetHandle = 0;
	}

	// ハンドルを作成
	void GetUDPNetHandle() {
		UDPNetHandle = MakeUDPSocket(Port);
	}

	// ＵＤＰソケットハンドルの削除
	void DeleteUDPNetHandle() {
		DeleteUDPSocket(UDPNetHandle);
		UDPNetHandle = -1;
	}

	void GetIp(IPDATA ip) {
		Ip = ip;
	}

	void GetPort(int port) {
		Port = port;
	}

	// ネットデータ内の要素を表示
	void DataDraw(int x, int y) {
		DrawBox(x - 1, y - 1, x + 281, y + 61, Cr, false);
		DrawFormatString(x, y, GetColor(255, 255, 255), "udpHandle..%d", UDPNetHandle);
		DrawFormatString(x, y + 20, GetColor(255, 255, 255), "port..%d", Port);
		DrawFormatString(x, y + 40, GetColor(255, 255, 255), "ip..%d.%d.%d.%d", Ip.d1, Ip.d2, Ip.d3, Ip.d4);
	}
private:
	
};