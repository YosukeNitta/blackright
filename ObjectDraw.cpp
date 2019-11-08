//�C���N���[�h��--------------------------------------------
#include "pch.h"

#define LOG_MAX		(10)

using namespace std;

// �A�i�E���X
static double m_size;	// �摜�̊g��

static int PowerBar[2];		// ���C�t�o�[
static int PowerPoint;	// ��ʂɕ\������̗�
static int PowerNum[10];// �摜 [�p���[�Q�[�W 0�`9]

static int Hit;			// �摜�uHIT�v
static int HitNum[10];	// �摜�u�q�b�g�� 0�`9�v
static int HitGraph, HitGraph2;	// �q�b�g���̎��ۂ̉摜(2�� 2�P�^��)
static boolean beat[2];	// �A���q�b�g��(false�Őr)

static int hitCount[2];		// �q�b�gor�K�[�h��
static int hDrawTime[2];	// �\�����鎞��
static int hHitCount[2];	// ���O�̃q�b�g��

static int Win1, Win2;	// �v���C���[WIN �̕\��
static int battle[5];	// �摜�@FIGHT
static int EX;

static int Load1;
static int TText[5];
static char TChar[TX_MAX];

//�_���[�W�v�Z
static int m_AllDamage;
static int m_DisplayDamage;
//��炤�O�̗̑�
static int m_P1_SLife, m_P2_SLife;
//��������̗̑�
static int m_P1_ALife, m_P2_ALife;

// �Q�[�W���
static int mGauge = 0;
static int mG_startPower = 0;
static int mG_Start = 0;
//�����t���[��
static int mHassei = 0;
static int aSpeed;	// �U���t���[��
// �t���[���A�J�n�t���[���H
static int mFrame = 0;
static int mF_Start = 0;
//
static float m_cPos;


// �L�[���O�\��
static int mKey[9];
static int mButton[4];
static int gKey[10];
static int gButton[4];

//static int oKey[LOG_MAX];
static vector<int> oKey;
//static int oButton[LOG_MAX][4];
static vector<int> oButton[LOG_MAX];

static boolean sButton, sKey;
static int saveKey = -1;
static int test, test2[2];	// ����Q�[�W
static int test3[2];
// Add
static int addGaugeTime[2];		// �\������
static int addGauge[10];		// �摜
static int addGaugePower[2];	// �Q�[�W��
/*
* ������g���Đ��l��S�ēn��
*/
static int mXA[2][HIT_MAX], mYA[2][HIT_MAX], mWA[2][HIT_MAX], mHA[2][HIT_MAX];
static int mHXA[2][HM], mHYA[2][HM], mHWA[2][HM], mHHA[2][HM];

// ��炢����
static int m_XD[HIT_MAX], m_YD[HIT_MAX], m_WD[HIT_MAX], m_HD[HIT_MAX];
static int mHXH[2][HM], mHYH[2][HM], mHWH[2][HM], mHHH[2][HM];

// �L�[���O�\��
static int m_KeyPos, m_Key;
static int m_Button[4];

// ��ʂ̖��邳
static int blackOut;

void BoxCheck();	// ����`�F�b�N
void BoxKurai();	// ��炢����\��
void BoXAtt();		// �U������\��


class Test {

public:
	int x, y;	// �摜�T�C�Y

	Test::Test()
	{
		x = 11;
		y = 22;
	}
};

static Test tes;

void ObjectDraw_KeyDisplay();

//���̑��̕`��
void ObjectDraw()
{

	// �摜
	if (Load1 == 0){
		//�p���[�o�[�̉摜
		LoadDivGraph("ob/PowerBar.png", 2, 1, 2, 241, 36, PowerBar);
		PowerPoint = LoadGraph("ob/PowerPoint.bmp");	// �Q�[�W�̃h�b�g
		LoadDivGraph("ob/number.png", 10, 5, 2, 25, 30, PowerNum);	// �Q�[�W�{���̐���

		Hit = LoadGraph("ob/hit.png");	// Hit �̕���
		LoadDivGraph("ob/hitnum.png", 10, 5, 2, 36, 56, HitNum);	// �q�b�g��

		LoadDivGraph("ob/key.png", 10, 5, 2, 20, 20, gKey);
		LoadDivGraph("ob/button.png", 4, 4, 1, 20, 20, gButton);

		Win1 = LoadGraph("ob/1P.png");	// 1P
		Win2 = LoadGraph("ob/2P.png");	// 2P

		test = LoadGraph("char/bouncer/o1.bmp");
		test2[0] = LoadGraph("char/helios/o1.bmp");
		test2[1] = LoadGraph("char/helios/o2.bmp");
		test3[0] = LoadGraph("char/dan/gauge.bmp");
		test3[1] = LoadGraph("char/dan//gauge2.bmp");
		EX = LoadGraph("ob/EX.png");

		LoadDivGraph("ob/battle.png", 5, 1, 5, 280, 70, battle);

		// �L�[���
		m_Key = LoadGraph("ob/r_Key.png");
		m_KeyPos = LoadGraph("ob/r_KeyPos.png");
		LoadDivGraph("ob/r_Button.png", 4, 4, 1, 20, 20, m_Button);

		LoadDivGraph("ob/num2.png", 10, 5, 2, 12, 20, addGauge);

		// ����
		//
		for (int i = 0; i < LOG_MAX; i++){
			oKey.push_back(-1);
		}

		for (int a = 0; a < LOG_MAX; a++){
			for (int i = 0; i < 4; i++){
				oButton[a].push_back(0);
			}
		}
		blackOut = 0;

		Load1 = 1;
	}
		
		/**************
		* �q�b�g��
		**************/
		Player P[2];
		int hDrawPos[2];
		hDrawPos[0] = 5;
		hDrawPos[1] = (SCREEN_W - 640) + 320 + 122;
		hitCount[0] = 0, hitCount[1] = 0;
		P[0] = P1, P[1] = P2;


		for (int i = 0; i < 2; i++) {
			// ��������ւ�
			if (i == 1) {
				P[0] = P2, P[1] = P1;
			}

			// �q�b�g��
			if (P[0].HitCount > 1) {
				HitGraph = HitNum[P[0].HitCount % 10];
				HitGraph2 = HitNum[P[0].HitCount / 10];
				hitCount[0] = P[0].HitCount;
			}
			// �K�[�h��
			else if (P[0].A.guardCount > 1) {
				HitGraph = HitNum[P[0].A.guardCount % 10];
				HitGraph2 = HitNum[P[0].A.guardCount / 10];
				hitCount[0] = P[0].A.guardCount;
			}

			// �K�[�h��

			// 1P�� �q�b�g���\��
			// ��炢or�A���K�[�h
			if ((P[1].HFlg) || (P[0].A.guardCount > 1)) {
				// 2�q�b�g�ȏ�ŁA 1�P�^�ڕ\��
				if (hitCount[0] >= 2) {
					// 99�q�b�g�ȓ�
					if (hitCount[0] <= 99) {
						// �q�b�g�����Ȃ��Ȃ�
						if (P[0].HitCount == 0) {
							// �K�[�h���\����
							SetDrawBright(128, 128, 128);
						}
						DrawGraph(hDrawPos[i] + 80, 90 + 5, Hit, true);

						//�r�̏ꍇ
						if (!beat[i]) {
							SetDrawBright(255, 30, 30);
						}
						else { SetDrawBright(30, 30, 255); }
						
						DrawGraph(hDrawPos[i] + 39, 90, HitGraph, true);
						
						// �q�b�g���� 2�P�^�ȏ�̎��́A2�P�^�ڕ\��
						if (hitCount[0] >= 10) {
							DrawGraph(hDrawPos[i] + 8, 90, HitGraph2, true);
						}

						SetDrawBright(255, 255, 255);	// �F���Z�b�g
					}
					// 99�q�b�g�ŃJ���X�g
					else if (hitCount[0] >= 100) {
						DrawGraph(hDrawPos[i], 90 + 5, Hit, true);
						DrawGraph(hDrawPos[i] + 39, 90, HitNum[9], true);
						DrawGraph(hDrawPos[i] + 8, 90, HitNum[9], true);
					}
				}
			}
		}


		/**************
		* ����Q�[�W
		**************/
		if (!(S.roundState == 0 && S.roundTime == 0) && (S.noGauge == 0)){
			/*
			// EX
			if (P1.Var[30] != 0)SetAlpha(68);
			DrawGraph(0 + 2 + 30, 62 + 2, EX, true);
			NoBlend();
			SetDrawBright(255, 255, 255);

			if (P2.Var[30] != 0)SetAlpha(68);
			DrawGraph(SCREEN_W - 2 - 24 - 30, 62 + 2, EX, true);
			NoBlend();
			SetDrawBright(255, 255, 255);
			*/

			// �o�E���T�[
			if (P1.Name == BOUNCER){
				if (P1.D.armor <= 0)SetAlpha(68);
				if (P1.D.armor > 2)SetDrawBright(255, 10, 10);
				else if (P1.D.armor > 1)SetDrawBright(255, 128, 128);

				DrawGraph(0 + 2, 62 + 2, test, true);
				NoBlend();
				SetDrawBright(255, 255, 255);
				if (P1.Var[13] > 0){
					DrawFormatString(80, (SCREEN_H - 480) + 430, Oran, "%d", P1.Var[13] / 60);
				}
			}
			if (P2.Name == BOUNCER){
				if (P2.D.armor <= 0)SetAlpha(68);
				if (P2.D.armor > 2)SetDrawBright(255, 10, 10);
				else if (P2.D.armor > 1)SetDrawBright(255, 128, 128);

				DrawGraph(SCREEN_W - 2 - 26, 62 + 2, test, true);
				NoBlend();
				SetDrawBright(255, 255, 255);
				if (P2.Var[13] > 0){
					DrawFormatString(SCREEN_W - 80 - 10, (SCREEN_H - 480) + 430, Oran, "%d", P2.Var[13] / 60);
				}
			}

			// �w���I�X
			if (P1.Name == HELIOS){
				if (P1.Var[11] >= 3000){
					DrawFormatString(80, (SCREEN_H - 480) + 430, Oran, "%d", P1.Var[11] / 1000);
				}
				else if (P1.Var[11] >= 2000){
					DrawFormatString(80, (SCREEN_H - 480) + 430, GetColor(255, 255, 0), "%d", P1.Var[11] / 1000);
				}
				else if(P1.Var[11] >= 1000){
					DrawFormatString(80, (SCREEN_H - 480) + 430, Cr, "%d", P1.Var[11] / 1000);
				}
				else{
					DrawFormatString(80, (SCREEN_H - 480) + 430, GetColor(128, 128, 128), "%d", P1.Var[11] / 1000);
				}

				// �Q�[�W
				DrawBox(100, (SCREEN_H - 480) + 435, 100 + (P1.Var[11] * 0.05), (SCREEN_H - 480) + 445, GetColor(255, 215, 0), true);
				// �Q�[�W�g
				for (int i = 1; i < 4; i++){
					DrawBox(100 + (((i - 1) * 1000) * 0.05), (SCREEN_H - 480) + 435, 
						100 + ((i * 1000) * 0.05), (SCREEN_H - 480) + 445, GetColor(20, 20, 20), false);
				}
			}
			if (P2.Name == HELIOS){

				if (P2.Var[11] >= 3000){
					DrawFormatString(SCREEN_W - 80 - 10, (SCREEN_H - 480) + 430, Oran, "%d", P2.Var[11] / 1000);
				}
				else if (P2.Var[11] >= 2000){
					DrawFormatString(SCREEN_W - 80 - 10, (SCREEN_H - 480) + 430, GetColor(255, 255, 0), "%d", P2.Var[11] / 1000);
				}
				else if (P2.Var[11] >= 1000){
					DrawFormatString(SCREEN_W - 80 - 10, (SCREEN_H - 480) + 430, Cr, "%d", P2.Var[11] / 1000);
				}
				else{
					DrawFormatString(SCREEN_W - 80 - 10, (SCREEN_H - 480) + 430, GetColor(128, 128, 128), "%d", P2.Var[11] / 1000);
				}
				DrawBox(SCREEN_W - 80 - 20 - (P2.Var[11] * 0.05), (SCREEN_H - 480) + 435, 
					SCREEN_W - 80 - 20, (SCREEN_H - 480) + 445, GetColor(255, 215, 0), true);
				for (int i = 1; i < 4; i++){
					DrawBox(SCREEN_W - 80 - 20 - ((i * 1000) * 0.05), (SCREEN_H - 480) + 435,
						SCREEN_W - 80 - 20 - (((i - 1) * 1000) * 0.05), (SCREEN_H - 480) + 445, GetColor(0, 0, 0), false);
				}
			}

			// �_��
			if (P1.Name == HYDE) {
				// �Q�[�W�g
				DrawBox(99, (SCREEN_H - 480) + 434,
					99 + 101, (SCREEN_H - 480) + 456, GetColor(20, 20, 20), false);
				// �Q�[�W
				DrawBox(100, (SCREEN_H - 480) + 435, 100 + (P1.Var[11] * 0.05), (SCREEN_H - 480) + 455, GetColor(255, 215, 0), true);
			}
			if (P2.Name == HYDE) {
				DrawBox(SCREEN_W - 80 - 19, (SCREEN_H - 480) + 434,
					SCREEN_W - 80 - 19 - 101, (SCREEN_H - 480) + 456, GetColor(0, 0, 0), false);
				DrawBox(SCREEN_W - 80 - 20 - (P2.Var[11] * 0.05), (SCREEN_H - 480) + 435,
					SCREEN_W - 80 - 20, (SCREEN_H - 480) + 455, GetColor(255, 215, 0), true);
			}
		}
		/**************
		* ����m�F �i�g��������j
		**************/
		if (S.TraningFlg){
			BoxCheck();
		}

		//--------------
		// ���v�_���[�W
		//--------------
		if (P2.HFlg == 0){
			m_P2_SLife = P2.Life;
			m_AllDamage = 0;
		}
		if (P2.HFlg){
			m_AllDamage = m_P2_SLife - P2.Life;
			m_DisplayDamage = m_AllDamage;
		}

		/**************
		* �p���[�Q�[�W
		**************/
		if (!(S.roundState == 0 && S.roundTime == 0) && (S.noGauge == 0)){
			//--------------
			// �������Q�[�W
			//--------------
			// �摜�T�C�Y�擾 P1,2
			int bar_x, bar_y;
			GetGraphSize(PowerBar[0], &bar_x, &bar_y);
			
			//--------------
			// �������Q�[�W
			//--------------

			SetAlpha(158);
			
			// �p���[�o�[���@�����\��
			DrawGraph(20, 435 + (SCREEN_H - 480), PowerBar[1], true);
			DrawTurnGraph(SCREEN_W - 20 - bar_x, 435 + (SCREEN_H - 480), PowerBar[1], true);

			// ����������
			NoBlend();

			// �Q�[�W1000�ȏ�
			{
				SetDrawBright(128,100,0);
				//DrawGraph(20, 435, PowerBar[1], true);

				if (P1.Power >= 1000 && P1.Power < 3000){
					DrawRotaGraph3F(320 - (1000 / 5) - 62, 454 + (SCREEN_H - 480),
						0, 0, (1000 / 5), 1, 0, PowerPoint, true, false);
				}
				if (P2.Power >= 1000 && P2.Power < 3000){
					DrawRotaGraph3F(SCREEN_W - 320 + 62, 454 + (SCREEN_H - 480),
						0, 0, (1000 / 5), 1, 0, PowerPoint, true, false);
				}

				SetDrawBright(255, 255, 255);
			}


			//�p���[�ʁ@1P
			//for (int i = 0; i < P1.Power % 1000; i++){
				//DrawGraph((SCREEN_W / 2) - 62 - (i / 5), 454, PowerPoint, true);
			//}
			
			// �Q�[�WMAX��
			SetDrawBright(255, 180, 0);

			if (P1.Power == POWER_MAX){
				DrawRotaGraph3F(320 - (1000 / 5) - 62, 454 + (SCREEN_H - 480),
					0, 0, (1000 / 5), 1, 0, PowerPoint, true, false);
			}
			else{
				DrawRotaGraph3F((float)(320 - ((P1.Power % 1000) / 5) - 62), 454 + (SCREEN_H - 480),
					0, 0, ((P1.Power % 1000) / 5), 1, 0, PowerPoint, true, false);
			}
			SetDrawBright(255, 255, 255);

			// �Q�[�W�{��
			DrawGraph(25 + 1, 440 + (SCREEN_H - 480) - 1, PowerNum[P1.Power / 1000], true);

			//�p���[�ʁ@2P
			// �Q�[�WMAX��
			SetDrawBright(255, 180, 0);

			if (P2.Power == POWER_MAX){
				DrawRotaGraph3F((SCREEN_W - 640) + 320 + 62, 454 + (SCREEN_H - 480),
					0, 0, (1000 / 5), 1, 0, PowerPoint, true, false);
			}
			else{
				DrawRotaGraph3F((SCREEN_W - 640) + 320 + 62, 454 + (SCREEN_H - 480),
					0, 0, ((P2.Power % 1000) / 5), 1, 0, PowerPoint, true, false);
			}
			SetDrawBright(255, 255, 255);

			// �Q�[�W�{��
			DrawGraph(SCREEN_W - 50, 440 + (SCREEN_H - 480) - 1, PowerNum[P2.Power / 1000], true);

			///////////////////
			// �p���[�o�[�\��
			DrawGraph(20, 435 + (SCREEN_H - 480), PowerBar[0], true);
			DrawTurnGraph(SCREEN_W - 20 - bar_x, 435 + (SCREEN_H - 480), PowerBar[0], true);

		}
		
		// �f�o�b�O
		{
			/*
			// �f�o�b�O�p
			// �����l�p
			SetAlpha(128);
			DrawBox(498, SCREEN_H - 42, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
			NoBlend();

			DrawFormatString(500, SCREEN_H - 40, Cr, "ctrl..%d,%d", P1.ctrl, P2.ctrl);
			DrawFormatString(500, SCREEN_H - 20, Cr, "sflg..%d,%d", P1.SFlg, P2.SFlg);
			*/
		}

		/**************
		* �G�t�F�N�g
		**************/
		{
			EffDraw();
		}

		// ���v���C�L�[�\��
		if (Replay_Mode(-1) == 1)
			ObjectDraw_KeyDisplay();

		/**************
		* �A�i�E���X
		**************/
		{
			// �J��
			if (S.roundState == 0){

				// ������������
				if (S.roundTime > 0){
					SetAlpha(98);
					if (S.roundTime <= 110){
						DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
					}
					else if (S.roundTime > 110){
						DrawBox(0, 0, SCREEN_W, ((121 - S.roundTime) * 32), GetColor(0, 0, 0), true);
						DrawBox(0, SCREEN_H - ((121 - S.roundTime) * 32), SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
					}
					SetAlpha(255);
					
				}
				// SE
				if(S.roundTime == 1)SEStart(34);
				// SE
				if (S.roundTime == 71)SEStart(34);

				// �����ɂȂ�����
				if (S.roundTime >= 1 && S.roundTime <= 10)m_size = (11 - S.roundTime) / 2 + 0.5;
				else if (S.roundTime >= 70 && S.roundTime <= 80)m_size = (81 - S.roundTime) / 2 + 0.5;
				else{ m_size = 1; }
				if (m_size < 0)m_size = 1;

				if (S.roundTime >= 50 && S.roundTime < 70)SetAlpha(255 - (S.roundTime - 50) * 12.8);
				if (S.roundTime >= 1 && S.roundTime < 70){
					DrawRotaGraph(260, 240, m_size, 0, battle[1], true);
					DrawRotaGraph(400, 240, m_size, 0, battle[0], true);
				}


				if(S.roundTime >= 100)SetAlpha(255 - (S.roundTime - 100) * 12.8);
				if (S.roundTime >= 70)DrawRotaGraph(340, 240, m_size, 0, battle[2], true);


				NoBlend();
			}
			// �o�g����
			else if (S.roundState == 1){
				//
				/*
				for (int i = 0; i < 2; i++){
					if (addGaugeTime[i] > 0){
						if (addGaugeTime[i] == 60)SetAlpha(128);
						else if (addGaugeTime[i] == 1)SetAlpha(128);
						DrawRotaGraph(80 + (i * (SCREEN_W - 92 - 80)), 120, 1, 0, addGauge[0], true);
						SetAlpha(255);
						addGaugeTime[i] -= 1;
					}
				}
				*/
			}
			// ���E���h�擾
			else if (S.roundState == 2){
				
				// SE
				if (S.roundTime == 2)SEStart(34);

				// ������������
				if (S.roundTime > 0){
					SetAlpha(98);
					if (S.roundTime <= 40){
						DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
					}
					else if (S.roundTime > 40){
						DrawBox(0, 0, SCREEN_W, ((51 - S.roundTime) * 32), GetColor(0, 0, 0), true);
						DrawBox(0, SCREEN_H - ((51 - S.roundTime) * 32), SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
					}
					SetAlpha(255);
				}

				// DOWN �`��
				if (S.roundTime <= 1 && S.StopTime > 0){
					//SetAlpha(255 - (S.StopTime * 12.8));
					
					/*
					if (S.StopTime > 1){
						m_size = S.StopTime;
					}
					else{ m_size = 1; }
					*/
					DrawRotaGraph(360, 240, 1, 0, battle[3], true);
				}

				if (S.roundTime >= 1 && S.roundTime <= 10)m_size = (11 - S.roundTime) / 2 + 0.5;
				else{ m_size = 1; }
				if (m_size < 0)m_size = 1;

				if (S.roundTime >= 30)SetAlpha(255 - (S.roundTime - 30) * 12.8);
				// �����ɂȂ�����
				if (S.roundTime > 0)DrawRotaGraph(340, 240, m_size,0,battle[2], true);

				NoBlend();
			}
			

		}
		//-----------------
		// �t�F�[�h�A�E�g
		//-----------------
		{
			// �������̃z���C�g�A�E�g
			if (S.roundState == 3){
				int add;
				add = S.roundTime * 4;
				SetAlpha(add);
				DrawBox(0, 0, SCREEN_W, SCREEN_H, Cr, true);
				NoBlend();
			}

			// �u���b�N�A�E�g
			if (blackOut != 0){
				SetAlpha(blackOut);
				DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0,0,0), true);
				NoBlend();
				blackOut = 0;
			}
		}

}


// �q�b�g�{�b�N�X�ȊO�����ׂ܂���
void BoxCheck()
{
	// �`��A�\���I���̎��̂�
	if (S.TSwitch[5] == 0 || S.TSwitch[5] == 1){

		// �����l�p
		SetAlpha(128);
		DrawBox(498, 58, 620, 160, GetColor(0, 0, 0), true);
		NoBlend();

		// ���̑��̃`�F�b�N
		DrawFormatString(500, 60, Cr, "�_���[�W %d", m_DisplayDamage);
		// �Q�[�W���
		DrawFormatString(500, 80, Cr, "�Q�[�W %d", mGauge);

		// �t���[���`�F�b�N�E�J�n
		if ((mG_Start == 0) && (P2.ctrl == 0) &&
			((P2.HFlg == 1) || (P2.stateno >= 50 && P2.stateno <= 59))){
			mG_Start = 1, mGauge = 0;
		}
		// �Q�[�W�`�F�b�N
		if (mG_Start){
			mGauge = P1.Power - mG_startPower;
		}

		// �Q�[�W����E�I��
		if ((mG_Start) && (P1.ctrl && P2.ctrl)){
			mG_Start = 0;
		}
		else if (!mG_Start)mG_startPower = P1.Power;



		// �����t���[��
		DrawFormatString(500, 100, Cr, "���� %d", mHassei);
		if ((P1.MoveHit == 0) 
			&& 
			(
			(P2.stateno == 1000) || (P2.stateno == 1010) || (P2.stateno == 1020) || (P2.stateno == 1030) 
			|| (P2.stateno == 1060) || (P2.stateno == 1065)
			|| (P2.stateno == 50) || (P2.stateno == 51) || (P2.stateno == 52)
			) 
			&& (P2.time == 0))
			mHassei = aSpeed + 1;	// �����L��

		// �t���[���`�F�b�N
		DrawFormatString(500, 120, Cr, "�d���� %d", mFrame);

		// �t���[���`�F�b�N�E�J�n
		if ((mF_Start == 0) && (P2.ctrl == 0) && 
			((P2.HFlg == 1) || (P2.stateno >= 50 && P2.stateno <= 59))){
			mF_Start = 1, mFrame = 0;
		}

		// ���ԃ`�F�b�N
		if (mF_Start){
			if ((P1.ctrl) && (!P2.ctrl)){
				mFrame += 1;
			}
			else if ((!P1.ctrl) && (P2.ctrl)){
				mFrame -= 1;
			}
		}

		// �t���[���`�F�b�N�E�I��
		if ((mF_Start) && (P1.ctrl && P2.ctrl)){
			mF_Start = 0;
		}

		//DrawFormatString(500, 140, Cr, "�␳ %.2f", P1.A.hosei_K);
		if (S.TSwitch[2] == 3)DrawFormatString(500, 140, Cr, "�v���C���[����");
		else if (S.TSwitch[2] == 4)DrawFormatString(500, 140, Cr, "�R���s���[�^");
		else if (S.TSwitch[7] == 0)DrawFormatString(500, 140, Cr, "�ʏ�");
		else if (S.TSwitch[7] == 1)DrawFormatString(500, 140, Cr, "�^�揀��");
		else if (S.TSwitch[7] == 2){
			DrawString(500, 140, "�^�撆", GetColor(255,255,0));
			DrawFormatString(560, 140, Cr, "%d", TText[0]);
		}
		else if (S.TSwitch[7] == 3)DrawFormatString(500, 140, Cr, "�Đ��� %d", TText[0]);
		else if (S.TSwitch[7] > 3)DrawFormatString(500, 140, Cr, "������������");

		// �w���p�[
		//DrawFormatString(500, 160, Cr, "W.%d time.%d", H1[18].WAtt[1], H1[18].time);
		
		//DrawFormatString(500, 160, Cr, "%.2f", P1.XVel);
		//DrawFormatString(500, 160, Cr, "�U����� %d", P1.muki);
		
		// �K�[�N��
		//DrawFormatString(500, 160, Cr, "�K�[�h %d", TText[1]);

		/*
		DrawFormatString(500, 160, Cr, "%.2f", P1.YPos);
		if ((int)P1.YPos <= GROUND - 190){
			DrawBox(0, GROUND - S.ScroolY - 190, STAGE_WIDTH, GROUND - S.ScroolY -191, 
				Oran, true);
		}
		else{
			DrawBox(0, GROUND - S.ScroolY - 190, STAGE_WIDTH, GROUND - S.ScroolY -191, 
				Cr, true);
		}
		*/

		//DrawFormatString(500, 160, Cr, "%s", P1.nameC);	// �������ł��Ă�
		//DrawFormatString(500, 180, Cr, "XV:%.2f", P1.C.runF[0]);	// �������ł��Ă�
		//DrawFormatString(500, 200, Cr, "t1:%d", TText[1]);	// �������ł��Ă�
		//DrawFormatString(500, 220, Cr, "t2:%d", TText[2]);	// �������ł��Ă�
		//DrawFormatString(500, 240, Cr, "t3:%d", TText[3]);	// �������ł��Ă�
		//DrawFormatString(500, 260, Cr, "1:%d", TText[4]);	// �������ł��Ă�
		//DrawGraph(300, 260, TText[4], true);
		//DrawFormatString(500, 160, Cr, "%.2f", (float)((float)P1.Life / (float)P1.C.lifeMax));
		//if ((P1.YVel > 0) || ((P1.YVel < 0) && (P1.YPos < GROUND - 135)))DrawString(500, 160, "AirOK", Cr);
		//DrawFormatString(500, 160, Cr, "%d, %d", P1.Muteki, P2.Muteki);
		/*
		DrawFormatString(500, 160, Cr, "%d", P1.MoveHit);
		DrawFormatString(500, 180, Cr, "%d", P1.time);
		DrawFormatString(500, 200, Cr, "%d, %d", P1.WAtt[0], P1.HAtt[0]);
		*/
		//int xDist = (int)(P2.XPos - P1.XPos);
		//if (xDist < 0)xDist = -xDist;
		//int yDist = (int)(P2.YPos - P1.YPos);
		//if (yDist < 0)yDist = -yDist;
		
		
		//DrawFormatString(500, 160, Cr, "%d, %d", P1.Var[11], P1.D.armor);
		//DrawFormatString(500, 180, Cr, "F %d, T %d", P1.mutekiF, P1.Muteki);
		
		//DrawFormatString(500, 160, Cr, "%d", P1.addG);
		//DrawFormatString(500, 160, Cr, "c1:%d c4:%d", P1.cmd[1], P1.cmd[4]);
		//DrawFormatString(500, 180, Cr, "%d %d", P2.button[1], P2.rbutton[1]);
		//DrawFormatString(500, 215, Cr, "���� %s", TChar);
		//DrawFormatString(500, 180, Cr, "�e�X�g %d", TText);

	}

		//m_cPos = P1.XPos;
		if (S.TSwitch[5] == 0 || S.TSwitch[5] == 2){

		int ke = P1.keyPos;
		
		// �ǉ�
		if (P1.muki == 1){
			if (P1.keyPos == 1)ke = 3;
			else if (P1.keyPos == 3)ke = 1;
			else if (P1.keyPos == 4)ke = 6;
			else if (P1.keyPos == 6)ke = 4;
			else if (P1.keyPos == 7)ke = 9;
			else if (P1.keyPos == 9)ke = 7;
		}
		
		// �L�[
		//if (S.StopTime == 0)
		{
		for (int i = 0; i < 9; i++){
			if ((ke == i+1) && (ke != 5) && (ke != oKey[0] + 1 || ke != saveKey)){
				sKey = true;
				break;
			}
			else { sKey = false; }
		}

		// �{�^��
		for (int i = 0; i < 4; i++){
			if (P1.button[i + 1] == 1){
				sButton = true;
				sKey = true;
				break;
			}
			else { sButton = false; }
		}

		
		// �L�[or�{�^���J�n
		if (sKey || sButton){

			// �L�[
			for (int a = 1; a < LOG_MAX; a++){
				oKey[LOG_MAX - a] = oKey[LOG_MAX - 1 - a];
			}
			for (int i = 0; i < 9; i++){
				if (ke == i+1){
					oKey[0] = i;

					//else{ oKey[0] = i; }
					break;
				}
				else { oKey[0] = 0; }
			}

			// �{�^��
			for (int a = 1; a < LOG_MAX; a++){
				for (int i = 0; i < 4; i++){
					oButton[LOG_MAX - a][i] = oButton[LOG_MAX - 1 - a][i];
				}
			}
			for (int i = 0; i < 4; i++){
				if (P1.button[i + 1] == 1){
					oButton[0][i] = 1;
				}
				else { oButton[0][i] = 0; }
			}
		}
		}	//  stoptime

		// �`��
		int px;
		for (int a = 0; a < LOG_MAX; a++){
			px = 0;
			if (oKey[a] > -1){
				DrawGraph(px, 95 + (a*20), gKey[oKey[a]], true);
			}
			px = 20;
			for (int i = 0; i < 4; i++){
				if (oButton[a][i] == 1){
					DrawGraph(px, 95 + (a * 20), gButton[i], true);
					px += 20;
				}
			}
		}
		
		saveKey = ke;
		}	// S.T[5]

		//DrawFormatString(0, 35, Cr, "�e���L�[ %d", P1.keyPos);
		
		// �~�j�摜
		//DrawRotaGraph(20, 25, 0.4, 0, P1.Image, true, P1.muki);
		//DrawRotaGraph(SCREEN_W - 20, 25, 0.4, 0, P2.Image, true, P2.muki);
		
		// �e�X�g�摜
		//DrawRotaGraph(120, 125, 0.5, 0, TText, true, 0);
		//DrawFormatString(500, 155, Cr, "�P�\ %d", TText);
		//DrawFormatString(500, 135, Cr, "C���� %d", P1.Senkou[3]);
		//DrawFormatString(500, 155, Cr, "��~ %d", S.Anten);


		//DrawFormatString(500, 115, Cr, "X���x %.1f", P1.XVel);
		//DrawString(500, 115, "�K�[�h����", Cr);
		//if (P1.GuardF == 0)DrawString(590, 115, "�K�[�s", Cr);
		//else if (P1.GuardF == 1)DrawString(590, 115, "��i", Cr);
		//else if (P1.GuardF == 2)DrawString(590, 115, "���i", Cr);
		//else if (P1.GuardF == 3)DrawString(590, 115, "���i", Cr);

	

	// �{�b�N�X�`��
	{
		// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);

		// ��炢����
		if (S.TSwitch[6] == 1 || S.TSwitch[6] == 3)BoxKurai();
		// �U������
		if (S.TSwitch[6] == 2 || S.TSwitch[6] == 3)BoXAtt();
		

		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
}


void GetP_ObjectDraw(Player GP1, Player GP2)
{
	P1 = GP1;
	P2 = GP2;
}

void GetS_ObjectDraw(System_t GS)
{
	S = GS;
}

void GetH_ObjectDraw(Helper GH1[], Helper GH2[])
{
	int i;
	for (i = 0; i < HM; i++){
		H1[i] = GH1[i];
		H2[i] = GH2[i];
	}
}

void BoxKurai()
{
	int color;

	for (int j = 0; j < 2; j++){

		// �ŏ��ɐ�����
		color = GetColor(0, 0, 255);
		// ���G�ŗ΂�
		if (j == 0){
			if (P1.Muteki != 0) {
				switch (P1.mutekiF){
				case 0:
					color = GetColor(0, 255, 0);
					break;
				case 1:
					color = GetColor(0, 255, 255);
					break;
				case 2:
					color = GetColor(255, 255, 0);
					break;
				case 3:
					color = GetColor(255, 255, 0);
					break;
				}
			}

		}
		if(j == 1){
			if (P2.Muteki != 0) {
				switch (P2.mutekiF){
				case 0:
					color = GetColor(0, 255, 0);
					break;
				case 1:
					color = GetColor(0, 255, 255);
					break;
				case 2:
					color = GetColor(255, 255, 0);
					break;
				case 3:
					color = GetColor(255, 255, 0);
					break;
				}
			}
		}
		// 2P ��炢����`�F�b�N
		for (int i = 0; i < HIT_MAX; i++){
			// �X�N���[���ʂ𑫂�
			if (j == 0){
				m_XD[i] = P1.XHit[i] - (int)S.ScroolX;
				m_YD[i] = P1.YHit[i] - (int)S.ScroolY;
				m_WD[i] = P1.WHit[i];
				m_HD[i] = P1.HHit[i];
			}
			else if (j == 1){
				m_XD[i] = P2.XHit[i] - (int)S.ScroolX;
				m_YD[i] = P2.YHit[i] - (int)S.ScroolY;
				m_WD[i] = P2.WHit[i];
				m_HD[i] = P2.HHit[i];
			}

			if (m_WD[i] > 0){
				// �`��
				DrawBox(m_XD[i], m_YD[i], m_XD[i] +
					m_WD[i], m_YD[i] + m_HD[i], color, true);
			}
				
		}
		// ���W
		if (j == 0){
			DrawBox((int)P1.XPos - 3 - (int)S.ScroolX, (int)P1.YPos - 3 - (int)S.ScroolY,
				(int)P1.XPos + 3 - (int)S.ScroolX, (int)P1.YPos + 3 - (int)S.ScroolY, Oran, true);
		}
		else if (j == 1){
			DrawBox((int)P2.XPos - 3 - (int)S.ScroolX, (int)P2.YPos - 3 - (int)S.ScroolY,
				(int)P2.XPos + 3 - (int)S.ScroolX, (int)P2.YPos + 3 - (int)S.ScroolY, Oran, true);
		}


		// �w���p�[��炢����
		for (int i = 0; i < HM; i++){
			for (int n = 0; n < HIT_MAX; n++){
				mHXH[0][i] = H1[i].XHit[n] - (int)S.ScroolX;
				mHYH[0][i] = H1[i].YHit[n] - (int)S.ScroolY;
				mHWH[0][i] = H1[i].WHit[n];
				mHHH[0][i] = H1[i].HHit[n];

				mHXH[1][i] = H2[i].XHit[n] - (int)S.ScroolX;
				mHYH[1][i] = H2[i].YHit[n] - (int)S.ScroolY;
				mHWH[1][i] = H2[i].WHit[n];
				mHHH[1][i] = H2[i].HHit[n];

				// �U������`�F�b�N
				DrawBox(mHXH[j][i], mHYH[j][i], mHXH[j][i] + mHWH[j][i],
					mHYH[j][i] + mHHH[j][i], GetColor(0, 0, 255), true);

				
			}
			if (H1[i].var){
				// ���W
				DrawBox((int)H1[i].XPos - 3 - (int)S.ScroolX, (int)H1[i].YPos - 3 - (int)S.ScroolY,
					(int)H1[i].XPos + 3 - (int)S.ScroolX, (int)H1[i].YPos + 3 - (int)S.ScroolY, Oran, true);
			}
		}
	}	// for�I��

}

void BoXAtt()
{
	for (int j = 0; j < 2; j++){
		
		/*
		{
			mXA[0][0] = P1.X_UL - (int)S.ScroolX;
			mYA[0][0] = P1.Y_UL - (int)S.ScroolY;
			mWA[0][0] = P1.xSize;
			mHA[0][0] = P1.ySize;

			// �L�����T�C�Y�`�F�b�N
			DrawBox(mXA[0][0], mYA[0][0], mXA[0][0] + mWA[0][0],
				mYA[0][0] + mHA[0][0], GetColor(0, 240, 0), true);
		}
		*/

		for (int i = 0; i < HIT_MAX; i++){
			mXA[0][i] = P1.XAtt[i] - (int)S.ScroolX;
			mYA[0][i] = P1.YAtt[i] - (int)S.ScroolY;
			mWA[0][i] = P1.WAtt[i];
			mHA[0][i] = P1.HAtt[i];

			mXA[1][i] = P2.XAtt[i] - (int)S.ScroolX;
			mYA[1][i] = P2.YAtt[i] - (int)S.ScroolY;
			mWA[1][i] = P2.WAtt[i];
			mHA[1][i] = P2.HAtt[i];

			if (mWA[j][i] > 0){
				// �U������`�F�b�N
				DrawBox(mXA[j][i], mYA[j][i], mXA[j][i] + mWA[j][i],
					mYA[j][i] + mHA[j][i], GetColor(255, 0, 0), true);
			}
		}


		// �w���p�[�U������
		for (int i = 0; i < HM; i++){
			for (int n = 0; n < HIT_MAX; n++){
				mHXA[0][i] = H1[i].XAtt[n] - (int)S.ScroolX;
				mHYA[0][i] = H1[i].YAtt[n] - (int)S.ScroolY;
				mHWA[0][i] = H1[i].WAtt[n];
				mHHA[0][i] = H1[i].HAtt[n];

				mHXA[1][i] = H2[i].XAtt[n] - (int)S.ScroolX;
				mHYA[1][i] = H2[i].YAtt[n] - (int)S.ScroolY;
				mHWA[1][i] = H2[i].WAtt[n];
				mHHA[1][i] = H2[i].HAtt[n];

				// �U������`�F�b�N
				DrawBox(mHXA[j][i], mHYA[j][i], mHXA[j][i] + mHWA[j][i],
					mHYA[j][i] + mHHA[j][i], GetColor(255, 0, 0), true);
			}
		}

	}
}


void TestText(int i, int pos)
{
	TText[pos] = i;
}

void TestChar(char ch[], int size)
{
	for (int i = 0; i < size; i++){
		TChar[i] = ch[i];
	}
}

// �����t���[��
void AttackSpeed(int i)
{
	aSpeed = i;
}

// �t���[���`�F�b�N
void Frame_Check()
{
	mF_Start = 1, mFrame = 0;
}

// �A���q�b�g(�r)�m�F
void Beat_Check(int state, int pside)
{
	// 1P
	if (pside == 1){
		// �����Ă���
		if (P2.HFlg){
			// �󂯐g�\�Ȃ�r
			if (state == 1035){
				beat[0] = false;
			}
		}
		// �����ĂȂ����͐ԃr
		else{
			beat[0] = true;
		}
	}

	// 2P
	else if (pside == 2){
		if (P1.HFlg){
			if (state == 1035){
				beat[1] = false;
			}
		}
		else{
			beat[1] = true;
		}
	}
}

// �L�[�\��
void ObjectDraw_KeyDisplay()
{
	SetAlpha(158);
	DrawGraph((SCREEN_W / 2) - 60, 65, m_KeyPos, true);

	SetAlpha(255);
	int x = 0, y = 0;
	if (P1.button[102] > 0)y = 1;
	if (P1.button[108] > 0)y = -1;
	if (P1.button[106] > 0)x = 1;
	if (P1.button[104] > 0)x = -1;
	DrawGraph((SCREEN_W / 2) - 60 + 12 + (x * 16), 65 + 12 + (y * 16), m_Key, true);

	SetAlpha(255);
	if (P1.button[1] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W/2) + 10, 65, m_Button[0], true);

	SetAlpha(255);
	if (P1.button[2] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 35, 65, m_Button[1], true);

	SetAlpha(255);
	if (P1.button[3] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 60, 65, m_Button[2], true);

	SetAlpha(255);
	if (P1.button[4] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 10, 90, m_Button[3], true);

	NoBlend();

	////////////////////////
	// P2
	SetAlpha(158);
	DrawGraph((SCREEN_W / 2) - 60, 125, m_KeyPos, true);

	SetAlpha(255);
	x = 0, y = 0;
	if (P2.button[102] > 0)y = 1;
	if (P2.button[108] > 0)y = -1;
	if (P2.button[106] > 0)x = 1;
	if (P2.button[104] > 0)x = -1;
	DrawGraph((SCREEN_W / 2) - 60 + 12 + (x * 16), 125 + 12 + (y * 16), m_Key, true);

	SetAlpha(255);
	if (P2.button[1] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 10, 125, m_Button[0], true);

	SetAlpha(255);
	if (P2.button[2] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 35, 125, m_Button[1], true);

	SetAlpha(255);
	if (P2.button[3] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 60, 125, m_Button[2], true);

	SetAlpha(255);
	if (P2.button[4] == 0){
		SetAlpha(128);
	}
	DrawGraph((SCREEN_W / 2) + 10, 150, m_Button[3], true);

	NoBlend();
}

// �A�h�Q�[�W
void AddGaugeDraw(int power, int side)
{
	addGaugeTime[side - 1] = 60;
	addGaugePower[side - 1] = power;
}

// ���̓x����
void SetBlack(int b)
{
	blackOut = b;
	// �ő�l�ݒ�
	max(0, min(255, blackOut));	// �E�ɍ��̐��l��
}