//�C���N���[�h��--------------------------------------------
#include "pch.h"

static Player P[2];

void DamSetting()
{
	P[0] = P1, P[1] = P2;

	for (int i = 0; i < 2; i++){
		if (i == 0)P[0] = P1, P[1] = P2;
		else if (i == 1){
			Player p = P[0];
			P[0] = P[1], P[1] = p;
		}
		//----------
		// �g�����p
		//----------
		if (S.TraningFlg){
			// ���ȂȂ�
			if (P[0].Life <= 0)P[0].Life = 1;
		}
		//-------------------
		// �L�����Z������
		// �q�b�g���J�E���g
		// ���G���ԕύX
		//-------------------

		//[ �L�����Z������ ]
		// �q�b�g���ĂȂ����̓L�����Z�����ł��Ȃ�����
		if ((P[0].time <= 1) && (P[0].stateno >= 200)){
			P[0].CFlg = 0;
		}

		// 1000 �ȍ~�i��炢�j�̃X�e�[�g�Ȃ�A�q�b�g�����J�E���g
		// 2P �����Ă邩�m�F
		if ((P[1].stateno >= 1000) && (P[1].ctrl == 0)){
			P[1].HFlg = 1;
		}
		// ���S
		else if (P[1].Life <= 0){
			P[1].HFlg = 0;
			P[1].D.boundCount = 0;
			P[0].HitCount = 0;
			P[0].A.hosei_K = 1;
		}
		// 2P�̗̑͂������āA��炢�ł͂Ȃ�
		else{
			P[1].HFlg = 0;
			P[1].D.boundCount = 0;
			P[1].D.wall = 0;

			P[0].HitCount = 0;
			// �K�[�h���[�V��������Ȃ�
			if ((P[1].stateno < 50) || (P[1].stateno >= 60))P[0].A.guardCount = 0;
			P[0].A.hosei_K = 1;	// �����␮��1�ɖ߂�
			P[0].A.gaugeHosei = false; // �Q�[�W�␳���~
		}

	}
	P1 = P[1], P2 = P[0];

	
	P[0] = P1, P[1] = P2;
	for (int n = 0; n < 2; n++){
		// [ �J�E���^�[�q�b�g ]
		if (P[n].stateno >= 200 && P[n].stateno <= 999){
			// �����J�E���^�[�ȊO�̓J�E���^�[���蔭��
			if ((P[n].time == 0) && (P[n].D.counter != 2))
				P[n].D.counter = 1;

			// �����J�E���^�[�Ȃ�I��
			if (P[n].D.counter == 2)break;

			// �U�����肪�o����J�E���^�[�I��
			for (int i = 0; i < ATT_MAX; i++){
				if ((P[n].WAtt[i] > 0) && (P[n].HAtt[i] > 0)){					
					P[n].D.counter = 0;
					P[n].D.fatal = 0;
				}
			}
		}
		// �Z����Ȃ��Ȃ�J�E���^�[���Ȃ�
		else{ 
			P[n].D.counter = 0; 
			P[n].D.fatal = 0;
		}

		// [�X�[�p�[�A�[�}�[]
		if (P[n].D.armor < 0)P[n].D.armor = 0;

		// �X�p�L�����P�\
		if ((P[n].scTime > 0) && (P[n].StopTime == 0))P[n].scTime--;
		if ((P[n].A.ncTime > 0) && (P[n].StopTime == 0))P[n].A.ncTime--;
	}
	P1 = P[0], P2 = P[1];
	
	// �l��Ԃ�
	//
	Get_PSet(P1, P2);
}

void GetP_SubDamSet(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_SubDamSet(System_t GS)
{
	S = GS;
}
