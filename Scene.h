#pragma once

//	シーン基底クラス
class Scene
{
public:
	//	初期化
	virtual void Initialize() = 0;

	//	更新
	virtual void Update() = 0;

	//	解放
	virtual void Release() = 0;
};
