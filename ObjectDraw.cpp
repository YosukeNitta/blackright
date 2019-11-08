//インクルード部--------------------------------------------
#include "pch.h"

#define LOG_MAX		(10)

using namespace std;

// アナウンス
static double m_size;	// 画像の拡大

static int PowerBar[2];		// ライフバー
static int PowerPoint;	// 画面に表示する体力
static int PowerNum[10];// 画像 [パワーゲージ 0～9]

static int Hit;			// 画像「HIT」
static int HitNum[10];	// 画像「ヒット数 0～9」
static int HitGraph, HitGraph2;	// ヒット数の実際の画像(2は 2ケタ目)
static boolean beat[2];	// 連続ヒットか(falseで青ビ)

static int hitCount[2];		// ヒットorガード数
static int hDrawTime[2];	// 表示する時間
static int hHitCount[2];	// 直前のヒット数

static int Win1, Win2;	// プレイヤーWIN の表示
static int battle[5];	// 画像　FIGHT
static int EX;

static int Load1;
static int TText[5];
static char TChar[TX_MAX];

//ダメージ計算
static int m_AllDamage;
static int m_DisplayDamage;
//喰らう前の体力
static int m_P1_SLife, m_P2_SLife;
//喰らった後の体力
static int m_P1_ALife, m_P2_ALife;

// ゲージ回収
static int mGauge = 0;
static int mG_startPower = 0;
static int mG_Start = 0;
//発生フレーム
static int mHassei = 0;
static int aSpeed;	// 攻撃フレーム
// フレーム、開始フレーム？
static int mFrame = 0;
static int mF_Start = 0;
//
static float m_cPos;


// キーログ表示
static int mKey[9];
static int mButton[4];
static int gKey[10];
static int gButton[4];

//static int oKey[LOG_MAX];
static vector<int> oKey;
//static int oButton[LOG_MAX][4];
static vector<int> oButton[LOG_MAX];

static boolean sButton, sKey;
static int saveKey = -1;
static int test, test2[2];	// 特殊ゲージ
static int test3[2];
// Add
static int addGaugeTime[2];		// 表示時間
static int addGauge[10];		// 画像
static int addGaugePower[2];	// ゲージ量
/*
* これを使って数値を全て渡す
*/
static int mXA[2][HIT_MAX], mYA[2][HIT_MAX], mWA[2][HIT_MAX], mHA[2][HIT_MAX];
static int mHXA[2][HM], mHYA[2][HM], mHWA[2][HM], mHHA[2][HM];

// 喰らい判定
static int m_XD[HIT_MAX], m_YD[HIT_MAX], m_WD[HIT_MAX], m_HD[HIT_MAX];
static int mHXH[2][HM], mHYH[2][HM], mHWH[2][HM], mHHH[2][HM];

// キーログ表示
static int m_KeyPos, m_Key;
static int m_Button[4];

// 画面の明るさ
static int blackOut;

void BoxCheck();	// 判定チェック
void BoxKurai();	// 喰らい判定表示
void BoXAtt();		// 攻撃判定表示


class Test {

public:
	int x, y;	// 画像サイズ

	Test::Test()
	{
		x = 11;
		y = 22;
	}
};

static Test tes;

void ObjectDraw_KeyDisplay();

//その他の描画
void ObjectDraw()
{

	// 画像
	if (Load1 == 0){
		//パワーバーの画像
		LoadDivGraph("ob/PowerBar.png", 2, 1, 2, 241, 36, PowerBar);
		PowerPoint = LoadGraph("ob/PowerPoint.bmp");	// ゲージのドット
		LoadDivGraph("ob/number.png", 10, 5, 2, 25, 30, PowerNum);	// ゲージ本数の数字

		Hit = LoadGraph("ob/hit.png");	// Hit の文字
		LoadDivGraph("ob/hitnum.png", 10, 5, 2, 36, 56, HitNum);	// ヒット数

		LoadDivGraph("ob/key.png", 10, 5, 2, 20, 20, gKey);
		LoadDivGraph("ob/button.png", 4, 4, 1, 20, 20, gButton);

		Win1 = LoadGraph("ob/1P.png");	// 1P
		Win2 = LoadGraph("ob/2P.png");	// 2P

		test = LoadGraph("char/bouncer/o1.bmp");
		test2[0] = LoadGraph("char/helios/o1.bmp");
		test2[1] = LoadGraph("char/helios/o2.bmp");
		test3[0] = LoadGraph("char/dan/gauge.bmp");
		test3[1] = LoadGraph("char/dan//gauge2.bmp");
		EX = LoadGraph("ob/EX.png");

		LoadDivGraph("ob/battle.png", 5, 1, 5, 280, 70, battle);

		// キー画面
		m_Key = LoadGraph("ob/r_Key.png");
		m_KeyPos = LoadGraph("ob/r_KeyPos.png");
		LoadDivGraph("ob/r_Button.png", 4, 4, 1, 20, 20, m_Button);

		LoadDivGraph("ob/num2.png", 10, 5, 2, 12, 20, addGauge);

		// 生成
		//
		for (int i = 0; i < LOG_MAX; i++){
			oKey.push_back(-1);
		}

		for (int a = 0; a < LOG_MAX; a++){
			for (int i = 0; i < 4; i++){
				oButton[a].push_back(0);
			}
		}
		blackOut = 0;

		Load1 = 1;
	}
		
		/**************
		* ヒット数
		**************/
		Player P[2];
		int hDrawPos[2];
		hDrawPos[0] = 5;
		hDrawPos[1] = (SCREEN_W - 640) + 320 + 122;
		hitCount[0] = 0, hitCount[1] = 0;
		P[0] = P1, P[1] = P2;


		for (int i = 0; i < 2; i++) {
			// 数字入れ替え
			if (i == 1) {
				P[0] = P2, P[1] = P1;
			}

			// ヒット数
			if (P[0].HitCount > 1) {
				HitGraph = HitNum[P[0].HitCount % 10];
				HitGraph2 = HitNum[P[0].HitCount / 10];
				hitCount[0] = P[0].HitCount;
			}
			// ガード数
			else if (P[0].A.guardCount > 1) {
				HitGraph = HitNum[P[0].A.guardCount % 10];
				HitGraph2 = HitNum[P[0].A.guardCount / 10];
				hitCount[0] = P[0].A.guardCount;
			}

			// ガード数

			// 1P側 ヒット数表示
			// 喰らいor連続ガード
			if ((P[1].HFlg) || (P[0].A.guardCount > 1)) {
				// 2ヒット以上で、 1ケタ目表示
				if (hitCount[0] >= 2) {
					// 99ヒット以内
					if (hitCount[0] <= 99) {
						// ヒット数がないなら
						if (P[0].HitCount == 0) {
							// ガード数表示へ
							SetDrawBright(128, 128, 128);
						}
						DrawGraph(hDrawPos[i] + 80, 90 + 5, Hit, true);

						//青ビの場合
						if (!beat[i]) {
							SetDrawBright(255, 30, 30);
						}
						else { SetDrawBright(30, 30, 255); }
						
						DrawGraph(hDrawPos[i] + 39, 90, HitGraph, true);
						
						// ヒット数が 2ケタ以上の時は、2ケタ目表示
						if (hitCount[0] >= 10) {
							DrawGraph(hDrawPos[i] + 8, 90, HitGraph2, true);
						}

						SetDrawBright(255, 255, 255);	// 色リセット
					}
					// 99ヒットでカンスト
					else if (hitCount[0] >= 100) {
						DrawGraph(hDrawPos[i], 90 + 5, Hit, true);
						DrawGraph(hDrawPos[i] + 39, 90, HitNum[9], true);
						DrawGraph(hDrawPos[i] + 8, 90, HitNum[9], true);
					}
				}
			}
		}


		/**************
		* 特殊ゲージ
		**************/
		if (!(S.roundState == 0 && S.roundTime == 0) && (S.noGauge == 0)){
			/*
			// EX
			if (P1.Var[30] != 0)SetAlpha(68);
			DrawGraph(0 + 2 + 30, 62 + 2, EX, true);
			NoBlend();
			SetDrawBright(255, 255, 255);

			if (P2.Var[30] != 0)SetAlpha(68);
			DrawGraph(SCREEN_W - 2 - 24 - 30, 62 + 2, EX, true);
			NoBlend();
			SetDrawBright(255, 255, 255);
			*/

			// バウンサー
			if (P1.Name == BOUNCER){
				if (P1.D.armor <= 0)SetAlpha(68);
				if (P1.D.armor > 2)SetDrawBright(255, 10, 10);
				else if (P1.D.armor > 1)SetDrawBright(255, 128, 128);

				DrawGraph(0 + 2, 62 + 2, test, true);
				NoBlend();
				SetDrawBright(255, 255, 255);
				if (P1.Var[13] > 0){
					DrawFormatString(80, (SCREEN_H - 480) + 430, Oran, "%d", P1.Var[13] / 60);
				}
			}
			if (P2.Name == BOUNCER){
				if (P2.D.armor <= 0)SetAlpha(68);
				if (P2.D.armor > 2)SetDrawBright(255, 10, 10);
				else if (P2.D.armor > 1)SetDrawBright(255, 128, 128);

				DrawGraph(SCREEN_W - 2 - 26, 62 + 2, test, true);
				NoBlend();
				SetDrawBright(255, 255, 255);
				if (P2.Var[13] > 0){
					DrawFormatString(SCREEN_W - 80 - 10, (SCREEN_H - 480) + 430, Oran, "%d", P2.Var[13] / 60);
				}
			}

			// ヘリオス
			if (P1.Name == HELIOS){
				if (P1.Var[11] >= 3000){
					DrawFormatString(80, (SCREEN_H - 480) + 430, Oran, "%d", P1.Var[11] / 1000);
				}
				else if (P1.Var[11] >= 2000){
					DrawFormatString(80, (SCREEN_H - 480) + 430, GetColor(255, 255, 0), "%d", P1.Var[11] / 1000);
				}
				else if(P1.Var[11] >= 1000){
					DrawFormatString(80, (SCREEN_H - 480) + 430, Cr, "%d", P1.Var[11] / 1000);
				}
				else{
					DrawFormatString(80, (SCREEN_H - 480) + 430, GetColor(128, 128, 128), "%d", P1.Var[11] / 1000);
				}

				// ゲージ
				DrawBox(100, (SCREEN_H - 480) + 435, 100 + (P1.Var[11] * 0.05), (SCREEN_H - 480) + 445, GetColor(255, 215, 0), true);
				// ゲージ枠
				for (int i = 1; i < 4; i++){
					DrawBox(100 + (((i - 1) * 1000) * 0.05), (SCREEN_H - 480) + 435, 
						100 + ((i * 1000) * 0.05), (SCREEN_H - 480) + 445, GetColor(20, 20, 20), false);
				}
			}
			if (P2.Name == HELIOS){

				if (P2.Var[11] >= 3000){
					DrawFormatString(SCREEN_W - 80 - 10, (SCREEN_H - 480) + 430, Oran, "%d", P2.Var[11] / 1000);
				}
				else if (P2.Var[11] >= 2000){
					DrawFormatString(SCREEN_W - 80 - 10, (SCREEN_H - 480) + 430, GetColor(255, 255, 0), "%d", P2.Var[11] / 1000);
				}
				else if (P2.Var[11] >= 1000){
					DrawFormatString(SCREEN_W - 80 - 10, (SCREEN_H - 480) + 430, Cr, "%d", P2.Var[11] / 1000);
				}
				else{
					DrawFormatString(SCREEN_W - 80 - 10, (SCREEN_H - 480) + 430, GetColor(128, 128, 128), "%d", P2.Var[11] / 1000);
				}
				DrawBox(SCREEN_W - 80 - 20 - (P2.Var[11] * 0.05), (SCREEN_H - 480) + 435, 
					SCREEN_W - 80 - 20, (SCREEN_H - 480) + 445, GetColor(255, 215, 0), true);
				for (int i = 1; i < 4; i++){
					DrawBox(SCREEN_W - 80 - 20 - ((i * 1000) * 0.05), (SCREEN_H - 480) + 435,
						SCREEN_W - 80 - 20 - (((i - 1) * 1000) * 0.05), (SCREEN_H - 480) + 445, GetColor(0, 0, 0), false);
				}
			}

			// ダン
			if (P1.Name == HYDE) {
				// ゲージ枠
				DrawBox(99, (SCREEN_H - 480) + 434,
					99 + 101, (SCREEN_H - 480) + 456, GetColor(20, 20, 20), false);
				// ゲージ
				DrawBox(100, (SCREEN_H - 480) + 435, 100 + (P1.Var[11] * 0.05), (SCREEN_H - 480) + 455, GetColor(255, 215, 0), true);
			}
			if (P2.Name == HYDE) {
				DrawBox(SCREEN_W - 80 - 19, (SCREEN_H - 480) + 434,
					SCREEN_W - 80 - 19 - 101, (SCREEN_H - 480) + 456, GetColor(0, 0, 0), false);
				DrawBox(SCREEN_W - 80 - 20 - (P2.Var[11] * 0.05), (SCREEN_H - 480) + 435,
					SCREEN_W - 80 - 20, (SCREEN_H - 480) + 455, GetColor(255, 215, 0), true);
			}
		}
		/**************
		* 判定確認 （トレモ限定）
		**************/
		if (S.TraningFlg){
			BoxCheck();
		}

		//--------------
		// 合計ダメージ
		//--------------
		if (P2.HFlg == 0){
			m_P2_SLife = P2.Life;
			m_AllDamage = 0;
		}
		if (P2.HFlg){
			m_AllDamage = m_P2_SLife - P2.Life;
			m_DisplayDamage = m_AllDamage;
		}

		/**************
		* パワーゲージ
		**************/
		if (!(S.roundState == 0 && S.roundTime == 0) && (S.noGauge == 0)){
			//--------------
			// 半透明ゲージ
			//--------------
			// 画像サイズ取得 P1,2
			int bar_x, bar_y;
			GetGraphSize(PowerBar[0], &bar_x, &bar_y);
			
			//--------------
			// 半透明ゲージ
			//--------------

			SetAlpha(158);
			
			// パワーバー下　透明表示
			DrawGraph(20, 435 + (SCREEN_H - 480), PowerBar[1], true);
			DrawTurnGraph(SCREEN_W - 20 - bar_x, 435 + (SCREEN_H - 480), PowerBar[1], true);

			// 半透明解除
			NoBlend();

			// ゲージ1000以上
			{
				SetDrawBright(128,100,0);
				//DrawGraph(20, 435, PowerBar[1], true);

				if (P1.Power >= 1000 && P1.Power < 3000){
					DrawRotaGraph3F(320 - (1000 / 5) - 62, 454 + (SCREEN_H - 480),
						0, 0, (1000 / 5), 1, 0, PowerPoint, true, false);
				}
				if (P2.Power >= 1000 && P2.Power < 3000){
					DrawRotaGraph3F(SCREEN_W - 320 + 62, 454 + (SCREEN_H - 480),
						0, 0, (1000 / 5), 1, 0, PowerPoint, true, false);
				}

				SetDrawBright(255, 255, 255);
			}


			//パワー量　1P
			//for (int i = 0; i < P1.Power % 1000; i++){
				//DrawGraph((SCREEN_W / 2) - 62 - (i / 5), 454, PowerPoint, true);
			//}
			
			// ゲージMAX時
			SetDrawBright(255, 180, 0);

			if (P1.Power == POWER_MAX){
				DrawRotaGraph3F(320 - (1000 / 5) - 62, 454 + (SCREEN_H - 480),
					0, 0, (1000 / 5), 1, 0, PowerPoint, true, false);
			}
			else{
				DrawRotaGraph3F((float)(320 - ((P1.Power % 1000) / 5) - 62), 454 + (SCREEN_H - 480),
					0, 0, ((P1.Power % 1000) / 5), 1, 0, PowerPoint, true, false);
			}
			SetDrawBright(255, 255, 255);

			// ゲージ本数
			DrawGraph(25 + 1, 440 + (SCREEN_H - 480) - 1, PowerNum[P1.Power / 1000], true);

			//パワー量　2P
			// ゲージMAX時
			SetDrawBright(255, 180, 0);

			if (P2.Power == POWER_MAX){
				DrawRotaGraph3F((SCREEN_W - 640) + 320 + 62, 454 + (SCREEN_H - 480),
					0, 0, (1000 / 5), 1, 0, PowerPoint, true, false);
			}
			else{
				DrawRotaGraph3F((SCREEN_W - 640) + 320 + 62, 454 + (SCREEN_H - 480),
					0, 0, ((P2.Power % 1000) / 5), 1, 0, PowerPoint, true, false);
			}
			SetDrawBright(255, 255, 255);

			// ゲージ本数
			DrawGraph(SCREEN_W - 50, 440 + (SCREEN_H - 480) - 1, PowerNum[P2.Power / 1000], true);

			///////////////////
			// パワーバー表示
			DrawGraph(20, 435 + (SCREEN_H - 480), PowerBar[0], true);
			DrawTurnGraph(SCREEN_W - 20 - bar_x, 435 + (SCREEN_H - 480), PowerBar[0], true);

		}
		
		// デバッグ
		{
			/*
			// デバッグ用
			// 薄い四角
			SetAlpha(128);
			DrawBox(498, SCREEN_H - 42, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
			NoBlend();

			DrawFormatString(500, SCREEN_H - 40, Cr, "ctrl..%d,%d", P1.ctrl, P2.ctrl);
			DrawFormatString(500, SCREEN_H - 20, Cr, "sflg..%d,%d", P1.SFlg, P2.SFlg);
			*/
		}

		/**************
		* エフェクト
		**************/
		{
			EffDraw();
		}

		// リプレイキー表示
		if (Replay_Mode(-1) == 1)
			ObjectDraw_KeyDisplay();

		/**************
		* アナウンス
		**************/
		{
			// 開幕
			if (S.roundState == 0){

				// 少し黒くする
				if (S.roundTime > 0){
					SetAlpha(98);
					if (S.roundTime <= 110){
						DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
					}
					else if (S.roundTime > 110){
						DrawBox(0, 0, SCREEN_W, ((121 - S.roundTime) * 32), GetColor(0, 0, 0), true);
						DrawBox(0, SCREEN_H - ((121 - S.roundTime) * 32), SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
					}
					SetAlpha(255);
					
				}
				// SE
				if(S.roundTime == 1)SEStart(34);
				// SE
				if (S.roundTime == 71)SEStart(34);

				// 立ちになったら
				if (S.roundTime >= 1 && S.roundTime <= 10)m_size = (11 - S.roundTime) / 2 + 0.5;
				else if (S.roundTime >= 70 && S.roundTime <= 80)m_size = (81 - S.roundTime) / 2 + 0.5;
				else{ m_size = 1; }
				if (m_size < 0)m_size = 1;

				if (S.roundTime >= 50 && S.roundTime < 70)SetAlpha(255 - (S.roundTime - 50) * 12.8);
				if (S.roundTime >= 1 && S.roundTime < 70){
					DrawRotaGraph(260, 240, m_size, 0, battle[1], true);
					DrawRotaGraph(400, 240, m_size, 0, battle[0], true);
				}


				if(S.roundTime >= 100)SetAlpha(255 - (S.roundTime - 100) * 12.8);
				if (S.roundTime >= 70)DrawRotaGraph(340, 240, m_size, 0, battle[2], true);


				NoBlend();
			}
			// バトル中
			else if (S.roundState == 1){
				//
				/*
				for (int i = 0; i < 2; i++){
					if (addGaugeTime[i] > 0){
						if (addGaugeTime[i] == 60)SetAlpha(128);
						else if (addGaugeTime[i] == 1)SetAlpha(128);
						DrawRotaGraph(80 + (i * (SCREEN_W - 92 - 80)), 120, 1, 0, addGauge[0], true);
						SetAlpha(255);
						addGaugeTime[i] -= 1;
					}
				}
				*/
			}
			// ラウンド取得
			else if (S.roundState == 2){
				
				// SE
				if (S.roundTime == 2)SEStart(34);

				// 少し黒くする
				if (S.roundTime > 0){
					SetAlpha(98);
					if (S.roundTime <= 40){
						DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
					}
					else if (S.roundTime > 40){
						DrawBox(0, 0, SCREEN_W, ((51 - S.roundTime) * 32), GetColor(0, 0, 0), true);
						DrawBox(0, SCREEN_H - ((51 - S.roundTime) * 32), SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
					}
					SetAlpha(255);
				}

				// DOWN 描画
				if (S.roundTime <= 1 && S.StopTime > 0){
					//SetAlpha(255 - (S.StopTime * 12.8));
					
					/*
					if (S.StopTime > 1){
						m_size = S.StopTime;
					}
					else{ m_size = 1; }
					*/
					DrawRotaGraph(360, 240, 1, 0, battle[3], true);
				}

				if (S.roundTime >= 1 && S.roundTime <= 10)m_size = (11 - S.roundTime) / 2 + 0.5;
				else{ m_size = 1; }
				if (m_size < 0)m_size = 1;

				if (S.roundTime >= 30)SetAlpha(255 - (S.roundTime - 30) * 12.8);
				// 立ちになったら
				if (S.roundTime > 0)DrawRotaGraph(340, 240, m_size,0,battle[2], true);

				NoBlend();
			}
			

		}
		//-----------------
		// フェードアウト
		//-----------------
		{
			// 勝利時のホワイトアウト
			if (S.roundState == 3){
				int add;
				add = S.roundTime * 4;
				SetAlpha(add);
				DrawBox(0, 0, SCREEN_W, SCREEN_H, Cr, true);
				NoBlend();
			}

			// ブラックアウト
			if (blackOut != 0){
				SetAlpha(blackOut);
				DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0,0,0), true);
				NoBlend();
				blackOut = 0;
			}
		}

}


// ヒットボックス以外も調べます☆
void BoxCheck()
{
	// 描画、表示オンの時のみ
	if (S.TSwitch[5] == 0 || S.TSwitch[5] == 1){

		// 薄い四角
		SetAlpha(128);
		DrawBox(498, 58, 620, 160, GetColor(0, 0, 0), true);
		NoBlend();

		// その他のチェック
		DrawFormatString(500, 60, Cr, "ダメージ %d", m_DisplayDamage);
		// ゲージ回収
		DrawFormatString(500, 80, Cr, "ゲージ %d", mGauge);

		// フレームチェック・開始
		if ((mG_Start == 0) && (P2.ctrl == 0) &&
			((P2.HFlg == 1) || (P2.stateno >= 50 && P2.stateno <= 59))){
			mG_Start = 1, mGauge = 0;
		}
		// ゲージチェック
		if (mG_Start){
			mGauge = P1.Power - mG_startPower;
		}

		// ゲージ回収・終了
		if ((mG_Start) && (P1.ctrl && P2.ctrl)){
			mG_Start = 0;
		}
		else if (!mG_Start)mG_startPower = P1.Power;



		// 発生フレーム
		DrawFormatString(500, 100, Cr, "発生 %d", mHassei);
		if ((P1.MoveHit == 0) 
			&& 
			(
			(P2.stateno == 1000) || (P2.stateno == 1010) || (P2.stateno == 1020) || (P2.stateno == 1030) 
			|| (P2.stateno == 1060) || (P2.stateno == 1065)
			|| (P2.stateno == 50) || (P2.stateno == 51) || (P2.stateno == 52)
			) 
			&& (P2.time == 0))
			mHassei = aSpeed + 1;	// 発生記憶

		// フレームチェック
		DrawFormatString(500, 120, Cr, "硬直差 %d", mFrame);

		// フレームチェック・開始
		if ((mF_Start == 0) && (P2.ctrl == 0) && 
			((P2.HFlg == 1) || (P2.stateno >= 50 && P2.stateno <= 59))){
			mF_Start = 1, mFrame = 0;
		}

		// 時間チェック
		if (mF_Start){
			if ((P1.ctrl) && (!P2.ctrl)){
				mFrame += 1;
			}
			else if ((!P1.ctrl) && (P2.ctrl)){
				mFrame -= 1;
			}
		}

		// フレームチェック・終了
		if ((mF_Start) && (P1.ctrl && P2.ctrl)){
			mF_Start = 0;
		}

		//DrawFormatString(500, 140, Cr, "補正 %.2f", P1.A.hosei_K);
		if (S.TSwitch[2] == 3)DrawFormatString(500, 140, Cr, "プレイヤー操作");
		else if (S.TSwitch[2] == 4)DrawFormatString(500, 140, Cr, "コンピュータ");
		else if (S.TSwitch[7] == 0)DrawFormatString(500, 140, Cr, "通常");
		else if (S.TSwitch[7] == 1)DrawFormatString(500, 140, Cr, "録画準備");
		else if (S.TSwitch[7] == 2){
			DrawString(500, 140, "録画中", GetColor(255,255,0));
			DrawFormatString(560, 140, Cr, "%d", TText[0]);
		}
		else if (S.TSwitch[7] == 3)DrawFormatString(500, 140, Cr, "再生中 %d", TText[0]);
		else if (S.TSwitch[7] > 3)DrawFormatString(500, 140, Cr, "何かおかしい");

		// ヘルパー
		//DrawFormatString(500, 160, Cr, "W.%d time.%d", H1[18].WAtt[1], H1[18].time);
		
		//DrawFormatString(500, 160, Cr, "%.2f", P1.XVel);
		//DrawFormatString(500, 160, Cr, "振り向き %d", P1.muki);
		
		// ガークラ
		//DrawFormatString(500, 160, Cr, "ガード %d", TText[1]);

		/*
		DrawFormatString(500, 160, Cr, "%.2f", P1.YPos);
		if ((int)P1.YPos <= GROUND - 190){
			DrawBox(0, GROUND - S.ScroolY - 190, STAGE_WIDTH, GROUND - S.ScroolY -191, 
				Oran, true);
		}
		else{
			DrawBox(0, GROUND - S.ScroolY - 190, STAGE_WIDTH, GROUND - S.ScroolY -191, 
				Cr, true);
		}
		*/

		//DrawFormatString(500, 160, Cr, "%s", P1.nameC);	// 初期化できてる
		//DrawFormatString(500, 180, Cr, "XV:%.2f", P1.C.runF[0]);	// 初期化できてる
		//DrawFormatString(500, 200, Cr, "t1:%d", TText[1]);	// 初期化できてる
		//DrawFormatString(500, 220, Cr, "t2:%d", TText[2]);	// 初期化できてる
		//DrawFormatString(500, 240, Cr, "t3:%d", TText[3]);	// 初期化できてる
		//DrawFormatString(500, 260, Cr, "1:%d", TText[4]);	// 初期化できてる
		//DrawGraph(300, 260, TText[4], true);
		//DrawFormatString(500, 160, Cr, "%.2f", (float)((float)P1.Life / (float)P1.C.lifeMax));
		//if ((P1.YVel > 0) || ((P1.YVel < 0) && (P1.YPos < GROUND - 135)))DrawString(500, 160, "AirOK", Cr);
		//DrawFormatString(500, 160, Cr, "%d, %d", P1.Muteki, P2.Muteki);
		/*
		DrawFormatString(500, 160, Cr, "%d", P1.MoveHit);
		DrawFormatString(500, 180, Cr, "%d", P1.time);
		DrawFormatString(500, 200, Cr, "%d, %d", P1.WAtt[0], P1.HAtt[0]);
		*/
		//int xDist = (int)(P2.XPos - P1.XPos);
		//if (xDist < 0)xDist = -xDist;
		//int yDist = (int)(P2.YPos - P1.YPos);
		//if (yDist < 0)yDist = -yDist;
		
		
		//DrawFormatString(500, 160, Cr, "%d, %d", P1.Var[11], P1.D.armor);
		//DrawFormatString(500, 180, Cr, "F %d, T %d", P1.mutekiF, P1.Muteki);
		
		//DrawFormatString(500, 160, Cr, "%d", P1.addG);
		//DrawFormatString(500, 160, Cr, "c1:%d c4:%d", P1.cmd[1], P1.cmd[4]);
		//DrawFormatString(500, 180, Cr, "%d %d", P2.button[1], P2.rbutton[1]);
		//DrawFormatString(500, 215, Cr, "文字 %s", TChar);
		//DrawFormatString(500, 180, Cr, "テスト %d", TText);

	}

		//m_cPos = P1.XPos;
		if (S.TSwitch[5] == 0 || S.TSwitch[5] == 2){

		int ke = P1.keyPos;
		
		// 追加
		if (P1.muki == 1){
			if (P1.keyPos == 1)ke = 3;
			else if (P1.keyPos == 3)ke = 1;
			else if (P1.keyPos == 4)ke = 6;
			else if (P1.keyPos == 6)ke = 4;
			else if (P1.keyPos == 7)ke = 9;
			else if (P1.keyPos == 9)ke = 7;
		}
		
		// キー
		//if (S.StopTime == 0)
		{
		for (int i = 0; i < 9; i++){
			if ((ke == i+1) && (ke != 5) && (ke != oKey[0] + 1 || ke != saveKey)){
				sKey = true;
				break;
			}
			else { sKey = false; }
		}

		// ボタン
		for (int i = 0; i < 4; i++){
			if (P1.button[i + 1] == 1){
				sButton = true;
				sKey = true;
				break;
			}
			else { sButton = false; }
		}

		
		// キーorボタン開始
		if (sKey || sButton){

			// キー
			for (int a = 1; a < LOG_MAX; a++){
				oKey[LOG_MAX - a] = oKey[LOG_MAX - 1 - a];
			}
			for (int i = 0; i < 9; i++){
				if (ke == i+1){
					oKey[0] = i;

					//else{ oKey[0] = i; }
					break;
				}
				else { oKey[0] = 0; }
			}

			// ボタン
			for (int a = 1; a < LOG_MAX; a++){
				for (int i = 0; i < 4; i++){
					oButton[LOG_MAX - a][i] = oButton[LOG_MAX - 1 - a][i];
				}
			}
			for (int i = 0; i < 4; i++){
				if (P1.button[i + 1] == 1){
					oButton[0][i] = 1;
				}
				else { oButton[0][i] = 0; }
			}
		}
		}	//  stoptime

		// 描画
		int px;
		for (int a = 0; a < LOG_MAX; a++){
			px = 0;
			if (oKey[a] > -1){
				DrawGraph(px, 95 + (a*20), gKey[oKey[a]], true);
			}
			px = 20;
			for (int i = 0; i < 4; i++){
				if (oButton[a][i] == 1){
					DrawGraph(px, 95 + (a * 20), gButton[i], true);
					px += 20;
				}
			}
		}
		
		saveKey = ke;
		}	// S.T[5]

		//DrawFormatString(0, 35, Cr, "テンキー %d", P1.keyPos);
		
		// ミニ画像
		//DrawRotaGraph(20, 25, 0.4, 0, P1.Image, true, P1.muki);
		//DrawRotaGraph(SCREEN_W - 20, 25, 0.4, 0, P2.Image, true, P2.muki);
		
		// テスト画像
		//DrawRotaGraph(120, 125, 0.5, 0, TText, true, 0);
		//DrawFormatString(500, 155, Cr, "猶予 %d", TText);
		//DrawFormatString(500, 135, Cr, "C入力 %d", P1.Senkou[3]);
		//DrawFormatString(500, 155, Cr, "停止 %d", S.Anten);


		//DrawFormatString(500, 115, Cr, "X速度 %.1f", P1.XVel);
		//DrawString(500, 115, "ガード属性", Cr);
		//if (P1.GuardF == 0)DrawString(590, 115, "ガー不", Cr);
		//else if (P1.GuardF == 1)DrawString(590, 115, "上段", Cr);
		//else if (P1.GuardF == 2)DrawString(590, 115, "中段", Cr);
		//else if (P1.GuardF == 3)DrawString(590, 115, "下段", Cr);

	

	// ボックス描画
	{
		// 描画ブレンドモードをアルファブレンド（透明）にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);

		// 喰らい判定
		if (S.TSwitch[6] == 1 || S.TSwitch[6] == 3)BoxKurai();
		// 攻撃判定
		if (S.TSwitch[6] == 2 || S.TSwitch[6] == 3)BoXAtt();
		

		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
}


void GetP_ObjectDraw(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_ObjectDraw(System_t GS)
{
	S = GS;
}

void GetH_ObjectDraw(Helper GH1[], Helper GH2[])
{
	int i;
	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}
}

void BoxKurai()
{
	int color;

	for (int j = 0; j < 2; j++){

		// 最初に青を準備
		color = GetColor(0, 0, 255);
		// 無敵で緑に
		if (j == 0){
			if (P1.Muteki != 0) {
				switch (P1.mutekiF){
				case 0:
					color = GetColor(0, 255, 0);
					break;
				case 1:
					color = GetColor(0, 255, 255);
					break;
				case 2:
					color = GetColor(255, 255, 0);
					break;
				case 3:
					color = GetColor(255, 255, 0);
					break;
				}
			}

		}
		if(j == 1){
			if (P2.Muteki != 0) {
				switch (P2.mutekiF){
				case 0:
					color = GetColor(0, 255, 0);
					break;
				case 1:
					color = GetColor(0, 255, 255);
					break;
				case 2:
					color = GetColor(255, 255, 0);
					break;
				case 3:
					color = GetColor(255, 255, 0);
					break;
				}
			}
		}
		// 2P 喰らい判定チェック
		for (int i = 0; i < HIT_MAX; i++){
			// スクロール量を足す
			if (j == 0){
				m_XD[i] = P1.XHit[i] - (int)S.ScroolX;
				m_YD[i] = P1.YHit[i] - (int)S.ScroolY;
				m_WD[i] = P1.WHit[i];
				m_HD[i] = P1.HHit[i];
			}
			else if (j == 1){
				m_XD[i] = P2.XHit[i] - (int)S.ScroolX;
				m_YD[i] = P2.YHit[i] - (int)S.ScroolY;
				m_WD[i] = P2.WHit[i];
				m_HD[i] = P2.HHit[i];
			}

			if (m_WD[i] > 0){
				// 描画
				DrawBox(m_XD[i], m_YD[i], m_XD[i] +
					m_WD[i], m_YD[i] + m_HD[i], color, true);
			}
				
		}
		// 座標
		if (j == 0){
			DrawBox((int)P1.XPos - 3 - (int)S.ScroolX, (int)P1.YPos - 3 - (int)S.ScroolY,
				(int)P1.XPos + 3 - (int)S.ScroolX, (int)P1.YPos + 3 - (int)S.ScroolY, Oran, true);
		}
		else if (j == 1){
			DrawBox((int)P2.XPos - 3 - (int)S.ScroolX, (int)P2.YPos - 3 - (int)S.ScroolY,
				(int)P2.XPos + 3 - (int)S.ScroolX, (int)P2.YPos + 3 - (int)S.ScroolY, Oran, true);
		}


		// ヘルパー喰らい判定
		for (int i = 0; i < HM; i++){
			for (int n = 0; n < HIT_MAX; n++){
				mHXH[0][i] = H1[i].XHit[n] - (int)S.ScroolX;
				mHYH[0][i] = H1[i].YHit[n] - (int)S.ScroolY;
				mHWH[0][i] = H1[i].WHit[n];
				mHHH[0][i] = H1[i].HHit[n];

				mHXH[1][i] = H2[i].XHit[n] - (int)S.ScroolX;
				mHYH[1][i] = H2[i].YHit[n] - (int)S.ScroolY;
				mHWH[1][i] = H2[i].WHit[n];
				mHHH[1][i] = H2[i].HHit[n];

				// 攻撃判定チェック
				DrawBox(mHXH[j][i], mHYH[j][i], mHXH[j][i] + mHWH[j][i],
					mHYH[j][i] + mHHH[j][i], GetColor(0, 0, 255), true);

				
			}
			if (H1[i].var){
				// 座標
				DrawBox((int)H1[i].XPos - 3 - (int)S.ScroolX, (int)H1[i].YPos - 3 - (int)S.ScroolY,
					(int)H1[i].XPos + 3 - (int)S.ScroolX, (int)H1[i].YPos + 3 - (int)S.ScroolY, Oran, true);
			}
		}
	}	// for終了

}

void BoXAtt()
{
	for (int j = 0; j < 2; j++){
		
		/*
		{
			mXA[0][0] = P1.X_UL - (int)S.ScroolX;
			mYA[0][0] = P1.Y_UL - (int)S.ScroolY;
			mWA[0][0] = P1.xSize;
			mHA[0][0] = P1.ySize;

			// キャラサイズチェック
			DrawBox(mXA[0][0], mYA[0][0], mXA[0][0] + mWA[0][0],
				mYA[0][0] + mHA[0][0], GetColor(0, 240, 0), true);
		}
		*/

		for (int i = 0; i < HIT_MAX; i++){
			mXA[0][i] = P1.XAtt[i] - (int)S.ScroolX;
			mYA[0][i] = P1.YAtt[i] - (int)S.ScroolY;
			mWA[0][i] = P1.WAtt[i];
			mHA[0][i] = P1.HAtt[i];

			mXA[1][i] = P2.XAtt[i] - (int)S.ScroolX;
			mYA[1][i] = P2.YAtt[i] - (int)S.ScroolY;
			mWA[1][i] = P2.WAtt[i];
			mHA[1][i] = P2.HAtt[i];

			if (mWA[j][i] > 0){
				// 攻撃判定チェック
				DrawBox(mXA[j][i], mYA[j][i], mXA[j][i] + mWA[j][i],
					mYA[j][i] + mHA[j][i], GetColor(255, 0, 0), true);
			}
		}


		// ヘルパー攻撃判定
		for (int i = 0; i < HM; i++){
			for (int n = 0; n < HIT_MAX; n++){
				mHXA[0][i] = H1[i].XAtt[n] - (int)S.ScroolX;
				mHYA[0][i] = H1[i].YAtt[n] - (int)S.ScroolY;
				mHWA[0][i] = H1[i].WAtt[n];
				mHHA[0][i] = H1[i].HAtt[n];

				mHXA[1][i] = H2[i].XAtt[n] - (int)S.ScroolX;
				mHYA[1][i] = H2[i].YAtt[n] - (int)S.ScroolY;
				mHWA[1][i] = H2[i].WAtt[n];
				mHHA[1][i] = H2[i].HAtt[n];

				// 攻撃判定チェック
				DrawBox(mHXA[j][i], mHYA[j][i], mHXA[j][i] + mHWA[j][i],
					mHYA[j][i] + mHHA[j][i], GetColor(255, 0, 0), true);
			}
		}

	}
}


void TestText(int i, int pos)
{
	TText[pos] = i;
}

void TestChar(char ch[], int size)
{
	for (int i = 0; i < size; i++){
		TChar[i] = ch[i];
	}
}

// 発生フレーム
void AttackSpeed(int i)
{
	aSpeed = i;
}

// フレームチェック
void Frame_Check()
{
	mF_Start = 1, mFrame = 0;
}

// 連続ヒット(青ビ)確認
void Beat_Check(int state, int pside)
{
	// 1P
	if (pside == 1){
		// 喰らっている
		if (P2.HFlg){
			// 受け身可能なら青ビ
			if (state == 1035){
				beat[0] = false;
			}
		}
		// 喰らってない時は赤ビ
		else{
			beat[0] = true;
		}
	}

	// 2P
	else if (pside == 2){
		if (P1.HFlg){
			if (state == 1035){
				beat[1] = false;
			}
		}
		else{
			beat[1] = true;
		}
	}
}

// キー表示
void ObjectDraw_KeyDisplay()
{
	SetAlpha(158);
	DrawGraph((SCREEN_W / 2) - 60, 65, m_KeyPos, true);

	SetAlpha(255);
	int x = 0, y = 0;
	if (P1.button[102] > 0)y = 1;
	if (P1.button[108] > 0)y = -1;
	if (P1.button[106] > 0)x = 1;
	if (P1.button[104] > 0)x = -1;
	DrawGraph((SCREEN_W / 2) - 60 + 12 + (x * 16), 65 + 12 + (y * 16), m_Key, true);

	SetAlpha(255);
	if (P1.button[1] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W/2) + 10, 65, m_Button[0], true);

	SetAlpha(255);
	if (P1.button[2] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 35, 65, m_Button[1], true);

	SetAlpha(255);
	if (P1.button[3] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 60, 65, m_Button[2], true);

	SetAlpha(255);
	if (P1.button[4] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 10, 90, m_Button[3], true);

	NoBlend();

	////////////////////////
	// P2
	SetAlpha(158);
	DrawGraph((SCREEN_W / 2) - 60, 125, m_KeyPos, true);

	SetAlpha(255);
	x = 0, y = 0;
	if (P2.button[102] > 0)y = 1;
	if (P2.button[108] > 0)y = -1;
	if (P2.button[106] > 0)x = 1;
	if (P2.button[104] > 0)x = -1;
	DrawGraph((SCREEN_W / 2) - 60 + 12 + (x * 16), 125 + 12 + (y * 16), m_Key, true);

	SetAlpha(255);
	if (P2.button[1] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 10, 125, m_Button[0], true);

	SetAlpha(255);
	if (P2.button[2] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 35, 125, m_Button[1], true);

	SetAlpha(255);
	if (P2.button[3] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 60, 125, m_Button[2], true);

	SetAlpha(255);
	if (P2.button[4] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 10, 150, m_Button[3], true);

	NoBlend();
}

// アドゲージ
void AddGaugeDraw(int power, int side)
{
	addGaugeTime[side - 1] = 60;
	addGaugePower[side - 1] = power;
}

// 黒の度合い
void SetBlack(int b)
{
	blackOut = b;
	// 最大値設定
	max(0, min(255, blackOut));	// 右に今の数値を
}