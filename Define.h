#include "DxLib.h"


#ifndef DEF_DEFINE_H	//一度定義されていたら、定義しない
#define DEF_DEFINE_H

// 変更するもの
static const int STAGE_MAX = 3;		// ステージの最大
static const int CHARACTERS_MAX = 6;

// むりやりキャラ導入
static const int EIN = 1;
static const int CORNEL = 2;
static const int BOUNCER = 3;
static const int HELIOS = 4;
static const int HYDE = 5;
//static const int ERIKU = 6;

//*************
//  定数
//*************

static enum GameScene
{
	NoScene,
	MenuScene,
	SelectScene,
	VersusScene,
	ConfigScene,
	TitleScene,
	ResultScene,
	NetworkScene,
	ReplayScene,
	ArcadeScene,
};
static GameScene gameScene;

// スクリーン数値
static const int SCREEN_W = 640;	//画面の横幅
static const int SCREEN_H = 480;	//画面の縦幅

// ステージ数値
static const int STAGE_WIDTH = 1280;	//ステージの横幅(1280)
static const int STAGE_HEIGHT = 720;	//ステージの縦幅(7]:20)
static const int GAMENHAJI = 35;	//画面端の位置
static const int KURAI_YBASE = 12;		//喰らい判定の +-0 の位置

static const int NAME_MAX = 18;	//名前文字数の最大

//
static const int POWER_MAX = 3000;	//パワーの最大値
static const int GUARD_MAX = 1000;	// ガードゲージ最大
static const int ADD_MAX = 100;	// アドゲージ最大

static const double GRAVITY = 0.35;  // キャラに掛かる重力加速度
static const int GROUND = STAGE_HEIGHT - 48;	//地面の位置(-45)
static const int D_GROUND = GROUND + 8;			//喰らい中の地面

// ステート、イメージ
static const int STATE_MAX = 3000;	// ステートのMAX
static const int STATE2_MAX = 50;	// ステートのMAX
static const int IMAGE_MAX = STATE_MAX;	// 保存画像数のMAX
static const int IMAGE2_MAX = STATE2_MAX;	// 保存横画像のMAX
static const int PICT_MAX = 10000;	// csvから読み込んだ画像のMAX(これでmugenから丸コピできる)

// ボタン設定
static const int BUTTON_MAX = 9;		// ボタン設定の最大
static const int KEY_MAX = 256;		// キー入力の最大
static const int PAD_MAX = 119;		// パッド入力の最大

// 判定
static const int HIT_MAX = 10;		// 喰らい判定.MAX
static const int ATT_MAX = 10;		// 攻撃判定.MAX
static const int TX_MAX = 64;	// 文字数の最大

//
static const int COLOR_MAX = 6;	// カラーの数
static const int T_MENU_MAX = 5;		// トレモ設定項目の数

// ヘルパー
static const int HELPER_MAX = 20;	// ヘルパー数.MAX
static const int HM = HELPER_MAX;	// 文字数削減用

// ゲーム内部数値 //
static const int LIFECOUNT_MAX = 2;	// ライフカウンタの最大値
static const int ROUNDTIME = 99;			// 試合時間
static const int TR_TIME = 7-6;			// 投げ抜け猶予(-6はヒットストップ)
static const int VAR_MAX = 50;		// キャラ変数の最大
static const int SC_TIME = 30;		// スパキャン猶予	
static const int NC_TIME = 10;		// 通常技キャン猶予	
static const int ThrowMuteki_Down = 7;		// 投げ無敵時間
static const int ThrowMuteki_Guard = 3;		// 投げ無敵時間
static const int ThrowMuteki_Hit = 5;		// 投げ無敵時間

// 白の色
static const int Cr = GetColor(255, 255, 255);
// オレンジの色
static const int Oran = GetColor(255, 200, 0);

#endif