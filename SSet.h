#ifndef DEF_SSET_H	//一度定義されていたら、定義しない
#define DEF_SSET_H

// ゲームの数値を記録しておく
typedef struct {

	/***********************
	* システムの変数
	***********************/
	// スクロールの位置
	double ScroolX, ScroolY;
	// 終了フラグ
	int EndFlg;
	// 時間停止（演出・暗転等）
	int StopTime;
	// 暗転時間（この時間中は画面が黒くなる、state関数でタイムを引く）
	int Anten;
	// ゲージ表示停止
	int noGauge;
	// ゲームの流れ　0:イントロ、1:対戦、2:ラウンド取得、3:ゲームセット
	int roundState;
	int roundTime;	// 設定時間
	boolean noScroolX, noScroolY;	// スクロールを停止する
	// 衝撃(カメラ)
	double quakeY;
	int quakeTime;

	// 現在のステージ
	int NowStage;
	// AIオンオフ
	boolean AI;
	/************
	/ トレモ設定
	/***********/
	boolean TraningFlg;
	//トレモメニュー
	// 0.体力　1.パワー  2.状態（立屈空）
	// 3.受け身  4.ガード  5.設定確認
	// 6.判定  7.2P操作
	int TSwitch[20];
	int TMAX[20];			// 横項目の数

	// int Select_MAX;		// 選択項目の数(pauseに個別で用意)

	int T_Life;			// 体力
	int T_Power;		// パワー
	int T_GGauge;		// ガードゲージ
	int T_AddGauge;		// アド
	int T_Condition;	// 状態（立屈空）
	int T_Recovery;		// 受け身
	int T_Guard;		// ガード	(しない、する、一回)
	int T_Counter;		// カウンターヒット	0.しない 1.する
	int T_ThrowReject;	// 投げ抜け　0.しない 1.する
	int T_Box;			// 判定表示　0.しない 1.喰らい 2.攻撃 3.両方

}System_t;

// プレイヤー1・2の定義、ここでstaticを使い被らないようにする
static System_t S;

#endif