//==================================================================================
//
//			数値計算の管理部
//
//========================================================================

//インクルード部--------------------------------------------
#include "pch.h"

//デファイン部----------------------------------------------


//ﾃﾞｰﾀ定義部------------------------------------------------

static Player PP2;
// 
static int Load_1;
static boolean Load_2;

// 前回のStopTime
static int mOldStop[2];
static int hOldStop[2][HM];

static int count;
static int fpSet;	// FirstPosSetで使用

static int oldState[2];
static int hh[2];

//
static int bo;	// ブラックアウト停止

// コマンド入力中止
static bool stopCmd;

//内部関数宣言部--------------------------------------------
// 無敵時間計測
void MutekiCount();
// 画像の準備
void GraphicSetting();
// 攻撃の内容
void AttackSetting();
// 判定
void HitBoxSetting();
// 接触と位置
void TouchPos();
// ステート内容
void SetParam();
// 
void SetParam2();
// 計測の値
void DrawSetting();
// 時間加算
void TimeCount();

//プログラム部----------------------------------------------

//-------------------------------------------------------------------------------
//	定義　int CharMove( void )
//
//	説明　バトルモードの1P, 2Pの値を設定する、このモードの要
//
//	引数　int (可変)  引数の値で次の画面が決まる
//
//	戻り値　int メニューを決める変数
//-------------------------------------------------------------------------------


/// <summary>
/// 対戦部分動作処理
/// </summary>
/// <returns>特になし</returns>
int CharMove(void)
{
	// 画像のロード
	if (Load_1 == 0)
	{
		// 画像を読み込む
		LoadPict();
		Load_1 = 1;
	}
	//-------------
	// トレモ設定
	//-------------
	{
		if (S.TraningFlg){
			GetP_Traning(P1, P2);
			GetS_Traning(S);
			GetH_Traning(H1, H2);
			SetTraning();	// OK!
		}
	}
	//-------------
	// キー入力
	//-------------
	{
		// 録画
		if (Replay_Mode(-1) == 2)Replay_ReserveKey();

		GetP_Key(P1, P2);
		GetS_Key(S);
		InputSend(); 
		// ラウンド取得時操作不能
		if (S.roundState == 2 && S.roundTime == 0){
			// ボタン受付中止
			for (int i = 0; i < BUTTON_MAX + 1; i++){
				P1.button[i] = 0;
				P2.button[i] = 0;
				P1.Senkou[i] = 0;
				P2.Senkou[i] = 0;
			}
		}
	}
	//-------------
	// コマンド入力確認
	//-------------
	//P1,2
	if (!stopCmd)
	{
		// 1P
		{
			// コマンド入力
			GetP_EinCmd(P1, P2);
			Ein_ComSet();	// 1P// OK!
		}
		// 2P
		{
			// コマンド入力
			GetP_EinCmd(P2, P1);
			Ein_ComSet();	// OK!
		}
	}
	
	// AI用
	if(S.AI)PP2 = P2;

	P1.Var[30] = 10, P2.Var[30] = 10;

#pragma region 処理
	// ヒットストップ中は停止
	if(S.StopTime == 0){
	/***
	* キャラクター処理
	***/
		//1P,2P処理開始
	{
		//-----
		// 特殊設定
		//-----
		{
			// 起き上がり
			if (P1.stateno == 0 && oldState[0] == 1055){
				P1.Muteki = 1000, P1.mutekiF = 0;
			}
			if (P2.stateno == 0 && oldState[1] == 1055){
				P2.Muteki = 1000, P2.mutekiF = 0;
			}
			oldState[0] = P1.stateno;
			oldState[1] = P2.stateno;
		}
		//----------------
		// 接触判定セット
		//----------------
		{
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			TouchHantei();// OK!
		}
		
		//------------
		//
		// しゃがみ(11) or 立ち(0)判定
		// 方向確認
		// 歩き＆ダッシュ
		// ジャンプ
		// ガード
		// 通常技（共通）
		//
		//------------

		// ラウンド中は移動のみ可能
		if (S.roundState == 1 || S.roundState == 2)
		{
			GetP_SubState(P1, P2);
			GetS_SubState(S);
			GetH_SubState(H1, H2);
			// 値は向こうで返してる
			EnterState();
		}
		// 対戦中なら
		if (S.roundState == 1){
		{
			GetP_SubState(P1, P2);
			GetS_SubState(S);
			GetH_SubState(H1, H2);
			// 値は向こうで返してる
			EnterAttack();
		}
		//------------------------
		//
		// 特殊・必殺技（キャラ別）
		//
		//------------------------
		{
			Player P[2];
			P[0] = P1, P[1] = P2;

			for (int i = 0; i < 2; i++){
				// 共通
				//GetPH_EinCmd(P[i]);
				if (i == 0){
					GetP_EinCmd(P[0], P[1]);
					GetH_EinCmd(H1, H2);
				}
				else if (i == 1){
					GetP_EinCmd(P[1], P[0]);
					GetH_EinCmd(H2, H1);
				}
				CharCmd();
			}

		}	// コマンド終了
		}	// roundState == 1終了
		if (S.AI)P2 = PP2;
		//----
		// AI
		//----
		{
			// AI //
			if ((S.AI) && (S.roundState == 1)){
				GetP_AI(P1, P2);
				GetS_AI(S);
				GetH_AI(H1, H2);
				AI_Move();
			}
		}
		//----------
		//
		// 接触開始
		// 座標設定
		//
		//----------
		{
			//TouchPos();// OK!

			// 基本座標セット、
			// この後、スクロールに合わせて修正
			//GetP_SubSystem(P1, P2);
			//GetS_SubSystem(S);
			//VelPosSet();

			//------------
			// 速度減少
			//------------
			{
				// 設定を渡す
				//GetP_SubSystem(P1, P2);
				//GetS_SubSystem(S);
				//VelXYAdd();	// 値を返す

			}

			// ここじゃない
			// 設定を渡す
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			// 当たってたら位置変更
			TouchCheck();
			
		}
		//--------------
		//
		// スクロール設定
		// 位置修正
		//
		//--------------
		{
			/*
			// 設定を渡す
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			// スクロール開始
			SetScrool();// OK!
			*/

			// 設定を渡す
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			// スクロール見ながら位置修正
			PosHosei();	// 値を返す
		}
		
		//***********
		// 喰らい・画像判定設定1
		//***********
		{
			HitBoxSetting();
		}
		//----------------
		//
		// 攻撃ヒット判定
		//
		// キャンセル判定
		// ヒット数カウント
		// 無敵時間変更
		//
		//------------------
		{

			AttackSetting();// OK!
			
		}
		//-------------------
		//
		// ステート設定適用
		//
		//-------------------
		{
			MutekiCount();

			// 特殊設定(投げ無敵)
			Player P[2];
			P[0] = P1, P[1] = P2;
			for (int i = 0; i < 2; i++){
				if (P[i].Muteki == 999)P[i].Muteki = ThrowMuteki_Down, P[i].mutekiF = 2;
			}
			P1 = P[0], P2 = P[1];

			SetParam();// OK!


			//============
			// 追加
			/*
			AttackSetting();
			SetParam2();
			MutekiCount();

			// ヒット後設定
			GetP_SubDamSet(P1, P2);
			DamSetting();// OK!
			*/
			// ヒット後設定
			GetP_SubDamSet(P1, P2);
			DamSetting();// OK!
			//============
		}
		//--
		// 追加
		//--
		{
			//----------
			//
			// 接触開始
			// 座標設定
			//
			//----------
			{
				//TouchPos();// OK!

				// 基本座標セット、
				// この後、スクロールに合わせて修正
				GetP_SubSystem(P1, P2);
				GetS_SubSystem(S);
				VelPosSet();

				//------------
				// 速度減少
				//------------
				{
					// 設定を渡す
					GetP_SubSystem(P1, P2);
					GetS_SubSystem(S);
					VelXYAdd();	// 値を返す

				}

				// 設定を渡す
				GetP_SubSystem(P1, P2);
				GetS_SubSystem(S);
				// 当たってたら位置変更
				TouchCheck();
			}
		}
		//------------
		// スクロール
		// 座標設定 2
		// ※重要
		//------------
		{

			// 設定を渡す
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			// スクロール開始
			SetScrool();// OK!

			// 設定を渡す
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			// スクロール見ながら位置修正
			PosHosei();	// 値を返す

		}
		//***********
		// 喰らい・画像判定設定 2
		//***********
		{
			HitBoxSetting();

			// ヒット後設定
			//GetP_SubDamSet(P1, P2);
			//DamSetting();// OK!
		}
		/**********
		/ 画像用の準備
		/*********/
		{
			GraphicSetting();	//ここのせい?
		}
		/**********
		/ 勝敗設定
		/*********/
		{
			/***********
			* ライフカウンター＆勝敗決定
			***********/
			GetPS_LifeCount(P1, P2, S);
			GetH_LifeCount(H1, H2);
			LifeCount();
		}
		//************
	}
	// 1P,2Pの処理終了
	
	//------------
	// 描画「準備」
	//------------
	{
		DrawSetting();
	}
	//----------
	// 時間計測
	//----------
	{
		TimeCount();	// 反映されるのはグラ準備した後なので、問題なし
	}



	}// 終了、ヒットストップしてないなら動く
#pragma endregion

	//-----------------
	//
	// ヒットストップ中
	//
	//-----------------

	// ストップ中は全て動かない
	else if(S.StopTime > 0){
		S.StopTime -= 1;
		//------------
		// 描画「準備」
		//------------
		{
			DrawSetting();
		}
	}

	

	//-----------------
	//
	// 暗転時間の調整
	//
	//-----------------
	{
		if (S.Anten > 0)S.Anten -= 1;
		else if (S.Anten < 0)S.Anten = 0;
		if (S.noGauge > 0)S.noGauge -= 1;
		else if (S.noGauge < 0)S.noGauge = 0;
	}
	//-----------------
	//
	// トレモ設定その２
	//
	//-----------------

	// 2P操作用その２
	if (S.TraningFlg){
		//[ 2P操作 ]
		if (S.TSwitch[7] == 1 || S.TSwitch[7] == 2){
			P2Ctrl2();
		}
	}

	//**************
	//
	// ポーズ画面
	//
	//**************
		if (
			((!S.TraningFlg) && (P_BInput(7) == 1))
			||
			((S.TraningFlg) && (P1_BInput(7) == 1))
		){
			// 録画をオフにする
			S.TSwitch[7] = 0;

			// 一度だけ操作を受け付けない
			P1_BCheck();
			P2_BCheck();
			GetP_Pause(P1, P2);
			GetS_Pause(S);

			// ポーズ画面に移行
			PauseSetting(1);
			raw_set();	// 左右いれっぱを一度だけオフ
		}
		return 0;
}


/// <summary>
/// 体力、位置を初期化
/// </summary>
void CharLoad()
{
	// 名前をselectで出した値からロード
	P1.Name = Player_Name_1();
	P2.Name = Player_Name_2();
	load_LifeCount();	// ライフ・生死判定

	// nameC設定
	GetP_Air(P1);
	GetPName();
	GetP_Air(P2);
	GetPName();

	// 声
	PVoiceSet(1, P1.Name);
	PVoiceSet(2, P2.Name);

	/*********
	* 能力設定
	*********/
	// 1P
	GetP1_Player(P1);
	GetS_Player(S);
	Player_State();
	// 2P
	GetP1_Player(P2);
	GetS_Player(S);
	Player_State();

	// パラメータ初期化
	Player P[2];
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){

		// プレイヤー側
		P[i].PSide = i+1;
		P[i].muki = i;


		// 体力をライフの最大値に初期化
		P[i].Life = P[i].C.lifeMax;
		P[i].LifeC = LIFECOUNT_MAX;
		P[i].Power = 0;
		P[i].aGauge = GUARD_MAX;
		P[i].GRecovery = 0;
		P[i].addG = 0;

		P[i].YPos = GROUND;
		P[i].XVel = 0;
		P[i].YVel = 0;
		P[i].SFlg = 0;
		P[i].ctrl = 1;
		P[i].touchF = true;
		P[i].time = 0;
		P[i].StopTime = 0;
		P[i].MoveHit = 1;

		P[i].aPosX = 0;
		P[i].aPosY = 0;

		P[i].D.armor = 0;

		// Var初期化
		for (int j = 0; j < VAR_MAX; j++){
			P[i].Var[j] = 0;
		}
		for (int j = 0; j < ATT_MAX; j++){
			P[i].XAtt[j] = -100, P[i].YAtt[j] = -100;
			P[i].WAtt[j] = 0, P[i].HAtt[j] = 0;
		}
	}
	P1 = P[0], P2 = P[1];

	
	
	//初期座標をセット
	P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
	if (fpSet == 1){
		P1.XPos = STAGE_WIDTH - GAMENHAJI - 85, P2.XPos = STAGE_WIDTH - GAMENHAJI - 5;
	}
	else if (fpSet == -1){
		P1.XPos = GAMENHAJI + 5, P2.XPos = GAMENHAJI + 85;
	}

	// ヘルパー初期化
	for (int i = 0; i < HM; i++){
		H1[i].var = false;
		H1[i].A.damage = 0;
		H2[i].var = false;
		H2[i].A.damage = 0;

		for (int j = 0; j < HIT_MAX; j++){
			H1[i].XAtt[j] = -1;
			H1[i].YAtt[j] = -1;
			H1[i].WAtt[j] = -1;
			H1[i].HAtt[j] = -1;

			H2[i].XAtt[j] = -1;
			H2[i].YAtt[j] = -1;
			H2[i].WAtt[j] = -1;
			H2[i].HAtt[j] = -1;
		}
	}

	P1.colorCTime = 0, P2.colorCTime = 0;
	P1.D.yaccel = 0.35, P2.D.yaccel = 0.35;

	//================
	// システム初期化
	//================
	S.ScroolX = ((STAGE_WIDTH / 2) - (SCREEN_W / 2));
	if (fpSet == 1){
		S.ScroolX = STAGE_WIDTH - SCREEN_W;
	}
	else if (fpSet == -1){
		S.ScroolX = 0;
	}

	S.ScroolY = (STAGE_HEIGHT - SCREEN_H);
	// 開幕はスクロールしない
	if (S.TraningFlg){
		S.noScroolX = false;
		S.noScroolY = false;
	}
	else{
		S.noScroolX = true;
		S.noScroolY = true;
	}

	S.roundState = 0;
	P1.stateno = 90, P2.stateno = 90;
	if (S.TraningFlg){
		S.roundState = 1;
		P1.stateno = 0, P2.stateno = 0;
	}
	S.roundTime = 0;
	S.StopTime = 0;
	S.Anten = 0;
	S.noGauge = 0;

	// 残像
	afterimageSetting(1, 0, 0, 0, 0);
	afterimageSetting(2, 0, 0, 0, 0);

	// ダッシュ関連
	SubState_DashReset();

	fpSet = 0;
}

void Get_PSet(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void Get_P1Set(Player GP1)
{
	P1 = GP1;
}

void Get_P2Set(Player GP2)
{
	P2 = GP2;
}

void Get_SSet(System_t GS)
{
	S = GS;
}

void Get_PSSet(Player GP1, Player GP2, System_t GS)
{
	P1 = GP1;
	P2 = GP2;
	S = GS;
}

void Get_HSet(Helper GH1[], Helper GH2[])
{
	int i;
		for (i = 0; i < HM; i++){
			H1[i] = GH1[i];
			H2[i] = GH2[i];
		}
}


//-------------------------------------------------------------------------------
//
// カラー決定
//
//-------------------------------------------------------------------------------

void Get_Color(int c, int PSide)
{
	if (PSide == 1)P1.Palno = c;
	if (PSide == 2)P2.Palno = c;

	// パレットもう一度ロード
	Load_1 = 0;
}


//-------------------------------------------------------------------------------
//
// トレモスイッチ
//
//-------------------------------------------------------------------------------

// トレモのオン・オフ
void TraningSwitch(boolean flg)
{
	S.TraningFlg = flg;

}

void AISwitch(boolean flg)
{
	S.AI = flg;
}

// varにnumをセット
void T_VarSet(int var, int num)
{
	S.TSwitch[var] = num;
}


//-------------------------------------------------------------------------------
//
// 処理の内容
//
//-------------------------------------------------------------------------------

void AttackSetting()
{
	// ヘルパー
	if ((P2.stateno < 1060) || (P2.stateno >= 1070)){
		GetP_HelDamage(P1, P2);
		GetH_HelDamage(H1, H2);
		GetS_HelDamage(S);
		HelperDamCheck();
	}
	if ((P1.stateno < 1060) || (P1.stateno >= 1070)){
		GetP_HelDamage(P2, P1);
		GetH_HelDamage(H2, H1);
		GetS_HelDamage(S);
		HelperDamCheck();
	}

	// 本体
	GetP_SubDamage(P1, P2);
	GetS_SubDamage(S);
	DamageCheck();	

	GetP_SubDamSet(P1, P2);
	GetS_SubDamSet(S);
	DamSetting();// OK!
}

void MutekiCount()
{
	//[ 無敵時間減少 ]
	{
		if (P1.StopTime == 0){
			if (P1.Muteki > 0)P1.Muteki -= 1;
			else if (P1.Muteki < 0)P1.Muteki = 0;
		}
		if (P2.StopTime == 0){
			if (P2.Muteki > 0)P2.Muteki -= 1;
			else if (P2.Muteki < 0)P2.Muteki = 0;
		}
	}

	// ついでに色も
	if (P1.colorCTime > 0)P1.colorCTime -= 1;
	else if (P1.colorCTime < 0)P1.colorCTime = 0;
	if (P2.colorCTime > 0)P2.colorCTime -= 1;
	else if (P2.colorCTime < 0)P2.colorCTime = 0;

}

void GraphicSetting()
{
	//画像をもってくる
	//P1.Image = P1.GraphImage[P1.aNum1][P1.aNum2];
	//P2.Image = P2.GraphImage[P2.aNum1][P2.aNum2];
	P1.Image = Player_ImageGet(P1);
	P2.Image = Player_ImageGet(P2);

	// 画像サイズ取得 P1,2
	GetGraphSize(P1.Image, &P1.GraphW, &P1.GraphH);
	GetGraphSize(P2.Image, &P2.GraphW, &P2.GraphH);

	//---------------
	// 演出（地震）
	//---------------
	{
		if (S.quakeTime > 0)S.quakeTime--;
		else{
			S.quakeX = 0;
			S.quakeY = 0;
			S.quakeTime = 0;
		}
	}

	//画像をもってくる
	GetP1_Player(P1);
	GetH_Player(H1, H2);
	Helper_Graph();

	GetP1_Player(P2);
	GetH_Player(H2, H1);
	Helper_Graph();

}

void TouchPos()
{
	

	// 基本座標セット、
	// この後、スクロールに合わせて修正
	GetP_SubSystem(P1, P2);
	GetS_SubSystem(S);
	VelPosSet();

	//------------
	// 速度減少
	//------------
	{
		// 設定を渡す
		GetP_SubSystem(P1, P2);
		GetS_SubSystem(S);
		VelXYAdd();	// 値を返す

	}

	// 設定を渡す
	GetP_SubSystem(P1, P2);
	GetS_SubSystem(S);
	// 当たってたら位置変更
	TouchCheck();
}


// ステート内容
void SetParam()
{
	Player P[2];
	Helper H[2][HM];


	count = 0;
	
	// ステート
	do{
		// プレイヤー側を判断
		for (int i = 0; i < 2; i++){

			if (i == 0){
				P[0] = P1;
				P[1] = P2;
				for (int j = 0; j < HM; j++){
					H[0][j] = H1[j];
					H[1][j] = H2[j];
				}
			}
			else if (i == 1){
				P[0] = P2;
				P[1] = P1;
				for (int j = 0; j < HM; j++){
					H[0][j] = H2[j];
					H[1][j] = H1[j];
				}
			}

			// ステート実行
			if ((count == 0) || (P[0].More == 1 && count > 0)){
				// 名前のステートに移動し、値をもらってくる
				switch (P[0].Name){
				case EIN:
					//ステートの設定を適用
					GetP_EinParam(P[0], P[1]);
					GetS_EinParam(S);
					GetH_EinParam(H[0], H[1]);
					EinParam();
					break;
				case CORNEL:
					//ステートの設定を適用
					GetP_CorParam(P[0], P[1]);
					GetS_CorParam(S);
					GetH_CorParam(H[0], H[1]);
					CorParam();
					break;
				case BOUNCER:
					//ステートの設定を適用
					GetP_BouParam(P[0], P[1]);
					GetS_BouParam(S);
					GetH_BouParam(H[0], H[1]);
					BouParam();
					break;
				case HELIOS:
					//ステートの設定を適用
					GetP_HelParam(P[0], P[1]);
					GetS_HelParam(S);
					GetH_HelParam(H[0], H[1]);
					HelParam();
					break;
				case HYDE:
					//ステートの設定を適用
					GetP_HydParam(P[0], P[1]);
					GetS_HydParam(S);
					GetH_HydParam(H[0], H[1]);
					HydParam();
					break;
				case SYUICHI:
					//ステートの設定を適用
					GetP_SyuParam(P[0], P[1]);
					GetS_SyuParam(S);
					GetH_SyuParam(H[0], H[1]);
					SyuParam();
					break;
				default:
					//ステートの設定を適用
					GetP_HydParam(P[0], P[1]);
					GetS_HydParam(S);
					GetH_HydParam(H[0], H[1]);
					HydParam();
					break;
				}	// switch
			}	// if
			
			
		}	// for

		count++;

	} while ((P1.More == 1) || (P2.More == 1));


}

// ステート内容
void SetParam2()
{
	Player P[2];
	Helper H[2][HM];


	count = 0;

	// ステート
	do{
		// プレイヤー側を判断
		for (int i = 0; i < 2; i++){
			if (i == 0){
				P[0] = P1;
				P[1] = P2;
				for (int j = 0; j < HM; j++){
					H[0][j] = H1[j];
					H[1][j] = H2[j];
				}
			}
			else if (i == 1){
				P[0] = P2;
				P[1] = P1;
				for (int j = 0; j < HM; j++){
					H[0][j] = H2[j];
					H[1][j] = H1[j];
				}
			}

			// ステート実行

			if ((count == 0) || (P[0].More == 1 && count > 0)){
				// 名前のステートに移動し、値をもらってくる
				switch (P[0].Name){
				case EIN:
					//ステートの設定を適用
					GetP_EinParam(P[0], P[1]);
					GetS_EinParam(S);
					GetH_EinParam(H[0], H[1]);
					EinParam();
					break;
				case CORNEL:
					//ステートの設定を適用
					GetP_CorParam(P[0], P[1]);
					GetS_CorParam(S);
					GetH_CorParam(H[0], H[1]);
					CorParam();
					break;
				case BOUNCER:
					//ステートの設定を適用
					GetP_BouParam(P[0], P[1]);
					GetS_BouParam(S);
					GetH_BouParam(H[0], H[1]);
					BouParam();
					break;
				case HELIOS:
					//ステートの設定を適用
					GetP_HelParam(P[0], P[1]);
					GetS_HelParam(S);
					GetH_HelParam(H[0], H[1]);
					HelParam();
					break;
				case HYDE:
					//ステートの設定を適用
					GetP_HydParam(P[0], P[1]);
					GetS_HydParam(S);
					GetH_HydParam(H[0], H[1]);
					HydParam();
					break;
				default:
					//ステートの設定を適用
					GetP_HydParam(P[0], P[1]);
					GetS_HydParam(S);
					GetH_HydParam(H[0], H[1]);
					HydParam();
					break;
				}
			}
			
		}
		count++;

	} while ((P1.More == 1) || (P2.More == 1));


}

void DrawSetting()
{
	//TestText(P1.WHit[0]);
	// 計測に使う値を送る
	GetP_MainDraw(P1, P2);
	GetS_MainDraw(S);
	GetH_MainDraw(H1, H2);

	GetP_ObjectDraw(P1, P2);
	GetS_ObjectDraw(S);
	GetH_ObjectDraw(H1, H2);

	GetP_Effect(P1, P2);
	GetS_Effect(S);
	GetH_Effect(H1, H2);

}

void HitBoxSetting()
{
	// 1P
	GetP_GHitbox(P1);
	GetH_GHitbox(H1, H2);
	GHitbox();// OK!
	HelperKurai();

	// 2P
	GetP_GHitbox(P2);
	GetH_GHitbox(H2, H1);
	GHitbox();// OK!
	HelperKurai();
}

void TimeCount()
{
	Player p[2];
	Helper h[2][HM];
	p[0] = P1, p[1] = P2;
	for (int i = 0; i < HM; i++){
		h[0][i] = H1[i];
		h[1][i] = H2[i];
	}


	for (int i = 0; i < 2; i++){
		
		// NowTime
		if (p[i].StopTime == 0){
			p[i].time += 1;
		}

		// AnimTime
		p[i].AnimTime += 1;

		// StopTime
		if ((p[i].StopTime > 0) && (mOldStop[i] >= p[i].StopTime)){
			p[i].StopTime -= 1;
		}
		mOldStop[i] = p[i].StopTime;

		// ヘルパー
		for (int j = 0; j < HM; j++){
			if (h[i][j].var){

				// NowTime
				if (h[i][j].stopTime == 0){
					h[i][j].time += 1;
				}
				h[i][j].HAnimTime += 1;

				// StopTime
				if ((h[i][j].stopTime > 0) && (hOldStop[i][j] >= h[i][j].stopTime)){
					h[i][j].stopTime -= 1;
				}
				hOldStop[i][j] = h[i][j].stopTime;

			}
		}
		
	}

	P1 = p[0], P2 = p[1];
	
	for (int i = 0; i < HM; i++){
		H1[i] = h[0][i];
		H2[i] = h[1][i];
	}
	

	// 開幕
	if (S.roundState == 0){
		// ブラックアウト
		bo = 0;

		if (S.roundTime <= 0){
			// ボタン押して演出省略
			for (int i = 0; i < 4; i++){
				if (P_BInput(i + 1) == 1){
					P1.stateno = 0;
					P2.stateno = 0;
					
					/*
					//初期座標をセット
					P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
					P1.YPos = GROUND, P2.YPos = GROUND;
					P1.XVel = 0, P2.XVel = 0;
					P1.YVel = 0, P2.YVel = 0;
					*/
				}
			}
		}

		// 立ちへ移行したら
		if ((P1.stateno == 0) && (P2.stateno == 0))
		{
			if (S.roundTime == 0){
				S.noScroolX = false;
				S.noScroolY = false;
				//初期座標をセット
				P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
				P1.YPos = GROUND, P2.YPos = GROUND;
				P1.XVel = 0, P2.XVel = 0;
				P1.YVel = 0, P2.YVel = 0;
			}
			// システム関連
			if (S.roundTime < 120){
				S.roundTime++;
			}
		// 対戦へ
			else if (S.roundTime >= 120){
				S.roundState = 1;
				S.roundTime = 0;
				S.roundTime = 60 * ROUNDTIME + 59;
				load_LifeCount();
			}
		}
	}
	// 対戦中
	else if (S.roundState == 1){
		//if (S.roundTime != 0)S.roundTime = 0;
		
		bo = 0;
		S.roundTime--;
		if (S.TraningFlg)S.roundTime = 60 * ROUNDTIME;	// トレモ時は無効
		if (S.roundTime <= 0){
			if ((float)((float)P1.Life / (float)P1.C.lifeMax) > (float)((float)P2.Life / (float)P2.C.lifeMax)) 
				P2.Life = 0;
			else if ((float)((float)P2.Life / (float)P2.C.lifeMax) > (float)((float)P1.Life / (float)P1.C.lifeMax)) 
				P1.Life = 0;
			else if ((float)((float)P1.Life / (float)P1.C.lifeMax) == (float)((float)P2.Life / (float)P2.C.lifeMax));
			//P1.ctrl = 0, P2.ctrl = 0;
			//S.roundTime = 0;
			//S.StopTime = 20;
			// 移行
			//if ((P1.LifeC <= 0) || (P2.LifeC <= 0))S.roundState = 3;
			//else{ S.roundState = 2; }
		}
	}
	// ラウンド取得
	else if (S.roundState == 2){
		
		// いったん動きを止める
		if (S.roundTime <= 0){
			if (P1.ctrl){
				P1.ctrl = 0;
				if (P1.SFlg == 2)P1.stateno = 46, P1.ignoreG = 0;
				else{ P1.stateno = 0; }
			}
			if (P2.ctrl){
				P2.ctrl = 0;
				if (P2.SFlg == 2)P2.stateno = 46, P2.ignoreG = 0;
				else{ P2.stateno = 0; }
			}

			// 位置戻し処理
			// ダウン後にねっぱなし
			if (P1.stateno == 1055){
				if ((P2.stateno != 0) && (P1.Life == 0) )P1.time = 0;
			}
			if (P2.stateno == 1055){
				if ((P1.stateno != 0) && (P2.Life == 0))P2.time = 0;
			}

			// お互い立ち上がったら
			if (P1.stateno == 0 && P2.stateno == 0){
				bo--;
				if (bo < -15){
					//SetBlack(255 - ((bo + 15) * -30));
				}
			}
		}

		// 準備できたら暗転
		if ((P1.stateno == 0) && (P2.stateno == 1055)){
			bo--;
			//SetBlack(bo * -25);
			// 経過するまで寝てる
			P2.time = 1;
			// 戻す
			if (bo == -15){
				P1.time = 0, P2.time = 0;
				P1.stateno = 0, P2.stateno = 0;
				P1.muki = 0, P2.muki = 1;
				// 初期位置に戻す
				S.ScroolX = ((STAGE_WIDTH / 2) - (SCREEN_W / 2));
				//初期座標をセット
				P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
				P1.YPos = GROUND, P2.YPos = GROUND;
				P1.XVel = 0, P2.XVel = 0;
				P1.YVel = 0, P2.YVel = 0;

				P1.Life = P1.C.lifeMax;
				P2.Life = P2.C.lifeMax;
				P1.aGauge = GUARD_MAX;
				P2.aGauge = GUARD_MAX;
				P1.GRecovery = 0;
				P2.GRecovery = 0;
				P1.D.armor = 0;
				P2.D.armor = 0;
			}
		}
		else if ((P1.stateno == 1055) && (P2.stateno == 0)){
			bo--;
			//SetBlack(bo * -25);
			// 経過するまで寝てる
			P1.time = 1;
			if (bo == -15){
				P1.time = 0, P2.time = 0;
				P1.stateno = 0, P2.stateno = 0;
				P1.muki = 0, P2.muki = 1;
				// 初期位置に戻す
				S.ScroolX = ((STAGE_WIDTH / 2) - (SCREEN_W / 2));
				//初期座標をセット
				P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
				P1.YPos = GROUND, P2.YPos = GROUND;
				P1.XVel = 0, P2.XVel = 0;
				P1.YVel = 0, P2.YVel = 0;

				P1.Life = P1.C.lifeMax;
				P2.Life = P2.C.lifeMax;
				P1.aGauge = GUARD_MAX;
				P2.aGauge = GUARD_MAX;
				P1.GRecovery = 0;
				P2.GRecovery = 0;
				P1.D.armor = 0;
				P2.D.armor = 0;
			}
		}

		// 立ちへ移行して、まだ始まってない
		// roundTimeが1以上で移動可能にする
		if (((P1.Life > 0) && (P2.Life > 0))
			&& 
			(P1.stateno <= 199 && P2.stateno <= 199)
			&&
			(S.roundTime <= 0)
			&&
			(bo == -15))	// 開始時間
		{
			S.roundTime = 1;
			P1.stateno = 0, P2.stateno = 0;
			// 初期位置に戻す
			/*
			S.ScroolX = ((STAGE_WIDTH / 2) - (SCREEN_W / 2));
			//初期座標をセット
			P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
			P1.YPos = GROUND, P2.YPos = GROUND;
			P1.XVel = 0, P2.XVel = 0;
			P1.YVel = 0, P2.YVel = 0;
			*/
		}

		// 始まったら
		if (S.roundTime > 0)
		{
			// 移動のみできるか
			P1.ctrl = 0, P2.ctrl = 0;
			// システム関連
			if (S.roundTime < 50){
				S.roundTime++;
			}
			// 対戦へ
			else if (S.roundTime >= 50){
				S.roundState = 1;
				S.roundTime = 0;
				S.roundTime = 60 * ROUNDTIME + 59;
				load_LifeCount();
			}
		}

	}
	// 終了
	else if (S.roundState == 3){
		S.roundTime++;
		if (S.StopTime == 0)S.StopTime++;
		
		// 勝者
		if (S.roundTime == 1){
			if (P1.Life <= 0 && P2.Life <= 0){
				Result_WinSide(0, -1, -1);
				//Result_GetData(P1.Name, P2.Name, 1, 2, S.NowStage);
				WinCount(3);
			}
			else if (P1.Life <= 0){
				Result_WinSide(2, P2.Name, P1.Name);
				//Result_GetData(P1.Name, P2.Name, 1, 2, S.NowStage);
				WinCount(2);
			}
			else if (P2.Life <= 0){
				Result_WinSide(1, P1.Name, P2.Name);
				//Result_GetData(P1.Name, P2.Name, 1, 2, S.NowStage);
				WinCount(1);
			}
		}

		// 動きを止める
		if (P1.ctrl){
			P1.ctrl = 0;
			//if (P1.SFlg == 2)P1.stateno = 46;
			//else{ P1.stateno = 0; }
		}
		if (P2.ctrl){
			P2.ctrl = 0;
			//if (P2.SFlg == 2)P2.stateno = 46;
			//else{ P2.stateno = 0; }
		}

		// 終了へ移行する
		if (S.roundTime >= 90){
			load_LifeCount();
			S.roundState = 0;
			S.roundTime = 0;
			Load_1 = 0;
			CharLoad();
			if (!S.TraningFlg){
				// アケモか対戦か
				if (Arcade_Switch(-1) == 0){
					ModeChange(GameScene(ResultScene));	// リザルト
				}
				// アケモ
				else if(Arcade_Switch(-1) == 1){
					if (P1.Life > 0){
						Arcade_BattleCount(1);	// 勝数
					}
					else{
						Arcade_BattleCount(0);	// 勝数
					}
					ModeChange(GameScene(ResultScene));	// アケ用のリザルト
				}
				// 再生 or 録画
				if (Replay_Mode(-1) > 0){
					// 再生
					if (Replay_Mode(-1) == 1){
						ModeChange(GameScene(ReplayScene));	// リザルト
					}
					Replay_End();
				}
			}
			else{
				ModeChange(GameScene(VersusScene));	// 対戦画面へループ
			}
		}
	}	// roundstate 3
}

void Load_State()
{  
	Load_1 = 0;
}                                                                        

// 画像、判定のデータを読み込み
// Load_2（air）はここで解決
// airの読み込みが目的ではない
void LoadPict()
{
	
	// カラー被り防止
	if ((P1.Name == P2.Name) && (P1.Palno == P2.Palno)){
		P2.Palno = 2;
		if (P1.Palno == 2)P2.Palno = 1;
	}

	// アニメ時間取得
	//P1 = GetAnimElem(P1);
	//P2 = GetAnimElem(P2);

	// 判定
	GetP_GHitbox(P1);
	load_GHitBox();	// airにair2の内容をセット
	GetP_GHitbox(P2);
	load_GHitBox();

	// アニメ時間取得
	P1 = GetAnimElem(P1);
	P2 = GetAnimElem(P2);

	// コモンステータス

	// 画像を読み込む
	GetP1_Player(P1);	//原因
	//load_pict(P1.Name, P1.PSide);
	Player_GraphLoad();

	GetP1_Player(P2);	//原因
	//load_pict(P2.Name, P2.PSide);
	Player_GraphLoad();

}

void fLoad_State()
{
	if (!Load_2){

		// 名前取得
		// LoadAllFileでやってみる(やってた)
		GetName();
		Load1_Pict();	// vector作る
		// air取得
		for (int i = 0; i < Character_Max(); i++){
			load_air(i, 1);	// アニメ@@
			load_pict(i, 1);	// 画像
								// ポトレ読み込むだけなので、1P限定にする
		}

		// トレモ設定読み込み
		S.TSwitch[5] = GetPrivateProfileInt("Training", "data", 0, "char/tlist.ini");

		// ロード完了
		Load_2 = true;
	}
}

void FirstPosSet(int n)
{
	fpSet = n;
}

void StopTime_Add(int n)
{
	S.StopTime += n;
}

void StopCmd(boolean cmd)
{
	stopCmd = cmd;
}

///////////////////////////////////////////////////////////////////////
// トレモ設定のセーブ
int Tlist_Save(void)
{
	FILE *fp;

	// ハイスコアセーブ用ファイルを開く
	// (２番目の引数の"wb"の'w'は「書きこみ」の意味
	// 'b'はバイナリの意味(逆はテキスト))
	fopen_s(&fp, "char/tlist.ini", "wt");

	// オープンできなかったらここで終了
	if (fp == NULL){ return FALSE; }


	// データの書き出し
	//fwrite(In_BLog, sizeof(In_BLog), 1, fp);
	
	// [sizeof]はメモリサイズを調べる
	//fprintf(fp, "\n");		// なんか最初の行が消されるのでこれ使う
	{
		fprintf(fp, "[Training]\n");
		fprintf(fp, "data = %d\n", S.TSwitch[5]);
	}


	// ファイルを閉じる
	fclose(fp);

	/* 読み込みデータの確認 */

	return TRUE;
}