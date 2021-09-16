//==================================================================================
//
//			���l�v�Z�̊Ǘ���
//
//========================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include <stdio.h>	// �t�@�C������ɕK�v�ȃw�b�_�t�@�C��
#include <string.h>	// �����񑀍�ɕK�v�ȃw�b�_�t�@�C��
#include <ctype.h>	// isdigit�ɕK�v

#include <string>	// string�Ɏg��
#include <vector>

//�f�t�@�C����----------------------------------------------
#define BOX_MAX	(10)

//�ް���`��------------------------------------------------

// ��d��Air���Z�b�g


static Air air[STATE_MAX];
//static Air kawari[STATE_MAX];	// ����p

//static Name_t N[C_M]; 
static std::vector<Name_t>N;
static boolean load_1;

// ���O�p
static std::string fn1("char/");
static std::string fn2("/box.air");

//�����֐��錾��--------------------------------------------
static char t_Action[7] = { "Action" };	// sffno
static char t_Clsn2Default[13] = { "Clsn2Default" };	// num1,2
static char t_Clsn1[6] = { "Clsn1" };			// sizeX
static char t_Clsn2[6] = { "Clsn2" };			// sizeY
static char t_Loopstart[10] = { "Loopstart" };	// posX
static char t_H[2] = { "H" };
static char t_V[2] = { "V" };
static char t_HV[3] = { "HV" };
static char t_A[2] = { "A" };

//�v���O������----------------------------------------------

// mugen�L�q���R�s�[����
// box.air�ł��̂܂܂����Ă����
void load_air(int na, int pSide)
{
	// ����Z�b�e�B���O //
	int i, fp, num;
	int bo = 0;	 // �{�b�N�X�ԍ��Ŏg�p
	int	ht = 0, at = 0;	// Stateno
	int clsnMax = -1;	// clsn�̐�
	int clsnCount = 0;	// clsn��ʂ�����(0�͔��萔�J�E���g�Ɏg��)
	boolean clsnDefault = false;	// clsnDefault���L����
	boolean clsnSet = false;	// clsnDefault����x�g������
	int anum = -1;	// �z��̏ꏊ

	//int input[TX_MAX];
	int *input;
	input = (int *)malloc(TX_MAX);
	if (input == NULL) {
		printfDx("input�ǂݍ��߂�");
	}

	char inputc[TX_MAX];	// input��inputc�ɕ������͂���
	int flg = 0;	// ���������������߂�
	boolean iflg;	// �����t���O

	////
	// ���͂����ŕ����̊֐��錾

	// ���O����
	std::string fname;
	fname = fn1 + N[na].nameC + fn2;	// ���O����

	// AIR�ϐ����� //
	for (int n = 0; n < STATE_MAX; n++){
		air[n].setup();
	}
	
	// �t�@�C�����J�� //
	fp = FileRead_open(fname.c_str());//�t�@�C���ǂݍ��� c_str��char�ɂł���H
	if (fp == NULL){			// �G���[���N��������
		printfDx("error air\n");// ������\��
		return;
	}
	for (i = 0; i < 2; i++){//�ŏ���2�s�ǂݔ�΂�
		while (FileRead_getc(fp) != '\n');
	}

	while (1){
		//=========================
		// ��{���[�v
		// ���̃��[�v�I����ɉ���
		//=========================
		for (i = 0; i < TX_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1�����擾����

			// �J���}�ŉ��s
			if (inputc[i] == ';'){					//�X���b�V���������
				while (FileRead_getc(fp) != '\n');	//���s�܂Ń��[�v
				i = -1;//�J�E���^���ŏ��ɖ߂��� (�߂����Ƃ���0�ɂȂ�)
				continue;
			}

			// ���s�Ŏ��̍s�ɐi��
			if (input[i] == '\n'){
				inputc[i] = '\0';
				i = -1;
				break;
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
			if (inputc[0] == '0' || inputc[0] == '1' ||
				inputc[0] == '2' || inputc[0] == '3' || 
				inputc[0] == '4' || inputc[0] == '5' || 
				inputc[0] == '6' || inputc[0] == '7' || 
				inputc[0] == '8' || inputc[0] == '9' ||
				inputc[0] == '-')iflg = true;

			// ����(HV�̔���)
			if ((flg == 5) && (num == 5)){
				if (iflg)
				{
					flg = 5;
					num = 0;
					bo++;
				}

				if (inputc[0] == 'H'){
					air[anum].B[bo].HV = 1;
					iflg = true;
				}
				else if (inputc[0] == 'V'){
					air[anum].B[bo].HV = 2;
					iflg = true;
				}
				else if (inputc[0] == 'H' && inputc[1] == 'V'){
					air[anum].B[bo].HV = 3;
					iflg = true;
				}
			}

		}

		// �S�Đ�����������
		if (iflg){
			// �������
			switch (flg){
			case 1:	// Stateno(Action)

				anum = atoi(inputc);
				air[anum].stateno = atoi(inputc);	// �X�e�[�g�ԍ�������
				air[anum].ANum = 0;	// ������0�ɂ���
				flg = 5;	// �摜�ݒ�(time)�Ɉڍs
				num = 0;

				// �����ݒ���s��
				clsnMax = -1;
				clsnCount = 0;
				clsnDefault = false;
				bo = 0;		// �{�b�N�X�ԍ��Ŏg�p
				at = 0;
				ht = 0;

				break;

			case 2: // Clsn2Default 

				// ����̌�Aclsn2�̓��e������
				clsnMax = atoi(inputc);
				clsnDefault = true;
				air[anum].def = true;
				flg = 0, num = 0;
				clsnCount = 1;
				break;

			case 3:	// Clsn1
				if (clsnCount == 0){
					// clsnDefault��ʂ��Ă��Ȃ��ꍇ
					clsnMax = atoi(inputc);
					flg = 0, num = 0;
					clsnCount++;
				}
				else {
					switch (num){
					case 0:
						at = atoi(inputc),
							air[anum].B[bo].a[at] = true;
						break;
					case 1: air[anum].B[bo].ax[at] = atoi(inputc); break;
					case 2: air[anum].B[bo].ay[at] = atoi(inputc); break;
					case 3: air[anum].B[bo].aw[at] = atoi(inputc); break;
					case 4: air[anum].B[bo].ah[at] = atoi(inputc); 
						// ���΂�������߂�
						if (air[anum].B[bo].ax[at] > air[anum].B[bo].aw[at]){
							int hozon = air[anum].B[bo].ax[at];
							air[anum].B[bo].ax[at] = air[anum].B[bo].aw[at];
							air[anum].B[bo].aw[at] = hozon;
						}
						if (air[anum].B[bo].ay[at] > air[anum].B[bo].ah[at]){
							int hozon = air[anum].B[bo].ay[at];
							air[anum].B[bo].ay[at] = air[anum].B[bo].ah[at];
							air[anum].B[bo].ah[at] = hozon;
						}
						break;
					}
					if (num < 4){ num++; }
					// clsn2�I��
					else{
						// �J�E���g���ő吔�Ɠ����ɂȂ�����
						if (clsnCount >= clsnMax){
							flg = 5, num = 0;	// time�Ɉڍs
							clsnCount = 0;
							clsnMax = -1;
						}
						// �܂�clsn�𑱍s
						else{
							flg = 0, num = 0;
							clsnCount++;

						}
					}
				}
				break;
			case 4:	// Clsn2
				if (clsnCount == 0){
					// clsnDefault��ʂ��Ă��Ȃ��ꍇ
					clsnMax = atoi(inputc);
					flg = 0, num = 0;
					clsnCount++;
				}
				else {
					switch (num){
					case 0:
						ht = atoi(inputc),
						air[anum].B[bo].h[ht] = true;
						break;
					case 1: air[anum].B[bo].hx[ht] = atoi(inputc); break;
					case 2: air[anum].B[bo].hy[ht] = atoi(inputc); break;
					case 3: air[anum].B[bo].hw[ht] = atoi(inputc); break;
					case 4: air[anum].B[bo].hh[ht] = atoi(inputc); 
						// ���肪���΂�������߂�
						if (air[anum].B[bo].hx[ht] > air[anum].B[bo].hw[ht]){
							int hozon = air[anum].B[bo].hx[ht];
							air[anum].B[bo].hx[ht] = air[anum].B[bo].hw[ht];
							air[anum].B[bo].hw[ht] = hozon;
						}
						if (air[anum].B[bo].hy[ht] > air[anum].B[bo].hh[ht]){
							int hozon = air[anum].B[bo].hy[ht];
							air[anum].B[bo].hy[ht] = air[anum].B[bo].hh[ht];
							air[anum].B[bo].hh[ht] = hozon;
						}
						break;
					}

					// �܂�clsn2������
					if (num < 4){ num++; }
					// clsn2�I��	
					else{
						// �J�E���g���ő吔�Ɠ����ɂȂ�����
						if (clsnCount >= clsnMax){
							flg = 5, num = 0;	// time�Ɉڍs
							clsnCount = 0;
							clsnMax = -1;
							// �f�t�H���g�{�b�N�X�p��
							if (clsnDefault){
								for (int d = 0; d < HIT_MAX; d++){
									air[anum].defhx[d] = air[anum].B[bo].hx[d];
									air[anum].defhy[d] = air[anum].B[bo].hy[d];
									air[anum].defhw[d] = air[anum].B[bo].hw[d];
									air[anum].defhh[d] = air[anum].B[bo].hh[d];
								}
								clsnDefault = false;
							}
						}
						else{
							flg = 0, num = 0;
							clsnCount++;
						}
					}// num4�ȏ�I��
				}
				break;
			case 5:	// time(�摜)
				switch (num){

				case 0: air[anum].B[bo].num1 = atoi(inputc); break;
				case 1: air[anum].B[bo].num2 = atoi(inputc); break;
				case 2: air[anum].B[bo].posX = atoi(inputc); break;
				case 3: air[anum].B[bo].posY = atoi(inputc); break;
				case 4: air[anum].B[bo].time = atoi(inputc);
					// �f�t�H���g�{�b�N�X�p��
					if ((air[anum].B[bo].hw[0] == 0) && (air[anum].B[bo].hh[0] == 0) && air[anum].def && (bo > 0)){
						for (int d = 0; d < HIT_MAX; d++){
							air[anum].B[bo].h[d] = true;
							air[anum].B[bo].hx[d] = air[anum].defhx[d];
							air[anum].B[bo].hy[d] = air[anum].defhy[d];
							air[anum].B[bo].hw[d] = air[anum].defhw[d];
							air[anum].B[bo].hh[d] = air[anum].defhh[d];
						}
					}
					break;

				}
				if (num < 5){
					num++;
				}
				// case4��ʂ��đ��s
				else{
					flg = 5, num = 0;
					bo++;	// ���֐i�߂�
				}
				break;

			default: break;
			}

		}

		// �����ł͂Ȃ�
		if (!iflg){
			// �X�e�[�g�A���萔�A������A��炢�̔��f
			// "Action "
			if (strcmp(inputc, t_Action) == 0){
				flg = 1; clsnSet = false;
			}
			// "Clsn2Default "
			else if (strcmp(inputc, t_Clsn2Default) == 0){
				// �܂��g���ĂȂ�������
				if (!clsnSet){
					flg = 2; clsnSet = true;
				}
			}
			// "Clsn1 "
			else if (strcmp(inputc, t_Clsn1) == 0){
				if ((flg == 5) && (num == 5))bo++;
				flg = 3;
			}
			// "Clsn2 "
			else if (strcmp(inputc, t_Clsn2) == 0){
				if ((flg == 5) && (num == 5))bo++;
				flg = 4;
			}
			// "Loopstart "
			//else if (strcmp(inputc, t_Loopstart) == 0){
			//flg = 5, num = 0;
			//}
		}


	}	// while ���[�v�I��


	// �t�@�C�������
EXFILE:
	FileRead_close(fp);
	
	// aTime����
	int time;
	// �Z�b�g�X�^�[�g
	for (int n1 = 0; n1 < STATE_MAX; n1++){
		time = 0;
		for (int a = 0; a < STATE2_MAX; a++){
			// �I���łȂ��Ȃ�
			if (air[n1].B[a].time != -2)
				time += air[n1].B[a].time;
		}
		air[n1].aTime = time;
	}

	// air�ɑ��
	for (int n = 0; n < STATE_MAX; n++) {
		// ���肪�Ȃ��Ȃ�ݒ���`�F�b�N
		if (air[n].stateno == -1) {
			// �����K�[�h(���ۂ͂��Ⴊ��)
			if (n == 53) {
				air[n] = air[51];	// ��{���Ⴊ��
				// �G�̂ݗ���
				for (int aa = 0; aa < 50; aa++) {
					air[n].B[aa].num1 = air[50].B[aa].num1;
					air[n].B[aa].num2 = air[50].B[aa].num2;
				}
			}
			// ���Ⴊ�݃K�[�h(���ۂ͗���)
			if (n == 54) {
				air[n] = air[50];
				for (int aa = 0; aa < 50; aa++) {
					air[n].B[aa].num1 = air[51].B[aa].num1;
					air[n].B[aa].num2 = air[51].B[aa].num2;
				}
			}

			// ��������
			if (n == 55) {
				air[n] = air[50];
			}
			else if (n == 56) {
				air[n] = air[52];
			}

			// ���t�K
			if (n == 57) {
				air[n] = air[50];
			}
			else if (n == 58) {
				air[n] = air[51];
			}
			else if (n == 59) {
				air[n] = air[52];
			}

			// ��_
			else if (n == 60) {
				air[n] = air[30];
			}
			else if (n == 65) {
				air[n] = air[43];
			}

			// �K�[�L����
			else if (n == 520) {
				air[n] = air[210];
			}

			// �ǉ����� //
			// �Q�[�W����
			else if (n == 550) {
				air[n] = air[520];
			}
			// �o�[�X�g
			else if (n == 551) {
				air[n] = air[1085];
			}

			// �o�E���h
			else if (n == 1040) {
				air[n] = air[1035];
			}
			// �o�E���h2
			else if (n == 1041) {
				air[n] = air[1035];
			}
			// �ǁi��j
			else if (n == 1046) {
				air[n] = air[1035];
			}
			// �n��󂯐g
			else if (n == 1080) {
				air[n] = air[1085];
			}
		}
	}

	// ���������
	free(input);

	//GetA_GHitbox(air2[na], na);	// GHitBox�ɑ���
	//GetA_Select(air2[na][0], na);

	//@@
	//GetA_GHitbox(air, na);
	GetA_GHitbox(air, pSide);	// GHitBox�ɑ���
							// ����𖈓x�s���悤�ɂ���

	// �������[�V����
	GetA_Select(air[0], na);
}

// state�����P1���󂯎��
void GetP_Air(Player GP1)
{
	P1 = GP1;
}

void GetName()
{
	// ����Z�b�e�B���O //

	int i, fp;
	int anum;	// Name�̏ꏊ
	anum = -1;

	char fname[14] = { "char/name.txt" };

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// input��inputc�ɕ������͂���
	boolean iflg;	// ���O���̓t���O
	

	// �t�@�C�����J�� //
	fp = FileRead_open(fname);//�t�@�C���ǂݍ���
	if (fp == NULL){			// �G���[���N��������
		printfDx("error name\n");// ������\��
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
				input[i] == ';'){ //�J���}�����s�Ȃ�
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				break;	// i��1���₵�ă��[�v�E�o�i�������j
			}

			// �ǂݍ��ݏI��
			if (input[i] == EOF){//�t�@�C���̏I���Ȃ�
				goto EXFILE;//�I��
			}
		}	// ��{���[�v�I��



		// �ŏ��ɐ��������ɂ��āA�����������Ă���ύX����
		iflg = false;
		{
			if ((inputc[0] != '\0') && (inputc[0] != '\n') && (inputc[0] != NULL)
				&& (anum < Character_Max()))iflg = true;
		}
		
		// ���O��������
		if (iflg){
			N.push_back(Name_t());	// �ǉ�
			anum++;	// �ŏ���0�ɂȂ�
			N[anum].name = anum + 1;	// �ŏ�..1
			strcpy_s(N[anum].nameC, inputc);
		}

		if (input[i] == EOF){//�t�@�C���̏I���Ȃ�
			goto EXFILE;//�I��
		}
	}

	// �t�@�C�������
EXFILE:
	FileRead_close(fp);

	// Pict��Name�𑗂�
	GetN_Pict(N);
	GetN_GHitBox(N);
	// Select�ɂ�����
	Select_GetName(N);
	GetN_SameParam(N);
	GetN_Result(N);
}

// P1��Name�ɍ��������̂�nameC�ɃR�s�[
void GetPName()
{
	for (int n = 0; n < Character_Max(); n++){
		if (P1.Name == N[n].name){
			strcpy_s(P1.nameC, N[n].nameC);
		}
	}
	// GHitbox�ɑ���
	if (P1.PSide == 1)Get_P1Set(P1);
	if (P1.PSide == 2)Get_P2Set(P1);

	// ������N�������H
}

/*
Player GetAnimElem(Player GP1)
{
	for (int n = 0; n < Character_Max(); n++){
		// �Z�b�g�X�^�[�g
		if (GP1.Name == N[n].name){
			for (int n1 = 0; n1 < STATE_MAX; n1++){
				GP1.animElem[n1] = air2[n][n1].aTime;
			}
			break;
		}
	}
	
	return GP1;
}
*/