#include "DxLib.h"

#ifndef DEF_DEFINE2_H	//��x��`����Ă�����A��`���Ȃ�
#define DEF_DEFINE2_H

//--------------
// �f�B�t�@�C��
//--------------
#define TIMENO	(P1.time)		// ���Ԑݒ�
#define HI	(H1[i])
#define ANIMELEM P1.animElem[P1.stateno]

////////////////


////////////////
static void HitVel(double X1, double Y1, double X2, double Y2);
static void GuardVel(double X1, double X2);
static void SESet(int H, int G);
static void Damage(int i, int j);
static void Power(int i);
static void Power(int i, int j);

static boolean triggerT(int i, int j);	// 0.==  1.>=  2.<=
static int TimeNo(int t1, int t2);
static void HelperReset(int i);	// �w���p�[�ݒ��������
static void H2_AttSet(int i, int j, int W, int H, int X, int Y);
// ���W�Z�b�g
static void H_PosSet(int i, double X, double Y);
// ���x�Z�b�g
static void H_VelSet(int i, double X, double Y);
static void H_HitVel(int i, double X1, double Y1, double X2, double Y2);
static void H_HitTime(int i, int HS, int GH, int AH, int GG);
// ���x�ǂ���Ɉړ�
static void H_Move(int i);
// i.�ԍ�  j.����  k 0.==  1.>=  2.<=
static boolean H_triggerT(int i, int j, int k);
static void HitTime(int HS, int GH, int AH, int GG);

static void DamReset();
// ��������
static void ThrowReject();

// �q�b�g�G�t�F�N�g
static void HitEff(int g, double x, double y);
static void h_HitEff(int i, int g, double x, double y);

// ���x�Z�b�g
static void VelSet(double X, double Y);
// ���x���Z
static void VelAdd(double X, double Y);
// �ʒu�Z�b�g
static void PosAdd(double X, double Y);
// ���n����
static boolean Chakuchi();
// �X�e�Q�����߂ĂȂ�����
static boolean AnimElem(int state2);
static int InputPAD(int num);
// ��s���͂܂��͂������ώ󂯕t��
static boolean key(int num);

//-------------------------------------------------------------------------------
//
// �X�e�[�g�ݒ�
//
//-------------------------------------------------------------------------------

int InputPAD(int num)
{
	int kekka = 0;

	if (P1.PSide == 1){
		kekka = P1_BInput(num);
	}
	else if (P1.PSide == 2){
		kekka = P2_BInput(num);
	}

	return kekka;
}

boolean key(int num)
{
	boolean kekka = false;

	if (P1.K_Senkou[num] > 0){
		kekka = true;
	}
	else if(P1.key[num] > 0){
		kekka = true;
	}
	
	return kekka;
}

void SESet(int H, int G)
{
	P1.HitSE = H;
	//P1.G_GuardTime = G;
}




void HitVel(double X1, double Y1, double X2, double Y2)
{
	P1.GX = X1, P1.GY = Y1;
	P1.A.guard_gx = X1, P1.A.guard_ax = X2 * 1.5;
	P1.AX = X2, P1.AY = Y2;
}

void GuardVel(double X1, double X2)
{
	P1.A.guard_gx = X1, P1.A.guard_ax = X2;
}

void HitTime(int HS, int GH, int AH, int GG)
{
	P1.HitStop = HS;
	P1.HSSelf = HS;
	P1.G_HitTime = GH, P1.A_HitTime = AH;
	P1.G_GuardTime = GG;
	P1.A.A_GuardTime = GG;
}

void Damage(int i, int j)
{
	P1.A.damage = i, P1.A.hosyo = j;
}

void Power(int i)
{
	P1.GetPow = i, P1.GivePow = i / 2;
}

void Power(int i, int j)
{
	P1.GetPow = i, P1.GivePow = j;
}

boolean triggerT(int i, int j)
{
	switch (j){
	case 0:
		if (P1.time == i)return true;
		else { return false; }
		break;

	case 1:
		if (P1.time >= i)return true;
		else { return false; }
		break;

	case 2:
		if (P1.time <= i)return true;
		else { return false; }
		break;
	}

	// ����������Ȃ�������
	return false;
}

int TimeNo(int t1, int t2){
	if ((P1.time >= t1) && (P1.time <= t2)){
		return true;
	}
	else{ return false; }
}
//-------------------------------------------------------------------------------
//
// �w���p�[�ݒ�
//
//-------------------------------------------------------------------------------

//�_���[�W�Z�b�g
void H2_AttSet(int i, int j, int W, int H, int X, int Y)
{
	// �U������Z�b�g
	H1[i].WAtt[j] = W, H1[i].HAtt[j] = H;
	if (H1[i].muki == 0){
		H1[i].XAtt[j] = (int)H1[i].XPos + X;
	}
	else{
		H1[i].XAtt[j] = (int)H1[i].XPos - X - H1[i].WAtt[j];
	}
	H1[i].YAtt[j] = (int)H1[i].YPos + Y;
}


// ���W�Z�b�g
void H_PosSet(int i, double X, double Y)
{
	// 2P�̏ꍇ�A���΂�
	if (H1[i].muki == 1){
		X = -X;
	}
	H1[i].XPos = P1.XPos + X;
	H1[i].YPos = P1.YPos + Y;

}
// ���x�Z�b�g
void H_VelSet(int i, double X, double Y)
{
	H1[i].HXVel = X;
	H1[i].HYVel = Y;

}

void H_HitTime(int i, int HS, int GH, int AH, int GG)
{
	H1[i].HHitStop = HS;
	H1[i].HSSelf = HS;
	H1[i].HG_HitTime = GH, H1[i].HA_HitTime = AH;
	H1[i].HG_GuardTime = GG;
}

void H_Move(int i)
{
	// �ʒu��ς���
	if (H1[i].muki == 0){
		H1[i].XPos += H1[i].HXVel;
	}
	else{ H1[i].XPos -= H1[i].HXVel; }
	H1[i].YPos += H1[i].HYVel;
}

void H_HitVel(int i, double X1, double Y1, double X2, double Y2)
{
	H1[i].H_GX = X1, H1[i].H_GY = Y1;
	H1[i].H_AX = X2, H1[i].H_AY = Y2;
}

void h_HitEff(int i, int g, double x, double y)
{
	H1[i].hitEff = g;
	H1[i].heSizeX = x;
	H1[i].heSizeY = y;
}

boolean H_triggerT(int i, int j, int k)
{
	switch (k){
	case 0:
		if (H1[i].time == j)return true;
		else { return false; }
		break;

	case 1:
		if (H1[i].time >= j)return true;
		else { return false; }
		break;

	case 2:
		if (H1[i].time <= j)return true;
		else { return false; }
		break;
	}

	// ����������Ȃ�������
	return false;
}


//-------------------------------------------------------------------------------
//
// �L�����Z���ݒ�
//
//-------------------------------------------------------------------------------




void HelperReset(int i)
{
	H1[i].var = false;

	H1[i].time = 0, H1[i].HAnimTime = 0;
	H1[i].Image = 0;
	H1[i].GSize = P1.GSize;
	H1[i].Alpha = false;

	H1[i].XPos = 0, H1[i].YPos = 0;
	H1[i].HXVel = 0, H1[i].HYVel = 0;

	for (int j = 0; j < ATT_MAX; j++){
		H1[i].XAtt[j] = -1, H1[i].YAtt[j] = -1;
		H1[i].WAtt[j] = -1, H1[i].HAtt[j] = -1;
	}

	H1[i].A.damage = 0;
	H1[i].A.hosyo = 0;
	H1[i].HGetPow = 0, H1[i].HGivePow = 0;
	H1[i].HGuardF = 1;
	H1[i].HMoveHit = 0;
	H1[i].stopTime = 0;
	H1[i].HSSelf = 0;
	// [�q�b�g�E�K�[�h�T�E���h]
	H1[i].HHitSE = 11, H1[i].HGuardSE = 17;
	H1[i].attF = 3;	// �f�t�H�Ŕ�ѓ���
	H1[i].fallF = 1;
	H1[i].A.boundLevel = 0;
	H1[i].A.wallLevel = 0;
	h_HitEff(i, 1, 0.5, 0.5);
}

void DamReset()
{
	
	for (int i = 0; i < HIT_MAX; i++)
	{
		P1.XAtt[i] = -100, P1.YAtt[i] = -100;
		P1.WAtt[i] = 0, P1.HAtt[i] = 0;
	}
	
	Damage(0, 0);
	Power(0, 0);
	P1.attF = 1;
	P1.fallF = 1;
	P1.MoveHit = 0;

	P1.A.forceKurai = false;
	P1.A.boundLevel = 0;
	P1.A.bound_xvel = -1.3;
	P1.A.bound_yvel = -3.0;
	P1.A.wall_xvel = 0.4;
	P1.A.wall_yvel = -6.4;
	P1.A.wallLevel = 0;
	P1.A.yaccel = GRAVITY;
	P1.A.hitEff = 1;
	P1.A.heSizeX = 0.5;
	P1.A.heSizeY = 0.5;
	P1.A.guard_gx = 0;
	P1.A.guard_ax = 0;
	P1.A.quakeTime = 0;
	P1.A.quakeY = 0;
	P1.A.kill = true;

	P1.GuardF = 1;	// �f�t�H�͏�i��
	
	HitTime(0, 0, 0, 0);

}

// ��������
void ThrowReject(){
	if (P1.SFlg != 2){
		if ((P2.Senkou[1] > 0 && P2.Senkou[2] > 0 && P2.Senkou[3] == 0)
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
	}
	else if (P1.SFlg == 2){
		// ��������
		if ((P2.Senkou[1] > 0 && P2.Senkou[2] > 0 && P2.Senkou[3] == 0)
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
			EffStart(12, (int)P2.XPos, (int)P2.YPos, 0, -(P2.ySize / 1.5),
				0.4, 0.4, false);
		}
	}
}

void HitEff(int g, double x, double y)
{
	P1.A.hitEff = g;
	P1.A.heSizeX = x;
	P1.A.heSizeY = y;
}

// ���x�Z�b�g
void VelSet(double X, double Y)
{
	if (P1.StopTime == 0){
		P1.XVel = X;
		P1.YVel = Y;
	}
}

// ���x���Z
void VelAdd(double X, double Y)
{
	if (P1.StopTime == 0){
		P1.XVel += X;
		P1.YVel += Y;
	}
}

// �ʒu�Z�b�g
void PosAdd(double X, double Y)
{
	if (!P1.muki){
		P1.XPos += X;
	}
	else{ P1.XPos -= X; }
	P1.YPos += Y;
}

// ���n�m�F����
boolean Chakuchi(){
	if(P1.YPos + (P1.YVel * 2) > GROUND)return true;
	return false;
}

// �X�e�Q�����߂ĂȂ�����
boolean AnimElem(int state2)
{
	// -1��mugen�ɍ��킹�邽��
	// ���̃X�e�[�g�ɂ������A�w�肵���X�e�[�g�ԍ��A�~�܂��ĂȂ�
	if ((P1.State2 != P1.oldState2) && (P1.State2 == state2 - 1) && (P1.StopTime == 0)
		&& (P1.stateno == P1.OldState)){
		return true;
	}

	return false;
}

#endif