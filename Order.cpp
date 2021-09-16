//==================================================================================
//
//			���ʖ��߂̊Ǘ���
//
//========================================================================

//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include <iostream>	// ���t�Ŏg�p
#include <string>

//�f�t�@�C����----------------------------------------------



//�ް���`��------------------------------------------------


//�����֐��錾��--------------------------------------------
static int anten;
static int fadeout;
static int GHandle;
//�v���O������----------------------------------------------

// �`��u�����h���[�h���A���t�@�u�����h�i�����j�ɂ���
void SetAlpha(int n)
{
	
	if (n < 0)n=0;
	else if (n > 255)n = 255;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, n);
}

// �`��u�����h���[�h���m�[�u�����h�ɂ���
void NoBlend()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// �Ó]�@�\
void Anten(int num)
{
	if (num == 255)anten = 255;

	// �����l�p��\��
	if (anten > 0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, anten);
		DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		anten -= 20;
	}
	else{
		anten = 0;
	}
}

// �Ó]�m�F
boolean AntenCheck()
{
	if (anten <= 0)return false;
	return true;
}

// �t�F�[�h�A�E�g
void Fadeout(int num)
{
	if (num == 0)anten = 0;

	// �����l�p��\��
	if (anten > 0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, anten);
		DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		anten += 20;
	}
	else{
		anten = 0;
	}
}

// �t�F�[�h�A�E�g�m�F
boolean FadeoutCheck()
{
	if (fadeout >= 255)return false;
	return true;
}

// �X�N���[���V���b�g�擾
void ScreenShot()
{
	SEStart(28);
	// ���t�擾
	DATEDATA Date;
	// ���ݎ����𓾂�
	GetDateTime(&Date);

	std::string str;	// ������(���ꂪ�摜��)
	std::string year = std::to_string(Date.Year);
	std::string mon = std::to_string(Date.Mon);
	std::string day = std::to_string(Date.Day);
	std::string hour = std::to_string(Date.Hour);
	std::string min = std::to_string(Date.Min);
	std::string sec = std::to_string(Date.Sec);
	std::string fn = ".bmp";
	if (mon.size() == 1)mon = "0" + std::to_string(Date.Mon);	// 0������
	if (day.size() == 1)day = "0" + std::to_string(Date.Day);	// 0������
	if (hour.size() == 1)hour = "0" + std::to_string(Date.Hour);	// 0������
	if (min.size() == 1)min = "0" + std::to_string(Date.Min);	// 0������
	if (sec.size() == 1)sec = "0" + std::to_string(Date.Sec);	// 0������

	str = "SS_" + year + "_" + mon + day + "_" + hour + min + sec + fn;
	SaveDrawScreen(0, 0, SCREEN_W, SCREEN_H, str.c_str());
}

void SetFont()
{
	// ********** �t�H���g�̃��[�h **********
	LPCSTR font_path = "ob/PixelMplus-20130602/PixelMplus10-Regular.ttf"; // �ǂݍ��ރt�H���g�t�@�C���̃p�X
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
	}

	// ********** ��������ǂݍ��񂾃t�H���g�𗘗p�ł��� **********
	SetFontSize(20);	// ���₷���̂��߁A�t�H���g�T�C�Y��傫��
	ChangeFont("PixelMplus10", DX_CHARSET_DEFAULT);

	// ********** �t�H���g�̃A�����[�h **********
	if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
	}
	else {
		MessageBox(NULL, "remove failure", "", MB_OK);
	}
}

bool Box_Touch(int x1, int y1, int w1, int h1,
	int x2, int y2, int w2, int h2)
{
	// ���T�C�Y��0�ȉ��Ȃ�
	if ((w1 == 0) || (w2 == 0))return false;

	// x�`�F�b�N
	if ((x1 >= x2 && x1 <= x2 + w2) ||
		(x2 >= x1 && x2 <= x1 + w1))
	{
		// y�`�F�b�N
		if (((y1 >= y2 && y1 <= y2 + h2) ||
			(y2 >= y1 && y2 <= y1 + h1)))
		{
			return true;	// �ڐG
		}
	}
	return false;
}

void SaveScreen()
{
	// ��ʂ̃T�C�Y�Ɠ����T�C�Y�̃O���t�B�b�N���쐬
	GHandle = MakeGraph(SCREEN_W, SCREEN_H);

	// ��ʃf�[�^�̎�肱��
	GetDrawScreenGraph(0, 0, SCREEN_W, SCREEN_H, GHandle);
}

void Draw_SaveScreen()
{
	if(GHandle != 0)
		DrawGraph(0, 0, GHandle, false);
}

// ���� (999�܂ł̐��Ŋm�����w��)
boolean random(int num)
{
	boolean f;
	if (GetRand(999) < num)f = true;
	else { f = false; }

	return f;
}