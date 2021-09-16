//インクルード部--------------------------------------------
#include "pch.h"

static Player P[2];

void DamSetting()
{
	P[0] = P1, P[1] = P2;

	for (int i = 0; i < 2; i++){
		if (i == 0)P[0] = P1, P[1] = P2;
		else if (i == 1){
			Player p = P[0];
			P[0] = P[1], P[1] = p;
		}
		//----------
		// トレモ用
		//----------
		if (S.TraningFlg){
			// 死なない
			if (P[0].Life <= 0)P[0].Life = 1;
		}
		//-------------------
		// キャンセル判定
		// ヒット数カウント
		// 無敵時間変更
		//-------------------

		//[ キャンセル判定 ]
		// ヒットしてない時はキャンセルをできなくする
		if ((P[0].time <= 1) && (P[0].stateno >= 200)){
			P[0].CFlg = 0;
		}

		// 1000 以降（喰らい）のステートなら、ヒット数をカウント
		// 2P 喰らってるか確認
		if ((P[1].stateno >= 1000) && (P[1].ctrl == 0)){
			P[1].HFlg = 1;
		}
		// 死亡
		else if (P[1].Life <= 0){
			P[1].HFlg = 0;
			P[1].D.boundCount = 0;
			P[0].HitCount = 0;
			P[0].A.hosei_K = 1;
		}
		// 2Pの体力があって、喰らいではない
		else{
			P[1].HFlg = 0;
			P[1].D.boundCount = 0;
			P[1].D.wall = 0;

			P[0].HitCount = 0;
			// ガードモーションじゃない
			if ((P[1].stateno < 50) || (P[1].stateno >= 60))P[0].A.guardCount = 0;
			P[0].A.hosei_K = 1;	// 強制補整を1に戻す
			P[0].A.gaugeHosei = false; // ゲージ補正を停止
		}

	}
	P1 = P[1], P2 = P[0];

	
	P[0] = P1, P[1] = P2;
	for (int n = 0; n < 2; n++){
		// [ カウンターヒット ]
		if (P[n].stateno >= 200 && P[n].stateno <= 999){
			// 強制カウンター以外はカウンター判定発生
			if ((P[n].time == 0) && (P[n].D.counter != 2))
				P[n].D.counter = 1;

			// 強制カウンターなら終了
			if (P[n].D.counter == 2)break;

			// 攻撃判定が出たらカウンター終了
			for (int i = 0; i < ATT_MAX; i++){
				if ((P[n].WAtt[i] > 0) && (P[n].HAtt[i] > 0)){					
					P[n].D.counter = 0;
					P[n].D.fatal = 0;
				}
			}
		}
		// 技じゃないならカウンターしない
		else{ 
			P[n].D.counter = 0; 
			P[n].D.fatal = 0;
		}

		// [スーパーアーマー]
		if (P[n].D.armor < 0)P[n].D.armor = 0;

		// スパキャン猶予
		if ((P[n].scTime > 0) && (P[n].StopTime == 0))P[n].scTime--;
		if ((P[n].A.ncTime > 0) && (P[n].StopTime == 0))P[n].A.ncTime--;
	}
	P1 = P[0], P2 = P[1];
	
	// 値を返す
	//
	Get_PSet(P1, P2);
}

void GetP_SubDamSet(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_SubDamSet(System_t GS)
{
	S = GS;
}
