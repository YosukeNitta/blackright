//==================================================================================
//
//			�|�[�Y��ʂ̊Ǘ���
//
//========================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"

//�f�t�@�C����----------------------------------------------




//�ް���`��------------------------------------------------

static int Load_1;
static int P1_Key;
static const int TxtDist = 29;
static const int drawMax = 15;	// �|�[�Y�\���ő�
static int drawPos;
static int SPoint;	// �I���|�C���^�̈ʒu������
static int TimeStop, kettei, kettei2;
static int inputUP, inputDOWN;
static int SentakuNum;
static int selectMax = 12;

static int page;
static const int page_max = 2;
static const int pageBoxSize = SCREEN_H - 40 - 40;

static Player P[2];

// ���ʁA�@�R�}���h�\�ƃg�����ݒ�
static int ComList;

//�ʏ탁�j���[
static int SNum;

// �g�����ݒ�
static int column, row;
static int c_Max, r_Max;

// ���̍���
static int nx;

// �R�}���h
static int gK[10], gB[4], gP;

// �摜
//static int cursor;

// raw��ύX���Ȃ�
static boolean noChangeRow;


//�����֐��錾��--------------------------------------------

static void CommandList();
static void TraningSet();
static void TrainingList();
//void BackGraph();	// �w�i�`��
static void BackGraphT();

//�v���O������----------------------------------------------



int Pause()
{

	if (Load_1 == 0){

		// �I�����ڂ̐����擾
		kettei = 0, SPoint = 0,
		SentakuNum = 5;

		//�ʏ탁�j���[
		SNum = 3;

		// ���ڂ̍ő吔
		selectMax = 12;
		// �\���ʒu
		drawPos = 0;

		// �g����
		column = 0, row = 0;
		c_Max = 0, r_Max = 0;
		inputUP = 0, inputDOWN = 0;
		ComList = 0;

		// �摜�ǂݍ���
		// Load_Pause�ōs��

		Load_1 = 1;
	}


	{
		// ���͂͂����Ń`�F�b�N
		//P1_Key = P_Input(1);	// OK
	}

	// ���[�v�A�v���O�������s�J�n
	// [ ���ʐݒ�]
	{
		
		// 2P���g����
		if (!S.TraningFlg){
			// ����
			if ((P_BInput(1) == 1 || P_BInput(3) == 1) && (kettei != 1))
			{
				kettei = 1;
			}
			else{ kettei = 0; }

			// ��
			if (KInput(P_BInput(108))){
				inputUP += 1;
			}
			else{ inputUP = 0; }

			// ��
			if (KInput(P_BInput(102))){
				inputDOWN += 1;
			}
			else{ inputDOWN = 0; }
		}
		// �g�����p
		else{
			// ����
			if ((P1_BInput(1) == 1 || P1_BInput(3) == 1) && (kettei != 1))
			{
				kettei = 1;
			}
			else{ kettei = 0; }

			// ��
			if (KInput(P1_BInput(108))){
				inputUP += 1;
			}
			else{ inputUP = 0; }

			// ��
			if (KInput(P1_BInput(102))){
				inputDOWN += 1;
			}
			else{ inputDOWN = 0; }

			
		}

		// ���肵�Ă���J�E���g���Z
		if (kettei == 1){
			TimeStop += 1;
		}
		else{
			TimeStop = 0;
		}

	}

	// [�g�������j���[]
	if (S.TraningFlg)
	{
		
		// ��{
		if (ComList == 0){

			/**************
			* �w�i
			**************/
			if (P1_BInput(4) == 0)
				// �w�i
				{
					BackGraphT();
				}

				

				// �J�[�\���ړ�
				if (kettei == 0){
					if (inputUP == 1) SPoint--, SEStart(36);
					if (inputDOWN == 1) SPoint++, SEStart(36);
					if (SPoint < 0)SPoint = SentakuNum + selectMax - 1;
					if (SPoint > SentakuNum + selectMax - 1)SPoint = 0;
				}
				
				// �g�����ݒ�
				TraningSet();

				// ����L�[����
				// �ʒu������
				if ((kettei == 1) && (SPoint == selectMax))
				{
					kettei = 0;
					return 1;
				}
				// �R�}���h�\
				else if ((kettei == 1) && (SPoint == selectMax + 1))
				{
					kettei = 0;
					ComList = 1;
				}
				// �ΐ��ʂɖ߂�
				else if (((kettei == 1) && (SPoint == selectMax + 2)) || (P1_BInput(2) == 1) || (P1_BInput(7) == 1))
				{
					kettei = 0;
					return 4;
				}
				// �L�����Z���ɖ߂�
				else if ((kettei == 1) && (SPoint == selectMax + 3))
				{
					kettei = 0;
					return 2;
				}
				// ���j���[�ɖ߂�
				else if ((kettei == 1) && (SPoint == selectMax + 4))
				{
					kettei = 0;
					Load_1 = 0;
					return 3;
				}
				

				// �`��
				if (P1_BInput(4) == 0)
				{
					// �J�[�\���`��
					{
						// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
						// �J�[�\���`��
						DrawBox(0, (SPoint * TxtDist) + 29 + (drawPos * TxtDist), 
							SCREEN_W, (SPoint * TxtDist) + 48 + (drawPos * TxtDist), GetColor(20, 20, 250), true);
						NoBlend();
					}

					// �ݒ�f�[�^�`��
					{
						TrainingList();
						//DrawString(0, 0, "D�������ςō��ڂ�����", Cr);
						DrawString(TxtDist, TxtDist * (selectMax + 1) + (drawPos * TxtDist), "�|�W�V�������Z�b�g", Cr);
						DrawString(TxtDist, TxtDist * (selectMax + 2) + (drawPos * TxtDist), "�R�}���h�\", Cr);
						DrawString(TxtDist, TxtDist * (selectMax + 3) + (drawPos * TxtDist), "�퓬��ʂ֖߂�", Cr);
						
						if (drawPos < 0)	// ���炷
						DrawString(TxtDist, TxtDist * (selectMax + 4) + (drawPos * TxtDist), "�L�����N�^�[�Z���N�g�ɖ߂�", Cr);
						if (drawPos < -1)	// ���炷
							DrawString(TxtDist, TxtDist * (selectMax + 5) + (drawPos * TxtDist), "���j���[�ɖ߂�", Cr);

						// ���ڈʒu�̎l�p
						if (drawPos == 0)
							DrawBox(SCREEN_W - 20, 20, SCREEN_W, 20 + pageBoxSize, Cr, true);
						else if (drawPos == -1)
							DrawBox(SCREEN_W - 20, 40, SCREEN_W, 40 + pageBoxSize, Cr, true);
						else if (drawPos == -2)
							DrawBox(SCREEN_W - 20, SCREEN_H - 20 - pageBoxSize, SCREEN_W, SCREEN_H - 20, Cr, true);
					}
					// �J�[�\���`��
					//DrawBox(5, (SPoint * TxtDist) + 29, 24, (SPoint * TxtDist) + 48, Cr, true);
					
				}
			}
			else if(ComList == 1){
				CommandList();
			}

	}//�g�������j���[�I���A
	// �ʏ탁�j���[
	else{

		if (ComList == 0){
			/**************
			* �w�i
			**************/
			if (P_BInput(4) == 0)
			{
				BackGraphT();
			}

			// �J�[�\���ړ�
			if (kettei == 0){
				if (inputUP == 1) SPoint--, SEStart(36);
				if (inputDOWN == 1) SPoint++, SEStart(36);
				if (SPoint < 0)SPoint = SNum - 1;
				if (SPoint > SNum - 1)SPoint = 0;

			}

			// ����L�[����
			// ��ʂɖ߂�
			if (((kettei == 1) && (SPoint == 0)) || (P_BInput(2) == 1) || (P_BInput(7) == 1))
			{
				kettei = 0;
				return 4;
			}
			// �R�}���h�\
			else if ((kettei == 1) && (SPoint == 1))
			{			
				kettei = 0;
				ComList = 1;
			}
			// ���j���[�ɖ߂�
			else if ((kettei == 1) && (SPoint == 2))
			{
				kettei = 0;
				Load_1 = 0;
				return 3;
			}

			// ���j���[�ɖ߂�
			if (Replay_Mode(-1) == 1)
			{
				kettei = 0;
				Load_1 = 0;
				Replay_End();
				return GameScene(ReplayScene);
			}

			// �`��
			if (P_BInput(4) == 0)
			{
				
				// �J�[�\���`��
				{
					// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
					// �J�[�\���`��
					DrawBox(0, (SPoint * TxtDist) + 29, SCREEN_W, (SPoint * TxtDist) + 48, GetColor(20, 20, 250), true);
					NoBlend();
				}
				//DrawString(0, 0, "D�������ςō��ڂ�����", Cr);
				DrawString(TxtDist, TxtDist * 1, "�ΐ��ʂɖ߂�", Cr);
				DrawString(TxtDist, TxtDist * 2, "�R�}���h�\", Cr);
				DrawString(TxtDist, TxtDist * 3, "���j���[�ɖ߂�", Cr);
				
			}

		}
		else if (ComList == 1){
			CommandList();
		}
	}// �ΐ탁�j���[�I��

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
	* �v�Z
	**************/
	{
		{
			// ���E�̓���
			if ((P1_BInput(104) == 1) || (P1_BInput(106) == 1)){
				if (P1_BInput(104) == 1)row -= 1;
				if (P1_BInput(106) == 1)row += 1;
			}
			else{ row = 0; }

			if (row == 1 || row == -1)SEStart(35);

			// �C�L�X�M�B�I
			
			//if ((P1_BInput(104) == 0) && (P1_BInput(106) == 0)){

			// ���[�h�����ŏ��͕ς��Ȃ��i������ςł����̖h�~�j
			if (noChangeRow){
				row = 0;
				noChangeRow = false;
			}

			// �`��ʒu
			if ((SPoint > drawMax - 1) && (drawPos > -1))drawPos = -1;
			else if ((SPoint > drawMax) && (drawPos > -2))drawPos = -2;
			
			if ((SPoint == 0) && (drawPos != 0))drawPos = 0;
			else if ((SPoint == 1) && (drawPos < -1))drawPos = -1;
		}

		{
			nx = 0;
			// �̗�
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
			// �Q�[�W
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
			// �K�[�h�Q�[�W
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
			// �A�h�Q�[�W
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
			// �̐�
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
			// �󂯐g
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
			// �K�[�h
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
			// �J�E���^�[
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
			// ��������
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
			// ���ڕ\��
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
			// ����
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
			// �Đ����@
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
	* ���M
	**************/
	{
		Get_SSet(S);
	}
}

void TrainingList()
{
	/**************
	* �`��
	**************/
	
		nx = 0 + drawPos;

		nx++;
		
		if (drawPos >= 0) {
			// ���炷
			DrawString(30, TxtDist * nx, "�̗�", Cr);
			if (S.T_Life == 0)DrawString(160, TxtDist * nx, "������", Oran);
			else { DrawString(160, TxtDist * nx, "�ʏ�", Cr); }
			
		}
		
		nx++;
		
		if (drawPos >= -1) {
			DrawString(30, TxtDist * nx, "�p���[�Q�[�W", Cr);
			if (S.T_Power == 0)DrawString(160, TxtDist * nx, "������", Oran);
			else if (S.T_Power == 1) { DrawString(160, TxtDist * nx, "1�Q�[�W", Cr); }
			else if (S.T_Power == 2) { DrawString(160, TxtDist * nx, "2�Q�[�W", Cr); }
			else if (S.T_Power == 3) { DrawString(160, TxtDist * nx, "�ʏ�", Cr); }
		}

		nx++;

		DrawString(30, TxtDist * nx, "�K�[�h�Q�[�W", Cr);
		if (S.T_GGauge == 0)DrawString(160, TxtDist * nx, "������", Oran);
		else if (S.T_GGauge == 1){ DrawString(160, TxtDist * nx, "�ʏ�", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "�A�h�Q�[�W", Cr);
		if (S.T_AddGauge == 0)DrawString(160, TxtDist * nx, "�ʏ�", Oran);
		else if (S.T_AddGauge == 1){ DrawString(160, TxtDist * nx, "MAX", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "�̐�", Cr);
		if (S.TSwitch[2] == 0)DrawString(160, TxtDist * nx, "����", Oran);
		else if (S.TSwitch[2] == 1)DrawString(160, TxtDist * nx, "���Ⴊ��", Cr);
		else if (S.TSwitch[2] == 2)DrawString(160, TxtDist * nx, "�W�����v", Cr);
		else if (S.TSwitch[2] == 3){ DrawString(160, TxtDist * nx, "�v���C���[����", Cr); }
		else { DrawString(160, TxtDist * nx, "�R���s���[�^����", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "�󂯐g", Cr);
		if (S.TSwitch[3] == 0)DrawString(160, TxtDist * nx, "�j���[�g����", Oran);
		else if (S.TSwitch[3] == 1) { DrawString(160, TxtDist * nx, "�O��", Cr); }
		else if (S.TSwitch[3] == 2) { DrawString(160, TxtDist * nx, "���", Cr); }
		else if (S.TSwitch[3] == 3) { DrawString(160, TxtDist * nx, "�Ȃ�", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "�K�[�h", Cr);
		if (S.TSwitch[4] == 0)DrawString(160, TxtDist * nx, "���Ȃ�", Oran);
		else if (S.TSwitch[4] == 1) { DrawString(160, TxtDist * nx, "����", Cr); }
		else if (S.TSwitch[4] == 2) { DrawString(160, TxtDist * nx, "�����_��", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "�J�E���^�[", Cr);
		if (S.T_Counter == 0)DrawString(160, TxtDist * nx, "�ʏ�", Oran);
		else if (S.T_Counter == 1) { DrawString(160, TxtDist * nx, "����", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "��������", Cr);
		if (S.T_ThrowReject == 0)DrawString(160, TxtDist * nx, "���Ȃ�", Oran);
		else if (S.T_ThrowReject == 1) { DrawString(160, TxtDist * nx, "����", Cr); }

		nx++;

		DrawString(30, TxtDist * nx, "�ݒ�\��", Cr);
		if (S.TSwitch[5] == 0)DrawString(160, TxtDist * nx, "�S��", Oran);
		else if (S.TSwitch[5] == 1) { DrawString(160, TxtDist * nx, "���l", Cr); }
		else if (S.TSwitch[5] == 2) { DrawString(160, TxtDist * nx, "�L�[", Cr); }
		else if (S.TSwitch[5] == 3) { DrawString(160, TxtDist * nx, "���Ȃ�", Cr); }

		nx++;
		DrawString(30, TxtDist * nx, "����", Cr);
		if (S.TSwitch[6] == 0)DrawString(160, TxtDist * nx, "�Ȃ�", Oran);
		else if (S.TSwitch[6] == 1) { DrawString(160, TxtDist * nx, "��炢", Cr); }
		else if (S.TSwitch[6] == 2) { DrawString(160, TxtDist * nx, "�U��", Cr); }
		else if (S.TSwitch[6] == 3) { DrawString(160, TxtDist * nx, "����", Cr); }

		nx++;
		DrawString(30, TxtDist * nx, "�Đ����@", Cr);
		if (S.TSwitch[8] == 0)DrawString(160, TxtDist * nx, "�ʏ�", Oran);
		else if (S.TSwitch[8] == 1) { DrawString(160, TxtDist * nx, "�A��", Cr); }
		else if (S.TSwitch[8] == 2) { DrawString(160, TxtDist * nx, "���o�T", Cr); }
		
}

void CommandList()
{
	/**************
	* �w�i
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
	


	// �`��
	int tx = 29;
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){

		if (i == 1)tx += SCREEN_W/2;

		if (P[i].Name == EIN)
		{
			DrawString(tx, TxtDist * 1, "�� +�b", Cr);
			DrawString(tx, TxtDist * 2, "�c�i�ݒu��c �󒆉j", Cr);
			DrawString(tx, TxtDist * 3, "���_�� +�`or�aor�b�i�󒆉j", Cr);
			DrawString(tx, TxtDist * 4, "���^�� +�`or�a", Cr);
			DrawString(tx - 19, TxtDist * 5, "�Q�[�W�Z", Cr);
			DrawString(tx, TxtDist * 6, "���_�� +�c�i 0.5 �j", Cr);
			DrawString(tx, TxtDist * 7, "���^�� +�b�i 1 �󒆉� �j", Cr);
			DrawString(tx, TxtDist * 8, "���� +�c�i 1 �ݒu�� �j", Cr);
			DrawString(tx, TxtDist * 9, "�`+�a+�b�i 3 �j", Cr);
		}
		else if (P[i].Name == CORNEL)
		{
			DrawString(tx, TxtDist * 1, "�� +�a", Cr);
			DrawString(tx, TxtDist * 2, "�_ +�b", Cr);
			DrawString(tx, TxtDist * 3, "�c�i�q�b�g��c �󒆉j", Cr);	
			DrawString(tx, TxtDist * 4, "���_�� +�`or�a", Cr);
			DrawString(tx, TxtDist * 5, "�����_ +�`or�a", Cr);
			DrawString(tx, TxtDist * 6, "���^�� +�`or�a", Cr);
			DrawString(tx - 19, TxtDist * 7, "�Q�[�W�Z", Cr);
			DrawString(tx, TxtDist * 8, "���^�� +�c�i 0.5 �j", Cr);
			DrawString(tx, TxtDist * 9, "���_�� +�b�i 1 �j", Cr);
			DrawString(tx, TxtDist * 10, "�����_ +�b�i 1 �󒆉� �j", Cr);
			DrawString(tx, TxtDist * 11, "���^�� +�b�i 2 �j", Cr);
			DrawString(tx, TxtDist * 12, "�`+�a+�b�i 3 �j", Cr);
		}
		else if (P[i].Name == BOUNCER)
		{
			DrawString(tx, TxtDist * 1, "�� +�a", Cr);
			DrawString(tx, TxtDist * 2, "�� +�b�i�z�[���h�j", Cr);
			DrawString(tx, TxtDist * 3, "�c", Cr);
			DrawString(tx, TxtDist * 4, "���_�� +�`or�a", Cr);
			DrawString(tx, TxtDist * 5, "���^�� +�`or�a", Cr);
			DrawString(tx - 19, TxtDist * 6, "�Q�[�W�Z", Cr);
			DrawString(tx, TxtDist * 7, "���_�� +�b�i 0.5 �j", Cr);
			DrawString(tx, TxtDist * 8, "���^�� +�b�i 0.5 �j", Cr);
			DrawString(tx, TxtDist * 9, "�`+�a+�b�i 1 �j", Cr);
			DrawString(tx, TxtDist * 10, "���� +�c�i 3 �j", Cr);
		}
		else if (P[i].Name == HELIOS){
			DrawString(tx, TxtDist * 1, "�� +�`", Cr);
			DrawString(tx, TxtDist * 2, "�� +�a", Cr);
			DrawString(tx, TxtDist * 3, "�c�i�z�[���h�j", Cr);		
			DrawString(tx, TxtDist * 4, "���_�� +�`or�aor�c", Cr);
			DrawString(tx, TxtDist * 5, "�����_ +�`or�aor�c", Cr);
			DrawString(tx, TxtDist * 6, "���^�� +�`or�aor�c�i�󒆉j", Cr);
			DrawString(tx - 19, TxtDist * 7, "�Q�[�W�Z", Cr);
			DrawString(tx, TxtDist * 8, "���_�� +�b�i 1 �j", Cr);
			DrawString(tx, TxtDist * 9, "���^�� +�b�i 1 �j", Cr);
			DrawString(tx, TxtDist * 10, "���� +�c�i 1 �j", Cr);
			DrawString(tx, TxtDist * 11, "�`+�a+�b�i 3 �j", Cr);
		}
		else if (P[i].Name == HYDE){
			
		}
		else if (P[i].Name == SYUICHI) {
			DrawString(tx, TxtDist * 1, "�� +�`", Cr);
			DrawString(tx, TxtDist * 2, "�� +�a", Cr);
			DrawString(tx, TxtDist * 3, "�� +�b", Cr);
			DrawString(tx, TxtDist * 4, "�_ +�b", Cr);
			DrawString(tx, TxtDist * 5, "�c", Cr);
			DrawString(tx, TxtDist * 6, "���_�� +�`or�a", Cr);
			DrawString(tx, TxtDist * 7, "���^�� +�`or�a", Cr);
			DrawString(tx - 19, TxtDist * 8, "�Q�[�W�Z", Cr);
			DrawString(tx, TxtDist * 9, "���_�� +�b�i 1 �j", Cr);
		}
	}
	
	
	//���E��
	DrawLine(SCREEN_W / 2 - 1, 20, SCREEN_W / 2 - 1, SCREEN_H - 20, Cr);

	tx = 29 + (SCREEN_W/2);
}


// �g�����p�w�i
void BackGraphT()
{
	// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
	// ��������
	DrawBox(0, 20, SCREEN_W, SCREEN_H - 20, GetColor(0, 0, 0), true);
	// �`��u�����h���[�h���m�[�u�����h�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �㉺��
	DrawLine(0, 19, SCREEN_W, 19, Cr);
	DrawLine(0, SCREEN_H - 19, SCREEN_W, SCREEN_H - 19, Cr);

}

void BackGraph()
{
	// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
	// ��������
	DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
	// �`��u�����h���[�h���m�[�u�����h�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void raw_set()
{
	noChangeRow = true;
}

void Load_Pause(){
	if (Load_1 == 0){

		// �摜�ǂݍ���
		//int cursor = LoadGraph("ob/cursor.png");
		LoadDivGraph("ob/key.png", 10, 5, 2, 20, 20, gK);
		LoadDivGraph("ob/button.png", 4, 4, 1, 20, 20, gB);
		int gP = LoadGraph("ob/fontP.png");

		//Load_1 = 1;
	}
}