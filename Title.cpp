//�C���N���[�h��--------------------------------------------
#include "pch.h"
#include "Title.h"
#include "MainSystem.h"

static int titleG;
//static int tGLogo;
static int fill;
static boolean load;
static int timeStop;
static int kettei;
static int alpha = 255;	// �����x
static boolean change = true;

//���j���[���
int Title::Mode()
{
	

	/*
	// ���[�h
	if (!load)
	{
		fill = 255;
		load = true;
	}

	// �h��Ԃ�
	if (fill > 0){
		fill -= 6;
	}
	if (fill < 0){
		fill = 0;
	}
	*/

	if (change) {
		if (alpha > 205)alpha -= 3;
		alpha += 6;
	}
	else {
		if (alpha < 50)alpha += 3;
		alpha -= 6;
	}

	if (alpha < 0) {
		change = true;
		alpha = 0;
	}
	else if (alpha > 255) {
		change = false;
		alpha = 255;
	}

	// �`��
	{
		// �摜
		//DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(220,220,220), true);
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawGraph(0, 0, titleG, true);
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//DrawGraph(0, 0, tGLogo, true);

		SetAlpha(alpha);
		DrawString(240, 400, "PRESS START BUTTON", Cr);
		NoBlend();
	}

	// �L�[����
	// ����L�[������
	if ((P_BInput(1) == 1 || P_BInput(3) == 1 || P_BInput(7) == 1) && (kettei == 0))
	{
		kettei = 1;
		SEStart(35);
	}

	// ���肵�Ă���J�E���g���Z
	if (kettei == 1) {
		timeStop += 1;
	}
	else {
		timeStop = 0;
	}


	// �L�[���͂ŏI��
	if (timeStop >= 10) {
		load = false;
		//ModeChange(SceneNum(MenuScene));
		MainSystem::Instance().SetNextMode("Menu");
		kettei = 0;
		
	}
	return 0;	//�ǂ��ɂ�������Ȃ�������I��

}	//���j���[�I��

void Title::Draw()
{

}

void Title::Load_1second()
{
	//titleG = LoadGraph("ob/title.png");
}

void Title::Load_Reload()
{
	titleG = LoadGraph("ob/title.png");
}


void Title::Release(void)
{
	// �I������
	DeleteGraph(titleG);
	//titleG = LoadGraph("ob/title.png");
}