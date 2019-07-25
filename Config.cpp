#include "pch.h"
//
//static int Load_1;
//static int P1_Key, P2_Key, Gamen;
//static int inputUP, inputDOWN;
//static int TimeStop, kettei, kettei2;
//static int SentakuNum, SentakuYoko;
//// 変更するボタン、最大ボタン数
//static int henkou, henkouMax;
//static int henkou2, henkouMax2;
//
//static int SNum[256], SNum2[256];
//static int Load1, Load2;
//static int owari;
//// 決定
//
//// 選択ポインタ
//static int SPoint;
//// ポインタ縦
//static int TPoint;
//// 画像
//static int cursor;
//
//// コンフィグのメイン関数
//int Config()
//{
//	if(Load_1 == 0){
//		//描画先を裏画面に
//		SetDrawScreen(DX_SCREEN_BACK);
//
//		// 選択ポインタの位置初期化
//		SPoint = 0;
//
//		// ボタン数値初期化
//		henkou = 1, henkouMax = BUTTON_MAX;
//		henkou2 = 1, henkouMax2 = BUTTON_MAX;
//
//		// 画像読み込み
//		cursor = LoadGraph("OB/cursor.png");
//
//		// 選択項目の数を取得
//		SentakuNum = 2, SentakuYoko = 0;
//		kettei = 0, kettei2 = 0;
//		owari = 0;
//
//		kettei += 1;
//
//		Load_1 = 1;
//	}
//
//	// ループ、プログラム実行開始
//	{
//
//		// 入力状態ゲット
//		P1_Key = P_Input(1);
//		P2_Key = P_Input(2);
//
//		// キー入力準備
//		if (P1_Key){
//			kettei += 1;
//		}
//		else{
//			kettei = 0;
//		}
//		if (P2_Key){
//			kettei2 += 1;
//		}
//		else{
//			kettei2 = 0;
//		}
//
//		// 項目を選択
//		if (((P1_Key & PAD_INPUT_UP) != 0) || 
//			((P2_Key & PAD_INPUT_UP) != 0)){
//			inputUP += 1;
//		}
//		else{ inputUP = 0; }
//		if (((P1_Key & PAD_INPUT_DOWN) != 0) ||
//			((P2_Key & PAD_INPUT_DOWN) != 0)){
//			inputDOWN += 1;
//		}
//		else{ inputDOWN = 0; }
//
//		if (inputUP == 1){
//			SPoint -= 1;
//		}
//		if (inputDOWN == 1){
//			SPoint += 1;
//		}
//		if (SPoint < 0)SPoint = 0;
//		if (SPoint > 2)SPoint = 2;
//
//
//		// 範囲の限定
//		if (SPoint < 0)SPoint = 0;
//		if (SPoint >= 2)SPoint = 2;
//		
//		// 終了
//		if (((P1_Key & PAD_INPUT_1) != 0) ||
//			((P2_Key & PAD_INPUT_1) != 0)){
//			owari += 1;
//		}
//		else{
//			owari = 0;
//		}
//
//
//		// P1 キーコンフィグへ
//		// 最初だけ数値入れておく
//		if (Load1 == 0){
//			for (int i = 1; i <= henkouMax; i++){
//				SNum[i] = i;
//				if (OutSend(i) != 0)SNum[i] = OutSend(i);
//			}
//			Load1 = 1;
//		}
//		if ((kettei == 1 ) && (SPoint == 0)){
//			if ((kettei == 1) && (henkou <= henkouMax)){
//				if ((P1_Key & PAD_INPUT_1) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 1);
//					henkou += 1;
//					OutNew(henkou, 1);
//				}
//				else if ((P1_Key & PAD_INPUT_2) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 2);
//					henkou += 1;
//					OutNew(henkou, 2);
//				}
//				else if ((P1_Key & PAD_INPUT_3) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 3);
//					henkou += 1;
//					OutNew(henkou, 3);
//				}
//				else if ((P1_Key & PAD_INPUT_4) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 4);
//					henkou += 1;
//					OutNew(henkou, 4);
//				}
//				else if ((P1_Key & PAD_INPUT_5) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 5);
//					henkou += 1;
//					OutNew(henkou, 5);
//				}
//				else if ((P1_Key & PAD_INPUT_6) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 6);
//					henkou += 1;
//					OutNew(henkou, 6);
//				}
//				else if ((P1_Key & PAD_INPUT_7) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 7);
//					henkou += 1;
//					OutNew(henkou, 7);
//				}
//				else if ((P1_Key & PAD_INPUT_8) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 8);
//					henkou += 1;
//					OutNew(henkou, 8);
//				}
//				else if ((P1_Key & PAD_INPUT_9) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 9);
//					henkou += 1;
//					OutNew(henkou, 9);
//				}
//				else if ((P1_Key & PAD_INPUT_10) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 10);
//					henkou += 1;
//					OutNew(henkou, 10);
//				}
//				else if ((P1_Key & PAD_INPUT_11) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 11);
//					henkou += 1;
//					OutNew(henkou, 11);
//				}
//				else if ((P1_Key & PAD_INPUT_12) != 0){
//					SNum[henkou] = P1_BConfig(henkou, 12);
//					henkou += 1;
//					OutNew(henkou, 12);
//				}
//			}
//		}// P1 終了
//
//		// P2 キーコンフィグへ
//		// 最初だけ数値入れておく
//		if (Load2 == 0){
//			for (int i = 1; i <= henkouMax2; i++){
//				SNum2[i] = i;
//				if (OutSend(i+10) != 0)SNum2[i] = OutSend(i + 10);
//			}
//			Load2 = 1;
//		}
//		if ((kettei2 == 1) && (SPoint == 1)){
//			
//			if ((kettei2 == 1) && (henkou2 <= henkouMax2)){
//				if ((P2_Key & PAD_INPUT_1) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 1);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 1);
//				}
//				else if ((P2_Key & PAD_INPUT_2) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 2);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 2);
//				}
//				else if ((P2_Key & PAD_INPUT_3) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 3);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 3);
//				}
//				else if ((P2_Key & PAD_INPUT_4) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 4);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 4);
//				}
//				else if ((P2_Key & PAD_INPUT_5) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 5);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 5);
//				}
//				else if ((P2_Key & PAD_INPUT_6) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 6);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 6);
//				}
//				else if ((P2_Key & PAD_INPUT_7) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 7);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 7);
//				}
//				else if ((P2_Key & PAD_INPUT_8) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 8);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 8);
//				}
//				else if ((P2_Key & PAD_INPUT_9) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 9);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 9);
//				}
//				else if ((P2_Key & PAD_INPUT_10) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 10);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 10);
//				}
//				else if ((P2_Key & PAD_INPUT_11) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 11);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 11);
//				}
//				else if ((P2_Key & PAD_INPUT_12) != 0){
//					SNum2[henkou2] = P2_BConfig(henkou2, 12);
//					henkou2 += 1;
//					OutNew(henkou2 + 10, 12);
//				}
//			}
//		}// P2 終了
//
//		// 終了
//		if ((SPoint == 2) && (owari == 1)){
//			// おしっぱを拒否
//			P1_BCheck();
//			// 終了、メニューへ
//			ModeChange(GameScene(MenuScene));
//		}
//		
//
//		// 描画
//		{
//			if (SPoint == 0){
//				DrawString(0, 0, "ボタン設定 1P", Cr);
//			}
//			else if(SPoint == 1){
//				DrawString(0, 0, "ボタン設定 2P", Cr);
//			}
//			else if (SPoint == 2){
//				DrawString(0, 0, "１ボタン（Ｚキー）で終了", Cr);
//			}
//			if ((SPoint == 0) || (SPoint == 1)){
//				DrawString(0, 33 * 1, "Ａ(弱)", Cr);
//				DrawString(0, 33 * 2, "Ｂ(中)", Cr);
//				DrawString(0, 33 * 3, "Ｃ(強)", Cr);
//				DrawString(0, 33 * 4, "Ｄ(特殊)", Cr);
//				DrawString(0, 33 * 5, "Ａ+Ｂ(投げ)", Cr);
//				DrawString(0, 33 * 6, "Ａ+Ｂ+Ｃ(3ゲージ)", Cr);
//				DrawString(0, 33 * 7, "Ｐ(ポーズ)", Cr);
//				DrawString(0, 33 * 8, "ＦＮ(2P操作)", Cr);
//			}
//			DrawString(0, 400, "上下キーで項目を変更できます", Cr);
//
//		
//
//			if (SPoint == 1){
//				//	 2P
//				for (int i = 1; i <= henkouMax2; i++){
//					DrawFormatString(320, (i * 33), GetColor(255, 255, 255), "%d", SNum2[i]);
//				}
//			}
//			else if(SPoint == 0){
//				//	 1P
//				for (int i = 1; i <= henkouMax; i++){
//					DrawFormatString(320, (i * 33), GetColor(255, 255, 255), "%d", SNum[i]);
//				}
//			}
//			//DrawString(33, 66, "メニューに戻る", Cr);
//
//			// カーソル描画
//			if (SPoint == 0){
//				DrawBox(320 + 30, (henkou * 33), 320 + 50, (henkou * 33) + 20, Cr, true);
//			}
//			if (SPoint == 1){
//				DrawBox(320 + 30, (henkou2 * 33), 320 + 50, (henkou2 * 33) + 20, Cr, true);
//			}
//
//			
//		}
//
//
//	}//全体のループ
//
//	return 0;
//}
//
//void Load_Config()
//{
//	Load_1 = 0;
//}