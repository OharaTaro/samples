#include "DxLib.h"

#include "game.h"

namespace
{
	constexpr int kStretchWindowWidth = 640;
	constexpr int kStretchWindowHeight = 180;
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

	int window = LoadGraph("window.png");
	int player = LoadGraph("player.png");

	int frameCount = 0;

	int width = kStretchWindowWidth;
	int height = kStretchWindowHeight;

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();

		frameCount++;
		if (frameCount > 120 * 3)
		{
			if (frameCount > 120 * 3 + 60)
			{
				width -= 8;
				height++;
			}
			else
			{
				width += 8;
				height--;
			}
		}
		if (frameCount > 120 * 4)
		{
			frameCount = 0;
			width = kStretchWindowWidth;
			height = kStretchWindowHeight;
		}

		// 画面のクリア
		ClearDrawScreen();

		DrawString(16, 16, "UI用の素材を使用して可変サイズのウインドウを表示するサンプル", 0xffffff);

		DrawGraph(64,64, window, true);
		DrawString(64, 64 + 96, "↑96x96の素材を使用してウインドウを表示する", 0xffffff);

		DrawExtendGraph(64, 220, 64 + width, 220 + height, window, true);
		DrawString(64, 220 + kStretchWindowHeight, "↑ダメな例　単純に拡大する　角の部分の丸みやアスペクト比がウインドウサイズによって変化してしまう", 0xffffff);

		DrawGraph(64, 450, window, true);
		DrawLine(64, 450 + 32, 64 + 96, 450 + 32,0xf0f0f0);
		DrawLine(64, 450 + 64, 64 + 96, 450 + 64, 0xf0f0f0);
		DrawLine(64 + 32, 450, 64 + 32, 450 + 96, 0xf0f0f0);
		DrawLine(64 + 64, 450, 64 + 64, 450 + 96, 0xf0f0f0);
		DrawString(64, 450 + 96, "↑96x96の素材9個に分割して角、各辺、ウインドウ内部として使用する", 0xffffff);

		// 256*96のウインドウ表示
		DrawRectGraph(64, 600,  0,  0, 32, 32, window, true);	// 左上
		DrawRectGraph(64, 600 + height - 32,  0, 64, 32, 32, window, true);	// 左下
		DrawRectGraph(64 + width - 32, 600, 64,  0, 32, 32, window, true);	// 右上
		DrawRectGraph(64 + width - 32, 600 + height - 32, 64, 64, 32, 32, window, true);	// 右下
		DrawString(64, 600 + kStretchWindowHeight, "1.角をそれぞれ等倍で表示", 0xffffff);
		if (frameCount > 120 * 1)
		{
			DrawRectExtendGraph(64 + 32, 600, 64 + width - 32, 600 + 32, 32, 0, 32, 32, window, true);	// 上辺
			DrawRectExtendGraph(64 + 32, 600 + height - 32, 64 + width - 32, 600 + height - 32 + 32, 32, 64, 32, 32, window, true);	// 下辺
			DrawRectExtendGraph(64, 600 + 32, 64 + 32, 600 + height - 32, 0, 32, 32, 32, window, true);// 左辺
			DrawRectExtendGraph(64 + width - 32, 600 + 32, 64 + width, 600 + height - 32, 64, 32, 32, 32, window, true);// 右辺
			DrawString(64, 600 + kStretchWindowHeight + 16, "2.各辺を並べて表示　伸ばして問題なさそうなら伸ばして表示", 0xffffff);
		}
		
		if (frameCount > 120 * 2)
		{
			DrawRectExtendGraph(64 + 32, 600 + 32, 64 + width - 32, 600 + height - 32, 32, 32, 32, 32, window, true);	// ウインドウ内部
			DrawString(64, 600 + kStretchWindowHeight + 32, "3.ウインドウの中身をを並べて表示　伸ばして問題なさそうなら伸ばして表示", 0xffffff);
		}
		
		DrawRectRotaGraph(1280, 120, (frameCount/8) % 2 * 32, 0, 32, 32, 1.0, 0, player, true);
		DrawString(1200, 120 + 32, "元画像", 0xffffff);

		DrawRectRotaGraph(1280, 240, (frameCount / 8) % 2 * 32, 0, 32, 32, 2.0, 0, player, true);
		DrawRectRotaGraph(1400, 240, (frameCount / 8) % 2 * 32, 0, 32, 32, 4.0, 0, player, true);
		DrawString(1200, 240 + 64, "縦横同じ比率で拡大→〇", 0xffffff);

		DrawRectRotaGraph3F(1280, 400, (frameCount / 8) % 2 * 32, 0, 32, 32, 16, 16, 4.0, 1.0,0.0,player,true);
		DrawRectRotaGraph3F(1400, 400, (frameCount / 8) % 2 * 32, 0, 32, 32, 16, 16, 1.0, 5.0, 0.0, player, true);
		DrawRectRotaGraph3F(1520, 400, (frameCount / 8) % 2 * 32, 0, 32, 32, 16, 16, 2.5, 2.0, 0.0, player, true);
		DrawString(1200, 400 + 64, "アスペクト比(縦横比)がおかしい→×", 0xffffff);
		DrawString(1200, 400 + 64 + 16, "著作権的に改変ととられる恐れあり", 0xffffff);
		DrawString(1200, 400 + 64 + 32, "それ以前に見た目がおかしい、と気付ける感覚を", 0xffffff);
		DrawString(1200, 400 + 64 + 48, "ダメージを受けた時に一瞬変形、等はあり", 0xffffff);
		DrawString(1200, 400 + 64 + 64, "ただし著作権は…", 0xffffff);
		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	DeleteGraph(window);

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}