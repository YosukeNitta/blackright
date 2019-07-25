#include "pch.h"
#include "Fps.h"

/*
static int mStartTime;      //測定開始時刻
static int mCount;          //カウンタ
static float mFps;          //fps
static const int N = 60;	//60 平均を取るサンプル数
static const int FPS = 60;	//60 設定したFPS

bool Update(){
	if (mCount == 0){ // 1フレーム目なら時刻を記憶
		mStartTime = GetNowCount();
	}
	if (mCount == N){ // 60フレーム目なら平均を計算する
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

// しばらく待つ
void Wait(){
	int tookTime = GetNowCount() - mStartTime;		// かかった時間
	int waitTime = mCount * 1000 / FPS - tookTime;	// 待つべき時間
	if (waitTime > 0){
		Sleep(waitTime);				// 待機
	}
}
*/
bool Fps::Update() {
	if (mCount == 0) { // 1フレーム目なら時刻を記憶
		mStartTime = GetNowCount();
	}
	if (mCount == N) { // 60フレーム目なら平均を計算する
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
	int tookTime = GetNowCount() - mStartTime;		// かかった時間
	int waitTime = mCount * 1000 / FPS - tookTime;	// 待つべき時間
	if (waitTime > 0) {
		Sleep(waitTime);				// 待機
	}
}