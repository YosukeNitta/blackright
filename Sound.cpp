#include "pch.h"

using namespace std;

#define	BGM_MAX		(20)
#define	SE_MAX		(50)

static vector<int> bgm;
static vector<int> se;
static vector<vector<int>> voice;	// テスト用
static vector<int> pVoice[2];	// テスト用

//static int se[SE_MAX];
static boolean LoadB = false;
static boolean LoadS = false;
static boolean LoadV = false;
static int volumeB, volumeS;
static int setB, setS;
static int n_bgm;	// 今のBGM
static boolean noSound;	// サウンドonoff
static int noSoundSw = 0;

void GetSoundData();

int BGMStart(int num)
{

	// BGMをロード
	if (LoadB == 0)
	{
		// 空のを作成
		for (int i = 0; i < BGM_MAX; i++){
			bgm.push_back(0);
		}

		// 非同期読み込み設定に変更
		SetUseASyncLoadFlag(TRUE);

		for (int i = 0; i < STAGE_MAX; i++){
			bgm[i + 1] = LoadSoundMem("music/01.mp3");	// ステージ曲 
			//if(i == 2)bgm[i + 1] = LoadSoundMem("music/03.mp3");	// ステージ曲 
		}

		bgm[11] = LoadSoundMem("music/menu.mp3");	// メニュー
		bgm[12] = LoadSoundMem("music/select.mp3");	// キャラセレ
		//bgm[13] = LoadSoundMem("music/versus.mp3");	// 対戦前
		//bgm[14] = LoadSoundMem("music/win.mp3");		// 勝利画面 
		//bgm[15] = LoadSoundMem("music/versus.mp3");	// 対戦前

		// 非同期読み込み設定に変更
		SetUseASyncLoadFlag(FALSE);

		// 音量読み込み
		volumeB = GetPrivateProfileInt("Config", "BGM", 10, "config.ini");
		
		// 音量設定
		setB = (int)(volumeB * 23);
		//if (setB > 255)setB = 255;
		//else if (setB < 0)setB = 0;
		// ↑のかわり
		max(0,min(255,setB));	// 右に今の数値を
		// 音量の設定
		for (int i = 0; i < BGM_MAX; i++){
			ChangeVolumeSoundMem(setB, bgm[i]);
		}

		n_bgm = 0;
		LoadB = 1;
	}

	for (int i = 0; i < BGM_MAX; i++){
		// 他のBGMを止める
		StopSoundMem(bgm[i]);
	}
	//ChangeVolumeSoundMem(setB, bgm[num]);
	if (!noSound){
		// BGM最初から流さない
		if (n_bgm == num)PlaySoundMem(bgm[num], DX_PLAYTYPE_LOOP, false);	//バックグラウンドで再生;
		else{ PlaySoundMem(bgm[num], DX_PLAYTYPE_LOOP); }	//バックグラウンドで再生
	}

	n_bgm = num;

	return 0;
}

int SEStart(int num2)
{
	// BGMをロード
	if (LoadS == 0)
	{
		// 空のを作成
		for (int i = 0; i < SE_MAX; i++){
			se.push_back(0);
		}

		// 非同期読み込み設定に変更
		SetUseASyncLoadFlag(TRUE);

		GetSoundData();
		/*
		se[1] = LoadSoundMem("sound/01_swing.wav"); // サウンドをロード
		*/

		// 非同期読み込み設定に変更
		SetUseASyncLoadFlag(FALSE);
		


		volumeS = GetPrivateProfileInt("Config", "SE", 10, "config.ini");

		// SE版
		setS = (int)(volumeS * 25);
		if (setS > 255)setS = 255;
		else if (setS < 0)setS = 0;
		// 音量の設定
		for (int i = 0; i < SE_MAX; i++){
			ChangeVolumeSoundMem(setS, se[i]);
		}

		LoadS = 1;
	}

	//if (se > se.size)return 0;

	// 音を鳴らす
	if ((se[num2] != 0) && (!noSound)){
		PlaySoundMem(se[num2], DX_PLAYTYPE_BACK);	//バックグラウンドで再生
	}

	return 0;
}

int VOStart(int ch, int num, int percent)
{
	/*

	// BGMをロード
	if (LoadV == 0)
	{
		// 空のを作成
		for (int n = 0; n < CHARACTERS_MAX; n++){
			voice.push_back(vector<int>());
			for (int i = 0; i < SE_MAX; i++){
				voice[n].push_back(0);
			}
		}
		// 空のを作成
		for (int n = 0; n < 2; n++){
			for (int i = 0; i < SE_MAX; i++){
				pVoice[n].push_back(0);
			}
		}

		int homo;
		homo = EIN;
		voice[homo][1] = LoadSoundMem("sound/ein/01.wav"); // ふっ
		voice[homo][2] = LoadSoundMem("sound/ein/02.wav"); // はっ
		voice[homo][3] = LoadSoundMem("sound/ein/03.wav"); // そこっ
		voice[homo][10] = LoadSoundMem("sound/ein/10.wav"); // 投げ
		voice[homo][11] = LoadSoundMem("sound/ein/11.wav"); // 投げスカ
		voice[homo][15] = LoadSoundMem("sound/ein/15.wav"); // ここだ
		voice[homo][16] = LoadSoundMem("sound/ein/16.wav"); // くらえ
		voice[homo][17] = LoadSoundMem("sound/ein/17.wav"); // よっと
		voice[homo][18] = LoadSoundMem("sound/ein/18.wav"); // そこだ
		voice[homo][19] = LoadSoundMem("sound/ein/19.wav"); // あたれ
		voice[homo][20] = LoadSoundMem("sound/ein/20.wav"); // 必中
		voice[homo][21] = LoadSoundMem("sound/ein/21.wav"); // いっけー
		voice[homo][22] = LoadSoundMem("sound/ein/22.wav"); // 逃がさない
		voice[homo][23] = LoadSoundMem("sound/ein/23.wav"); // 今だ
		voice[homo][25] = LoadSoundMem("sound/ein/25.wav"); // 風よ
		voice[homo][26] = LoadSoundMem("sound/ein/26.wav"); // デヤーッ
		voice[homo][31] = LoadSoundMem("sound/ein/31.wav"); // 被ダメ
		voice[homo][32] = LoadSoundMem("sound/ein/32.wav"); // 
		voice[homo][34] = LoadSoundMem("sound/ein/34.wav"); // まだ追いつける
		voice[homo][35] = LoadSoundMem("sound/ein/35.wav"); // まだ追いつける

		homo = BOUNCER;
		voice[homo][1] = LoadSoundMem("sound/bouncer/01.wav"); // ふっ
		voice[homo][2] = LoadSoundMem("sound/bouncer/02.wav"); // はっ
		voice[homo][3] = LoadSoundMem("sound/bouncer/03.wav"); // そこっ
		voice[homo][4] = LoadSoundMem("sound/bouncer/04.wav"); // 押し通る
		voice[homo][5] = LoadSoundMem("sound/bouncer/05.wav"); // そらよ
		voice[homo][6] = LoadSoundMem("sound/bouncer/06.wav"); // どあー
		voice[homo][10] = LoadSoundMem("sound/bouncer/10.wav"); // 投げ
		voice[homo][11] = LoadSoundMem("sound/bouncer/11.wav"); // 投げスカ
		voice[homo][12] = LoadSoundMem("sound/bouncer/12.wav"); // 全力でこい
		voice[homo][13] = LoadSoundMem("sound/bouncer/13.wav"); // 上等
		voice[homo][20] = LoadSoundMem("sound/bouncer/20.wav"); // 全開
		voice[homo][25] = LoadSoundMem("sound/bouncer/25.wav"); // デヤーッ
		voice[homo][21] = LoadSoundMem("sound/bouncer/21.wav"); // ずおりゃあああ
		voice[homo][31] = LoadSoundMem("sound/bouncer/31.wav"); // 被ダメ
		voice[homo][32] = LoadSoundMem("sound/bouncer/32.wav"); // 
		voice[homo][34] = LoadSoundMem("sound/bouncer/34.wav"); // まだ追いつける
		voice[homo][35] = LoadSoundMem("sound/bouncer/35.wav"); // まだ追いつける

		int size = setS*1.2;
		if (size > 255)size = 255;

		// 音量の設定
		for (int n = 0; n < CHARACTERS_MAX; n++){
			for (int i = 0; i < SE_MAX; i++){
				ChangeVolumeSoundMem(setS*1.5, voice[n][i]);
			}
		}

		LoadV = 1;
	}

	int per = GetRand(percent);

	for (int i = 0; i < SE_MAX; i++){
		// 同じSEだけ止める
		StopSoundMem(voice[ch][num]);
	}
	
	// 同じSEだけ止める
	StopSoundMem(voice[ch][num]);

	// 音を鳴らす
	if (per == 0){
		PlaySoundMem(voice[ch][num], DX_PLAYTYPE_BACK);	//バックグラウンドで再生
	}

	*/
	return 0;
}

void PVoiceSet(int pside, int name)
{
	/*
	// 音量の設定
	for (int i = 0; i < SE_MAX; i++){
		pVoice[pside - 1][i] = voice[name][i];
	}

	if (name == EIN){
		pVoice[pside - 1][1] = LoadSoundMem("sound/ein/01.wav"); // ふっ
		pVoice[pside - 1][2] = LoadSoundMem("sound/ein/02.wav"); // はっ
		pVoice[pside - 1][3] = LoadSoundMem("sound/ein/03.wav"); // そこっ
		pVoice[pside - 1][10] = LoadSoundMem("sound/ein/10.wav"); // 投げ
		pVoice[pside - 1][11] = LoadSoundMem("sound/ein/11.wav"); // 投げスカ
		pVoice[pside - 1][15] = LoadSoundMem("sound/ein/15.wav"); // ここだ
		pVoice[pside - 1][16] = LoadSoundMem("sound/ein/16.wav"); // くらえ
		pVoice[pside - 1][17] = LoadSoundMem("sound/ein/17.wav"); // よっと
		pVoice[pside - 1][18] = LoadSoundMem("sound/ein/18.wav"); // そこだ
		pVoice[pside - 1][19] = LoadSoundMem("sound/ein/19.wav"); // あたれ
		pVoice[pside - 1][20] = LoadSoundMem("sound/ein/20.wav"); // 必中
		pVoice[pside - 1][21] = LoadSoundMem("sound/ein/21.wav"); // いっけー
		pVoice[pside - 1][22] = LoadSoundMem("sound/ein/22.wav"); // 逃がさない
		pVoice[pside - 1][23] = LoadSoundMem("sound/ein/23.wav"); // 今だ
		pVoice[pside - 1][25] = LoadSoundMem("sound/ein/25.wav"); // 風よ
		pVoice[pside - 1][26] = LoadSoundMem("sound/ein/26.wav"); // デヤーッ
		pVoice[pside - 1][31] = LoadSoundMem("sound/ein/31.wav"); // 被ダメ
		pVoice[pside - 1][32] = LoadSoundMem("sound/ein/32.wav"); // 
		pVoice[pside - 1][34] = LoadSoundMem("sound/ein/34.wav"); // まだ追いつける
		pVoice[pside - 1][35] = LoadSoundMem("sound/ein/35.wav"); // まだ追いつける
	}

	if (name == BOUNCER){
		pVoice[pside - 1][1] = LoadSoundMem("sound/bouncer/01.wav"); // ふっ
		pVoice[pside - 1][2] = LoadSoundMem("sound/bouncer/02.wav"); // はっ
		pVoice[pside - 1][3] = LoadSoundMem("sound/bouncer/03.wav"); // そこっ
		pVoice[pside - 1][4] = LoadSoundMem("sound/bouncer/04.wav"); // 押し通る
		pVoice[pside - 1][5] = LoadSoundMem("sound/bouncer/05.wav"); // そらよ
		pVoice[pside - 1][6] = LoadSoundMem("sound/bouncer/06.wav"); // どあー
		pVoice[pside - 1][10] = LoadSoundMem("sound/bouncer/10.wav"); // 投げ
		pVoice[pside - 1][11] = LoadSoundMem("sound/bouncer/11.wav"); // 投げスカ
		pVoice[pside - 1][12] = LoadSoundMem("sound/bouncer/12.wav"); // 全力でこい
		pVoice[pside - 1][13] = LoadSoundMem("sound/bouncer/13.wav"); // 上等
		pVoice[pside - 1][20] = LoadSoundMem("sound/bouncer/20.wav"); // 全開
		pVoice[pside - 1][25] = LoadSoundMem("sound/bouncer/25.wav"); // デヤーッ
		pVoice[pside - 1][21] = LoadSoundMem("sound/bouncer/21.wav"); // ずおりゃあああ
		pVoice[pside - 1][31] = LoadSoundMem("sound/bouncer/31.wav"); // 被ダメ
		pVoice[pside - 1][32] = LoadSoundMem("sound/bouncer/32.wav"); // 
		pVoice[pside - 1][34] = LoadSoundMem("sound/bouncer/34.wav"); // まだ追いつける
		pVoice[pside - 1][35] = LoadSoundMem("sound/bouncer/35.wav"); // まだ追いつける
	}

	// 音量の設定
	for (int i = 0; i < SE_MAX; i++){
		ChangeVolumeSoundMem(setS*1.5, pVoice[pside - 1][i]);
	}
	*/
}


void StopSE(int num)
{
	for (int i = 0; i < SE_MAX; i++){
		// 全てのSEを止める
		StopSoundMem(se[num]);
	}
}

int PVOStart(int pside, int num, int percent)
{
	/*
	int per = GetRand(percent);

	for (int i = 0; i < SE_MAX; i++){
		// 同じSEだけ止める
		StopSoundMem(pVoice[pside - 1][i]);
	}

	// 音を鳴らす
	if (per == 0){
		PlaySoundMem(pVoice[pside-1][num], DX_PLAYTYPE_BACK);	//バックグラウンドで再生
	}
	*/

	return 0;
}

void StopSound(int num)
{
	if (num > 0)noSoundSw++;
	else{ noSoundSw = 0; }

	if (noSoundSw == 1){
		if (noSound)noSound = false;
		else{ noSound = true; }
	}
}

void GetSoundData()
{
	// 判定セッティング //

	int i, fp;
	int anum;	// Nameの場所
	int num;	// セット番号
	anum = -1;

	char fname[15] = { "sound/list.txt" };
	
	// 名前読み込み
	string setName;
	string fn1 = "sound/";
	
	int input[NAME_MAX];
	char inputc[NAME_MAX];	// inputとinputcに文字がはいる
	boolean iflg = false;	// 名前入力フラグ
	boolean next = false;

	// ファイルを開く //
	fp = FileRead_open(fname);//ファイル読み込み
	if (fp == NULL) {			// エラーが起こったら
		printfDx("error name\n");// 文字を表示
		return;
	}

	//最初の1行読み飛ばす
	while (FileRead_getc(fp) != '\n'); // FileRead_getc:一文字読み出す

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
					inputc[j] = input[j] = NULL;
					//inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				}
				i = -1;
				break;
			}

			// 文字列にする
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
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
		/*
		iflg = false;
		{
			if ((inputc[0] != '\0') && (inputc[0] != '\n') && (inputc[0] != NULL)
				&& (anum < Character_Max()))iflg = true;
		}
		*/

		// 最初に数字扱いにして、文字が入ってたら変更する
		iflg = false;

		// 文字列セットに移行していない
		if(!next)
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
			num = atoi(inputc);	// 番号をセット
			next = true;	// 次の準備
		}

		// 文字列セット
		if (next && (inputc[0] != '\0' || inputc[0] != NULL || inputc[0] != '\n')) {
			//strcpy_s(buf, inputc);
			setName = fn1 + inputc;
			se[num] = LoadSoundMem(setName.c_str());
			next = false;
		}

		if (input[i] == EOF) {//ファイルの終わりなら
			goto EXFILE;//終了
		}
	}

	// ファイルを閉じる
EXFILE:
	FileRead_close(fp);

	// PictにNameを送る
	//GetN_Pict(N);
}