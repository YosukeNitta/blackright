//==================================================================================
//
//			���l�v�Z�̊Ǘ���
//
//========================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include "Define2.h"

//�f�t�@�C����---------------------------------------------

//�ް���`��------------------------------------------------


// �ǂ����̃v���C���[�������f
static int PSide;


//�����֐��錾��--------------------------------------------

static void SCancel();
static void ACancel();
static void HCancel();

//�v���O������----------------------------------------------


/****
�X�e�[�g�̐��l����ݒ肷��
****/
void EriParam(void)
{


	//�������A��؂�
	if (P1.More)P1.More = 0;

	// �R�}���h�ɐ��l��n��
	//GetPH_EinCmd(P1);

	// �X�e�[�g���؂�ւ������
	if (P1.stateno != P1.OldState){
		GetP_SameParam(P1, P2);
		GetH_SameParam(H1, H2);
		GetS_SameParam(S);
		ChangeState();
	}


	// 1P
	if (P1.StopTime == 0 || P1.time == 0){
		//�ݒ�J�n
		//--------------------
		// �U�����胊�Z�b�g
		//--------------------
		if (P1.stateno < 200 || P1.stateno > 999){
			DamReset();
		}
		//--------------------
		// 40�`47�F
		// �W�����v
		// 50�`59�F
		// �K�[�h
		//--------------------
		if (((P1.stateno >= 40) && (P1.stateno <= 47)) ||	// �W�����v
			((P1.stateno >= 50) && (P1.stateno <= 69))){	// �K�[�h�E�󒆃_�b�V���E�n�C�W�����v�@
			GetP_SameParam(P1, P2);
			GetH_SameParam(H1, H2);
			GetS_SameParam(S);
			SameParam();
		}
		//------------
		//
		// ��炢�֘A
		//
		//------------
		if ((P1.stateno >= 1000) && (P1.stateno <= 1999)){	// ������炢
			GetS_SameParam(S);
			GetP_SameParam(P1, P2);
			DamParam();
		}

		// �`�F�[��������
		if (P1.ctrl)P1.Var[1] = 0;
		// �A�ŃX�C�b�`
		if (P1.ctrl)P1.Var[20] = 0;

		switch (P1.stateno){

			//********************
			// 0 ����
			//********************
		case 0:		// ����
			//***************
			//  ��{�ݒ�
			//***************

			// �����ݒ�F�����
			P1.SFlg = 0, P1.Lock = 0;
			P1.ctrl = 1;
			P1.YPos = GROUND;
			P1.ignoreG = false;
			if (P1.time == 1){
				VelSet(0, 0);
			}

			// �ꉞ������
			DamReset();

			// ���� �� �S�̃t���[���A54�𒴂����烊�Z�b�g
			if (P1.time > ANIMELEM)P1.time = 0;

			//***************
			//  �s�����e
			//***************

			// �����|�[�Y��
			if ((P1.time > 30) && (S.roundState == 3) && (P2.stateno == 1070))
				P1.time = 0, P1.More = 1, P1.stateno = 95;

			break;
			//**** �I�� ****//
			//********************
			// 5 �U�����
			//********************
		case 5:

			P1.SFlg = 0, P1.ctrl = 1;
			VelSet(0, 0);
			if (P1.time == 0){
				P1.Lock = 1;
			}
			if (P1.time >= 8){
				P1.Lock = 0;
			}

			break;
			//********************
			// 6 �U������i���Ⴊ�݁j
			//********************
		case 6:

			P1.SFlg = 1, P1.ctrl = 1;
			VelSet(0, 0);
			if (P1.time == 0){
				P1.Lock = 1;
			}
			if (P1.time >= 8){
				P1.Lock = 0;
				P1.More = 1, P1.time = 0, P1.stateno = 11;
			}

			break;
			//********************
			// 10 ���������Ⴊ��
			//********************
		case 10:
			P1.SFlg = 1;	//���Ⴊ�ݔ����
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// ���Ⴊ�݂��痧���� 

			if (InputPAD(102) == 0){
				P1.time = 0, P1.stateno = 12, P1.More = 1;
			}


			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 11, P1.More = 1;

			break;
			//********************
			// 11 ���Ⴊ��
			//********************
		case 11:
			P1.SFlg = 1, P1.ctrl = 1;	//���Ⴊ�ݔ����
			VelSet(0, 0);
			// �ꉞ������
			DamReset();

			// �������G�t�^
			if (P1.Muteki == 1 && P1.time == 0 &&
				P1.mutekiF == 0){
				P1.mutekiF = 2;
				P1.Muteki = 10;
			}
			// �����͉����� 12 ��
			if (InputPAD(102) == 0){
				P1.time = 0, P1.stateno = 12, P1.More = 1;
			}

			break;
			//********************
			// 12 ���Ⴊ�݁�����
			//********************
		case 12:
			P1.SFlg = 0;	//���������
			// �ŏ��Ƀ��b�N�I
			if (P1.time == 0)P1.Lock = 1;
			// ���삪�������烂�[�V�����L�����Z��
			if (P1.Key != 0)P1.time = 0, P1.stateno = 0, P1.SFlg = 0, P1.More = 1, P1.Lock = 0;
			if (P1.time > ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.SFlg = 0, P1.More = 1, P1.Lock = 0;
			}

			break;
		case 20:	// �O����
			P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
			P1.XVel = P1.C.walkF; //���x�𑫂�
			// SE��炷
			if ((P1.time == 10) || (P1.time == 35))SEStart(7);
			if (P1.time > ANIMELEM)P1.time = 0;

			break;
		case 21:	// ������
			P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
			P1.XVel = P1.C.walkB; //���x�𑫂�
			// SE��炷
			if (P1.time == 20)SEStart(7);
			if (P1.time > ANIMELEM)P1.time = 0;

			break;
		case 30:	// �_�b�V��
			P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
			P1.XVel = P1.C.runF[0]; //���x�𑫂�
			// SE��炷
			if ((P1.time == 14) || (P1.time == 31))SEStart(4);
			if (P1.time > ANIMELEM)P1.time = 0;


			break;
		case 35:	// �o�b�N�X�e�b�v
			P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
			P1.ignoreG = true;

			// ���G
			if (P1.time <= 3){
				P1.Muteki = 1, P1.mutekiF = 0;
			}

			// SE��炷
			if (P1.time == 1)SEStart(5);

			VelAdd(0, P1.C.yAccel);

			//���x�𑫂�
			if (P1.time == 0)VelSet(P1.C.runB[0], P1.C.runB[1]);


			// �I��
			if ((P1.YPos >= GROUND) && (P1.time > 2)){
				P1.stateno = 36;
				P1.time = 0;
				P1.SFlg = 0;
				P1.ctrl = 0;
			}

			break;

		case 36:	// ���n�d��
			P1.SFlg = 0, P1.ctrl = 0;

			// SE��炷
			if (P1.time == 1)SEStart(6);
			VelSet(0, 0);
			P1.YPos = GROUND;

			// �I��
			if (P1.time >= ANIMELEM + 1){
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
			}

			break;

		case 90:	// �J��
			P1.SFlg = 0, P1.ctrl = 0;

			// �S�̃t���[���𒴂����痧����
			if (P1.time >= P1.animElem[P1.stateno]){
				P1.time = 0;
				P1.stateno = 0;
			}

		case 95:	// ����
			P1.SFlg = 0, P1.ctrl = 0;

			break;
		case 100:	// �_�b�V��
			P1.SFlg = 2;
			// �O�W�����v �� �S�̃t���[��
			if (P1.time > 1)P1.More = 1, P1.stateno = 0;

			break;
			//********************
			// 200�F����A
			//********************
		case 200:
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// ��s���͏���
			if (P1.time <= 1)P1.Senkou[1] = 0;

			// ���� �� �S�̃t���[���A54�𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}
			// SE��炷
			if (P1.time == 1){
				SEStart(1);
			}
			// �q�b�g���Z�b�g�A�����U�t��
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(46, 0);

				// [�Q�[�W] 
				Power(30);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 13, 20, 11);
				// [�m�b�N�o�b�N]
				HitVel(-2.8, 0, -1.4, -4.8);

				// [�K�[�h����]
				P1.GuardF = 1;
				// [�q�b�g���̃A�j��]
				P1.HitAnim = 1010;
				// [�q�b�g�T�E���h]
				SESet(10, 16);

				// [�t�H�[�����]
				P1.fallF = 1;
			}
			else {
				DamReset();
			}
			// �q�b�g���`�F�[��
			if ((P1.CFlg) && (P1.time >= 1)){
				// [�W�����v�L�����Z��]
				if (P1.keyAtt[8]){		// ��s���͌������Ă݂�
					P1.stateno = 40, P1.More = 1,
						P1.time = 0, P1.A.damage = 0;
				}
				// [�ʏ�Z]
				if (P1.Senkou[3] > 0){
					if (key(2)){
						P1.stateno = 320, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else{
						P1.stateno = 220, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if (P1.Senkou[2] > 0){		// ��s���͌������Ă݂�
					if (key(2)){
						P1.stateno = 310, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else{
						P1.stateno = 210, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				// ��s����
				else if (P1.Senkou[1] > 0){
					if (key(2)){
						P1.stateno = 300, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else{
						P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}

				// [�K�E�Z�E�Q�[�W�Z]
				SCancel();
			}
			// �L�����Z���A�A�ŃL�����Z��
			if ((P1.button[1] == 1) && (P1.time >= 9) && (P1.stateno == 200)){
				if (!key(2)){
					P1.More = 1, P1.time = 0, P1.A.damage = 0;
				}
			}

			break;
			//********************
			// 205�F����A
			//********************
		case 205:	//����A
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			//[ �ʏ�Z�̎����U����� ]
			if (P1.time == 0){
				if ((P1.XPos < P2.XPos) && (P1.muki == 1)){
					P1.muki = 0;
				}
				else if ((P1.XPos > P2.XPos) && (P1.muki == 0)){
					P1.muki = 1;
				}
			}
			// �q�b�g���Z�b�g 
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}
			if (P1.time == 1)VelSet(2.0, 0);
			// �ʒu�����炷
			if (P1.time == 2){
				PosAdd(6, 0);
			}
			if (P1.time == 2 + 4){
				PosAdd(8, 0);
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(82, 0);

				// [�Q�[�W] 
				Power(94);
				HitTime(8, 18, 22, 14);

				// [�m�b�N�o�b�N]
				HitVel(-2.6, 0, -1.7, -5);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1000;	// ��i��炢
				P1.HitSE = 11;
			}
			else {
				DamReset();
			}
			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 6)){
					// [�W�����v�L�����Z��]
					if (P1.K_Senkou[8]){		// ��s���͌������Ă݂�
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0){
						if (key(2)){
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else{
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}

			break;
			//********************
			// 210�F����B
			//********************
		case 210:	//����B
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// �q�b�g���Z�b�g 
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(80, 0);

				// [�Q�[�W] 
				Power(98);
				HitTime(8, 18, 22, 14);

				// [�m�b�N�o�b�N]
				HitVel(-2.6, 0, -1.7, -5);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1000;	// ��i��炢
				P1.HitSE = 11;
			}
			else {
				DamReset();
			}
			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1)){
					// [�W�����v�L�����Z��]
					if (P1.K_Senkou[8]){		// ��s���͌������Ă݂�
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0){
						// ���v�f���m�F
						if (key(6)){
							P1.stateno = 225, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else if (key(2)){
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else{
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					else if (P1.Senkou[2] > 0){
						// ���v�f���m�F
						if (key(2) && P1.Var[1] == 0){
							P1.stateno = 310, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							P1.Var[1]++;
						}
					}
					else if (P1.Senkou[1] > 0){
						// ���v�f���m�F
						if (key(6)){
							P1.stateno = 205, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}

			break;
			//********************
			// 220�F����C
			//********************
		case 220:
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// �ʒu�����炷
			if (P1.time == 0){
				PosAdd(6, 0);
			}
			// SE��炷
			if (P1.time == 1)SEStart(3);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;


			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(105, 0);
				// [�Q�[�W] 
				Power(128);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 22, 28, 18);
				// [�m�b�N�o�b�N]
				HitVel(-3.8, 0, -2.8, -4);
				// [�K�[�h����]
				P1.GuardF = 1;


				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// ���i��炢


				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			// [�K�[�h����]
			P1.GuardDist = 300;

			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1)){
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}

			}
			break;
			//********************
			// 225�F2�b
			//********************
		case 225:
			P1.ctrl = 0, P1.SFlg = 1;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);

			// SE��炷
			if (P1.time == 1)SEStart(3);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;
			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [�_���[�W]
				Damage(100, 0);
				// [�Q�[�W] 
				Power(120);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 22, 28, 18);
				// [�m�b�N�o�b�N]
				HitVel(-3.8, 0, -1.8, -4.0);
				// [�K�[�h����]
				P1.GuardF = 1;


				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// ���i��炢


				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			// ���}���u
			if (P1.StopTime == 0){
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 1)){
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}
			break;
			//********************
			// 300�F���Ⴊ��A
			//********************
		case 300:
			P1.ctrl = 0, P1.SFlg = 1;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			//[ �ʏ�Z�̎����U����� ]
			if (P1.time == 0){
				if ((P1.XPos < P2.XPos) && (P1.muki == 1)){
					P1.muki = 0;
				}
				else if ((P1.XPos > P2.XPos) && (P1.muki == 0)){
					P1.muki = 1;
				}
			}
			// ��s���͏���
			if (P1.time <= 1)P1.Senkou[1] = 0;
			// SE��炷
			if (P1.time == 1)SEStart(1);
			//  �S�̃t���[���A14�𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;
			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(44, 0);

				// [�Q�[�W] 
				P1.GetPow = 24, P1.GivePow = 12;

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(6, 12, 20, 10);

				// [�m�b�N�o�b�N]
				HitVel(-3.0, 0, -1.6, -4.8);
				P1.GuardF = 1;

				P1.HitAnim = 1010;	// ���i��炢
				P1.HitSE = 10;
			}
			else {
				DamReset();
			}
			// �L�����Z��
			if ((P1.CFlg) && (P1.time >= 1)){
				if (P1.Senkou[3] > 0){
					if (key(2)){
						P1.stateno = 320, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else{
						P1.stateno = 220, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if (P1.Senkou[2] > 0){		// ��s���͌������Ă݂�
					if (key(2)){
						P1.stateno = 310, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else{
						P1.stateno = 210, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if (P1.Senkou[1] > 0){		// ��s���͌������Ă݂�
					if (key(2)){
						P1.stateno = 300, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else{
						P1.stateno = 200, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				// [�K�E�Z�E�Q�[�W�Z]
				SCancel();
			}
			// �L�����Z���A�A�ŃL�����Z��
			if ((P1.button[1] == 1) && (P1.time >= 9) && (P1.stateno == 300)){
				// ���v�f�������Ă���I
				if (key(2)){
					P1.More = 1, P1.time = 0, P1.A.damage = 0;
				}
			}
			break;
			//********************
			// 310�F���Ⴊ��B
			//********************
		case 310:
			P1.ctrl = 0, P1.SFlg = 1;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// �ʒu�����炷
			if (P1.time == 5){
				if (P1.muki == 0){
					P1.XPos += 6;
				}
				else{
					P1.XPos -= 6;
				}
			}
			// �ʒu�����炷
			if (P1.time == 9){
				if (P1.muki == 0){
					P1.XPos -= 6;
				}
				else{
					P1.XPos += 6;
				}
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;
			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;
			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(75, 0);

				// [�Q�[�W] 
				Power(86);

				HitTime(8, 18, 22, 14);

				// [�m�b�N�o�b�N]
				HitVel(-3, 0, -1.8, -4.6);
				P1.GuardF = 3;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1010;	// ���i��炢
				P1.HitSE = 11;
			}
			else{
				DamReset();
			}
			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1)){
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0){		// ��s���͌������Ă݂�
						if (key(2)){
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else{
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					else if (P1.Senkou[2] > 0){		// ��s���͌������Ă݂�
						// ���v�f���m�F
						if (!key(2) && P1.Var[1] == 0){
							P1.stateno = 210, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							P1.Var[1]++;
						}
					}
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}


			break;
			//********************
			// 320�F2�b
			//********************
		case 320:
			P1.ctrl = 0, P1.SFlg = 1;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);

			// �ʒu�����炷
			if (P1.time == 0){
				PosAdd(5, 0);
			}

			// SE��炷
			if (P1.time == 1)SEStart(3);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;
			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(100, 0);
				// [�Q�[�W] 
				Power(120);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 32, 38, 18);
				// [�m�b�N�o�b�N]
				HitVel(-1.8, -4.0, -1.8, -4.0);
				// [�K�[�h����]
				P1.GuardF = 1;


				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// ���i��炢


				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			// ���}���u
			if (P1.StopTime == 0){
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 1)){
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}
			break;
			//********************
			// 400�F�W�����vA
			//********************
		case 400:
			P1.ctrl = 0, P1.SFlg = 2;
			if (P1.time == 1){
				SEStart(1);
			}
			// ��s���͏���
			if (P1.time <= 1)P1.Senkou[1] = 0;
			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(42, 0);
				// [�Q�[�W] 
				Power(24);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(6, 12, 18, 10);
				// [�m�b�N�o�b�N]
				HitVel(-4.2, 0, -1.4, -4.0);
				P1.GuardF = 1;
				P1.HitAnim = 1000;
				P1.HitSE = 10;
			}
			else {
				DamReset();
			}
			/*
			// ����\�t���[��
			if (P1.time >= 19)P1.ctrl = 1;
			*/
			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					if (P1.Senkou[3] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[2] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 410, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�󒆕K�E�Z]
					ACancel();
				}

				// [�W�����v�L�����Z��]
				if ((P1.CFlg) && (P1.time >= 1) && (P1.AirJump > 0)){
					if (P1.K_Senkou[8] >= 1){		// ��s���͌������Ă݂�
						P1.AirJump -= 1;
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}
			break;
			//********************
			// 410�F�W�����vB
			//********************
		case 410:
			P1.ctrl = 0, P1.SFlg = 2;
			// SE
			if (P1.time == 1)SEStart(1);

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;
			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(75, 0);

				// [�Q�[�W] 
				Power(80);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 16, 20, 14);
				// [�m�b�N�o�b�N]
				HitVel(-4.6, 0, -1.8, -4.0);
				P1.GuardF = 2;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				P1.HitSE = 11;
			}
			else{
				DamReset();
			}
			/*
			// ����\�t���[��
			if (P1.time >= 23){
			P1.ctrl = 1;
			}
			*/
			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					if (P1.Senkou[3] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}

					// [�󒆕K�E�Z]
					ACancel();
				}

				// [�W�����v�L�����Z��]
				if ((P1.CFlg) && (P1.time >= 1) && (P1.AirJump > 0)){
					if (P1.K_Senkou[8] >= 1){		// ��s���͌������Ă݂�
						P1.AirJump -= 1;
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}
			break;
			//********************
			// 420�F�W�����vC
			//********************
		case 420:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(2);

			if (P1.time == 0)VelSet(0, -0.8);
			if (P1.time == 10)VelSet(0, 10);

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 5�t��
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(105, 0);

				// [�Q�[�W] 
				Power(110);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 28, 42, 8);

				// [�m�b�N�o�b�N]
				HitVel(-2.8, -4.2, -3.2, 6.4);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				P1.HitSE = 12;
				P1.A.boundLevel = 1;
			}
			else {
				DamReset();
			}
			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [�󒆕K�E�Z]
					ACancel();
				}

			}

			if (P1.YPos + P1.YVel * 2 >= GROUND){
				P1.stateno = 421, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;
			//********************
			// 431�F�W�����vD���n
			//********************
		case 421:
			P1.SFlg = 0, P1.A.damage = 0, P1.ctrl = 0;
			P1.YPos = GROUND;
			P1.ignoreG = false;
			P1.D.counter = 0;
			VelSet(0, 0);
			// SE��炷
			if (P1.time == 1)SEStart(6);
			// �ŏ��Ƀ��b�N�I
			if (P1.time == 0)P1.Lock = 1;
			// �O�W�����v �� �S�̃t���[��
			if (P1.time >= ANIMELEM + 4)P1.More = 1, P1.stateno = 0,
				P1.time = 0, P1.Lock = 0;
			break;

			//********************
			// 500�F�n�㓊��
			//********************
		case 500:
			P1.ctrl = 0, P1.SFlg = 0;
			// ���������ݒ�
			if (P1.time == 0){
				P1.throwSide = P1.muki;
				P1.A.throwTurn = false;
				if (P1.keyAtt[4]){
					P1.A.throwTurn = true;
				}
				P1.XVel = 0;
			}


			// SE
			if (P1.time == 1)SEStart(2);
			// �q�b�g���Z�b�g�A
			// �K�[�h���͖���
			if ((P1.time == 5) && (P2.stateno <= 999) && (P1.XPos - P2.XPos < 170 && P2.XPos - P1.XPos < 170))	// ��炢�E�󒆏�Ԃ���Ȃ�
			{
				P1.MoveHit = 1;	// �P��
				// �ǂ����������^�C�~���O�������Ȃ�A2P������
				//if ((P1.stateno == P2.stateno) && (P1.time == P2.time) && (P1.PSide == 2))
				//	P1.MoveHit = 0;
			}

			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 5){

				// [�_���[�W]
				Damage(0, 0);
				// [�Q�[�W] 
				Power(0, 0);
				HitTime(0, 3, 3, 0);

				P1.GX = 0, P1.GY = 0;
				P1.GuardF = 0;
				P1.attF = 2;

				P1.HitAnim = 1060;
				P1.HitSE = 9;
				P1.A.hitEff = 2;
			}

			// [�K�[�h����]
			P1.GuardDist = 0;


			// ����������ύX
			if ((P1.CFlg) && (P1.time == 8)){
				P1.stateno = 505;
				P1.time = 0, P2.time = 0;
				P1.More = 1;
				P1.A.damage = 0;
				// �����̌�����ύX
				if (P1.A.throwTurn){
					if (P1.muki == 0){
						P1.muki = 1, P2.muki = 0;
					}
					else if (P1.muki == 1){
						P1.muki = 0, P2.muki = 1;
					}
				}


			}
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= 32)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 505�F�����i���o�j
			//********************
		case 505:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.Muteki = 1, P1.mutekiF = 0;


			P1.XVel = 0;
			// �������Ƃ�
			if (P1.time == 0){
				int dist = 86;
				if (P1.muki == 0){
					P2.XPos = P1.XPos + dist;
					// ��������������Ȃ�������
					if (P2.XPos > STAGE_WIDTH - GAMENHAJI){
						P2.XPos = STAGE_WIDTH - GAMENHAJI;
						P1.XPos = P2.XPos - dist;
					}
				}
				else{
					P2.XPos = P1.XPos - dist;
					if (P2.XPos < GAMENHAJI){
						P2.XPos = GAMENHAJI;
						P1.XPos = P2.XPos + dist;
					}
				}
				P2.YVel = 0;
				P2.YPos = GROUND;
			}
			// ��������������Ȃ�������


			// �����𗣂�
			if (AnimElem(4))P1.XVel = 4.2;
			if (AnimElem(4))P1.XVel = 3.2;
			// �q�b�g��
			if (P1.time == 7)P1.HitSE = 11;

			// 2P���G
			if (P1.time < 7)P2.Muteki = 1, P2.mutekiF = 0;


			// �q�b�g���Z�b�g
			if (P1.time == 8)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 8){

				// [�U������Z�b�g]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [�_���[�W]
				Damage(0, 70);
				// [�Q�[�W] 
				Power(100, 150);
				HitTime(6, 55, 55, 0);
				// [�m�b�N�o�b�N]
				HitVel(0, -9.4, 0, 0);
				P1.HitAnim = 1030;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitSE = 12;
				P1.A.hosei_K = 0.3;
			}
			else {
				DamReset();
			}

			// ��������
			if ((P2.Senkou[1] > 0 && P2.Senkou[2] > 0)
				&& (P1.time < TR_TIME) && (P2.rejectF)){
				P1.stateno = 55, P2.stateno = 55;
				P1.time = 0, P2.time = 0;

				if (P1.throwSide == 0){
					P1.muki = 0, P2.muki = 1;
					P2.XPos = P1.XPos + 40;
				}
				else{
					P1.muki = 1, P2.muki = 0;
					P2.XPos = P1.XPos - 40;
				}
				P1.YPos = GROUND, P2.YPos = GROUND;
				P1.XVel = -8, P2.XVel = -8;
				P1.YVel = 0, P2.YVel = 0;
				P1.More = 1, P2.More = 1;

				SEStart(29);
				EffStart(12, P2.XPos, (int)P2.YPos, 0, -(P2.ySize / 1.5),
					0.4, 0.4, false);
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			break;
			//********************
			// 510 �󒆓���
			//********************
		case 510:	// �󒆓���
			P1.ctrl = 0, P1.SFlg = 2;

			// ���������ݒ�
			if (P1.time == 0){
				P1.throwSide = P1.muki;
				P1.A.throwTurn = false;
				if (P1.muki == 0){
					if (InputPAD(104) >= 1){
						P1.A.throwTurn = true;
					}
				}
				if (P1.muki == 1){
					if (InputPAD(106) >= 1){
						P1.A.throwTurn = true;
					}
				}
			}
			// SE��炷
			if (P1.time == 1)SEStart(1);
			// �q�b�g����
			// �q�b�g���Z�b�g�A
			if ((P1.time == 5))	// �����̃^�C�~���O������ĂȂ�
			{
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 5){

				// [�_���[�W]
				Damage(0, 0);
				// [�Q�[�W] 
				Power(0, 0);

				HitTime(4, 16, 20, 0);
				// [�m�b�N�o�b�N]
				HitVel(0, 0, 0, -5);
				P1.GuardF = 0;
				P1.attF = 2;
				// [�K�[�h����]
				P1.GuardDist = 0;

				P1.HitAnim = 1065;
				P1.HitSE = 9;
				P1.A.hitEff = 2;
			}
			// ����������ύX
			if ((P1.CFlg) && (P1.time >= 7)){
				P1.stateno = 515;
				P1.time = 0, P2.time = 0;
				P1.More = 1;
				P1.A.damage = 0;
				// �����̌�����ύX
				if (P1.A.throwTurn){
					if (P1.muki == 0){
						P1.muki = 1, P2.muki = 0;
					}
					else if (P1.muki == 1){
						P1.muki = 0, P2.muki = 1;
					}
				}
			}
			break;

			//********************
			// 515 �󓊂����o
			//********************
		case 515:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.Muteki = 1, P1.mutekiF = 0;
			P1.A.damage = 0, P1.MoveHit = 1;
			P1.WAtt[0] = 0, P1.HAtt[0] = 0;
			P1.XAtt[0] = 0, P1.YAtt[0] = 0;
			P1.XVel = 0;
			P1.ignoreG = true;

			// �o�O�h�~�̕���
			if ((P1.time == 0) && (P1.YPos + 15 >= GROUND)){
				P1.YPos = GROUND - 15;
			}

			// �������Ƃ�
			if (P1.time == 0){
				P1.YPos -= 2;
				P2.YPos = P1.YPos - 1;
				int dist = P2.xSize + 5;
				if (P1.muki == 0){
					P2.XPos = P1.XPos + dist;
					// ��������������Ȃ�������
					if (P2.XPos > STAGE_WIDTH - GAMENHAJI){
						P2.XPos = STAGE_WIDTH - GAMENHAJI;
						P1.XPos = P2.XPos - dist;
					}
				}
				else{
					P2.XPos = P1.XPos - dist;
					if (P2.XPos < GAMENHAJI){
						P2.XPos = GAMENHAJI;
						P1.XPos = P2.XPos + dist;
					}
				}
				P1.YVel = -4;
				P2.YVel = -4;
			}
			// �d�͉��Z
			if (P1.time > 16){
				P1.YVel += GRAVITY + 0.05;
			}
			// ���G�I��
			else if (P1.time == 16){
				P1.YVel += GRAVITY;
				P2.YVel += GRAVITY;
			}
			// ���łɖ��G����
			else {
				P2.Muteki = 1, P2.mutekiF = 0;
				P2.time = 0;
				P1.YVel += GRAVITY;
				P2.YVel += GRAVITY;
			}
			// SE��炷
			if (P1.time == 15)SEStart(10);

			// �q�b�g���Z�b�g
			if (P1.time == 15)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 16){

				// ���o�E���h
				//P1.A.boundLevel = 1;
				// [�U������Z�b�g]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [�_���[�W]
				Damage(140, 10);
				// [�Q�[�W] 
				Power(300);

				HitTime(6, 100, 100, 0);
				// [�m�b�N�o�b�N]
				HitVel(-4.5, 9, -4.5, 9);
				P1.HitAnim = 1030;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitSE = 11;


			}
			else{
				DamReset();
			}

			// �p���ύX
			if (P1.time == 17)P2.time = 20;

			// ��������
			if ((P2.Senkou[1] > 0 && P2.Senkou[2] > 0)
				&& (P1.time < TR_TIME) && (P2.rejectF)){
				P1.stateno = 56, P2.stateno = 56;
				P1.time = 0, P2.time = 0;

				if (P1.throwSide == 0){
					P1.muki = 0, P2.muki = 1;
					P2.XPos = P1.XPos + 40;
				}
				else{
					P1.muki = 1, P2.muki = 0;
					P2.XPos = P1.XPos - 40;
				}
				P1.XVel = -2, P2.XVel = -2;
				P1.YVel = -3, P2.YVel = -3;
				P1.ignoreG = false, P2.ignoreG = false;
				P1.D.yaccel = 0.35, P2.D.yaccel = 0.35;
				P1.fall = 0, P2.fall = 0;

				P2.YPos = P1.YPos;
				P1.More = 1, P2.More = 1;

				SEStart(29);
				EffStart(12, P2.XPos, (int)P2.YPos, 0, -(P2.ySize / 1.5),
					0.4, 0.4, false);
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.YPos + P1.YVel * 2 >= GROUND){
				P1.time = 0, P1.stateno = 47,
					P1.YPos = GROUND, P1.YVel = 0,
					P1.ctrl = 0, P1.SFlg = 0;
			}
			break;
			//********************
			// 520�F�K�[�L����
			//********************
		case 520:	//����B
			P1.ctrl = 0, P1.SFlg = 0;
			// SE
			if (P1.StopTime == 0){
				if (P1.time == 0){
					SEStart(30);

					P1.Power -= 1000;
					VelSet(0, 0);

					S.StopTime = 10;
					S.Anten = 10;
					P2.CFlg = 0;

					P1.colorCTime = 10;
					P1.colorC[0] = 100, P1.colorC[1] = 100, P1.colorC[2] = 255;
				}
			}

			// �U���o��܂Ŗ��G
			if (P1.D.counter == 1){
				P1.Muteki = 3;
				P1.mutekiF = 0;
			}

			// �q�b�g���Z�b�g�A�W�t��
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 1)VelSet(2.8, 0);
			// �ʒu�����炷
			if (P1.time == 3){
				PosAdd(5, 0);
			}

			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [�_���[�W]
				Damage(0, 0);

				// [�Q�[�W] 
				P1.GetPow = 0, P1.GivePow = 0;
				HitTime(10, 20, 20, 18);

				// [�m�b�N�o�b�N]
				HitVel(-2.1, -2.6, -1.7, -3);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 2;
				P1.HitAnim = 1000;	// ��i��炢
				P1.HitSE = 11;
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 600 �╥�� B
			//********************
		case 600:
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷�A�ړ�
			if (P1.time == 1){
				SEStart(21);
			}
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�U������Z�b�g]
				AttSet(0, 120, 100, 69, -100);

				// [�_���[�W]
				Damage(105, 0);

				// [�Q�[�W] 
				Power(120);
				HitTime(8, 53, 60, 16);

				// [�m�b�N�o�b�N]
				HitVel(-3.2, -4.8, -3.1, -3.4);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;

				if (P2.SFlg == 2){
					P1.A.wall_xvel = 0.4;
					P1.A.wall_yvel = -5.4;
				}
				else{
					P1.A.wall_xvel = 0.4;
					P1.A.wall_yvel = -5.8;
				}
				P1.A.wallLevel = 2;
				// SE
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// �q�b�g���L�����Z��
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [�Q�[�W�Z]
					HCancel();
				}
			}

			break;
			//********************
			// �X�E�B�t�g2
			//********************
		case 601:
			P1.ctrl = 0, P1.SFlg = 2;
			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 1.8;
				P1.YVel = -4.5;
				P1.ignoreG = true;
			}

			P1.YVel += 0.3;

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}


			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [�_���[�W]
				Damage(105, 0);
				// [�Q�[�W] 
				Power(120);
				HitTime(10, 32, 38, 18);
				// [�m�b�N�o�b�N]
				HitVel(-3.4, -2.5, -3.5, -2.5);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}



			// ���n
			if ((P1.time > 0) && (P1.YVel > 0) && (P1.YPos + P1.YVel * 2 >= GROUND)){
				P1.time = 0, P1.stateno = 621, P1.More = 1,
					P1.ctrl = 0, P1.ignoreG = false;
			}

			break;
			//********************
			// 610 �X�s�i�[
			//********************
		case 610:
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 5){
				P1.XVel = 13;
			}

			/*
			if (P1.time == 5){
			P1.XVel = 13;
			}
			*/

			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [�_���[�W]
				Damage(100, 0);
				// [�Q�[�W] 
				Power(120);
				HitTime(2, 53, 60, 24);
				// [�m�b�N�o�b�N]
				HitVel(-2.4, -3.5, -2.6, -2.2);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;
				P1.A.wallLevel = 2;

			}
			else {
				DamReset();
			}

			// �L�����Z��
			if (P1.StopTime == 0){
				// �q�b�g���L�����Z��
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [�Q�[�W�Z]
					HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 620 �A�b�p�[
			//********************
		case 620:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;

			// SE��炷
			if (P1.time == 0){
				SEStart(21);
				VelSet(0, 0);
			}
			if (P1.time == 1){
				VelSet(0, 0);
			}

			if (AnimElem(3)){
				VelSet(1, -6);
			}

			// �A�j������
			if (P1.time == ANIMELEM - 1){
				VelSet(0.1, -0.5);
			}
			if (P1.time > ANIMELEM - 1){
				VelAdd(0, 0.4);
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [�_���[�W]
				Damage(95, 10);
				// [�Q�[�W] 
				Power(126);

				HitTime(8, 100, 100, 12);
				// [�m�b�N�o�b�N]
				HitVel(-1.7, -5.4, -1.7, -4.8);

				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;

				P1.HitSE = 13;
			}

			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 1)){ //&& (P2.stateno >= 1000)
				}
			}

			// ���n
			if (P1.YPos > GROUND){
				P1.time = 0, P1.stateno = 621, P1.ctrl = 0;
			}
			break;

			//********************
			// 621 ���n
			//********************
		case 621:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.ignoreG = false;
			VelSet(0, 0);
			// SE��炷
			if (P1.time == 1){
				SEStart(6);
				P1.YPos = GROUND;
			}

			// �I��
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;

			//********************
			// 700 ��u��[5]
			//********************
		case 700:
			P1.ctrl = 0, P1.SFlg = 0;
			// �Q�[�W����
			if (P1.time == 1)P1.Power -= 500;
			// SE��炷
			if (P1.time == 2){
				SEStart(22);
			}
			// �w���p�[�Ăяo��.[5]
			if (P1.time == 4){
				HelperReset(5);
				H1[5].var = true;
				H1[5].time = 0;
				H1[5].stateno = 2030;
			}
			// �I���E58F
			if (P1.time >= 58)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 800 �n�[�h�p���`
			//********************
		case 800:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			P1.touchF = false;

			if (P1.time == 12)VelAdd(180, 0);
			

			if (P1.StopTime == 0){
				// �Q�[�W����
				if (P1.time == 0){
					P1.Power -= 3000;
					S.StopTime = 10;
					S.Anten = 25;
					SEStart(25);
					// ���i�G�t�F�N�g
					EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
				}
			}

			// SE��炷�A�ړ�
			if (P1.time == 1){
				SEStart(21);
				// �J���[�ύX
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}


			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(250, 350);
				// [�Q�[�W] 
				Power(0, 600);
				HitTime(8, 600, 600, 24);
				// [�m�b�N�o�b�N]
				HitVel(0, -5.8, 0, -5.4);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 14;
				P1.A.yaccel = 0.11;
				// �G�t�F�N�g
				HitEff(1, 1, 1);
			}
			else {
				DamReset();
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				P1.touchF = true;
			}
			break;
			//===============================//
		}// switch�I��

		// �X�e�[�g�ԍ���ۑ�
		P1.OldState = P1.stateno;
		P1.oldState2 = P1.State2;

	}// P1�����I��


	for (int i = 0; i < HM; i++){
		{
			//********************
			//
			// �w���p�[
			//
			//********************

			// �I����������X�e�[�g�ɔ�΂�
			if (H1[i].var){
				switch (H1[i].stateno){
					//********************
					// 2000 �u���ꂽ��u�� 
					//********************
				case 2000:

					//***************
					//  ��{�ݒ�
					//***************	
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H_PosSet(i, -80, 0);
					}
					//***************
					//  �s�����e
					//***************

					// �S�̃t���[��33or��_��
					if ((H1[i].time > 33) ||
						(P1.stateno >= 1000)){
						HelperReset(i);
					}
					break;


					//===============================//
					//******************
					// �w���p�[�����I��
					//******************
				}// switch�I��
			}// �I���E�I�t���f�I��
		}// ���̃w���p�[��
	}// for�I��

	// �l��Ԃ�
	Get_SSet(S);
	if (P1.PSide == 1)Get_PSet(P1, P2), Get_HSet(H1, H2);
	if (P1.PSide == 2)Get_PSet(P2, P1), Get_HSet(H2, H1);

}//StateParam�I��




//-------------------------------------------------------------------------------
//
// �󂯎��p
//
//-------------------------------------------------------------------------------

// ���Ԃ��t�ɂ���
void GetP_EriParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_EriParam(System_t GS)
{
	S = GS;
}

void GetH_EriParam(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}



//-------------------------------------------------------------------------------
//
// �󂯎��Q
//
//-------------------------------------------------------------------------------


/*
int InputCOM(int num)
{

return 0;//Ein_ComCheck(num);

}
*/

//-------------------------------------------------------------------------------
//
// ���l������
//
//-------------------------------------------------------------------------------



//-------------------------------------------------------------------------------
//
// �L�����Z���ݒ�
//
//-------------------------------------------------------------------------------

// �K�E�Z�L�����Z��
void SCancel()
{
	// 214 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0))
		&& (P1.cmd[2])){		// ��s���͌������Ă݂�
		P1.stateno = 600, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}

	HCancel();
	//�Ō�ɔ��������
	DamReset();
}

// �K�E�Z�L�����Z��
void HCancel()
{
	//  [ �R�Q�[�W ]
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.Senkou[3] > 0)
		&& (P1.Power == 3000)){		// �n��
		P1.stateno = 800;	// ���K
		P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	//�Ō�ɔ��������
	DamReset();

}

// �󒆕K�L�����Z��
void ACancel()
{

}