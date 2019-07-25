//==================================================================================
//
//			���l�v�Z�̊Ǘ���
//
//========================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"

//�f�t�@�C����----------------------------------------------

// #include "Define.h" �ɂāA�֐����Z�b�g



//�ް���`��------------------------------------------------


//�����֐��錾��--------------------------------------------
static const int REC_TIME = 600;
// 0.�ʏ� 1.���� 2.�^�撆 3.�Đ���
static int button[REC_TIME][8];
static int uButton[REC_TIME][8];
static int rTime;	// ���R�[�f�B���O����
static int rTime_Max;	// ���R�[�f�B���O��������

static boolean turn = false;	// ���R�[�f�B���O�J�n���̌���
static boolean reversal = false, oldHFlg = false;

static Player sP2;	// �Z�b�g����P2
static int oldSwitch;	// �O�̃X�C�b�`
static int onCmd[20];

//�v���O������----------------------------------------------


//-------------------------------------------------------------------------------
//	��`�@int CharMove( void )
//
//	�����@�o�g�����[�h��1P, 2P�̒l��ݒ肷��A���̃��[�h�̗v
//
//	�����@int (��)  �����̒l�Ŏ��̉�ʂ����܂�
//
//	�߂�l�@int ���j���[�����߂�ϐ�
//-------------------------------------------------------------------------------


void SetTraning()
{
	//================================================
	// �̗�
	if (S.T_Life == 0){
		if (P1.stateno < 1000 && (P1.ctrl)){
			if (P1.Life < P1.C.lifeMax){ P1.Life += 16; }
			if (P1.Life > P1.C.lifeMax)P1.Life = P1.C.lifeMax;
		}
		if (P2.stateno < 1000 && (P2.ctrl)){
			if (P2.Life < P2.C.lifeMax){ P2.Life += 16; }
			if (P2.Life > P2.C.lifeMax)P2.Life = P2.C.lifeMax;
		}

		P1.LifeC = LIFECOUNT_MAX, P2.LifeC = LIFECOUNT_MAX;	// ���C�t�J�E���g��ɍő�
	}

	//================================================
	// �Q�[�W
	if (S.T_Power == 0){
		if ((P2.HFlg == 0) && (P1.stateno < 200)){
			if (P1.Power > POWER_MAX)P1.Power = POWER_MAX;
			else if (P1.Power < POWER_MAX){ P1.Power = POWER_MAX; }
		}
		if ((P1.HFlg == 0) && (P2.stateno < 200)){
			if (P2.Power > POWER_MAX)P2.Power = POWER_MAX;
			else if (P2.Power < POWER_MAX){ P2.Power = POWER_MAX; }
		}
	}
	else if (S.T_Power == 1){
		if ((P2.HFlg == 0) && (P1.stateno < 200) && (P1.ctrl)){
			if (P1.Power > 1000)P1.Power = 1000;
			else if (P1.Power < 1000){ P1.Power = 1000; }
		}
		if ((P1.HFlg == 0) && (P2.stateno < 200) && (P2.ctrl)){
			if (P2.Power > 1000)P2.Power = 1000;
			else if (P2.Power < 1000){ P2.Power = 1000; }
		}
	}
	else if (S.T_Power == 2){
		if ((P2.HFlg == 0) && (P1.stateno < 200) && (P1.ctrl)){
			if (P1.Power > 2000)P1.Power = 2000;
			else if (P1.Power < 2000){ P1.Power = 2000; }
		}
		if ((P1.HFlg == 0) && (P2.stateno < 200) && (P2.ctrl)){
			if (P2.Power > 2000)P2.Power = 2000;
			else if (P2.Power < 2000){ P2.Power = 2000; }
		}
	}

	//================================================
	// �K�[�h�Q�[�W
	if (S.T_GGauge == 0){
		if (P1.ctrl == 1){
			if (P1.aGauge <= 0)P1.aGauge = GUARD_MAX;
			if (P1.GRecovery != 0)P1.GRecovery = 0;
		}
		if (P2.ctrl == 1){
			if (P2.aGauge <= 0)P2.aGauge = GUARD_MAX;
			if (P2.GRecovery != 0)P2.GRecovery = 0;
		}
	}

	//================================================
	// �A�h�Q�[�W
	if (S.T_AddGauge == 1){
		if (P1.ctrl == 1 && P2.ctrl == 1){
			P1.addG = ADD_MAX;
			P2.addG = ADD_MAX;
		}
	}

	//================================================
	// �J�E���^�[
	if (S.T_Counter == 1){
		if (P1.HitCount == 0 && P2.stateno < 1000){
			P2.D.counter = 1;
		}
		if (P2.HitCount == 0 && P1.stateno < 1000){
			P1.D.counter = 1;
		}
	}

	//================================================
	// ���	�i������E�v���C���[�EAI�j

	// ������~
	if (S.TSwitch[2] != 3)Stop_P2_B();

	if (S.TSwitch[2] == 0);	// ����
	else if (S.TSwitch[2] == 1){	// ��
		P2_Auto(102);
	}
	else if (S.TSwitch[2] == 2){	// ��
		// �n��ɂ�����W�����v
		if (P2.SFlg != 2)P2_Auto(108);
	}

	// AI
	if (S.TSwitch[2] == 4)S.AI = true;
	else{ S.AI = false; }

	

	//================================================
	// ���Z�b�g
	if ((P1.button[8] == 1) && (P1.keyAtt[2] > 0) && (S.TSwitch[7] == 0)){
		// �ʒu����
		if (P1.button[104] > 0)FirstPosSet(-1);
		else if (P1.button[106] > 0)FirstPosSet(1);
		else{ FirstPosSet(0); }
		ModeChange(GameScene(VersusScene));	// ������
	}

	//================================================
	//--------------------
	// �_�~�[�̃A�N�V����
	//--------------------
	if ((S.TSwitch[2] < 3) && (P2.stateno != 1055))// ������̂Ƃ�
	{

	//[ �󂯐g ]
		if (S.TSwitch[3] == 0){
			if ((P2.stateno >= 1030) && (P2.stateno < 1080)){
				P2_Auto(1);
			}
		}
		else if (S.TSwitch[3] == 1){
			if ((P2.stateno >= 1030) && (P2.stateno < 1080)){
				P2_Auto(1);
				if (P2.muki == 0)P2_Auto(106);
				else { P2_Auto(104); }
			}
		}
		else if (S.TSwitch[3] == 2){
			if ((P2.stateno >= 1030) && (P2.stateno < 1080)){
				P2_Auto(1);
				if (P2.muki == 0)P2_Auto(104);
				else { P2_Auto(106); }
			}
		}
	
	//[ �K�[�h ]
	// 1.����
		/*
		if (S.TSwitch[4] == 1){
			
			if ((P1.stateno >= 200) && (P1.stateno <= 999) //&& (P1.WAtt[0] > 0 || P1.WAtt[1] > 0) 
				&& ((P2.XPos - P1.XPos < 300) &&
				(P1.XPos - P2.XPos < 300))){
				if (P1.XPos < P2.XPos)P2_Auto(106);
				else if (P1.XPos > P2.XPos)P2_Auto(104);

				if (P1.XPos == P2.XPos){
					if (P2.muki == 0)P2_Auto(104);
					else { P2_Auto(106); }
				}

				// ���K����
				if ((P1.stateno >= 300) && (P1.stateno <= 399))P2_Auto(102);
			}
			// ���K����
			if ((P2.stateno >= 54))P2_Auto(102);

			
			for (int i = 0; i < HELPER_MAX; i++){
				// �w���p�[
				if ((H1[i].var) && 
					(H1[i].HMoveHit > 0 && H1[i].A.damage > 0) &&
					(H1[i].WHit > 0)
					){
					((H1[i].XPos - P2.XPos < 105) &&
						(P2.XPos - H1[i].XPos < 105)) ||
					if (P1.XPos < P2.XPos)P2_Auto(106);
					else if (P1.XPos > P2.XPos)P2_Auto(104);

					if (P1.XPos == P2.XPos){
						if (P2.muki == 0)P2_Auto(104);
						else { P2_Auto(106); }
					}

				}
			}
			
		}
		*/
	
		//[ �������� ]
		// 1.����
		if (S.T_ThrowReject == 1){

			if ((P1.stateno == 505 || P1.stateno == 515) && P1.time <= 1){
				P2_Auto(1), 
				P2_Auto(2);
			}

		}
	//================================================
	//[ ���R�[�f�B���O ]
		// �X�C�b�`�̃I���E�I�t
		if (P1_BInput(8) == 1){
			if ((P1.keyAtt[2] == 0)){
				//  �^�揀��
				if (S.TSwitch[7] == 0)S.TSwitch[7] = 1;
				// �^���
				else if (S.TSwitch[7] == 1){
					S.TSwitch[7] = 2;
				}
				else if (S.TSwitch[7] == 2){
					S.TSwitch[7] = 0;
					rTime_Max = rTime;
				}
				// �Đ��I��
				else if (S.TSwitch[7] == 3){
					S.TSwitch[7] = 0;
				}
				SEStart(36);
			}
		}

		// ���o�T�m�F
		if ((oldHFlg && P2.HFlg == 0) && (S.TSwitch[8] == 2) && 
			(P2.stateno > 59 || P2.stateno < 50))
			reversal = true;

		// �Đ��A���o�T�ł�������
		if (P1_BInput(9) == 1 || reversal){
			// �Đ���
			if (S.TSwitch[7] == 0){
				S.TSwitch[7] = 3;
				SEStart(36);
				//if (reversal);
			}
			// ��{�ɖ߂�
			else if (S.TSwitch[7] == 1){
				S.TSwitch[7] = 0;
				SEStart(36);
			}
			// �^�撆�~
			else if (S.TSwitch[7] == 2){
				S.TSwitch[7] = 0;
				// ���l������
				for (int i = 0; i < 8; i++){
					button[rTime][i] = 0;
					rTime_Max = 0;
				}
				SEStart(37);
			}
			// ������x�Đ�
			else if (S.TSwitch[7] == 3){
				rTime = 0;
				SEStart(36);
				//if (reversal);
			}

			// ���o�T�t���O�I��
			reversal = false;
		}

		// ��炢��Ԋm�F
		//oldHFlg = P2.HFlg;
		if ((P2.HFlg == 1) || (P2.stateno >= 50 && P2.stateno <= 52))oldHFlg = 1;
		else{ oldHFlg = 0; }
		// 2P����J�n
		if (S.TSwitch[7] == 1){
			P2Ctrl();
		}
		//else{ P2_AI(P1, P2); }

	}// �_�~�[�̃A�N�V�����I��

	// �Đ�
	if (S.TSwitch[7] == 3){

		// �ŏ���
		if (rTime == 0){
			// ���o�T�Ȃ�
			if (S.TSwitch[8] == 2){
				// �R�}���h�ڂ�
				Traning_P2Command(onCmd);
				// �L�[4�`7�ǂꂩ����
				for (int n = 0; n < REC_TIME; n++){
					// ������
					for (int i = 0; i < 8; i++){
						uButton[n][i] = 0;
					}
					for (int i = 4; i < 8; i++){
						if (button[n][i] > 0)uButton[rTime][i] = 1;
					}
				}
			}
			// ����ȊO
			else{
				for (int n = 0; n < REC_TIME; n++){
					for (int i = 0; i < 8; i++){
						uButton[n][i] = button[n][i];
					}
				}
				// ����
				if (P2.muki != turn){
					// ���l������
					int hozon = 0;
					for (int i = 0; i < REC_TIME; i++){
						hozon = uButton[i][2];
						uButton[i][2] = uButton[i][3];
						uButton[i][3] = hozon;
					}
				}
			}
		}
		// �s��������
		P2Ctrl3(uButton[rTime]);
		P2Ctrl3_2();

		// �Đ��I��
		if (rTime >= rTime_Max){
			//S.TSwitch[7] = 0;	// ���͂���
			// �ʏ�or���o�T
			if (S.TSwitch[8] == 0 || S.TSwitch[8] == 2 || reversal){
				S.TSwitch[7] = 0;
			}
			// �A��
			else if (S.TSwitch[8] == 1){
				rTime = -1;
				S.TSwitch[7] = 3;
			}
		}
	}

	// �^��
	if (S.TSwitch[7] == 2){

		// ����
		if (rTime == 0){
			if (!P2.muki)turn = false;
			else{ turn = true; }
		}

		P2Ctrl();

		for (int i = 0; i < 4; i++){
			if (P2_BInput(i + 1) > 0){
				button[rTime][i + 4] = 1;
			}
			else{ button[rTime][i + 4] = 0; }
		}
		// �㉺���E
		if (P2_BInput(108) > 0){
			button[rTime][0] = 1;
		}
		else{ button[rTime][0] = 0; }

		if (P2_BInput(102) > 0){
			button[rTime][1] = 1;
		}
		else{ button[rTime][1] = 0; }

		if (P2_BInput(104) > 0){
			button[rTime][2] = 1;
		}
		else{ button[rTime][2] = 0; }

		if (P2_BInput(106) > 0){
			button[rTime][3] = 1;
		}
		else{ button[rTime][3] = 0; }

		if (rTime == 0){
			for (int i = 0; i < 20; i++){
				onCmd[i] = 0;
			}
		}
		for (int i = 0; i < 20; i++){
			if (P2.cmd[i] > 0)onCmd[i] = 1;
		}
	}

	// ��������rTime��0��
	if (S.TSwitch[7] == 0 || S.TSwitch[7] == 1){
		rTime = 0;
	}
	if (S.TSwitch[7] == 2 || S.TSwitch[7] == 3){
		rTime++;
	}
	// ���E���ԂŃj���[�g�����ɖ߂�
	if (rTime > REC_TIME - 1){
		rTime_Max = REC_TIME - 1;
		rTime = 0;
		S.TSwitch[7] = 0;
	}
	
	// �g�����ł̕\��
	TestText(rTime, 0);

	// �^��I�����A�O��P���L��
	if (oldSwitch == 2 && S.TSwitch[7] != 2)sP2 = P2;
	// �L��
	oldSwitch = S.TSwitch[7];

	

	//================================================
	// �Ԃ�
	Get_PSet(P1, P2);
	Get_SSet(S);
}


void GetP_Traning(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}


void GetS_Traning(System_t GS)
{
	S = GS;
}

void GetH_Traning(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}