#include "pch.h"
//#include <DxLib.h>
#include <memory>
#include "ModeManager.h"

//	デストラクタ
ModeManager::~ModeManager()
{
	// モード配列をすべて削除
	for (auto scene : _modes) {
		delete scene.second;
	}
}

//	インスタンスを取得
ModeManager& ModeManager::Instance() {
	static std::unique_ptr<ModeManager> instance(new ModeManager);

	return *instance;
}

void ModeManager::AddMode(std::string modeName, ModeData* scene)
{
	if (_modes.count(modeName))
		return;
	_modes[modeName] = scene;
}

//	開始シーンを設定
void ModeManager::StartMode(std::string modeName) {

	if (!_modes.count(modeName))
		return;

	_currentMode = _modes[modeName];
}

// モード変更
void ModeManager::LoadMode(std::string modeName)
{
	if (!_modes.count(modeName))
		return;

	_nextMode = _modes[modeName];	// 次のモードを設定
	_state = ModeState::Release;	// モードの状態をリリースにする
}

void ModeManager::Update() 
{
	ModeUpdate();
}

//	シーンの更新処理
void ModeManager::ModeUpdate()
{
	if (!_currentMode)
		return;
	//	初期化
	if (_state == ModeState::Initialize) {
		_currentMode->Initialize();
		_state = ModeState::Update;
	}

	//	更新処理
	_currentMode->Update();

	//	解放処理
	if (_state == ModeState::Release) {
		_currentMode->Release();
		NextMode();	// ここでモード切替
		_state = ModeState::Initialize;
	}
}

//	次のシーンに切り替える
void ModeManager::NextMode()
{
	_currentMode = _nextMode;	// 切り替え
}
