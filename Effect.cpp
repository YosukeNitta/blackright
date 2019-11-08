//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include <math.h>
#include <vector>
using namespace std;

#define EFF_MAX	(30)	//�G�t�F�N�g�̍ő吔
static const int BE_MAX = 10;	//���G�t�F�N�g�̍ő吔
static const int E_MAXTIME = 300;	//�G�t�F�N�g�\������

// �摜�̔z��
static int EffSet[EFF_MAX][EFF_MAX];
static int LoadE;
static int xadd, yadd;

//[ �G�t�F�N�g ]
typedef struct {
	int Image;		// ���݂̉摜
	
	float SizeX,SizeY;	// �`��T�C�Y
	float defX, defY;	// �ŏ��̃T�C�Y(���ۂɕ\�������T�C�Y�ł͂Ȃ�)
	int alpha;		// ����

	int XPos, YPos;	// �摜�̕\���ʒu
	int XAdd, YAdd;	// ���W���ړ�������
	int XSize, YSize;// �摜�T�C�Y
	int Num1, Num2;	// �摜�ԍ� �c�Ɖ�
	int Time;		// ����
	boolean turn;
	int RGB[3];		// �ς���ƐF���ς��
	float rot;		// �p�x

	void Draw(){
		DrawRotaGraph3(XPos + XAdd - (int)S.ScroolX, YPos + YAdd - (int)S.ScroolY + S.quakeY,
			XSize / 2, YSize / 2,
			SizeX, SizeY, rot, Image, true, turn);
	}
}Effect_t;

// �G�t�F�N�g�̕ϐ��p��
static Effect_t E[EFF_MAX];
//static vector<Effect_t> E;
// �ȈՕ\���G�t�F�N�g
static Effect_t E2[EFF_MAX];
static Effect_t BE[BE_MAX];

//�\���t���O
static boolean E2drawFlg;
static Effect_t EffImage(Effect_t e);
static Effect_t EffSetting(int Effnum, int EffXP, int EffYP, int basePosX, int basePosY,
	float SizeX, float SizeY, boolean turn);
static Effect_t EffectDraw(Effect_t e);

// num �͌Ăяo���摜
int EffStart(int Effnum, int EffXP, int EffYP, int basePosX, int basePosY, 
				float SizeX, float SizeY, boolean turn)
{
	// �摜�����[�h
	if (LoadE == 0)
	{
		xadd = 0, yadd = 0;
		SetTransColor(0, 0, 0);
		// �񓯊��ǂݍ��ݐݒ�ɕύX
		SetUseASyncLoadFlag(TRUE);

		EffSet[0][0] = LoadGraph("ob/Effect/03_slash.png");
		LoadDivGraph("ob/Effect/01_hit.png", 5, 5, 1, 320, 320, EffSet[1]);
		LoadDivGraph("ob/Effect/02_hit2.png", 5, 5, 1, 320, 320, EffSet[2]);
		EffSet[3][0] = LoadGraph("ob/Effect/03_slash.png");
		EffSet[4][0] = LoadGraph("ob/Effect/04_slash.png");
		LoadDivGraph("ob/Effect/05.png", 5, 5, 1, 400, 700, EffSet[5]);
		LoadDivGraph("ob/Effect/06_air.png", 6, 3, 2, 400, 400, EffSet[6]);
		EffSet[7][0] = LoadGraph("ob/Effect/07_hydro.png");
		EffSet[8][0] = LoadGraph("ob/Effect/08_shot.png");
		EffSet[9][0] = LoadGraph("ob/Effect/10_right.png");

		EffSet[10][0] = LoadGraph("ob/Effect/15_wave.png");
		LoadDivGraph("ob/Effect/11_guard.png", 10, 5, 2, 500, 500, EffSet[11]);
		LoadDivGraph("ob/Effect/12_guard2.png", 10, 5, 2, 500, 500, EffSet[12]);
		EffSet[15][0] = LoadGraph("ob/Effect/15_wave.png");
		EffSet[16][0] = LoadGraph("ob/Effect/16_wave.png");
		EffSet[18][0] = LoadGraph("ob/Effect/18_wave2.png");
		EffSet[19][0] = LoadGraph("ob/Effect/19_wave.png");
		EffSet[17][0] = LoadGraph("ob/Effect/17_right.png");
		EffSet[20][0] = LoadGraph("ob/Effect/20_fire.png");
		EffSet[21][0] = LoadGraph("ob/Effect/21_fire2.png");

		// �񓯊��ǂݍ��ݐݒ�ɕύX
		SetUseASyncLoadFlag(FALSE);

		LoadE = 1;
	}

	for (int i = 0; i < EFF_MAX; i++){
		// �摜�������Ă��Ȃ�������o�^
		if (E[i].Image == 0){
			E[i] = EffSetting(Effnum, EffXP, EffYP, basePosX, basePosY,
				SizeX, SizeY, turn);

			//e.push_back(Effect_t());
			//e[i] = EffSetting(Effnum, EffXP, EffYP, basePosX, basePosY,
				//SizeX, SizeY, turn);
			break;	// �o�^�I��
		}

	}
	
	return 0;
}

// num �͌Ăяo���摜
int EffStartB(int Effnum, int EffXP, int EffYP, int basePosX, int basePosY,
				float SizeX, float SizeY, boolean turn)
{
	for (int i = 0; i < BE_MAX; i++){
		// �摜�������Ă��Ȃ�������o�^
		if (BE[i].Image == 0){
			BE[i] = EffSetting(Effnum, EffXP, EffYP, basePosX, basePosY,
				SizeX, SizeY, turn);
			break;	// �o�^�I��
		}

	}

	return 0;
}

void EffDraw()
{
	

	// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	for (int i = 0; i < EFF_MAX; i++){

		// �G�t�F�N�g�������Ă�����`��
		if (E[i].Image != 0){
			E[i] = EffectDraw(E[i]);
		}

		// E2�`��
		if (E2drawFlg){
			if (E2[i].Image != 0){
				// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
				if(E2[i].alpha == 0)SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
				else if (E2[i].alpha != 0){
					SetDrawBlendMode(DX_BLENDMODE_ADD, E2[i].alpha);
				}

				E2[i].Draw();

				if (E[i].alpha != 0){
					// �`��u�����h���[�h���m�[�u�����h�ɂ���
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}

				E2[i].Image = 0;
			}
		}
		
	}
	// easyDraw�I��
	E2drawFlg = false;

	// �`��u�����h���[�h���m�[�u�����h�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void EffDrawB()
{
	for (int i = 0; i < BE_MAX; i++){
		// �G�t�F�N�g�������Ă�����`��
		if (BE[i].Image != 0){
			BE[i] = EffectDraw(BE[i]);
		}
	}
}

Effect_t EffImage(Effect_t e)
{
	int i, time;
	time = e.Time;

	// P1 �X�e�[�g�m�F
	switch (e.Num1)
	{
	case 0:	//��
		i = 19;	// 2
		break;
	case 1:	//�q�b�g�P
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		if (time >= 4){
			i = 2;	// 2
		}
		if (time >= 6){
			i = 3;	// 2
		}
		if (time >= 8){
			i = 4;	// 2
		}
		if (time >= 10){
			i = 19;	// 2
		}
		break;

	case 2:	//�q�b�g�Q
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		if (time >= 4){
			i = 2;	// 2
		}
		if (time >= 6){
			i = 3;	// 2
		}
		if (time >= 8){
			i = 4;	// 2
		}
		if (time >= 10){
			i = 19;	// 2
		}
		break;

		// �X���b�V��
	case 3:
		i = 0;
		if (time == 0){
			e.rot = GetRand(180);
			e.SizeX -= (float)(e.defX * 0.7);
			e.SizeY -= (float)(e.defY * 0.7);
		}
		if (time < 12){
			e.SizeX += (float)(e.defX / 10.0);
			e.SizeY += (float)(e.defY / 10.0);
			//e.SizeX -= (float)(e.defX / 10.0);
			//e.SizeY -= (float)(e.defY / 10.0);
		}
		else { i = 19; }

		break;

		// �X���b�V��(�^��)
	case 4:
		i = 0;
		if (time == 0) {
			//e.rot = 15.0;
			e.SizeX -= (float)(e.defX * 0.7);
			e.SizeY -= (float)(e.defY * 0.7);
		}
		
		if (time < 12) {
			e.SizeX += (float)(e.defX / 10.0);
			e.SizeY += (float)(e.defY / 10.0);
			//e.SizeX -= (float)(e.defX / 10.0);
			//e.SizeY -= (float)(e.defY / 10.0);
		}
		else if (time < 20) {
			e.SizeX -= (float)(e.defX / 10.0);
			e.SizeY -= (float)(e.defY / 10.0);
			//e.SizeX -= (float)(e.defX / 10.0);
			//e.SizeY -= (float)(e.defY / 10.0);
		}
		else { i = 19; }

		break;

	case 5:	//�^��
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		if (time >= 4){
			i = 2;	// 2
		}
		if (time >= 6){
			i = 3;	// 2
		}
		if (time >= 8){
			i = 4;	// 2
		}
		e.Image = EffSet[e.Num1][i];
		// �摜�T�C�Y�擾
		GetGraphSize(e.Image, &e.XSize, &e.YSize);
		yadd = -(e.YSize / 2) + 2;
		break;

	case 6:	// 3�Q�[�W��
		if (time >= 0){
			i = 5;	// 5
			xadd = -103;
		}
		if (time >= 5){
			i = 4;	// 3
			xadd = -95;
		}
		if (time >= 8){
			i = 3;	// 3
			xadd = -71;
		}
		if (time >= 11){
			i = 2;	// 3
			xadd = -24;
		}
		if (time >= 14){
			i = 1;	// 3
			xadd = 5;
		}
		if (time >= 17){
			i = 0;	// 3
			xadd = 28;
		}
		
		if (time >= 20){
			i = 0;	// 3
			xadd = 42;
		}
		if (time >= 23){
			i = 1;	// 2
			xadd = 93;
		}
		if (time >= 25){
			i = 2;	// 2
			xadd = 139;
		}
		if (time >= 27){
			i = 3;	// 2
			xadd = 172;
		}
		if (time >= 29){
			i = 4;	// 4
			xadd = 215;
		}
		if (time >= 33){
			i = 5;	// 4
			xadd = 231;
		}
		if (time >= 37){
			i = 19;	// end
			xadd = 0;
		}
		break;

	case 7:	// �R�[�l���F�J
		if (time >= 0){
			i = 0;	// 5
			xadd = -103;
		}
		/*
		if (time >= 10){
			i = 19;	// 2
		}
		*/
		break;
		
	case 8:	// �w���I�X�F�V���b�g
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 10){
			i = 19;	// 5
		}
		break;

	case 9:	// ��2(�ړ�)
		i = 0;
		if (time == 0){
			e.alpha = 250;
			e.rot = GetRand(360);
			xadd = sin(e.rot) * 200;
			yadd = cos(e.rot) * 200;
		}
		else if (time == 1){
			e.alpha = 240;
			xadd = sin(e.rot) * 300;
			yadd = cos(e.rot) * 300;
		}
		else if (time == 2){
			e.alpha = 220;
			xadd = sin(e.rot) * 500;
			yadd = cos(e.rot) * 500;
		}
		else if (time == 3){
			e.alpha = 200;
			xadd = sin(e.rot) * 700;
			yadd = cos(e.rot) * 700;
		}
		else if (time == 4){
			e.alpha = 180;
			xadd = sin(e.rot) * 900;
			yadd = cos(e.rot) * 900;
			e.SizeX -= (float)(e.defX * 0.1);
			e.SizeY -= (float)(e.defY * 0.1);
		}
		else if (time == 5){
			e.alpha = 160;
			xadd = sin(e.rot) * 1100;
			yadd = cos(e.rot) * 1100;
			e.SizeX -= (float)(e.defX * 0.1);
			e.SizeY -= (float)(e.defY * 0.1);
		}
		else if (time == 6){
			e.alpha = 140;
			xadd = sin(e.rot) * 1400;
			yadd = cos(e.rot) * 1400;
			e.SizeX -= (float)(e.defX * 0.1);
			e.SizeY -= (float)(e.defY * 0.1);
		}
		else if (time == 7){
			e.alpha = 130;
			xadd = sin(e.rot) * 1900;
			yadd = cos(e.rot) * 1900;
			e.SizeX -= (float)(e.defX * 0.1);
			e.SizeY -= (float)(e.defY * 0.1);
		}
		else if (time == 8){
			e.alpha = 128;
			xadd = sin(e.rot) * 2500;
			yadd = cos(e.rot) * 2500;
			e.SizeX -= (float)(e.defX * 0.1);
			e.SizeY -= (float)(e.defY * 0.1);
		}
		else { i = 19; }
		break;

	case 10:	// ��
		i = 0;
		if (time == 0){
			e.alpha = 194;
		}
		else if (time == 1){
			e.alpha = 128;
			e.SizeX += (float)(e.defX * 0.1);
			e.SizeY += (float)(e.defY * 0.1);
		}
		else if (time == 2){
			e.alpha = 64;
			e.SizeX += (float)(e.defX * 0.2);
			e.SizeY += (float)(e.defY * 0.2);
		}
		else if (time == 3){
			e.alpha = 32;
			e.SizeX += (float)(e.defX * 0.4);
			e.SizeY += (float)(e.defY * 0.4);
		}
		else { i = 19; }
		break;
	
	case 11:	//�K�[�h�P
		if (time >= 0){
			i = 0;	// 1
		}
		if (time >= 1){
			i = 1;	// 1
		}
		if (time >= 2){
			i = 2;	// 1
		}
		if (time >= 3){
			i = 3;	// 1
		}
		if (time >= 4){
			i = 4;	// 1
		}
		if (time >= 5){
			i = 5;	// 1
		}
		if (time >= 6){
			i = 6;	// 1
		}
		if (time >= 7){
			i = 7;	// 2
		}
		if (time >= 9){
			i = 1;	// 2
		}
		if (time >= 11){
			i = 19;	// end
		}
		break;

	case 12:	//�K�[�h�Q
		if (time >= 0){
			i = 0;	// 1
		}
		if (time >= 1){
			i = 1;	// 1
		}
		if (time >= 2){
			i = 2;	// 1
		}
		if (time >= 3){
			i = 3;	// 1
		}
		if (time >= 4){
			i = 4;	// 1
		}
		if (time >= 5){
			i = 5;	// 1
		}
		if (time >= 6){
			i = 6;	// 1
		}
		if (time >= 7){
			i = 7;	// 2
		}
		if (time >= 9){
			i = 1;	// 2
		}
		if (time >= 11){
			i = 19;	// end
		}
		break;
	// �Ռ��g
	case 15:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX += (float)(e.defX / 10.0);
			e.SizeY += (float)(e.defY / 10.0);
		}

		if (e.alpha > 0) {
			e.alpha -= 25;
		}
		else { i = 19; }
		break;
	// �Ռ��g
	case 16:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX+=(float)(e.defX/10.0);
			e.SizeY+=(float)(e.defY/10.0);
		}
		
		if (e.alpha > 0) {
			e.alpha -= 25;
		}
		else { i = 19; }

		break;

		// ��
	case 17:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX += (float)(e.defX / 12.0);
			e.SizeY += (float)(e.defY / 12.0);
		}

		if (e.alpha > 0) {
			e.alpha -= 25;
		}
		else { i = 19; }

		break;

		// �Ռ��g
	case 18:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX += (float)(e.defX / 10.0);
			e.SizeY += (float)(e.defY / 10.0);
		}

		if (e.alpha > 0) {
			e.alpha -= 25;
		}
		else { i = 19; }

		break;
		
		// �Ռ��g
	case 19:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX += (float)(e.defX / 10.0);
			e.SizeY += (float)(e.defY / 10.0);
		}

		if (e.alpha > 0) {
			e.alpha -= 25;
		}
		else { i = 19; }

		break;

		// ��
	case 20:
		i = 0;
		if (time == 0)e.alpha = 255;
		if (time < 12){
			e.SizeX += (float)(e.defX / 12.0);
			e.SizeY += (float)(e.defY / 12.0);
		}
		

		if (e.alpha > 0) {
			e.alpha -= 18;
		}		
		else { i = 19; }
		break;

	case 21:	// �w���I�X�F�V���b�g2
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 10){
			i = 19;	// 5
		}
		break;
	default:
		i = time / 2;
		break;
	
	} // �摜�Z�b�g�I���
	
	e.Num2 = i;
	return e;
}

// �ȈՕ\��
void EasyEffDraw(int num1, int time, int EffXP, int EffYP, int basePosX, int basePosY,
	float SizeX, float SizeY, boolean turn)
{
	for (int i = 0; i < EFF_MAX; i++){
		// �摜�������Ă��Ȃ�������o�^
		if (E2[i].Image == 0){

			// ���Ԃ�������
			E2[i].Time = time, E2[i].alpha = 0;
			E2[i].XAdd = 0, E2[i].YAdd = 0;
			E2[i].RGB[0] = 255, E2[i].RGB[1] = 255, E2[i].RGB[2] = 255;

			// �`��T�C�Y
			E2[i].SizeX = SizeX;
			E2[i].SizeY = SizeY;
			E2[i].defX = SizeX;
			E2[i].defY = SizeY;

			// �摜�ԍ��擾
			E2[i].Num1 = num1;
			E2[i] = EffImage(E2[i]);	// Num2���̑�

			E2[i].XAdd = (int)(xadd * SizeX), E2[i].YAdd = (int)(yadd * SizeY);
			if (turn){
				E2[i].XAdd = -E2[i].XAdd;
			}
			xadd = 0, yadd = 0;

			// �摜��ǂݍ���
			E2[i].Image = EffSet[E2[i].Num1][E2[i].Num2];
			
			// �摜�T�C�Y�擾
			GetGraphSize(E2[i].Image, &E2[i].XSize, &E2[i].YSize);
			
			E2[i].turn = turn;

			// �摜�̈ʒu
			if (turn)basePosX = -basePosX;	// ���΂ɂ���
			E2[i].XPos = EffXP + basePosX;// -(int)S.ScroolX;
			E2[i].YPos = EffYP + basePosY; //- (int)((E[i].YSize * E[i].SizeY) / 2);

			break;	// �o�^�I��
		}

	}
	E2drawFlg = true;

}

// �摜�\��
Effect_t EffectDraw(Effect_t e)
{
	// ���摜�擾
	e = EffImage(e);	// Num2���̑�

	// �摜��ǂݍ���
	e.Image = EffSet[e.Num1][e.Num2];

	// ���W����
	e.XAdd = (int)(xadd * e.SizeX);
	e.YAdd = (int)(yadd * e.SizeY);
	if (e.turn) {
		e.XAdd = -e.XAdd;
	}
	xadd = 0, yadd = 0;


	// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
	if (e.alpha == 0)SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	else if (e.alpha != 0) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, e.alpha);
	}

	e.Draw();

	if (e.alpha != 0) {
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	// ���Ԍo��
	e.Time += 1;

	// �摜�폜���čė��p�ł���悤�ɂ���
	if (e.Time >= E_MAXTIME)e.Image = 0;

	return e;
}

// �摜�o�^
Effect_t EffSetting(int Effnum, int EffXP, int EffYP, int basePosX, int basePosY,
	float SizeX, float SizeY, boolean turn)
{
	Effect_t e;

	// ���Ԃ�������
	e.Time = 0, e.alpha = 0;
	e.XAdd = 0, e.YAdd = 0;
	e.rot = 0;
	e.RGB[0] = 255, e.RGB[1] = 255, e.RGB[2] = 255;

	// �`��T�C�Y
	e.SizeX = SizeX;
	e.SizeY = SizeY;
	e.defX = SizeX;
	e.defY = SizeY;

	// �摜�ԍ��擾
	e.Num1 = Effnum;
	e = EffImage(e);	// Num2���̑�

	e.XAdd = (int)(xadd * SizeX), e.YAdd = (int)(yadd * SizeY);
	if (turn){
		e.XAdd = -e.XAdd;
	}
	xadd = 0, yadd = 0;

	// �摜��ǂݍ���
	e.Image = EffSet[e.Num1][e.Num2];

	// �摜�T�C�Y�擾
	GetGraphSize(e.Image, &e.XSize, &e.YSize);

	e.turn = turn;

	// �摜�̈ʒu
	if (turn)basePosX = -basePosX;	// ���΂ɂ���
	e.XPos = EffXP + basePosX;// -(int)S.ScroolX;
	e.YPos = EffYP + basePosY; //- (int)((e.YSize * e.SizeY) / 2);

	return e;	// �o�^�I��
}


void GetP_Effect(Player GP1, Player GP2)
{
	P2 = GP2;
}

void GetS_Effect(System_t GS)
{
	S = GS;
}

void GetH_Effect(Helper GH1[], Helper GH2[])
{
	int i;
	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}
}