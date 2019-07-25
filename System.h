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
bool Update();	//FPS�X�V
void Draw();	//FPS��\��
void Wait();	//

/**********
* Key
**********/

void GetInput();

int P_Input(int n);
// �{�^�����̓`�F�b�N
int P1_BCheck();
int P2_BCheck();
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

void Get_Network(boolean net);
void Network_Setting(int side, IPDATA ip, int port, int handle);
void NB_Reset();


void End_Network();

// 0.�Ȃ� 1.�Đ� 2.�^��
int Replay_Mode(int num);
void Replay_End();
void Replay_Setting(int n1, int n2, int c1, int c2, int stageNum);
int Replay_ReserveKey();
void Replay_EnterSetting(int n1, int n2, int c1, int c2, int stageNum);
void Key_GetString(std::string st);

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
// ����d�Ȃ��Ă邩�`�F�b�N
bool Box_Touch(int x1, int y1, int w1, int h1,
	int x2, int y2, int w2, int h2);

//-------
// Connect
//-------
// �l�b�g���[�N�擾
void Connect_GetCn(boolean net);
// �l�b�g���[�N�m�F
boolean Connect_CheckCn();
// �v���C���[�T�C�h�m�F
void Connect_GetSide(int num);
int Connect_CheckSide();
// �|�[�g�m�F
void Connect_GetPort(int num);
int Connect_CheckPort();
// �n���h���擾
void Connect_GetNetHandle(int num);
int Connect_CheckNetHandle();
// IP�擾
void Connect_GetIp(IPDATA num);
IPDATA Connect_CheckIp();

// �f�B���C
int Connect_CheckDelay();
void Delay_Check();
void Delay_Draw();
boolean Delay_IfEnd();

// ����֘A
void Connect_Get_GetData(BYTE num, BYTE num1, BYTE num2);
BYTE Connect_CheckGetData(int num1, int num2);

void Connect_Load();
void Connect_SetGameMode();
boolean Connect_WaitGameMode();

//-------------
// Def
//-------------
void GetDef();
// �Q�[�����̃L��������Ԃ�
int Character_Max();
// �o�^�ԍ��̖��O��Ԃ�
Name_t Character_Name(int num);

#endif