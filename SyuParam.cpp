//========================================================================
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
void SyuParam(void)
{


	//�������A��؂�
	if (P1.More)P1.More = 0;

	// �X�e�[�g���؂�ւ������
	if (P1.stateno != P1.OldState) {
		GetP_SameParam(P1, P2);
		GetH_SameParam(H1, H2);
		GetS_SameParam(S);
		ChangeState();
	}

	//=============
	// �L�����ʎ��O�ݒ�
	//=============

	if (P1.Var[13] > 0) {
		// �J���[�ύX
		if (P1.colorCTime <= 0) {
			P1.colorC[0] = 255, P1.colorC[1] = 124, P1.colorC[2] = 124;
			P1.colorCTime = 1;
		}
		P1.Var[13]--;
		P1.Power = 0;
		P1.A.gaugeHosei = 1;
	}
	// 1P
	if (P1.StopTime == 0 || P1.time == 0) {
		//�ݒ�J�n
		//--------------------
		// �U�����胊�Z�b�g
		//--------------------
		if (P1.stateno < 200 || P1.stateno > 999) {
			DamReset();

		}
		//--------------------
		// 5�`6�F�U�����
		// 40�`47�F
		// �W�����v
		// 50�`59�F
		// �K�[�h
		//--------------------
		if (
			((P1.stateno >= 5) && (P1.stateno <= 10)) ||	// �U������A���Ⴊ�݈ڍs
			(P1.stateno == 40) ||
			((P1.stateno >= 46) && (P1.stateno <= 47)) ||	// �W�����v
			((P1.stateno >= 50) && (P1.stateno <= 69)) ||	// �K�[�h�E�󒆃_�b�V���E�n�C�W�����v�@
			(P1.stateno == 520) ||	// �K�[�L����
			((P1.stateno >= 550) && (P1.stateno <= 599))	// EX�A�N�V����
			) {
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
		if ((P1.stateno >= 1000) && (P1.stateno <= 1999)) {	// ������炢
			GetP_SameParam(P1, P2);
			GetH_SameParam(H1, H2);
			GetS_SameParam(S);
			DamParam();
		}

		// �`�F�[���������A�R�}����0.5�t���O
		if (P1.ctrl) {
			P1.Var[2] = 0;
		}
		// �A�ŃX�C�b�`
		//if (P1.ctrl)P1.Var[20] = 0;

		if ((P1.stateno != 610) && (P1.stateno != 611))P1.Var[15] = 0;
		// �A�[�}�[�L�����Z��
		if (P1.stateno == 225 || P1.stateno == 230 || P1.stateno == 320);
		else { P1.Var[14] = 0; }


		//
		switch (P1.stateno) {

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
			if (P1.time == 1) {
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
				P1.mutekiF == 0) {
				P1.mutekiF = 2;
				P1.Muteki = 10;
			}
			// �����͉����� 12 ��
			if (InputPAD(102) == 0) {
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
			if (P1.time > ANIMELEM) {
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
		case 30:	// �X�e�b�v
			P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
			P1.ignoreG = true;

			// SE��炷
			if (P1.time == 1)SEStart(5);

			VelAdd(0, P1.C.yAccel);

			//���x�𑫂�
			if (P1.time == 0) {
				VelSet(P1.C.runF[0], P1.C.runF[1]);
			}


			// �I��
			if ((P1.YPos + P1.YVel * 2 >= GROUND) && (P1.YVel > 0)) {
				P1.stateno = 0;
				P1.time = 0;
				P1.More = 1;
				P1.SFlg = 0;
				P1.ctrl = 1;
				P1.ignoreG = false, VelSet(0, 0);
				P1.YPos = GROUND;
				SEStart(6);
			}

			break;
		case 31:	// ���n�d��
			P1.SFlg = 0, P1.ctrl = 0;

			// SE��炷
			if (P1.time == 0)SEStart(6);
			VelSet(0, 0);
			P1.YPos = GROUND;

			// �I��
			if (P1.time >= ANIMELEM) {
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
				P1.More = 1;
			}

			break;
		case 35:	// �o�b�N�X�e�b�v
			P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
			P1.ignoreG = true;

			// ���G
			if (P1.time <= 9) {
				P1.Muteki = 1, P1.mutekiF = 0;
			}

			// SE��炷
			if (P1.time == 1)SEStart(5);

			VelAdd(0, P1.C.yAccel);

			//���x�𑫂�
			if (P1.time == 0) {
				VelSet(P1.C.runB[0], P1.C.runB[1]);
				if (P1.Var[13] > 0)P1.XVel -= 0.5;
			}

			// �I��
			if ((P1.YPos + P1.YVel * 2 >= GROUND) && (P1.YVel > 0)) {
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
			if (P1.time >= ANIMELEM + 1) {
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
			}

			break;

			//
		case 41:	// ����
			P1.SFlg = 2, P1.ctrl = 1;
			P1.Muteki = 0;
			P1.XVel = 0;	// �����͍��E�ɔ�΂Ȃ�
			// �^�C���� 0 �̂Ƃ��A���ɔ��
			if (P1.time == 0) {
				// ��i�W�����v���A�W�����v�͂�������
				{
					//P1.YPos -= 2;
				}
			}
			// ����
			if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
			// �n�ʂɂ��Ď��Ԃ� 0 ����Ȃ���
			if ((P1.YPos > GROUND) && (P1.time != 0)) {
				P1.stateno = 47, P1.More = 1, P1.time = 0;
			}
			break;
		case 42:	// �O�W�����v
			P1.SFlg = 2, P1.ctrl = 1;
			P1.Muteki = 0;

			P1.XVel = P1.C.jumpF[0]; //���x�𑫂�;

			if (P1.JumpVel == 2)P1.XVel = P1.C.jumpF[1];// += (P1.XVel / 4) + 0.2;	// �_�b�V����
			if (P1.hyJump)P1.XVel = P1.C.jumpF[1];//P1.C.jumpF*1.5; //���x�𑫂�;
			// �^�C���� 0 �̂Ƃ��A���ɔ��
			if (P1.time == 0) {
				// ��i�W�����v���A�W�����v�͂�������
				{
					//P1.YPos -= 2;
				}
			}
			// ����
			if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
			// �n�ʂɂ��Ď��Ԃ� 0 ����Ȃ���
			if ((P1.YPos > GROUND) && (P1.time != 0)) {
				P1.stateno = 47, P1.More = 1, P1.time = 0;
			}
			break;
		case 43:	// ���W�����v
			P1.SFlg = 2, P1.ctrl = 1;
			P1.Muteki = 0;

			P1.XVel = P1.C.jumpB; //���x�𑫂�;

			if (P1.hyJump)P1.XVel = P1.C.jumpB*1.4; //���x�𑫂�;
			// �^�C���� 0 �̂Ƃ��A���ɔ��
			if (P1.time == 0) {
				// ��i�W�����v���A�W�����v�͂�������
				{
					//P1.YPos -= 2;
				}
			}
			// ����
			if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
			// �n�ʂɂ��Ď��Ԃ� 0 ����Ȃ���
			if ((P1.YPos > GROUND) && (P1.time != 0)) {
				P1.stateno = 47, P1.More = 1, P1.time = 0;
			}
			break;

		case 45:	// �󒆃W�����v
			P1.ctrl = 0, P1.SFlg = 2;
			P1.XVel = 0, P1.YVel = -0.2;


			if (P1.time == 0) {
				// ������
				if (P1.keyAtt[4]) {
					P1.JumpVel = -1;
				}
				// �E����
				else if (P1.keyAtt[6]) {
					P1.JumpVel = 1;
				}
				else { P1.JumpVel = 0; }

				P1.AirJump--, P1.airDash--;
				P1.hyJump = false;

				// ��s���͂�����
				P1.K_Senkou[8] = 0;
			}

			// �U�����
			if (P1.time == 0) {
				if (P1.XPos < P2.XPos && P1.muki == 1) {
					P1.muki = 0;
					P1.JumpVel = -P1.JumpVel;
				}
				else if (P2.XPos < P1.XPos && P1.muki == 0) {
					P1.muki = 1;
					P1.JumpVel = -P1.JumpVel;
				}
			}

			// �X�e�[�g�ύX�A2�t���𒴂������W�����v��
			if (P1.time > 1) {
				//�G�t�F�N�g
				EffStart(16, (int)P1.XPos, (int)P1.YPos - 20, 0, 0,
					0.24, 0.04, P1.muki);
				// �W�����v�t���O���I��
				P1.JumpFlg = 1;
				// �������A�Ŏ��̃X�e�[�g��
				P1.More = 1, P1.time = 0;
				switch (P1.JumpVel) {
				case 0:
					P1.stateno = 41;
					break;
				case 1:
					P1.stateno = 42;
					break;
				case -1:
					P1.stateno = 43;
					break;
				}
				P1.YVel = P1.C.jumpA + 0.8;
			}
			break;	// �W�����v�����I��

		//
		case 90:	// �J��
			P1.SFlg = 0, P1.ctrl = 0;

			if (P1.time == 15 - 1)SEStart(9);

			// �S�̃t���[���𒴂����痧����
			if (P1.time >= P1.animElem[P1.stateno]) {
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
			if (P1.time == 0) {
				if ((P1.XPos < P2.XPos) && (P1.muki == 1)) {
					P1.muki = 0;
				}
				else if ((P1.XPos > P2.XPos) && (P1.muki == 0)) {
					P1.muki = 1;
				}
			}
			// ��s���͏���
			if (P1.time <= 1)P1.Senkou[1] = 0;

			// �A�ŉ񐔃J�E���g
			if (P1.time == 0) {
				P1.Var[2]++;
			}

			// ���� �� �S�̃t���[���A54�𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM) {
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}
			// SE��炷
			if (P1.time == 1) {
				SEStart(1);
			}

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 1, 2);
			}

			// �q�b�g���Z�b�g�A�����U�t��
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.MoveHit > 0)) {

				// [�_���[�W]
				Damage(46, 0);

				// [�Q�[�W] 
				Power(31);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(6, 12, 16, 10);
				// [�m�b�N�o�b�N]
				HitVel(-3.6, 0, -1.6, -4.8);
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
			if ((P1.CFlg) && (P1.time >= 1)) {
				// [�W�����v�L�����Z��]
				if (P1.keyAtt[8]) {		// ��s���͌������Ă݂�
					P1.stateno = 40, P1.More = 1,
						P1.time = 0, P1.A.damage = 0;
				}
				// [�ʏ�Z]
				if (P1.Senkou[3] > 0) {
					// ������
					if ((key(6)) && (key(2))) {
						P1.stateno = 325, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// 6C
					else if ((key(6)) && (!key(2)) && (!key(8))) {
						P1.stateno = 225, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)) {
						P1.stateno = 320, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.stateno = 220, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if (P1.Senkou[2] > 0) {		// ��s���͌������Ă݂�
					// 6B
					if ((key(6)) && (!key(2)) && (!key(8))) {
						P1.stateno = 215, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)) {
						P1.stateno = 310, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.stateno = 210, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				// ��s����
				else if ((P1.Senkou[1] > 0) && (P1.Var[2] < 5)) {
					if (key(2)) {
						P1.stateno = 300, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}

				// [�K�E�Z�E�Q�[�W�Z]
				SCancel();
			}
			// �L�����Z���A�A�ŃL�����Z��
			if ((P1.button[1] == 1) && (P1.time >= ANIMELEM - 2) && (P1.stateno == 200)) {
				// ���v�f�������Ă�
				if (!key(2)) {
					P1.More = 1, P1.time = 0, P1.A.damage = 0;
				}
			}

			// ����
			DelayThrow(1);
			// 3�Q�[�W
			Delay3Button(850, 3000);

			break;

			//********************
			// 205�F����A
			//********************
		case 205:	//����A
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// �q�b�g���Z�b�g 
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}
			// �ʒu�����炷
			if (P1.time == 2) {
				PosAdd(4, 0);
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(90, 0);

				// [�Q�[�W] 
				Power(99);
				HitTime(8, 18, 20, 16);

				// [�m�b�N�o�b�N]
				HitVel(-3.6, 0, -1.4, -5);
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
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
					// [�W�����v�L�����Z��]
					if (P1.K_Senkou[8]) {		// ��s���͌������Ă݂�
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0) {
						if (key(2)) {
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else {
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}

			// ����
			// 2F�ɓ��������炵����������
			if ((P1.time == 1) &&
				(P1.button[1] == 2 && P1.button[2] == 1) &&
				(P2.HFlg == 0)
				) {
				P1.stateno = 500, P1.More = 1,
					P1.time = 1, P1.A.damage = 0;
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
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}
			// SE
			if (P1.time == 1)SEStart(1);

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 2, 1);
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.MoveHit > 0)) {

				// [�_���[�W]
				Damage(90, 0);

				// [�Q�[�W] 
				Power(108);
				HitTime(8, 18, 22, 16);

				// [�m�b�N�o�b�N]
				HitVel(-2.6, 0, -1.7, -4.4);
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
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
					// [�W�����v�L�����Z��]
					if ((P1.K_Senkou[8]) && (P2.HFlg == 1)) {		// ��s���͌������Ă݂�
						P1.stateno = 40, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0) {
						// ������
						if ((key(6)) && (key(2))) {
							P1.stateno = 325, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else if (key(6)) {
							P1.stateno = 225, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else if (key(2)) {
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else {
							P1.stateno = 220, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
					}
					else if (P1.Senkou[2] > 0) {
						// ���v�f���m�F
						if (key(2)) {
							P1.stateno = 310, P1.More = 1,
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
			// 215�F6B
			//********************
		case 215:
			P1.SFlg = 2, P1.ctrl = 0, P1.Lock = 0;
			P1.ignoreG = true;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// �ʒu�����炷
			if (P1.time == 0) {
				PosAdd(6, 0);
			}
			// SE��炷
			if (P1.time == 1)SEStart(3);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			
			// �q�b�g���Z�b�g �����F26
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}
			//else{ P1.MoveHit = 0; }

			if (P1.time >= 5) {
				VelAdd(0, 0.38);
			}

			//���x�𑫂�
			if (P1.time == 5) {
				VelSet(1.2, -5.0);
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(90, 20);
				// [�Q�[�W] 
				Power(155);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 19, 22, 16);
				// [�m�b�N�o�b�N]
				HitVel(-3.8, 0, -2.8, -4);
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

			// �I��
			if ((P1.YPos + P1.YVel * 2 >= GROUND) && (P1.YVel > 0) && (P1.time > 9)) {
				P1.stateno = 216;
				P1.time = 0;
				P1.SFlg = 0;
				P1.ctrl = 0;
			}

			// ����
			DelayThrow(2);
			// 3�Q�[�W
			Delay3Button(850, 3000);

			break;

		case 216:	// ���n�d��
			P1.SFlg = 0, P1.ctrl = 0;

			// SE��炷
			if (P1.time == 1)SEStart(6);
			VelSet(0, 0);
			P1.YPos = GROUND;

			// �I��
			if (P1.time >= ANIMELEM + 1) {
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
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

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 3, 0);
			}
			
			// �ʒu�����炷
			if (P1.time == 0) {
				VelSet(2.2, 0);
			}

			if (P1.time == 7) {
				if (P1.muki == 0) {
					P1.XPos += 9;
				}
				else {
					P1.XPos -= 9;
				}
			}

			// �A�[�}�[�J�E���g
			if (P1.time == 0)P1.Var[12] = P1.D.armor;

			// �q�b�g���Z�b�g
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(110, 10);
				// [�Q�[�W] 
				Power(150);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 24, 25, 22);
				//HitTime(8, 22, 24, 18);
				// [�m�b�N�o�b�N]
				HitVel(-4.2, 0, -2.8, -2.8);
				GuardVel(-6.2, -4.0);
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

			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

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
			// SE��炷
			if (P1.time == 1) {
				SEStart(3);
			}

			if (P1.time == 0) {
				if (P1.muki == 0) {
					P1.XPos += 6;
				}
				else {
					P1.XPos -= 6;
				}
			}

			if (P1.time == 9) {
				P1.XVel = 4.2;
			}

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 3, 0);
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(110, 10);
				// [�Q�[�W] 
				Power(150);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 22, 22, 20);
				// [�m�b�N�o�b�N]
				HitVel(-4.2, 0, -2.8, -2.8);
				GuardVel(-6.4, -4.0);
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

			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
					// [�K�E�Z�E�Q�[�W�Z]
					HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

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
			//[ �ʏ�Z�̎����U����� ]
			if (P1.time == 0) {
				if ((P1.XPos < P2.XPos) && (P1.muki == 1)) {
					P1.muki = 0;
				}
				else if ((P1.XPos > P2.XPos) && (P1.muki == 0)) {
					P1.muki = 1;
				}
			}
			// ��s���͏���
			if (P1.time <= 1)P1.Senkou[1] = 0;

			// �A�ŉ񐔃J�E���g
			if (P1.time == 0) {
				P1.Var[2]++;
			}

			// SE��炷
			if (P1.time == 1)SEStart(1);
			//  �S�̃t���[���A14�𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 1, 2);
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;
			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.MoveHit > 0)) {

				// [�_���[�W]
				Damage(45, 0);

				// [�Q�[�W] 
				Power(30);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(6, 12, 16, 10);

				// [�m�b�N�o�b�N]
				HitVel(-3.6, 0, -1.6, -4.8);


				P1.GuardF = 1;

				P1.HitAnim = 1010;	// ���i��炢
				P1.HitSE = 10;
			}
			else {
				DamReset();
			}


			// �L�����Z��
			if ((P1.CFlg) && (P1.time >= 1)) {

				if (P1.Senkou[3] > 0) {
					// ������
					if ((key(6)) && (key(2))) {
						P1.stateno = 325, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// 6C
					else if ((key(6)) && (!key(2)) && (!key(8))) {
						P1.stateno = 225, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)) {
						P1.stateno = 320, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.stateno = 220, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if (P1.Senkou[2] > 0) {		// ��s���͌������Ă݂�
					// 6B
					if ((key(6)) && (!key(2)) && (!key(8))) {
						P1.stateno = 215, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else if (key(2)) {
						P1.stateno = 310, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.stateno = 210, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
				else if ((P1.Senkou[1] > 0) && (P1.Var[2] < 5)) {		// ��s���͌������Ă݂�
					if (key(2)) {
						P1.stateno = 300, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					else {
						P1.stateno = 200, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}

				// [�K�E�Z�E�Q�[�W�Z]
				SCancel();
			}

			// �L�����Z���A�A�ŃL�����Z��
			if ((P1.button[1] == 1) && (P1.time >= ANIMELEM - 2) && (P1.stateno == 300)) {
				// ���v�f�������Ă�
				if (key(2)) {
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
			// �ʒu�����炷
			if (P1.time == 5) {
				if (P1.muki == 0) {
					P1.XPos += 6;
				}
				else {
					P1.XPos -= 6;
				}
			}
			// SE
			if (P1.time == 1)SEStart(1);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;
			// �q�b�g���Z�b�g
			if (P1.time == 5) {
				P1.MoveHit = 1;	// �P��
			}


			// �_���[�W�Z�b�g
			if ((P1.time >= 5) && (P1.time <= 10)) {

				// [�_���[�W]
				Damage(85, 0);

				// [�Q�[�W] 
				Power(95);

				HitTime(8, 18, 20, 14);

				// [�m�b�N�o�b�N]
				HitVel(-4.0, 0, -1.8, -4.6);
				P1.GuardF = 3;	// ���i
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
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 5)) {
					// [�ʏ�Z]
					if (P1.Senkou[3] > 0) {		// ��s���͌������Ă݂�
						// ������
						if ((key(6)) && (key(2))) {
							P1.stateno = 325, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						// �X��
						else if (key(6)) {
							P1.stateno = 225, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						// ���v�f���m�F
						else if (key(2)) {
							P1.stateno = 320, P1.More = 1,
								P1.time = 0, P1.A.damage = 0;
						}
						else {
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
			// 320�F������
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
			if (P1.time == 1) {
				PVOStart(P1.PSide, 3, 0);
			}

			// �q�b�g���Z�b�g 10�t��
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(105, 0);
				// [�Q�[�W] 
				Power(125);
				HitTime(10, 40, 40, 18);
				// [�m�b�N�o�b�N]
				HitVel(-1.8, -4, -2.2, -3.5);
				GuardVel(-4.0, -1.5);

				P1.GuardF = 3;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// �󒆋�炢(�m��_�E��)
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;
			}
			else { DamReset(); }
			// ���}���u
			if (P1.StopTime == 0) {
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 10)) {
					// [�K�E�Z�E�Q�[�W�Z]
					SCancel();
				}
			}
			
			// 3�Q�[�W
			Delay3Button(850, 3000);
			
			break;

			//********************
			// 325�F�ł��グ
			//********************
		case 325:
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SE��炷
			if (P1.time == 1)SEStart(2);
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 11, P1.time = 0;

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 3, 0);
			}

			// �q�b�g���Z�b�g 10�t��
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(110, 0);
				// [�Q�[�W] 
				Power(132);
				HitTime(8, 30, 30, 18);
				// [�m�b�N�o�b�N]
				HitVel(-1.2, -6.5, -1.5, -4.6);
				GuardVel(-3.8, -1.5);

				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// �󒆋�炢(�m��_�E��)
				P1.HitSE = 12;
			}
			else { DamReset(); }
			// ���}���u
			if (P1.StopTime == 0) {
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 10)) {
					// [�W�����v�L�����Z��]
					if ((P1.K_Senkou[8]) && (P2.stateno >= 1000)) {		// ��s���͌������Ă݂�
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
			// 400�F�W�����vA
			//********************
		case 400:
			P1.ctrl = 0, P1.SFlg = 2;
			if (P1.time == 1)SEStart(1);
			// ��s���͏���
			if (P1.time <= 1)P1.Senkou[1] = 0;

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 1, 2);
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(45, 0);
				// [�Q�[�W] 
				Power(26);

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

			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)) {
					if (P1.Senkou[3] > 0) {		// ��s���͌������Ă݂�
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[2] > 0) {		// ��s���͌������Ă݂�
						P1.stateno = 410, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					if (P1.Senkou[1] > 0) {		// ��s���͌������Ă݂�
						P1.stateno = 400, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�󒆕K�E�Z]
					ACancel();
				}

				// [�W�����v�L�����Z��]
				if ((P1.CFlg) && (P1.time >= 1) && (P1.AirJump > 0)) {
					if (P1.K_Senkou[8] >= 1) {		// ��s���͌������Ă݂�
						P1.stateno = 45, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
				}
			}
			// �S�̃t���[���𒴂����烊�Z�b�g
			/*
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
			if (P1.time == 1) {
				PVOStart(P1.PSide, 2, 1);
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;
			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(80, 0);

				// [�Q�[�W] 
				Power(90);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 16, 18, 14);
				// [�m�b�N�o�b�N]
				HitVel(-4.6, 0, -1.6, -4.0);
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
			if (P1.time >= 23){
			P1.ctrl = 1;
			}
			*/
			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)) {
					if (P1.Senkou[3] > 0) {		// ��s���͌������Ă݂�
						// 6B
						P1.stateno = 420, P1.More = 1,
							P1.time = 0, P1.A.damage = 0;
					}
					// [�󒆕K�E�Z]
					ACancel();
				}

				// [�W�����v�L�����Z��]
				if ((P1.CFlg) && (P1.time >= 1) && (P1.AirJump > 0)) {
					if (P1.K_Senkou[8] >= 1) {		// ��s���͌������Ă݂�
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
			P1.ignoreG = true;
			// SE
			if (P1.time == 1)SEStart(3);

			if (P1.time == 0)VelSet(0, -0.2);
			if (P1.time == 11)VelSet(0, 7.6);

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 5�t��
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(116, 0);
				// [�Q�[�W] 
				Power(148);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 68, 60, 14);

				// [�m�b�N�o�b�N]
				HitVel(-1.0, -1.8, -1.2, 6.2);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.A.boundLevel = 1;
				if (P2.SFlg == 2)P1.A.boundLevel = 2;	// 2..1��܂�;
				P1.A.bound_yvel = -2.2;
				P1.HitAnim = 1000;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;
			}
			else {
				DamReset();
			}
			if (Chakuchi()) {
				P1.stateno = 421, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;
			//********************
			// 421�F�d��
			//********************
		case 421:
			P1.SFlg = 0, P1.ctrl = 0;
			P1.ignoreG = false;
			P1.D.counter = 0;

			// SE��炷
			if (P1.time == 1)SEStart(6);

			VelSet(0, 0);
			P1.YPos = GROUND;

			// �L�����Z��
			if (P1.StopTime == 0) {
				// �q�b�g���L�����Z��
				if (P2.HFlg == 1) {
					// [�Q�[�W�Z]
					HCancel();
				}
			}

			// �I��
			if (P1.time >= ANIMELEM) {
				P1.stateno = 0;
				P1.time = 0;
				P1.ctrl = 1;
			}

			break;
			//********************
			// 425�FJC2
			//********************
		case 425:
			P1.SFlg = 2, P1.ctrl = 0;
			P1.ignoreG = true;
			P1.D.counter = 0;
			P1.YVel += P1.C.yAccel;

			// �I��
			if ((P1.YPos + P1.YVel >= GROUND) && (P1.time >= 1)) {
				//P1.stateno = 421;
				P1.stateno = 47;
				P1.time = 0;
				P1.ctrl = 1;
			}

			break;
			//********************
			// 430�F�W�����vD
			//********************
		case 430:
			P1.ctrl = 0, P1.SFlg = 2;

			if (P1.time == 0) {
				P1.ignoreG = false;
				afterimageSetting(P1.PSide, 15, 155, 155, 255);
			}
			else { P1.ignoreG = true; }
			// SE
			if (P1.time == 1)SEStart(3);

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 3, 0);
			}

			P1.YVel += GRAVITY;
			if (P1.time == 0) {
				P1.YVel = -3.8;
				if (P1.XVel > P1.C.jumpF[0])P1.XVel = P1.C.jumpF[0];
				else if (P1.XVel < P1.C.jumpB)P1.XVel = P1.C.jumpB;
			}
			if (P1.YVel >= 9.0)P1.YVel = 9.0;

			// �q�b�g���Z�b�g
			if (P1.time == 1)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 5�t��
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(110, 0);
				// [�Q�[�W] 
				Power(130);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 60, 60, 16);

				// [�m�b�N�o�b�N]
				HitVel(-2.2, -3.6, -2.4, 5.2);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1000;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;
				// �o�E���h
				if (P1.HitCount == 0) {
					P1.A.boundLevel = 1;
					P1.A.bound_xvel = -1.2;
					P1.A.bound_yvel = -3.2;
				}
				else {
					P1.A.boundLevel = 1;
					P1.A.bound_yvel = -2.8;
				}
				P1.A.hosei_K = 0.9;
			}
			else {
				DamReset();
			}

			// ���n
			if (P1.YPos + P1.YVel * 2 >= GROUND) {
				P1.stateno = 421, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;

			//********************
			// 500�F�n�㓊��
			//********************
		case 500:
			P1.ctrl = 0, P1.SFlg = 0;
			// ���������ݒ�
			// ���炵�����ɑΉ����邽�߂�1F����
			if (P1.time == 1) {
				P1.throwSide = P1.muki;
				P1.A.throwTurn = false;
				if (P1.keyAtt[4]) {
					P1.A.throwTurn = true;
				}
				P1.XVel = 0;
			}

			// VO
			if ((P1.time == 9) && (P1.StopTime == 0)) {
				PVOStart(P1.PSide, 11, 0);
			}

			// SE
			if (P1.time == 1)SEStart(2);
			// �q�b�g���Z�b�g�A
			// �K�[�h���͖���
			if ((P1.time == 4) && (P2.stateno <= 999) && (P1.XPos - P2.XPos < 160 && P2.XPos - P1.XPos < 160))	// ��炢�E�󒆏�Ԃ���Ȃ�
			{
				P1.MoveHit = 1;	// �P��
				// �ǂ����������^�C�~���O�������Ȃ�A2P������
				//if ((P1.stateno == P2.stateno) && (P1.time == P2.time) && (P1.PSide == 2))
				//	P1.MoveHit = 0;
			}

			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 4) {

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
			if ((P1.CFlg) && (P1.time == 8)) {
				P1.stateno = 505;
				P1.time = 0, P2.time = 0;
				P1.More = 1;
				P1.A.damage = 0;

				P1.StopTime = 6;

				// �����̌�����ύX
				if (P1.A.throwTurn) {
					if (P1.muki == 0) {
						P1.muki = 1, P2.muki = 0;
					}
					else if (P1.muki == 1) {
						P1.muki = 0, P2.muki = 1;
					}
				}


			}
			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

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
			if (P1.time == 0) {
				int dist = 76;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + dist;
					// ��������������Ȃ�������
					if (P2.XPos > STAGE_WIDTH - GAMENHAJI) {
						P2.XPos = STAGE_WIDTH - GAMENHAJI;
						P1.XPos = P2.XPos - dist;
					}
				}
				else {
					P2.XPos = P1.XPos - dist;
					if (P2.XPos < GAMENHAJI) {
						P2.XPos = GAMENHAJI;
						P1.XPos = P2.XPos + dist;
					}
				}
				P2.YVel = 0;
				P2.YPos = GROUND;
			}
			// ��������������Ȃ�������


			// �����𗣂�
			//if (AnimElem(8))P1.XVel = 4.2;
			//if (AnimElem(9))P1.XVel = 3.2;
			// �q�b�g��
			if (P1.time == 7)P1.HitSE = 11;

			// 2P���G
			if (P1.time < 21)P2.Muteki = 1, P2.mutekiF = 0;
			
			if (P1.time > 25 && P1.time < 42)P2.Muteki = 1, P2.mutekiF = 0;


			// �q�b�g���Z�b�g
			if (P1.time == 22 || P1.time == 43)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 22) {

				// [�U������Z�b�g]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [�_���[�W]
				Damage(0, 80);
				// [�Q�[�W] 
				Power(140);
				HitTime(2, 45, 45, 0);
				// [�m�b�N�o�b�N]
				HitVel(-3.0, 0, 0, 0);
				P1.HitAnim = 1000;
				// [��炢���̕���]
				P1.fallF = 0;
				P1.HitSE = 12;
			}
			// �_���[�W�Z�b�g�A���� 1�t��
			else if (P1.time >= 43) {

				// [�U������Z�b�g]
				ExAtt(P1.PSide, 0, 100, 90, 20, -100);

				// [�_���[�W]
				Damage(0, 100);
				P1.A.hosei_K = 0.6;
				// [�Q�[�W] 
				Power(160);
				HitTime(6, 45, 45, 0);
				// [�m�b�N�o�b�N]
				HitVel(-2.6, -3.4, 0, 0);
				P1.HitAnim = 1030;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitSE = 12;
			}
			else { DamReset(); }

			// ���}���u
			if (P1.StopTime == 0) {
				// �L�����Z��
				if ((P1.CFlg) && (P1.time >= 48) && (P1.scTime > 0)) {
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
			if (P1.time == 1) {
				P1.throwSide = P1.muki;
				P1.A.throwTurn = false;
				if (P1.muki == 0) {
					if (InputPAD(104) >= 1) {
						P1.A.throwTurn = true;
					}
				}
				if (P1.muki == 1) {
					if (InputPAD(106) >= 1) {
						P1.A.throwTurn = true;
					}
				}
			}
			// SE��炷
			if (P1.time == 1)SEStart(1);
			// �q�b�g����
			// �q�b�g���Z�b�g�A
			if ((P1.time == 4))	// �����̃^�C�~���O������ĂȂ�
			{
				P1.MoveHit = 1;	// �P��
				/*
				// �ǂ����������^�C�~���O�������Ȃ�A2P������
				if ((P1.stateno == P2.stateno) && (P1.time == P2.time) && (P1.PSide == 2))
					P1.MoveHit = 0;
					*/
			}

			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 4) {

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
			if ((P1.CFlg) && (P1.time >= 7)) {
				P1.stateno = 515;
				P1.time = 0, P2.time = 0;
				P1.More = 1;
				P1.A.damage = 0;
				// �����̌�����ύX
				if (P1.A.throwTurn) {
					if (P1.muki == 0) {
						P1.muki = 1, P2.muki = 0;
					}
					else if (P1.muki == 1) {
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
			if ((P1.time == 0) && (P1.YPos + 20 >= GROUND)) {
				P1.YPos = GROUND - 20;
			}

			// �������Ƃ�
			if (P1.time == 0) {
				P1.YPos -= 2;
				P2.YPos = P1.YPos - 26;
				int dist = P2.xSize + 5;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + dist;
					// ��������������Ȃ�������
					if (P2.XPos > STAGE_WIDTH - GAMENHAJI) {
						P2.XPos = STAGE_WIDTH - GAMENHAJI;
						P1.XPos = P2.XPos - dist;
					}
				}
				else {
					P2.XPos = P1.XPos - dist;
					if (P2.XPos < GAMENHAJI) {
						P2.XPos = GAMENHAJI;
						P1.XPos = P2.XPos + dist;
					}
				}
				P1.YVel = -2.2;
				P2.YVel = -2.2;
			}
			// �d�͉��Z
			if (P1.time > 16) {
				P1.YVel += GRAVITY + 0.05;
			}
			// ���G�I��
			else if (P1.time == 16) {
				P1.YVel += GRAVITY;
				P2.YVel += GRAVITY;
				P1.YVel = -4;
			}
			// ���łɖ��G����
			else {
				P2.Muteki = 1, P2.mutekiF = 0;
				P2.time = 0;
				P1.YVel += GRAVITY / 2;
				P2.YVel += GRAVITY / 2;
			}
			// SE��炷
			if (P1.time == 15)SEStart(10);

			// VO
			if ((P1.time == 9) && (P1.StopTime == 0)) {
				PVOStart(P1.PSide, 10, 0);
			}

			// �q�b�g���Z�b�g
			if (P1.time == 16)P1.MoveHit = 1;	// �P��
			// �_���[�W�Z�b�g�A���� 1�t��
			if (P1.time == 17) {

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
				HitVel(-4.8, 9, -4.8, 9);
				P1.HitAnim = 1030;
				// [��炢���̕���]
				P1.fallF = 1;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 11;


			}
			else {
				DamReset();
			}

			// �p���ύX
			if (P1.time == 18)P2.time = 20;

			// ��������
			ThrowReject();

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.YPos + P1.YVel * 2 >= GROUND) {
				P1.time = 0, P1.stateno = 47,
					P1.YPos = GROUND, P1.YVel = 0,
					P1.ctrl = 0, P1.SFlg = 0;
			}
			break;

			//********************
			// 600 �d�j�a
			//********************
		case 600:
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷�A�ړ�
			if (P1.time == 0) {
				SEStart(3);
				P1.XVel = 0;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
				P1.Var[11] = 0;
			}

			// �K�[�|�̐F
			if ((P1.time >= 1) && (P1.time < 50)) {
				P1.colorCTime = 2;
				P1.colorC[0] = 160, P1.colorC[1] = 160, P1.colorC[2] = 255;
			}

			if(P1.time == 50){
				SEStart(31);
				// ���i�G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
				P1.Var[11] = 1;	// �^��
			}

			//
			if ((P1.K_Senkou[6] == 1) && (P1.time >= 10) && (P1.time < 50)) {
				P1.stateno = 30;
				P1.time = 0;
				P1.SFlg = 0;
				P1.More = 1;
			}
			// �o�N�X�e
			if ((P1.K_Senkou[4] == 1) && (P1.time >= 10) && (P1.time < 50)) {
				P1.stateno = 35;
				P1.time = 0;
				P1.SFlg = 0;
				P1.More = 1;
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if ((P1.time >= ANIMELEM) || 
				((P1.button[4] == 0) && (P1.time >= 14) && (P1.time < 50))) {
				P1.time = 0, P1.stateno = 601, P1.ctrl = 1;
				P1.More = 1;
			}

			break;

			//********************
			// 601 �d�j�a�h��
			//********************
		case 601:
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷�A�ړ�
			if (P1.time == 0) {
				SEStart(21);
				P1.XVel = 0;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(110, 0);
				// [�Q�[�W] 
				Power(135);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 22, 24, 18);
				// [�m�b�N�o�b�N]
				HitVel(-2.4, -3.4, -2.6, -2.6);
				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 2;
				// [�K�[�h����]
				P1.GuardF = 1;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 13;
				P1.A.wallLevel = 2;
				P1.A.wall_xvel = 0.2;
				P1.A.boundLevel = 0;

				// �ő�^��
				if (P1.Var[11] == 1) {
					Damage(170, 100);
					P1.A.hosei_K = 0.6;
					Power(380);
					HitTime(14, 38, 38, 18);
					HitVel(-1.4, -4.0, -1.6, -3.2);
					// �G�t�F�N�g
					HitEff(3, 0.8, 0.8);
					P1.HitSE = 14;
					P1.GuardF = 0;	// �K�[�s
					P1.fallF = 1;
					P1.A.wallLevel = 0;
				}
			}
			else {
				DamReset();
			}

			// �L�����Z��
			if (P1.StopTime == 0) {
				// �q�b�g���L�����Z��
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)) {
					// [�Q�[�W�Z]
					HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;

			//********************
			// 610�F����(��)
			//********************
		case 610:
			P1.ctrl = 0, P1.SFlg = 0;

			if (P1.time == 0)VelSet(0, 0);

			// SE
			if (P1.time == 1)SEStart(2);

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;
			//********************
			// 611�F����(��)
			//********************
		case 611:
			P1.ctrl = 0, P1.SFlg = 0;

			if (P1.time == 0)VelSet(0, 0);

			// SE
			if (P1.time == 1)SEStart(2);

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;


			//********************
			// 615 ����(����)
			//********************
		case 615:
			P1.ctrl = 0, P1.SFlg = 0;
			// SE��炷�A�ړ�
			if (P1.time == 1) {
				SEStart(21);
				P1.XVel = 0;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1 || P1.time == 20) {
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 0) {
				DamReset();
			}

			// �_���[�W�Z�b�g
			if (P1.time == 1) {

				// [�_���[�W]
				Damage(80, 0);
				// [�Q�[�W] 
				Power(110);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 22, 24, 18);
				// [�m�b�N�o�b�N]
				HitVel(-2.6, 0, -2.8, 0);
				P1.HitAnim = 1060;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 13;
			}
			// �_���[�W�Z�b�g
			else if (P1.time == 20) {

				// [�_���[�W]
				Damage(0, 110);
				P1.A.hosei_K = 0.6;

				// [�Q�[�W] 
				Power(210);

				HitTime(10, 100, 100, 12);
				// [�m�b�N�o�b�N]
				HitVel(-4.0, -3.6, -3.8, -3.2);

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
			if ((P1.CFlg) && (P1.time < 5) && (P2.stateno == 1060) && (P2.SFlg == 0)) {
				P2.YPos = GROUND;

				P2.XVel = 0;


				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 100;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 100;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + 100;
				}
				else if (P1.muki == 1) {
					P2.XPos = P1.XPos - 100;
				}
			}

			// �L�����Z��
			if (P1.StopTime == 0) {
				// �q�b�g���L�����Z��
				if ((P1.CFlg) && (P1.time >= 20) && (P1.scTime > 0)) {
					// [�Q�[�W�Z]
					HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;

			//********************
			// 620�F��
			//********************
		case 620:
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SE��炷
			if (P1.time == 1)SEStart(3);

			// �ʒu�����炷
			if (P1.time == 4) {
				VelAdd(6.0, 0);
			}

			// �A�[�}�[�J�E���g
			if (P1.time == 0)P1.Var[12] = P1.D.armor;

			// �q�b�g���Z�b�g
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(110, 10);
				// [�Q�[�W] 
				Power(150);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 27, 27, 23);
				// [�m�b�N�o�b�N]
				HitVel(-4.0, 0, -2.0, 3.0);
				GuardVel(-6.2, -4.0);
				// [�K�[�h����]
				P1.GuardF = 1;


				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// ���i��炢

				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;

				P1.A.boundLevel = 2;
			}
			else {
				DamReset();
			}

			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
					// [�K�E�Z�E�Q�[�W�Z]
					HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

			break;

			//********************
			// 621�F��
			//********************
		case 621:
			P1.ctrl = 0, P1.SFlg = 0;
			// ����
			if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
			// SE��炷
			if (P1.time == 1)SEStart(3);

			// �ʒu�����炷
			if (P1.time == 8) {
				VelAdd(10.0, 0);
			}

			// �A�[�}�[�J�E���g
			if (P1.time == 0)P1.Var[12] = P1.D.armor;

			// �q�b�g���Z�b�g
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(110, 10);
				// [�Q�[�W] 
				Power(150);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(10, 27, 27, 23);
				// [�m�b�N�o�b�N]
				HitVel(-4.0, 0, -2.0, 3.1);
				GuardVel(-6.2, -4.0);
				// [�K�[�h����]
				P1.GuardF = 2;


				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1020;	// ���i��炢
				P1.A.forceKurai = true;	// ��炢�p��������

				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
				P1.HitSE = 12;

				P1.A.boundLevel = 2;
			}
			else {
				DamReset();
			}

			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1)) {
					// [�K�E�Z�E�Q�[�W�Z]
					HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

			break;

			//********************
			// 625�F�O�����h
			//********************
		case 625:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;
			// ����
			if (P1.time == 0) {
				VelSet(0, 0);
				PosAdd(20, 0);
			}
			// SE��炷
			if (P1.time == 1)SEStart(3);

			// VO
			if (P1.time == 1) {
				PVOStart(P1.PSide, 6, 0);
			}

			// �Q�[�W����
			if (P1.time == 1) {
				P1.Power -= 500;
				SEStart(31);
				P1.colorCTime = 20;
				P1.colorC[0] = 255, P1.colorC[1] = 255, P1.colorC[2] = 0;
			}

			// �q�b�g���Z�b�g
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
			}

			if (P1.time == 13 - 1) {
				if (P1.muki == 0) {
					EffStartB(18, (int)P1.XPos + 140, P1.YPos, 0, -7,
						0.25, 0.06, P1.muki);
				}
				else {
					EffStartB(18, (int)P1.XPos - 140, P1.YPos, 0, -7,
						0.25, 0.06, P1.muki);
				}
			}

			// �_���[�W�Z�b�g
			if (P1.time >= 1) {

				// [�_���[�W]
				Damage(130, 10);
				// [�Q�[�W] 
				Power(0, 100);
				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(8, 24, 28, 22);
				// [�m�b�N�o�b�N]
				HitVel(-2.2, -3.4, -1.4, 3.4);
				GuardVel(-4.4, -2.4);
				// [�K�[�h����]
				P1.GuardF = 3;


				// [��炢���̕���]
				P1.fallF = 1;
				P1.HitAnim = 1030;	// ���i��炢
				P1.fall = 1;
				P1.A.boundLevel = 3;
				P1.HitSE = 12;
			}
			else {
				DamReset();
			}

			// �L�����Z��
			// ���}���u
			if (P1.StopTime == 0) {
				if ((P1.CFlg) && (P1.time >= 1) && (P1.scTime > 0)) {
					// [�K�E�Z�E�Q�[�W�Z]
					HCancel();
				}
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.ctrl = 1, P1.More = 1, P1.stateno = 0, P1.time = 0;

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
			if (P1.time == 0) {
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

			if (AnimElem(20)) {
				SEStart(22);
			}

			// 3456 9
			// �O�i
			if (AnimElem(27)) {
				P1.XVel = 10.0;
			}

			// �q�b�g���Z�b�g	14�t���`
			if (AnimElem(2) || AnimElem(6) ||
				AnimElem(10) || AnimElem(13) ||
				AnimElem(17) || AnimElem(30)) {
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g

			if (P1.time < 1) {
				DamReset();
			}
			if ((P1.time >= 1) && (P1.time < 24)) {

				// [�_���[�W]
				Damage(60, 4);
				// [�Q�[�W] 
				Power(0, 70);
				HitTime(0, 60, 60, 22);
				// [�m�b�N�o�b�N]
				HitVel(-3.2, 0, -1.4, -2.0);
				// �ꔭ��
				if (P1.time < 14) {
					if (P2.SFlg != 2) {
						P1.HitAnim = 1060;	// �n��
					}
					else if (P2.SFlg == 2) {
						P1.HitAnim = 1060;	// �n���
					}
				}
				else {
					P1.HitAnim = 1000;	// �n��
				}
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
				P1.A.kill = 0;
			}
			// �_���[�W�Z�b�g
			else if (P1.time >= 100) {

				// [�_���[�W]
				Damage(35, 40);
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
				P1.A.kill = 1;
				// �n�k�G�t�F�N�g
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}

			// ����������ύX
			if ((P1.CFlg) && (P1.time < 16) && (P2.stateno == 1060) && (P2.SFlg == 0)) {
				P2.YPos = GROUND;

				P2.XVel = 0;

				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 100;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 100;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + 100;
				}
				else if (P1.muki == 1) {
					P2.XPos = P1.XPos - 100;
				}

			}

			// ���� �� �S�̃t���[���A54�𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM) {
				P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
			}

			break;

			//********************
			// 810�F�����g
			//********************
		case 810:
			P1.ctrl = 0, P1.SFlg = 0;

			if (P1.time == 0) {
				VelSet(0, 0);
				P1.Power -= 1000;
			}

			// SE
			if (P1.time == 1)SEStart(2);

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

			break;

			//********************
			// 811 �����g(����)
			//********************
		case 811:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = 1;

			// �Q�[�W����
			if (P1.time == 0) {
				S.StopTime = 10;
				S.Anten = 25;
				SEStart(25);
				// ���i�G�t�F�N�g
				EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.25, 0.25, P1.PSide);
			}

			// SE��炷�A�ړ�
			if (P1.time == 1) {
				SEStart(21);
				//P1.XVel = 10;
				P1.XPos = P2.XPos;
				PosAdd(98, 0);
			}
			// �q�b�g���Z�b�g
			if (P1.time == 1) {
				P1.MoveHit = 1;	// �P��
				// �G�t�F�N�g
				EffStart(4, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize - (P1.GSize * 29.0), 0, 0, 7.0, 0.8, P1.PSide);
			}

			if (P1.time == 0) {
				DamReset();
				P1.touchF = false;
			}

			// ����U��
			if (P1.time == 45)SEStart(27);

			// �ڐG����
			if (P1.time == 10)P1.touchF = true;

			// �_���[�W�Z�b�g
			if (P1.time == 1) {

				// [�_���[�W]
				Damage(260, 30);
				// [�Q�[�W] 
				Power(0, 420);

				HitTime(16, 100, 100, 10);
				// [�m�b�N�o�b�N]
				HitVel(0.0, -5.6, 0.0, -5.2);

				P1.HitAnim = 1030;	// �󒆋�炢
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 14;
				// �G�t�F�N�g
				HitEff(0, 0.8, 0.8);
				// �n�k�G�t�F�N�g
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}

			// �����ύX
			if((P1.time >= 1 && P1.time <= 4) && (P2.stateno >= 1000) && (P1.muki != P2.muki) 
				&& (P1.XPos > GAMENHAJI && P1.XPos < STAGE_WIDTH - GAMENHAJI)){
				if (P2.muki == 0)P2.muki = 1;
				else { P2.muki = 0; }
			}

			// �S�̃t���[���𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

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
			}

			// SE��炷
			if (P1.time == 1) {
				SEStart(21);
			}

			if (P1.time == 1) {
				P1.MoveHit = 1;
			}
			if (P1.time == 28) {
				P1.MoveHit = 0;
			}

			// �_���[�W�Z�b�g
			if ((P1.time >= 1) && (P1.time < 28)) {

				// [�_���[�W]
				Damage(100, 0);
				// [�Q�[�W] 
				Power(0, 100);
				HitTime(4, 24, 24, 14);
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

			// ����������ύX
			if ((P1.CFlg) && (P1.time >= 1) && (!P1.ignoreG) && (P2.stateno >= 1000) && (P2.YPos != GROUND)) {
				P2.YPos = GROUND;
				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 50;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 50;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + 50;
				}
				else if (P1.muki == 1) {
					P2.XPos = P1.XPos - 50;
				}
			}


			// �����L�b�N��
			if ((P1.CFlg) && (P2.HFlg == 1) && (P1.time > 0)) {
				P1.time = 0, P1.stateno = 851, P1.ctrl = 0;
				P1.More = 1;
				P1.A.damage = 0;
			}

			// �I��
			if (P1.time >= ANIMELEM) {
				P1.time = 0, P1.stateno = 0, P1.ctrl = 0;
				
			}

			break;

			//********************
			// 851 3�Q�[�W�q�b�g
			//********************
		case 851:
			P1.ctrl = 0, P1.SFlg = 0;
			P1.A.gaugeHosei = true;

			// ���G
			if (P1.time <= 9)P1.Muteki = 1, P1.mutekiF = 0;
			else { P1.Muteki = 0; }

			// 3456 9
			// �O�i
			if (AnimElem(4) || AnimElem(6) ||
				(AnimElem(9) || (AnimElem(12)))) {
				P1.XVel = 2.2;
			}
			if (AnimElem(20)) {
				P1.XVel = 6.0;
			}

			// �q�b�g���Z�b�g	14�t���`
			if (AnimElem(3) || AnimElem(6) ||
				AnimElem(9) || AnimElem(12) ||
				AnimElem(14)) {
				P1.MoveHit = 1;	// �P��
			}
			// �_���[�W�Z�b�g

			if (P1.time == 0) {
				DamReset();
			}
			if ((P1.time >= 1) && (P1.time < 30)) {

				// [�_���[�W]
				Damage(90, 10);
				// [�Q�[�W] 
				Power(0, 90);
				HitTime(0, 60, 60, 22);
				// [�m�b�N�o�b�N]
				HitVel(-3.4, 0, -0.4, -2.0);
				// �ꔭ��
				if (P1.time < 14) {
					if (P2.SFlg != 2) {
						P1.HitAnim = 1060;	// �n��
					}
					else if (P2.SFlg == 2) {
						P1.HitAnim = 1060;	// �n���
					}
				}
				else {
					P1.HitAnim = 1000;	// �n��
				}
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				P1.HitSE = 13;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
				P1.A.kill = 0;

			}
			// �_���[�W�Z�b�g
			else if (P1.time >= 58) {

				// [�_���[�W]
				Damage(80, 20);
				// [�Q�[�W] 
				Power(0, 100);
				HitTime(5, 60, 60, 22);
				// [�m�b�N�o�b�N]
				HitVel(-0.6, -7.4, -0.8, -7.2);
				P1.A.yaccel = 0.16;	// ��������
				P1.HitAnim = 1030;	// ��
				// [��炢���̕���]
				P1.fallF = 1;
				// [�K�[�h����]
				P1.GuardF = 1;
				

				P1.HitSE = 13;
				// �G�t�F�N�g
				HitEff(3, 0.6, 0.6);
				P1.A.kill = 0;

			}

			// ����������ύX
			if ((P1.CFlg) && (P1.time < 16) && (P2.stateno == 1060) && (P2.SFlg == 0)) {
				P2.YPos = GROUND;

				P2.XVel = 0;

				if (P1.XPos <= GAMENHAJI)P1.XPos = GAMENHAJI + 100;
				if (P1.XPos >= STAGE_WIDTH - GAMENHAJI)P1.XPos = STAGE_WIDTH - GAMENHAJI - 100;
				if (P1.muki == 0) {
					P2.XPos = P1.XPos + 100;
				}
				else if (P1.muki == 1) {
					P2.XPos = P1.XPos - 100;
				}

			}

			// ���� �� �S�̃t���[���A54�𒴂����烊�Z�b�g
			if (P1.time >= ANIMELEM) {
				P1.time = 0, P1.stateno = 852, P1.ctrl = 0;
			}

			break;

			//********************
			// 852�F�W�����vC
			//********************
		case 852:
			P1.ctrl = 0, P1.SFlg = 2;
			P1.A.gaugeHosei = true;
			P1.ignoreG = true;

			// SE
			if (P1.time == 47)SEStart(3);

			if (P1.time == 1)VelSet(0.0, -10.2);
			if (P1.time == 15) {
				VelSet(0, 0);
			}
			if (P1.time == 56)VelSet(0, 8.6);

			// �q�b�g���Z�b�g
			if (P1.time == 1 || P1.time == 56)P1.MoveHit = 1;	// �P��

			// �_���[�W�Z�b�g
			if (P1.time >= 1 && P1.time < 47) {

				// [�_���[�W]
				Damage(25, 40);
				// [�Q�[�W] 
				Power(0, 80);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(5, 68, 60, 14);

				// [�m�b�N�o�b�N]
				HitVel(0.0, -1.8, 0.0, 6.2);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.A.boundLevel = 3;	// 2..1��܂�
				P1.A.bound_yvel = -6.2;
				P1.A.bound_xvel = 0.0;
				P1.HitAnim = 1030;
				// �G�t�F�N�g
				HitEff(1, 0.6, 0.6);
				P1.HitSE = 12;
				P1.A.kill = 0;
				// �n�k�G�t�F�N�g
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}
			else if (P1.time >= 56) {

				// [�_���[�W]
				Damage(25, 100);
				// [�Q�[�W] 
				Power(0, 120);

				// [�q�b�g�X�g�b�v�E�̂����莞��]
				HitTime(12, 68, 60, 14);

				// [�m�b�N�o�b�N]
				HitVel(-1.0, -1.8, -1.2, 6.2);
				P1.GuardF = 1;
				// [��炢���̕���]
				P1.fallF = 1;
				P1.A.boundLevel = 1;	// 2..1��܂�
				P1.A.bound_yvel = -2.2;
				P1.HitAnim = 1030;
				// �G�t�F�N�g
				HitEff(3, 1.0, 1.0);
				P1.HitSE = 14;
				P1.A.kill = 1;
				// �n�k�G�t�F�N�g
				P1.A.quakeTime = 2;
				P1.A.quakeY = 2;
			}
			else {
				DamReset();
			}
			if (Chakuchi()) {
				P1.stateno = 421, P1.More = 1, P1.time = 0;
				P1.SFlg = 0;
				VelSet(0, 0);
				P1.ignoreG = false;
			}

			break;

			//===============================//
		}// switch�I��

		// �X�e�[�g�ԍ���ۑ�
		P1.OldState = P1.stateno;
		P1.oldState2 = P1.State2;

	}// P1�����I��


	for (int i = 0; i < HM; i++) {
		{
			//********************
			//
			// �w���p�[
			//
			//********************

			// �I����������X�e�[�g�ɔ�΂�
			if (H1[i].var) {
				switch (H1[i].stateno) {
					//********************
					// 2000 �u���ꂽ��u�� 
					//********************
				case 2000:

					//***************
					//  ��{�ݒ�
					//***************	
					if (H1[i].time == 0) {
						H1[i].muki = P1.muki;
						H_PosSet(i, -80, 0);
					}
					//***************
					//  �s�����e
					//***************

					// �S�̃t���[��33or��_��
					if ((H1[i].time > 33) ||
						(P1.stateno >= 1000)) {
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
void GetP_SyuParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_SyuParam(System_t GS)
{
	S = GS;
}

void GetH_SyuParam(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++) {
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
	if ((P1.Senkou[4] > 0) && (P1.stateno != 600) && (P1.Var[14] == 0)
		&& (P1.SFlg != 2)) { //&& (P1.D.armor == 0 || (P1.stateno == 225 || P1.stateno == 320))){		// ��s���͌������Ă݂�
		{
			P1.stateno = 600, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	// 236 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[1])) {		// ��s���͌������Ă݂�
		P1.stateno = 620, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		if (P1.Senkou[2] > 0 || P1.Senkou[3] > 0)P1.stateno = 621;
	}

	// 214 + AorB
	if (((P1.Senkou[1] > 0) || (P1.Senkou[2] > 0) || (P1.Senkou[3] > 0))
		&& (P1.cmd[2])) {		// ��s���͌������Ă݂�
		P1.stateno = 610, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
		if (P1.Senkou[2] > 0 || P1.Senkou[3] > 0)P1.stateno = 611;
	}
	/*
	// 236 + C
	if ((P1.Senkou[3] > 0) && (P1.cmd[1]) && (P1.Power >= 500)) {		// ��s���͌������Ă݂�
		P1.stateno = 625, P1.More = 1,
			P1.time = 0, P1.A.damage = 0;
	}
	*/
	HCancel();

	//�Ō�ɔ��������
	DamReset();
}

// �K�E�Z�L�����Z��
void HCancel()
{
	//  236+C		[��i�؂�]
	if ((P1.Senkou[3] > 0) && (P1.cmd[1])
		&& (P1.Power >= 1000)) {
		if (P1.SFlg != 2) {		// �n��
			P1.stateno = 800, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
		}
	}
	//  214+C		[�����g]
	if ((P1.Senkou[3] > 0) && (P1.cmd[2])
		&& (P1.Power >= 1000)) {
		if (P1.SFlg != 2) {		// �n��
			P1.stateno = 811, P1.More = 1,
				P1.time = 0, P1.A.damage = 0;
			P1.Power -= 1000;
		}
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

}
