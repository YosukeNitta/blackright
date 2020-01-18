#include <DxLib.h>
#include <memory>
#include "SceneManager.h"

//	デストラクタ
SceneManager::~SceneManager()
{
	for (auto scene : _scenes) {
		delete scene.second;
	}
}

//	クラスのインスタンスを取得
SceneManager & SceneManager::Instance()
{
	static std::unique_ptr<SceneManager> instance(new SceneManager);

	return *instance;
}

// シーンを追加
void SceneManager::AddScene(std::string sceneName, Scene* scene)
{
	if (_scenes.count(sceneName))
		return;
	_scenes[sceneName] = scene;
}

//	開始シーンを設定
void SceneManager::StartScene(std::string sceneName)
{
	if (!_scenes.count(sceneName))
		return;

	_currentScene = _scenes[sceneName];
}


//	シーン遷移
void SceneManager::LoadScene(std::string sceneName)
{
	if (!_scenes.count(sceneName))
		return;

	_nextScene = _scenes[sceneName];
	_state = SceneState::Release;
}

//	更新処理
void SceneManager::Update()
{
	//	メインループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//	背景をクリアする
		ClearDrawScreen();

		//	シーンを更新する
		SceneUpdate();

		//	更新
		ScreenFlip();
	}
}

//	シーンの更新処理
void SceneManager::SceneUpdate()
{
	if (!_currentScene)
		return;
	//	初期化
	if (_state == SceneState::Initialize) {
		_currentScene->Initialize();
		_state = SceneState::Update;
	}

	//	更新処理
	_currentScene->Update();

	//	解放処理
	if (_state == SceneState::Release) {
		_currentScene->Release();
		NextScene();
		_state = SceneState::Initialize;
	}
}

//	次のシーンに切り替える
void SceneManager::NextScene()
{
	_currentScene = _nextScene;
}
