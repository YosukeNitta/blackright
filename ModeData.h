#pragma once
#include <DxLib.h>

class ModeData
{
private:
	boolean load_1s = false;

public:
	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void All()
	{
		Update();
		Draw();
	}

	// �ǂݍ���
	virtual void Load()
	{
		if (!load_1s) {
			Initialize();
			load_1s = true;
		}
		Load_Reload();
	}

	// ���[�h�ύX���̃��[�h
	// �����ɋL�q
	virtual void Load_Reload() = 0;

	// 1��݂̂̃��[�h(������)
	// �����ɋL�q
	virtual void Initialize() = 0;

	// ���
	virtual void Release() = 0;
};
