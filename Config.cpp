#include "pch.h"
//
//static int Load_1;
//static int P1_Key, P2_Key, Gamen;
//static int inputUP, inputDOWN;
//static int TimeStop, kettei, kettei2;
//static int SentakuNum, SentakuYoko;
//// �ύX����{�^���A�ő�{�^����
//static int henkou, henkouMax;
//static int henkou2, henkouMax2;
//
//static int SNum[256], SNum2[256];
//static int Load1, Load2;
//static int owari;
//// ����
//
//// �I���|�C���^
//static int SPoint;
//// �|�C���^�c
//static int TPoint;
//// �摜
//static int cursor;
//
//// �R���t�B�O�̃��C���֐�
//int Config()
//{
//	if(Load_1 == 0){
//		//�`���𗠉�ʂ�
//		SetDrawScreen(DX_SCREEN_BACK);
//
//		// �I���|�C���^�̈ʒu������
//		SPoint = 0;
//
//		// �{�^�����l������
//		henkou = 1, henkouMax = BUTTON_MAX;
//		henkou2 = 1, henkouMax2 = BUTTON_MAX;
//
//		// �摜�ǂݍ���
//		cursor = LoadGraph("OB/cursor.png");
//
//		// �I�����ڂ̐����擾
//		SentakuNum = 2, SentakuYoko = 0;
//		kettei = 0, kettei2 = 0;
//		owari = 0;
//
//		kettei += 1;
//
//		Load_1 = 1;
//	}
//
//	// ���[�v�A�v���O�������s�J�n
//	{
//
//		// ���͏�ԃQ�b�g
//		P1_Key = P_Input(1);
//		P2_Key = P_Input(2);
//
//		// �L�[���͏���
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
//		// ���ڂ�I��
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
//		// �͈͂̌���
//		if (SPoint < 0)SPoint = 0;
//		if (SPoint >= 2)SPoint = 2;
//		
//		// �I��
//		if (((P1_Key & PAD_INPUT_1) != 0) ||
//			((P2_Key & PAD_INPUT_1) != 0)){
//			owari += 1;
//		}
//		else{
//			owari = 0;
//		}
//
//
//		// P1 �L�[�R���t�B�O��
//		// �ŏ��������l����Ă���
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
//		}// P1 �I��
//
//		// P2 �L�[�R���t�B�O��
//		// �ŏ��������l����Ă���
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
//		}// P2 �I��
//
//		// �I��
//		if ((SPoint == 2) && (owari == 1)){
//			// �������ς�����
//			P1_BCheck();
//			// �I���A���j���[��
//			ModeChange(GameScene(MenuScene));
//		}
//		
//
//		// �`��
//		{
//			if (SPoint == 0){
//				DrawString(0, 0, "�{�^���ݒ� 1P", Cr);
//			}
//			else if(SPoint == 1){
//				DrawString(0, 0, "�{�^���ݒ� 2P", Cr);
//			}
//			else if (SPoint == 2){
//				DrawString(0, 0, "�P�{�^���i�y�L�[�j�ŏI��", Cr);
//			}
//			if ((SPoint == 0) || (SPoint == 1)){
//				DrawString(0, 33 * 1, "�`(��)", Cr);
//				DrawString(0, 33 * 2, "�a(��)", Cr);
//				DrawString(0, 33 * 3, "�b(��)", Cr);
//				DrawString(0, 33 * 4, "�c(����)", Cr);
//				DrawString(0, 33 * 5, "�`+�a(����)", Cr);
//				DrawString(0, 33 * 6, "�`+�a+�b(3�Q�[�W)", Cr);
//				DrawString(0, 33 * 7, "�o(�|�[�Y)", Cr);
//				DrawString(0, 33 * 8, "�e�m(2P����)", Cr);
//			}
//			DrawString(0, 400, "�㉺�L�[�ō��ڂ�ύX�ł��܂�", Cr);
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
//			//DrawString(33, 66, "���j���[�ɖ߂�", Cr);
//
//			// �J�[�\���`��
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
//	}//�S�̂̃��[�v
//
//	return 0;
//}
//
//void Load_Config()
//{
//	Load_1 = 0;
//}