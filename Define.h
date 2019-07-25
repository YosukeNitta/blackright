#include "DxLib.h"


#ifndef DEF_DEFINE_H	//��x��`����Ă�����A��`���Ȃ�
#define DEF_DEFINE_H

// �ύX�������
static const int STAGE_MAX = 3;		// �X�e�[�W�̍ő�
static const int CHARACTERS_MAX = 6;

// �ނ���L��������
static const int EIN = 1;
static const int CORNEL = 2;
static const int BOUNCER = 3;
static const int HELIOS = 4;
static const int HYDE = 5;
//static const int ERIKU = 6;

//*************
//  �萔
//*************

static enum GameScene
{
	NoScene,
	MenuScene,
	SelectScene,
	VersusScene,
	ConfigScene,
	TitleScene,
	ResultScene,
	NetworkScene,
	ReplayScene,
	ArcadeScene,
};
static GameScene gameScene;

// �X�N���[�����l
static const int SCREEN_W = 640;	//��ʂ̉���
static const int SCREEN_H = 480;	//��ʂ̏c��

// �X�e�[�W���l
static const int STAGE_WIDTH = 1280;	//�X�e�[�W�̉���(1280)
static const int STAGE_HEIGHT = 720;	//�X�e�[�W�̏c��(7]:20)
static const int GAMENHAJI = 35;	//��ʒ[�̈ʒu
static const int KURAI_YBASE = 12;		//��炢����� +-0 �̈ʒu

static const int NAME_MAX = 18;	//���O�������̍ő�

//
static const int POWER_MAX = 3000;	//�p���[�̍ő�l
static const int GUARD_MAX = 1000;	// �K�[�h�Q�[�W�ő�
static const int ADD_MAX = 100;	// �A�h�Q�[�W�ő�

static const double GRAVITY = 0.35;  // �L�����Ɋ|����d�͉����x
static const int GROUND = STAGE_HEIGHT - 48;	//�n�ʂ̈ʒu(-45)
static const int D_GROUND = GROUND + 8;			//��炢���̒n��

// �X�e�[�g�A�C���[�W
static const int STATE_MAX = 3000;	// �X�e�[�g��MAX
static const int STATE2_MAX = 50;	// �X�e�[�g��MAX
static const int IMAGE_MAX = STATE_MAX;	// �ۑ��摜����MAX
static const int IMAGE2_MAX = STATE2_MAX;	// �ۑ����摜��MAX
static const int PICT_MAX = 10000;	// csv����ǂݍ��񂾉摜��MAX(�����mugen����ۃR�s�ł���)

// �{�^���ݒ�
static const int BUTTON_MAX = 9;		// �{�^���ݒ�̍ő�
static const int KEY_MAX = 256;		// �L�[���͂̍ő�
static const int PAD_MAX = 119;		// �p�b�h���͂̍ő�

// ����
static const int HIT_MAX = 10;		// ��炢����.MAX
static const int ATT_MAX = 10;		// �U������.MAX
static const int TX_MAX = 64;	// �������̍ő�

//
static const int COLOR_MAX = 6;	// �J���[�̐�
static const int T_MENU_MAX = 5;		// �g�����ݒ荀�ڂ̐�

// �w���p�[
static const int HELPER_MAX = 20;	// �w���p�[��.MAX
static const int HM = HELPER_MAX;	// �������팸�p

// �Q�[���������l //
static const int LIFECOUNT_MAX = 2;	// ���C�t�J�E���^�̍ő�l
static const int ROUNDTIME = 99;			// ��������
static const int TR_TIME = 7-6;			// ���������P�\(-6�̓q�b�g�X�g�b�v)
static const int VAR_MAX = 50;		// �L�����ϐ��̍ő�
static const int SC_TIME = 30;		// �X�p�L�����P�\	
static const int NC_TIME = 10;		// �ʏ�Z�L�����P�\	
static const int ThrowMuteki_Down = 7;		// �������G����
static const int ThrowMuteki_Guard = 3;		// �������G����
static const int ThrowMuteki_Hit = 5;		// �������G����

// ���̐F
static const int Cr = GetColor(255, 255, 255);
// �I�����W�̐F
static const int Oran = GetColor(255, 200, 0);

#endif