#pragma once

#include "Define.h"
#include "DxLib.h"

class Pict {

public:
	int x, y;	// 画像サイズ
	int posX, posY;	// ずらす座標
	int image;

	Pict::Pict()
	{
		x = 0;
		y = 0;
		posX = 0;
		posY = 0;
		image = 0;
	}
};

class Box {

public:
	
	int time;	// 時間
	int num1, num2;	// 画像No
	int posX, posY;	// 座標
	int HV;	// 1.左右 2.上下 3.どっちも

	int hw[HIT_MAX], hh[HIT_MAX],
		hx[HIT_MAX], hy[HIT_MAX];
	boolean h[HIT_MAX];
	int aw[ATT_MAX], ah[ATT_MAX],
		ax[ATT_MAX], ay[ATT_MAX];
	boolean a[HIT_MAX];

};

class Air {

public:
	
	int stateno;// ステート番号
	int ANum;	// 判定の数(最大30)
	Box B[STATE2_MAX];// 実際の判定

	int defhw[HIT_MAX], defhh[HIT_MAX],
		defhx[HIT_MAX], defhy[HIT_MAX];	// デフォルトボックス
	boolean defh[HIT_MAX];

	boolean def;

	int aTime;	// ステートの合計時間

	void setup(){
		// AIR変数準備 //
			stateno = -1;
			ANum = -1;
			def = false;
			aTime = 0;
			for (int h = 0; h < HIT_MAX; h++){
				defhw[h] = 0;
				defhh[h] = 0;
				defhx[h] = 0;
				defhy[h] = 0;
				defh[h] = false;
			}
			for (int n2 = 0; n2 < STATE2_MAX; n2++){
				B[n2].posX = 0;
				B[n2].posY = 0;
				B[n2].num1 = 0;
				B[n2].num2 = 0;
				B[n2].time = -2;
				B[n2].HV = 0;	// これないとおかしくなる
				for (int n3 = 0; n3 < HIT_MAX; n3++){
					B[n2].h[n3] = false;
					B[n2].a[n3] = false;
					defh[n3] = false;

					B[n2].hw[n3] = 0;
					B[n2].hh[n3] = 0;
					B[n2].hx[n3] = 0;
					B[n2].hy[n3] = 0;

					B[n2].aw[n3] = 0;
					B[n2].ah[n3] = 0;
					B[n2].ax[n3] = 0;
					B[n2].ay[n3] = 0;
				}
			}
	}

	Air(){

	}
	// できればコンストで初期化
	/*
	Air::Air(){
		// AIR変数準備 //
		stateno = -1;
		ANum = -1;
		def = false;
		aTime = 0;
		for (int h = 0; h < HIT_MAX; h++){
			defhw[h] = 0;
			defhh[h] = 0;
			defhx[h] = 0;
			defhy[h] = 0;
			defh[h] = false;
		}
		for (int n2 = 0; n2 < STATE2_MAX; n2++){
			B[n2].posX = 0;
			B[n2].posY = 0;
			B[n2].num1 = 0;
			B[n2].num2 = 0;
			B[n2].time = -2;
			B[n2].HV = 0;	// これないとおかしくなる
			for (int n3 = 0; n3 < HIT_MAX; n3++){
				B[n2].h[n3] = false;
				B[n2].a[n3] = false;
				defh[n3] = false;

				B[n2].hw[n3] = 0;
				B[n2].hh[n3] = 0;
				B[n2].hx[n3] = 0;
				B[n2].hy[n3] = 0;

				B[n2].aw[n3] = 0;
				B[n2].ah[n3] = 0;
				B[n2].ax[n3] = 0;
				B[n2].ay[n3] = 0;
			}
		}
	}
	*/
};




//基本データの構造体
typedef struct {

public:
	double walkF, walkB;			// 
	double runF[2], runB[2];		// 
	double jumpY, jumpF[2], jumpB, jumpA;	// Yのジャンプ力、前後のX距離、空中ジャンプの高さ
	double yAccel;				// 
	int lifeMax;	// 体力の最大値
	
	int airJump;	// 空中ジャンプ回数
	int airDash;	// 空中ダッシュ回数

}Const_t;


//被ダメデータの構造体
typedef struct {

public:
	double yaccel;
	int bound;		// バウンドレベル、0：なし、１：小バウンド、２：大バウンド
	int boundCount;	// バウンド回数
	double bound_xvel;
	double bound_yvel;
	double wall_xvel;
	double wall_yvel;
	int wall;		// バウンドレベル、0：なし、１：小バウンド、２：大バウンド
	boolean counter;	// カウンター判定、１：あり
	int armor;		// アーマー回数
	//X,Yの喰らい速度
	//double hitVelX, hitVelY;
	int selfTime;	// 自動めくりガードの時間
	int nokezori;	// のけぞり時間
	boolean fatal;	// フェイタルカウンター
	double addVelX;	// アドガでの距離

}Damage_t;


//攻撃データの構造体
typedef struct {

public:
	int damage;	// 攻撃力、ちなみにshortの最大は3万くらい
	int hosyo;	// 最低ダメージ保障

	int A_GuardTime;		// ガード時間

	double comboLate;	// コンボレート
	double cl_max;	// 最大レート
	double cl_min;	// 最低レート

	boolean forceKurai; // 強制立ち・屈喰らい
	double hosei_K; // 強制補正
	boolean throwTurn; // 投げ方向変更(0,なし、1,後ろ)

	// 相打ちレベル

	// ヒットカウント
	int guardCount;	// ガード回数

	// バウンドのレベル
	// ０：なし、１：小、２：大
	int boundLevel;		 
	int wallLevel;

	double bound_xvel;
	double bound_yvel;
	double wall_xvel;
	double wall_yvel;

	boolean gaugeHosei;	// ゲージ増加補正、ゲージ技で使う
	double yaccel;	// 相手の加速度

	//X,Yのヒットバック
	double hbVelX;	// 専用の距離
	double hbVelY;	// 専用の距離
	double guard_gx;
	double guard_ax;

	// ヒットエフェクト
	int hitEff;
	double heSizeX;
	double heSizeY;

	// 相手に無敵付加
	boolean addMuteki;
	int amTime;	// 無敵付加時間

	// 地震エフェクト
	int quakeTime;
	double quakeX, quakeY;

	int att_Level;	// 攻撃レベル　通常投げ.-1 コマ投げ.0 弱.1 基本.2 強.3
	boolean kill;	// キル判定、0でとどめを刺せなくできる

	int ncTime;	// 通常技キャン

	void setup(){
		A_GuardTime = 0;

		comboLate = 0;
		cl_max = 0;
		cl_min = 0;

		forceKurai = 0;
		hosei_K = 0;
		throwTurn = 0;

		// 相打ちレベル


		// バウンドのレベル
		// ０：なし、１：小、２：大、３：強制(必ず大バウンド)
		boundLevel = 0;
		wallLevel = 0;

		bound_xvel = 0;
		bound_yvel = 0;

		wall_xvel = 0;
		wall_yvel = 0;

		gaugeHosei = 0;
		yaccel = 0;

		//X,Yのヒットバック
		hbVelX = 0;
		guard_gx = 0;
		guard_ax = 0;

		// ヒットエフェクト
		hitEff = 0;
		heSizeX = 0;
		heSizeY = 0;

		// 相手に無敵付加
		addMuteki = false;
		amTime = 0;

		att_Level = 0;

		kill = true;

		ncTime = 0;
	}
}Attack_t;


/// <summary>
/// キャラクターの基本となる情報
/// </summary>
class Character{
public:
	// 描画のサイズ
	double GSize;	
	//X,Yの座標
	double XPos, YPos;
	// ステートの経過時間
	int time;
	//向き
	boolean muki;
	// キャラ画像反転（Mukiとは別）
	boolean turnG;

	// sff設定
	int sPosX, sPosY;
	
	// air設定
	int aPosX, aPosY;	// 画像位置
	int aNum1, aNum2;	// 画像
	int aHV;	// 反転等

	// セットされているキャラ画像
	int Image;

	//[ 攻撃関連 ]//
	//攻撃判定
	int XAtt[ATT_MAX], YAtt[ATT_MAX];
	int WAtt[ATT_MAX], HAtt[ATT_MAX];
	// 喰らい判定
	int XHit[HIT_MAX], YHit[HIT_MAX];
	int WHit[HIT_MAX], HHit[HIT_MAX];

	// 攻撃設定
	Attack_t A;

	// 初期化？
	void Reset(){
		for (int i = 0; i < ATT_MAX; i++){
			WAtt[i] = 0;
			HAtt[i] = 0;
			XAtt[i] = -60;
			YAtt[i] = -60;
		}
		for (int i = 0; i < HIT_MAX; i++){
			WHit[i] = 0;
			HHit[i] = 0;
			XHit[i] = -70;
			YHit[i] = -70;
		}
	}
};

class Helper:public Character {

public:	
	//***********************
	// ヘルパーの変数
	//***********************
	boolean var;	// ヘルパーのオン・オフ
	int HAnimTime;	// アニメーションの時間
	int stateno;	// ステート
	int state2;		// ステート2（画像用）
	int oldState;	// 前のステート

	//int HImage;	// 現在のキャラ画像
	//double GSize;	// 描画のサイズ

	// 位置
	//double XPos, YPos;

	// 画像のサイズ
	int HGraphW, HGraphH;
	// 半透明
	boolean Alpha;
	// 速度
	double HXVel, HYVel;


	//[ 攻撃関連 ]//

	//攻撃設定
	//int damage;	// 攻撃力
	//int hosyo;	// 保障ダメージ
	int HMoveHit;	// 攻撃判定
	int HHitSE;			// ヒットサウンド
	int HGuardSE;		// ガード音
	int HHitAnim;		// ヒット時のアニメ


	// ガード条件  
	// 0:ガード不能 1:全て 2:立ち/空中 3:しゃがみ/空中 4:空ガ不可上段
	// 12:立ガ 13: 屈ガ 14:空ガ
	int HGuardF;
	// ガードが起こる距離
	int HGuardDist;
	// 攻撃属性　1.打撃 2.投げ 3.飛び道具
	int attF = 1;

	int HGetPow; // ゲージ回収
	int HGivePow;	//与えるゲージ
	double H_GX, H_GY;	// ヒットバック(地上)
	double H_AX, H_AY;	// ヒットバック(空中)

	int HHitStop;		// ヒットストップ
	int HSSelf;			// 自分のヒットストップ
	int HG_HitTime;		// 地上喰らいの時間
	int HA_HitTime;		// 空中喰らいの時間
	int HG_GuardTime;		// ガード硬直

	int stopTime;		// 自分のヒットストップ

	//受け身関連
	int fallF;	// 0 で着地 1 でダウン

	// ヒットエフェクト
	int hitEff;
	double heSizeX;
	double heSizeY;

	//int HA_GuardTime;		// ガードストップ時間

	void Reset(){
		HMoveHit = 0;
	}
};

// 1Pと2Pのヘルパー
static Helper H1[HM], H2[HM];

// キャラクターのクラス
class Player:public Character{

public:
	int LifeC;
	// キャラの1P.2P
	int PSide;
	//***********************
	// キャラクターの変数
	//***********************

	//[ 座標関連 ]//
	//基本判定の大きさ
	int xSize, ySize;
	// 描画のサイズ
	//double GSize;	// 小数点で縮小
	//上下左右の判定の座標
	int X_UL, Y_UL;

	// XPos YPos
	//X,Yの速度
	double XVel, YVel;
	

	//[ ステータス関連 ]//
	//キャラクター名
	int Name;
	char nameC[NAME_MAX];

	//体力
	int Life;
	//パワーゲージ
	int Power;
	// アドゲージ
	int aGauge;
	int GRecovery;
	int addG;
	// 選択したカラー
	int color;

	// 1F前の向き
	boolean oldMuki;
	//操作の可否
	boolean ctrl;
	// キャラ固有の設定
	int Var[VAR_MAX];

	//0,立ち 1,しゃがみ 2,空中
	int SFlg;
	// 重力を無視する
	boolean ignoreG;
	//ジャンプ条件
	boolean JumpFlg;
	boolean	hyJump;	// ハイジャンプ
	//ジャンプの方向判断
	int JumpVel;
	//空中ジャンプ回数
	int AirJump;
	int airDash;
	//ダッシュの可否
	boolean DashFlg;
	// 空中ダッシュの時間
	int airDashTime;
	// 接触判定
	boolean touchF;
	// アドガ
	boolean advancing;
	//現在のステート
	int stateno;
	// タイムで決定された番号
	int State2;	

	//前のステート
	int OldState;
	int oldState2;

	//もう一度ステートを変える
	boolean More;
	// ステート固定する
	boolean Lock;
	//キー入力
	int Key;
	// 振り向き
	boolean Turn;

	// ボタンの要素
	int button[PAD_MAX];

	int keyPos;		// キー位置チェック、　1～9 になる
	int keyAtt[9 + 1];		// キー入力の属性
	int key[9 + 1];		// キー入力時間

	//先行入力
	int Senkou[BUTTON_MAX + 1];
	int K_Senkou[9 + 1];
	// リバサ入力
	int rbutton[BUTTON_MAX + 1];
	

	//攻撃設定
	int GetPow; // ゲージ回収
	int GivePow;	//与えるゲージ

	double GX, GY;	// ヒットバック(地上)
	double AX, AY;	// ヒットバック(空中)

	int HitStop;		// ヒットストップ時間
	int HSSelf;			// 自分のヒットストップ
	int StopTime;		// 被ヒットストップ

	int G_HitTime;		// ヒットの喰らい状態の時間
	int A_HitTime;		// 空中の喰らい状態の時間
	int G_GuardTime;		// ガードの喰らい状態の時間
	int HitAnim;		// 当たった時のステート
	int HitSE;			// ヒットサウンド
	int scTime;			// スパキャン時間

	// ガード条件  
	// 0:ガード不能 1:全て 2:立ち/空中 3:しゃがみ/空中 4:空ガ不可上段
	// 12:立ガ 13: 屈ガ 14:空ガ
	int GuardF;
	// ガードができる距離
	int GuardDist;

	int attF;	// 攻撃の種類　1.打撃 2.投げ 3.飛び道具

	// 投げ抜け
	boolean rejectF;	// 投げ抜け可能フラグ
	boolean throwTurn;	// 投げ抜け前の向き
	boolean throwSide;	// 投げ前の方向
	

	//喰らいフラグ
	int HitCount;	// ヒット数カウント
	int HFlg;			// ヒット判定
	int CFlg;			// キャンセル判定
	int MoveHit;		// ステートのヒット確認
	
	int Muteki;		// 無敵判定
	int mutekiF;	// 無敵の種類　0.完全無敵 1.打撃 2.投げ 3.飛び道具

	//受け身関連
	int fall;	// 0 で着地 1 でダウン 2.地上受け身可能
	// 0 で着地 1 でダウン
	int fallF;
	int recover;	// 受け身の可否
	int recTime;	// 受け身を取れるようになるまでの時間

	// コマンド成否
	int cmd[20];

	// ヘルパー用意（これ解決できればよし）
	//Attack_t A;
	Damage_t D;
	Const_t C;


	//***********************
	// キャラクターの画像用変数
	//***********************
	
	// アニメ時間
	int animElem[STATE_MAX];

	// パレット
	int Palno;
	// 画像のサイズ
	int GraphW, GraphH;
	// アニメーションのフレーム
	int AnimTime;
	
	// ステートを切り替えた時、時間をリセットしない（デフォはオフ）
	int NoChangeTime;
	// 画像の優先度
	int SprPriority;
	// 色変化
	int colorC[3];	// 0,r 1,g 2,b
	int colorCTime;		// 色が変わる！
	// 残像オンオフ
	boolean mirage;

	// コンストラクタ
	/*
	~Player()          // デストラクタ
	{

	}
	*/
};

static Player P1, P2;
