#include "pch.h"
#include "MainSystem.h"
#include "Network.h"
#include "NetData.h"

using namespace std;	// string��std���ȗ�

static NetData *netData;	// ���̏ꏊ�ł��g����f�[�^

// �ڑ��m�F�pbyte�f�[�^
// 0:�Ȃ� 1:�ڑ��҂� 2:�ڑ�����
static BYTE Data[2];	
static BYTE getData[2];
static int connectTest;	// �ʐM���s��ꂽ��

static boolean nCheck;

static const int HOST = 1;		// �z�X�g�ڑ��O
static const int CLIENT = 2;	// �N���C�A���g�ڑ��O

// ���ڍő吔
static const int SP_MAX = 2;
static int sPoint;	// ��ʂ̐i�ݓx����
// 0.���ڑI��
static int yPoint;	// ���ڂ̈ʒu
static boolean load_f = false;


static void Input();
// net.txt�̃f�[�^���擾����
static void GetPortIp();

// ���C���֐�
int Network::Mode()
{
	Input();

	// �ڑ�
	if (sPoint == HOST || sPoint == CLIENT) {
		int ret = netData->connect_Test();
		// 1���Ԃ��ꂽ��
		if (ret == 1) {

			// netData��Socket�ɑ���
			ChangeNetOn(1);	// �l�b�g���[�N���I��
			Main_GetNetData(netData);

			Key_SetNetOn(1);
			Key_SetPlayerSide(sPoint);

			// �ΐ탂�[�h�Ɉڍs���邽�߂̐ݒ�
			Select_ReturnCursor();
			TraningSwitch(false);
			AISwitch(false);
			Arcade_Switch(0);
			BattleMode(0);
			//DrawString(0, 260, "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||", Cr);
			MainSystem::Instance().SetNextMode("Select");
		}
	}

	//////////////////
	Draw();	// �`��

	// �I��
	if ((sPoint == 0) && (yPoint == SP_MAX) && (P1_BInput(1) == 1 || P1_BInput(2) == 1 || P1_BInput(3) == 1)){
		SEStart(37);
		MainSystem::Instance().SetNextMode("Menu");
	}

	return 0;
}


void Input(){
	// �I����ʁH
	if (sPoint == 0){
		if (P1_BInput(102) == 1){
			yPoint++;
			SEStart(36);
		}
		else if (P1_BInput(108) == 1){
			yPoint--;
			SEStart(36);
		}

		// �ő�ŏ���
		if (yPoint < 0)yPoint = SP_MAX;
		else if (yPoint > SP_MAX)yPoint = 0;

		// �z�X�g��
		if (yPoint == 0 && (P1_BInput(1) == 1 || P1_BInput(3) == 1)){
			// �l�b�g���[�N�e�X�g
			//netData->setPort(7500);
			
			netData->MakeUDPNetHandle();
			netData->connect_Reset();
			netData->setPlayerSide(1);
			Data[0] = 1;
			Data[1] = 0;
		}
		// �N���C�A���g��
		else if (yPoint == 1 && (P1_BInput(1) == 1 || P1_BInput(3) == 1)){
			// �l�b�g���[�N�e�X�g
			//netData->setPort(7600);
			
			netData->MakeUDPNetHandle();	// �w�肵���|�[�g�ō쐬
			netData->connect_Reset();
			netData->setPlayerSide(2);
			Data[0] = 0;
			Data[1] = 1;
		}

		// ���莞�̏���
		if (yPoint < 2 && ((P1_BInput(1) == 1 || P1_BInput(3) == 1))) {
			sPoint = yPoint + 1;
			yPoint = 0;
			SEStart(35);
		}

		// �I����
		if (P1_BInput(2) == 2){
			sPoint = 0;
			yPoint = SP_MAX;
			SEStart(37);
		}
	}
	else if (sPoint == HOST){
		// �I����
		if (P1_BInput(2) == 2){
			sPoint = 0;
			yPoint = 0;
			// �t�c�o�\�P�b�g�n���h���̍폜
			netData->DeleteUDPNetHandle();
			netData->setPlayerSide(0);
			for (int i = 0; i < 2; i++) {
				Data[i] = 0;
				getData[i] = 0;
			}
			SEStart(37);
		}
	}
	else if (sPoint == CLIENT){
		// �I����
		if (P1_BInput(2) == 2){
			sPoint = 0;
			yPoint = 1;
			// �t�c�o�\�P�b�g�n���h���̍폜
			netData->DeleteUDPNetHandle();
			netData->setPlayerSide(0);
			for (int i = 0; i < 2; i++) {
				Data[i] = 0;
				getData[i] = 0;
			}
			SEStart(37);
		}
	}
}

void Network::Draw(){
	// �|�C���^�[
	DrawBox(0, yPoint * 20, 20, (yPoint + 1) * 20, Cr, true);
	// �e�X�g�p�ʒu�\��
	//DrawFormatString(420, 240, Cr, "S..%d", sPoint);
	//DrawFormatString(420, 260, Cr, "X..%d", xPoint);
	//DrawFormatString(420, 280, Cr, "Y..%d", yPoint);
	//DrawFormatString(0, SCREEN_H - 40, Cr, "1P..%d, 2P..%d", Data[0], Data[1]);	// �ڑ��m�F
	//netData->DataDraw(240, 40);

	// 1P��2P�̏󋵕\��
	if (Data[0] == 2)
		DrawString(0, SCREEN_H - 40, "1P..����", Cr);
	else if (Data[0] == 1)
		DrawString(0, SCREEN_H - 40, "1P..�ڑ��҂�", Cr);
	else if(Data[0] == 0){
		DrawString(0, SCREEN_H - 40, "1P..��~", Cr);
	}

	if (Data[1] == 2)
		DrawString(120, SCREEN_H - 40, "2P..����", Cr);
	else if (Data[1] == 1)
		DrawString(120, SCREEN_H - 40, "2P..�ڑ��҂�", Cr);
	else if(Data[1] == 0){
		DrawString(120, SCREEN_H - 40, "2P..��~", Cr);
	}

	// �ʐM���s��ꂽ��
	DrawFormatString(320, SCREEN_H - 40, Cr, "�ʐM..%d", connectTest);

	if (sPoint == 0){
		DrawString(20, 20 * 0, "�z�X�g", Cr);
		DrawString(20, 20 * 1, "�N���C�A���g", Cr);
		/*
		// IP���l
		for (int i = 0; i < ip_x; i++){
			for (int j = 0; j < ip_y; j++){
				DrawFormatString(20 + (j * 10) + (i * 40), 20 * 2, Cr, "%d", ip[i][j]);
			}
			if (i != 3)DrawString(20 + (i * 40) + 30, 20 * 2, ".", Cr);
		}
		// �|�[�g���l
		for (int i = 0; i < 5; i++){
			DrawFormatString(20 + (i * 10), 20 * 3, Cr, "%d", port[i]);
		}
		*/
		DrawString(20, 20 * 2, "�I��", Cr);

		// ���ڂ̐���
		switch (yPoint){
		case 0:
			DrawString(80, 20 * 6, "�������ڑ���҂�", Cr);
			break;
		case 1:
			DrawString(80, 20 * 6, "�������ڑ�����", Cr);
			break;
		case SP_MAX:
			DrawString(80, 20 * 6, "�I������", Cr);
			break;
		}
	}
	else if (sPoint == HOST) {
		DrawString(20, 20 * 0, "�z�X�g", Cr);
		if (!nCheck)DrawString(20, 20 * 1, "�ҋ@��", Cr);
		else { DrawString(20, 20 * 1, "�ڑ����܂���", Cr); }
		DrawFormatString(20, 20 * 2, Cr, "%d", netData->getPort());
		DrawFormatString(20, 20 * 3, Cr, "�N���C�A���g %d.%d.%d.%d",
			netData->ip.d1, netData->ip.d2, netData->ip.d3, netData->ip.d4);
		//DrawFormatString(20, SCREEN_H - 40, Cr, "%d.%d", Data[0], Data[1]);
	}
	else if (sPoint == CLIENT) {
		DrawString(20, 20 * 0, "�N���C�A���g", Cr);
		if (!nCheck)DrawString(20, 20 * 1, "�ҋ@��", Cr);
		else { DrawString(20, 20 * 1, "�ڑ����܂���", Cr); }
		// �|�[�g���ꏏ�ɕ\��
		DrawFormatString(20, 20 * 3, Cr, "%d.%d.%d.%d:%d",
			netData->ip.d1, netData->ip.d2, netData->ip.d3, netData->ip.d4, netData->getPort());
		//DrawFormatString(20, SCREEN_H - 40, Cr, "%d.%d", Data[0], Data[1]);
	}

	// �f�[�^�\��
	netData->DataDraw(10,320);
}


int Network::End() {
	return 0;
}

void Network::Load_Reload() {
	if (!load_f) {
		// �����ɓ���
		load_f = true;
	}

	sPoint = 0;
	yPoint = 0;
	nCheck = false;

	// �擾�f�[�^��������
	for (int i = 0; i < sizeof(Data); i++) {
		Data[i] = 0;
		getData[i] = 0;
	}

	// �|�[�g�AIP�擾
	GetPortIp();

	// �l�b�g�f�[�^��������
	//netData->GetIp(Ip);
	//netData->GetPort(Port);
	//netData->GetUDPNetHandle(UDPNetHandle);

	// �ʐM�e�X�g
	connectTest = 0;
}

// ��x�ǂݍ���
void Network::Load_1second() {
	netData = new NetData();
}

void Network::Release(void)
{
}

void GetPortIp()
{
	// ����Z�b�e�B���O //

	int i, fp;

	char fname[8] = { "net.txt" };

	// ���O�ǂݍ���
	char t_port[5] = "PORT";
	char t_ip[3] = "IP";
	char t_delay_max[10] = "DELAY_MAX";
	IPDATA Ip = {0,0,0,0};	// ����p

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// input��inputc�ɕ������͂���
	boolean iflg = false;	// ���O���̓t���O
	int flg = 0;	// 0:�Ȃ� 1:PORT 2:IP
	int ip_flg = 0;

	// �t�@�C�����J�� //
	fp = FileRead_open(fname);//�t�@�C���ǂݍ���
	if (fp == NULL) {			// �G���[���N��������
		printfDx("error name\n");// ������\��
		return;
	}

	while (1) {
		//=========================
		// ��{���[�v
		// ���̃��[�v�I����ɉ���
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1�����擾����

			// �X���b�V���ŉ��s
			if (inputc[i] == '/') {					//�X���b�V���������
				while (FileRead_getc(fp) != '\n');	//���s�܂Ń��[�v
				i = -1;//�J�E���^���ŏ��ɖ߂��� (�߂����Ƃ���0�ɂȂ�)
				continue;
			}

			// ���s�Ŏ��̍s�ɐi��
			if (input[i] == '\n') {
				for (int j = 0; j < NAME_MAX; j++) {
					//inputc[j] = input[j] = NULL;
					inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				}
				i = -1;
				break;
			}

			// ������ɂ���
			if (input[i] == '.' || input[i] == ' ' ||
				input[i] == ';' || input[i] == '=') { //�J���}�����s�Ȃ�
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				break;	// i��1���₵�ă��[�v�E�o�i�������j
			}

			// �ǂݍ��ݏI��
			if (input[i] == EOF) {//�t�@�C���̏I���Ȃ�
				goto EXFILE;//�I��
			}
		}	// ��{���[�v�I��

		// �ŏ��ɐ��������ɂ��āA�����������Ă���ύX����
		iflg = false;

		// ������Z�b�g�Ɉڍs���Ă���
		if (flg > 0)
		{
			if (inputc[0] == '0')iflg = true;
			else if (inputc[0] == '1')iflg = true;
			else if (inputc[0] == '2')iflg = true;
			else if (inputc[0] == '3')iflg = true;
			else if (inputc[0] == '4')iflg = true;
			else if (inputc[0] == '5')iflg = true;
			else if (inputc[0] == '6')iflg = true;
			else if (inputc[0] == '7')iflg = true;
			else if (inputc[0] == '8')iflg = true;
			else if (inputc[0] == '9')iflg = true;
		}

		// ���O��������
		if (iflg) {
			// PORT
			if (flg == 1) {
				int port = atoi(inputc);
				netData->setPort(port);
				flg = 0;
			}
			// IP
			if (flg == 2) {
				// ��ɃZ�b�g
				int atoiNum = atoi(inputc);
				// �͈�
				if (atoiNum < 0)atoiNum = 0;
				else if (atoiNum > 255)atoiNum = 255;

				if (ip_flg == 1)Ip.d1 = atoiNum;
				if (ip_flg == 2)Ip.d2 = atoiNum;
				if (ip_flg == 3)Ip.d3 = atoiNum;
				if (ip_flg == 4)Ip.d4 = atoiNum;
				ip_flg++;
				// �S�ďI��
				if (ip_flg > 4) {
					netData->setIp(Ip);
					flg = 0, ip_flg = 0;
				}
			}
			// DELAY_MAX
			if (flg == 3) {
				int delayMax = atoi(inputc);
				netData->setDelayMax(delayMax);
				flg = 0;
			}
		}

		// �����ł͂Ȃ�
		if (!iflg) {
			// "PORT"
			if (strcmp(inputc, t_port) == 0) {
				flg = 1;
			}
			// "IP"
			if (strcmp(inputc, t_ip) == 0) {
				flg = 2;
				ip_flg = 1;
			}
			// "IP"
			if (strcmp(inputc, t_delay_max) == 0) {
				flg = 3;
			}
		}
	}

	// �t�@�C�������
EXFILE:
	FileRead_close(fp);

}

