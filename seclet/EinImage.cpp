//==================================================================================
//
//			���l�v�Z�̊Ǘ���
//
//==================================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"

//�f�t�@�C����----------------------------------------------

//�ް���`��------------------------------------------------



//�����֐��錾��--------------------------------------------

// #include "Setting.h" �ɂăZ�b�g

// �c�ԍ������߂Ă���A����ɑ���
static int m_row, m_column;
// ����摜�̏c�ԍ�
static int i;
// ����摜�̉��ԍ��i�摜�t�@�C���̔ԍ��j�@���g�p
static int r;

//�v���O������----------------------------------------------


//�f�[�^����n�֐�

//-------------------------------------------------------------------------------
//	��`�@int EinImage
//
//	�����@�����ŉ摜�����������ق������������A�X�e�[�g�E���ԁ@
//  �ɍ��킹�ČĂԉ摜�����߂�i���ԍ��E�c�ԍ��j�@�̂ق�����������
//
//	�����@
//
//	�߂�l�@int �摜�̏c�ԍ�
//-------------------------------------------------------------------------------



int EinImage(int Stateno, int time)
{
	// ��
	// ��{�̓X�e�[�g�Ɠ���
	r = Stateno;	
	// �c�A����� i ������
	m_column = 0;	


	// P1 �X�e�[�g�m�F
	switch (Stateno)
	{
	case 0:	//����	54 �t���[��
		// ��d�X�C�b�` ���Ԋm�F
		if (time >= 0){	
			i = 0;	//15
		}
		if (time >= 15){
			i = 1;	// 7
		}
		if (time >= 22){
			i = 2;	// 6
		}
		if (time >= 28){
			i = 3;	// 9
		}
		if (time >= 37){
			i = 4;	// 3
		}
		if (time >= 40){
			i = 5;	// 6
		}
		if (time >= 46){
			i = 6;	// 8
		}
	
		break;

	case 5:	// �����U�����	8 �t��
		if (time >= 0){
			i = 0;	// 4
		}
		if (time >= 4){
			i = 1;	// 4
		}
		break;

	case 6:	// ���Ⴊ�ݐU�����	8 �t��
		if(time >= 0){
			r = 5;
			i = 2;	// 4
		}
		if (time >= 4){
			i = 3;	// 4
		}
		break;

	case 10:	//���������Ⴊ��	11F
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 5){
			i = 1;	// 6
		}
		break;


	case 11:	// ���Ⴊ��	0�`F
		if (time >= 0){
			i = 0;	//1�`
		}
		break;


	case 12:	//���Ⴊ�݁�����	8
		if (time >= 0){
			i = 1;	// 3
		}
		if (time >= 3){
			i = 0;	// 5
		}
		break;


	case 20:	// �O����	47
		if (time >= 0){
			i = 0;	// 8
		}
		if (time >= 8){
			i = 1;	// 5
		}
		if (time >= 13){
			i = 2;	// 4
		}
		if (time >= 17){
			i = 3;	// 5
		}
		if (time >= 22){
			i = 4;	// 9
		}
		if (time >= 31){
			i = 5;	// 5
		}
		if (time >= 36){
			i = 6;	// 4
		}
		if (time >= 40){
			i = 7;	// 7
		}
		break;

	case 21:	// ������		48
		if (time >= 0){
			i = 0;	// 11
		}
		if (time >= 11){
			i = 1;	// 9
		}
		if (time >= 20){
			i = 2;	// 7
		}
		if (time >= 27){
			i = 3;	// 5
		}
		if (time >= 32){
			i = 4;	// 7
		}
		if (time >= 39){
			i = 5;	// 9
		}

		break;


	case 30:	//�O�_�b�V��	42
		if (time >= 0){
			i = 0;	// 3
		}
		if (time >= 3){
			i = 1;	// 5
		}
		if (time >= 8){
			i = 2;	// 4
		}
		if (time >= 12){
			i = 3;	// 5
		}
		if (time >= 17){
			i = 4;	// 7
		}
		if (time >= 24){
			i = 3;	// 5
		}
		if (time >= 29){
			i = 2;	// 4
		}
		if (time >= 33){
			i = 1;	// 4
		}
		if (time >= 37){
			i = 0;	// 5
		}
		break;


	case 35:	// �o�b�N�X�e�b�v	9�`
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 5){
			i = 1;	// 4�`
		}
		break;

	case 40:	// �W�����v�J�n		3
		if (time >= 0){
			i = 0;	// 3
		}
		break;

	case 41:	// �����W�����v		27�`(24�`���[�v
		if (time >= 0){
			i = 0;	// 9
		}
		if (time >= 9){
			i = 1;	// 3
		}
		if (time >= 12){
			i = 2;	// 5
		}
		if (time >= 17){
			i = 3;	// 4
		}
		if (time >= 21){
			i = 4;	// 3
		}
		if (time >= 24){
			i = 5;	// 3
		}

		if (time >= 27){
			i = 4;	// 3
		}
		if (time >= 30){
			i = 5;	// 3
		}
		break;

	case 42:	// �O�W�����v
		if (time >= 0){
			i = 0;	// 9
		}
		if (time >= 9){
			i = 1;	// 3
		}
		if (time >= 12){
			i = 2;	// 5
		}
		if (time >= 17){
			i = 3;	// 4
		}
		if (time >= 21){
			i = 4;	// 3
		}
		if (time >= 24){
			i = 5;	// 3
		}

		if (time >= 27){
			i = 4;	// 3
		}
		if (time >= 30){
			i = 5;	// 3
		}
		break;

	case 43:	// ���W�����v		23�`(18�`���[�v)
		if (time >= 0){
			i = 0;	// 18
		}
		if (time >= 18){
			i = 1;	// 4
		}
		if (time >= 22){
			i = 2;	// 4
		}

		if (time >= 26){
			i = 1;	// 4
		}
		if (time >= 30){
			i = 2;	// 4
		}

		break;

	case 45:	// �󒆃W�����v		2
		if (time >= 0){
			i = 0;	// 9
		}
		if (time >= 9){
			i = 1;	// 3
		}
		break;

	case 46:	// �W�����v���		6�`
		if (time >= 0){
			r = 41;
			i = 4;	// 9
		}
		if (time >= 3){
			r = 41;
			i = 5;	// 3
		}
		break;

	case 47:	// ���n		2
		if (time >= 0){
			i = 0;	// 2
		}
		break;

	case 50:	// �K�[�h(��)	4
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		break;
	case 51:	// �K�[�h(��)	1�`
		if (time >= 0){
			i = 1;	// 2
		}
		break;
	case 52:	// ����(��)	4
		if (time >= 0){
			i = 1;	// 2
		}
		if (time >= 2){
			i = 0;	// 2
		}
		break;
	case 53:	// �K�[�h(��)	4
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		break;
	case 54:	// �K�[�h(��)	1
		if (time >= 0){
			i = 1;	// 2
		}
		break;
	case 55:	// ����(��)	4
		if (time >= 0){
			i = 1;	// 2
		}
		if (time >= 2){
			i = 0;	// 2
		}
		break;
	case 56:	// �K�[�h(��)	4
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		break;
	case 57:	// �K�[�h(��)	1
		if (time >= 0){
			i = 1;	// 2
		}
		break;
	case 58:	// ����(��)	4
		if (time >= 0){
			i = 1;	// 2
		}
		if (time >= 2){
			i = 0;	// 2
		}
		break;
	case 95:	//����	87 �t���[��
		// ��d�X�C�b�` ���Ԋm�F
		if (time >= 0){
			i = 0;	// 4
		}
		if (time >= 4){
			i = 1;	//14
		}
		if (time >= 18){
			i = 2;	// 7
		}
		if (time >= 25){
			i = 3;	// 5
		}
		if (time >= 30){
			i = 5;	// 5
		}
		if (time >= 35){
			i = 6;	// 46
		}
		if (time >= 81){
			i = 5;	// 2
		}
		if (time >= 83){
			i = 4;	// 2
		}
		if (time >= 85){
			i = 7;	// 2
		}
		if (time >= 87){
			i = 8;	// 2
		}
	
		break;

	case 200:	// ����A	14
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		if (time >= 4){
			i = 2;	// 3
		}
		if (time >= 7){
			i = 3;	// 3
		}
		if (time >= 10){
			i = 1;	// 1
		}
		if (time >= 11){
			i = 0;	// 2
		}
		break;

	case 210:	// ����B	25
		if (time >= 0){
			i = 0;	// 1
		}
		if (time >= 1){
			i = 1;	// 2
		}
		if (time >= 3){
			i = 2;	// 2
		}
		if (time >= 5){
			i = 3;	// 2
		}
		if (time >= 7){
			i = 4;	// 2
		}
		if (time >= 9){
			i = 5;	// 5
		}
		if (time >= 14){
			i = 6;	// 3
		}
		if (time >= 17){
			i = 7;	// 3
		}
		if (time >= 20){
			i = 8;	// 2
		}
		if (time >= 22){
			i = 9;	// 2
		}
		if (time >= 24){
			i = 0;	// 1
		}
		break;
	case 220:	// ����C	51
		if (time >= 0){
			i = 0;	// 1
		}
		if (time >= 1){
			i = 1;	// 7
		}
		if (time >= 8){
			i = 2;	// 6
		}
		if (time >= 14){
			i = 3;	// 4
		}
		if (time >= 18){
			i = 4;	// 4
		}
		if (time >= 22){
			i = 5;	// 2
		}
		if (time >= 24){
			i = 6;	// 2
		}
		if (time >= 26){
			i = 7;	// 2
		}
		if (time >= 28){
			i = 8;	// 2
		}
		if (time >= 30){
			i = 9;	// 9
		}
		if (time >= 39){
			i = 6;	// 7
		}
		if (time >= 46){
			i = 10;	// 2
		}
		if (time >= 48){
			i = 0;	// 2
		}
		break;
	case 300:	// ���Ⴊ��A	14
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 3
		}
		if (time >= 5){
			i = 2;	// 3
		}
		if (time >= 8){
			i = 3;	// 3
		}
		if (time >= 11){
			i = 1;	// 2
		}
		if (time >= 13){
			i = 0;	// 1
		}
		break;

	case 310:	// ���Ⴊ��B	28
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 4
		}
		if (time >= 6){
			i = 2;	// 3
		}
		if (time >= 9){
			i = 3;	// 3
		}
		if (time >= 12){
			i = 4;	// 4
		}
		if (time >= 16){
			i = 5;	// 6
		}
		if (time >= 22){
			i = 6;	// 4
		}
		if (time >= 26){
			i = 0;	// 2
		}
		break;
	case 320:	// ���Ⴊ��C	33
		if (time >= 0){
			i = 0;	// 4
		}
		if (time >= 4){
			i = 1;	// 3
		}
		if (time >= 7){
			i = 2;	// 2
		}
		if (time >= 9){
			i = 3;	// 4
		}
		if (time >= 13){
			i = 4;	// 5
		}
		if (time >= 18){
			i = 5;	// 3
		}
		if (time >= 21){
			i = 6;	// 5
		}
		if (time >= 26){
			i = 1;	// 4
		}
		if (time >= 30){
			i = 0;	// 3
		}
		break;
	case 400:	// �W�����vA	19
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 3
		}
		if (time >= 5){
			i = 2;	// 10
		}
		if (time >= 14){
			i = 3;	// 2
		}
		if (time >= 16){
			i = 1;	// 2
		}
		if (time >= 18){
			i = 0;	// 1
		}
		break;

	case 410:	// �W�����vB	27
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 3
		}
		if (time >= 5){
			i = 2;	// 2
		}
		if (time >= 7){
			i = 3;	// 2
		}
		if (time >= 9){
			i = 4;	// 7
		}
		if (time >= 16){
			i = 5;	// 4
		}
		if (time >= 20){
			i = 6;	// 6
		}
		if (time >= 26){
			i = 0;	// 1
		}
		break;

	case 420:	// �W�����vC	22
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 4
		}
		if (time >= 6){
			i = 2;	// 2
		}
		if (time >= 8){
			i = 3;	// 2
		}
		if (time >= 10){
			i = 4;	// 6
		}
		if (time >= 16){
			i = 5;	// 5
		}
		if (time >= 21){
			i = 0;	// 1
		}
		break;

	case 500:	// ����		32
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 6
		}
		if (time >= 8){
			i = 2;	// 14
		}
		if (time >= 22){
			i = 3;	// 8
		}
		if (time >= 30){
			i = 0;	// 2
		}
		break;
	case 505:	// ����		24
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 3
		}
		if (time >= 5){
			i = 2;	// 3
		}
		if (time >= 8){
			i = 3;	// 1
		}
		if (time >= 9){
			i = 4;	// 1
		}
		if (time >= 10){
			i = 5;	// 9
		}
		if (time >= 19){
			i = 2;	// 4
		}
		break;

	case 506:	// �����A�Ō�����	46
		if (time >= 0){
			i = 1;	// 3
		}
		if (time >= 3){
			i = 0;	// 10
		}
		if (time >= 12){
			i = 1;	// 2
		}
		if (time >= 14){
			i = 2;	// 2
		}
		if (time >= 16){
			i = 3;	// 2
		}
		if (time >= 18){
			i = 4;	// 2
		}
		if (time >= 20){
			i = 5;	// 13
		}
		if (time >= 33){
			i = 6;	// 3
		}
		if (time >= 36){
			i = 7;	// 6
		}
		if (time >= 42){
			i = 8;	// 1
		}
		break;

	case 510:	// �󒆓���		22�`
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 6
		}
		if (time >= 8){
			i = 2;	// 14
		}
		break;

	case 515:	// �󒆓����q�b�g	33�`
		if (time >= 0){
			i = 0;	// 3
		}
		if (time >= 3){
			i = 1;	// 11
		}
		if (time >= 14){
			i = 2;	// 3
		}
		if (time >= 17){
			i = 3;	// 3
		}
		if (time >= 20){
			i = 4;	// 6
		}
		if (time >= 26){
			i = 5;	// 6
		}
		if (time >= 32){
			i = 0;	// 1�`
		}
		break;

	case 600:	// �u������ (A)	45
		if (time >= 0){
			i = 0;	// 11
		}
		if (time >= 11){
			i = 1;	// 2
		}
		if (time >= 13){
			i = 2;	// 4
		}
		if (time >= 17){
			i = 3;	// 10
		}
		if (time >= 27){
			i = 4;	// 6
		}
		if (time >= 33){
			i = 5;	// 7
		}
		if (time >= 40){
			i = 6;	// 7
		}
		if (time >= 47){
			i = 7;	// 1
		}

		break;

	case 605:	// �󒆃u������	45
		if (time >= 0){
			i = 0;	// 11
		}
		if (time >= 11){
			i = 1;	// 2
		}
		if (time >= 13){
			i = 2;	// 3
		}
		if (time >= 16){
			i = 3;	// 10
		}

		break;

	case 606:	//���n�d��	6F
		if (time >= 0){
			r = 10;
			i = 1;	// 6
		}
		if (time >= 2){
			r = 11;
			i = 0;	// 5
		}
		if (time >= 10){
			r = 10;
			i = 1;	// 6
		}
		if (time >= 14){
			r = 10;
			i = 0;	// 6
		}
		break;

	case 610:	// �╥�� (A)	42
		if (time >= 0){
			i = 0;	// 8
		}
		if (time >= 8){
			i = 1;	// 3
		}
		if (time >= 11){
			i = 2;	// 2
		}
		if (time >= 13){
			i = 3;	// 2
		}
		if (time >= 15){
			i = 4;	// 1
		}
		if (time >= 16){
			i = 5;	// 12
		}
		if (time >= 28){
			i = 6;	// 6
		}
		if (time >= 34){
			i = 7;	// 6
		}
		if (time >= 40){
			i = 8;	// 2
		}
		break;
	case 611:	// �╥�� (B)	45
		if (time >= 0){
			i = 0;	// 9
		}
		if (time >= 9){
			i = 1;	// 3
		}
		if (time >= 12){
			i = 2;	// 2
		}
		if (time >= 14){
			i = 3;	// 2
		}
		if (time >= 16){
			i = 4;	// 1
		}
		if (time >= 17){
			i = 5;	// 12
		}
		if (time >= 29){
			i = 6;	// 6
		}
		if (time >= 35){
			i = 7;	// 6
		}
		if (time >= 41){
			i = 8;	// 2
		}
		break;

	case 620:	// �u���ݒu		36
		if (time >= 0){
			i = 0;	// 7
		}
		if (time >= 7){
			i = 1;	// 3
		}
		if (time >= 10){
			i = 2;	// 3
		}
		if (time >= 13){
			i = 3;	// 5
		}
		if (time >= 18){
			i = 4;	// 5
		}
		if (time >= 23){
			i = 5;	// 6
		}
		if (time >= 29){
			i = 6;	// 6
		}
		if (time >= 35){
			i = 7;	// 1
		}

		break;

	case 622:	// �󒆐ݒu	45
		if (time >= 0){
			i = 0;	// 11
			r = 605;
		}
		if (time >= 11){
			i = 1;	// 2
		}
		if (time >= 13){
			i = 2;	// 3
		}
		if (time >= 16){
			i = 3;	// 10
		}

		break;

	case 625:	// �u������		19
		if (time >= 0){
			i = 0;	// 1
			r = 200;
		}
		if (time >= 1){
			i = 0;	// 3
			r = 625;
		}
		if (time >= 4){
			i = 1;	// 10
			r = 625;
		}
		if (time >= 14){
			i = 0;	// 3
			r = 625;
		}
		if (time >= 17){
			i = 0;	// 2
			r = 200;
		}
		break;

	case 626:	// �󒆔���		15�`
		if (time >= 0){
			i = 0;	// 1
			r = 400;
		}
		if (time >= 1){
			i = 2;	// 3
			r = 625;	// �u������
		}
		if (time >= 4){
			i = 3;	// 9
			r = 625;
		}
		if (time >= 13){
			i = 2;	// 1
			r = 625;
		}
		if (time >= 14){
			i = 0;	// 1�`
			r = 400;
		}
		break;

	case 700:	// ��u��		58
		if (time >= 0){
			i = 0;	// 2
		}
		if (time >= 2){
			i = 1;	// 2
		}
		if (time >= 4){
			i = 2;	// 4
		}
		if (time >= 8){
			r = 600;
			i = 2;	// 5
		}
		if (time >= 13){
			r = 600;
			i = 3;	// 25
		}
		if (time >= 38){
			r = 600;
			i = 2;	// 7
		}
		if (time >= 45){
			i = 2;	// 6
		}
		if (time >= 51){
			r = 210;
			i = 6;	// 2
		}
		if (time >= 53){
			r = 210;
			i = 7;	// 3
		}
		if (time >= 56){
			r = 610;
			i = 6;	// 2
		}

		break;

	case 800:	// ��i�؂�	48
		if (time >= 0){
			i = 0;	// 10
		}
		if (time >= 10){
			i = 1;	// 3
		}
		if (time >= 13){
			i = 2;	// 23
		}
		if (time >= 29){
			i = 3;	// 10
		}
		if (time >= 39){
			i = 4;	// 9
		}
		break;

	case 801:	// ��i�؂�2	113
		if (time >= 0){
			i = 0;	// 10
		}


		if (time >= 10){	// ���[�v
			i = 1;	// 2
		}
		if (time >= 12){
			i = 2;	// 2
		}
		if (time >= 14){
			i = 3;	// 2
		}
		if (time >= 16){
			i = 4;	// 2
		}
		if (time >= 18){
			i = 1;	// 2
		}
		if (time >= 20){
			i = 2;	// 2
		}
		if (time >= 22){
			i = 3;	// 2
		}
		if (time >= 24){
			i = 4;	// 2
		}
		if (time >= 26){
			i = 1;	// 2
		}
		if (time >= 28){	// ���[�v�I��
			i = 2;	// 2
		}

		if (time >= 30){
			i = 5;	// 25
		}
		if (time >= 55){
			i = 6;	// 3
		}
		if (time >= 58){
			i = 7;	// 2
		}
		if (time >= 60){
			i = 8;	// 2
		}
		if (time >= 62){
			i = 9;	// 2
		}
		if (time >= 64){
			i = 10;	// 2
		}
		if (time >= 66){
			i = 11;	// 30
		}
		if (time >= 96){
			i = 12;	// 6
		}
		if (time >= 102){
			i = 13;	// 5
		}
		if (time >= 107){
			i = 14;	// 3
		}
		if (time >= 110){	// �I��
			i = 15;	// 2
		}
		break;

	case 820:	// �^��u�[������	34
		if (time >= 0){
			i = 0;	// 7
			r = 820;
		}
		if (time >= 7){
			i = 0;	// 3
			r = 625;
		}
		if (time >= 10){
			i = 1;	// 18
			r = 625;
		}
		if (time >= 28){
			i = 0;	// 4
			r = 625;
		}
		if (time >= 32){
			i = 0;	// 2
			r = 200;
		}
		break;


	case 1000:	// ��i����	1�`
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 4){
			i = 1;	// 1�`
		}
		break;

	case 1005:	// ��i�߂�		1�`
		if (time >= 0){
			i = 0;	// 1�`
		}
		break;
	case 1010:	// ���i����	1�`
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 4){
			i = 1;	// 1�`
		}
		break;

	case 1015:	// �i�߂�		1�`
		if (time >= 0){
			i = 0;	// 1�`
		}
		break;

	case 1020:	// ���Ⴊ�݂���	1�`
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 4){
			i = 1;	// 1�`
		}
		break;

	case 1025:	// ���Ⴊ�ݖ߂�		1�`
		if (time >= 0){
			i = 0;	// 1�`
		}
		break;
	case 1030:	// �󒆂���	11�`
		if (time >= 0){
			i = 0;	// 8
		}
		if (time >= 7){
			i = 1;	// 4�`
		}
		if (time >= 11){
			i = 2;	// 1�`
		}
		break;

	case 1035:	// �󒆖߂�		1�`
		if (time >= 0){
			i = 2;	// 1�`
		}
		break;

	case 1040:	// ���o�E���h	1�`
		if (time >= 0){
			r = 1030;
			i = 2;	// 1�`
		}
		break;

	case 1050:	// �_�E��		4�`
		if (time >= 0){
			i = 0;	// 3
		}
		if (time >= 3){
			i = 1;	// 1�`
		}
		break;

	case 1055:	// �N���オ��	28
		if (time >= 0){
			i = 0;	// 6
		}
		if (time >= 6){
			i = 1;	// 9
		}
		if (time >= 15){
			i = 2;	// 5
		}
		if (time >= 20){
			i = 3;	// 4
		}
		if (time >= 24){
			i = 4;	// 4
		}
		break;

	case 1060:	// ��i����	1�`
		if (time >= 0){
			i = 0;	// 5
		}
		if (time >= 4){
			i = 1;	// 1�`
		}
		break;

	case 1065:	// �󒆂���	11�`
		if (time >= 0){
			r = 1030;
			i = 0;	// 8
		}
		
		if (time >= 7){
			i = 1;	// 4�`
		}
		if (time >= 11){
			i = 2;	// 1�`
		}
		
		break;


	case 1070:	// �s�k		1�`
		if (time >= 0){
			i = 0;	// 1
		}
		break;

	case 1085:	// �󒆎󂯐g		27�`(24�`���[�v
		if (time >= 0){
			r = 41;
			i = 0;	// 9
		}
		if (time >= 9){
			i = 1;	// 3
		}
		if (time >= 12){
			i = 2;	// 5
		}
		if (time >= 17){
			i = 3;	// 4
		}
		if (time >= 21){
			i = 4;	// 3
		}
		if (time >= 24){
			i = 5;	// 3
		}

		if (time >= 27){
			i = 4;	// 3
		}
		if (time >= 30){
			i = 5;	// 3
		}
		break;

	case 2000:	// ��u���u��	1�`
		if (time >= 0){
			i = 0;	// 1�`
		}
		break;

	case 2010:	// ���u��	24�`
		if (time >= 0){
			i = 0;	// 3�`
		}
		if (time >= 3){
			i = 1;	// 3�`
		}
		if (time >= 6){
			i = 2;	// 3�`
		}
		if (time >= 9){
			i = 3;	// 3�`
		}
		if (time >= 12){
			i = 4;	// 3�`
		}
		if (time >= 15){
			i = 5;	// 3�`
		}
		if (time >= 18){
			i = 6;	// 3�`
		}
		if (time >= 21){
			i = 7;	// 3�`
		}

		break;

	case 2020:	// �ݒu	24�`
		if (time >= 0){
			i = 0;	// 3�`
		}
		if (time >= 3){
			i = 1;	// 3�`
		}
		if (time >= 6){
			i = 2;	// 3�`
		}
		if (time >= 9){
			i = 3;	// 3�`
		}
		if (time >= 12){
			i = 4;	// 3�`
		}
		if (time >= 15){
			i = 5;	// 3�`
		}
		if (time >= 18){
			i = 6;	// 3�`
		}
		if (time >= 21){
			i = 7;	// 3�`
		}
		r = 2010;	// ���u��

		break;

	case 2025:	// �ݒu	24�`
		if (time >= 0){
			i = 0;	// 3�`
		}
		if (time >= 3){
			i = 1;	// 3�`
		}
		if (time >= 6){
			i = 2;	// 3�`
		}
		if (time >= 9){
			i = 3;	// 3�`
		}
		if (time >= 12){
			i = 4;	// 3�`
		}
		if (time >= 15){
			i = 5;	// 3�`
		}
		if (time >= 18){
			i = 6;	// 3�`
		}
		if (time >= 21){
			i = 7;	// 3�`
		}
		r = 2010;	// ���u��

		break;

	case 2030:	// ��u��	32�`
		if (time >= 0){
			i = 0;	// 4�`
		}
		if (time >= 4){
			i = 1;	// 4�`
		}
		if (time >= 8){
			i = 2;	// 4�`
		}
		if (time >= 12){
			i = 3;	// 4�`
		}
		if (time >= 16){
			i = 4;	// 4�`
		}
		if (time >= 20){
			i = 5;	// 4�`
		}
		if (time >= 24){
			i = 6;	// 4�`
		}
		if (time >= 28){
			i = 7;	// 4�`
		}

		break;

	case 2100:	// ��u��	32�`
		if (time >= 0){
			i = 0;	// 3�`
		}
		if (time >= 3){
			i = 1;	// 3�`
		}
		if (time >= 6){
			i = 2;	// 3�`
		}
		if (time >= 9){
			i = 3;	// 3�`
		}
		if (time >= 12){
			i = 4;	// 3�`
		}

		break;

	default: 
		if (time >= 0){
			i = 0;	// 1
		}
		break;

		break;	// i �̏����I��

	} // �摜�Z�b�g�I���

	// �X�e�Ɠ�������Ȃ��ꍇ�Ar ������
	if (m_row != r){
		m_row = r;
	}
	// i ���c�ɃZ�b�g
	m_column = i;
	Get_N12(m_row, m_column);
	return 0;


}// �I��

