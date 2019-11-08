//==================================================================================
//
//			���l�v�Z�̊Ǘ���
//
//==================================================================================
#pragma warning(error:4456)

//�C���N���[�h��--------------------------------------------
#include "pch.h"

//�f�t�@�C����----------------------------------------------


//�ް���`��------------------------------------------------

static double hosei;
static int getPower;	// �Q�[�W����A��������ƌ���
static int gTime;	// �K�[�|�A���g�̕ۑ�����

//�����֐��錾��--------------------------------------------

static void GuardPoint(int i);
static boolean GuardCheck(int n);
static void GuardParam(int i);
static void HitParam(int i);

//�v���O������----------------------------------------------

void HelperDamCheck()
{

	// 1P�U��
	for (int i = 0; i < HM; i++){
		for (int j = 0; j < ATT_MAX; j++){
			for (int k = 0; k < HIT_MAX; k++){
				//�����蔻��`�F�b�N�A�R�s�y�� > �̕������C�R�[���ɂ���
				if(Box_Touch(H1[i].XAtt[j], H1[i].YAtt[j], H1[i].WAtt[j], H1[i].HAtt[j],
					P2.XHit[k], P2.YHit[k], P2.WHit[k], P2.HHit[k]) == true)
				{
					// �A�[�}�[�A���K�E�ȊO�͎󂯂Ȃ��A�n��
					if ((H1[i].HMoveHit > 0) && (P2.D.armor > 0) && (H1[i].attF != 2)
						&& (P2.SFlg != 2)){

						//---------------
						// ���ʐݒ�
						//---------------
						if (GuardCheck(i))
							// [�K�[�h�̐ݒ�]
						{
							P2.time = 0;
							P2.StopTime = 0;
							GuardParam(i);
						}
						// [�q�b�g�̐ݒ�]
						else
						{
							// [1P�̃q�b�g�m�F�p]
							H1[i].HMoveHit--;
							// [�X�p�L�����P�\]
							P1.scTime = SC_TIME;
							P1.A.ncTime = NC_TIME;
							// [�_���[�W�␳]
							P2.Life -= H1[i].A.damage;

							P2.colorCTime = 8;
							P2.colorC[0] = 255, P2.colorC[1] = 0, P2.colorC[2] = 0;

							// [�Q�[�W���]
							P1.Power += H1[i].HGetPow;
							P2.Power += H1[i].HGivePow;

							// [�q�b�gSE]
							SEStart(H1[i].HHitSE);

							// [�q�b�g�G�t�F�N�g]
							int posX;
							{ posX = 20 + GetRand(10); }
							EffStart(H1[i].hitEff, (int)P2.XPos, (int)P2.YPos, posX, -(P2.ySize / 1.5),
								H1[i].heSizeX, H1[i].heSizeY, P2.muki);

							// [�q�b�g�X�g�b�v�A�̂����莞��]
							P2.StopTime = H1[i].HHitStop;
							H1[i].stopTime = H1[i].HSSelf;
							if (P2.SFlg != 2){
								P2.D.nokezori = H1[i].HG_HitTime;
							}
							else{ P2.D.nokezori = H1[i].HA_HitTime; }

							// �A�[�}�[����
							P2.D.armor--;
						}

						
					}
					// P1 �U���q�b�g�`�F�b�N
					// �q�b�g�t���O���I�t�ŁA���肪���G����Ȃ�
					// �X�C�b�`���I��
					else if ((H1[i].HMoveHit > 0) && (H1[i].var) &&
						(
						(P2.Muteki == 0) ||
						(P2.Muteki > 0 && P2.mutekiF == 1 && H1[i].attF != 1) ||
						(P2.Muteki > 0 && P2.mutekiF == 2 && H1[i].attF != 2) ||
						(P2.Muteki > 0 && P2.mutekiF == 3 && H1[i].attF != 3)
						)
					   )
					{
						// �K�[�|�E���g�p
						gTime = P2.time;

						//---------------
						// ���ʐݒ�
						//---------------
						P2.time = 0;
						P2.StopTime = 0;
						//***************
						//
						// �q�b�gor�K�[�h
						//
						//***************
						// �K�[�h�̐ݒ�
						if (GuardCheck(i))
						// [�K�[�h�̐ݒ�]
						{
							if ((P2.Name == SYUICHI) && ((P2.stateno >= 600) && (P2.stateno < 820))) {
								if (P2.stateno == 600) {
									P2.time = gTime;
									GuardPoint(i);
								}
							}
							else { GuardParam(i); }
						}
						// [�K�[�h���ݒ�I��]
						// [�q�b�g�̐ݒ�]
						else
						{
							HitParam(i);
						
						}// �q�b�g���ݒ�I��
						
						// ���ʐݒ�
						// [�_���[�W���ɖ߂�]
						H1[i].A.damage = 0;

					}// �q�b�gor�K�[�h���f�I��

				}// if�`�F�b�N�I��
			}// 2P ��炢����`�F�b�N
		}// �U������`�F�b�N
	}// �w���p�[�`�F�b�N


	//********************
	// �w���p�[���m�̔���
	//********************

	for (int i = 0; i < HM; i++){
		for (int l = 0; l < HM; l++){
			for (int j = 0; j < ATT_MAX; j++){
				for (int k = 0; k < HIT_MAX; k++){
					//�����蔻��`�F�b�N�A�R�s�y�� > �̕������C�R�[���ɂ���
					if (Box_Touch(H1[i].XAtt[j], H1[i].YAtt[j], H1[i].WAtt[j], H1[i].HAtt[j],
						H2[l].XAtt[k], H2[l].YAtt[k], H2[l].WAtt[k], H2[l].HAtt[k]) == true)
					{

						// P1 �U���q�b�g�`�F�b�N
						// �q�b�g�t���O���I�t�ŁA���肪���G����Ȃ�
						// �X�C�b�`���I��
						// ���݂���ѓ���
						if (
							(H1[i].HMoveHit > 0) && (H1[i].var) && (H1[i].attF == 3) &&
							(H2[l].HMoveHit > 0) && (H2[l].var) && (H2[l].attF == 3)
							)
						{

							// [�q�b�g�̐ݒ�]

							// [�T�E���h]
							SEStart(H1[i].HHitSE);
							// [�q�b�g�G�t�F�N�g]
							int dPos, dist;
							if (H1[i].XPos > H2[l].XPos){
								dist = (int)((H1[i].XPos - H2[l].XPos) / 2);
								dPos = H1[i].XPos - dist;
							}
							else{ 
								dist = (int)((H2[l].XPos - H1[i].XPos) / 2); 
								dPos = H2[l].XPos - dist;
							}
							EffStart(1, dPos, (int)H1[i].YPos, 0, -25,
								0.5, 0.5, false);

							// ���ʐݒ�
							// [1P�̃q�b�g�m�F�p]
							H1[i].HMoveHit--, H2[l].HMoveHit--;
							// [�_���[�W���ɖ߂�]
							H1[i].A.damage = 0, H2[l].A.damage = 0;
							// [�X�p�L�����P�\]
							//P1.scTime = SC_TIME;

						}// �q�b�g���ݒ�I��

						
					}// if�`�F�b�N�I��
				}// 2P ��炢����`�F�b�N
			}// �U������`�F�b�N
		}// 2P �w���p�[
	}// �w���p�[�`�F�b�N


	// �Ō�ɒl��n��
	if (P1.PSide == 1){
		Get_PSet(P1, P2);
		Get_HSet(H1, H2);
	}
	if (P1.PSide == 2){
		Get_PSet(P2, P1);
		Get_HSet(H2, H1);	
	}
	Get_SSet(S);

}

void GuardPoint(int i)
{
	// [�Q�[�W���]
	P1.Power += H1[i].HGetPow / 2;
	P2.Power += H1[i].HGivePow / 2;

	// SE
	if (P2.D.nokezori >= 18)SEStart(17);
	else { SEStart(16); }

	// [�q�b�g�G�t�F�N�g]
	int posX, posY;
	int defPosY;	// ��ʒu
	defPosY = (int)P2.YPos;

	if (P1.YPos < P2.YPos - 50)defPosY = (int)P2.YPos - 50;

	//P1.GuardF; �K�[�h�����ō��W������
	if ((P1.GuardF == 3 || P1.GuardF == 13) && (defPosY == (int)P2.YPos)) {
		posY = P2.ySize / 2;
	}
	else { posY = P2.ySize; }

	// [1P�̃q�b�g�m�F�p]
	H1[i].HMoveHit = 0;

	// �G�t�F�N�g�J�n
	{ posX = 20 + GetRand(10); }
	{
		EffStart(11, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			0.2, 0.6, P2.muki);
	}

	// �q�b�g�X�g�b�v�A�̂����莞��
	H1[i].stopTime = H1[i].HSSelf;

	if (P2.StopTime < 0)P2.StopTime = 0;

	// [�K�[�h���J�E���g]
	P1.A.guardCount += 1;	// �J�E���g
}

void GuardParam(int i)
{

	// [ �U����� ]
	{
		if (P2.XPos < P1.XPos){
			if (P2.muki == 1)P2.muki = 0;
		}
		if (P1.XPos < P2.XPos){
			if (P2.muki == 0)P2.muki = 1;
		}
	}

	// [�Q�[�W���]
	P1.Power += H1[i].HGetPow / 2;
	P2.Power += H1[i].HGivePow / 2;

	// [�_���[�W�v�Z]
	P2.Life -= (H1[i].A.damage + H1[i].A.hosyo) / 10;
	if ((P2.Life <= 0) && (P2.aGauge > 0))P2.Life = 1;	// ���E���h�~

	// [�K�[�h�Q�[�W����]
	//if (P2.SFlg == 2)P2.aGauge -= H1[i].A.damage / 8;	// �󒆃K�[�h�͑���
	//else{ P1.aGauge -= H1[i].A.damage / 10; }
	
	boolean gotoCrash = false;
	// [�K�[�h�Q�[�W����]

	if (P2.aGauge > 0 && P2.SFlg == 2) 
	{
		int n = 0;
		// ���ʌ���
		if (P2.SFlg == 2){
			n = H1[i].A.damage + H1[i].A.hosyo + 20;
		}
		else{
			//n = (int)((float)(H1[i].A.damage + H1[i].A.hosyo) * 0.55);
		}

		// ���i
		if (P1.A.guardCount == 0){
			if (n > 250)n = 250;
			else if (n < 50)n = 50;
		}
		// �K�[�h��
		else{
			switch (P1.A.guardCount){
			case 1:
				n = ((float)n * 0.95);  break;
			case 2:
				n = ((float)n * 0.90);  break;
			default:
				n = ((float)n * 0.85);  break;
			}
			// �ő�l����
			if (n > 200)n = 200;
			else if (n < 20)n = 20;
		}
		// ���l�ύX
		P2.aGauge -= n;
		TestText(n, 1);

		// �N���b�V��������
		if (P2.aGauge <= 0){
			gotoCrash = true;
			P1.A.guardCount = 0;
			H1[i].H_GX = -2.0;
			H1[i].H_AX = -1.1;
			H1[i].H_GY = 0.0;
			H1[i].H_AY = -5.0 / 2.0;
			P2.D.yaccel = 0.14;
		}
	}

	// �K�[�N���ł͂Ȃ�
	if (!gotoCrash){
		// [�A�j���ݒ�]
		// �d���Ɉڍs
		if (P2.SFlg == 0)P2.stateno = 50;
		if (P2.SFlg == 1 || (P2.keyAtt[2] > 0))P2.stateno = 51;
		if (P2.SFlg == 2)P2.stateno = 52;

		// [�̂����莞��]
		P2.D.nokezori = H1[i].HG_GuardTime;
		P2.StopTime = H1[i].HHitStop;
	}
	// �K�[�N��
	else{
		// �d���Ɉڍs
		if (P2.SFlg == 0)P2.stateno = 1000;
		if (P2.SFlg == 1)P2.stateno = 1020;
		if (P2.SFlg == 2)P2.stateno = 1030;

		// �K�[�h����
		P2.D.nokezori = 70;
	}

	H1[i].stopTime = H1[i].HSSelf;

	// [�T�E���h]
	SEStart(H1[i].HGuardSE);
	// [�q�b�g�G�t�F�N�g]
	int posX, posY;
	int defPosY;	// ��ʒu
	defPosY = (int)P2.YPos;

	if (P1.YPos < P2.YPos - 50)defPosY = (int)P2.YPos - 50;

	//P1.GuardF; �K�[�h�����ō��W������
	if (H1[i].HGuardF == 3 || H1[i].HGuardF == 13){
		posY = P2.ySize / 2;
	}
	else{ posY = P2.ySize; }
	{ posX = 20 + GetRand(10); }

	EffStart(11, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
		0.2, 0.6, P2.muki);

	// [1P�̃q�b�g�m�F�p]
	H1[i].HMoveHit = 0;

	// [�m�b�N�o�b�N����]
	if (((P2.XPos <= GAMENHAJI) || (P2.XPos >= (STAGE_WIDTH - GAMENHAJI))) &&
		(P1.SFlg != 2)){
		// �󒆂ŋ����Ă���
		if (P2.SFlg == 2){
			P2.XVel = H1[i].H_AX, P2.YVel = H1[i].H_AY;
		}
		else{ P2.XVel = H1[i].H_GX, P2.YVel = H1[i].H_GY; }
	}
	else{
		// �󒆂ŋ����Ă���
		if (P2.SFlg == 2)P2.XVel = H1[i].H_AX, P2.YVel = H1[i].H_AY;
		else{ P2.XVel = H1[i].H_GX, P2.YVel = H1[i].H_GY; }
	}
	
	if (!gotoCrash){
		// ��K����
		if (P2.SFlg == 2){
			if (P2.YVel < -1.2)P2.YVel = -1.2;
			else if (P2.YVel > -0.2)P2.YVel = -0.2;

			if (P2.XVel < -2)P2.XVel = -2;
		}
	}
	// [�K�[�h���J�E���g]
	if (!gotoCrash)
		P1.A.guardCount += 1;	// �J�E���g

	// [�X�p�L�����P�\]
	P1.scTime = SC_TIME;

	// �g�����\���p
	AttackSpeed(P1.time);
	Frame_Check();
}

void HitParam(int i)
{
	// [�Q�[�W���]
	// �Q�[�W�␳�����邩�A����������Ȃ�A�Q�[�W���������炷
	if (P1.A.gaugeHosei || getPower >= 1000){
		P1.Power += H1[i].HGetPow / 5;
		if (P1.stateno != 520)P1.Power += P1.addG;
	}
	else if (getPower >= 500){
		P1.Power += (int)(H1[i].HGetPow * 0.8);
		if (P1.stateno != 520)P1.Power += P1.addG;
	}
	else{ 
		P1.Power += H1[i].HGetPow; 
		if (P1.stateno != 520)P1.Power += P1.addG * 5;
	}
	P1.addG = 0;

	// ����Q�[�W
	if (getPower >= 500){
		P2.Power += (int)(H1[i].HGivePow * 0.8);
	}
	else{ P2.Power += H1[i].HGivePow; }

	// �Q�[�W����ʌv��
	if (P1.HitCount == 0){
		getPower = 0;
	}
	getPower += P1.GetPow;


	hosei = (P1.A.cl_max - ((P1.HitCount - 1) * P1.A.comboLate)) * P1.A.hosei_K;

	// [�_���[�W�␳]
	if (P1.HitCount == 0){	// 1Hit
		P2.Life -= H1[i].A.damage;
		// �J�E���^�[�q�b�g
		if (P2.D.counter){
			// �t�F�C�^���ǉ��_��
			if (P2.D.fatal)P2.Life -= 20;
			P2.Life -= (int)H1[i].A.damage * 0.2;
			if (H1[i].HHitStop > 0)H1[i].HHitStop += 2;
			if (H1[i].HHitStop > 0){
				H1[i].HG_HitTime += 2;
				H1[i].HA_HitTime += 2;
			}
			P2.colorCTime = 5;
			P2.colorC[0] = 255, P2.colorC[1] = 0, P2.colorC[2] = 0;
		}
	}
	else if (P1.HitCount >= 1){
		// �_�������肷��
		if (hosei < P1.A.cl_min){
			P2.Life -= (int)(H1[i].A.damage *P1.A.cl_min);
		}
		else{
			P2.Life -= (int)(H1[i].A.damage *hosei);
		}
	}
	// [�ۏ�_���[�W]
	P2.Life -= H1[i].A.hosyo;



	// [�A�j���ݒ�]
	// �A�j���ύX�Ȃ��ꍇ���^�C����0��
	P2.time = 0;
	// ���b�N�Z�͂��̂܂ܓK�p
	if ((H1[i].HHitAnim >= 1060) && (H1[i].HHitAnim <= 1069)){
		P2.stateno = H1[i].HHitAnim;
	}
	// ���ʂ̋�炢
	else{
		// �󒆋�炢�𔻒f
		if ((P2.SFlg == 2) || (H1[i].H_GY < 0)){
			P2.stateno = 1030;
		}
		// ���ʂ̋�炢�ŗ���
		else if (P2.SFlg == 0){
			if (H1[i].HHitAnim == 1000)P2.stateno = 1000;
			else{ P2.stateno = 1010; }
		}
		// ���ʂ̋�炢�ł��Ⴊ��
		else if (P2.SFlg == 1){
			P2.stateno = 1020;
		}
	}


	// [�T�E���h]
	SEStart(H1[i].HHitSE);
	// [�q�b�g�G�t�F�N�g]
	int posX, posY;

	//P1.GuardF; �K�[�h�����ō��W������
	if (H1[i].HGuardF == 3 || H1[i].HGuardF == 13){
		posY = P2.ySize / 2;
	}
	else{ posY = P2.ySize; }

	{ posX = 20 + GetRand(10); }
	EffStart(10, (int)P2.XPos, (int)P2.YPos, posX, -(posY / 1.5),
		0.2, 0.2, P2.muki);
	for (int i = 0; i < 6; i++){
		float rand = GetRand(4);
		rand = (rand * 0.01) + 0.07;
		EffStart(9, (int)P2.XPos, (int)P2.YPos, posX, -(posY / 1.5),
			rand, rand, P2.muki);
	}
	EffStart(H1[i].hitEff, (int)P2.XPos, (int)P2.YPos, posX, -(posY / 1.5),
		H1[i].heSizeX, H1[i].heSizeY, P2.muki);
	
	// [�m�b�N�o�b�N����]
	if (((P2.XPos <= GAMENHAJI) || (P2.XPos >= (STAGE_WIDTH - GAMENHAJI))) &&
		(P1.SFlg != 2)){
		// �󒆂ŋ����Ă���
		if (P2.SFlg == 2){

			P2.XVel = H1[i].H_AX;
			P2.YVel = H1[i].H_AY;
			// �q�b�g�������ŏd��
			//if (P1.HitCount > 20){
				//P2.YVel = (H1[i].H_AY + ((float)0.25 * (P1.HitCount - 10)));
			//}
			//else { P2.YVel = H1[i].H_AY; }


		}
		else{ P2.XVel = H1[i].H_GX, P2.YVel = H1[i].H_GY; }
	}
	else{
		// �󒆂ŋ����Ă���
		if (P2.SFlg == 2){
			P2.XVel = H1[i].H_AX;
			/*
			// �q�b�g�������ŏd��
			if (P1.HitCount > 20){
			P2.YVel = (H1[i].H_AY + ((float)0.25 * (P1.HitCount - 10)));
			}
			*/
			//else
			{ P2.YVel = H1[i].H_AY; }

		}
		else{ P2.XVel = H1[i].H_GX, P2.YVel = H1[i].H_GY; }
	}

	// [���b�N���̓q�b�g�o�b�N�Ȃ�]
	if ((P2.stateno >= 1060) && (P2.stateno <= 1069)){
		P2.XVel = 0;
		P2.YVel = 0;
	}

	// [�d��]
	P2.D.yaccel = GRAVITY;

	// [�̂����莞��]
	if (P2.SFlg != 2){
		P2.D.nokezori = H1[i].HG_HitTime;
	}
	else{ P2.D.nokezori = H1[i].HA_HitTime; }
	P2.StopTime = H1[i].HHitStop;
	H1[i].stopTime = H1[i].HSSelf;

	P2.fall = H1[i].fallF;

	// [�X�p�L�����P�\]
	P1.scTime = SC_TIME;

	// [�A�[�}�[�폜]
	if (P2.D.armor > 0)P2.D.armor = 0;

	// [�q�b�g���J�E���g]
	if (H1[i].A.damage > 0)
		P1.HitCount += 1;

	// [1P�̃q�b�g�m�F�p]
	H1[i].HMoveHit = 0;
	
	// [2P�̃R���g���[����D��]
	P2.ctrl = 0;

	// [�o�E���h�ݒ�]
	P2.D.bound_xvel = -1.3;
	P2.D.bound_yvel = -3.0;
	P2.D.wall_xvel = 0.4;
	P2.D.wall_yvel = -6.4;

	// ���o�E���h
	P2.D.bound = H1[i].A.boundLevel;
	if (P2.D.bound > 2)P2.D.bound = 2;

	if (P2.D.boundCount > 0){
		// �S�ď��o�E���h
		if ((H1[i].A.boundLevel > 0) && (H1[i].A.boundLevel != 3)){
			P2.D.bound = 1;
		}
	}
	// ���x�߂�
	if (P2.D.bound == 0){
		P2.D.bound_xvel = -1.3;
		P2.D.bound_yvel = -3.0;
	}

	// �ǃo�E���h
	// �傫��������Ȃ�
	if (P2.D.wall <= 2){
		P2.D.wall = H1[i].A.wallLevel;
	}
	// ���x�߂�
	if (P2.D.wall == 0){
		P2.D.wall_xvel = -0.4;
		P2.D.wall_yvel = -6.4;
	}

	// �o�E���h�Ƃ肠����0
	//P2.D.bound = 0;
	
	// �g�����\���p
	AttackSpeed(P1.time);
	Frame_Check();
	Beat_Check(P2.OldState, P1.PSide);

}


boolean GuardCheck(int n)
{
	// �g�����K�[�h�K�p
	if ((S.TraningFlg) && (S.TSwitch[4] > 0) && (S.TSwitch[2] < 3) && (P1.PSide == 1)){
		if ((S.TSwitch[4] == 1) ||
			((S.TSwitch[4] == 2) && (GetRand(1) == 0))){
			P2.keyAtt[4] = 1;
			if ((H1[n].HGuardF == 3) || (H1[n].HGuardF == 13))P2.keyAtt[2] = 1;
		}
	}

	boolean gu = false;
	boolean airGuard = false;

	// �����������Ȃ�6���͂ł��󒆃K�[�h�\
	if ((P2.SFlg == 2) && (P2.keyAtt[6] != 0) && (P2.aGauge > 0)){
		if (P2.muki == 0){
			if (P1.XPos < P2.XPos)airGuard = true;
		}
		else if (P2.muki == 1){
			if (P2.XPos < P1.XPos)airGuard = true;
		}
	}

	if ((P2.SFlg == 2) && (P2.keyAtt[4] != 0) && (P2.aGauge > 0)){
		airGuard = true;
	}


	if (
		// ����K�[�h(�W�����v�ڍs�ł͂Ȃ��A��O�Ƃ��Ē��n�d���̓K�[�h��)�@|| �󒆃K�[�h���� || �A���K�[�h
		((P2.ctrl || P2.stateno == 47) && ((P2.keyAtt[4] != 0) || ((P2.D.selfTime > 0) && (P2.keyAtt[6] != 0)))
		&& (P2.SFlg != 2) && (P2.stateno != 40) ||
		(P2.ctrl) && (airGuard) || (P2.stateno >= 50 && P2.stateno <= 59))
		)
	{
		if (
			(H1[n].HGuardF == 1) ||

			// ���i
			((H1[n].HGuardF == 2)
			&& (P2.SFlg == 0 || P2.SFlg == 2)) ||

			(H1[n].HGuardF == 3)
			&& (((P2.SFlg == 1) || (P2.keyAtt[2] > 0)) || (P2.SFlg == 2)) ||

			(H1[n].HGuardF == 4)
			&& ((P2.SFlg == 0) || ((P2.SFlg == 1) || (P2.keyAtt[2] > 0)))
			)
			gu = true;
	}

	// �K�[�h�|�C���g
	if ((!gu) && (P2.Name == SYUICHI)) {
		if ((P2.stateno == 600) &&
			((gTime >= 1) && (gTime < 50))) {
			if ((H1[n].HGuardF == 1) || (H1[n].HGuardF == 2))
				gu = true;
		}
	}

	return gu;
	////////////////////////
	
}

void GetP_HelDamage(Player GP1, Player GP2)
{

	P1 = GP1;
	P2 = GP2;
}

void GetH_HelDamage(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}

void GetS_HelDamage(System_t GS)
{
	S = GS;
}

