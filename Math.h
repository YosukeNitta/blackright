//===============================================================================
//
//			�i���Q�[���@�@�@�v�Z�w�b�_��
//
//===============================================================================
#include "PSet.h"
#include "SSet.h"
#include "vector"
#include "DxLib.h"

#ifndef DEF_MATH_H	//��x��`����Ă�����A��`���Ȃ�
#define DEF_MATH_H

/****
* State
****/
//�L�������l������
void CharLoad(void);
//�L�����̈ʒu�v�Z
int CharMove(void);
//�X�e�[�g�̓��e   �L�������ʂł���ŏ����ł���悤�ɂ���
//void StateParam(void);
// �摜�̃��[�h
void LoadPict();

void Get_PSet(Player GP1, Player GP2);
void Get_P1Set(Player GP1);
void Get_P2Set(Player GP2);
void Get_SSet(System_t GS);
void Get_PSSet(Player GP1, Player GP2, System_t GS);
void Get_HSet(Helper GH1[], Helper GH2[]);
void Get_Color(int c, int PSide);

void TraningSwitch(boolean flg);
void AISwitch(boolean flg);
void T_VarSet(int var, int num);
// �摜�ݒ��ǂݍ���
void Load_State();
// �厖�ȃt�@�C����ǂݍ���
void fLoad_State();
// �����ʒu����
void FirstPosSet(int n);
int Tlist_Save(void);
void StopTime_Add(int n);
void StopCmd(boolean cmd);

/****
* SameParam
****/
void GetN_SameParam(std::vector<Name_t>GN);

/****
* SubState
****/
//�X�e�[�g�ԍ�����
void EnterState();
void EnterAttack();
void GetP_SubState(Player GP1, Player GP2);
void GetS_SubState(System_t GS);
void GetH_SubState(Helper GH1[], Helper GH2[]);
void SubState_DashReset();

/****
* SubSystem
****/
void TouchHantei();
int SetScrool();
int TouchCheck();
void VelPosSet();
void PosHosei();
void VelXYAdd();
void GetP_SubSystem(Player GP1, Player GP2);
void GetS_SubSystem(System_t GS);


/****
* SubDamage
****/
void DamageCheck();
void GetP_SubDamage(Player GP1, Player GP2);
void GetS_SubDamage(System_t GS);

/****
* SubDamSet
****/
void DamSetting();
void GetP_SubDamSet(Player GP1, Player GP2);
void GetS_SubDamSet(System_t GS);

/****
* HelDamage
****/
void HelperDamCheck();
void GetP_HelDamage(Player GP1, Player GP2);
void GetH_HelDamage(Helper GH1[], Helper GH2[]);
void GetS_HelDamage(System_t GS);


/****
* LifeCount
****/
void LifeCount();
void GetPS_LifeCount(Player GP1, Player GP2, System_t GS);
void GetH_LifeCount(Helper GH1[], Helper GH2[]);
void load_LifeCount();

//----------
// Struct
//----------
void Struct_PH(Player GP, int PSide);
void Set_P();

//----------
// Traning
//----------
void SetTraning();
void GetP_Traning(Player GP1, Player GP2);
void GetS_Traning(System_t GS);
void GetH_Traning(Helper GH1[], Helper GH2[]);
void Traning_P2Command(int command[20]);

//--------
// result
//--------
int Result();
void Load_Result();
void Result_WinSide(int i, int chara, int chara2);
void WinCount(int side);
void Result_GetData(int c1, int c2, int cl1, int cl2, int stage);

/****
* Player
****/
int Player_State();
void Player_GraphLoad();
int Player_ImageGet(Player GP1);
int Helper_Graph();
void GetP1_Player(Player GP1);
void GetS_Player(System_t GS);
void Get_N12(int N_1, int N_2);
void GetH_Player(Helper GH1[], Helper GH2[]);

#endif