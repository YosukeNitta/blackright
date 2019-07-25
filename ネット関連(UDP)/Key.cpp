#include "pch.h"
#pragma warning(disable : 4996)	// fscanfのエラー表示を消す

#define CO_MAX (4 + BUTTON_MAX)	// 上下左右、8ボタン

// ネット対戦
static boolean network = false;
static IPDATA Ip;
static int Port;
static int network_side = 0;	// 1,1P 2,2P

static const int PUT_MAX = 10;	// 記録するフレームの最大
static const int DATA_MAX = 10;
static BYTE Data[DATA_MAX];	// 入力(確定したものを入れる)
static BYTE gData[DATA_MAX];	// 入力(相手なので遅らせる必要はない)

static BYTE getData[DATA_MAX];	// 取得した入力情報
static BYTE myData[DATA_MAX];	// 自分の入力情報
static BYTE getMyData[DATA_MAX];	// 自分の入力情報

static BYTE dataLog[PUT_MAX][DATA_MAX];	// 入力(確定したものを入れる)
//static BYTE gDataLog[PUT_MAX][DATA_MAX];	// 入力(相手なので遅らせる必要はない)

static BYTE netD[DATA_MAX];	// 相手からの受け取り用

static int net_time = 1;

static int UDPNetHandle;        // ネットワークハンドル

static int NetHandle, LostHandle;    // ネットワークハンドル
static int DataLength;            // 受信データ量保存用変数

// 1P、2Pのパッド接続
static int m_P_Input[2];

// ボタンの要素（実際に使うもの）
static int P1_B[PAD_MAX], P2_B[PAD_MAX];
static int P1_NB[PAD_MAX], P2_NB[PAD_MAX];
// ボタンの条件
static int P1_I[PAD_MAX], P2_I[PAD_MAX];

// 操作の可否
static int ctrl[2];

static int button3[8];

static int mOld_B[PAD_MAX];

// 俺たちに任せろ
static int key[PAD_MAX];
static int pad[PAD_MAX];
static int co_k[2][CO_MAX];
static int co_p[2][CO_MAX];

static int Load_1;

static void NumSetting();
static int load_config();

static void Send_NetKey();
static void Get_NetKey();
static void Send_YourKey();
static void Get_MyKey();

// パッドが接続されているか確認
void GetInput()
{
	// 宣言


	{
		// P1
		int i;
		i = GetJoypadInputState(DX_INPUT_PAD1);
		m_P_Input[0] = i;	// iを代入

		// P2
		int j;
		j = GetJoypadInputState(DX_INPUT_PAD2);
		m_P_Input[1] = j; // jを代入
	}

}


// 接続のみ確認
int P_Input(int n)
{
	return m_P_Input[n - 1];
}


// ボタン入力チェック
// ボタン配置をいじる場合はここで

int P1_BCheck()
{
	if (!network){	//
		// P1の 上要素 が押されていたら
		if (((m_P_Input[0] & pad[co_p[0][0]]) != 0) ||
			(CheckHitKey(key[co_k[0][0]]) == 1)){
			P1_B[108] += 1;
		}
		else{
			P1_B[108] = 0;
		}
		// 下
		if (((m_P_Input[0] & pad[co_p[0][1]]) != 0) ||
			(CheckHitKey(key[co_k[0][1]]) == 1)){
			P1_B[102] += 1;
		}
		else{
			P1_B[102] = 0;
		}
		// 左
		if (((m_P_Input[0] & pad[co_p[0][2]]) != 0) ||
			(CheckHitKey(key[co_k[0][2]]) == 1)){
			P1_B[104] += 1;
		}
		else{
			P1_B[104] = 0;
		}
		// 右
		if (((m_P_Input[0] & pad[co_p[0][3]]) != 0) ||
			(CheckHitKey(key[co_k[0][3]]) == 1)){
			P1_B[106] += 1;
		}
		else{
			P1_B[106] = 0;
		}


		// 1ボタン	A
		if (
			((m_P_Input[0] & pad[co_p[0][4]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][4]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P1_B[1] ++;
		}
		else{
			P1_B[1] = 0;
		}
		// 2ボタン	B
		if (
			((m_P_Input[0] & pad[co_p[0][5]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][5]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)
			){
			P1_B[2] += 1;
		}
		else{
			P1_B[2] = 0;
		}
		// 3ボタン	C
		if (
			((m_P_Input[0] & pad[co_p[0][6]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||

			(CheckHitKey(key[co_k[0][6]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)
			){
			P1_B[3] += 1;
		}
		else{
			P1_B[3] = 0;
		}
		// 4ボタン	D
		if (((m_P_Input[0] & pad[co_p[0][7]]) != 0) ||
			(CheckHitKey(key[co_k[0][7]]) == 1)
			){
			P1_B[4] += 1;
		}
		else{
			P1_B[4] = 0;
		}
		// 5ボタン	A+B+C
		if (((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)){
			P1_B[5] += 1;
		}
		else{
			P1_B[5] = 0;
		}
		// 6ボタン	A+B (投げ)
		if (((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P1_B[6] += 1;
		}
		else{
			P1_B[6] = 0;
		}
		// 7ボタン	PAUSE
		if (((m_P_Input[0] & pad[co_p[0][10]]) != 0) ||
			(CheckHitKey(key[co_k[0][10]]) == 1)){
			P1_B[7] += 1;
		}
		else{
			P1_B[7] = 0;
		}
		// 8ボタン  トレモ(1)
		if (((m_P_Input[0] & pad[co_p[0][11]]) != 0) ||
			(CheckHitKey(key[co_k[0][11]]) == 1)){
			P1_B[8] += 1;
		}
		else{
			P1_B[8] = 0;
		}
		// 9ボタン  トレモ(2)
		if (((m_P_Input[0] & pad[co_p[0][12]]) != 0) ||
			(CheckHitKey(key[co_k[0][12]]) == 1)){
			P1_B[9] += 1;
		}
		else{
			P1_B[9] = 0;
		}
	}	// 0
	else if (network_side == 1){	//
		// P1の 上要素 が押されていたら
		if (((m_P_Input[0] & pad[co_p[0][0]]) != 0) ||
			(CheckHitKey(key[co_k[0][0]]) == 1)){
			P1_NB[108] += 1;
		}
		else{
			P1_NB[108] = 0;
		}
		// 下
		if (((m_P_Input[0] & pad[co_p[0][1]]) != 0) ||
			(CheckHitKey(key[co_k[0][1]]) == 1)){
			P1_NB[102] += 1;
		}
		else{
			P1_NB[102] = 0;
		}
		// 左
		if (((m_P_Input[0] & pad[co_p[0][2]]) != 0) ||
			(CheckHitKey(key[co_k[0][2]]) == 1)){
			P1_NB[104] += 1;
		}
		else{
			P1_NB[104] = 0;
		}
		// 右
		if (((m_P_Input[0] & pad[co_p[0][3]]) != 0) ||
			(CheckHitKey(key[co_k[0][3]]) == 1)){
			P1_NB[106] += 1;
		}
		else{
			P1_NB[106] = 0;
		}

		// 1ボタン	A
		if (
			((m_P_Input[0] & pad[co_p[0][4]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][4]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P1_NB[1] += 1;
		}
		else{
			P1_NB[1] = 0;
		}
		// 2ボタン	B
		if (
			((m_P_Input[0] & pad[co_p[0][5]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][5]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)
			){
			P1_NB[2] += 1;
		}
		else{
			P1_NB[2] = 0;
		}
		// 3ボタン	C
		if (
			((m_P_Input[0] & pad[co_p[0][6]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||

			(CheckHitKey(key[co_k[0][6]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)
			){
			P1_NB[3] += 1;
		}
		else{
			P1_NB[3] = 0;
		}
		// 4ボタン	D
		if (((m_P_Input[0] & pad[co_p[0][7]]) != 0) ||
			(CheckHitKey(key[co_k[0][7]]) == 1)
			){
			P1_NB[4] += 1;
		}
		else{
			P1_NB[4] = 0;
		}
		// 5ボタン	A+B (投げ)
		if (((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)){
			P1_NB[5] += 1;
		}
		else{
			P1_NB[5] = 0;
		}
		// 6ボタン	A+B+C
		if (((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P1_NB[6] += 1;
		}
		else{
			P1_NB[6] = 0;
		}
		// 7ボタン	PAUSE
		if (((m_P_Input[0] & pad[co_p[0][10]]) != 0) ||
			(CheckHitKey(key[co_k[0][10]]) == 1)){
			P1_NB[7] += 1;
		}
		else{
			P1_NB[7] = 0;
		}
		// 8ボタン  トレモ(1)
		if (((m_P_Input[0] & pad[co_p[0][11]]) != 0) ||
			(CheckHitKey(key[co_k[0][11]]) == 1)){
			P1_NB[8] += 1;
		}
		else{
			P1_NB[8] = 0;
		}
		// 9ボタン  トレモ(2)
		if (((m_P_Input[0] & pad[co_p[0][12]]) != 0) ||
			(CheckHitKey(key[co_k[0][12]]) == 1)){
			P1_NB[9] += 1;
		}
		else{
			P1_NB[9] = 0;
		}
	}	// 1
	else if (network_side == 2){
		// P1の 上要素 が押されていたら
		if (((m_P_Input[0] & pad[co_p[0][0]]) != 0) ||
			(CheckHitKey(key[co_k[0][0]]) == 1)){
			P2_NB[108] += 1;
		}
		else{
			P2_NB[108] = 0;
		}
		// 下
		if (((m_P_Input[0] & pad[co_p[0][1]]) != 0) ||
			(CheckHitKey(key[co_k[0][1]]) == 1)){
			P2_NB[102] += 1;
		}
		else{
			P2_NB[102] = 0;
		}
		// 左
		if (((m_P_Input[0] & pad[co_p[0][2]]) != 0) ||
			(CheckHitKey(key[co_k[0][2]]) == 1)){
			P2_NB[104] += 1;
		}
		else{
			P2_NB[104] = 0;
		}
		// 右
		if (((m_P_Input[0] & pad[co_p[0][3]]) != 0) ||
			(CheckHitKey(key[co_k[0][3]]) == 1)){
			P2_NB[106] += 1;
		}
		else{
			P2_NB[106] = 0;
		}

		// 1ボタン	A
		if (
			((m_P_Input[0] & pad[co_p[0][4]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][4]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P2_NB[1] += 1;
		}
		else{
			P2_NB[1] = 0;
		}
		// 2ボタン	B
		if (
			((m_P_Input[0] & pad[co_p[0][5]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][5]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)
			){
			P2_NB[2] += 1;
		}
		else{
			P2_NB[2] = 0;
		}
		// 3ボタン	C
		if (
			((m_P_Input[0] & pad[co_p[0][6]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||

			(CheckHitKey(key[co_k[0][6]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)
			){
			P2_NB[3] += 1;
		}
		else{
			P2_NB[3] = 0;
		}
		// 4ボタン	D
		if (((m_P_Input[0] & pad[co_p[0][7]]) != 0) ||
			(CheckHitKey(key[co_k[0][7]]) == 1)
			){
			P2_NB[4] += 1;
		}
		else{
			P2_NB[4] = 0;
		}
		// 5ボタン	A+B (投げ)
		if (((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)){
			P2_NB[5] += 1;
		}
		else{
			P2_NB[5] = 0;
		}
		// 6ボタン	A+B+C
		if (((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)){
			P2_NB[6] += 1;
		}
		else{
			P2_NB[6] = 0;
		}
		// 7ボタン	PAUSE
		if (((m_P_Input[0] & pad[co_p[0][10]]) != 0) ||
			(CheckHitKey(key[co_k[0][10]]) == 1)){
			P2_NB[7] += 1;
		}
		else{
			P2_NB[7] = 0;
		}
		// 8ボタン  トレモ(1)
		if (((m_P_Input[0] & pad[co_p[0][11]]) != 0) ||
			(CheckHitKey(key[co_k[0][11]]) == 1)){
			P2_NB[8] += 1;
		}
		else{
			P2_NB[8] = 0;
		}
		// 9ボタン  トレモ(2)
		if (((m_P_Input[0] & pad[co_p[0][12]]) != 0) ||
			(CheckHitKey(key[co_k[0][12]]) == 1)){
			P2_NB[9] += 1;
		}
		else{
			P2_NB[9] = 0;
		}
	}// 2


	return 0;	// 終わりっ！閉廷！
}

// ボタン入力チェック
// ボタン配置をいじる場合はここで

int P2_BCheck()
{
	// 通常の2P操作
	if (!network){
		// P1の 上要素 が押されていたら
		if (((m_P_Input[1] & pad[co_p[1][0]]) != 0) ||
			(CheckHitKey(key[co_k[1][0]]) == 1)){
			P2_B[108] += 1;
		}
		else{
			P2_B[108] = 0;
		}
		// 下
		if (((m_P_Input[1] & pad[co_p[1][1]]) != 0) ||
			(CheckHitKey(key[co_k[1][1]]) == 1)){
			P2_B[102] += 1;
		}
		else{
			P2_B[102] = 0;
		}
		// 左
		if (((m_P_Input[1] & pad[co_p[1][2]]) != 0) ||
			(CheckHitKey(key[co_k[1][2]]) == 1)){
			P2_B[104] += 1;
		}
		else{
			P2_B[104] = 0;
		}
		// 右
		if (((m_P_Input[1] & pad[co_p[1][3]]) != 0) ||
			(CheckHitKey(key[co_k[1][3]]) == 1)){
			P2_B[106] += 1;
		}
		else{
			P2_B[106] = 0;
		}

		// 1ボタン	A
		if (
			((m_P_Input[1] & pad[co_p[1][4]]) != 0) ||
			((m_P_Input[1] & pad[co_p[1][8]]) != 0) ||
			((m_P_Input[1] & pad[co_p[1][9]]) != 0) ||

			(CheckHitKey(key[co_k[1][4]]) == 1) ||
			(CheckHitKey(key[co_k[1][8]]) == 1) ||
			(CheckHitKey(key[co_k[1][9]]) == 1)){
			P2_B[1] ++;
		}
		else{
			P2_B[1] = 0;
		}
		// 2ボタン	B
		if (
			((m_P_Input[1] & pad[co_p[1][5]]) != 0) ||
			((m_P_Input[1] & pad[co_p[1][8]]) != 0) ||
			((m_P_Input[1] & pad[co_p[1][9]]) != 0) ||

			(CheckHitKey(key[co_k[1][5]]) == 1) ||
			(CheckHitKey(key[co_k[1][8]]) == 1) ||
			(CheckHitKey(key[co_k[1][9]]) == 1)
			){
			P2_B[2] += 1;
		}
		else{
			P2_B[2] = 0;
		}
		// 3ボタン	C
		if (
			((m_P_Input[1] & pad[co_p[1][6]]) != 0) ||
			((m_P_Input[1] & pad[co_p[1][8]]) != 0) ||

			(CheckHitKey(key[co_k[1][6]]) == 1) ||
			(CheckHitKey(key[co_k[1][8]]) == 1)
			){
			P2_B[3] += 1;
		}
		else{
			P2_B[3] = 0;
		}
		// 4ボタン	D
		if (((m_P_Input[1] & pad[co_p[1][7]]) != 0) ||
			(CheckHitKey(key[co_k[1][7]]) == 1)
			){
			P2_B[4] += 1;
		}
		else{
			P2_B[4] = 0;
		}
		// 5ボタン	A+B (投げ)
		if (((m_P_Input[1] & pad[co_p[1][8]]) != 0) ||
			(CheckHitKey(key[co_k[1][8]]) == 1)){
			P2_B[5] += 1;
		}
		else{
			P2_B[5] = 0;
		}
		// 6ボタン	A+B+C
		if (((m_P_Input[1] & pad[co_p[1][9]]) != 0) ||
			(CheckHitKey(key[co_k[1][9]]) == 1)){
			P2_B[6] += 1;
		}
		else{
			P2_B[6] = 0;
		}
		// 7ボタン	PAUSE
		if (((m_P_Input[1] & pad[co_p[1][10]]) != 0) ||
			(CheckHitKey(key[co_k[1][10]]) == 1)){
			P2_B[7] += 1;
		}
		else{
			P2_B[7] = 0;
		}
		// 8ボタン  トレモ(1)
		if (((m_P_Input[1] & pad[co_p[1][11]]) != 0) ||
			(CheckHitKey(key[co_k[1][11]]) == 1)){
			P2_B[8] += 1;
		}
		else{
			P2_B[8] = 0;
		}
		// 9ボタン  トレモ(2)
		if (((m_P_Input[1] & pad[co_p[1][12]]) != 0) ||
			(CheckHitKey(key[co_k[1][12]]) == 1)){
			P2_B[9] += 1;
		}
		else{
			P2_B[9] = 0;
		}
	}	// ネットワークに繋がっていない場合のみ行う


	// ネットワーク処理
	else if (network){
		net_time++;
		if (net_time > 10)net_time = 1;

		// データが繋がってる
		{
			// 自分の送信データ決定
			Main_GetNetwork(true);
			if (network_side == 1){
				for (int i = 0; i < DATA_MAX; i++){
					if (myData[i] > 60)myData[i] = 60;
				}
				myData[0] = net_time;
				myData[1] = P1_NB[1];
				myData[2] = P1_NB[2];
				myData[3] = P1_NB[3];
				myData[4] = P1_NB[4];
				myData[5] = P1_NB[102];
				myData[6] = P1_NB[104];
				myData[7] = P1_NB[106];
				myData[8] = P1_NB[108];
				myData[9] = P1_NB[7];
			}
			else if (network_side == 2){
				for (int i = 0; i < DATA_MAX; i++){
					if (myData[i] > 60)myData[i] = 60;
				}
				myData[0] = net_time;
				myData[1] = P2_NB[1];
				myData[2] = P2_NB[2];
				myData[3] = P2_NB[3];
				myData[4] = P2_NB[4];
				myData[5] = P2_NB[102];
				myData[6] = P2_NB[104];
				myData[7] = P2_NB[106];
				myData[8] = P2_NB[108];
				myData[9] = P2_NB[7];
			}
			//
			/////////////////////////////////////////////

			// 前のキーから入力呼び出し
			/*
			for (int i = 0; i < 10; i++){
				// 入力ない時に過去のデータにはある場合
				if (Data[i][3] == 0){
					if (Data[i + 10][2] > 0){
						Data[i][3] = Data[i + 10][2];
						if (Data[i + 20][1] > 0){
							Data[i][3] = Data[i + 20][1];
						}
					}
				}
			}
			*/
			/////////////////////////////////////////////

		{
			// 受信データ初期化
			for (int i = 0; i < DATA_MAX; i++){
				Data[i] = 0;
				gData[i] = 0;	// コマンド
				//myData[i] = 0;
				getData[i] = 0;
				getMyData[i] = 0;

				netD[i] = 0;
			}
			// UDP
			///////////////////////
			int cnwr_ret = CheckNetWorkRecvUDP(UDPNetHandle);
			if (cnwr_ret && cnwr_ret != -1){
				// データ受け取り
				if (network_side == 1){
					Send_NetKey();
					Get_NetKey();
					//Send_YourKey();
					//Get_MyKey();
				}
				else if (network_side == 2){
					Get_NetKey();
					Send_NetKey();
					//Get_MyKey();
					//Send_YourKey();
				}
			}
			else{
				//Main_GetNetwork(false);
				if (network_side == 1){
					for (int i = 0; i < PAD_MAX; i++){
						P1_NB[i]--;
						if (P1_NB[i] < 0)P1_NB[i] = 0;
					}
				}
				else if (network_side == 2){
					for (int i = 0; i < PAD_MAX; i++){
						P2_NB[i]--;
						if (P2_NB[i] < 0)P2_NB[i] = 0;
					}
				}
			}
			///////////////////////

			for (int data = 0; data < DATA_MAX; data++){
				dataLog[0][data] = myData[data];
			}

			// 自分と相手のデータ取得完了
			// 次に進める
			for (int i = 0; i < PUT_MAX - 1; i++){
				for (int data = 0; data < DATA_MAX; data++){
					// 1P
					dataLog[i+1][data] = dataLog[i][data];
					// 2P
					//gDataLog[i + 1][data] = gDataLog[i][data];
				}
			}

			//取得した相手のキーと時間が同じなら進める
			for (int i = 0; i < PUT_MAX; i++){
				// ログと時間が同じ
				if (getData[0] == dataLog[i][0]){
					for (int data = 0; data < DATA_MAX; data++){
						Data[data] = dataLog[i][data];
						gData[data] = getData[data];
					}
				}

			}

			///////////////////////
		}


		// ボタン決定
		if (network_side == 1){
			// 1P
			P1_B[1] = Data[1];
			P1_B[2] = Data[2];
			P1_B[3] = Data[3];
			P1_B[4] = Data[4];
			P1_B[102] = Data[5];
			P1_B[104] = Data[6];
			P1_B[106] = Data[7];
			P1_B[108] = Data[8];
			//P1_B[7] = Data3[9];
			// 2P
			P2_B[1] = gData[1];
			P2_B[2] = gData[2];
			P2_B[3] = gData[3];
			P2_B[4] = gData[4];
			P2_B[102] = gData[5];
			P2_B[104] = gData[6];
			P2_B[106] = gData[7];
			P2_B[108] = gData[8];
		}
		else if (network_side == 2){
			// 1P
			P1_B[1] = gData[1];
			P1_B[2] = gData[2];
			P1_B[3] = gData[3];
			P1_B[4] = gData[4];
			P1_B[102] = gData[5];
			P1_B[104] = gData[6];
			P1_B[106] = gData[7];
			P1_B[108] = gData[8];
			// 2P
			P2_B[1] = Data[1];
			P2_B[2] = Data[2];
			P2_B[3] = Data[3];
			P2_B[4] = Data[4];
			P2_B[102] = Data[5];
			P2_B[104] = Data[6];
			P2_B[106] = Data[7];
			P2_B[108] = Data[8];
		}



		}	// 全体終了
	}// ネットワーク処理


	return 0;	// 終わりっ！閉廷！
}

void InputSend()
{
	// P1,P2の変数に保存
	for (int i = 0; i < PAD_MAX; i++){
		P1.button[i] = P1_B[i];
		P2.button[i] = P2_B[i];
	}


	// 先行入力の設定、とりま6フレ
	for (int i = 1; i < BUTTON_MAX; i++){

		// キャラ＆画面が動いてるときは先行入力が減る
		if (S.StopTime == 0){
			if (P1.StopTime == 0){
				if (P1.Senkou[i] > 0)P1.Senkou[i] -= 1;
				if (P1.rbutton[i] > 0)P1.rbutton[i] -= 1;
			}
			if (P2.StopTime == 0){
				if (P2.Senkou[i] > 0)P2.Senkou[i] -= 1;
				if (P2.rbutton[i] > 0)P2.rbutton[i] -= 1;
			}
		}

		// リバサ(ガード、起き上がり)
		if ((P1.stateno >= 50 && P1.stateno < 60) ||
			(P1.stateno == 1055)){
			if (P1.button[i] == 1)P1.rbutton[i] = 6;
		}
		else{ if (P1.button[i] == 1)P1.rbutton[i] = 1; }
		if ((P2.stateno >= 50 && P2.stateno < 60) ||
			(P2.stateno == 1055)){
			if (P2.button[i] == 1)P2.rbutton[i] = 6;
		}
		else{ if (P2.button[i] == 1)P2.rbutton[i] = 1; }

		// 入力した瞬間に先行入力6フレ追加
		// おしっぱで猶予が伸びる
		//if (P1.button[i] >= 1 && P1.button[i] <= 5)P1.Senkou[i] = 6;
		//if (P2.button[i] >= 1 && P2.button[i] <= 5)P2.Senkou[i] = 6;
		if (P1.button[i] == 1)P1.Senkou[i] = 6;
		if (P2.button[i] == 1)P2.Senkou[i] = 6;
	}

	// リバサ入力
	ctrl[0] = P1.ctrl;
	ctrl[1] = P2.ctrl;

	// テンキー位置判断、mukiが逆だと左右も変わる（常に右向き）

	Player P[2];
	P[0] = P1, P[1] = P2;

	for (int i = 0; i < 2; i++)
	{

		// まずは上
		if (P[i].button[108] > 0){
			if (P[i].button[104] > 0)P[i].keyPos = 7;
			else if (P[i].button[106] > 0)P[i].keyPos = 9;
			else { P[i].keyPos = 8; }
		}
		// 次に下
		else if (P[i].button[102] > 0){
			if (P[i].button[104] > 0)P[i].keyPos = 1;
			else if (P[i].button[106] > 0)P[i].keyPos = 3;
			else { P[i].keyPos = 2; }
		}
		// そして左
		else if (P[i].button[104] > 0)P[i].keyPos = 4;
		// さらに右
		else if (P[i].button[106] > 0)P[i].keyPos = 6;
		// トドメにニュートラル
		else{ P[i].keyPos = 5; }

		// あと逆にする
		if (P[i].muki == 1){
			if (P[i].keyPos == 1)P[i].keyPos = 3;
			else if (P[i].keyPos == 3)P[i].keyPos = 1;
			else if (P[i].keyPos == 4)P[i].keyPos = 6;
			else if (P[i].keyPos == 6)P[i].keyPos = 4;
			else if (P[i].keyPos == 7)P[i].keyPos = 9;
			else if (P[i].keyPos == 9)P[i].keyPos = 7;
		}


		//**************
		// キー属性決定
		//**************

		// まずは上下
		if (P[i].button[108] > 0)P[i].keyAtt[8] = 1;
		else{ P[i].keyAtt[8] = 0; }
		if (P[i].button[102] > 0)P[i].keyAtt[2] = 1;
		else{ P[i].keyAtt[2] = 0; }

		// 左右
		if (P[i].muki == 0){
			if (P[i].button[104] > 0)P[i].keyAtt[4] = 1;
			else{ P[i].keyAtt[4] = 0; }
			if (P[i].button[106] > 0)P[i].keyAtt[6] = 1;
			else{ P[i].keyAtt[6] = 0; }
		}
		else{
			if (P[i].button[106] > 0)P[i].keyAtt[4] = 1;
			else{ P[i].keyAtt[4] = 0; }
			if (P[i].button[104] > 0)P[i].keyAtt[6] = 1;
			else{ P[i].keyAtt[6] = 0; }
		}


		// 斜め
		if (P[i].keyAtt[2] > 0){
			if (P[i].keyAtt[4] > 0)P[i].keyAtt[1] = 1;
			else{ P[i].keyAtt[1] = 0; }
			if (P[i].keyAtt[6] > 0)P[i].keyAtt[3] = 1;
			else{ P[i].keyAtt[3] = 0; }
		}
		else{
			P[i].keyAtt[1] = 0;
			P[i].keyAtt[3] = 0;
		}

		if (P[i].keyAtt[8] > 0){
			if (P[i].keyAtt[4] > 0)P[i].keyAtt[7] = 1;
			else{ P[i].keyAtt[7] = 0; }
			if (P[i].keyAtt[6] > 0)P[i].keyAtt[9] = 1;
			else{ P[i].keyAtt[9] = 0; }
		}
		else{
			P[i].keyAtt[7] = 0;
			P[i].keyAtt[9] = 0;
		}

		// [Key] 先行入力
		for (int j = 1; j < 9 + 1; j++){
			if (P[i].keyAtt[j] == 1){
				P[i].key[j] ++;
			}
			else{
				P[i].key[j] = 0;
			}

			// キャラ＆画面が動いてるときは先行入力が減る
			if (S.StopTime == 0){
				if (P[i].StopTime == 0){
					if (P[i].K_Senkou[j] > 0)P[i].K_Senkou[j] -= 1;
					if (P[i].K_Senkou[j] < 0)P[i].K_Senkou[j] = 0;
				}
			}

			// 入力した瞬間に先行入力５フレ追加
			if (P[i].key[j] == 1)P[i].K_Senkou[j] = 6;
		}

	}
	P1 = P[0], P2 = P[1];


	Get_PSSet(P1, P2, S);		// 値を返す

}


// 1P側のボタン入力判断
int P1_BInput(int P1_BNum)
{

	// 注文された値を返す
	return P1_B[P1_BNum];

}

// 2P側
int P2_BInput(int P2_BNum)
{

	// 注文された値を返す
	return P2_B[P2_BNum];

}

// キーを呼び出す
int PH_BInput(int PSide, int P_Num)
{
	int n = 0;
	if (PSide == 1){
		n = P1_B[P_Num];
	}
	else if (PSide == 2){
		n = P2_B[P_Num];
	}
	return n;
}

// どちらかのキーを入力
int P_BInput(int P_Num)
{
	// これに数値をぶっこむ
	int P_InputNum = 0;

	{
		if (P1_B[P_Num] != 0)P_InputNum = P1_B[P_Num];
		if (P2_B[P_Num] != 0)P_InputNum = P2_B[P_Num];
	}

	// 注文された値を返す
	return P_InputNum;

}

// キーの入れっぱチェック
// [問題点]他メニューから戻り時におしっぱしていると、そのまま流れる
boolean KInput(int n)
{
	boolean key;

	// 大きすぎないかチェック
	if (n == 1 || (n >= 30 && n % 5 == 0))key = true;
	else{ key = false; }

	return key;
}

//================================================




int P1_BSet(int num)
{
	int watasu = 0;

	if (((m_P_Input[0] & pad[co_p[0][num]]) != 0)
		//(CheckHitKey(key[co_k[0][num]]) == 1) 
		){
		watasu = 1;
	}

	return watasu;
}

int P2_BSet(int num)
{
	int watasu = 0;

	for (int i = 1; i < PAD_MAX; i++){
		if (P2_I[num] == i){
			if (m_P_Input[1] & pad[i])watasu = 1;
		}
	}

	return watasu;
}


int P1_BConfig(int num1, int num2)
{
	// 簡易キー準備
	if (Load_1 == 0){
		NumSetting();
		load_config();
		Load_1 = 0;
	}

	P1_I[num1] = num2;

	// コンフィグに映る数値
	return num2;
}

int P2_BConfig(int num1, int num2)
{
	P2_I[num1] = num2;

	// コンフィグに映る数値
	return num2;
}

void NumSetting()
{

	key[48] = KEY_INPUT_0;	// ０キー
	key[49] = KEY_INPUT_1;	// １キー
	key[50] = KEY_INPUT_2;	// ２キー
	key[51] = KEY_INPUT_3;	// ３キー
	key[52] = KEY_INPUT_4;	// ４キー
	key[53] = KEY_INPUT_5;	// ５キー
	key[54] = KEY_INPUT_6;	// ６キー
	key[55] = KEY_INPUT_7;	// ７キー
	key[56] = KEY_INPUT_8;	// ８キー
	key[57] = KEY_INPUT_9;	// ９キー

	key[65] = KEY_INPUT_A;	// Ａキー
	key[66] = KEY_INPUT_B;	// Ｂキー
	key[67] = KEY_INPUT_C;	// Ｃキー
	key[68] = KEY_INPUT_D;	// Ｄキー
	key[69] = KEY_INPUT_E;	// Ｅキー
	key[70] = KEY_INPUT_F;	// Ｆキー
	key[71] = KEY_INPUT_G;	// Ｇキー
	key[72] = KEY_INPUT_H;	// Ｈキー
	key[73] = KEY_INPUT_I;	// Ｉキー
	key[74] = KEY_INPUT_J;	// Ｊキー
	key[75] = KEY_INPUT_K;	// Ｋキー
	key[76] = KEY_INPUT_L;	// Ｌキー
	key[77] = KEY_INPUT_M;	// Ｍキー
	key[78] = KEY_INPUT_N;	// Ｎキー
	key[79] = KEY_INPUT_O;	// Ｏキー
	key[80] = KEY_INPUT_P;	// Ｐキー
	key[81] = KEY_INPUT_Q;	// Ｑキー
	key[82] = KEY_INPUT_R;	// Ｒキー
	key[83] = KEY_INPUT_S;	// Ｓキー
	key[84] = KEY_INPUT_T;	// Ｔキー
	key[85] = KEY_INPUT_U;	// Ｕキー
	key[86] = KEY_INPUT_V;	// Ｖキー
	key[87] = KEY_INPUT_W;	// Ｗキー
	key[88] = KEY_INPUT_X;	// Ｘキー
	key[89] = KEY_INPUT_Y;	// Ｙキー
	key[90] = KEY_INPUT_Z;	// Ｚキー

	key[96] = KEY_INPUT_NUMPAD0;	// テンキー０
	key[97] = KEY_INPUT_NUMPAD1;	// テンキー１
	key[98] = KEY_INPUT_NUMPAD2;	// テンキー２
	key[99] = KEY_INPUT_NUMPAD3;	// テンキー３
	key[100] = KEY_INPUT_NUMPAD4;	// テンキー４
	key[101] = KEY_INPUT_NUMPAD5;	// テンキー５
	key[102] = KEY_INPUT_NUMPAD6;	// テンキー６
	key[103] = KEY_INPUT_NUMPAD7;	// テンキー７
	key[104] = KEY_INPUT_NUMPAD8;	// テンキー８
	key[105] = KEY_INPUT_NUMPAD9;	// テンキー９

	key[32] = KEY_INPUT_SPACE;	// スペースキー

	key[37] = KEY_INPUT_LEFT;	// 左キー
	key[38] = KEY_INPUT_UP;		// 上キー
	key[39] = KEY_INPUT_RIGHT;	// 右キー
	key[40] = KEY_INPUT_DOWN;	// 下キー



	pad[4] = PAD_INPUT_1;
	pad[5] = PAD_INPUT_2;
	pad[6] = PAD_INPUT_3;
	pad[7] = PAD_INPUT_4;
	pad[8] = PAD_INPUT_5;
	pad[9] = PAD_INPUT_6;
	pad[10] = PAD_INPUT_7;
	pad[11] = PAD_INPUT_8;
	pad[12] = PAD_INPUT_9;
	pad[13] = PAD_INPUT_10;
	pad[14] = PAD_INPUT_11;
	pad[15] = PAD_INPUT_12;
	pad[16] = PAD_INPUT_13;
	pad[17] = PAD_INPUT_14;
	pad[18] = PAD_INPUT_15;
	pad[19] = PAD_INPUT_16;
	pad[20] = PAD_INPUT_17;
	pad[21] = PAD_INPUT_18;
	pad[22] = PAD_INPUT_19;
	pad[23] = PAD_INPUT_20;
	pad[24] = PAD_INPUT_21;
	pad[25] = PAD_INPUT_22;
	pad[26] = PAD_INPUT_23;
	pad[27] = PAD_INPUT_24;
	pad[28] = PAD_INPUT_25;
	pad[29] = PAD_INPUT_26;
	pad[30] = PAD_INPUT_27;
	pad[31] = PAD_INPUT_28;

	pad[0] = PAD_INPUT_UP;
	pad[1] = PAD_INPUT_DOWN;
	pad[2] = PAD_INPUT_LEFT;
	pad[3] = PAD_INPUT_RIGHT;

}


int Send_PI(int num, int Side)
{
	int re;

	if (Side == 1)re = P1_I[num];
	if (Side == 2)re = P2_I[num];

	return re;
}


//----------------
//
// トレモ用設定
//
//----------------

// 勝手に入力
void P2_Auto(int i)
{
	P2_B[i] = 1;
}

// 2Pを操作
void P2Ctrl()
{
	// 2Pパッド確認
	// m_P_Input[1] = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	for (int i = 0; i < PAD_MAX; i++){
		mOld_B[i] = P1_B[i];	// 1Pの操作を記憶
		P2_B[i] = mOld_B[i];	// 2Pの操作に1Pのを適用
		P1_B[i] = 0;	// 1Pを動かなくする、必須
	}
}

void P2Ctrl2()
{
	for (int i = 0; i < PAD_MAX; i++){

		P1_B[i] = mOld_B[i];	// 終了後に1Pの操作を復活
	}
}

void P2Ctrl3(int num[8])
{
	for (int i = 0; i < 8; i++){
		if (num[i] > 0){
			button3[i] += 1;
		}
		else{ button3[i] = 0; }
	}
}

void P2Ctrl3_2()
{
	P2_B[108] = button3[0];
	P2_B[102] = button3[1];
	P2_B[104] = button3[2];
	P2_B[106] = button3[3];
	P2_B[1] = button3[4];
	P2_B[2] = button3[5];
	P2_B[3] = button3[6];
	P2_B[4] = button3[7];
}

// 簡易AI
void P2_AI(Player GP1, Player GP2)
{
	m_P_Input[1] = 0;	// 2P操作停止
	Player p[2];
	p[0] = GP1, p[1] = GP2;
	int rS = 0, lS = 0;
	int xDist = P2.XPos - P1.XPos;
	if (xDist < 0)xDist = -xDist;

	// P1,P2の変数に保存
	for (int i = 0; i < PAD_MAX; i++){
		P2.button[i] = 0;
		P2_B[i] = 0;
	}

	// 前歩き
	if ((xDist > 180) && (P2.ctrl) && (P2.ctrl) && (P2.stateno != 20)){
		if (GetRand(999) < 100){
			P2.stateno = 20;
		}
	}

	if ((rS > 0) && (P2.muki == 0))P2_B[106]++;
	else if ((rS > 0) && (P2.muki == 1))P2_B[104]++;
	if ((lS > 0) && (P2.muki == 0))P2_B[104]++;
	else if ((lS > 0) && (P2.muki == 0))P2_B[106]++;


	P2_B[7] = 0;	// ポーズは絶対に0
}

void Stop_P2_B()
{
	// P1,P2の変数に保存
	for (int i = 0; i < PAD_MAX; i++){
		P2_B[i] = 0;
	}
}


//----------------
//
// キーコンフィグロード用
//
//----------------
int load_config()
{
	FILE *fp;

	// ハイスコアセーブ用ファイルを開く
	fp = fopen("keyconfig.ini", "rt");

	// 開けなかった場合は何もせずに終了
	if (fp == NULL){ printfDx("read error"); return 0; }


	// インデータの読み込み
	for (int i = 0; i < CO_MAX; i++)
	{
		fscanf(fp, "P1 Key%d = %d\n",
			&i, &co_k[0][i]);
	}
	for (int i = 0; i < CO_MAX; i++)
	{
		fscanf(fp, "P2 Key%d = %d\n",
			&i, &co_k[1][i]);
	}

	for (int i = 0; i < CO_MAX; i++)
	{
		// パッド１
		fscanf(fp, "P1 Pad%d = %d\n",
			&i, &co_p[0][i]
			);
	}
	for (int i = 0; i < CO_MAX; i++)
	{
		// パッド２
		fscanf(fp, "P2 Pad%d = %d\n",
			&i, &co_p[1][i]
			);
	}


	// ファイルを閉じる
	fclose(fp);


	return 0;
}


//----------------
//
// いつもの
//
//----------------

void GetPH_Key(Player GP)
{
	P1 = GP;
}


void GetS_Key(System_t GS)
{
	S = GS;
}

void GetP_Key(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}


//----------------
//
// ネットワーク
//
//----------------
void Get_Network(boolean net)
{
	network = net;

}

// ボタンリセット
void NB_Reset()
{
	for (int num = 0; num < DATA_MAX; num++){
		Data[num] = 0;
		gData[num] = 0;	// 1回だけ行う
	}
	for (int i = 0; i < PAD_MAX; i++){
		P1_NB[i] = 0;
		P2_NB[i] = 0;
	}
}

// ネットワークの情報を取得
void Network_Setting(int side, IPDATA ip, int port, int handle)
{
	network_side = side;
	Ip = ip;
	Port = port;
	UDPNetHandle = handle;
	//NetHandle = handle;
}

// 自分のキーを送る
void Send_NetKey()
{
	// 送り
	//for (int i = 0; i < 4; i++){
		NetWorkSendUDP(UDPNetHandle, Ip, Port, myData, sizeof(myData));
	//}
}

// 相手のキーを取得
void Get_NetKey()
{
	// [ UDP ]
	//for (int i = 0; i < 4; i++){
		while(NetWorkRecvUDP(UDPNetHandle, NULL, NULL, netD, sizeof(netD), FALSE) == 0);	// 一生待つ
		for (int d = 0; d < DATA_MAX; d++){
			if (netD[d] > 0)getData[d] = netD[d];
		}
	//}
	
}

// 取得した相手のキーを送る
void Send_YourKey()
{
	// 送り
	//for (int i = 0; i < 4; i++){
		NetWorkSendUDP(UDPNetHandle, Ip, Port, gData, sizeof(gData));
	//}
}

// 自分のキーを取得(ここでDataに決定)
void Get_MyKey()
{
	// [ UDP ]
	//for (int i = 0; i < 4; i++){
		while(NetWorkRecvUDP(UDPNetHandle, NULL, NULL, netD, sizeof(netD), FALSE) == 0);
		for (int d = 0; d < DATA_MAX; d++){
			if (netD[d] > 0)getMyData[d] = netD[d];
		}
	//}

}

// ネットワーク終了
void End_Network(){

	//[ UDP ]
	// ＵＤＰソケットハンドルの削除
	DeleteUDPSocket(UDPNetHandle);
}