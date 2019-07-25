//==================================================================================
//
//			���l�v�Z�̊Ǘ���
//
//========================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include "vector"

using namespace std;

//�f�t�@�C����----------------------------------------------


//�ް���`��------------------------------------------------
static Air air[2][STATE_MAX];

//static Air air2[CHARACTERS_MAX][STATE_MAX];	// �ۑ��p
static vector<vector<Air>>air2;	// ����ɑS�ẴL�����̔���

static Pict pic[2][PICT_MAX][IMAGE2_MAX];

typedef struct{
	int attFlg;	// ���߂Ŕ��������
	int attSide;	// �v���C���[�̕���
	int i, w, h, x, y;
}ExSet;

ExSet ex;

static vector<Name_t>N;

//�����֐��錾��--------------------------------------------


//�v���O������----------------------------------------------




void GHitbox(){


	// �ŏ��ɔ��菉����
	GetPH_Hantei(P1);
	KuraiReset();
	AttReset();
	P1 = ReturnPSet();

	GetPH_Hantei(P1);	// ���l�𔻒�ɓn��


	//=================
	// box�Z�b�e�B���O
	//=================
	int num = 0;
	int bTime = 0;
	int side = P1.PSide - 1;

	// ANum�����݂���Ȃ�X�^�[�g
	if (air[side][P1.stateno].ANum > -1){
		for (num = 0; num < STATE2_MAX-1; num++){
			bTime += air[side][P1.stateno].B[num].time;
			// ���Ԃ������Ă��違��������
			if ((bTime <= P1.time) &&
				(air[side][P1.stateno].B[num + 1].time > -2)){
			}
			//
			else{ break; }
		}
		
		for (int n2 = 0; n2 < HIT_MAX; n2++){
			//if (air[side][P1.stateno].B[num].h[0])TestText(1);
			//else { TestText(0); } ������
			// ���肪����Ȃ�`��
			if (air[side][P1.stateno].B[num].h[n2]){
				KuraiSet(n2,
					(int)((air[side][P1.stateno].B[num].hw[n2] - air[side][P1.stateno].B[num].hx[n2]) * P1.GSize),
					(int)((air[side][P1.stateno].B[num].hh[n2] - air[side][P1.stateno].B[num].hy[n2]) * P1.GSize),
					(int)(air[side][P1.stateno].B[num].hx[n2] * P1.GSize), (int)(air[side][P1.stateno].B[num].hy[n2] * P1.GSize));
			}
			if (air[side][P1.stateno].B[num].a[n2]){
				AttSet(n2,
					(int)((air[side][P1.stateno].B[num].aw[n2] - air[side][P1.stateno].B[num].ax[n2]) * P1.GSize),
					(int)((air[side][P1.stateno].B[num].ah[n2] - air[side][P1.stateno].B[num].ay[n2]) * P1.GSize),
					(int)(air[side][P1.stateno].B[num].ax[n2] * P1.GSize), (int)(air[side][P1.stateno].B[num].ay[n2] * P1.GSize));
			}
		} // n2 �I��

	}

	//========================
	// ��������ǉ�����������
	//========================
	if ((ex.attFlg > 0) && (ex.attSide == P1.PSide)){
		AttSet(ex.i, ex.w, ex.h, ex.x, ex.y);
		ex.attFlg --;
	}
	
	// �l��Ԃ�
	P1 = ReturnPSet();


	// P1�ɐݒ�
	P1.State2 = num;
	P1.aNum1 = air[side][P1.stateno].B[num].num1;
	P1.aNum2 = air[side][P1.stateno].B[num].num2;
	
	P1.aHV = air[side][P1.stateno].B[num].HV;
	
	if (P1.aHV == 0){
		P1.aPosX = air[side][P1.stateno].B[num].posX;
	}
	else if (P1.aHV != 0){
		P1.aPosX = -air[side][P1.stateno].B[num].posX;
	}
	P1.aPosY = air[side][P1.stateno].B[num].posY;

	P1.sPosX = pic[side][P1.aNum1][P1.aNum2].posX;
	P1.sPosY = pic[side][P1.aNum1][P1.aNum2].posY;
	
	

	// �l��n��
	if (P1.PSide == 1)Get_P1Set(P1);
	if (P1.PSide == 2)Get_P2Set(P1);

}


void HelperKurai(){

	Helper H;

	
	// �ŏ��ɔ��菉����
	GetH1_Hantei(H1);
	H_KuraiReset();
	H_AttReset();
	for (int i = 0; i < HELPER_MAX; i++){
		H1[i] = ReturnHSet(i);
	}

	GetH1_Hantei(H1);	// ���l�𔻒�ɓn��
	

	//=================
	// box�Z�b�e�B���O
	//=================
	int num, bTime;
	int side;
	side = P1.PSide - 1;
	
	// �w���p�[�S�Ăɍs��
	for (int hn = 0; hn < HM; hn++){
	// ����Ȃ�
	if (H1[hn].var){
	// �ϊ�����
	H = H1[hn];
	bTime = 0;
	// ANum�����݂���Ȃ�X�^�[�g
	if (air[side][H.stateno].ANum > -1){
		for (num = 0; num < STATE2_MAX - 1; num++){
			bTime += air[side][H.stateno].B[num].time;
			// ���Ԃ������Ă��違��������
			if ((bTime <= H.HAnimTime) &&
				(air[side][H.stateno].B[num + 1].time > -2)){
			}
			//
			else{ break; }
		}


		for (int n2 = 0; n2 < HIT_MAX; n2++){
			// ���肪����Ȃ�`��
			if (air[side][H.stateno].B[num].h[n2]){
				H_KuraiSet(hn, n2,
					(int)((air[side][H.stateno].B[num].hw[n2] - air[side][H.stateno].B[num].hx[n2]) * H.GSize),
					(int)((air[side][H.stateno].B[num].hh[n2] - air[side][H.stateno].B[num].hy[n2]) * H.GSize),
					(int)(air[side][H.stateno].B[num].hx[n2] * H.GSize), (int)(air[side][H.stateno].B[num].hy[n2] * H.GSize));
			}
			// �Ȃ��ăf�t�H���肪����Ȃ�
			else if (air[side][H.stateno].def && air[side][H.stateno].defh[n2]){
				H_KuraiSet(hn, n2,
					(int)((air[side][H.stateno].defhw[n2] - air[side][H.stateno].defhx[n2]) * H.GSize),
					(int)((air[side][H.stateno].defhh[n2] - air[side][H.stateno].defhy[n2]) * H.GSize),
					(int)(air[side][H.stateno].defhx[n2] * H.GSize), (int)(air[side][H.stateno].defhy[n2] * H.GSize));
			}
			// �U������
			if (air[side][H.stateno].B[num].a[n2]){
				H_AttSet(hn, n2,
					(int)((air[side][H.stateno].B[num].aw[n2] - air[side][H.stateno].B[num].ax[n2]) * H.GSize),
					(int)((air[side][H.stateno].B[num].ah[n2] - air[side][H.stateno].B[num].ay[n2]) * H.GSize),
					(int)(air[side][H.stateno].B[num].ax[n2] * H.GSize), (int)(air[side][H.stateno].B[num].ay[n2] * H.GSize));
			}
		} // n2 �I��
		H = ReturnHSet(hn);

		// H�ɐݒ�
		H.state2 = num;
		H.aNum1 = air[side][H.stateno].B[num].num1;
		H.aNum2 = air[side][H.stateno].B[num].num2;
		H.aPosX = air[side][H.stateno].B[num].posX;
		H.aPosY = air[side][H.stateno].B[num].posY;
		H.aHV = air[side][H.stateno].B[num].HV;
		H.sPosX = pic[side][H.aNum1][H.aNum2].posX;
		H.sPosY = pic[side][H.aNum1][H.aNum2].posY;

		// ���ɖ߂�
		H1[hn] = H;

	}	// if�I��


	}	// var if �I��
	
	}	// for�I��


	// �l��Ԃ�
	for (int i = 0; i < HELPER_MAX; i++){
		//H1[i] = ReturnHSet(i);
	}

	

	

	// �l��n��
	if (P1.PSide == 1)Get_HSet(H1, H2);
	else if (P1.PSide == 2)Get_HSet(H2, H1);
}


void GetP_GHitbox(Player GP1)
{
	P1 = GP1;
}

void GetS_GHitbox(System_t GS)
{
	S = GS;
}

void GetH_GHitbox(Helper GH1[], Helper GH2[])
{
	int i;
	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}


}

void GetA_GHitbox(Air GA[STATE_MAX], int side)
{
	// AIR2�쐬
	air2.push_back(std::vector<Air>());
	air2[side].reserve(STATE_MAX);	// �e�ʂ��m��
	for (int j = 0; j < STATE_MAX; j++){
		air2[side].push_back(Air());
	}
	
	// AIR�ϐ����� //
	// �����������
	for (int n = 0; n < STATE_MAX; n++){
		air2[side][n].setup();
		air2[side][n] = GA[n];
	}
}

void GetPI_GHitbox(Pict GPI[PICT_MAX][IMAGE2_MAX], int side)
{
	for (int a = 0; a < PICT_MAX; a++){
		for (int b = 0; b < IMAGE2_MAX; b++){
			pic[side-1][a][b] = GPI[a][b];
		}
	}
}

void GetN_GHitBox(vector<Name_t>GN)
{
	for (int i = 0; i < Character_Max(); i++) {
		N.push_back(GN[i]);
		//N[i] = GN[i];
	}
}

Player GetAnimElem(Player GP1)
{
	for (int n = 0; n < Character_Max(); n++) {
		// �Z�b�g�X�^�[�g
		if (GP1.Name == N[n].name) {
			for (int n1 = 0; n1 < STATE_MAX; n1++) {
				GP1.animElem[n1] = air2[n][n1].aTime;
			}
			break;
		}
	}
	

	return GP1;
}

void load_GHitBox()
{
	// AIR�ϐ����� //
	// ���������ڐA
	for (int n = 0; n < STATE_MAX; n++){
		air[P1.PSide - 1][n].setup();
		air[P1.PSide - 1][n] = air2[P1.Name - 1][n];
	}
}

void ExAtt(int PSide, int i, int W, int H, int X, int Y)
{
	ex.i = i;
	ex.w = W;
	ex.h = H;
	ex.x = X;
	ex.y = Y;
	ex.attSide = PSide;
	ex.attFlg = 2;
}