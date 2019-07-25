#include "pch.h"

#include <stdio.h>	// ファイル操作に必要なヘッダファイル
#include <string.h>	// 文字列操作に必要なヘッダファイル
#include <ctype.h>	// isdigitに必要

#include <string>	// stringに使う
#include <vector>	// 文字列操作に必要なヘッダファイル

using namespace std;	// 名前空間らしい、これでstd::と書く必要がなくなる

//デファイン部----------------------------------------------

//ﾃﾞｰﾀ定義部------------------------------------------------
static vector<string>tlist;

// ステートに入れるデータ
class stateData{
public:
	stateData(){

	}
	void math(){

	}
};

// ステート
// この中に指示を入れる
class state{
private:
	int stateNum;	// ステートの番号[state ○○]
	//vector<stateData>sdata;
public: 
	// コンストラクタ
	state(){
		stateNum = -1;
	}
	state(int num){
		stateNum = num;
	}

	// stateNum
	int Get_stateNum(int n){
		stateNum = n;
	}
	int Send_stateNum(){
		return stateNum;
	}
};

// これに全ての内容が入る
static vector<state>stateBox;

//内部関数宣言部--------------------------------------------
static char t_Common[7] = { "Common" };	//

static string fn1("char/");
static string fn2("/st.txt");

//プログラム部----------------------------------------------

void load_status(Player gp)
{
	// 判定セッティング //

	int i, fp, num = 0;

	int input[TX_MAX];
	char inputc[TX_MAX];	// inputとinputcに文字がはいる
	int flg = -1;	// 代入する条件を決める
	boolean iflg = false;	// 数字フラグ
	boolean nextRaw = false;

	////
	// 元はここで文字の関数宣言
	//tlist.push_back("Common");
	tlist.push_back("gsize");
	tlist.push_back("xsize");
	tlist.push_back("ysize");
	tlist.push_back("life");
	
	tlist.push_back("walk.f");
	tlist.push_back("walk.b");
	tlist.push_back("run.f");
	tlist.push_back("run.b");
	tlist.push_back("jump.y");
	tlist.push_back("jump.a");
	tlist.push_back("jump.f");
	tlist.push_back("jump.b");
	tlist.push_back("yaccel");
	tlist.push_back("airjump");
	tlist.push_back("airdash");
	tlist.push_back("combolate");
	tlist.push_back("clmax");
	tlist.push_back("clmin");
	//
	////

	// 名前準備
	
	string fname;
	fname = fn1 + gp.nameC + fn2;
	//fname = "char/cornel/st.txt";

	// ファイルを開く //
	fp = FileRead_open(fname.c_str());//ファイル読み込み
	if (fp == NULL){			// エラーが起こったら
		fp = FileRead_open("char/def_data.txt");	// 共通設定を開く
		if (fp == NULL){	// これもダメなら
			printfDx("error def_data\n");// 文字を表示
			return;
		}
	}

	//string copy;	// stringとして使う用
	while (1){
		//=========================
		// 基本ループ
		// このループ終了後に下へ
		//=========================
		for (i = 0; i < TX_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1文字取得する
			
			// 改行で次の行に進む
			if (input[i] == '\n'){
				inputc[i] = '\0';
				i = -1;
				break;
			}

			// カンマで改行(手前の-1はbreakで大丈夫)、次がEOFだとフリーズ
			if (inputc[i] == ';'){					//スラッシュがあれば
				inputc[i] = '\0';	// そこまでは文字になる
				while (FileRead_getc(fp) != '\n');	//改行までループ
				i = -1;	// 次のループに
				break;	// 他の作業はないので脱出
				//continue;
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
			else if (inputc[0] == '-')iflg = true;
		}

		// 全て数字だったら
		if (iflg){
			// 代入する
			switch (flg){
				case 0:	// gsize
					gp.GSize = atof(inputc);
					break;
				case 1:	// xsize
					gp.xSize = atoi(inputc);
					break;
				case 2:	// ysize
					gp.ySize = atoi(inputc);
					break;
				case 3:	// life
					gp.C.lifeMax = atoi(inputc);
					break;
				case 4:	// walk.f
					gp.C.walkF = atof(inputc);
					break;
				case 5:	// walk.b
					gp.C.walkB = atof(inputc);
					break;
				case 6:	// run.f
					gp.C.runF[num] = atof(inputc);
					break;
				case 7:	// run.b
					gp.C.runB[num] = atof(inputc);
					break;
				case 8:	// jump.y
					gp.C.jumpY = atof(inputc);
					break;
				case 9:	// jump.a
					gp.C.jumpA = atof(inputc);
					break;
				case 10:	// jump.f
					gp.C.jumpF[num] = atof(inputc);
					break;
				case 11:	// jump.b
					gp.C.jumpB = atof(inputc);
					break;
				case 12:	// yaccel
					gp.C.yAccel = atof(inputc);
					break;
				case 13:	// airjump
					gp.C.airJump = atoi(inputc);
					break;
				case 14:	// airdash
					gp.C.airDash = atoi(inputc);
					break;
				case 15:	// combolate
					gp.A.comboLate = atof(inputc);
					break;
				case 16:	// clmax
					gp.A.cl_max = atof(inputc);
					break;
				case 17:	// clmin
					gp.A.cl_min = atof(inputc);
					break;
				default: break;
			}
			num++;	// numを進める
		}
		
		// 数字ではない
		if (!iflg){
			// 文字があってるか判断
			for (int h = 0; h < (int)tlist.size(); h++){
				if (strcmp(inputc, tlist[h].c_str()) == 0){
					flg = h;
					num = 0;
					break;
				}
			}
		}	// if終了
		
		//flgは最初の行で初期化
		if (i == -1)flg = -1; 

	}	// while ループ終了


	// ファイルを閉じる
EXFILE:
	FileRead_close(fp);

	// 値を返却
	GetP1_Player(gp);
}
