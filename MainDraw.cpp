//==================================================================================
//
//			���l�v�Z�̊Ǘ���
//
//==================================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include <vector>

using namespace std;

//�f�t�@�C����----------------------------------------------

// #include "Define.h" �ɂāA�֐����Z�b�g


//�ް���`��------------------------------------------------

static int ScX, ScY;	// �������X�N���[����

static float m_PGraphX[2], m_PGraphY[2];	//�摜�̕\���ʒu
static int m_HGraphX, m_HGraphY;	// �w���p�[�\���ʒu
static float plusX[2], plusY[2];
static int m_Stage[STAGE_MAX];
static int Stage, sNum;		//�X�e�[�W�摜
static int stageBack[STAGE_MAX];	// �X�e�[�W�w�i

static int Load1, P1_Load;
static int LifeBar[2];		// ���C�t�o�[
//static int LifePoint;	// ��ʂɕ\������̗�

static int lpGauge;
//static int LifePoint[230];
static int lpGauge2;
//static int LifePoint2[230];

static int AG2;
static int AddGauge2[70];
//static vector<int> AddGauge2;

static int ag_sizeX, ag_sizeY;	// �A�h�Q�[�W�̃T�C�Y�H

static int port[2];
static int pName[2];
static int pnSize[2];

// �J�E���^�[�摜
static int m_LifeC;
static int PPort[2];
static int PP[2][2];

static int name[2];
static int nX[2], nY[2];

// �^�C���摜
static int number[10];

// �c���\��
static int mN = 0;
static int mTCount;
static int mOldXPos[6][2], mOldYPos[6][2];
static int mOldImage[6][2];
static int mOldMuki[6][2];
static int mTime[2];
static int mColor[3][2];

// �̗͕\��
static int drawLPos = 10;	// �̗̓o�[�̂��炷�ʒu
static float lifeMax[2];	// p1.c.lifemax
static float nLife[2];	// p1.life
static float ab[2];	// �ۑ�
static float hp[2];	// ""
static float ap[2];	// All_Damage
static int SLife[2];	// ��炤�O�̗̑�
static int allDamage[2];	// �������_���[�W��
static int oldLife[2];
static boolean oldHFlg[2];

// �摜�̌���
static boolean HV[2];


//�����֐��錾��--------------------------------------------

void CharGraph();
void P1Graph();
void P2Graph();
void LifeGraph();
void HelperGraph();

//�v���O������----------------------------------------------


//�f�[�^����n�֐�

//-------------------------------------------------------------------------------
//	��`�@int EinImage
//
//	�����@�����ŉ摜�����������ق������������A�X�e�[�g�E���ԁ@
//  �ɍ��킹�ČĂԉ摜�����߂�i���ԍ��E�c�ԍ��j�@�̂ق�����������
//
//	�����@
//
//	�߂�l�@int �摜�̏c�ԍ�
//-------------------------------------------------------------------------------



void MainDraw()
{

	if (Load1 == 0){
		//���C�t�o�[�̉摜
		LoadDivGraph("ob/lifeBar.png", 2, 1, 2, 640, 60, LifeBar);
		
		m_LifeC = LoadGraph("ob/recover.png");	//���J�o�[�̉摜

		for (int i = 0; i < sizeof(AddGauge2) / sizeof(AddGauge2[0]); i++){
			AddGauge2[i] = 0;
		}
		AG2 = LoadGraph("ob/addGauge.png");
		//LoadDivGraph("ob/AddGauge.png", 37, 37, 1, 1, 25, AddGauge);	//�A�h�Q�̉摜
		LoadDivGraph("ob/addGauge.png", 70, 70, 1, 1, 13, AddGauge2);	//�A�h�Q�̉摜

		
		//LoadDivGraph("ob/LifePoint.png", 230, 230, 1, 1, 18, LifePoint);	//raifu�̉摜
		lpGauge = LoadGraph("ob/LifePoint.png");	//

		lpGauge2 = LoadGraph("ob/LifePoint2.png");	//
		//LoadDivGraph("ob/LifePoint2.png", 230, 230, 1, 1, 18, LifePoint2);	//�A�h�Q�̉摜
		
		LoadDivGraph("ob/PPort.png", 2, 1, 2, 62, 62, PPort);
		LoadDivGraph("ob/number.png", 10, 5, 2, 25, 30, number);

		if (STAGE_MAX >= 1)stageBack[0] = LoadGraph("back/st1_2.png");
		if (STAGE_MAX >= 2)stageBack[1] = LoadGraph("back/st2_2.png");
		if (STAGE_MAX >= 3)stageBack[2] = LoadGraph("back/st3_2.png");
		if (STAGE_MAX >= 4)stageBack[3] = LoadGraph("back/st4_2.png");

		//GetGraphSize(AddGauge, &ag_sizeX, 0);
		ag_sizeX = 38;
		ag_sizeY = 25;

		Load1 = 1;
	}


	// �X�N���[���E�w�i
	{
		// ���ۂ̃X�N���[���Ƀ|�C���g����
		// �ʒu�������߂�������
		ScX = (int)S.ScroolX;
		ScY = (int)S.ScroolY;

		// �X�e�w�i
		if (stageBack[sNum] != 0){
			DrawGraph(0 - (int)((float)ScX * 0.25) + S.quakeX, 0 - (int)((float)ScY * 0.25) + S.quakeY,
				stageBack[sNum], false);
			
		}
		// �X�e�[�W�`��
		//DrawGraph(0 - ScX, 0 - ScY + S.quakeY, Stage, true);
		DrawRotaGraph((SCREEN_W / 2) - ScX + S.quakeX + (SCREEN_W) / 2,
		((STAGE_HEIGHT - SCREEN_H) / 2) - ScY + S.quakeY + (SCREEN_H) / 2,
			1.0, 0, Stage, true);

		//DrawRotaGraph((SCREEN_W / 2) - ScX + (SCREEN_W + S.camSize) / 2, 
			//((STAGE_HEIGHT - SCREEN_H) / 2) - ScY + S.quakeY + (SCREEN_H + S.camSize) / 2,
			//S.camExRate, 0, Stage, true);
	}

	//---------------
	// ���o�i�Ó]�j
	//---------------
	{
		if(S.Anten > 0)BackGraph();
	}
	
	{
		// �J���͏���
		if (!(S.roundState == 0 && S.roundTime == 0) && (S.noGauge == 0)){
		/**************
		* ���C�t�J�E���^�[
		**************/

		// 1P, 2P
		for (int i = 0; i < LIFECOUNT_MAX; i++){
			// ���ʂ̃��E���h
			/*
			if (P2.LifeC < LIFECOUNT_MAX){
				DrawGraph(SCREEN_W / 2 - 30, 35 + drawLPos, m_LifeC, true);
			}
			if (P1.LifeC < LIFECOUNT_MAX){
				DrawGraph(SCREEN_W / 2 + 6, 35 + drawLPos, m_LifeC, true);
			}
			*/
			
			if (P1.LifeC - 1 > i){
				DrawGraph(SCREEN_W / 2 - 30 - (i * 30), 35 + drawLPos, m_LifeC, true);
			}
			if (P2.LifeC - 1 > i){
				DrawGraph(SCREEN_W / 2 + 6 + (i * 30), 35 + drawLPos, m_LifeC, true);
			}
			
		}
		/**************
		* ���C�t�o�[
		**************/

		//============
		// �|�[�g���C�g
		//============		
		// �������
		{
			// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
			SetAlpha(125);
			DrawGraph(0, 0, PPort[1], true);
			DrawTurnGraph(SCREEN_W - 62, 0, PPort[1], true);

			SetAlpha(230);
			// �A�h�Q�[�W
			if (P1.addG > 0){
				DrawBox(0, (int)(60 - (P1.addG * 0.6)), 60, 60, GetColor(205 + (P1.addG/2), 10, 10), true);
			}
			// �A�h�Q�[�W
			if (P2.addG > 0){
				DrawBox(SCREEN_W - 60, (int)(60 - (P2.addG * 0.6)), SCREEN_W, 60, GetColor(205 + (P2.addG/2), 10, 10), true);
			}

			// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		
		//
		DrawGraph(0, 0, port[0], true);
		DrawTurnGraph(SCREEN_W - 60, 0, port[1], true);

		//
		DrawGraph(0, 0, PPort[0], true);
		DrawTurnGraph(SCREEN_W - 62, 0, PPort[0], true);

		// �l�[���v���[�g
		SetAlpha(85);
		DrawBox(62, 56 - nY[0], 63 + nX[0] + 1, 57 + 1, GetColor(0, 0, 0), true);
		DrawBox(SCREEN_W - 62 - nX[1] - 2, 56 - nY[0], SCREEN_W - 62, 57 + 1, GetColor(0, 0, 0), true);
		NoBlend();
		// ���O�\��
		DrawGraph(63, 57 - nY[0], name[0], true);
		DrawGraph(SCREEN_W - 63 - nX[1], 57 - nY[1], name[1], true);

		// ���C�t�o�[�\��
		LifeGraph();
		
		//========
		// �^�C��
		//========
		//DrawFormatString(310, 18, Cr, "%d", S.roundTime / 60);
		if ((S.roundState == 1) || (S.roundState == 3) || (S.roundState == 2)){
			if (S.roundTime >= 0){
				DrawGraph(320 - 25 + 3, 14, number[(S.roundTime / 10) / 60], true);
				int time = (S.roundTime / 60);
				while (time > 9){
					time -= 10;
				}
				DrawGraph(320 - 3, 14, number[time], true);
			}
			else if (S.roundTime < 0)
			{
				DrawGraph(320 - 25 + 3, 14, number[0], true);
				DrawGraph(320 - 3, 14, number[0], true);
			}
		}
		else if (S.roundState == 0){
			DrawGraph(320 - 25 + 3, 14, number[9], true);
			DrawGraph(320 - 3, 14, number[9], true);
		}

		//DrawRotaGraph3F(319 - (float)ag_sizeX * ((float)P1.aGauge / 100.0), 6 + (float)drawLPos, 
			//0, 0, (float)P1.aGauge / 100.0, 1, 0, AddGauge, true);
		//DrawRotaGraph3F(320, 6 + (float)drawLPos, 0, 0, (float)P2.aGauge / 100.0, 1, 0, AddGauge, true, true);
		/*
		if(P1.aGauge > 0)
			DrawFormatString(SCREEN_W / 2-20, 14, Cr, "%d", P1.aGauge/10+1);
		else{ DrawFormatString(SCREEN_W / 2 - 20, 14, Cr, "%d", 0); }
		if(P2.aGauge > 0)
			DrawFormatString(SCREEN_W / 2+10, 14, Cr, "%d", P2.aGauge/10+1);
		else{ DrawFormatString(SCREEN_W / 2 + 10, 14, Cr, "%d", 0); }
		*/
			
		} // �J��
	}
	
	{
		/**************
		* �G�t�F�N�g(�L�����̌��)
		**************/
		{
			EffDrawB();
		}
		//********
		// �L����
		//********
		{
			CharGraph();
		
		}
		//**********
		// �w���p�[
		//**********
		{
			HelperGraph();
		}
		
	}
}


void GetStageNum(int num, int stage)
{
	//�w�i�̉摜���[�h
	{
		if (STAGE_MAX >= 1)m_Stage[0] = LoadGraph("back/st1.png");
		if (STAGE_MAX >= 2)m_Stage[1] = LoadGraph("back/st2.png");
		if (STAGE_MAX >= 3)m_Stage[2] = LoadGraph("back/st3.png");
		if (STAGE_MAX >= 4)m_Stage[3] = LoadGraph("back/st4.png");
	}

	Stage = m_Stage[num];
	//Stage = stage;
	sNum = num;
}

void MainDraw_GetPort(int n, int pside)
{
	// �摜�擾
	port[pside - 1] = n;

}

void MainDraw_GetName(int n, int pside)
{
	// ���O�擾
	name[pside - 1] = n;
	GetGraphSize(name[pside - 1], &nX[pside - 1], &nY[pside - 1]);
}


void GetP_MainDraw(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_MainDraw(System_t GS)
{
	S = GS;
}

void GetH_MainDraw(Helper GH1[], Helper GH2[])
{
	int i;
		for (i = 0; i < HM; i++){
			H1[i] = GH1[i];
			H2[i] = GH2[i];
		}
}


void CharGraph()
{
	// �ʒu�ɒǉ�
	Player P[2];

	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){

		// ���E����
		if ((P[i].muki == 0 && P[i].aHV == 1) ||
			(P[i].muki == 1 && P[i].aHV == 0))HV[i] = true;
		else { HV[i] = false; }

		plusX[i] = (double)(((P[i].GraphW / 2) * P[i].GSize) + ((-P[i].sPosX * P[i].GSize) + (P[i].aPosX * P[i].GSize)));

		if (HV[i])plusX[i] =
			(float)(((-P[i].GraphW / 2) * P[i].GSize) + ((P[i].sPosX * P[i].GSize) + (-P[i].aPosX * P[i].GSize)));

		plusY[i] = (double)(P[i].GraphH / 2.0) * P[i].GSize;

		//GraphX,Y�̐ݒ�
		m_PGraphX[i] = (double)P[i].XPos - ScX;
		m_PGraphY[i] = (double)(P[i].YPos - ScY //+ (plusY[i])
			+ ((-P[i].sPosY + P[i].aPosY) * P[i].GSize));

		// �����_�̍��ق��C��
		if (P[i].GraphW % 2 != 0){
			if (i == 0){
				if (HV[i] == 0)
					m_PGraphX[i] += (double)(P[i].GSize/2.0);
				else{ m_PGraphX[i] -= (double)(P[i].GSize / 2.0); }
			}
			else if (i == 1){
				if (HV[i] == 0)
					m_PGraphX[i] += (double)(P[i].GSize / 2.0);
				else{ m_PGraphX[i] -= (double)(P[i].GSize / 2.0); }
			}
		}
		//if (P[i].GraphH % 2 != 0)m_PGraphY[i] += (float)(P[i].GSize / 2.0);

		// �摜�D��x
		if (P[i].stateno >= 200 && P[i].stateno <= 999)P[i].SprPriority = 3;
		else if (P[i].stateno >= 1 && P[i].stateno <= 39)P[i].SprPriority = 1;
		else if (P[i].stateno >= 40 && P[i].stateno <= 59)P[i].SprPriority = 1;
		else if (P[i].stateno >= 1000)P[i].SprPriority = -1;
		else { P[i].SprPriority = 0; }

	}
	P1 = P[0], P2 = P[1];


	// ���ԃJ�E���g�A30�t���Ń��[�v
	mTCount += 1;
	if (mTCount > 30)mTCount = 1, mN = 0;
	
	P[0] = P1, P[1] = P2;
	// �c���ݒ�
	for (int i = 0; i < 2; i++){
		if (mTCount % 6 == 0){
			if (mTime[i] > 0)mOldImage[mN][i] = (int)P[i].image;
			else{ mOldImage[mN][i] = 0; }
			mOldXPos[mN][i] = (int)(m_PGraphX[i] + plusX[i]) + ScX;
			mOldYPos[mN][i] = (int)(m_PGraphY[i] + plusY[i]) + ScY;
			mOldMuki[mN][i] = HV[i];
		}
	}

	if (mTCount % 6 == 0)mN += 1;

	P1 = P[0], P2 = P[1];


	//2P���O
	if (P1.SprPriority < P2.SprPriority)
	{
		if (P1.image != 0)
		P1Graph();
		if (P2.image != 0)
		P2Graph();
	}
	//1P���O
	else
	{
		if (P2.image != 0)
		P2Graph();
		if (P1.image != 0)
		P1Graph();
	}
	// ���ɖ߂������ꍇ��(*2)������

}

void HelperGraph()
{
	// �ʒu�ɒǉ�
	Helper H[2][HM];
	boolean HV[2];
	int plusX[2];


	//
	for (int j = 0; j < HM; j++){
		H[0][j] = H1[j];
		H[1][j] = H2[j];
		if (H[0][j].GSize == 0)H[0][j].GSize = 1;
		if (H[1][j].GSize == 0)H[1][j].GSize = 1;
	}

	//P1
	for (int h = 0; h < 2; h++){
		for (int i = 0; i < HM; i++){
		if (H[h][i].var){	// �w���p�[����Ȃ�
			// ���E����
			if ((H[h][i].muki == 0 && H[h][i].aHV == 1) ||
				(H[h][i].muki == 1 && H[h][i].aHV == 0))HV[h] = true;
			else { HV[h] = false; }
			
			plusX[h] = (int)(((H[h][i].HGraphW / 2) * H[h][i].GSize) + ((-H[h][i].sPosX + H[h][i].aPosX) * H[h][i].GSize));
			if (HV[h])plusX[h] =
				(int)(((-H[h][i].HGraphW / 2) * H[h][i].GSize) + ((H[h][i].sPosX + H[h][i].aPosX) * H[h][i].GSize));
			
			
			//GraphX,Y�̐ݒ�
			m_HGraphX = (int)H[h][i].XPos - ScX + plusX[h];
			m_HGraphY = (int)(H[h][i].YPos - ScY + ((H[h][i].HGraphH / 2) * H[h][i].GSize)
				+ ((-H[h][i].sPosY + H[h][i].aPosY) * H[h][i].GSize));

			
			if (H[h][i].Alpha){
				// �`��u�����h���[�h���i���Z�j�ɂ���
				SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
			}

			// �`��i+12�őO�̂��g����H�j
			if (H[h][i].image != 0)
			DrawRotaGraph(m_HGraphX + S.quakeX, m_HGraphY + S.quakeY, H[h][i].GSize, 0, H[h][i].image, true, H[h][i].muki);

			if (H[h][i].Alpha){
				// �`��u�����h���[�h���m�[�u�����h�ɂ���
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			
		}	// if
		
		}	// for
	}
	
}

void P1Graph()
{
	// �c��
	if (mTime[0] > 0 && S.StopTime == 0)
	{
		SetDrawBright(mColor[0][0], mColor[1][0], mColor[2][0]);
		// �c����
		for (int i = 0; i < 6; i++){
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 - i * 30);

			DrawRotaGraph(mOldXPos[i][0] - ScX + S.quakeX, mOldYPos[i][0] - ScY + S.quakeY, P1.GSize, 0, mOldImage[i][0], true, mOldMuki[i][0]);

		}
		SetDrawBright(255, 255, 255);
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		mTime[0]--;
	}

	// �󂯐g
	/*
	if (P1.stateno == 1085 && P1.time <= 4){
		// �`��u�����h���[�h���i���Z�j�ɂ���
		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	}
	*/
	// �`��i+12�őO�̂��g����j
	//DrawRotaGraph(m_PGraphX[0], m_PGraphY[0], P1.GSize, 0, P1.Image, true, P1.muki + P1.turnG);
	//DrawRotaGraph((int)m_PGraphX[0], (int)m_PGraphY[0], P1.GSize, 0, P1.Image, true, HV[0]);

	// �J���[�ύX
	if (P1.colorCTime > 0){
		// ��������
		if ((P1.colorC[0] < 0) && (P1.colorC[1] < 0) && (P1.colorC[2] < 0)){
			SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
			DrawRotaGraph((int)(m_PGraphX[0] + plusX[0] + S.quakeX), (int)(m_PGraphY[0] + plusY[0] + S.quakeY), P1.GSize, 0, P1.image, true, HV[0]);
			SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		}
		// �ʂ̐F��
		else{
			SetDrawBright(P1.colorC[0], P1.colorC[1], P1.colorC[2]);
		}
	}
	
	// �J���[�ύX�Ȃ�
	DrawRotaGraph((int)(m_PGraphX[0] + plusX[0] + S.quakeX), (int)(m_PGraphY[0] + plusY[0] + S.quakeY), P1.GSize, 0, P1.image, true, HV[0]);

	// �J���[�ύX���Ă���ݒ����
	if (P1.colorCTime > 0){
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawBright(255, 255, 255);
	}
	//DrawBox(m_PGraphX[0] - 1, m_PGraphY[0] - 1, m_PGraphX[0] + 1, m_PGraphY[0] + 1, Oran, true);
	//DrawRotaGraphF(m_PGraphX[0] + plusX[0], m_PGraphY[0] + plusY[0], P1.GSize, 0, P1.Image, true, HV[0]);
	
}


void P2Graph()
{
	if (mTime[1] > 0 && S.StopTime == 0)
	{
		// �c��
		SetDrawBright(mColor[0][1], mColor[1][1], mColor[2][1]);
		for (int i = 0; i < 6; i++){
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 - i * 30);

			DrawRotaGraph(mOldXPos[i][1] - ScX + S.quakeX, mOldYPos[i][1] - ScY + S.quakeY, P2.GSize, 0, mOldImage[i][1], true, mOldMuki[i][1]);

		}
		SetDrawBright(255, 255, 255);
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		mTime[1]--;
	}


	if (P2.colorCTime > 0){
		if ((P2.colorC[0] == -1) && (P2.colorC[1] == -1) && (P2.colorC[2] == -1)){
			SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
			// ���ꂪ�Ȃ��Ɣ����Ȃ�Ȃ�
			DrawRotaGraph((int)(int)(m_PGraphX[1] + plusX[1]), (int)(m_PGraphY[1] + plusY[1] + S.quakeY), P2.GSize, 0, P2.image, true, HV[1]);
			SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		}
		else{
			SetDrawBright(P2.colorC[0], P2.colorC[1], P2.colorC[2]);
		}
	}

	// ���ꂩ��ς���
	DrawRotaGraph((int)(int)(m_PGraphX[1] + plusX[1] + S.quakeX), (int)(m_PGraphY[1] + plusY[1] + S.quakeY), P2.GSize, 0, P2.image, true, HV[1]);
	
	if (P2.colorCTime > 0){
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawBright(255, 255, 255);
	}

}


void LifeGraph()
{
	float base = 230.0;
	float ff[2];
	Player P[2];
	P[0] = P1, P[1] = P2;
	
	for (int i = 0; i < 2; i++){
		lifeMax[i] = (float)P[i].C.lifeMax;
		nLife[i] = (float)P[i].Life;
		
		ff[i] = (float)(P[i].C.lifeMax / base);

		ab[i] = (lifeMax[i] / ff[i]);
		hp[i] = (nLife[i] / ab[i] * base);
	}
	P1 = P[0], P2 = P[1];

	/*
	float lifeMax = 1200.0;
	float nlife = 900.0;
	float a;
	float base = 1000.0;
	float base2 = 250.0;

	a = (float)(lifeMax / 4.0);

	printf("MaxLife = %.1f\n", lifeMax / a * base2);
	printf("NowLife = %.1f\n", nlife / a * base2);
	*/
	
	// ���C�t�o�[���`��
	{
		// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 188);
		// ���C�t�o�[���\��
		DrawGraph((SCREEN_W - 640) / 2, drawLPos, LifeBar[1], true);
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	

	/*
	//���C�t��
	DrawRotaGraph3F((SCREEN_W / 2) - (hp[0] / ff[0]) - 28, 7 + (float)drawLPos, 0, 0, 
		(hp[0] / ff[0]), 1, 0, LifePoint, true, false);
	DrawRotaGraph3F((SCREEN_W / 2) + 28, 7 + (float)drawLPos, 0, 0, 
		(hp[1] / ff[1]), 1, 0, LifePoint, true, false);
		*/

	// [���C�t������]
		{
			P[0] = P1, P[1] = P2;

			

			for (int i = 0; i < 2; i++){
				

				// P[i]���v�_���[�W
				// �ő�l�ɏ����ݒ肷��
				if (P[i].Life == P[i].C.lifeMax)SLife[i] = P[i].Life;
				// �s�k���Ƀ��J�o�������炷
				if ((P[i].Life <= 0) && (P[i].LifeC == 0)){
					if (SLife[i] > P[i].Life){ SLife[i] -= 6; }
					else if (SLife[i] <= P[i].Life)SLife[i] = P[i].Life;
				}
				if (P[i].HFlg){
					// ��炤�O�̃��C�t�ƌ��݂̃��C�t���r
					allDamage[i] = SLife[i] - P[i].Life;
				}
				// ��炢����Ȃ����̓��J�o���Q�[�W�����炷
				else if (P[i].HFlg == 0){
					if (allDamage[i] > 0){ allDamage[i] -= 6; }
					else if (allDamage[i] <= 0)allDamage[i] = 0;

					if (SLife[i] > P[i].Life){ SLife[i] -= 6; }
					else if (SLife[i] <= P[i].Life)SLife[i] = P[i].Life;
				}

				while (1){
					if (P[i].Life + allDamage[i] >= P[i].C.lifeMax)allDamage[i] -= 1;
					else{ break; }
				}
				
				

				ap[i] = (SLife[i] - allDamage[i] / ab[i] * base);
			}

			
			if (P2.HFlg == 1 && oldHFlg[1] == 0 && P2.stateno >= 1000){
				SLife[1] = oldLife[1];
			}
			if (P1.HFlg == 1 && oldHFlg[0] == 0 && P1.stateno >= 1000){
				SLife[0] = oldLife[0];
			}
			

			// �O�̃��C�t�L��
			oldLife[0] = P[0].Life;
			oldLife[1] = P[1].Life;
			oldHFlg[0] = P[0].HFlg;
			oldHFlg[1] = P[1].HFlg;

			P1 = P[0], P2 = P[1];

			/*
			// P1���v�_���[�W
			// �ő�l�ɏ����ݒ肷��
			if (P[i].Life == P[i].C.lifeMax)SLife[i] = P[i].Life;
			// �s�k���Ƀ��J�o�������炷
			if ((P[i].Life <= 0) && (P[i].LifeC == 0)){
			SLife[i] = P[i].Life;
			}

			if (P[i].HFlg){
			// ��炤�O�̃��C�t�ƌ��݂̃��C�t���r
			SLife[i] = SLife[i] - P[i].Life;
			}
			// ��炢����Ȃ����̓��J�o���Q�[�W�����炷
			// allDamage��SLife�����炵�Ă���
			else if (P[i].HFlg == 0){
			if (allDamage[i] > 0){ allDamage[i] -= 6; }
			else if (allDamage[i] <= 0)allDamage[i] = 0;

			if (SLife[i] > P[i].Life){ SLife[i] -= 6; }
			else if (SLife[i] <= P[i].Life)SLife[i] = P[i].Life;
			}

			// �g�����p
			while (1){
			if (P[i].Life + allDamage[i] >= P[i].C.lifeMax)allDamage[i] -= 1;
			else{ break; }
			}
			*/

			//ap[0] = (allDamage[0] / ab[0] * base);
			//ap[1] = (allDamage[1] / ab[1] * base);

			// �`��u�����h���[�h���i���Z�j�ɂ���
			//SetDrawBlendMode(DX_BLENDMODE_ADD, (255));

			/*
			if (P1.Life > 0){
				//�����ʁ@1P
				for (int i = (int)((hp[0] / ff[0])) + 0; i < ((SLife[0] / ab[0] * base) / ff[0]); i++){
					DrawGraph((SCREEN_W/2) - 1 - i - 26, 7 + drawLPos, LifePoint2[229 - i], true);
				}
			}
			
			if (P2.Life > 0){
				//�����ʁ@2P	// +0��1�ɂ��Ă��炽�܂ɕςɂȂ���
				for (int i = (int)((hp[1] / ff[1])) + 0; i < ((SLife[1] / ab[1] * base) / ff[1]); i++){
					DrawGraph((SCREEN_W / 2) + i + 26, 7 + drawLPos, LifePoint2[229 - i], true);
				}
			}
			*/

			if (P1.Life > 0){
				DrawRectGraph((SCREEN_W / 2) - 1 - ((SLife[0] / ab[0] * base) / ff[0]) - 26, 7 + drawLPos,
					230 - ((SLife[0] / ab[0] * base) / ff[0]), 0, ((SLife[0] / ab[0] * base) / ff[0]), 18,
					lpGauge2, true, false);
			}
			if (P2.Life > 0){
				DrawRectGraph((SCREEN_W / 2) + 1 + 26, 7 + drawLPos,
					230 - ((SLife[1] / ab[1] * base) / ff[1]), 0, ((SLife[1] / ab[1] * base) / ff[1]), 18,
					lpGauge2, true, true);
			}
			// [ ���C�t�`�� ]
			/*
			for (int i = 0; i < (hp[0] / ff[0]); i++){
				DrawGraph((SCREEN_W / 2) - 1 - i - 26, 7 + drawLPos, LifePoint[229 - i], true);
			}
			for (int i = 0; i < (hp[1] / ff[1]); i++){
				DrawGraph((SCREEN_W / 2) + i + 26, 7 + drawLPos, LifePoint[229 - i], true);
			}
			*/
			

			DrawRectGraph((SCREEN_W / 2) - 1 - (hp[0] / ff[0]) - 26, 7 + drawLPos,
				230 - (hp[0] / ff[0]), 0, (hp[0] / ff[0]), 18,
				lpGauge, true, false);
			DrawRectGraph((SCREEN_W / 2) + 1 + 26, 7 + drawLPos,
				230 - (hp[1] / ff[1]), 0, (hp[1] / ff[1]), 18,
				lpGauge, true, true);
			// �`��u�����h���[�h���m�[�u�����h�ɂ���
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		//=============
		// �K�[�h�Q�[�W
		//=============

		// �����w�i
		SetDrawBright(30, 30, 30);
		// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

		//DrawGraph(((SCREEN_W - 640) / 2) + 320 - 37 - 1, 5 + drawLPos, AG, true);
		//DrawTurnGraph(((SCREEN_W - 640) / 2) + 320 + 1, 5 + drawLPos, AG, true);
		DrawGraph((SCREEN_W / 2) + 37 + 1, drawLPos + 34 + 3, AG2, true);
		DrawGraph((SCREEN_W / 2) - 37 - 70 - 1, drawLPos + 34 + 3, AG2, true);
		
		SetDrawBright(255, 255, 255);
		
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//==
		// �K�[�h�Q�[�W�`��
		//==
		if (P1.aGauge > 0){
			SetDrawBright(100, 100, 255);
			for (int i = 0; i < (int)(0.70 * (P1.aGauge / 10)); i++){
				DrawRotaGraph((SCREEN_W / 2) - 38 - i - 1, drawLPos + 39 + 3, 1, 0, AddGauge2[69 - i], true);
			}
		}
		else{
			SetDrawBright(200, 30, 30);
			for (int i = 0; i < (int)(0.70 * (P1.GRecovery / 10)); i++){
				DrawRotaGraph((SCREEN_W / 2) - 38 - i - 1, drawLPos + 39 + 3, 1, 0, AddGauge2[69 - i], true);
			}
			// �~�}�[�N
			SetDrawBright(255, 255, 255);
			DrawLine((SCREEN_W / 2) - 38 - 1 - 70, drawLPos + 41 - 6, (SCREEN_W / 2) - 38 - 1, drawLPos + 41 + 7, Cr);
			DrawLine((SCREEN_W / 2) - 38 - 1 - 70, drawLPos + 41 + 7, (SCREEN_W / 2) - 38 - 1, drawLPos + 41 - 6, Cr);
		}
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBright(255, 255, 255);

		if (P2.aGauge > 0){
			SetDrawBright(100, 100, 255);
			for (int i = 0; i < (int)(0.70 * (P2.aGauge / 10)); i++){
				DrawRotaGraph((SCREEN_W / 2) + 38 + i, drawLPos + 39 + 3, 1, 0, AddGauge2[69 - i], true);
			}
		}
		else{
			SetDrawBright(200, 30, 30);
			for (int i = 0; i < (int)(0.70 * (P2.GRecovery / 10)); i++){
				DrawRotaGraph((SCREEN_W / 2) + 38 + i, drawLPos + 39 + 3, 1, 0, AddGauge2[69 - i], true);
			}
			// �~�}�[�N
			SetDrawBright(255, 255, 255);
			DrawLine((SCREEN_W / 2) + 38 - 1, drawLPos + 41 - 6, (SCREEN_W / 2) + 38 + 70, drawLPos + 41 + 7, Cr);
			DrawLine((SCREEN_W / 2) + 38, drawLPos + 41 + 7, (SCREEN_W / 2) + 38 + 70, drawLPos + 41 - 6, Cr);
		}
		SetDrawBright(255, 255, 255);

		//================
		// ���C�t�o�[�\��
		//===============
		DrawGraph((SCREEN_W - 640) / 2, drawLPos, LifeBar[0], true);
}

void afterimageSetting(int side, int time, int r, int g, int b)
{
	mTime[side - 1] = time;
	mColor[0][side - 1] = r;
	mColor[1][side - 1] = g;
	mColor[2][side - 1] = b;
}

