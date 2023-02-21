#include "DxLib.h"

#include "game.h"
#include "Vec2.h"

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

	int handle = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);

	float sinRate = 0.0f;
	Vec2 basePos{128, 128+32};

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		// update
		float randSinRate = static_cast<float>(GetRand(360)) / 360.0f * DX_TWO_PI_F;
		sinRate += 0.05f;	// 一定方向に変化する角度
		basePos.x += 4.0f;
		if (basePos.x > Game::kScreenWidth)
		{
			basePos.x = 0.0f;
		}

		// draw
		SetDrawScreen(handle);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 16);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawString(16, 8, "sin(),cos()を使用していい感じに動かす", 0xffffff);
		DrawFormatString(16, 24, 0xffffff, "DrawCall: %d", GetDrawCallCount());

		// シンプルなサインカーブ
		Vec2 pos = basePos;
		pos.y += sinf(sinRate) * 100;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);
		DrawString(128, 128, "シンプルなサインカーブ", 0xffffff);

		pos = Vec2{196, 280+128};
		DrawString(pos.x, pos.y, "cos(),sin()で\n円の軌道を生成する", 0xffffff);
		pos.x += cosf(sinRate) * 128;
		pos.y += sinf(sinRate) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280 * 1, 280 + 128 };
		DrawString(pos.x, pos.y, "XYの周期を変更することで\n軌道を変化させる", 0xffffff);
		pos.x += cosf(sinRate) * 128;
		pos.y += sinf(sinRate*2) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280 * 2, 280 + 128 };
	//	DrawString(pos.x, pos.y, "周期を変更することで\n軌道を変化させる", 0xffffff);
		pos.x += cosf(sinRate*4) * 128;
		pos.y += sinf(sinRate) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280 * 3, 280 + 128 };
		DrawString(pos.x, pos.y, "X方向,Y方向のみsin()で移動させる\nその場で左右、上下に動く敵に使用したりする", 0xffffff);
	//	pos.x += cosf(sinRate) * 128;
		pos.y += sinf(sinRate) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280 * 4, 280 + 128 };
		pos.x += cosf(sinRate) * 128;
	//	pos.y += sinf(sinRate) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);
		

		pos = Vec2{ 196, 280*2 + 128 };
		DrawString(pos.x, pos.y, "X方向,Y方向の半径を変えることで楕円を表現", 0xffffff);
		pos.x += cosf(sinRate) * 128;
		pos.y += sinf(sinRate) * 64;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280*1, 280 * 2 + 128 };
		pos.x += cosf(sinRate) * 64;
		pos.y += sinf(sinRate) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280 * 2, 280 * 2 + 128 };
		DrawString(pos.x, pos.y, "Y方向に関して絶対値を\nとるようにして半円を表現", 0xffffff);
		pos.x += cosf(sinRate) * 128;
		pos.y -= abs(sinf(sinRate) * 128);
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);
		

		for (int i = 0; i < 18; i++)
		{
			float radius = sinf(sinRate) * 128;
			float angle = (360.0f / 18.0f * i) / 360.0f * DX_TWO_PI_F;

			pos = Vec2{ 196 + 280 * 3 + 32, 280 * 2 + 128 };
			pos.x += cosf(angle) * radius;
			pos.y -= sinf(angle) * radius;
			DrawCircle(pos.x, pos.y, 4, 0xff0000, true);
		}

		for (int i = 0; i < 18; i++)
		{
			float radius = sinf(sinRate) * 128;
			float angle = (360.0f / 18.0f * i) / 360.0f * DX_TWO_PI_F + sinRate;

			pos = Vec2{ 196 + 280 * 4 + 32, 280 * 2 + 128 };
			pos.x += cosf(angle) * radius;
			pos.y -= sinf(angle) * radius;
			DrawCircle(pos.x, pos.y, 4, 0xff0000, true);
		}

		SetDrawScreen(DX_SCREEN_BACK);
		DrawGraph(0, 0, handle, true);

		DrawString(32, Game::kScreenHeight-48, "※前のフレームの表示内容を完全に消さずに残しておくことで\n画面全体に対するブラーを表現する", 0xffffff);

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