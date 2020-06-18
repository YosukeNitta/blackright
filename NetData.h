#pragma once
#include "DxLib.h"

class NetData {

public:
	IPDATA Ip, P2Ip;	// ������IP�A�擾����IP
	int Port;
	int UDPNetHandle;        // �l�b�g���[�N�n���h��
	
	// �R���X�g���N�^
	NetData() {
		NetData::Port = 0;
		NetData::UDPNetHandle = 0;
	}

	// �n���h�����쐬
	void GetUDPNetHandle() {
		UDPNetHandle = MakeUDPSocket(Port);
	}

	// �t�c�o�\�P�b�g�n���h���̍폜
	void DeleteUDPNetHandle() {
		DeleteUDPSocket(UDPNetHandle);
		UDPNetHandle = -1;
	}

	void GetIp(IPDATA ip) {
		Ip = ip;
	}

	void GetPort(int port) {
		Port = port;
	}

	// �l�b�g�f�[�^���̗v�f��\��
	void DataDraw(int x, int y) {
		DrawBox(x - 1, y - 1, x + 281, y + 61, Cr, false);
		DrawFormatString(x, y, GetColor(255, 255, 255), "udpHandle..%d", UDPNetHandle);
		DrawFormatString(x, y + 20, GetColor(255, 255, 255), "port..%d", Port);
		DrawFormatString(x, y + 40, GetColor(255, 255, 255), "ip..%d.%d.%d.%d", Ip.d1, Ip.d2, Ip.d3, Ip.d4);
	}
private:
	
};