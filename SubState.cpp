//==================================================================================
//
//			�X�e�[�g�����̊Ǘ���
//
//==================================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include "SData.h"
using namespace stateData;

//�f�t�@�C����----------------------------------------------

// #include "Define.h" �ɂāA�֐����Z�b�g


//�ް���`��------------------------------------------------

//[ �_�b�V�� ]
typedef struct {
	int Image;		// ���݂̉摜
	float GSize;	// �`��T�C�Y
	int XPos, YPos;
	int XSize, YSize;
	int Num1, Num2;	// �摜�ԍ� �c�Ɖ�
	int Time;		// ����

}Dash_t;

// �G�t�F�N�g�̕ϐ��p��
static Dash_t D[2];
static Player P[2];

// ��̃_�b�V���֘A
static int m_DashFlg[2];
static int m_dashTime[2];
static boolean m_dMuki[2];
static int m_KeyL[2], m_KeyR[2];

static int m_OldState[2];

static const int DASH_TIME = 13;

// �n�C�W�����v�P�\
static int hjFlg[2];


//�����֐��錾��--------------------------------------------

static void P_Dash();
static Player Neutral(Player p1);

//�v���O������----------------------------------------------

//�f�[�^����n�֐�

//-------------------------------------------------------------------------------
//	��`�@int CharMove( void )
//
//	�����@�o�g�����[�h��1P, 2P�̒l��ݒ肷��A���̃��[�h�̗v
//
//	�����@int (��)  �����̒l�Ŏ��̉�ʂ����܂�
//
//	�߂�l�@�Ȃ��񂶂�Ȃ��H�i�K���j
//-------------------------------------------------------------------------------


//�X�e�[�g�ԍ�����
void EnterState()
{	
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){
		// �֐��ւ̑���p
		Player EP;
		if (i == 0)EP = P[1];
		else{ EP = P[0]; }

		/************
		* ���Ⴊ��(11) or ����(0)����
		************/
		P[i] = Neutral(P[i]);
	}
	P1 = P[0], P2 = P[1];
	

	/********
	* �U�����
	********/

	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){
		// ����
		Player EP = P[1];
		if (i == 1)EP = P[0];

		// �P�t���O�̌����L��
		P[i].oldMuki = P[i].muki;

		// �_�b�V�����Ȃ�U������Ȃ�
		if ((P[i].DashFlg == 1) && (P[i].ctrl))P[i].Turn = false;
		else { P[i].Turn = true; }
		//else { P[i].Turn = true; }

		// �n��ɂ���A�R���g���[���\
		// �t�������Ă�����
		// 1P
		if ((P[i].SFlg != 2) && (P[i].ctrl) && (P[i].Turn) && (P[i].stateno < 1000)){
			if ((P[i].XPos < EP.XPos) && (P[i].muki == 1)){
				P[i].muki = 0;
				P[i].XVel = -P[i].XVel;
				if (P[i].SFlg == 1)P[i].stateno = 6;
				else{ P[i].stateno = 5; }
			}
			if ((P[i].XPos > EP.XPos) && (P[i].muki == 0)){
				P[i].muki = 1;
				P[i].XVel = -P[i].XVel;
				if (P[i].SFlg == 1)P[i].stateno = 6;
				else{ P[i].stateno = 5; }
			}
		}
	}
	P1 = P[0], P2 = P[1];

	/********
	* �������_�b�V��
	********/

	// �_�b�V���ݒ�
	P_Dash();
	{
		P[0] = P1, P[1] = P2;

		for (int i = 0; i < 2; i++){

			// �_�b�V���ɕω����邩���f
			//P[i].DashFlg = m_DashFlg[i];
			if (m_DashFlg[i] > 0)P[i].DashFlg = true;
			else{ P[i].DashFlg = false; }
		
			// �n��ɂ��ăR���g���[���\�A�㉺���͂��Ȃ��A�_�b�V���t���O���I�t�̎�
			// 1P 
			if ((P[i].SFlg == 0) && (P[i].ctrl) && (P[i].button[102] == 0)){
				if (P[i].DashFlg){
					if (P[i].button[104] != 0){
						if (P[i].muki == 0){
							P[i].stateno = 35; // ���E�����Ĕ��f
							P[i].DashFlg = 0, m_DashFlg[i] = 0;	// �o�N�X�e��~
						}
						else {
							P[i].stateno = 30;
						}
					}
					if (P[i].button[106] != 0){
						if (P[i].muki == 0){
							P[i].stateno = 30;
						}
						else {
							P[i].stateno = 35; // ���E�����Ĕ��f
							P[i].DashFlg = 0, m_DashFlg[i] = 0;	// �o�N�X�e��~
						}
					}
				}
				else{
					if (P[i].button[104] != 0){
						if (P[i].muki == 0)P[i].stateno = 21; 	// �O�����
						else{ P[i].stateno = 20; }

					}
					if (P[i].button[106] != 0){
						if (P[i].muki == 0)P[i].stateno = 20; 	// �O�����
						else{ P[i].stateno = 21; }
					}
				}
			}

			// �_�b�V���t���O�� & �_�b�V������Ȃ� & ���Ԑ؂�Ȃ璆�~
			if ((m_DashFlg[i] > 0) && m_dashTime[i] == 0 && P[i].stateno != 30){
				// �������ςŏ����Ȃ�
				if (m_DashFlg[i] == 1 && m_KeyL[i] == 0)m_DashFlg[i] = 0;
				else if (m_DashFlg[i] == 2 && m_KeyR[i] == 0)m_DashFlg[i] = 0;
			}
			// �_�b�V�����X�e�b�v�Ȃ�
			if (P[i].stateno == 30 && P[i].SFlg == 2){
				m_DashFlg[i] = 0;
			}
			// �_�b�V�����ɑ��̍s�������Ȃ�
			if ((P[i].stateno != 30) && (m_OldState[i] == 30)){
				P[i].DashFlg = 0, m_DashFlg[i] = 0;	// ��~
			}

			m_OldState[i] = P[i].stateno;

		}	// for�I��
		P1 = P[0], P2 = P[1];
	}

	
	/********
	* �K�[�h
	********/

	int dist1, dist2;
	dist1 = (int)P1.XPos - (int)P2.XPos;
	dist2 = (int)P2.XPos - (int)P1.XPos;

	// 1P 
	/*
	// ����\�A���肪�U���X�e�[�g�A���͉̂��}���u
	if ((P1.ctrl) && (P2.stateno >= 200) && (P2.stateno <= 999) &&
		((dist1 < P2.GuardDist) && (dist2 < P2.GuardDist))){
		// �����ƃ��o�[��OK�̂Ƃ�
		if (((P1.XPos <= P2.XPos) && (P1_BInput(104) != 0)) ||
			((P1.XPos >= P2.XPos) && (P1_BInput(106) != 0))){
	
			if (P1.SFlg == 2){
				P1.stateno = 56;
				// �󒆂ɂ���
			}
			else if (P1_BInput(102) != 0){
				P1.stateno = 53;
				// + ���v�f
			}

			else{
				P1.stateno = 50;
				// ����ȊO
			}
		}
	}
	

	// �w���p�[�̃K�[�h
	for (int i = 0; i < HELPER_MAX; i++){
		if ((P1.ctrl) && ((H2[i].XPos - P1.XPos <= 150) &&
			(H2[i].XPos - P1.XPos >= -150))
			&& (H2[i].WAtt[0] > 0 && H2[i].HAtt[0] > 0)){
			// �����ƃ��o�[��OK�̂Ƃ�
			if (((P1.XPos <= P2.XPos) && (P1_BInput(104) != 0)) ||
				((P1.XPos >= P2.XPos) && (P1_BInput(106) != 0))){

				if (P1.SFlg == 2){
					P1.stateno = 56;
					// �󒆂ɂ���
				}
				else if (P1_BInput(102) != 0){
					P1.stateno = 53;
					// + ���v�f
				}

				else{
					P1.stateno = 50;
					// ����ȊO
				}
			}
		}
	}
	*/

	/********
	* �W�����v
	********/
	Player P[2];
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){
		// �n�ɑ��������Ă���ꍇ�̂ݏ���͂�����
		// 1P
		if ((P[i].ctrl) && (P[i].SFlg != 2) && (P[i].button[108] != 0) &&
			(P[i].stateno != 40)){
			P[i].stateno = 40;
		}

		// �n�C�W�����v
		if ((hjFlg[i] > 0) && (P[i].StopTime == 0))hjFlg[i]--;
		if (P[i].button[102] == 1){
			hjFlg[i] = 9;
		}
		if ((hjFlg[i] > 0) && (P[i].SFlg != 2) && (P[i].button[108] != 0)){
			P[i].hyJump = true;
			//P[i].AirJump--;
		}

		// ��i�W�����v�A�W�����v�񐔂�����
		// �n���菭�������Ȃ��Ƃł��Ȃ�
		if ((P[i].ctrl) && (P[i].SFlg == 2) && (P[i].button[108] == 1) &&
			(P[i].YPos < GROUND - 16) && (P[i].AirJump > 0)){	// �O����10
			P[i].stateno = 45;
		}

		// ��_���Ԍ���
		if (P[i].airDashTime > 0)P[i].airDashTime -= 1;
		
		// �󒆃_�b�V��
		// �O����20
		if ((P[i].ctrl) && (P[i].SFlg == 2) && (P[i].DashFlg && P[i].K_Senkou[6] > 0) &&
			((P[i].YVel >= 0 && (P[i].YPos + (P[i].YVel * 2) < GROUND - 22)) || (P[i].YVel < 0 && P[i].YPos < GROUND - 134)) &&
			(P[i].airDash > 0)){
			P[i].stateno = 60;
			P[i].AirJump--, P[i].airDash--;
			P[i].airDashTime = 20;
			P[i].time = 0;
			P[i].DashFlg = 0, m_DashFlg[i] = 0;	//�@�_�b�V����~
		}
		else if ((P[i].ctrl) && (P[i].SFlg == 2) && (P[i].DashFlg && P[i].keyAtt[4] > 0) &&// keyatt or senkou
			((P[i].YVel >= 0 && (P[i].YPos + (P[i].YVel * 2) < GROUND - 22)) || (P[i].YVel < 0 && P[i].YPos < GROUND - 134)) &&
			(P[i].airDash > 0)){
			P[i].stateno = 65; 
			P[i].AirJump--, P[i].airDash--;
			P[i].DashFlg = 0, m_DashFlg[i] = 0;	//�@�_�b�V����~
			P[i].airDashTime = 20;
			P[i].time = 0;
		}

		// �W�����v�񐔐ݒ�A2��܂�
		if ((P[i].SFlg != 2) && (P[i].YPos == GROUND) && (P[i].stateno != 40)){
			P[i].AirJump = P[i].C.airJump;
			P[i].airDash = P[i].C.airDash;
			P[i].hyJump = false;
		}


		//  [ �v�b�V���K�[�h ]
		if ((P[i].button[4] > 0) &&  (P[i].ctrl) && (P[i].SFlg != 2)
			&& (P[i].aGauge > 0) && (P[i].keyAtt[4] > 0)) {
			//if(P[i].SFlg == 2)P[i].stateno = 59;
			if (P[i].keyAtt[2] > 0)P[i].stateno = 58;
			else { P[i].stateno = 57; }
			P[i].ctrl = 0;
			P[i].D.nokezori = 0;
		}
		
	}
	P1 = P[0], P2 = P[1];

	/********
	* �l��State�ɓn��
	********/
	Get_PSet(P1, P2);
	Get_SSet(S);
	Get_HSet(H1, H2);
}

void EnterAttack()
{

	/********
	* �ʏ�Z�i���ʁj
	********/

	P[0] = P1, P[1] = P2;
	Helper h[2][HM];
	for (int n = 0; n < HM; n++){
		h[0][n] = H1[n];
		h[1][n] = H2[n];
	}

	for (int i = 0; i < 2; i++){
		// ��_����@��s����
		if ((P[i].stateno == 60 || P[i].stateno == 65)){
			for (int n = 1; n < 5; n++){
				if (P[i].Senkou[n] > 0){
					P[i].rbutton[n] = 1;
					P[i].Senkou[n] = 5;
				}
			}
		}

		// ����\�Ȏ�
		// 1P
		if (P[i].ctrl){
			if ((P[i].SFlg == 2) && (P[i].YPos < GROUND)){		// ��
				// A.B.C �𔻒f
				if (P[i].Senkou[3] > 0)P[i].stateno = 420;
				else if (P[i].Senkou[2] > 0)P[i].stateno = 410;
				else if (P[i].Senkou[1] > 0)P[i].stateno = 400;
			}
			else if ((P[i].SFlg != 2) && (P[i].button[102] > 0)){	// ���Ⴊ��
				if (P[i].Senkou[3] > 0)P[i].stateno = 320;
				else if (P[i].Senkou[2] > 0)P[i].stateno = 310;
				else if (P[i].Senkou[1] > 0)P[i].stateno = 300;
			}
			else if ((P[i].SFlg != 2) && (P[i].button[102] == 0)){	// ����
				if (P[i].Senkou[3] > 0)P[i].stateno = 220;
				else if (P[i].Senkou[2] > 0)P[i].stateno = 210;
				else if (P[i].Senkou[1] > 0)P[i].stateno = 200;
			}
		}
		

		// �K�[�L����
		if ((P[i].stateno == 50 || P[i].stateno == 51 || 
			 P[i].stateno == 53 || P[i].stateno == 54)
			//&& (P[i].aGauge >= (GUARD_MAX / 2)) 
			&& (P[i].Power >= 1000)
			&& (P[i].keyAtt[6] > 0)
			&& ((P[i].rbutton[1] > 0) && (P[i].rbutton[2] > 0))
			){
			P[i].stateno = 520;
			P[i].time = 0;
			P[i].ctrl = 0;
		}

	}
	P1 = P[0], P2 = P[1];
	
	
	for (int n = 0; n < HM; n++){
		H1[n] = h[0][n];
		H2[n] = h[1][n];
	}
	

	// �l��Ԃ�
	Get_PSet(P1, P2);
	Get_SSet(S);
	Get_HSet(H1, H2);
}

// �_�b�V�����\�ɂ���t���O
void P_Dash()
{

	Player GP[2];
	GP[0] = P1, GP[1] = P2;

	for (int i = 0; i < 2; i++){

		// �������͂𔻒f
		{
			// �E���́��㉺�v�f���Ȃ�
			if ((GP[i].button[106] != 0)
				&& ((GP[i].SFlg == 2) || (GP[i].keyAtt[2] == 0))){//&& (GP[i].keyAtt[8] == 0)){
				m_KeyR[i] += 1;
			}
			else{
				m_KeyR[i] = 0;
			}

			// ��
			if ((GP[i].button[104] != 0)
				&& ((GP[i].SFlg == 2) || (GP[i].keyAtt[2] == 0))){//&& (GP[i].keyAtt[8] == 0)){
				m_KeyL[i] += 1;
			}
			else{
				m_KeyL[i] = 0;
			}
		}

		// ���͗P�\�������āA �L�[���͂��J�n�����u��
		// ��
		if ((m_KeyL[i] == 1) && (m_dMuki[i] == 0) && (m_dashTime[i] > 0)){
			m_DashFlg[i] = 1;
		}
		// ��
		if ((m_KeyR[i] == 1) && (m_dMuki[i] == 1) && (m_dashTime[i] > 0)){
			m_DashFlg[i] = 2;
		}


		// �L�[���͂��I�����āA�_�b�V���t���O���I���̎�
		//if (((m_KeyR[i] == 0) && (m_KeyL[i] == 0)) && (m_DashFlg[i] = 1))m_DashFlg[i] = 0;

		// ���E�ǂ��炩�ɓ��� & ��t�P�\��0
		if ((m_KeyL[i] == 1) && (m_dashTime[i] == 0)){
			m_dashTime[i] = DASH_TIME;	// ��t�P�\�̐ݒ�
			m_dMuki[i] = 0;
		}
		if ((m_KeyR[i] == 1) && (m_dashTime[i] == 0)){
			m_dashTime[i] = DASH_TIME;	// ��t�P�\�̐ݒ�
			m_dMuki[i] = 1;
		}

		// �_�b�V���^�C�������炵�Ă���
		if (m_dashTime[i] != 0)m_dashTime[i] -= 1;
	}
}

Player Neutral(Player p1){
	
	/*
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){
	// �n��ɂ��ăR���g���[���\�Ȏ��A2�����ɓ���
	// ������痧����
	// 1P
	if ((P[i].ctrl) && (P[i].SFlg != 2) && (P[i].Lock == 0)){
	// �����͂��Ă��āA�X�e�[�g11����Ȃ�
	if ((P[i].button[102] != 0) && (P[i].stateno != 11)){
	P[i].stateno = 10;
	}
	// ���b�N�I���͗����X�e�[�g�ɖ߂�Ȃ�
	else if ((P[i].button[102] == 0) && (P[i].SFlg == 0)){
	P[i].stateno = 0;
	}
	}
	}
	P1 = P[0], P2 = P[1];
	*/

	// �n��ɂ��ăR���g���[���\�Ȏ��A2�����ɓ���
	// ������痧����
	// 1P
	if ((p1.ctrl) && (p1.SFlg != 2) && (p1.Lock == 0)){
		// �����͂��Ă��āA�X�e�[�g11����Ȃ�
		if ((p1.button[102] != 0) && (p1.stateno != 11)){
			p1.stateno = 10;
		}
		// ���b�N�I���͗����X�e�[�g�ɖ߂�Ȃ�
		else if ((p1.button[102] == 0) && (p1.SFlg == 0)){
			p1.stateno = 0;
		}
	}

	return p1;
}

void SubState_DashReset()
{
	for (int i = 0; i < 2; i++){
		m_KeyR[i] = 0;
		m_KeyL[i] = 0;
		m_DashFlg[i] = 0;
		m_dashTime[i] = 0;
		m_dMuki[i] = 0;
		hjFlg[i] = 0;
	}
}

void GetP_SubState(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_SubState(System_t GS)
{
	S = GS;
}

void GetH_SubState(Helper GH1[], Helper GH2[])
{
	int i;

	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}

}