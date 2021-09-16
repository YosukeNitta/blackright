#pragma once
#include "DxLib.h"



class NetData {
	
private:
	static const int CDATA_SIZE = 2;
	BYTE cData[CDATA_SIZE];		// �ʐM�e�X�g�����p
	BYTE cGetData[CDATA_SIZE];	// �ʐM�e�X�g�󂯎��p
	int port;
	int delayMax;	// �ʐM�f�B���C�̍ő�l
public:
	int playerSide;	// 1P��2P��
	IPDATA ip;	// ������IP�A�擾����IP
	int UDPNetHandle;        // �l�b�g���[�N�n���h��
	int waitTime;	// ���ꂪ��莞�Ԃ������玟��
	boolean connectOn;	// �ڑ������̃T�C��

	// �R���X�g���N�^
	NetData::NetData() {
		for (int i = 0; i < CDATA_SIZE; i++) {
			cData[i] = 0;
			cGetData[i] = 0;
		}

		playerSide = 0;
		ip = { 0,0,0,0 };
		
		waitTime = 0;
		connectOn = false;
		
		port = 0;
		delayMax = 1;
		UDPNetHandle = 0;//MakeUDPSocket(-1);	// �n���h���쐬
	}

	// �f�X�g���N�^
	~NetData() {
		if (UDPNetHandle != 0) DeleteUDPNetHandle();
	}

	// 1P2P��ݒ�
	void setPlayerSide(int side) {
		playerSide = side;
	}

	// �|�[�g�ԍ��Ńn���h�����쐬
	void MakeUDPNetHandle() {
		UDPNetHandle = MakeUDPSocket(port);
	}

	// �t�c�o�\�P�b�g�n���h���̍폜
	void DeleteUDPNetHandle() {
		DeleteUDPSocket(UDPNetHandle);
		UDPNetHandle = 0;
	}

	void setIp(IPDATA getIp) {
		ip = getIp;
	}

	void setPort(int getPort) {
		port = getPort;
	}

	IPDATA getIp() {
		return ip;
	}

	int getPort() {
		return port;
	}

	int getDelayMax() {
		return delayMax;
	}

	void setDelayMax(int delay) {
		// �ő�E�ŏ��l
		if (delay > 20)delay = 20;
		else if (delay < 2)delay = 2;
		delayMax = delay;
	}

	// �ڑ��O�Ɏg�p����
	void connect_Reset() {
		for (int i = 0; i < CDATA_SIZE; i++) {
			cData[i] = 0;
			cGetData[i] = 0;
		}
		waitTime = 0;
		connectOn = false;
	}

	// �ڑ��e�X�g
	// ��������1���Ԃ��Ă���
	int connect_Test() {
		int rNum = 0;
		
		if (playerSide == 1)rNum = connect_Host();
		else if (playerSide == 2)rNum = connect_Client();

		return rNum;
	}

	int connect_Host() {
		// �z�X�g
		// 0���ڑ��҂�(�e�X�g�\���p)
		if(cData[0] < 2)cData[0] = 1;
		// UDP�f�[�^�󂯎��ҋ@
		int cnwr_ret = CheckNetWorkRecvUDP(UDPNetHandle);
		// �f�[�^����M����������
		if (cnwr_ret && cnwr_ret != -1) {

			// ��Ƀf�[�^�󂯎��
			NetWorkRecvUDP(UDPNetHandle, &ip, NULL, cGetData, sizeof(cGetData), FALSE);	// IP�擾����

			// ���肪�ҋ@���Ȃ瑗��Ԃ�
			if(cGetData[1] > 0)cData[0] = 2;
			// �l�b�g���[�N�e�X�g
			//NetWorkSendUDP(UDPNetHandle, ip, 7600, cData, sizeof(cData));	// �Ԃ�
			NetWorkSendUDP(UDPNetHandle, ip, port, cData, sizeof(cData));	// �Ԃ�
			
			
			
			// �ڑ����������� //
			if (cData[0] == 2 && cGetData[1] == 2)
			{
				//DrawFormatString(420, SCREEN_H - 240, Cr, "%d,%d %d,%d", cData[0], cData[1], cGetData[0], cGetData[1]);
				DrawFormatString(420, SCREEN_H - 260, Cr, "�I�ڑ��ɐ����I");
				connectOn = true;
			}
		}
		else {
			DrawFormatString(420, SCREEN_H - 260, Cr, "�ڑ��ҋ@��");
		}

		// �������̓J�E���g
		if (connectOn)waitTime++;
		// �o�߂����玟��
		if (waitTime > 60)return 1;

		return 0;
	}

	int connect_Client() {
		if (cData[1] < 2)cData[1] = 1;
		// �N���C�A���g
		// �����������Ă�����2P���i�߂�
		if (cGetData[0] > 0)cData[1] = 2;
		// ������̑��M
		// �l�b�g���[�N�e�X�g
		//NetWorkSendUDP(UDPNetHandle, ip, 7500, cData, sizeof(cData));
		NetWorkSendUDP(UDPNetHandle, ip, port, cData, sizeof(cData));

		// UDP��ڑ�
		int cnwr_ret = CheckNetWorkRecvUDP(UDPNetHandle);
		// ��M������ŃG���[����Ȃ�
		if (cnwr_ret && cnwr_ret != -1) {

			// �f�[�^����M IP�A�h���X�A�|�[�g�͒m���Ă�̂ł���Ȃ�
			NetWorkRecvUDP(UDPNetHandle, NULL, NULL, cGetData, sizeof(cGetData), FALSE);

			// �ڑ����������� //
			if (cGetData[0] == 2 && cData[1] == 2)
			{
				//DrawFormatString(420, SCREEN_H - 240, Cr, "%d,%d %d,%d", cData[0], cData[1], cGetData[0], cGetData[1]);
				DrawFormatString(420, SCREEN_H - 260, Cr, "�I�ڑ��ɐ����I");
				connectOn = true;
			}
		}
		else {
			DrawFormatString(420, SCREEN_H - 260, Cr, "�ڑ��ҋ@��");
		}

		// �������̓J�E���g
		if (connectOn)waitTime++;
		// �o�߂����玟��
		if (waitTime > 60)return 1;

		return 0;
	}

	// �l�b�g�f�[�^���̗v�f��\��
	void DataDraw(int x, int y) {
		DrawBox(x - 1, y - 1, x + 281, y + 101, Cr, false);
		DrawFormatString(x, y, GetColor(255, 255, 255), "player..%d", playerSide);
		DrawFormatString(x, y + 20, GetColor(255, 255, 255), "udpHandle..%d", UDPNetHandle);
		DrawFormatString(x, y + 40, GetColor(255, 255, 255), "port..%d", port);
		DrawFormatString(x, y + 60, GetColor(255, 255, 255), "ip..%d.%d.%d.%d", ip.d1, ip.d2, ip.d3, ip.d4);
		DrawFormatString(x, y + 80, GetColor(255, 255, 255), "delay..%d", delayMax);
		//DrawFormatString(x, y + 80, GetColor(255, 255, 255), "cGetData..%d, %d", cGetData[0], cGetData[1]);
	}
	
};