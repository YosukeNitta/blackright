//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include <vector>
#include <string>
#include <stdio.h>	// �t�@�C������ɕK�v�ȃw�b�_�t�@�C��
#include <string.h>	// �����񑀍�ɕK�v�ȃw�b�_�t�@�C��
#include "Define2.h"

using namespace std;

void Furimuki();
static void PSetSend();

//
static char t_State[6] = { "State" };	// sffno
static char t_sflg[5] = { "sflg" };
static char t_ctrl[5] = { "ctrl" };
static char t_time[5] = { "time" };
static char t_lock[5] = { "lock" };
static char t_advancing[10] = { "advancing" };

static char t_S[2] = { "S" };
static char t_trigger1[9] = { "trigger1" };
static char t_icoico[5] = { "t_==" };

class Flag{
public:
	boolean var;	// true�Ȃ�ⓚ���p�Ŏ��s
	int num[3];	// 0..�Ώ� 1..������(==,>=,<=) 2..�ύX���e
	Flag(){
		var = false;
		for (int i = 0; i < 3; i++){
			num[i] = 0;
		}
	}
};

// �A�N�V�������e�A�X�e�[�g�ɂ���Đ��͂܂��܂�
class ActState{
public:
	int actionNum;	// �ԍ��Ŗ��߂����߂�
	int num[4];	// ���߂ő�����鐔��
	ActState(){
		actionNum = 0;
		for (int i = 0; i < 4; i++){
			num[i] = 0;
		}
	}
};

// ���s����A�N�V�������e
class Action{
public:
	vector<Flag>flg;		// ���s�������
	vector<ActState>aSt;	// �A�N�V�����̓��e
					// flg�̏����������ꂽ���Ɏ��s
					// ���͂܂��܂�
	Action(){

	}
};

// �����X�e�[�g
class State{
public:
	int num;	// �X�e�[�g�ԍ�
	boolean ctrl;
	int sflg;
	vector<Action>act;	// ���s����A�N�V�����Ə���
						// ���̓X�e�[�g�ɂ���Ă܂��܂�
	State(){
		num = 0;
		ctrl = 0;
		sflg = 0;
	}
};
//
static int n;
static vector<Name_t>N;
static vector<vector<State>>St;
static State defSt;

static void FirstState();
int GetPNum(int set);	// P1�̏����擾

// �f�t�H���g�X�e�[�g(mugen�̂���)�ǂݍ���
void Load_StateDef(int name){

	// ����Z�b�e�B���O //

	int i, fp;
	int row, nextSt;	// �Z�b�g����V�X�e��
	//int trigger;	// �g���K�[�ݒ�A0..�Ȃ� 1..���� 2..�����ł̃A�N�V����
	int icole;	// �C�R�[���A�t���O�̔�r�Ɏg��  -1..�Ȃ� 0..== 1..>= 2..<=
	row = -2;
	nextSt = -1;
	icole = -1;

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// input��inputc�ɕ������͂���
	boolean iflg;	// ���O���̓t���O

	// ���O�ǂݍ���
	string fname;
	// �f�t�H���g�ݒ�
	if (name == -1){
		fname = "char/def_state.txt";
	}
	else{
		string fn1 = "char/";
		string fn2 = "/state.txt";
		fname = fn1 + N[name].nameC + fn2;
	}
	name++;	// +1����

	// �t�@�C�����J�� //
	fp = FileRead_open(fname.c_str());//�t�@�C���ǂݍ���
	if (fp == NULL){			// �G���[���N��������
		printfDx("error def_state\n");// ������\��
		return;
	}

	//�ŏ���1�s�ǂݔ�΂�
	while (FileRead_getc(fp) != '\n'); // FileRead_getc:�ꕶ���ǂݏo��

	while (1){
		//=========================
		// ��{���[�v
		// ���̃��[�v�I����ɉ���
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1�����擾����

			// �X���b�V���ŉ��s
			if (inputc[i] == '/'){					//�X���b�V���������
				while (FileRead_getc(fp) != '\n');	//���s�܂Ń��[�v
				i = -1;//�J�E���^���ŏ��ɖ߂��� (�߂����Ƃ���0�ɂȂ�)
				continue;
			}

			// ���s�Ŏ��̍s�ɐi��
			if (input[i] == '\n'){
				for (int j = 0; j < NAME_MAX; j++){
					inputc[j] = input[j] = NULL;
					//inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				}
				i = -1;
				break;
			}

			// ������ɂ���
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == ';' || input[i] == '\t'){ //�J���}�����s�Ȃ�
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				break;	// i��1���₵�ă��[�v�E�o�i�������j
			}

			// �ǂݍ��ݏI��
			if (input[i] == EOF){//�t�@�C���̏I���Ȃ�
				goto EXFILE;//�I��
			}
		}	// ��{���[�v�I��

		// �����𔻒f
		iflg = false;
		{
			if (inputc[0] == '0')iflg = true;
			else if (inputc[0] == '1')iflg = true;
			else if (inputc[0] == '2')iflg = true;
			else if (inputc[0] == '3')iflg = true;
			else if (inputc[0] == '4')iflg = true;
			else if (inputc[0] == '5')iflg = true;
			else if (inputc[0] == '6')iflg = true;
			else if (inputc[0] == '7')iflg = true;
			else if (inputc[0] == '8')iflg = true;
			else if (inputc[0] == '9')iflg = true;
			else if (inputc[0] == '-')iflg = true;
		}

		// ���O��������
		if (iflg){
			switch (row)
			{
			case 0:	// stateNum
				nextSt++;
				St[name].push_back(State());
				St[name][nextSt].num = atoi(inputc);
				break;
			case 1:	// sflg
				St[name][nextSt].sflg = atoi(inputc);
				break;
			case 2:	// ctrl
				St[name][nextSt].ctrl = atoi(inputc);
				break;
			case 10:	// time
				//St[name][nextSt].act[0].aSt[0].num[0];
				break;
			case 11:	// lock
				//St[name][nextSt].act[0].aSt[0].num[0] = atoi(inputc);
				break;
			case 12:	// advancing
				//St[name][nextSt].act[0].aSt[0].num[0] = atoi(inputc);
				break;
			default:
				break;
			}
		}

		// �Z�b�g
		if (!iflg){
			// �X�e�[�g�A���萔�A������A��炢�̔��f
			// "State"
			if (strcmp(inputc, t_State) == 0){
				row = 0;
			}
			// "sflg"
			if (strcmp(inputc, t_sflg) == 0){
				row = 1;
			}
			// "ctrl"
			if (strcmp(inputc, t_ctrl) == 0){
				row = 2;
			}
			// "time"
			if (strcmp(inputc, t_time) == 0){
				row = 10;
			}
			// "lock"
			if (strcmp(inputc, t_lock) == 0){
				row = 11;
			}
			// "advancing"
			if (strcmp(inputc, t_advancing) == 0){
				row = 12;
			}
			// "trigger1"
			if (strcmp(inputc, t_trigger1) == 0){
				//trigger = 1;
				//St[name][nextSt].act.push_back(Action());
				//St[name][nextSt].act[0].flg.push_back(Flag());
				//St[name][nextSt].act[0].aSt.push_back(ActState());
			}
			// "=="
			if (strcmp(inputc, t_icoico) == 0){
				icole = 0;
			}
		}
		if (input[i] == EOF){//�t�@�C���̏I���Ȃ�
			goto EXFILE;//�I��
		}
	}

	// �t�@�C�������
EXFILE:
	FileRead_close(fp);

}

void FirstState(){
	boolean check = false;
	// ��������
	for (int i = 0; i < St[P1.Name].size(); i++){
		if (St[P1.Name][i].num == P1.stateno){
			P1.SFlg = St[P1.Name][i].sflg;
			P1.ctrl = St[P1.Name][i].ctrl;
			check = true;
			break;
		}
	}
	// ������Ȃ�������f�t�H���g
	if (!check){
		for (int i = 0; i < St[0].size(); i++){
			if (St[0][i].num == P1.stateno){
				P1.SFlg = St[0][i].sflg;
				P1.ctrl = St[0][i].ctrl;
				for (int j = 0; j < St[0][i].act.size(); j++){
					// �v���C���[���l�̔ԍ����擾
					P1.Lock = St[0][i].act[j].aSt[0].num[0];
				}
				//if ((P1.time) == (St[P1.Name][i].act[0].flg[0].num[2])){
					/*
					// lock
					if (St[P1.Name][i].act[0].aSt[0].actionNum = 11){
						P1.Lock = St[P1.Name][i].act[0].aSt[0].num[0];
					}
					// advancing
					if (St[P1.Name][i].act[0].aSt[0].actionNum = 12){
						P1.advancing = St[P1.Name][i].act[0].aSt[0].num[0];
					}
					*/
				//}
				break;
			}
		}
	}
}

int GetPNum(int set){
	int get = -1;
	switch (set){
	case 11:
		get = P1.Lock;
		break;
	case 12:
		get = P1.advancing;
		break;
	default:
		break;
	}
	return get;
}

int SetPNum(int set){
	int get = -1;
	switch (set){
	case 11:
		//P1.Lock = St[P1.Name][0];
		break;
	case 12:
		//get = P1.advancing;
		break;
	default:
		break;
	}
	return get;
}

// �w��Ȃ��X�e�[�g�ύX���̋��ʏ���
void ChangeState()
{
	// ���Ԃ�0�ɂ���
	if (P1.NoChangeTime)P1.NoChangeTime = 0;
	else{ 
		P1.time = 0; 
		P1.AnimTime = 0;
	}
	

	// �_���ݒ胊�Z�b�g
	for (int i = 0; i < HIT_MAX; i++)
	{
		P1.XAtt[i] = -100, P1.YAtt[i] = -100;
		P1.WAtt[i] = 0, P1.HAtt[i] = 0;
	}
	//Damage(0, 0);
	//Power(0, 0);
	P1.attF = 1;
	P1.fallF = 1;
	P1.MoveHit = 0;

	P1.A.boundLevel = 0;
	P1.A.bound_xvel = -1.3;
	P1.A.bound_yvel = -3.0;
	
	P1.A.wallLevel = 0;
	P1.A.forceKurai = false;
	P1.GuardF = 1;	// �f�t�H�͏�i��
	P1.A.boundLevel = 0;

	P1.A.addMuteki = false;
	P1.A.amTime = 0;

	//HitTime(0, 0, 0, 0);

	// [���W��0�ɂ���]
	if (P1.SFlg != 2)P1.YPos = GROUND;

	// [�K�[�h����]
	P1.GuardDist = 280;

	// [�d�͂�������悤��]
	// [�摜�̔��]��߂�]
	P1.ignoreG = false;
	P1.turnG = false;

	P1.touchF = true;
	P1.CFlg = false;

	// �_�b�V�����Ȃ�U������Ȃ�
	if (P1.DashFlg == 1)P1.Turn = false;
	else { P1.Turn = true; }

	//[ �_�b�V��������̎����U����� ]
	if ((P1.SFlg != 2) && (P1.time == 0) && (!P1.Turn)){

		if ((P1.XPos < P2.XPos) && (P1.muki == 1)){
			P1.muki = 0;
		}
		else if ((P1.XPos > P2.XPos) && (P1.muki == 0)){
			P1.muki = 1;
		}
	}

	// �߂�
	PSetSend();
}


void SameParam()
{
	// ��Ԃ�ǂݍ���
	if (P1.stateno >= 40 && P1.stateno <= 57)FirstState();

	switch (P1.stateno){
	
		//********************
		// 5 �U�����
		//********************
	case 5:
		P1.SFlg = 0, P1.ctrl = 1;
		VelSet(0, 0);
		if (P1.time == 0){
			P1.Lock = 1;
		}
		if (P1.time >= ANIMELEM){
			P1.Lock = 0;
		}
		break;
		//********************
		// 6 �U������i���Ⴊ�݁j
		//********************
	case 6:

		P1.SFlg = 1, P1.ctrl = 1;
		VelSet(0, 0);
		if (P1.time == 0){
			P1.Lock = 1;
		}
		if (P1.time >= ANIMELEM){
			P1.Lock = 0;
			P1.More = 1, P1.time = 0, P1.stateno = 11;
		}
		break;
		//********************
		// 10 ���������Ⴊ��
		//********************
	case 10:
		P1.SFlg = 1;	//���Ⴊ�ݔ����
		// ����
		if ((P1.time == 0) && (P1.XVel > 0))VelSet(P1.XVel / 1.5, 0);
		// ���Ⴊ�݂��痧���� 
		if (P1.button[102] == 0){
			P1.time = 0, P1.stateno = 12, P1.More = 1;
		}

		if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 11, P1.More = 1;

		break;
	case 40:	// �W�����v�ڍs
		//40�`57FS 
		P1.XVel = 0, P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 2;
		//if (P1.XPos < P2.XPos)P1.muki = 0;
		// �^�C�� 0 �̏��Ŕ��f
		if (P1.time == 0){
			
			// ������
			P1.JumpVel = 0;
			if (P1.muki == 0){
				if (P1.button[104] > 0){
					P1.JumpVel = -1;
				}
				// �E����
				else if (P1.button[106] > 0){

					P1.JumpVel = 1;
					// ��O�ő����Ă���
					if (P1.OldState == 30){
						P1.JumpVel = 2;
					}
				}
			}
			else if (P1.muki == 1){
				if (P1.button[104] > 0){
					P1.JumpVel = 1;
					// ��O�ő����Ă���
					if (P1.OldState == 30){
						P1.JumpVel = 2;
					}
				}
				// �E����
				else if (P1.button[106] > 0){
					P1.JumpVel = -1;
				}
			}

			// ��s���͂�����
			P1.K_Senkou[8] = 0;
		}
		// �X�e�[�g�ύX�A3�t���𒴂������W�����v��
		if (P1.time >= P1.animElem[P1.stateno]){
			// �W�����v�t���O���I��
			P1.JumpFlg = 1;
			// �������A�Ŏ��̃X�e�[�g��
			P1.More = 1, P1.time = 0;
			switch (P1.JumpVel){
			case 0:
				P1.stateno = 41;
				break;
			case 1:
				P1.stateno = 42;
				break;
			case 2:
				P1.stateno = 42;
				break;
			case -1:
				P1.stateno = 43;
				break;
			}
			P1.YVel = P1.C.jumpY;
			// �n�C�W�����v
			if (P1.hyJump){
				//�G�t�F�N�g
				EffStartB(16, P1.XPos, P1.YPos, 0, 0,
					0.24, 0.03, P1.muki);
				P1.YVel = P1.C.jumpY * 1.25;
				P1.AirJump--;
				afterimageSetting(P1.PSide, 16, 128,128,250);
			}
		}

		break;	// �W�����v�����I��

	case 41:	// ����
		P1.Muteki = 0;
		P1.XVel = 0;	// �����͍��E�ɔ�΂Ȃ�
		// �^�C���� 0 �̂Ƃ��A���ɔ��
		if (P1.time == 0){
			// ��i�W�����v���A�W�����v�͂�������
			{ 
				//P1.YPos -= 2;
			}
		}
		// ����
		if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
		// �n�ʂɂ��Ď��Ԃ� 0 ����Ȃ���
		if ((P1.YPos > GROUND) && (P1.time != 0)){
			P1.stateno = 47, P1.More = 1, P1.time = 0;
		}
		break;
	case 42:	// �O�W�����v
		P1.Muteki = 0;
		P1.XVel = P1.C.jumpF[0]; //���x�𑫂�;
		if (P1.JumpVel == 2)P1.XVel = P1.C.jumpF[1];//P1.XVel += (P1.XVel / 4) + 0.2;	// �_�b�V����
		if (P1.hyJump)P1.XVel = P1.C.jumpF[1];//*1.3; //���x�𑫂�;
		// �^�C���� 0 �̂Ƃ��A���ɔ��
		if (P1.time == 0){
			// ��i�W�����v���A�W�����v�͂�������
			{ 
				//P1.YPos -= 2;
			}
		}
		// ����
		if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
		// �n�ʂɂ��Ď��Ԃ� 0 ����Ȃ���
		if ((P1.YPos > GROUND) && (P1.time != 0)){
			P1.stateno = 47, P1.More = 1, P1.time = 0;
		}
		break;
	case 43:	// ���W�����v
		P1.Muteki = 0;
		P1.XVel = P1.C.jumpB; //���x�𑫂�;
		if (P1.hyJump)P1.XVel = P1.C.jumpB*1.2; //���x�𑫂�;
		// �^�C���� 0 �̂Ƃ��A���ɔ��
		if (P1.time == 0){
			// ��i�W�����v���A�W�����v�͂�������
			{ 
				//P1.YPos -= 2;
			}
		}
		// ����
		if (P1.YVel > 0)P1.stateno = 46, P1.time = 0;
		// �n�ʂɂ��Ď��Ԃ� 0 ����Ȃ���
		if ((P1.YPos > GROUND) && (P1.time != 0)){
			P1.stateno = 47, P1.More = 1, P1.time = 0;
		}
		break;
	case 45:	// �󒆃W�����v
		P1.XVel = 0, P1.YVel = -0.2;
		
		if (P1.time == 0){
			// ������
			if (P1.keyAtt[4]){
				P1.JumpVel = -1;
			}
			// �E����
			else if (P1.keyAtt[6]){
				if (P1.JumpVel == 2)P1.JumpVel = 2;
				else{ P1.JumpVel = 1; }
			}
			else{ P1.JumpVel = 0; }

			P1.AirJump--, P1.airDash--;
			P1.hyJump = false;

			// ��s���͂�����
			P1.K_Senkou[8] = 0;
		}

		// �U�����
		if (P1.time == 0){
			if (P1.XPos < P2.XPos && P1.muki == 1){
				P1.muki = 0;
				P1.JumpVel = -P1.JumpVel;
			}
			else if (P2.XPos < P1.XPos && P1.muki == 0){
				P1.muki = 1;
				P1.JumpVel = -P1.JumpVel;
			}
		}

		// �X�e�[�g�ύX�A2�t���𒴂������W�����v��
		if (P1.time > 1){
			//�G�t�F�N�g
			EffStartB(16, P1.XPos, P1.YPos - 20, 0, 0,
				0.24, 0.03, P1.muki);
			// �W�����v�t���O���I��
			P1.JumpFlg = 1;
			// �������A�Ŏ��̃X�e�[�g��
			P1.More = 1, P1.time = 0;
			switch (P1.JumpVel){
			case 0:
				P1.stateno = 41;
				break;
			case 1:
				P1.stateno = 42;
				break;
			case 2:
				P1.stateno = 42;
				break;
			case -1:
				P1.stateno = 43;
				break;
			}
			P1.YVel = P1.C.jumpA;
		}
		break;	// �W�����v�����I��

	case 46:	// �W�����v���
		P1.Muteki = 0;
		P1.ignoreG = false;
		if (P1.time >= P1.animElem[P1.stateno])P1.time = 0;

		// �n�ʂɂ��鎞
		if (P1.YPos + P1.YVel * 2 == GROUND){
			P1.stateno = 47, P1.More = 1, P1.time = 0;
		}

		break;

	case 47:	// ���n
		P1.A.damage = 0;
		P1.YPos = GROUND;
		P1.YVel = 0;
		// SE��炷
		if (P1.time == 1)SEStart(6);
		// �ŏ��Ƀ��b�N�I
		if (P1.time == 0){
			//�G�t�F�N�g
			//EffStartB(16, P1.XPos, P1.YPos, 0, 0,
				//0.20, 0.02, P1.muki);
			P1.Lock = 1;
		}
		// ���n �� �S�̃t���[��
		if (P1.time >= 1)P1.More = 0, P1.stateno = 0,
			P1.time = 0, P1.Lock = 0;

		break;

		/*
	case 50:	// �����K�[�h�J�n
		P1.SFlg = 0, P1.ctrl = 1;
		P1.YVel = 0;
		// �ŏ��Ƀ��b�N�I
		if (P1.time == 0)P1.Lock = 1;
		// �L�[�����ŃK�[�h����
		if (!P1.keyAtt[4]){
			P1.More = 1, P1.stateno += 2, P1.Lock = 1;
			P1.time = 0;
		}

		// �L�[���ł��Ⴊ��
		if (P1.keyAtt[2]){
			P1.More = 1, P1.stateno = 53, P1.Lock = 1;
			P1.time = 0;
		}
		break;
		*/

	case 50:	// �����K�[�h �d��
		P1.SFlg = 0;
		P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 2;
		// �ŏ��Ƀ��b�N�I
		if (P1.time == 0){
			P1.Lock = 1;
			P1.advancing = false;
		}

		// ������ł��Ⴊ�݃K�[�h��
		if (P1.keyAtt[2] > 0){
			P1.More = 1, P1.stateno = 53, P1.Lock = 1;
			P1.NoChangeTime = 1;
			P1.ctrl = 0;
		}

		// �A�h�K
		/*
		if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.keyAtt[4]) && (P1.advancing == false)
			&& (P1.StopTime == 0) && (P1.aGauge > (GUARD_MAX / 5)) && (P1.time + 2 < P1.D.nokezori)){
			if ((P2.XPos - P1.XPos < (SCREEN_W / 2)) && (P1.XPos - P2.XPos < (SCREEN_W / 2)))
				P2.D.addVelX += -7.0;
			EffStart(16, P1.XPos, P1.YPos, 10, -(P1.ySize / 1.5),
				0.04, 0.25, P1.muki);
			P1.colorCTime = 3;
			P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			SEStart(19);
			P1.advancing = true;
			P1.aGauge -= (GUARD_MAX / 5);
			P1.D.nokezori += 4;
			P1.Senkou[1] = 0, P1.Senkou[2] = 0;
		}
		*/

		// �K�[�h���s
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 0, P1.Lock = 1;
			P1.time = 0, P1.More = 1;
			P1.Muteki = ThrowMuteki_Guard, P1.mutekiF = 2;
			P1.ctrl = 1;
			P1.NoChangeTime = 0;
		}
		
		break;
	case 51:	// ���Ⴊ�� �d��
		P1.SFlg = 1;	// �ǉ�
		P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 2;
		if (P1.time == 0){
			P1.Lock = 1;
			P1.advancing = false;
		}

		// ������ĂȂ��Ɨ����K
		if (P1.keyAtt[2] == 0){
			P1.More = 1, P1.stateno = 54, P1.Lock = 1;
			P1.NoChangeTime = 1;
			P1.ctrl = 0;
		}
		
		// �A�h�K
		/*
		if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.keyAtt[4]) && (P1.advancing == false)
			&& (P1.StopTime == 0) && (P1.aGauge > (GUARD_MAX / 5)) && (P1.time + 2 < P1.D.nokezori)){
			if ((P2.XPos - P1.XPos < (SCREEN_W / 2)) && (P1.XPos - P2.XPos < (SCREEN_W / 2)))
				P2.D.addVelX += -7.0;
			EffStart(16, P1.XPos, P1.YPos, 10, -(P1.ySize / 1.5),
				0.04, 0.25, P1.muki);
			P1.colorCTime = 3;
			P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			SEStart(19);
			P1.advancing = true;
			P1.aGauge -= (GUARD_MAX / 5);
			P1.D.nokezori += 4;
			P1.Senkou[1] = 0, P1.Senkou[2] = 0;
		}
		*/

		// �I���ֈڍs
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 11, P1.Lock = 1;
			P1.time = 0, P1.More = 1;
			P1.Muteki = ThrowMuteki_Guard, P1.mutekiF = 2;
			P1.ctrl = 1;
			P1.NoChangeTime = 0;
		}
		
		break;
	case 52:	// �� �d��
		P1.SFlg = 2; //�ǉ�
		P1.Muteki = 1, P1.mutekiF = 2;
		if (P1.time == 0){
			P1.Lock = 1;
			P1.advancing = false;
		}
		// �U�����
		if (P1.time == 0){
			if ((P1.XPos < P2.XPos) && (P1.muki == 1))P1.muki = 0;
			else if ((P1.XPos > P2.XPos) && (P1.muki == 0))P1.muki = 1;
		}
		/*
		// ���n������ڍs
		if (P1.YPos + P1.YVel * 2 >= GROUND){
			P1.More = 1, P1.stateno = 51, P1.Lock = 1;
			if (P1.keyAtt[2] > 0)P1.stateno = 54;
		}
		*/

		// �A�h�K
		/*
		if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.keyAtt[4]) && (P1.advancing == false)
			&& (P1.StopTime == 0) && (P1.aGauge > (GUARD_MAX / 5)) && (P1.time + 2 < P1.D.nokezori)){
			if ((P2.XPos - P1.XPos < (SCREEN_W / 2)) && (P1.XPos - P2.XPos < (SCREEN_W / 2)))
				P2.D.addVelX += -8.2;
				P1.A.hbVelY -= 1.8;
			EffStart(16, P1.XPos, P1.YPos, 10, -(P1.ySize / 1.5),
				0.04, 0.25, P1.muki);
			P1.colorCTime = 3;
			P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			SEStart(19);
			P1.advancing = true;
			P1.aGauge -= (GUARD_MAX / 5);
			P1.D.nokezori += 4;
			P1.Senkou[1] = 0, P1.Senkou[2] = 0;
		}
		*/

		// �O�W�����v �� �S�̃t���[��
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 46, P1.Lock = 0;
			P1.time = 0, P1.More = 1;
			P1.Muteki = ThrowMuteki_Guard, P1.mutekiF = 2;
		}
		break;

	case 53:	// �����K�[�h(2����)
		P1.SFlg = 1;
		P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 2;
		// �ŏ��Ƀ��b�N�I
		if (P1.time == 0){
			P1.Lock = 1;
			P1.advancing = false;
		}

		// ������ł��Ⴊ�݃K�[�h��
		if (P1.keyAtt[2] == 0){
			P1.More = 1, P1.stateno = 50, P1.Lock = 1;
			P1.NoChangeTime = 1;
			P1.ctrl = 0;
		}

		// �K�[�h���s
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 0, P1.Lock = 1;
			P1.time = 0, P1.More = 1;
			P1.Muteki = ThrowMuteki_Guard, P1.mutekiF = 2;
			P1.ctrl = 1;
			P1.NoChangeTime = 0;
		}

		break;
	case 54:	// ���Ⴊ�݃K�[�h(2���͂Ȃ�)
		P1.SFlg = 0;
		P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 2;
		if (P1.time == 0){
			P1.Lock = 1;
			P1.advancing = false;
		}

		// ������ŏ�����
		if (P1.keyAtt[2] > 0){
			P1.More = 1, P1.stateno = 51, P1.Lock = 1;
			P1.NoChangeTime = 1;
			P1.ctrl = 0;
		}

		// �A�h�K
		/*
		if ((P1.Senkou[1] > 0) && (P1.Senkou[2] > 0) && (P1.keyAtt[4]) && (P1.advancing == false)
		&& (P1.StopTime == 0) && (P1.aGauge > (GUARD_MAX / 5)) && (P1.time + 2 < P1.D.nokezori)){
		if ((P2.XPos - P1.XPos < (SCREEN_W / 2)) && (P1.XPos - P2.XPos < (SCREEN_W / 2)))
		P2.D.addVelX += -7.0;
		EffStart(16, P1.XPos, P1.YPos, 10, -(P1.ySize / 1.5),
		0.04, 0.25, P1.muki);
		P1.colorCTime = 3;
		P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
		SEStart(19);
		P1.advancing = true;
		P1.aGauge -= (GUARD_MAX / 5);
		P1.D.nokezori += 4;
		P1.Senkou[1] = 0, P1.Senkou[2] = 0;
		}
		*/

		// �I���ֈڍs
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 11, P1.Lock = 1;
			P1.time = 0, P1.More = 1;
			P1.Muteki = ThrowMuteki_Guard, P1.mutekiF = 2;
			P1.ctrl = 1;
			P1.NoChangeTime = 0;
		}

		break;
	case 55:	// ��������
		P1.YVel = 0;
		P1.Muteki = 1, P1.mutekiF = 0;
		// �ŏ��Ƀ��b�N�I
		if (P1.time == 0)P1.Lock = 1;

		// �K�[�h���s
		if (P1.time >= 26){
			P1.stateno = 0, P1.Lock = 0;
			P1.time = 0, P1.More = 1;
			P1.Muteki = 0, P1.mutekiF = 0;
			P1.ctrl = 1;
			P1.NoChangeTime = 0;
		}

		break;

	case 56:	// �󒆓�������
		P1.Muteki = 1, P1.mutekiF = 0;
		// �ŏ��Ƀ��b�N�I
		if (P1.time == 0)P1.Lock = 1;
		// �U�����
		if (P1.time == 0){
			if ((P1.XPos < P2.XPos) && (P1.muki == 1))P1.muki = 0;
			else if ((P1.XPos > P2.XPos) && (P1.muki == 0))P1.muki = 1;
		}
		/*
		// �O�W�����v �� �S�̃t���[��
		if (P1.time >= P1.D.nokezori){
			P1.stateno = 46, P1.Lock = 0;
			P1.time = 0, P1.More = 1;
			P1.Muteki = 0, P1.mutekiF = 0;
		}
		*/
		break;

	case 60:	// �󒆃_�b�V��
		P1.SFlg = 2, P1.Lock = 0;
		P1.XVel = 0.5 + (P1.C.runF[0] * 0.75); //���x�𑫂�
		P1.YVel = 0;
		P1.ignoreG = true;

		if (P1.time == 0){
			SEStart(5);
			EffStart(16, (int)P1.XPos, (int)P1.YPos, -10, -(P1.ySize / 1.5),
				0.04, 0.25, P1.muki);
		}
		if (P1.time <= 6){
			P1.ctrl = 0;
		}
		else{ P1.ctrl = 1; }
		
		/*
		// ��s����
		if (P1.time <= 8){
			for (int i = 1; i < 5; i++){
				if (P1.button[i] > 0)P1.Senkou[i] = 10;
			}
		}
		if (P1.time == 9){
			for (int i = 1; i < 5; i++){
				if (P1.Senkou[i] > 0){
					P1.button[i] = 1;
				}
			}
		}
		*/

		if (P1.airDashTime == 0){
			P1.stateno = 46, P1.time = 0;
		}
		break;
	case 65:	// �󒆃o�b�N�_�b�V��
		P1.SFlg = 2, P1.Lock = 0;
		P1.XVel = -0.6 + (-P1.C.runF[0] * 0.75); //���x�𑫂�
		P1.YVel = 0;
		P1.ignoreG = true;

		if (P1.time == 0){
			SEStart(5);
			EffStart(16, P1.XPos, P1.YPos, 10, -(P1.ySize / 1.5),
				0.04, 0.25, P1.muki);
		}

		if (P1.time <= 6)P1.ctrl = 0;
		else{ P1.ctrl = 1; }
		// SE��炷
		//if ((P1.time == 20) || (P1.time == 40))SEStart(4);
		//P1.aNum2 = 0;

		/*
		// ��s����
		if (P1.time <= 8){
			for (int i = 1; i < 5; i++){
				if (P1.button[i] > 0)P1.Senkou[i] = 10;
			}
		}
		if (P1.time == 9){
			for (int i = 1; i < 5; i++){
				if (P1.Senkou[i] > 0){
					P1.button[i] = 1;
				}
			}
		}
		*/

		if (P1.airDashTime == 0){
			P1.stateno = 46, P1.time = 0;
		}
		break;
		//********************
		// 520�F�K�[�L����
		//********************
	case 520:	//����B
		P1.ctrl = 0, P1.SFlg = 0;
		// SE
		if (P1.StopTime == 0){
			if (P1.time == 0){
				SEStart(30);

				//P1.aGauge -= (GUARD_MAX / 2);
				P1.Power -= 1000;
				VelSet(0, 0);

				S.StopTime = 10;
				S.Anten = 10;
				P2.CFlg = 0;

				P1.colorCTime = 10;
				P1.colorC[0] = 100, P1.colorC[1] = 100, P1.colorC[2] = 255;
			}
		}

		if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 20, 0);

		// �U���o��܂Ŗ��G
		if (P1.D.counter == 1){
			P1.Muteki = 3;
			P1.mutekiF = 0;
		}

		// �q�b�g���Z�b�g�A�W�t��
		if (P1.time == 1){
			P1.MoveHit = 1;	// �P��
		}
		// �_���[�W�Z�b�g
		if ((P1.time >= 1) && (P1.time <= ANIMELEM - 1)){

			// [�_���[�W]
			Damage(0, 0);

			// [�Q�[�W] 
			P1.GetPow = 0, P1.GivePow = 0;
			HitTime(8, 20, 20, 18);

			// [�m�b�N�o�b�N]
			HitVel(-4.6, -2.6, -4.2, -3);
			P1.GuardF = 1;
			// [��炢���̕���]
			P1.fallF = 2;
			P1.HitAnim = 1000;	// ��i��炢
			P1.HitSE = 11;
		}

		// �S�̃t���[���𒴂����烊�Z�b�g
		if (P1.time >= ANIMELEM)P1.time = 0, P1.stateno = 0, P1.ctrl = 1;

		break;
		//********************
		// 550�F�Q�[�W�ǉ�
		//********************
	case 550:	//����B
		P1.ctrl = 0, P1.SFlg = 0;
		// SE
		if (P1.StopTime == 0){
			if (P1.time == 0){
				SEStart(30);

				//P1.aGauge -= (GUARD_MAX / 2);
				VelSet(0, 0);

				//S.StopTime = 10;
				P2.StopTime += 25;
				S.Anten = 20;
				P2.CFlg = 0;

				P1.Var[30] += 1;	// �񐔃J�E���g

				P1.colorCTime = 10;
				P1.colorC[0] = 200, P1.colorC[1] = 200, P1.colorC[2] = 200;
			}
		}

		if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 20, 0);

		if (P1.time == 18){
			P1.Power += 1000;
			// ���i�G�t�F�N�g
			EffStartB(17, P1.XPos, P1.YPos - (P1.GraphH / 2) * P1.GSize, 0, 0, 0.50, 0.50, P1.PSide);
			SEStart(31);
		}

		// �S�̃t���[���𒴂����烊�Z�b�g
		if (P1.time >= ANIMELEM){
			P1.time = 0, P1.stateno = 0, P1.ctrl = 1;
		}
		break;
		//********************
		// 551�F�o�[�X�g
		//********************
	case 551:	//�o�[�X�g
		P1.ctrl = 0;
		P1.ignoreG = false;
		
		P1.SFlg = 2;

		P1.YVel -= 0.2;
		// SE
		if (P1.StopTime == 0){
			if (P1.time == 0){
				SEStart(30);

				//P1.aGauge -= (GUARD_MAX / 2);
				//P1.Power += 1000;
				VelSet(0, -6);
				S.StopTime = 10;
				S.Anten = 10;
				//P2.CFlg = 0;

				P1.Var[30] += 1;	// �񐔃J�E���g
				//P1.colorCTime = 10;
				//P1.colorC[0] = 200, P1.colorC[1] = 200, P1.colorC[2] = 200;
			}
		}

		if (P1.time <= 11){
			P1.mutekiF = 0;
			P1.Muteki = 2;
		}

		// �q�b�g���Z�b�g
		if (P1.time == 1)P1.MoveHit = 1;	// �P��
		// �_���[�W�Z�b�g�A���� 5�t��
		if (P1.time >= 1){

			// [�_���[�W]
			Damage(0, 0);

			// [�Q�[�W] 
			Power(0);
			HitTime(6, 60, 60, 12);

			// [�m�b�N�o�b�N]
			HitVel(-2.8, -4.7, -2.6, -3.5);
			GuardVel(-3.8, -2.6 * 1.2);
			P1.HitAnim = 1030;	// �󒆋�炢
			// [��炢���̕���]
			P1.fallF = 2;
			// [�K�[�h����]
			P1.GuardF = 1;

			P1.HitSE = 13;
		}
		else {
			DamReset();
		}

		// ��
		if ((P1.time == 1) && (P1.StopTime == 0))PVOStart(P1.PSide, 20, 0);

		// �n�ʂɂ��鎞
		if (P1.YPos + P1.YVel * 2 == GROUND){
			P1.stateno = 47, P1.More = 1, P1.time = 0;
		}

		break;
	}

	// �߂�
	PSetSend();
}


// �_���[�W�ݒ�
void DamParam()
{
	// ���ʐݒ�

	switch (P1.stateno){
	case 1000:	// ������炢
		P1.ctrl = 0, P1.SFlg = 0;
		// ���̖��G���Ȃ��Ȃ�A�������G
		if(P1.Muteki == 0)P1.Muteki = 1, P1.mutekiF = 2;
		Furimuki();

		if ((P1.time == 0) && (P1.StopTime == 0)){
			n = GetRand(2);
			if (n == 0){
				PVOStart(P1.PSide, 31, 4);
			}
			else{
				PVOStart(P1.PSide, 32, 4);
			}
		}

		// ���x�����i���j
		if (P1.time >= 8 && P1.XVel != 0){
			P1.XVel = P1.XVel * 0.62;
		}

		// �߂��
		if (P1.time >= P1.D.nokezori - 2){
			P1.stateno = 1005, P1.time = 0;
		}
		// �ύX�E��
		if (P1.SFlg == 1){
			P1.stateno = 1020, P1.More = 1, P1.time = 0;
		}
		// �ύX�E��
		if ((P1.SFlg == 2) || (P1.YPos < GROUND)){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// �n�ʂȂ�
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > GROUND){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// ����Ă�����
		if (P1.Life <= 0){
			P1.stateno = 1030,
				P1.time = 0, P1.More = 1;
		}
		break;
	case 1005:	// ��i�߂�
		P1.ctrl = 0;
		P1.XVel = 0, P1.YVel = 0;
		if (P1.time >= 2){
			P1.stateno = 0, P1.time = 0;
			// �������G��t�^
			P1.Muteki = ThrowMuteki_Hit, P1.mutekiF = 2;
			P1.ctrl = 1;
		}
		break;
	case 1010:	// ������炢(��)
		P1.ctrl = 0, P1.SFlg = 0;
		// ���̖��G���Ȃ��Ȃ�A�������G
		if (P1.Muteki == 0)P1.Muteki = 1, P1.mutekiF = 2;
		Furimuki();

		if ((P1.time == 0) && (P1.StopTime == 0)){
			n = GetRand(2);
			if (n == 0){
				PVOStart(P1.PSide, 31, 4);
			}
			else{
				PVOStart(P1.PSide, 32, 4);
			}
		}

		// ���x�����i���j
		if (P1.time >= 8 && P1.XVel != 0){
			P1.XVel = P1.XVel * 0.62;
		}
		if (P1.time >= P1.D.nokezori - 2){
			P1.stateno = 1015, P1.time = 0;
		}
		// �ύX�E��
		if ((P1.SFlg == 2) || (P1.YPos < GROUND)){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// �n�ʂȂ�
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > GROUND){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// ����Ă�����
		if (P1.Life <= 0){
			P1.stateno = 1030,
				P1.time = 0, P1.More = 1;
		}
		break;
	case 1015:	// ���i�߂�
		P1.ctrl = 0;
		P1.XVel = 0, P1.YVel = 0;
		if (P1.time >= 2){
			P1.stateno = 0, P1.time = 0;
			// �������G��t�^
			P1.Muteki = ThrowMuteki_Hit, P1.mutekiF = 2;
			P1.ctrl = 1;
		}
		break;
	case 1020:	// ����炢
		P1.ctrl = 0, P1.SFlg = 1;
		// ���̖��G���Ȃ��Ȃ�A�������G
		if (P1.Muteki == 0)P1.Muteki = 1, P1.mutekiF = 2;
		Furimuki();

		if ((P1.time == 0) && (P1.StopTime == 0)){
			n = GetRand(2);
			if (n == 0){
				PVOStart(P1.PSide, 31, 4);
			}
			else{
				PVOStart(P1.PSide, 32, 4);
			}
		}

		if (P1.time >= P1.D.nokezori - 2){
			P1.stateno = 1025, P1.time = 0;
		}
		// �ύX�E��
		if ((P1.SFlg == 2) || (P1.YPos < GROUND)){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// �n�ʂȂ�
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > GROUND){
			P1.stateno = 1030, P1.More = 1, P1.time = 0;
		}
		// ����Ă�����
		if (P1.Life <= 0){
			P1.stateno = 1030,
				P1.time = 0, P1.More = 1;
		}
		break;
	case 1025:	// �߂�
		P1.ctrl = 0;
		if (P1.time >= 2){
			P1.stateno = 11, P1.time = 0;
			// �������G��t�^
			P1.Muteki = ThrowMuteki_Hit, P1.mutekiF = 2;
			P1.ctrl = 1;
		}
		break;
	case 1030:	// �󒆋�炢
		P1.ctrl = 0, P1.SFlg = 2;
		P1.ignoreG = false;
		Furimuki();

		if ((P1.time == 0) && (P1.StopTime == 0)){
			n = GetRand(2);
			if (n == 0){
				PVOStart(P1.PSide, 31, 4);
			}
			else{
				PVOStart(P1.PSide, 32, 4);
			}
		}

		//�ŏ����牺�ɂ����ꍇ�A�����߂�
		if ((P1.time == 0) && (P1.YPos >= GROUND)){
			P1.YPos = GROUND - 1;
		}
		// ���Ԍo�� �Ŗ߂��
		if ((P1.time >= P1.D.nokezori)){
			P1.stateno = 1035, P1.time = 0, P1.More = 1;
		}

		// �ǃo�E���h
		if ((
			(P1.XPos <= GAMENHAJI) || (P1.XPos >= STAGE_WIDTH - GAMENHAJI) || 
			(P1.XPos <= S.ScroolX + GAMENHAJI + 1) || (P1.XPos >= S.ScroolX + SCREEN_W - GAMENHAJI - 1)
			)
			&& (P1.time > 0) && (P1.YPos < GROUND) && (P1.D.wall > 0 && P1.D.wall <= 2)
			&& (P1.StopTime == 0)){
			P1.stateno = 1046, P1.time = 0, P1.More = 1;
		}

		// ���n
		if ((P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel * 2) >= GROUND) && (P1.StopTime == 0)){
			// ��o�E���h
			if (P1.D.bound == 1){
				P1.stateno = 1040, P1.time = 0, P1.More = 1;
			}
			// ���o�E���h
			else if (P1.D.bound == 2){
				P1.stateno = 1041, P1.time = 0, P1.More = 1;
			}
			// �_�E��
			else{
				// ���n���_�E�����f
				if(P1.fall == 1 || P1.Life <= 0){
					P1.stateno = 1050, P1.time = 0, P1.More = 1;
				}
				else if (P1.fall == 0){
					P1.stateno = 47, P1.time = 0, P1.More = 1;
				}
				// �n��󂯐g�\
				else if (P1.fall == 2){
					// [�󂯐g]
					// �󒆂ɂ��鎞�A�{�^���P�`�R����
					if ((P1.button[1] > 0) || (P1.button[2] > 0) ||
						(P1.button[3] > 0)){
						P1.stateno = 1080, P1.time = 0, P1.More = 1;
					}
					else {
						P1.stateno = 1050, P1.time = 0, P1.More = 1;
					}
				}
			}
		}


		break;
	case 1035:	// �߂�
		P1.ctrl = 0, P1.SFlg = 2;

		/*
		// ���n�����ꍇ�A�X�e�[�g���f��
		if ((P1.time >= 0) && (P1.YPos >= D_GROUND)){
			// �P�Ȃ�_�E���A�O�Ȃ璅�n
			if (P1.fall == 1){
				P1.stateno = 1050, P1.time = 0, P1.More = 1;
			}
			else if (P1.fall == 0){
				P1.stateno = 47, P1.time = 0, P1.More = 1;
			}
		}
		*/

		// �ǃo�E���h
		if ((
			(P1.XPos <= GAMENHAJI) || (P1.XPos >= STAGE_WIDTH - GAMENHAJI) ||
			(P1.XPos <= S.ScroolX + GAMENHAJI + 1) || (P1.XPos >= S.ScroolX + SCREEN_W - GAMENHAJI - 1)
			)
			&& (P1.time > 0) && (P1.YPos < GROUND) && (P1.D.wall > 0 && P1.D.wall <= 2)
			&& (P1.StopTime == 0)){
			P1.stateno = 1046, P1.time = 0, P1.More = 1;
		}

		// ���n
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel * 2) >= GROUND){
			// ��o�E���h
			if (P1.D.bound == 1){
				P1.stateno = 1040, P1.time = 0, P1.More = 1;
			}
			// ���o�E���h
			else if (P1.D.bound == 2){
				P1.stateno = 1041, P1.time = 0, P1.More = 1;
			}
			// �_�E��
			else{
				// ���n���_�E�����f
				if (P1.fall == 1 || P1.Life <= 0){
					P1.stateno = 1050, P1.time = 0, P1.More = 1;
				}
				else if (P1.fall == 0){
					P1.stateno = 47, P1.time = 0, P1.More = 1;
				}
				// �n��󂯐g�\
				else if (P1.fall == 2){
					// [�󂯐g]
					// �󒆂ɂ��鎞�A�{�^���P�`�R����
					if ((P1.button[1] > 0) || (P1.button[2] > 0) ||
						(P1.button[3] > 0)){
						P1.stateno = 1080, P1.time = 0, P1.More = 1;
					}
					else {
						P1.stateno = 1050, P1.time = 0, P1.More = 1;
					}
				}
			}
			break;
		}

		// [�󂯐g]
		// �����Ă鎞�ɁA
		// �󒆂ɂ��鎞�A�{�^���P�`�R����
		if ((P1.Life > 0) &&
			(P1.time >= 0) && (P1.YPos < D_GROUND)){
			if ((P1.button[1] > 0) || (P1.button[2] > 0) ||
				(P1.button[3] > 0)){
				P1.stateno = 1085, P1.time = 0, P1.More = 1;
			}
		}

		// �̗͂�0�̏ꍇ�_�E����
		if ((P1.Life <= 0) && (P1.YPos >= D_GROUND)){
			P1.stateno = 1050, P1.time = 0, P1.More = 1,
				P1.Lock = 1, P1.XVel = 0, P1.YVel = 0;
		}

		break;

	case 1040:	// ���o�E���h�i���j�A�ǌ��s�\
		P1.ctrl = 0, P1.SFlg = 2;
		P1.D.bound = 0;
		P1.D.boundCount = 2;
		P1.Muteki = 1, P1.mutekiF = 0;

		if (P1.time == 0){
			P1.XVel = P1.D.bound_xvel;
			P1.YVel = P1.D.bound_yvel;
			P1.YPos = D_GROUND - 2;
			P1.D.yaccel = GRAVITY;
			SEStart(8);
			EffStartB(19, P1.XPos, P1.YPos, 0, -7,
				0.25, 0.04, P1.muki);
		}

		// ���n�Ń_�E��
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > D_GROUND){
			// �_�E��
			P1.stateno = 1050, P1.time = 0, P1.More = 1;
			// �n��󂯐g�\
			if (P1.fall == 2){
				// [�󂯐g]
				// �󒆂ɂ��鎞�A�{�^���P�`�R����
				if ((P1.button[1] > 0) || (P1.button[2] > 0) ||
					(P1.button[3] > 0)){
					P1.stateno = 1080, P1.time = 0, P1.More = 1;
				}
				else {
					P1.stateno = 1050, P1.time = 0, P1.More = 1;
				}
			}
		}
		break;

	case 1041:	// ���o�E���h�i��j
		P1.ctrl = 0, P1.SFlg = 2;
		P1.D.bound = -1;
		P1.D.boundCount = 1;
		P1.Muteki = 0, P1.mutekiF = 0;

		if (P1.time == 0){
			P1.XVel = P1.D.bound_xvel;
			P1.YVel = P1.D.bound_yvel;
			P1.YPos = D_GROUND - 2;
			P1.D.yaccel = GRAVITY;
			SEStart(8);
			EffStartB(16, P1.XPos, P1.YPos, 0, -7,
				0.25, 0.04, P1.muki);
		}

		// ���n�Ń_�E��
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > D_GROUND){
			// �_�E��
			P1.stateno = 1050, P1.time = 0, P1.More = 1;
			// �n��󂯐g�\
			if (P1.fall == 2){
				// [�󂯐g]
				// �󒆂ɂ��鎞�A�{�^���P�`�R����
				if ((P1.button[1] > 0) || (P1.button[2] > 0) ||
					(P1.button[3] > 0)){
					P1.stateno = 1080, P1.time = 0, P1.More = 1;
				}
				else {
					P1.stateno = 1050, P1.time = 0, P1.More = 1;
				}
			}
		}
		break;
	
	case 1046:	// �ǃo�E���h�i��j
		P1.ctrl = 0, P1.SFlg = 2;
		P1.Muteki = 0, P1.mutekiF = 0;

		if (P1.time == 0){
			/*
			P1.XVel = 0.4;
			P1.YVel = -6.4;
			P1.YPos = P1.YPos;
			*/
			P1.XVel = P1.D.wall_xvel;
			P1.YVel = P1.D.wall_yvel;
			P1.YPos = P1.YPos;
			
			P1.D.yaccel = 0.3;
			SEStart(8);
			P1.D.wall++;	// �����ǃo�����Ȃ�
			EffStartB(16, P1.XPos, P1.YPos, 0, - (P1.ySize / 1.5), 
				0.04, 0.25, P1.muki);
		}

		// ���n�Ń_�E��
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel) > D_GROUND){
			// �_�E��
			P1.stateno = 1050, P1.time = 0, P1.More = 1;
		}

		break;
	case 1050:	// �_�E��
		P1.ctrl = 0, P1.SFlg = 0;
		P1.Muteki = 1, P1.mutekiF = 0;
		P1.XVel = 0, P1.YVel = 0;
		P1.YPos = GROUND;
		// SE
		if (P1.time == 0 && P1.StopTime == 0){
			SEStart(8);
			EffStartB(16, P1.XPos, P1.YPos, 0, -1, 
				0.20, 0.04, P1.muki);
		}
		if (P1.time >= 20){
			// �N���オ�萺
			if ((P1.time == 20) && (P1.Life <= 0) && (P1.LifeC > 0))VOStart(P1.Name, 34, 0);
			P1.stateno = 1055, P1.time = 0, P1.More = 1;
			// ���S�ɂ���Ă�����
			if (P1.Life <= 0 && P1.LifeC <= 0){
				P1.stateno = 1070,
					P1.time = 0, P1.More = 1;
			}
		}

		// ���������Ă����i�_�u��KO�j
		//if (P2.Life <= 0 && P2.LifeC <= 0)ModeChange(2);

		break;

	case 1055:	// �N���オ��
		P1.ctrl = 0, P1.Lock = 0;
		P1.Muteki = 1, P1.mutekiF = 0;
		P1.XVel = 0, P1.YVel = 0;
		P1.D.bound = 0, P1.D.wall = 0;
		// �̗͉񕜓�
		if (S.roundState == 2){
			
			//VOStart(P1.Name, 34, 0);
			/*
			P1.Life = P1.C.lifeMax;
			P2.Life = P2.C.lifeMax;
			P1.aGauge = GUARD_MAX;
			P2.aGauge = GUARD_MAX;
			P1.GRecovery = 0;
			P2.GRecovery = 0;
			P1.D.armor = 0;
			P2.D.armor = 0;
			*/

			// �L�����ϐ�������
			for (int i = 0; i < VAR_MAX; i++){
				//if ((P1.Name == HELIOS) && (i == 11));
				//else if ((P1.Name == CORNEL) && (i == 20));
				{ P1.Var[i] = 0; }
				
				//if ((P2.Name == HELIOS) && (i == 11));
				//else if ((P2.Name == CORNEL) && (i == 20)); //�R�[�l�������v�f
				{ P2.Var[i] = 0; }
				//P1.Var[i] = 0;
				//P2.Var[i] = 0;
			}
		}
		// ���G�t���Ȃ��痧����
		if (P1.time >= P1.animElem[1055]){
			P1.stateno = 0, P1.time = 0;
			//P1.More = 0;
			P1.More = 1;
			P1.ctrl = 1;
			//P1.Muteki = 3, P1.mutekiF = 0;
			P1.Muteki = ThrowMuteki_Down, P1.mutekiF = 2;
		}

		
		break;
	case 1060:	// ���b�N�Z��炢
		P1.ctrl = 0, P1.SFlg = 0;
		P1.ignoreG = true;

		// ��u�ނĂ�
		if (P1.time <= 1){
			P1.Muteki = 1;
			P1.mutekiF = 0;
		}
		
		// ����Ƒ��ł�
		if (P2.stateno >= 1000){
			P1.stateno = 1000, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 30;
		}
		// �o�O�œ�����
		else if (P2.stateno < 200 && P1.time > 30){
			P1.stateno = 1000, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 10;
		}

		// �F�����s���������ꍇor�n�ʂɖ��܂�
		if ((P1.YPos < -100) || (P1.YPos > GROUND + 30)){
			P1.stateno = 1030, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 30;
		}
		break;
	case 1065:	// ���b�N�Z��炢�i�󒆁j
		P1.ctrl = 0, P1.SFlg = 2;
		P1.ignoreG = true;
		
		// ����Ƒ��ł�
		if (P2.stateno >= 1000){
			P1.stateno = 1030, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 30;
		}
		// �o�O�œ�����
		else if (P2.stateno < 200 && P1.time > 30){
			P1.stateno = 1030, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 10;
		}

		// �F�����s���������ꍇ
		if ((P1.YPos < -50) || (P1.YPos > GROUND + 30)){
			P1.stateno = 1030, P1.More = 1;
			P1.time = 0, P1.D.nokezori = 30;
		}
		break;

	case 1070:	// �s�k
		P1.ctrl = 0, P1.Lock = 0, 
		P1.Muteki = 1, P1.mutekiF = 0;
		P1.XVel = 0;

		break;

	case 1080:	// �n��󂯐g
		P1.ctrl = 0,
		P1.Muteki = 1, P1.mutekiF = 0;
		P1.SFlg = 2;
		P1.ignoreG = false;
		P1.D.bound = 0, P1.D.wall = 0;

		if (P1.time == 0){
			P1.colorCTime = 7;
			P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			P1.D.yaccel = GRAVITY;
		}

		if (P1.time == 0){
			// �󂯐g�����͌Œ�
			P1.XVel = 0;
			P1.YPos = GROUND;
			EffStartB(16, P1.XPos, P1.YPos, 0, -1,
				0.20, 0.04, P1.muki);
		}

		// ����炷
		if (P1.time == 0)SEStart(5), P1.YVel = -4.6;

		// ���n
		if (P1.YVel + P1.C.yAccel > 0 && (P1.YPos + P1.YVel * 2) >= GROUND){
			P1.stateno = 47, P1.time = 0, P1.More = 1;
			// �������G�t�^
			P1.mutekiF = 2;
			P1.Muteki = 10;
		}
		break;

	case 1085:	// �󒆎󂯐g
		P1.ctrl = 0, 
		P1.Muteki = 1, P1.mutekiF = 0;
		P1.SFlg = 2;
		P1.ignoreG = false;
		P1.D.bound = 0, P1.D.wall = 0;

		if (P1.time == 0){
			P1.D.yaccel = GRAVITY;
			P1.colorCTime = 8;
			P1.colorC[0] = -1, P1.colorC[1] = -1, P1.colorC[2] = -1;
			P1.XVel = 0;
			P1.AirJump = 1;
			if(P1.C.airDash > 0)P1.airDash = 1;
		}

		// �n�ʂɖ��܂��Ă���
		if (P1.YPos > GROUND)P1.YPos = GROUND;

		if (P1.time == 0){
			// �󂯐g������I��
			if (P1.keyAtt[4]){
				P1.XVel = -2.2;
			}
			else if (P1.keyAtt[6]){
				P1.XVel = 2.2;
			}
			else { P1.XVel = 0; }
		}

		// ����炷
		if (P1.time == 1)SEStart(5), P1.YVel = -4.8;
		if (P1.time >= 16){
			// �W�����v���
			P1.stateno = 46, P1.Muteki = 0,
				P1.time = 0, P1.More = 0;
			P1.ctrl = 1;
		}
		break;

	}

	// �߂�
	PSetSend();
}

// �U�����
void Furimuki()
{
	// �U�����
	if ((P1.time == 0) && (P1.muki == P2.muki)){
		if (P2.muki == 1){
			P1.muki = 0;
		}
		else if (P2.muki == 0){
			P1.muki = 1;
		}
	}
}

static void PSetSend()
{
	// �߂�
	if (P1.Name == EIN){
		GetP_EinParam(P1, P2);
		GetH_EinParam(H1, H2);
		GetS_EinParam(S);
	}
	else if (P1.Name == CORNEL){
		GetP_CorParam(P1, P2);
		GetH_CorParam(H1, H2);
		GetS_CorParam(S);
	}
	else if (P1.Name == BOUNCER){
		GetP_BouParam(P1, P2);
		GetH_BouParam(H1, H2);
		GetS_BouParam(S);
	}
	else if (P1.Name == HELIOS){
		GetP_HelParam(P1, P2);
		GetH_HelParam(H1, H2);
		GetS_HelParam(S);
	}
	else if (P1.Name == HYDE){
		GetP_HydParam(P1, P2);
		GetH_HydParam(H1, H2);
		GetS_HydParam(S);
	}
	else { 
		GetP_HydParam(P1, P2); 
		GetH_HydParam(H1, H2);
		GetS_HydParam(S);
	}
}

// ���Ԃ��t�ɂ���
void GetP_SameParam(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_SameParam(System_t GS)
{
	S = GS;
}

void GetH_SameParam(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}

// Name���擾�A���łɓǂݍ���
void GetN_SameParam(std::vector<Name_t>GN)
{
	// �f�t�H���g
	St.push_back(vector<State>());
	Load_StateDef(-1);
	for (int i = 0; i < Character_Max(); i++){
		N.push_back(GN[i]);
		St.push_back(vector<State>());
		Load_StateDef(i);
	}
}

