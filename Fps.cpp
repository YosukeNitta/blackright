#include "pch.h"
#include "Fps.h"

/*
static int mStartTime;      //����J�n����
static int mCount;          //�J�E���^
static float mFps;          //fps
static const int N = 60;	//60 ���ς����T���v����
static const int FPS = 60;	//60 �ݒ肵��FPS

bool Update(){
	if (mCount == 0){ // 1�t���[���ڂȂ玞�����L��
		mStartTime = GetNowCount();
	}
	if (mCount == N){ // 60�t���[���ڂȂ畽�ς��v�Z����
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void Draw(){
	DrawFormatString(SCREEN_W - 80, 0, GetColor(255, 255, 255), "FPS %.1f", mFps);
}

// ���΂炭�҂�
void Wait(){
	int tookTime = GetNowCount() - mStartTime;		// ������������
	int waitTime = mCount * 1000 / FPS - tookTime;	// �҂ׂ�����
	if (waitTime > 0){
		Sleep(waitTime);				// �ҋ@
	}
}
*/
bool Fps::Update() {
	if (mCount == 0) { // 1�t���[���ڂȂ玞�����L��
		mStartTime = GetNowCount();
	}
	if (mCount == N) { // 60�t���[���ڂȂ畽�ς��v�Z����
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void Fps::Draw() {
	DrawFormatString(SCREEN_W - 80, 0, GetColor(255, 255, 255), "FPS %.1f", mFps);
}

void Fps::Wait() {
	int tookTime = GetNowCount() - mStartTime;		// ������������
	int waitTime = mCount * 1000 / FPS - tookTime;	// �҂ׂ�����
	if (waitTime > 0) {
		Sleep(waitTime);				// �ҋ@
	}
}