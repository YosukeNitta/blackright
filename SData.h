// ヘッダファイルを作成する場合は、無限ループを防ぐために、インクルードガードを記載します。
// …らしいけどわかってないので使っちゃう
#pragma once
#include "PSet.h"
#include "SSet.h"

namespace stateData
{

	class StateData
	{
	public:
		//Player P1, P2;
		//System_t S;
		//Helper H1[HM], H2[HM];

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

		// stateに戻す
		void Send()
		{
			
		}
	};

}// namespace stateData

namespace modeData
{

	class ModeData
	{
	private:
		boolean load_1s = false;

	public:

		virtual int Mode()
		{
			return 0;
		}
		virtual void Draw() = 0;

		virtual void All()
		{
			Mode();
			Draw();
		}

		// mainで読み込む
		virtual int Load_First()
		{
			if (!load_1s){
				Load_1second();
				load_1s = true;
			}
			Load_Reload();

			return 0;
		}

		// モード変更時のロード
		// ここに記述
		virtual int Load_Reload()
		{
			return 0;
		}

		// 1回のみのロード
		// ここに記述
		virtual void Load_1second() = 0;
	};

}// namespace stateData