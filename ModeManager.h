#pragma once
#include "ModeData.h"
#include "SData.h"
#include <unordered_map>

enum ModeState {
	Initialize,	//	初期化
	Update,	//	更新
	Release	//	ゲームシーン
};

//	シーン管理クラス
class ModeManager
{
private:
	ModeManager() : _state(ModeState::Initialize),		// 宣言
		_currentMode(nullptr), _nextMode(nullptr) {}
	ModeManager(const ModeManager&) {}
	ModeManager& operator=(const ModeManager&) {}
public:
	//	デストラクタ
	~ModeManager();

	//	インスタンスを取得
	static ModeManager& Instance();

	//	シーンを追加する
	void AddMode(std::string modeName, ModeData* scene);

	//	開始シーンを設定
	void StartMode(std::string modeName);

	//	モード移動
	void LoadMode(std::string modeName);

	//	更新処理
	void Update();

private:
	//	シーンの更新処理
	void ModeUpdate();

	//	次のシーンの存在有無を確認
	void NextMode();

private:
	ModeData* _currentMode;	//	現在実行しているシーン
	ModeData* _nextMode;		//	次に実行するシーン
	ModeState _state;			//	シーンの状態
	std::unordered_map<std::string, ModeData*> _modes;	// シーンとキーをここにまとめる
};
