#include "pch.h"

using namespace std;

#define	BGM_MAX		(20)

/*
class Stage {
public:
	int front;
	int center;
	int back;

	Stage() {
		front = 0;
		center = 0;
		back = 0;
	}

	~Stage() {

	}

	void getFront(string str) {
		front = LoadGraph(str.c_str());
	}
	void getCenter(string str) {
		center = LoadGraph(str.c_str());
	}
	void getBack(string str) {
		back = LoadGraph(str.c_str());
	}
};
*/

static int maxStage;	// ステージ最大数

// ステージ
//static vector<Stage> stages;
static vector<vector<int>> stages;

int getMaxStage() {
	return maxStage;
}

int getStageGraph(int stageNum, int pictNum) {
	/*
	// ステージ最大値以内なら
	if (stageNum  <= maxStage) {
		if (pictNum == 0)return stages[stageNum].front;
		if (pictNum == 1)return stages[stageNum].center;
		if (pictNum == 2)return stages[stageNum].back;
	}
	*/
	return 0;
}

int drawStageGraph(int stageNum, int pictNum,
	float x, float y, float size) {
		// ステ背景
	DrawRotaGraph(x, y,
		size, 0, stages[stageNum][pictNum], true);

	return 0;
}

// ステージ全体を表示
int drawStageGraphAll(int stageNum, float x, float y, float size) {
	// ステ背景
	DrawRotaGraph(x, y,
		size, 0, stages[stageNum][2], false);
	DrawRotaGraph(x, y + (82 * size),
		size * 0.80, 0, stages[stageNum][1], true);
	DrawRotaGraph(x, y + (76 * size),
		size * 0.625, 0, stages[stageNum][0], true);
	return 0;
}

void StageLoad()
{
	// 判定セッティング //

	int i, fp;
	int anum;	// Nameの場所
	int num;	// セット番号
	anum = -1;

	char fname[14] = { "back/list.txt" };
	
	// 名前読み込み
	string setName;
	string fn1 = "back/";

	// ステージの0個目を追加
	//stages.push_back(Stage());
	/*
	for (int max = 0; max < 10; max++) {
		stages.push_back(vector<int>());
		// 3つ用意する
		for (int l = 0; l < 3; l++) {
			stages[max].push_back(0);
		}
	}
	*/
	
	//==================
	// ランダムステージ
	//==================
	stages.push_back(vector<int>());
	// 3つ用意する
	for (int l = 0; l < 3; l++) {
		stages[0].push_back(0);
	}

	// ランダムステージ画像
	stages[0][0] = LoadGraph("back/random.png");
	stages[0][1] = LoadGraph("back/random2.png");
	stages[0][2] = LoadGraph("back/random3.png");
	maxStage = 0;	// ステージの数
	//
	//==================

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// inputとinputcに文字がはいる
	boolean iflg = false;	// 名前入力フラグ
	boolean next = false;
	boolean checkText = false;	// 一定数のテキスト読み込み

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
		checkText = false;

		// 文字列セットに移行していない
		if (!next)
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

		// 文字列セット
		if (next && (inputc[0] != '\0' && inputc[0] != NULL && inputc[0] != '\n')) {
			// ステージ追加
			stages.push_back(vector<int>());
			// 3つ用意する
			for (int l = 0; l < 3; l++) {
				stages[num].push_back(0);
			}
			maxStage++;	// ステージの数

			string fn2;
			fn2 = ".png";
			setName = fn1 + inputc + fn2;
			stages[num][0] = LoadGraph(setName.c_str());
			//stages[num].getFront(setName);

			fn2 = "_2.png";
			setName = fn1 + inputc + fn2;
			stages[num][1] = LoadGraph(setName.c_str());
			//stages[num].getCenter(setName);
			
			fn2 = "_3.png";
			setName = fn1 + inputc + fn2;
			stages[num][2] = LoadGraph(setName.c_str());
			//stages[num].getBack(setName);
			next = false;

			checkText = true;
			
		}

		// 名前だったら
		if (iflg && !checkText) {
			num = atoi(inputc);	// 番号をセット
			next = true;	// 次の準備
		}

		if (input[i] == EOF) {//ファイルの終わりなら
			goto EXFILE;//終了
		}
	}

	// ファイルを閉じる
EXFILE:
	FileRead_close(fp);
}