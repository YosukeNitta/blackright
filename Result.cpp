#include "pch.h"

static int nPos;
static int color[2];
static int side;

static int winChara[2];	// �������L����
static int stand[7];
static boolean load_1 = false;
static int time;

static int p_color[2];
static int p_name[2];
static int StageNum;
// �ΐ퐔
static int winCount[2];
static void Draw();

int Result()
{
	if (!load_1){
		stand[CORNEL] = LoadGraph("ob/standCornell.png");
		stand[EIN] = LoadGraph("ob/standEin.png");
		stand[BOUNCER] = LoadGraph("ob/standBouncer.png");
		stand[HELIOS] = LoadGraph("ob/standHelios.png");
		stand[HYDE] = LoadGraph("ob/stand3sample.png");
		stand[SYUICHI] = LoadGraph("ob/stand3sample.png");
		load_1 = true;
	}

	if (P_BInput(108) == 1){
		nPos--;
		SEStart(36);
	}
	if (P_BInput(102) == 1){
		nPos++;
		SEStart(36);
	}
	if (nPos > 1)nPos = 0;
	if (nPos < 0)nPos = 1;

	// ��

	// �A�P�Ȃ�
	if (Arcade_Switch(-1) == 1){
		if ((P_BInput(1) == 1) || (P_BInput(3) == 1)){
			SEStart(35);
			if (nPos == 0){
				ModeChange(GameScene(ArcadeScene));	// �ΐ��ʂփ��[�v
				Replay_Mode(0);	// ������x���v���C
			}
			else if (nPos == 1)ModeChange(GameScene(MenuScene));	// ���j���[��
		}
		if ((P_BInput(2) == 1)){
			SEStart(37);
			nPos = 1;
		}
	}
	// �A�P����Ȃ�
	else{
		if ((P_BInput(1) == 1) || (P_BInput(3) == 1)){
			SEStart(35);
			if (nPos == 0){
				ModeChange(GameScene(VersusScene));	// �ΐ��ʂփ��[�v
				if (BattleMode(-1) == 0)Replay_Mode(2);	// ������x�^�悷��
				else{ Replay_Mode(0); }
				//Replay_Setting(p_name[0], p_name[1], p_color[0], p_color[1], StageNum);
			}
			else if (nPos == 1)ModeChange(GameScene(SelectScene));	// �L�����Z����
		}
		if ((P_BInput(2) == 1)){
			SEStart(37);
			nPos = 1;
		}
	}
	
	
	Draw();

	return 0;
}

void Draw()
{
	if (time < 10)time++;

	// ��
	DrawBox(0, 0, SCREEN_W, SCREEN_H, Cr, true);

	// �����L����
	if (winChara[0] != -1)
		DrawRotaGraph(SCREEN_W/3 + 300 - (time * 30), SCREEN_H / 2, 0.8, 0, stand[winChara[0]], true, false);

	if ((winChara[0] != -1) && (time == 10)){
		// �{�b�N�X
		// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
		// ��������
		DrawBox(360 - 2, SCREEN_H / 2 - 122, 600 + 2, SCREEN_H / 2 - 80, GetColor(0, 0, 0), true);
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		if (winChara[0] == CORNEL){
			if (winChara[1] == winChara[0]){
				DrawString(360, SCREEN_H / 2 - 120, "�n���^�[�͓�l������Ȃ�", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "�����Ă��炤��", Cr);
			}
			else{
				DrawString(360, SCREEN_H / 2 - 120, "���O�̔\�͂ł�", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "�͕s�����A������I", Cr);
			}
		}
		else if (winChara[0] == BOUNCER){
			if (winChara[1] == winChara[0]){
				DrawString(360, SCREEN_H / 2 - 120, "�������ȁA�����ڂ����łȂ�", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "���͂��{�����c", Cr);
			}
			else{
				DrawString(360, SCREEN_H / 2 - 120, "�܂�����̗]�n������", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "�b���Ȃ����񂾂�", Cr);
			}
		}
		else if (winChara[0] == EIN){
			if (winChara[1] == winChara[0]){
				DrawString(360, SCREEN_H / 2 - 120, "�����A�����������", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "���Ⴀ�����{�����Ă��ƂŁI", Cr);
			}
			else{
				DrawString(360, SCREEN_H / 2 - 120, "�悵�A�������q�I", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "���̃m����厖�ɂ�������", Cr);
			}
		}
		else if (winChara[0] == HELIOS){
			if (winChara[1] == winChara[0]){
				DrawString(360, SCREEN_H / 2 - 120, "���O���ɂ������肾�ȁI", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "�����ʂ�̌Z�M���I�H", Cr);
			}
			else{
				DrawString(360, SCREEN_H / 2 - 120, "�ʔ����z����", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "���x���W�͊��}�����I", Cr);
			}
		}
		else if (winChara[0] == HYDE){
			if (winChara[1] == winChara[0]){
				DrawString(360, SCREEN_H / 2 - 120, "�_�������Z���t", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "", Cr);
			}
			else{
				DrawString(360, SCREEN_H / 2 - 120, "���L���������Z���t", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "", Cr);
			}
		}
		else if (winChara[0] == SYUICHI) {
			if (winChara[1] == winChara[0]) {
				DrawString(360, SCREEN_H / 2 - 120, "�V���E�C�`�����Z���t", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "", Cr);
			}
			else {
				DrawString(360, SCREEN_H / 2 - 120, "���L���������Z���t", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "", Cr);
			}
		}
		else {
			{
				DrawString(360, SCREEN_H / 2 - 120, "�����Z���t����Y��I", Cr);
				DrawString(360, SCREEN_H / 2 - 100, "�Ԃ����낷���I", Cr);
			}
		}
	}

	// ���U���g�Ȃ�
	if ((CheckGameMode() == GameScene(ResultScene)) && (time == 10)){
		// ���҂͂ǂ�����
		if (side == 0){
			DrawString(0, 0, "draw", GetColor(0, 0, 0));
		}
		else if (side == 1){
			DrawString(0, 0, "1P win", GetColor(0, 0, 0));
		}
		else if (side == 2){
			DrawString(0, 0, "2P win", GetColor(0, 0, 0));
		}

		//------------
		// �ΐ퐔�\��
		//------------
		int pos[2];
		for (int i = 0; i < 2; i++){
			pos[i] = 0;
			if (winCount[i] >= 100)pos[i] = -20;
			else if (winCount[i] >= 10)pos[i] = -10;
		}

		DrawFormatString(0 + 80 + pos[0], 20, GetColor(0, 0, 0), "%d win", winCount[0]);
		DrawFormatString(SCREEN_W - 120 + pos[1], 20, GetColor(0, 0, 0), "%d win", winCount[1]);


		// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
		// ��������
		DrawBox(280 - 2, SCREEN_H / 2 - 22, 368 + 2, SCREEN_H / 2 + 20, GetColor(0, 0, 0), true);
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		if (nPos == 0)color[0] = Oran;
		else{ color[0] = Cr; }
		if (nPos == 1)color[1] = Oran;
		else{ color[1] = Cr; }

		// �A�P��
		// 2P�������������
		if (Arcade_Switch(-1) == 1){
			DrawString(280, SCREEN_H / 2 - 20, "�Đ�", color[0]);
			DrawString(280, SCREEN_H / 2, "���j���[", color[1]);
		}
		else {
			DrawString(280, SCREEN_H / 2 - 20, "�Đ�", color[0]);
			DrawString(280, SCREEN_H / 2, "�L�����Z��", color[1]);
		}
	}
}

void Load_Result()
{
	nPos = 0;
	BGMStart(0);	// BGM���~
	time = 0;
}

void Result_WinSide(int i, int chara, int chara2){
	side = i;
	winChara[0] = chara;
	winChara[1] = chara2;
}

void Result_GetData(int c1, int c2, int cl1, int cl2, int stage) {
	p_name[0] = c1;
	p_name[1] = c2;
	p_color[0] = cl1;
	p_color[1] = cl2;
	StageNum = stage;
}

void WinCount(int side)
{
	if (side == 1)winCount[0]++;
	else if (side == 2)winCount[1]++;
	// ���Z�b�g
	else if (side != 3){
		winCount[0] = 0;
		winCount[1] = 0;
	}
}