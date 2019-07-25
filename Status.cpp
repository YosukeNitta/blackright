#include "pch.h"

#include <stdio.h>	// �t�@�C������ɕK�v�ȃw�b�_�t�@�C��
#include <string.h>	// �����񑀍�ɕK�v�ȃw�b�_�t�@�C��
#include <ctype.h>	// isdigit�ɕK�v

#include <string>	// string�Ɏg��
#include <vector>	// �����񑀍�ɕK�v�ȃw�b�_�t�@�C��

using namespace std;	// ���O��Ԃ炵���A�����std::�Ə����K�v���Ȃ��Ȃ�

//�f�t�@�C����----------------------------------------------

//�ް���`��------------------------------------------------
static vector<string>tlist;

// �X�e�[�g�ɓ����f�[�^
class stateData{
public:
	stateData(){

	}
	void math(){

	}
};

// �X�e�[�g
// ���̒��Ɏw��������
class state{
private:
	int stateNum;	// �X�e�[�g�̔ԍ�[state ����]
	//vector<stateData>sdata;
public: 
	// �R���X�g���N�^
	state(){
		stateNum = -1;
	}
	state(int num){
		stateNum = num;
	}

	// stateNum
	int Get_stateNum(int n){
		stateNum = n;
	}
	int Send_stateNum(){
		return stateNum;
	}
};

// ����ɑS�Ă̓��e������
static vector<state>stateBox;

//�����֐��錾��--------------------------------------------
static char t_Common[7] = { "Common" };	//

static string fn1("char/");
static string fn2("/st.txt");

//�v���O������----------------------------------------------

void load_status(Player gp)
{
	// ����Z�b�e�B���O //

	int i, fp, num = 0;

	int input[TX_MAX];
	char inputc[TX_MAX];	// input��inputc�ɕ������͂���
	int flg = -1;	// ���������������߂�
	boolean iflg = false;	// �����t���O
	boolean nextRaw = false;

	////
	// ���͂����ŕ����̊֐��錾
	//tlist.push_back("Common");
	tlist.push_back("gsize");
	tlist.push_back("xsize");
	tlist.push_back("ysize");
	tlist.push_back("life");
	
	tlist.push_back("walk.f");
	tlist.push_back("walk.b");
	tlist.push_back("run.f");
	tlist.push_back("run.b");
	tlist.push_back("jump.y");
	tlist.push_back("jump.a");
	tlist.push_back("jump.f");
	tlist.push_back("jump.b");
	tlist.push_back("yaccel");
	tlist.push_back("airjump");
	tlist.push_back("airdash");
	tlist.push_back("combolate");
	tlist.push_back("clmax");
	tlist.push_back("clmin");
	//
	////

	// ���O����
	
	string fname;
	fname = fn1 + gp.nameC + fn2;
	//fname = "char/cornel/st.txt";

	// �t�@�C�����J�� //
	fp = FileRead_open(fname.c_str());//�t�@�C���ǂݍ���
	if (fp == NULL){			// �G���[���N��������
		fp = FileRead_open("char/def_data.txt");	// ���ʐݒ���J��
		if (fp == NULL){	// ������_���Ȃ�
			printfDx("error def_data\n");// ������\��
			return;
		}
	}

	//string copy;	// string�Ƃ��Ďg���p
	while (1){
		//=========================
		// ��{���[�v
		// ���̃��[�v�I����ɉ���
		//=========================
		for (i = 0; i < TX_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1�����擾����
			
			// ���s�Ŏ��̍s�ɐi��
			if (input[i] == '\n'){
				inputc[i] = '\0';
				i = -1;
				break;
			}

			// �J���}�ŉ��s(��O��-1��break�ő��v)�A����EOF���ƃt���[�Y
			if (inputc[i] == ';'){					//�X���b�V���������
				inputc[i] = '\0';	// �����܂ł͕����ɂȂ�
				while (FileRead_getc(fp) != '\n');	//���s�܂Ń��[�v
				i = -1;	// ���̃��[�v��
				break;	// ���̍�Ƃ͂Ȃ��̂ŒE�o
				//continue;
			}

			// ������ɂ���
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == '/' || input[i] == ':' ||
				input[i] == '='){ //�J���}�����s�Ȃ�
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j


				break;	// i��1���₵�ă��[�v�E�o�i�������j
				// �E�o�������i��1������?
			}

			// �ǂݍ��ݏI��
			if (input[i] == EOF){//�t�@�C���̏I���Ȃ�
				goto EXFILE;//�I��
			}
		}	// ��{���[�v�I��


		// �ŏ��ɐ��������ɂ��āA�����������Ă���ύX����
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

		// �S�Đ�����������
		if (iflg){
			// �������
			switch (flg){
				case 0:	// gsize
					gp.GSize = atof(inputc);
					break;
				case 1:	// xsize
					gp.xSize = atoi(inputc);
					break;
				case 2:	// ysize
					gp.ySize = atoi(inputc);
					break;
				case 3:	// life
					gp.C.lifeMax = atoi(inputc);
					break;
				case 4:	// walk.f
					gp.C.walkF = atof(inputc);
					break;
				case 5:	// walk.b
					gp.C.walkB = atof(inputc);
					break;
				case 6:	// run.f
					gp.C.runF[num] = atof(inputc);
					break;
				case 7:	// run.b
					gp.C.runB[num] = atof(inputc);
					break;
				case 8:	// jump.y
					gp.C.jumpY = atof(inputc);
					break;
				case 9:	// jump.a
					gp.C.jumpA = atof(inputc);
					break;
				case 10:	// jump.f
					gp.C.jumpF[num] = atof(inputc);
					break;
				case 11:	// jump.b
					gp.C.jumpB = atof(inputc);
					break;
				case 12:	// yaccel
					gp.C.yAccel = atof(inputc);
					break;
				case 13:	// airjump
					gp.C.airJump = atoi(inputc);
					break;
				case 14:	// airdash
					gp.C.airDash = atoi(inputc);
					break;
				case 15:	// combolate
					gp.A.comboLate = atof(inputc);
					break;
				case 16:	// clmax
					gp.A.cl_max = atof(inputc);
					break;
				case 17:	// clmin
					gp.A.cl_min = atof(inputc);
					break;
				default: break;
			}
			num++;	// num��i�߂�
		}
		
		// �����ł͂Ȃ�
		if (!iflg){
			// �����������Ă邩���f
			for (int h = 0; h < (int)tlist.size(); h++){
				if (strcmp(inputc, tlist[h].c_str()) == 0){
					flg = h;
					num = 0;
					break;
				}
			}
		}	// if�I��
		
		//flg�͍ŏ��̍s�ŏ�����
		if (i == -1)flg = -1; 

	}	// while ���[�v�I��


	// �t�@�C�������
EXFILE:
	FileRead_close(fp);

	// �l��ԋp
	GetP1_Player(gp);
}
