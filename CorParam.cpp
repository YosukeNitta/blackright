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
void CorParam(void)
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
		if (P1.ctrl){
			P1.Var[1] = 0;
			P1.Var[2] = 0;
			P1.Var[4] = 0;
		}

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
			// �ŏ��Ƀ��b�N
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
			
			//if (P1.AnimTime <= 8)P1.XVel = P1.C.runF[0] * (0.1 * (P1.AnimTime + 1)); //���x�𑫂�;
			if (P1.AnimTime <= 8)P1.XVel = P1.C.runF[0] * (0.1 * ((float)P1.AnimTime + 1)); //���x�𑫂�;
			if (P1.AnimTime <= 6)P1.ctrl = 0; //���x�𑫂�
																					 // SE��炷
			if ((P1.time == 14) || (P1.time == 31))SEStart(4);
			if (P1.time > ANIMELEM)P1.time = 0;

			break;
		case 35:	// �o�b�N�X�e�b�v
			P1.SFlg = 0, P1.ctrl = 0, P1.Lock = 0;

			// ���G
			if (P1.time <= 3){
				P1.Muteki = 1, P1.mutekiF = 0;
			}

			// SE��炷
			if (P1.time == 1)SEStart(5);

			//if ((P1.YPos == GROUND) && (P1.time == 0))P1.YVel = -P1.C.runB[1];
			if(P1.time <= 1)P1.XVel = P1.C.runB[0] * 2.5; //���x�𑫂�

			//-----------------
			// �h��
			//-----------------
			// �o�N�X�e
			if ((P1.time >= 1) && (P1.key[4] == 1)){
				P1.stateno = 37;
				P1.time = 0;
				P1.SFlg = 0;
				P1.More = 1;
			}

			// �I��
			if (P1.time >= ANIMELEM){
				P1.stateno = 36;
				P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
			}

			break;

		case 36:	// �o�N�X�e�d��
			P1.SFlg = 0, P1.ctrl = 0;

			VelSet(0, 0);

			// �I��
			if (P1.time >= ANIMELEM){
				P1.stateno = 0;
				P1.time = 0;
				P1.SFlg = 0;
			}

			break;

		case 37:	// �h���o�N�X�e
			P1.SFlg = 0, P1.ctrl = 0, P1.Lock = 0;

			// SE��炷
			if (P1.time == 1)SEStart(5);

			//if ((P1.YPos == GROUND) && (P1.time == 0))P1.YVel = -P1.C.runB[1];
			if (P1.time == 2)P1.XVel = P1.C.runB[0] * 2.5; //���x�𑫂�
			else if (P1.time < 2){ P1.XVel = 0; }

			//-----------------
			// �h��
			//-----------------
			// �o�N�X�e
			if ((P1.time >= 3) && (P1.key[4] == 1)){
				P1.stateno = 37;
				P1.time = 0;
				P1.SFlg = 0;
				P1.More = 1;
			}

			// �I��
			if (P1.time >= ANIMELEM){
				P1.stateno = 36;
				P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
			}

			break;
		case 90:	// �J��
			P1.SFlg = 0, P1.ctrl = 0;

			// �ŏ��̈ʒu
			if (P1.time == 0){
				if (P1.muki == 0){
					//�������W���Z�b�g
					P1.XPos = (STAGE_WIDTH / 2) - 366;
				}
				else{ P1.XPos = (STAGE_WIDTH / 2) + 366; }
			}

			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SE��炷
			if (P1.time == 1 || P1.time == 37)SEStart(26);

			// �X�����x�Z�b�g
			//if (P1.time <= 8 && P1.time > 0)VelSet(9,0);
			//if (P1.time == 14)VelSet(5,0);

			if (P1.time == 4 || P1.time == 40)VelSet(10, 0);
			if (P1.time == 10 || P1.time == 46)VelAdd(2, 0);

			if ((P1.muki == 0) && (P1.XPos > (STAGE_WIDTH / 2) - 120)){
				P1.XPos = (STAGE_WIDTH / 2) - 120;
			}
			else if ((P1.muki == 1) && (P1.XPos < (STAGE_WIDTH / 2) + 120)){
				P1.XPos = (STAGE_WIDTH / 2) + 120;
			}

			// �S�̃t���[���𒴂����痧����
			if (P1.time >= P1.animElem[P1.stateno]){
				P1.time = 0;
				P1.stateno = 0;
				// �ŏ��̈ʒu
				if (P1.muki == 0){
					//�������W���Z�b�g
					P1.XPos = (STAGE_WIDTH / 2) - 120;
				}
				else{ P1.XPos = (STAGE_WIDTH / 2) + 120; }
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
			// button�͂��̂܂�
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

			// �q�b�g���Z�b�g 
			if (P1.time == 0 && P1.MoveHit == 0){
				P1.MoveHit = 1;	// �P��
			}
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
			if ((P1.time >= 1) && (P1.MoveHit == 1)){

				// [�_���[�W]
				Damage(46, 0);

				// [�Q�[�W] 
				Power(40);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 12, 16, 10);
				// [�m�b�N�o�b�N]
				HitVel(-3.8, 0, -2.2, -4.6);
				GuardVel(-4.2, -3.4);

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
			// �A�ŃL�����Z��
			if (P1.time >= ANIMELEM - 1){
				// ���v�f�������ĂȂ�
				if ((P1.Senkou[1] > 0) &&
					(P1.button[102] == 0) && (P1.CFlg == 0)){
					P1.More = 1, P1.time = 0, P1.A.damage = 0;
				}
			}
			// �q�b�g���`�F�[��
			if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
				// [�W�����v�L�����Z��]
				if (P1.keyAtt[8]){		// ��s���͌������Ă݂�
					P1.stateno = 40, P1.More = 1,
						P1.time = 0, P1.A.damage = 0;
				}
				// [�ʏ�Z]
				if (P1.Senkou[3] > 0){
					// �X��
					if (key(3)){
						P1.stateno = 321, P1.More = 1,
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
					// 6B
					if ((key(6)) && (!key(2)) && (!key(8))){
						P1.stateno = 215, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)){
						P1.stateno = 310, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else{
						if (P1.Senkou[1] == 0){
							P1.stateno = 210, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
				}
				else if (P1.Senkou[1] > 0){		
					if (key(2) && (P1.Var[4] == 0)){
						P1.stateno = 300, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
						P1.Var[4]++;
					}
				}
				// [�K�E�Z�E�Q�[�W�Z]
				SCancel();
			}

			// ����
			DelayThrow(1);
			// 3�Q�[�W
			Delay3Button(850, 3000);

			break;
			
			//********************
			// 210�F����B
			//********************
		case 210:	//����B
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// �q�b�g���Z�b�g 
			if (P1.time == 6){
				P1.MoveHit = 1;	// �P��
			}
			if (P1.time == 1)VelSet(2.0, 0);
			// �ʒu�����炷
			if (AnimElem(2)){
				PosAdd(6, 0);
			}
			if (AnimElem(3)){
				PosAdd(10, 0);
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// �_���[�W�Z�b�g
			if ((P1.time >= 3) && (P1.time <= 12)){

				// [�_���[�W]
				Damage(82, 0);

				// [�Q�[�W] 
				Power(94);
				HitTime(8, 18, 20, 14);

				// [�m�b�N�o�b�N]
				HitVel(-4.2, 0, -2.0, -4.8);
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
				if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
					// [�W�����v�L�����Z��]
					if ((P1.K_Senkou[8]) && (P2.HFlg == 1)) {		// ��s���͌������Ă݂�
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0){
						
						// �X��
						if (key(3)){
							P1.stateno = 321, P1.More = 1,
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
						// 6B
						if ((key(6)) && (!key(2)) && (!key(8))){
							P1.stateno = 215, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						// ���v�f���m�F
						else if (key(2) && P1.Var[1] == 0){
							P1.stateno = 310, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
							P1.Var[1]++;
						}
					}
					// ��s����
					else if (P1.Senkou[1] > 0){
						// �A�ŃR���{
						if (P1.Var[20] == 1){
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}
			
			// ����
			DelayThrow(2);
			// 3�Q�[�W
			Delay3Button(850, 3000);

			break;
			//********************
			// 215�F6B
			//********************
		case 215:
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(0,0);

			// �ړ�
			if (AnimElem(3))VelAdd(2,0);
			if (AnimElem(6))PosAdd(22, 0);
			if (AnimElem(10))PosAdd(-39, 0);



			// SE
			if (P1.time == 1){
				SEStart(1);
			}
			// �q�b�g���Z�b�g�A�W�t��
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time <= 33)){

				// [�_���[�W]
				Damage(95, 0);

				// [�Q�[�W] 
				Power(106);
				HitTime(8, 22, 24, 16);

				// [�m�b�N�o�b�N]
				HitVel(-4.4, 0, -2.0, -5);
				P1.GuardF = 2;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// ���i��炢
				P1.A.forceKurai = true;	// ��炢�p��������
				P1.HitSE = 12;
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 8) && (P1.A.ncTime > 0)){

					// [�ʏ�Z]
					/*
					if (P1.Senkou[3] > 0){
						if (InputPAD(102) == 0){
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					*/
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}

			// ����
			DelayThrow(2);
			// 3�Q�[�W
			Delay3Button(850, 3000);

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


			// �q�b�g���Z�b�g �����F26
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}
			//else{ P1.MoveHit = 0; }

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(100, 0);
				// [�Q�[�W] 
				Power(120);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 22, 24, 18);
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
				if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
					// [�K�E�Z�E�Q�[�W�Z]
					// ��s����
					if (P1.Senkou[1] > 0){
						// �A�ŃR���{
						if (P1.Var[20] == 1){
							P1.stateno = 610, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					SCancel();
				}

			}

			// 3�Q�[�W
			Delay3Button(850, 3000);

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
			// �q�b�g���Z�b�g�A�����U�t��
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if ((P1.time >= 1)){

				// [�_���[�W]
				Damage(44, 0);

				// [�Q�[�W] 
				P1.GetPow = 24, P1.GivePow = 12;

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(6, 12, 16, 10);

				// [�m�b�N�o�b�N]
				HitVel(-3.4, 0, -1.6, -4.8);
				P1.GuardF = 1;

				P1.HitAnim = 1010;	// ���i��炢
				P1.HitSE = 10;
			}
			else {
				DamReset();
			}

			// �L�����Z��
			if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
				if (P1.Senkou[3] > 0){
					// �X��
					if (key(3)){
						P1.stateno = 321, P1.More = 1,
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
					// 6B
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
					else if(P1.Var[4] == 0) {
						P1.stateno = 200, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
						P1.Var[4]++;
					}
				}
				// [�K�E�Z�E�Q�[�W�Z]
				SCancel();
			}

			// �L�����Z���A�A�ŃL�����Z��
			if ((P1.button[1] == 1) && (P1.time >= 9) && (P1.stateno == 300)){
				// ���v�f�������Ă�
				if (P1.button[102] != 0){
					P1.More = 1, P1.time = 0, P1.A.damage = 0;
				}
			}

			

			// ����
			DelayThrow(1);
			

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
			if (P1.time == 5){
				P1.MoveHit = 1;	// �P��
			}


			// �_���[�W�Z�b�g
			if ((P1.time >= 5) && (P1.time <= 10)){
				
				// [�_���[�W]
				Damage(80, 0);

				// [�Q�[�W] 
				Power(88);

				HitTime(8, 18, 20, 14);

				// [�m�b�N�o�b�N]
				HitVel(-3.8, 0, -1.8, -4.6);
				P1.GuardF = 3;	// ���i
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
				if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0){		// ��s���͌������Ă݂�
						// �X��
						if (key(3)){
							P1.stateno = 321, P1.More = 1,
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
						// 6B
						if ((key(6)) && (!key(2)) && (!key(8))){
							P1.stateno = 215, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						// ���v�f���m�F
						else if (!key(2) && P1.Var[1] == 0){
							P1.stateno = 210, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							P1.Var[1]++;
						}
					}
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}

			// ����
			DelayThrow(2);
			// 3�Q�[�W
			Delay3Button(850, 3000);

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

			// �q�b�g���Z�b�g 10�t��
			if (P1.time == 8){
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time <= 22)){

				// [�_���[�W]
				Damage(100, 0);
				// [�Q�[�W] 
				Power(120);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 22, 24, 18);
				// [�m�b�N�o�b�N]
				HitVel(-4.0, 0, -1.8, -4.0);
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
				if ((P1.CFlg) && (P1.time >= 1) && (P1.A.ncTime > 0)){
					// [�W�����v�L�����Z��]
					if ((P1.K_Senkou[8]) && (P2.HFlg == 1)){		// ��s���͌������Ă݂�
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}

			// 3�Q�[�W
			Delay3Button(850, 3000);

			break;
			//********************
			// 321�F�X���C�f�B���O
			//********************
		case 321:
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SE��炷
			if (P1.time == 1)SEStart(26);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;
			
			// �X�����x�Z�b�g
			//if (P1.time <= 8 && P1.time > 0)VelSet(9,0);
			//if (P1.time == 14)VelSet(5,0);

			if (P1.time == 4)VelSet(10, 0);
			if (P1.time == 10)VelAdd(2.2, 0);

			// �q�b�g���Z�b�g 10�t��
			if (P1.time == 8){
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if ((P1.time >= 8) && (P1.time <= 22)){

				// [�_���[�W]
				Damage(100, 0);
				// [�Q�[�W] 
				Power(120);
				HitTime(10, 40, 40, 18);
				// [�m�b�N�o�b�N]
				HitVel(-1.8, -3.6, -2.2, -3.5);
				GuardVel(-3.6, -1.4);
				P1.GuardF = 3;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// �󒆋�炢(�m��_�E��)
				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			
			// �K�L����
			if (P1.StopTime == 0){
				// �q�b�g���L�����Z��
				if ((P1.CFlg) && (P1.time >= 1)){
					// [�K�E�Z�E�Q�[�W�Z]
					if(P1.scTime > 0)HCancel();
					if(P2.stateno >= 1000)SCancel();	// �q�b�g���̂݉\
				}
			}

			//-----------------
			// �h��
			//-----------------
			// �X��
			if ((P1.button[3] == 1) && (P1.time >= ANIMELEM - 16) && (!P1.CFlg)){
				// ���v�f�������Ă�
				if (key(2)){
					P1.More = 1, P1.time = 0, P1.A.damage = 0;
					P1.stateno = 321;
				}
			}

			// 3�Q�[�W
			Delay3Button(850, 3000);

			break;
			//********************
			// 400�F�W�����vA
			//********************
		case 400:
			P1.ctrl = 0, P1.SFlg = 2;
			if (P1.time == 1)SEStart(1);
			// ��s���͏���
			if (P1.time <= 1)P1.Senkou[1] = 0;
			// �q�b�g���Z�b�g 6�t��
			if (P1.time == 4)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g
			if ((P1.time >= 4) && (P1.time <= 6)){

				// [�_���[�W]
				Damage(42, 0);
				// [�Q�[�W] 
				Power(24);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(5, 12, 16, 10);
				// [�m�b�N�o�b�N]
				HitVel(-4.2, 0, -1.4, -4.0);
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
				if ((P1.CFlg) && (P1.time >= 4) && (P1.scTime > 0)){
					if (P1.Senkou[4] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 430, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[3] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[2] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 410, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[1] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 400, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�󒆕K�E�Z]
					ACancel();
				}

				// [�W�����v�L�����Z��]
				if ((P1.CFlg) && (P1.time >= 4) && (P1.AirJump > 0)){
					if (P1.K_Senkou[8] >= 1){		// ��s���͌������Ă���
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}
			/*
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM){
				P1.ctrl = 1, P1.More = 1, 
				P1.stateno = 46, P1.time = 0;
			}
			*/
			// ����
			DelayThrow(1);

			break;
			//********************
			// 410�F�W�����vB
			//********************
		case 410:
			P1.ctrl = 0, P1.SFlg = 2;
			// SE
			if (P1.time == 1)SEStart(1);
			// �q�b�g���Z�b�g
			if (P1.time == 5)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g
			if ((P1.time >= 5) && (P1.time <= 9)){
			
				// [�_���[�W]
				Damage(75, 0);

				// [�Q�[�W] 
				Power(80);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 16, 18, 14);
				// [�m�b�N�o�b�N]
				HitVel(-4.6, 0, -1.8, -4.0);
				// �K�[�h����
				if (P1.YVel >= 0.0)P1.GuardF = 2;
				else { P1.GuardF = 1; }
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
				if ((P1.CFlg) && (P1.time >= 5) && (P1.scTime > 0)){
					if (P1.Senkou[4] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 430, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[3] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					
					// [�󒆕K�E�Z]
					ACancel();
				}

				// [�W�����v�L�����Z��]
				if ((P1.CFlg) && (P1.time >= 5) && (P1.AirJump > 0)){
					if (P1.K_Senkou[8] >= 1){		// ��s���͌������Ă݂�
						//P1.AirJump -= 1;
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// ����
			DelayThrow(2);

			break;
			//********************
			// 420�F�W�����vC
			//********************
		case 420:
			P1.ctrl = 0, P1.SFlg = 2;
			// SE
			if (P1.time == 1)SEStart(2);
			// �q�b�g���Z�b�g
			if (P1.time == 6)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 5�t��
			if ((P1.time >= 6) && (P1.time <= 11)){

				// [�_���[�W]
				Damage(96, 0);

				// [�Q�[�W] 
				Power(102);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 17, 19, 14);

				// [�m�b�N�o�b�N]
				HitVel(-4.8, 0, -2.0, -3.0);
				// �K�[�h����
				if (P1.YVel >= 0.0)P1.GuardF = 2;
				else { P1.GuardF = 1; }
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				P1.HitSE = 12;

			}
			else {
				DamReset();
			}
			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0){
				if ((P1.CFlg) && (P1.time >= 6) && (P1.scTime > 0)){
					if (P1.Senkou[4] > 0){		// ��s���͌������Ă݂�
						P1.stateno = 430, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}

					// [�󒆕K�E�Z]
					ACancel();
				}

			}
			break;
			//********************
			// 430�F�W�����vD
			//********************
		case 430:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(3);

			if (P1.time == 0)VelSet(0,-0.8);
			if (P1.time == 12)VelSet(7.6,7);

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 5�t��
			if (P1.time >= 1){

				// [�_���[�W]
				if (P1.AirJump == 0)Damage(118, 0);
				else{ Damage(105, 0); }
				// [�Q�[�W] 
				Power(120);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 28, 48, 14);

				// [�m�b�N�o�b�N]
				//HitVel(-2.8, -3.6, -3.6, 6.0);
				HitVel(-2.8, -3.6, -3.6, 5.2);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 2;
				P1.HitAnim = 1000;
				P1.HitSE = 12;
				// �o�E���h
				if (P1.HitCount == 0){
					P1.A.boundLevel = 2;
					P1.A.bound_xvel = -0.9;
					P1.A.bound_yvel = -3.6;
				}
				else{
					P1.A.boundLevel = 1;
					P1.A.bound_xvel = -0.9;
					P1.A.bound_yvel = -3.4;
				}
				P1.A.hosei_K = 0.9;
			}
			else {
				//DamReset();
			}
			if (//(P2.stateno >= 1000) && 
				(P1.CFlg == 1) && (P1.time > 1)){
				P1.stateno = 432, P1.More = 1, P1.time = 0;
				P1.SFlg = 2;
				VelSet(0, 0);
				P1.ignoreG = true;
			}

			if (P1.YPos + P1.YVel * 2 >= GROUND){
				P1.stateno = 431, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;
			//********************
			// 431�F�W�����vD���n
			//********************
		case 431:	
			P1.SFlg = 0, P1.A.damage = 0, P1.ctrl = 0;
			P1.YPos = GROUND;
			P1.ignoreG = false;
			P1.D.counter = 0;
			VelSet(0, 0);
			// SE��炷
			if (P1.time == 1)SEStart(6);
			// �ŏ��Ƀ��b�N
			if (P1.time == 0)P1.Lock = 1;
			// �O�W�����v �� �S�̃t���[��
			if (P1.time >= ANIMELEM)P1.More = 1, P1.stateno = 0,
				P1.time = 0, P1.Lock = 0;
			break;
			//********************
			// 432�F�W�����vD�q�b�g..���
			//********************
		case 432:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;

			if (P1.time == 0)VelSet(-2.6, -3.4);

			VelAdd(0, P1.C.yAccel);

			if ((P1.time >= 1) && (P2.HFlg) && (P1.AirJump > 0)){
				if (P1.K_Senkou[8] > 0){
					P1.SFlg = 2;
					P1.stateno = 45;
					P1.time = 0;
					P1.ignoreG = false;
					P1.AirJump -= 1;
					P1.More = 1;
				}
				// [�W�����v�L�����Z��]
				if ((P1.CFlg) && (P1.time >= 5) && (P1.AirJump > 0)){
					if (P1.K_Senkou[8] >= 1){		// ��s���͌������Ă݂�
						P1.AirJump -= 1;
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// �q�b�g��
			if ((P1.time >= 4 + 1) && (P2.HFlg) && (P2.stateno >= 1000)){
				P1.ctrl = 1;
			}

			if ((P1.YPos + P1.YVel * 2 >= GROUND) && (P1.time >= 4)){
				P1.stateno = 47, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				P1.YVel = 0;
				P1.ignoreG = false;
			}

			// �L�����Z��
			// ���}���u
			/*
			if (P1.StopTime == 0){
				// [�W�����v�L�����Z��]
				if ((P1.time >= 0) && (P1.AirJump > 0) && (P2.HFlg)){
					if (P1.K_Senkou[8] >= 1){		// ��s���͌������Ă݂�
						P1.ignoreG = false;
						P1.AirJump -= 1;
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}
			*/
			/*
			if (P1.time >= 20) {
				P1.ctrl = 1, P1.SFlg = 2;
				P1.stateno = 46;
				P1.time = 0;
				P1.ignoreG = false;
			}
			*/
			break;
			//********************
			// 500�F�n�㓊��
			//********************
		case 500:
			P1.ctrl = 0, P1.SFlg = 0;
			// ���������ݒ�
			if (P1.time == 1){
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

			// 3�Q�[�W
			Delay3Button(850, 3000);

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


			// �����𗣂�
			if (AnimElem(8))P1.XVel = 4.2;
			if (AnimElem(9))P1.XVel = 3.2;
			// �q�b�g���A�G�t�F�N�g
			if (P1.time == 7){
				SEStart(11);
				EffStart(P1.A.hitEff, (int)P2.XPos, (int)P2.YPos, 20 + GetRand(10), -(P2.ySize / 1.5),
					P1.A.heSizeX, P1.A.heSizeY, P2.muki);
				// �����𗣂�
				P2.XVel = -3.5;
			}
			// 2P���G
			if (P1.time < 33)P2.Muteki = 1, P2.mutekiF = 0;


			// �q�b�g���Z�b�g
			if (P1.time == 34)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 34){

				// [�U������Z�b�g]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [�_���[�W]
				Damage(0, 160);
				P1.A.hosei_K = 0.5;
				// [�Q�[�W] 
				Power(270);
				HitTime(6, 45, 45, 0);
				// [�m�b�N�o�b�N]
				HitVel(-2.6, -3.4, 0, 0);
				P1.HitAnim = 1030;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitSE = 12;
				
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
			if (P1.time == 1){
				P1.throwSide = P1.muki;
				P1.A.throwTurn = false;
				if (P1.keyAtt[4]) {
					P1.A.throwTurn = true;
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
			if ((P1.CFlg) && (P1.time >= 5)){
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
				Damage(150, 10);
				// [�Q�[�W] 
				Power(270);

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
			// 600 �X�E�B�t�g1
			//********************
		case 600:	// A�u��
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 0;
				P1.Var[2]++;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 5){
				P1.XVel = 5.4;
			}

			/*
			if (P1.time == 5){
			P1.XVel = 13;
			}
			*/
			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [�_���[�W]
				Damage(80, 0);
				// [�Q�[�W] 
				Power(90);
				HitTime(8, 24, 24, 22);
				// [�m�b�N�o�b�N]
				HitVel(-2.6, 0, -2.2, -2.0);
				P1.A.guard_gx = -5.0;
				P1.HitAnim = 1000;	// �󒆋�炢
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
				if ((P1.CFlg) && (P1.time >= 1)){
					// [�Q�[�W�Z]
					HCancel();
					if ((P1.Var[20] > 0) && (P1.Var[2] < 3) && (P1.scTime > 0))SCancel();
					// �c�h���Z
					if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)){		// ��s���͌������Ă݂�
						if ((P1.cmd[2]) && (P1.Power >= 500))P1.stateno = 700;
						else if ((P1.keyAtt[6]) && (!P1.keyAtt[8]) && (!P1.keyAtt[2]))P1.stateno = 603;
						else if (P1.keyAtt[2])P1.stateno = 604;
						else{ P1.stateno = 600 + P1.Var[2]; }
						P1.More = 1;
						P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 601 �X�E�B�t�g2
			//********************
		case 601:	// A�u��
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = -2.8;
				P1.Var[2]++;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(86, 0);
				// [�Q�[�W] 
				Power(95);
				HitTime(8, 24, 24, 22);
				// [�m�b�N�o�b�N]
				HitVel(5.6, 0, 2.0, -2.0);
				P1.HitAnim = 1000;	// �󒆋�炢
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
				if ((P1.CFlg) && (P1.time >= 1)){
					// [�Q�[�W�Z]
					HCancel();
					if ((P1.Var[20] > 0) && (P1.Var[2] < 3) && (P1.scTime > 0))SCancel();
					// �c�h���Z
					if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)){		// ��s���͌������Ă݂�
						if ((P1.cmd[2]) && (P1.Power >= 500))P1.stateno = 700;
						else if ((P1.keyAtt[6]) && (!P1.keyAtt[8]) && (!P1.keyAtt[2]))P1.stateno = 603;
						else if (P1.keyAtt[2])P1.stateno = 604;
						else{ P1.stateno = 600 + P1.Var[2]; }
						P1.More = 1;
						P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// �X�E�B�t�g3
			//********************
		case 602:
			P1.ctrl = 0, P1.SFlg = 2;
			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 1.8;
				P1.YVel = -4.5;
				P1.ignoreG = true;
				P1.Var[2]++;
			}

			P1.YVel += 0.3;

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}


			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [�_���[�W]
				Damage(85, 5);
				// [�Q�[�W] 
				Power(100);
				HitTime(8, 32, 38, 18);
				// [�m�b�N�o�b�N]
				HitVel(-3.8, -3.8, -3.6, -2.8);
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

			// ���}���u
			if (P1.StopTime == 0){
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 1)){
					/*
					if ((P1.Senkou[4] > 0)){
						//&& (P1.Var[20] > 1)) {		// ��s���͌������Ă݂�
						P1.stateno = 430, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					*/
					// [�Q�[�W�Z]
					ACancel();
				}
			}

			// ���n
			if ((P1.time > 0) && (P1.YVel > 0) && (P1.YPos + P1.YVel * 2 >= GROUND)){
				P1.time = 0, P1.stateno = 625, P1.More = 1,
					P1.ctrl = 0, P1.ignoreG = false;
			}

			break;
			//********************
			// 603 6D(���i)
			//********************
		case 603:	// A�u��
			P1.ctrl = 0;
			if(P1.time == 0)P1.SFlg = 2;
			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = 1.2;
				P1.YVel = -4.4;
				P1.ignoreG = true;
				P1.Var[2]++;
				P1.Var[3] = 0;
			}

			if(P1.YPos != GROUND)P1.YVel += GRAVITY;

			// �z�[���h
			/*
			if ((P1.time == 10) && (P1.button[4] > 10)){
				P1.Var[3] = 1;
				VelAdd(-0.2, GRAVITY);
			}
			*/

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(95, 0);
				// [�Q�[�W] 
				Power(110);
				HitTime(8, 28, 28, 22);
				// [�m�b�N�o�b�N]
				HitVel(-3.0, 0, -2.4, -2.0);
				P1.A.guard_gx = -3.8;
				P1.HitAnim = 1000;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 2;	// �n��󂯐g�\
				// [�K�[�h����]
				P1.GuardF = 2;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// ���}���u
			if (P1.StopTime == 0){
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 1)){
					// [�Q�[�W�Z]
					HCancel();
					if ((P1.Var[20] > 0) && (P1.Var[2] < 3) && (P1.scTime > 0))SCancel();
					// �c�h���Z
					if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)){		// ��s���͌������Ă݂�
						if ((P1.cmd[2]) && (P1.Power >= 500))P1.stateno = 700;
					    else if (P1.keyAtt[2])P1.stateno = 604;
						else{ P1.stateno = 600 + P1.Var[2]; }
						P1.More = 1;
						P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// ���n
			if ((P1.time > 0) && (P1.YVel > 0) && (P1.YPos + P1.YVel * 2 >= GROUND)){
				P1.YPos = GROUND;
				P1.YVel = 0;
				P1.XVel = 0;
				P1.ignoreG = false;
				P1.SFlg = 0;
				// �t�F�C���g��
				if (P1.Var[3] != 0){
					P1.time = 0, P1.stateno = 605, P1.ctrl = 0;
					P1.SFlg = 1;
					P1.More = 1;
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 604 2D(���i)
			//********************
		case 604:	// A�u��
			P1.ctrl = 0, P1.SFlg = 1;
			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);
				P1.XVel = -0.4;
				P1.Var[2]++;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 12){
				PosAdd(-4, 0);
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(98, 0);
				// [�Q�[�W] 
				Power(108);
				HitTime(8, 32, 38, 23);
				// [�m�b�N�o�b�N]
				HitVel(-2.6, -3.6, -2.4, -3.0);
				P1.A.guard_gx = -4.4;
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 2;
				// [�K�[�h����]
				P1.GuardF = 3;
				P1.HitSE = 13;

			}
			else {
				DamReset();
			}

			// ���}���u
			if (P1.StopTime == 0){
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 1)){
					// [�Q�[�W�Z]
					HCancel();
					if ((P1.Var[20] > 0) && (P1.Var[2] < 3) && (P1.scTime > 0))SCancel();
					// �c�h���Z
					if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)){		// ��s���͌������Ă݂�
						if ((P1.cmd[2]) && (P1.Power >= 500))P1.stateno = 700;
						else if ((P1.keyAtt[6]) && (!P1.keyAtt[8]) && (!P1.keyAtt[2]))P1.stateno = 603;
						else{ P1.stateno = 600 + P1.Var[2]; }
						P1.More = 1;
						P1.time = 0, P1.A.damage = 0;
					}
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 11, P1.ctrl = 1;

			break;
			//********************
			// 605 �t�F�C���g
			//********************
		case 605:
			P1.ctrl = 0, P1.SFlg = 1;
			P1.ignoreG = false;
			P1.YPos = GROUND;

			VelSet(0, 0);
			// SE��炷
			if (P1.time == 1){
				SEStart(6);
			}

			// �I��
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 11, P1.ctrl = 1;
			}
			break;
			//********************
			// 610 �X�s�i�[
			//********************
		case 610:	
			if (((P1.Senkou[3] > P1.Senkou[1]) || (P1.Senkou[2] > P1.Senkou[1])) &&
				(P1.time == 0)){
				P1.stateno++;
				P1.More = 1, P1.time = 0;
				P1.ctrl = 0;
			}
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
			if (P1.time >= 1){

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
					// ��s����
					if (P1.Senkou[1] > 0){
						// �A�ŃR���{
						if (P1.Var[20] == 1){
							if (P1.Power >= 1000){
								P1.stateno = 800, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							}
						}
					}
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
				P1.XVel = 0;
			}
			if (P1.time == 1)SEStart(21);

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 6){
				P1.XVel = 17;
			}

			/*
			if (P1.time == 5){
			P1.XVel = 13;
			}
			*/

			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(116, 0);
				// [�Q�[�W] 
				Power(142);
				HitTime(2, 53, 60, 24);
				// [�m�b�N�o�b�N]
				HitVel(-2.9, -3.6, -2.8, -2.6);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 0.3;
				P1.A.wall_yvel = -6.6;

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
			// �X�e�[�g�ύX
			if (((P1.Senkou[3] > P1.Senkou[1]) || (P1.Senkou[2] > P1.Senkou[1])) &&
				(P1.time == 0)){
				P1.stateno++;
				P1.More = 1, P1.time = 0;
				P1.ctrl = 0;
			}
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;

			// SE��炷
			if (P1.time == 0){
				SEStart(21);
				VelSet(0,0);
			}
			if (P1.time == 1){
				VelSet(0, 0);
			}

			if (AnimElem(3)){
				VelSet(1.6, -4.7);
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
				Damage(105, 10);
				// [�Q�[�W] 
				Power(145);

				HitTime(8, 100, 100, 12);
				// [�m�b�N�o�b�N]
				HitVel(-1.8, -5.4, -1.8, -4.8);
				
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;

				P1.HitSE = 13;
			}

			// �L�����Z��
			if (P1.StopTime == 0){
				// �q�b�g���L�����Z��
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > SC_TIME - 4)){
					// [�Q�[�W�Z]
					HCancel();
					// ���n����
					if (P1.stateno != 620){
						P1.SFlg = 0;
						P1.YPos = GROUND;
						VelSet(0, 0);
					}
				}
				// �q�b�g���L�����Z��
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)){
					// [�Q�[�W�Z]
					ACancel();
				}
			}

			// ���n
			if (P1.YPos > GROUND){
				P1.time = 0, P1.stateno = 625, P1.ctrl = 0;
				P1.More = 1;
			}
			break;

			//********************
			// 621 B�A�b�p�[
			//********************
		case 621:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			P1.D.counter = true;
			P1.D.fatal = true;

			// SE��炷
			if (P1.time == 0){
				SEStart(21);
				VelSet(0, 0);
			}
			if (P1.time == 1){
				VelSet(0, 0);
			}

			if (AnimElem(3)){
				VelSet(1.6, -6.1);
			}

			// �A�j������
			if (P1.time == ANIMELEM - 1){
				VelSet(0.1, -0.5);
			}
			if (P1.time > ANIMELEM - 1){
				VelAdd(0, 0.4);
			}

			// ���G
			if (P1.time <= 6){
				P1.mutekiF = 0;
				P1.Muteki = 1;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [�_���[�W]
				Damage(120, 0);
				// [�Q�[�W] 
				Power(150);

				HitTime(8, 100, 100, 12);
				// [�m�b�N�o�b�N]
				HitVel(-1.7, -5.4, -1.7, -4.8);

				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 2;	// �n��󂯐g�\
				// [�K�[�h����]
				P1.GuardF = 1;

				P1.HitSE = 13;
			}

			// �L�����Z��
			if (P1.StopTime == 0){
				// �q�b�g���L�����Z��
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > SC_TIME - 4)){
					// [�Q�[�W�Z]
					//HCancel();
					// ���n����
					if (P1.stateno != 621){
						P1.SFlg = 0;
						P1.YPos = GROUND;
						VelSet(0, 0);
					}
				}
			}

			// ���n
			if (P1.YPos > GROUND){
				P1.time = 0, P1.stateno = 625, P1.ctrl = 0;
				P1.More = 1;
			}
			break;

			//********************
			// 625 ���n
			//********************
		case 625:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.ignoreG = false;
			P1.YPos = GROUND;
			P1.D.counter = true;
			P1.D.fatal = true;

			VelSet(0, 0);
			// SE��炷
			if (P1.time == 1){
				SEStart(6);
			}

			// �I��
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}
			break;
			//********************
			// 630 �X�E�B�t�g1
			//********************
		case 630:	// A�u��
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
				HitTime(8, 26, 28, 22);
				// [�m�b�N�o�b�N]
				HitVel(-2.8, -2.6, -2.4, -2.0);
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
					HCancel();

				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 650 �G���n���X
			//********************
		case 650:	// �G���n
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

			if (AnimElem(2)){
				//P1.MoveHit = 1;	// �P��
				SEStart(22);
			}

			if (P1.time == 33){
				if(P1.Var[20] < 3)P1.Var[20] += 1;
				SEStart(31);
				P1.colorCTime = 10;
				P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// �ł��グ
			//********************
		case 700:	// A�u��
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			// SE��炷�A�ړ�
			if (P1.time == 0){
				SEStart(21);	
				P1.Var[2]++;
			}

			if (AnimElem(2))PosAdd(8, 0);

			// �Q�[�W����
			if (P1.time == 1){
				if (P1.Var[20] < 3)P1.Power -= 500;
				SEStart(31);
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
				Damage(80, 20);
				// [�Q�[�W] 
				Power(0, 90);
				//HitTime(8, 18, 20, 14);
				HitTime(8, 22, 24, 18);
				// [�m�b�N�o�b�N]
				HitVel(-0.6, -6.8, -0.6, -6.0);
				GuardVel(-4.8, -1.8);
				//P1.A.guard_gx = -4.6;
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
				if ((P1.CFlg) && (P1.time >= 1)){
					// [�W�����v�L�����Z��]
					if ((P1.K_Senkou[8]) && (P2.HFlg)){		// ��s���͌������Ă݂�
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// �c�h���Z
					if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)){		// ��s���͌������Ă݂�
						if ((P1.keyAtt[6]) && (!P1.keyAtt[8]) && (!P1.keyAtt[2]))P1.stateno = 603;
						else if (P1.keyAtt[2])P1.stateno = 604;
						else{ P1.stateno = 600 + P1.Var[2]; }
						P1.More = 1;
						P1.time = 0, P1.A.damage = 0;
					}
					// [�Q�[�W�Z]
					HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 800 �n�[�h�p���`
			//********************
		case 800:
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
				afterimageSetting(P1.PSide, 30, 128, 128, 250);
			}

			// SE��炷�A�ړ�
			if (P1.time == 1){
				SEStart(21);
				P1.XVel = 0;
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 205, P1.colorC[2] = 0;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}


			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time <= 20)){

				// [�_���[�W]
				Damage(140, 80);
				// [�Q�[�W] 
				Power(0, 310);
				HitTime(10, 60, 60, 24);
				// [�m�b�N�o�b�N]
				HitVel(-4.2, -3.8, -3.8, -3.4);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 14;
				// �G�t�F�N�g
				HitEff(1, 1, 1);
				// �n�k�G�t�F�N�g
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}
			else {
				DamReset();
			}


			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM){
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}
			break;
			//********************
			// �n�C�h���X�g�[��
			//********************
		case 810:
			P1.ctrl = 0, P1.SFlg = 0;

			// �Q�[�W����
			if (P1.time == 0){
				P1.Power -= 2000;
				S.StopTime = 15;
				S.Anten = 15;
				SEStart(25);
				// ���i�G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}
			// �w���p�[�ύX.[2]�A���ˑO�Ȃ�ύX
			if (P1.time == 45){
				if (!H1[1].var){
					H1[1].var = true;
					H1[1].time = 0;
					H1[1].stateno = 2100;
				}
			}


			// �I��
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 820 EX�A�b�p�[
			//********************
		case 820:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			P1.A.gaugeHosei = true;
			P1.D.counter = true;
			P1.D.fatal = true;

			// �Q�[�W����
			if (P1.time == 0){
				P1.Power -= 1000;
				afterimageSetting(P1.PSide, 50, 128, 128, 250);
				S.StopTime = 15;
				S.Anten = 15;
				SEStart(25);
				// ���i�G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}

			// SE��炷
			if (P1.time == 0){
				SEStart(21);
				VelSet(0, 0);
			}
			if (P1.time == 1){
				VelSet(0, 0);
			}

			if (AnimElem(3)){
				VelSet(3.4, -7.2);
			}

			// �A�j������
			if (P1.time == ANIMELEM - 1){
				VelSet(0.1, -0.5);
			}
			if (P1.time > ANIMELEM - 1){
				VelAdd(0, 0.4);
			}

			// ���G
			if (P1.time <= 6){
				P1.mutekiF = 0;
				P1.Muteki = 2;
			}

			if (AnimElem(2) || AnimElem(3) || AnimElem(4)){
				P1.MoveHit = 1;
				// [�m�b�N�o�b�N]
				
				if (AnimElem(4)){
					// [�m�b�N�o�b�N]
					HitVel(-1.2, -4.2, -1.2, -4.0);
					P1.A.yaccel = 0.2;
				}
				else{ HitVel(-1.2, -6.8, -1.2, -6.8); }
			}
			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(75, 0);
				// [�Q�[�W] 
				Power(0,70);

				HitTime(8, 100, 100, 12);
				// [�m�b�N�o�b�N]
				//HitVel(-1.4, -5.6, -1.4, -5.4);

				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 2;	// �n��󂯐g�\
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;
			}

			// �@������
			if ((P1.CFlg) && (P1.ignoreG) && (P2.stateno >= 1000) && (P1.time >= ANIMELEM)){
				P1.time = 0, P1.stateno = 821, P1.ctrl = 0;
				P1.More = 1;
				P1.A.damage = 0;
			}

			// ���n
			if (P1.YPos > GROUND){
				P1.time = 0, P1.stateno = 625, P1.ctrl = 0;
				P1.More = 1;
			}
			break;
			//********************
			// 821: EX�����@�����Ƃ�
			//********************
		case 821:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(3);

			if (P1.time == 0)VelSet(0, -0.2);
			else{ P1.YVel += GRAVITY; }

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 5�t��
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(20, 64);
				// [�Q�[�W] 
				Power(0, 80);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 28, 48, 18);

				// [�m�b�N�o�b�N]
				HitVel(-4.0, -3.6, -4.4, 5.2);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1030;
				P1.HitSE = 14;
				// �G�t�F�N�g
				HitEff(1, 1, 1);
				// �o�E���h
				P1.A.boundLevel = 1;
				// �n�k�G�t�F�N�g
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}
			else {
				//DamReset();
			}

			if (P1.YPos + P1.YVel * 2 >= GROUND){
				P1.stateno = 431, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;
			//********************
			// 850 ���@���p�C�A�L���[
			//********************
		case 850:
			P1.ctrl = 0;
			P1.A.gaugeHosei = true;
			if (P1.time < 28){
				P1.ignoreG = false;
				P1.SFlg = 0;
			}
			else{
				P1.ignoreG = true;
				P1.SFlg = 2;
			}

			// �Q�[�W����
			if (P1.time == 0){
				P1.Power -= 3000;
				S.StopTime = 10;
				S.Anten = 25;
				SEStart(25);
				// �G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);

				DamReset();
			}

			// ���G
			/*
			if (P1.time <= 5){
				P1.mutekiF = 0;
				P1.Muteki = 1;
			}
			*/

			// SE��炷
			if (P1.time == 1){
				SEStart(21);
			}
			// SE��炷
			if (P1.time == 28+1){
				SEStart(21);
			}

			if (P1.time == 5){
				P1.XVel = 13;
			}

			if (P1.time == 1 + 28){
				VelSet(0, 0);
			}
			if (P1.time == 7 + 28){
				VelSet(1.6, -6);
			}
			// �A�j������
			if (P1.time == ANIMELEM - 1){
				VelSet(0.1, -0.5);
			}
			if (P1.time > ANIMELEM - 1){
				VelAdd(0, 0.4);
			}

			if (P1.time == 1 || P1.time == 28+7 || P1.time == 28+9 || P1.time == 28+11){
				P1.MoveHit = 1;
			}
			if (P1.time == 28){
				P1.MoveHit = 0;
			}

			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time < 28)){

				// [�_���[�W]
				Damage(100, 0);
				// [�Q�[�W] 
				Power(0, 100);
				HitTime(10, 24, 24, 24);
				// [�m�b�N�o�b�N]
				HitVel(-1.6, 0, -2.4, -2.0);
				P1.HitAnim = 1060;	// �n��
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;

				P1.A.kill = 0;
			}
			// �_���[�W�Z�b�g
			else if ((P1.time >= 28) && (P1.time <= ANIMELEM)){

				// [�_���[�W]
				Damage(40, 25);
				// [�Q�[�W] 
				Power(0, 100);

				HitTime(8, 100, 100, 12);
				// [�m�b�N�o�b�N]
				HitVel(-1.6, -6.6, -1.6, -6.2);

				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;

				P1.HitSE = 13;
				P1.A.kill = 0;
			}

			// ����������ύX
			if ((P1.CFlg) && (P1.time >= 1) && (!P1.ignoreG) && (P2.stateno >= 1000) && (P2.YPos != GROUND)){
				P2.YPos = GROUND;
				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 50;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 50;
				if (P1.muki == 0){
					P2.XPos = P1.XPos + 50;
				}
				else if (P1.muki == 1){
					P2.XPos = P1.XPos - 50;
				}
			}
			

			// �����L�b�N��
			if ((P1.CFlg) && (P1.ignoreG) && (P2.stateno >= 1000) && (P1.time == 44)){
				P1.time = 0, P1.stateno = 851, P1.ctrl = 0;
				P1.More = 1;
				P1.A.damage = 0;
			}

			// �I��
			if (P1.time >= ANIMELEM){
				if (P1.YPos + P1.YVel * 2 >= GROUND){
					P1.time = 0, P1.stateno = 621, P1.ctrl = 0;
				}
			}

			break;
			//********************
			// 851 �����L�b�N
			//********************
		case 851:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.touchF = false;
			P1.ignoreG = true;
			P1.A.gaugeHosei = true;
			// �Q�[�W����
			if (P1.time == 1+6-1){
				S.StopTime = 15;
				S.Anten = 30;
				S.noGauge = 15;
				SEStart(25);
				// �G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize - 50, 0, 0, 0.25, 0.25, P1.PSide);
			}

			if (P1.time == 0)VelSet(-8,-11);
			if (P1.time == 1+6)VelSet(16.0, 12);


			// �q�b�g���Z�b�g 10�t��
			if (P1.time == 1){
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if (P1.time >= 1){

				// [�_���[�W]
				Damage(55, 155);
				// [�Q�[�W] 
				Power(0, 200);
				HitTime(16, 300, 300, 15);
				// [�m�b�N�o�b�N]
				//HitVel(-0.5, -13.8, -0.5, -13.2);
				HitVel(0, -6.0, 0, -5.2);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// �󒆋�炢(�m��_�E��)
				P1.HitSE = 14;
				//P1.A.yaccel = 0.28;
				// �G�t�F�N�g
				HitEff(1, 1, 1);
				//P1.A.hosei_K = 0.05;
				P1.A.addMuteki = true;
				P1.A.amTime = 120;

				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;

				P1.A.kill = true;
			}
			else{
				DamReset();
			}

			// �I��
			if (P1.time >= ANIMELEM){
				if (P1.YPos + P1.YVel * 2 >= GROUND){
					P1.time = 0, P1.stateno = 625, P1.ctrl = 0;
					P1.touchF = true;
				}
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
						H1[i].A.damage = 75;
					}
					// �_���[�W�ʒu�Z�b�g
					if (H_triggerT(i, 0, 1)){

						//[ �U������Z�b�g ]
						H_AttSet(i, 0, 50, 30, -20, -50);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 40, H1[i].HGivePow = 20;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						// [�q�b�g�d��]
						HI.HHitStop = 4;
						H1[i].HG_HitTime = 20, H1[i].HA_HitTime = 24,
							// [�K�[�h�d��]
							H1[i].HG_GuardTime = 16;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H1[i].H_GX = -4.2, H1[i].H_GY = 0;
						H1[i].H_AX = -3, H1[i].H_AY = -5;
					}


					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 24)H1[i].HAnimTime = 0;
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
						H1[i].HMoveHit = 1;
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
					if (H1[i].HAnimTime >= 24)H1[i].HAnimTime = 0;
					// �I���ԍۂɔ�������
					if (H1[i].time >= 240)H1[i].Alpha = true;
					// �I�������A���Ԍo��
					if (H1[i].time >= 300){
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
					// ���W�E���x�ݒ�
					// ���x�� 625 �Ō��߂�
					// ���x���Ȃ��Ƃ��͂����Ō��߂�
					if (H1[i].time == 0 && H1[i].HXVel == 0){
						H1[i].HXVel = 5.0;
					}
					// �����]��
					if (H1[i].time == 50 && H1[i].HXVel == 4.5){
						H1[i].HXVel = -5.0;
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
						H_AttSet(i, 0, 50, 30, -20, -38);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 40, H1[i].HGivePow = 20;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;

						// [�q�b�g�d��]
						HI.HHitStop = 6;
						H1[i].HG_HitTime = 26, H1[i].HA_HitTime = 34,
							// [�K�[�h�d��]
							H1[i].HG_GuardTime = 24;


						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H1[i].H_GX = -3.8, H1[i].H_GY = 0;
						H1[i].H_AX = -2, H1[i].H_AY = -6;
					}

					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 24)H1[i].HAnimTime = 0;
					// �I�������A�[���Bor�U���q�b�g
					if ((H1[i].XPos < (S.ScroolX)) ||
						(H1[i].XPos >(S.ScroolX + SCREEN_W)) ||
						(H1[i].HMoveHit == 0)
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

						H_PosSet(i, 20, -65);
					}
					// ���x�ݒ�
					if (H1[i].time == 1){
						H1[i].HXVel = 8.5;
					}
					if (H1[i].time == 25){
						H1[i].HXVel = -8.5;
					}


					// �q�b�g���E�_���[�W�Z�b�g
					if ((H1[i].time == 1) || (H1[i].time == 25)){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 70;
					}

					// �_���[�W�ʒu�Z�b�g�A2�t���ȍ~
					if (H1[i].time >= 1){

						//[ �U������Z�b�g ]
						H_AttSet(i, 0, 80, 60, -40, -108);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 12, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 40;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;

						// [�q�b�g�d��]
						HI.HHitStop = 6;
						H1[i].HG_HitTime = 40, H1[i].HA_HitTime = 40,
							// [�K�[�h�d��]
							H1[i].HG_GuardTime = 15;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H_HitVel(i, -3.7, 0,
							-4.2, -6);

						if (H1[i].time >= 25){
							H_HitVel(i, -4.6, -8,
								4, -8.8);
						}

						// [�␳]
						if (P1.CFlg){
							P1.A.hosei_K = 0.7;
						}

					}

					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 32)H1[i].HAnimTime = 0;
					// �I�������A���Ԍo��or��炢
					if ((H1[i].time > 42) || (P1.stateno >= 1000))
					{
						HelperReset(i);
					}
					break;

					//********************
					// 2100 �n�C�h���J
					//********************
				case 2100:

					//***************
					//  ��{�ݒ�
					//***************
					H1[i].XPos = 0;
					H1[i].YPos = 0;
					H1[i].GSize = 1;
					H1[i].Alpha = 1;	//������
					P1.A.gaugeHosei = true;
					if (H1[i].time == 0){
						H1[i].muki = 0;
					}
					
					if(H1[i].time >= 14)S.Anten = 1;

					//SE
					if (H1[i].time == 6 && P1.Var[11] == 0)SEStart(23);

					

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time % 15 == 0 && (H1[i].time != 0)){
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 55;
						H1[i].A.hosyo = 5;
					}
					else{
						H1[i].HMoveHit = 0;
						H1[i].A.damage = 0;
					}

					// �_���[�W�ʒu�Z�b�g�A2�t���ȍ~
					if (H1[i].time >= 0){

						//[ �U������Z�b�g ]
						H_AttSet(i, 0, 80, 220, -40, -220);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 13, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 40;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;

						// [�q�b�g�d��]
						H_HitTime(i, 0, 28, 120, 24);

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1030;
						// [�m�b�N�o�b�N]
						H1[i].H_GX = 0, H1[i].H_GY = 0;
						H1[i].H_AX = 0, H1[i].H_AY = -2.8;
						// �G�t�F�N�g
						h_HitEff(i, 3, 0.6, 0.6);
					}

					H_Move(i);


					//if (H1[i].time == 24);//H1[i].HAnimTime = 1;
					
					// �G�t�F�N�g�Ƃ��ĕ`��
					if (H1[i].HAnimTime > 0){
						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							-100 + H1[i].HAnimTime * 4, -500 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);
						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							486 + H1[i].HAnimTime * 4, -500 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);

						// �G�t�F�N�g�Ƃ��ĕ`��
						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							-100 + H1[i].HAnimTime * 4, -200 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);
						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							-100 + H1[i].HAnimTime * 4, 100 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);

						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							486 + H1[i].HAnimTime * 4, -200 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);
						EasyEffDraw(7, H1[i].HAnimTime, 80 + S.ScroolX, 120 + S.ScroolY,
							486 + H1[i].HAnimTime * 4, 100 + (H1[i].HAnimTime - 15) * 10,
							1, 1, H1[i].muki);
					}

					//***************
					//  �s�����e
					//***************

					// �I�������A2�b�o��
					if (H1[i].time >= 120)
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
	if (P1.PSide == 1)Get_PSet(P1, P2), Get_HSet(H1, H2);//, Get_SSet(S);
	if (P1.PSide == 2)Get_PSet(P2, P1), Get_HSet(H2, H1);//, Get_SSet(S);

}//StateParam�I��




//-------------------------------------------------------------------------------
//
// �󂯎��p
//
//-------------------------------------------------------------------------------

// ���Ԃ��t�ɂ���
void GetP_CorParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_CorParam(System_t GS)
{
	S = GS;
}

void GetH_CorParam(Helper GH1[], Helper GH2[])
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
	// �c�h���Z
	if ((P1.Senkou[4] > 0) && (P1.Var[2] < 3)
		&& (P1.SFlg != 2)){		// ��s���͌������Ă݂�
		if ((P1.keyAtt[6]) && (!P1.keyAtt[8]) && (!P1.keyAtt[2]))P1.stateno = 603;
		else if (P1.keyAtt[2])P1.stateno = 604;
		else{ P1.stateno = 600 + P1.Var[2]; }
			P1.More = 1;
			P1.time = 0, P1.A.damage = 0;
	}
	// 236 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[1])){		// ��s���͌������Ă݂�
		P1.stateno = 630, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 214 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[2])){		// ��s���͌������Ă݂�
		P1.stateno = 610, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 623 + B
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[4])){		// ��s���͌������Ă݂�
		P1.stateno = 620, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 214 + D
	if ((P1.Senkou[4] > 0) && ((P1.Power >= 500) || (P1.Var[20] > 2))
		&& (P1.cmd[2])){		// ��s���͌������Ă݂�
		P1.stateno = 700, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	HCancel();

	//�Ō�ɔ��������
	DamReset();
	
}

// �K�E�Z�L�����Z��
void HCancel()
{
	
	// 214 + C
	if ((P1.Senkou[3] > 0)
		&& (P1.cmd[2]) && (P1.Power >= 2000) && (!H1[1].var)){		// ��s���͌������Ă݂�
		P1.stateno = 810, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 236 + C
	if ((P1.Senkou[3] > 0)
		&& (P1.cmd[1]) && (P1.Power >= 1000)){		// ��s���͌������Ă݂�
		P1.stateno = 800, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	// 623 + C
	if ((P1.Senkou[3] > 0)
		&& (P1.cmd[4]) && (P1.Power >= 1000)){		// ��s���͌������Ă݂�
		P1.stateno = 820, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	//  [ �R�Q�[�W ]
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.Senkou[3] > 0)
		&& (P1.Power >= 3000)){		// �n��
			P1.stateno = 850;	// ���K
			P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}

	//�Ō�ɔ��������
	DamReset();

}

// �󒆕K�L�����Z��
void ACancel()
{
	/*
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
	if ((InputCOM(1)) && (!H1[1].var)){		// ��ʂɃw���p�[�P���Ȃ�
		if ((P1.Senkou[1] > 0) ||
			(P1.Senkou[2] > 0) ||
			(P1.Senkou[3] > 0)){
			P1.stateno = 605, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}

	*/

	// [���K]
	//  214+D		[�����L�b�N]
	/*
	if ((P1.Senkou[4] > 0) && (P1.cmd[2])
		&& (P1.Power >= 1000)){
		if (P1.SFlg == 2){		// �n��
			P1.stateno = 820, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	*/
	// 623 + C
	if ((P1.Senkou[3] > 0)
		&& (P1.cmd[4]) && (P1.Power >= 1000)){		// ��s���͌������Ă݂�
		P1.stateno = 820, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}

	//�Ō�ɔ��������
	DamReset();

	
}

