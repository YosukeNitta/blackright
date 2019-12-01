#pragma once

#include "Define.h"
#include "DxLib.h"

class Pict {

public:
	int x, y;	// �摜�T�C�Y
	int posX, posY;	// ���炷���W
	int image;

	Pict::Pict()
	{
		x = 0;
		y = 0;
		posX = 0;
		posY = 0;
		image = 0;
	}
};

class Box {

public:
	
	int time;	// ����
	int num1, num2;	// �摜No
	int posX, posY;	// ���W
	int HV;	// 1.���E 2.�㉺ 3.�ǂ�����

	int hw[HIT_MAX], hh[HIT_MAX],
		hx[HIT_MAX], hy[HIT_MAX];
	boolean h[HIT_MAX];
	int aw[ATT_MAX], ah[ATT_MAX],
		ax[ATT_MAX], ay[ATT_MAX];
	boolean a[HIT_MAX];

};

class Air {

public:
	
	int stateno;// �X�e�[�g�ԍ�
	int ANum;	// ����̐�(�ő�30)
	Box B[STATE2_MAX];// ���ۂ̔���

	int defhw[HIT_MAX], defhh[HIT_MAX],
		defhx[HIT_MAX], defhy[HIT_MAX];	// �f�t�H���g�{�b�N�X
	boolean defh[HIT_MAX];

	boolean def;

	int aTime;	// �X�e�[�g�̍��v����

	void setup(){
		// AIR�ϐ����� //
			stateno = -1;
			ANum = -1;
			def = false;
			aTime = 0;
			for (int h = 0; h < HIT_MAX; h++){
				defhw[h] = 0;
				defhh[h] = 0;
				defhx[h] = 0;
				defhy[h] = 0;
				defh[h] = false;
			}
			for (int n2 = 0; n2 < STATE2_MAX; n2++){
				B[n2].posX = 0;
				B[n2].posY = 0;
				B[n2].num1 = 0;
				B[n2].num2 = 0;
				B[n2].time = -2;
				B[n2].HV = 0;	// ����Ȃ��Ƃ��������Ȃ�
				for (int n3 = 0; n3 < HIT_MAX; n3++){
					B[n2].h[n3] = false;
					B[n2].a[n3] = false;
					defh[n3] = false;

					B[n2].hw[n3] = 0;
					B[n2].hh[n3] = 0;
					B[n2].hx[n3] = 0;
					B[n2].hy[n3] = 0;

					B[n2].aw[n3] = 0;
					B[n2].ah[n3] = 0;
					B[n2].ax[n3] = 0;
					B[n2].ay[n3] = 0;
				}
			}
	}

	Air(){

	}
	// �ł���΃R���X�g�ŏ�����
	/*
	Air::Air(){
		// AIR�ϐ����� //
		stateno = -1;
		ANum = -1;
		def = false;
		aTime = 0;
		for (int h = 0; h < HIT_MAX; h++){
			defhw[h] = 0;
			defhh[h] = 0;
			defhx[h] = 0;
			defhy[h] = 0;
			defh[h] = false;
		}
		for (int n2 = 0; n2 < STATE2_MAX; n2++){
			B[n2].posX = 0;
			B[n2].posY = 0;
			B[n2].num1 = 0;
			B[n2].num2 = 0;
			B[n2].time = -2;
			B[n2].HV = 0;	// ����Ȃ��Ƃ��������Ȃ�
			for (int n3 = 0; n3 < HIT_MAX; n3++){
				B[n2].h[n3] = false;
				B[n2].a[n3] = false;
				defh[n3] = false;

				B[n2].hw[n3] = 0;
				B[n2].hh[n3] = 0;
				B[n2].hx[n3] = 0;
				B[n2].hy[n3] = 0;

				B[n2].aw[n3] = 0;
				B[n2].ah[n3] = 0;
				B[n2].ax[n3] = 0;
				B[n2].ay[n3] = 0;
			}
		}
	}
	*/
};




//��{�f�[�^�̍\����
typedef struct {

public:
	double walkF, walkB;			// 
	double runF[2], runB[2];		// 
	double jumpY, jumpF[2], jumpB, jumpA;	// Y�̃W�����v�́A�O���X�����A�󒆃W�����v�̍���
	double yAccel;				// 
	int lifeMax;	// �̗͂̍ő�l
	
	int airJump;	// �󒆃W�����v��
	int airDash;	// �󒆃_�b�V����

}Const_t;


//��_���f�[�^�̍\����
typedef struct {

public:
	double yaccel;
	int bound;		// �o�E���h���x���A0�F�Ȃ��A�P�F���o�E���h�A�Q�F��o�E���h
	int boundCount;	// �o�E���h��
	double bound_xvel;
	double bound_yvel;
	double wall_xvel;
	double wall_yvel;
	int wall;		// �o�E���h���x���A0�F�Ȃ��A�P�F���o�E���h�A�Q�F��o�E���h
	boolean counter;	// �J�E���^�[����A�P�F����
	int armor;		// �A�[�}�[��
	//X,Y�̋�炢���x
	//double hitVelX, hitVelY;
	int selfTime;	// �����߂���K�[�h�̎���
	int nokezori;	// �̂����莞��
	boolean fatal;	// �t�F�C�^���J�E���^�[
	double addVelX;	// �A�h�K�ł̋���

}Damage_t;


//�U���f�[�^�̍\����
typedef struct {

public:
	int damage;	// �U���́A���Ȃ݂�short�̍ő��3�����炢
	int hosyo;	// �Œ�_���[�W�ۏ�

	int A_GuardTime;		// �K�[�h����

	double comboLate;	// �R���{���[�g
	double cl_max;	// �ő僌�[�g
	double cl_min;	// �Œ჌�[�g

	boolean forceKurai; // ���������E����炢
	double hosei_K; // �����␳
	boolean throwTurn; // ���������ύX(0,�Ȃ��A1,���)

	// ���ł����x��

	// �q�b�g�J�E���g
	int guardCount;	// �K�[�h��

	// �o�E���h�̃��x��
	// �O�F�Ȃ��A�P�F���A�Q�F��
	int boundLevel;		 
	int wallLevel;

	double bound_xvel;
	double bound_yvel;
	double wall_xvel;
	double wall_yvel;

	boolean gaugeHosei;	// �Q�[�W�����␳�A�Q�[�W�Z�Ŏg��
	double yaccel;	// ����̉����x

	//X,Y�̃q�b�g�o�b�N
	double hbVelX;	// ��p�̋���
	double hbVelY;	// ��p�̋���
	double guard_gx;
	double guard_ax;

	// �q�b�g�G�t�F�N�g
	int hitEff;
	double heSizeX;
	double heSizeY;

	// ����ɖ��G�t��
	boolean addMuteki;
	int amTime;	// ���G�t������

	// �n�k�G�t�F�N�g
	int quakeTime;
	double quakeX, quakeY;

	int att_Level;	// �U�����x���@�ʏ퓊��.-1 �R�}����.0 ��.1 ��{.2 ��.3
	boolean kill;	// �L������A0�łƂǂ߂��h���Ȃ��ł���

	int ncTime;	// �ʏ�Z�L����

	void setup(){
		A_GuardTime = 0;

		comboLate = 0;
		cl_max = 0;
		cl_min = 0;

		forceKurai = 0;
		hosei_K = 0;
		throwTurn = 0;

		// ���ł����x��


		// �o�E���h�̃��x��
		// �O�F�Ȃ��A�P�F���A�Q�F��A�R�F����(�K����o�E���h)
		boundLevel = 0;
		wallLevel = 0;

		bound_xvel = 0;
		bound_yvel = 0;

		wall_xvel = 0;
		wall_yvel = 0;

		gaugeHosei = 0;
		yaccel = 0;

		//X,Y�̃q�b�g�o�b�N
		hbVelX = 0;
		guard_gx = 0;
		guard_ax = 0;

		// �q�b�g�G�t�F�N�g
		hitEff = 0;
		heSizeX = 0;
		heSizeY = 0;

		// ����ɖ��G�t��
		addMuteki = false;
		amTime = 0;

		att_Level = 0;

		kill = true;

		ncTime = 0;
	}
}Attack_t;


/// <summary>
/// �L�����N�^�[�̊�{�ƂȂ���
/// </summary>
class Character{
public:
	// �`��̃T�C�Y
	double GSize;	
	//X,Y�̍��W
	double XPos, YPos;
	// �X�e�[�g�̌o�ߎ���
	int time;
	//����
	boolean muki;
	// �L�����摜���]�iMuki�Ƃ͕ʁj
	boolean turnG;

	// sff�ݒ�
	int sPosX, sPosY;
	
	// air�ݒ�
	int aPosX, aPosY;	// �摜�ʒu
	int aNum1, aNum2;	// �摜
	int aHV;	// ���]��

	// �Z�b�g����Ă���L�����摜
	int Image;

	//[ �U���֘A ]//
	//�U������
	int XAtt[ATT_MAX], YAtt[ATT_MAX];
	int WAtt[ATT_MAX], HAtt[ATT_MAX];
	// ��炢����
	int XHit[HIT_MAX], YHit[HIT_MAX];
	int WHit[HIT_MAX], HHit[HIT_MAX];

	// �U���ݒ�
	Attack_t A;

	// �������H
	void Reset(){
		for (int i = 0; i < ATT_MAX; i++){
			WAtt[i] = 0;
			HAtt[i] = 0;
			XAtt[i] = -60;
			YAtt[i] = -60;
		}
		for (int i = 0; i < HIT_MAX; i++){
			WHit[i] = 0;
			HHit[i] = 0;
			XHit[i] = -70;
			YHit[i] = -70;
		}
	}
};

class Helper:public Character {

public:	
	//***********************
	// �w���p�[�̕ϐ�
	//***********************
	boolean var;	// �w���p�[�̃I���E�I�t
	int HAnimTime;	// �A�j���[�V�����̎���
	int stateno;	// �X�e�[�g
	int state2;		// �X�e�[�g2�i�摜�p�j
	int oldState;	// �O�̃X�e�[�g

	//int HImage;	// ���݂̃L�����摜
	//double GSize;	// �`��̃T�C�Y

	// �ʒu
	//double XPos, YPos;

	// �摜�̃T�C�Y
	int HGraphW, HGraphH;
	// ������
	boolean Alpha;
	// ���x
	double HXVel, HYVel;


	//[ �U���֘A ]//

	//�U���ݒ�
	//int damage;	// �U����
	//int hosyo;	// �ۏ�_���[�W
	int HMoveHit;	// �U������
	int HHitSE;			// �q�b�g�T�E���h
	int HGuardSE;		// �K�[�h��
	int HHitAnim;		// �q�b�g���̃A�j��


	// �K�[�h����  
	// 0:�K�[�h�s�\ 1:�S�� 2:����/�� 3:���Ⴊ��/�� 4:��K�s��i
	// 12:���K 13: ���K 14:��K
	int HGuardF;
	// �K�[�h���N���鋗��
	int HGuardDist;
	// �U�������@1.�Ō� 2.���� 3.��ѓ���
	int attF = 1;

	int HGetPow; // �Q�[�W���
	int HGivePow;	//�^����Q�[�W
	double H_GX, H_GY;	// �q�b�g�o�b�N(�n��)
	double H_AX, H_AY;	// �q�b�g�o�b�N(��)

	int HHitStop;		// �q�b�g�X�g�b�v
	int HSSelf;			// �����̃q�b�g�X�g�b�v
	int HG_HitTime;		// �n���炢�̎���
	int HA_HitTime;		// �󒆋�炢�̎���
	int HG_GuardTime;		// �K�[�h�d��

	int stopTime;		// �����̃q�b�g�X�g�b�v

	//�󂯐g�֘A
	int fallF;	// 0 �Œ��n 1 �Ń_�E��

	// �q�b�g�G�t�F�N�g
	int hitEff;
	double heSizeX;
	double heSizeY;

	//int HA_GuardTime;		// �K�[�h�X�g�b�v����

	void Reset(){
		HMoveHit = 0;
	}
};

// 1P��2P�̃w���p�[
static Helper H1[HM], H2[HM];

// �L�����N�^�[�̃N���X
class Player:public Character{

public:
	int LifeC;
	// �L������1P.2P
	int PSide;
	//***********************
	// �L�����N�^�[�̕ϐ�
	//***********************

	//[ ���W�֘A ]//
	//��{����̑傫��
	int xSize, ySize;
	// �`��̃T�C�Y
	//double GSize;	// �����_�ŏk��
	//�㉺���E�̔���̍��W
	int X_UL, Y_UL;

	// XPos YPos
	//X,Y�̑��x
	double XVel, YVel;
	

	//[ �X�e�[�^�X�֘A ]//
	//�L�����N�^�[��
	int Name;
	char nameC[NAME_MAX];

	//�̗�
	int Life;
	//�p���[�Q�[�W
	int Power;
	// �A�h�Q�[�W
	int aGauge;
	int GRecovery;
	int addG;
	// �I�������J���[
	int color;

	// 1F�O�̌���
	boolean oldMuki;
	//����̉�
	boolean ctrl;
	// �L�����ŗL�̐ݒ�
	int Var[VAR_MAX];

	//0,���� 1,���Ⴊ�� 2,��
	int SFlg;
	// �d�͂𖳎�����
	boolean ignoreG;
	//�W�����v����
	boolean JumpFlg;
	boolean	hyJump;	// �n�C�W�����v
	//�W�����v�̕������f
	int JumpVel;
	//�󒆃W�����v��
	int AirJump;
	int airDash;
	//�_�b�V���̉�
	boolean DashFlg;
	// �󒆃_�b�V���̎���
	int airDashTime;
	// �ڐG����
	boolean touchF;
	// �A�h�K
	boolean advancing;
	//���݂̃X�e�[�g
	int stateno;
	// �^�C���Ō��肳�ꂽ�ԍ�
	int State2;	

	//�O�̃X�e�[�g
	int OldState;
	int oldState2;

	//������x�X�e�[�g��ς���
	boolean More;
	// �X�e�[�g�Œ肷��
	boolean Lock;
	//�L�[����
	int Key;
	// �U�����
	boolean Turn;

	// �{�^���̗v�f
	int button[PAD_MAX];

	int keyPos;		// �L�[�ʒu�`�F�b�N�A�@1�`9 �ɂȂ�
	int keyAtt[9 + 1];		// �L�[���͂̑���
	int key[9 + 1];		// �L�[���͎���

	//��s����
	int Senkou[BUTTON_MAX + 1];
	int K_Senkou[9 + 1];
	// ���o�T����
	int rbutton[BUTTON_MAX + 1];
	

	//�U���ݒ�
	int GetPow; // �Q�[�W���
	int GivePow;	//�^����Q�[�W

	double GX, GY;	// �q�b�g�o�b�N(�n��)
	double AX, AY;	// �q�b�g�o�b�N(��)

	int HitStop;		// �q�b�g�X�g�b�v����
	int HSSelf;			// �����̃q�b�g�X�g�b�v
	int StopTime;		// ��q�b�g�X�g�b�v

	int G_HitTime;		// �q�b�g�̋�炢��Ԃ̎���
	int A_HitTime;		// �󒆂̋�炢��Ԃ̎���
	int G_GuardTime;		// �K�[�h�̋�炢��Ԃ̎���
	int HitAnim;		// �����������̃X�e�[�g
	int HitSE;			// �q�b�g�T�E���h
	int scTime;			// �X�p�L��������

	// �K�[�h����  
	// 0:�K�[�h�s�\ 1:�S�� 2:����/�� 3:���Ⴊ��/�� 4:��K�s��i
	// 12:���K 13: ���K 14:��K
	int GuardF;
	// �K�[�h���ł��鋗��
	int GuardDist;

	int attF;	// �U���̎�ށ@1.�Ō� 2.���� 3.��ѓ���

	// ��������
	boolean rejectF;	// ���������\�t���O
	boolean throwTurn;	// ���������O�̌���
	boolean throwSide;	// �����O�̕���
	

	//��炢�t���O
	int HitCount;	// �q�b�g���J�E���g
	int HFlg;			// �q�b�g����
	int CFlg;			// �L�����Z������
	int MoveHit;		// �X�e�[�g�̃q�b�g�m�F
	
	int Muteki;		// ���G����
	int mutekiF;	// ���G�̎�ށ@0.���S���G 1.�Ō� 2.���� 3.��ѓ���

	//�󂯐g�֘A
	int fall;	// 0 �Œ��n 1 �Ń_�E�� 2.�n��󂯐g�\
	// 0 �Œ��n 1 �Ń_�E��
	int fallF;
	int recover;	// �󂯐g�̉�
	int recTime;	// �󂯐g������悤�ɂȂ�܂ł̎���

	// �R�}���h����
	int cmd[20];

	// �w���p�[�p�Ӂi��������ł���΂悵�j
	//Attack_t A;
	Damage_t D;
	Const_t C;


	//***********************
	// �L�����N�^�[�̉摜�p�ϐ�
	//***********************
	
	// �A�j������
	int animElem[STATE_MAX];

	// �p���b�g
	int Palno;
	// �摜�̃T�C�Y
	int GraphW, GraphH;
	// �A�j���[�V�����̃t���[��
	int AnimTime;
	
	// �X�e�[�g��؂�ւ������A���Ԃ����Z�b�g���Ȃ��i�f�t�H�̓I�t�j
	int NoChangeTime;
	// �摜�̗D��x
	int SprPriority;
	// �F�ω�
	int colorC[3];	// 0,r 1,g 2,b
	int colorCTime;		// �F���ς��I
	// �c���I���I�t
	boolean mirage;

	// �R���X�g���N�^
	/*
	~Player()          // �f�X�g���N�^
	{

	}
	*/
};

static Player P1, P2;
