// ヘッダファイルを作成する場合は、無限ループを防ぐために、インクルードガードを記載します。
#pragma once
#include "pch.h"

namespace stateData
{

class StateData
{
public:
	void PSet(Player GP1, Player GP2)
	{
		P1 = GP1;
		P2 = GP2;
	}

	void PHSet(Player GP, int side)
	{
		if (side == 1)P1Set(GP);
		else if (side == 2)P2Set(GP);
	}

	void P1Set(Player GP1)
	{
		P1 = GP1;
	}

	void P2Set(Player GP2)
	{
		P2 = GP2;
	}

	void SSet(System_t GS)
	{
		S = GS;
	}

	void PSSet(Player GP1, Player GP2, System_t GS)
	{
		P1 = GP1;
		P2 = GP2;
		S = GS;
	}

	void HSet(Helper GH1[], Helper GH2[])
	{
		int i;
		for (i = 0; i < HM; i++){
			H1[i] = GH1[i];
			H2[i] = GH2[i];
		}
	}
};

}// namespace stateData