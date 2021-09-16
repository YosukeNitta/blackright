#include "pch.h"
#include "Result.h"
#include "MainSystem.h"
#include <string>	// string�Ɏg��
#include <vector>

using namespace std;


class TextBox {
public:
	// ����̖��O
	string enemyName;
	// �e�L�X�g
	string text[2];

	TextBox() {

	}
};

class WinText {
public:
	// ���O
	string name;
	// �����G
	int image;

	// �i�[���������Z���t
	vector<TextBox> tex;

	WinText() {

	}
};

static vector<WinText> winTexts;	// �L�������̏����Z���t�܂Ƃ�

// ���O�iair����ǂݍ��݁js
static vector<Name_t> N;

static int nPos;
static int color[2];
static int side;

static int winChara[2];	// �������L����
static string charName[2];
static boolean load_1 = false;
static int time;

static int p_color[2];
static int p_name[2];
static int StageNum;

// �Ó]
static int _timeStop;
// ���̃V�[��
static int _nextScene;

// �ΐ퐔
static int winCount[2];
static void Draw();

int Result::Mode()
{
	if (!load_1){
		load_1 = true;
	}

	if (P1_BInput(108) == 1){
		nPos--;
		SEStart(36);
	}
	if (P1_BInput(102) == 1){
		nPos++;
		SEStart(36);
	}
	if (nPos > 1)nPos = 0;
	if (nPos < 0)nPos = 1;

	// ��

	// �A�P�Ȃ�
	if (Arcade_Switch(-1) == 1){
		if ((P1_BInput(1) == 1) || (P1_BInput(3) == 1)){
			SEStart(35);
			if (nPos == 0) {
				// 1P��������
				if(side == 1)_nextScene = SceneNum(ArcadeScene);	// �V�[���ݒ�
				// cpu��������
				else { _nextScene = SceneNum(VersusScene); }	// �V�[���ݒ�
				Replay_Mode(0);	// ������x���v���C
			}
			else if (nPos == 1)
				_nextScene = SceneNum(MenuScene);
		}
		if ((P_BInput(2) == 1)){
			SEStart(37);
			nPos = 1;
		}
	}
	// �A�P����Ȃ�
	else{
		if ((P1_BInput(1) == 1) || (P1_BInput(3) == 1)){
			SEStart(35);
			if (nPos == 0){
				// �ΐ��ʂփ��[�v
				//MainSystem::Instance().SetNextMode("Versus");
				_nextScene = SceneNum(VersusScene);	// �V�[���ݒ�
				//if (BattleMode(-1) == 0)Replay_Mode(2);	// ������x�^�悷��
				//else{ Replay_Mode(0); }
				//Replay_Setting(p_name[0], p_name[1], p_color[0], p_color[1], StageNum);
			}
			// �L�����Z��
			else if (nPos == 1)
				_nextScene = SceneNum(SelectScene);	// �V�[���ݒ�
		}
		if ((P1_BInput(2) == 1)){
			SEStart(37);
			nPos = 1;
		}
	}

	// �ڍs����
	if ((_timeStop >= 11) && (_nextScene != 0)) {
		// �V�[���̔ԍ�
		switch (_nextScene) {
			case SceneNum(ArcadeScene):
				MainSystem::Instance().SetNextMode("Arcade");	// ���j���[��
				break;
			case SceneNum(MenuScene):
				MainSystem::Instance().SetNextMode("Menu");	// ���j���[��
				break;
			case SceneNum(VersusScene):
				MainSystem::Instance().SetNextMode("Versus");	// �ΐ��ʂփ��[�v
				// �A�P�Ȃ�
				if (Arcade_Switch(-1) == 1) {
					Replay_Mode(0);	// ������x���v���C
				}
				// �A�P����Ȃ�
				else {
					if (BattleMode(-1) == 0)Replay_Mode(2);	// ������x�^�悷��
					else { Replay_Mode(0); }
				}
				break;
			case SceneNum(SelectScene):
				MainSystem::Instance().SetNextMode("Select");
				break;
		}
	}
	// �Ó]�i�߂�
	if (_nextScene != 0)
		_timeStop++;
	
	
	//Draw();

	return 0;
}

void Result::Draw()
{
	if (time < 10)time++;

	// ��
	DrawBox(0, 0, SCREEN_W, SCREEN_H, Cr, true);

	// �����L�����̉摜�\��
	if (winChara[0] != -1) {
		for (int i = 0; i < winTexts.size(); i++) {
			if (charName[0] == winTexts[i].name) {
				DrawRotaGraph(SCREEN_W / 3 + 300 - (time * 30), SCREEN_H / 2, 0.8, 0, winTexts[i].image, true, false);	
				break;
			}
		}
	}
		

	if ((winChara[0] != -1) && (time == 10)){
		// �{�b�N�X
		// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
		// ��������
		DrawBox(340 - 2, SCREEN_H / 2 - 122, 620 + 2, SCREEN_H / 2 - 80, GetColor(0, 0, 0), true);
		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �e�L�X�g��ǂݍ���
		boolean textCheck = false;

		// �e�L�X�g�\��
		// �e�L�X�g�W����T��
		for (int i = 0; i < winTexts.size(); i++) {
			// ���҂Ɠ������O
			if (charName[0] == winTexts[i].name) {
				for (int j = 0; j < winTexts[i].tex.size(); j++) {
					// �G�̖��O����
					if (charName[1] == winTexts[i].tex[j].enemyName) {
						//DrawFormatString(400, 100, Oran, "��v���� %s", winTexts[i].tex[j].enemyName.c_str());
						// �e�L�X�g
						DrawFormatString(340, SCREEN_H / 2 - 120, Cr, "%s", winTexts[i].tex[j].text[0].c_str());
						DrawFormatString(340, SCREEN_H / 2 - 100, Cr, "%s", winTexts[i].tex[j].text[1].c_str());
						textCheck = true;
						break;
					}
				}

				/*
				DrawFormatString(360, 160, Oran, "size:%d", winTexts[i].tex.size());

				DrawFormatString(360, 180, Oran, "%s", charName[0].c_str());
				DrawFormatString(360, 200, Oran, "%s", charName[1].c_str());
				*/

				

				// �e�L�X�g���Ȃ�����
				if (!textCheck) {
					// �ėp�Z���t
					if (winTexts[i].tex.size() > 0) {
						// �e�L�X�g��ėp��
						DrawFormatString(340, SCREEN_H / 2 - 120, Cr, "%s", winTexts[i].tex[0].text[0].c_str());
						DrawFormatString(340, SCREEN_H / 2 - 100, Cr, "%s", winTexts[i].tex[0].text[1].c_str());
					}
					else 
					{
						// �e�L�X�g��ėp��
						DrawString(340, SCREEN_H / 2 - 120, "�����e�L�X�g��i", Cr);
						DrawString(340, SCREEN_H / 2 - 100, "�����e�L�X�g���i", Cr);

						// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
						// ��������
						DrawBox(100 - 2, 300 - 2, 620 + 2, 300 + 40, GetColor(0, 0, 0), true);
						// �`��u�����h���[�h���m�[�u�����h�ɂ���
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
						DrawString(100, 300, "�������e�L�X�g���ݒ肳��Ă��܂���", Oran);
						DrawString(100, 320, "�@�t�@�C�������݂��Ȃ����A�L�q�ɃG���[������\��������܂�", Oran);

						// �����e�L�X�g
						DrawFormatString(360, 160, Oran, "�e�L�X�g��:%d", winTexts[i].tex.size());

					}
				}

				break;
			}
			
		}

		/*
		// �R�[�l��
		if (winChara[0] == CORNEL){
			if (winChara[1] == winChara[0]){
				DrawString(340, SCREEN_H / 2 - 120, "�n���^�[�͓�l������Ȃ�", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�����Ă��炤��", Cr);
			}
			else if(winChara[1] == EIN) {
				DrawString(340, SCREEN_H / 2 - 120, "�������Ǝ�����A����", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "���ɂ����Ȃ���΂�", Cr);
			}
			else if (winChara[1] == HELIOS) {
				DrawString(340, SCREEN_H / 2 - 120, "���܂蒲�q�ɏ��Ȃ�", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "", Cr);
			}
			else if (winChara[1] ==	BOUNCER) {
				DrawString(340, SCREEN_H / 2 - 120, "�͂����Ō��܂�悤��", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "", Cr);
			}

		}
		// �o�E���T�[
		else if (winChara[0] == BOUNCER){
			if (winChara[1] == winChara[0]){
				DrawString(340, SCREEN_H / 2 - 120, "��߂Ƃ���", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�^���ł���킢������˂���", Cr);
			}
			else if(winChara[1] == CORNEL){
				DrawString(340, SCREEN_H / 2 - 120, "���̓{�肪���O�̌����͂�", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�Ԃ��鑊�肪���邾���}�V��", Cr);
			}
			else if (winChara[1] == HELIOS) {
				DrawString(340, SCREEN_H / 2 - 120, "���O�����܂ŔѐH���Ă񂾂�H", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�����ۊo�����ق���������", Cr);
			}
			else if (winChara[1] == EIN) {
				DrawString(340, SCREEN_H / 2 - 120, "���̕ӂɂ��Ƃ���", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "���O����ɕ�����قǓ݂��Ă˂�", Cr);
			}
			else if (winChara[1] == HYDE) {
				DrawString(340, SCREEN_H / 2 - 120, "�Ƃ肠���������ł悩������", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�܂����͎R�ς݂݂�������", Cr);
			}
			else if (winChara[1] == SYUICHI) {
				DrawString(340, SCREEN_H / 2 - 120, "�C�ɐH��˂��z����", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "����������悤�Ȑ킢��������", Cr);
			}
			else{
				DrawString(340, SCREEN_H / 2 - 120, "�܂�����̗]�n������", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�b���Ȃ����񂾂�", Cr);
			}
		}
		// �A�C��
		else if (winChara[0] == EIN){
			if (winChara[1] == winChara[0]){
				DrawString(340, SCREEN_H / 2 - 120, "�����A�����������", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "���Ⴀ�����{�����Ă��ƂŁI", Cr);
			}
			else if(winChara[1] == CORNEL){
				DrawString(340, SCREEN_H / 2 - 120, "����ł��������ł���", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�l�������Ă΂�肵����ł�����I", Cr);
			}
			else if (winChara[1] == HELIOS) {
				DrawString(340, SCREEN_H / 2 - 120, "����ł��������ł���", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�l�������Ă΂�肵����ł�����I", Cr);
			}
			else{
				DrawString(340, SCREEN_H / 2 - 120, "�悵�A�������q�I", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "���̃m����厖�ɂ�������", Cr);
			}
		}
		else if (winChara[0] == HELIOS){
			if (winChara[1] == winChara[0]){
				DrawString(340, SCREEN_H / 2 - 120, "���O���ɂ������肾�ȁI", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�����ʂ�̌Z�M���I�H", Cr);
			}
			else if (winChara[1] == CORNEL) {
				DrawString(340, SCREEN_H / 2 - 120, "�����������̂͂킩�������ǂ�", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "���O���\�����܂�Ă�������", Cr);
			}
			else if (winChara[1] == BOUNCER) {
				DrawString(340, SCREEN_H / 2 - 120, "���񂭂炢�ŗp�S�_�ɂȂ��̂�", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "���ɂ��ł��������ȁI", Cr);
			}
			else if (winChara[1] == EIN) {
				DrawString(340, SCREEN_H / 2 - 120, "�����ڒʂ�̖ʔ����������ȁI", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�܂������͂Ȃ��������ǂ�", Cr);
			}
			else if (winChara[1] == HYDE) {
				DrawString(340, SCREEN_H / 2 - 120, "�Â��z���˂��c", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "����Ȃ���E�����Ȃ�ĂȂ�Ȃ���", Cr);
			}
			else if (winChara[1] == SYUICHI) {
				DrawString(340, SCREEN_H / 2 - 120, "�ςȌ��p���ȁA�ǂ��̍��̓z���H", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "���̂��������Ƃ����킩��˂���", Cr);
			}
			else{
				DrawString(340, SCREEN_H / 2 - 120, "�ʔ����z����", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "���x���W�͊��}�����I", Cr);
			}
		}
		// �_��
		else if (winChara[0] == HYDE){
			if (winChara[1] == winChara[0]){
				DrawString(340, SCREEN_H / 2 - 120, "���Ǝ҂Ȃ�킩���Ă����", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "����ł��炤��", Cr);
			}
			else{
				DrawString(340, SCREEN_H / 2 - 120, "�����ē��R��", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�E���o����ł��Ă��Ȃ��Ȃ��", Cr);
			}
		}
		// �V���E�C�`
		else if (winChara[0] == SYUICHI) {
			if (winChara[1] == winChara[0]) {
				DrawString(340, SCREEN_H / 2 - 120, "���l���^���ł͂��ꂪ���E��", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�Z�����ł͉��ɂ͒ǂ�����", Cr);
			}
			else if (winChara[1] == CORNEL) {
				DrawString(340, SCREEN_H / 2 - 120, "�{��ɐg��C���ď��Ă�ق�", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�Â����肾�Ǝv�������H", Cr);
			}
			else if (winChara[1] == HELIOS) {
				DrawString(340, SCREEN_H / 2 - 120, "�e�ƌ��ǂ�����g����̂�", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�ǂ�����G��Ȃ������悤����", Cr);
			}
			else{
				DrawString(340, SCREEN_H / 2 - 120, "�Ȃ��Ȃ�����������������", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "�����͂Ȃ�����", Cr);
			}
		}
		// �ėp
		else {
			if (winChara[1] == winChara[0]) {
				DrawString(340, SCREEN_H / 2 - 120, "���L���������Z���t", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "", Cr);
			}
			else {
				DrawString(340, SCREEN_H / 2 - 120, "�����Z���t", Cr);
				DrawString(340, SCREEN_H / 2 - 100, "", Cr);
			}
		}
		*/
	}

	// ���U���g�Ȃ�
	//(CheckGameMode() == SceneNum(ResultScene)) 
	if (time == 10){
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

		// �A�P��2P�������������
		if (Arcade_Switch(-1) == 1){
			DrawString(280, SCREEN_H / 2 - 20, "�Đ�", color[0]);
			DrawString(280, SCREEN_H / 2, "���j���[", color[1]);
		}
		else {
			DrawString(280, SCREEN_H / 2 - 20, "�Đ�", color[0]);
			DrawString(280, SCREEN_H / 2, "�L�����Z��", color[1]);
		}
	}

	// ���V�[���ڍs�̈Ó]
	if ((_nextScene != 0) && (_timeStop > 0)) {
		int num = _timeStop * 25;
		if (num > 255) num = 255;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, num);
		DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(255, 255, 255), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Result::Load_Reload()
{
	nPos = 0;
	BGMStart(0);	// BGM���~
	time = 0;
	_timeStop = 0;
	_nextScene = 0;
}

void Result::Load_1second()
{

}

void Result::Release(void)
{
}

// �����L�����Ɩ��O
void Result_WinSide(int i, int chara, int chara2, char name1[NAME_MAX], char name2[NAME_MAX]){
	side = i;
	winChara[0] = chara;
	winChara[1] = chara2;
	charName[0] = name1;
	charName[1] = name2;
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

// �����Z���t�ǂݍ���
// csv�œǂݍ���
void load_winText(char charName[NAME_MAX])
{
	// ����Z�b�e�B���O //

	int i, fp;
	int enemyNum = 0;	// �G�̔ԍ�

	char inputc[TX_MAX];	// input��inputc�ɕ������͂���
	int flg = -1;	// ���������������߂�
	boolean nameFlg = false;	// ���O�X�^�[�g

	// ���O����
	string fname, fn;
	fn = charName;
	fname = "char/" + fn + "/win.txt";

	// �Z�b�g�p
	WinText winText;
	TextBox textBox;

	// �t�@�C�����J�� //
	fp = FileRead_open(fname.c_str());//�t�@�C���ǂݍ���
	if (fp == NULL) {			// �G���[���N��������
		printfDx("error result_text\n");// ������\��
		return;
	}

	// �L�������Z�b�g
	winText.name = fn;
	// �����G�Z�b�g
	fname = "char/" + fn + "/pal/stand.png";
	winText.image = LoadGraph(fname.c_str());

	//string copy;	// string�Ƃ��Ďg���p

	// �t�@�C���I���܂�
	while (FileRead_eof(fp) == 0) {
		// ������
		inputc[0] = '\0';	// �擪��null�ɂ���
	
		FileRead_gets(inputc, TX_MAX, fp);//1�����擾����
		// ������̉��
		for (i = 0; i < TX_MAX; i++)
		{
			// �I��
			if (inputc[i] == ']' && (i > 0) && nameFlg) {
				inputc[i] = '\0';
				flg = 1;	// ���O�Z�b�g�t���O
				// inputc����蒼��
				for (int n = 0; n < i; n++) {
					// 63�ȉ��Ȃ�
					if(n < TX_MAX - 1)
						inputc[n] = inputc[n + 1];	// ���炷
				}
				break;
			}

			// 1�����ڂ�����
			if (inputc[i] == '[' && (i == 0)) {
				nameFlg = true;
			}
		}

		// ������������
		// �������
		switch (flg) {
		case 1:	// gsize
			textBox.enemyName = inputc;
			flg = 2;	// �e�L�X�g��
			nameFlg = false;
			//printfDx("flg1 ");// ������\��
			break;
		case 2:	// �e�L�X�g1
			textBox.text[0] = inputc;
			flg = 3;	// �e�L�X�g���s��
			//printfDx("flg2 ");// ������\��
			break;
		case 3:	// �e�L�X�g2
			textBox.text[1] = inputc;
			flg = -1;	// �e�L�X�g��

			// �ǉ�����
			winText.tex.push_back(textBox);

			//printfDx("flg3\n");// ������\��
			break;
		default: break;
		}
		// num��i�߂�	
	}	// while ���[�v�I��

	// �t�@�C�������
//EXFILE:
	FileRead_close(fp);

	printfDx("\n");// ������\��

	//�������Z�b�g����
	winTexts.push_back(winText);
}

// Name���擾
void GetN_Result(vector<Name_t>GN)
{
	for (int i = 0; i < Character_Max(); i++) {
		N.push_back(GN[i]);
		// �����e�L�X�g�ǂݍ���
		load_winText(N[i].nameC);
	}
}
