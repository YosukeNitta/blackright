//==================================================================================
//
//			�X�e�[�g�����̊Ǘ���
//
//==================================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include <vector>
using namespace std;
//�f�t�@�C����----------------------------------------------

// #include "Define.h" �ɂāA�֐����Z�b�g


//�f�[�^��`��------------------------------------------------

static int PSide;
static boolean load_1;

class Command{
public:
	Command(){

	}
};

static vector<Command>charCmd;

//�����֐��錾��--------------------------------------------

typedef struct {
	// �R�}���h�����̉�
	int Cmd[20];
	// �R�}���h��������
	int cmdOK[20]; 
	// �R�}���h���̎�t����
	int Time[20];
	// �R�}���h���͂̃t���O
	int Flg[20];

}Command_t;

// �R�}���h�̕ϐ��p��
static Command_t C[2];


//�v���O������----------------------------------------------

int InPAD(int num);
int InCOM(int num);

static boolean checkThrow();
static boolean check3Gauge();

static int Def_Cmd();
static int EinCmd();
static int Char2_Cmd();
static int Char3_Cmd();
static int Char4_Cmd();
static int Char5_Cmd();

//-------------------------------------------------------------------------------
//
// �A�C���̃R�}���h�Z����������
// ����E�K�E�Z
//
//-------------------------------------------------------------------------------

int CharCmd()
{
	if (!load_1){
		for (int i = 0; i < Character_Max(); i++){
			charCmd.push_back(Command());
		}
	}

	// �R�}���h���͂��擾
	for (int i = 0; i < 20; i++){
		P1.cmd[i] = C[P1.PSide - 1].Cmd[i];
	}

	// �L�������̃R�}���h
	switch (P1.Name){
	case EIN:
		EinCmd();
		break;
	case CORNEL:
		Char2_Cmd();
		break;
	case BOUNCER:
		Char3_Cmd();
		break;
	case HELIOS:
		Char4_Cmd();
		break;
	case HYDE:
		Char5_Cmd();
		break;
	default:
		Def_Cmd();
		break;
	}

	// �X�e�[�g�̌��ʂ�Ԃ�
	if (P1.PSide == 1)Get_P1Set(P1);
	if (P1.PSide == 2)Get_P2Set(P1);

	return 0;
}

int EinCmd()
{

	/********
	/
	/ [�c�A�N�V����]
	/
	/*******/

	//  6B
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]) && (P1.Senkou[3] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 225;
		}
	}


	// D�A�N�V�����AD�������Ă��đ���\
	if ((P1.ctrl) && (P1.button[4] == 1)){
		// �n��ɂ��āA�w���p�[[2]�����݂��Ȃ�
		if ((!H1[2].var) && (P1.SFlg != 2)){
			P1.stateno = 620;
		}
		// �󒆐ݒu
		else if ((!H1[2].var) && (P1.SFlg == 2)){
			P1.stateno = 622;
		}
		// �w���p�[[2]������
		if (H1[2].var){
			if (P1.SFlg != 2){
				P1.stateno = 625;
			}
			else if (P1.SFlg == 2){
				P1.stateno = 626;
			}
		}
	}
	

	/********
	/
	/ [�K�E�Z�E�Q�[�W�Z�E����]
	/
	/*******/
	// [�u�[����������]
	// �w���p�[�P���o���Ă��Ȃ�
	if ((P1.ctrl) && (InCOM(1)) && (!H1[1].var)){
		if (P1.SFlg != 2){		// �n��
			// ABC�S��
			if (P1.Senkou[1] > 0 || P1.Senkou[2] > 0 || P1.Senkou[3] > 0){
				P1.stateno = 600;
				if (P1.Senkou[1] > P1.Senkou[2]){
					if (P1.Senkou[1] > P1.Senkou[3])P1.Var[6] = 1;
					else{ P1.Var[6] = 3; }
				}
				else if (P1.Senkou[2] > P1.Senkou[3])P1.Var[6] = 2;
				else{ P1.Var[6] = 3; }
			}
		}
		else if (P1.SFlg == 2){	// ��
			// ABC�S��
			if (P1.Senkou[1] > 0 || P1.Senkou[2] > 0 || P1.Senkou[3] > 0){
				P1.stateno = 605;
				if (P1.Senkou[1] >= P1.Senkou[2]){
					if (P1.Senkou[1] >= P1.Senkou[3])P1.Var[6] = 1;
					else{ P1.Var[6] = 3; }
				}
				else if (P1.Senkou[2] >= P1.Senkou[3])P1.Var[6] = 2;
				else{ P1.Var[6] = 3; }
			}
		}
	}
	// ����\���� 214 �����
	// [ �╥�� ]
	if ((P1.ctrl) && (InCOM(2))){
		if (P1.SFlg != 2){		// �n��
			if (P1.Senkou[1] > 0)P1.stateno = 610;
			if (P1.Senkou[2] > 0)P1.stateno = 611;
		}
	}
	//[EX�Z]
	// [��u�[������]
	if ((P1.ctrl) && (InCOM(1)) && (P1.Power >= 500)){
		if (P1.SFlg != 2){		// �n��
			if (P1.Senkou[4] > 0)P1.stateno = 700;
		}
	}

	// [ ��i�؂� ]
	if ((P1.ctrl) && (InCOM(2)) && (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// �n��
			if (P1.Senkou[3] > 0)P1.stateno = 800;
		}	
		else if (P1.SFlg == 2){		// �n��
			if (P1.Senkou[3] > 0)P1.stateno = 830;
		}
	}
	
	// [ �^��u�[������ ]
	if ((P1.ctrl) && (InCOM(3)) && (P1.Power >= 1000) 
		&& (H1[2].var) && (H1[2].stateno == 2020)){
		if (P1.SFlg != 2){		// �n��
			if (P1.button[4] > 0)P1.stateno = 820;
		}
	}

	// EX�A�N�V����
	// A..�Q�[�W����
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Var[30] == 0)){
		if (P1.SFlg != 2){		// �n��
			if (P1.button[1] > 0)P1.stateno = 550;	// �Q�[�W����
			if (P1.button[3] > 0)P1.stateno = 552;	// ���g
		}
	}
	// B..�o�[�X�g
	// ��~���Ԃ�0�A1060�`1069�ł͂Ȃ�
	if ((P1.cmd[3]) && (P1.Var[30] == 0) && (P1.StopTime == 0) &&
		(P1.stateno < 1060 || P1.stateno >= 1070) && (P1.Life > 0) &&
		(P1.stateno >= 1000) && (P2.stateno < 800 || P2.stateno >= 900)){
			{		// �ǂ��ł�
				if (P1.button[2] > 0)P1.stateno = 551;	// �Q�[�W����
			}
	}

	//  ����
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}

	//  [ �R�Q�[�W ]
	if (check3Gauge()){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 850;	// ���K
		}
	}

	// �R�}���h���L��


	return 0;
}


// �R�[�l��
int Char2_Cmd()
{
	
	/********
	/
	/ [�c�A�N�V�����E����Z]
	/
	/*******/
	// D�A�N�V�����AD�������Ă��đ���\
	if ((P1.ctrl) && (P1.Senkou[4] > 0)){
		// �n��ɂ���
		if (P1.SFlg != 2){
			if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]))P1.stateno = 603;
			else if (P1.keyAtt[2])P1.stateno = 604;
			else{ P1.stateno = 600; }
		}
		// �󒆃L�b�N
		else if (P1.SFlg == 2 && (P1.YPos < GROUND - 5)){
			P1.stateno = 430;
		}
	}

	//  6B
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]) && (P1.Senkou[2] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 215;
		}
	}

	//  3C
	if ((P1.keyAtt[6]) && (P1.keyAtt[2]) && (P1.Senkou[3] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 321;
		}
	}


	/********
	/
	/ [�K�E�Z�E�Q�[�W�Z�E����]
	/
	/*******/

	// [�X�s�i�[]
	if ((P1.ctrl) && (P1.cmd[1]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 630;
		}
	}
	// [�X�s�i�[]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 610;
		}
	}
	// [�A�b�p�[]
	if ((P1.ctrl) && (P1.cmd[4]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 620;
		}
	}
	// [�G���n]
	/*
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Senkou[4] > 0)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 650;
		}
	}
	*/

	//==============
	// �Q�[�W�Ɠ���
	//==============
	
	// [ �ł��グ ]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[4] > 0) && (P1.Power >= 500)){
		if (P1.SFlg != 2){		// �ǂ��ł�
			P1.stateno = 700;
		}
	}
	// [ �X�}�b�V�� ]
	if ((P1.ctrl) && (P1.cmd[1]) && (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// �n��
			if (P1.Senkou[3] > 0)P1.stateno = 800;
		}
	}
	// [ �E�B���O�A�b�p�[ ]
	if ((P1.ctrl) && (P1.cmd[4]) && (P1.Power >= 1000)){
		{		// �ǂ��ł�
			if (P1.Senkou[3] > 0)P1.stateno = 820;
		}
	}
	// [ �n�C�h���X�g�[�� ]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Power >= 2000) && (!H1[1].var)){
		if (P1.SFlg != 2){		// �n��
			if (P1.Senkou[3] > 0)P1.stateno = 810;
		}
	}

	// EX�A�N�V����
	// A..�Q�[�W����
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Var[30] == 0)){
		if (P1.SFlg != 2){		// �n��
			if (P1.button[1] > 0)P1.stateno = 550;	// �Q�[�W����
			if (P1.button[3] > 0)P1.stateno = 552;	// ���g
		}
	}
	// B..�o�[�X�g
	// ��~���Ԃ�0�A1060�`1069�ł͂Ȃ�
	if ((P1.cmd[3]) && (P1.Var[30] == 0) && (P1.StopTime == 0) &&
		(P1.stateno < 1060 || P1.stateno >= 1070) && (P1.Life > 0) &&
		(P1.stateno >= 1000) && (P2.stateno < 800 || P2.stateno >= 900)){
			{		// �ǂ��ł�
				if (P1.button[2] > 0)P1.stateno = 551;	// �Q�[�W����
			}
	}

	//  ����
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}

	//  [ �R�Q�[�W ]
	if (check3Gauge()){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 850;	// ���K
		}
	}


	return 0;
}

// �o�E���T�[
int Char3_Cmd()
{

	/********
	/
	/ [�c�A�N�V�����E����Z]
	/
	/*******/
	//  6B
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]) && (P1.Senkou[2] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 215;
		}
	}
	//  6B
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]) && (P1.Senkou[3] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 225;
		}
	}
	//  3C
	if ((P1.keyAtt[6]) && (P1.keyAtt[2]) && (P1.Senkou[3] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 235;
		}
	}
	//  J2C
	if ((P1.keyAtt[2]) && (P1.Senkou[3] > 0) && (P1.ctrl)){
		if ((P1.SFlg == 2)){		// �n��
			P1.stateno = 430;
		}
	}
	// D�A�N�V�����AD�������Ă��đ���\
	if ((P1.ctrl) && (P1.Senkou[4] > 0)){
		// �n��ɂ���
		if (P1.SFlg != 2){
			P1.stateno = 600;
		}
	}
	/********
	/
	/ [�K�E�Z�E�Q�[�W�Z�E����]
	/
	/*******/
	// [�O�����h]
	if ((P1.ctrl) && (P1.cmd[1]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 620;
		}
	}
	// [�X�s�i�[]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 610;
		}
	}
	

	//[EX�Z]
	// [��u�[������]
	if ((P1.ctrl) && (P1.cmd[1]) && (P1.Power >= 500)){
		if (P1.SFlg != 2){		// �n��
			if (P1.Senkou[3] > 0)P1.stateno = 625;
		}
	}
	// [0.5����]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[3] > 0) 
		&& (P1.Power >= 500)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 610;
			P1.Var[15] = 1;
		}
	}
	

	// EX�A�N�V����
	// A..�Q�[�W����
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Var[30] == 0)){
		if (P1.SFlg != 2){		// �n��
			if (P1.button[1] > 0)P1.stateno = 550;	// �Q�[�W����
			if (P1.button[3] > 0)P1.stateno = 552;	// ���g
		}
	}
	// B..�o�[�X�g
	// ��~���Ԃ�0�A1060�`1069�ł͂Ȃ�
	if ((P1.cmd[3]) && (P1.Var[30] == 0) && (P1.StopTime == 0) &&
		(P1.stateno < 1060 || P1.stateno >= 1070) && (P1.Life > 0) &&
		(P1.stateno >= 1000) && (P2.stateno < 800 || P2.stateno >= 900)){
			{		// �ǂ��ł�
				if (P1.button[2] > 0)P1.stateno = 551;	// �Q�[�W����
			}
	}

	//  ����
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}

	//  [ �P�Q�[�W ]
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.Senkou[3] > 0) && (P1.ctrl)
		&& (P1.Power >= 1000 || P1.Var[13] > 0)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 800;	// ���K
		}
	}

	//  [ �R�Q�[�W ]
	//  [ �P�Q�[�W ]
	if ((P1.Senkou[4] > 0) && (P1.cmd[3]) && (P1.ctrl)
		&& (P1.Power >= 3000)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 850;	// ���K
		}
	}


	return 0;
}

// �w���I�X
int Char4_Cmd()
{

	/********
	/
	/ [�c�A�N�V�����E����Z]
	/
	/*******/
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (P1.Senkou[1] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 205;
		}
	}
	//  6B
	if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (P1.Senkou[2] > 0) && (P1.ctrl)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 215;
		}
	}
	// D�A�N�V�����AD�������Ă��đ���\
	if ((P1.ctrl) && (P1.Senkou[4] > 0 || P1.button[4] > 0) && (P1.Var[11] < 3000)){
		// �n��ɂ���
		if (P1.SFlg != 2){
			P1.stateno = 600;
		}
	}
	/********
	/
	/ [�K�E�Z�E�Q�[�W�Z�E����]
	/
	/*******/
	// [�u�[����������]
	// �w���p�[�P���o���Ă��Ȃ�
	if ((P1.ctrl) && (P1.cmd[1]) && (!H1[0].var)){
		if (P1.SFlg != 2){		// �n��
			if (P1.Senkou[1] > 0 || P1.Senkou[2] > 0)P1.stateno = 630;
		}
	}
	if ((P1.ctrl) && (P1.cmd[1]) && (!H1[0].var) && (P1.Var[13] == 0)){
		if (P1.SFlg != 2){		// �n��
			if (P1.Senkou[4] > 0){
				// 2�{�ȏ�
				if (P1.Var[11] >= 2000)
					P1.stateno = 635;
				else{ P1.stateno = 630; }
			}
		}
	}

	// [214]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0 || P1.Senkou[3] > 0)){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 610;
			if (P1.Senkou[2] > 0)P1.stateno = 611;
		}
		if (P1.SFlg == 2){
			if (P1.YPos < GROUND - 20)P1.stateno = 616;
		}
	}

	// [�Q�P�S�c]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Senkou[4] > 0)){
		if (P1.SFlg != 2){		// �n��
			if (P1.Var[11] >= 1000)
				P1.stateno = 615;
			else{ 
				P1.stateno = 611;
			}
		}
		if (P1.SFlg == 2){
			if (P1.YPos < GROUND - 20){
				if (P1.Var[11] >= 1000)
					P1.stateno = 617;
				else{ P1.stateno = 616; }
			}
		}
	}

	// [�U�Q�R�`or�aor�b]
	if ((P1.ctrl) && (P1.cmd[4]) && (P1.Senkou[1] > 0 || P1.Senkou[2] > 0 || P1.Senkou[3] > 0)){
		if (P1.SFlg != 2){
			P1.stateno = 620;
		}
	}
	// [�U�Q�R�c]
	if ((P1.ctrl) && (P1.cmd[4]) && (P1.Senkou[4] > 0)){
		if (P1.SFlg != 2){		// �n��
			if(P1.Var[11] >= 1000)
				P1.stateno = 625;
			else{ P1.stateno = 620; }
		}
	}

	// [ 1�Q�[�W ]
	if ((P1.ctrl) && (P1.cmd[1]) && (P1.Power >= 1000) && (!H1[2].var)){
		if (P1.SFlg != 2){		// �n��
			if (P1.Senkou[3] > 0)P1.stateno = 820;
		}
	}
	// [ 1�Q�[�W ]
	if ((P1.ctrl) && (P1.cmd[2]) && (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// �n��
			if (P1.Senkou[3] > 0)P1.stateno = 800;
		}
	}
	// [ 22D ]
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Power >= 1000)){
		if (P1.SFlg != 2){		// �n��
			if (P1.Senkou[4] > 0)P1.stateno = 830;
		}
	}


	//  ����
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}

	//  [ �R�Q�[�W ]
	if (check3Gauge()){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 850;	// ���K
		}
	}

	return 0;
}

// �n�C�h
int Char5_Cmd()
{

	/********
	/
	/ [�c�A�N�V�����E����Z]
	/
	/*******/
	// D�A�N�V�����AD�������Ă��đ���\
	if (P1.ctrl){
		if (P1.Senkou[4] > 0){
			// �n��ɂ���
			if (P1.SFlg != 2){
				if ((P1.keyAtt[6]) && (!P1.keyAtt[2]) && (!P1.keyAtt[8]))P1.stateno = 605;	// 6D
				else if (P1.keyAtt[2])P1.stateno = 610;	// 2D
				else{ P1.stateno = 600; }	// 5D
			}
			// �󒆃L�b�N
			else if (P1.SFlg == 2 && (P1.YPos < GROUND - 5)){
				if (P1.keyAtt[2])P1.stateno = 630;	// J2D
				else{P1.stateno = 620;}	// JD
			}
		}
	}

	/********
	/
	/ [�K�E�Z�E�Q�[�W�Z�E����]
	/
	/*******/

	// EX�A�N�V����
	// A..�Q�[�W����
	if ((P1.ctrl) && (P1.cmd[3]) && (P1.Var[30] == 0)){
		if (P1.SFlg != 2){		// �n��
			if (P1.button[1] > 0)P1.stateno = 550;	// �Q�[�W����
			if (P1.button[3] > 0)P1.stateno = 552;	// ���g
		}
	}
	// B..�o�[�X�g
	// ��~���Ԃ�0�A1060�`1069�ł͂Ȃ�
	if ((P1.cmd[3]) && (P1.Var[30] == 0) && (P1.StopTime == 0) &&
		(P1.stateno < 1060 || P1.stateno >= 1070) && (P1.Life > 0) &&
		(P1.stateno >= 1000) && (P2.stateno < 800 || P2.stateno >= 900)){
			{		// �ǂ��ł�
				if (P1.button[2] > 0)P1.stateno = 551;	// �Q�[�W����
			}
	}

	//  ����
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}
	/*
	//  [ �R�Q�[�W ]
	if (check3Gauge()){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 850;	// ���K
		}
	}
	*/
	return 0;
}

// �ėp
int Def_Cmd()
{

	/********
	/
	/ [�c�A�N�V�����E����Z]
	/
	/*******/

	/********
	/
	/ [�K�E�Z�E�Q�[�W�Z�E����]
	/
	/*******/
	//  ����
	if (checkThrow()){
		if (P1.SFlg == 2)P1.stateno = 510;
		else{ P1.stateno = 500; }
	}
	//  [ �R�Q�[�W ]
	if (check3Gauge()){
		if (P1.SFlg != 2){		// �n��
			P1.stateno = 850;	// ���K
		}
	}

	return 0;
}


int Ein_ComSet()
{
	int i;

	if (P1.PSide == 1)i = 0;
	if (P1.PSide == 2)i = 1;

	// ���̓X�^�[�g
	// 1( 236 )
	{
		// 2 ����
		if ((P1.keyPos == 2)){
			if (C[i].Flg[1] == 0)
				C[i].Flg[1] = 1;
				// ���̓X�^�[�g
				C[i].Time[1] = 17;
		}
		// 3 ����
		if ((C[i].Time[1] != 0) && (C[i].Flg[1] == 1)){
			if (P1.keyPos == 3)C[i].Flg[1] = 2;
		}
		// 6 ����
		if ((C[i].Time[1] != 0) && (C[i].Flg[1] == 2)){
			if (P1.keyPos == 6)C[i].Flg[1] = 3;
		}
		// 236 ����
		if (C[i].Flg[1] == 3){
			C[i].Flg[1] = 0;
			C[i].cmdOK[1] = C[i].Time[1];
		}
	}

	// 2( 214 )
	{
		// 2 ����
		if ((P1.keyPos == 2)){
			if (C[i].Flg[2] == 0)
				C[i].Flg[2] = 1;
			// ���̓X�^�[�g
			C[i].Time[2] = 17;
		}
		// 1 ����
		if ((C[i].Time[2] != 0) && (C[i].Flg[2] == 1)){
			if (P1.keyPos == 1)C[i].Flg[2] = 2;
		}
		// 4 ����
		if ((C[i].Time[2] != 0) && (C[i].Flg[2] == 2)){
			if (P1.keyPos == 4)C[i].Flg[2] = 3;
		}
		// 214 ����
		if (C[i].Flg[2] == 3){
			C[i].Flg[2] = 0;
			C[i].cmdOK[2] = C[i].Time[2];
		}
	}

	// 3( 22 )
	// 2 ����
	// �ŏ��̂Q���͂̂�
	if ((P1.button[102] == 2) && (P1.keyPos == 2)){
	
		if (C[i].Flg[3] == 0)
			C[i].Flg[3] = 1;
		// ���̓X�^�[�g
		C[i].Time[3] = 20;
	}
	// 2 ����
	if ((C[i].Time[3] != 0) && (C[i].Flg[3] == 1)){
		if ((P1.keyPos == 2) && (P1.button[102] == 1))
			C[i].Flg[3] = 2;
	}
	// 22 ����
	if (C[i].Flg[3] == 2){
		C[i].Flg[3] = 0;
		C[i].cmdOK[3] = C[i].Time[3];
	}


	// 4( 623 )
	{
		// 6 ����
		if ((P1.keyPos == 6)){
			if (C[i].Flg[4] == 0)
				C[i].Flg[4] = 1;
				// ���̓X�^�[�g
				C[i].Time[4] = 17;
		}
		// 2 ����
		if ((C[i].Time[4] != 0) && (C[i].Flg[4] == 1)){
			if (P1.keyPos == 2)C[i].Flg[4] = 2;
		}
		// 3 ����
		if ((C[i].Time[4] != 0) && (C[i].Flg[4] == 2)){
			if (P1.keyPos == 3)C[i].Flg[4] = 3;
		}
		// 623 ����
		if (C[i].Flg[4] == 3){
			C[i].Flg[4] = 0;
			C[i].cmdOK[4] = C[i].Time[4];
		}
	}

	

	//--------------
	// ���ԃJ�E���g
	//--------------
	for (int j = 0; j < 20; j++){
		//--------------
		// �P�\����
		//--------------
		// ��炢�E�d��
		if ((C[i].cmdOK[j] == C[i].Time[j]) && (C[i].cmdOK[j] > 0)){
			if (P1.HFlg == 1)C[i].cmdOK[j] += 15;
			else if ((P1.stateno >= 200) && (P1.stateno <= 999))C[i].cmdOK[j] += 5;
		}

		// ���͗P�\
		if (C[i].Time[j] != 0){
			C[i].Time[j] -= 1;
		}
		// �R�}���h����
		if (C[i].cmdOK[j] != 0){
			C[i].cmdOK[j] -= 1;
		}
		if (C[i].Time[j] == 0){
			C[i].Flg[j] = 0;
		}
		// �R�}���h�̃I���E�I�t
		if (C[i].cmdOK[j] > 0){
			C[i].Cmd[j] = 1;
		}
		else{ C[i].Cmd[j] = 0; }
	}
	



	return 0;
}


// �R�}���h�̃t���O���m�F
int Ein_ComCheck(int num)
{
	int n, i;
	if (P1.PSide == 1)i = 0;
	if (P1.PSide == 2)i = 1;

	n = C[i].Cmd[num];

	

	return n;
}


//-------------------------------------------------------------------------------
//
// �ϐ��̎󂯎��
//
//-------------------------------------------------------------------------------

void GetP_EinCmd(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetPH_EinCmd(Player GP1)
{
	P1 = GP1;
}

void GetS_EinCmd(System_t GS)
{
	S = GS;
}


void GetH_EinCmd(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}

//-------------------------------------------------------------------------------
//
// ���͊m�F
//
//-------------------------------------------------------------------------------

int InPAD(int num)
{
	int i;

	if (P1.PSide == 1)
		i = P1_BInput(num);

	if (P1.PSide == 2)
		i = P2_BInput(num);

	return i;
}

int InCOM(int num)
{
	return Ein_ComCheck(num);
}

void Traning_P2Command(int command[20])
{
	for (int i = 0; i < 20; i++){
		if (command[i] > 0){
			C[1].Cmd[i] = 1;
			C[1].cmdOK[i] = 5;
		}
	}
}

static boolean checkThrow()
{
	// ����
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) &&
		(P1.ctrl || ((P1.stateno >= 200 && P1.stateno < 500) && P1.time <= 1 && P2.HFlg == 0 &&
		(P2.stateno < 50 && P2.stateno > 52)))){
		return true;
	}
	return false;
}

static boolean check3Gauge()
{
	//  [ �R�Q�[�W ]
	if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.Senkou[3] > 0) && (P1.ctrl)
		&& (P1.Power >= 3000)){
		return true;
	}
	return false;
}