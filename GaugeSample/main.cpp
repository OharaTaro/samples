#include "DxLib.h"
#include "game.h"
#include <cmath>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// windowモード設定
	ChangeWindowMode(Game::kWindowMode);
	// ウインドウ名設定
	SetMainWindowText(Game::kTitleText);
	// 画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);
	// バックグラウンド実行をONに
//	SetAlwaysRunFlag(true);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	int handle = LoadGraph("gaugeImage.bmp");
	float sinRate = 0.0f;

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		sinRate += 0.01f;
		float rate = sin(sinRate)*0.5f + 0.5f;

		DrawFormatString(64, 32, 0xffffff, "HP RATE : %f%%", rate*100);

		DrawString(64, 64, "DrawGraph 画像をそのまま表示", 0xffffff);
		DrawGraph(64, 64 + 16, handle, true);

		DrawString(64, 128, "DrawBox 残りHPの割合に応じてBoxの表示サイズを変更してHPバーに", 0xffffff);
		DrawBox(64, 128 + 16, 64 + 256 * rate, 128 + 16 + 32, 0xff0000, true);

		DrawString(64, 192, "DrawQuadrangle 四角形の各頂点の座標を指定して表示", 0xffffff);
		DrawQuadrangle(64       , 192 + 16,
			64 + 256 * rate     , 192 + 16,
			64 + 256 * rate + 16, 192 + 16 + 32,
			64 + 16             , 192 + 16 + 32,
			0xff0000, true);

		DrawString(64, 256, "DrawRectGraph HPバーに使用するグラフィックの切り取りサイズを変えることでHPの量を表現", 0xffffff);
		DrawRectGraph(64, 256+16,
			0, 0, 256 * rate, 32,
			handle, true);

		DrawString(64, 320, "DrawModiGraph グラフィックの4つの頂点座標を指定して表示することができる", 0xffffff);
		DrawModiGraph(64      , 320 + 16,
			64 + 256 * rate   , 320 + 16,
			64 + 256 * rate+16, 320 + 16+32,
			64+16, 320 + 16+32,
			handle, true);

		DrawString(64, 384, "DrawRectModiGraph DrawRectGraph+DrawModiGraph", 0xffffff);
		DrawRectModiGraph(64    , 384 + 16,
			64 + 256 * rate     , 384 + 16,
			64 + 256 * rate + 16, 384 + 16 + 32,
			64 + 16             , 384 + 16 + 32,
			0, 0, 256 * rate, 32,
			handle, true);

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