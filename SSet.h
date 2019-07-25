#ifndef DEF_SSET_H	//��x��`����Ă�����A��`���Ȃ�
#define DEF_SSET_H

// �Q�[���̐��l���L�^���Ă���
typedef struct {

	/***********************
	* �V�X�e���̕ϐ�
	***********************/
	// �X�N���[���̈ʒu
	double ScroolX, ScroolY;
	// �I���t���O
	int EndFlg;
	// ���Ԓ�~�i���o�E�Ó]���j
	int StopTime;
	// �Ó]���ԁi���̎��Ԓ��͉�ʂ������Ȃ�Astate�֐��Ń^�C���������j
	int Anten;
	// �Q�[�W�\����~
	int noGauge;
	// �Q�[���̗���@0:�C���g���A1:�ΐ�A2:���E���h�擾�A3:�Q�[���Z�b�g
	int roundState;
	int roundTime;	// �ݒ莞��
	boolean noScroolX, noScroolY;	// �X�N���[�����~����
	// �Ռ�(�J����)
	double quakeY;
	int quakeTime;

	// ���݂̃X�e�[�W
	int NowStage;
	// AI�I���I�t
	boolean AI;
	/************
	/ �g�����ݒ�
	/***********/
	boolean TraningFlg;
	//�g�������j���[
	// 0.�̗́@1.�p���[  2.��ԁi������j
	// 3.�󂯐g  4.�K�[�h  5.�ݒ�m�F
	// 6.����  7.2P����
	int TSwitch[20];
	int TMAX[20];			// �����ڂ̐�

	// int Select_MAX;		// �I�����ڂ̐�(pause�Ɍʂŗp��)

	int T_Life;			// �̗�
	int T_Power;		// �p���[
	int T_GGauge;		// �K�[�h�Q�[�W
	int T_AddGauge;		// �A�h
	int T_Condition;	// ��ԁi������j
	int T_Recovery;		// �󂯐g
	int T_Guard;		// �K�[�h	(���Ȃ��A����A���)
	int T_Counter;		// �J�E���^�[�q�b�g	0.���Ȃ� 1.����
	int T_ThrowReject;	// ���������@0.���Ȃ� 1.����
	int T_Box;			// ����\���@0.���Ȃ� 1.��炢 2.�U�� 3.����

}System_t;

// �v���C���[1�E2�̒�`�A������static���g�����Ȃ��悤�ɂ���
static System_t S;

#endif