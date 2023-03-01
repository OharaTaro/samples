#include "DxLib.h"

#include "game.h"
#include <string>

namespace
{
	// タイトルとサンプルの説明
	const char* const kTitkeMessage = "文字列を表示するサンプル";
	const char* const kPracticeMessage = "文字列はパッと全体を表示するのではなく1文字ずつ表示することが多い";

	// \0の位置をずらして文字を表示する
	const char* const kBasicDispMessage0 = "終端位置をずらして一文字ずつ表示するのが基本";
	const char* const kBasicDispMessage1 = "すべてが２バイト文字なら終端を２バイトずつずらせばいいが";
	const char* const kBasicDispMessage2 = "文字列の中に1バイトの文字があると以降ずれる(バグる)";
	const char* const kBasicDispMessage3 = "次の文字が何バイトの文字かをcheckしてずらす量を変えないといけない";

	// 一度グラフィックデータにして様々な効果をかける
	const char* const kGraphicDispMessage = "本題とはずれるが一度グラフィックにして色々な効果をかけるという考え方もあり";
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

	

	int frameCount = 12;
	// 何バイト文文字を表示するか指定する
	int dispNum0 = 0;
	int dispNum1 = 0;
	int dispNum2 = 0;
	// 何文字表示するかを指定する
	int dispCharCount0 = 0;

	int messageHeight = 16;
	int messageWidth = GetDrawStringWidth(kGraphicDispMessage, strlen(kGraphicDispMessage));
	int messageDispWidth = 0;

	// 文字列グラフィックを生成
	int handle = MakeScreen(messageWidth, messageHeight, true);

	SetDrawScreen(handle);
	DrawString(0, 0, kGraphicDispMessage, 0xffffff);

	// フォント
	int fontHandle = CreateFontToHandle("Showcard Gothic", 64, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	int menuItemHandle = CreateFontToHandle("メイリオ", 24, 8, DX_FONTTYPE_ANTIALIASING_8X8);

	float sinRate = 0.0f;

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		// update
		sinRate += 0.05f;
		frameCount--;
		if (frameCount <= 0)
		{
			frameCount = 12;

			dispNum0 += 2;
			if (strlen(kBasicDispMessage0) < dispNum0)
			{
				dispNum0 = 0;
			}

			dispNum1 += 2;
			if (strlen(kBasicDispMessage1) < dispNum1)
			{
				dispNum1 = 0;
			}
			dispNum2 += 2;
			if (strlen(kBasicDispMessage2) < dispNum2)
			{
				dispNum2 = 0;
			}

			dispCharCount0++;	// 分かりづらくなあるので0に戻すのは表示部分で
		}
		messageDispWidth++;
		if (messageDispWidth > messageWidth)
		{
			messageDispWidth = 0;
		}
		// draw
		DrawString(16, 8, kTitkeMessage, 0xffffff);

		DrawString(32, 32, kPracticeMessage, 0xffffff);

		std::string temp = kBasicDispMessage0;
		DrawFormatString(0, 96, 0xffffff, "%d",dispNum0);
		DrawString(32, 96, temp.substr(0, dispNum0).c_str(), 0xffffff);
		
		temp = kBasicDispMessage1;
		DrawFormatString(0, 96+16, 0xffffff, "%d", dispNum1);
		DrawString(32, 96+16, temp.substr(0, dispNum1).c_str(), 0xffffff);

		temp = kBasicDispMessage2;
		DrawFormatString(0, 96 + 32, 0xffffff, "%d", dispNum2);
		DrawString(32, 96 + 32, temp.substr(0, dispNum2).c_str(), 0xffffff);

		// dispCharCount0	// 表示する文字数(バイト数ではない)
		int byteCount = 0;	// 表示するバイト数に変換する
		temp = "その文字が何バイトかを1文字ずつcheckして文字列の先頭から何byte表示するか指定しないといけない";
		for (int i = 0; i < dispCharCount0; i++)
		{
			if (byteCount > temp.size())
			{
				dispCharCount0 = 0;
				byteCount = 0;
				break;
			}
			unsigned char charData = temp[byteCount];	// チェックする文字
			if (charData < 0x80)
			{
				byteCount += 1;
			}
			else
			{
				byteCount += 2;
			}
		}
		DrawFormatString(0, 96 + 48, 0xffffff, "%d", byteCount);
		DrawString(32, 96 + 48, temp.substr(0, byteCount).c_str(), 0xffffff);

		byteCount = 0;	// 表示するバイト数に変換する
		temp = "DrawString()で文字列をまとめて渡すのではなく1文字ずつ表示させることでこんな表現も";
		int charX = 32;
		int charY = 96 + 64;
		while (1)
		{
			if (byteCount > temp.size())	break;
			unsigned char charData = temp[byteCount];	// チェックする文字
			int size = 0;
			if (charData < 0x80)
			{
				size = 1;
			}
			else
			{
				size = 2;
			}
			int shakeX = GetRand(4) - 2;
			int shakeY = GetRand(4) - 2;

			DrawString(charX+ shakeX, charY+ shakeY, temp.substr(byteCount, size).c_str(), 0xffffff);
		//	DrawString(charX + shakeX, charX + shakeY, temp.substr(byteCount, size).c_str(), 0xffffff);

			charX += GetDrawStringWidth(temp.substr(byteCount, size).c_str(), size);

			byteCount += size;
		}

		// グラフィックに変換してから表示
		DrawRectGraph(32, 200, 0, 0, messageDispWidth, messageHeight,handle, true);

		DrawRectGraph(32, 200 + 32, 0, 0, messageDispWidth, messageHeight, handle, true);
		for (int i = 0; i < 16; i++)
		{
			int x = 32 + messageDispWidth + i * 2;
		//	int y = 200 + 32 + i * 2;
			int y = 200 + 32 + GetRand(i*2) - i*1;
			DrawRectGraph(x, y, 
				messageDispWidth + i * 2, 0, 
				2, messageHeight, handle, true);
		}

		DrawRectGraph(32, 200 + 64, 0, 0, messageDispWidth, messageHeight, handle, true);
		for (int i = 0; i < 8; i++)
		{
			int x = 32 + messageDispWidth + i * 2;
			int y = 200 + 64 + i * 2;
			DrawRectGraph(x, y,
				messageDispWidth + i * 2, 0,
				2, messageHeight, handle, true);
		}

		
		{
			int moveY = sinf(sinRate*2) * 4;

			DrawStringToHandle(32, 400 - 64 + moveY, "GAME TITLE", 0xff0000, fontHandle);

			DrawStringToHandle(32 + 4, 400 + 4 + moveY, "GAME TITLE", 0xffffff, fontHandle);
			//	DrawStringToHandle(32 + 2, 400 + 2, "GAME TITLE", 0xff8080, fontHandle);
			DrawStringToHandle(32, 400 + moveY, "GAME TITLE", 0xff0000, fontHandle);
		}
		
		DrawString(32, 480, "↑グラフィックではなくフォントのみで表示", 0xffffff);
		DrawString(32, 480+16, "タイトルを単純に表示するのではなく少しずらして重ねる", 0xffffff);
		DrawString(32, 480+32, "CreateFontToHandle()の引数も色々試してみよう", 0xffffff);


		byteCount = 0;	// 表示するバイト数に変換する
		temp = "GAMEOVER!";
	//	temp = "CLEAR!";
		charX = 500;
		charY = 400;
		float tempSin = sinRate;
		while (1)
		{
			if (byteCount > temp.size())	break;
			unsigned char charData = temp[byteCount];	// チェックする文字
			int size = 0;
			if (charData < 0x80)
			{
				size = 1;
			}
			else
			{
				size = 2;
			}
			int moveX = 0;
			int moveY = sinf(tempSin) * 16.0f;
			tempSin += DX_PI_F;

			DrawStringToHandle(charX + moveX+2, charY + moveY+2, temp.substr(byteCount, size).c_str(), 0xffffff, fontHandle);
			DrawStringToHandle(charX + moveX, charY + moveY, temp.substr(byteCount, size).c_str(), 0xff0000, fontHandle);
			charX += GetDrawStringWidthToHandle(temp.substr(byteCount, size).c_str(), size, fontHandle);
			byteCount += size;
		}
		DrawString(500, 480+16, "1文字ずつ表示 上下移動は毎度おなじみsin()", 0xffffff);

		temp = "さいしょから";
		charX = 64;
		charY = 640;
		byteCount = 0;
		while (1)
		{
			if (byteCount > temp.size())	break;
			unsigned char charData = temp[byteCount];	// チェックする文字
			int size = 0;
			if (charData < 0x80)
			{
				size = 1;
			}
			else
			{
				size = 2;
			}
			int moveX = 0;
			int moveY = sinf(tempSin*4) * 4.0f;
			tempSin += DX_PI_F / 16.0f;

		//	DrawStringToHandle(charX + moveX + 2, charY + moveY + 2, temp.substr(byteCount, size).c_str(), 0xffffff, menuItemHandle);
			DrawStringToHandle(charX + moveX, charY + moveY, temp.substr(byteCount, size).c_str(), 0xff0000, menuItemHandle);
			charX += GetDrawStringWidthToHandle(temp.substr(byteCount, size).c_str(), size, menuItemHandle);
			byteCount += size;
		}
		
		int cursorHeight = sinf(tempSin) * 12.0f;	// 実際にはHeight/2
		DrawTriangle(56, 640 + 12, 
					46, 640 + 12 - cursorHeight,
					46, 640 + 12 + cursorHeight, 0xff0000, true);
		DrawTriangle(56, 640 + 12,
					46, 640 + 12 - cursorHeight,
					46, 640 + 12 + cursorHeight, 0xffffff, false);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawStringToHandle(64, 640 + 32, "つづきから", 0x808080, menuItemHandle);
		DrawStringToHandle(64, 640 + 64, "オプション", 0x808080, menuItemHandle);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawString(32, 760, "カーソルを表示するだけでなく、\n色や動きでどれが選択されているかわかるように", 0xffffff);

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	// 文字列グラフィック削除
	DeleteGraph(handle);

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}