#include "pch.h"

static const int PUT_MAX = 10;	// 記録するフレームの最大
static const int DATA_MAX = 10;

// 接続
static boolean network = false;
// 0.なし 1.ホスト 2.クライアント
static int connectSide = 0;
// 相手のIPアドレス
static IPDATA Ip;
// ネットワークハンドル
static int NetHandle = 0;
// ポート番号
static int Port = 0;

// 待機
static boolean wait;

// [ 通信速度計測 ] //
static int delay = 0;	// 届くまで何フレームか
static int delayTime = 0;
static int delayTime_P1 = 0;
static BYTE delayNum[2];	// 自分の入力情報がどこまでいったか
static int gameMode[2];	// ゲームモード(0:自分 1:相手)

// [ 入力 ] //
// 配列１にフレーム毎の入力、配列２にそのキーとモードがある
// 0..カウント(使ってない) 1～8..キー 9..相手のモード
static BYTE getData[PUT_MAX][DATA_MAX];

//===========================
// データやりとり
//===========================
// ネットワーク取得
void Connect_GetCn(boolean net)
{
	network = net;
}
// ネットワーク確認
boolean Connect_CheckCn()
{
	return network;
}

// プレイヤーサイド取得
void Connect_GetSide(int num)
{
	connectSide = num;
}
// プレイヤーサイド確認
int Connect_CheckSide()
{
	return connectSide;
}

// ポート取得
void Connect_GetPort(int num)
{
	Port = num;
}
// ポート確認
int Connect_CheckPort()
{
	return Port;
}

// ハンドル取得
void Connect_GetNetHandle(int num)
{
	NetHandle = num;
}\
// ハンドル確認
int Connect_CheckNetHandle()
{
	return NetHandle;
}

// IP取得
void Connect_GetIp(IPDATA num)
{
	Ip = num;
}
// IP確認
IPDATA Connect_CheckIp()
{
	return Ip;
}

//===========================
// ディレイ設定
//===========================
// ディレイ速度チェック
void Delay_Check(){

	// 1P > 2P	接続確認
	// 1P < 2P	2Pチェック開始
	// 1P > 2P	1Pチェック開始、2P受信時の経過時間見る
	// 1P < 2P	2P経過時間見る
	// 1P > 2P	1P受け取ったことを報告し続ける
	//
	if (delayTime < 180){
		// 1P
		if (Connect_CheckSide() == 1){
			if (delayNum[1] == 100){
				delayNum[0] = delayTime;	// 自分のディレイタイム
			}
			else{
				delayNum[0] = 100;
			}

			// 1..送信
			NetWorkSendUDP(NetHandle, Ip, Port, delayNum, sizeof(delayNum));

			// 2..自分の速度or相手の初手を受け取る
			if (CheckNetWorkRecvUDP(NetHandle) == TRUE){	// 来たときだけ受け取る
				NetWorkRecvUDP(NetHandle, NULL, NULL, delayNum, sizeof(delayNum), FALSE);
			}
			// 相手のNumに数字が入ってたら、それをディレイにする

		}
		// 2P
		else if (Connect_CheckSide() == 2){
			if (delayNum[0] == 100){
				delayNum[1] = delayTime;	// 自分のディレイタイム
			}
			else{
				delayNum[1] = 100;
			}

			// 1..自分の送信速度測定
			NetWorkSendUDP(NetHandle, Ip, Port, delayNum, sizeof(delayNum));

			// 　 送信確定時のディレイタイムを取得
			if (CheckNetWorkRecvUDP(NetHandle) == TRUE){	// 来たときだけ受け取る
				NetWorkRecvUDP(NetHandle, NULL, NULL, delayNum, sizeof(delayNum), FALSE);
			}

		}

		// ディレイが0(まだ送れてない)
		if (delay == 0){
			// 相手が1以上をおくってきた
			if ((Connect_CheckSide() == 1) && (delayNum[1] > 0) && (delayNum[1] != 100)){
				delay = delayNum[1];	// 相手のDTで
				delayNum[1] = 0;		// 初期化する(これを送らないように)
			}
			else if ((Connect_CheckSide() == 2) && (delayNum[0] > 0) && (delayNum[0] != 100)){
				delay = delayNum[0];
				delayNum[0] = 0;
				// delayNum(最初に受け取ったときのディレイタイム);	// 相手のDT
			}
		}

		delayTime++;	// ディレイ時間プラス(10秒は待つ)	
	}
}

// ディレイを描画する
void Delay_Draw(){
	DrawFormatString(0, 0, GetColor(255, 255, 255), "delay %d", delay);
}

// ディレイチェックを終了してるか
boolean Delay_IfEnd(){
	if (delayTime >= 179)return true;
	return false;
}

// ディレイ数値を送る
int Connect_CheckDelay(){
	return delay;
}

// 変数の初期化
void Connect_Load(){
	delayNum[0] = -2;
	delayNum[1] = -2;
	gameMode[0] = 0;
	gameMode[1] = 0;
	Ip.d1 = 0;
	Ip.d2 = 0;
	Ip.d3 = 0;
	Ip.d4 = 0;
	for (int i = 0; i < PUT_MAX; i++){
		for (int j = 0; j < DATA_MAX; j++){
			getData[i][j] = 0;
		}
	}
}

BYTE Connect_CheckGetData(int num1, int num2){
	return getData[num1][num2];
}

//
void Connect_Get_GetData(BYTE num, BYTE num1, BYTE num2){
	getData[num1][num2] = num;
}

void Connect_SetGameMode()
{
	gameMode[0] = CheckGameMode();
	// 1以上なら
	if (getData[0][9] > 0){
		gameMode[1] = getData[0][9];
	}
}

boolean Connect_WaitGameMode()
{
	// 1Pと2Pが違った場合
	if (gameMode[0] != gameMode[1]){

		// セレクトに先に進んだ
		if (gameMode[0] == GameScene(SelectScene)){
			if (gameMode[1] == GameScene(NetworkScene))return true;
			if (gameMode[1] == GameScene(ResultScene))return true;
		}
		// バーサス
		else if (gameMode[0] == GameScene(VersusScene)){
			if (gameMode[1] == GameScene(SelectScene))return true;
		}
		// リザルト
		else if (gameMode[0] == GameScene(ResultScene)){
			if (gameMode[1] == GameScene(VersusScene))return true;
		}
	}
	return false;
}

/*
// ネットワークの情報を取得
void Set_Network(int side, IPDATA ip, int port, int handle)
{
	connectSide = side;
	Ip = ip;
	Port = port;
	NetHandle = handle;
}

// ネットワーク終了
void End_Network(){

	//[ UDP ]
	// ＵＤＰソケットハンドルの削除
	DeleteUDPSocket(NetHandle);
}

*/