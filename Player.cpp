//==================================================================================
//
//			�L�����N�^�[�̊Ǘ���
//
//========================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"

//�f�t�@�C����----------------------------------------------

// #include "Define.h" �ɂāA�֐����Z�b�g
static int N1, N2;
static int pict[2][PICT_MAX][IMAGE2_MAX];

//�ް���`��------------------------------------------------



//�����֐��錾��--------------------------------------------

// #include "Setting.h" �ɂăZ�b�g


//�v���O������----------------------------------------------


//�ΐ�̌v�Z���C���֐�

//-------------------------------------------------------------------------------
//	��`�@int CharMove( void )
//
//	�����@�o�g�����[�h��1P, 2P�̒l��ݒ肷��A���̃��[�h�̗v
//
//	�����@int (��)  �����̒l�Ŏ��̉�ʂ����܂�
//
//	�߂�l�@int ���j���[�����߂�ϐ�
//-------------------------------------------------------------------------------



// �摜�ԍ�
static int CharGraph;


// �L�����ŗL�̐��l���Z�b�g����
int Player_State()
{
	/*********
	* �\�͐ݒ� 
	*********/
	// �ǂݍ���
	if (P1.Name > 0)load_status(P1);

	// �X�e�[�g�ɐ��l�𑗂�
	if (P1.PSide == 1)Get_P1Set(P1);
	if (P1.PSide == 2)Get_P2Set(P1);
		
	return 0;
}


// �w���p�[�ɉ摜�������Ă���
int Helper_Graph()
{
	{
		for (int i = 0; i < HM; i++){
			if (H1[i].var){
				// N1, N2 ���擾
				// H1[i].HImage = P1.GraphImage[H1[i].aNum1][H1[i].aNum2];
				H1[i].Image = pict[P1.PSide - 1][H1[i].aNum1][H1[i].aNum2];

				// �摜�T�C�Y�擾 P1,2
				GetGraphSize(H1[i].Image, &H1[i].HGraphW, &H1[i].HGraphH);

				// ���Ԍv��
				//H1[i].time += 1;
				//H1[i].HAnimTime += 1;			
			}
		}
	}

	// P1��P2�̂ǂ����ɑ��邩���f
	if (P1.PSide == 1)Get_HSet(H1, H2);
	if (P1.PSide == 2)Get_HSet(H2, H1);

	// �I��
	return 0;

}

void Player_GraphLoad()
{
	// ����p
	int i, j;

	// Player�̐��l��n���A�p���b�g�Ɖ摜��PIC��ǂݍ���
	GetP1_Pict(P1);	// �v���C���[�f�[�^��n��
	PalletSet();
	GetPict(P1.PSide);	// ok
	for (i = 0; i < PICT_MAX; i++)
	{
		for (j = 0; j < IMAGE2_MAX; j++)
		{
			// �L�����̉摜��o�^
			pict[P1.PSide - 1][i][j] = GraphSet(i, j, P1.PSide);
		}
	}
	
	
	//�����܂�

	// �X�e�[�g�ɐ��l�𑗂�
	if (P1.PSide == 1)Get_P1Set(P1);
	if (P1.PSide == 2)Get_P2Set(P1);
	// ���[�h�I��
}

int Player_ImageGet(Player GP1)
{
	int img = pict[GP1.PSide - 1][GP1.aNum1][GP1.aNum2];

	return img;
}

void GetP1_Player(Player GP1)
{
	P1 = GP1;
}

void GetS_Player(System_t GS)
{
	S = GS;
}


void Get_N12(int N_1, int N_2)
{
	N1 = N_1;
	N2 = N_2;
}

void GetH_Player(Helper GH1[], Helper GH2[])
{
	int i;
	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}
}

