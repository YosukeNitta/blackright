#include "pch.h"
#include "Result.h"
#include "MainSystem.h"
#include <string>	// stringに使う
#include <vector>

using namespace std;


class TextBox {
public:
	// 相手の名前
	string enemyName;
	// テキスト
	string text[2];

	TextBox() {

	}
};

class WinText {
public:
	// 名前
	string name;
	// 立ち絵
	int image;

	// 格納した勝利セリフ
	vector<TextBox> tex;

	WinText() {

	}
};

static vector<WinText> winTexts;	// キャラ毎の勝利セリフまとめ

// 名前（airから読み込み）s
static vector<Name_t> N;

static int nPos;
static int color[2];
static int side;

static int winChara[2];	// 勝ったキャラ
static string charName[2];
static boolean load_1 = false;
static int time;

static int p_color[2];
static int p_name[2];
static int StageNum;

// 暗転
static int _timeStop;
// 次のシーン
static int _nextScene;

// 対戦数
static int winCount[2];
static void Draw();

int Result::Mode()
{
	if (!load_1){
		load_1 = true;
	}

	if (P1_BInput(108) == 1){
		nPos--;
		SEStart(36);
	}
	if (P1_BInput(102) == 1){
		nPos++;
		SEStart(36);
	}
	if (nPos > 1)nPos = 0;
	if (nPos < 0)nPos = 1;

	// 白

	// アケなら
	if (Arcade_Switch(-1) == 1){
		if ((P1_BInput(1) == 1) || (P1_BInput(3) == 1)){
			SEStart(35);
			if (nPos == 0) {
				// 1Pが勝った
				if(side == 1)_nextScene = SceneNum(ArcadeScene);	// シーン設定
				// cpuが勝った
				else { _nextScene = SceneNum(VersusScene); }	// シーン設定
				Replay_Mode(0);	// もう一度リプレイ
			}
			else if (nPos == 1)
				_nextScene = SceneNum(MenuScene);
		}
		if ((P_BInput(2) == 1)){
			SEStart(37);
			nPos = 1;
		}
	}
	// アケじゃない
	else{
		if ((P1_BInput(1) == 1) || (P1_BInput(3) == 1)){
			SEStart(35);
			if (nPos == 0){
				// 対戦画面へループ
				//MainSystem::Instance().SetNextMode("Versus");
				_nextScene = SceneNum(VersusScene);	// シーン設定
				//if (BattleMode(-1) == 0)Replay_Mode(2);	// もう一度録画する
				//else{ Replay_Mode(0); }
				//Replay_Setting(p_name[0], p_name[1], p_color[0], p_color[1], StageNum);
			}
			// キャラセレ
			else if (nPos == 1)
				_nextScene = SceneNum(SelectScene);	// シーン設定
		}
		if ((P1_BInput(2) == 1)){
			SEStart(37);
			nPos = 1;
		}
	}

	// 移行する
	if ((_timeStop >= 11) && (_nextScene != 0)) {
		// シーンの番号
		switch (_nextScene) {
			case SceneNum(ArcadeScene):
				MainSystem::Instance().SetNextMode("Arcade");	// メニューへ
				break;
			case SceneNum(MenuScene):
				MainSystem::Instance().SetNextMode("Menu");	// メニューへ
				break;
			case SceneNum(VersusScene):
				MainSystem::Instance().SetNextMode("Versus");	// 対戦画面へループ
				// アケなら
				if (Arcade_Switch(-1) == 1) {
					Replay_Mode(0);	// もう一度リプレイ
				}
				// アケじゃない
				else {
					if (BattleMode(-1) == 0)Replay_Mode(2);	// もう一度録画する
					else { Replay_Mode(0); }
				}
				break;
			case SceneNum(SelectScene):
				MainSystem::Instance().SetNextMode("Select");
				break;
		}
	}
	// 暗転進める
	if (_nextScene != 0)
		_timeStop++;
	
	
	//Draw();

	return 0;
}

void Result::Draw()
{
	if (time < 10)time++;

	// 白
	DrawBox(0, 0, SCREEN_W, SCREEN_H, Cr, true);

	// 勝利キャラの画像表示
	if (winChara[0] != -1) {
		for (int i = 0; i < winTexts.size(); i++) {
			if (charName[0] == winTexts[i].name) {
				DrawRotaGraph(SCREEN_W / 3 + 300 - (time * 30), SCREEN_H / 2, 0.8, 0, winTexts[i].image, true, false);	
				break;
			}
		}
	}
		

	if ((winChara[0] != -1) && (time == 10)){
		// ボックス
		// 描画ブレンドモードをアルファブレンド（透明）にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
		// 半透明に
		DrawBox(340 - 2, SCREEN_H / 2 - 122, 620 + 2, SCREEN_H / 2 - 80, GetColor(0, 0, 0), true);
		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// テキストを読み込んだ
		boolean textCheck = false;

		// テキスト表示
		// テキスト集から探す
		for (int i = 0; i < winTexts.size(); i++) {
			// 勝者と同じ名前
			if (charName[0] == winTexts[i].name) {
				for (int j = 0; j < winTexts[i].tex.size(); j++) {
					// 敵の名前同じ
					if (charName[1] == winTexts[i].tex[j].enemyName) {
						//DrawFormatString(400, 100, Oran, "一致した %s", winTexts[i].tex[j].enemyName.c_str());
						// テキスト
						DrawFormatString(340, SCREEN_H / 2 - 120, Cr, "%s", winTexts[i].tex[j].text[0].c_str());
						DrawFormatString(340, SCREEN_H / 2 - 100, Cr, "%s", winTexts[i].tex[j].text[1].c_str());
						textCheck = true;
						break;
					}
				}

				/*
				DrawFormatString(360, 160, Oran, "size:%d", winTexts[i].tex.size());

				DrawFormatString(360, 180, Oran, "%s", charName[0].c_str());
				DrawFormatString(360, 200, Oran, "%s", charName[1].c_str());
				*/

				

				// テキストがなかった
				if (!textCheck) {
					// 汎用セリフ
					if (winTexts[i].tex.size() > 0) {
						// テキストを汎用に
						DrawFormatString(340, SCREEN_H / 2 - 120, Cr, "%s", winTexts[i].tex[0].text[0].c_str());
						DrawFormatString(340, SCREEN_H / 2 - 100, Cr, "%s", winTexts[i].tex[0].text[1].c_str());
					}
					else 
					{
						// テキストを汎用に
						DrawString(340, SCREEN_H / 2 - 120, "勝利テキスト上段", Cr);
						DrawString(340, SCREEN_H / 2 - 100, "勝利テキスト下段", Cr);

						// 描画ブレンドモードをアルファブレンド（透明）にする
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
						// 半透明に
						DrawBox(100 - 2, 300 - 2, 620 + 2, 300 + 40, GetColor(0, 0, 0), true);
						// 描画ブレンドモードをノーブレンドにする
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
						DrawString(100, 300, "※勝利テキストが設定されていません", Oran);
						DrawString(100, 320, "　ファイルが存在しないか、記述にエラーがある可能性があります", Oran);

						// 勝利テキスト
						DrawFormatString(360, 160, Oran, "テキスト数:%d", winTexts[i].tex.size());

					}
				}

				break;
			}
			
		}

		/*
		// コーネル
		if (winChara[0] == CORNEL){
			if (winChara[1] == winChara[0]){
				DrawString(340, SCREEN_H / 2 - 120, "ハンターは二人もいらない", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "消えてもらうぞ", Cr);
			}
			else if(winChara[1] == EIN) {
				DrawString(340, SCREEN_H / 2 - 120, "さっさと失せろ、悪魔", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "死にたくなければな", Cr);
			}
			else if (winChara[1] == HELIOS) {
				DrawString(340, SCREEN_H / 2 - 120, "あまり調子に乗るなよ", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "", Cr);
			}
			else if (winChara[1] ==	BOUNCER) {
				DrawString(340, SCREEN_H / 2 - 120, "力だけで決まるような", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "", Cr);
			}

		}
		// バウンサー
		else if (winChara[0] == BOUNCER){
			if (winChara[1] == winChara[0]){
				DrawString(340, SCREEN_H / 2 - 120, "やめときな", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "真似できる戦い方じゃねぇぞ", Cr);
			}
			else if(winChara[1] == CORNEL){
				DrawString(340, SCREEN_H / 2 - 120, "その怒りがお前の原動力か", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "ぶつける相手がいるだけマシさ", Cr);
			}
			else if (winChara[1] == HELIOS) {
				DrawString(340, SCREEN_H / 2 - 120, "お前も喧嘩で飯食ってんだろ？", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "引き際覚えたほうがいいぜ", Cr);
			}
			else if (winChara[1] == EIN) {
				DrawString(340, SCREEN_H / 2 - 120, "その辺にしときな", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "お前さんに負けるほど鈍ってねぇ", Cr);
			}
			else if (winChara[1] == HYDE) {
				DrawString(340, SCREEN_H / 2 - 120, "とりあえず無事でよかったが", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "まだ問題は山積みみたいだな", Cr);
			}
			else if (winChara[1] == SYUICHI) {
				DrawString(340, SCREEN_H / 2 - 120, "気に食わねぇ奴だな", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "相手を試すような戦い方だった", Cr);
			}
			else{
				DrawString(340, SCREEN_H / 2 - 120, "まだ向上の余地がある", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "鍛えなおすんだな", Cr);
			}
		}
		// アイン
		else if (winChara[0] == EIN){
			if (winChara[1] == winChara[0]){
				DrawString(340, SCREEN_H / 2 - 120, "あっ、勝っちゃった", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "じゃあ私が本物ってことで！", Cr);
			}
			else if(winChara[1] == CORNEL){
				DrawString(340, SCREEN_H / 2 - 120, "これでおあいこですよ", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "人を悪魔呼ばわりしたんですから！", Cr);
			}
			else if (winChara[1] == HELIOS) {
				DrawString(340, SCREEN_H / 2 - 120, "これでおあいこですよ", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "人を悪魔呼ばわりしたんですから！", Cr);
			}
			else{
				DrawString(340, SCREEN_H / 2 - 120, "よし、いい調子！", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "このノリを大事にしたいな", Cr);
			}
		}
		else if (winChara[0] == HELIOS){
			if (winChara[1] == winChara[0]){
				DrawString(340, SCREEN_H / 2 - 120, "お前俺にそっくりだな！", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "生き別れの兄貴か！？", Cr);
			}
			else if (winChara[1] == CORNEL) {
				DrawString(340, SCREEN_H / 2 - 120, "悪魔が憎いのはわかったけどさ", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "お前も十分憎まれてそうだぜ", Cr);
			}
			else if (winChara[1] == BOUNCER) {
				DrawString(340, SCREEN_H / 2 - 120, "そんくらいで用心棒になれんのか", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "俺にもできそうだな！", Cr);
			}
			else if (winChara[1] == EIN) {
				DrawString(340, SCREEN_H / 2 - 120, "見た目通りの面白い動きだな！", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "まぁ強くはなかったけどな", Cr);
			}
			else if (winChara[1] == HYDE) {
				DrawString(340, SCREEN_H / 2 - 120, "暗い奴だねぇ…", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "じゃなきゃ殺し屋なんてなれないか", Cr);
			}
			else if (winChara[1] == SYUICHI) {
				DrawString(340, SCREEN_H / 2 - 120, "変な剣術だな、どこの国の奴だ？", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "俺のが強いことしかわかんねぇや", Cr);
			}
			else{
				DrawString(340, SCREEN_H / 2 - 120, "面白い奴だな", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "リベンジは歓迎だぜ！", Cr);
			}
		}
		// ダン
		else if (winChara[0] == HYDE){
			if (winChara[1] == winChara[0]){
				DrawString(340, SCREEN_H / 2 - 120, "同業者ならわかっているな", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "死んでもらうぞ", Cr);
			}
			else{
				DrawString(340, SCREEN_H / 2 - 120, "負けて当然だ", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "殺す覚悟もできていないならな", Cr);
			}
		}
		// シュウイチ
		else if (winChara[0] == SYUICHI) {
			if (winChara[1] == winChara[0]) {
				DrawString(340, SCREEN_H / 2 - 120, "見様見真似ではそれが限界だ", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "技だけでは俺には追いつけん", Cr);
			}
			else if (winChara[1] == CORNEL) {
				DrawString(340, SCREEN_H / 2 - 120, "怒りに身を任せて勝てるほど", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "甘い相手だと思ったか？", Cr);
			}
			else if (winChara[1] == HELIOS) {
				DrawString(340, SCREEN_H / 2 - 120, "銃と剣どちらも使えるのか", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "どちらも敵わなかったようだが", Cr);
			}
			else{
				DrawString(340, SCREEN_H / 2 - 120, "なかなかいい動きだったぞ", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "悪くはなかった", Cr);
			}
		}
		// 汎用
		else {
			if (winChara[1] == winChara[0]) {
				DrawString(340, SCREEN_H / 2 - 120, "同キャラ勝利セリフ", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "", Cr);
			}
			else {
				DrawString(340, SCREEN_H / 2 - 120, "勝利セリフ", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "", Cr);
			}
		}
		*/
	}

	// リザルトなら
	//(CheckGameMode() == SceneNum(ResultScene)) 
	if (time == 10){
		// 勝者はどっちだ
		if (side == 0){
			DrawString(0, 0, "draw", GetColor(0, 0, 0));
		}
		else if (side == 1){
			DrawString(0, 0, "1P win", GetColor(0, 0, 0));
		}
		else if (side == 2){
			DrawString(0, 0, "2P win", GetColor(0, 0, 0));
		}

		//------------
		// 対戦数表示
		//------------
		int pos[2];
		for (int i = 0; i < 2; i++){
			pos[i] = 0;
			if (winCount[i] >= 100)pos[i] = -20;
			else if (winCount[i] >= 10)pos[i] = -10;
		}

		DrawFormatString(0 + 80 + pos[0], 20, GetColor(0, 0, 0), "%d win", winCount[0]);
		DrawFormatString(SCREEN_W - 120 + pos[1], 20, GetColor(0, 0, 0), "%d win", winCount[1]);


		// 描画ブレンドモードをアルファブレンド（透明）にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
		// 半透明に
		DrawBox(280 - 2, SCREEN_H / 2 - 22, 368 + 2, SCREEN_H / 2 + 20, GetColor(0, 0, 0), true);
		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		if (nPos == 0)color[0] = Oran;
		else{ color[0] = Cr; }
		if (nPos == 1)color[1] = Oran;
		else{ color[1] = Cr; }

		// アケで2Pが勝っちゃった
		if (Arcade_Switch(-1) == 1){
			DrawString(280, SCREEN_H / 2 - 20, "再戦", color[0]);
			DrawString(280, SCREEN_H / 2, "メニュー", color[1]);
		}
		else {
			DrawString(280, SCREEN_H / 2 - 20, "再戦", color[0]);
			DrawString(280, SCREEN_H / 2, "キャラセレ", color[1]);
		}
	}

	// 次シーン移行の暗転
	if ((_nextScene != 0) && (_timeStop > 0)) {
		int num = _timeStop * 25;
		if (num > 255) num = 255;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, num);
		DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(255, 255, 255), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Result::Load_Reload()
{
	nPos = 0;
	BGMStart(0);	// BGMを停止
	time = 0;
	_timeStop = 0;
	_nextScene = 0;
}

void Result::Load_1second()
{

}

void Result::Release(void)
{
}

// 勝利キャラと名前
void Result_WinSide(int i, int chara, int chara2, char name1[NAME_MAX], char name2[NAME_MAX]){
	side = i;
	winChara[0] = chara;
	winChara[1] = chara2;
	charName[0] = name1;
	charName[1] = name2;
}

void Result_GetData(int c1, int c2, int cl1, int cl2, int stage) {
	p_name[0] = c1;
	p_name[1] = c2;
	p_color[0] = cl1;
	p_color[1] = cl2;
	StageNum = stage;
}

void WinCount(int side)
{
	if (side == 1)winCount[0]++;
	else if (side == 2)winCount[1]++;
	// リセット
	else if (side != 3){
		winCount[0] = 0;
		winCount[1] = 0;
	}
}

// 勝利セリフ読み込み
// csvで読み込み
void load_winText(char charName[NAME_MAX])
{
	// 判定セッティング //

	int i, fp;
	int enemyNum = 0;	// 敵の番号

	char inputc[TX_MAX];	// inputとinputcに文字がはいる
	int flg = -1;	// 代入する条件を決める
	boolean nameFlg = false;	// 名前スタート

	// 名前準備
	string fname, fn;
	fn = charName;
	fname = "char/" + fn + "/win.txt";

	// セット用
	WinText winText;
	TextBox textBox;

	// ファイルを開く //
	fp = FileRead_open(fname.c_str());//ファイル読み込み
	if (fp == NULL) {			// エラーが起こったら
		printfDx("error result_text\n");// 文字を表示
		return;
	}

	// キャラ名セット
	winText.name = fn;
	// 勝利絵セット
	fname = "char/" + fn + "/pal/stand.png";
	winText.image = LoadGraph(fname.c_str());

	//string copy;	// stringとして使う用

	// ファイル終わりまで
	while (FileRead_eof(fp) == 0) {
		// 初期化
		inputc[0] = '\0';	// 先頭をnullにする
	
		FileRead_gets(inputc, TX_MAX, fp);//1文字取得する
		// 文字列の解読
		for (i = 0; i < TX_MAX; i++)
		{
			// 終了
			if (inputc[i] == ']' && (i > 0) && nameFlg) {
				inputc[i] = '\0';
				flg = 1;	// 名前セットフラグ
				// inputcを作り直す
				for (int n = 0; n < i; n++) {
					// 63以下なら
					if(n < TX_MAX - 1)
						inputc[n] = inputc[n + 1];	// ずらす
				}
				break;
			}

			// 1文字目があれ
			if (inputc[i] == '[' && (i == 0)) {
				nameFlg = true;
			}
		}

		// 文字数がある
		// 代入する
		switch (flg) {
		case 1:	// gsize
			textBox.enemyName = inputc;
			flg = 2;	// テキストへ
			nameFlg = false;
			//printfDx("flg1 ");// 文字を表示
			break;
		case 2:	// テキスト1
			textBox.text[0] = inputc;
			flg = 3;	// テキスト改行へ
			//printfDx("flg2 ");// 文字を表示
			break;
		case 3:	// テキスト2
			textBox.text[1] = inputc;
			flg = -1;	// テキストへ

			// 追加する
			winText.tex.push_back(textBox);

			//printfDx("flg3\n");// 文字を表示
			break;
		default: break;
		}
		// numを進める	
	}	// while ループ終了

	// ファイルを閉じる
//EXFILE:
	FileRead_close(fp);

	printfDx("\n");// 文字を表示

	//文字をセットする
	winTexts.push_back(winText);
}

// Nameを取得
void GetN_Result(vector<Name_t>GN)
{
	for (int i = 0; i < Character_Max(); i++) {
		N.push_back(GN[i]);
		// 勝利テキスト読み込み
		load_winText(N[i].nameC);
	}
}
