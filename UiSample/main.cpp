#include "DxLib.h"

#include "game.h"
#include <cmath>
#include <cassert>

namespace
{
	// タイトルとサンプルの説明
	const char* const kTitkeMessage = "いい感じのUIサンプル";
	const char* const kMenuItemMessage[] =
	{
		"さいしょから",
		"つづきから",
		"オプション",
	};
	// 数字フォントのサイズ
	constexpr int kFontWidth = 16;
	constexpr int kFontHeight = 32;
	// カーソル
	constexpr int kCursorMoveFrame = 30;
	// ウインドウ
	constexpr int kWindowAppearFrame = 30;
}

// leftX:右端座標指定	桁数が変わって表示位置がずれないようにするため右端
// digit:桁数の指定	指定された桁数よりも小さい場合は0埋め,大きい場合は下の桁から表示できる分表示
void dispGraphNum(int leftX, int y, int dispNum, int handle, int digit = -1)
{
	int digitNum = 0;
	int temp = dispNum;
	int cutNum = 10;	// 表示桁数指定時に表示をおさめるために使用する

	// 表示する数字の桁数数える
	while (temp > 0)
	{
		digitNum++;
		temp /= 10;
		cutNum *= 10;
	}
	if (digitNum <= 0)
	{
		digitNum = 1;	// 0の場合は1桁表示する
	}
	
	// 表示桁数指定
	temp = dispNum;
	if (digit >= 0)
	{
		if (digitNum > digit)
		{
			// 下から指定桁数を表示するためはみ出し範囲を切り捨て
			temp %= cutNum;
		}
		digitNum = digit;
	}
	// 一番下の桁から表示
	int posX = leftX - kFontWidth;
	int posY = y;
	for (int i = 0; i < digitNum; i++)
	{
		int no = temp % 10;

		DrawRectGraph(posX, posY,
			no * 16, 0, 16, 32,
			handle, true);

		temp /= 10;
		posX -= 16;
	}

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

	int numFont = LoadGraph("numfont.png");

	// 固定数値表示
	int dispNum = 2023;
	// カーソル移動
	int cursorIndex = 0;
	int cursorNext = 0;
	int cursorMoveFrame = 0;

	int frameCount = 0;
	float sinRate = 0.0f;

	int menuItemHandle = CreateFontToHandle("メイリオ", 24, 8, DX_FONTTYPE_ANTIALIASING_8X8);

	// ウインドウ表示演出用ウインドウ
	int menuWindowHandle = MakeScreen(200, 116, true);
	assert(menuWindowHandle != -1);

	SetDrawScreen(menuWindowHandle);
	DrawBox(0, 0, 200, 116, 0x404040, false);
	DrawBox(1, 1, 200 - 1, 116 - 1, 0x808080, false);
	DrawBox(2, 2, 200 - 2, 116 - 2, 0xffffff, false);
	DrawBox(3, 3, 200 - 3, 116 - 3, 0xffffff, false);
	DrawBox(4, 4, 200 - 4, 116 - 4, 0x808080, false);
	DrawBox(5, 5, 200 - 5, 116 - 5, 0x404040, false);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, 200, 116, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(32, 16 + 30 * 0, "かう", 0xff4040, menuItemHandle);
	DrawStringToHandle(32, 16 + 30 * 1, "うる", 0x808080, menuItemHandle);
	DrawStringToHandle(32, 16 + 30 * 2, "たたかう", 0x808080, menuItemHandle);

	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		frameCount++;
		sinRate += 0.1f;

		if ((frameCount % 120) == 0)
		{
			cursorNext++;
			if (cursorNext > _countof(kMenuItemMessage) - 1)
			{
				cursorNext = 0;
			}
			cursorMoveFrame = kCursorMoveFrame;
		}
		if (cursorIndex != cursorNext)
		{
			cursorMoveFrame--;
			if (cursorMoveFrame <= 0)
			{
				cursorIndex = cursorNext;
				cursorMoveFrame = 0;
			}
		}

		// draw
		DrawString(16, 8, kTitkeMessage, 0xffffff);

		DrawString(32, 40, "スコア等の重要な数値はDrawStirng()系関数でなくグラフィックフォントを使用して表示する", 0xffffff);
		dispGraphNum(128, 64, dispNum, numFont);
		dispGraphNum(320, 64, dispNum, numFont, 6);

		DrawGraph(500, 64, numFont, true);
		DrawString(720, 64, "←使ったグラフィック", 0xffffff);

		DrawString(32, 120, "数字を変化させる時はカウントアップ、ダウンを見せる", 0xffffff);
		{
			int tempFrame = frameCount % 400;
			if (tempFrame < 100)
			{
				dispGraphNum(128, 144, dispNum, numFont);
			}
			else if (tempFrame < 200)
			{
				// カウントアップ
				dispGraphNum(128, 144, dispNum + tempFrame - 100, numFont);
			}
			else if (tempFrame < 300)
			{
				dispGraphNum(128, 144, dispNum + 100, numFont);
			}
			else if (tempFrame < 400)
			{
				// カウントダウン
				dispGraphNum(128, 144, dispNum + 100 - (tempFrame - 300), numFont);
			}
		}

		DrawString(32, 200, "数字が増えて桁数が変わってもわかりやすいよう一番下の桁の位置を固定する", 0xffffff);
		{
			int tempFrame = frameCount % 300;
			if (tempFrame < 100)
			{
				dispGraphNum(128, 224, 0, numFont);
			}
			else if (tempFrame < 200)
			{
				// カウントアップ
				dispGraphNum(128, 224, (tempFrame - 100), numFont);
			}
			else if (tempFrame < 300)
			{
				dispGraphNum(128, 224, 100, numFont);
			}
		}

		DrawString(32, 300, "カーソルの移動はパッと移動させるのではなく数フレームかけて行う 変化がわかるようにゆっくり移動 実際には不快にならない程度の速さに", 0xffffff);
	
		for (int i = 0; i < _countof(kMenuItemMessage); i++)
		{
			int color = 0x808080;
			int posY = 330 + 32 * i;
			if (cursorNext == i)
			{
				color = 0xff4040;
				int cursorHeight = sinf(sinRate) * 12.0f;	// 実際にはHeight/2
				DrawTriangle(56, posY + 12,
					46, posY + 12 - cursorHeight,
					46, posY + 12 + cursorHeight, 0xff0000, true);
				DrawTriangle(56, posY + 12,
					46, posY + 12 - cursorHeight,
					46, posY + 12 + cursorHeight, 0xffffff, false);
			}
			DrawStringToHandle(64, posY, kMenuItemMessage[i], color, menuItemHandle);
		}

		float cursorY = 0.0f;
		for (int i = 0; i < _countof(kMenuItemMessage); i++)
		{
			int r = 0x80;
			int g = 0x80;
			int b = 0x80;
			int posY = 330 + 32 * i;
			if (cursorIndex == cursorNext)
			{
				if (cursorIndex == i)
				{
					r = 0xff;
					g = 0x40;
					b = 0x40;
					cursorY = posY;
				}
			}
			else
			{
				// cursorMoveFrame kCursorMoveFrame->0と変化する
				float rate = static_cast<float>(cursorMoveFrame) / static_cast<float>(kCursorMoveFrame);	// 1.0->0.0
				if (cursorIndex == i)
				{
					// だんだん暗くなる
					r = 0xff * rate + 0x80 * (1.0f - rate);
					g = 0x40 * rate + 0x80 * (1.0f - rate);
					b = 0x40 * rate + 0x80 * (1.0f - rate);
				}
				else if (cursorNext == i)
				{
					// だんだん明るくなる
					r = 0xff * (1.0f - rate) + 0x80 * rate;
					g = 0x40 * (1.0f - rate) + 0x80 * rate;
					b = 0x40 * (1.0f - rate) + 0x80 * rate;
				}
				cursorY = (330 + 32 * cursorNext) * (1.0f - rate) + (330 + 32 * cursorIndex) * rate;
			}
			DrawStringToHandle(256, posY, kMenuItemMessage[i], GetColor(r,g,b), menuItemHandle);
		}
		{
			int cursorHeight = sinf(sinRate) * 12.0f;	// 実際にはHeight/2
			DrawTriangle(248, cursorY + 12,
				238, cursorY + 12 - cursorHeight,
				238, cursorY + 12 + cursorHeight, 0xff0000, true);
			DrawTriangle(248, cursorY + 12,
				238, cursorY + 12 - cursorHeight,
				238, cursorY + 12 + cursorHeight, 0xffffff, false);
		}

		DrawString(32, 480, "ウインドウをパッと表示するのではなくウインドウの登場演出を入れる\nウインドウの枠もグラフィックを使ったり、DrawBox()を使うにしても色を変えてエンボスを表現する", 0xffffff);
		{
			int tempFrame = frameCount % 480;
			int dispFrame = 0;
			if (tempFrame >= 360)
			{
				// 消える
				dispFrame = tempFrame - 360;
				dispFrame = 120 - dispFrame;
			}
			else if (tempFrame >= 120)
			{
				dispFrame = tempFrame - 120;
			}
			// elseは非表示

			if (dispFrame > 0)
			{
				
				// スライドイン
				{
					float rate = static_cast<float>(dispFrame) / 120.0f;
					rate *= 12.0f;
					if (rate >= 1.0f)	rate = 1.0f;
					DrawGraph(32 - 240 + 240 * rate, 520, menuWindowHandle, true);
				}
				// 拡縮+アルファ
				{
					float rate = static_cast<float>(dispFrame) / 120.0f;
					rate *= 12.0f;
					if (rate >= 1.0f)	rate = 1.0f;
					
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
				//	DrawGraph(32 + 240 * 1, 500, menuWindowHandle, true);
					DrawRotaGraph(32 + 240 * 1 + 100, 520 + 58,
						rate, 0.0, menuWindowHandle, true);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}

				DrawGraph(32 + 240 * 2, 520, menuWindowHandle, true);
			}
			
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

	DeleteFontToHandle(menuItemHandle);
	DeleteGraph(menuWindowHandle);
	DeleteGraph(numFont);

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}