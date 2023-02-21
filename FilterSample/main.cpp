#include "DxLib.h"

#include "game.h"
#include "Vec2.h"
#include <cmath>

struct Particle
{
	Vec2 pos;
	Vec2 vec;
	int alpha;
	bool isExist;
};

namespace
{
	constexpr int kParticleNum = 256;
	constexpr int kParticleInterval = 2;
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

	int handle = LoadGraph("sample.jpg");	// org

	int handleMono = LoadGraph("sample.jpg");	// DX_GRAPH_FILTER_MONO
	GraphFilter(handleMono, DX_GRAPH_FILTER_MONO, -60, 7);

	int handleGauss = LoadGraph("sample.jpg");	// DX_GRAPH_FILTER_GAUSS
	GraphFilter(handleGauss, DX_GRAPH_FILTER_GAUSS, 16, 1400);

	int handleHsb = LoadGraph("sample.jpg");	// DX_GRAPH_FILTER_HSB
	GraphFilter(handleHsb, DX_GRAPH_FILTER_HSB, 0, 100, 90, -60);

	int handleInvert = LoadGraph("sample.jpg");	// DX_GRAPH_FILTER_INVERT
	GraphFilter(handleInvert, DX_GRAPH_FILTER_INVERT);

	int handleTwoColor = LoadGraph("sample.jpg");	// DX_GRAPH_FILTER_INVERT
	GraphFilter(handleTwoColor, DX_GRAPH_FILTER_TWO_COLOR, 128, GetColor(0, 0, 128), 255, GetColor(255, 255, 0), 255);

	float sinRate = 0.0f;

	int width, height;
	GetGraphSize(handle, &width, &height);

	Particle particle[kParticleNum];
	for (auto& part : particle)
	{
		part.pos = Vec2{ 0,0 };
		part.vec = Vec2{ 0,0 };
		part.alpha = 0;
		part.isExist = false;
	}
	int partFrame = kParticleInterval;

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		// update
		sinRate += 0.02f;
		for (auto& part : particle)
		{
			if (!part.isExist)	continue;
			part.pos += part.vec;
			part.vec.y += 0.8f;
			part.alpha -= 4;
			if (part.pos.y > Game::kScreenHeight)
			{
				part.isExist = false;
			}
			if (part.alpha < 0)
			{
				part.isExist = false;
			}
		}
		// パーティクル生成
		partFrame--;
		if(partFrame <= 0)
		{
			int count = 0;
			for (auto& part : particle)
			{
				if (part.isExist)	continue;

				part.pos.x = 1300 + sinf(sinRate) * 32.0f;
				part.pos.y = 256;
				
				part.vec.x = static_cast<float>(GetRand(8)) - 4.0f;
				part.vec.y = -static_cast<float>(GetRand(12));

				part.alpha = 255;
				part.isExist = true;
				count++;
				if(count >=4)
				{
					break;
				}
			}

			partFrame = kParticleInterval;
		}

		// draw
		DrawString(16, 8, "GraphFilterを使用してグラフィックの見た目を変化させる", 0xffffff);

	//	DrawGraph(32,32,handle,false);
		DrawModiGraph(
			32 + sinf(sinRate) * width / 2 + width / 2, 32,
			32 - sinf(sinRate) * width / 2 + width / 2, 32,
			32 - sinf(sinRate) * width / 2 + width / 2, 32+height,
			32 + sinf(sinRate) * width / 2 + width / 2, 32+height,
			handle, false);
		DrawString(32, 32 + 240, "オリジナル", 0xffffff);
		DrawString(32, 48 + 240, "頂点座標を変化させて回転しているように見せる", 0xffffff);
		
		DrawGraph(32 + 360, 32, handleMono, false);
		DrawFormatString(32 + 360, 32 + 240, 0xffffff, "DX_GRAPH_FILTER_MONO %d, %d", -60, 7);

		DrawGraph(32 + 720, 32, handleGauss, false);
		DrawFormatString(32 + 720, 32 + 240, 0xffffff, "DX_GRAPH_FILTER_GAUSS %d, %d", 16, 1400);


		DrawGraph(32, 32 + 300, handleHsb, false);
		DrawFormatString(32, 32 + 240 + 300, 0xffffff, "DX_GRAPH_FILTER_HSB %d, %d, %d, %d", 0, 100, 90, -60);

		DrawGraph(32 + 360, 32 + 300, handleInvert, false);
		DrawFormatString(32 + 360, 32 + 240 + 300, 0xffffff, "DX_GRAPH_FILTER_INVERT");

		DrawGraph(32 + 720, 32 + 300, handleTwoColor, false);
		DrawFormatString(32 + 720, 32 + 240 + 300, 0xffffff, "DX_GRAPH_FILTER_TWO_COLOR, 128, GetColor(0, 0, 128), 255, GetColor(255, 255, 0), 255");

		DrawString(16, 8 + 600, "1ラインずつ描画して変形させる", 0xffffff);

		float rate = sinf(sinRate);
		rate *= rate;
		int randSize = 128 * rate;

		for (int y = 0; y < height; y++)
		{
			int x = 64 + GetRand(randSize) - randSize/2;
			
			DrawRectGraph( x, y + 32 + 600,
				0, y, width, 1,
				handle, false, false);
		}
		DrawString(32, 8 + 870, "※ランダム幅はsin()を利用して周期的に変化させる", 0xffffff);

		for (int x = 0; x < width; x++)
		{
			int y = 32 + 600 + GetRand(randSize) - randSize / 2;

			DrawRectGraph(x + 500, y,
				x, 0, 1, height,
				handle, false, false);
		}

		for (int y = 0; y < height; y++)
		{
			float lineSinRate = static_cast<float>(y*32) / height + sinRate;
			int x = 32 + 1000 + randSize * sinf(lineSinRate);

			DrawRectGraph(x, y + 32 + 600,
				0, y, width, 1,
				handle, false, false);
		}

		// パーティクル描画
		DrawString(1200, 32 + 240, "パーティクル", 0xffffff);
		DrawString(1200, 32 + 240 + 24, "丸を上の方に移動するベクトルを\n持った状態で生成後、\n透明にしつつ重量にしたがって落とす", 0xffffff);
		for (auto& part : particle)
		{
			if (!part.isExist)	continue;
			int r = GetRand(255);
			int g = GetRand(255);
			int b = GetRand(255);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, part.alpha);
		//	DrawPixel(part.pos.x, part.pos.y, GetColor(r,g,b));
		//	DrawCircleAA(part.pos.x, part.pos.y,2.0f, 8, GetColor(r, g, b), true);
			DrawCircleAA(part.pos.x, part.pos.y, 1.0f, 8, 0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

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