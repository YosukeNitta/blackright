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
void HydParam(void)
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
			((P1.stateno >= 50) && (P1.stateno <= 69)) ||
			(P1.stateno == 520) ||	// �K�[�L����
			((P1.stateno >= 550) && (P1.stateno <= 599))	// EX�A�N�V����
			){	// �K�[�h�E�󒆃_�b�V���E�n�C�W�����v�@
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
		// ������D�`�F�[��
		if ((P1.ctrl) && (P1.Var[11] > 0))P1.Var[12] = 0;

		// �������Ԍ���
		/*
		if (P1.Var[11] > 0) {
			P1.Var[11]--;
			// �I�����G�t�F�N�g
			if (P1.Var[11] == 0)
				EffStartB(16, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0,
					0.23, 0.23, P1.muki);
		}
		*/

		// ����`���[�W
		if (P1.Var[11] == 0) {
			P1.Var[10] = 180 * 6;
			P1.Var[11] = -1;
		}
		// �`���[�W����
		//if ((P1.Var[11] <= 0) && (P1.Var[10] > 0))
			//P1.Var[10]--;

		if (P1.Var[10] < 180 * 6)P1.Var[10]++;

		// ���e�ݒ� 3���܂�
		P1.Var[2] = 0;
		for (int i = 0; i < 3; i++) {
			if (H1[i].var)
				P1.Var[2]++;
			else { break; }
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
			if (TimeNo(3, 5)){

				// [�_���[�W]
				Damage(42, 0);
				// [�Q�[�W] 
				Power(24);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(6, 12, 16, 10);
				// [�m�b�N�o�b�N]
				HitVel(-3.2, 0, -1.6, -4.8);

				// [�K�[�h����]
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				// [�q�b�g���̃A�j��]
				P1.HitAnim = 1000;
				// [�q�b�g�T�E���h]
				SESet(10, 16);

				// [�t�H�[�����]
				P1.fallF = 1;
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
			
			// SE��炷
			if (P1.time == 1)SEStart(3);

			// VO
			if (P1.time == 1){
				PVOStart(P1.PSide, 3, 0);
			}

			// �ʒu�����炷
			if (P1.time == 0){
				PosAdd(5, 0);
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
				Damage(98, 0);
				// [�Q�[�W] 
				Power(120);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 22, 24, 18);
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
					// [�W�����v�L�����Z��]
					if ((P1.K_Senkou[8]) && (P2.HFlg == 1)) {		// ��s���͌������Ă݂�
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
				Damage(40, 0);

				// [�Q�[�W] 
				Power(20);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(6, 12, 16, 10);

				// [�m�b�N�o�b�N]
				HitVel(-3.1, 0, -1.6, -4.8);
				P1.GuardF = 1;
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

			// ����
			DelayThrow(1);
			// 3�Q�[�W
			Delay3Button(850, 3000);

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
				HitVel(-3.6, 0, -1.5, -4.6);
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

			// ����
			DelayThrow(2);
			// 3�Q�[�W
			Delay3Button(850, 3000);

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
				HitTime(10, 40, 40, 18);
				// [�m�b�N�o�b�N]
				HitVel(-1.8, -4, -2.2, -3.5);
				GuardVel(-4.2, -1.5);

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

			// 3�Q�[�W
			Delay3Button(850, 3000);

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
				else { P1.GuardF = 1; }
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
				else if (P1.Senkou[1] > 0) {		// ��s���͌������Ă݂�
					P1.stateno = 400, P1.More = 1,
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
				HitVel(-4.6, 0, -1.6, -4.6);
				// �K�[�h����
				if (P1.YVel >= 0.0)P1.GuardF = 2;
				else { P1.GuardF = 1; }
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
						P1.AirJump -= 1;
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

				// �K�[�h����
				if (P1.YVel >= 0.0)P1.GuardF = 2;
				else { P1.GuardF = 1; }
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
			if (P1.time == 1){
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

				int dist = 70;
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
			//if (P1.time == 3)P2.XVel = -2.5;
			//if (P1.time == 4)P2.XVel = -2.2;
			// �q�b�g��
			//if(P1.time == 7)SEStart(11);

			// 2P���G
			if (P1.time < 6) {
				P2.Muteki = 1, P2.mutekiF = 0;
			}
			if (P1.time > 8 && P1.time < 20)P2.Muteki = 1, P2.mutekiF = 0;
			if (P1.time > 22 && P1.time < 44)P2.Muteki = 1, P2.mutekiF = 0;

			// �q�b�g���Z�b�g
			if (P1.time == 7 || P1.time == 21 || P1.time == 45)P1.MoveHit = 1;	// �P��

			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 7 || P1.time == 21) {

				// [�U������Z�b�g]
				//ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [�_���[�W]
				Damage(0, 40);
				// [�Q�[�W] 
				Power(50);
				HitTime(0, 45, 45, 0);
				// [�m�b�N�o�b�N]
				HitVel(0, 0, 0, 0);
				P1.HitAnim = 1000;
				// [��炢���̕���]
				P1.fallF = 0;
				P1.HitSE = 11;
			}
			// �_���[�W�Z�b�g�A���� 1�t��
			else if (P1.time == 45){

				// [�U������Z�b�g]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [�_���[�W]
				Damage(0, 100);
				P1.A.hosei_K = 0.5;
				// [�Q�[�W] 
				Power(200);
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
				if ((P1.CFlg) && (P1.time >= 46) && (P1.scTime > 0)){
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
			if ((P1.time == 0) && (P1.YPos + 20 >= GROUND)){
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
			// 600 �T�c
			//********************
		case 600:
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷
			if (P1.time == 1){
				SEStart(2);
			}

			if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 15, 0);

			// �w���p�[�ݒ菀��.[1]	��ѓ���
			if (P1.time == 0){
				P1.Var[3] = P1.Var[2];	// �������e���L��
				HelperReset(P1.Var[3]);
				H1[P1.Var[3]].var = false;
			}
			// �Ăяo��.[1]	��ѓ���
			else if (P1.time == 13){
				H1[P1.Var[3]].var = true;
				H1[P1.Var[3]].time = 0;
				H1[P1.Var[3]].stateno = 2000;
				H_PosSet(P1.Var[3], 20, -168);
				P1.Var[10] -= 180;
			}

			if (P1.time == 13) {
				VelSet(-5.0, 0);
				SEStart(40);
			}

			// �L�����Z��
			if (P1.StopTime == 0){
				//&& (H1[P1.Var[3]].var == false) &&(P1.scTime > 0)
				if (P1.time > 13){
					// D�L�����Z��
					if ((P1.Var[11] > 0) && (P1.Senkou[4] > 0) && (P1.Var[12] == 0)) {
						// �n��ɂ��āA�w���p�[[1]�����݂��Ȃ�
						if (P1.SFlg != 2) {
							P1.Var[12]++;
							if ((P1.keyAtt[6]) && (P1.keyAtt[2]))
								P1.stateno = 603, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							else if (P1.keyAtt[6])
								P1.stateno = 601, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							else if (P1.keyAtt[2])
								P1.stateno = 602, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
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
			// 601 6D
			//********************
		case 601:
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷
			if (P1.time == 1) {
				SEStart(2);
			}

			if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 15, 0);

			// �w���p�[�ݒ菀��.[1]	��ѓ���
			if (P1.time == 0) {
				P1.Var[3] = P1.Var[2];	// �������e���L��
				HelperReset(P1.Var[3]);
				H1[P1.Var[3]].var = false;

			}
			// �Ăяo��.[1]	��ѓ���
			else if (P1.time == 13) {
				H1[P1.Var[3]].var = true;
				H1[P1.Var[3]].time = 0;
				H1[P1.Var[3]].stateno = 2001;
				P1.Var[10] -= 180;
			}

			if (P1.time == 13) {
				SEStart(40);
				VelSet(-5.0, 0);
			}

			// �L�����Z��
			if (P1.StopTime == 0) {
				if (P1.time > 13) {
					// D�L�����Z��
					if ((P1.Var[11] > 0) && (P1.Senkou[4] > 0) && (P1.Var[12] == 0)) {
						// �n��ɂ��āA�w���p�[[1]�����݂��Ȃ�
						if (P1.SFlg != 2) {
							P1.Var[12]++;
							if ((P1.keyAtt[6]) && (P1.keyAtt[2]))
								P1.stateno = 603, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							else if (P1.keyAtt[2])
								P1.stateno = 602, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							else if(!P1.keyAtt[8]){
								P1.stateno = 600, P1.More = 1,
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
			// 602 2D
			//********************
		case 602:
			P1.ctrl = 0, P1.SFlg = 1;
			// SE��炷
			if (P1.time == 1) {
				SEStart(2);
			}

			if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 15, 0);

			// �w���p�[�ݒ菀��.[1]	��ѓ���
			if (P1.time == 0) {
				P1.Var[3] = P1.Var[2];	// �������e���L��
				HelperReset(P1.Var[3]);
				H1[P1.Var[3]].var = false;
			}
			// �Ăяo��.[1]	��ѓ���
			else if (P1.time == 13) {
				H1[P1.Var[3]].var = true;
				H1[P1.Var[3]].time = 0;
				H1[P1.Var[3]].stateno = 2000; 
				H_PosSet(P1.Var[3], 22, -138);
				P1.Var[10] -= 180;
			}

			if (P1.time == 13) {
				VelSet(-4.8, 0);
				SEStart(40);
			}

			// �q�b�g���L�����Z��
			if (P1.StopTime == 0) {
				if (P1.time > 13) {
					// D�L�����Z��
					if ((P1.Var[11] > 0) && (P1.Senkou[4] > 0) && (P1.Var[12] == 0)) {
						// �n��ɂ��āA�w���p�[[1]�����݂��Ȃ�
						if (P1.SFlg != 2) {
							P1.Var[12]++;
							if ((P1.keyAtt[6]) && (P1.keyAtt[2]))
								P1.stateno = 603, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							else if (P1.keyAtt[6])
								P1.stateno = 601, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							else if (!P1.keyAtt[8]) {
								P1.stateno = 600, P1.More = 1,
									P1.time = 0, P1.A.damage = 0;
							}
						}
					}
					// [�Q�[�W�Z]
					HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 11, P1.ctrl = 1;

			break;

			//********************
			// 603 3D
			//********************
		case 603:
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷
			if (P1.time == 1) {
				SEStart(2);
			}

			if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 15, 0);

			// �w���p�[�ݒ菀��.[1]	��ѓ���
			if (P1.time == 0) {
				P1.Var[3] = P1.Var[2];	// �������e���L��
				HelperReset(P1.Var[3]);
				H1[P1.Var[3]].var = false;

			}
			// �Ăяo��.[1]	��ѓ���
			else if (P1.time == 12) {
				H1[P1.Var[3]].var = true;
				H1[P1.Var[3]].time = 0;
				H1[P1.Var[3]].stateno = 2003;
				P1.Var[10] -= 180;
			}

			if (P1.time == 12) {
				VelSet(-5.0, 0);
				SEStart(40);
			}

			// �q�b�g���L�����Z��
			if (P1.StopTime == 0) {
				if (P1.time > 12) {
					// D�L�����Z��
					if ((P1.Var[11] > 0) && (P1.Senkou[4] > 0) && (P1.Var[12] == 0)) {
						// �n��ɂ��āA�w���p�[[1]�����݂��Ȃ�
						if (P1.SFlg != 2) {
							P1.Var[12]++;
							if (P1.keyAtt[6])
								P1.stateno = 601, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							else if (P1.keyAtt[2])
								P1.stateno = 602, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
							else if (!P1.keyAtt[8]) {
								P1.stateno = 600, P1.More = 1,
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
			// 605 �i�c
			//********************
		case 605:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			// ���x������
			if (P1.time == 13) {
				P1.XVel = -2.0;
				P1.YVel = -3.6;
				SEStart(40);
			}
			// SE��炷
			if (P1.time == 1) {
				SEStart(2);
			}

			if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 15, 0);


			// �w���p�[�ݒ菀��.[1]	��ѓ���
			if (P1.time == 0) {
				P1.Var[3] = P1.Var[2];
				HelperReset(P1.Var[3]);
				H1[P1.Var[3]].var = false;

			}
			// �Ăяo��.[1]	��ѓ���
			else if (P1.time == 13) {
				H1[P1.Var[3]].var = true;
				H1[P1.Var[3]].time = 0;
				H1[P1.Var[3]].stateno = 2002;
				P1.Var[10] -= 180;
			}

			// �d�͊ɘa
			VelAdd(0, P1.C.yAccel);

			if ((P1.time > 13) && (P1.time < ANIMELEM)) {
				// D�L�����Z��
				if ((P1.Var[11] > 0) && (P1.Senkou[4] > 0) && (P1.Var[12] == 0)) {
					// �n��ɂ��āA�w���p�[[1]�����݂��Ȃ�
					if (P1.SFlg == 2) {
						P1.Var[12]++;
						if (P1.keyAtt[6]) {
							P1.stateno = 606, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else {
							P1.stateno = 605, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
				}
			}
				

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.YPos + P1.YVel * 2 >= GROUND)P1.time = 0, P1.stateno = 609,
				P1.ctrl = 0, P1.SFlg = 0;

			break;

			//********************
			// 606 J6D
			//********************
		case 606:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.ignoreG = true;
			// ���x������
			if (P1.time == 13) {
				P1.XVel = -2.0;
				P1.YVel = -3.6;
				SEStart(40);
			}
			// SE��炷
			if (P1.time == 1) {
				SEStart(2);
			}

			if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 15, 0);


			// �w���p�[�ݒ菀��.[1]	��ѓ���
			if (P1.time == 0) {
				P1.Var[3] = P1.Var[2];
				HelperReset(P1.Var[3]);
				H1[P1.Var[3]].var = false;

			}
			// �Ăяo��.[1]	��ѓ���
			else if (P1.time == 13) {
				H1[P1.Var[3]].var = true;
				H1[P1.Var[3]].time = 0;
				H1[P1.Var[3]].stateno = 2000;
				H_PosSet(P1.Var[3], 20, -166);
				P1.Var[10] -= 180;
			}

			// �d�͊ɘa
			VelAdd(0, P1.C.yAccel);

			if ((P1.time > 13) && (P1.time < ANIMELEM)) {
				// D�L�����Z��
				if ((P1.Var[11] > 0) && (P1.Senkou[4] > 0) && (P1.Var[12] == 0)) {
					// �n��ɂ��āA�w���p�[[1]�����݂��Ȃ�
					if (P1.SFlg == 2) {
						P1.Var[12]++;
						if (P1.keyAtt[6]) {
							P1.stateno = 606, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else {
							P1.stateno = 605, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.YPos + P1.YVel * 2 >= GROUND)P1.time = 0, P1.stateno = 609,
				P1.ctrl = 0, P1.SFlg = 0;

			break;


			//********************
			// 609 ���n�d��
			//********************
		case 609:
			P1.ctrl = 0, P1.SFlg = 0;
			// ���x������
			VelSet(0, 0);
			P1.YPos = GROUND;

			// SE��炷
			if (P1.time == 1) {
				SEStart(6);
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.stateno = 0, P1.SFlg = 0, P1.ctrl = 1;

			break;

			//********************
			// 610 �Q�c
			//********************
		case 610:
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
			// 625 �i�U�c
			//********************
		case 625:
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
					H_VelSet(1, 7.5, 0);
				}
			}
			// �Ăяo��.[1]	��ѓ���
			else if (P1.time == 12){
				H1[1].var = true;
				H1[1].time = 0;
				H1[1].stateno = 2010;
			}

			// �d�͊ɘa
			VelAdd(0, P1.C.yAccel - 0.2);


			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.YPos + P1.YVel * 2 >= GROUND)P1.time = 0, P1.stateno = 621,
				P1.ctrl = 0, P1.SFlg = 0;

			break;

			/*
			//********************
			// 630 ����
			//********************
		case 630:
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷�A�ړ�
			if (P1.time == 0) {
				SEStart(22);
				P1.XVel = 0;
			}

			//if (P1.time == 13 - 1)SEStart(9);
			//if (P1.time == 33 - 1)SEStart(9);

			if (AnimElem(11)) {
				P1.Var[11] = 600;
				P1.Var[10] = 1800;
				// ���i�G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.40, 0.40, P1.PSide);
				SEStart(31);
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			*/

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

			// �Q�[�W����
			if (P1.time == 0){
				P1.Power -= 1000;
				S.StopTime = 10;
				S.Anten = 25;
				SEStart(25);
				// ���i�G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}


			// �w���p�[�ݒ菀��.[1]	��ѓ���
			if (P1.time == 0) {
				HelperReset(5);
				H1[5].var = false;

			}
			// �Ăяo��.[1]	��ѓ���
			else if (P1.time == 10) {
				H1[5].var = true;
				H1[5].time = 0;
				H1[5].stateno = 2100;
				SEStart(40);
			}

			if (P1.time == 12) {
				VelSet(-5.4, 0);
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

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
				}
			}


			// �I���E34
			if (P1.time >= 34)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			
			//********************
			// 850 ���@���p�C�A�L���[
			//********************
		case 850:
			P1.ctrl = 0;
			P1.A.gaugeHosei = true;
			P1.ignoreG = false;
			P1.SFlg = 0;

			// �Q�[�W����
			if (P1.time == 0) {
				P1.Power -= 3000;
				S.StopTime = 15;
				S.noGauge = 15;
				S.Anten = 25;
				SEStart(25);
				// �G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);

				DamReset();
				P1.Var[13] = 13;
			}

			if(P1.time == 95)SEStart(22);


			// �Ăяo��.[1]	��ѓ���
			if ((P1.time == 10) || (P1.time == 17) || 
				(P1.time == 24) || (P1.time == 31) ||
				(P1.time == 38) || (P1.time == 45) ||
				(P1.time == 52) 
				//|| (P1.time == 59)
				) {
				HelperReset(P1.Var[13]);
				H1[P1.Var[13]].var = true;
				H1[P1.Var[13]].time = 0;
				if (P1.time == 52)
					H1[P1.Var[13]].stateno = 2150;
				else 
				{ H1[P1.Var[13]].stateno = 2050; }
				H_PosSet(P1.Var[13], 20, -168);
				P1.Var[13]++;

				VelSet(-0.8, 0);
				if (P1.time == 52)VelSet(-5.4, 0);
				SEStart(40);
			}

			// �I��
			if (P1.time >= ANIMELEM) {
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
					// 2000 5Dor2D
					//********************
				case 2000:


					//***************
					//  ��{�ݒ�
					//***************
					// ���W�E���x�ݒ�
					if (H1[i].time == 0) {
						H1[i].muki = P1.muki;

						// ���x�Ȃ�������ݒ�
						if (H1[i].HXVel == 0) {
							H_VelSet(i, 16.2, 0);
						}

						//H_PosSet(i, 20, -166);
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 0) {
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 90;
						// ����
						if (P1.Var[11] > 0) {
							H1[i].HMoveHit = 2;
							H1[i].A.damage = 100;
						}
					}
					// �_���[�W�ʒu�Z�b�g
					if (H_triggerT(i, 0, 1)) {

						//[ �U������Z�b�g ]
						//H2_AttSet(i, 0, 50, 30, -20, -36);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 120, H1[i].HGivePow = 60;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						// [�q�b�g�d��]
						H_HitTime(i, 5, 22, 24, 16);
						H1[i].HSSelf = 0;

						H1[i].fallF = 2;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H_HitVel(i, -2.6, 0, -1.4, -2.6);

						if (P1.Var[11] > 0) {
							H_HitTime(i, 6, 24, 26, 18);
							H_HitVel(i, -3.8, 0, -2.6, -3.0);
						}
					}


					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 16)H1[i].HAnimTime = 0;
					// �I�������A�[�ɓ��Bor�n�ʂɓ�����or�q�b�gor�{�̔�_��
					if ((H1[i].XPos < (S.ScroolX)) ||
						(H1[i].XPos > (S.ScroolX + SCREEN_W)) ||
						(H1[i].YPos > GROUND) ||
						(H1[i].HMoveHit == 0) ||
						(P1.stateno >= 1000)
						)
					{
						HelperReset(i);
					}

					break;
					
					//********************
					// 2001 �΂ߏ�
					//********************
				case 2001:


					//***************
					//  ��{�ݒ�
					//***************
					// ���W�E���x�ݒ�
					if (H1[i].time == 0) {
						H1[i].muki = P1.muki;

						// ���x�Ȃ�������ݒ�
						if (H1[i].HXVel == 0) {
							H_VelSet(i, 12.1, -12.1);
						}

						H_PosSet(i, 20, -188);
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 0) {
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 90;
						// ����
						if (P1.Var[11] > 0) {
							H1[i].HMoveHit = 2;
							H1[i].A.damage = 100;
						}
					}
					// �_���[�W�ʒu�Z�b�g
					if (H_triggerT(i, 0, 1)) {

						//[ �U������Z�b�g ]
						//H2_AttSet(i, 0, 50, 30, -20, -36);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 120, H1[i].HGivePow = 60;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						// [�q�b�g�d��]
						H_HitTime(i, 5, 22, 24, 16);
						H1[i].HSSelf = 0;

						H1[i].fallF = 2;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H_HitVel(i, -2.6, 0, -1.4, -2.6);
						
						if (P1.Var[11] > 0) {
							H_HitTime(i, 6, 24, 26, 18);
							H_HitVel(i, -3.8, 0, -2.6, -3.0);
						}
					}


					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 16)H1[i].HAnimTime = 0;
					// �I�������A�[�ɓ��Bor�n�ʂɓ�����or�q�b�gor�{�̔�_��
					if ((H1[i].XPos < (S.ScroolX)) ||
						(H1[i].XPos > (S.ScroolX + SCREEN_W)) ||
						(H1[i].YPos > GROUND) ||
						(H1[i].HMoveHit == 0) ||
						(P1.stateno >= 1000)
						)
					{
						HelperReset(i);
					}

					break;

					//********************
					// 2002 �΂߉�
					//********************
				case 2002:


					//***************
					//  ��{�ݒ�
					//***************
					// ���W�E���x�ݒ�
					if (H1[i].time == 0) {
						H1[i].muki = P1.muki;

						// ���x�Ȃ�������ݒ�
						if (H1[i].HXVel == 0) {
							H_VelSet(i, 12.1, 12.1);
						}

						H_PosSet(i, 20, -138);
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 0) {
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 90;
						// ����
						if (P1.Var[11] > 0) {
							H1[i].HMoveHit = 2;
							H1[i].A.damage = 100;
						}
					}
					// �_���[�W�ʒu�Z�b�g
					if (H_triggerT(i, 0, 1)) {

						//[ �U������Z�b�g ]
						//H2_AttSet(i, 0, 50, 30, -20, -36);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 120, H1[i].HGivePow = 60;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						// [�q�b�g�d��]
						H_HitTime(i, 5, 22, 24, 16);
						H1[i].HSSelf = 0;

						H1[i].fallF = 2;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H_HitVel(i, -2.6, 0, -1.4, -2.6);
						
						if (P1.Var[11] > 0) {
							H_HitTime(i, 6, 24, 26, 18);
							H_HitVel(i, -3.8, 0, -2.6, -3.0);
						}
					}


					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 16)H1[i].HAnimTime = 0;
					// �I�������A�[�ɓ��Bor�n�ʂɓ�����or�q�b�gor�{�̔�_��
					if ((H1[i].XPos < (S.ScroolX)) ||
						(H1[i].XPos > (S.ScroolX + SCREEN_W)) ||
						(H1[i].YPos > GROUND) ||
						(H1[i].HMoveHit == 0) ||
						(P1.stateno >= 1000)
						)
					{
						HelperReset(i);
					}

					break;

					//********************
					// 2003 30�x
					//********************
				case 2003:


					//***************
					//  ��{�ݒ�
					//***************
					// ���W�E���x�ݒ�
					if (H1[i].time == 0) {
						H1[i].muki = P1.muki;

						// ���x�Ȃ�������ݒ�
						if (H1[i].HXVel == 0) {
							H_VelSet(i, 14.2, 7.1);
						}

						H_PosSet(i, 20, -130);
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 0) {
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 90;
						// ����
						if (P1.Var[11] > 0) {
							H1[i].HMoveHit = 2;
							H1[i].A.damage = 100;
						}
					}
					// �_���[�W�ʒu�Z�b�g
					if (H_triggerT(i, 0, 1)) {

						//[ �U������Z�b�g ]
						//H2_AttSet(i, 0, 50, 30, -20, -36);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 120, H1[i].HGivePow = 60;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						// [�q�b�g�d��]
						H_HitTime(i, 5, 22, 24, 16);
						H1[i].HSSelf = 0;

						H1[i].fallF = 2;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H_HitVel(i, -2.6, 0, -1.4, -2.6);

						if (P1.Var[11] > 0) {
							H_HitTime(i, 6, 24, 26, 18);
							H_HitVel(i, -3.8, 0, -2.6, -3.0);
						}
					}


					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 16)H1[i].HAnimTime = 0;
					// �I�������A�[�ɓ��Bor�n�ʂɓ�����or�q�b�gor�{�̔�_��
					if ((H1[i].XPos < (S.ScroolX)) ||
						(H1[i].XPos > (S.ScroolX + SCREEN_W)) ||
						(H1[i].YPos > GROUND) ||
						(H1[i].HMoveHit == 0) ||
						(P1.stateno >= 1000)
						)
					{
						HelperReset(i);
					}

					break;


					//********************
					// 2050 5Dor2D
					//********************
				case 2050:

					//***************
					//  ��{�ݒ�
					//***************
					// ���W�E���x�ݒ�
					if (H1[i].time == 0) {
						H1[i].muki = P1.muki;

						// ���x�Ȃ�������ݒ�
						if (H1[i].HXVel == 0) {
							H_VelSet(i, 16.2, 0);
						}

						//H_PosSet(i, 20, -166);
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 0) {
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 87;
						H1[i].A.hosyo = 8;
					}
					// �_���[�W�ʒu�Z�b�g
					if (H_triggerT(i, 0, 1)) {

						//[ �U������Z�b�g ]
						//H2_AttSet(i, 0, 50, 30, -20, -36);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 60;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						// [�q�b�g�d��]
						H_HitTime(i, 5, 22, 24, 16);
						H1[i].HSSelf = 0;

						H1[i].fallF = 2;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1000;
						// [�m�b�N�o�b�N]
						H_HitVel(i, -4.0, 0, -2.8, -2.6);

					}


					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 16)H1[i].HAnimTime = 0;
					// �I�������A�[�ɓ��Bor�n�ʂɓ�����or�q�b�gor�{�̔�_��
					if ((H1[i].XPos < (S.ScroolX)) ||
						(H1[i].XPos > (S.ScroolX + SCREEN_W)) ||
						(H1[i].YPos > GROUND) ||
						(H1[i].HMoveHit == 0) ||
						(P1.stateno >= 1000)
						)
					{
						HelperReset(i);
					}

					break;
					
					//********************
					// 2100 ���e
					//********************
				case 2100:
					//***************
					//  ��{�ݒ�
					//***************
					P1.A.gaugeHosei = true;

					// ���W�E���x�ݒ�
					if (H1[i].time == 0) {
						H1[i].muki = P1.muki;

						// ���x�Ȃ�������ݒ�
						if (H1[i].HXVel == 0) {
							H_VelSet(i, 24.0, 0);
						}

						H_PosSet(i, 20, -168);
						EffStart(16, (int)H1[i].XPos, (int)H1[i].YPos, 20, 0,
							0.02, 0.14, P1.muki);
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 0) {
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 130;
						H1[i].A.hosyo = 80;
						// ����
						if (P1.Var[11] > 0) {
							H1[i].HMoveHit = 1;
							H1[i].A.damage = 150;
							H1[i].A.hosyo = 100;
						}
					}

					// �_���[�W�ʒu�Z�b�g
					if (H_triggerT(i, 0, 1)) {

						// �G�t�F�N�g
						h_HitEff(i, 1, 1, 1);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 14, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 220;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						// [�q�b�g�d��]
						H_HitTime(i, 8, 30, 40, 16);
						H1[i].HSSelf = 0;

						H1[i].fallF = 1;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1030;
						// [�m�b�N�o�b�N]
						H_HitVel(i, -3.6, -3.2, -2.6, -2.8);
						if (P1.Var[11] > 0) {
							H_HitTime(i, 10, 32, 42, 18);
							H_HitVel(i, -5.2, -3.8, -4.4, -3.2);
						}
					}


					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 16)H1[i].HAnimTime = 0;
					// �I�������A�[�ɓ��Bor�n�ʂɓ�����or�q�b�gor�{�̔�_��
					if ((H1[i].XPos < (S.ScroolX)) ||
						(H1[i].XPos > (S.ScroolX + SCREEN_W)) ||
						(H1[i].YPos > GROUND) ||
						(H1[i].HMoveHit == 0) ||
						(P1.stateno >= 1000)
						)
					{
						HelperReset(i);
					}

					break;


					//********************
					// 2150 ���e
					//********************
				case 2150:
					//***************
					//  ��{�ݒ�
					//***************
					P1.A.gaugeHosei = true;

					// ���W�E���x�ݒ�
					if (H1[i].time == 0) {
						H1[i].muki = P1.muki;

						// ���x�Ȃ�������ݒ�
						if (H1[i].HXVel == 0) {
							H_VelSet(i, 16.2, 0);
						}

						H_PosSet(i, 20, -168);
						EffStart(16, (int)H1[i].XPos, (int)H1[i].YPos, 20, 0,
							0.02, 0.14, P1.muki);
					}

					// �q�b�g���E�_���[�W�Z�b�g
					if (H1[i].time == 0) {
						H1[i].HMoveHit = 1;
						H1[i].A.damage = 130;
						H1[i].A.hosyo = 100;
					}

					// �_���[�W�ʒu�Z�b�g
					if (H_triggerT(i, 0, 1)) {

						// �G�t�F�N�g
						h_HitEff(i, 1, 1, 1);

						// [�q�b�g�E�K�[�h�T�E���h]
						H1[i].HHitSE = 14, H1[i].HGuardSE = 17;
						// [�Q�[�W] 
						H1[i].HGetPow = 0, H1[i].HGivePow = 220;
						//[ �K�[�h���� ]
						H1[i].HGuardF = 1;
						// [�q�b�g�d��]
						H_HitTime(i, 10, 30, 40, 16);
						H1[i].HSSelf = 0;

						H1[i].fallF = 1;

						// [�q�b�g���̃A�j��]
						H1[i].HHitAnim = 1030;
						// [�m�b�N�o�b�N]
						H_HitVel(i, -5.0, -3.2, -3.8, -2.8);
					}


					H_Move(i);

					//***************
					//  �s�����e
					//***************
					// �A�j�������[�v������
					if (H1[i].HAnimTime >= 16)H1[i].HAnimTime = 0;
					// �I�������A�[�ɓ��Bor�n�ʂɓ�����or�q�b�gor�{�̔�_��
					if ((H1[i].XPos < (S.ScroolX)) ||
						(H1[i].XPos > (S.ScroolX + SCREEN_W)) ||
						(H1[i].YPos > GROUND) ||
						(H1[i].HMoveHit == 0) ||
						(P1.stateno >= 1000)
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
	if (P1.PSide == 1)Get_PSet(P1, P2), Get_HSet(H1, H2);//, Get_SSet(S);
	if (P1.PSide == 2)Get_PSet(P2, P1), Get_HSet(H2, H1);//, Get_SSet(S);

}//StateParam�I��




//-------------------------------------------------------------------------------
//
// �󂯎��p
//
//-------------------------------------------------------------------------------

// ���Ԃ��t�ɂ���
void GetP_HydParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_HydParam(System_t GS)
{
	S = GS;
}

void GetH_HydParam(Helper GH1[], Helper GH2[])
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
	if (P1.Senkou[4] > 0 && (P1.Var[10] > 180)) {
		// �n��ɂ��āA�w���p�[[1]�����݂��Ȃ�
		if (P1.SFlg != 2) {
			P1.stateno = 600, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
			if (P1.keyAtt[2])
				P1.stateno = 603;
			else if (P1.keyAtt[6])
				P1.stateno = 601;
		}
	}

	// 22�c
	/*
	if ((P1.Senkou[4] > 0) && (P1.cmd[3]) && (P1.Var[10] <= 0)) {		// ��s���͌������Ă݂�
		if (P1.SFlg != 2) {
			P1.stateno = 630, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	*/

	HCancel();

	//�Ō�ɔ��������
	DamReset();
}

// �K�E�Z�L�����Z��
void HCancel()
{
	// 236 + C
	if ((P1.Senkou[3] > 0)
		&& (P1.cmd[1]) && (P1.Power >= 1000)) {		// ��s���͌������Ă݂�
		P1.stateno = 800, P1.More = 1,
		P1.time = 0, P1.A.damage = 0;
	}

	//  [ �R�Q�[�W ]
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.Senkou[3] > 0)
		&& (P1.Power >= 3000)) {		// �n��
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
	// [�K�E�Z]
	if (P1.SFlg == 2) {
		// D (�m�[�}���ݒu�A�n�㔭��)
		if (P1.Senkou[4] > 0 && (P1.Var[10] > 180)) {
			// �w���p�[[2]������
			if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8])) {
					P1.stateno = 606, P1.More = 1,
						P1.time = 0, P1.A.damage = 0;
			}
			// �ݒu���Ȃ�
			else {
				P1.stateno = 605, P1.More = 1,
					P1.time = 0, P1.A.damage = 0;
			}
		}
	}
}
