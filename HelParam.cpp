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
void HelParam(void)
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

	//=============
	// �L�����ʎ��O�ݒ�
	//=============
	if (P1.Var[11] > 3000)P1.Var[11] = 3000;	// �V���C���Q�[�W
	if (P1.Var[11] < 0)P1.Var[11] = 0;

	// D�ːi�J�E���g
	if (P2.stateno < 1000)P1.Var[3] = 0;

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
		// 5�`6�F�U�����
		// 40�`47�F
		// �W�����v
		// 50�`59�F
		// �K�[�h
		//--------------------
		if (((P1.stateno >= 5) && (P1.stateno <= 10)) ||	// �U������A���Ⴊ�݈ڍs
			((P1.stateno >= 40) && (P1.stateno <= 47)) ||	// �W�����v
			((P1.stateno >= 50) && (P1.stateno <= 69)) ||	// �K�[�h�E�󒆃_�b�V���E�n�C�W�����v�@
			(P1.stateno == 520) ||	// �K�[�L����
			((P1.stateno >= 550) && (P1.stateno <= 599))	// EX�A�N�V����
			){	
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
			GetP_SameParam(P1, P2);
			GetH_SameParam(H1, H2);
			GetS_SameParam(S);
			DamParam();
		}

		// �`�F�[��������
		if (P1.ctrl)P1.Var[1] = 0;

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
			if (P1.AnimTime <= 3)P1.XVel = P1.C.runF[0] * 0.5; //���x�𑫂�;
			// SE��炷
			if ((P1.time == 14) || (P1.time == 31))SEStart(4);
			if (P1.time > ANIMELEM)P1.time = 0;


			break;
		case 35:	// �o�b�N�X�e�b�v
			P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
			P1.ignoreG = true;

			// ���G
			if (P1.time <= 4){
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

			// SE��炷
			if (AnimElem(9))SEStart(3);

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
				HitTime(5, 13, 17, 11);
				// [�m�b�N�o�b�N]
				HitVel(-3.8, 0, -1.6, -4.8);

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
					// ���v�f���m�F
					if ((key(6)) && (!key(2)) && (!key(8))){
						P1.stateno = 215, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)){
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
			// �q�b�g���Z�b�g 
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}
			// �ʒu�����炷
			if (P1.time == 2){
				PosAdd(4, 0);
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
				HitTime(8, 18, 20, 14);

				// [�m�b�N�o�b�N]
				HitVel(-2.6, 0, -1.4, -5);
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
				HitTime(8, 18, 20, 14);

				// [�m�b�N�o�b�N]
				HitVel(-3.4, 0, -1.6, -5);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1000;	// ��i��炢
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
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
					if ((P1.K_Senkou[8]) && (P2.HFlg == 1)) {		// ��s���͌������Ă݂�
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0){
						// ���v�f���m�F
						if (key(2)){
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
						if ((key(6)) && (!key(2)) && (!key(8))){
							P1.stateno = 215, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else if (key(2) && P1.Var[1] == 0){
							P1.stateno = 310, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							P1.Var[1]++;
						}
					}
					else if (P1.Senkou[1] > 0){
						// ���v�f���m�F
						if (key(6) && (P1.Var[20] == 0)){
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
			// 215�F2B
			//********************
		case 215:
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);

			// �ʒu�����炷
			if (P1.time == 0){
				VelAdd(5.2, 0);
			}

			// SE��炷
			if (P1.time == 1)SEStart(3);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;
			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.MoveHit > 0)){

				// [�_���[�W]
				Damage(100, 0);
				// [�Q�[�W] 
				Power(120);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 22, 24, 18);
				// [�m�b�N�o�b�N]
				HitVel(-3.8, 0, -1.8, -4.0);
				// [�K�[�h����]
				P1.GuardF = 2;


				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// ���i��炢
				P1.A.forceKurai = true;	// ��炢�p��������

				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			// ���}���u
			if (P1.StopTime == 0){
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 1)){
					/*
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0){
						// ���v�f���m�F
						if (!key(2)){
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					*/
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
				HitTime(8, 22, 24, 18);
				// [�m�b�N�o�b�N]
				HitVel(-4.0, 0, -2.2, -4);
				// [�K�[�h����]
				P1.GuardF = 1;


				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// ���i��炢

				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
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
					/*
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0){
						// ���v�f���m�F
						if (key(2)){
							P1.stateno = 320, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
						}
					}
					*/
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
				HitTime(5, 12, 16, 10);

				// [�m�b�N�o�b�N]
				HitVel(-4.4, 0, -1.9, -4.8);
				P1.GuardF = 3;

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
					// ���v�f���m�F
					if ((key(6)) && (!key(2)) && (!key(8))){
						P1.stateno = 215, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)){
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

				HitTime(8, 18, 20, 14);

				// [�m�b�N�o�b�N]
				HitVel(-3.4, 0, -1.8, -4.6);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1010;	// ���i��炢
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
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
				HitTime(8, 32, 34, 18);
				// [�m�b�N�o�b�N]
				HitVel(-1.8, -3.6, -1.8, -3.4);
				GuardVel(-3.8, -1.8);
				// [�K�[�h����]
				P1.GuardF = 3;


				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// ���i��炢

				P1.A.boundLevel = 1;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
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
				HitTime(5, 14, 18, 12);
				// [�m�b�N�o�b�N]
				HitVel(-4.2, 0, -1.2, -4.0);
				// �K�[�h����
				if (P1.YVel >= 0.0)P1.GuardF = 2;
				else { P1.GuardF = 1; }
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
					else if (P1.Senkou[2] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 410, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					/*
					if (P1.Senkou[1] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 400, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					*/
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
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM){
				P1.ctrl = 1, P1.More = 1,
					P1.stateno = 46, P1.time = 0;
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
				HitTime(8, 16, 18, 14);
				// [�m�b�N�o�b�N]
				HitVel(-4.6, 0, -1.4, -4.0);
				P1.GuardF = 2;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
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
			// SE
			if (P1.time == 1)SEStart(2);
			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 5�t��
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(95, 0);

				// [�Q�[�W] 
				Power(100);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 17, 40, 14);

				// [�m�b�N�o�b�N]
				HitVel(-4.8, 0, -3.2, 6.9);
				P1.GuardF = 2;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
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
			/// �q�b�g���Z�b�g�A
			// �K�[�h���͖���
			if ((P1.time == 4) && (P2.stateno <= 999) && (P1.XPos - P2.XPos < 170 && P2.XPos - P1.XPos < 170))	// ��炢�E�󒆏�Ԃ���Ȃ�
			{
				P1.MoveHit = 1;	// �P��
				// �ǂ����������^�C�~���O�������Ȃ�A2P������
				//if ((P1.stateno == P2.stateno) && (P1.time == P2.time) && (P1.PSide == 2))
				//	P1.MoveHit = 0;
			}

			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 4){

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

				P1.StopTime = 6;

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


			// �q�b�g��
			if (P1.time == 7)P1.HitSE = 11;

			// 2P���G
			if (P1.time < 33)P2.Muteki = 1, P2.mutekiF = 0;


			// �q�b�g���Z�b�g
			if (P1.time == 34)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 34){

				// [�U������Z�b�g]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [�_���[�W]
				Damage(0, 180);
				P1.A.hosei_K = 0.5;
				// [�Q�[�W] 
				Power(300);
				HitTime(6, 45, 45, 0);
				// [�m�b�N�o�b�N]
				HitVel(-2.6, -3.4, 0, 0);
				P1.HitAnim = 1030;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitSE = 12;
				P1.A.boundLevel = 1;
			}
			else {
				DamReset();
			}

			// ���}���u
			if (P1.StopTime == 0){
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [�K�E�Z�E�Q�[�W�Z]
					HCancel();
				}
			}

			// ��������
			ThrowReject();

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
			if (P1.time == 4)	// �����̃^�C�~���O������ĂȂ�
			{
				DamReset();
				P1.MoveHit = 1;	// �P��
			}

			
			//if (P1.time == 1)DamReset();
			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 4){

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
				Damage(170, 10);
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
			ThrowReject();

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.YPos + P1.YVel * 2 >= GROUND){
				P1.time = 0, P1.stateno = 47,
					P1.YPos = GROUND, P1.YVel = 0,
					P1.ctrl = 0, P1.SFlg = 0;
			}
			break;
			//********************
			// 600 �`���[�W1
			//********************
		case 600:	// A�u��
			P1.ctrl = 0, P1.SFlg = 0;

			P1.Var[11] += 15;	// 20
			if (P1.Var[11] > 3000)P1.Var[11] = 3000;

			if (P1.time == 0)SEStart(39);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 601, P1.ctrl = 0, P1.More = 1;

			break;
			//********************
			// �`���[�W2
			//********************
		case 601:
			P1.ctrl = 0, P1.SFlg = 0;

			P1.Var[11] += 16;	// 22
			if (P1.Var[11] > 3000)P1.Var[11] = 3000;

			// ���[�v
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 601, P1.ctrl = 1, P1.More = 1;

			// D���� or �Q�[�WMAX �ŉ���
			if (P1.button[4] == 0 || P1.Var[11] >= 3000){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				StopSE(39);
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

			if (P1.time == 0){
				EffStart(16, P1.XPos, P1.YPos, -10, -(P1.ySize / 1.5),
					0.04, 0.25, P1.muki);
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 5){
				P1.XVel = 13;
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(110, 0);
				// [�Q�[�W] 
				Power(135);
				HitTime(4, 30, 32, 24);
				// [�m�b�N�o�b�N]
				HitVel(-2.6, -4.5, -2.8, -3.2);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 0.2;
				P1.A.boundLevel = 0;
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
			// 611 B�X�s�i�[
			//********************
		case 611:
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
			}

			if (P1.time == 0){
				EffStart(16, P1.XPos, P1.YPos, -10, -(P1.ySize / 1.5),
					0.04, 0.25, P1.muki);
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 5){
				P1.XVel = 20;
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(105, 0);
				// [�Q�[�W] 
				Power(130);
				HitTime(4, 30, 32, 24);
				// [�m�b�N�o�b�N]
				HitVel(-2.6, -4.5, -2.8, -3.2);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 0.2;
				if (P1.HitCount == 0){
					P1.A.boundLevel = 2;
					P1.A.bound_xvel = -0.8;
					P1.A.bound_yvel = -3.2;
				}
				else{ P1.A.boundLevel = 0; }

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
			// 615 �X�s�i�[
			//********************
		case 615:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.touchF = false;
			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
			}

			if (P1.time == 0){
				EffStart(16, P1.XPos, P1.YPos, -10, -(P1.ySize / 1.5),
					0.04, 0.25, P1.muki);
			}

			// �Q�[�W����
			if (P1.time == 1){
				P1.Var[11] -= 1000;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 5){
				P1.XVel = 20;
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(110, 20);
				// [�Q�[�W] 
				Power(170);
				HitTime(4, 50, 60, 26);
				// [�m�b�N�o�b�N]
				//HitVel(0.0, -8.2, 0.0, -6.6);
				HitVel(0.0, -7.0, 0.0, -6.4);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;
				P1.A.yaccel = 0.30;
			}
			else {
				DamReset();
			}

			/*
			// �L�����Z��
			if (P1.StopTime == 0){
				// �q�b�g���L�����Z��
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [�Q�[�W�Z]
					//HCancel();
				}
			}
			*/

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				P1.touchF = true;
			}
			break;
			//********************
			// 616�F�W�����vD
			//********************
		case 616:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(3);

			if (P1.time == 0)VelSet(0, -0.1);
			if (P1.time == 8)VelSet(6.0, 8);

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 5�t��
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(110, 0);
				// [�Q�[�W] 
				Power(140);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 68, 60, 14);

				// [�m�b�N�o�b�N]
				HitVel(-3.0, -1.8, -3.4, 6.0);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 2;
				P1.A.boundLevel = 3;
				P1.A.bound_yvel = -3.2;
				P1.HitAnim = 1000;
				P1.HitSE = 12;
				P1.A.hosei_K = 0.8;
			}
			else {
				DamReset();
			}
			if (Chakuchi()){
				P1.stateno = 619, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break; 
			//********************
			// 617�F�c�ːi�i��j
			//********************
		case 617:
			P1.ctrl = 0, P1.SFlg = 2;
			//P1.A.gaugeHosei = true;
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(3);
			// �Q�[�W����
			if (P1.time == 1){
				P1.Var[11] -= 1000;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			if (P1.time == 0)VelSet(0, -0.1);
			if (AnimElem(2))VelSet(8.2, 8.1);

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 5�t��
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(110, 10);
				// [�Q�[�W] 
				Power(190);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 68, 60, 18);

				// [�m�b�N�o�b�N]
				HitVel(-2.2, -2.0, -3.2, 6.0);
				P1.GuardF = 2;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.A.boundLevel = 3;
				P1.A.bound_xvel = -0.8;
				P1.A.bound_yvel = -4.2;
				P1.HitAnim = 1000;
				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			if (Chakuchi()){
				P1.stateno = 618, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;
			//********************
			// 618�F��c�ːi�ǂ��ł�
			//********************
		case 618:
			P1.ctrl = 0, P1.SFlg = 1;
			P1.YPos = GROUND;
			P1.ignoreG = false;
			VelSet(0, 0);

			// SE��炷
			if (P1.time == 1)SEStart(3);

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// �q�b�g���L�����Z��
			if ((P1.CFlg) && (P1.time >= 1)){
				// [�K�E�Z�E�Q�[�W�Z]
				P1.Var[3]++;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;
			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(80, 30);
				// [�Q�[�W] 
				Power(110);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 32, 34, 18);

				// �q�b�g�J�E���g
				if (P1.Var[3] == 0){
					// [�m�b�N�o�b�N]
					HitVel(0.0, -5.4, 0.0, -5.0);
				}
				else{ HitVel(-2.0, -3.0, -2.0, -2.8); }
				
				GuardVel(-3.8, -1.8);
				// [�K�[�h����]
				P1.GuardF = 1;

				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// ���i��炢

				P1.A.boundLevel = 0;
				// �G�t�F�N�g
				HitEff(3, 0.8, 0.8);
				P1.HitSE = 13;
			}
			else {
				DamReset();
			}
			// ���}���u
			if (P1.StopTime == 0){
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [�K�E�Z�E�Q�[�W�Z]
					HCancel();
				}
			}
			break;
			//********************
			// 619�F�W�����vD���n
			//********************
		case 619:
			P1.SFlg = 0, P1.A.damage = 0, P1.ctrl = 0;
			P1.YPos = GROUND;
			P1.ignoreG = false;
			VelSet(0, 0);
			// SE��炷
			if (P1.time == 1)SEStart(6);
			// �ŏ��Ƀ��b�N�I
			if (P1.time == 0)P1.Lock = 1;

			// �L�����Z��
			if (P1.StopTime == 0){
				// �q�b�g���L�����Z��
				if (P2.stateno >= 1000){
					// [�Q�[�W�Z]
					HCancel();
				}
			}

			// �O�W�����v �� �S�̃t���[��
			if (P1.time >= ANIMELEM)P1.More = 1, P1.stateno = 0,
				P1.time = 0, P1.Lock = 0;
			break;
			//********************
			// 620 ����
			//********************
		case 620:	
			P1.ctrl = 0, P1.SFlg = 0;
			P1.D.counter = true;
			P1.D.fatal = true;

			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);
				//P1.mutekiF = 0;
			}
			//if(P1.time <= 8)P1.Muteki = 1;

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 7)
				EffStart(20, P1.XPos + 17, P1.YPos - 10, 0, 0,
				0.50, 0.50, P1.muki);

			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [�_���[�W]
				Damage(110, 0);
				// [�Q�[�W] 
				Power(145);
				HitTime(8, 24, 24, 18);
				// [�m�b�N�o�b�N]
				HitVel(-2.6, -3.2, -2.2, -3.0);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 2;	// �n��󂯐g�\
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// ���}���u
			if (P1.StopTime == 0){
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [�Q�[�W�Z]
					//HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 625 �c����
			//********************
		case 625:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.D.counter = true;
			P1.D.fatal = true;

			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);
				P1.mutekiF = 0;
			}
			if (P1.time <= 10)P1.Muteki = 3;

			// �Q�[�W����
			if (P1.time == 1){
				P1.Var[11] -= 1000;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 8 || P1.time == 10){
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 7)
				EffStart(20, P1.XPos + 18, P1.YPos - 10, 0, 0,
				0.54, 0.51, P1.muki);

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(70, 10);
				// [�Q�[�W] 
				Power(80);
				HitTime(8, 24, 40, 18);
				// [�m�b�N�o�b�N]
				HitVel(-2.6, -3.4, -2.2, -3.2);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;	// �n��󂯐g�\
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// ���}���u
			if (P1.StopTime == 0){
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [�Q�[�W�Z]
					//HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;

			//********************
			// 630 �u������ A
			//********************
		case 630:	// A�u��
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷
			if (P1.time == 1){
				SEStart(2);
			}
			// �w���p�[�Ăяo��.[0]
			if (P1.time == 13){
				HelperReset(0);
				H1[0].var = true;
				H1[0].time = 0;
				H1[0].stateno = 2000;
			}

			// �q�b�g���L�����Z��
			if (P1.StopTime == 0){
				if ((H1[0].var == false) && (P1.time >= 12) && (P1.scTime > 0)){
					// [�Q�[�W�Z]
					HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;


			break;
			//********************
			// 635 �u������ A
			//********************
		case 635:	// A�u��
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷
			if (P1.time == 0){
				SEStart(2);
			}
			// �Q�[�W����
			if (P1.time == 1){
				P1.Var[11] -= 2000;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			// �w���p�[�Ăяo��.(�m�[�}���g��)
			if (P1.time == 12){
				HelperReset(1);
				H1[0].var = true;
				H1[0].time = 0;
				H1[0].stateno = 2010;
			}
			/*
			// �w���p�[�Ăяo��(EX�g��)
			if (P1.time == 21){
				HelperReset(0);
				H1[0].var = true;
				H1[0].time = 0;
				H1[0].stateno = 2010; 
			}
			*/

			// �q�b�g���L�����Z��
			if (P1.StopTime == 0){
				if ((H1[0].var == false) && (P1.time >= 12) && (P1.scTime > 0)){
					// [�Q�[�W�Z]
					HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
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
			// 800 ��i�؂�
			//********************
		case 800:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			P1.D.counter = true;
			P1.D.fatal = true;

			// �Q�[�W����
			if (P1.time == 0){
				P1.Power -= 1000;
				S.StopTime = 10;
				S.Anten = 25;
				SEStart(25);
				// ���i�G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}

			// ���G
			if (P1.time <= 9)P1.Muteki = 1, P1.mutekiF = 0;
			else { P1.Muteki = 0; }

			if (P1.time == 1){
				EffStart(16, P1.XPos, P1.YPos, -10, -(P1.ySize / 1.5),
					0.04, 0.25, P1.muki);
			}
			
			// 3456 9
			// �O�i
			if (P1.time <= 5){
				P1.XVel = 10.0;
			}

			// �q�b�g���Z�b�g	14�t���`
			if (AnimElem(3) || AnimElem(4) ||
				AnimElem(5) || AnimElem(6) ||
				AnimElem(9)){
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g

			if (P1.time <= 1){
				DamReset();
			}
			if ((P1.time >= 7) && (P1.time < 24)){

				// [�_���[�W]
				Damage(70, 5);
				// [�Q�[�W] 
				Power(0, 100);
				HitTime(6, 24, 24, 22);
				// [�m�b�N�o�b�N]
				HitVel(-3.2, 0, -4.4, -2.0);
				if (P2.SFlg != 2){
					P1.HitAnim = 1000;	// �n��
				}
				else if (P2.SFlg == 2){
					P1.HitAnim = 1060;	// �n���
				}
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
			}
			// �_���[�W�Z�b�g
			else if (P1.time >= 24){

				// [�_���[�W]
				Damage(35, 30);
				// [�Q�[�W] 
				Power(0, 100);

				HitTime(10, 100, 100, 12);
				// [�m�b�N�o�b�N]
				HitVel(-5.0, -3.6, -4.8, -3.2);

				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 14;
				// �G�t�F�N�g
				HitEff(3, 0.8, 0.8);
			}

			// ����������ύX
			if ((P1.CFlg) && (P1.time < 24) && (P2.stateno == 1060) && (P2.SFlg == 0)){
				P2.YPos = GROUND;
				
				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 40;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 40;
				if (P1.muki == 0){
					P2.XPos = P1.XPos + 40;
				}
				else if (P1.muki == 1){
					P2.XPos = P1.XPos - 40;
				}
				
			}

			// ���� �� �S�̃t���[���A54�𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}

			break;
			//********************
			// 820 ��ѓ���
			//********************
		case 820:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			// �Q�[�W����
			if (P1.time == 0){
				P1.Power -= 1000;
				S.StopTime = 10;
				S.Anten = 25;
				SEStart(25);
				// ���i�G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}
			// �w���p�[�ύX.[2]�A���ˑO�Ȃ�ύX
			if (P1.time == 6){
				HelperReset(2);
				H1[2].var = true;
				H1[2].XPos = P1.XPos;
				H1[2].time = 0;
				H1[2].stateno = 2020;
			}


			// �I���E34
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 830 �t���`���[�W
			//********************
		case 830:
			P1.ctrl = 0, P1.SFlg = 0;
			// �Q�[�W����
			if (P1.time == 0){
				P1.Power -= 1000;
				S.StopTime = 10;
				S.Anten = 25;
				SEStart(25);
				// ���i�G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}
			// �w���p�[�ύX.[2]�A���ˑO�Ȃ�ύX
			if (P1.time == 15){
				P1.Var[11] += 2000;
				if (P1.Var[11] > 3000)P1.Var[11] = 3000;
				// ���i�G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.50, 0.50, P1.PSide);
				SEStart(31);
			}

			// �I���E34
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 850 ��ѓ���
			//********************
		case 850:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			// �Q�[�W����
			if (P1.time == 0){
				P1.Power -= 3000;
				S.StopTime = 15;
				S.noGauge = 15;
				S.Anten = 25;
				SEStart(25);
				// ���i�G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}

			// �O�i
			if (P1.time == 1){
				VelSet(12.8, 0);
				afterimageSetting(P1.PSide, 180, 155, 155, 255);
			}

			// ���G
			if (P1.time <= 5){
				P1.mutekiF = 0;
				P1.Muteki = 1;
			}

			// �q�b�g���Z�b�g	14�t���`
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g�A���� 2 �t���[���H
			if (P1.time >= 1){


				// [�_���[�W]
				Damage(0, 0);

				// [�Q�[�W] 
				P1.GetPow = 0, P1.GivePow = 0;
				HitTime(5, 10, 8, 8);

				P1.G_GuardTime = 8;
				// [�m�b�N�o�b�N]
				HitVel(0, 0, 0, 0);
				P1.GuardF = 1;
				P1.HitAnim = 1060;
				P1.HitSE = 13;
				HitEff(3, 0.6, 0.6);
			}
			else {
				DamReset();
			}
			// ����������ύX
			if ((P1.CFlg) && (P2.stateno >= 1000) && (P1.time >= 1)){
				P1.stateno = 851;
				P1.time = 0, P2.time = 0;
				P1.More = 1, P1.A.damage = 0;
				P1.XVel = 0;
				if (P1.muki == 0){
					P2.XPos = P1.XPos + 100;
				}
				else {
					P2.XPos = P1.XPos - 100;
				}
				// ���葤
				P2.YPos = GROUND;
			}

			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				afterimageSetting(P1.PSide, 0, 255, 255, 255);
			}
			break;
			//********************
			// 851 3�Q�[�W�q�b�g��
			//********************
		case 851:
			P1.ctrl = 0;
			P1.A.gaugeHosei = true;
			
			if (P1.time == 23 - 1){
				VelSet(-2.4, -12);
				P1.SFlg = 2;
			}
			else if(P1.time == 0){
				P1.SFlg = 0;
			}

			if (P1.time == 23){
				//VelSet(-2.4, -5);
				//P1.SFlg = 2;
			}

			// �q�b�g���Z�b�g	14�t���`
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g�A���� 2 �t���[���H
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(0, 0);

				// [�Q�[�W] 
				P1.GetPow = 0, P1.GivePow = 0;
				HitTime(5, 120, 120, 8);

				P1.G_GuardTime = 8;
				// [�m�b�N�o�b�N]
				HitVel(-1, 0, 0, 0);
				P1.GuardF = 1;
				P1.HitAnim = 1000;
				P1.HitSE = 11;
			}
			else {
				DamReset();
			}

			// �I��
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 852, P1.ctrl = 1;
				P1.More = 1;
				VelSet(0, 0);
			}
			break;
			//********************
			// 852 3�Q�[�W�U��
			//********************
		case 852:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.A.gaugeHosei = true;

			if (P1.time == 9)
			{
				afterimageSetting(P1.PSide, 0, 255, 255, 255);
			}

			//
			if (P1.time == 6){
				int posY = P2.ySize;
				int posX = 20 + GetRand(10);
				for (int i = 0; i < 3; i++){
					float rand = GetRand(4);
					rand = (rand * 0.01) + 0.07;
					EffStart(9, (int)P2.XPos, (int)P2.YPos, posX, -(posY / 1.5),
						rand, rand, P2.muki);
				}
				EffStart(16, P2.XPos, P2.YPos - ((P2.GraphH * P2.GSize) / 2), 0, -1,
					0.40, 0.08, P2.muki);
			}

			if (P1.time >= 37 - 1){
				P1.ignoreG = false;
			}
			else{ P1.ignoreG = true; }
			
			if (P1.time == 37){
				VelSet(-1, -3);
			}

			// �U������
			if (P1.time == 0 || P1.time == 2 || P1.time == 4 || P1.time == 6){
				P1.MoveHit = 1;	// �P��
				P1.A.kill = 0;
				if (P1.time == 6)P1.A.kill = 1;
			}
			//  �b��G�t�F�N�g
			if (P1.time <= 5){
				int x, y;
				x = (int)P2.XPos + GetRand(200) - 100;
				y = (int)P2.YPos - 10 - GetRand(170);
				EffStart(10, x, y, 0, -1,
					0.2, 0.2, P1.muki);	// �q�b�g
				for (int i = 0; i < 3; i++){
					float rand = GetRand(4);
					rand = (rand * 0.01) + 0.07;
					EffStart(9, x, y, 0, -1,
						rand, rand, P2.muki);
				}
				EffStart(1, x, y, 0, -1,
					0.6, 0.6, P1.muki);
			}

			// �_���[�W�Z�b�g
			if ((P1.time >= 0) && (P1.time <= 9)){

				// [�_���[�W]
				Damage(100, 20);
				// [�Q�[�W] 
				Power(0, 150);

				HitTime(18, 200, 200, 15);
				P1.HSSelf = 6;
				// [�m�b�N�o�b�N]
				HitVel(-3.2, -6.2, -3.2, -6.2);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.A.boundLevel = 1;
				P1.HitAnim = 1030;	// �󒆋�炢(�m��_�E��)
				P1.HitSE = 13;
				if (P1.time == 6){
					P1.HitSE = 14;
					Damage(0, 150);
				}
				// �G�t�F�N�g
				HitEff(0, 0.6, 0.6);

			}
			else{ DamReset(); }

			// ���n
			if (Chakuchi()){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 0;
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
					// 2000 ���u�� �i�n��Łj
					//********************
				case 2000:
					EasyEffDraw(8, H1[i].HAnimTime, H1[i].XPos, H1[i].YPos,
						0, 0,
						0.5, 0.5, H1[i].muki);

					//***************
					//  ��{�ݒ�
					//***************
					// ���W�E���x�ݒ�
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;

						// ���x�Ȃ�������ݒ�
						H_VelSet(i, 11.8, 0);

						
						if (P1.stateno == 635)H_PosSet(i, 20, -112);
						else{ H_PosSet(i, 68, -150); }
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 0){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 80;
					}
					// �_���[�W�ʒu�Z�b�g
					if (H_triggerT(i, 0, 1)){

						//[ �U������Z�b�g ]
						//H2_AttSet(i, 0, 50, 30, -20, -36);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 100, H1[i].HGivePow = 50;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						// [�q�b�g�d��]
						H_HitTime(i, 8, 24, 24, 18);
						H1[i].HSSelf = 0;

						H1[i].fallF = 2;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H1[i].H_GX = -2.8, H1[i].H_GY = 0;
						H1[i].H_AX = -1.6, H1[i].H_AY = -3.2;
					}


					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 4)H1[i].HAnimTime = 0;
					// �I�������A�[�ɓ��Bor�n�ʂɓ�����or�q�b�gor�{�̔�_��
					if ((H1[i].XPos < (S.ScroolX)) ||
						(H1[i].XPos >(S.ScroolX + SCREEN_W)) ||
						(H1[i].YPos > GROUND) ||
						(H1[i].HMoveHit == 0) ||
						(P1.stateno >= 1000)
						)
					{
						HelperReset(i);
					}

					break;

					//********************
					// 2010 ���u�� �i�n��Łj
					//********************
				case 2010:
					EasyEffDraw(8, H1[i].HAnimTime, H1[i].XPos, H1[i].YPos,
						0, 0,
						0.7, 0.7, H1[i].muki);

					//***************
					//  ��{�ݒ�
					//***************
					// ���W�E���x�ݒ�
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;

						// ���x�Ȃ�������ݒ�
						H_VelSet(i, 10.2, 0);

						H_PosSet(i, 20, -112);
						P1.Var[12] = H1[0].XPos;	// ���W
						P1.Var[13] = 0;				// �q�b�g���ݒ�
					}
					if (H1[i].time >= 3)H_VelSet(i, 10.2, 0);

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 2){
						H1[i].HMoveHit = 1;
						//H1[i].A.damage = 110;
						H1[i].A.hosyo = 5;
						H1[i].A.damage = 45;
					}
					// �_���[�W�ʒu�Z�b�g
					if (H_triggerT(i, 0, 1)){

						//[ �U������Z�b�g ]
						//H2_AttSet(i, 0, 50, 30, -20, -36);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 150, H1[i].HGivePow = 75;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						// [�q�b�g�d��]
						H_HitTime(i, 8, 32, 32, 18);
						H1[i].HSSelf = 6;

						H1[i].fallF = 2;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H1[i].H_GX = -2.2, H1[i].H_GY = -4.6;
						H1[i].H_AX = -1.4, H1[i].H_AY = -4.4;
					}

					// �U�����q�b�g������A
					if ((H1[i].HMoveHit == 0) && (H1[i].stopTime == 0) && (H1[i].time > 2))
					{
						H1[i].time = 1;	// 0
						H1[i].HXVel = 0.0;
						P1.Var[13] += 1;
					}
					if (H1[i].stopTime == 0){
						H_Move(i);
					}
					//H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 4)H1[i].HAnimTime = 0;
					// �I�������A�[�ɓ��Bor�n�ʂɓ�����or�q�b�gor�{�̔�_��
					if (//(H1[i].XPos < (S.ScroolX + GAMENHAJI)) ||
						//(H1[i].XPos >(S.ScroolX + SCREEN_W - GAMENHAJI)) ||
						(H1[i].XPos < (S.ScroolX)) ||			// �O�ɏo��
						(H1[i].XPos >(S.ScroolX + SCREEN_W)) ||
						(P1.Var[13] > 2) ||						// 3�q�b�g�ȏ�
						(H1[i].XPos > P1.Var[12] + 420 ||		// �i�݂���
						H1[i].XPos < P1.Var[12] - 420) ||
						(P1.stateno >= 1000)
						)
					{
						// �ݒ胊�Z�b�g���ď���
						P1.Var[12] = 0;
						P1.Var[13] = 0;
						HelperReset(i);
					}

					break;
					//********************
					// 2020 �^��u�[������
					//********************
				case 2020:

					//***************
					//  ��{�ݒ�
					//***************
					H1[i].YPos = GROUND;
					H1[i].Alpha = true;	//������
					P1.A.gaugeHosei = true;

					//SE
					if (H1[i].time == 1)SEStart(23);


					// ���W�E���x�ݒ�
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H1[i].HXVel = 21.0;
						H1[i].attF = 1;
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 1){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 170;
						H1[i].A.hosyo = 60;
					}
					

					// �_���[�W�ʒu�Z�b�g�A2�t���ȍ~
					if (H1[i].time >= 1){


						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 14, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 300;

						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						H1[i].fallF = 1;

						// [�q�b�g�d��]
						H_HitTime(i, 8, 32, 60, 14);
						H1[i].HSSelf = 2;
						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1030;
						// [�m�b�N�o�b�N]
						H1[i].H_GX = -3.2, H1[i].H_GY = -3.2;
						H1[i].H_AX = -2.8, H1[i].H_AY = -2.8;
						//
						H1[i].A.boundLevel = 1;
						// �G�t�F�N�g
						h_HitEff(i, 3, 0.5, 0.5);

					}

					if (H1[i].stopTime == 0){
						H_Move(i);
					}

					// �G�t�F�N�g�Ƃ��ĕ`��
					EasyEffDraw(21, H1[i].HAnimTime, (int)H1[i].XPos, (int)H1[i].YPos, 0, 0, 0.7, 1, H1[i].muki);

					//***************
					//  �s�����e
					//***************

					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 8)H1[i].HAnimTime = 0;
					// �I�������A�[���B
					if ((H1[i].XPos < (S.ScroolX + GAMENHAJI)) ||
						(H1[i].XPos >(S.ScroolX + SCREEN_W - GAMENHAJI))
						)
					{
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
void GetP_HelParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_HelParam(System_t GS)
{
	S = GS;
}

void GetH_HelParam(Helper GH1[], Helper GH2[])
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
	// �c
	if ((P1.Senkou[4] > 0 || P1.button[4] > 0) && (P1.Var[11] < 3000)
		&& (P1.SFlg != 2)){		// ��s���͌������Ă݂�
		P1.stateno = 600, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 236 + �U��
	if ((P1.cmd[1]) && (!H1[0].var)){		// ��ʂɃw���p�[�P���Ȃ�
		if ((P1.Senkou[1] > 0) ||
			(P1.Senkou[2] > 0)){
			P1.stateno = 630, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	// 214 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0))
		&& (P1.cmd[2])){		// ��s���͌������Ă݂�
		if (P1.SFlg != 2){
			P1.stateno = 610, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
			if (P1.Senkou[2] > 0)P1.stateno = 611;
		}
	}
	// 623 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[4])){		// ��s���͌������Ă݂�
		if (P1.SFlg != 2){
			P1.stateno = 620, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	
	// 236 + D
	if ((P1.cmd[1]) && (!H1[0].var) && (P1.Senkou[4] > 0) && (P1.Var[13] == 0)){		// ��ʂɃw���p�[�P���Ȃ�
		if (P1.Var[11] >= 2000){
			P1.stateno = 635, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
		else {
			P1.stateno = 630, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	// 214 + D
	if ((P1.Senkou[4] > 0) 
		&& (P1.cmd[2])){		// ��s���͌������Ă݂�
		if (P1.Var[11] >= 1000){
			P1.stateno = 615, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		}
		else {
			P1.stateno = 610, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		}
	}
	// 623 + D
	if ((P1.Senkou[4] > 0)
		&& (P1.cmd[4])){		// ��s���͌������Ă݂�
		if (P1.Var[11] >= 1000){
			P1.stateno = 625, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		}
		else {
			P1.stateno = 620, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		}
	}

	HCancel();
	//�Ō�ɔ��������
	DamReset();
}

// �K�E�Z�L�����Z��
void HCancel()
{
	// [���K]
	//  236+C		[��ѓ���]
	if ((P1.Senkou[3] > 0) && (P1.cmd[1])
		&& (P1.Power >= 1000) && (!H1[2].var)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 820, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	//  214+C		[��i�؂�]
	if ((P1.Senkou[3] > 0) && (P1.cmd[2])
		&& (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 800, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	//  22+D		[�`���[�W]
	if ((P1.Senkou[4] > 0) && (P1.cmd[3])
		&& (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 830, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	// [ �n�[�h�p���` ]
	if ((P1.Senkou[1] > 0 && P1.Senkou[2] > 0 && P1.Senkou[3] > 0) && (P1.Power >= 3000)){
		P1.stateno = 850, P1.More = 1;
		P1.time = 0;
	}
	// EX�A�N�V����
	// A..�Q�[�W����
	if ((P1.Senkou[1] > 0) && (P1.cmd[3]) && (P1.Var[30] == 0)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 550;
			P1.More = 1;
			P1.time = 0;// �Q�[�W����
		}
	}
	//�Ō�ɔ��������
	//DamReset();

}

// �󒆕K�L�����Z��
void ACancel()
{
	// 214 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[2]) && (P1.YPos < GROUND - 20)){		// ��s���͌������Ă݂�
		if (P1.SFlg == 2){
			P1.stateno = 616, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	// 214 + D
	if (((P1.Senkou[4] > 0 || P1.button[4] == 1))
		&& (P1.cmd[2]) && (P1.YPos < GROUND - 20)){		// ��s���͌������Ă݂�
		if (P1.Var[11] >= 1000){
			P1.stateno = 617, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
		else{
			P1.stateno = 616, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	//�Ō�ɔ��������
	//DamReset();
}
