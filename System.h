//===============================================================================
//
//			�i���Q�[���@�@�@�`��w�b�_��
//
//===============================================================================
#include "SSet.h"
#include "PSet.h"
#include "vector"
#include <string>

#ifndef DEF_SYSTEM_H	//��x��`����Ă�����A��`���Ȃ�
#define DEF_SYSTEM_H

class Def_c{
public:
	int charMax;
	std::vector<Name_t>names;

};

/**********
* FPS
**********/
// �N���X������̂ł���Ȃ�

//bool Update();	//FPS�X�V
//void Draw();	//FPS��\��
//void Wait();	//

/**********
* Key
**********/

void GetInput();

int P_Input(int n);
// �{�^�����̓`�F�b�N
int P1_BCheck();
int P2_BCheck();
// P1P2��BCheck�����s
void Key_Setup();
void InputSend();		// ���͂�State�ɑ���

// �{�^�����͔��f
int P1_BInput(int P1_BNum);
int P2_BInput(int P2_BNum);
int PH_BInput(int PSide, int P_Num);
int P_BInput(int P_Num);
boolean KInput(int num);

int P1_BConfig();

int P1_BSet(int num);
int P2_BSet(int num);

void P2_Auto(int i);
void P2Ctrl();
void P2Ctrl2();
void P2Ctrl3(int num[8]);
void P2Ctrl3_2();
void P2_AI(Player GP1, Player GP2);
void Stop_P2_B();

void GetPH_Key(Player GP);
void GetP_Key(Player GP1, Player GP2);
void GetS_Key(System_t GS);

// network�֘A
void Key_SetPlayerSide(int n);
int Key_GetPlayerSide();

void Key_SetNetOn(boolean flg);
boolean Key_GetNetOn();

void Key_SetupSendKey();
// �����1�t���[�����߂�
void Key_Return1Frame();
void Key_Return1VFrame();
void set_stopVB(boolean flg);

void Key_SetGKeyLog(BYTE gKeyLog[KEYLOG_MAX]);
void Key_SetSendKey(BYTE myKeyLog[KEYLOG_MAX]);
// �l�b�g�ΐ�L�[�ݒ菀��
int Key_NetButtonUpdate();
// �l�b�g�ΐ�L�[�ݒ茈��
int Key_NetButtonEnter();

// P1_B�̒l�Z�b�g
void setP1_B(int num, int value);
void setP2_B(int num, int value);

// 0.�Ȃ� 1.�Đ� 2.�^��
int Replay_Mode(int num);
void Replay_End();
void Replay_Setting(int n1, int n2, int c1, int c2, int stageNum);
int Replay_ReserveKey();
void Replay_EnterSetting(int n1, int n2, int c1, int c2, int stageNum);
void Key_GetString(std::string st);
// ���v���C�̐i�s�l��Ԃ�
int GetRTimeCount();

void InputDevice_Start();
int InputDevice_Update();
void InputDevice_Draw();

// �Z�b�g����Ă���{�^����Ԃ�
int Key_ReturnButton(int player, int button);
// ���͂����l�����蓖�Ă�
void Key_SetButton(int player, int button, int input);
/**********
* Load
**********/

int LoadAllFile();

//-------
// Order
//-------
// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
void SetAlpha(int n);

// �`��u�����h���[�h���m�[�u�����h�ɂ���
void NoBlend();

// �ȈՈÓ]�\��
void Anten(int num);
boolean AntenCheck();

// �t�F�[�h�A�E�g
void Fadeout(int num);
// �t�F�[�h�A�E�g�m�F
boolean FadeoutCheck();
// �X�N�V��
void ScreenShot();
// �t�H���g�ύX
static void SetFont();
// ��ʂ�ۑ�(�X�N�V���Ƃ͕�)
void SaveScreen();
// �ۑ���ʂ�\��
void Draw_SaveScreen();
// ���� (999�܂ł̐��Ŋm�����w��)
boolean random(int num);
// ����d�Ȃ��Ă邩�`�F�b�N
bool Box_Touch(int x1, int y1, int w1, int h1,
	int x2, int y2, int w2, int h2);

//-------------
// Def
//-------------
void GetDef();
// �Q�[�����̃L��������Ԃ�
int Character_Max();
// �o�^�ԍ��̖��O��Ԃ�
Name_t Character_Name(int num);

#endif