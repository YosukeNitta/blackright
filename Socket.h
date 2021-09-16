#pragma once
#include "DxLib.h"
#include "NetData.h"

// �ʐM�p�N���X
class Socket {
protected:
	// �|�[�g�ԍ����̒ʐM�ɕK�v�ȃf�[�^
	static const int DATALOG_MAX = KEYLOG_MAX + 5;	// �L�^����ő�f�[�^
													// +3�͎���2�ƃQ�[�����[�h
													// ����ping�v���p�̃����_���l
													// ������nowTime���n��
	// 
	// 500�o�C�g���x�ɗ}�����ق��������炵��
	// 12 * 10 = 120 �ŉ��K
	// 20 * 14 �� 280�o�C�g�����x��
	//

	static const int SEND_SIZE = 10;	// ����M����L�[���O�̃T�C�Y
										// 10�t���[�����擾����
	static const int ALLLOG_SIZE = 60;	// �S�̃��O�̃T�C�Y
										// ���M�f�[�^ �~ ����̃T�C�Y
	//static const int DELAY_MAX = 6;	// ����ȏ㍷������Ǝ~�܂�
										// ���肪�f�B���C / 2 �܂ŋ߂Â��ƍĊJ
	static const int DELAY_MIDDLETIME = 4;	// �f�B���C�������n�߂鎞��
	static const int DISCONNECT_TIME = 300;	// �ؒf���鎞��
											// ������߂���ƒʐM�I������
	static const int SOCKET_WAITTIME = 250;	// �\�P�b�g�̑ҋ@����

	boolean load_f;	// �ŏ��̂݃��[�h
	int disconnectTime;	// �ʐM��~���̎���

	BYTE myGameMode;	// �Q�[�����[�h
	BYTE p2GameMode;	// �擾�����Q�[�����[�h
	BYTE waitMode;	// ���[�h�ύX�̑ҋ@	0..�ҋ@���Ȃ� 1..�ҋ@ 2..���[�h���ҋ@
					// �L�[���͂�҂��Ă���A�Q�[���i�s��isKeyUpdate���s��
	BYTE waitTime[2];	// �\�P�b�g�̑ҋ@���� [1]��60�ɂȂ�x��[0]�ɃX�g�b�N�����
	BYTE p2WaitTime[2];	// �\�P�b�g�̔ԍ�1�`60;
	BYTE p2MaxWait[2];	// ��ԑ傫���ҋ@�l

	BYTE nowWaitTime[2];	// ���ۂ̑ҋ@���� 2P�̃L�[�擾�ɍ��킹�Đi��
							// ����Ɠ����ҋ@�ԍ��𔽉f������

	int getData;	// �擾�f�[�^�m�F�p(�e�X�g)
	int getRecvData;	// �擾��(�e�X�g)
	boolean isChangeP2GameMode;	// 2P�̃��[�h�ύX����������
	int gameTime;
	boolean isKeyUpdate;	// �Q�[�����̃L�[���i�s����
	
	BYTE randNum;	// 1�`100�A101�`200 �����v���邽�т�+10�����ă����_���ɐݒ�
	BYTE randBaseNum;	// 10�i�K�̕ϓ��l
	BYTE returnRandNum;	// �A���Ă��������_���l
	BYTE p2RandNum;	// 2P�������Ă��������_���l
	BYTE pingCount;		// �������Ă���܂ŉ��Z��������
	BYTE ping;		// ��v�����l���A���Ă������Ԃ�\��

	int moreKey;	// �����ƃQ�[����i�s�ł���

public:
	// ���o�[+4�{�^��+�X�^�[�g
	//  +�Q�[�����[�h 
	//  +�o�ߎ���(�p�P�b�g�ԍ�)+�����ԍ�(���������̂������Ă邩�ؖ�)
	//static const int KEYLOG_MAX = 9;
	NetData* netData;
	BYTE myKeyLog[KEYLOG_MAX];	// �����̃L�[���O�AKey�N���X�Ŏg����
	BYTE p2KeyLog[KEYLOG_MAX];	// �擾�����L�[���O�AKey�N���X�Ŏg����

	BYTE mySendData[SEND_SIZE][DATALOG_MAX];	// ���M����f�[�^
	BYTE p2RecvData[SEND_SIZE][DATALOG_MAX];	// ��M����f�[�^
	
	BYTE myAllLog[ALLLOG_SIZE][SEND_SIZE][DATALOG_MAX];	// �S�̂̃��O
	BYTE p2AllLog[ALLLOG_SIZE][SEND_SIZE][DATALOG_MAX];	// 2P�S�̃��O


	// �R���X�g���N�^
	Socket() {
		load_f = false;
		disconnectTime = 0;
		waitTime[0] = 0;
		waitTime[1] = 1;
		p2WaitTime[0] = 0;	// 2P�͎�M���Ă��甽�f
		p2WaitTime[1] = 0;	//
		p2MaxWait[0] = 0;	// 
		p2MaxWait[1] = 0;	//

		nowWaitTime[0] = 0;
		nowWaitTime[1] = 1;

		// ���ۂɎg����L�[
		for (int i = 0;i < KEYLOG_MAX; i++) {
			myKeyLog[i] = 0;
			p2KeyLog[i] = 0;
		}

		// �ʐM�p�f�[�^
		for (int i = 0; i < SEND_SIZE; i++) {
			for (int j = 0; j < DATALOG_MAX; j++) {
				mySendData[i][j] = 0;
				p2RecvData[i][j] = 0;
			}
		}

		// �S�̃��O
		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				for (int k = 0; k < DATALOG_MAX; k++) {
					myAllLog[i][j][k] = 0;
					p2AllLog[i][j][k] = 0;
				}
			}
		}

		netData = new NetData();

		myGameMode = p2GameMode = SceneNum(NoScene);
		waitMode = 0;

		getData = 0;
		getRecvData = 0;
		isChangeP2GameMode = false;
		gameTime = 0;
		isKeyUpdate = false;

		randNum = GetRand(9) + 1;
		randBaseNum = 0;
		returnRandNum = 0;
		p2RandNum = 255;
		pingCount = 0;	
		ping = 0;

		moreKey = 0;
	}

	// �f�X�g���N�^
	~Socket() {
		DeleteUDPSocket(netData->UDPNetHandle);	// �\�P�b�g�폜
		delete netData;	// �l�b�g�f�[�^�폜
	}

	//================
	// getter, setter
	//================

	// get
	boolean getLoad_f() {
		return load_f;
	}
	// set
	void setLoad_f(boolean b) {
		load_f = b;
	}

	// �l�b�g�f�[�^�擾
	// Network�N���X����擾����
	/*
	NetData getNetData() {
		return *netData;
	}
	*/
	void setNetData(NetData* n) {
		netData = n;
	}

	int getNoConnectTime() {
		return disconnectTime;
	}

	void setNoConnectTime(int time) {
		disconnectTime = time;
	}

	BYTE getMyGameMode() {
		return p2GameMode;
	}

	void setMyGameMode(int b) {
		// ���[�h�ԍ����Ⴄ�Ȃ�
		if (myGameMode != (BYTE)b) {
			myGameMode = (BYTE)b;
			// �ҋ@���Ԃ�������������
			nowWaitTime[0] = 0;
			nowWaitTime[1] = 1;
			waitTime[0] = 0;
			waitTime[1] = 1;
			// ���[�h�ύX�ő҂�
			waitMode = 2;
		}
	}

	BYTE getP2GameMode() {
		return p2GameMode;
	}

	void setP2GameMode(int b) {
		p2GameMode = (BYTE)b;
	}

	BYTE getWaitMode() {
		return waitMode;
	}

	void setWaitMode(int f) {
		waitMode = (BYTE)f;
	}

	boolean getIsKeyUpdate() {
		return isKeyUpdate;
	}

	void setIsKeyUpdate(boolean flg) {
		isKeyUpdate = flg;
	}

	int getMoreKey() {
		return moreKey;
	}

	/////////////////////
	// �X�V����
	// �󂯓n���͕ʂōs��(���key��mode���擾���Ă�������)
	int update() {

		// �傫�����ׂĎ������ł���������~�܂�
		int nowTime = nowWaitTime[0] * 60 + nowWaitTime[1];
		int myTime = waitTime[0] * 60 + waitTime[1];
		int p2Time = p2WaitTime[0] * 60 + p2WaitTime[1];
		int p2Max = p2MaxWait[0] * 60 + p2MaxWait[1];	// �ő�l�v��

		// ���[�h�������̑ҋ@
		//if (waitTime[0] == 0 && waitTime[1] == 1) {
		if(waitMode == 2){
			//waitMode = 2;
			// now��莩������萔�ł���������L�[���͂��~�߂�
			//if (myTime - nowTime > DELAY_MAX) {
			if (myTime - nowTime > netData->getDelayMax()) {
				// ������傫���Ȃ�
				//if (myTime > p2Time)
				// nowTime������ƍ��킹�Đi�ނ̂ŁAnowTime�Ɣ�r����
				waitMode = 1;
			}
			// ��v������E�F�C�g�I��
			else if (p2WaitTime[0] == 0 && p2WaitTime[1] == 1) {
				waitMode = 0;
			}
			
		}
		// �E�F�C�g2(�J��)�ȊO�̃��[�h�ݒ�
		else {
			// now��莩������萔�ł���������L�[���͂��~�߂�
			//if (myTime - nowTime > DELAY_MAX) {
			if (myTime - nowTime > netData->getDelayMax()) {
				waitMode = 1;
			}
			// �L�[���͎��Ԃ��߂Â�����ĊJ
			//else if (myTime - p2Time <= DELAY_MAX / 2) {
			//else if (myTime - nowTime <= DELAY_MAX / 2) {
			// �L�[���͂��ǂ�������ĊJ
			else if (myTime == nowTime) {
				waitMode = 0;
			}
			
			//2P�̍ő�l���������傫���Ȃ�i�߂�
			// *�e�X�g* 
			if (p2Max > myTime) {
				waitMode = 0;
			}
		}

		// �����̓��̓L�[��ʐM�p�f�[�^�Ɉړ�
		moveKeyToData();

		// �f�[�^���M
		sendData();
		// �f�[�^�擾
		recvData();

		// �ʐM�p�f�[�^�����O�Ɉړ�
		moveDataToLog();	// �f�[�^�����O�Ɉړ�
		//moveP2DataToLog();	// �V
		//setP2WaitTime();	// �E�F�C�g�^�C������
							// ��������
		setP2MaxWait();	// �E�F�C�g�^�C���ő�l��ݒ肷��

		// ���O�̃J�E���g�����ăL�[����
		// �ǂ�����ҋ@���Ԃ̓��͂�����Ύ��ԍX�V

		// �����_���l�������Ă���ping����
		enterPing();

		
		// �L�[���X�V����
		keyUpdate();

		// nowWait���f�B���C�l����◣�ꂽ��
		// �\�Ȃ�Q�[���𑁑��肷��
		//if(myTime - nowTime > DELAY_MIDDLETIME)searchKey();	

		// �\�P�b�g���
		// �ʏ�E�F�C�g�ȊO�͐i�߂�
		if (waitMode == 0 || waitMode == 2)
		{
			// �\�P�b�g�ԍ��X�V
			waitTime[1]++;
			if (waitTime[1] >= 60) {
				waitTime[1] = 0;
				waitTime[0]++;	// �傫����
				// ���Z�b�g
				if (waitTime[0] >= SOCKET_WAITTIME) {
					waitTime[0] = 1;
				}
			}
		}

		// ��~���Ȃ�
		// ���͂���������
		// keyUpdate�ŏ������Ă����̂łȂ�
		/*
		if (waitMode != 0) {
			// ���͂�S�ď���
			for (int i = 0; i < KEYLOG_MAX; i++) {
				myKeyLog[i] = 0;
				p2KeyLog[i] = 0;
			}
		}
		*/
		// ���v���C�i�s�l���擾
		getGameTime(0);

		// ��莞�Ԍo�߂Őؒf������H
		return 0;
	}

	// �`��
	// �ʐM���̃f�[�^��\��
	void draw();

	// �L�[���O����͂���擾
	void setKeyLog(BYTE key[KEYLOG_MAX]) {
		myKeyLog[0] = 0;
		for (int i = 0; i < KEYLOG_MAX; i++) {
			myKeyLog[i] = key[i];
		}
	}
	// getter
	//BYTE* getGKeyLog() {return p2KeyLog;}

	// ���葤�Ƀf�[�^���M
	int sendData() {
		// �e�X�g
		// �ʏ�E�F�C�g�̂ݎ~�߂�
		// ��s�E�F�C�g��2��Ɉ�񑗂�
		//if (waitMode == 0 || waitMode == 2 ||
		//	((waitMode == 1) && (disconnectTime % 2 == 0)))

		// ��ɑ��M�����ق����悳����������
		{
			// �l�b�g���[�N�e�X�g
			/*
			if (netData->playerSide == 1) {
				// 10�t���[����
				NetWorkSendUDP(netData->UDPNetHandle, netData->getIp(), 7600, mySendData, sizeof(mySendData));
			}
			else if (netData->playerSide == 2) {
				// 10�t���[����
				NetWorkSendUDP(netData->UDPNetHandle, netData->getIp(), 7500, mySendData, sizeof(mySendData));
			}
			*/
			// �L�[���O���M
			NetWorkSendUDP(netData->UDPNetHandle, netData->getIp(), netData->getPort(), mySendData, sizeof(mySendData));
		}

		return 0;
	}

	// ����̃f�[�^��M
	int recvData() {
		// ��M�O�ɏ���������
		// �ʐM�p�f�[�^
		for (int i = 0; i < SEND_SIZE; i++) {
			for (int j = 0; j < DATALOG_MAX; j++) {
				//p2RecvData[i][j] = 0;
			}
		}

		// UDP��ڑ�
		int cnwr_ret = CheckNetWorkRecvUDP(netData->UDPNetHandle);
		// �f�[�^�擾�m�F�̕\���p
		getData = cnwr_ret;
		getRecvData = 0;

		// �I�����͓r���Ń��[�v�ɓ���Ȃ��悤�ɂ���
		if (disconnectTime >= DISCONNECT_TIME) {
			return -1;
		}
		
		// �f�[�^������Ƃ������ǂݍ��ށH
		while (1) {
			cnwr_ret = CheckNetWorkRecvUDP(netData->UDPNetHandle);
			// ��M������ŃG���[����Ȃ�
			if (cnwr_ret && cnwr_ret != -1) {
				// 10�t���[����
				NetWorkRecvUDP(netData->UDPNetHandle, NULL, NULL, p2RecvData, sizeof(p2RecvData), FALSE);
				// �f�[�^�����O�Ɉړ�
				moveP2DataToLog();
				getRecvData++;
			}
			// �G���[
			else {
				break;
			}
		}

		return 0;
	}

	// �����̃L�[���� > �ʐM�p�f�[�^�Ɉړ�
	int moveKeyToData() {
		// ����̃����_���l�͏�ɔ��f����
		mySendData[0][KEYLOG_MAX + 4] = p2RandNum;	

		// �ʏ�E�F�C�g���[�h�̂ݕύX���Ȃ�
		// �����̑��M�L�[��ς��ĂȂ�����
		if (waitMode == 1)return -1;

		// ��
		// �ʐM�p�f�[�^��1��X�V����
		// SendData��1�󂯂�
		for (int i = SEND_SIZE - 1; i > 0; i--) {
			for (int j = 0; j < DATALOG_MAX; j++) {
				mySendData[i][j] = mySendData[i-1][j];
			}
		}

		//----------------------------
		// �{�^���̓��͂��f�[�^�Ɉړ�
		for (int i = 0; i < KEYLOG_MAX; i++) {
			mySendData[0][i] = myKeyLog[i];
		}
		
		// �ԍ�
		mySendData[0][KEYLOG_MAX] = waitTime[0];
		mySendData[0][KEYLOG_MAX + 1] = waitTime[1];
		// ���[�h
		mySendData[0][KEYLOG_MAX + 2] = myGameMode;
		// ping
		mySendData[0][KEYLOG_MAX + 3] = randNum;	// �����̃����_���l
		// p2RandNum�̓��\�b�h�̍ŏ��ŕς��Ă���
		//
		//----------------------------

		return 0;
	}

	// �ʐM�p�f�[�^�����O�Ɉڍs����
	int moveDataToLog() {

		// �ҋ@���[�h�Ȃ�ς��Ȃ�
		// �^�C��0,1�Ȃ畁�ʂɒʂ�
		if (waitMode == 1)return -1;
		
		// ���O��1��ړ�����
		// �ǉ����郍�O[0][][]���󂯂�
		for (int i = ALLLOG_SIZE - 1; i > 0; i--) {
			for (int j = 0; j < SEND_SIZE; j++) {
				for (int k = 0; k < DATALOG_MAX; k++) {
					myAllLog[i][j][k] = myAllLog[i - 1][j][k];
				}
			}
		}
		// �ʐM�p�f�[�^�����O�Ɉړ�
		// �ŐV���̈ʒu�ɋL�^
		for (int i = 0; i < SEND_SIZE; i++) {
			for (int j = 0; j < DATALOG_MAX; j++) {
				myAllLog[0][i][j] = mySendData[i][j];
			}
		}

		// ���[�h���ς������
		if (myAllLog[0][0][KEYLOG_MAX + 2] != myAllLog[1][0][KEYLOG_MAX + 2]) {
			// �Â��Ȃ����O���[�h�̃f�[�^���폜����
			for (int i = 10; i < ALLLOG_SIZE; i++) {
				for (int j = 0; j < SEND_SIZE; j++) {
					for (int k = 0; k < DATALOG_MAX; k++) {
						myAllLog[i][j][k] = 0;
					}
				}
			}
		}

		return 0;
	}

	// 2P
	// �擾�f�[�^ > ���O
	int moveP2DataToLog() {
		// ���[�h����������
		// ���Ԃ��ꏏor����ȉ��Ȃ甽�f�����Ȃ�
		// ���ʂȃ��O�����Ȃ����߂̏���
		if (p2AllLog[0][0][KEYLOG_MAX + 2] == p2RecvData[0][KEYLOG_MAX + 2]) {
			if (p2AllLog[0][0][KEYLOG_MAX] == p2RecvData[0][KEYLOG_MAX]) {
				if (p2AllLog[0][0][KEYLOG_MAX + 1] >= p2RecvData[0][KEYLOG_MAX + 1]) {
					return -1;
				}
			}
		}

		// ���O��1��ړ�����
		// �ǉ����郍�O[0][][]���󂯂�
		for (int all = ALLLOG_SIZE - 1; all > 0; all--) {
			for (int send = 0; send < SEND_SIZE; send++) {
				for (int data = 0; data < DATALOG_MAX; data++) {
					p2AllLog[all][send][data] = p2AllLog[all - 1][send][data];
				}
			}
		}
		// �ʐM�p�f�[�^�����O�Ɉړ�
		// �ŐV���̈ʒu�ɋL�^
		for (int send = 0; send < SEND_SIZE; send++) {
			for (int data = 0; data < DATALOG_MAX; data++) {
				p2AllLog[0][send][data] = p2RecvData[send][data];
			}
		}

		// ���O�폜(���݂͈ړ�)

		return 0;
	}

	// 2P�̌Â����O���폜����
	int deleteP2AllLog() {
		//setP2WaitTime() �̓��e��������
		// ���[�h���ς������
		if (isChangeP2GameMode) {
			// �Â��Ȃ����O���[�h�̃f�[�^���폜����
			for (int i = 10; i < ALLLOG_SIZE; i++) {
				for (int j = 0; j < SEND_SIZE; j++) {
					// �Q�[�����[�h�����ƈႤ�Ȃ�
					if (p2AllLog[i][j][KEYLOG_MAX + 2] != p2GameMode) {
						for (int k = 0; k < DATALOG_MAX; k++) {
							p2AllLog[i][j][k] = 0;	// �����폜����
						}
					}
				}
			}
			isChangeP2GameMode = false;
		}

		return 0;
	}

	// �ő�E�F�C�g�^�C����ݒ肷��
	// �E�F�C�g���L�[���莞�ɐݒ肵���̂�
	// �������͍ő�l����ɑ���
	int setP2MaxWait() {
		// �ő�l������
		p2MaxWait[0] = 0;
		p2MaxWait[1] = 0;
		// �ҋ@���Ԃ̍ő�l�擾
		for (int i = 1; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// ���ԓ��Ō�������A
				if ((p2WaitTime[0] <= p2AllLog[i][j][KEYLOG_MAX]) &&
					(p2AllLog[i][j][KEYLOG_MAX + 2] == myGameMode)) {	// �Q�[�����[�h�������Ȃ�
					//p2WaitTime[0] = p2AllLog[i][j][KEYLOG_MAX];
					p2MaxWait[0] = p2AllLog[i][j][KEYLOG_MAX];
					// ����ɑ傫�����̂�������
					if (p2WaitTime[1] < p2AllLog[i][j][KEYLOG_MAX + 1]) {
						//p2WaitTime[1] = p2AllLog[i][j][KEYLOG_MAX + 1];
						p2MaxWait[1] = p2AllLog[i][j][KEYLOG_MAX + 1];
					}
				}
			}
		}

		return 0;
	}

	// ���O > �L�[���f�@���ꂪ���ۂ̑���ɂȂ�
	boolean moveLogToKey() {
		boolean flg = false;

		// �ŐV���̈ʒu�ɋL�^
		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// �Q�[�����[�h����v������
				if (myAllLog[i][j][KEYLOG_MAX + 2] == myGameMode) {
					// 1�P�^�ڂ̃\�P�b�g
					if (myAllLog[i][j][KEYLOG_MAX] == nowWaitTime[0]) {
						// 2�P�^��
						if (myAllLog[i][j][KEYLOG_MAX + 1] == nowWaitTime[1]) {
							// �L�[�Ƀ��O�f�[�^���Z�b�g
							for (int num = 0; num < KEYLOG_MAX; num++) {
								myKeyLog[num] = myAllLog[i][j][num];
							}
							flg = true;
							break;
						}
					}
				}
			}
		}

		return flg;
	}

	// ���O > �L�[���f
	boolean moveP2LogToKey() {
		boolean flg = false;

		// �ŐV���̈ʒu�ɋL�^
		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// �Q�[�����[�h����v������
				if (p2AllLog[i][j][KEYLOG_MAX + 2] == myGameMode) {
					// 1�P�^�ڂ̃E�F�C�g
					if (p2AllLog[i][j][KEYLOG_MAX] == nowWaitTime[0]) {
						// 2�P�^�ڂ̃E�F�C�g
						if (p2AllLog[i][j][KEYLOG_MAX + 1] == nowWaitTime[1]) {
							// �L�[�Ƀ��O�f�[�^���Z�b�g
							for (int num = 0; num < KEYLOG_MAX; num++) {
								p2KeyLog[num] = p2AllLog[i][j][num];
							}
							// �e��ݒ�
							{
								// ���Ԃ�ݒ�
								//if (p2AllLog[i][j][0] > 0)
								p2WaitTime[0] = p2AllLog[i][j][KEYLOG_MAX];
								//if (p2AllLog[i][j][KEYLOG_MAX + 1] > 0)
								p2WaitTime[1] = p2AllLog[i][j][KEYLOG_MAX + 1];
								
								// ���[�h������������ς���
								if (p2GameMode != p2AllLog[i][j][KEYLOG_MAX + 2]) {
									p2GameMode = p2AllLog[i][j][KEYLOG_MAX + 2];
									isChangeP2GameMode = true;	// �ύX����
									// ���[�h�ύX���ꂽ�Ȃ�
									deleteP2AllLog();	// �Â����O���폜
								}
								//returnRandNum = p2AllLog[i][j][KEYLOG_MAX + 4];	// ���肪�����Ă��������̃����_���l
								p2RandNum = p2AllLog[i][j][KEYLOG_MAX + 3];	// ����̃����_���l
							}
							flg = true;
							break;
						}
					}
				}
			}
		}

		return flg;
	}

	void getGameTime(int num);

	// ping��ݒ肷��
	void enterPing() {
		boolean flg = false;
		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// �����_���l����v������
				if (p2AllLog[i][j][KEYLOG_MAX + 4] == randNum) {
					flg = true;
					returnRandNum = p2AllLog[i][j][KEYLOG_MAX + 4];
				}
			}
		}

		pingCount++;
		// ��v������ping����
		if (flg) {
			if (returnRandNum == randNum) {
				ping = pingCount;
				pingCount = 0;	// �l��������

				//====================
				// ���̃����_���l����
				//====================
				setRandNum();
			}
		}
	}

	// ���̃����_���l����
	void setRandNum() {
		// ��l����
		randBaseNum += 10;
		if (randBaseNum >= 100)
			randBaseNum = 0;
		// ����
		randNum = randBaseNum + GetRand(9) + 1;
	}

	// �L�[�����ۂɐi�s���違���ݎ��Ԃ�i�߂�
	// 
	int keyUpdate() {
		
		// �L�[�i�s�̋������肷��
		isKeyUpdate = false;
		boolean flgP1, flgP2;
		flgP2 = moveP2LogToKey();
		flgP1 = moveLogToKey();
		// �L�[���͐���������E�F�C�g�^�C����i�߂�
		// 1P,2P�̃��O�Ɏ��Ԃƍ����L�[������Ȃ�
		if (flgP2 && flgP1) {
			// �\�P�b�g�ԍ��X�V
			nowWaitTime[1]++;
			if (nowWaitTime[1] >= 60) {
				nowWaitTime[1] = 0;
				nowWaitTime[0]++;	// �傫����
				// ���Z�b�g
				if (nowWaitTime[0] >= SOCKET_WAITTIME) {
					nowWaitTime[0] = 1;
				}
			}
			isKeyUpdate = true;	// �L�[�i�s�̋���
			disconnectTime = 0;
			return 0;
		}
		else {
			// ������Ȃ�����
			// �����œ��͂𖳂���
			disconnectTime++;	// �ڑ����s���Ă���Ƒ����Ă���

			for (int j = 0; j < KEYLOG_MAX; j++) {
				myKeyLog[j] = 0;
				p2KeyLog[j] = 0;
			}
		}

		return -1;
	}

	int searchKey() {
		// 2��ڈȍ~�͂��Ȃ�
		if (moreKey == 1) {
			moreKey = 0;
			return moreKey;
		}

		// 1P2P�̐����t���O
		boolean p1Flg, p2Flg;
		p1Flg = p2Flg = false;

		// �ŐV���̈ʒu�ɋL�^
		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// �Q�[�����[�h����v������
				if (myAllLog[i][j][KEYLOG_MAX + 2] == myGameMode) {
					// 1�P�^�ڂ̃\�P�b�g
					if (myAllLog[i][j][KEYLOG_MAX] == nowWaitTime[0]) {
						// 2�P�^��
						if (myAllLog[i][j][KEYLOG_MAX + 1] == nowWaitTime[1]) {
							p1Flg = true;
							break;
						}
					}
				}
			}
		}

		for (int i = 0; i < ALLLOG_SIZE; i++) {
			for (int j = 0; j < SEND_SIZE; j++) {
				// �Q�[�����[�h����v������
				if (p2AllLog[i][j][KEYLOG_MAX + 2] == myGameMode) {
					// 1�P�^�ڂ̃\�P�b�g
					if (p2AllLog[i][j][KEYLOG_MAX] == nowWaitTime[0]) {
						// 2�P�^��
						if (p2AllLog[i][j][KEYLOG_MAX + 1] == nowWaitTime[1]) {
							p2Flg = true;
							break;
						}
					}
				}
			}
		}

		if (p1Flg && p2Flg) {
			moreKey = 1;
		}

		return moreKey;
	}

	// �������Őؒf
	boolean isEndConnect() {
		boolean isEnd = false;
		// �w�肵�����Ԍo�߂Őؒf
		if (disconnectTime > DISCONNECT_TIME)isEnd = true;
		return isEnd;
	}
};
