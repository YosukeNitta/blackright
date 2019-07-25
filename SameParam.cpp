//インクルード部--------------------------------------------
#include "pch.h"
#include <vector>
#include <string>
#include <stdio.h>	// ファイル操作に必要なヘッダファイル
#include <string.h>	// 文字列操作に必要なヘッダファイル
#include "Define2.h"

using namespace std;

void Furimuki();
static void PSetSend();

//
static char t_State[6] = { "State" };	// sffno
static char t_sflg[5] = { "sflg" };
static char t_ctrl[5] = { "ctrl" };
static char t_time[5] = { "time" };
static char t_lock[5] = { "lock" };
static char t_advancing[10] = { "advancing" };

static char t_S[2] = { "S" };
static char t_trigger1[9] = { "trigger1" };
static char t_icoico[5] = { "t_==" };

class Flag{
public:
	boolean var;	// trueなら問答無用で実行
	int num[3];	// 0..対象 1..条件式(==,>=,<=) 2..変更内容
	Flag(){
		var = false;
		for (int i = 0; i < 3; i++){
			num[i] = 0;
		}
	}
};

// アクション内容、ステートによって数はまちまち
class ActState{
public:
	int actionNum;	// 番号で命令を決める
	int num[4];	// 命令で代入する数字
	ActState(){
		actionNum = 0;
		for (int i = 0; i < 4; i++){
			num[i] = 0;
		}
	}
};

// 実行するアクション内容
class Action{
public:
	vector<Flag>flg;		// 実行する条件
	vector<ActState>aSt;	// アクションの内容
					// flgの条件が許可された時に実行
					// 数はまちまち
	Action(){

	}
};

// 初期ステート
class State{
public:
	int num;	// ステート番号
	boolean ctrl;
	int sflg;
	vector<Action>act;	// 実行するアクションと条件
						// 数はステートによってまちまち
	State(){
		num = 0;
		ctrl = 0;
		sflg = 0;
	}
};
//
static int n;
static vector<Name_t>N;
static vector<vector<State>>St;
static State defSt;

static void FirstState();
int GetPNum(int set);	// P1の情報を取得

// デフォルトステート(mugenのあれ)読み込み
void Load_StateDef(int name){

	// 判定セッティング //

	int i, fp;
	int row, nextSt;	// セットするシステム
	//int trigger;	// トリガー設定、0..なし 1..条件 2..条件でのアクション
	int icole;	// イコール、フラグの比較に使う  -1..なし 0..== 1..>= 2..<=
	row = -2;
	nextSt = -1;
	icole = -1;

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// inputとinputcに文字がはいる
	boolean iflg;	// 名前入力フラグ

	// 名前読み込み
	string fname;
	// デフォルト設定
	if (name == -1){
		fname = "char/def_state.txt";
	}
	else{
		string fn1 = "char/";
		string fn2 = "/state.txt";
		fname = fn1 + N[name].nameC + fn2;
	}
	name++;	// +1する

	// ファイルを開く //
	fp = FileRead_open(fname.c_str());//ファイル読み込み
	if (fp == NULL){			// エラーが起こったら
		printfDx("error def_state\n");// 文字を表示
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
					//inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				}
				i = -1;
				break;
			}

			// 文字列にする
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == ';' || input[i] == '\t'){ //カンマか改行なら
				inputc[i] = '\0';//そこまでを文字列とし（これを見つけると文字列として扱う）
				break;	// iを1増やしてループ脱出（代入する）
			}

			// 読み込み終了
			if (input[i] == EOF){//ファイルの終わりなら
				goto EXFILE;//終了
			}
		}	// 基本ループ終了

		// 数字を判断
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

		// 名前だったら
		if (iflg){
			switch (row)
			{
			case 0:	// stateNum
				nextSt++;
				St[name].push_back(State());
				St[name][nextSt].num = atoi(inputc);
				break;
			case 1:	// sflg
				St[name][nextSt].sflg = atoi(inputc);
				break;
			case 2:	// ctrl
				St[name][nextSt].ctrl = atoi(inputc);
				break;
			case 10:	// time
				//St[name][nextSt].act[0].aSt[0].num[0];
				break;
			case 11:	// lock
				//St[name][nextSt].act[0].aSt[0].num[0] = atoi(inputc);
				break;
			case 12:	// advancing
				//St[name][nextSt].act[0].aSt[0].num[0] = atoi(inputc);
				break;
			default:
				break;
			}
		}

		// セット
		if (!iflg){
			// ステート、判定数、当たり、喰らいの判断
			// "State"
			if (strcmp(inputc, t_State) == 0){
				row = 0;
			}
			// "sflg"
			if (strcmp(inputc, t_sflg) == 0){
				row = 1;
			}
			// "ctrl"
			if (strcmp(inputc, t_ctrl) == 0){
				row = 2;
			}
			// "time"
			if (strcmp(inputc, t_time) == 0){
				row = 10;
			}
			// "lock"
			if (strcmp(inputc, t_lock) == 0){
				row = 11;
			}
			// "advancing"
			if (strcmp(inputc, t_advancing) == 0){
				row = 12;
			}
			// "trigger1"
			if (strcmp(inputc, t_trigger1) == 0){
				//trigger = 1;
				//St[name][nextSt].act.push_back(Action());
				//St[name][nextSt].act[0].flg.push_back(Flag());
				//St[name][nextSt].act[0].aSt.push_back(ActState());
			}
			// "=="
			if (strcmp(inputc, t_icoico) == 0){
				icole = 0;
			}
		}
		if (input[i] == EOF){//ファイルの終わりなら
			goto EXFILE;//終了
		}
	}

	// ファイルを閉じる
EXFILE:
	FileRead_close(fp);

}

void FirstState(){
	boolean check = false;
	// 検索する
	for (int i = 0; i < St[P1.Name].size(); i++){
		if (St[P1.Name][i].num == P1.stateno){
			P1.SFlg = St[P1.Name][i].sflg;
			P1.ctrl = St[P1.Name][i].ctrl;
			check = true;
			break;
		}
	}
	// 見つからなかったらデフォルト
	if (!check){
		for (int i = 0; i < St[0].size(); i++){
			if (St[0][i].num == P1.stateno){
				P1.SFlg = St[0][i].sflg;
				P1.ctrl = St[0][i].ctrl;
				for (int j = 0; j < St[0][i].act.size(); j++){
					// プレイヤー数値の番号を取得
					P1.Lock = St[0][i].act[j].aSt[0].num[0];
				}
				//if ((P1.time) == (St[P1.Name][i].act[0].flg[0].num[2])){
					/*
					// lock
					if (St[P1.Name][i].act[0].aSt[0].actionNum = 11){
						P1.Lock = St[P1.Name][i].act[0].aSt[0].num[0];
					}
					// advancing
					if (St[P1.Name][i].act[0].aSt[0].actionNum = 12){
						P1.advancing = St[P1.Name][i].act[0].aSt[0].num[0];
					}
					*/
				//}
				break;
			}
		}
	}
}

int GetPNum(int set){
	int get = -1;
	switch (set){
	case 11:
		get = P1.Lock;
		break;
	case 12:
		get = P1.advancing;
		break;
	default:
		break;
	}
	return get;
}

int SetPNum(int set){
	int get = -1;
	switch (set){
	case 11:
		//P1.Lock = St[P1.Name][0];
		break;
	case 12:
		//get = P1.advancing;
		break;
	default:
		break;
	}
	return get;
}

// 指定なしステート変更時の共通処理
void ChangeState()
{
	// 時間を0にする
	if (P1.NoChangeTime)P1.NoChangeTime = 0;
	else{ 
		P1.time = 0; 
		P1.AnimTime = 0;
	}
	

	// ダメ設定リセット
	for (int i = 0; i < HIT_MAX; i++)
	{
		P1.XAtt[i] = -100, P1.YAtt[i] = -100;
		P1.WAtt[i] = 0, P1.HAtt[i] = 0;
	}
	//Damage(0, 0);
	//Power(0, 0);
	P1.attF = 1;
	P1.fallF = 1;
	P1.MoveHit = 0;

	P1.A.boundLevel = 0;
	P1.A.bound_xvel = -1.3;
	P1.A.bound_yvel = -3.0;
	
	P1.A.wallLevel = 0;
	P1.A.forceKurai = false;
	P1.GuardF = 1;	// デフォは上段に
	P1.A.boundLevel = 0;

	P1.A.addMuteki = false;
	P1.A.amTime = 0;

	//HitTime(0, 0, 0, 0);

	// [座標を0にする]
	if (P1.SFlg != 2)P1.YPos = GROUND;

	// [ガード距離]
	P1.GuardDist = 280;

	// [重力がかかるように]
	// [画像の反転を戻す]
	P1.ignoreG = false;
	P1.turnG = false;

	P1.touchF = true;
	P1.CFlg = false;

	// ダッシュ中なら振り向かない
	if (P1.DashFlg == 1)P1.Turn = false;
	else { P1.Turn = true; }

	//[ ダッシュ中からの自動振り向き ]
	if ((P1.SFlg != 2) && (P1.time == 0) && (!P1.Turn)){

		if ((P1.XPos < P2.XPos) && (P1.muki == 1)){
			P1.muki = 0;
		}
		else if ((P1.XPos > P2.XPos) && (P1.muki == 0)){
			P1.muki = 1;
		}
	}

	// 戻す
	PSetSend();
}


void SameParam()
{
	// 状態を読み込み
	if (P1.stateno >= 40 && P1.stateno <= 57)FirstState();

	switch (P1.stateno){
	
		//********************
		// 5 振り向き
		//********************
	case 5:
		P1.SFlg = 0, P1.ctrl = 1;
		VelSet(0, 0);
		if (P1.time == 0){
			P1.Lock = 1;
		}
		if (P1.time >= ANIMELEM){
			P1.Lock = 0;
		}
		break;
		//********************
		// 6 振り向き（しゃがみ）
		//********************
	case 6:

		P1.SFlg = 1, P1.ctrl = 1;
		VelSet(0, 0);
		if (P1.time == 0){
			P1.Lock = 1;
		}
		if (P1.time >= ANIMELEM){
			P1.Lock = 0;
			P1.More = 1, P1.time = 0, P1.stateno = 11;
		}
		break;
		//********************
		// 10 立ち→しゃがみ
		//********************
	case 10:
		P1.SFlg = 1;	//しゃがみ判定に
		// 慣性
		if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
		// しゃがみから立ちへ 
		if (P1.button[102] == 0){
			P1.time = 0, P1.stateno = 12, P1.More = 1;
		}

		if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 11, P1.More = 1;

		break;
	case 40:	// ジャンプ移行
		//40～57FS 
		P1.XVel = 0, P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 2;
		//if (P1.XPos < P2.XPos)P1.muki = 0;
		// タイム 0 の所で判断
		if (P1.time == 0){
			
			// 左入力
			P1.JumpVel = 0;
			if (P1.muki == 0){
				if (P1.button[104] > 0){
					P1.JumpVel = -1;
				}
				// 右入力
				else if (P1.button[106] > 0){

					P1.JumpVel = 1;
					// 手前で走ってたら
					if (P1.OldState == 30){
						P1.JumpVel = 2;
					}
				}
			}
			else if (P1.muki == 1){
				if (P1.button[104] > 0){
					P1.JumpVel = 1;
					// 手前で走ってたら
					if (P1.OldState == 30){
						P1.JumpVel = 2;
					}
				}
				// 右入力
				else if (P1.button[106] > 0){
					P1.JumpVel = -1;
				}
			}

			// 先行入力を消す
			P1.K_Senkou[8] = 0;
		}
		// ステート変更、3フレを超えた時ジャンプへ
		if (P1.time >= P1.animElem[P1.stateno]){
			// ジャンプフラグをオン
			P1.JumpFlg = 1;
			// ワンモアで次のステートに
			P1.More = 1, P1.time = 0;
			switch (P1.JumpVel){
			case 0:
				P1.stateno = 41;
				break;
			case 1:
				P1.stateno = 42;
				break;
			case 2:
				P1.stateno = 42;
				break;
			case -1:
				P1.stateno = 43;
				break;
			}
			P1.YVel = P1.C.jumpY;
			// ハイジャンプ
			if (P1.hyJump){
				//エフェクト
				EffStartB(16, P1.XPos, P1.YPos, 0, 0,
					0.24, 0.03, P1.muki);
				P1.YVel = P1.C.jumpY * 1.25;
				P1.AirJump--;
				afterimageSetting(P1.PSide, 16, 128,128,250);
			}
		}

		break;	// ジャンプ準備終了

	case 41:	// 垂直
		P1.Muteki = 0;
		P1.XVel = 0;	// 垂直は左右に飛ばない
		// タイムが 0 のとき、↑に飛ぶ
		if (P1.time == 0){
			// 二段ジャンプ時、ジャンプ力を下げる
			{ 
				//P1.YPos -= 2;
			}
		}
		// 落下
		if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
		// 地面にいて時間が 0 じゃない時
		if ((P1.YPos > GROUND) && (P1.time != 0)){
			P1.stateno = 47, P1.More = 1, P1.time = 0;
		}
		break;
	case 42:	// 前ジャンプ
		P1.Muteki = 0;
		P1.XVel = P1.C.jumpF[0]; //速度を足す;
		if (P1.JumpVel == 2)P1.XVel = P1.C.jumpF[1];//P1.XVel += (P1.XVel / 4) + 0.2;	// ダッシュ時
		if (P1.hyJump)P1.XVel = P1.C.jumpF[1];//*1.3; //速度を足す;
		// タイムが 0 のとき、↑に飛ぶ
		if (P1.time == 0){
			// 二段ジャンプ時、ジャンプ力を下げる
			{ 
				//P1.YPos -= 2;
			}
		}
		// 落下
		if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
		// 地面にいて時間が 0 じゃない時
		if ((P1.YPos > GROUND) && (P1.time != 0)){
			P1.stateno = 47, P1.More = 1, P1.time = 0;
		}
		break;
	case 43:	// 後ろジャンプ
		P1.Muteki = 0;
		P1.XVel = P1.C.jumpB; //速度を足す;
		if (P1.hyJump)P1.XVel = P1.C.jumpB*1.2; //速度を足す;
		// タイムが 0 のとき、↑に飛ぶ
		if (P1.time == 0){
			// 二段ジャンプ時、ジャンプ力を下げる
			{ 
				//P1.YPos -= 2;
			}
		}
		// 落下
		if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
		// 地面にいて時間が 0 じゃない時
		if ((P1.YPos > GROUND) && (P1.time != 0)){
			P1.stateno = 47, P1.More = 1, P1.time = 0;
		}
		break;
	case 45:	// 空中ジャンプ
		P1.XVel = 0, P1.YVel = -0.2;
		
		if (P1.time == 0){
			// 左入力
			if (P1.keyAtt[4]){
				P1.JumpVel = -1;
			}
			// 右入力
			else if (P1.keyAtt[6]){
				if (P1.JumpVel == 2)P1.JumpVel = 2;
				else{ P1.JumpVel = 1; }
			}
			else{ P1.JumpVel = 0; }

			P1.AirJump--, P1.airDash--;
			P1.hyJump = false;

			// 先行入力を消す
			P1.K_Senkou[8] = 0;
		}

		// 振り向き
		if (P1.time == 0){
			if (P1.XPos < P2.XPos && P1.muki == 1){
				P1.muki = 0;
				P1.JumpVel = -P1.JumpVel;
			}
			else if (P2.XPos < P1.XPos && P1.muki == 0){
				P1.muki = 1;
				P1.JumpVel = -P1.JumpVel;
			}
		}

		// ステート変更、2フレを超えた時ジャンプへ
		if (P1.time > 1){
			//エフェクト
			EffStartB(16, P1.XPos, P1.YPos - 20, 0, 0,
				0.24, 0.03, P1.muki);
			// ジャンプフラグをオン
			P1.JumpFlg = 1;
			// ワンモアで次のステートに
			P1.More = 1, P1.time = 0;
			switch (P1.JumpVel){
			case 0:
				P1.stateno = 41;
				break;
			case 1:
				P1.stateno = 42;
				break;
			case 2:
				P1.stateno = 42;
				break;
			case -1:
				P1.stateno = 43;
				break;
			}
			P1.YVel = P1.C.jumpA;
		}
		break;	// ジャンプ準備終了

	case 46:	// ジャンプ状態
		P1.Muteki = 0;
		P1.ignoreG = false;
		if (P1.time >= P1.animElem[P1.stateno])P1.time = 0;

		// 地面にいる時
		if (P1.YPos + P1.YVel * 2 == GROUND){
			P1.stateno = 47, P1.More = 1, P1.time = 0;
		}

		break;

	case 47:	// 着地
		P1.A.damage = 0;
		P1.YPos = GROUND;
		P1.YVel = 0;
		// SEを鳴らす
		if (P1.time == 1)SEStart(6);
		// 最初にロック！
		if (P1.time == 0){
			//エフェクト
			//EffStartB(16, P1.XPos, P1.YPos, 0, 0,
				//0.20, 0.02, P1.muki);
			P1.Lock = 1;
		}
		// 着地 の 全体フレーム
		if (P1.time >= 1)P1.More = 0, P1.stateno = 0,
			P1.time = 0, P1.Lock = 0;

		break;

		/*
	case 50:	// 立ちガード開始
		P1.SFlg = 0, P1.ctrl = 1;
		P1.YVel = 0;
		// 最初にロック！
		if (P1.time == 0)P1.Lock = 1;
		// キー離しでガード解除
		if (!P1.keyAtt[4]){
			P1.More = 1, P1.stateno += 2, P1.Lock = 1;
			P1.time = 0;
		}

		// キー下でしゃがみ
		if (P1.keyAtt[2]){
			P1.More = 1, P1.stateno = 53, P1.Lock = 1;
			P1.time = 0;
		}
		break;
		*/

	case 50:	// 立ちガード 硬直
		P1.SFlg = 0;
		P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 2;
		// 最初にロック！
		if (P1.time == 0){
			P1.Lock = 1;
			P1.advancing = false;
		}

		// 下入れでしゃがみガードへ
		if (P1.keyAtt[2] > 0){
			P1.More = 1, P1.stateno = 53, P1.Lock = 1;
			P1.NoChangeTime = 1;
			P1.ctrl = 0;
		}

		// アドガ
		/*
		if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.keyAtt[4]) && (P1.advancing == false)
			&& (P1.StopTime == 0) && (P1.aGauge > (GUARD_MAX / 5)) && (P1.time + 2 < P1.D.nokezori)){
			if ((P2.XPos - P1.XPos < (SCREEN_W / 2)) && (P1.XPos - P2.XPos < (SCREEN_W / 2)))
				P2.D.addVelX += -7.0;
			EffStart(16, P1.XPos, P1.YPos, 10, -(P1.ySize / 1.5),
				0.04, 0.25, P1.muki);
			P1.colorCTime = 3;
			P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			SEStart(19);
			P1.advancing = true;
			P1.aGauge -= (GUARD_MAX / 5);
			P1.D.nokezori += 4;
			P1.Senkou[1] = 0, P1.Senkou[2] = 0;
		}
		*/

		// ガード続行
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 0, P1.Lock = 1;
			P1.time = 0, P1.More = 1;
			P1.Muteki = ThrowMuteki_Guard, P1.mutekiF = 2;
			P1.ctrl = 1;
			P1.NoChangeTime = 0;
		}
		
		break;
	case 51:	// しゃがみ 硬直
		P1.SFlg = 1;	// 追加
		P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 2;
		if (P1.time == 0){
			P1.Lock = 1;
			P1.advancing = false;
		}

		// 下入れてないと立ちガ
		if (P1.keyAtt[2] == 0){
			P1.More = 1, P1.stateno = 54, P1.Lock = 1;
			P1.NoChangeTime = 1;
			P1.ctrl = 0;
		}
		
		// アドガ
		/*
		if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.keyAtt[4]) && (P1.advancing == false)
			&& (P1.StopTime == 0) && (P1.aGauge > (GUARD_MAX / 5)) && (P1.time + 2 < P1.D.nokezori)){
			if ((P2.XPos - P1.XPos < (SCREEN_W / 2)) && (P1.XPos - P2.XPos < (SCREEN_W / 2)))
				P2.D.addVelX += -7.0;
			EffStart(16, P1.XPos, P1.YPos, 10, -(P1.ySize / 1.5),
				0.04, 0.25, P1.muki);
			P1.colorCTime = 3;
			P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			SEStart(19);
			P1.advancing = true;
			P1.aGauge -= (GUARD_MAX / 5);
			P1.D.nokezori += 4;
			P1.Senkou[1] = 0, P1.Senkou[2] = 0;
		}
		*/

		// 終了へ移行
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 11, P1.Lock = 1;
			P1.time = 0, P1.More = 1;
			P1.Muteki = ThrowMuteki_Guard, P1.mutekiF = 2;
			P1.ctrl = 1;
			P1.NoChangeTime = 0;
		}
		
		break;
	case 52:	// 空中 硬直
		P1.SFlg = 2; //追加
		P1.Muteki = 1, P1.mutekiF = 2;
		if (P1.time == 0){
			P1.Lock = 1;
			P1.advancing = false;
		}
		// 振り向き
		if (P1.time == 0){
			if ((P1.XPos < P2.XPos) && (P1.muki == 1))P1.muki = 0;
			else if ((P1.XPos > P2.XPos) && (P1.muki == 0))P1.muki = 1;
		}
		/*
		// 着地したら移行
		if (P1.YPos + P1.YVel * 2 >= GROUND){
			P1.More = 1, P1.stateno = 51, P1.Lock = 1;
			if (P1.keyAtt[2] > 0)P1.stateno = 54;
		}
		*/

		// アドガ
		/*
		if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.keyAtt[4]) && (P1.advancing == false)
			&& (P1.StopTime == 0) && (P1.aGauge > (GUARD_MAX / 5)) && (P1.time + 2 < P1.D.nokezori)){
			if ((P2.XPos - P1.XPos < (SCREEN_W / 2)) && (P1.XPos - P2.XPos < (SCREEN_W / 2)))
				P2.D.addVelX += -8.2;
				P1.A.hbVelY -= 1.8;
			EffStart(16, P1.XPos, P1.YPos, 10, -(P1.ySize / 1.5),
				0.04, 0.25, P1.muki);
			P1.colorCTime = 3;
			P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			SEStart(19);
			P1.advancing = true;
			P1.aGauge -= (GUARD_MAX / 5);
			P1.D.nokezori += 4;
			P1.Senkou[1] = 0, P1.Senkou[2] = 0;
		}
		*/

		// 前ジャンプ の 全体フレーム
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 46, P1.Lock = 0;
			P1.time = 0, P1.More = 1;
			P1.Muteki = ThrowMuteki_Guard, P1.mutekiF = 2;
		}
		break;

	case 53:	// 立ちガード(2入力)
		P1.SFlg = 1;
		P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 2;
		// 最初にロック！
		if (P1.time == 0){
			P1.Lock = 1;
			P1.advancing = false;
		}

		// 下入れでしゃがみガードへ
		if (P1.keyAtt[2] == 0){
			P1.More = 1, P1.stateno = 50, P1.Lock = 1;
			P1.NoChangeTime = 1;
			P1.ctrl = 0;
		}

		// ガード続行
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 0, P1.Lock = 1;
			P1.time = 0, P1.More = 1;
			P1.Muteki = ThrowMuteki_Guard, P1.mutekiF = 2;
			P1.ctrl = 1;
			P1.NoChangeTime = 0;
		}

		break;
	case 54:	// しゃがみガード(2入力なし)
		P1.SFlg = 0;
		P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 2;
		if (P1.time == 0){
			P1.Lock = 1;
			P1.advancing = false;
		}

		// 下入れで小さく
		if (P1.keyAtt[2] > 0){
			P1.More = 1, P1.stateno = 51, P1.Lock = 1;
			P1.NoChangeTime = 1;
			P1.ctrl = 0;
		}

		// アドガ
		/*
		if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.keyAtt[4]) && (P1.advancing == false)
		&& (P1.StopTime == 0) && (P1.aGauge > (GUARD_MAX / 5)) && (P1.time + 2 < P1.D.nokezori)){
		if ((P2.XPos - P1.XPos < (SCREEN_W / 2)) && (P1.XPos - P2.XPos < (SCREEN_W / 2)))
		P2.D.addVelX += -7.0;
		EffStart(16, P1.XPos, P1.YPos, 10, -(P1.ySize / 1.5),
		0.04, 0.25, P1.muki);
		P1.colorCTime = 3;
		P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
		SEStart(19);
		P1.advancing = true;
		P1.aGauge -= (GUARD_MAX / 5);
		P1.D.nokezori += 4;
		P1.Senkou[1] = 0, P1.Senkou[2] = 0;
		}
		*/

		// 終了へ移行
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 11, P1.Lock = 1;
			P1.time = 0, P1.More = 1;
			P1.Muteki = ThrowMuteki_Guard, P1.mutekiF = 2;
			P1.ctrl = 1;
			P1.NoChangeTime = 0;
		}

		break;
	case 55:	// 投げ抜け
		P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 0;
		// 最初にロック！
		if (P1.time == 0)P1.Lock = 1;

		// ガード続行
		if (P1.time >= 26){
			P1.stateno = 0, P1.Lock = 0;
			P1.time = 0, P1.More = 1;
			P1.Muteki = 0, P1.mutekiF = 0;
			P1.ctrl = 1;
			P1.NoChangeTime = 0;
		}

		break;

	case 56:	// 空中投げ抜け
		P1.Muteki = 1, P1.mutekiF = 0;
		// 最初にロック！
		if (P1.time == 0)P1.Lock = 1;
		// 振り向き
		if (P1.time == 0){
			if ((P1.XPos < P2.XPos) && (P1.muki == 1))P1.muki = 0;
			else if ((P1.XPos > P2.XPos) && (P1.muki == 0))P1.muki = 1;
		}
		/*
		// 前ジャンプ の 全体フレーム
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 46, P1.Lock = 0;
			P1.time = 0, P1.More = 1;
			P1.Muteki = 0, P1.mutekiF = 0;
		}
		*/
		break;

	case 60:	// 空中ダッシュ
		P1.SFlg = 2, P1.Lock = 0;
		P1.XVel = 0.5 + (P1.C.runF[0] * 0.75); //速度を足す
		P1.YVel = 0;
		P1.ignoreG = true;

		if (P1.time == 0){
			SEStart(5);
			EffStart(16, (int)P1.XPos, (int)P1.YPos, -10, -(P1.ySize / 1.5),
				0.04, 0.25, P1.muki);
		}
		if (P1.time <= 6){
			P1.ctrl = 0;
		}
		else{ P1.ctrl = 1; }
		
		/*
		// 先行入力
		if (P1.time <= 8){
			for (int i = 1; i < 5; i++){
				if (P1.button[i] > 0)P1.Senkou[i] = 10;
			}
		}
		if (P1.time == 9){
			for (int i = 1; i < 5; i++){
				if (P1.Senkou[i] > 0){
					P1.button[i] = 1;
				}
			}
		}
		*/

		if (P1.airDashTime == 0){
			P1.stateno = 46, P1.time = 0;
		}
		break;
	case 65:	// 空中バックダッシュ
		P1.SFlg = 2, P1.Lock = 0;
		P1.XVel = -0.6 + (-P1.C.runF[0] * 0.75); //速度を足す
		P1.YVel = 0;
		P1.ignoreG = true;

		if (P1.time == 0){
			SEStart(5);
			EffStart(16, P1.XPos, P1.YPos, 10, -(P1.ySize / 1.5),
				0.04, 0.25, P1.muki);
		}

		if (P1.time <= 6)P1.ctrl = 0;
		else{ P1.ctrl = 1; }
		// SEを鳴らす
		//if ((P1.time == 20) || (P1.time == 40))SEStart(4);
		//P1.aNum2 = 0;

		/*
		// 先行入力
		if (P1.time <= 8){
			for (int i = 1; i < 5; i++){
				if (P1.button[i] > 0)P1.Senkou[i] = 10;
			}
		}
		if (P1.time == 9){
			for (int i = 1; i < 5; i++){
				if (P1.Senkou[i] > 0){
					P1.button[i] = 1;
				}
			}
		}
		*/

		if (P1.airDashTime == 0){
			P1.stateno = 46, P1.time = 0;
		}
		break;
		//********************
		// 520：ガーキャン
		//********************
	case 520:	//立ちB
		P1.ctrl = 0, P1.SFlg = 0;
		// SE
		if (P1.StopTime == 0){
			if (P1.time == 0){
				SEStart(30);

				//P1.aGauge -= (GUARD_MAX / 2);
				P1.Power -= 1000;
				VelSet(0, 0);

				S.StopTime = 10;
				S.Anten = 10;
				P2.CFlg = 0;

				P1.colorCTime = 10;
				P1.colorC[0] = 100, P1.colorC[1] = 100, P1.colorC[2] = 255;
			}
		}

		if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 20, 0);

		// 攻撃出るまで無敵
		if (P1.D.counter == 1){
			P1.Muteki = 3;
			P1.mutekiF = 0;
		}

		// ヒット数セット、８フレ
		if (P1.time == 1){
			P1.MoveHit = 1;	// １回
		}
		// ダメージセット
		if ((P1.time >= 1) && (P1.time <= ANIMELEM - 1)){

			// [ダメージ]
			Damage(0, 0);

			// [ゲージ] 
			P1.GetPow = 0, P1.GivePow = 0;
			HitTime(8, 20, 20, 18);

			// [ノックバック]
			HitVel(-4.6, -2.6, -4.2, -3);
			P1.GuardF = 1;
			// [喰らい中の浮き]
			P1.fallF = 2;
			P1.HitAnim = 1000;	// 上段喰らい
			P1.HitSE = 11;
		}

		// 全体フレームを超えたらリセット
		if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

		break;
		//********************
		// 550：ゲージ追加
		//********************
	case 550:	//立ちB
		P1.ctrl = 0, P1.SFlg = 0;
		// SE
		if (P1.StopTime == 0){
			if (P1.time == 0){
				SEStart(30);

				//P1.aGauge -= (GUARD_MAX / 2);
				VelSet(0, 0);

				//S.StopTime = 10;
				P2.StopTime += 25;
				S.Anten = 20;
				P2.CFlg = 0;

				P1.Var[30] += 1;	// 回数カウント

				P1.colorCTime = 10;
				P1.colorC[0] = 200, P1.colorC[1] = 200, P1.colorC[2] = 200;
			}
		}

		if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 20, 0);

		if (P1.time == 18){
			P1.Power += 1000;
			// 中段エフェクト
			EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.50, 0.50, P1.PSide);
			SEStart(31);
		}

		// 全体フレームを超えたらリセット
		if (P1.time >= ANIMELEM){
			P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
		}
		break;
		//********************
		// 551：バースト
		//********************
	case 551:	//バースト
		P1.ctrl = 0;
		P1.ignoreG = false;
		
		P1.SFlg = 2;

		P1.YVel -= 0.2;
		// SE
		if (P1.StopTime == 0){
			if (P1.time == 0){
				SEStart(30);

				//P1.aGauge -= (GUARD_MAX / 2);
				//P1.Power += 1000;
				VelSet(0, -6);
				S.StopTime = 10;
				S.Anten = 10;
				//P2.CFlg = 0;

				P1.Var[30] += 1;	// 回数カウント
				//P1.colorCTime = 10;
				//P1.colorC[0] = 200, P1.colorC[1] = 200, P1.colorC[2] = 200;
			}
		}

		if (P1.time <= 11){
			P1.mutekiF = 0;
			P1.Muteki = 2;
		}

		// ヒット数セット
		if (P1.time == 1)P1.MoveHit = 1;	// １回
		// ダメージセット、持続 5フレ
		if (P1.time >= 1){

			// [ダメージ]
			Damage(0, 0);

			// [ゲージ] 
			Power(0);
			HitTime(6, 60, 60, 12);

			// [ノックバック]
			HitVel(-2.8, -4.7, -2.6, -3.5);
			GuardVel(-3.8, -2.6 * 1.2);
			P1.HitAnim = 1030;	// 空中喰らい
			// [喰らい中の浮き]
			P1.fallF = 2;
			// [ガード属性]
			P1.GuardF = 1;

			P1.HitSE = 13;
		}
		else {
			DamReset();
		}

		// 声
		if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 20, 0);

		// 地面にいる時
		if (P1.YPos + P1.YVel * 2 == GROUND){
			P1.stateno = 47, P1.More = 1, P1.time = 0;
		}

		break;
	}

	// 戻す
	PSetSend();
}


// ダメージ設定
void DamParam()
{
	// 共通設定

	switch (P1.stateno){
	case 1000:	// 立ち喰らい
		P1.ctrl = 0, P1.SFlg = 0;
		// 他の無敵がないなら、投げ無敵
		if(P1.Muteki == 0)P1.Muteki = 1, P1.mutekiF = 2;
		Furimuki();

		if ((P1.time == 0) && (P1.StopTime == 0)){
			n = GetRand(2);
			if (n == 0){
				PVOStart(P1.PSide, 31, 4);
			}
			else{
				PVOStart(P1.PSide, 32, 4);
			}
		}

		// 速度減少（仮）
		if (P1.time >= 8 && P1.XVel != 0){
			P1.XVel = P1.XVel * 0.62;
		}

		// 戻りへ
		if (P1.time >= P1.D.nokezori - 2){
			P1.stateno = 1005, P1.time = 0;
		}
		// 変更・屈
		if (P1.SFlg == 1){
			P1.stateno = 1020, P1.More = 1, P1.time = 0;
		}
		// 変更・空
		if ((P1.SFlg == 2) || (P1.YPos < GROUND)){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// 地面なら
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > GROUND){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// やられていたら
		if (P1.Life <= 0){
			P1.stateno = 1030,
				P1.time = 0, P1.More = 1;
		}
		break;
	case 1005:	// 上段戻り
		P1.ctrl = 0;
		P1.XVel = 0, P1.YVel = 0;
		if (P1.time >= 2){
			P1.stateno = 0, P1.time = 0;
			// 投げ無敵を付与
			P1.Muteki = ThrowMuteki_Hit, P1.mutekiF = 2;
			P1.ctrl = 1;
		}
		break;
	case 1010:	// 立ち喰らい(下)
		P1.ctrl = 0, P1.SFlg = 0;
		// 他の無敵がないなら、投げ無敵
		if (P1.Muteki == 0)P1.Muteki = 1, P1.mutekiF = 2;
		Furimuki();

		if ((P1.time == 0) && (P1.StopTime == 0)){
			n = GetRand(2);
			if (n == 0){
				PVOStart(P1.PSide, 31, 4);
			}
			else{
				PVOStart(P1.PSide, 32, 4);
			}
		}

		// 速度減少（仮）
		if (P1.time >= 8 && P1.XVel != 0){
			P1.XVel = P1.XVel * 0.62;
		}
		if (P1.time >= P1.D.nokezori - 2){
			P1.stateno = 1015, P1.time = 0;
		}
		// 変更・空
		if ((P1.SFlg == 2) || (P1.YPos < GROUND)){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// 地面なら
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > GROUND){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// やられていたら
		if (P1.Life <= 0){
			P1.stateno = 1030,
				P1.time = 0, P1.More = 1;
		}
		break;
	case 1015:	// 下段戻り
		P1.ctrl = 0;
		P1.XVel = 0, P1.YVel = 0;
		if (P1.time >= 2){
			P1.stateno = 0, P1.time = 0;
			// 投げ無敵を付与
			P1.Muteki = ThrowMuteki_Hit, P1.mutekiF = 2;
			P1.ctrl = 1;
		}
		break;
	case 1020:	// 屈喰らい
		P1.ctrl = 0, P1.SFlg = 1;
		// 他の無敵がないなら、投げ無敵
		if (P1.Muteki == 0)P1.Muteki = 1, P1.mutekiF = 2;
		Furimuki();

		if ((P1.time == 0) && (P1.StopTime == 0)){
			n = GetRand(2);
			if (n == 0){
				PVOStart(P1.PSide, 31, 4);
			}
			else{
				PVOStart(P1.PSide, 32, 4);
			}
		}

		if (P1.time >= P1.D.nokezori - 2){
			P1.stateno = 1025, P1.time = 0;
		}
		// 変更・空
		if ((P1.SFlg == 2) || (P1.YPos < GROUND)){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// 地面なら
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > GROUND){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// やられていたら
		if (P1.Life <= 0){
			P1.stateno = 1030,
				P1.time = 0, P1.More = 1;
		}
		break;
	case 1025:	// 戻り
		P1.ctrl = 0;
		if (P1.time >= 2){
			P1.stateno = 11, P1.time = 0;
			// 投げ無敵を付与
			P1.Muteki = ThrowMuteki_Hit, P1.mutekiF = 2;
			P1.ctrl = 1;
		}
		break;
	case 1030:	// 空中喰らい
		P1.ctrl = 0, P1.SFlg = 2;
		P1.ignoreG = false;
		Furimuki();

		if ((P1.time == 0) && (P1.StopTime == 0)){
			n = GetRand(2);
			if (n == 0){
				PVOStart(P1.PSide, 31, 4);
			}
			else{
				PVOStart(P1.PSide, 32, 4);
			}
		}

		//最初から下にいた場合、少し戻す
		if ((P1.time == 0) && (P1.YPos >= GROUND)){
			P1.YPos = GROUND - 1;
		}
		// 時間経過 で戻りへ
		if ((P1.time >= P1.D.nokezori)){
			P1.stateno = 1035, P1.time = 0, P1.More = 1;
		}

		// 壁バウンド
		if ((
			(P1.XPos <= GAMENHAJI) || (P1.XPos >= STAGE_WIDTH - GAMENHAJI) || 
			(P1.XPos <= S.ScroolX + GAMENHAJI + 1) || (P1.XPos >= S.ScroolX + SCREEN_W - GAMENHAJI - 1)
			)
			&& (P1.time > 0) && (P1.YPos < GROUND) && (P1.D.wall > 0 && P1.D.wall <= 2)
			&& (P1.StopTime == 0)){
			P1.stateno = 1046, P1.time = 0, P1.More = 1;
		}

		// 着地
		if ((P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel * 2) >= GROUND) && (P1.StopTime == 0)){
			// 大バウンド
			if (P1.D.bound == 1){
				P1.stateno = 1040, P1.time = 0, P1.More = 1;
			}
			// 小バウンド
			else if (P1.D.bound == 2){
				P1.stateno = 1041, P1.time = 0, P1.More = 1;
			}
			// ダウン
			else{
				// 着地かダウン判断
				if(P1.fall == 1 || P1.Life <= 0){
					P1.stateno = 1050, P1.time = 0, P1.More = 1;
				}
				else if (P1.fall == 0){
					P1.stateno = 47, P1.time = 0, P1.More = 1;
				}
				// 地上受け身可能
				else if (P1.fall == 2){
					// [受け身]
					// 空中にいる時、ボタン１～３入力
					if ((P1.button[1] > 0) || (P1.button[2] > 0) ||
						(P1.button[3] > 0)){
						P1.stateno = 1080, P1.time = 0, P1.More = 1;
					}
					else {
						P1.stateno = 1050, P1.time = 0, P1.More = 1;
					}
				}
			}
		}


		break;
	case 1035:	// 戻り
		P1.ctrl = 0, P1.SFlg = 2;

		/*
		// 着地した場合、ステート判断へ
		if ((P1.time >= 0) && (P1.YPos >= D_GROUND)){
			// １ならダウン、０なら着地
			if (P1.fall == 1){
				P1.stateno = 1050, P1.time = 0, P1.More = 1;
			}
			else if (P1.fall == 0){
				P1.stateno = 47, P1.time = 0, P1.More = 1;
			}
		}
		*/

		// 壁バウンド
		if ((
			(P1.XPos <= GAMENHAJI) || (P1.XPos >= STAGE_WIDTH - GAMENHAJI) ||
			(P1.XPos <= S.ScroolX + GAMENHAJI + 1) || (P1.XPos >= S.ScroolX + SCREEN_W - GAMENHAJI - 1)
			)
			&& (P1.time > 0) && (P1.YPos < GROUND) && (P1.D.wall > 0 && P1.D.wall <= 2)
			&& (P1.StopTime == 0)){
			P1.stateno = 1046, P1.time = 0, P1.More = 1;
		}

		// 着地
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel * 2) >= GROUND){
			// 大バウンド
			if (P1.D.bound == 1){
				P1.stateno = 1040, P1.time = 0, P1.More = 1;
			}
			// 小バウンド
			else if (P1.D.bound == 2){
				P1.stateno = 1041, P1.time = 0, P1.More = 1;
			}
			// ダウン
			else{
				// 着地かダウン判断
				if (P1.fall == 1 || P1.Life <= 0){
					P1.stateno = 1050, P1.time = 0, P1.More = 1;
				}
				else if (P1.fall == 0){
					P1.stateno = 47, P1.time = 0, P1.More = 1;
				}
				// 地上受け身可能
				else if (P1.fall == 2){
					// [受け身]
					// 空中にいる時、ボタン１～３入力
					if ((P1.button[1] > 0) || (P1.button[2] > 0) ||
						(P1.button[3] > 0)){
						P1.stateno = 1080, P1.time = 0, P1.More = 1;
					}
					else {
						P1.stateno = 1050, P1.time = 0, P1.More = 1;
					}
				}
			}
			break;
		}

		// [受け身]
		// 生きてる時に、
		// 空中にいる時、ボタン１～３入力
		if ((P1.Life > 0) &&
			(P1.time >= 0) && (P1.YPos < D_GROUND)){
			if ((P1.button[1] > 0) || (P1.button[2] > 0) ||
				(P1.button[3] > 0)){
				P1.stateno = 1085, P1.time = 0, P1.More = 1;
			}
		}

		// 体力が0の場合ダウンへ
		if ((P1.Life <= 0) && (P1.YPos >= D_GROUND)){
			P1.stateno = 1050, P1.time = 0, P1.More = 1,
				P1.Lock = 1, P1.XVel = 0, P1.YVel = 0;
		}

		break;

	case 1040:	// 床バウンド（小）、追撃不能
		P1.ctrl = 0, P1.SFlg = 2;
		P1.D.bound = 0;
		P1.D.boundCount = 2;
		P1.Muteki = 1, P1.mutekiF = 0;

		if (P1.time == 0){
			P1.XVel = P1.D.bound_xvel;
			P1.YVel = P1.D.bound_yvel;
			P1.YPos = D_GROUND - 2;
			P1.D.yaccel = GRAVITY;
			SEStart(8);
			EffStartB(19, P1.XPos, P1.YPos, 0, -7,
				0.25, 0.04, P1.muki);
		}

		// 着地でダウン
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > D_GROUND){
			// ダウン
			P1.stateno = 1050, P1.time = 0, P1.More = 1;
			// 地上受け身可能
			if (P1.fall == 2){
				// [受け身]
				// 空中にいる時、ボタン１～３入力
				if ((P1.button[1] > 0) || (P1.button[2] > 0) ||
					(P1.button[3] > 0)){
					P1.stateno = 1080, P1.time = 0, P1.More = 1;
				}
				else {
					P1.stateno = 1050, P1.time = 0, P1.More = 1;
				}
			}
		}
		break;

	case 1041:	// 床バウンド（大）
		P1.ctrl = 0, P1.SFlg = 2;
		P1.D.bound = -1;
		P1.D.boundCount = 1;
		P1.Muteki = 0, P1.mutekiF = 0;

		if (P1.time == 0){
			P1.XVel = P1.D.bound_xvel;
			P1.YVel = P1.D.bound_yvel;
			P1.YPos = D_GROUND - 2;
			P1.D.yaccel = GRAVITY;
			SEStart(8);
			EffStartB(16, P1.XPos, P1.YPos, 0, -7,
				0.25, 0.04, P1.muki);
		}

		// 着地でダウン
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > D_GROUND){
			// ダウン
			P1.stateno = 1050, P1.time = 0, P1.More = 1;
			// 地上受け身可能
			if (P1.fall == 2){
				// [受け身]
				// 空中にいる時、ボタン１～３入力
				if ((P1.button[1] > 0) || (P1.button[2] > 0) ||
					(P1.button[3] > 0)){
					P1.stateno = 1080, P1.time = 0, P1.More = 1;
				}
				else {
					P1.stateno = 1050, P1.time = 0, P1.More = 1;
				}
			}
		}
		break;
	
	case 1046:	// 壁バウンド（大）
		P1.ctrl = 0, P1.SFlg = 2;
		P1.Muteki = 0, P1.mutekiF = 0;

		if (P1.time == 0){
			/*
			P1.XVel = 0.4;
			P1.YVel = -6.4;
			P1.YPos = P1.YPos;
			*/
			P1.XVel = P1.D.wall_xvel;
			P1.YVel = P1.D.wall_yvel;
			P1.YPos = P1.YPos;
			
			P1.D.yaccel = 0.3;
			SEStart(8);
			P1.D.wall++;	// もう壁バンしない
			EffStartB(16, P1.XPos, P1.YPos, 0, - (P1.ySize / 1.5), 
				0.04, 0.25, P1.muki);
		}

		// 着地でダウン
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > D_GROUND){
			// ダウン
			P1.stateno = 1050, P1.time = 0, P1.More = 1;
		}

		break;
	case 1050:	// ダウン
		P1.ctrl = 0, P1.SFlg = 0;
		P1.Muteki = 1, P1.mutekiF = 0;
		P1.XVel = 0, P1.YVel = 0;
		P1.YPos = GROUND;
		// SE
		if (P1.time == 0 && P1.StopTime == 0){
			SEStart(8);
			EffStartB(16, P1.XPos, P1.YPos, 0, -1, 
				0.20, 0.04, P1.muki);
		}
		if (P1.time >= 20){
			// 起き上がり声
			if ((P1.time == 20) && (P1.Life <= 0) && (P1.LifeC > 0))VOStart(P1.Name, 34, 0);
			P1.stateno = 1055, P1.time = 0, P1.More = 1;
			// 完全にやられていたら
			if (P1.Life <= 0 && P1.LifeC <= 0){
				P1.stateno = 1070,
					P1.time = 0, P1.More = 1;
			}
		}

		// 相手もやられていた（ダブルKO）
		//if (P2.Life <= 0 && P2.LifeC <= 0)ModeChange(2);

		break;

	case 1055:	// 起き上がり
		P1.ctrl = 0, P1.Lock = 0;
		P1.Muteki = 1, P1.mutekiF = 0;
		P1.XVel = 0, P1.YVel = 0;
		P1.D.bound = 0, P1.D.wall = 0;
		// 体力回復等
		if (S.roundState == 2){
			
			//VOStart(P1.Name, 34, 0);
			/*
			P1.Life = P1.C.lifeMax;
			P2.Life = P2.C.lifeMax;
			P1.aGauge = GUARD_MAX;
			P2.aGauge = GUARD_MAX;
			P1.GRecovery = 0;
			P2.GRecovery = 0;
			P1.D.armor = 0;
			P2.D.armor = 0;
			*/

			// キャラ変数初期化
			for (int i = 0; i < VAR_MAX; i++){
				//if ((P1.Name == HELIOS) && (i == 11));
				//else if ((P1.Name == CORNEL) && (i == 20));
				{ P1.Var[i] = 0; }
				
				//if ((P2.Name == HELIOS) && (i == 11));
				//else if ((P2.Name == CORNEL) && (i == 20)); //コーネル強化要素
				{ P2.Var[i] = 0; }
				//P1.Var[i] = 0;
				//P2.Var[i] = 0;
			}
		}
		// 無敵付けながら立ちへ
		if (P1.time >= P1.animElem[1055]){
			P1.stateno = 0, P1.time = 0;
			//P1.More = 0;
			P1.More = 1;
			P1.ctrl = 1;
			//P1.Muteki = 3, P1.mutekiF = 0;
			P1.Muteki = ThrowMuteki_Down, P1.mutekiF = 2;
		}

		
		break;
	case 1060:	// ロック技喰らい
		P1.ctrl = 0, P1.SFlg = 0;
		P1.ignoreG = true;

		// 一瞬むてき
		if (P1.time <= 1){
			P1.Muteki = 1;
			P1.mutekiF = 0;
		}
		
		// 相手と相打ち
		if (P2.stateno >= 1000){
			P1.stateno = 1000, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 30;
		}
		// バグで動ける
		else if (P2.stateno < 200 && P1.time > 30){
			P1.stateno = 1000, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 10;
		}

		// 宇宙旅行しすぎた場合or地面に埋まる
		if ((P1.YPos < -100) || (P1.YPos > GROUND + 30)){
			P1.stateno = 1030, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 30;
		}
		break;
	case 1065:	// ロック技喰らい（空中）
		P1.ctrl = 0, P1.SFlg = 2;
		P1.ignoreG = true;
		
		// 相手と相打ち
		if (P2.stateno >= 1000){
			P1.stateno = 1030, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 30;
		}
		// バグで動ける
		else if (P2.stateno < 200 && P1.time > 30){
			P1.stateno = 1030, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 10;
		}

		// 宇宙旅行しすぎた場合
		if ((P1.YPos < -50) || (P1.YPos > GROUND + 30)){
			P1.stateno = 1030, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 30;
		}
		break;

	case 1070:	// 敗北
		P1.ctrl = 0, P1.Lock = 0, 
		P1.Muteki = 1, P1.mutekiF = 0;
		P1.XVel = 0;

		break;

	case 1080:	// 地上受け身
		P1.ctrl = 0,
		P1.Muteki = 1, P1.mutekiF = 0;
		P1.SFlg = 2;
		P1.ignoreG = false;
		P1.D.bound = 0, P1.D.wall = 0;

		if (P1.time == 0){
			P1.colorCTime = 7;
			P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			P1.D.yaccel = GRAVITY;
		}

		if (P1.time == 0){
			// 受け身方向は固定
			P1.XVel = 0;
			P1.YPos = GROUND;
			EffStartB(16, P1.XPos, P1.YPos, 0, -1,
				0.20, 0.04, P1.muki);
		}

		// 音を鳴らす
		if (P1.time == 0)SEStart(5), P1.YVel = -4.6;

		// 着地
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel * 2) >= GROUND){
			P1.stateno = 47, P1.time = 0, P1.More = 1;
			// 投げ無敵付与
			P1.mutekiF = 2;
			P1.Muteki = 10;
		}
		break;

	case 1085:	// 空中受け身
		P1.ctrl = 0, 
		P1.Muteki = 1, P1.mutekiF = 0;
		P1.SFlg = 2;
		P1.ignoreG = false;
		P1.D.bound = 0, P1.D.wall = 0;

		if (P1.time == 0){
			P1.D.yaccel = GRAVITY;
			P1.colorCTime = 8;
			P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			P1.XVel = 0;
			P1.AirJump = 1;
			if(P1.C.airDash > 0)P1.airDash = 1;
		}

		// 地面に埋まってたら
		if (P1.YPos > GROUND)P1.YPos = GROUND;

		if (P1.time == 0){
			// 受け身方向を選ぶ
			if (P1.keyAtt[4]){
				P1.XVel = -2.2;
			}
			else if (P1.keyAtt[6]){
				P1.XVel = 2.2;
			}
			else { P1.XVel = 0; }
		}

		// 音を鳴らす
		if (P1.time == 1)SEStart(5), P1.YVel = -4.8;
		if (P1.time >= 16){
			// ジャンプ状態
			P1.stateno = 46, P1.Muteki = 0,
				P1.time = 0, P1.More = 0;
			P1.ctrl = 1;
		}
		break;

	}

	// 戻す
	PSetSend();
}

// 振り向く
void Furimuki()
{
	// 振り向き
	if ((P1.time == 0) && (P1.muki == P2.muki)){
		if (P2.muki == 1){
			P1.muki = 0;
		}
		else if (P2.muki == 0){
			P1.muki = 1;
		}
	}
}

static void PSetSend()
{
	// 戻す
	if (P1.Name == EIN){
		GetP_EinParam(P1, P2);
		GetH_EinParam(H1, H2);
		GetS_EinParam(S);
	}
	else if (P1.Name == CORNEL){
		GetP_CorParam(P1, P2);
		GetH_CorParam(H1, H2);
		GetS_CorParam(S);
	}
	else if (P1.Name == BOUNCER){
		GetP_BouParam(P1, P2);
		GetH_BouParam(H1, H2);
		GetS_BouParam(S);
	}
	else if (P1.Name == HELIOS){
		GetP_HelParam(P1, P2);
		GetH_HelParam(H1, H2);
		GetS_HelParam(S);
	}
	else if (P1.Name == HYDE){
		GetP_HydParam(P1, P2);
		GetH_HydParam(H1, H2);
		GetS_HydParam(S);
	}
	else { 
		GetP_HydParam(P1, P2); 
		GetH_HydParam(H1, H2);
		GetS_HydParam(S);
	}
}

// 順番を逆にする
void GetP_SameParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_SameParam(System_t GS)
{
	S = GS;
}

void GetH_SameParam(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}

// Nameを取得、ついでに読み込み
void GetN_SameParam(std::vector<Name_t>GN)
{
	// デフォルト
	St.push_back(vector<State>());
	Load_StateDef(-1);
	for (int i = 0; i < Character_Max(); i++){
		N.push_back(GN[i]);
		St.push_back(vector<State>());
		Load_StateDef(i);
	}
}

