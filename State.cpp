//==================================================================================
//
//			���l�v�Z�̊Ǘ���
//
//========================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"

//�f�t�@�C����----------------------------------------------


//�ް���`��------------------------------------------------

static Player PP2;
// 
static int Load_1;
static boolean Load_2;

// �O���StopTime
static int mOldStop[2];
static int hOldStop[2][HM];

static int count;
static int fpSet;	// FirstPosSet�Ŏg�p

static int oldState[2];
static int hh[2];

//
static int bo;	// �u���b�N�A�E�g��~

// �R�}���h���͒��~
static bool stopCmd;

//�����֐��錾��--------------------------------------------
// ���G���Ԍv��
void MutekiCount();
// �摜�̏���
void GraphicSetting();
// �U���̓��e
void AttackSetting();
// ����
void HitBoxSetting();
// �ڐG�ƈʒu
void TouchPos();
// �X�e�[�g���e
void SetParam();
// 
void SetParam2();
// �v���̒l
void DrawSetting();
// ���ԉ��Z
void TimeCount();

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


/// <summary>
/// �ΐ핔�����쏈��
/// </summary>
/// <returns>���ɂȂ�</returns>
int CharMove(void)
{
	// �摜�̃��[�h
	if (Load_1 == 0)
	{
		// �摜��ǂݍ���
		LoadPict();
		Load_1 = 1;
	}
	//-------------
	// �g�����ݒ�
	//-------------
	{
		if (S.TraningFlg){
			GetP_Traning(P1, P2);
			GetS_Traning(S);
			GetH_Traning(H1, H2);
			SetTraning();	// OK!
		}
	}
	//-------------
	// �L�[����
	//-------------
	{
		// �^��
		if (Replay_Mode(-1) == 2)Replay_ReserveKey();

		GetP_Key(P1, P2);
		GetS_Key(S);
		InputSend(); 
		// ���E���h�擾������s�\
		if (S.roundState == 2 && S.roundTime == 0){
			// �{�^����t���~
			for (int i = 0; i < BUTTON_MAX + 1; i++){
				P1.button[i] = 0;
				P2.button[i] = 0;
				P1.Senkou[i] = 0;
				P2.Senkou[i] = 0;
			}
		}
	}
	//-------------
	// �R�}���h���͊m�F
	//-------------
	//P1,2
	if (!stopCmd)
	{
		// 1P
		{
			// �R�}���h����
			GetP_EinCmd(P1, P2);
			Ein_ComSet();	// 1P// OK!
		}
		// 2P
		{
			// �R�}���h����
			GetP_EinCmd(P2, P1);
			Ein_ComSet();	// OK!
		}
	}
	
	// AI�p
	if(S.AI)PP2 = P2;

	P1.Var[30] = 10, P2.Var[30] = 10;

#pragma region ����
	// �q�b�g�X�g�b�v���͒�~
	if(S.StopTime == 0){
	/***
	* �L�����N�^�[����
	***/
		//1P,2P�����J�n
	{
		//-----
		// ����ݒ�
		//-----
		{
			// �N���オ��
			if (P1.stateno == 0 && oldState[0] == 1055){
				P1.Muteki = 1000, P1.mutekiF = 0;
			}
			if (P2.stateno == 0 && oldState[1] == 1055){
				P2.Muteki = 1000, P2.mutekiF = 0;
			}
			oldState[0] = P1.stateno;
			oldState[1] = P2.stateno;
		}
		//----------------
		// �ڐG����Z�b�g
		//----------------
		{
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			TouchHantei();// OK!
		}
		
		//------------
		//
		// ���Ⴊ��(11) or ����(0)����
		// �����m�F
		// �������_�b�V��
		// �W�����v
		// �K�[�h
		// �ʏ�Z�i���ʁj
		//
		//------------

		// ���E���h���͈ړ��̂݉\
		if (S.roundState == 1 || S.roundState == 2)
		{
			GetP_SubState(P1, P2);
			GetS_SubState(S);
			GetH_SubState(H1, H2);
			// �l�͌������ŕԂ��Ă�
			EnterState();
		}
		// �ΐ풆�Ȃ�
		if (S.roundState == 1){
		{
			GetP_SubState(P1, P2);
			GetS_SubState(S);
			GetH_SubState(H1, H2);
			// �l�͌������ŕԂ��Ă�
			EnterAttack();
		}
		//------------------------
		//
		// ����E�K�E�Z�i�L�����ʁj
		//
		//------------------------
		{
			Player P[2];
			P[0] = P1, P[1] = P2;

			for (int i = 0; i < 2; i++){
				// ����
				//GetPH_EinCmd(P[i]);
				if (i == 0){
					GetP_EinCmd(P[0], P[1]);
					GetH_EinCmd(H1, H2);
				}
				else if (i == 1){
					GetP_EinCmd(P[1], P[0]);
					GetH_EinCmd(H2, H1);
				}
				CharCmd();
			}

		}	// �R�}���h�I��
		}	// roundState == 1�I��
		if (S.AI)P2 = PP2;
		//----
		// AI
		//----
		{
			// AI //
			if ((S.AI) && (S.roundState == 1)){
				GetP_AI(P1, P2);
				GetS_AI(S);
				GetH_AI(H1, H2);
				AI_Move();
			}
		}
		//----------
		//
		// �ڐG�J�n
		// ���W�ݒ�
		//
		//----------
		{
			//TouchPos();// OK!

			// ��{���W�Z�b�g�A
			// ���̌�A�X�N���[���ɍ��킹�ďC��
			//GetP_SubSystem(P1, P2);
			//GetS_SubSystem(S);
			//VelPosSet();

			//------------
			// ���x����
			//------------
			{
				// �ݒ��n��
				//GetP_SubSystem(P1, P2);
				//GetS_SubSystem(S);
				//VelXYAdd();	// �l��Ԃ�

			}

			// ��������Ȃ�
			// �ݒ��n��
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			// �������Ă���ʒu�ύX
			TouchCheck();
			
		}
		//--------------
		//
		// �X�N���[���ݒ�
		// �ʒu�C��
		//
		//--------------
		{
			/*
			// �ݒ��n��
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			// �X�N���[���J�n
			SetScrool();// OK!
			*/

			// �ݒ��n��
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			// �X�N���[�����Ȃ���ʒu�C��
			PosHosei();	// �l��Ԃ�
		}
		
		//***********
		// ��炢�E�摜����ݒ�1
		//***********
		{
			HitBoxSetting();
		}
		//----------------
		//
		// �U���q�b�g����
		//
		// �L�����Z������
		// �q�b�g���J�E���g
		// ���G���ԕύX
		//
		//------------------
		{

			AttackSetting();// OK!
			
		}
		//-------------------
		//
		// �X�e�[�g�ݒ�K�p
		//
		//-------------------
		{
			MutekiCount();

			// ����ݒ�(�������G)
			Player P[2];
			P[0] = P1, P[1] = P2;
			for (int i = 0; i < 2; i++){
				if (P[i].Muteki == 999)P[i].Muteki = ThrowMuteki_Down, P[i].mutekiF = 2;
			}
			P1 = P[0], P2 = P[1];

			SetParam();// OK!


			//============
			// �ǉ�
			/*
			AttackSetting();
			SetParam2();
			MutekiCount();

			// �q�b�g��ݒ�
			GetP_SubDamSet(P1, P2);
			DamSetting();// OK!
			*/
			// �q�b�g��ݒ�
			GetP_SubDamSet(P1, P2);
			DamSetting();// OK!
			//============
		}
		//--
		// �ǉ�
		//--
		{
			//----------
			//
			// �ڐG�J�n
			// ���W�ݒ�
			//
			//----------
			{
				//TouchPos();// OK!

				// ��{���W�Z�b�g�A
				// ���̌�A�X�N���[���ɍ��킹�ďC��
				GetP_SubSystem(P1, P2);
				GetS_SubSystem(S);
				VelPosSet();

				//------------
				// ���x����
				//------------
				{
					// �ݒ��n��
					GetP_SubSystem(P1, P2);
					GetS_SubSystem(S);
					VelXYAdd();	// �l��Ԃ�

				}

				// �ݒ��n��
				GetP_SubSystem(P1, P2);
				GetS_SubSystem(S);
				// �������Ă���ʒu�ύX
				TouchCheck();
			}
		}
		//------------
		// �X�N���[��
		// ���W�ݒ� 2
		// ���d�v
		//------------
		{

			// �ݒ��n��
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			// �X�N���[���J�n
			SetScrool();// OK!

			// �ݒ��n��
			GetP_SubSystem(P1, P2);
			GetS_SubSystem(S);
			// �X�N���[�����Ȃ���ʒu�C��
			PosHosei();	// �l��Ԃ�

		}
		//***********
		// ��炢�E�摜����ݒ� 2
		//***********
		{
			HitBoxSetting();

			// �q�b�g��ݒ�
			//GetP_SubDamSet(P1, P2);
			//DamSetting();// OK!
		}
		/**********
		/ �摜�p�̏���
		/*********/
		{
			GraphicSetting();	//�����̂���?
		}
		/**********
		/ ���s�ݒ�
		/*********/
		{
			/***********
			* ���C�t�J�E���^�[�����s����
			***********/
			GetPS_LifeCount(P1, P2, S);
			GetH_LifeCount(H1, H2);
			LifeCount();
		}
		//************
	}
	// 1P,2P�̏����I��
	
	//------------
	// �`��u�����v
	//------------
	{
		DrawSetting();
	}
	//----------
	// ���Ԍv��
	//----------
	{
		TimeCount();	// ���f�����̂̓O������������Ȃ̂ŁA���Ȃ�
	}



	}// �I���A�q�b�g�X�g�b�v���ĂȂ��Ȃ瓮��
#pragma endregion

	//-----------------
	//
	// �q�b�g�X�g�b�v��
	//
	//-----------------

	// �X�g�b�v���͑S�ē����Ȃ�
	else if(S.StopTime > 0){
		S.StopTime -= 1;
		//------------
		// �`��u�����v
		//------------
		{
			DrawSetting();
		}
	}

	

	//-----------------
	//
	// �Ó]���Ԃ̒���
	//
	//-----------------
	{
		if (S.Anten > 0)S.Anten -= 1;
		else if (S.Anten < 0)S.Anten = 0;
		if (S.noGauge > 0)S.noGauge -= 1;
		else if (S.noGauge < 0)S.noGauge = 0;
	}
	//-----------------
	//
	// �g�����ݒ肻�̂Q
	//
	//-----------------

	// 2P����p���̂Q
	if (S.TraningFlg){
		//[ 2P���� ]
		if (S.TSwitch[7] == 1 || S.TSwitch[7] == 2){
			P2Ctrl2();
		}
	}

	//**************
	//
	// �|�[�Y���
	//
	//**************
		if (
			((!S.TraningFlg) && (P_BInput(7) == 1))
			||
			((S.TraningFlg) && (P1_BInput(7) == 1))
		){
			// �^����I�t�ɂ���
			S.TSwitch[7] = 0;

			// ��x����������󂯕t���Ȃ�
			P1_BCheck();
			P2_BCheck();
			GetP_Pause(P1, P2);
			GetS_Pause(S);

			// �|�[�Y��ʂɈڍs
			PauseSetting(1);
			raw_set();	// ���E������ς���x�����I�t
		}
		return 0;
}


/// <summary>
/// �̗́A�ʒu��������
/// </summary>
void CharLoad()
{
	// ���O��select�ŏo�����l���烍�[�h
	P1.Name = Player_Name_1();
	P2.Name = Player_Name_2();
	load_LifeCount();	// ���C�t�E��������

	// nameC�ݒ�
	GetP_Air(P1);
	GetPName();
	GetP_Air(P2);
	GetPName();

	// ��
	PVoiceSet(1, P1.Name);
	PVoiceSet(2, P2.Name);

	/*********
	* �\�͐ݒ�
	*********/
	// 1P
	GetP1_Player(P1);
	GetS_Player(S);
	Player_State();
	// 2P
	GetP1_Player(P2);
	GetS_Player(S);
	Player_State();

	// �p�����[�^������
	Player P[2];
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){

		// �v���C���[��
		P[i].PSide = i+1;
		P[i].muki = i;


		// �̗͂����C�t�̍ő�l�ɏ�����
		P[i].Life = P[i].C.lifeMax;
		P[i].LifeC = LIFECOUNT_MAX;
		P[i].Power = 0;
		P[i].aGauge = GUARD_MAX;
		P[i].GRecovery = 0;
		P[i].addG = 0;

		P[i].YPos = GROUND;
		P[i].XVel = 0;
		P[i].YVel = 0;
		P[i].SFlg = 0;
		P[i].ctrl = 1;
		P[i].touchF = true;
		P[i].time = 0;
		P[i].StopTime = 0;
		P[i].MoveHit = 1;

		P[i].aPosX = 0;
		P[i].aPosY = 0;

		P[i].D.armor = 0;

		// Var������
		for (int j = 0; j < VAR_MAX; j++){
			P[i].Var[j] = 0;
		}
		for (int j = 0; j < ATT_MAX; j++){
			P[i].XAtt[j] = -100, P[i].YAtt[j] = -100;
			P[i].WAtt[j] = 0, P[i].HAtt[j] = 0;
		}
	}
	P1 = P[0], P2 = P[1];

	
	
	//�������W���Z�b�g
	P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
	if (fpSet == 1){
		P1.XPos = STAGE_WIDTH - GAMENHAJI - 85, P2.XPos = STAGE_WIDTH - GAMENHAJI - 5;
	}
	else if (fpSet == -1){
		P1.XPos = GAMENHAJI + 5, P2.XPos = GAMENHAJI + 85;
	}

	// �w���p�[������
	for (int i = 0; i < HM; i++){
		H1[i].var = false;
		H1[i].A.damage = 0;
		H2[i].var = false;
		H2[i].A.damage = 0;

		for (int j = 0; j < HIT_MAX; j++){
			H1[i].XAtt[j] = -1;
			H1[i].YAtt[j] = -1;
			H1[i].WAtt[j] = -1;
			H1[i].HAtt[j] = -1;

			H2[i].XAtt[j] = -1;
			H2[i].YAtt[j] = -1;
			H2[i].WAtt[j] = -1;
			H2[i].HAtt[j] = -1;
		}
	}

	P1.colorCTime = 0, P2.colorCTime = 0;
	P1.D.yaccel = 0.35, P2.D.yaccel = 0.35;

	//================
	// �V�X�e��������
	//================
	S.ScroolX = ((STAGE_WIDTH / 2) - (SCREEN_W / 2));
	if (fpSet == 1){
		S.ScroolX = STAGE_WIDTH - SCREEN_W;
	}
	else if (fpSet == -1){
		S.ScroolX = 0;
	}

	S.ScroolY = (STAGE_HEIGHT - SCREEN_H);
	// �J���̓X�N���[�����Ȃ�
	if (S.TraningFlg){
		S.noScroolX = false;
		S.noScroolY = false;
	}
	else{
		S.noScroolX = true;
		S.noScroolY = true;
	}

	S.roundState = 0;
	P1.stateno = 90, P2.stateno = 90;
	if (S.TraningFlg){
		S.roundState = 1;
		P1.stateno = 0, P2.stateno = 0;
	}
	S.roundTime = 0;
	S.StopTime = 0;
	S.Anten = 0;
	S.noGauge = 0;

	// �c��
	afterimageSetting(1, 0, 0, 0, 0);
	afterimageSetting(2, 0, 0, 0, 0);

	// �_�b�V���֘A
	SubState_DashReset();

	fpSet = 0;
}

void Get_PSet(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void Get_P1Set(Player GP1)
{
	P1 = GP1;
}

void Get_P2Set(Player GP2)
{
	P2 = GP2;
}

void Get_SSet(System_t GS)
{
	S = GS;
}

void Get_PSSet(Player GP1, Player GP2, System_t GS)
{
	P1 = GP1;
	P2 = GP2;
	S = GS;
}

void Get_HSet(Helper GH1[], Helper GH2[])
{
	int i;
		for (i = 0; i < HM; i++){
			H1[i] = GH1[i];
			H2[i] = GH2[i];
		}
}


//-------------------------------------------------------------------------------
//
// �J���[����
//
//-------------------------------------------------------------------------------

void Get_Color(int c, int PSide)
{
	if (PSide == 1)P1.Palno = c;
	if (PSide == 2)P2.Palno = c;

	// �p���b�g������x���[�h
	Load_1 = 0;
}


//-------------------------------------------------------------------------------
//
// �g�����X�C�b�`
//
//-------------------------------------------------------------------------------

// �g�����̃I���E�I�t
void TraningSwitch(boolean flg)
{
	S.TraningFlg = flg;

}

void AISwitch(boolean flg)
{
	S.AI = flg;
}

// var��num���Z�b�g
void T_VarSet(int var, int num)
{
	S.TSwitch[var] = num;
}


//-------------------------------------------------------------------------------
//
// �����̓��e
//
//-------------------------------------------------------------------------------

void AttackSetting()
{
	// �w���p�[
	if ((P2.stateno < 1060) || (P2.stateno >= 1070)){
		GetP_HelDamage(P1, P2);
		GetH_HelDamage(H1, H2);
		GetS_HelDamage(S);
		HelperDamCheck();
	}
	if ((P1.stateno < 1060) || (P1.stateno >= 1070)){
		GetP_HelDamage(P2, P1);
		GetH_HelDamage(H2, H1);
		GetS_HelDamage(S);
		HelperDamCheck();
	}

	// �{��
	GetP_SubDamage(P1, P2);
	GetS_SubDamage(S);
	DamageCheck();	

	GetP_SubDamSet(P1, P2);
	GetS_SubDamSet(S);
	DamSetting();// OK!
}

void MutekiCount()
{
	//[ ���G���Ԍ��� ]
	{
		if (P1.StopTime == 0){
			if (P1.Muteki > 0)P1.Muteki -= 1;
			else if (P1.Muteki < 0)P1.Muteki = 0;
		}
		if (P2.StopTime == 0){
			if (P2.Muteki > 0)P2.Muteki -= 1;
			else if (P2.Muteki < 0)P2.Muteki = 0;
		}
	}

	// ���łɐF��
	if (P1.colorCTime > 0)P1.colorCTime -= 1;
	else if (P1.colorCTime < 0)P1.colorCTime = 0;
	if (P2.colorCTime > 0)P2.colorCTime -= 1;
	else if (P2.colorCTime < 0)P2.colorCTime = 0;

}

void GraphicSetting()
{
	//�摜�������Ă���
	//P1.Image = P1.GraphImage[P1.aNum1][P1.aNum2];
	//P2.Image = P2.GraphImage[P2.aNum1][P2.aNum2];
	P1.Image = Player_ImageGet(P1);
	P2.Image = Player_ImageGet(P2);

	// �摜�T�C�Y�擾 P1,2
	GetGraphSize(P1.Image, &P1.GraphW, &P1.GraphH);
	GetGraphSize(P2.Image, &P2.GraphW, &P2.GraphH);

	//---------------
	// ���o�i�n�k�j
	//---------------
	{
		if (S.quakeTime > 0)S.quakeTime--;
		else{
			S.quakeX = 0;
			S.quakeY = 0;
			S.quakeTime = 0;
		}
	}

	//�摜�������Ă���
	GetP1_Player(P1);
	GetH_Player(H1, H2);
	Helper_Graph();

	GetP1_Player(P2);
	GetH_Player(H2, H1);
	Helper_Graph();

}

void TouchPos()
{
	

	// ��{���W�Z�b�g�A
	// ���̌�A�X�N���[���ɍ��킹�ďC��
	GetP_SubSystem(P1, P2);
	GetS_SubSystem(S);
	VelPosSet();

	//------------
	// ���x����
	//------------
	{
		// �ݒ��n��
		GetP_SubSystem(P1, P2);
		GetS_SubSystem(S);
		VelXYAdd();	// �l��Ԃ�

	}

	// �ݒ��n��
	GetP_SubSystem(P1, P2);
	GetS_SubSystem(S);
	// �������Ă���ʒu�ύX
	TouchCheck();
}


// �X�e�[�g���e
void SetParam()
{
	Player P[2];
	Helper H[2][HM];


	count = 0;
	
	// �X�e�[�g
	do{
		// �v���C���[���𔻒f
		for (int i = 0; i < 2; i++){

			if (i == 0){
				P[0] = P1;
				P[1] = P2;
				for (int j = 0; j < HM; j++){
					H[0][j] = H1[j];
					H[1][j] = H2[j];
				}
			}
			else if (i == 1){
				P[0] = P2;
				P[1] = P1;
				for (int j = 0; j < HM; j++){
					H[0][j] = H2[j];
					H[1][j] = H1[j];
				}
			}

			// �X�e�[�g���s
			if ((count == 0) || (P[0].More == 1 && count > 0)){
				// ���O�̃X�e�[�g�Ɉړ����A�l��������Ă���
				switch (P[0].Name){
				case EIN:
					//�X�e�[�g�̐ݒ��K�p
					GetP_EinParam(P[0], P[1]);
					GetS_EinParam(S);
					GetH_EinParam(H[0], H[1]);
					EinParam();
					break;
				case CORNEL:
					//�X�e�[�g�̐ݒ��K�p
					GetP_CorParam(P[0], P[1]);
					GetS_CorParam(S);
					GetH_CorParam(H[0], H[1]);
					CorParam();
					break;
				case BOUNCER:
					//�X�e�[�g�̐ݒ��K�p
					GetP_BouParam(P[0], P[1]);
					GetS_BouParam(S);
					GetH_BouParam(H[0], H[1]);
					BouParam();
					break;
				case HELIOS:
					//�X�e�[�g�̐ݒ��K�p
					GetP_HelParam(P[0], P[1]);
					GetS_HelParam(S);
					GetH_HelParam(H[0], H[1]);
					HelParam();
					break;
				case HYDE:
					//�X�e�[�g�̐ݒ��K�p
					GetP_HydParam(P[0], P[1]);
					GetS_HydParam(S);
					GetH_HydParam(H[0], H[1]);
					HydParam();
					break;
				case SYUICHI:
					//�X�e�[�g�̐ݒ��K�p
					GetP_SyuParam(P[0], P[1]);
					GetS_SyuParam(S);
					GetH_SyuParam(H[0], H[1]);
					SyuParam();
					break;
				default:
					//�X�e�[�g�̐ݒ��K�p
					GetP_HydParam(P[0], P[1]);
					GetS_HydParam(S);
					GetH_HydParam(H[0], H[1]);
					HydParam();
					break;
				}	// switch
			}	// if
			
			
		}	// for

		count++;

	} while ((P1.More == 1) || (P2.More == 1));


}

// �X�e�[�g���e
void SetParam2()
{
	Player P[2];
	Helper H[2][HM];


	count = 0;

	// �X�e�[�g
	do{
		// �v���C���[���𔻒f
		for (int i = 0; i < 2; i++){
			if (i == 0){
				P[0] = P1;
				P[1] = P2;
				for (int j = 0; j < HM; j++){
					H[0][j] = H1[j];
					H[1][j] = H2[j];
				}
			}
			else if (i == 1){
				P[0] = P2;
				P[1] = P1;
				for (int j = 0; j < HM; j++){
					H[0][j] = H2[j];
					H[1][j] = H1[j];
				}
			}

			// �X�e�[�g���s

			if ((count == 0) || (P[0].More == 1 && count > 0)){
				// ���O�̃X�e�[�g�Ɉړ����A�l��������Ă���
				switch (P[0].Name){
				case EIN:
					//�X�e�[�g�̐ݒ��K�p
					GetP_EinParam(P[0], P[1]);
					GetS_EinParam(S);
					GetH_EinParam(H[0], H[1]);
					EinParam();
					break;
				case CORNEL:
					//�X�e�[�g�̐ݒ��K�p
					GetP_CorParam(P[0], P[1]);
					GetS_CorParam(S);
					GetH_CorParam(H[0], H[1]);
					CorParam();
					break;
				case BOUNCER:
					//�X�e�[�g�̐ݒ��K�p
					GetP_BouParam(P[0], P[1]);
					GetS_BouParam(S);
					GetH_BouParam(H[0], H[1]);
					BouParam();
					break;
				case HELIOS:
					//�X�e�[�g�̐ݒ��K�p
					GetP_HelParam(P[0], P[1]);
					GetS_HelParam(S);
					GetH_HelParam(H[0], H[1]);
					HelParam();
					break;
				case HYDE:
					//�X�e�[�g�̐ݒ��K�p
					GetP_HydParam(P[0], P[1]);
					GetS_HydParam(S);
					GetH_HydParam(H[0], H[1]);
					HydParam();
					break;
				default:
					//�X�e�[�g�̐ݒ��K�p
					GetP_HydParam(P[0], P[1]);
					GetS_HydParam(S);
					GetH_HydParam(H[0], H[1]);
					HydParam();
					break;
				}
			}
			
		}
		count++;

	} while ((P1.More == 1) || (P2.More == 1));


}

void DrawSetting()
{
	//TestText(P1.WHit[0]);
	// �v���Ɏg���l�𑗂�
	GetP_MainDraw(P1, P2);
	GetS_MainDraw(S);
	GetH_MainDraw(H1, H2);

	GetP_ObjectDraw(P1, P2);
	GetS_ObjectDraw(S);
	GetH_ObjectDraw(H1, H2);

	GetP_Effect(P1, P2);
	GetS_Effect(S);
	GetH_Effect(H1, H2);

}

void HitBoxSetting()
{
	// 1P
	GetP_GHitbox(P1);
	GetH_GHitbox(H1, H2);
	GHitbox();// OK!
	HelperKurai();

	// 2P
	GetP_GHitbox(P2);
	GetH_GHitbox(H2, H1);
	GHitbox();// OK!
	HelperKurai();
}

void TimeCount()
{
	Player p[2];
	Helper h[2][HM];
	p[0] = P1, p[1] = P2;
	for (int i = 0; i < HM; i++){
		h[0][i] = H1[i];
		h[1][i] = H2[i];
	}


	for (int i = 0; i < 2; i++){
		
		// NowTime
		if (p[i].StopTime == 0){
			p[i].time += 1;
		}

		// AnimTime
		p[i].AnimTime += 1;

		// StopTime
		if ((p[i].StopTime > 0) && (mOldStop[i] >= p[i].StopTime)){
			p[i].StopTime -= 1;
		}
		mOldStop[i] = p[i].StopTime;

		// �w���p�[
		for (int j = 0; j < HM; j++){
			if (h[i][j].var){

				// NowTime
				if (h[i][j].stopTime == 0){
					h[i][j].time += 1;
				}
				h[i][j].HAnimTime += 1;

				// StopTime
				if ((h[i][j].stopTime > 0) && (hOldStop[i][j] >= h[i][j].stopTime)){
					h[i][j].stopTime -= 1;
				}
				hOldStop[i][j] = h[i][j].stopTime;

			}
		}
		
	}

	P1 = p[0], P2 = p[1];
	
	for (int i = 0; i < HM; i++){
		H1[i] = h[0][i];
		H2[i] = h[1][i];
	}
	

	// �J��
	if (S.roundState == 0){
		// �u���b�N�A�E�g
		bo = 0;

		if (S.roundTime <= 0){
			// �{�^�������ĉ��o�ȗ�
			for (int i = 0; i < 4; i++){
				if (P_BInput(i + 1) == 1){
					P1.stateno = 0;
					P2.stateno = 0;
					
					/*
					//�������W���Z�b�g
					P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
					P1.YPos = GROUND, P2.YPos = GROUND;
					P1.XVel = 0, P2.XVel = 0;
					P1.YVel = 0, P2.YVel = 0;
					*/
				}
			}
		}

		// �����ֈڍs������
		if ((P1.stateno == 0) && (P2.stateno == 0))
		{
			if (S.roundTime == 0){
				S.noScroolX = false;
				S.noScroolY = false;
				//�������W���Z�b�g
				P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
				P1.YPos = GROUND, P2.YPos = GROUND;
				P1.XVel = 0, P2.XVel = 0;
				P1.YVel = 0, P2.YVel = 0;
			}
			// �V�X�e���֘A
			if (S.roundTime < 120){
				S.roundTime++;
			}
		// �ΐ��
			else if (S.roundTime >= 120){
				S.roundState = 1;
				S.roundTime = 0;
				S.roundTime = 60 * ROUNDTIME + 59;
				load_LifeCount();
			}
		}
	}
	// �ΐ풆
	else if (S.roundState == 1){
		//if (S.roundTime != 0)S.roundTime = 0;
		
		bo = 0;
		S.roundTime--;
		if (S.TraningFlg)S.roundTime = 60 * ROUNDTIME;	// �g�������͖���
		if (S.roundTime <= 0){
			if ((float)((float)P1.Life / (float)P1.C.lifeMax) > (float)((float)P2.Life / (float)P2.C.lifeMax)) 
				P2.Life = 0;
			else if ((float)((float)P2.Life / (float)P2.C.lifeMax) > (float)((float)P1.Life / (float)P1.C.lifeMax)) 
				P1.Life = 0;
			else if ((float)((float)P1.Life / (float)P1.C.lifeMax) == (float)((float)P2.Life / (float)P2.C.lifeMax));
			//P1.ctrl = 0, P2.ctrl = 0;
			//S.roundTime = 0;
			//S.StopTime = 20;
			// �ڍs
			//if ((P1.LifeC <= 0) || (P2.LifeC <= 0))S.roundState = 3;
			//else{ S.roundState = 2; }
		}
	}
	// ���E���h�擾
	else if (S.roundState == 2){
		
		// �������񓮂����~�߂�
		if (S.roundTime <= 0){
			if (P1.ctrl){
				P1.ctrl = 0;
				if (P1.SFlg == 2)P1.stateno = 46, P1.ignoreG = 0;
				else{ P1.stateno = 0; }
			}
			if (P2.ctrl){
				P2.ctrl = 0;
				if (P2.SFlg == 2)P2.stateno = 46, P2.ignoreG = 0;
				else{ P2.stateno = 0; }
			}

			// �ʒu�߂�����
			// �_�E����ɂ˂��ςȂ�
			if (P1.stateno == 1055){
				if ((P2.stateno != 0) && (P1.Life == 0) )P1.time = 0;
			}
			if (P2.stateno == 1055){
				if ((P1.stateno != 0) && (P2.Life == 0))P2.time = 0;
			}

			// ���݂������オ������
			if (P1.stateno == 0 && P2.stateno == 0){
				bo--;
				if (bo < -15){
					//SetBlack(255 - ((bo + 15) * -30));
				}
			}
		}

		// �����ł�����Ó]
		if ((P1.stateno == 0) && (P2.stateno == 1055)){
			bo--;
			//SetBlack(bo * -25);
			// �o�߂���܂ŐQ�Ă�
			P2.time = 1;
			// �߂�
			if (bo == -15){
				P1.time = 0, P2.time = 0;
				P1.stateno = 0, P2.stateno = 0;
				P1.muki = 0, P2.muki = 1;
				// �����ʒu�ɖ߂�
				S.ScroolX = ((STAGE_WIDTH / 2) - (SCREEN_W / 2));
				//�������W���Z�b�g
				P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
				P1.YPos = GROUND, P2.YPos = GROUND;
				P1.XVel = 0, P2.XVel = 0;
				P1.YVel = 0, P2.YVel = 0;

				P1.Life = P1.C.lifeMax;
				P2.Life = P2.C.lifeMax;
				P1.aGauge = GUARD_MAX;
				P2.aGauge = GUARD_MAX;
				P1.GRecovery = 0;
				P2.GRecovery = 0;
				P1.D.armor = 0;
				P2.D.armor = 0;
			}
		}
		else if ((P1.stateno == 1055) && (P2.stateno == 0)){
			bo--;
			//SetBlack(bo * -25);
			// �o�߂���܂ŐQ�Ă�
			P1.time = 1;
			if (bo == -15){
				P1.time = 0, P2.time = 0;
				P1.stateno = 0, P2.stateno = 0;
				P1.muki = 0, P2.muki = 1;
				// �����ʒu�ɖ߂�
				S.ScroolX = ((STAGE_WIDTH / 2) - (SCREEN_W / 2));
				//�������W���Z�b�g
				P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
				P1.YPos = GROUND, P2.YPos = GROUND;
				P1.XVel = 0, P2.XVel = 0;
				P1.YVel = 0, P2.YVel = 0;

				P1.Life = P1.C.lifeMax;
				P2.Life = P2.C.lifeMax;
				P1.aGauge = GUARD_MAX;
				P2.aGauge = GUARD_MAX;
				P1.GRecovery = 0;
				P2.GRecovery = 0;
				P1.D.armor = 0;
				P2.D.armor = 0;
			}
		}

		// �����ֈڍs���āA�܂��n�܂��ĂȂ�
		// roundTime��1�ȏ�ňړ��\�ɂ���
		if (((P1.Life > 0) && (P2.Life > 0))
			&& 
			(P1.stateno <= 199 && P2.stateno <= 199)
			&&
			(S.roundTime <= 0)
			&&
			(bo == -15))	// �J�n����
		{
			S.roundTime = 1;
			P1.stateno = 0, P2.stateno = 0;
			// �����ʒu�ɖ߂�
			/*
			S.ScroolX = ((STAGE_WIDTH / 2) - (SCREEN_W / 2));
			//�������W���Z�b�g
			P1.XPos = (STAGE_WIDTH / 2) - 120, P2.XPos = (STAGE_WIDTH / 2) + 120;
			P1.YPos = GROUND, P2.YPos = GROUND;
			P1.XVel = 0, P2.XVel = 0;
			P1.YVel = 0, P2.YVel = 0;
			*/
		}

		// �n�܂�����
		if (S.roundTime > 0)
		{
			// �ړ��݂̂ł��邩
			P1.ctrl = 0, P2.ctrl = 0;
			// �V�X�e���֘A
			if (S.roundTime < 50){
				S.roundTime++;
			}
			// �ΐ��
			else if (S.roundTime >= 50){
				S.roundState = 1;
				S.roundTime = 0;
				S.roundTime = 60 * ROUNDTIME + 59;
				load_LifeCount();
			}
		}

	}
	// �I��
	else if (S.roundState == 3){
		S.roundTime++;
		if (S.StopTime == 0)S.StopTime++;
		
		// ����
		if (S.roundTime == 1){
			if (P1.Life <= 0 && P2.Life <= 0){
				Result_WinSide(0, -1, -1);
				//Result_GetData(P1.Name, P2.Name, 1, 2, S.NowStage);
				WinCount(3);
			}
			else if (P1.Life <= 0){
				Result_WinSide(2, P2.Name, P1.Name);
				//Result_GetData(P1.Name, P2.Name, 1, 2, S.NowStage);
				WinCount(2);
			}
			else if (P2.Life <= 0){
				Result_WinSide(1, P1.Name, P2.Name);
				//Result_GetData(P1.Name, P2.Name, 1, 2, S.NowStage);
				WinCount(1);
			}
		}

		// �������~�߂�
		if (P1.ctrl){
			P1.ctrl = 0;
			//if (P1.SFlg == 2)P1.stateno = 46;
			//else{ P1.stateno = 0; }
		}
		if (P2.ctrl){
			P2.ctrl = 0;
			//if (P2.SFlg == 2)P2.stateno = 46;
			//else{ P2.stateno = 0; }
		}

		// �I���ֈڍs����
		if (S.roundTime >= 90){
			load_LifeCount();
			S.roundState = 0;
			S.roundTime = 0;
			Load_1 = 0;
			CharLoad();
			if (!S.TraningFlg){
				// �A�P�����ΐ킩
				if (Arcade_Switch(-1) == 0){
					ModeChange(GameScene(ResultScene));	// ���U���g
				}
				// �A�P��
				else if(Arcade_Switch(-1) == 1){
					if (P1.Life > 0){
						Arcade_BattleCount(1);	// ����
					}
					else{
						Arcade_BattleCount(0);	// ����
					}
					ModeChange(GameScene(ResultScene));	// �A�P�p�̃��U���g
				}
				// �Đ� or �^��
				if (Replay_Mode(-1) > 0){
					// �Đ�
					if (Replay_Mode(-1) == 1){
						ModeChange(GameScene(ReplayScene));	// ���U���g
					}
					Replay_End();
				}
			}
			else{
				ModeChange(GameScene(VersusScene));	// �ΐ��ʂփ��[�v
			}
		}
	}	// roundstate 3
}

void Load_State()
{  
	Load_1 = 0;
}                                                                        

// �摜�A����̃f�[�^��ǂݍ���
// Load_2�iair�j�͂����ŉ���
// air�̓ǂݍ��݂��ړI�ł͂Ȃ�
void LoadPict()
{
	
	// �J���[���h�~
	if ((P1.Name == P2.Name) && (P1.Palno == P2.Palno)){
		P2.Palno = 2;
		if (P1.Palno == 2)P2.Palno = 1;
	}

	// �A�j�����Ԏ擾
	//P1 = GetAnimElem(P1);
	//P2 = GetAnimElem(P2);

	// ����
	GetP_GHitbox(P1);
	load_GHitBox();	// air��air2�̓��e���Z�b�g
	GetP_GHitbox(P2);
	load_GHitBox();

	// �A�j�����Ԏ擾
	P1 = GetAnimElem(P1);
	P2 = GetAnimElem(P2);

	// �R�����X�e�[�^�X

	// �摜��ǂݍ���
	GetP1_Player(P1);	//����
	//load_pict(P1.Name, P1.PSide);
	Player_GraphLoad();

	GetP1_Player(P2);	//����
	//load_pict(P2.Name, P2.PSide);
	Player_GraphLoad();

}

void fLoad_State()
{
	if (!Load_2){

		// ���O�擾
		// LoadAllFile�ł���Ă݂�(����Ă�)
		GetName();
		Load1_Pict();	// vector���
		// air�擾
		for (int i = 0; i < Character_Max(); i++){
			load_air(i, 1);	// �A�j��@@
			load_pict(i, 1);	// �摜
								// �|�g���ǂݍ��ނ����Ȃ̂ŁA1P����ɂ���
		}

		// �g�����ݒ�ǂݍ���
		S.TSwitch[5] = GetPrivateProfileInt("Training", "data", 0, "char/tlist.ini");

		// ���[�h����
		Load_2 = true;
	}
}

void FirstPosSet(int n)
{
	fpSet = n;
}

void StopTime_Add(int n)
{
	S.StopTime += n;
}

void StopCmd(boolean cmd)
{
	stopCmd = cmd;
}

///////////////////////////////////////////////////////////////////////
// �g�����ݒ�̃Z�[�u
int Tlist_Save(void)
{
	FILE *fp;

	// �n�C�X�R�A�Z�[�u�p�t�@�C�����J��
	// (�Q�Ԗڂ̈�����"wb"��'w'�́u�������݁v�̈Ӗ�
	// 'b'�̓o�C�i���̈Ӗ�(�t�̓e�L�X�g))
	fopen_s(&fp, "char/tlist.ini", "wt");

	// �I�[�v���ł��Ȃ������炱���ŏI��
	if (fp == NULL){ return FALSE; }


	// �f�[�^�̏����o��
	//fwrite(In_BLog, sizeof(In_BLog), 1, fp);
	
	// [sizeof]�̓������T�C�Y�𒲂ׂ�
	//fprintf(fp, "\n");		// �Ȃ񂩍ŏ��̍s���������̂ł���g��
	{
		fprintf(fp, "[Training]\n");
		fprintf(fp, "data = %d\n", S.TSwitch[5]);
	}


	// �t�@�C�������
	fclose(fp);

	/* �ǂݍ��݃f�[�^�̊m�F */

	return TRUE;
}