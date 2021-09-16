//===============================================================================
//
//			�i���Q�[���@�@�@�L�����w�b�_��
//
//===============================================================================

// �O��PS��������

#ifndef DEF_CHAR_H	//��x��`����Ă�����A��`���Ȃ�
#define DEF_CHAR_H

#include "SSet.h"
#include "PSet.h"
#include "vector"

//��{�f�[�^�̍\����
//typedef struct {

class Name_t{

public:
	int name;
	char nameC[NAME_MAX];

	Name_t(){
		
	}
	Name_t(int nam){
		name = nam;
	}

};//Name_t;

/****
* Ein
****/
//---------------------
// 
// EinCmd
//
//---------------------

int CharCmd();

int Ein_ComSet();
int Ein_ComCheck(int num);

void GetP_EinCmd(Player GP1, Player GP2);
void GetPH_EinCmd(Player GP1);
void GetS_EinCmd(System_t GS);
void GetH_EinCmd(Helper GH1[], Helper GH2[]);




//---------------------
// EinParam
//---------------------
void EinParam();

void GetP_EinParam(Player GP1, Player GP2);
void GetS_EinParam(System_t GS);
void GetH_EinParam(Helper GH1[], Helper GH2[]);

void HelperReset(int i);



//---------------------
// GHitbox
//---------------------
void GHitbox();
void HelperKurai();
void GetP_GHitbox(Player GP1);
void GetS_GHitbox(System_t GS);
void GetA_GHitbox(Air GA[STATE_MAX], int pSide);
void GetPI_GHitbox(Pict GPI[PICT_MAX][IMAGE2_MAX], int side);
void GetN_GHitBox(std::vector<Name_t>GN);
//void GetPI_GHitbox(Pict GPI[][], int side);
void GetH_GHitbox(Helper GH1[], Helper GH2[]);
// GP1�̃A�j�����Ԏ擾
Player GetAnimElem(Player GP1);

void load_GHitBox();
void ExAtt(int PSide, int i, int W, int H, int X, int Y);

//--------------------------------------
// 
//              �R�[�l��
//
//--------------------------------------
//---------------------
// Param
//---------------------
void CorParam(void);
void GetP_CorParam(Player GP1, Player GP2);
void GetS_CorParam(System_t GS);
void GetH_CorParam(Helper GH1[], Helper GH2[]);


//--------------------------------------
// 
//              �o�E���T�[
//
//--------------------------------------
void BouParam(void);
void GetP_BouParam(Player GP1, Player GP2);
void GetS_BouParam(System_t GS);
void GetH_BouParam(Helper GH1[], Helper GH2[]);

//---------------------
// 
// �w���I�X
//
//---------------------
void HelParam(void);
void GetP_HelParam(Player GP1, Player GP2);
void GetS_HelParam(System_t GS);
void GetH_HelParam(Helper GH1[], Helper GH2[]);

//---------------------
// 
// �G���N
//
//---------------------
void EriParam(void);
void GetP_EriParam(Player GP1, Player GP2);
void GetS_EriParam(System_t GS);
void GetH_EriParam(Helper GH1[], Helper GH2[]);

//---------------------
// 
// �V���E�C�`
//
//---------------------
void SyuParam(void);
void GetP_SyuParam(Player GP1, Player GP2);
void GetS_SyuParam(System_t GS);
void GetH_SyuParam(Helper GH1[], Helper GH2[]);


//---------------------
// 
// ����
//
//---------------------
void HydParam(void);
void GetP_HydParam(Player GP1, Player GP2);
void GetS_HydParam(System_t GS);
void GetH_HydParam(Helper GH1[], Helper GH2[]);


//---------------------
// 
// Hantei
//
//---------------------
Player ReturnPSet();
Helper ReturnHSet(int i);
void KuraiSet(int i, int W, int H, int X, int Y);
void KuraiReset();
void AttSet(int i, int W, int H, int X, int Y);
void AttReset();

void GetPH_Hantei(Player GP1);
void GetS_Hantei(System_t GS);
void GetH_Hantei(Helper GH1[], Helper GH2[]);
void GetH1_Hantei(Helper GH1[]);

void H_KuraiSet(int n, int i, int W, int H, int X, int Y);
void H_KuraiReset();
void H_AttSet(int n, int i, int W, int H, int X, int Y);
void H_AttReset();
//---------------------
//
// SameParam
//
//---------------------
void SameParam();
void DamParam();
void ChangeState();
void GetP_SameParam(Player GP1, Player GP2);
void GetS_SameParam(System_t GS);
void GetH_SameParam(Helper GH1[], Helper GH2[]);

//=====================
//
// Pict�ݒ�
//
//=====================
int PalletSet();
int GraphSet(int s1, int s2, int side);
void GetP1_Pict(Player GP1);
void GetPict(int side);
// Name���擾
//void GetN_Pict(Name_t GN[]);
void GetN_Pict(std::vector<Name_t>GN);
void load_pict(int np, int side);
void Load1_Pict();

//=====================
//
// Air�ݒ�
//
//=====================
void load_air(int na, int pSide);
// Air��P1���擾
void GetP_Air(Player GP1);
void GetName();
void GetPName();

//=====================
//
// AI�ݒ�
//
//=====================
void AI_Move();
void GetP_AI(Player GP1, Player GP2);
void GetS_AI(System_t GS);
void GetH_AI(Helper GH1[], Helper GH2[]);

//=====================
//
// �R�����X�e�[�^�X
//
//=====================
void load_status(Player gp);

#endif
