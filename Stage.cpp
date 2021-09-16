#include "pch.h"

using namespace std;

#define	BGM_MAX		(20)

/*
class Stage {
public:
	int front;
	int center;
	int back;

	Stage() {
		front = 0;
		center = 0;
		back = 0;
	}

	~Stage() {

	}

	void getFront(string str) {
		front = LoadGraph(str.c_str());
	}
	void getCenter(string str) {
		center = LoadGraph(str.c_str());
	}
	void getBack(string str) {
		back = LoadGraph(str.c_str());
	}
};
*/

static int maxStage;	// �X�e�[�W�ő吔

// �X�e�[�W
//static vector<Stage> stages;
static vector<vector<int>> stages;

int getMaxStage() {
	return maxStage;
}

int getStageGraph(int stageNum, int pictNum) {
	/*
	// �X�e�[�W�ő�l�ȓ��Ȃ�
	if (stageNum  <= maxStage) {
		if (pictNum == 0)return stages[stageNum].front;
		if (pictNum == 1)return stages[stageNum].center;
		if (pictNum == 2)return stages[stageNum].back;
	}
	*/
	return 0;
}

int drawStageGraph(int stageNum, int pictNum,
	float x, float y, float size) {
		// �X�e�w�i
	DrawRotaGraph(x, y,
		size, 0, stages[stageNum][pictNum], true);

	return 0;
}

// �X�e�[�W�S�̂�\��
int drawStageGraphAll(int stageNum, float x, float y, float size) {
	// �X�e�w�i
	DrawRotaGraph(x, y,
		size, 0, stages[stageNum][2], false);
	DrawRotaGraph(x, y + (82 * size),
		size * 0.80, 0, stages[stageNum][1], true);
	DrawRotaGraph(x, y + (76 * size),
		size * 0.625, 0, stages[stageNum][0], true);
	return 0;
}

void StageLoad()
{
	// ����Z�b�e�B���O //

	int i, fp;
	int anum;	// Name�̏ꏊ
	int num;	// �Z�b�g�ԍ�
	anum = -1;

	char fname[14] = { "back/list.txt" };
	
	// ���O�ǂݍ���
	string setName;
	string fn1 = "back/";

	// �X�e�[�W��0�ڂ�ǉ�
	//stages.push_back(Stage());
	/*
	for (int max = 0; max < 10; max++) {
		stages.push_back(vector<int>());
		// 3�p�ӂ���
		for (int l = 0; l < 3; l++) {
			stages[max].push_back(0);
		}
	}
	*/
	
	//==================
	// �����_���X�e�[�W
	//==================
	stages.push_back(vector<int>());
	// 3�p�ӂ���
	for (int l = 0; l < 3; l++) {
		stages[0].push_back(0);
	}

	// �����_���X�e�[�W�摜
	stages[0][0] = LoadGraph("back/random.png");
	stages[0][1] = LoadGraph("back/random2.png");
	stages[0][2] = LoadGraph("back/random3.png");
	maxStage = 0;	// �X�e�[�W�̐�
	//
	//==================

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// input��inputc�ɕ������͂���
	boolean iflg = false;	// ���O���̓t���O
	boolean next = false;
	boolean checkText = false;	// ��萔�̃e�L�X�g�ǂݍ���

	// �t�@�C�����J�� //
	fp = FileRead_open(fname);//�t�@�C���ǂݍ���
	if (fp == NULL) {			// �G���[���N��������
		printfDx("error name\n");// ������\��
		return;
	}

	//�ŏ���1�s�ǂݔ�΂�
	while (FileRead_getc(fp) != '\n'); // FileRead_getc:�ꕶ���ǂݏo��

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
					inputc[j] = input[j] = NULL;
					//inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				}
				i = -1;
				break;
			}

			// ������ɂ���
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
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
		/*
		iflg = false;
		{
			if ((inputc[0] != '\0') && (inputc[0] != '\n') && (inputc[0] != NULL)
				&& (anum < Character_Max()))iflg = true;
		}
		*/

		// �ŏ��ɐ��������ɂ��āA�����������Ă���ύX����
		iflg = false;
		checkText = false;

		// ������Z�b�g�Ɉڍs���Ă��Ȃ�
		if (!next)
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

		// ������Z�b�g
		if (next && (inputc[0] != '\0' && inputc[0] != NULL && inputc[0] != '\n')) {
			// �X�e�[�W�ǉ�
			stages.push_back(vector<int>());
			// 3�p�ӂ���
			for (int l = 0; l < 3; l++) {
				stages[num].push_back(0);
			}
			maxStage++;	// �X�e�[�W�̐�

			string fn2;
			fn2 = ".png";
			setName = fn1 + inputc + fn2;
			stages[num][0] = LoadGraph(setName.c_str());
			//stages[num].getFront(setName);

			fn2 = "_2.png";
			setName = fn1 + inputc + fn2;
			stages[num][1] = LoadGraph(setName.c_str());
			//stages[num].getCenter(setName);
			
			fn2 = "_3.png";
			setName = fn1 + inputc + fn2;
			stages[num][2] = LoadGraph(setName.c_str());
			//stages[num].getBack(setName);
			next = false;

			checkText = true;
			
		}

		// ���O��������
		if (iflg && !checkText) {
			num = atoi(inputc);	// �ԍ����Z�b�g
			next = true;	// ���̏���
		}

		if (input[i] == EOF) {//�t�@�C���̏I���Ȃ�
			goto EXFILE;//�I��
		}
	}

	// �t�@�C�������
EXFILE:
	FileRead_close(fp);
}