#include "pch.h"
#include <string>	// stringに使う
#include <vector>

using namespace std;

#pragma warning(disable:4996)	// エラー表記を消す？


// 代入で使う
static Pict pic[PICT_MAX][IMAGE2_MAX];

// これが関数で使われる
static Pict pic2[2][PICT_MAX][IMAGE2_MAX];
//static vector<vector<vector<Pict>>> pic2;	// 渡す時に使用
//static int P_Image2[2][PICT_MAX][IMAGE2_MAX];	// これに全てのキャラの実際の画像
												// 2キャラ分のみ入れる

static vector<int> r,g,b;	// これに全てのキャラの判定

// 全てのpictを保存
static vector<vector<vector<Pict>>> pic3;	// これに全てのキャラの画像設定

class Image3{
public:
	int image[PICT_MAX][IMAGE2_MAX];
	Image3(){
		for (int i = 0; i < PICT_MAX; i++){
			for (int j = 0; j < IMAGE2_MAX; j++){
				image[i][j] = 0;
			}
		}
	}
};

//static int image3[C_M][PICT_MAX][IMAGE2_MAX];	// 同じく
//static vector<vector<vector<int>>> vimage3;	// これに全てのキャラの判定
// ここで作った画像を代入する
// picには入ってない
static vector<Image3>im3;

// 名前（airから読み込み）s
static vector<Name_t>N;

static void GetPort();	// ポートレイト取得

// 画像準備
int GraphSet(int s1, int s2, int side)
{
	//return P_Image2[side-1][s1][s2];
	return pic2[side - 1][s1][s2].image;
}

// 画像・パレットを準備(読み込み後に使う)
int PalletSet()
{
	/****
	パレット取得テスト
	****/
	int color[COLOR_MAX], i, j;
	int setPal;

	// [パレットセット開始]
	//char fname[TX_MAX];
	// 名前読み込み
	string fname;
	string fn1 = "char/";
	string fn2 = "/pal/c";
	string fn3 = ".bmp";
	
	//
	// 名前読み込み
	for (int n = 0; n < COLOR_MAX; n++)
	{
		char ch[2];
		sprintf(ch, "%d", n+1);	// 文字に変換
		fname = fn1 + P1.nameC + fn2 + ch[0] + fn3;

		// setPal に選んだ番号をセット
		color[n] = LoadSoftImage(fname.c_str());
	}

	setPal = P1.Palno;

	// RGBにパレットの色を記憶
	for (i = 0; i < 256; i++)
	{
		// パレットの色を取得する 1～16, 17～
		GetPaletteSoftImage(color[setPal], i, &r[i], &g[i], &b[i], 0);
	}
	
	//====================
	// ここで画像読み込み
	//====================
	//load2_pict();
	
	// 透過色を変更
	SetTransColor(r[0], g[0], b[0]);
	
	// ポトレ取得
	GetPort();

	// 透過色を戻す
	SetTransColor(r[0], g[0], b[0]);

	// pic2に代入
	for (int n = 0; n < PICT_MAX; n++){
		for (int n2 = 0; n2 < IMAGE2_MAX; n2++){
			// image2を初期化
			//P_Image2[P1.PSide - 1][n][n2] = 0;
			
			// pic2にpic3設定をセット
			pic2[P1.PSide - 1][n][n2] = pic3[P1.Name - 1][n][n2];

			// 使う画像のみ読み込み(これないとクッソ重い)
			if (im3[P1.Name - 1].image[n][n2] != 0){
				// image3、color画像の色を取得
				for (int c = 0; c < 256; c++){
					SetPaletteSoftImage(im3[P1.Name - 1].image[n][n2], c, r[c], g[c], b[c], 0);
				}
				// image2に色を変えたimage3を入れる
				//P_Image2[P1.PSide - 1][n][n2] = CreateGraphFromSoftImage(im3[P1.Name - 1].image[n][n2]);
				// image2に色を変えたimage3を入れる
				pic2[P1.PSide - 1][n][n2].image = CreateGraphFromSoftImage(im3[P1.Name - 1].image[n][n2]);
			}
		}
	}
	
	// 透過色を元に戻す
	SetTransColor(0, 0, 0);

	// カラー画像全て削除
	for (j = 0; j < COLOR_MAX; j++)
	{
		DeleteSoftImage(color[j]);
	}

	return 0;
}

// csvで読み込み
void load_pict(int np, int side)
{

	FILE *fp;

	char buf[100];
	int c;
	int col = 1;
	int row = 0;
	int sizeX = 0, sizeY = 0;
	int x = 0, y = 0;	// 名前の通り
	int num1 = 0, num2 = 0;

	// イメージ追加

	//////

	/// 初期化してるからいらない？
	/*
	for (int i = 0; i < IMAGE_MAX; i++){
		for (int j = 0; j < IMAGE2_MAX; j++){
			pic[i][j].image = 0;
			pic[i][j].posX = 0;
			pic[i][j].posY = 0;
			pic[i][j].x = 0;
			pic[i][j].y = 0;
		}
	}
	*/
	
	// 名前読み込み
	string fname;
	string fn1 = "char/";
	string fn2 = "/sff/sffdata.csv";
	fname = fn1 + N[np].nameC + fn2;

	std::string fsff = "/sff/";
	// 事前準備
	std::string str;
	//

	memset(buf, 0, sizeof(buf));

	fp = fopen(fname.c_str(), "r");
	if (fp == NULL){			// エラーが起こったら
		printfDx("error sffdata\n");// 文字を表示
		return;
	}
	// 透明色決定
	SetTransColor(r[0], g[0], b[0]);

	// 非同期読み込み設定に変更
	SetUseASyncLoadFlag(TRUE);

	//ヘッダ読み飛ばし
	while (fgetc(fp) != '\n');

	while (1){

		while (1){

			c = fgetc(fp);

			//末尾ならループを抜ける。
			if (c == EOF)
				goto out;

			//カンマか改行でなければ、文字としてつなげる
			if (c != '\n' && c != ',')
					strcat(buf, (const char*)&c);
			//カンマか改行ならループ抜ける。
			else
				break;
		}

		//ここに来たということは、1セル分の文字列が出来上がったということ
		switch (col){
			// 文字のみ取得
		case 1:
			str = fn1 + N[np].nameC + fsff + buf;
			break;

			//2列目は弾種類。以降省略。
		case 2: num1 = atoi(buf); break;
		case 3: num2 = atoi(buf); break;
		case 4: pic[num1][num2].posX = atoi(buf); break;
		case 5: pic[num1][num2].posY = atoi(buf);
	
			// 文字の画像を代入
			//P_Image3[np][num1][num2] = LoadSoftImage(str.c_str());
			// おかしな数値が入ってたら
			if (num1 >= PICT_MAX || num1 < 0)break;
			else if (num2 >= IMAGE2_MAX || num2 < 0)break;
			//vimage3[np][num1][num2] = LoadSoftImage(str.c_str());

			// 文字列で画像を読み込む(これを色変えしながら使う)
			im3[np].image[num1][num2] = LoadSoftImage(str.c_str());
			
			GetSoftImageSize(im3[np].image[num1][num2], &sizeX, &sizeY);	// 画像サイズ取得
			
			// サイズ・sffno決定
			pic[num1][num2].x = sizeX;
			pic[num1][num2].y = sizeY;
			break;
		case 6: break;
		}

		//バッファを初期化
		memset(buf, 0, sizeof(buf));
		//列数を足す
		++col;

		//もし読み込んだ文字が改行 
		//だったら列数を初期化して行数を増やす
		if (c == '\n' || col >= 6){
			col = 1;
			row++;
		}
	}
out:
	// 非同期読み込み設定に変更
	SetUseASyncLoadFlag(FALSE);

	// 透明色を元に戻す
	SetTransColor(0, 0, 0);

	// pic2に代入
	for (int n = 0; n < PICT_MAX; n++){
		for (int n2 = 0; n2 < IMAGE2_MAX; n2++){
			// 変更した
			pic2[side - 1][n][n2] = pic[n][n2];
			pic3[np][n][n2] = pic[n][n2];
			//vimage3[np][n][n2] = P_Image[n][n2];
		}
	}

	// セレクト用
	int r, g, b;
	GetPaletteSoftImage(im3[np].image[0][0], 0, &r, &g, &b, 0);

	// 透明色
	SetTransColor(r, g, b);
	for (int n2 = 0; n2 < IMAGE2_MAX; n2++){
		// 変更した
		//pic[0][n2].image = CreateGraphFromSoftImage(im3[np].image[0][n2]);
		pic[0][n2].image = im3[np].image[0][n2];
	}
	SetTransColor(0, 0, 0);

	GetPI_Select(pic[0], np);	// セレクトに送る
	
	// ファイル閉じたほうがよくない？
	fclose(fp);
}

void GetPort()
{
	/****
	パレット取得テスト
	****/
	int PR, PR2;

	// [パレットセット開始]
	string fname;
	string fn1 = "char/";
	string fn2 = "/pal/port.bmp";
	fname = fn1 + P1.nameC + fn2;
	{
		// setPal に選んだ番号をセット
		PR = LoadSoftImage(fname.c_str());
	}

	//setPal = P1.Palno;


	//====================
	// ここで画像読み込み
	//====================
	
		// 色を取得
		for (int c = 0; c < 256; c++){
			SetPaletteSoftImage(PR, c, r[c], g[c], b[c], 0);
		}
		// PRに準備したデータを読み込む
		PR2 = CreateGraphFromSoftImage(PR);

		// 色変えた画像を読み込み
		MainDraw_GetPort(PR2, P1.PSide);
		
		
		// ネームプレート
		{
			fn2 = "/pal/name.bmp";
			fname = fn1 + P1.nameC + fn2;
			// setPal に選んだ番号をセット
			PR = LoadSoftImage(fname.c_str());
		}

		// 色を取得
		int r1 = 0, g1 = 0, b1 = 0;
		GetPaletteSoftImage(PR, 0, &r1, &g1, &b1, 0);
		
		SetTransColor(r1, g1, b1);
		PR2 = CreateGraphFromSoftImage(PR);
		//SetTransColor(0, 0, 0);

		// 名前取得
		MainDraw_GetName(PR2, P1.PSide);
}

void GetPict(int side)
{
	// 喰らいに送る
	GetPI_GHitbox(pic2[side - 1], side);
}

void GetP1_Pict(Player GP1)
{
	P1 = GP1;
}

// Nameを取得
//void GetN_Pict(Name_t GN[])
void GetN_Pict(std::vector<Name_t>GN)
{
	for (int i = 0; i < Character_Max(); i++){
		N.push_back(GN[i]);
		//N[i] = GN[i];
	}
}

void Load1_Pict()
{
	// 今のうちに他も作る

	//P_IMage3
	//pic3準備
	for (int i = 0; i < Character_Max(); i++) {
		//im3
		im3.push_back(Image3());

		pic3.push_back(vector<vector<Pict>>());
		pic3[i].resize(PICT_MAX);	// 多い場合はあらかじめ容量を確保
		for (int n1 = 0; n1 < PICT_MAX; n1++) {
			pic3[i][n1].resize(IMAGE2_MAX);
		}
	}

	// rgb
	r.resize(256);
	g.resize(256);
	b.resize(256);
	/*
	for (int i = 0; i < 256; i++){
		r.push_back(0);
		g.push_back(0);
		b.push_back(0);
	}
	*/
}