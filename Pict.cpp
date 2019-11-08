#include "pch.h"
#include <string>	// string�Ɏg��
#include <vector>

using namespace std;

#pragma warning(disable:4996)	// �G���[�\�L�������H


// ����Ŏg��
static Pict pic[PICT_MAX][IMAGE2_MAX];

// ���ꂪ�֐��Ŏg����
static Pict pic2[2][PICT_MAX][IMAGE2_MAX];
//static vector<vector<vector<Pict>>> pic2;	// �n�����Ɏg�p
//static int P_Image2[2][PICT_MAX][IMAGE2_MAX];	// ����ɑS�ẴL�����̎��ۂ̉摜
												// 2�L�������̂ݓ����

static vector<int> r,g,b;	// ����ɑS�ẴL�����̔���

// �S�Ă�pict��ۑ�
static vector<vector<vector<Pict>>> pic3;	// ����ɑS�ẴL�����̉摜�ݒ�

class Image3{
public:
	int image[PICT_MAX][IMAGE2_MAX];
	Image3(){
		for (int i = 0; i < PICT_MAX; i++){
			for (int j = 0; j < IMAGE2_MAX; j++){
				image[i][j] = 0;
			}
		}
	}
};

//static int image3[C_M][PICT_MAX][IMAGE2_MAX];	// ������
//static vector<vector<vector<int>>> vimage3;	// ����ɑS�ẴL�����̔���
// �����ō�����摜��������
// pic�ɂ͓����ĂȂ�
static vector<Image3>im3;

// ���O�iair����ǂݍ��݁js
static vector<Name_t>N;

static void GetPort();	// �|�[�g���C�g�擾

// �摜����
int GraphSet(int s1, int s2, int side)
{
	//return P_Image2[side-1][s1][s2];
	return pic2[side - 1][s1][s2].image;
}

// �摜�E�p���b�g������(�ǂݍ��݌�Ɏg��)
int PalletSet()
{
	/****
	�p���b�g�擾�e�X�g
	****/
	int color[COLOR_MAX], i, j;
	int setPal;

	// [�p���b�g�Z�b�g�J�n]
	//char fname[TX_MAX];
	// ���O�ǂݍ���
	string fname;
	string fn1 = "char/";
	string fn2 = "/pal/c";
	string fn3 = ".bmp";
	
	//
	// ���O�ǂݍ���
	for (int n = 0; n < COLOR_MAX; n++)
	{
		char ch[2];
		sprintf(ch, "%d", n+1);	// �����ɕϊ�
		fname = fn1 + P1.nameC + fn2 + ch[0] + fn3;

		// setPal �ɑI�񂾔ԍ����Z�b�g
		color[n] = LoadSoftImage(fname.c_str());
	}

	setPal = P1.Palno;

	// RGB�Ƀp���b�g�̐F���L��
	for (i = 0; i < 256; i++)
	{
		// �p���b�g�̐F���擾���� 1�`16, 17�`
		GetPaletteSoftImage(color[setPal], i, &r[i], &g[i], &b[i], 0);
	}
	
	//====================
	// �����ŉ摜�ǂݍ���
	//====================
	//load2_pict();
	
	// ���ߐF��ύX
	SetTransColor(r[0], g[0], b[0]);
	
	// �|�g���擾
	GetPort();

	// ���ߐF��߂�
	SetTransColor(r[0], g[0], b[0]);

	// pic2�ɑ��
	for (int n = 0; n < PICT_MAX; n++){
		for (int n2 = 0; n2 < IMAGE2_MAX; n2++){
			// image2��������
			//P_Image2[P1.PSide - 1][n][n2] = 0;
			
			// pic2��pic3�ݒ���Z�b�g
			pic2[P1.PSide - 1][n][n2] = pic3[P1.Name - 1][n][n2];

			// �g���摜�̂ݓǂݍ���(����Ȃ��ƃN�b�\�d��)
			if (im3[P1.Name - 1].image[n][n2] != 0){
				// image3�Acolor�摜�̐F���擾
				for (int c = 0; c < 256; c++){
					SetPaletteSoftImage(im3[P1.Name - 1].image[n][n2], c, r[c], g[c], b[c], 0);
				}
				// image2�ɐF��ς���image3������
				//P_Image2[P1.PSide - 1][n][n2] = CreateGraphFromSoftImage(im3[P1.Name - 1].image[n][n2]);
				// image2�ɐF��ς���image3������
				pic2[P1.PSide - 1][n][n2].image = CreateGraphFromSoftImage(im3[P1.Name - 1].image[n][n2]);
			}
		}
	}
	
	// ���ߐF�����ɖ߂�
	SetTransColor(0, 0, 0);

	// �J���[�摜�S�č폜
	for (j = 0; j < COLOR_MAX; j++)
	{
		DeleteSoftImage(color[j]);
	}

	return 0;
}

// csv�œǂݍ���
void load_pict(int np, int side)
{

	FILE *fp;

	char buf[100];
	int c;
	int col = 1;
	int row = 0;
	int sizeX = 0, sizeY = 0;
	int x = 0, y = 0;	// ���O�̒ʂ�
	int num1 = 0, num2 = 0;

	// �C���[�W�ǉ�

	//////

	/// ���������Ă邩�炢��Ȃ��H
	/*
	for (int i = 0; i < IMAGE_MAX; i++){
		for (int j = 0; j < IMAGE2_MAX; j++){
			pic[i][j].image = 0;
			pic[i][j].posX = 0;
			pic[i][j].posY = 0;
			pic[i][j].x = 0;
			pic[i][j].y = 0;
		}
	}
	*/
	
	// ���O�ǂݍ���
	string fname;
	string fn1 = "char/";
	string fn2 = "/sff/sffdata.csv";
	fname = fn1 + N[np].nameC + fn2;

	std::string fsff = "/sff/";
	// ���O����
	std::string str;
	//

	memset(buf, 0, sizeof(buf));

	fp = fopen(fname.c_str(), "r");
	if (fp == NULL){			// �G���[���N��������
		printfDx("error sffdata\n");// ������\��
		return;
	}
	// �����F����
	SetTransColor(r[0], g[0], b[0]);

	// �񓯊��ǂݍ��ݐݒ�ɕύX
	SetUseASyncLoadFlag(TRUE);

	//�w�b�_�ǂݔ�΂�
	while (fgetc(fp) != '\n');

	while (1){

		while (1){

			c = fgetc(fp);

			//�����Ȃ烋�[�v�𔲂���B
			if (c == EOF)
				goto out;

			//�J���}�����s�łȂ���΁A�����Ƃ��ĂȂ���
			if (c != '\n' && c != ',')
					strcat(buf, (const char*)&c);
			//�J���}�����s�Ȃ烋�[�v������B
			else
				break;
		}

		//�����ɗ����Ƃ������Ƃ́A1�Z�����̕����񂪏o���オ�����Ƃ�������
		switch (col){
			// �����̂ݎ擾
		case 1:
			str = fn1 + N[np].nameC + fsff + buf;
			break;

			//2��ڂ͒e��ށB�ȍ~�ȗ��B
		case 2: num1 = atoi(buf); break;
		case 3: num2 = atoi(buf); break;
		case 4: pic[num1][num2].posX = atoi(buf); break;
		case 5: pic[num1][num2].posY = atoi(buf);
	
			// �����̉摜����
			//P_Image3[np][num1][num2] = LoadSoftImage(str.c_str());
			// �������Ȑ��l�������Ă���
			if (num1 >= PICT_MAX || num1 < 0)break;
			else if (num2 >= IMAGE2_MAX || num2 < 0)break;
			//vimage3[np][num1][num2] = LoadSoftImage(str.c_str());

			// ������ŉ摜��ǂݍ���(�����F�ς����Ȃ���g��)
			im3[np].image[num1][num2] = LoadSoftImage(str.c_str());
			
			GetSoftImageSize(im3[np].image[num1][num2], &sizeX, &sizeY);	// �摜�T�C�Y�擾
			
			// �T�C�Y�Esffno����
			pic[num1][num2].x = sizeX;
			pic[num1][num2].y = sizeY;
			break;
		case 6: break;
		}

		//�o�b�t�@��������
		memset(buf, 0, sizeof(buf));
		//�񐔂𑫂�
		++col;

		//�����ǂݍ��񂾕��������s 
		//��������񐔂����������čs���𑝂₷
		if (c == '\n' || col >= 6){
			col = 1;
			row++;
		}
	}
out:
	// �񓯊��ǂݍ��ݐݒ�ɕύX
	SetUseASyncLoadFlag(FALSE);

	// �����F�����ɖ߂�
	SetTransColor(0, 0, 0);

	// pic2�ɑ��
	for (int n = 0; n < PICT_MAX; n++){
		for (int n2 = 0; n2 < IMAGE2_MAX; n2++){
			// �ύX����
			pic2[side - 1][n][n2] = pic[n][n2];
			pic3[np][n][n2] = pic[n][n2];
			//vimage3[np][n][n2] = P_Image[n][n2];
		}
	}

	// �Z���N�g�p
	int r, g, b;
	GetPaletteSoftImage(im3[np].image[0][0], 0, &r, &g, &b, 0);

	// �����F
	SetTransColor(r, g, b);
	for (int n2 = 0; n2 < IMAGE2_MAX; n2++){
		// �ύX����
		//pic[0][n2].image = CreateGraphFromSoftImage(im3[np].image[0][n2]);
		pic[0][n2].image = im3[np].image[0][n2];
	}
	SetTransColor(0, 0, 0);

	GetPI_Select(pic[0], np);	// �Z���N�g�ɑ���
	
	// �t�@�C�������ق����悭�Ȃ��H
	fclose(fp);
}

void GetPort()
{
	/****
	�p���b�g�擾�e�X�g
	****/
	int PR, PR2;

	// [�p���b�g�Z�b�g�J�n]
	string fname;
	string fn1 = "char/";
	string fn2 = "/pal/port.bmp";
	fname = fn1 + P1.nameC + fn2;
	{
		// setPal �ɑI�񂾔ԍ����Z�b�g
		PR = LoadSoftImage(fname.c_str());
	}

	//setPal = P1.Palno;


	//====================
	// �����ŉ摜�ǂݍ���
	//====================
	
		// �F���擾
		for (int c = 0; c < 256; c++){
			SetPaletteSoftImage(PR, c, r[c], g[c], b[c], 0);
		}
		// PR�ɏ��������f�[�^��ǂݍ���
		PR2 = CreateGraphFromSoftImage(PR);

		// �F�ς����摜��ǂݍ���
		MainDraw_GetPort(PR2, P1.PSide);
		
		
		// �l�[���v���[�g
		{
			fn2 = "/pal/name.bmp";
			fname = fn1 + P1.nameC + fn2;
			// setPal �ɑI�񂾔ԍ����Z�b�g
			PR = LoadSoftImage(fname.c_str());
		}

		// �F���擾
		int r1 = 0, g1 = 0, b1 = 0;
		GetPaletteSoftImage(PR, 0, &r1, &g1, &b1, 0);
		
		SetTransColor(r1, g1, b1);
		PR2 = CreateGraphFromSoftImage(PR);
		//SetTransColor(0, 0, 0);

		// ���O�擾
		MainDraw_GetName(PR2, P1.PSide);
}

void GetPict(int side)
{
	// ��炢�ɑ���
	GetPI_GHitbox(pic2[side - 1], side);
}

void GetP1_Pict(Player GP1)
{
	P1 = GP1;
}

// Name���擾
//void GetN_Pict(Name_t GN[])
void GetN_Pict(std::vector<Name_t>GN)
{
	for (int i = 0; i < Character_Max(); i++){
		N.push_back(GN[i]);
		//N[i] = GN[i];
	}
}

void Load1_Pict()
{
	// ���̂����ɑ������

	//P_IMage3
	//pic3����
	for (int i = 0; i < Character_Max(); i++) {
		//im3
		im3.push_back(Image3());

		pic3.push_back(vector<vector<Pict>>());
		pic3[i].resize(PICT_MAX);	// �����ꍇ�͂��炩���ߗe�ʂ��m��
		for (int n1 = 0; n1 < PICT_MAX; n1++) {
			pic3[i][n1].resize(IMAGE2_MAX);
		}
	}

	// rgb
	r.resize(256);
	g.resize(256);
	b.resize(256);
	/*
	for (int i = 0; i < 256; i++){
		r.push_back(0);
		g.push_back(0);
		b.push_back(0);
	}
	*/
}