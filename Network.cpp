#include "pch.h"
#include "MainSystem.h"
#include "Network.h"
#include "NetData.h"

using namespace std;	// stringのstdを省略

static NetData *netData;	// 他の場所でも使えるデータ

// 接続確認用byteデータ
// 0:なし 1:接続待ち 2:接続成功
static BYTE Data[2];	
static BYTE getData[2];
static int connectTest;	// 通信が行われたか

static boolean nCheck;

static const int HOST = 1;		// ホスト接続前
static const int CLIENT = 2;	// クライアント接続前

// 項目最大数
static const int SP_MAX = 2;
static int sPoint;	// 画面の進み度合い
// 0.項目選択
static int yPoint;	// 項目の位置
static boolean load_f = false;


static void Input();
// net.txtのデータを取得する
static void GetPortIp();

// メイン関数
int Network::Mode()
{
	Input();

	// 接続
	if (sPoint == HOST || sPoint == CLIENT) {
		int ret = netData->connect_Test();
		// 1が返された時
		if (ret == 1) {

			// netDataをSocketに送る
			ChangeNetOn(1);	// ネットワークをオン
			Main_GetNetData(netData);

			Key_SetNetOn(1);
			Key_SetPlayerSide(sPoint);

			// 対戦モードに移行するための設定
			Select_ReturnCursor();
			TraningSwitch(false);
			AISwitch(false);
			Arcade_Switch(0);
			BattleMode(0);
			//DrawString(0, 260, "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||", Cr);
			MainSystem::Instance().SetNextMode("Select");
		}
	}

	//////////////////
	Draw();	// 描画

	// 終了
	if ((sPoint == 0) && (yPoint == SP_MAX) && (P1_BInput(1) == 1 || P1_BInput(2) == 1 || P1_BInput(3) == 1)){
		SEStart(37);
		MainSystem::Instance().SetNextMode("Menu");
	}

	return 0;
}


void Input(){
	// 選択画面？
	if (sPoint == 0){
		if (P1_BInput(102) == 1){
			yPoint++;
			SEStart(36);
		}
		else if (P1_BInput(108) == 1){
			yPoint--;
			SEStart(36);
		}

		// 最大最小幅
		if (yPoint < 0)yPoint = SP_MAX;
		else if (yPoint > SP_MAX)yPoint = 0;

		// ホストに
		if (yPoint == 0 && (P1_BInput(1) == 1 || P1_BInput(3) == 1)){
			// ネットワークテスト
			//netData->setPort(7500);
			
			netData->MakeUDPNetHandle();
			netData->connect_Reset();
			netData->setPlayerSide(1);
			Data[0] = 1;
			Data[1] = 0;
		}
		// クライアントに
		else if (yPoint == 1 && (P1_BInput(1) == 1 || P1_BInput(3) == 1)){
			// ネットワークテスト
			//netData->setPort(7600);
			
			netData->MakeUDPNetHandle();	// 指定したポートで作成
			netData->connect_Reset();
			netData->setPlayerSide(2);
			Data[0] = 0;
			Data[1] = 1;
		}

		// 決定時の処理
		if (yPoint < 2 && ((P1_BInput(1) == 1 || P1_BInput(3) == 1))) {
			sPoint = yPoint + 1;
			yPoint = 0;
			SEStart(35);
		}

		// 終了に
		if (P1_BInput(2) == 2){
			sPoint = 0;
			yPoint = SP_MAX;
			SEStart(37);
		}
	}
	else if (sPoint == HOST){
		// 選択へ
		if (P1_BInput(2) == 2){
			sPoint = 0;
			yPoint = 0;
			// ＵＤＰソケットハンドルの削除
			netData->DeleteUDPNetHandle();
			netData->setPlayerSide(0);
			for (int i = 0; i < 2; i++) {
				Data[i] = 0;
				getData[i] = 0;
			}
			SEStart(37);
		}
	}
	else if (sPoint == CLIENT){
		// 選択へ
		if (P1_BInput(2) == 2){
			sPoint = 0;
			yPoint = 1;
			// ＵＤＰソケットハンドルの削除
			netData->DeleteUDPNetHandle();
			netData->setPlayerSide(0);
			for (int i = 0; i < 2; i++) {
				Data[i] = 0;
				getData[i] = 0;
			}
			SEStart(37);
		}
	}
}

void Network::Draw(){
	// ポインター
	DrawBox(0, yPoint * 20, 20, (yPoint + 1) * 20, Cr, true);
	// テスト用位置表示
	//DrawFormatString(420, 240, Cr, "S..%d", sPoint);
	//DrawFormatString(420, 260, Cr, "X..%d", xPoint);
	//DrawFormatString(420, 280, Cr, "Y..%d", yPoint);
	//DrawFormatString(0, SCREEN_H - 40, Cr, "1P..%d, 2P..%d", Data[0], Data[1]);	// 接続確認
	//netData->DataDraw(240, 40);

	// 1Pと2Pの状況表示
	if (Data[0] == 2)
		DrawString(0, SCREEN_H - 40, "1P..成功", Cr);
	else if (Data[0] == 1)
		DrawString(0, SCREEN_H - 40, "1P..接続待ち", Cr);
	else if(Data[0] == 0){
		DrawString(0, SCREEN_H - 40, "1P..停止", Cr);
	}

	if (Data[1] == 2)
		DrawString(120, SCREEN_H - 40, "2P..成功", Cr);
	else if (Data[1] == 1)
		DrawString(120, SCREEN_H - 40, "2P..接続待ち", Cr);
	else if(Data[1] == 0){
		DrawString(120, SCREEN_H - 40, "2P..停止", Cr);
	}

	// 通信が行われたか
	DrawFormatString(320, SCREEN_H - 40, Cr, "通信..%d", connectTest);

	if (sPoint == 0){
		DrawString(20, 20 * 0, "ホスト", Cr);
		DrawString(20, 20 * 1, "クライアント", Cr);
		/*
		// IP数値
		for (int i = 0; i < ip_x; i++){
			for (int j = 0; j < ip_y; j++){
				DrawFormatString(20 + (j * 10) + (i * 40), 20 * 2, Cr, "%d", ip[i][j]);
			}
			if (i != 3)DrawString(20 + (i * 40) + 30, 20 * 2, ".", Cr);
		}
		// ポート数値
		for (int i = 0; i < 5; i++){
			DrawFormatString(20 + (i * 10), 20 * 3, Cr, "%d", port[i]);
		}
		*/
		DrawString(20, 20 * 2, "終了", Cr);

		// 項目の説明
		switch (yPoint){
		case 0:
			DrawString(80, 20 * 6, "自分が接続を待つ", Cr);
			break;
		case 1:
			DrawString(80, 20 * 6, "自分が接続する", Cr);
			break;
		case SP_MAX:
			DrawString(80, 20 * 6, "終了する", Cr);
			break;
		}
	}
	else if (sPoint == HOST) {
		DrawString(20, 20 * 0, "ホスト", Cr);
		if (!nCheck)DrawString(20, 20 * 1, "待機中", Cr);
		else { DrawString(20, 20 * 1, "接続しました", Cr); }
		DrawFormatString(20, 20 * 2, Cr, "%d", netData->getPort());
		DrawFormatString(20, 20 * 3, Cr, "クライアント %d.%d.%d.%d",
			netData->ip.d1, netData->ip.d2, netData->ip.d3, netData->ip.d4);
		//DrawFormatString(20, SCREEN_H - 40, Cr, "%d.%d", Data[0], Data[1]);
	}
	else if (sPoint == CLIENT) {
		DrawString(20, 20 * 0, "クライアント", Cr);
		if (!nCheck)DrawString(20, 20 * 1, "待機中", Cr);
		else { DrawString(20, 20 * 1, "接続しました", Cr); }
		// ポートも一緒に表示
		DrawFormatString(20, 20 * 3, Cr, "%d.%d.%d.%d:%d",
			netData->ip.d1, netData->ip.d2, netData->ip.d3, netData->ip.d4, netData->getPort());
		//DrawFormatString(20, SCREEN_H - 40, Cr, "%d.%d", Data[0], Data[1]);
	}

	// データ表示
	netData->DataDraw(10,320);
}


int Network::End() {
	return 0;
}

void Network::Load_Reload() {
	if (!load_f) {
		// ここに入力
		load_f = true;
	}

	sPoint = 0;
	yPoint = 0;
	nCheck = false;

	// 取得データを初期化
	for (int i = 0; i < sizeof(Data); i++) {
		Data[i] = 0;
		getData[i] = 0;
	}

	// ポート、IP取得
	GetPortIp();

	// ネットデータを初期化
	//netData->GetIp(Ip);
	//netData->GetPort(Port);
	//netData->GetUDPNetHandle(UDPNetHandle);

	// 通信テスト
	connectTest = 0;
}

// 一度読み込み
void Network::Load_1second() {
	netData = new NetData();
}

void Network::Release(void)
{
}

void GetPortIp()
{
	// 判定セッティング //

	int i, fp;

	char fname[8] = { "net.txt" };

	// 名前読み込み
	char t_port[5] = "PORT";
	char t_ip[3] = "IP";
	char t_delay_max[10] = "DELAY_MAX";
	IPDATA Ip = {0,0,0,0};	// 代入用

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// inputとinputcに文字がはいる
	boolean iflg = false;	// 名前入力フラグ
	int flg = 0;	// 0:なし 1:PORT 2:IP
	int ip_flg = 0;

	// ファイルを開く //
	fp = FileRead_open(fname);//ファイル読み込み
	if (fp == NULL) {			// エラーが起こったら
		printfDx("error name\n");// 文字を表示
		return;
	}

	while (1) {
		//=========================
		// 基本ループ
		// このループ終了後に下へ
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1文字取得する

			// スラッシュで改行
			if (inputc[i] == '/') {					//スラッシュがあれば
				while (FileRead_getc(fp) != '\n');	//改行までループ
				i = -1;//カウンタを最初に戻して (戻ったときに0になる)
				continue;
			}

			// 改行で次の行に進む
			if (input[i] == '\n') {
				for (int j = 0; j < NAME_MAX; j++) {
					//inputc[j] = input[j] = NULL;
					inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				}
				i = -1;
				break;
			}

			// 文字列にする
			if (input[i] == '.' || input[i] == ' ' ||
				input[i] == ';' || input[i] == '=') { //カンマか改行なら
				inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				break;	// iを1増やしてループ脱出（代入する）
			}

			// 読み込み終了
			if (input[i] == EOF) {//ファイルの終わりなら
				goto EXFILE;//終了
			}
		}	// 基本ループ終了

		// 最初に数字扱いにして、文字が入ってたら変更する
		iflg = false;

		// 文字列セットに移行していｒ
		if (flg > 0)
		{
			if (inputc[0] == '0')iflg = true;
			else if (inputc[0] == '1')iflg = true;
			else if (inputc[0] == '2')iflg = true;
			else if (inputc[0] == '3')iflg = true;
			else if (inputc[0] == '4')iflg = true;
			else if (inputc[0] == '5')iflg = true;
			else if (inputc[0] == '6')iflg = true;
			else if (inputc[0] == '7')iflg = true;
			else if (inputc[0] == '8')iflg = true;
			else if (inputc[0] == '9')iflg = true;
		}

		// 名前だったら
		if (iflg) {
			// PORT
			if (flg == 1) {
				int port = atoi(inputc);
				netData->setPort(port);
				flg = 0;
			}
			// IP
			if (flg == 2) {
				// 先にセット
				int atoiNum = atoi(inputc);
				// 範囲
				if (atoiNum < 0)atoiNum = 0;
				else if (atoiNum > 255)atoiNum = 255;

				if (ip_flg == 1)Ip.d1 = atoiNum;
				if (ip_flg == 2)Ip.d2 = atoiNum;
				if (ip_flg == 3)Ip.d3 = atoiNum;
				if (ip_flg == 4)Ip.d4 = atoiNum;
				ip_flg++;
				// 全て終了
				if (ip_flg > 4) {
					netData->setIp(Ip);
					flg = 0, ip_flg = 0;
				}
			}
			// DELAY_MAX
			if (flg == 3) {
				int delayMax = atoi(inputc);
				netData->setDelayMax(delayMax);
				flg = 0;
			}
		}

		// 数字ではない
		if (!iflg) {
			// "PORT"
			if (strcmp(inputc, t_port) == 0) {
				flg = 1;
			}
			// "IP"
			if (strcmp(inputc, t_ip) == 0) {
				flg = 2;
				ip_flg = 1;
			}
			// "IP"
			if (strcmp(inputc, t_delay_max) == 0) {
				flg = 3;
			}
		}
	}

	// ファイルを閉じる
EXFILE:
	FileRead_close(fp);

}

