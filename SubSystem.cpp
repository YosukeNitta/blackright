//==================================================================================
//
//			�⏕�v�Z�̊Ǘ���
//
//==================================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"

//�f�t�@�C����----------------------------------------------

//#include "Define.h" �Őݒ�

//�ް���`��------------------------------------------------

static void TH_Set(Player GP1, Player GP2);

//�����֐��錾��--------------------------------------------

//#include "Setting.h" �Őݒ�

// �����ׂ�����
static double Distance, DisPoint;
// �X�N���[����
static double ScXPoint, ScYPoint;
// �X�N���[���p�̃L�������x
static double P1_ScX, P2_ScX;

// 1�t���[���O�̌���
static int OldMuki[2];
static double m_oldXPos[2], m_oldYPos[2];

static Player mP1, mP2;

// �ߋ��̍��W����A���炷�ʂ��v�Z
double xp[2], yp[2];

// �����̃L�����A�E���̃L�����A�L�����Ԃ̋���
static double leftP, rightP, PDist;

//�v���O������----------------------------------------------

//==================================================================================
//
//			�����ł� �N���X�EState �����ł͌��Â炢�����������Ă��Ă���
//
//==================================================================================

void TouchHantei()
{
	TH_Set(P1, P2);
	Get_PSSet(P1, P2, S);
}

void TH_Set(Player GP1, Player GP2)
{

	Player P[2];
	P[0] = GP1, P[1] = GP2;

	for (int i = 0; i < 2; i++){
		//P1
		P[i].X_UL = (int)P[i].XPos - (P[i].xSize / 2);	//�����X���W
		// �󒆂��n�ォ�Ŕ���ς���
		if (P[i].SFlg == 2){
			P[i].Y_UL = (int)P[i].YPos - P[i].ySize - (20 * 2);		// �󒆂̎��͔��菬����
		}
		else{
			P[i].Y_UL = (int)P[i].YPos - P[i].ySize;		//�����Y���W�A10*2 �͉摜�̑���
		}

		if (!P[i].touchF){
			P[i].X_UL = 0 - (i+1) * 500;
			P[i].Y_UL = 0 - (i+1) * 500;
		}
	}

	GetP_SubSystem(P[0],P[1]);
}

int SetScrool()
{
	//--------------------
	//
	// X�X�N���[��
	//
	//--------------------

	/*
	//----------------
	//
	// �p�^�[���P�F��������������
	// ���������F��ʂ�4/5�̋������߂��Ȃ�
	// �߂Â������F�ǂ��炩���߂Â������ɂ���
	//
	//----------------
	// ���ɔ���ScX�����̈ʒu��
	ScXPoint = S.ScroolX;

	double mDist = GAMENHAJI * 3;		// �J�����ړ���������̋���
	double mSDist = mDist * 2;			// �J�������ڋ߂������

	// 1P
	if (P1.XPos < S.ScroolX + mDist){
		ScXPoint = P1.XPos - mDist;
	}
	else if (P1.XPos > S.ScroolX + SCREEN_W - mDist){
		ScXPoint = P1.XPos + mDist - SCREEN_W;
	}
	// 2P
	if (P2.XPos < S.ScroolX + mDist){
		ScXPoint = P2.XPos - mDist;
	}
	else if (P2.XPos > S.ScroolX + SCREEN_W - mDist){
		ScXPoint = P2.XPos + mDist - SCREEN_W;
	}
	
	// ���������ꂷ������A�X�N���[�����Ȃ�
	if (PDist >= SCREEN_W - (mDist * 2))ScXPoint = S.ScroolX;
	

	// 1P��2P����ʒ[�ɂ����ꍇ�A�X�N���[�����Ȃ�
	if ((SCREEN_W - mSDist <= P1.XPos - P2.XPos) ||
		(SCREEN_W - mSDist <= P2.XPos - P1.XPos))
	{
		ScXPoint = S.ScroolX;
	}
	*/


	/*
	//----------------
	//
	// �p�^�[���Q�F�����������L�[�v
	// ���������F1P��2P�̋���
	//
	//----------------
	// ���ɔ���ScX�����̈ʒu��
	ScXPoint = S.ScroolX;

	if (P1.XPos <= P2.XPos){
		leftP = P1.XPos;
		rightP = P2.XPos;
	}
	else if (P2.XPos < P1.XPos){
		leftP = P2.XPos;
		rightP = P1.XPos;
	}
	PDist = rightP - leftP;
	if (PDist >= SCREEN_W - (GAMENHAJI * 2)){}
	else{ ScXPoint = leftP - (SCREEN_W / 2) + (PDist / 2); }
	*/

	
	//----------------
	//
	// �p�^�[���R�F�����������L�[�v
	// ���������F1P��2P�̋���
	//
	//----------------
	// ���ɔ���ScX�����̈ʒu��
	ScXPoint = S.ScroolX;
	
	double mDist = GAMENHAJI * 4;		// �J�����ړ���������̋���
	double mSDist = mDist * 2;			// �J�������ڋ߂������

	// �����𑪂�A1P��2P�̍��E�����߂�
	if (P1.XPos <= P2.XPos){
	leftP = P1.XPos;
	rightP = P2.XPos;
	}
	else if (P2.XPos < P1.XPos){
	leftP = P2.XPos;
	rightP = P1.XPos;
	}
	// �L�����Ԃ̋���
	PDist = rightP - leftP;

	// �[�[����Ȃ��Ƃ��A��������
	if (PDist < SCREEN_W - (GAMENHAJI * 2)){
		// 1P
		if (P1.XPos < S.ScroolX + mDist){
			ScXPoint = P1.XPos - mDist;
		}
		else if (P1.XPos > S.ScroolX + SCREEN_W - mDist){
			ScXPoint = P1.XPos + mDist - SCREEN_W;
		}
		// 2P
		if (P2.XPos < S.ScroolX + mDist){
			ScXPoint = P2.XPos - mDist;
		}
		else if (P2.XPos > S.ScroolX + SCREEN_W - mDist){
			ScXPoint = P2.XPos + mDist - SCREEN_W;
		}
	}

	// �L�����Ԃ̋������[�X���傫�����A�ω��Ȃ�
	if (PDist >= SCREEN_W - (GAMENHAJI * 2)){}	// 640 - 70 = 570
	// ��≓�����A���������ɒu��
	else if (PDist >= SCREEN_W - mSDist){ 
		ScXPoint = leftP - (SCREEN_W / 2) + (PDist / 2); // leftP - 360 + PDist
	}
	// �߂�
	else {}

	/*
	// 1P
	if (P1.XPos < S.ScroolX + mDist){
		ScXPoint = P1.XPos - mDist;
	}
	else if (P1.XPos > S.ScroolX + SCREEN_W - mDist){
		ScXPoint = P1.XPos + mDist - SCREEN_W;
	}
	// 2P
	if (P2.XPos < S.ScroolX + mDist){
		ScXPoint = P2.XPos - mDist;
	}
	else if (P2.XPos > S.ScroolX + SCREEN_W - mDist){
		ScXPoint = P2.XPos + mDist - SCREEN_W;
	}

	// ���������ꂷ������A�X�N���[�����Ȃ�
	if (PDist >= SCREEN_W - (mDist * 2))ScXPoint = S.ScroolX;
	*/

	//--------------------
	//
	// Y���X�N���[��
	//
	//--------------------
	// Y�����ɁA��ʂ� 3/4 �ȏ�i�񂾂�
	if ((P1.YPos < (S.ScroolY + (SCREEN_H / 2.2))) ||
		(P2.YPos < (S.ScroolY + (SCREEN_H / 2.2)))){
		if (P1.YPos < P2.YPos){
			//ScYPoint -= P1_ScY;		// ��ʂ����
			ScYPoint = P1.YPos - 210;
		}
		else if (P1.YPos > P2.YPos){
			ScYPoint = P2.YPos - 210;
		}
		else{ ScYPoint = P1.YPos - 210; }
	}
	else {
		if (P1.YPos < P2.YPos){
			//ScYPoint += P1_ScY;		// ��
			ScYPoint = P1.YPos - 210;
		}
		else if (P1.YPos > P2.YPos){
			ScYPoint = P2.YPos - 210;
		}
		else{ ScYPoint = P1.YPos - 210; }
	}



	//--------------------
	// �C��
	//--------------------
	// �X�N���[���ʉ��Z(noScrool���Ȃ����)
	if (!S.noScroolX){
		// �X�N���[���������Ȃ�
		if (S.ScroolX - ScXPoint > 100)S.ScroolX -= 100;
		else if (ScXPoint - S.ScroolX > 100)S.ScroolX += 100;
		else{ S.ScroolX = ScXPoint; }	// �ʏ�
	}
	if (!S.noScroolY){
		S.ScroolY = ScYPoint;
	}

	// �X�N���[���X�e�[�W�𒴂��Ă�����߂�
	if ((STAGE_WIDTH - SCREEN_W) < (S.ScroolX)){
		S.ScroolX = STAGE_WIDTH - SCREEN_W;
	}
	else if (S.ScroolX < 0){
		S.ScroolX = 0;
	}
	// Y
	if ((STAGE_HEIGHT - SCREEN_H) < (S.ScroolY)){
		S.ScroolY = STAGE_HEIGHT - SCREEN_H;
	}
	else if (S.ScroolY < 0){
		S.ScroolY = 0;
	}
	
	// ���ɔ���ScX�����̈ʒu��
	ScXPoint = S.ScroolX;

	// S��Ԃ�
	Get_SSet(S);

	return 0;
}

// �L�����̐ڐG�`�F�b�N
int TouchCheck()
{
	// �ق�
	if (S.roundState != 0){
	Player P[2];
	P[0] = P1, P[1] = P2;

	for (int i = 0; i < 2; i++){

		//X�� �[����
		if (P[i].XPos <= (S.ScroolX + GAMENHAJI)){
			P[i].XPos = S.ScroolX + GAMENHAJI;
		}
		else if (P[i].XPos >= (S.ScroolX + SCREEN_W - GAMENHAJI)){
			P[i].XPos = S.ScroolX + (SCREEN_W - GAMENHAJI);
		}

	}
	// �߂��Ă���
	P1 = P[0], P2 = P[1];

	double m_X[2];
	m_X[0] = P1.XPos - m_oldXPos[0];
	m_X[1] = P2.XPos - m_oldXPos[1];
	TH_Set(P1, P2);

	//�����蔻��`�F�b�N�A�R�s�y�� > �̕������C�R�[���ɂ���
	if ((((P1.X_UL >= P2.X_UL && P1.X_UL <= P2.X_UL + P2.xSize) ||
		(P2.X_UL >= P1.X_UL && P2.X_UL <= P1.X_UL + P1.xSize)) &&
		((P1.Y_UL >= P2.Y_UL && P1.Y_UL <= P2.Y_UL + P2.ySize) ||
		(P2.Y_UL >= P1.Y_UL && P2.Y_UL <= P1.Y_UL + P1.ySize))))
	{
		// ��������X�^�[�g
		// �܂��͗��������𑪂��Ă݂悤
		Distance = (double)((P1.xSize / 2.0) + (P2.xSize / 2.0));

		// ���� Distance ���g���ė����ʂ𑪂�
		// 1P��2P��荶 �̂Ƃ�
		if (P1.XPos < P2.XPos){
			DisPoint = Distance - (P2.XPos - P1.XPos);
			// ����   2P��1P��荶 �̂Ƃ�
		}
		else if (P1.XPos > P2.XPos){
			DisPoint = Distance - (P1.XPos - P2.XPos);
			// �Ō�� X�̋��������� �Ƃ�
		}
		else if (P1.XPos == P2.XPos){
			DisPoint = Distance;
		}

		double a, a2;

		//------------
		// ����������
		//------------
		if (P1.YPos == P2.YPos){
			
			// �������Ă��� ���x ���`�F�b�N
			// 1P �̂�����
			if (P1.XVel > P2.XVel){
				if (P1.XPos <= P2.XPos){
					P1.XPos -= (DisPoint / 2), P2.XPos += (DisPoint / 2);
					a = P1.XPos + m_X[0] / 2;
					a2 = P2.XPos + m_X[1] / 2;
					if (a <= P1.XPos)P1.XPos = a;
					if (a2 >= P2.XPos)P2.XPos = a2;
				}
				else{
					P1.XPos += (DisPoint / 2), P2.XPos -= (DisPoint / 2);
					a = P1.XPos + m_X[0] / 2;
					a2 = P2.XPos + m_X[1] / 2;
					if (a >= P1.XPos)P1.XPos = a;
					if (a2 <= P2.XPos)P2.XPos = a2;
				}
			}
			// 2P
			else if (P1.XVel < P2.XVel){
				if (P2.XPos <= P1.XPos){
					P1.XPos += (DisPoint / 2), P2.XPos -= (DisPoint / 2);
					a = P1.XPos + m_X[0] / 2;
					a2 = P2.XPos + m_X[1] / 2;
					if (a >= P1.XPos)P1.XPos = a;
					if (a2 <= P2.XPos)P2.XPos = a2;
				}
				else{
					P1.XPos -= (DisPoint / 2), P2.XPos += (DisPoint / 2);
					a = P1.XPos + m_X[0] / 2;
					a2 = P2.XPos + m_X[1] / 2;
					if (a <= P1.XPos)P1.XPos = a;
					if (a2 >= P2.XPos)P2.XPos = a2;
				}
			}
			// ��������
			else if (P1.XVel == P2.XVel){
				if (P1.XPos <= P2.XPos){
					P1.XPos -= (DisPoint / 2), P2.XPos += (DisPoint / 2);
				}
				else{
					P1.XPos += (DisPoint / 2), P2.XPos -= (DisPoint / 2);
				}
			}

			
			// X���W�� ����
			if (P1.XPos == P2.XPos){
				// �ǂ����������[
				if ((P1.XPos <= GAMENHAJI) || (P2.XPos <= GAMENHAJI)){
					// 1P���E����
					if (P1.muki == 0){
						P2.XPos += DisPoint;
					}
					// 2P���E����
					else if (P2.muki == 0){
						P1.XPos += DisPoint;
					}
				}
				// �ǂ��������E�[
				else if ((P1.XPos >= (STAGE_WIDTH - GAMENHAJI)) ||
					(P2.XPos >= (STAGE_WIDTH - GAMENHAJI))){

					// 1P���E����
					if (P1.muki == 1){
						P2.XPos -= DisPoint;
					}
					// 2P���E����
					else if (P2.muki == 1){
						P1.XPos -= DisPoint;
					}
				}
			}// X���W�������ꍇ 

		}// ���������̒����I��

		/**
		* �������Ⴄ�ꍇ
		**/
		else if (P1.YPos != P2.YPos){
			
			// ��ʒ[
			// 1P ���[
			if (P1.XPos <= GAMENHAJI){
			P1.XPos = GAMENHAJI;
			}
			// 2P ���[
			else if (P2.XPos <= GAMENHAJI){
			P2.XPos = GAMENHAJI;
			}
			// 1P �E�[
			if (P1.XPos >= (STAGE_WIDTH - GAMENHAJI)){
			P1.XPos = (STAGE_WIDTH - GAMENHAJI);
			}
			// 2P �E�[
			else if (P2.XPos >= (STAGE_WIDTH - GAMENHAJI)){
			P2.XPos = (STAGE_WIDTH - GAMENHAJI);
			}
			

			// 1P ��
			if (P1.XPos < P2.XPos){
				P1.XPos -= (DisPoint / 2), P2.XPos += (DisPoint / 2);
			}
			// 1P �E
			if (P1.XPos > P2.XPos){
				P1.XPos += (DisPoint / 2), P2.XPos -= (DisPoint / 2);
			}

			// X���W�� ����
			if (P1.XPos == P2.XPos){
				// �ǂ����������[
				if ((P1.XPos <= GAMENHAJI) || (P2.XPos <= GAMENHAJI)){
					// 1P���E����
					if (P1.muki == 0){
						P2.XPos += DisPoint;
					}
					// 2P���E����
					else if (P2.muki == 0){
						P1.XPos += DisPoint;
					}
				}
				// �ǂ��������E�[
				else if ((P1.XPos >= (STAGE_WIDTH - GAMENHAJI)) ||
					(P2.XPos >= (STAGE_WIDTH - GAMENHAJI))){

					// 1P���E����
					if (P1.muki == 1){
						P2.XPos -= DisPoint;
					}
					// 2P���E����
					else if (P2.muki == 1){
						P1.XPos -= DisPoint;
					}
				}
			}// X���W�������ꍇ 
		}// �������Ⴄ�ꍇ 
	}//if�I��

	// �������l�𑗂�
	Get_PSet(P1, P2);

	} // �ق��I��
	// �I��
	return 0;
}


void PosHosei()
{
	// �ق�
	if (S.roundState != 0){

	// ���W�L�^
	m_oldXPos[0] = P1.XPos, m_oldXPos[1] = P2.XPos;
	m_oldYPos[0] = P1.YPos, m_oldYPos[1] = P2.YPos;
	
	Player P[2];
	P[0] = P1, P[1] = P2;

	for (int i = 0; i < 2; i++){

		//X�� �[����
		if (P[i].XPos <= (S.ScroolX + GAMENHAJI)){
			P[i].XPos = (double)S.ScroolX + GAMENHAJI;
		}
		else if (P[i].XPos >= (S.ScroolX + SCREEN_W - GAMENHAJI)){
			P[i].XPos = (double)S.ScroolX + (SCREEN_W - GAMENHAJI);
		}
	
	}
	// �߂��Ă���
	P1 = P[0], P2 = P[1];

	
	TH_Set(P1, P2);
	//�����蔻��`�F�b�N�A�R�s�y�� > �̕������C�R�[���ɂ���
	if (((P1.X_UL >= P2.X_UL && P1.X_UL <= P2.X_UL + P2.xSize) ||
		(P2.X_UL >= P1.X_UL && P2.X_UL <= P1.X_UL + P1.xSize)) &&
		((P1.Y_UL >= P2.Y_UL && P1.Y_UL <= P2.Y_UL + P2.ySize) ||
		(P2.Y_UL >= P1.Y_UL && P2.Y_UL <= P1.Y_UL + P1.ySize)))
	{
		// �C������Ȃ�
		{
			// �ߋ��̍��W����A���炷�ʂ��v�Z
			xp[0] = P1.XPos - m_oldXPos[0];
			xp[1] = P2.XPos - m_oldXPos[1];
			yp[0] = P1.YPos - m_oldYPos[0];
			yp[1] = P2.YPos - m_oldYPos[1];

			// ���� Distance ���g���ė����ʂ𑪂�
			// 1P��2P��荶 �̂Ƃ�
			if (P1.XPos < P2.XPos){
				DisPoint = Distance - (P2.XPos - P1.XPos);
				// ����   2P��1P��荶 �̂Ƃ�
			}
			else if (P1.XPos > P2.XPos){
				DisPoint = Distance - (P1.XPos - P2.XPos);
				// �Ō�� X�̋��������� �Ƃ�
			}
			else if (P1.XPos == P2.XPos){
				DisPoint = Distance;
			}

			//------------
			// ����������
			//------------
			if (P1.YPos == P2.YPos){

				// 1P ���[
				if (P1.XPos <= GAMENHAJI){
					P2.XPos += DisPoint;
				}
				// 1P �E�[
				else if (P1.XPos >= (STAGE_WIDTH - GAMENHAJI)){
					P2.XPos -= DisPoint;
				}
				// 2P ���[
				if (P2.XPos <= GAMENHAJI){
					P1.XPos += DisPoint;
				}
				// 2P �E�[
				else if (P2.XPos >= (STAGE_WIDTH - GAMENHAJI)){
					P1.XPos -= DisPoint;
				}

			}// ���������̒����I��
			//------------------
			// �������Ⴄ�ꍇ
			//------------------
			else if (P1.YPos != P2.YPos){
				// ��ʒ[
				// 1P ���[
				if (P1.XPos <= (double)GAMENHAJI){
					P1.XPos = (double)GAMENHAJI;
				}
				// 2P ���[
				else if (P2.XPos <= (double)GAMENHAJI){
					P2.XPos = (double)GAMENHAJI;
				}
				// 1P �E�[
				if (P1.XPos >= (double)(STAGE_WIDTH - GAMENHAJI)){
					P1.XPos = (double)(STAGE_WIDTH - GAMENHAJI);
				}
				// 2P �E�[
				else if (P2.XPos >= (double)(STAGE_WIDTH - GAMENHAJI)){
					P2.XPos = (double)(STAGE_WIDTH - GAMENHAJI);
				}
				/*
					// 1P��
					if (P1.YPos > P2.YPos){
						// 1P���E����
						if (P1.XPos <= GAMENHAJI){
							P2.XPos += DisPoint;
						}
						else if (P1.XPos >= (STAGE_WIDTH - GAMENHAJI)){
							P2.XPos -= DisPoint;
						}
					}
					// 2P��
					else if (P2.YPos > P1.YPos){
						// 1P��������
						if (P2.XPos <= GAMENHAJI){
							P1.XPos += DisPoint;
						}
						else if (P2.XPos >= (STAGE_WIDTH - GAMENHAJI)){
							P1.XPos -= DisPoint;
						}
					}
					*/
			}// �������Ⴄ�ꍇ 
			
		}
	}
	

	// �������l�𑗂�
	Get_PSet(P1, P2);

	}	// roundstate �I��
}

void VelPosSet()
{
	// �������L��
	// �������ς���Ă��P�t�������͑��x���ς��Ȃ�
	OldMuki[0] = P1.muki;
	OldMuki[1] = P2.muki;

	// �ړ��O�̍��W�L�^
	m_oldXPos[0] = P1.XPos, m_oldXPos[1] = P2.XPos;
	m_oldYPos[0] = P1.YPos, m_oldYPos[1] = P2.YPos;

	Player P[2];
	P[0] = P1, P[1] = P2;
	for (int i = 0; i < 2; i++){
		// ���W�̈ʒu�ɑ��x�𑫂�
		// 1P
		if (P[i].StopTime == 0){

			if (P[i].muki == 0){
				if (P[i].muki == OldMuki[i]){
					P[i].XPos += P[i].XVel * 2;
					P[i].XPos += P[i].A.hbVelX * 2;
					P[i].XPos += P[i].D.addVelX * 2;
				}
				else { 
					P[i].XPos -= P[i].XVel * 2; 
					P[i].XPos -= P[i].A.hbVelX * 2;
					P[i].XPos -= P[i].D.addVelX * 2;
				}
			}
			// �E��
			else if(P[i].muki == 1){
				if (P[i].muki == OldMuki[i]){
					P[i].XPos -= P[i].XVel * 2;
					P[i].XPos -= P[i].A.hbVelX * 2;
					P[i].XPos -= P[i].D.addVelX * 2;
				}
				else { 
					P[i].XPos += P[i].XVel * 2; 
					P[i].XPos += P[i].A.hbVelX * 2;
					P[i].XPos += P[i].D.addVelX * 2;
				}
			}
			//==============
			// Y ���x����
			//==============

			// �d�͖�������Ȃ�
			if (!P[i].ignoreG){

				// �K�|�hor��炢�ȊO
				if (P[i].stateno < 1000){
					//&& P[i].time > 0
					// ��
					if (P[i].SFlg == 2){
						// �K�[�h
						if (P[i].stateno >= 50 && P[i].stateno <= 59 && P[i].time > 0){
							P[i].YVel += GRAVITY;
						}
						// �ʏ펞�͌ʂ̏d�͂�������
						else{ 
							// ��_���ĂȂ�
							if (!P[i].airDashTime)
							P[i].YVel += P[i].C.yAccel; 
						}
					}
				}

				// ��炢
				if (P[i].stateno >= 1000 && P[i].time > 0){
					if (P[i].SFlg == 2){
						// �󂯐g�@�ǃo�E���h��
						if (P[i].stateno == 1085){
							P[i].YVel += GRAVITY;
						}
						// �ʏ��炢
						else{
							P[i].YVel += P[i].D.yaccel;
						}
					}
				}

			}// ignoreG�I��

			// Y���x�� �ړ�
			P[i].YPos += (P[i].YVel * 2);
			P[i].YPos += (P[i].A.hbVelY * 2);
		}


	}
	P1 = P[0], P2 = P[1];

	// �������L��
	// �������ς���Ă��P�t�������͑��x���ς��Ȃ�
	//OldMuki[0] = P1.muki;
	//OldMuki[1] = P2.muki;

	// �������l�𑗂�
	Get_PSSet(P1, P2, S);
}

void VelXYAdd()
{
	Player P[2];
	P[0] = P1, P[1] = P2;
	
	for (int i = 0; i < 2; i++){
		if (P[i].StopTime == 0){

			//==============
			// X ���x����
			//==============
			
			// �͂₷����
			//if (P[i].XVel > 25)P[i].XVel = 25;
			//else if (P[i].XVel < -25)P[i].XVel = -25;
			P[i].XVel = max(-25, min(25, P[i].XVel));
			P[i].A.hbVelX = max(-25, min(25, P[i].A.hbVelX));
			if (P[i].SFlg != 2){
				if (P[i].XVel > 0){
					//P[i].XVel -= 0.5;
					P[i].XVel = P[i].XVel * 0.82;	// 0.82�͖��C
				}
				else if (P[i].XVel < 0){
					P[i].XVel = P[i].XVel * 0.82;
				}
				// ��炢or�K�[�h�r���ɉ�ʒ[�ɓ��B
				// �e�X�g����
				if (((P[i].XPos <= GAMENHAJI) || (P[i].XPos >= STAGE_WIDTH - GAMENHAJI)) && (P[i].XVel < 0) && 
					((P[i].stateno >= 50 && P[i].stateno < 60) || (P1.stateno == 1000)	// �K�[�hor�n���炢
					|| (P[i].stateno == 1010) || (P[i].stateno == 1020))){
					if (i == 0){
						//if(P[1].A.hbVelX != 0.0)
						P[1].A.hbVelX = P[i].XVel;
					}
					else{ 
						//if (P[].A.hbVelX != 0.0)
						P[0].A.hbVelX = P[i].XVel;
					}
					P[i].XVel = 0;
				}

				// Vel�����Ȃ��ꍇ�A0�ɂ���(MUGEN��1)
				if (P[i].XVel < 1 && P[i].XVel > -1)P[i].XVel = 0;	// ��肷�����ꍇ
				
			}
			
			// �q�b�g�o�b�N
			if (P[i].A.hbVelX > 0){
				P[i].A.hbVelX = P[i].A.hbVelX * 0.82;	// 0.82�͖��C
			}
			else if (P[i].A.hbVelX < 0){
				P[i].A.hbVelX = P[i].A.hbVelX * 0.82;
			}
			if ((P[i].A.hbVelX < 1) && (P[i].A.hbVelX > -1))P[i].A.hbVelX = 0;	// ��肷�����ꍇ

			// �A�h�K�o�b�N
			if (P[i].D.addVelX > 0){
				P[i].D.addVelX = P[i].D.addVelX * 0.82;	// 0.82�͖��C
			}
			else if (P[i].D.addVelX < 0){
				P[i].D.addVelX = P[i].D.addVelX * 0.82;
			}
			if ((P[i].D.addVelX < 1) && (P[i].D.addVelX > -1))P[i].D.addVelX = 0;	// ��肷�����ꍇ

			//==============
			// Y ���x����
			//==============
			// �q�b�g�o�b�N
			if (P[i].A.hbVelY > 0){
				P[i].A.hbVelY = P[i].A.hbVelY * 0.82;	// 0.82�͖��C
			}
			else if (P[i].A.hbVelY < 0){
				P[i].A.hbVelY = P[i].A.hbVelY * 0.82;
			}
			if ((P[i].A.hbVelY < 1) && (P[i].A.hbVelY > -1))P[i].A.hbVelY = 0;	// ��肷�����ꍇ

			// �d�͂������āA�����⒅�n������
			// ignoreG ������Ƃ��́A�󒆂ɂ��Ă�������Ȃ�
			/*
			// �d�͖�������Ȃ�
			if (!P[i].ignoreG){

				// �K�|�hor��炢 �ł͂Ȃ�
				if (P[i].stateno < 1000 && P[i].HFlg == 0){
					//&& P[i].time > 0
					// 1P �n�ʂ�艺�Œ��n�X�e�[�g��
					if (P[i].YPos > GROUND){
						P[i].YVel = 0, P[i].YPos = GROUND;
						P[i].stateno = 47, P[i].time = 0;
					}
					
					// ��
					if (P[i].SFlg == 2){
						// �ړ����͌ʂ̏d�͂�������
						P[i].YVel += P[i].C.yAccel;
					}
				}

				// �K�[�hor��炢
				if (P[i].stateno >= 1000 && P[i].time > 0){
					if (P[i].SFlg == 2){
						// ��炢���͏d������
						P[i].YVel += GRAVITY;
					}
				}

			}// ignoreG�I��
			*/
			// �͂₷����
			if (P[i].YVel > 18)P[i].YVel = 18;
			// �d�͖�������Ȃ�
			if (!P[i].ignoreG){

				// �K�|�hor��炢 �ł͂Ȃ� ���x����
				if (P[i].stateno < 1000 && P[i].HFlg == 0 && P[i].YVel > 0){
					//&& P[i].time > 0
					// 1P �n�ʂ�艺�Œ��n�X�e�[�g��
					if (P[i].YPos > GROUND){
						P[i].YVel = 0, P[i].YPos = GROUND;
						P[i].stateno = 47, P[i].time = 0;
					}

				}

			}// ignoreG�I��

		}// �����I��
	}// ���[�v
	
	P1 = P[0], P2 = P[1];
	
	// �������l�𑗂�
	Get_PSet(P1, P2);
}

void GetP_SubSystem(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_SubSystem(System_t GS)
{
	S = GS;
}
