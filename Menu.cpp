//インクルード部--------------------------------------------
#include "pch.h"

//#include <list>
//using namespace std;
#include "Menu.h"
#include "MainSystem.h"

//static boolean load_1 = false;
//static boolean load_f = false; //一度のみ
static int P1_Key;
static int inputUP, inputDOWN;
static int TimeStop, kettei;
static int modeG[7];
static int back, mode[7];
// 現在の選択番号, 選択項目の数
static int SPoint = 0;
static const int SentakuNum = 6;
static int next;	// 次の項目へのスクロール
static int gameTime;	// 時間
//static void Draw();

//static 
int Menu::Mode()
{
	// ループ、プログラム実行開始
	{

		//*********
		// 必須処理
		//*********

		if (P_BInput(108) == 1){
			inputUP += 1;
		}
		else{
			inputUP = 0;
		}
		// 下キー
		if (P_BInput(102) == 1){
			inputDOWN += 1;
		}
		else{
			inputDOWN = 0;
		}

		// キー入力に応じて処理
		// カーソル移動
		// 上キーの入力時間、決定時は動けない
		if (kettei == 0){
			if (KInput(P_BInput(108))){
				SPoint--, SEStart(36);
				next = 3;
			}
			if (KInput(P_BInput(102))){
				SPoint++, SEStart(36);
				next = -3;
			}
			if (SPoint < 0)SPoint = SentakuNum - 1;
			if (SPoint > SentakuNum - 1)SPoint = 0;
		}

		// キー入力
		// 決定キー時処理
		if ((P_BInput(1) == 1 || P_BInput(3) == 1) && (kettei == 0))
		{
			kettei = 1;
			SEStart(35);
		}

		// 決定してたらカウント加算
		if (kettei == 1){
			TimeStop += 1;
		}
		else{
			TimeStop = 0;
		}

		// スクロール変更
		if (next > 0){
			next -= 1;
		}
		else if(next < 0){
			next += 1;
		}

		// ゲームタイム加算
		gameTime++;
		if (gameTime > 300)gameTime = 300;


		// ターボモード変更
		/*
		if (P_BInput(4) == 1) {
			if (Versus_GetTurboMode()) {
				SEStart(35);
			}
			else { SEStart(38); }
			Versus_TurboMode();
		}
		*/
		
		// 1 秒経過した対応
		// ARCADE
		if ((kettei == 1) && (TimeStop > 10) && (SPoint == 0)) {
			TraningSwitch(false);
			AISwitch(true);
			Arcade_Switch(1);
			BattleMode(3);
			WinCount(0);
			//ModeChange(SceneNum(SelectScene));	// キャラセレへ
			MainSystem::Instance().SetNextMode("Select");
		}
		// 対戦
		if ((kettei == 1) && (TimeStop > 10) && (SPoint == 1)) {
			TraningSwitch(false);
			AISwitch(false);
			Arcade_Switch(0);
			BattleMode(0);
			WinCount(0);
			//ModeChange(SceneNum(SelectScene));	// キャラセレへ
			MainSystem::Instance().SetNextMode("Select");
		}
		// VS.AI
		if ((kettei == 1) && (TimeStop > 10) && (SPoint == 2)) {
			TraningSwitch(false);
			AISwitch(true);
			Arcade_Switch(0);
			BattleMode(2);
			WinCount(0);
			//ModeChange(SceneNum(SelectScene));	// キャラセレへ
			MainSystem::Instance().SetNextMode("Select");
		}
		// トレモ
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 3)) {
			TraningSwitch(true);
			AISwitch(false);
			BattleMode(1);
			//ModeChange(SceneNum(SelectScene));	// トレモ起動してからキャラセレへ
			MainSystem::Instance().SetNextMode("Select");
		}
		/*
		// ネットワーク
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 4)) {
			ModeChange(SceneNum(NetworkScene));	// 
			//ModeChange(SceneNum(ConfigScene));	// 
		}
		*/
		// リプレイ再生
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 4)) {
			//ModeChange(SceneNum(ReplayScene));	// 
			MainSystem::Instance().SetNextMode("Replay");
		}
		// 終了
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 5)) {
			//ModeChange(4);
			//ModeChange(0);	// 終了
			MainSystem::Instance().EndMainSystem();
		}

		// F3でネット対戦準備
		if (CheckHitKey(KEY_INPUT_F3) != 0) {
			MainSystem::Instance().SetNextMode("Network");
		}

		// タイトルへ戻る
		if (P_BInput(2) == 1) {
			MainSystem::Instance().SetNextMode("Title");
			SEStart(49);
			BGMStart(0);
		}
		// ネットワークに繋いでたなら終了
		//if (Connect_CheckCn())ModeChange(0);	// 終了


	}//全体を終了

	return 0;
}

// メニュー描画
void Menu::Draw()
{

	// 背景
	DrawGraph(0, 0, back, true);
	
	DrawBox(0, SCREEN_H - 40, SCREEN_W, SCREEN_H - 20, GetColor(0, 0, 0), true);
	int drawX =  80;
	int drawY = SCREEN_H - 40 + 1;
	switch (SPoint){
	case 0:
		DrawString(drawX, drawY, "CPUを倒して勝ち抜くモードです", Cr);
		break;
	case 1:
		DrawString(drawX, drawY, "プレイヤーと対戦します", Cr);
		break;
	case 2:
		DrawString(drawX, drawY, "CPUと対戦します", Cr);
		break;
	case 3:
		DrawString(drawX, drawY, "様々な状況での動きを確認できます", Cr);
		break;
	/*
	case 4:
		DrawString(drawX, drawY, "ネット対戦(未実装)", Cr);
		break;
		*/
	case 4:
		DrawString(drawX, drawY, "記録したリプレイを再生します", Cr);
		break;
	case 5:
		DrawString(drawX, drawY, "ゲームを終了します", Cr);
		break;
	}
	
	// ターボモード表示
	/*
	if (Versus_GetTurboMode()){
		DrawString(2, 2, "Turbo", Cr);
	}
	else
	{
		DrawString(2, 2, "Normal", Cr);
	}
	*/

	


	{
		// 画像表示
		/*
		for (int i = 0; i < SentakuNum; i++){
			SetDrawBright(128, 128, 128);	// 基本は暗く
			if (i == SPoint)SetDrawBright(250, 250, 255);	// 選んでるのを明るく
			DrawGraph(25, 33 * i, mode[i], true);
			SetDrawBright(255, 255, 255);
		}
		*/
		// 画像表示
		int b[2], f[2];
		b[0] = SPoint - 1;
		b[1] = SPoint - 2;
		f[0] = SPoint + 1;
		f[1] = SPoint + 2;
		if (b[0] < 0)b[0] = SentakuNum - 1;
		if (f[0] >= SentakuNum)f[0] = 0;

		if (b[1] < 0){
			b[1] = SentakuNum - 1;
			if (b[0] == SentakuNum - 1)b[1] = SentakuNum - 2;
		}
		if (f[1] >= SentakuNum){
			f[1] = 0;
			if (f[0] == 0)f[1] = 1;
		}

		for (int i = 0; i < 3; i++){
			int xpos = -5;
			xpos += (int)(gameTime * 1.2);
			if (xpos > 25)
				xpos = 25;

			SetDrawBright(128, 128, 128);	// 基本は暗く
			DrawGraph(xpos, 33 * 1 + next, mode[b[1]], true);
			DrawGraph(xpos, 33 * 3 + next, mode[b[0]], true);
			DrawGraph(xpos, 33 * 7 + next, mode[f[0]], true);
			DrawGraph(xpos, 33 * 9 + next, mode[f[1]], true);
			
			SetDrawBright(250, 250, 255);	// 選んでるのを明るく
			DrawGraph(xpos, 33 * 5 + next, mode[SPoint], true);
			
			SetDrawBright(255, 255, 255);
		}

		// 暗転
		if (AntenCheck()){
			Anten(0);
		}

	}
}

int Menu::End() {
	DeleteGraph(back);
	for (int i = 0; i < 7; i++) {
		DeleteGraph(mode[i]);
	}

	return 0;
}

void Menu::Load_Reload(){
	Anten(255);

	// BGM読み込み
	BGMStart(11);
	kettei = 0;

	// 時間
	gameTime = 0;

	// リプレイ終了(念のため)
	Replay_End();
}


void Menu::Load_1second(){
	// 画像
	back = LoadGraph("ob/titleB.png");
	LoadDivGraph("ob/menu.png", 7, 1, 7, 160, 33, mode);

	// モード1と2を逆にする
	int hozon = mode[1];
	mode[1] = mode[2];
	mode[2] = hozon;

	hozon = mode[4];
	mode[4] = mode[5];
	mode[5] = hozon;

	hozon = mode[5];
	mode[5] = mode[6];
	mode[6] = hozon;
}

void Menu::Release(void)
{
}