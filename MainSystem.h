#pragma once
#include <unordered_map>
#include "SData.h"
#include "Fps.h"

#include "Title.h"
#include "Menu.h"
#include "Select.h"
#include "Config.h"
#include "Replay.h"
#include "Arcade.h"
#include "Versus.h"
#include "Result.h"
#include "Network.h"

#include "Socket.h"

/*

これの中でモード機能を実行する？

*/
class MainSystem {
protected:
	
public:
	std::unordered_map<std::string, MData*> _modes;	// シーンとキーをここにまとめる
	MData* _currentMode;	//	現在実行しているシーン
	MData* _nextMode;	//	次に実行するシーン
	int _currentState = 0;	// 0で開始 1でUpdate 2でリリース
	bool _endMainSystem = false;
	Fps _fps;
	boolean _drawFPS;			// FPS表示
	Socket* _socket;		// 通信用クラス
	int _modeNum;	// currentModeの番号

	// シングルトンクラスなのでコンストラクタ周りを隠す
	// コンストラクタ
	MainSystem() {
		_modes["Title"] = new Title;
		_modes["Menu"] = new Menu;
		_modes["Select"] = new Select;
		_modes["Config"] = new Config;
		_modes["Arcade"] = new Arcade;
		_modes["Versus"] = new Versus;
		_modes["Result"] = new Result;
		_modes["Replay"] = new Replay;
		_modes["Network"] = new Network;
		_currentMode = _modes["Title"];	// 初期モード決定
		_nextMode = _currentMode;	// 初期モードを設定しておく
		//_currentMode->Load_First();	// 読み込み

		// 設定
		_drawFPS = GetPrivateProfileInt("Config", "drawfps", 1, "./config.ini");
		//_socket();
		_modeNum = SceneNum(NoScene);
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

	void Game_Processing();

	//======================
	// ネットワーク用
	//======================
	// ポインタ渡す
	Socket GetSocket() {
		return *_socket;
	}

	// ポインタ取得
	void SetSocket(Socket* soc) {
		_socket = soc;
	}
};

// ネットワークのオンオフ
boolean GetNetOn();