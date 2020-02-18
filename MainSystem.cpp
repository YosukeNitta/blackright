#include "pch.h"
#include "MainSystem.h"
#include <memory>

//	クラスのインスタンスを取得
MainSystem& MainSystem::Instance()
{
	// memoryを使って何かを行っている？
	static std::unique_ptr<MainSystem> instance(new MainSystem);
	// インスタンス？を返す
	return *instance;
}

// メインメソッド(これに各種モード)
void MainSystem::MainMove()
{
	// ないなら止める
	if (!_currentMode)
		return;

	// モード読み込み
	if (_currentState == 0) {
		_currentMode->Load_First();	// 読み込み
		_currentState = 1;	// 実行モードへ
	}

	// モード進行
	_currentMode->All();

	//	解放処理
	if (_currentState == 2) {
		_currentMode->Release();
		_currentMode->Release();	// 解放処理
		_currentMode = _nextMode;	// 次のモードセット
		_currentState = 0;
	}
}