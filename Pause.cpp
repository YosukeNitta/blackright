//==================================================================================
//
//			ポーズ画面の管理部
//
//========================================================================

//インクルード部--------------------------------------------
#include "pch.h"

//デファイン部----------------------------------------------




//ﾃﾞｰﾀ定義部------------------------------------------------

static int Load_1;
static int P1_Key;
static const int TxtDist = 29;
static const int drawMax = 15;	// ポーズ表示最大
static int drawPos;
static int SPoint;	// 選択ポインタの位置初期化
static int TimeStop, kettei, kettei2;
static int inputUP, inputDOWN;
static int SentakuNum;
static int selectMax = 12;

static int page;
static const int page_max = 2;
static const int pageBoxSize = SCREEN_H - 40 - 40;

static Player P[2];

// 共通、　コマンド表とトレモ設定
static int ComList;

//通常メニュー
static int SNum;

// トレモ設定
static int column, row;
static int c_Max, r_Max;

// 次の項目
static int nx;

// コマンド
static int gK[10], gB[4], gP;

// 画像
//static int cursor;

// rawを変更しない
static boolean noChangeRow;


//内部関数宣言部--------------------------------------------

static void CommandList();
static void TraningSet();
static void TrainingList();
//void BackGraph();	// 背景描画
static void BackGraphT();

//プログラム部----------------------------------------------



int Pause()
{

	if (Load_1 == 0){

		// 選択項目の数を取得
		kettei = 0, SPoint = 0,
		SentakuNum = 5;

		//通常メニュー
		SNum = 3;

		// 項目の最大数
		selectMax = 12;
		// 表示位置
		drawPos = 0;

		// トレモ
		column = 0, row = 0;
		c_Max = 0, r_Max = 0;
		inputUP = 0, inputDOWN = 0;
		ComList = 0;

		// 画像読み込み
		// Load_Pauseで行う

		Load_1 = 1;
	}


	{
		// 入力はここでチェック
		//P1_Key = P_Input(1);	// OK
	}

	// ループ、プログラム実行開始
	// [ 共通設定]
	{
		
		// 2Pも使える
		if (!S.TraningFlg){
			// 決定
			if ((P_BInput(1) == 1 || P_BInput(3) == 1) && (kettei != 1))
			{
				kettei = 1;
			}
			else{ kettei = 0; }

			// 上
			if (KInput(P_BInput(108))){
				inputUP += 1;
			}
			else{ inputUP = 0; }

			// 下
			if (KInput(P_BInput(102))){
				inputDOWN += 1;
			}
			else{ inputDOWN = 0; }
		}
		// トレモ用
		else{
			// 決定
			if ((P1_BInput(1) == 1 || P1_BInput(3) == 1) && (kettei != 1))
			{
				kettei = 1;
			}
			else{ kettei = 0; }

			// 上
			if (KInput(P1_BInput(108))){
				inputUP += 1;
			}
			else{ inputUP = 0; }

			// 下
			if (KInput(P1_BInput(102))){
				inputDOWN += 1;
			}
			else{ inputDOWN = 0; }

			
		}

		// 決定してたらカウント加算
		if (kettei == 1){
			TimeStop += 1;
		}
		else{
			TimeStop = 0;
		}

	}

	// [トレモメニュー]
	if (S.TraningFlg)
	{
		
		// 基本
		if (ComList == 0){

			/**************
			* 背景
			**************/
			if (P1_BInput(4) == 0)
				// 背景
				{
					BackGraphT();
				}

				

				// カーソル移動
				if (kettei == 0){
					if (inputUP == 1) SPoint--, SEStart(36);
					if (inputDOWN == 1) SPoint++, SEStart(36);
					if (SPoint < 0)SPoint = SentakuNum + selectMax - 1;
					if (SPoint > SentakuNum + selectMax - 1)SPoint = 0;
				}
				
				// トレモ設定
				TraningSet();

				// 決定キー入力
				// 位置初期化
				if ((kettei == 1) && (SPoint == selectMax))
				{
					kettei = 0;
					return 1;
				}
				// コマンド表
				else if ((kettei == 1) && (SPoint == selectMax + 1))
				{
					kettei = 0;
					ComList = 1;
				}
				// 対戦画面に戻る
				else if (((kettei == 1) && (SPoint == selectMax + 2)) || (P1_BInput(2) == 1) || (P1_BInput(7) == 1))
				{
					kettei = 0;
					return 4;
				}
				// キャラセレに戻る
				else if ((kettei == 1) && (SPoint == selectMax + 3))
				{
					kettei = 0;
					return 2;
				}
				// メニューに戻る
				else if ((kettei == 1) && (SPoint == selectMax + 4))
				{
					kettei = 0;
					Load_1 = 0;
					return 3;
				}
				

				// 描画
				if (P1_BInput(4) == 0)
				{
					// カーソル描画
					{
						// 描画ブレンドモードをアルファブレンド（透明）にする
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
						// カーソル描画
						DrawBox(0, (SPoint * TxtDist) + 29 + (drawPos * TxtDist), 
							SCREEN_W, (SPoint * TxtDist) + 48 + (drawPos * TxtDist), GetColor(20, 20, 250), true);
						NoBlend();
					}

					// 設定データ描画
					{
						TrainingList();
						//DrawString(0, 0, "D押しっぱで項目を消す", Cr);
						DrawString(TxtDist, TxtDist * (selectMax + 1) + (drawPos * TxtDist), "ポジションリセット", Cr);
						DrawString(TxtDist, TxtDist * (selectMax + 2) + (drawPos * TxtDist), "コマンド表", Cr);
						DrawString(TxtDist, TxtDist * (selectMax + 3) + (drawPos * TxtDist), "戦闘画面へ戻る", Cr);
						
						if (drawPos < 0)	// ずらす
						DrawString(TxtDist, TxtDist * (selectMax + 4) + (drawPos * TxtDist), "キャラクターセレクトに戻る", Cr);
						if (drawPos < -1)	// ずらす
							DrawString(TxtDist, TxtDist * (selectMax + 5) + (drawPos * TxtDist), "メニューに戻る", Cr);

						// 項目位置の四角
						if (drawPos == 0)
							DrawBox(SCREEN_W - 20, 20, SCREEN_W, 20 + pageBoxSize, Cr, true);
						else if (drawPos == -1)
							DrawBox(SCREEN_W - 20, 40, SCREEN_W, 40 + pageBoxSize, Cr, true);
						else if (drawPos == -2)
							DrawBox(SCREEN_W - 20, SCREEN_H - 20 - pageBoxSize, SCREEN_W, SCREEN_H - 20, Cr, true);
					}
					// カーソル描画
					//DrawBox(5, (SPoint * TxtDist) + 29, 24, (SPoint * TxtDist) + 48, Cr, true);
					
				}
			}
			else if(ComList == 1){
				CommandList();
			}

	}//トレモメニュー終了、
	// 通常メニュー
	else{

		if (ComList == 0){
			/**************
			* 背景
			**************/
			if (P_BInput(4) == 0)
			{
				BackGraphT();
			}

			// カーソル移動
			if (kettei == 0){
				if (inputUP == 1) SPoint--, SEStart(36);
				if (inputDOWN == 1) SPoint++, SEStart(36);
				if (SPoint < 0)SPoint = SNum - 1;
				if (SPoint > SNum - 1)SPoint = 0;

			}

			// 決定キー入力
			// 画面に戻る
			if (((kettei == 1) && (SPoint == 0)) || (P_BInput(2) == 1) || (P_BInput(7) == 1))
			{
				kettei = 0;
				return 4;
			}
			// コマンド表
			else if ((kettei == 1) && (SPoint == 1))
			{			
				kettei = 0;
				ComList = 1;
			}
			// メニューに戻る
			else if ((kettei == 1) && (SPoint == 2))
			{
				kettei = 0;
				Load_1 = 0;
				return 3;
			}

			// メニューに戻る
			if (Replay_Mode(-1) == 1)
			{
				kettei = 0;
				Load_1 = 0;
				Replay_End();
				return GameScene(ReplayScene);
			}

			// 描画
			if (P_BInput(4) == 0)
			{
				
				// カーソル描画
				{
					// 描画ブレンドモードをアルファブレンド（透明）にする
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
					// カーソル描画
					DrawBox(0, (SPoint * TxtDist) + 29, SCREEN_W, (SPoint * TxtDist) + 48, GetColor(20, 20, 250), true);
					NoBlend();
				}
				//DrawString(0, 0, "D押しっぱで項目を消す", Cr);
				DrawString(TxtDist, TxtDist * 1, "対戦画面に戻る", Cr);
				DrawString(TxtDist, TxtDist * 2, "コマンド表", Cr);
				DrawString(TxtDist, TxtDist * 3, "メニューに戻る", Cr);
				
			}

		}
		else if (ComList == 1){
			CommandList();
		}
	}// 対戦メニュー終了

	return 0;
}


void GetP_Pause(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_Pause(System_t GS)
{
	S = GS;
}


void TraningSet()
{

	/**************
	* 計算
	**************/
	{
		{
			// 左右の入力
			if ((P1_BInput(104) == 1) || (P1_BInput(106) == 1)){
				if (P1_BInput(104) == 1)row -= 1;
				if (P1_BInput(106) == 1)row += 1;
			}
			else{ row = 0; }

			if (row == 1 || row == -1)SEStart(35);

			// イキスギィ！
			
			//if ((P1_BInput(104) == 0) && (P1_BInput(106) == 0)){

			// ロードした最初は変えない（いれっぱでかわるの防止）
			if (noChangeRow){
				row = 0;
				noChangeRow = false;
			}

			// 描画位置
			if ((SPoint > drawMax - 1) && (drawPos > -1))drawPos = -1;
			else if ((SPoint > drawMax) && (drawPos > -2))drawPos = -2;
			
			if ((SPoint == 0) && (drawPos != 0))drawPos = 0;
			else if ((SPoint == 1) && (drawPos < -1))drawPos = -1;
		}

		{
			nx = 0;
			// 体力
			if (SPoint == nx){
				if (S.T_Life == 0){
					if (row == 1 || row == -1)
						S.T_Life = 1;
				}
				else if (S.T_Life == 1){
					if (row == 1 || row == -1)
						S.T_Life = 0;
				}
			}
			nx++;
			// ゲージ
			if (SPoint == nx){
				if (row == 1)
					S.T_Power += 1;
				else if (row == -1)
					S.T_Power -= 1;

				if (S.T_Power < 0)
					S.T_Power = 3;
				if (S.T_Power > 3)
					S.T_Power = 0;
			}
			nx++;
			// ガードゲージ
			if (SPoint == nx){
				if (row == 1)
					S.T_GGauge += 1;
				else if (row == -1)
					S.T_GGauge -= 1;

				if (S.T_GGauge < 0)
					S.T_GGauge = 1;
				if (S.T_GGauge > 1)
					S.T_GGauge = 0;
			}
			nx++;
			// アドゲージ
			if (SPoint == nx){
				if (row == 1)
					S.T_AddGauge += 1;
				else if (row == -1)
					S.T_AddGauge -= 1;

				if (S.T_AddGauge < 0)
					S.T_AddGauge = 1;
				if (S.T_AddGauge > 1)
					S.T_AddGauge = 0;
			}
			nx++;
			// 体勢
			if (SPoint == nx){
				if (row == 1)
					S.TSwitch[2] += 1;
				else if (row == -1)
					S.TSwitch[2] -= 1;

				if (S.TSwitch[2] < 0)
					S.TSwitch[2] = 4;
				if (S.TSwitch[2] > 4)
					S.TSwitch[2] = 0;
			}
			nx++;
			// 受け身
			if (SPoint == nx){
				if (row == 1)
					S.TSwitch[3] += 1;
				else if (row == -1)
					S.TSwitch[3] -= 1;

				if (S.TSwitch[3] < 0)
					S.TSwitch[3] = 3;
				if (S.TSwitch[3] > 3)
					S.TSwitch[3] = 0;
			}
			nx++;
			// ガード
			if (SPoint == nx){
				if (row == 1)
					S.TSwitch[4] += 1;
				else if (row == -1)
					S.TSwitch[4] -= 1;

				if (S.TSwitch[4] < 0)
					S.TSwitch[4] = 2;
				if (S.TSwitch[4] > 2)
					S.TSwitch[4] = 0;
			}
			nx++;
			// カウンター
			if (SPoint == nx){
				if (row == 1)
					S.T_Counter += 1;
				else if (row == -1)
					S.T_Counter -= 1;

				if (S.T_Counter < 0)
					S.T_Counter = 1;
				if (S.T_Counter > 1)
					S.T_Counter = 0;
			}
			nx++;
			// 投げ抜け
			if (SPoint == nx){
				if (row == 1)
					S.T_ThrowReject += 1;
				else if (row == -1)
					S.T_ThrowReject -= 1;

				if (S.T_ThrowReject < 0)
					S.T_ThrowReject = 1;
				if (S.T_ThrowReject > 1)
					S.T_ThrowReject = 0;
			}
			nx++;
			// 項目表示
			if (SPoint == nx){
				if (row == 1)
					S.TSwitch[5] += 1;
				else if (row == -1)
					S.TSwitch[5] -= 1;

				if (S.TSwitch[5] < 0)
					S.TSwitch[5] = 3;
				if (S.TSwitch[5] > 3)
					S.TSwitch[5] = 0;
			}
			nx++;
			// 判定
			if (SPoint == nx){
				if (row == 1)
					S.TSwitch[6] += 1;
				else if (row == -1)
					S.TSwitch[6] -= 1;

				if (S.TSwitch[6] < 0)
					S.TSwitch[6] = 3;
				if (S.TSwitch[6] > 3)
					S.TSwitch[6] = 0;
			}
			nx++;
			// 再生方法
			if (SPoint == nx){
				if (row == 1)
					S.TSwitch[8] += 1;
				else if (row == -1)
					S.TSwitch[8] -= 1;

				if (S.TSwitch[8] < 0)
					S.TSwitch[8] = 2;
				if (S.TSwitch[8] > 2)
					S.TSwitch[8] = 0;
			}
		}


	}
	
	/**************
	* 送信
	**************/
	{
		Get_SSet(S);
	}
}

void TrainingList()
{
	/**************
	* 描画
	**************/
	
		nx = 0 + drawPos;

		nx++;
		
		if (drawPos >= 0) {
			// ずらす
			DrawString(30, TxtDist * nx, "体力", Cr);
			if (S.T_Life == 0)DrawString(160, TxtDist * nx, "自動回復", Oran);
			else { DrawString(160, TxtDist * nx, "通常", Cr); }
			
		}
		
		nx++;
		
		if (drawPos >= -1) {
			DrawString(30, TxtDist * nx, "パワーゲージ", Cr);
			if (S.T_Power == 0)DrawString(160, TxtDist * nx, "自動回復", Oran);
			else if (S.T_Power == 1) { DrawString(160, TxtDist * nx, "1ゲージ", Cr); }
			else if (S.T_Power == 2) { DrawString(160, TxtDist * nx, "2ゲージ", Cr); }
			else if (S.T_Power == 3) { DrawString(160, TxtDist * nx, "通常", Cr); }
		}

		nx++;

		DrawString(30, TxtDist * nx, "ガードゲージ", Cr);
		if (S.T_GGauge == 0)DrawString(160, TxtDist * nx, "自動回復", Oran);
		else if (S.T_GGauge == 1){ DrawString(160, TxtDist * nx, "通常", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "アドゲージ", Cr);
		if (S.T_AddGauge == 0)DrawString(160, TxtDist * nx, "通常", Oran);
		else if (S.T_AddGauge == 1){ DrawString(160, TxtDist * nx, "MAX", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "体勢", Cr);
		if (S.TSwitch[2] == 0)DrawString(160, TxtDist * nx, "立ち", Oran);
		else if (S.TSwitch[2] == 1)DrawString(160, TxtDist * nx, "しゃがみ", Cr);
		else if (S.TSwitch[2] == 2)DrawString(160, TxtDist * nx, "ジャンプ", Cr);
		else if (S.TSwitch[2] == 3){ DrawString(160, TxtDist * nx, "プレイヤー操作", Cr); }
		else { DrawString(160, TxtDist * nx, "コンピュータ操作", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "受け身", Cr);
		if (S.TSwitch[3] == 0)DrawString(160, TxtDist * nx, "ニュートラル", Oran);
		else if (S.TSwitch[3] == 1) { DrawString(160, TxtDist * nx, "前方", Cr); }
		else if (S.TSwitch[3] == 2) { DrawString(160, TxtDist * nx, "後方", Cr); }
		else if (S.TSwitch[3] == 3) { DrawString(160, TxtDist * nx, "なし", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "ガード", Cr);
		if (S.TSwitch[4] == 0)DrawString(160, TxtDist * nx, "しない", Oran);
		else if (S.TSwitch[4] == 1) { DrawString(160, TxtDist * nx, "する", Cr); }
		else if (S.TSwitch[4] == 2) { DrawString(160, TxtDist * nx, "ランダム", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "カウンター", Cr);
		if (S.T_Counter == 0)DrawString(160, TxtDist * nx, "通常", Oran);
		else if (S.T_Counter == 1) { DrawString(160, TxtDist * nx, "する", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "投げ抜け", Cr);
		if (S.T_ThrowReject == 0)DrawString(160, TxtDist * nx, "しない", Oran);
		else if (S.T_ThrowReject == 1) { DrawString(160, TxtDist * nx, "する", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "設定表示", Cr);
		if (S.TSwitch[5] == 0)DrawString(160, TxtDist * nx, "全て", Oran);
		else if (S.TSwitch[5] == 1) { DrawString(160, TxtDist * nx, "数値", Cr); }
		else if (S.TSwitch[5] == 2) { DrawString(160, TxtDist * nx, "キー", Cr); }
		else if (S.TSwitch[5] == 3) { DrawString(160, TxtDist * nx, "しない", Cr); }

		nx++;
		DrawString(30, TxtDist * nx, "判定", Cr);
		if (S.TSwitch[6] == 0)DrawString(160, TxtDist * nx, "なし", Oran);
		else if (S.TSwitch[6] == 1) { DrawString(160, TxtDist * nx, "喰らい", Cr); }
		else if (S.TSwitch[6] == 2) { DrawString(160, TxtDist * nx, "攻撃", Cr); }
		else if (S.TSwitch[6] == 3) { DrawString(160, TxtDist * nx, "両方", Cr); }

		nx++;
		DrawString(30, TxtDist * nx, "再生方法", Cr);
		if (S.TSwitch[8] == 0)DrawString(160, TxtDist * nx, "通常", Oran);
		else if (S.TSwitch[8] == 1) { DrawString(160, TxtDist * nx, "連続", Cr); }
		else if (S.TSwitch[8] == 2) { DrawString(160, TxtDist * nx, "リバサ", Cr); }
		
}

void CommandList()
{
	/**************
	* 背景
	**************/
	{
		BackGraphT();
	}

	if (!S.TraningFlg){
		if (P_BInput(2) == 1)ComList = 0;
	}
	else{
		if (P1_BInput(2) == 1)ComList = 0;
	}
	


	// 描画
	int tx = 29;
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){

		if (i == 1)tx += SCREEN_W/2;

		if (P[i].Name == EIN)
		{
			DrawString(tx, TxtDist * 1, "→ +Ｃ", Cr);
			DrawString(tx, TxtDist * 2, "Ｄ（設置後Ｄ 空中可）", Cr);
			DrawString(tx, TxtDist * 3, "↓＼→ +ＡorＢorＣ（空中可）", Cr);
			DrawString(tx, TxtDist * 4, "↓／← +ＡorＢ", Cr);
			DrawString(tx - 19, TxtDist * 5, "ゲージ技", Cr);
			DrawString(tx, TxtDist * 6, "↓＼→ +Ｄ（ 0.5 ）", Cr);
			DrawString(tx, TxtDist * 7, "↓／← +Ｃ（ 1 空中可 ）", Cr);
			DrawString(tx, TxtDist * 8, "↓↓ +Ｄ（ 1 設置中 ）", Cr);
			DrawString(tx, TxtDist * 9, "Ａ+Ｂ+Ｃ（ 3 ）", Cr);
		}
		else if (P[i].Name == CORNEL)
		{
			DrawString(tx, TxtDist * 1, "→ +Ｂ", Cr);
			DrawString(tx, TxtDist * 2, "＼ +Ｃ", Cr);
			DrawString(tx, TxtDist * 3, "Ｄ（ヒット後Ｄ 空中可）", Cr);	
			DrawString(tx, TxtDist * 4, "↓＼→ +ＡorＢ", Cr);
			DrawString(tx, TxtDist * 5, "→↓＼ +ＡorＢ", Cr);
			DrawString(tx, TxtDist * 6, "↓／← +ＡorＢ", Cr);
			DrawString(tx - 19, TxtDist * 7, "ゲージ技", Cr);
			DrawString(tx, TxtDist * 8, "↓／← +Ｄ（ 0.5 ）", Cr);
			DrawString(tx, TxtDist * 9, "↓＼→ +Ｃ（ 1 ）", Cr);
			DrawString(tx, TxtDist * 10, "→↓＼ +Ｃ（ 1 空中可 ）", Cr);
			DrawString(tx, TxtDist * 11, "↓／← +Ｃ（ 2 ）", Cr);
			DrawString(tx, TxtDist * 12, "Ａ+Ｂ+Ｃ（ 3 ）", Cr);
		}
		else if (P[i].Name == BOUNCER)
		{
			DrawString(tx, TxtDist * 1, "→ +Ｂ", Cr);
			DrawString(tx, TxtDist * 2, "→ +Ｃ（ホールド可）", Cr);
			DrawString(tx, TxtDist * 3, "Ｄ", Cr);
			DrawString(tx, TxtDist * 4, "↓＼→ +ＡorＢ", Cr);
			DrawString(tx, TxtDist * 5, "↓／← +ＡorＢ", Cr);
			DrawString(tx - 19, TxtDist * 6, "ゲージ技", Cr);
			DrawString(tx, TxtDist * 7, "↓＼→ +Ｃ（ 0.5 ）", Cr);
			DrawString(tx, TxtDist * 8, "↓／← +Ｃ（ 0.5 ）", Cr);
			DrawString(tx, TxtDist * 9, "Ａ+Ｂ+Ｃ（ 1 ）", Cr);
			DrawString(tx, TxtDist * 10, "↓↓ +Ｄ（ 3 ）", Cr);
		}
		else if (P[i].Name == HELIOS){
			DrawString(tx, TxtDist * 1, "→ +Ａ", Cr);
			DrawString(tx, TxtDist * 2, "→ +Ｂ", Cr);
			DrawString(tx, TxtDist * 3, "Ｄ（ホールド可）", Cr);		
			DrawString(tx, TxtDist * 4, "↓＼→ +ＡorＢorＤ", Cr);
			DrawString(tx, TxtDist * 5, "→↓＼ +ＡorＢorＤ", Cr);
			DrawString(tx, TxtDist * 6, "↓／← +ＡorＢorＤ（空中可）", Cr);
			DrawString(tx - 19, TxtDist * 7, "ゲージ技", Cr);
			DrawString(tx, TxtDist * 8, "↓＼→ +Ｃ（ 1 ）", Cr);
			DrawString(tx, TxtDist * 9, "↓／← +Ｃ（ 1 ）", Cr);
			DrawString(tx, TxtDist * 10, "↓↓ +Ｄ（ 1 ）", Cr);
			DrawString(tx, TxtDist * 11, "Ａ+Ｂ+Ｃ（ 3 ）", Cr);
		}
		else if (P[i].Name == HYDE){
			
		}
		else if (P[i].Name == SYUICHI) {
			DrawString(tx, TxtDist * 1, "→ +Ａ", Cr);
			DrawString(tx, TxtDist * 2, "→ +Ｂ", Cr);
			DrawString(tx, TxtDist * 3, "→ +Ｃ", Cr);
			DrawString(tx, TxtDist * 4, "＼ +Ｃ", Cr);
			DrawString(tx, TxtDist * 5, "Ｄ", Cr);
			DrawString(tx, TxtDist * 6, "↓＼→ +ＡorＢ", Cr);
			DrawString(tx, TxtDist * 7, "↓／← +ＡorＢ", Cr);
			DrawString(tx - 19, TxtDist * 8, "ゲージ技", Cr);
			DrawString(tx, TxtDist * 9, "↓＼→ +Ｃ（ 1 ）", Cr);
		}
	}
	
	
	//境界線
	DrawLine(SCREEN_W / 2 - 1, 20, SCREEN_W / 2 - 1, SCREEN_H - 20, Cr);

	tx = 29 + (SCREEN_W/2);
}


// トレモ用背景
void BackGraphT()
{
	// 描画ブレンドモードをアルファブレンド（透明）にする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
	// 半透明に
	DrawBox(0, 20, SCREEN_W, SCREEN_H - 20, GetColor(0, 0, 0), true);
	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 上下線
	DrawLine(0, 19, SCREEN_W, 19, Cr);
	DrawLine(0, SCREEN_H - 19, SCREEN_W, SCREEN_H - 19, Cr);

}

void BackGraph()
{
	// 描画ブレンドモードをアルファブレンド（透明）にする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
	// 半透明に
	DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void raw_set()
{
	noChangeRow = true;
}

void Load_Pause(){
	if (Load_1 == 0){

		// 画像読み込み
		//int cursor = LoadGraph("ob/cursor.png");
		LoadDivGraph("ob/key.png", 10, 5, 2, 20, 20, gK);
		LoadDivGraph("ob/button.png", 4, 4, 1, 20, 20, gB);
		int gP = LoadGraph("ob/fontP.png");

		//Load_1 = 1;
	}
}