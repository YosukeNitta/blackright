#include "pch.h"
#include <stdio.h>	// �t�@�C������ɕK�v�ȃw�b�_�t�@�C��
#include <string.h>	// �����񑀍�ɕK�v�ȃw�b�_�t�@�C��
#include <ctype.h>	// isdigit�ɕK�v
#include <vector>	// string�Ɏg��



// �z��̐� = character_Max
// ���g�̖��O = EIN = ����;
static std::vector<Name_t>names;
static int char_max;
static Def_c def;

void GetDef()
{
	// ����Z�b�e�B���O //

	int i, fp;
	int anum;	// Name�̏ꏊ
	anum = -1;

	char fname[14] = { "char/name.txt" };

	int input[NAME_MAX];
	char inputc[NAME_MAX];	// input��inputc�ɕ������͂���
	boolean iflg;	// ���O���̓t���O


	// �t�@�C�����J�� //
	fp = FileRead_open(fname);//�t�@�C���ǂݍ���
	if (fp == NULL){			// �G���[���N��������
		printfDx("error name\n");// ������\��
		return;
	}

	//�ŏ���1�s�ǂݔ�΂�
	while (FileRead_getc(fp) != '\n'); // FileRead_getc:�ꕶ���ǂݏo��


	while (1){
		//=========================
		// ��{���[�v
		// ���̃��[�v�I����ɉ���
		//=========================
		for (i = 0; i < NAME_MAX; i++)
		{
			inputc[i] = input[i] = FileRead_getc(fp);//1�����擾����

			// �X���b�V���ŉ��s
			if (inputc[i] == '/'){					//�X���b�V���������
				while (FileRead_getc(fp) != '\n');	//���s�܂Ń��[�v
				i = -1;//�J�E���^���ŏ��ɖ߂��� (�߂����Ƃ���0�ɂȂ�)
				continue;
			}

			// ���s�Ŏ��̍s�ɐi��
			if (input[i] == '\n'){
				for (int j = 0; j < NAME_MAX; j++){
					inputc[j] = input[j] = NULL;
				}
				i = -1;
				break;
			}

			// ������ɂ���
			if (input[i] == ',' || input[i] == ' ' ||
				input[i] == '[' || input[i] == ']' ||
				input[i] == ';'){ //�J���}�����s�Ȃ�
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ��i�����������ƕ�����Ƃ��Ĉ����j
				break;	// i��1���₵�ă��[�v�E�o�i�������j
			}

			// �ǂݍ��ݏI��
			if (input[i] == EOF){//�t�@�C���̏I���Ȃ�
				goto EXFILE;//�I��
			}
		}	// ��{���[�v�I��



		// �ŏ��ɐ��������ɂ��āA�����������Ă���ύX����
		iflg = false;
		{
			if ((inputc[0] != '\0') && (inputc[0] != '\n') && (inputc[0] != NULL))
				iflg = true;
		}

		// ���O��������
		if (iflg){
			anum++;	// �ŏ���0�ɂȂ�
			names.push_back(Name_t(anum+1));	// new�͂���Ȃ�
			strcpy_s(names[anum].nameC, inputc);
		}

		if (input[i] == EOF){//�t�@�C���̏I���Ȃ�
			goto EXFILE;//�I��
		}
	}

	// �t�@�C�������
EXFILE:
	FileRead_close(fp);

	// char_max = anum + 1
	char_max = (int)names.size();

	def.charMax = (int)names.size();
	def.names = names;
}

int Character_Max()
{
	return char_max;
}

Name_t Character_Name(int num)
{
	return names[num];
}