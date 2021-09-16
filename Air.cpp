//==================================================================================
//
//			数値計算の管理部
//
//========================================================================

//インクルード部--------------------------------------------
#include "pch.h"
#include <stdio.h>	// ファイル操作に必要なヘッダファイル
#include <string.h>	// 文字列操作に必要なヘッダファイル
#include <ctype.h>	// isdigitに必要

#include <string>	// stringに使う
#include <vector>

//デファイン部----------------------------------------------
#define BOX_MAX	(10)

//ﾃﾞｰﾀ定義部------------------------------------------------

// 二重にAirをセット


static Air air[STATE_MAX];
//static Air kawari[STATE_MAX];	// 代入用

//static Name_t N[C_M]; 
static std::vector<Name_t>N;
static boolean load_1;

// 名前用
static std::string fn1("char/");
static std::string fn2("/box.air");

//内部関数宣言部--------------------------------------------
static char t_Action[7] = { "Action" };	// sffno
static char t_Clsn2Default[13] = { "Clsn2Default" };	// num1,2
static char t_Clsn1[6] = { "Clsn1" };			// sizeX
static char t_Clsn2[6] = { "Clsn2" };			// sizeY
static char t_Loopstart[10] = { "Loopstart" };	// posX
static char t_H[2] = { "H" };
static char t_V[2] = { "V" };
static char t_HV[3] = { "HV" };
static char t_A[2] = { "A" };

//プログラム部----------------------------------------------

// mugen記述をコピーする
// box.airでそのままもってこれる
void load_air(int na, int pSide)
{
	// 判定セッティング //
	int i, fp, num;
	int bo = 0;	 // ボックス番号で使用
	int	ht = 0, at = 0;	// Stateno
	int clsnMax = -1;	// clsnの数
	int clsnCount = 0;	// clsnを通した回数(0は判定数カウントに使う)
	boolean clsnDefault = false;	// clsnDefaultが有効か
	boolean clsnSet = false;	// clsnDefaultを一度使ったか
	int anum = -1;	// 配列の場所

	//int input[TX_MAX];
	int *input;
	input = (int *)malloc(TX_MAX);
	if (input == NULL) {
		printfDx("input読み込めず");
	}

	char inputc[TX_MAX];	// inputとinputcに文字がはいる
	int flg = 0;	// 代入する条件を決める
	boolean iflg;	// 数字フラグ

	////
	// 元はここで文字の関数宣言

	// 名前準備
	std::string fname;
	fname = fn1 + N[na].nameC + fn2;	// 名前決定

	// AIR変数準備 //
	for (int n = 0; n < STATE_MAX; n++){
		air[n].setup();
	}
	
	// ファイルを開く //
	fp = FileRead_open(fname.c_str());//ファイル読み込み c_strでcharにできる？
	if (fp == NULL){			// エラーが起こったら
		printfDx("error air\n");// 文字を表示
		return;
	}
	for (i = 0; i < 2; i++){//最初の2行読み飛ばす
		while (FileRead_getc(fp) != '\n');
	}

	while (1){
		//=========================
		// 基本ループ
		// このループ終了後に下へ
		//=========================
		for (i = 0; i < TX_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1文字取得する

			// カンマで改行
			if (inputc[i] == ';'){					//スラッシュがあれば
				while (FileRead_getc(fp) != '\n');	//改行までループ
				i = -1;//カウンタを最初に戻して (戻ったときに0になる)
				continue;
			}

			// 改行で次の行に進む
			if (input[i] == '\n'){
				inputc[i] = '\0';
				i = -1;
				break;
			}

			// 文字列にする
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == '/' || input[i] == ':' ||
				input[i] == '='){ //カンマか改行なら
				inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）


				break;	// iを1増やしてループ脱出（代入する）
				// 脱出させるとiが1増える?
			}

			// 読み込み終了
			if (input[i] == EOF){//ファイルの終わりなら
				goto EXFILE;//終了
			}
		}	// 基本ループ終了



		// 最初に数字扱いにして、文字が入ってたら変更する
		iflg = false;
		{
			if (inputc[0] == '0' || inputc[0] == '1' ||
				inputc[0] == '2' || inputc[0] == '3' || 
				inputc[0] == '4' || inputc[0] == '5' || 
				inputc[0] == '6' || inputc[0] == '7' || 
				inputc[0] == '8' || inputc[0] == '9' ||
				inputc[0] == '-')iflg = true;

			// 特殊(HVの判定)
			if ((flg == 5) && (num == 5)){
				if (iflg)
				{
					flg = 5;
					num = 0;
					bo++;
				}

				if (inputc[0] == 'H'){
					air[anum].B[bo].HV = 1;
					iflg = true;
				}
				else if (inputc[0] == 'V'){
					air[anum].B[bo].HV = 2;
					iflg = true;
				}
				else if (inputc[0] == 'H' && inputc[1] == 'V'){
					air[anum].B[bo].HV = 3;
					iflg = true;
				}
			}

		}

		// 全て数字だったら
		if (iflg){
			// 代入する
			switch (flg){
			case 1:	// Stateno(Action)

				anum = atoi(inputc);
				air[anum].stateno = atoi(inputc);	// ステート番号を決定
				air[anum].ANum = 0;	// ここで0にする
				flg = 5;	// 画像設定(time)に移行
				num = 0;

				// 初期設定を行う
				clsnMax = -1;
				clsnCount = 0;
				clsnDefault = false;
				bo = 0;		// ボックス番号で使用
				at = 0;
				ht = 0;

				break;

			case 2: // Clsn2Default 

				// これの後、clsn2の内容を決定
				clsnMax = atoi(inputc);
				clsnDefault = true;
				air[anum].def = true;
				flg = 0, num = 0;
				clsnCount = 1;
				break;

			case 3:	// Clsn1
				if (clsnCount == 0){
					// clsnDefaultを通っていない場合
					clsnMax = atoi(inputc);
					flg = 0, num = 0;
					clsnCount++;
				}
				else {
					switch (num){
					case 0:
						at = atoi(inputc),
							air[anum].B[bo].a[at] = true;
						break;
					case 1: air[anum].B[bo].ax[at] = atoi(inputc); break;
					case 2: air[anum].B[bo].ay[at] = atoi(inputc); break;
					case 3: air[anum].B[bo].aw[at] = atoi(inputc); break;
					case 4: air[anum].B[bo].ah[at] = atoi(inputc); 
						// 反対だったら戻す
						if (air[anum].B[bo].ax[at] > air[anum].B[bo].aw[at]){
							int hozon = air[anum].B[bo].ax[at];
							air[anum].B[bo].ax[at] = air[anum].B[bo].aw[at];
							air[anum].B[bo].aw[at] = hozon;
						}
						if (air[anum].B[bo].ay[at] > air[anum].B[bo].ah[at]){
							int hozon = air[anum].B[bo].ay[at];
							air[anum].B[bo].ay[at] = air[anum].B[bo].ah[at];
							air[anum].B[bo].ah[at] = hozon;
						}
						break;
					}
					if (num < 4){ num++; }
					// clsn2終了
					else{
						// カウントが最大数と同じになったら
						if (clsnCount >= clsnMax){
							flg = 5, num = 0;	// timeに移行
							clsnCount = 0;
							clsnMax = -1;
						}
						// まだclsnを続行
						else{
							flg = 0, num = 0;
							clsnCount++;

						}
					}
				}
				break;
			case 4:	// Clsn2
				if (clsnCount == 0){
					// clsnDefaultを通っていない場合
					clsnMax = atoi(inputc);
					flg = 0, num = 0;
					clsnCount++;
				}
				else {
					switch (num){
					case 0:
						ht = atoi(inputc),
						air[anum].B[bo].h[ht] = true;
						break;
					case 1: air[anum].B[bo].hx[ht] = atoi(inputc); break;
					case 2: air[anum].B[bo].hy[ht] = atoi(inputc); break;
					case 3: air[anum].B[bo].hw[ht] = atoi(inputc); break;
					case 4: air[anum].B[bo].hh[ht] = atoi(inputc); 
						// 判定が反対だったら戻す
						if (air[anum].B[bo].hx[ht] > air[anum].B[bo].hw[ht]){
							int hozon = air[anum].B[bo].hx[ht];
							air[anum].B[bo].hx[ht] = air[anum].B[bo].hw[ht];
							air[anum].B[bo].hw[ht] = hozon;
						}
						if (air[anum].B[bo].hy[ht] > air[anum].B[bo].hh[ht]){
							int hozon = air[anum].B[bo].hy[ht];
							air[anum].B[bo].hy[ht] = air[anum].B[bo].hh[ht];
							air[anum].B[bo].hh[ht] = hozon;
						}
						break;
					}

					// まだclsn2がある
					if (num < 4){ num++; }
					// clsn2終了	
					else{
						// カウントが最大数と同じになったら
						if (clsnCount >= clsnMax){
							flg = 5, num = 0;	// timeに移行
							clsnCount = 0;
							clsnMax = -1;
							// デフォルトボックス用意
							if (clsnDefault){
								for (int d = 0; d < HIT_MAX; d++){
									air[anum].defhx[d] = air[anum].B[bo].hx[d];
									air[anum].defhy[d] = air[anum].B[bo].hy[d];
									air[anum].defhw[d] = air[anum].B[bo].hw[d];
									air[anum].defhh[d] = air[anum].B[bo].hh[d];
								}
								clsnDefault = false;
							}
						}
						else{
							flg = 0, num = 0;
							clsnCount++;
						}
					}// num4以上終了
				}
				break;
			case 5:	// time(画像)
				switch (num){

				case 0: air[anum].B[bo].num1 = atoi(inputc); break;
				case 1: air[anum].B[bo].num2 = atoi(inputc); break;
				case 2: air[anum].B[bo].posX = atoi(inputc); break;
				case 3: air[anum].B[bo].posY = atoi(inputc); break;
				case 4: air[anum].B[bo].time = atoi(inputc);
					// デフォルトボックス用意
					if ((air[anum].B[bo].hw[0] == 0) && (air[anum].B[bo].hh[0] == 0) && air[anum].def && (bo > 0)){
						for (int d = 0; d < HIT_MAX; d++){
							air[anum].B[bo].h[d] = true;
							air[anum].B[bo].hx[d] = air[anum].defhx[d];
							air[anum].B[bo].hy[d] = air[anum].defhy[d];
							air[anum].B[bo].hw[d] = air[anum].defhw[d];
							air[anum].B[bo].hh[d] = air[anum].defhh[d];
						}
					}
					break;

				}
				if (num < 5){
					num++;
				}
				// case4を通って続行
				else{
					flg = 5, num = 0;
					bo++;	// 次へ進める
				}
				break;

			default: break;
			}

		}

		// 数字ではない
		if (!iflg){
			// ステート、判定数、当たり、喰らいの判断
			// "Action "
			if (strcmp(inputc, t_Action) == 0){
				flg = 1; clsnSet = false;
			}
			// "Clsn2Default "
			else if (strcmp(inputc, t_Clsn2Default) == 0){
				// まだ使ってなかったら
				if (!clsnSet){
					flg = 2; clsnSet = true;
				}
			}
			// "Clsn1 "
			else if (strcmp(inputc, t_Clsn1) == 0){
				if ((flg == 5) && (num == 5))bo++;
				flg = 3;
			}
			// "Clsn2 "
			else if (strcmp(inputc, t_Clsn2) == 0){
				if ((flg == 5) && (num == 5))bo++;
				flg = 4;
			}
			// "Loopstart "
			//else if (strcmp(inputc, t_Loopstart) == 0){
			//flg = 5, num = 0;
			//}
		}


	}	// while ループ終了


	// ファイルを閉じる
EXFILE:
	FileRead_close(fp);
	
	// aTime決定
	int time;
	// セットスタート
	for (int n1 = 0; n1 < STATE_MAX; n1++){
		time = 0;
		for (int a = 0; a < STATE2_MAX; a++){
			// 終了でないなら
			if (air[n1].B[a].time != -2)
				time += air[n1].B[a].time;
		}
		air[n1].aTime = time;
	}

	// airに代入
	for (int n = 0; n < STATE_MAX; n++) {
		// 判定がないなら設定をチェック
		if (air[n].stateno == -1) {
			// 立ちガード(実際はしゃがみ)
			if (n == 53) {
				air[n] = air[51];	// 基本しゃがみ
				// 絵のみ立ち
				for (int aa = 0; aa < 50; aa++) {
					air[n].B[aa].num1 = air[50].B[aa].num1;
					air[n].B[aa].num2 = air[50].B[aa].num2;
				}
			}
			// しゃがみガード(実際は立ち)
			if (n == 54) {
				air[n] = air[50];
				for (int aa = 0; aa < 50; aa++) {
					air[n].B[aa].num1 = air[51].B[aa].num1;
					air[n].B[aa].num2 = air[51].B[aa].num2;
				}
			}

			// 投げ抜け
			if (n == 55) {
				air[n] = air[50];
			}
			else if (n == 56) {
				air[n] = air[52];
			}

			// リフガ
			if (n == 57) {
				air[n] = air[50];
			}
			else if (n == 58) {
				air[n] = air[51];
			}
			else if (n == 59) {
				air[n] = air[52];
			}

			// 空ダ
			else if (n == 60) {
				air[n] = air[30];
			}
			else if (n == 65) {
				air[n] = air[43];
			}

			// ガーキャン
			else if (n == 520) {
				air[n] = air[210];
			}

			// 追加項目 //
			// ゲージ増加
			else if (n == 550) {
				air[n] = air[520];
			}
			// バースト
			else if (n == 551) {
				air[n] = air[1085];
			}

			// バウンド
			else if (n == 1040) {
				air[n] = air[1035];
			}
			// バウンド2
			else if (n == 1041) {
				air[n] = air[1035];
			}
			// 壁（大）
			else if (n == 1046) {
				air[n] = air[1035];
			}
			// 地上受け身
			else if (n == 1080) {
				air[n] = air[1085];
			}
		}
	}

	// メモリ解放
	free(input);

	//GetA_GHitbox(air2[na], na);	// GHitBoxに送る
	//GetA_Select(air2[na][0], na);

	//@@
	//GetA_GHitbox(air, na);
	GetA_GHitbox(air, pSide);	// GHitBoxに送る
							// これを毎度行うようにする

	// 立ちモーション
	GetA_Select(air[0], na);
}

// stateからのP1を受け取る
void GetP_Air(Player GP1)
{
	P1 = GP1;
}

void GetName()
{
	// 判定セッティング //

	int i, fp;
	int anum;	// Nameの場所
	anum = -1;

	char fname[14] = { "char/name.txt" };

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// inputとinputcに文字がはいる
	boolean iflg;	// 名前入力フラグ
	

	// ファイルを開く //
	fp = FileRead_open(fname);//ファイル読み込み
	if (fp == NULL){			// エラーが起こったら
		printfDx("error name\n");// 文字を表示
		return;
	}
	
	//最初の1行読み飛ばす
	while (FileRead_getc(fp) != '\n'); // FileRead_getc:一文字読み出す


	while (1){
		//=========================
		// 基本ループ
		// このループ終了後に下へ
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1文字取得する

			// スラッシュで改行
			if (inputc[i] == '/'){					//スラッシュがあれば
				while (FileRead_getc(fp) != '\n');	//改行までループ
				i = -1;//カウンタを最初に戻して (戻ったときに0になる)
				continue;
			}

			// 改行で次の行に進む
			if (input[i] == '\n'){
				for (int j = 0; j < NAME_MAX; j++){
					inputc[j] = input[j] = NULL;
					//inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				}
				i = -1;
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
				goto EXFILE;//終了
			}
		}	// 基本ループ終了



		// 最初に数字扱いにして、文字が入ってたら変更する
		iflg = false;
		{
			if ((inputc[0] != '\0') && (inputc[0] != '\n') && (inputc[0] != NULL)
				&& (anum < Character_Max()))iflg = true;
		}
		
		// 名前だったら
		if (iflg){
			N.push_back(Name_t());	// 追加
			anum++;	// 最初に0になる
			N[anum].name = anum + 1;	// 最初..1
			strcpy_s(N[anum].nameC, inputc);
		}

		if (input[i] == EOF){//ファイルの終わりなら
			goto EXFILE;//終了
		}
	}

	// ファイルを閉じる
EXFILE:
	FileRead_close(fp);

	// PictにNameを送る
	GetN_Pict(N);
	GetN_GHitBox(N);
	// Selectにも送る
	Select_GetName(N);
	GetN_SameParam(N);
	GetN_Result(N);
}

// P1のNameに合ったものをnameCにコピー
void GetPName()
{
	for (int n = 0; n < Character_Max(); n++){
		if (P1.Name == N[n].name){
			strcpy_s(P1.nameC, N[n].nameC);
		}
	}
	// GHitboxに送る
	if (P1.PSide == 1)Get_P1Set(P1);
	if (P1.PSide == 2)Get_P2Set(P1);

	// ここでNを消す？
}

/*
Player GetAnimElem(Player GP1)
{
	for (int n = 0; n < Character_Max(); n++){
		// セットスタート
		if (GP1.Name == N[n].name){
			for (int n1 = 0; n1 < STATE_MAX; n1++){
				GP1.animElem[n1] = air2[n][n1].aTime;
			}
			break;
		}
	}
	
	return GP1;
}
*/