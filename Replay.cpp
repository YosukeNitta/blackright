//インクルード部--------------------------------------------
#include "pch.h"

// メニュー部分
#include "Replay.h"
using namespace modeData;

// ファイル名準備
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <string>	// stringに使う
#include <vector>

using namespace std;	// vecterで使用

void Replay_GetFileData();
void Replay_GetPort(int num);
int Replay_Delete();

//ﾃﾞｰﾀ定義部------------------------------------------------
static int yPos = 0, xPos = 0;
static int fileCount = 0;
static int deleteSwitch = 0;
static vector<string> repName;	// これに全てのキャラの判定
static vector<int> r_port;	// これに全てのキャラの判定
static vector<int> r_cn1;	// これに全てのキャラの判定
static vector<int> r_cn2;	// これに全てのキャラの判定
static int r_name[2], r_color[2], r_stage;

// 
void ReadReplayFile()
{
	// あるなら削除
	int rSize = repName.size();
	if (rSize > 0){
		for (int i = 0; i < rSize; i++){
			repName.pop_back();    //  末尾データを削除
			r_cn1.pop_back();    //  末尾データを削除
			r_cn2.pop_back();    //  末尾データを削除
		}
	}

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	TCHAR *target = "replay/\\*.rep"; 

	hFind = FindFirstFile(target, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			repName.push_back(FindFileData.cFileName);
			r_cn1.push_back(0);
			r_cn2.push_back(0);
			fileCount++;
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}

	for (int i = 0; i < repName.size(); i++){
		Replay_GetPort(i);
	}
}

int Replay::Mode()
{
	if (P_BInput(2) == 1){
		SEStart(37);
		ModeChange(GameScene(MenuScene));	// キャラセレへ
	}
	// キャラ、カラー、ステージ　を読み込む
	else if (((P_BInput(1) == 1) || (P_BInput(3) == 1)) && (repName.size() > 0)){
		Replay_GetFileData();

		Get_Color(r_color[0], 1);		// カラー決定
		Get_Color(r_color[1], 2);		// カラー決定
		GetStageNum(r_stage, r_stage);	// ステージ受け取り
		Versus_bgmNum(r_stage + 1);
		Select_SetName(r_name[0], r_name[1]);

		TraningSwitch(false);
		AISwitch(false);
		BattleMode(0);
		WinCount(0);

		Replay_End();
		Key_GetString(repName[yPos + (xPos * 7)]);
		Replay_Mode(1);
		
		ModeChange(GameScene(VersusScene));
	}
	// キャラ、カラー、ステージ　を読み込む
	else if ((P_BInput(4) == 1) && (repName.size() > 0)){
		SEStart(38);
		Replay_Delete();
	}

	if (repName.size() > 0){
		if ((P_BInput(108) == 1)){
			yPos--;
			SEStart(36);
		}
		else if ((P_BInput(102) == 1)){
			yPos++;
			SEStart(36);
		}

		if (yPos < 0){
			yPos = 6;
			if (((xPos * 7) + yPos) > repName.size() - 1)
				yPos = (repName.size() % 7) - 1;
		}
		if ((yPos > 6) ||
			(((xPos * 7) + yPos) > repName.size() - 1)
			){
			yPos = 0;
		}

		
	}
	if (repName.size() > 7){
		if ((P_BInput(104) == 1)){
			xPos--;
			SEStart(36);
		}
		else if ((P_BInput(106) == 1)){
			xPos++;
			SEStart(36);
		}

		if ((xPos * 7) < 0){
			xPos = repName.size() / 7;
		}
		if ((xPos * 7) >(repName.size() - 1)){
			xPos = 0;
		}
	}

	return 0;
}

// メニュー描画
void Replay::Draw()
{
	// カーソル描画
	{
		int minus = 0;
		if (xPos > 0)minus = xPos * 6;

		// 描画ブレンドモードをアルファブレンド（透明）にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
		// カーソル描画
		DrawBox(0, (yPos * 60) + 20, SCREEN_W, (yPos * 60) + 80, GetColor(20, 20, 250), true);
		NoBlend();
	}

	DrawString(0,0,"リプレイ", Cr);
	DrawString(10, SCREEN_H - 20, "D..ファイル削除  S..再生終了", Cr);
	DrawFormatString(100, 0, Cr, "%d / %d", yPos + (xPos * 7) + 1, repName.size());
	for (int i = 0; i < 7; i++){
		if ((i + (xPos * 7)) < repName.size()){
			DrawFormatString(20, 20 + (60 * i), Cr, "%s", repName[i + (xPos * 7)].c_str());
			DrawGraph(320, 20 + (60 * i), r_port[r_cn1[i + (xPos * 7)]], true);
			DrawGraph(380, 20 + (60 * i), r_port[r_cn2[i + (xPos * 7)]], true);
		}
	}
	//DrawFormatString(420, 20, Cr, "%s", repName[yPos].c_str());
	//DrawFormatString(0, SCREEN_H - 20, Cr, "ファイル数%d", repName.size());
}

int Replay::Load_Reload()
{
	fileCount = 0;
	// repファイルロード
	ReadReplayFile();

	for (int i = 0; i < 2; i++){
		r_name[i] = 0;
		r_color[i] = 0;
	}
	r_stage = 0;

	// BGM読み込み
	BGMStart(11);

	return 0;
}

void Replay::Load_1second()
{
	Name_t name;
	string str, str2, fn;
	int r, g, b;
	int handle;
	str2 = "char/";
	fn = "/pal/port.bmp";

	// ポートレート取得
	for (int i = 0; i < Character_Max(); i++){
		r_port.push_back(0);
		name = Character_Name(i);
		str = str2 + name.nameC + fn;

		// 画像の読み込み
		handle = LoadSoftImage(str.c_str());
		// パレットの色を取得する
		GetPaletteSoftImage(handle, 0, &r, &g, &b, 0);
		// 透明色決定
		SetTransColor(r, g, b);

		// 画像生成
		r_port[i] = LoadGraph(str.c_str());
	}
	SetTransColor(0, 0, 0);
	// 使い終わったら解放
	DeleteSoftImage(handle);
}

void Replay_EnterSetting(int n1, int n2, int c1, int c2, int stageNum)
{
	r_name[0] = n1, r_name[1] = n2;
	r_color[0] = c1, r_color[1] = c2;
	r_stage = stageNum;
}

void Replay_GetFileData()
{
	// 判定セッティング //

	int i, fp;
	int anum;	// Nameの場所
	anum = 0;

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// inputとinputcに文字がはいる
	boolean iflg = false;	// 名前入力フラグ
	char t_setting[8] = { "setting" };

	string str = "replay/" + repName[yPos + (xPos * 7)];

	// ファイルを開く //
	fp = FileRead_open(str.c_str());//ファイル読み込み
	if (fp == NULL){			// エラーが起こったら
		printfDx("error repfile\n");// 文字を表示
		return;
	}
	// ファイルポインタを先頭に移動する
	FileRead_seek(fp, 0, SEEK_SET);

	// 1行目のみ開いて閉じる
	while (1){
		//=========================
		// 基本ループ
		// このループ終了後に下へ
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1文字取得する

			// 改行で読み込み終了
			if (input[i] == '\n'){
				inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				break;
			}

			// 文字列にする
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == ';'){ //カンマか改行なら
				inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				break;	// iを1増やしてループ脱出（代入する）
			}

		}	// 基本ループ終了


		// 名前だったら
		if (iflg){
			switch (anum)
			{
			case 0:
				r_name[0] = atoi(inputc); break;
			case 1:
				r_name[1] = atoi(inputc); break;
			case 2:
				r_color[0] = atoi(inputc); break;
			case 3:
				r_color[1] = atoi(inputc); break;
			case 4:
				r_stage = atoi(inputc); break;
			default:
				break;
			}
			anum++;	// 最初に0になる
		}

		if (strcmp(inputc, t_setting) == 0){
			iflg = true;
		}

		if (input[i] == '\n')goto EXFILE;
	}

	// ファイルを閉じる
EXFILE:
	FileRead_close(fp);
	fp = NULL;
}

void Replay_GetPort(int num)
{
	// 判定セッティング //

	int i, fp;
	int anum;	// Nameの場所
	anum = 0;

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// inputとinputcに文字がはいる
	boolean iflg = false;	// 名前入力フラグ
	char t_setting[8] = { "setting" };

	string str = "replay/" + repName[num];

	// ファイルを開く //
	fp = FileRead_open(str.c_str());//ファイル読み込み
	if (fp == NULL){			// エラーが起こったら
		printfDx("error repfile\n");// 文字を表示
		return;
	}
	// ファイルポインタを先頭に移動する
	FileRead_seek(fp, 0, SEEK_SET);

	// 1行目のみ開いて閉じる
	while (1){
		//=========================
		// 基本ループ
		// このループ終了後に下へ
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1文字取得する

			// 改行で読み込み終了
			if (input[i] == '\n'){
				inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				break;
			}

			// 文字列にする
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == ';'){ //カンマか改行なら
				inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				break;	// iを1増やしてループ脱出（代入する）
			}

		}	// 基本ループ終了


		// 名前だったら
		if (iflg){
			switch (anum)
			{
			case 0:
				r_cn1[num] = atoi(inputc) - 1; break;
			case 1:
				r_cn2[num] = atoi(inputc) - 1; break;
			default:
				break;
			}
			anum++;	// 最初に0になる
		}

		if (strcmp(inputc, t_setting) == 0){
			iflg = true;
		}

		if (input[i] == '\n'){
			goto EXFILE;
		}
	}

	// ファイルを閉じる
EXFILE:
	FileRead_close(fp);
	fp = NULL;
}

int Replay_Delete()
{
	string str = "replay/" + repName[yPos + (xPos * 7)];

	if ((remove(str.c_str()) == 0) && (repName.size() > 0)){
		repName.erase(repName.begin() + yPos + (xPos * 7));    //  末尾データを削除
		r_cn1.erase(r_cn1.begin() + yPos + (xPos * 7));    //  末尾データを削除
		r_cn2.erase(r_cn2.begin() + yPos + (xPos * 7));    //  末尾データを削除
	}
	return 0;
}