#pragma once
#include <unordered_map>
#include "SData.h"
#include "Fps.h"

#include "Title.h"
#include "Menu.h"
#include "Select.h"
#include "Replay.h"
#include "Arcade.h"
#include "Versus.h"
#include "Result.h"
#include "Network.h"

class MainSystem {

private:
	std::unordered_map<std::string, MData*> _modes;	// シーンとキーをここにまとめる
	MData* _currentMode;	//	現在実行しているシーン
	MData* _nextMode;	//	現在実行しているシーン
	int _currentState = 0;	// 0で開始 1でUpdate 2でリリース
	bool _endMainSystem = false;
	Fps _fps;
	boolean _drawFPS;			// FPS表示

	// シングルトンクラスなのでコンストラクタ周りを隠す
	// コンストラクタ
	MainSystem() {
		_modes["Title"] = new Title;
		_modes["Menu"] = new Menu;
		_modes["Select"] = new Select;
		_modes["Arcade"] = new Arcade;
		_modes["Versus"] = new Versus;
		_modes["Result"] = new Result;
		_modes["Replay"] = new Replay;
		_modes["Network"] = new Network;
		_currentMode = _modes["Title"];	// 初期モード決定
		_nextMode = _currentMode;
		//_currentMode->Load_First();	// 読み込み

		// 設定
		_drawFPS = GetPrivateProfileInt("Config", "drawfps", 1, "./config.ini");
	}
	//MainSystem(const MainSystem&) {}
	//MainSystem& operator=(const MainSystem&) {}
public:
	//	インスタンスを取得
	static MainSystem& Instance();

	// メインメソッド(これに各種モード)
	void MainLoop();

	void Update();

	void MainDraw()
	{

	}

	void SetNextMode(std::string next)
	{
		// 探してないなら終了
		if (!_modes.count(next))
			return;

		_nextMode = _modes[next];
		_currentState = 2;
	}

	// これを終了する
	void EndMainSystem()
	{
		_endMainSystem = true;
	}

	// これを終了する
	bool CheckEnd()
	{
		return _endMainSystem;
	}
};