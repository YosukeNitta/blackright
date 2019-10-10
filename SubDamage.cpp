//==================================================================================
//
//			���l�v�Z�̊Ǘ���
//
//==================================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"

//�f�t�@�C����----------------------------------------------



//�ް���`��------------------------------------------------
static void GuardParam();
static void GuardPoint();
static void HitParam();
static boolean GuardCheck();
static void DamageEnter();

//�����֐��錾��--------------------------------------------

static int posX;	// �G�t�F�N�g�̈ʒu
//static int velY;	// �ړ����x
static double hosei;	// �␳�ł��B
static double knockBack;	// ��ʒ[�m�b�N�o�b�N
static int getPower;	// �Q�[�W����A��������ƌ���
static int hit, guard;	// �q�b�g�A�K�[�h����
static Player P[2], hozon[2];
static boolean touch[2];
static int gTime;	// �K�[�|�A���g�̕ۑ�����
//�v���O������----------------------------------------------

void DamageCheck()
{

	//
	touch[0] = 0, touch[1] = 0;

	//[ �ʏ퓊�������ݒ� ]
	// �ǂ����������Ȃ�Ȃ�
	if ((P1.stateno == 500 && P2.stateno == 500) ||
		(P1.stateno == 510 && P2.stateno == 510)){
		// �ǂ��������O
		if (P1.time <= 6 && P2.time <= 6){
			P1.D.counter = 0;
			P1.rejectF = 1;
			P2.D.counter = 0;
			P2.rejectF = 1;
			P1.Senkou[1] = 5, P1.Senkou[2] = 5;
			P2.Senkou[1] = 5, P2.Senkou[2] = 5;
		}
		// 2P�������s
		else if (P1.time <= 6 && P2.time > 6){
			P2.D.counter = 0;
			P2.rejectF = 0;
			P2.Senkou[1] = 0, P2.Senkou[2] = 0;
		}
		// 1P�������s
		else if (P2.time <= 6 && P1.time > 6){
			P1.D.counter = 0;
			P1.rejectF = 0;
			P1.Senkou[1] = 0, P1.Senkou[2] = 0;
		}
	}

	P[0] = P1, P[1] = P2;

	// �����K�[�h
	for (int i = 0; i < 2; i++){
		if (P[i].D.selfTime > 0){
			P[i].D.selfTime--;
		}
		if (P[i].HFlg == 1){
			P[i].D.selfTime = 6;
		}
	}
	
	// �q�b�g����
	for (int p = 0; p < 2; p++){
		if (p == 0){
			P1 = P[0], P2 = P[1];
		}
		else{
			P1 = P[1], P2 = P[0];
		}


		// 1P�U��
		for (int i = 0; i < HIT_MAX; i++){
			for (int j = 0; j < ATT_MAX; j++){
				//�����蔻��`�F�b�N�A�R�s�y�� > �̕������C�R�[���ɂ���
				/*
				if (
					((P1.XAtt[j] >= P2.XHit[i] && P1.XAtt[j] <= P2.XHit[i] + P2.WHit[i]) ||
					(P2.XHit[i] >= P1.XAtt[j] && P2.XHit[i] <= P1.XAtt[j] + P1.WAtt[j])) &&
					((P1.YAtt[j] >= P2.YHit[i] && P1.YAtt[j] <= P2.YHit[i] + P2.HHit[i]) ||
					(P2.YHit[i] >= P1.YAtt[j] && P2.YHit[i] <= P1.YAtt[j] + P1.HAtt[j]))
					)
					*/
				if (Box_Touch(P1.XAtt[j], P1.YAtt[j], P1.WAtt[j], P1.HAtt[j],
					P2.XHit[i], P2.YHit[i], P2.WHit[i], P2.HHit[i]) == true)
				{
					// ��������Ȃ�
					touch[p] = 1;
					//break;
				}// 1P �U������`�F�b�N �I��
			}// �U������`�F�b�N2
		}// �S�ďI��

		if (p == 0){
			P[0] = P1, P[1] = P2;
		}
		else{
			P[0] = P2, P[1] = P1;
		}

	}// 1P.2P�I��

	P1 = P[0], P2 = P[1];

	// �����ɍU��
	if ((touch[0] == 1) && (touch[1] == 1)){
		// �Ō�
		if ((P1.attF == 1) && (P2.attF == 1)){
			/*
			if ((P1.MoveHit > 0) && (P2.MoveHit > 0)){
				touch[0] = 0;
				touch[1] = 0;
				P1.MoveHit--;
				P2.MoveHit--;
				P1.StopTime = 6;
				P2.StopTime = 6;
				SEStart(17);
				int dist = 0;
				if (P1.XPos < P2.XPos)dist = P2.XPos - P1.XPos;
				else{ dist = -(P1.XPos - P2.XPos); }
				dist /= 2;
				EffStart(12, (int)P1.XPos + dist, (int)P1.YPos, 0, -(P1.ySize / 1.5),
					0.5, 0.5, 0);
			}
			*/
		}
		// ����
		else if ((P1.attF == 2) && (P2.attF == 2)){
			// ���� �R�}�����Ȃ牽���N����Ȃ�
			if (P1.stateno > 510 && P2.stateno > 510){
				touch[0] = 0;
				touch[1] = 0;
			}
			// 1P �R�}
			else if (P1.stateno > 510 && P2.stateno <= 510){
				touch[1] = 0;
			}// 2P �R�}
			else if (P1.stateno <= 510 && P2.stateno > 510){
				touch[0] = 0;
			}
			// �����̏ꍇ�A1P������������
			else if (P1.stateno <= 510 && P2.stateno <= 510){
				//if (GetRand(2) == 0){
					touch[1] = 0;
				//}
			}
		}
		// 1P���Ō�
		else if (P1.attF != 2 && P2.attF == 2){
			// �R�}����������
			if ((P2.stateno < 500) || (P2.stateno > 510)){
				touch[0] = 0;
			}
			// �Ō�������
			else{ touch[0] = 0; }
		}
		// 2P���Ō�
		else if (P1.attF == 2 && P2.attF != 2){
			// �R�}����������
			if ((P1.stateno < 500) || (P1.stateno > 510)){
				touch[1] = 0;
			}
			else{ touch[1] = 0; }
		}
	}
	
	// 1P�U��
	if (touch[0] == 1){
		DamageEnter();
	}
	// 2P�U��
	if (touch[1] == 1){
		hozon[0] = P1, hozon[1] = P2;
		P2 = hozon[0], P1 = hozon[1];
		DamageEnter();
		hozon[0] = P2, hozon[1] = P1;
		P1 = hozon[0], P2 = hozon[1];
	}

	Get_PSet(P1, P2);
	Get_SSet(S);
	
}

void DamageEnter()
{
	// �A�[�}�[�A���K�E�E�����ȊO�͎󂯂Ȃ��A�n�����
	// ��������Ȃ�
	if ((P1.MoveHit > 0) && (P2.D.armor > 0) && ((P1.stateno < 800) && (P1.stateno != 520)) && (P1.attF != 2)
		&& (P2.SFlg != 2 || P2.stateno == 30)){

		//---------------
		// ���ʐݒ�
		//---------------
		// [�L�����Z���\��]
		P1.CFlg = 1;
		// [1P�̃q�b�g�m�F�p]
		P1.MoveHit--;
		// [�X�p�L�����P�\]
		P1.scTime = SC_TIME;

		if (GuardCheck())
			// [�K�[�h�̐ݒ�]
		{
			P2.time = 0;
			P2.StopTime = 0;
			GuardParam();
		}
		// [�X�p�A�}���q�b�g�̐ݒ�]
		else
		{
			// [�L�����Z���\��]
			P1.CFlg = 1;
			// [1P�̃q�b�g�m�F�p]
			P1.MoveHit--;
			// [�X�p�L�����P�\]
			P1.scTime = SC_TIME;
			P1.A.ncTime = NC_TIME;
			// [�_���[�W�␳]
			P2.Life -= P1.A.damage;
			// [�ۏ�_���[�W]
			P2.Life -= P1.A.hosyo;

			P2.colorCTime = 8;
			P2.colorC[0] = 255, P2.colorC[1] = 0, P2.colorC[2] = 0;

			// [�Q�[�W���]
			P1.Power += P1.GetPow;
			P2.Power += P1.GivePow;

			// [�q�b�gSE]
			SEStart(P1.HitSE);

			// [�q�b�g�G�t�F�N�g]
			int posX;
			{ posX = 20 + GetRand(10); }
			EffStart(P1.A.hitEff, (int)P2.XPos, (int)P2.YPos, posX, -(P2.ySize / 1.5),
				P1.A.heSizeX, P1.A.heSizeY, P2.muki);

			// [�q�b�g�X�g�b�v�A�̂����莞��]
			P2.StopTime = P1.HitStop;
			P1.StopTime = P1.HSSelf;
			/*
			if (P2.SFlg != 2){
				P2.D.nokezori = P1.G_HitTime;
			}
			else{ P2.D.nokezori = P1.A_HitTime; }
			*/
			// �A�[�}�[����
			P2.D.armor--;
		}
	}	// �A�[�}�[�I��

	// ��������Ȃ�
	// P1 �U���q�b�g�`�F�b�N
	// �_���[�W������A�q�b�g�t���O���I�t�ŁA���肪���G����Ȃ�
	else if ((P1.MoveHit > 0) &&
		(
		(P2.Muteki == 0 && P1.attF != 2) ||
		(P2.Muteki > 0 && P2.mutekiF == 1 && P1.attF != 1) ||
		(P2.Muteki > 0 && P2.mutekiF == 2 && P1.attF != 2) ||
		(P2.Muteki > 0 && P2.mutekiF == 3 && P1.attF != 3) ||
		// ���� �n��A��
		(P2.Muteki == 0 && P1.SFlg != 2 && P2.SFlg != 2 && P1.attF == 2) ||	
		(P2.Muteki == 0 && P1.SFlg == 2 && P2.SFlg == 2 && P1.attF == 2)
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
		// [�L�����Z���\��]
		P1.CFlg = 1;
		// [1P�̃q�b�g�m�F�p]
		P1.MoveHit--;
		// [�X�p�L�����P�\]
		P1.scTime = SC_TIME;
		P1.A.ncTime = NC_TIME;
		//***************
		//
		// �q�b�gor�K�[�h
		//
		//***************

		// �K�[�h�̐ݒ�

		if (GuardCheck())
		// [�K�[�h�̐ݒ�]
		{
			if ((P2.Name == SYUICHI) && ((P2.stateno >= 600) && (P2.stateno < 700))) {
				if (P2.stateno == 600){
					P2.time = gTime;
					GuardPoint();
				}
				if ((P2.stateno == 610) || (P2.stateno == 611)) {
					P2.time = 0;
					P2.stateno = 615;
					P1.StopTime = 10, P2.StopTime = 10;
					P1.CFlg = 0;
					SEStart(19);
				}
			}
			else { GuardParam(); }
		}
		// [�q�b�g�̐ݒ�]
		else
		{
			HitParam();
		}
		
	}// �ڐG�����I��

}

void GuardPoint()
{
	// [�Q�[�W���]
	P1.Power += P1.GetPow / 2;
	P2.Power += P1.GivePow / 2;

	// SE
	if (P2.D.nokezori >= 18)SEStart(17);
	else { SEStart(16); }

	// [�q�b�g�G�t�F�N�g]
	int posX, posY;
	int defPosY;	// ��ʒu
	float effSize = 0.0;
	defPosY = (int)P2.YPos;
	if (P1.YPos < P2.YPos - 50)defPosY = (int)P2.YPos - 50;
	if (P2.D.nokezori >= 20)effSize = 0.1;

	//P1.GuardF; �K�[�h�����ō��W������
	if ((P1.GuardF == 3 || P1.GuardF == 13) && (defPosY == (int)P2.YPos)) {
		posY = P2.ySize / 2;
	}
	else { posY = P2.ySize; }

	// �G�t�F�N�g�J�n
	{ posX = 20 + GetRand(10); }
	{
		EffStart(11, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			0.2, 0.6 + effSize, P2.muki);
	}

	// �q�b�g�X�g�b�v�A�̂����莞��
	P2.StopTime = P1.HitStop;
	P1.StopTime = P1.HSSelf;
	if (P1.StopTime < 0)P1.StopTime = 0;
	if (P2.StopTime < 0)P2.StopTime = 0;

	// [�K�[�h���J�E���g]
	P1.A.guardCount += 1;	// �J�E���g
}

void GuardParam()
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

	// [���]
	int kezuri;
	kezuri = (P1.A.damage + P1.A.hosyo) / 10;

	if (P1.stateno >= 600){
		P2.Life -= kezuri;
	}
	else if ((P1.stateno >= 220 && P1.stateno < 300) || 
			(P1.stateno >= 320 && P1.stateno < 400)){
		if (kezuri >= 8)kezuri = 8;
		P2.Life -= kezuri;
	}
	/*
	// �ʏ�Z���@�_���͍ő�4
	else if (
		(P1.stateno > 210 && P1.stateno < 300) ||
		(P1.stateno > 310 && P1.stateno < 400)){
		if (kezuri > 4)kezuri = 4;
		P2.Life -= kezuri;
	}
	*/
	if (P2.Life <= 0)P2.Life = 1;	// ���E���h�~

	// [�Q�[�W���]
	P1.Power += P1.GetPow / 2;
	P2.Power += P1.GivePow / 2;

	boolean gotoCrash = false;

	// [�K�[�h�Q�F�W����]
	if (P2.aGauge > 0){
		int n = 0;
		// ���ʌ���
		if (P2.SFlg == 2){
			n = (P1.A.damage + P1.A.hosyo) * 1.1 + 20;
		}
		else{
			n = (int)((float)(P1.A.damage + P1.A.hosyo) * 0.55);
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

		//int n = 0;
		/*
		// ���ʌ���
		if (P2.SFlg == 2){
			n = P1.A.damage + P1.A.hosyo;
		}
		else{
			n = (P1.A.damage + P1.A.hosyo) / 4;
		}

		// ���i
		if (P2.A.guardCount == 0){
			if (n > 200)n = 200;
			else if (n < 1)n = 1;
			P2.aGauge -= n;
		}
		// �K�[�h��
		else{
			if (n > 100)n = 100;
			else if (n < 1)n = 1;
			P2.aGauge -= n;
		}
		*/

		// �N���b�V��������
		if (P2.aGauge <= 0){
			gotoCrash = true;
			P1.A.guardCount = 0;
			P1.A.guard_gx = -2.0;
			P1.A.guard_ax = -1.1;
			P1.GY = 0.0;
			P1.AY = -5.0;
			P2.D.yaccel = 0.14;
			P2.Life -= 50;
			P1.CFlg = 0;
			if (P2.Life <= 0)P2.Life = 1;	// ���E���h�~
		}
	}

	// [�A�h�Q�[�W����]
	//P1.addG += (P1.A.damage + P1.A.hosyo) / 12;

	// �K�[�N���ł͂Ȃ�
	if (!gotoCrash){
		// [�A�j���ݒ�]
		if (P2.SFlg == 0)P2.stateno = 50;
		if (P2.SFlg == 1 || (P2.keyAtt[2] > 0))P2.stateno = 51;
		if (P2.SFlg == 2)P2.stateno = 52;

		// �K�[�h����
		P2.D.nokezori = P1.G_GuardTime;
		if ((P2.SFlg == 2) && (P1.A.A_GuardTime > 0))P2.D.nokezori = P1.A.A_GuardTime;
	}
	else{
		// [�A�j���ݒ�]
		if (P2.SFlg == 0)P2.stateno = 1000;
		if (P2.SFlg == 1)P2.stateno = 1020;
		if (P2.SFlg == 2)P2.stateno = 1030;

		// �K�[�h����
		P2.D.nokezori = 70;
	}

	// SE
	if (P2.D.nokezori >= 18)SEStart(17);
	else{ SEStart(16); }

	// [�q�b�g�G�t�F�N�g]
	int posX, posY;
	int defPosY;	// ��ʒu
	float effSize = 0.0;
	defPosY = (int)P2.YPos;
	if (P1.YPos < P2.YPos - 50)defPosY = (int)P2.YPos - 50;
	if (P2.D.nokezori >= 20)effSize = 0.1;

	//P1.GuardF; �K�[�h�����ō��W������
	if ((P1.GuardF == 3 || P1.GuardF == 13) && (defPosY == (int)P2.YPos)){
		posY = P2.ySize / 2;
	}
	else{ posY = P2.ySize; }

	// �G�t�F�N�g�J�n
	{ posX = 20 + GetRand(10); }
	if (!gotoCrash){
		EffStart(11, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			0.2, 0.6 + effSize, P2.muki);
	}
	else{
		EffStart(15, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			0.4, 0.4 + effSize, P2.muki);
	}

	// �q�b�g�X�g�b�v�A�̂����莞��
	P2.StopTime = P1.HitStop;
	P1.StopTime = P1.HSSelf;
	if (P1.StopTime < 0)P1.StopTime = 0;
	if (P2.StopTime < 0)P2.StopTime = 0;

	// �K�[�h����

	// [�q�b�g�o�b�N�@��ʒ[�ɂ��āA1P���n��]
	if (((P2.XPos <= GAMENHAJI) || (P2.XPos >= (STAGE_WIDTH - GAMENHAJI))) &&
		(P1.SFlg != 2)){
		// �󒆂ŋ����Ă���
		if (P2.SFlg == 2){
			P1.A.hbVelX = P1.A.guard_gx;
			P2.XVel = P1.A.guard_ax;
			P2.YVel = P1.AY / 2.0;
		}
		else{ 
			//P1.A.hbVelX = P1.A.guard_gx + (P1.A.guard_gx * 0.1), 
			P2.XVel = P1.A.guard_gx + (P1.A.guard_gx * 0.1);
			P2.YVel = P1.GY; 
		}
		//else{ P1.XVel = P1.GX + (P1.GX * 0.1), P2.XVel = P1.GX, P2.YVel = P1.GY; }
		// �����񂹋Z�K�[�h���A�߂Â��Ȃ��悤��
		if (P1.A.hbVelX > 0)P1.A.hbVelX = 0;
	}
	// [������炢]
	else{
		// �󒆂ŋ����Ă���
		if (P2.SFlg == 2){
			//P2.XVel = P1.AX * 1.5;
			P2.XVel = P1.A.guard_ax;
			P2.YVel = P1.AY / 2.0;
		}
		// ������K�[�h���͋�������
		else{ 
			P2.XVel = P1.A.guard_gx + (P1.A.guard_gx * 0.1);
			P2.YVel = P1.GY; 
		}
	}
	// �󒆃K�[�h�̑��x�A�K�x��
	if (!gotoCrash){
		if (P2.SFlg == 2){
			if (P2.YVel < -1.2)P2.YVel = -1.2;
			else if (P2.YVel > -0.2)P2.YVel = -0.2;

			if (P2.XVel < -2)P2.XVel = -2;
		}
	}

	// [�K�[�h���J�E���g]
	if (!gotoCrash)
		P1.A.guardCount += 1;	// �J�E���g

	//i = HIT_MAX + 1;
	//break;


	// �g�����\���p
	AttackSpeed(P1.time);
	Frame_Check();

}

void HitParam()
{
	
	hosei = (P1.A.cl_max - ((P1.HitCount - 1) * P1.A.comboLate)) * P1.A.hosei_K;

	// [�_���[�W�␳]
	if (P1.HitCount == 0){			// ���i�_���[�W
		P2.Life -= P1.A.damage;
		
		// �J�E���^�[�q�b�g
		if (P2.D.counter){		
			// �t�F�C�^���ǉ��_��
			if(P2.D.fatal)P2.Life -= 20;
			// �����ȊO�Ȃ�_������
			if ((P1.A.damage + P1.A.hosyo) > 200){
				P2.Life -= 40;
			}
			else{ P2.Life -= (P1.A.damage + P1.A.hosyo) * 0.2; }

			if (P1.HitStop > 0){
				P1.G_HitTime += 2;
				P1.A_HitTime += 2;
			}
			P2.colorCTime = 5;
			P2.colorC[0] = 255, P2.colorC[1] = 0, P2.colorC[2] = 0;
		}
		
	}
	else if (P1.HitCount >= 1){		// ���i�ȍ~
		// �_��������������
		if (hosei < P1.A.cl_min)
		{
			// �����␳�̂�������
			if (P1.A.hosei_K < P1.A.cl_min){
				P2.Life -= (int)(P1.A.damage * P1.A.hosei_K);
			}
			// �Œ�_���[�W
			else{
				P2.Life -= (int)(P1.A.damage * P1.A.cl_min);
			}
		}
		else
		{ 
			P2.Life -= (int)(P1.A.damage * hosei);
		}
	}
	
	/*
	else if (P1.HitCount >= 11){
		P2.Life -= (int)((P1.A.damage * 0.15) * P1.A.hosei_K);
	}
	*/
	// [�ۏ�_���[�W]
	P2.Life -= P1.A.hosyo;
	// kill
	if ((!P1.A.kill) && (P2.Life <= 0))P2.Life = 1;

	// [�Q�[�W���]
	// ����\��
	AddGaugeDraw(P1.addG, P1.PSide);

	// �Q�[�W�␳�����邩�A����������Ȃ�A�Q�[�W���������炷
	if (P1.A.gaugeHosei || getPower >= 1000){
		P1.Power += (int)(P1.GetPow * 0.2);
		if (P1.stateno != 520)P1.Power += P1.addG * 5;
	}
	else if (getPower >= 500){
		P1.Power += (int)(P1.GetPow * 0.8);
		if (P1.stateno != 520)P1.Power += P1.addG * 5;
	}
	else{ 
		P1.Power += P1.GetPow; 
		if(P1.stateno != 520)P1.Power += P1.addG * 5;
	}
	if(P1.stateno != 520)P1.addG = 0;

	// ����Q�[�W
	if (getPower >= 500){
		P2.Power += (int)(P1.GivePow * 0.8);
	}
	else{ P2.Power += P1.GivePow; }

	// �Q�[�W����ʌv��
	if (P1.HitCount == 0){
		getPower = 0;
	}
	getPower += P1.GetPow;


	// [����������]
	// ����\�����n�Ȃ�
	if (P2.ctrl || P2.stateno == 47)P2.rejectF = true;
	// ����������Ȃ�A�������Ȃ�
	else if ((P2.stateno == 500 || P2.stateno == 510)){

	}
	// �ǂ�ł��Ȃ�
	else{ P2.rejectF = false; }


	// [�A�j���ݒ�]
	// ���b�N�Z�͂��̂܂�
	if ((P1.HitAnim >= 1060) && (P1.HitAnim <= 1069)){
		P2.stateno = P1.HitAnim;
	}
	else{
		// ������炢�Ȃ�p���ύX
		if (P1.A.forceKurai){
			if (P2.SFlg == 2){ P2.stateno = 1030; }
			else { P2.stateno = P1.HitAnim; }
		}
		// �󒆋�炢�𔻒f
		else if ((P2.SFlg == 2) || (P1.GY < 0)){
			P2.stateno = 1030;
		}
		// ���ʂ̋�炢�ŗ���
		else if (P2.SFlg == 0){
			if (P1.HitAnim == 1000)P2.stateno = 1000;
			else{ P2.stateno = 1010; }
		}
		// ���ʂ̋�炢�ł��Ⴊ��
		else if (P2.SFlg == 1){
			P2.stateno = 1020;
		}

	}

	// [�q�b�gSE]
	SEStart(P1.HitSE);

	// [�q�b�g�G�t�F�N�g]
	int posX, posY;
	int defPosY;	// ��ʒu
	//if (P2.muki == 1)posX = -20 - GetRand(10);
	//else
	defPosY = (int)P2.YPos;
	if (P1.YPos < P2.YPos - 50)defPosY = (int)P2.YPos - 50;

	//P1.GuardF; �K�[�h�����ō��W������
	if ((P1.GuardF == 3 || P1.GuardF == 13) && (defPosY == (int)P2.YPos)){
		posY = P2.ySize / 2;
	}
	else{ posY = P2.ySize; }

	{ posX = 20 + GetRand(10); }
	if (P1.A.hitEff != 0){
		EffStart(10, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			0.2, 0.2, P2.muki);
		for (int i = 0; i < 6; i++){
			float rand = GetRand(4);
			rand = (rand * 0.01) + 0.07;
			EffStart(9, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
				rand, rand, P2.muki);
		}
		EffStart(P1.A.hitEff, (int)P2.XPos, defPosY, posX, -(posY / 1.5),
			P1.A.heSizeX, P1.A.heSizeY, P2.muki);
	}

	// [�q�b�g�X�g�b�v�A�̂����莞��]
	P2.StopTime = P1.HitStop;
	P1.StopTime = P1.HSSelf;
	if (P1.StopTime < 0)P1.StopTime = 0;
	if (P2.StopTime < 0)P2.StopTime = 0;

	if (P2.SFlg != 2){
		P2.D.nokezori = P1.G_HitTime;
	}
	else{ P2.D.nokezori = P1.A_HitTime; }

	// �i�p�h�~
	if ((P2.SFlg == 2) && (P1.HitCount >= 20))knockBack += 1;
	if (P1.HitCount <= 1)knockBack = 0;

	// [�q�b�g�o�b�N�@��ʒ[�ɂ��āA1P���n��]
	if ((P2.XPos <= GAMENHAJI) || (P2.XPos >= (STAGE_WIDTH - GAMENHAJI)))
	{
		if (P1.SFlg != 2)P1.A.hbVelX = P1.GX;
		// �󒆂ŋ����Ă���
		if (P2.SFlg == 2){
			P2.XVel = P1.AX;
			// ���K�E�͌Œ�
			if (P1.stateno >= 800)P2.YVel = P1.AY;
			else{ P2.YVel = P1.AY + (knockBack * 0.1); }
			/*
			// �q�b�g�������ŏd��
			if (P1.HitCount > 20){
				P2.YVel = (P1.AY + ((float)0.25 * (P1.HitCount - 10)));
			}
			//else { P2.YVel = P1.AY; }
			*/
		}
		// ��ʒ[��2P�n��
		else{ 
			P2.XVel = P1.GX;
			P2.YVel = P1.GY; 
		}
		// �����񂹋Z�ł��O�i���Ȃ�
		if (P1.A.hbVelX > 0)P1.A.hbVelX = 0;
	}
	else{
		// �󒆂ŋ����Ă���
		if (P2.SFlg == 2){
			P2.XVel = P1.AX;
			P2.YVel = P1.AY + (knockBack * 0.1);
		}
		else{ 
			P2.XVel = P1.GX, P2.YVel = P1.GY; 
		}
	}

	// [���b�N���̓q�b�g�o�b�N�Ȃ�]
	if ((P1.HitAnim >= 1060) && (P1.HitAnim <= 1069)){
		P1.A.hbVelX = 0;
		P2.XVel = 0;
		P2.YVel = 0;
	}

	// [�d��]
	P2.D.yaccel = P1.A.yaccel;

	// [�q�b�g���J�E���g]
	if ((P1.A.damage > 0) || (P1.A.hosyo > 0))
		P1.HitCount += 1;	// �_���[�W�����鎞�̂݃J�E���g

	// [2P�̃R���g���[����D��]
	P2.ctrl = 0;
	// [�t�H�[������]
	P2.fall = P1.fallF;


	// [�o�E���h�ݒ�]
	P2.D.bound_xvel = P1.A.bound_xvel;
	P2.D.bound_yvel = P1.A.bound_yvel;
	P2.D.wall_xvel = P1.A.wall_xvel;
	P2.D.wall_yvel = P1.A.wall_yvel;
	
	P2.D.bound = P1.A.boundLevel;
	if (P2.D.bound > 2)P2.D.bound = 2;

	if (P2.D.boundCount > 0){
		// �S�ď��o�E���h
		if ((P1.A.boundLevel > 0) && (P1.A.boundLevel != 3)){
			P2.D.bound = 1;
		}
	}
	// ���x�߂�
	if (P2.D.bound == 0){
		P2.D.bound_xvel = -1.3;
		P2.D.bound_yvel = -3.0;
	}
	
	// �傫��������Ȃ�
	if (P2.D.wall <= 2){
		P2.D.wall = P1.A.wallLevel;
	}
	// ���x�߂�
	if (P2.D.wall == 0){
		P2.D.wall_xvel = -0.4;
		P2.D.wall_yvel = -6.4;
	}
	
	// [�A�[�}�[�폜]
	if(P2.D.armor > 0)P2.D.armor = 0;

	// [���G�t��]	�ꕔ�̋Z�p�i���b�N���߂�ǂ����Ƃ��j
	if (P1.A.addMuteki){
		P2.Muteki = P1.A.amTime;
		P2.mutekiF = 0;
		P1.A.addMuteki = false;
		P1.A.amTime = 0;
	}

	// [�n�k�G�t�F�N�g]
	if (P1.A.quakeTime > 0){
		S.quakeTime = P1.A.quakeTime;
		S.quakeY = P1.A.quakeY;
	}
	
	// �g�����\���p
	AttackSpeed(P1.time);
	Frame_Check();
	Beat_Check(P2.OldState, P1.PSide);
}

boolean GuardCheck()
{
	// �g�����K�[�h�K�p
	if ((S.TraningFlg) && (S.TSwitch[4] > 0) && (S.TSwitch[2] < 3) && (P1.PSide == 1)){
		if ((S.TSwitch[4] == 1) || 
			((S.TSwitch[4] == 2) && (GetRand(1) == 0))){
			P2.keyAtt[4] = 1;	// �K�[�h���邩�����_��
			if ((P1.GuardF == 3) || (P1.GuardF == 13))P2.keyAtt[2] = 1;
		}

		// �K�[�h
		if (P2.stateno >= 50 && P2.stateno < 60){
			if ((P1.GuardF == 3) || (P1.GuardF == 13)){
				P2.keyAtt[2] = 1;
				P2.button[102] = 1;
			}
		}
	}

	// �K�[�h���s
	boolean gu = false;
	// �󒆃K�[�h��
	boolean airGuard = false;

	// �����������Ȃ�6���͂ł��󒆃K�[�h�\
	if ((P2.SFlg == 2) && (P2.keyAtt[6] != 0) && (P2.aGauge > 0) && (P2.stateno != 60 && P2.stateno != 65)){
		if (P2.muki == 0){
			if (P1.XPos < P2.XPos)airGuard = true;
		}
		else if (P2.muki == 1){
			if (P2.XPos < P1.XPos)airGuard = true;
		}
	}

	// ��K����
	if ((P2.SFlg == 2) && (P2.keyAtt[4] != 0) && (P2.aGauge > 0) && 
		(P2.stateno != 60 && P2.stateno != 65)){
		airGuard = true;
	}

	if (
		// ����K�[�h(�W�����v�ڍs�ł͂Ȃ��A��O�Ƃ��Ē��n�d���̓K�[�h��) &&
		// �󒆃K�[�h���� || �A���K�[�h || �N���オ��߂��聕���肪��
		// ���肪�󒆂ł͂Ȃ� &&
		// �W�����v�ڍs�ȊO || ����\�ŋ�Kok || ���ɃK�[�h��
		(
		(P2.ctrl || P2.stateno == 47) && 
		((P2.keyAtt[4] != 0) || ((P2.D.selfTime > 0) && (P2.keyAtt[6] != 0) && (P1.SFlg == 2))) && 
		(P2.SFlg != 2) && 
		(P2.stateno != 40) || (P2.ctrl) && (airGuard) || (P2.stateno >= 50 && P2.stateno <= 59)
		)
	   )
		{
		// �K�[�h��F���A�K�[�h�����m�F�J�n
		if (
			(P1.GuardF == 1) ||
			
			// ���i
			((P1.GuardF == 2)
			&& ((P2.SFlg == 0 && (P2.keyAtt[2] == 0)) || P2.SFlg == 2)) ||	// �������Ă��Ȃ�
			// ���i
			(P1.GuardF == 3)
			&& (((P2.SFlg == 1) || (P2.keyAtt[2] > 0)) || (P2.SFlg == 2)) || 
			// ��K�s��
			(P1.GuardF == 4)
			&& ((P2.SFlg == 0) || ((P2.SFlg == 1) || (P2.keyAtt[2] > 0)))
			)
			gu = true;
	}
	// �K�[�h�|�C���g
	if ((!gu) && (P2.Name == SYUICHI)) {
		if ((P2.stateno == 600) && 
			((gTime >= 2) && (gTime < 30))) {
			if((P1.GuardF == 1) || (P1.GuardF == 2))
			gu = true;
		}
		if ((P2.stateno == 610) &&
			((gTime >= 2) && (gTime <= 17))) {
			if ((P1.GuardF == 1) || (P1.GuardF == 2))
				gu = true;
		}
		if ((P2.stateno == 611) &&
			((gTime >= 2) && (gTime <= 17))) {
			if ((P1.GuardF == 3) || (P1.GuardF == 13))
				gu = true;
		}
	}

	return gu;	// �K�[�h�̉�
}

void GetP_SubDamage(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetH_SubDamage(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}

void GetS_SubDamage(System_t GS)
{
	S = GS;
}
