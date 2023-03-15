#include "DxLib.h"

#include "game.h"
#include "Vec2.h"

namespace
{
	constexpr int kParticleNum = 256;
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

	int gameScreen = LoadGraph("clear.png");
	int effectScreen = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);

	Vec2 pos[kParticleNum];
	Vec2 vec[kParticleNum];
	for (int i = 0; i < kParticleNum; i++)
	{
		pos[i].x = GetRand(Game::kScreenWidth);
		pos[i].y = -GetRand(Game::kScreenHeight);

		vec[i].x = 0.0f;
		vec[i].y = GetRand(20)+20;
		vec[i].y /= 10.0f;
	}

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// update
		for (int i = 0; i < kParticleNum; i++)
		{
			pos[i] += vec[i];
			if (pos[i].y > Game::kScreenHeight)
			{
				pos[i].x = GetRand(Game::kScreenWidth);
				pos[i].y = 0.0f;

				vec[i].x = 0.0f;
				vec[i].y = GetRand(20) + 20;
				vec[i].y /= 10.0f;
			}
		}
		sinRate += 0.05f;
		
		// draw
		SetDrawScreen(effectScreen);
		ClearDrawScreen();
		BeginAADraw();
		for (int i = 0; i < kParticleNum; i++)
		{
			DrawCircleAA(pos[i].x, pos[i].y, 4 * (vec[i].length() / 4.0f), 32, 0x808020, true);
		}
		EndAADraw();
		SetDrawScreen(DX_SCREEN_BACK);
		// 画面のクリア
		ClearDrawScreen();

		DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, gameScreen, false);

		
		//加算合成する
		SetDrawBlendMode(DX_BLENDMODE_ADD, 192);

		DrawGraph(0, 0, effectScreen, true);
		//画面をぼかす
		GraphFilter(effectScreen, DX_GRAPH_FILTER_GAUSS, 16, 800);
		for (int i = 0; i < 8; i++)
		{
			DrawGraph(GetRand(4)-2, GetRand(4) - 2, effectScreen, false);
		}

		GraphFilter(effectScreen, DX_GRAPH_FILTER_GAUSS, 32, 2400);
		for (int i = 0; i < 8; i++)
		{
			DrawGraph(GetRand(8) - 4, GetRand(8) - 4, effectScreen, false);
		}
		//元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawFormatString(32, 32, 0xffffff, "FPS = %f",GetFPS());
		DrawFormatString(32, 64, 0xffffff, "DC = %d", GetDrawCallCount());

		DrawFormatString(32, 720 + sinf(sinRate) * 16, 0xffffff, "画面を光らせる処理を応用して光るパーティクル ゲーム画面は光ってない");
		DrawFormatString(32, 736 + sinf(sinRate) * 16, 0xffffff, "画面サイズの描画を繰り返しているので処理は重い(はず)");
		DrawFormatString(32, 780 + sinf(sinRate) * 16, 0xffffff, "プレイヤーに注目してもらいたい部分は動かす(この文章のこと)");

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	DeleteGraph(gameScreen);
	DeleteGraph(effectScreen);

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}