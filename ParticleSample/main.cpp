#include "DxLib.h"

#include "game.h"
#include "Vec2.h"
#include <cmath>
#include <array>
#include <memory>

#include "ParticleBase.h"

namespace
{
	constexpr int kParticleNum = 4096;
	constexpr int kShowerInterval = 2;
	constexpr int kFlowerInterval = 120;
	constexpr int kAuraInterval = 2;
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// windowモード設定
	ChangeWindowMode(Game::kWindowMode);
	// ウインドウ名設定
	SetMainWindowText(Game::kTitleText);
	// 画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetAlwaysRunFlag(true);


	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	float sinRate = 0.0f;

//	ParticleBase particle[kParticleNum];
	std::array<std::shared_ptr<ParticleBase>, kParticleNum> particle;
	for (auto& pPart : particle)
	{
		pPart = std::make_shared<ParticleBase>();
	}
	int showerFrame = kShowerInterval;
	int flowerFrame = kFlowerInterval;
	int auraFrame = kAuraInterval;

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		// update
		sinRate += 0.20f;
		int mouseX, mouseY;
		GetMousePoint(&mouseX, &mouseY);

		for (auto& pPart : particle)
		{
			if (!pPart->isExist())	continue;
			pPart->update();
		}
		// パーティクル生成
		showerFrame--;
		if (showerFrame <= 0)
		{
			int count = 0;
			// 発生位置から上に飛んで落ちていく
			for (auto& pPart : particle)
			{
				if (pPart->isExist())	continue;

				Vec2 pos;
				pos.x = 1300 + sinf(sinRate) * 32.0f;
				pos.y = 256;

				Vec2 vec;
				vec.x = static_cast<float>(GetRand(8)) - 4.0f;
				vec.y = -static_cast<float>(GetRand(12));

				pPart->start(pos);
				pPart->setVec(vec);
				pPart->setGravity(0.8f);

				count++;
				if (count >= 4)
				{
					break;
				}
			}

			showerFrame = kShowerInterval;
		}

		flowerFrame--;
		
		if (flowerFrame <= 0)
		{
			int count = 0;
			// 発生位置から上に飛んで落ちていく
			for (auto& pPart : particle)
			{
				if (pPart->isExist())	continue;

				float randSin = static_cast<float>(GetRand(360)) / 360.0f;
				randSin *= DX_TWO_PI_F;
				float randSpeed = static_cast<float>(GetRand(160)) / 10.0f + 1.0f;

				Vec2 pos;
				pos.x = 256 + cosf(randSin) * 2.0f;
				pos.y = 256 + sinf(randSin) * 2.0f;

				Vec2 vec;
				vec.x = cosf(randSin) * randSpeed;
				vec.y = sinf(randSin) * randSpeed;

				pPart->start(pos);
				pPart->setVec(vec);
				pPart->setRadius(2.0f);
				pPart->setColor(0xff2020);
				pPart->setGravity(0.4f);

				count++;
				if (count >= 64)
				{
					break;
				}
			}
			flowerFrame = kFlowerInterval;
		}
		auraFrame--;
		if (auraFrame <= 0)
		{
			int count = 0;
			for (auto& pPart : particle)
			{
				if (pPart->isExist())	continue;

				float randSin = static_cast<float>(GetRand(360)) / 360.0f;
				randSin *= DX_TWO_PI_F;

				float randDist = GetRand(6) + 29.0f;

				Vec2 pos;
				pos.x = 512 + cosf(randSin) * randDist;
				pos.y = 256 + sinf(randSin) * randDist * 0.5f;

				Vec2 vec;
				vec.x = 0.0f;
				vec.y = -GetRand(4) - 1.0f;

				pPart->start(pos);
				pPart->setVec(vec);
				pPart->setRadius(4.0f);
			//	pPart->setColor(0xa0a0ff);
				pPart->setColor(0x802010);
				pPart->setGravity(-0.4f);
				pPart->setAlphaDec(16);
				pPart->setRadiusAcc(0.1f);

				count++;
				if (count >= 64)
				{
					break;
				}
			}

			count = 0;
			for (auto& pPart : particle)
			{
				if (pPart->isExist())	continue;

				float randSin = static_cast<float>(GetRand(360)) / 360.0f;
				randSin *= DX_TWO_PI_F;
				float randSpeed = static_cast<float>(GetRand(60)) / 10.0f + 1.0f;

				Vec2 pos;
				pos.x = mouseX + cosf(sinRate) * 128.0f;
				pos.y = mouseY + sinf(sinRate) * 128.0f;

				Vec2 vec;
				vec.x = cosf(randSin) * randSpeed;
				vec.y = sinf(randSin) * randSpeed;

				pPart->start(pos);
				pPart->setVec(vec);
				pPart->setRadius(4.0f);
				pPart->setColor(0xf0f0ff);
				pPart->setGravity(0.0f);
				pPart->setAlphaDec(16);

				count++;
				if (count >= 32)
				{
					break;
				}
			}

			count = 0;
			for (auto& pPart : particle)
			{
				if (pPart->isExist())	continue;

				float randSin = static_cast<float>(GetRand(360)) / 360.0f;
				randSin *= DX_TWO_PI_F;
				float randSpeed = static_cast<float>(GetRand(60)) / 10.0f + 1.0f;

				Vec2 pos;
				float dist = static_cast<float>(128 + GetRand(32));
				pos.x = 256*3 + cosf(randSin) * dist;
				pos.y = 512 + sinf(randSin) * dist;
				
				Vec2 vec;
				vec.x = -cosf(randSin) * randSpeed;
				vec.y = -sinf(randSin) * randSpeed;

				pPart->start(pos);
				pPart->setVec(vec);
				pPart->setRadius(4.0f);
				pPart->setColor(0x80ff80);
				pPart->setGravity(0.0f);
				pPart->setAlphaDec(16);
				pPart->setRadiusAcc(-0.05f);

				count++;
				if (count >= 32)
				{
					break;
				}
			}

			auraFrame = kAuraInterval;
		}


		// draw
		DrawString(16, 8, "パーティクル(という名のただの丸)を移動させて演出を行う", 0xffffff);

		DrawString(512-64, 300, "sin,cosを使ってパーティクルの発生位置を楕円上にする\n重力を上方向に設定してだんだん早く移動するように", 0xffffff);

		DrawString(1200, 32 + 240 + 24, "丸を上の方に移動するベクトルを\n持った状態で生成後、\n透明にしつつ重量にしたがって落とす", 0xffffff);

		DrawString(mouseX, mouseY + 64, "マウスのまわりを回るようにパーティクル生成", 0xffffff);

		int count = 0;
		for (auto& pPart : particle)
		{
			if (!pPart->isExist())	continue;
			pPart->draw();
			count++;
		}
		DrawFormatString(24, 24, 0xffffff, "Particle Num : %d", count);
		DrawFormatString(24, 40, 0xffffff, "DrawCall : %d", GetDrawCallCount());

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}