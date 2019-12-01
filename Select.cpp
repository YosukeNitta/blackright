//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include "vector"
//using namespace std;

//#include <list>
//using namespace std;
#include "Select.h"
using namespace modeData;

// �L�����Z���̓����Ă�L����
typedef struct Move_t{

	//int time;	// �A�j���[�V��������
	Name_t name;
	double sizeG;	// �`��T�C�Y
	Air air;	// �A�j���[�V���������Ă�

	Move_t(){
		//time = 0;
		// name
		sizeG = 0;
	}

}Move_t;

// �p���b�g
typedef struct {
public:
	int r[256];
	int g[256];
	int b[256];
}Pallet_t;


static boolean Load_1;
static boolean FLoad;	// ��x�����̃��[�h
// �L�����Z�����
static int m_P1, m_P2;
static int BMode;	// 0.�ΐ�@1.�g����  2.VSAI  3.�A�[�P�[�h

static int back;
// ����A����̉񐔁A�㉺���E�̃J�[�\���A�X�e�[�W�ԍ��A�J���[
static int kettei;
static int ketteiNum;	// ���ꂪ2�ɂȂ�����J�n�A0.�L�����Z�� 1.�X�e�[�W
static int StageNum;
static const int StageMax = STAGE_MAX;

// �X�e�[�W�摜
static int mStage[STAGE_MAX];
static int mStageB[STAGE_MAX];

// �����G�摜
//static int stand;

// �I���|�C���^�̈ʒu������

// �L�����E�J�[�\���摜
static int cursor_char;
static int side[2]; //1P, 2P

static std::vector<int>port;	// �L�����Z���ł̈ʒu
//static int PortBlank;	// �Ȃ�
static std::vector<int>charPos;	// �L�����Z���ł̈ʒu

//
static int gK[10];
static int oCurY[2];


/// ���\���p���b�g ///
static std::vector<int>color[COLOR_MAX];

// ���̂ւ��vector��
static std::vector<Move_t>move;				// ��ʓ��œ�����

static Pict pic[CHARACTERS_MAX][IMAGE2_MAX];	// 0�Ԃ̉摜
//static std::vector<Pict>pic[IMAGE2_MAX];
static Pallet_t cpal[CHARACTERS_MAX][COLOR_MAX];// �p���b�g

static void InputSelect();	// �ϐ��ESelect_t�̐ݒ�
//static void Draw();	// �Z���N�g�̕`��
static void EnterSelect();	// ����E����
static int InputSide(int PSide, int key);	// P1_BInput�̗��Ή���
static void CharAnimetion();	// �ҋ@���̃A�j��


//��{�f�[�^�̍\����
typedef struct {
	
	int posX, posY;	// �J�[�\���̕\���ʒu
	int curX, curY;	// �㉺�A���E�̍��W
	boolean enter;		// ����
	boolean charEnter;		// �L��������
	int name;		// �L������
	int color;		// �J���[

	// �����O���p
	int image;
	int time;	// �摜�̃A�j������
	int iNum;	// �摜�̉��ԍ�
}Select_t;

static Select_t P[2];
static Select_t mP;	// �_�~�[

//�L�����N�^�[�Z���N�g
int Select::Mode(void)
{
	// �v���O�����J�n
	{
		//==========
		// ����
		//==========
		{
			InputSelect();
		}
		//==========
		// �v�Z
		//==========
		{
			EnterSelect();
		}


		// �A�[�P�[�h
		// �L�����E�X�e�[�W�����肵����
		if ((P[0].charEnter) && (BMode == 3))
		{
			Arcade_GetData(P[0].name, P[0].color);	// �f�[�^�𑗂�
			Arcade_Setting();
			if (Arcade_Switch(-1) == 1){
				ModeChange(GameScene(ArcadeScene));
			}
			else{
				ModeChange(GameScene(VersusScene));	// �ΐ��ʂ�
			}
		}

		// �L�����E�X�e�[�W�����肵����
		if (ketteiNum == 2)
		{
			ModeChange(GameScene(VersusScene));	// �ΐ��ʂ�
		}

	}

	return 0;	//�I��

}



int Select::Load_Reload()
{
	Load_1 = 0;
	Anten(255);

	// �L�����Z��BGM
	BGMStart(12);

	// ����A����̉񐔁A�㉺���E�̃J�[�\���A�X�e�[�W�ԍ��A�J���[
	kettei = 0, ketteiNum = 0;
	//StageNum = 0;

	// �I���|�C���^�̈ʒu������
	for (int i = 0; i < 2; i++){
		P[i].name = 0;
		P[i].color = 1; //���ꂾ�����
		P[i].enter = false;
		P[i].charEnter = false;
		P[i].posY = 0;
	}
	mP = P[1];	// �_�~�[�p��

	return 0;
}

void Select::Load_1second(){
	// ��x����̃��[�h
	// �摜�ǂݍ���
	back = LoadGraph("ob/selectB.png");

	side[0] = LoadGraph("ob/1p.png");
	side[1] = LoadGraph("ob/2p.png");

	// �L�����E�J�[�\���摜
	cursor_char = LoadGraph("ob/selectC.png");
	// �����G
	//stand = LoadGraph("ob/stand2sample.png");

	// �L�����Z���G
	// ���O����
	std::string fname2, fn1, fn2;
	fn1 = "char/";
	//fn2 = "/pal/port_s.png";
	fn2 = "/pal/port.png";

	for (int i = 0; i < Character_Max(); i++){
		charPos.push_back(i);	// charPos
		port.push_back(0);		// port
		fname2 = fn1 + move[i].name.nameC + fn2;	// ���O����
		port[i] = LoadGraph(fname2.c_str());	// �摜���[�h
	}

	//LoadDivGraph("ob/port.png", Character_Max(), Character_Max(), 1, 100, 140, Port);

	// �X�e�[�W
	if (STAGE_MAX >= 1)mStage[0] = LoadGraph("back/st1.png");
	if (STAGE_MAX >= 2)mStage[1] = LoadGraph("back/st2.png");
	if (STAGE_MAX >= 3)mStage[2] = LoadGraph("back/st3.png");
	//if (STAGE_MAX >= 4)mStage[3] = LoadGraph("back/st4.png");

	if (STAGE_MAX >= 1)mStageB[0] = LoadGraph("back/st1_2.png");
	if (STAGE_MAX >= 2)mStageB[1] = LoadGraph("back/st2_2.png");
	if (STAGE_MAX >= 3)mStageB[2] = LoadGraph("back/st3_2.png");
	//if (STAGE_MAX >= 4)mStageB[3] = LoadGraph("back/st4_2.png");

	// �J�[�\���ʒu
	P[0].curX = 2, P[1].curX = 3;
	P[0].posX = P[0].curX * 110 - 110, P[1].posX = P[1].curX * 110 - 110;
	P[0].curY = 0, P[1].curY = 0;
	StageNum = 0;

	// �㉺�L�[
	LoadDivGraph("ob/key.png", 10, 5, 2, 20, 20, gK);

	//char fname[TX_MAX];
	// ���O�ǂݍ���
	for (int cm = 0; cm < Character_Max(); cm++){

		// �J���[����
		for (int n = 0; n < COLOR_MAX; n++)
		{
			////
			std::string fn3;
			char ch[2];
			sprintf(ch, "%d", n + 1);	// �����ɕϊ�

			fn1 = "char/";
			fn2 = "/pal/c";
			fn3 = ".bmp";

			fname2 = fn1 + move[cm].name.nameC + fn2 + ch + fn3;	// ���O����

			int tr, tg, tb, palName;
			int r, g, b;
			// RGB�Ƀp���b�g�̐F���L��
			{
				palName = LoadSoftImage(fname2.c_str());
				// �p���b�g�̐F���擾���� 1�`16, 17�`
				GetPaletteSoftImage(palName, 0, &tr, &tg, &tb, 0);

				// �F���p���b�g�ɕۑ�
				for (int p = 0; p < 256; p++){
					GetPaletteSoftImage(palName, p, &r, &g, &b, 0);
					cpal[cm][n].r[p] = r;
					cpal[cm][n].g[p] = g;
					cpal[cm][n].b[p] = b;
				}
			}

			// ���ߐF��ύX
			SetTransColor(tr, tg, tb);

			int gr = LoadGraph(fname2.c_str());
			color[cm].push_back(gr);	// �Î~�摜�Z�b�g

			////
		}

		// �L�����T�C�Y����
		move[cm].sizeG = 1.0;

		if(cm == 5)
		move[cm].sizeG = 0.8;

	}	// cm

	SetTransColor(0, 0, 0);	// ���ߐF��߂�
	///////////////////////////////////
	if (charPos.size() > 0)charPos[0] = HYDE - 1;
	if (charPos.size() > 1)charPos[1] = BOUNCER - 1;
	if (charPos.size() > 2)charPos[2] = CORNEL - 1;
	if (charPos.size() > 3)charPos[3] = HELIOS - 1;
	if (charPos.size() > 4)charPos[4] = EIN - 1;
	if (charPos.size() > 5)charPos[5] = SYUICHI - 1;
}


void Select::Draw()
{
	// �w�i
	DrawGraph(0, 0, back, true);

	int pos = (SCREEN_W / 2) - (3 * 70);

	// �L������|�g��
	for (int i = 0; i < Character_Max(); i++){
		DrawBox(pos + (i * 70), 310, pos + (i * 70) + 60, 310 + 60, GetColor(80, 80, 200), true);
		DrawGraph(pos + (i * 70), 310, port[charPos[i]], true);
		//DrawExtendGraph(SCREEN_W / 2 - 215 + (i * 110), 300, SCREEN_W / 2 - 215 + (i * 110) + 80, 300 + 80, port[charPos[i]], true);
	}

	//============
	// �L�����`��
	//============
	CharAnimetion();

	//==============
	// �J�[�\���`��
	for (int i = 0; i < 2; i++){

		// �J�[�\���F�ݒ�
		switch (i){
		case 0:
			SetDrawBright(255, 0, 0);
			break;
		case 1:
			SetDrawBright(0, 0, 255);
			break;
		}
		if ((P[0].curX == P[1].curX) && ((BMode == 0) || ((BMode == 1 || BMode == 2) && (P[0].charEnter))))
			SetDrawBright(255, 0, 255);

		// �J�[�\�� 
		if ((i == 0) || ((BMode == 0) || ((BMode == 1 || BMode == 2) && (P[0].charEnter)))){
			//DrawGraph((P[i].posX) + SCREEN_W / 2 - 3 - 216, 297,
			DrawGraph(pos + (P[i].posX) - 3, 307,
				cursor_char, true);
		}
		// �`��P�x��߂�
		SetDrawBright(255, 255, 255);

		// �v���C���[�T�C�h
		if ((i == 0) || ((BMode == 0) || ((BMode == 1 || BMode == 2) && (P[0].charEnter)))){
			if (P[i].charEnter)SetDrawBright(128, 128, 128);
			DrawGraph(pos + (P[i].posX) - 3 + (i * 34), 307 - 22,
				side[i], false);
			if (P[i].charEnter)SetDrawBright(255, 255, 255);
		}
	}

	// �J���[�ԍ�
	int cr;
	if (P[0].charEnter)cr = Oran;
	else{ cr = Cr; }

	int cy;

	// 1P
	cy = 0;
	if (oCurY[0] == -1)cy -= 2;
	DrawGraph(45, 0 + cy, gK[7], true);
	cy = 0;
	if (oCurY[0] == 1)cy += 2;
	DrawGraph(45, 40 + cy, gK[1], true);
	DrawFormatString(20, 20, cr, "color %d", P[0].color + 1);

	// 2P
	if ((BMode == 0) || ((BMode == 1 || BMode == 2) && (P[0].charEnter))){
		if (P[1].charEnter)cr = Oran;
		else{ cr = Cr; }

		cy = 0;
		if (oCurY[1] == -1)cy -= 2;
		DrawGraph(SCREEN_W - 65, 0 + cy, gK[7], true);
		cy = 0;
		if (oCurY[1] == 1)cy += 2;
		DrawGraph(SCREEN_W - 65, 40 + cy, gK[1], true);
		DrawFormatString(SCREEN_W - 90, 20, cr, "color %d", P[1].color + 1);
	}

	//==================
	// �X�e�[�W�Z���N�g
	//==================
	if (ketteiNum == 1){

		//DrawFormatString(220, 70, Cr, "�X�e�[�W %d", StageNum + 1);
		//DrawString(220, 70, "�X�e�[�W�I��", Cr);
		switch(StageNum){
		case 0:
			DrawString(220, 70, "�O���b�h", Cr);
			break;
		case 1:
			DrawString(220, 70, "�r��", Cr);
			break;
		case 2:
			DrawString(220, 70, "���", Cr);
			break;
		case 3:
			DrawString(220, 70, "���Z��", Cr);
			break;
		}

		// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

		// ���E�̃X�e�[�W
		if (StageNum < STAGE_MAX - 1){
			//DrawRotaGraph(420, 180, 0.1, 0, mStage[StageNum + 1], false, 0);
			// �w�i
			if (mStage[StageNum] != 0)
				DrawRotaGraph(420, 180, 0.27 * 0.6, 0, mStageB[StageNum + 1], false, 0);
			// �X�e�[�W
			if (mStage[StageNum] != 0)
				DrawRotaGraph(420, 200, 0.169 * 0.6, 0, mStage[StageNum + 1], true, 0);
		}
		if (StageNum > 0){
			//DrawRotaGraph(220, 180, 0.1, 0, mStage[StageNum - 1], false, 0);
			// �w�i
			if (mStage[StageNum] != 0)
				DrawRotaGraph(220, 180, 0.27 * 0.6, 0, mStageB[StageNum - 1], false, 0);
			// �X�e�[�W
			if (mStage[StageNum] != 0)
				DrawRotaGraph(220, 200, 0.169 * 0.6, 0, mStage[StageNum - 1], true, 0);
		}
		// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �w�i
		if (mStage[StageNum] != 0)
			DrawRotaGraph(320, 180, 0.27, 0, mStageB[StageNum], false, 0);
		// �X�e�[�W
		if (mStage[StageNum] != 0)
			//DrawRotaGraph(320, 180, 0.18, 0, mStage[StageNum], true, 0);
			DrawRotaGraph(320, 200, 0.169, 0, mStage[StageNum], true, 0);

	}

	// �Ó]
	if (AntenCheck()){
		Anten(0);
	}

}


void Load_Select()
{
	Load_1 = 0;
	Anten(255);
}

// P1�AP2�̖��O��Ԃ�
int Player_Name_1()
{
	return P[0].name;
}

int Player_Name_2()
{
	return P[1].name;
}

void Select_SetName(int p1, int p2)
{
	P[0].name = p1;
	P[1].name = p2;
}

int BattleMode(int f)
{
	if (f != -1)BMode = f;
	return BMode;
}

void InputSelect()
{

	// P0,1������
	for (int i = 0; i < 2; i++){
		// �O�̃J�[�\���̈ʒu
		oCurY[i] = 0;
		if (!AntenCheck()){
			// �L���������܂��ĂȂ�
			if (!P[i].charEnter){
				// �J���[���㉺��
				if (KInput(InputSide(i, 108))){
					P[i].curY -= 1;
					SEStart(36);
					oCurY[i] = -1;
				}
				else if (KInput(InputSide(i, 102))){
					P[i].curY += 1;
					SEStart(36);
					oCurY[i] = 1;
				}
			}
		}
		// �㉺���
		if (P[i].curY < 0){
			P[i].curY = (COLOR_MAX - 1);
		}
		else if (P[i].curY >(COLOR_MAX - 1)){
			P[i].curY = 0;
		}

		if (!AntenCheck()){
			// ������Ȃ�A�L�����J�[�\�����E��
			if ((ketteiNum == 0) && (!P[i].charEnter)){
				if (KInput(InputSide(i, 104))){
					P[i].curX -= 1;
					SEStart(36);	// �ړ��̉�
					P[i].time = 0;
				}

				else if (KInput(InputSide(i, 106))){
					P[i].curX += 1;
					SEStart(36);
					P[i].time = 0;
				}
			}
		}


		// ���E���
		if (P[i].curX < 0){
			P[i].curX = Character_Max() - 1;
			P[i].posX = (Character_Max() - 1) * 70;
		}
		else if (P[i].curX > Character_Max() - 1){
			P[i].curX = 0;
			P[i].posX = 0;
		}

		// �J�[�\���\���ړ�
		if (P[i].posX != P[i].curX * 70){
			if (P[i].posX < P[i].curX * 70)
			P[i].posX += 10;
			if (P[i].posX > P[i].curX * 70)
				P[i].posX -= 10;
		}
		// ����
		if (InputSide(i, 1) == 1 || InputSide(i, 3) == 1){
			kettei = 1;
			P[i].enter = true;
		}
		else{
			P[i].enter = false;
		}
		// �����_��
		if ((ketteiNum == 0) && (InputSide(i, 4) == 1) && (!Connect_CheckCn()) && (!P[i].charEnter)){
			kettei = 1;
			P[i].curX = GetRand(Character_Max() - 1);
			P[i].curY = GetRand(COLOR_MAX - 1);
			P[i].enter = true;
			SEStart(36);
		}

		// �L�����Z��
		if (InputSide(i, 2) == 1)
		{
			if (ketteiNum > 0){
				ketteiNum -= 1;
			}
			// ���j���[�ɖ߂� or �L�����������
			else if (ketteiNum == 0 && !P[i].charEnter){
				ModeChange(GameScene(MenuScene));
			}
			SEStart(37);
			P[0].charEnter = false;	// �L�������������
			P[1].charEnter = false;
		}

		// ������Ȃ�A�J���[����
		if (!P[i].charEnter)
		{
			P[i].color = P[i].curY;
		}
	}
	// �I��
}


void EnterSelect()
{
	// �ΐ�E�g�������L
	{

		for (int i = 0; i < 2; i++){
			// �L��������
			if ((ketteiNum == 0) && (kettei) && (P[i].enter) && (!P[i].charEnter))
			{
				// �L�������I�ׂ�
				if (P[i].curX < Character_Max()){
					
					//================
					//P[i].name = P[i].curX + 1;
					
					switch (P[i].curX){
					case 0:
						P[i].name = HYDE;
						break;
					case 1:
						P[i].name = BOUNCER;
						break;
					case 2:
						P[i].name = CORNEL;
						break;
					case 3:
						P[i].name = HELIOS;
						break;
					case 4:
						P[i].name = EIN;
						break;
					default:
						P[i].name = P[i].curX + 1;
						break;
					}
					//=================

					// �J���[����
					{
						// �܂������������܂��ĂȂ�
						if ((i == 0 && (!P[1].charEnter)) ||
							(i == 1 && (!P[0].charEnter))){
							//printfDx("Ok1\n");
							Get_Color(P[i].color, i + 1);		// �J���[����
						}
						// ���������I���ς�
						else{
							// �L����������
							if (P[0].name == P[1].name){
								if (P[0].color == P[1].color){
									if (P[i].color == 0){
										P[i].color = 1;
										Get_Color(P[i].color, i + 1);
									}
									else{
										P[i].color = 0;
										Get_Color(P[i].color, i + 1);
									}
								}
								else{
									//printfDx("Same\n");
									Get_Color(P[i].color, i + 1);		// �J���[����
								}
							}
							// �Ⴄ
							else{
								// printfDx("Ok2\n");
								Get_Color(P[i].color, i + 1);		// �J���[����
							}
						}
					}
					// �J���[�I��
					SEStart(35);	// ����
					P[i].charEnter = true;
				}

				if (P[0].charEnter && P[1].charEnter)kettei = 0;
			}
			// if�I��
		}
	}
	// �g�����ŏI��

	// �������ł�����A�X�e�[�W�Z���N�g�ֈڍs
	if ((P[0].charEnter && P[1].charEnter) && (ketteiNum == 0)){
		ketteiNum = 1;
	}
	
	// �������狤�ʑ���
	for (int i = 0; i < 2; i++){
		// �X�e�[�W����
		if ((ketteiNum == 1) && (InputSide(i, 106) == 1)){
			StageNum += 1;
			SEStart(36);
		}
		if ((ketteiNum == 1) && (InputSide(i, 104) == 1)){
			StageNum -= 1;
			SEStart(36);
		}
	}
		// ���������߂�
		if (StageNum < 0){
			StageNum = StageMax - 1;
		}
		else if (StageNum > StageMax - 1){
			StageNum = 0;
		}

		// ���肵����
		if ((ketteiNum == 1) && (kettei))
		{
			SEStart(35);
			
			GetStageNum(StageNum, mStage[StageNum]);	// �X�e�[�W�󂯎��
			Versus_bgmNum(StageNum + 1);
			
			// �ΐ펞�̂݃��v���C�ݒ�
			if ((BMode == 0) && (GetPrivateProfileInt("Config", "replay", 0, "./config.ini") == 1)){
				Replay_Mode(2);
				// �Đ�Őݒ�͕ς��Ȃ��̂ŁA���������ł���
				Replay_Setting(P[0].name, P[1].name, P[0].color, P[1].color, StageNum);
			}
			else{ Replay_Mode(0); }

			ketteiNum += 1;
			kettei = 0;
		}

}

// �A�j���[�V�����Z�b�g���`��
void CharAnimetion()
{
	int bTime, num;
	int homo[2];
	homo[0] = 0, homo[1] = 0;

	// �A�j���[�V����ver3
	for (int i = 0; i < 2; i++){
		P[i].time++;	// ����x�ɃA�j�����ԉ��Z
		bTime = 0;
		P[i].iNum = 0;
		for (num = 0; num < STATE2_MAX - 1; num++){
			bTime += move[charPos[P[i].curX]].air.B[num].time;	// �A�j�����Ԃ����Z
			// ���Ԃ������Ă��違��������
			if ((bTime <= P[i].time)
				&& (move[charPos[P[i].curX]].air.B[num].time > -2))
			{
			}
			else{ 
				break; 
			}
		}
		// num2�Ԃ�����
		homo[i] = move[charPos[P[i].curX]].air.B[num].num2;
		// �ł���������
		if (move[charPos[P[i].curX]].air.B[num].time == -2)P[i].time = -1;

		// �p���b�g
		for (int n = 0; n < 256; n++){
			SetPaletteSoftImage(pic[charPos[P[i].curX]][homo[i]].image, n,
				cpal[charPos[P[i].curX]][P[i].color].r[n],
				cpal[charPos[P[i].curX]][P[i].color].g[n],
				cpal[charPos[P[i].curX]][P[i].color].b[n], 0);
		}
		
		SetTransColor(
			cpal[charPos[P[i].curX]][P[i].color].r[0],
			cpal[charPos[P[i].curX]][P[i].color].g[0], 
			cpal[charPos[P[i].curX]][P[i].color].b[0]);
		// �摜���Z�b�g
		if(P[i].image != 0)DeleteGraph(P[i].image);
		// �摜����
		P[i].image = CreateGraphFromSoftImage(pic[charPos[P[i].curX]][homo[i]].image);
		SetTransColor(0,0,0);
	}

	// �A�j���[�V�����ݒ�
	/*
	for (int i = 0; i < 2; i++){
		move[i].time++;
		bTime = 0;
		for (num = 0; num < STATE2_MAX - 1; num++){
			bTime += move[charPos[P[i].curX]].air.B[num].time;
			// ���Ԃ������Ă��違��������
			if ((bTime <= move[i].time++) &&
				(move[charPos[P[i].curX]].air.B[num + 1].time > -2)){
			}
			else{ break; }
		}
		if (move[charPos[P[i].curX]].air.B[num + 1].time == -2)move[i].time++, num = 0;
	}
	*/

	// �A�j�����e
	//int num2[2];

	/*
	// �A�j���[�V����
	for (int i = 0; i < 2; i++){
		bTime = 0;
		for (num[i] = 0; num[i] < STATE2_MAX - 1; num[i]++){
			bTime += move[charPos[P[i].curX]].air.B[num[i]].time;
			// ���Ԃ������Ă��違��������
			if (//(bTime <= move[i].time) &&
				(move[charPos[P[i].curX]].air.B[num[i] + 1].time > -2)){
			}
			//
			else{ break; }
		}
		// �A�j������
		move[i].time++;
		if (move[charPos[P[i].curX]].air.aTime >= move[i].time){
			move[i].time = 0;
		}
	}
	*/

	/// �L�����N�^�[�`�� ///
	// ����
	int xs = 0, ys = 0;	// x,y�̃T�C�Y�擾
	GetGraphSize(color[charPos[P[0].curX]][P[0].color], &xs, &ys);	// �T�C�Y
	
	DrawRotaGraphF(
		//52.0 + ((double)xs / 2.0) - (double)pic[charPos[P[0].curX]][homo[0]].posX,
		52.0 + ((double)xs / 2.0)
			- (double)pic[charPos[P[0].curX]][homo[0]].posX + (double)move[charPos[P[0].curX]].air.B[homo[0]].posX,
		220.0 + ((double)ys / 2.0)
			- (double)pic[charPos[P[0].curX]][homo[0]].posY + (double)move[charPos[P[0].curX]].air.B[homo[0]].posY,
		move[P[0].curX].sizeG, 0,
		P[0].image,
		true, 0);

	// 2P��
	if ((BMode == 0) || ((BMode == 1 || BMode == 2) && (P[0].charEnter))){
		GetGraphSize(color[charPos[P[1].curX]][P[1].color], &xs, &ys);	// �T�C�Y

		DrawRotaGraphF(
			//52.0 + ((double)xs / 2.0) - (double)pic[charPos[P[0].curX]][homo[0]].posX,
			((double)SCREEN_W - 52.0) - ((double)xs / 2.0)
			+ (double)pic[charPos[P[1].curX]][homo[1]].posX - (double)move[charPos[P[1].curX]].air.B[homo[1]].posX,
			220.0 + ((double)ys / 2.0)
			- (double)pic[charPos[P[1].curX]][homo[1]].posY + (double)move[charPos[P[1].curX]].air.B[homo[1]].posY,
			move[P[1].curX].sizeG, 0,
			P[1].image,
			true, 1);
	}

}

int InputSide(int PSide, int key)
{
	int n;

	// �ΐ�
	if (BMode == 0){
		if (PSide == 0)
			n = P1_BInput(key);
		else{
			n = P2_BInput(key);
		}
	}
	else if (BMode == 1){
		if (PSide == 0)
			n = P1_BInput(key);
		// 2P��
		else{
			if ((ketteiNum == 0) && (P[0].charEnter)){
				n = P1_BInput(key);
			}
			else{ n = 0; }
		}
	}
	else if (BMode == 2){
		if (PSide == 0)
			n = P1_BInput(key);
		// 2P��
		else{
			if ((ketteiNum == 0) && (P[0].charEnter)){
				n = P1_BInput(key);
			}
			else{ n = 0; }
		}
	}
	else if (BMode == 3){
		if (PSide == 0)
			n = P1_BInput(key);
		// 2P��
		else{
			if ((ketteiNum == 0) && (P[0].charEnter)){
				n = P1_BInput(key);
			}
			else{ n = 0; }
		}
	}

	return n;
}

void Select_GetName(std::vector<Name_t>GN){
	for (int i = 0; i < Character_Max(); i++){
		move.push_back(Move_t());
		move[i].name = GN[i];
	}
}

// air���擾
void GetA_Select(Air GA, int name)
{
	// AIR�ϐ����� //
	// �����������
	{
		move[name].air.setup();
		move[name].air = GA;
		
		// �A�j������
		int n = 0;
		move[name].air.aTime = 0;
		while (move[name].air.B[n].time > 0){
			move[name].air.aTime += move[name].air.B[n].time;
			n++;
		}
	}
}

// pict�擾
void GetPI_Select(Pict GPI[IMAGE2_MAX], int name)
{
	for (int i = 0; i < IMAGE2_MAX; i++){
		pic[name][i] = GPI[i];
	}
}

// �J�[�\���ʒu��߂�
void Select_ReturnCursor()
{
	if (Character_Max() > 2){
		P[0].curX = 1, P[1].curX = 2;
	}
	else{
		P[0].curX = 0, P[1].curX = 0;
	}
}