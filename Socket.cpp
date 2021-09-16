#include "pch.h"
#include "Socket.h"

void Socket::draw() {
	// 待機中
	if (waitMode > 0) {
		// 
		if (waitMode == 1)DrawString(SCREEN_W / 2 - 30, SCREEN_H / 2 - 10, "wait..", Cr);
		// モード切り替え時の待機
		if (waitMode == 2) {
			DrawString(SCREEN_W / 2 - 30, SCREEN_H / 2 - 10, "wait(2)", Cr);
		}
	}
	// 画面下の黒ウィンドウ
	else {
		//==========================
		// 暗くする
		//==========================
		// 少し暗くする
		SetAlpha(188);
		// 同じなら解除
		//DrawBox(0, SCREEN_H - 22, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
		DrawBox(430 - 2, SCREEN_H - 22, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
		NoBlend();
	}


	/*
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 2; j++) {
			DrawFormatString(j * 60, i * 20, Cr, "%d,%d", myAllLog[i][j][KEYLOG_MAX], myAllLog[i][j][KEYLOG_MAX + 1]);
			DrawFormatString(120 + j * 60, i * 20, Cr, "%d,%d", p2AllLog[i][j][KEYLOG_MAX], p2AllLog[i][j][KEYLOG_MAX + 1]);
		}
	}
	
	DrawString(20, 0, "p2AllLog[0][n][waitTime]", Cr);
	for (int i = 0; i < 20; i++) {
		DrawFormatString(0, i * 20 + 20, Cr, "%d,%d", p2AllLog[0][i][KEYLOG_MAX], p2AllLog[0][i][KEYLOG_MAX + 1]);
		DrawFormatString(80, i * 20 + 20, Cr, "%d,%d", p2AllLog[1][i][KEYLOG_MAX], p2AllLog[1][i][KEYLOG_MAX + 1]);
		DrawFormatString(160, i * 20 + 20, Cr, "%d,%d", p2AllLog[2][i][KEYLOG_MAX], p2AllLog[2][i][KEYLOG_MAX + 1]);
	}
	*/

	/*
	DrawFormatString(0, SCREEN_H - 20, Cr, "recv:%d.%d", getData, getRecvData);
	DrawFormatString(80, SCREEN_H - 20, Cr, "mode:%d.%d", myGameMode, p2GameMode);
	*/
	/*
	DrawFormatString(180, SCREEN_H - 20, Cr, "%d.%d", waitTime[0], waitTime[1]);
	DrawFormatString(240, SCREEN_H - 20, Oran, "%d.%d", nowWaitTime[0], nowWaitTime[1]);
	DrawFormatString(300, SCREEN_H - 20, Cr, "%d.%d", p2WaitTime[0], p2WaitTime[1]);
	*/

	DrawFormatString(430, SCREEN_H - 20, Cr, "ping:%d", ping);
	//DrawFormatString(420, SCREEN_H - 20, Cr, "delay:%d", ping);

	//int num = nowWaitTime[0] * 60 + nowWaitTime[1];
	//DrawFormatString(200, SCREEN_H - 20, Cr, "net:%d",  num);
	//DrawFormatString(300, SCREEN_H - 20, Cr, "game:%d", gameTime);
	//num = gameTime - num;
	//DrawFormatString(390, SCREEN_H - 20, Oran, "%d", num);
	if (disconnectTime == 0) {
		if (nowWaitTime[1] > 40)DrawString(510, SCREEN_H - 20, "実行中..", Cr);
		else if (nowWaitTime[1] > 20)DrawString(510, SCREEN_H - 20, "実行中.", Cr);
		else {
			DrawString(510, SCREEN_H - 20, "実行中", Cr);
		}
	}
	else {
		DrawFormatString(510, SCREEN_H - 20, Cr, "接続待ち..%d", disconnectTime);
	}
}

// リプレイ進行値を取得する
void Socket::getGameTime(int num) {
	gameTime = GetRTimeCount();
}