//==================================================================================
//
//			���l�v�Z�̊Ǘ���
//
//========================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include "Define2.h"

//�f�t�@�C����----------------------------------------------

// #include "Define2.h" �ɂāA�֐����Z�b�g

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
void EinParam(void)
{

	//�������A��؂�
	if (P1.More)P1.More = 0;

	// �R�}���h�ɐ��l��n��
	GetPH_EinCmd(P1);

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


				
				// �������G�t�^
				/*
				if (P1.Muteki == 10 && P1.time == 0 &&
					P1.mutekiF == 0){
					P1.mutekiF = 2;
					P1.Muteki = 10;
				}
				*/

				// ���� �� �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.time >= P1.animElem[P1.stateno])P1.time = 0;

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
				if (P1.time >= P1.animElem[P1.stateno]){
					P1.time = 0, P1.stateno = 0, P1.SFlg = 0, P1.More = 1, P1.Lock = 0;
				}
			
				break;
			case 20:	// �O����
				P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
				P1.XVel = P1.C.walkF; //���x�𑫂�
				// SE��炷
				if ((P1.time == 10) || (P1.time == 35))SEStart(7);
				if (P1.time >= ANIMELEM)P1.time = 0;
				break;
			case 21:	// ������
				P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
				P1.XVel = P1.C.walkB; //���x�𑫂�
				// SE��炷
				if (P1.time == 20)SEStart(7);
				if (P1.time >= P1.animElem[P1.stateno])P1.time = 0;

				break;
			case 30:	// �_�b�V��
				P1.SFlg = 0, P1.ctrl = 1, P1.Lock = 0;
				P1.XVel = P1.C.runF[0]; //���x�𑫂�
				if (P1.AnimTime <= 8)P1.XVel = P1.C.runF[0] * (0.1 * (P1.AnimTime + 1)); //���x�𑫂�;
				// SE��炷
				if ((P1.time == 20) || (P1.time == 40))SEStart(4);
				if (P1.time >= P1.animElem[P1.stateno])P1.time = 0;

				
				break;
			case 35:	// �o�b�N�X�e�b�v
				P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
				P1.ignoreG = true;

				// ���G
				if (P1.time <= 5){
					P1.Muteki = 1, P1.mutekiF = 0;
				}

				// SE��炷
				if (P1.time == 1)SEStart(5);

				VelAdd(0, P1.C.yAccel);

				//���x�𑫂�
				if (P1.time == 0)VelSet(P1.C.runB[0], P1.C.runB[1]);
				

				// �I��
				if ((P1.YPos >= GROUND) && (P1.time > 1)){
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
				/*
				if (P1.time == 7)SEStart(32);
				if (AnimElem(13))SEStart(28);
				*/

				// ����炷
				if (P1.time == 15){
					SEStart(2);
				}

				// �S�̃t���[���𒴂����痧����
				if (P1.time >= P1.animElem[P1.stateno]){
					P1.time = 0;
					P1.stateno = 0;
				}
				break;

			case 95:	// ���E���h�擾
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
				//[ �ʏ�Z�̎����U����� ]
				if (P1.time == 0){
					if ((P1.XPos < P2.XPos) && (P1.muki == 1)){
						P1.muki = 0;
					}
					else if ((P1.XPos > P2.XPos) && (P1.muki == 0)){
						P1.muki = 1;
					}
				}
				// �S�̃t���[��
				if (P1.time >= ANIMELEM){
					P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				}
				// SE��炷
				if (P1.time == 1){
					SEStart(1);
				}

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 1, 2);
				}

				// �q�b�g���Z�b�g
				if (P1.time == 3){
					P1.MoveHit = 1;	// �P��
				}

				// �_���[�W�Z�b�g�A���� 2 �t���[���H
				if (P1.time >= 1){

					// [�_���[�W]
					Damage(45, 0);

					// [�Q�[�W] 
					P1.GetPow = 25, P1.GivePow = 12;
					// [�q�b�g�X�g�b�v�E�̂����莞��]
					HitTime(6, 12, 16, 10);
					// [�m�b�N�o�b�N]
					HitVel(-3.6, 0, -1.5, -4.8);

					// [�K�[�h����]
					P1.GuardF = 1;
					// [��炢���̕���]
					P1.fallF = 1;
					// [�q�b�g���̃A�j��]
					P1.HitAnim = 1000;
					// [�q�b�g�T�E���h]
					SESet(10, 16);
				}
				else {
					DamReset();
				}
				

				// �q�b�g���`�F�[��
				if ((P1.CFlg) && (P1.time >= 5)){
					// [�W�����v�L�����Z��]
					if (P1.keyAtt[8]){		// ��s���͌������Ă݂�
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0){
						// 6C
						if ((key(6)) && (!key(2)) && (!key(8))){
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
			// 210�F����B
			//********************
			case 210:	//����B
				P1.ctrl = 0, P1.SFlg = 0;
				// ����
				if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
				// SE
				if (P1.time == 1){
					SEStart(1);
				}
				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 2, 1);
				}

				// �q�b�g���Z�b�g�A�W�t��
				if (P1.time == 6){
					P1.MoveHit = 1;	// �P��
				}
				// �_���[�W�Z�b�g
				if ((P1.time >= 6) && (P1.time <= 10)){

					// [�_���[�W]
					Damage(76, 0);

					// [�Q�[�W] 
					Power(88);
					HitTime(8, 18, 20, 16);

					// [�m�b�N�o�b�N]
					HitVel(-2.8, 0, -1.7, -4.0);
					P1.GuardF = 1;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.HitAnim = 1000;	// ��i��炢
					P1.HitSE = 11;
				}
				else {
					DamReset();
				}
				// �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

				// �L�����Z��
				// ���}���u
				if (P1.StopTime == 0){
					if ((P1.CFlg) && (P1.time >= 8)){

						// [�ʏ�Z]
						if (P1.Senkou[3] > 0){
							// 6C
							if ((key(6)) && (!key(2)) && (!key(8))){
								P1.stateno = 225, P1.More = 1,
									P1.time = 0, P1.A.damage = 0;
							}
							// ���v�f���m�F
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
								P1.Var[1] ++;
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
					PosAdd(1, 0);
				}
				// SE��炷
				if (P1.time == 1)SEStart(3);

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 3, 0);
				}

				// �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;


				// �q�b�g���Z�b�g �����F26
				if (P1.time == 9){
					P1.MoveHit = 1;	// �P��
				}
				//else{ P1.MoveHit = 0; }

				// �_���[�W�Z�b�g
				if ((P1.time >= 9) && (P1.time <= 18)){

					// [�_���[�W]
					Damage(96, 0);
					// [�Q�[�W] 
					Power(114);
					// [�q�b�g�X�g�b�v�E�̂����莞��]
					HitTime(8, 22, 24, 18);
					// [�m�b�N�o�b�N]
					HitVel(-4.0, 0, -2.0, -4);
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
			// 225�F6C
			//********************
			case 225:
				P1.ctrl = 0, P1.SFlg = 0;
				// ����
				if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
				
				// �ʒu�����炷
				if (P1.time == 0){
					PosAdd(5, 0);
				}
				// SE��炷
				if (P1.time == 1)SEStart(3);
				// �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 3, 0);
				}

				// �q�b�g���Z�b�g �����F26
				if (P1.time == 1){
					P1.MoveHit = 1;	// �P��
				}
				// �_���[�W�Z�b�g
				if ((P1.time >= 1)){

					// [�_���[�W]
					Damage(106, 0);
					// [�Q�[�W] 
					Power(128);
					// [�q�b�g�X�g�b�v�E�̂����莞��]
					HitTime(8, 22, 24, 18);
					// [�m�b�N�o�b�N]
					HitVel(-4.8, 0, -2.8, -4);
					// [�K�[�h����]
					P1.GuardF = 2;
					

					// [��炢���̕���]
					P1.fallF = 1;
					P1.HitAnim = 1020;	// ���i��炢
					P1.A.forceKurai = true;	// ����炢��

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
					if ((P1.CFlg) && (P1.time >= 27)){

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
				// ��s���͏���
				if (P1.time <= 1)P1.Senkou[1] = 0;
				//[ �ʏ�Z�̎����U����� ]
				if (P1.time == 0){
					if ((P1.XPos < P2.XPos) && (P1.muki == 1)){
						P1.muki = 0;
					}
					else if ((P1.XPos > P2.XPos) && (P1.muki == 0)){
						P1.muki = 1;
					}
				}
				// SE��炷
				if (P1.time == 1)SEStart(1);
				//  �S�̃t���[���A14�𒴂����烊�Z�b�g
				if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;
				
				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 1, 2);
				}
				
				// �q�b�g���Z�b�g�A�����U�t��
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
					HitTime(6, 12, 16, 10);
	
					// [�m�b�N�o�b�N]
					HitVel(-3.1, 0, -1.6, -4.8);
					P1.GuardF = 3;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.HitAnim = 1010;	// ���i��炢
					P1.HitSE = 10;
				}
				else {
					DamReset();
				}
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 6)){
					if (P1.Senkou[3] > 0){
						// 6C
						if ((key(6)) && (!key(2)) && (!key(8))){
							P1.stateno = 225, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						// ���v�f���m�F
						else if (key(2)){
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
							P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else{
							P1.stateno = 200, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}// �L�����Z���A�A�ŃL�����Z��
				if ((P1.button[1] == 1) && (P1.time >= 9) && (P1.stateno == 300)){
					// ���v�f�������Ă���I
					if (P1.keyAtt[2]){
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

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 2, 1);
				}

				// �q�b�g���Z�b�g
				if (P1.time == 1){
					P1.MoveHit = 1;	// �P��
				}

				
				// �_���[�W�Z�b�g
				if (P1.time >= 1){

					// [�_���[�W]
					Damage(75, 0);

					// [�Q�[�W] 
					Power(86);

					HitTime(8, 18, 20, 16);

					// [�m�b�N�o�b�N]
					HitVel(-3.2, 0, -1.5, -4.6);
					P1.GuardF = 1;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.HitAnim = 1010;	// ���i��炢
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
						if (P1.Senkou[3] > 0){		// ��s���͌������Ă݂�
							// 6C
							if ((key(6)) && (!key(2)) && (!key(8))){
								P1.stateno = 225, P1.More = 1,
									P1.time = 0, P1.A.damage = 0;
							}
							// ���v�f���m�F
							else if (key(2)){
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
							if ((!key(2)) && (P1.Var[1] == 0)){
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
			// 320�F���Ⴊ��C
			//********************
			case 320:
				P1.ctrl = 0, P1.SFlg = 0;
				// ����
				if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
				// SE��炷
				if (P1.time == 1)SEStart(2);
				// �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 3, 0);
				}

				// �q�b�g���Z�b�g 10�t��
				if (P1.time == 1){
					P1.MoveHit = 1;	// �P��
				}
				// �_���[�W�Z�b�g
				if (P1.time >= 1){

					// [�_���[�W]
					Damage(98, 0);
					// [�Q�[�W] 
					Power(118);
					HitTime(8, 40, 40, 18);
					// [�m�b�N�o�b�N]
					HitVel(-1.8, -4, -2.2, -3.5);
					GuardVel(-3.8, -1.5);

					P1.GuardF = 3;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.HitAnim = 1030;	// �󒆋�炢(�m��_�E��)
					P1.HitSE = 12;
				}
				else{ DamReset(); }
				// ���}���u
				if (P1.StopTime == 0){
					// �L�����Z��
					if ((P1.CFlg) && (P1.time >= 10)){
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
				if (P1.time == 1)SEStart(1);

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 1, 2);
				}
				// ��s���͏���
				if (P1.time <= 1)P1.Senkou[1] = 0;
				// �q�b�g���Z�b�g 6�t��
				if (P1.time == 4)P1.MoveHit = 1;	// �P��
				// �_���[�W�Z�b�g
				if ((P1.time >= 4) && (P1.time <= 10)){

					// [�_���[�W]
					Damage(40, 0);
					// [�Q�[�W] 
					Power(30, 15);

					// [�q�b�g�X�g�b�v�E�̂����莞��]
					HitTime(5, 14, 18, 12);
					// [�m�b�N�o�b�N]
					HitVel(-4.6, 0, -1.4, -4.2);
					// �K�[�h����
					if (P1.YVel >= 0.0)P1.GuardF = 2;
					else{ P1.GuardF = 1; }
					//P1.GuardF = 1;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.HitAnim = 1000;
					P1.HitSE = 10;
				}
				else {
					DamReset();
				}
				// ����\�t���[��
				//if (P1.time >= 19)P1.ctrl = 1;
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					if (P1.Senkou[3] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (P1.Senkou[2] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 410, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}

					// [�󒆕K�E�Z]
					ACancel();

				}
				/*
				// �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.time >= ANIMELEM){
					P1.ctrl = 1, P1.More = 1,
						P1.stateno = 46, P1.time = 0;
				}
				*/
				break;
			//********************
			// 410�F�W�����vB
			//********************
			case 410:
				P1.ctrl = 0, P1.SFlg = 2;
				// SE
				if (P1.time == 1)SEStart(1);

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 2, 1);
				}

				// �q�b�g���Z�b�g
				if (P1.time == 2)P1.MoveHit = 1;	// �P��
				// �_���[�W�Z�b�g
				if (P1.time >= 2){

					// [�_���[�W]
					Damage(70, 0);

					// [�Q�[�W] 
					Power(80);
					// [�q�b�g�X�g�b�v�E�̂����莞��]
					HitTime(8, 16, 18, 14);
					// [�m�b�N�o�b�N]
					HitVel(-4.6, 0,  -1.6, -4.6);
					P1.GuardF = 2;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.HitAnim = 1000;
					P1.HitSE = 11;
				}
				else {
					DamReset();
				}
				/*
				// ����\�t���[��
				if (P1.time >= 24){
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
					if ((P1.CFlg) && (P1.time >= 6) && (P1.AirJump > 0)){
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

				// VO
				if (P1.time == 1){
					PVOStart(P1.PSide, 3, 0);
				}

				// �q�b�g���Z�b�g
				if (P1.time == 7)P1.MoveHit = 1;	// �P��
				// �_���[�W�Z�b�g�A���� 5�t��
				if ((P1.time >= 7) && (P1.time <= 11)){

					// [�_���[�W]
					Damage(95, 0);

						// [�Q�[�W] 
					Power(100);

					// [�q�b�g�X�g�b�v�E�̂����莞��]
					HitTime(8, 18, 40, 16);

					// [�m�b�N�o�b�N]
					if (P1.HitCount > 10)
					HitVel(-4.8, 0, -3.8, 4.5);
					else { HitVel(-4.8, 0, -3.8, 6.5); }

					P1.GuardF = 2;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.HitAnim = 1000;
					P1.HitSE = 12;
					P1.A.boundLevel = 1;
				}
				else {
					//DamReset();
				}
				// [�L�����Z��]
				if ((P1.CFlg) && (P1.time >= 9)){
					// [�󒆕K�E�Z]
					ACancel();
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

				if ((P1.time == 9) && (P1.StopTime == 0))PVOStart(P1.PSide, 11, 0);

				// SE
				if (P1.time == 1)SEStart(2);
				// �q�b�g���Z�b�g�A
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

					int dist = 80;
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

					// �߂��Ȃ���W���炷
					if (P2.XPos < GAMENHAJI){
						P2.XPos = GAMENHAJI;
						P1.XPos = P2.XPos + 80;
					}
					if (P2.XPos > STAGE_WIDTH - GAMENHAJI){
						P2.XPos = STAGE_WIDTH - GAMENHAJI;
						P1.XPos = P2.XPos - 80;
					}

					P2.YVel = 0;
					P2.YPos = GROUND;
				}


				// �����𗣂�
				if (P1.time == 3)P2.XVel = -2.5;
				if (P1.time == 4)P2.XVel = -2.2;
				// �q�b�g��
				//if(P1.time == 7)SEStart(11);
				// �q�b�g���A�G�t�F�N�g
				if (P1.time == 4){
					SEStart(11);
					EffStart(P1.A.hitEff, (int)P2.XPos, (int)P2.YPos, 20 + GetRand(10), -(P2.ySize / 1.5),
						P1.A.heSizeX, P1.A.heSizeY, P2.muki);
				}

				// 2P���G
				if (P1.time < 39)P2.Muteki = 1, P2.mutekiF = 0;

				// VO
				if ((P1.time == 35) && (P1.StopTime == 0)){
					PVOStart(P1.PSide, 10, 0);
				}

				// �q�b�g���Z�b�g
				if (P1.time == 40)P1.MoveHit = 1;	// �P��
				// �_���[�W�Z�b�g�A���� 1�t��
				if (P1.time == 40){

					// [�U������Z�b�g]
					ExAtt(P1.PSide, 0, 100, 90, 20, -100);

					// [�_���[�W]
					Damage(0, 180);
					P1.A.hosei_K = 0.5;
					// [�Q�[�W] 
					Power(300);
					HitTime(6, 45, 45, 0);
					// [�m�b�N�o�b�N]
					HitVel(-2, -3.4, 0, 0);
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
				if (P1.time == 4)
				{
					P1.MoveHit = 1;	// �P��
				}

				if ((P1.time == 9) && (P1.StopTime == 0))PVOStart(P1.PSide, 11, 0);

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
				P1.XVel = 0;
				P1.ignoreG = true;
				
				// �o�O�h�~�̕���
				if ((P1.time == 0) && (P1.YPos+20 >= GROUND)){
					P1.YPos = GROUND - 20;
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

					P1.YVel = 0;
					P2.YVel = 0;
				}
				// �d�͉��Z
				if (AnimElem(5)){
					P1.YVel = -5;
				}
				// �d��
				else if (P1.State2 >= 4){
					P1.YVel += 0.5;
				}
				
				// ���łɖ��G����
				if (P1.State2 < 3){
					P2.Muteki = 1, P2.mutekiF = 0;
					P2.time = 0;
					//P1.YVel += GRAVITY;
					//P2.YVel += GRAVITY;
				}
				// SE��炷
				if (P1.time == 15)SEStart(10);

				// VO
				if ((P1.time == 9) && (P1.StopTime == 0)){
					PVOStart(P1.PSide, 10, 0);
				}

				// �q�b�g���Z�b�g
				if (P1.time == 1)P1.MoveHit = 1;	// �P��
				// �_���[�W�Z�b�g�A���� 1�t��
				if (P1.time >= 1){

					// ���o�E���h
					//P1.A.boundLevel = 1;
					// [�U������Z�b�g]
					if (P1.time == 17)
					ExAtt(P1.PSide, 0, 100, 90, 20, -100);

					// [�_���[�W]
					Damage(170, 10);
					// [�Q�[�W] 
					Power(300);

					HitTime(0, 100, 100, 0);
					// [�m�b�N�o�b�N]
					HitVel(-4.8, 3.8, -4.8, 3.8);
					P1.HitAnim = 1030;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.HitSE = 11;
					
				}
				else{
					DamReset();
				}
				// ����̎p�����㔼�̂�
				if (P1.CFlg && P2.stateno == 1030 && P2.time < 20)P2.time = 20;

				// ��������
				ThrowReject();

				// ���n
				if (P1.YPos + P1.YVel * 2 >= GROUND){
					P1.time = 0, P1.stateno = 47,
					P1.ctrl = 0, P1.SFlg = 0;
					P1.YPos = GROUND;
					P1.ignoreG = false;
				}
				break;
				
			//********************
			// 600 �u������ A
			//********************
			case 600:	// A�u��
				P1.ctrl = 0, P1.SFlg = 0;
				// SE��炷
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 15, 0);


				// �w���p�[�Ăяo��.[0]
				if (P1.time == 0){
					HelperReset(0);
					H1[0].var = true;
					H1[0].time = 0;
					H1[0].stateno = 2000;
				}

				// �w���p�[�ݒ菀��.[1]	��ѓ���
				if (P1.time == 0){
					HelperReset(1);
					H1[1].var = false;

					if (P1.Var[6] == 1)H1[1].HXVel = 9.8;
					else if (P1.Var[6] == 2)H1[1].HXVel = 13.8;
					else if (P1.Var[6] == 3){
						H_VelSet(1, 6.6, -6);
					}
				}
				// �Ăяo��.[1]	��ѓ���
				else if (P1.time == 12){
					H1[1].var = true;
					H1[1].time = 0;
					H1[1].stateno = 2010;
				}

				// �q�b�g���L�����Z��
				if (P1.StopTime == 0){
					if ((H1[1].var == false) && (P1.time >= 12) && (P1.scTime > 0)){
						// [�Q�[�W�Z]
						HCancel();
					}
				}

				// �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.time >= 48)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;


				break;
			//********************
			// 605 �󒆃u������
			//********************
			case 605:	// A�u��
				P1.ctrl = 0, P1.SFlg = 2;
				P1.ignoreG = true;
				// ���x������
				if (P1.time == 0){
					P1.XVel = -1.8;
					P1.YVel = -3;
				}
				// SE��炷
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 15, 0);


				// �w���p�[�ݒ菀��.[1]	��ѓ���
				if (P1.time == 0){
					HelperReset(1);
					H1[1].var = false;

					if (P1.Var[6] == 1)H_VelSet(1, 6.2, 6.2);
					else if (P1.Var[6] == 2)H_VelSet(1, 8.9, 8.9);
					else if (P1.Var[6] == 3){
						H_VelSet(1, 9.5, 0);
					}
				}
				// �Ăяo��.[1]	��ѓ���
				else if (P1.time == 12){
					H1[1].var = true;
					H1[1].time = 0;
					H1[1].stateno = 2010;
				}

				// �d�͊ɘa
				VelAdd(0, P1.C.yAccel-0.2);
				
				// �q�b�g���L�����Z��
				if (P1.StopTime == 0){
					if ((H1[1].var == false) && (P1.time >= 12) && (P1.scTime > 0)){
						// [�Q�[�W�Z]
						ACancel();
					}
				}

				// �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.YPos + P1.YVel * 2 >= GROUND)P1.time = 0, P1.stateno = 606,
											P1.ctrl = 0, P1.SFlg = 0;

				break;

				//********************
				// 606 ���n�d��
				//********************
			case 606:
				P1.ctrl = 0, P1.SFlg = 0;
				// ���x������
				VelSet(0, 0);
				P1.YPos = GROUND;
				
				// SE��炷
				if (P1.time == 1){
					SEStart(6);
				}



				// �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.time >= 17)P1.stateno = 11, P1.SFlg = 1, P1.ctrl = 1;

				break;
			//********************
			// 610 �╥�� A
			//********************
			case 610:	// A�╥��
				P1.ctrl = 0, P1.SFlg = 0;
				// SE��炷�A�ړ�
				if (P1.time == 0){
					SEStart(21);
					P1.XVel = 4;
				}
				// �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				// �q�b�g���Z�b�g
				if (P1.time == 12){
					P1.MoveHit = 1;	// �P��
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 16, 0);

				// �_���[�W�Z�b�g
				if (P1.time >= 1){

					// [�_���[�W]
					Damage(100, 0);
			
					// [�Q�[�W] 
					Power(120);
					HitTime(6, 53, 60, 18);
					
					// [�m�b�N�o�b�N]
					HitVel(-2.8, -4.7, -2.6, -3.5);
					GuardVel(-3.8 ,-2.6 * 1.2);
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

				// �q�b�g���L�����Z��
				if (P1.StopTime == 0){
					if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
						// [�Q�[�W�Z]
						HCancel();
					}
				}

				break;
			//********************
			// 611 �╥�� B
			//********************
			case 611:
				P1.ctrl = 0, P1.SFlg = 0;
				// SE��炷�A�ړ�
				if (P1.time == 1){
					SEStart(21);
					P1.XVel = 10;
				}
				// �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				// �q�b�g���Z�b�g
				if (P1.time == 14){
					P1.MoveHit = 1;	// �P��
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 16, 0);

				// �_���[�W�Z�b�g
				if (P1.time >= 1){
		
					// [�U������Z�b�g]
					AttSet(0, 120, 100, 69, -100);

					// [�_���[�W]
					Damage(105, 0);

					// [�Q�[�W] 
					Power(130);
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
			// 620 �ݒu�i�m�[�}���j
			//********************
			case 620:
				P1.ctrl = 0, P1.SFlg = 0;
				// SE��炷
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 17, 0);

				// �w���p�[�Ăяo��.[1]
				if (P1.time == 0){
					HelperReset(0);
					H1[0].var = true;
					H1[0].time = 0;
					H1[0].stateno = 2000;
				}
				// �w���p�[�Ăяo��.[2]	�ݒu
				if (P1.time == 12){
					HelperReset(2);
					H1[2].var = true;
					H1[2].time = 0;
					H1[2].stateno = 2020;
				}

				// �w���p�[�폜
				if (P1.time >= 23){
					HelperReset(0);
					H1[0].var = false;
				}
				// �S�̃t���[���𒴂����烊�Z�b�g
				if (P1.time >= 36)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

				break;
			//********************
			// 622 �ݒu�i�󒆁j
			//********************
			case 622:
				P1.ctrl = 0, P1.SFlg = 2;
				P1.ignoreG = true;
				// ���x������
				if (P1.time == 0){
					P1.XVel = -2;
					P1.YVel = -2.8;
				}
				/*
				if (AnimElem(2)){
					VelSet(-1.1, 0);
				}
				*/
				// SE��炷
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 17, 0);

				// �w���p�[�Ăяo��.[2]	�ݒu
				if (P1.time == 12){
					HelperReset(2);
					H1[2].var = true;
					H1[2].time = 0;
					H1[2].HYVel = 5;
					H1[2].stateno = 2020;
				}
				// �d�͊ɘa
				VelAdd(0, P1.C.yAccel-0.2);

				// ���n
				if (P1.YPos + P1.YVel * 2 >= GROUND)P1.time = 0, P1.stateno = 606, P1.ctrl = 0;

				break;
			//********************
			// 625 ����
			//********************
			case 625:
				P1.ctrl = 0, P1.SFlg = 0;
				// SE��炷
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 18, 0);

				// �w���p�[�ύX.[2]�A���ˑO�Ȃ�ύX
				if (P1.time == 0){
					if ((H1[2].var) && (H1[2].stateno == 2020)){
						//HelperReset(2);
						H1[2].var = true;
						H1[2].time = 0;
						H1[2].stateno = 2025;

						// ���o�[�����ő��x�ύX
						if (P1.keyAtt[6])H1[2].HXVel = 9.2;
						else if (P1.keyAtt[4])H1[2].HXVel = -6.4;
					}
				}
				

				// �I���E19F
				if (P1.time >= 19)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

				break;
			//********************
			// 626 �󒆔���
			//********************
			case 626:
				P1.ctrl = 0, P1.SFlg = 2;
				// SE��炷
				if (P1.time == 1){
					SEStart(2);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 18, 0);

				// �w���p�[�ύX.[2]�A���ˑO�Ȃ�ύX
				if ((H1[2].var) && (H1[2].stateno == 2020)){
					//HelperReset(2);
					H1[2].var = true;
					H1[2].time = 0;
					H1[2].stateno = 2025;

					// ���o�[�����ő��x�ύX
					if (P1.keyAtt[6])H1[2].HXVel = 9.2;
					else if (P1.keyAtt[4])H1[2].HXVel = -6.4;
				}
				// ���Ԍo�߂ŃW�����v�~���
				if (P1.time >= ANIMELEM)P1.ctrl = 1;

				// ���n�ŏI��
				if (P1.YPos + P1.YVel * 2 >= GROUND)P1.time = 0, P1.stateno = 47, P1.More = 1;

				break;
			//********************
			// 700 ��u��[5]
			//********************
			case 700:
				P1.ctrl = 0, P1.SFlg = 0;
				P1.A.gaugeHosei = true;
				// �Q�[�W����
				if (P1.time == 1){
					P1.Power -= 500;
					SEStart(31);
					P1.colorCTime = 20;
					P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
				}
				// SE��炷
				if (P1.time == 2){
					SEStart(22);
				}

				if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 19, 0);

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

				// �O�i
				if (P1.time == 1){
					VelSet(12.6,0);
				}
				if (P1.time <= 16){
					VelAdd(0.4, 0);
				}
				else{ P1.XVel = 0; }
				// SE��炷
				if (P1.time == 7){
					SEStart(21);
				}

				if ((P1.time == 0) && (P1.StopTime == 0))PVOStart(P1.PSide, 20, 0);

				// ���G
				if (P1.time <= 14)P1.Muteki = 1, P1.mutekiF = 0;
				else { P1.Muteki = 0; }

				// �q�b�g���Z�b�g	14�t���`
				if (P1.time == 13){
					P1.MoveHit = 1;	// �P��
				}
				// �_���[�W�Z�b�g�A���� 2 �t���[���H
				if (P1.time >= 1){
	
					// [�U������Z�b�g]
					AttSet(0, 55, 115, 25, -115);

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
				}
				else {
					DamReset();
				}
				// ����������ύX
				if ((P1.CFlg) && (P2.stateno >= 1000)&& (P1.time >= 13)){
					P1.stateno = 801;
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

				// ���� �� �S�̃t���[���A54�𒴂����烊�Z�b�g
				if (P1.time >= ANIMELEM){
					P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				}

				break;
			//********************
			// 801 ��i�؂�F���o
			//********************
			case 801:
				P1.ctrl = 0, P1.SFlg = 0;
				P1.Muteki = 1, P1.mutekiF = 0;
				// ���� �� �S�̃t���[���A113�𒴂����烊�Z�b�g
				if (P1.time > 113){
					P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				}
				// SE��炷
				if (P1.time == 11){
					SEStart(22);
				}

				if ((P1.time == 42) && (P1.StopTime == 0))PVOStart(P1.PSide, 21, 0);

				// 2P���G
				if (P1.time < 62){
					P2.Muteki = 1, P2.mutekiF = 0;
				}

				// �q�b�g���Z�b�g	14�t���`
				if (P1.time == 62){
					P1.MoveHit = 1;	// �P��
				}
				// �_���[�W�Z�b�g�A���� 2 �t���[���H
				if (P1.time == 62){

					// [�U������Z�b�g]
					ExAtt(P1.PSide, 0, 60, 35, 25, -165);

					// [�_���[�W]
					Damage(160, 80);
					// [�Q�[�W] 
					Power(0, 240);
					P1.HitStop = 2, 
						P1.G_HitTime = 90, P1.A_HitTime = 20;
					P1.G_GuardTime = 0;
					// [�m�b�N�o�b�N]
					HitVel(-3.4, -4.1, -3.4, -4.1);
					P1.GuardF = 1;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.HitAnim = 1000;
					P1.HitSE = 13;
					// �G�t�F�N�g
					HitEff(1, 1, 1);
					// �n�k�G�t�F�N�g
					P1.A.quakeTime = 3;
					P1.A.quakeY = 2;
				}
				else {
					DamReset();
				}
				break;
			//********************
			// 820 �^��u�[������
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
					// �q�b�g���ݒ�
					P1.Var[11] = 0;
				}

				if ((P1.time == 0) && (P1.StopTime == 0))PVOStart(P1.PSide, 22, 0);

				// �w���p�[�ύX.[2]�A���ˑO�Ȃ�ύX
				if (P1.time == 4){
					if ((H1[2].var) && (H1[2].stateno == 2020)){
						H1[2].var = true;
						H1[2].time = 5;

						if (P2.XPos < H1[2].XPos)
							H1[2].muki = 1;
						else if (P2.XPos > H1[2].XPos)
							H1[2].muki = 0;

						H1[2].YPos += 100;
						H1[2].stateno = 2100;
						P1.Var[12] = H1[2].XPos;	// ���W
						P1.Var[13] = 0;	// ���ԃ��Z�b�g
					}
				}


				// �I���E34
				if (P1.time >= 34)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

				break;
				//********************
				// 830�F���t�؂�
				//********************
			case 830:
				P1.ctrl = 0, P1.SFlg = 2;
				P1.ignoreG = true;
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

				if ((P1.time == 0) && (P1.StopTime == 0))PVOStart(P1.PSide, 23, 0);

				// SE
				if (P1.time == 1)SEStart(3);

				if (P1.time == 0)VelSet(0, -0.1);
				if (P1.time == 5)VelSet(0, 12);
				if (P1.time > 5)VelAdd(0, GRAVITY);

				// �q�b�g���Z�b�g
				if (P1.time == 1)P1.MoveHit = 1;	// �P��
				// �_���[�W�Z�b�g�A���� 5�t��
				if (P1.time >= 1){

					// [�_���[�W]
					Damage(190, 30);
					// [�Q�[�W] 
					Power(0, 330);

					// [�q�b�g�X�g�b�v�E�̂����莞��]
					HitTime(10, 68, 60, 14);

					// [�m�b�N�o�b�N]
					HitVel(-1.4, -1.8, -0.4, 12.0);
					P1.GuardF = 1;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.A.boundLevel = 2;
					P1.A.bound_xvel = -1.6;
					P1.A.bound_yvel = -3.4;
					P1.HitAnim = 1000;
					P1.HitSE = 14;
					// �G�t�F�N�g
					HitEff(1, 1, 1);
				}
				else {
					//DamReset();
				}
				if (P1.YPos + P1.YVel * 2 >= GROUND){
					P1.stateno = 831, P1.More = 1, P1.time = 0;
					P1.SFlg = 0;
					VelSet(0, 0);
					P1.ignoreG = false;
				}

				break;
				//********************
				// 831�F�W�����vD���n
				//********************
			case 831:
				P1.SFlg = 0, P1.A.damage = 0, P1.ctrl = 0;
				P1.YPos = GROUND;
				P1.ignoreG = false;
				VelSet(0, 0);
				if (P1.time == 0){
					SEStart(8);
					EffStartB(16, P1.XPos, P1.YPos, 0, -1,
						0.20, 0.04, P1.muki);
				}

				// �q�b�g���X�p�L����
				if (P1.StopTime == 0){
					if ((P1.time >= 0) && (P2.stateno >= 1000)){
						// [�Q�[�W�Z]
						HCancel();
					}
				}

				// SE��炷
				if (P1.time == 1)SEStart(6);
				// �ŏ��Ƀ��b�N�I
				if (P1.time == 0)P1.Lock = 1;
				// �O�W�����v �� �S�̃t���[��
				if (P1.time >= ANIMELEM)P1.More = 1, P1.stateno = 0,
					P1.time = 0, P1.Lock = 0;
				break;
			//********************
			// 850 ���u�[�������i3�Q�[�W�j
			//********************
			case 850:
				P1.ctrl = 0, P1.SFlg = 0;
				P1.A.gaugeHosei = true;
				// �Q�[�W����
				if (P1.time == 1){
					P1.Power -= 3000;
					S.StopTime = 10;
					S.Anten = 30;
					SEStart(25);
					// ���i�G�t�F�N�g
					EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
				}

				if ((P1.time == 0) && (P1.StopTime == 0))PVOStart(P1.PSide, 25, 0);

				// ���G
				/*
				if (P1.time <= 2){
					P1.mutekiF = 0;
					P1.Muteki = 1;
				}
				*/

				// �q�b�g���Z�b�g 10�t��
				if (P1.time == 1){
					P1.MoveHit = 1;	// �P��
				}
				// �_���[�W�Z�b�g
				if ((P1.time >= 1) && (P1.time <= 10)){

					// [�_���[�W]
					Damage(0, 0);
					// [�Q�[�W] 
					Power(0, 0);
					HitTime(12, 300, 300, 15);
					// [�m�b�N�o�b�N]
					//HitVel(-0.5, -14.8, -0.5, -14.0);
					HitVel(-0.5, -7.6, -0.5, -7.2);
					GuardVel(-3.2, -2.8);
					P1.GuardF = 1;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.HitAnim = 1030;	// �󒆋�炢(�m��_�E��)
					P1.HitSE = 12;
					P1.A.yaccel = 0.2;
					P1.A.addMuteki = true;
					P1.A.amTime = 30;
				}

				if ((P1.CFlg) && (P2.stateno >= 1000) && (P1.time > 0)){
					P1.stateno = 851, P1.time = 0;
					P1.A.addMuteki = false;
				}

				// �I��
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
				break;
				//********************
				// 851 �R�Q�[�W���o
				//********************
			case 851:
				P1.ctrl = 0, P1.SFlg = 0;
				P1.Muteki = 1, P1.mutekiF = 0;

				// �q�b�g���Z�b�g 10�t��
				if (P1.time == 1){
					SEStart(23);	// ��
					S.Anten = 80;

					P1.Var[15] = (int)P1.XPos;
					if (P1.muki == 0){
						P1.Var[15] += 110;
					}
					else{ P1.Var[15] -= 110; }
				}
				// �U������
				if (AnimElem(18) || AnimElem(19) || AnimElem(20)){
					P1.MoveHit = 1;	// �P��
				}
				if (AnimElem(21)){
					P1.MoveHit = 1;
				}

				if ((P1.State2 != P1.oldState2) && (P1.State2 == 14)){
					S.noGauge = 15;
					S.StopTime = 15;
					SEStart(25);
				}

				if ((P1.State2 != P1.oldState2) && (P1.State2 == 16)){
					EffStart(6, P1.XPos, (int)P1.YPos, 110, -160,
						1.2, 0.9, P1.muki);
				}

				if ((P1.State2 != P1.oldState2) && (P1.State2 == 18)){
					VelSet(-12, 0);
					PVOStart(P1.PSide, 26, 0);
				}

				// �w���p�[�߂�u��
				if ((P1.State2 != P1.oldState2) && (P1.State2 == 33)){
					HelperReset(0);
					H1[0].var = true;
					H1[0].time = 0;
					H1[0].stateno = 2150;

					SEStart(27);
				}

				// �L���b�`��
				if ((P1.State2 != P1.oldState2) && (P1.State2 == 35)){
					SEStart(28);
				}
				
				// �_���[�W�Z�b�g
				if ((P1.State2 >= 17) && (P1.State2 <= 22)){

					// [�_���[�W]
					Damage(90, 15);
					// [�Q�[�W] 
					Power(0, 150);
					
					HitTime(19, 200, 200, 15);
					P1.HSSelf = 0;
					// [�m�b�N�o�b�N]
					HitVel(-2.6, -6.2, -2.6, -6.2);
					P1.GuardF = 1;
					// [��炢���̕���]
					P1.fallF = 1;
					P1.A.boundLevel = 1;
					P1.HitAnim = 1030;	// �󒆋�炢(�m��_�E��)
					P1.HitSE = 13;
					if (AnimElem(21)){
						P1.HitSE = 14;
						Damage(0, 165);
					}
					// �G�t�F�N�g
					HitEff(3, 0.6, 0.6);

				}
				else{ DamReset(); }

				// �I��
				if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
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

					// �S�̃t���[��33or���̃X�e�[�g��
					if ((H1[i].time > 33) || 
						(P1.stateno != 600 && P1.stateno != 620)){
						HelperReset(i);
					}
					break;

					//********************
					// 2010 ���u�� �i�n��Łj
					//********************
				case 2010:


					//***************
					//  ��{�ݒ�
					//***************
					// ���W�E���x�ݒ�
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						
						// ���x�Ȃ�������ݒ�
						if (H1[i].HXVel == 0){
							H_VelSet(i, 6.1, 0);
						}
						
						H_PosSet(i, 20, -100);
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
						H_HitTime(i, 6, 24, 26, 16);
						H1[i].HSSelf = 0;

						H1[i].fallF = 2;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H1[i].H_GX = -2.5, H1[i].H_GY = 0;
						H1[i].H_AX = -1.6, H1[i].H_AY = -3.2;
					}


					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 16)H1[i].HAnimTime = 0;
					// �I�������A�[�ɓ��Bor�n�ʂɓ�����or�q�b�gor�{�̔�_��
					if ((H1[i].XPos < (S.ScroolX + GAMENHAJI)) ||
						(H1[i].XPos >(S.ScroolX + SCREEN_W - GAMENHAJI)) ||
						(H1[i].YPos > GROUND) ||
						(H1[i].HMoveHit == 0) ||
						(P1.stateno >= 1000)
						)
					{
						HelperReset(i);
					}

					break;

					//********************
					// 2020 �ݒu�m�[�}�� 
					//********************
				case 2020:

					//***************
					//  ��{�ݒ�
					//***************
					// ���W�E���x�ݒ�
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H1[i].HXVel = 0;

						H_PosSet(i, 20, -100);
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 0){
						H1[i].HMoveHit = 0;
						H1[i].A.damage = 75;
					}

					// �ʒu��ς���
					if (H1[i].muki == 0){
						H1[i].XPos += H1[i].HXVel;
					}
					else{ H1[i].XPos -= H1[i].HXVel; }

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 40)H1[i].HAnimTime = 0;
					// �I���ԍۂɔ�������
					if (H1[i].time >= 300)H1[i].Alpha = true;
					// �I�������A���Ԍo��
					if (H1[i].time >= 360){
						HelperReset(i);
					}

					break;
					//********************
					// 2025 �h��D (����)
					//********************
				case 2025:

					//***************
					//  ��{�ݒ�
					//***************
					H1[i].Alpha = false;
					if (H1[i].time == 0)H1[i].muki = P1.muki;

					// ���W�E���x�ݒ�
					// ���x�� 625 �Ō��߂�
					// ���x���Ȃ��Ƃ��͂����Ō��߂�
					if (H1[i].time == 0 && H1[i].HXVel == 0){
						H1[i].HXVel = 5.4;
					}
					// �����]��
					if ((H1[i].HXVel < 0) &&
						((H1[i].muki == 0 && H1[i].XPos < (S.ScroolX + 20)) ||
						(H1[i].muki == 1 && H1[i].XPos >(S.ScroolX + SCREEN_W - 20)))){
						H1[i].HXVel = -H1[i].HXVel - (H1[i].HXVel * 0.2);
						SEStart(5);
					}

					// ����炷
					if (H1[i].time == 3){
						SEStart(5);
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 4){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 75;
					}

					// �_���[�W�ʒu�Z�b�g�A5�t���ȍ~
					if (H1[i].time >= 4){

						//[ �U������Z�b�g ]
						//H2_AttSet(i, 0, 50, 30, -20, -38);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 50, H1[i].HGivePow = 25;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;

						// [�q�b�g�d��]
						H_HitTime(i, 6, 20, 34, 20);
						H1[i].HSSelf = 0;

						H1[i].fallF = 2;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H1[i].H_GX = -1.9, H1[i].H_GY = 0;
						H1[i].H_AX = -1.2, H1[i].H_AY = -3;
					}

					// �o�E���h
					if (H1[i].YPos + H1[i].HYVel >= GROUND){
						H1[i].HYVel = -H1[i].HYVel;
						SEStart(5);
					}
					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 24)H1[i].HAnimTime = 0;
					// �I�������A���̌����Œ[���Bor�U���q�b�g
					if ((H1[i].XPos < 0 || H1[i].XPos > STAGE_WIDTH) ||
						(H1[i].muki == 1 && H1[i].XPos < (S.ScroolX)) ||
						(H1[i].muki == 0 && H1[i].XPos >(S.ScroolX + SCREEN_W)) ||
						(H1[i].HMoveHit == 0 && H1[i].time >= 4) ||
						(P1.stateno >= 1000)
						)
					{
						HelperReset(i);
					}
					break;

					//********************
					// 2030 ��u�[������ 
					//********************
				case 2030:

					//***************
					//  ��{�ݒ�
					//***************
					// ���W�ݒ�
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H1[i].HXVel = 8;

						H_PosSet(i, 0, -100);
					}
					// ���x�ݒ�
					if (H1[i].time == 0){
						H1[i].HXVel = 12.0;
					}
					// ���x�ݒ�
					if (H1[i].time == 16){
						H1[i].HXVel = 1.4;
					}
					if (H1[i].time == 31){
						H1[i].HXVel = -13.4;
					}

					
					// �q�b�g���E�_���[�W�Z�b�g
					if ((H1[i].time == 1) || (H1[i].time == 16) ||
						(H1[i].time == 31)){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 65;
					}

					// �_���[�W�ʒu�Z�b�g�A2�t���ȍ~
					if (H1[i].time >= 1){

						//[ �U������Z�b�g ]
						//H2_AttSet(i, 0, 80, 60, -40, -108);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 12, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 42;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;

						// [�q�b�g�d��]
						H_HitTime(i, 6, 36, 40, 16);
						H1[i].HSSelf = 0;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;

						// [�m�b�N�o�b�N]
						H_HitVel(i, 
							-1.9, 0, -2.1, -3);
						if (H1[i].time >= 16){
							H_HitVel(i, 0.6, 0,
								0.5, -1.6);
						}
						if (H1[i].time >= 25){
							H_HitVel(i, 2.1, -4.1, 
										2, -4.3);
						}

						// [�␳]
						if (H1[i].time >= 31)P1.A.hosei_K = 0.4;

					}

					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= P1.animElem[H1[i].stateno])H1[i].HAnimTime = 0;
					// �I�������A���Ԍo��or��炢
					if ((H1[i].time > 44) || (P1.stateno >= 1000))
					{
						HelperReset(i);
					}
					break;

				//********************
				// 2100 �^��u�[������
				//********************
				case 2100:
					
					//***************
					//  ��{�ݒ�
					//***************
					H1[i].HYVel = 0;
					if (H1[i].YPos >= GROUND)H1[i].YPos = GROUND;
					H1[i].Alpha = true;	//������
					P1.A.gaugeHosei = true;
					P1.Var[13]++;	// ���ԉ��Z

					//SE
					if (H1[i].time == 6 && P1.Var[11] == 0)SEStart(23);


					// ���W�E���x�ݒ�
					// ���x���Ȃ��Ƃ��͂����Ō��߂�
					if (H1[i].time == 5 && H1[i].HXVel == 0){
						H1[i].HXVel = 0;// 1.0;
					}
					if (H1[i].time == 7){
						H1[i].HXVel = 0;//5.0;
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 6){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 54;
						H1[i].A.hosyo = 6;
					}
					
					// �_���[�W�ʒu�Z�b�g�A2�t���ȍ~
					if (H1[i].time >= 6){


						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 13, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 80;

						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						H1[i].fallF = 1;

						// [�q�b�g�d��]
						//H_HitTime(i, 6, 32, 60, 14);
						H_HitTime(i, 6, 32, 60, 14);
						H1[i].HSSelf = 1;
						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1030;
						// [�m�b�N�o�b�N]
						H1[i].H_GX = 0, H1[i].H_GY = -2;
						H1[i].H_AX = 0, H1[i].H_AY = -2.8;
						// �G�t�F�N�g
						h_HitEff(i, 3, 0.6, 0.6);
					}
					// �U�����q�b�g������A
					if (H1[i].HMoveHit == 0 && H1[i].time >= 6 && H1[i].stopTime == 0)
					{
						H1[i].time = 3;	// 0
						H1[i].HXVel = 0;
						P1.Var[11] += 1;
					}

					if (H1[i].stopTime == 0){
						H_Move(i);
					}
					
					// �G�t�F�N�g�Ƃ��ĕ`��
					EasyEffDraw(5, H1[i].HAnimTime, (int)H1[i].XPos, (int)H1[i].YPos, 0, 0, 0.55, 0.55, H1[i].muki);

					//***************
					//  �s�����e
					//***************

					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 10)H1[i].HAnimTime = 0;
					// �I�������A�[���Bor�T��q�b�g
					if (//(H1[i].XPos < (S.ScroolX - H1[i].WAtt[0])) ||
						//(H1[i].XPos > (S.ScroolX + SCREEN_W + H1[i].WAtt[0])) ||
						(H1[i].XPos < GAMENHAJI) ||
						(H1[i].XPos >(STAGE_WIDTH - GAMENHAJI)) ||
						(P1.Var[11] >= 5) ||
						//(P1.stateno >= 1000) ||
						(H1[i].time > 300)
						//|| (H1[i].XPos > P1.Var[12] + 290 || 
						//H1[i].XPos < P1.Var[12] - 290)
						)
					{
						P1.Var[11] = 0;
						P1.Var[12] = 0;
						HelperReset(i);
					}
					break;
					
					//********************
					// 2150 ���߂�u�� 
					//********************
				case 2150:

					//***************
					//  ��{�ݒ�
					//***************	
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H_PosSet(i, 18, -160);
					}
					//***************
					//  �s�����e
					//***************

					// �S�̃t���[��or��_��
					if ((H1[i].time > P1.animElem[H1[i].stateno]) ||
						(P1.stateno >= 1000)){
						HelperReset(i);
					}
					break;

					//********************
					// 2552 ���g 
					//********************
				case 2552:
					//***************
					//  ��{�ݒ�
					//***************	
					if (H1[i].time == 0){
						H1[i].muki = P1.muki;
						H_PosSet(i, 0, 0);
					}
					//***************
					//  �s�����e
					//***************
					//H1[i].HImage = P1.Image;

					// �X�e�[�g���V�����Ȃ�����
					//if (H1[i].stateno != H1[i].oldState){
						//H1[i].Reset();
					//}
					if(H1[i].time == 0)H1[i].Reset();

					// �S�̃t���[��or��_��
					if (H1[i].time > 2000){
						HelperReset(i);
					}
					break;

					//===============================//
				//******************
				// �w���p�[�����I��
				//******************

				// �X�e�[�g�ԍ���ۑ�
				H1[i].oldState = H1[i].stateno;

				}// switch�I��
			}// �I���E�I�t���f�I��
		}// ���̃w���p�[��
	}// for�I��

	// �l��Ԃ�
	Get_SSet(S);
	if (P1.PSide == 1)Get_PSet(P1, P2), Get_HSet(H1, H2);//, Get_SSet(S);
	if (P1.PSide == 2)Get_PSet(P2, P1), Get_HSet(H2, H1);//, Get_SSet(S);

}//StateParam�I��

//-------------------------------------------------------------------------------
//
// �󂯎��p
//
//-------------------------------------------------------------------------------

// ���Ԃ��t�ɂ���
void GetP_EinParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_EinParam(System_t GS)
{
	S = GS;
}

void GetH_EinParam(Helper GH1[], Helper GH2[])
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




// �K�E�Z�L�����Z��
void SCancel()
{


	// [�K�E�Z]
	// D (�m�[�}���ݒu�A�n�㔭��)
	if (P1.Senkou[4] > 0){
		// �n��ɂ��āA�w���p�[[2]�����݂��Ȃ�
		if ((!H1[2].var) && (P1.SFlg != 2)){
			P1.stateno = 620, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
		// �w���p�[[2]������
		if (H1[2].var){
			if (P1.SFlg != 2){
				P1.stateno = 625, P1.More = 1,
					P1.time = 0, P1.A.damage = 0;
			}
			else if (P1.SFlg == 2){
				P1.stateno = 626, P1.More = 1,
					P1.time = 0, P1.A.damage = 0;
			}
		}
	}

	//  ����
	if ((P1_BInput(1) == 1) && (P1_BInput(4) == 1) && (P1.ctrl)){
		if (P1.SFlg != 2){		// ����
			P1.stateno = 500;	// ����
		}
		else {
			P1.stateno = 510;	// �󓊂�
		}
	}
	// 236 + �U��
	if ((P1.cmd[1]) && (!H1[1].var)){		// ��ʂɃw���p�[�P���Ȃ�
		if ((P1.Senkou[1] > 0) ||
			(P1.Senkou[2] > 0) ||
			(P1.Senkou[3] > 0)){
			P1.stateno = 600, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
			if (P1.Senkou[1] >= P1.Senkou[2]){
				if (P1.Senkou[1] >= P1.Senkou[3])P1.Var[6] = 1;
				else{ P1.Var[6] = 3; }
			}
			else if (P1.Senkou[2] >= P1.Senkou[3])P1.Var[6] = 2;
			else{ P1.Var[6] = 3; }
		}
	}

	// 214 + AorB
	if ((P1.Senkou[2] > 0)
		&& (P1.cmd[2])){		// ��s���͌������Ă݂�
		P1.stateno = 611, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	else if ((P1.Senkou[1] > 0)
		&& (P1.cmd[2])){		// ��s���͌������Ă݂�
		P1.stateno = 610, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// [EX�Z] 0.5�Q�[�W
	if ((P1.Senkou[4] > 0)
		&& (P1.cmd[1]) && (P1.Power >= 500)){
		P1.stateno = 700, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}

	// H�����Ē����B
	HCancel();

	//�Ō�ɔ��������
	DamReset();
}


// ���K�L�����Z��
void HCancel()
{

	// [���K]
	//  214+C		[��i�؂�]
	if ((P1.Senkou[3] > 0) && (P1.cmd[2])
		&& (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 800, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	// [ �^��u�[������ ]
	if ((P1.Senkou[4] > 0) && (P1.cmd[3])
		&& (P1.Power >= 1000)
		&& (H1[2].var) && (H1[2].stateno == 2020)){
		P1.stateno = 820, P1.More = 1,
			P1.time = 0;
	}

	// [ ���u�[������ ]
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
	DamReset();
}

// �󒆕K�L�����Z��
void ACancel()
{
	// [�K�E�Z]
	// D (�m�[�}���ݒu�A�n�㔭��)
	if (P1.Senkou[4] > 0){
		// �w���p�[[2]������
		if (H1[2].var){
			if (P1.SFlg == 2){
				P1.stateno = 626, P1.More = 1,
					P1.time = 0, P1.A.damage = 0;
			}
		}
		// �ݒu���Ȃ�
		else if (!H1[2].var){
			if (P1.SFlg == 2){
				P1.stateno = 622, P1.More = 1,
					P1.time = 0, P1.A.damage = 0;
			}
		}
	}

	// 236 + �U��
	if ((P1.cmd[1]) && (!H1[1].var)){		// ��ʂɃw���p�[�P���Ȃ�
		if ((P1.Senkou[1] > 0) ||
			(P1.Senkou[2] > 0) ||
			(P1.Senkou[3] > 0)){
			P1.stateno = 605, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
			if (P1.Senkou[1] >= P1.Senkou[2]){
				if (P1.Senkou[1] >= P1.Senkou[3])P1.Var[6] = 1;
				else{ P1.Var[6] = 3; }
			}
			else if (P1.Senkou[2] >= P1.Senkou[3])P1.Var[6] = 2;
			else{ P1.Var[6] = 3; }
		}
	}

	// [���K]
	//  214+C		[��i�؂�]
	if ((P1.Senkou[3] > 0) && (P1.cmd[2])
		&& (P1.Power >= 1000)){
		if (P1.SFlg == 2){		// �n��
			P1.stateno = 830, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	//�Ō�ɔ��������
	DamReset();
}