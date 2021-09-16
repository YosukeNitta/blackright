//インクルード部--------------------------------------------
#include "pch.h"

//#include <list>
//using namespace std;
#include "Menu.h"
#include "MainSystem.h"

static int* test1;
static int* test2;
static int* test3;
//static boolean load_1 = false;
//static boolean load_f = false; //一度のみ
static int P1_Key;
static int inputUP, inputDOWN;
static int TimeStop, kettei;
static int modeG[8];
static int config;	// コンフィグ画像
static int back, mode[8];
// 現在の選択番号, 選択項目の数
static int SPoint = 0;
static const int SentakuNum = 8;
static int next;	// 次の項目へのスクロール
static int gameTime;	// 時間
static int anten;	// 終了時の暗転
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
		// ネットワーク
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 4)) {
			MainSystem::Instance().SetNextMode("Network");
		}
		// リプレイ再生
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 5)) {
			//ModeChange(SceneNum(ReplayScene));	// 
			MainSystem::Instance().SetNextMode("Replay");
		}
		// コンフィグ
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 6)) {
			MainSystem::Instance().SetNextMode("Config");
		}
		// 終了
		else if ((kettei == 1) && (TimeStop > 10) && (SPoint == 7)) {
			//ModeChange(4);
			//ModeChange(0);	// 終了
			MainSystem::Instance().EndMainSystem();
		}

		/*
		// F3でネット対戦準備
		if (CheckHitKey(KEY_INPUT_F3) != 0) {
			MainSystem::Instance().SetNextMode("Network");
		}

		// F4でネット対戦準備
		if (CheckHitKey(KEY_INPUT_F4) != 0) {
			MainSystem::Instance().SetNextMode("Config");
		}
		*/

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
	case 4:
		DrawString(drawX, drawY, "ネット対戦(未実装)", Cr);
		break;
	case 5:
		DrawString(drawX, drawY, "記録したリプレイを再生します", Cr);
		break;
	case 6:
		DrawString(drawX, drawY, "各種設定を行います", Cr);
		break;
	case 7:
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
		int b[3], f[3];
		b[0] = SPoint - 1;
		b[1] = SPoint - 2;
		b[2] = SPoint - 3;
		
		f[0] = SPoint + 1;
		f[1] = SPoint + 2;
		f[2] = SPoint + 3;

		// 位置調整
		if (b[0] < 0)b[0] = SentakuNum - 1;
		if (f[0] >= SentakuNum)f[0] = 0;

		// 
		if (b[1] < 0){
			b[1] = SentakuNum - 1;
			if (b[0] == SentakuNum - 1)b[1] = SentakuNum - 2;
		}
		if (f[1] >= SentakuNum){
			f[1] = 0;
			if (f[0] == 0)f[1] = 1;
		}

		// 
		if (b[2] < 0) {
			b[2] = SentakuNum - 1;
			if (b[0] == SentakuNum - 1)b[2] = SentakuNum - 2;
		}
		if (f[2] >= SentakuNum) {
			f[2] = 0;
			if (f[0] == 0)f[2] = 1;
		}

		for (int i = 0; i < 3; i++){
			int xpos = -5;
			// 画像表示位置設定
			xpos += (int)(gameTime * 1.0);
			if (xpos > 25)
				xpos = 25;

			SetDrawBright(128, 128, 128);	// 基本は暗く
			//DrawGraph(xpos, 22 * 0 + next, mode[b[2]], true);
			DrawGraph(xpos, 30 * 1 + next + 3, mode[b[1]], true);
			DrawGraph(xpos, 30 * 3 + next, mode[b[0]], true);

			DrawGraph(xpos, 30 * 7 + next, mode[f[0]], true);
			DrawGraph(xpos, 30 * 9 + next, mode[f[1]], true);
			//DrawGraph(xpos, 22 * 11 + next, mode[f[2]], true);

			// 移動が終わったら
			if(next == 0)SetDrawBright(250, 250, 255);	// 選んでるのを明るく			
			DrawGraph(xpos, 30 * 5 + next, mode[SPoint], true);
			
			SetDrawBright(255, 255, 255);
		}

		// 暗転
		if (AntenCheck()){
			Anten(0);
		}

		// 次シーン移行の暗転
		if ((kettei == 1) && (TimeStop <= 11)) {
			int num = TimeStop * 25;
			if (num > 255) num = 255;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, num);
			DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
	// 暗転
	anten = 0;

	// リプレイ終了(念のため)
	Replay_End();
}


void Menu::Load_1second(){

	// 画像
	back = LoadGraph("ob/titleB.png");
	
	int setMode[7];	// 代入用
	LoadDivGraph("ob/menu.png", 7, 1, 7, 160, 33, setMode);
	config = LoadGraph("ob/config.png");

	for(int i=0; i<7; i++){
		mode[i] = setMode[i];
	}

	// モード1と2を逆にする
	int hozon = mode[1];
	mode[1] = mode[2];
	mode[2] = hozon;

	/*
	hozon = mode[4];
	mode[4] = mode[5];
	mode[5] = hozon;
	*/
	
	// 画像
	mode[7] = mode[6];
	
	// 画像変更
	mode[6] = config;
}

void Menu::Release(void)
{
}