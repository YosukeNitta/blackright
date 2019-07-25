#include "pch.h"
#include <stdio.h>	// ファイル操作に必要なヘッダファイル
#include <string.h>	// 文字列操作に必要なヘッダファイル
#include <ctype.h>	// isdigitに必要
#include <vector>	// stringに使う



// 配列の数 = character_Max
// 中身の名前 = EIN = ○○;
static std::vector<Name_t>names;
static int char_max;
static Def_c def;

void GetDef()
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
			if ((inputc[0] != '\0') && (inputc[0] != '\n') && (inputc[0] != NULL))
				iflg = true;
		}

		// 名前だったら
		if (iflg){
			anum++;	// 最初に0になる
			names.push_back(Name_t(anum+1));	// newはいらない
			strcpy_s(names[anum].nameC, inputc);
		}

		if (input[i] == EOF){//ファイルの終わりなら
			goto EXFILE;//終了
		}
	}

	// ファイルを閉じる
EXFILE:
	FileRead_close(fp);

	// char_max = anum + 1
	char_max = (int)names.size();

	def.charMax = (int)names.size();
	def.names = names;
}

int Character_Max()
{
	return char_max;
}

Name_t Character_Name(int num)
{
	return names[num];
}