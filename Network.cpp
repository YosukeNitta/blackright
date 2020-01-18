
#include "pch.h"

static IPDATA Ip;	// �O���[�o��?
static BYTE Data[2];	// *2�Ȃ̂�1P2P��
static int UDPNetHandle;        // �l�b�g���[�N�n���h��
static int port[5];
static int Port;
static boolean nCheck;

static const int ip_x = 4;
static const int ip_y = 3;
static int ip[ip_x][ip_y];	// IP([4]�ɐ���,[3]���s)
//============
// TCP
//static int NetHandle, LostHandle;    // �l�b�g���[�N�n���h��
static int DataLength;            // ��M�f�[�^�ʕۑ��p�ϐ�
//============

static const int HOST = 1;		// �z�X�g�ڑ��O
static const int CLIENT = 2;	// �N���C�A���g�ڑ��O
static const int IP_SET = 3;
static const int PORT_SET = 4;

static int sPoint;	// ��ʂ̐i�ݓx����
// 0.���ڑI��
static int yPoint;	// ���ڂ̈ʒu
static int xPoint;	//
static boolean load_f = false;


static void Input();
static void Draw();

// ���C���֐�
int Network()
{
	Input();

	//////////////////
	// �f�B���C�`�F�b�N
	//if (Connect_CheckCn()){

	//}
	// �z�X�g
	if (sPoint == HOST){

		int cnwr_ret = CheckNetWorkRecvUDP(UDPNetHandle);
		// �f�[�^����M
		if (cnwr_ret && cnwr_ret != -1){

			// ��Ƀf�[�^�󂯎��
			NetWorkRecvUDP(UDPNetHandle, &Ip, NULL, Data, sizeof(Data), FALSE);	// IP�擾����

			// ����Ԃ�
			Data[0] = 2;
			NetWorkSendUDP(UDPNetHandle, Ip, Port, Data, sizeof(Data));	// �Ԃ�

			nCheck = true;

			// �ڑ����������� //
			if (Data[0] == 2 && Data[1] == 2)
			{
				Get_Network(true);

				Connect_GetSide(1);	// �v���C���[�T�C�h
				Connect_GetPort(Port);	// �|�[�g�ԍ�
				Connect_GetNetHandle(UDPNetHandle);	// �n���h��
				Connect_GetIp(Ip);	// Ip
				Network_Setting(1, Ip, Port, UDPNetHandle);

				Select_ReturnCursor();
				TraningSwitch(false);
				AISwitch(false);
				BattleMode(0);
				ModeChange(SceneNum(SelectScene));
			}
		}
		else {
			//DrawFormatString(20, SCREEN_H - 60, Cr, "false");
		}

	}
	// �N���C�A���g
	else if (sPoint == CLIENT){
		// 1���ڑ��҂��̃T�C��
		Data[1] = 1;
		// �����������Ă�����
		if (Data[0] > 0)Data[1] = 2;
		// ������̑��M
		NetWorkSendUDP(UDPNetHandle, Ip, Port, Data, sizeof(Data));
		// �ڑ�
		int cnwr_ret = CheckNetWorkRecvUDP(UDPNetHandle);
		if (cnwr_ret && cnwr_ret != -1){

			// �f�[�^����M
			NetWorkRecvUDP(UDPNetHandle, NULL, NULL, Data, sizeof(Data), FALSE);
			nCheck = true;

			if (Data[0] == 2 && Data[1] == 2)
			{
				Get_Network(true);

				Connect_GetSide(2);
				Connect_GetPort(Port);	// �|�[�g�ԍ�
				Connect_GetNetHandle(UDPNetHandle);	// �n���h��
				Connect_GetIp(Ip);	// Ip
				Network_Setting(2, Ip, Port, UDPNetHandle);

				Select_ReturnCursor();
				TraningSwitch(false);
				AISwitch(false);
				BattleMode(0);
				ModeChange(SceneNum(SelectScene));
			}
		}
		else {
			//DrawFormatString(20, SCREEN_H - 60, Cr, "false");
		}

	}
	//////////////////
	Draw();	// �`��

	// �I��
	if ((sPoint == 0) && (yPoint == 4) && (P1_BInput(1) == 1 || P1_BInput(2) == 1 || P1_BInput(3) == 1)){
		SEStart(37);
		ModeChange(SceneNum(MenuScene));
	}

	return 0;
}

//  �l�b�g���[�N�ǂݍ���
int Load_Network()
{
	if (!load_f){
		Ip.d1 = 0;
		Ip.d2 = 0;
		Ip.d3 = 0;
		Ip.d4 = 0;

		for (int i = 0; i < 5; i++){
			port[i] = 0;
		}
		for (int i = 0; i < ip_x; i++){
			for (int j = 0; j < ip_y; j++){
				ip[i][j] = 0;
			}
		}
		port[1] = 7;
		port[2] = 5;

		load_f = true;
	}

	Port = 0;
	// UDP
	UDPNetHandle = 0;
	// TCP
	DataLength = 0;

	sPoint = 0;
	yPoint = 0;
	xPoint = 0;
	nCheck = false;


	for (int i = 0; i < sizeof(Data); i++){
		Data[i] = 0;
	}

	//Main_GetNetwork(false);
	Connect_GetCn(false);
	NB_Reset();

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

		if (yPoint < 0)yPoint = 4;
		else if (yPoint > 4)yPoint = 0;

		// �z�X�g��
		if (yPoint == 0 && (P1_BInput(1) == 2 || P1_BInput(3) == 2)){
			SEStart(35);
			// �|�[�g�쐬
			Ip.d1 = 0, Ip.d2 = 0, Ip.d3 = 0, Ip.d4 = 0;
			// UDP
			// Ip�쐬
			Ip.d1 = (ip[0][0] * 100) + (ip[0][1] * 10) + ip[0][2];
			Ip.d2 = (ip[1][0] * 100) + (ip[1][1] * 10) + ip[1][2];
			Ip.d3 = (ip[2][0] * 100) + (ip[2][1] * 10) + ip[2][2];
			Ip.d4 = (ip[3][0] * 100) + (ip[3][1] * 10) + ip[3][2];

			int p = (port[0] * 10000) + (port[1] * 1000) + (port[2] * 100) + (port[3] * 10) + port[4];
			Port = p;
			UDPNetHandle = MakeUDPSocket(Port);
			sPoint = HOST;
			Data[0] = 0;
			Data[1] = 0;
		}
		// �N���C�A���g��
		else if (yPoint == 1 && (P1_BInput(1) == 2 || P1_BInput(3) == 2)){
			SEStart(35);
			// �|�[�g�쐬
			Ip.d1 = 0, Ip.d2 = 0, Ip.d3 = 0, Ip.d4 = 0;
			// Ip�쐬
			Ip.d1 = (ip[0][0] * 100) + (ip[0][1] * 10) + ip[0][2];
			Ip.d2 = (ip[1][0] * 100) + (ip[1][1] * 10) + ip[1][2];
			Ip.d3 = (ip[2][0] * 100) + (ip[2][1] * 10) + ip[2][2];
			Ip.d4 = (ip[3][0] * 100) + (ip[3][1] * 10) + ip[3][2];

			// �|�[�g�쐬
			int p = (port[0] * 10000) + (port[1] * 1000) + (port[2] * 100) + (port[3] * 10) + port[4];
			Port = p;
			UDPNetHandle = MakeUDPSocket(Port);
			sPoint = CLIENT;
			yPoint = 0;
			Data[0] = 0;
			Data[1] = 0;
		}
		// IP��
		else if (yPoint == 2 && (P1_BInput(1) == 1 || P1_BInput(3) == 1)){
			sPoint = IP_SET;
			yPoint = 0;
			xPoint = 0;
			SEStart(35);
		}
		// �|�[�g��
		else if (yPoint == 3 && (P1_BInput(1) == 1 || P1_BInput(3) == 1)){
			sPoint = PORT_SET;
			yPoint = 0;
			xPoint = 0;
			SEStart(35);
		}

		// �I����
		if (P1_BInput(2) == 2){
			sPoint = 0;
			yPoint = 4;
			SEStart(37);
		}
	}
	else if (sPoint == HOST){
		// �I����
		if (P1_BInput(2) == 2){
			sPoint = 0;
			yPoint = 0;
			// �t�c�o�\�P�b�g�n���h���̍폜
			DeleteUDPSocket(UDPNetHandle);
			SEStart(37);
		}
	}
	else if (sPoint == CLIENT){
		// �I����
		if (P1_BInput(2) == 2){
			sPoint = 0;
			yPoint = 1;
			// �t�c�o�\�P�b�g�n���h���̍폜
			DeleteUDPSocket(UDPNetHandle);
			SEStart(37);
		}
	}
	// IP�Z�b�g��� //
	else if (sPoint == IP_SET){
		if (P1_BInput(104) == 1){
			xPoint--;
			SEStart(36);
		}
		else if (P1_BInput(106) == 1){
			xPoint++;
			SEStart(36);
		}

		// xPoint�̈ʒu
		int po = 0, po2 = xPoint;
		if (xPoint > 8)po = 3, po2 = xPoint - 9;
		else if (xPoint > 5)po = 2, po2 = xPoint - 6;
		else if (xPoint > 2)po = 1, po2 = xPoint - 3;

		if (xPoint < 0)xPoint = 11;
		else if (xPoint > 11)xPoint = 0;
		if (P1_BInput(102) == 1){
			ip[po][po2]--;
			SEStart(36);
		}
		else if (P1_BInput(108) == 1){
			ip[po][po2]++;
			SEStart(36);
		}
		// ���l��0�`9�ɂ���
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 3; j++){
				if (j == 0){
					if (ip[i][j] < 0)ip[i][j] = 2;
					else if (ip[i][j] > 2)ip[i][j] = 0;
				}
				else {
					if (ip[i][j] < 0)ip[i][j] = 9;
					else if (ip[i][j] > 9)ip[i][j] = 0;
				}
			}
		}

		// �I����
		if (P1_BInput(2) == 2){
			sPoint = 0;
			yPoint = 2;
			int kekka = 0;
			for (int i = 0; i < ip_x; i++){
				kekka = (ip[i][0] * 100) + (ip[i][1] * 10) + ip[i][2];
				if (kekka > 255)ip[i][0] = 2, ip[i][1] = 5, ip[i][2] = 5;
			}
			SEStart(37);
		}
	}
	else if (sPoint == PORT_SET){
		if (P1_BInput(104) == 1){
			xPoint--;
			SEStart(36);
		}
		else if (P1_BInput(106) == 1){
			xPoint++;
			SEStart(36);
		}
		if (xPoint < 0)xPoint = 4;
		else if (xPoint > 4)xPoint = 0;
		if (P1_BInput(102) == 1){
			port[xPoint]--;
			SEStart(36);
		}
		else if (P1_BInput(108) == 1){
			port[xPoint]++;
			SEStart(36);
		}
		// ���l��0�`9�ɂ���
		for (int i = 0; i < 5; i++){
			if (port[i] < 0)port[i] = 9;
			else if (port[i] > 9)port[i] = 0;
		}

		// �I����
		if (P1_BInput(2) == 2){
			sPoint = 0;
			yPoint = 3;
			SEStart(37);
		}
	}
}

void Draw(){
	// �|�C���^�[
	DrawBox(0, yPoint * 20, 20, (yPoint + 1) * 20, Cr, true);
	// �e�X�g�p�ʒu�\��
	//DrawFormatString(420, 240, Cr, "S..%d", sPoint);
	//DrawFormatString(420, 260, Cr, "X..%d", xPoint);
	//DrawFormatString(420, 280, Cr, "Y..%d", yPoint);
	DrawFormatString(0, SCREEN_H - 40, Cr, "1P..%d, 2P..%d", Data[0], Data[1]);	// �ڑ��m�F
	//DrawFormatString(0, SCREEN_H - 20, Cr, "1P..%d, 2P..%d", Data[0], Data[1]);	// �ڑ��m�F

	if (sPoint == 0){
		DrawString(20, 20 * 0, "�z�X�g", Cr);
		DrawString(20, 20 * 1, "�N���C�A���g", Cr);
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
		DrawString(20, 20 * 4, "�I��", Cr);

		// ���ڂ̐���
		switch (yPoint){
		case 0:
			DrawString(80, 20 * 6, "�������ڑ���҂�", Cr);
			break;
		case 1:
			DrawString(80, 20 * 6, "�������ڑ�����", Cr);
			break;
		case 2:
			DrawString(80, 20 * 6, "����̃O���[�o��IP�A�h���X", Cr);
			DrawString(80, 20 * 7, "�N���C�A���g���̂ݓ��͂���", Cr);
			break;
		case 3:
			DrawString(80, 20 * 6, "�ڑ�����UDP�|�[�g�ԍ�", Cr);
			break;
		case 4:
			DrawString(80, 20 * 6, "�I������", Cr);
			break;
		}
	}
	else if (sPoint == HOST){
		DrawString(20, 20 * 0, "�z�X�g", Cr);
		if (!nCheck)DrawString(20, 20 * 1, "�ҋ@��", Cr);
		else{ DrawString(20, 20 * 1, "�ڑ����܂���", Cr); }
		DrawFormatString(20, 20 * 2, Cr, "%d", Port);
		DrawFormatString(20, 20 * 3, Cr, "�N���C�A���g %d.%d.%d.%d", Ip.d1, Ip.d2, Ip.d3, Ip.d4);
		//DrawFormatString(20, SCREEN_H - 40, Cr, "%d.%d", Data[0], Data[1]);
	}
	else if (sPoint == CLIENT){
		DrawString(20, 20 * 0, "�N���C�A���g", Cr);
		if (!nCheck)DrawString(20, 20 * 1, "�ҋ@��", Cr);
		else{ DrawString(20, 20 * 1, "�ڑ����܂���", Cr); }
		// �|�[�g���ꏏ�ɕ\��
		DrawFormatString(20, 20 * 3, Cr, "%d.%d.%d.%d:%d", Ip.d1, Ip.d2, Ip.d3, Ip.d4, Port);
		//DrawFormatString(20, SCREEN_H - 40, Cr, "%d.%d", Data[0], Data[1]);
	}
	else if (sPoint == IP_SET){
		// IP���l
		for (int n = 0; n < ip_x; n++){
			for (int i = 0; i < ip_y; i++){
				if ((n * 3) + i == xPoint)DrawFormatString(20 + (i * 10) + (n * 40), 20 * 0, Oran, "%d", ip[n][i]);
				else{ DrawFormatString(20 + (i * 10) + (n * 40), 20 * 0, Cr, "%d", ip[n][i]); }
			}
			if (n != 3)DrawString(20 + (n * 40) + 30, 20 * 0, ".", Cr);
		}
	}
	else if (sPoint == PORT_SET){
		// �|�[�g���l
		for (int i = 0; i < 5; i++){
			if (i == xPoint)DrawFormatString(20 + (i * 10), 20 * 0, Oran, "%d", port[i]);
			else{ DrawFormatString(20 + (i * 10), 20 * 0, Cr, "%d", port[i]); }
		}
	}
}




/*

#include "pch.h"

static IPDATA Ip;	// �O���[�o��?
static BYTE Data[2];	// *2�Ȃ̂�1P2P��
static int UDPNetHandle;        // �l�b�g���[�N�n���h��
static int port[5];
static int Port;
static boolean nCheck;

static const int ip_x = 4;
static const int ip_y = 3;
static int ip[ip_x][ip_y];	// IP([4]�ɐ���,[3]���s)
//============
// TCP
//static int NetHandle, LostHandle;    // �l�b�g���[�N�n���h��
static int DataLength;            // ��M�f�[�^�ʕۑ��p�ϐ�
//============

static const int HOST = 1;		// �z�X�g�ڑ��O
static const int CLIENT = 2;	// �N���C�A���g�ڑ��O
static const int IP_SET = 3;
static const int PORT_SET = 4;

static int sPoint;	// ��ʂ̐i�ݓx����
// 0.���ڑI��
static int yPoint;	// ���ڂ̈ʒu
static int xPoint;	//
static boolean load_f = false;


static void Input();
static void Draw();

// ���C���֐�
int Network()
{
	Input();

	//////////////////
	// �f�B���C�`�F�b�N
	//if (Connect_CheckCn()){

	//}
	// �z�X�g
	if (sPoint == HOST) {

		int cnwr_ret = CheckNetWorkRecvUDP(UDPNetHandle);
		// �f�[�^����M
		if (cnwr_ret && cnwr_ret != -1) {

			// ��Ƀf�[�^�󂯎��
			NetWorkRecvUDP(UDPNetHandle, &Ip, NULL, Data, sizeof(Data), FALSE);	// IP�擾����

			// ����Ԃ�
			Data[0] = 2;
			NetWorkSendUDP(UDPNetHandle, Ip, Port, Data, sizeof(Data));

			nCheck = true;

			// �ڑ����������� //
			if (Data[0] == 2 && Data[1] == 2)
			{
				Get_Network(true);

				Connect_GetSide(1);	// �v���C���[�T�C�h
				Connect_GetPort(Port);	// �|�[�g�ԍ�
				Connect_GetNetHandle(UDPNetHandle);	// �n���h��
				Connect_GetIp(Ip);	// Ip
				Network_Setting(1, Ip, Port, UDPNetHandle);

				Select_ReturnCursor();
				TraningSwitch(false);
				AISwitch(false);
				BattleMode(0);
				ModeChange(SceneNum(SelectScene));
			}
		}
		else {
			//DrawFormatString(20, SCREEN_H - 60, Cr, "false");
		}

	}
	// �N���C�A���g
	else if (sPoint == CLIENT) {

		Data[1] = 1;
		if (Data[0] > 0)Data[1] = 2;
		// ������̑��M
		NetWorkSendUDP(UDPNetHandle, Ip, Port, Data, sizeof(Data));
		// �ڑ�
		int cnwr_ret = CheckNetWorkRecvUDP(UDPNetHandle);
		if (cnwr_ret && cnwr_ret != -1) {

			// �f�[�^����M
			NetWorkRecvUDP(UDPNetHandle, NULL, NULL, Data, sizeof(Data), FALSE);
			nCheck = true;

			if (Data[0] == 2 && Data[1] == 2)
			{
				Get_Network(true);

				Connect_GetSide(2);
				Connect_GetPort(Port);	// �|�[�g�ԍ�
				Connect_GetNetHandle(UDPNetHandle);	// �n���h��
				Connect_GetIp(Ip);	// Ip
				Network_Setting(2, Ip, Port, UDPNetHandle);

				Select_ReturnCursor();
				TraningSwitch(false);
				AISwitch(false);
				BattleMode(0);
				ModeChange(SceneNum(SelectScene));
			}
		}
		else {
			//DrawFormatString(20, SCREEN_H - 60, Cr, "false");
		}

	}
	//////////////////
	Draw();	// �`��

	// �I��
	if ((sPoint == 0) && (yPoint == 4) && (P1_BInput(1) == 1 || P1_BInput(2) == 1 || P1_BInput(3) == 1)) {
		SEStart(37);
		ModeChange(SceneNum(MenuScene));
	}

	return 0;
}

//  �l�b�g���[�N�ǂݍ���
int Load_Network()
{
	if (!load_f) {
		Ip.d1 = 0;
		Ip.d2 = 0;
		Ip.d3 = 0;
		Ip.d4 = 0;

		for (int i = 0; i < 5; i++) {
			port[i] = 0;
		}
		for (int i = 0; i < ip_x; i++) {
			for (int j = 0; j < ip_y; j++) {
				ip[i][j] = 0;
			}
		}
		port[1] = 7;
		port[2] = 5;

		load_f = true;
	}

	Port = 0;
	// UDP
	UDPNetHandle = 0;
	// TCP
	DataLength = 0;

	sPoint = 0;
	yPoint = 0;
	xPoint = 0;
	nCheck = false;


	for (int i = 0; i < sizeof(Data); i++) {
		Data[i] = 0;
	}

	//Main_GetNetwork(false);
	Connect_GetCn(false);
	NB_Reset();

	return 0;
}


void Input() {
	// �I����ʁH
	if (sPoint == 0) {
		if (P1_BInput(102) == 1) {
			yPoint++;
			SEStart(36);
		}
		else if (P1_BInput(108) == 1) {
			yPoint--;
			SEStart(36);
		}

		if (yPoint < 0)yPoint = 4;
		else if (yPoint > 4)yPoint = 0;

		// �z�X�g��
		if (yPoint == 0 && (P1_BInput(1) == 2 || P1_BInput(3) == 2)) {
			SEStart(35);
			// �|�[�g�쐬
			Ip.d1 = 0, Ip.d2 = 0, Ip.d3 = 0, Ip.d4 = 0;
			// UDP
			// Ip�쐬
			Ip.d1 = (ip[0][0] * 100) + (ip[0][1] * 10) + ip[0][2];
			Ip.d2 = (ip[1][0] * 100) + (ip[1][1] * 10) + ip[1][2];
			Ip.d3 = (ip[2][0] * 100) + (ip[2][1] * 10) + ip[2][2];
			Ip.d4 = (ip[3][0] * 100) + (ip[3][1] * 10) + ip[3][2];

			int p = (port[0] * 10000) + (port[1] * 1000) + (port[2] * 100) + (port[3] * 10) + port[4];
			Port = p;
			UDPNetHandle = MakeUDPSocket(Port);
			sPoint = HOST;
			Data[0] = 0;
			Data[1] = 0;
		}
		// �N���C�A���g��
		else if (yPoint == 1 && (P1_BInput(1) == 2 || P1_BInput(3) == 2)) {
			SEStart(35);
			// �|�[�g�쐬
			Ip.d1 = 0, Ip.d2 = 0, Ip.d3 = 0, Ip.d4 = 0;
			// Ip�쐬
			Ip.d1 = (ip[0][0] * 100) + (ip[0][1] * 10) + ip[0][2];
			Ip.d2 = (ip[1][0] * 100) + (ip[1][1] * 10) + ip[1][2];
			Ip.d3 = (ip[2][0] * 100) + (ip[2][1] * 10) + ip[2][2];
			Ip.d4 = (ip[3][0] * 100) + (ip[3][1] * 10) + ip[3][2];

			// �|�[�g�쐬
			int p = (port[0] * 10000) + (port[1] * 1000) + (port[2] * 100) + (port[3] * 10) + port[4];
			Port = p;
			UDPNetHandle = MakeUDPSocket(Port);
			sPoint = CLIENT;
			yPoint = 0;
			Data[0] = 0;
			Data[1] = 0;
		}
		// IP��
		else if (yPoint == 2 && (P1_BInput(1) == 1 || P1_BInput(3) == 1)) {
			sPoint = IP_SET;
			yPoint = 0;
			xPoint = 0;
			SEStart(35);
		}
		// �|�[�g��
		else if (yPoint == 3 && (P1_BInput(1) == 1 || P1_BInput(3) == 1)) {
			sPoint = PORT_SET;
			yPoint = 0;
			xPoint = 0;
			SEStart(35);
		}

		// �I����
		if (P1_BInput(2) == 2) {
			sPoint = 0;
			yPoint = 4;
			SEStart(37);
		}
	}
	else if (sPoint == HOST) {
		// �I����
		if (P1_BInput(2) == 2) {
			sPoint = 0;
			yPoint = 0;
			// �t�c�o�\�P�b�g�n���h���̍폜
			DeleteUDPSocket(UDPNetHandle);
			SEStart(37);
		}
	}
	else if (sPoint == CLIENT) {
		// �I����
		if (P1_BInput(2) == 2) {
			sPoint = 0;
			yPoint = 1;
			// �t�c�o�\�P�b�g�n���h���̍폜
			DeleteUDPSocket(UDPNetHandle);
			SEStart(37);
		}
	}
	// IP�Z�b�g��� //
	else if (sPoint == IP_SET) {
		if (P1_BInput(104) == 1) {
			xPoint--;
			SEStart(36);
		}
		else if (P1_BInput(106) == 1) {
			xPoint++;
			SEStart(36);
		}

		// xPoint�̈ʒu
		int po = 0, po2 = xPoint;
		if (xPoint > 8)po = 3, po2 = xPoint - 9;
		else if (xPoint > 5)po = 2, po2 = xPoint - 6;
		else if (xPoint > 2)po = 1, po2 = xPoint - 3;

		if (xPoint < 0)xPoint = 11;
		else if (xPoint > 11)xPoint = 0;
		if (P1_BInput(102) == 1) {
			ip[po][po2]--;
			SEStart(36);
		}
		else if (P1_BInput(108) == 1) {
			ip[po][po2]++;
			SEStart(36);
		}
		// ���l��0�`9�ɂ���
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				if (j == 0) {
					if (ip[i][j] < 0)ip[i][j] = 2;
					else if (ip[i][j] > 2)ip[i][j] = 0;
				}
				else {
					if (ip[i][j] < 0)ip[i][j] = 9;
					else if (ip[i][j] > 9)ip[i][j] = 0;
				}
			}
		}

		// �I����
		if (P1_BInput(2) == 2) {
			sPoint = 0;
			yPoint = 2;
			int kekka = 0;
			for (int i = 0; i < ip_x; i++) {
				kekka = (ip[i][0] * 100) + (ip[i][1] * 10) + ip[i][2];
				if (kekka > 255)ip[i][0] = 2, ip[i][1] = 5, ip[i][2] = 5;
			}
			SEStart(37);
		}
	}
	else if (sPoint == PORT_SET) {
		if (P1_BInput(104) == 1) {
			xPoint--;
			SEStart(36);
		}
		else if (P1_BInput(106) == 1) {
			xPoint++;
			SEStart(36);
		}
		if (xPoint < 0)xPoint = 4;
		else if (xPoint > 4)xPoint = 0;
		if (P1_BInput(102) == 1) {
			port[xPoint]--;
			SEStart(36);
		}
		else if (P1_BInput(108) == 1) {
			port[xPoint]++;
			SEStart(36);
		}
		// ���l��0�`9�ɂ���
		for (int i = 0; i < 5; i++) {
			if (port[i] < 0)port[i] = 9;
			else if (port[i] > 9)port[i] = 0;
		}

		// �I����
		if (P1_BInput(2) == 2) {
			sPoint = 0;
			yPoint = 3;
			SEStart(37);
		}
	}
}

void Draw() {
	// �|�C���^�[
	DrawBox(0, yPoint * 20, 20, (yPoint + 1) * 20, Cr, true);
	// �e�X�g�p�ʒu�\��
	//DrawFormatString(420, 240, Cr, "S..%d", sPoint);
	//DrawFormatString(420, 260, Cr, "X..%d", xPoint);
	//DrawFormatString(420, 280, Cr, "Y..%d", yPoint);
	DrawFormatString(0, SCREEN_H - 20, Cr, "1P..%d, 2P..%d", Data[0], Data[1]);	// �ڑ��m�F

	if (sPoint == 0) {
		DrawString(20, 20 * 0, "�z�X�g", Cr);
		DrawString(20, 20 * 1, "�N���C�A���g", Cr);
		// IP���l
		for (int i = 0; i < ip_x; i++) {
			for (int j = 0; j < ip_y; j++) {
				DrawFormatString(20 + (j * 10) + (i * 40), 20 * 2, Cr, "%d", ip[i][j]);
			}
			if (i != 3)DrawString(20 + (i * 40) + 30, 20 * 2, ".", Cr);
		}
		// �|�[�g���l
		for (int i = 0; i < 5; i++) {
			DrawFormatString(20 + (i * 10), 20 * 3, Cr, "%d", port[i]);
		}
		DrawString(20, 20 * 4, "�I��", Cr);

		// ���ڂ̐���
		switch (yPoint) {
		case 0:
			DrawString(80, 20 * 6, "�������ڑ���҂�", Cr);
			break;
		case 1:
			DrawString(80, 20 * 6, "�������ڑ�����", Cr);
			break;
		case 2:
			DrawString(80, 20 * 6, "����̃O���[�o��IP�A�h���X", Cr);
			DrawString(80, 20 * 7, "�N���C�A���g���̂ݓ��͂���", Cr);
			break;
		case 3:
			DrawString(80, 20 * 6, "�ڑ�����UDP�|�[�g�ԍ�", Cr);
			break;
		case 4:
			DrawString(80, 20 * 6, "�I������", Cr);
			break;
		}
	}
	else if (sPoint == HOST) {
		DrawString(20, 20 * 0, "�z�X�g", Cr);
		if (!nCheck)DrawString(20, 20 * 1, "�ҋ@��", Cr);
		else { DrawString(20, 20 * 1, "�ڑ����܂���", Cr); }
		DrawFormatString(20, 20 * 2, Cr, "%d", Port);
		DrawFormatString(20, 20 * 3, Cr, "�N���C�A���g %d.%d.%d.%d", Ip.d1, Ip.d2, Ip.d3, Ip.d4);
		//DrawFormatString(20, SCREEN_H - 40, Cr, "%d.%d", Data[0], Data[1]);
	}
	else if (sPoint == CLIENT) {
		DrawString(20, 20 * 0, "�N���C�A���g", Cr);
		if (!nCheck)DrawString(20, 20 * 1, "�ҋ@��", Cr);
		else { DrawString(20, 20 * 1, "�ڑ����܂���", Cr); }
		// �|�[�g���ꏏ�ɕ\��
		DrawFormatString(20, 20 * 3, Cr, "%d.%d.%d.%d:%d", Ip.d1, Ip.d2, Ip.d3, Ip.d4, Port);
		//DrawFormatString(20, SCREEN_H - 40, Cr, "%d.%d", Data[0], Data[1]);
	}
	else if (sPoint == IP_SET) {
		// IP���l
		for (int n = 0; n < ip_x; n++) {
			for (int i = 0; i < ip_y; i++) {
				if ((n * 3) + i == xPoint)DrawFormatString(20 + (i * 10) + (n * 40), 20 * 0, Oran, "%d", ip[n][i]);
				else { DrawFormatString(20 + (i * 10) + (n * 40), 20 * 0, Cr, "%d", ip[n][i]); }
			}
			if (n != 3)DrawString(20 + (n * 40) + 30, 20 * 0, ".", Cr);
		}
	}
	else if (sPoint == PORT_SET) {
		// �|�[�g���l
		for (int i = 0; i < 5; i++) {
			if (i == xPoint)DrawFormatString(20 + (i * 10), 20 * 0, Oran, "%d", port[i]);
			else { DrawFormatString(20 + (i * 10), 20 * 0, Cr, "%d", port[i]); }
		}
	}
}
*/
