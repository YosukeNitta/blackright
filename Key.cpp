#include "pch.h"
#include <iostream>	// 日付取得用
#include <string>
#include "MainSystem.h"
#include "NetData.h"
#pragma warning(disable : 4996)	// fscanfのエラー表示を消す

#define CO_MAX (4 + BUTTON_MAX)	// 上下左右、8ボタン

using namespace std;

//===================================
// [ ネット対戦 ] //
static boolean netOn = false;
static int playerSide = 0;	// 1,1P 2,2P
static BYTE sendKey[KEYLOG_MAX];	// 送信用キー
static BYTE getKey[KEYLOG_MAX];	// 受信用キー
//===================================
// [ UDP ] //
static int UDPNetHandle;        // ネットワークハンドル
static int NetHandle;    // ネットワークハンドル
//static int DataLength;            // 受信データ量保存用変数

//===================================
// [ リプレイ ] //
static int replayMode = 0;	// 0.なし 1.再生 2.録画
static int rTimeCount;
static int ifp;	// 読み込みで使用
static FILE *fp;
static int r_color[2], r_name[2], r_stage;	// リプレイ用データ
static string gString;
static int r_key[2][8];

//===================================
// [ コントローラ設定 ]
static int device[2];	// 0..1P 1..2P
static int d_Input[2][3];	// 0..L 1..R 2..1

// 1P、2Pのパッド接続
static int m_P_Input[2];

// ボタンの要素（実際に使うもの）、これがbuttonに杯ｒ
static vector<int> P1_B;
static vector<int> P2_B;
// 記憶用ボタン(ネット対戦で使用)
static vector<int> P1_VB;

// キーボードでの
static int enter[2];

// 操作の可否
//static int ctrl[2];
static int button3[8];

//static int mOld_B[PAD_MAX];
static vector<int> mOld_B;

// 俺たちに任せろ
static vector<int> key;
static vector<int> pad;
static int co_k[2][CO_MAX];
static int co_p[2][CO_MAX];

static int Load_1;

static boolean stopVB = false;

static void NumSetting();
static int load_config();

void Key_SetSendKey();
static void KeySetting_Network();	
static void Send_NetKey();
static void Get_NetKey();
int Replay_LoadKey();

// パッドが接続されているか確認
void GetInput()
{
	// 宣言
	// P1
	int i;
	i = GetJoypadInputState(DX_INPUT_PAD1);
	m_P_Input[0] = i; // jを代入
	/*
	if (device[0] == 1) {
		i = GetJoypadInputState(DX_INPUT_PAD1);
		m_P_Input[0] = i;
	}
	else if (device[0] == 2) {
		i = GetJoypadInputState(DX_INPUT_PAD2);
		m_P_Input[0] = i;
	}
	else { m_P_Input[0] = 0; }
	*/

	// P2
	int j;
	j = GetJoypadInputState(DX_INPUT_PAD2);
	m_P_Input[1] = j; // jを代入
	/*
	if (device[1] == 1) {
		j = GetJoypadInputState(DX_INPUT_PAD1);
		m_P_Input[1] = j;
	}
	else if (device[1] == 2) {
		j = GetJoypadInputState(DX_INPUT_PAD2);
		m_P_Input[1] = j;
	}
	else { m_P_Input[1] = 0; }
	*/

	/*
	if (j != 0) {
		m_P_Input[0] = j;	// iを代入
		m_P_Input[1] = i;	// iを代入
	}
	*/
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
	if (CheckHitKey(KEY_INPUT_RETURN) == 1)
	{
		enter[0] += 1;
	}
	else { enter[0] = 0; }
	if (CheckHitKey(KEY_INPUT_BACK) == 1)
	{
		enter[1] += 1;
	}
	else { enter[1] = 0; }


	DINPUT_JOYSTATE input;

	GetJoypadDirectInputState(m_P_Input[0], &input);

	// P1の 上要素 が押されていたら
	if (netOn) {
		// 入力停止じゃなければ
		if (!stopVB) {
			if (
				((input.POV[0] == 0) || (input.POV[0] == 4500) || (input.POV[0] == 31500)) ||
				((m_P_Input[0] & pad[co_p[0][0]]) != 0) ||
				(CheckHitKey(key[co_k[0][0]]) == 1)
				)
			{
				P1_VB[108] += 1;
			}
			else {
				P1_VB[108] = 0;
			}

			// 下
			if (
				((input.POV[0] == 18000) || (input.POV[0] == 13500) || (input.POV[0] == 22500)) ||
				((m_P_Input[0] & pad[co_p[0][1]]) != 0) ||
				(CheckHitKey(key[co_k[0][1]]) == 1)) {
				P1_VB[102] += 1;
			}
			else {
				P1_VB[102] = 0;
			}
			// 左
			if (
				((input.POV[0] == 27000) || (input.POV[0] == 22500) || (input.POV[0] == 31500)) ||
				((m_P_Input[0] & pad[co_p[0][2]]) != 0) ||
				(CheckHitKey(key[co_k[0][2]]) == 1)) {
				P1_VB[104] += 1;
			}
			else {
				P1_VB[104] = 0;
			}
			// 右
			if (
				((input.POV[0] == 9000) || (input.POV[0] == 4500) || (input.POV[0] == 13500)) ||
				((m_P_Input[0] & pad[co_p[0][3]]) != 0) ||
				(CheckHitKey(key[co_k[0][3]]) == 1)) {
				P1_VB[106] += 1;
				P1_VB[104] = 0;
			}
			else {
				P1_VB[106] = 0;
			}


			// 1ボタン	A
			if (
				((m_P_Input[0] & pad[co_p[0][4]]) != 0) ||
				((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
				((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

				(CheckHitKey(key[co_k[0][4]]) == 1) ||
				(CheckHitKey(key[co_k[0][8]]) == 1) ||
				(CheckHitKey(key[co_k[0][9]]) == 1)) {
				P1_VB[1] ++;
			}
			else {
				P1_VB[1] = 0;
			}
			// 2ボタン	B
			if (
				((m_P_Input[0] & pad[co_p[0][5]]) != 0) ||
				((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
				((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

				(CheckHitKey(key[co_k[0][5]]) == 1) ||
				(CheckHitKey(key[co_k[0][8]]) == 1) ||
				(CheckHitKey(key[co_k[0][9]]) == 1)
				) {
				P1_VB[2] += 1;
			}
			else {
				P1_VB[2] = 0;
			}
			// 3ボタン	C
			if (
				((m_P_Input[0] & pad[co_p[0][6]]) != 0) ||
				((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||

				(CheckHitKey(key[co_k[0][6]]) == 1) ||
				(CheckHitKey(key[co_k[0][8]]) == 1)
				) {
				P1_VB[3] += 1;
			}
			else {
				P1_VB[3] = 0;
			}
			// 4ボタン	D
			if (((m_P_Input[0] & pad[co_p[0][7]]) != 0) ||
				(CheckHitKey(key[co_k[0][7]]) == 1)
				) {
				P1_VB[4] += 1;
			}
			else {
				P1_VB[4] = 0;
			}
			// 5ボタン	A+B+C
			if (((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
				(CheckHitKey(key[co_k[0][8]]) == 1)) {
				P1_VB[5] += 1;
			}
			else {
				P1_VB[5] = 0;
			}
			// 6ボタン	A+B (投げ)
			if (((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||
				(CheckHitKey(key[co_k[0][9]]) == 1)) {
				P1_VB[6] += 1;
			}
			else {
				P1_VB[6] = 0;
			}
			// 7ボタン	PAUSE
			if (((m_P_Input[0] & pad[co_p[0][10]]) != 0) ||
				(CheckHitKey(key[co_k[0][10]]) == 1)) {
				P1_VB[7] += 1;
			}
			else {
				P1_VB[7] = 0;
			}
			// 8ボタン  トレモ(1)
			if (((m_P_Input[0] & pad[co_p[0][11]]) != 0) ||
				(CheckHitKey(key[co_k[0][11]]) == 1)) {
				P1_VB[8] += 1;
			}
			else {
				P1_VB[8] = 0;
			}
			// 9ボタン  トレモ(2)
			if (((m_P_Input[0] & pad[co_p[0][12]]) != 0) ||
				(CheckHitKey(key[co_k[0][12]]) == 1)) {
				P1_VB[9] += 1;
			}
			else {
				P1_VB[9] = 0;
			}
		}
	}
	// netOnの2P以外はこれ
	else {
		if (
			((input.POV[0] == 0) || (input.POV[0] == 4500) || (input.POV[0] == 31500)) ||
			((m_P_Input[0] & pad[co_p[0][0]]) != 0) ||
			(CheckHitKey(key[co_k[0][0]]) == 1)
			)
		{
			P1_B[108] += 1;
		}
		else {
			P1_B[108] = 0;
		}

		// 下
		if (
			((input.POV[0] == 18000) || (input.POV[0] == 13500) || (input.POV[0] == 22500)) ||
			((m_P_Input[0] & pad[co_p[0][1]]) != 0) ||
			(CheckHitKey(key[co_k[0][1]]) == 1)) {
			P1_B[102] += 1;
		}
		else {
			P1_B[102] = 0;
		}
		// 左
		if (
			((input.POV[0] == 27000) || (input.POV[0] == 22500) || (input.POV[0] == 31500)) ||
			((m_P_Input[0] & pad[co_p[0][2]]) != 0) ||
			(CheckHitKey(key[co_k[0][2]]) == 1)) {
			P1_B[104] += 1;
		}
		else {
			P1_B[104] = 0;
		}
		// 右
		if (
			((input.POV[0] == 9000) || (input.POV[0] == 4500) || (input.POV[0] == 13500)) ||
			((m_P_Input[0] & pad[co_p[0][3]]) != 0) ||
			(CheckHitKey(key[co_k[0][3]]) == 1)) {
			P1_B[106] += 1;
		}
		else {
			P1_B[106] = 0;
		}


		// 1ボタン	A
		if (
			((m_P_Input[0] & pad[co_p[0][4]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||

			(CheckHitKey(key[co_k[0][4]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)) {
			P1_B[1] ++;
		}
		else {
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
			) {
			P1_B[2] += 1;
		}
		else {
			P1_B[2] = 0;
		}
		// 3ボタン	C
		if (
			((m_P_Input[0] & pad[co_p[0][6]]) != 0) ||
			((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||

			(CheckHitKey(key[co_k[0][6]]) == 1) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)
			) {
			P1_B[3] += 1;
		}
		else {
			P1_B[3] = 0;
		}
		// 4ボタン	D
		if (((m_P_Input[0] & pad[co_p[0][7]]) != 0) ||
			(CheckHitKey(key[co_k[0][7]]) == 1)
			) {
			P1_B[4] += 1;
		}
		else {
			P1_B[4] = 0;
		}
		// 5ボタン	A+B+C
		if (((m_P_Input[0] & pad[co_p[0][8]]) != 0) ||
			(CheckHitKey(key[co_k[0][8]]) == 1)) {
			P1_B[5] += 1;
		}
		else {
			P1_B[5] = 0;
		}
		// 6ボタン	A+B (投げ)
		if (((m_P_Input[0] & pad[co_p[0][9]]) != 0) ||
			(CheckHitKey(key[co_k[0][9]]) == 1)) {
			P1_B[6] += 1;
		}
		else {
			P1_B[6] = 0;
		}
		// 7ボタン	PAUSE
		if (((m_P_Input[0] & pad[co_p[0][10]]) != 0) ||
			(CheckHitKey(key[co_k[0][10]]) == 1)) {
			P1_B[7] += 1;
		}
		else {
			P1_B[7] = 0;
		}
		// 8ボタン  トレモ(1)
		if (((m_P_Input[0] & pad[co_p[0][11]]) != 0) ||
			(CheckHitKey(key[co_k[0][11]]) == 1)) {
			P1_B[8] += 1;
		}
		else {
			P1_B[8] = 0;
		}
		// 9ボタン  トレモ(2)
		if (((m_P_Input[0] & pad[co_p[0][12]]) != 0) ||
			(CheckHitKey(key[co_k[0][12]]) == 1)) {
			P1_B[9] += 1;
		}
		else {
			P1_B[9] = 0;
		}
	}

	return 0;	// 終わり
}

// ボタン入力チェック
// ボタン配置をいじる場合はここで

int P2_BCheck()
{
	DINPUT_JOYSTATE input;

	GetJoypadDirectInputState(m_P_Input[1], &input);

	// 中止する
	if (netOn)return -1;

	// 通常の2P操作
	// P1の 上要素 が押されていたら
	if (
		((input.POV[1] == 0) || (input.POV[1] == 4500) || (input.POV[1] == 31500)) ||
		((m_P_Input[1] & pad[co_p[1][0]]) != 0) ||
		(CheckHitKey(key[co_k[1][0]]) == 1)
		)
	{
		P2_B[108] += 1;
	}
	else {
		P2_B[108] = 0;
	}
	// 下
	if (
		((input.POV[1] == 18000) || (input.POV[1] == 13500) || (input.POV[1] == 22500)) ||
		((m_P_Input[1] & pad[co_p[1][1]]) != 0) ||
		(CheckHitKey(key[co_k[1][1]]) == 1)) {
		P2_B[102] += 1;
	}
	else {
		P2_B[102] = 0;
	}
	// 左
	if (
		((input.POV[1] == 27000) || (input.POV[1] == 22500) || (input.POV[1] == 31500)) ||
		((m_P_Input[1] & pad[co_p[1][2]]) != 0) ||
		(CheckHitKey(key[co_k[1][2]]) == 1)) {
		P2_B[104] += 1;
	}
	else {
		P2_B[104] = 0;
	}
	// 右
	if (
		((input.POV[1] == 9000) || (input.POV[1] == 4500) || (input.POV[1] == 13500)) ||
		((m_P_Input[1] & pad[co_p[1][3]]) != 0) ||
		(CheckHitKey(key[co_k[1][3]]) == 1)) {
		P2_B[106] += 1;
		P2_B[104] = 0;
	}
	else {
		P2_B[106] = 0;
	}

	// 1ボタン	A
	if (
		((m_P_Input[1] & pad[co_p[1][4]]) != 0) ||
		((m_P_Input[1] & pad[co_p[1][8]]) != 0) ||
		((m_P_Input[1] & pad[co_p[1][9]]) != 0) ||

		(CheckHitKey(key[co_k[1][4]]) == 1) ||
		(CheckHitKey(key[co_k[1][8]]) == 1) ||
		(CheckHitKey(key[co_k[1][9]]) == 1)) {
		P2_B[1] ++;
	}
	else {
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
		) {
		P2_B[2] += 1;
	}
	else {
		P2_B[2] = 0;
	}
	// 3ボタン	C
	if (
		((m_P_Input[1] & pad[co_p[1][6]]) != 0) ||
		((m_P_Input[1] & pad[co_p[1][8]]) != 0) ||

		(CheckHitKey(key[co_k[1][6]]) == 1) ||
		(CheckHitKey(key[co_k[1][8]]) == 1)
		) {
		P2_B[3] += 1;
	}
	else {
		P2_B[3] = 0;
	}
	// 4ボタン	D
	if (((m_P_Input[1] & pad[co_p[1][7]]) != 0) ||
		(CheckHitKey(key[co_k[1][7]]) == 1)
		) {
		P2_B[4] += 1;
	}
	else {
		P2_B[4] = 0;
	}
	// 5ボタン	A+B (投げ)
	if (((m_P_Input[1] & pad[co_p[1][8]]) != 0) ||
		(CheckHitKey(key[co_k[1][8]]) == 1)) {
		P2_B[5] += 1;
	}
	else {
		P2_B[5] = 0;
	}
	// 6ボタン	A+B+C
	if (((m_P_Input[1] & pad[co_p[1][9]]) != 0) ||
		(CheckHitKey(key[co_k[1][9]]) == 1)) {
		P2_B[6] += 1;
	}
	else {
		P2_B[6] = 0;
	}
	// 7ボタン	PAUSE
	if (((m_P_Input[1] & pad[co_p[1][10]]) != 0) ||
		(CheckHitKey(key[co_k[1][10]]) == 1)) {
		P2_B[7] += 1;
	}
	else {
		P2_B[7] = 0;
	}
	// 8ボタン  トレモ(1)
	if (((m_P_Input[1] & pad[co_p[1][11]]) != 0) ||
		(CheckHitKey(key[co_k[1][11]]) == 1)) {
		P2_B[8] += 1;
	}
	else {
		P2_B[8] = 0;
	}
	// 9ボタン  トレモ(2)
	if (((m_P_Input[1] & pad[co_p[1][12]]) != 0) ||
		(CheckHitKey(key[co_k[1][12]]) == 1)) {
		P2_B[9] += 1;
	}
	else {
		P2_B[9] = 0;
	}

	return 0;	// 終わり
}

void Key_Setup() {
	P1_BCheck();
	P2_BCheck();

	// ネット対戦がオンなら
	if (netOn) {
		if (playerSide == 1)
			for (int i = 0; i < PAD_MAX; i++) {
				P1_B[i] = P1_VB[i];
			}
		else if (playerSide == 2) {
			for (int i = 0; i < PAD_MAX; i++) {
				P2_B[i] = P1_VB[i];
			}
		}
	}

	// キー上限設定
	for (int i = 0; i < PAD_MAX; i++) {
		if (P1_B[i] > 200)P1_B[i] = 200;
		if (P2_B[i] > 200)P2_B[i] = 200;
	}
}

// 1フレーム分戻す
void Key_Return1Frame() {
	for (int i = 0; i < PAD_MAX; i++) {
		if (P1_B[i] > 0)P1_B[i]--;
		if (P2_B[i] > 0)P2_B[i]--;
	}
}

void Key_Return1VFrame() {
	for (int i = 0; i < PAD_MAX; i++) {
		if (P1_VB[i] > 0)P1_VB[i]--;
	}
}

void set_stopVB(boolean flg) {
	stopVB = flg;
}

//=================================
// ネットワーク
//=================================

void Key_SetupSendKey() {
	if (playerSide == 1) {
		sendKey[0] = (BYTE)P1_B[108];
		sendKey[1] = (BYTE)P1_B[102];
		sendKey[2] = (BYTE)P1_B[104];
		sendKey[3] = (BYTE)P1_B[106];
		sendKey[4] = (BYTE)P1_B[1];
		sendKey[5] = (BYTE)P1_B[2];
		sendKey[6] = (BYTE)P1_B[3];
		sendKey[7] = (BYTE)P1_B[4];
		sendKey[8] = (BYTE)P1_B[7];
	}

	if (playerSide == 2) {
		sendKey[0] = (BYTE)P2_B[108];
		sendKey[1] = (BYTE)P2_B[102];
		sendKey[2] = (BYTE)P2_B[104];
		sendKey[3] = (BYTE)P2_B[106];
		sendKey[4] = (BYTE)P2_B[1];
		sendKey[5] = (BYTE)P2_B[2];
		sendKey[6] = (BYTE)P2_B[3];
		sendKey[7] = (BYTE)P2_B[4];
		sendKey[8] = (BYTE)P2_B[7];
	}
}

// 実行するキーを取得
void Key_SetGKeyLog(BYTE gKeyLog[KEYLOG_MAX]) {
	getKey[0] = gKeyLog[0];
	getKey[1] = gKeyLog[1];
	getKey[2] = gKeyLog[2];
	getKey[3] = gKeyLog[3];
	getKey[4] = gKeyLog[4];
	getKey[5] = gKeyLog[5];
	getKey[6] = gKeyLog[6];
	getKey[7] = gKeyLog[7];
	getKey[8] = gKeyLog[8];
}

void Key_SetSendKey(BYTE myKeyLog[KEYLOG_MAX]) {
	sendKey[0] = myKeyLog[0];
	sendKey[1] = myKeyLog[1];
	sendKey[2] = myKeyLog[2];
	sendKey[3] = myKeyLog[3];
	sendKey[4] = myKeyLog[4];
	sendKey[5] = myKeyLog[5];
	sendKey[6] = myKeyLog[6];
	sendKey[7] = myKeyLog[7];
	sendKey[8] = myKeyLog[8];
}

// sendKey更新処理
int Key_NetButtonUpdate() {
	// 
	Key_SetupSendKey();	// 自分のキーを送信用にセット
	Main_SetKeyLog(sendKey);

	return 0;
}

// P1_BとP2_Bを決定する
int Key_NetButtonEnter() {
	// 熱帯じゃないならやめる
	if (!netOn)return -1;

	// 1P
	if (playerSide == 1) {
		P2_B[108] = (int)getKey[0];
		P2_B[102] = (int)getKey[1];
		P2_B[104] = (int)getKey[2];
		P2_B[106] = (int)getKey[3];
		P2_B[1] = (int)getKey[4];
		P2_B[2] = (int)getKey[5];
		P2_B[3] = (int)getKey[6];
		P2_B[4] = (int)getKey[7];
		P2_B[7] = (int)getKey[8];

		// 自分
		P1_B[108] = (int)sendKey[0];
		P1_B[102] = (int)sendKey[1];
		P1_B[104] = (int)sendKey[2];
		P1_B[106] = (int)sendKey[3];
		P1_B[1] = (int)sendKey[4];
		P1_B[2] = (int)sendKey[5];
		P1_B[3] = (int)sendKey[6];
		P1_B[4] = (int)sendKey[7];
		P1_B[7] = (int)sendKey[8];
	}
	// 2P
	else if (playerSide == 2) {
		/*
		for (int i = 0; i < PAD_MAX; i++) {
			P2_B[i] = P1_B[i];	// 1P2Pを変える
		}
		*/
		// 取得した値をセット
		P1_B[108] = (int)getKey[0];
		P1_B[102] = (int)getKey[1];
		P1_B[104] = (int)getKey[2];
		P1_B[106] = (int)getKey[3];
		P1_B[1] = (int)getKey[4];
		P1_B[2] = (int)getKey[5];
		P1_B[3] = (int)getKey[6];
		P1_B[4] = (int)getKey[7];
		P1_B[7] = (int)getKey[8];

		// 自分
		P2_B[108] = (int)sendKey[0];
		P2_B[102] = (int)sendKey[1];
		P2_B[104] = (int)sendKey[2];
		P2_B[106] = (int)sendKey[3];
		P2_B[1] = (int)sendKey[4];
		P2_B[2] = (int)sendKey[5];
		P2_B[3] = (int)sendKey[6];
		P2_B[4] = (int)sendKey[7];
		P2_B[7] = (int)sendKey[8];
	}

	return 0;
}

//////////////////////////////////////////////

// 入力を送る
void InputSend()
{
	// 熱帯がオンなら行う
	// MainSystemで実行中
	//if(netOn)Key_NetButtonEnter();
	

	// 再生モード
	if (replayMode == 1){
		Replay_LoadKey();
	}
	// P1,P2の変数に保存
	for (int i = 0; i < PAD_MAX; i++){
		P1.button[i] = P1_B[i];
		P2.button[i] = P2_B[i];
	}

	//
	Player P[2];
	P[0] = P1, P[1] = P2;

	// リバサ入力
	//ctrl[0] = P1.ctrl;

	// テンキー位置判断、mukiが逆だと左右も変わる（常に右向き）
	for (int i = 0; i < 2; i++)
	{
		// 先行入力の設定、とりま6フレ
		for (int j = 1; j < BUTTON_MAX; j++){
			// キャラ＆画面が動いてるときは先行入力が減る
			if (S.StopTime == 0){
				if (P[i].StopTime == 0){
					if (P[i].Senkou[j] > 0)P[i].Senkou[j] -= 1;
					if (P[i].rbutton[j] > 0)P[i].rbutton[j] -= 1;
				}
			}

			// リバサ(ガード、起き上がり)
			if ((P[i].stateno >= 50 && P[i].stateno < 60) ||
				(P[i].stateno == 1055)){
				if (P[i].button[j] == 1)P[i].rbutton[j] = 6;
			}
			else{ if (P[i].button[j] == 1)P[i].rbutton[j] = 1; }

			// 入力した瞬間に先行入力6フレ追加
			// おしっぱで猶予が伸びる
			if (P[i].button[j] == 1)P[i].Senkou[j] = 6;
		}

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
	// 1P優先
	{
		if (P1_B[P_Num] != 0)P_InputNum = P1_B[P_Num];
		else if (P2_B[P_Num] != 0)P_InputNum = P2_B[P_Num];

		// 決定
		if (P_Num == 1 && (enter[0] > 0))P_InputNum = enter[0];
		// キャンセル
		if (P_Num == 2 && (enter[1] > 0))P_InputNum = enter[1];
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
		if (m_P_Input[1] & pad[i])watasu = 1;
	}

	return watasu;
}


int P1_BConfig()
{
	// 簡易キー準備
	if (Load_1 == 0){
		NumSetting();
		load_config();
		P1_VB.resize(PAD_MAX);
		P1_B.resize(PAD_MAX);
		P2_B.resize(PAD_MAX);
		mOld_B.resize(PAD_MAX);

		device[0] = 1;	// 暫定
		device[1] = 2;

		Load_1 = 0;
	}

	// コンフィグに映る数値
	return 0;
}

void NumSetting()
{
	// 空のを作成
	key.reserve(PAD_MAX);
	pad.reserve(PAD_MAX);
	for (int i = 0; i < PAD_MAX; i++){
		key.push_back(0);
		pad.push_back(0);
	}

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

	key[108] = 0;

	pad[0] = PAD_INPUT_UP;
	pad[1] = PAD_INPUT_DOWN;
	pad[2] = PAD_INPUT_LEFT;
	pad[3] = PAD_INPUT_RIGHT;

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

	

	pad[108] = 0;
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
	string str;
	char str2[10];
	// ハイスコアセーブ用ファイルを開く
	fp = fopen("keyconfig.ini", "rt");

	// 開けなかった場合は何もせずに終了
	if (fp == NULL){ printfDx("error keyconfig\n"); return 0; }

	// インデータの読み込み
	for (int i = 0; i < CO_MAX; i++)
	{
		str2[0] = '\0';
		fscanf(fp, "P1 Key%d = %s\n",
			//&i, &co_k[0][i]);
			&i, str2);
		co_k[0][i] = atoi(str2);
	}
	for (int i = 0; i < CO_MAX; i++)
	{
		str2[0] = '\0';
		fscanf(fp, "P2 Key%d = %s\n",
			&i, str2);
		co_k[1][i] = atoi(str2);
	}

	for (int i = 0; i < CO_MAX; i++)
	{
		str2[0] = '\0';
		// パッド１
		fscanf(fp, "P1 Pad%d = %s\n",
			&i, str2);
		co_p[0][i] = atoi(str2);
	}
	for (int i = 0; i < CO_MAX; i++)
	{
		str2[0] = '\0';
		// パッド２
		fscanf(fp, "P2 Pad%d = %s\n",
			&i, str2);
		co_p[1][i] = atoi(str2);
	}

	// -1を用意
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < CO_MAX; j++){
			if (co_k[i][j] == -1)co_k[i][j] = 108;
			if (co_p[i][j] == -1)co_p[i][j] = 108;
		}
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

// ネットワークのキー処理を全て行う
void KeySetting_Network()
{

}

// 自分のキーを送る
void Send_NetKey()
{

}

// 相手のキーを取得
void Get_NetKey()
{

}


void Key_SetPlayerSide(int n) {
	playerSide = n;
}

int Key_GetPlayerSide() {
	return playerSide;
}

void Key_SetNetOn(boolean flg) {
	netOn = flg;
}

boolean Key_GetNetOn() {
	return netOn;
}

void setP1_B(int num, int value) {
	P1_B[num] = value;
}

void setP2_B(int num, int value) {
	P2_B[num] = value;
}

//----------------
//
// リプレイキー保存
//
//----------------
// 録画
int Replay_ReserveKey()
{
	// オープンできなかったらここで終了
	if (fp == NULL){ return FALSE; }

	// [sizeof]はメモリサイズを調べる
	//fprintf(fp, "\n");		// なんか最初の行が消されるのでこれ使う
	{
		// 最初のみ
		if (rTimeCount == 0)fprintf(fp, "[setting]%d,%d,%d,%d,%d\n", 
			r_name[0], r_name[1], r_color[0], r_color[1], r_stage);

		fprintf(fp, "[%d]%d,%d,%d,%d,%d,%d,%d,%d,\n", 
			rTimeCount,
			P1_B[108], P1_B[102], P1_B[104], P1_B[106],
			P1_B[1], P1_B[2], P1_B[3], P1_B[4]);
		fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d,\n",
			P2_B[108], P2_B[102], P2_B[104], P2_B[106],
			P2_B[1], P2_B[2], P2_B[3], P2_B[4]);
	}
	rTimeCount++;


	/* 読み込みデータの確認 */
	return TRUE;
}


int Replay_LoadKey()
{
	// 判定セッティング //

	int i;
	int count;	// 代入の場所(最初の[ ] 2つ分)
	int nCount;	// 改行のカウント
	boolean iflg;
	count = -1, nCount = 0;

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// inputとinputcに文字がはいる

	// はじめに
	if (rTimeCount == 0){
		// ファイルポインタを先頭に移動する
		FileRead_seek(ifp, 0, SEEK_SET);
		//最初の1行読み飛ばす
		while (FileRead_getc(ifp) != '\n'); // FileRead_getc:一文字読み出す;
	}

	// ファイルを開く //
	while (1){
		//=========================
		// 基本ループ
		// このループ終了後に下へ
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(ifp);//1文字取得する

			// 改行で読み込み終了
			if (input[i] == '\n'){
				inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				nCount++;	// 列のカウント
				count = 0;	// 文字位置のカウント
				break;
			}

			// 文字列にする
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == ';'){ //カンマか改行なら
				inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				break;	// iを1増やしてループ脱出（代入する）
			}

			// 読み込み終了
			if (input[i] == EOF){//ファイルの終わりなら
				FileRead_close(ifp);
				MainSystem::Instance().SetNextMode("Replay");
				goto EXLOAD;//終了
			}
		}	// 基本ループ終了

		// 最初に数字扱いにして、文字が入ってたら変更する
		iflg = false;
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

		// 0～7で代入
		if (iflg){
			if ((count >= 0) && (count <= 7)){
				r_key[nCount][count] = atoi(inputc);
			}
			count++;
		}

		// 2回改行した
		if (nCount == 2){
			goto EXLOAD;	// 移動する
		}
	}
EXLOAD:

	int pa1, pa2;
	pa1 = P1_B[7];
	pa2 = P2_B[7];

	// P1,P2の変数に保存
	for (int i = 0; i < PAD_MAX; i++){
		P1_B[i] = 0;
		P2_B[i] = 0;
	}

	P1_B[7] = pa1;
	P2_B[7] = pa2;

	// 代入する
	P1_B[108] = r_key[0][0];
	P1_B[102] = r_key[0][1];
	P1_B[104] = r_key[0][2];
	P1_B[106] = r_key[0][3];
	P1_B[1] = r_key[0][4];
	P1_B[2] = r_key[0][5];
	P1_B[3] = r_key[0][6];
	P1_B[4] = r_key[0][7];

	P2_B[108] = r_key[1][0];
	P2_B[102] = r_key[1][1];
	P2_B[104] = r_key[1][2];
	P2_B[106] = r_key[1][3];
	P2_B[1] = r_key[1][4];
	P2_B[2] = r_key[1][5];
	P2_B[3] = r_key[1][6];
	P2_B[4] = r_key[1][7];

	/* 読み込みデータの確認 */
	rTimeCount++;
	return 0;
}

// 0.なし 1.再生 2.録画 それ以外(-1等)、状態確認
int Replay_Mode(int num)
{
	// 0～2
	if ((num >= 0) && (num <= 2)){
		replayMode = num;
		rTimeCount = 0;	// ここで初期化

		// 再生
		if (replayMode == 1){
			string str;	// 文字列(これが名前)
			str = "replay/" + gString;
			//strcpy(fileHomo, str.c_str());

			// (２番目の引数の"wb"の'w'は「書きこみ」の意味
			// 'b'はバイナリの意味(逆はテキスト))
			ifp = FileRead_open(str.c_str());//ファイル読み込み c_strでcharにできる？
			if (ifp == NULL){			// エラーが起こったら
				printfDx("リプレイを再生できませんでした\n");// 文字を表示
				return 0;
			}
		}
		//録画
		else if (replayMode == 2){
			// 日付取得
			DATEDATA Date;
			// 現在時刻を得る
			GetDateTime(&Date);

			string str;	// 文字列(これが名前)
			string year = std::to_string(Date.Year);
			string mon = std::to_string(Date.Mon);
			string day = std::to_string(Date.Day);
			string hour = std::to_string(Date.Hour);
			string min = std::to_string(Date.Min);
			if (mon.size() == 1)mon = "0" + std::to_string(Date.Mon);	// 0をつける
			if (day.size() == 1)day = "0" + std::to_string(Date.Day);	// 0をつける
			if (hour.size() == 1)hour = "0" + std::to_string(Date.Hour);	// 0をつける
			if (min.size() == 1)min = "0" + std::to_string(Date.Min);	// 0をつける

			str = "replay/" + year + mon + day + "_" + hour + min + ".rep";

			// (２番目の引数の"wb"の'w'は「書きこみ」の意味
			// 'b'はバイナリの意味(逆はテキスト))
			// 0以外(失敗)だったら
			if ((fopen_s(&fp, str.c_str(), "wt")) != 0) {
				printfDx("リプレイを保存できませんでした\n");
			}
		}
	}

	return replayMode;
}

void Key_GetString(string st)
{
	gString = st;
}

int GetRTimeCount() {
	return rTimeCount;
}

//
// リプレイ
//

void Replay_End()
{
	// オープンできたら
	if (fp != NULL){ 
		// ファイルを閉じる
		fclose(fp);
	}
	if (ifp != NULL){
		// ファイルを閉じる
		FileRead_close(ifp);
	}
	fp = NULL;	// ファイルを空にする
	ifp = NULL;	// ファイルを空にする
	gString = "0";
	replayMode = 0;
}

void Replay_Setting(int n1, int n2, int c1, int c2, int stageNum)
{
	r_name[0] = n1, r_name[1] = n2;
	r_color[0] = c1, r_color[1] = c2;
	r_stage = stageNum;
}


void InputDevice_Start() {
	//device[0] = -1;
	//device[1] = -1;
}

int InputDevice_Update() {
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT) != 0) {
		d_Input[0][0]++;
	}
	else { d_Input[0][0] = 0; }
	
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT) != 0) {
		d_Input[0][1]++;
	}
	else { d_Input[0][1] = 0; }
	
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1) != 0) {
		d_Input[0][2]++;
	}
	else { d_Input[0][2] = 0; }

	// 2P
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_LEFT) != 0) {
		d_Input[1][0]++;
	}
	else { d_Input[1][0] = 0; }

	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_RIGHT) != 0) {
		d_Input[1][1]++;
	}
	else { d_Input[1][1] = 0; }

	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_1) != 0) {
		d_Input[1][2]++;
	}
	else { d_Input[1][2] = 0; }


	// 左右

	// 左空いてる
	if (device[0] == -1) {
		if (d_Input[0][0] == 1) {
			if(device[0] != 1 && device[1] != 1)
				device[0] = 1;
			// →にある場合
			else if (device[1] == 1)
				device[1] = -1;
		}
		else if (d_Input[1][0] == 1) {
			if (device[0] != 2 && device[1] != 2)
			device[0] = 2;
			// →にある場合
			else if (device[1] == 2)
				device[1] = -1;
		}
	}

	if (device[1] == -1) {
		if (d_Input[0][1] == 1) {
			if (device[0] != 1 && device[1] != 1)
				device[1] = 1;
			// 左にある場合
			else if (device[0] == 1)
				device[0] = -1;
		}
		else if (d_Input[1][1] == 1) {
			if (device[0] != 2 && device[1] != 2)
				device[1] = 2;
			// 左にある場合
			else if (device[0] == 2)
				device[0] = -1;
		}
	}

	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1) != 0) {
		return 1; //完全決定
	}
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_1) != 0) {
		return 1; //完全決定
	}

	/*
	{
		// 決定
		if (device[0] == GetJoypadInputState(DX_INPUT_PAD1) ||
			device[1] == GetJoypadInputState(DX_INPUT_PAD1)) {

		}
		if (device[0] == GetJoypadInputState(DX_INPUT_PAD2) ||
			device[1] == GetJoypadInputState(DX_INPUT_PAD2)) {
			if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_1) != 0) {
				return 1; //完全決定
			}
		}
	}
	*/

	return 0;
}

void InputDevice_Draw() {
	if (device[0] == 1){
		DrawString(20,60,"PAD1", Cr);
	}
	else if (device[0] == 2) {
		DrawString(20, 60, "PAD2", Cr);
	}
	else {
		DrawString(20, 60, "NO INPUT", Cr);
	}

	if (device[1] == 1) {
		DrawString(SCREEN_W - 60, 60, "PAD1", Cr);
	}
	else if (device[1] == 2) {
		DrawString(SCREEN_W - 60, 60, "PAD2", Cr);
	}
	else {
		DrawString(SCREEN_W - 100, 60, "NO INPUT", Cr);
	}

	// 中央
	// どちらにもない
	if (device[0] != 1 && device[1] != 1)
		if (GetJoypadNum() >= 1)DrawString(SCREEN_W / 2 - 40, 50, "PAD1", Cr);
	if (device[0] != 2 && device[1] != 2)
		if (GetJoypadNum() >= 2)
		DrawString(SCREEN_W / 2 - 40, 70, "PAD2", Cr);
}

// セットされているボタンを返す
int Key_ReturnButton(int player, int button) {
	if (player < 0)player = 0;
	else if (player > 1)player = 1;
	if (button < 0)button = 0;
	else if (button > 12)button = 12;

	// ボタン条件
	return pad[co_p[player][button]];
}

// 入力した値をコントローラに割り当てる
void Key_SetButton(int player, int button, int input) {
	if (player < 0)player = 0;
	else if (player > 1)player = 1;
	if (button < 0)button = 0;
	else if (button > 12)button = 12;

	// 
	boolean flg = false;
	int num = 0;
	for (int i = 4; i < 32; i++) {
		// 条件一致
		if (input == pad[i]) {
			flg = true;
			num = i;
		}
	}

	// 存在するならnumをセット
	if (flg) {
		co_p[player][button] = num;
	}
}
