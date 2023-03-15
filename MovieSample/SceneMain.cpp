#include "SceneMain.h"
#include "DxLib.h"
#include "game.h"
#include <cassert>
#include <cmath>

namespace
{
	const char* const kGameTitle = "GAME TITLE!";
	const char* const kKeyWaitMessage = "Aボタンでスタート";
	const char* const kDemoMessage = "Demo...";

	// タイトル画面でデモが始まるまでの時間(秒)
	constexpr int kDemoWaitSec = 10;
	// デモを終了してタイトル画面に戻るまでの時間(秒)
	constexpr int kDemoEndSec = 20;

//	constexpr int kCrossFadeSpeed = 8;
	constexpr int kCrossFadeSpeed = 4;	// わかりやすくするためゆっくりに
}

SceneMain::SceneMain():
	m_updateFunc(&SceneMain::updateNormal),
	m_wallPaper(-1),
	m_movieHandle(-1),
	m_titleFontHandle(-1),
	m_waitFontHandle(-1),
	m_wallOffset(0),
	m_waitFrame(0),
	m_demoAlpha(0)
{
	
}

SceneMain::~SceneMain()
{
	DeleteGraph(m_wallPaper);
	DeleteFontToHandle(m_titleFontHandle);
	DeleteFontToHandle(m_waitFontHandle);
	DeleteGraph(m_movieHandle);
}

void SceneMain::init()
{
	m_wallPaper = LoadGraph("pink.png");

	m_movieHandle = LoadGraph("demo.mp4");
	assert(m_movieHandle != -1);
//	PlayMovieToGraph(m_movieHandle);
//	SetMovieVolumeToGraph(0, m_movieHandle);

	m_titleFontHandle = CreateFontToHandle("Showcard Gothic", 128, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_waitFontHandle = CreateFontToHandle("メイリオ", 32, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
}

void SceneMain::update()
{
	(this->*m_updateFunc)();
}

void SceneMain::draw()
{
	for (int x = 0; x < Game::kScreenWidth / 64+1; x++)
	{
		for (int y = 0; y < Game::kScreenHeight / 64+1; y++)
		{
			DrawGraph(x*64 - m_wallOffset, y*64,m_wallPaper, false);
		}
	}

	int width = GetDrawStringWidthToHandle(kGameTitle, strlen(kGameTitle), m_titleFontHandle);
	int posX = (Game::kScreenWidth - width) / 2;
	DrawStringToHandle(posX+4, 200+4, kGameTitle, 0xffffff, m_titleFontHandle);
	DrawStringToHandle(posX, 200, kGameTitle, 0xff0000, m_titleFontHandle);

	if (m_updateFunc == &SceneMain::updateNormal)
	{
		if ((m_waitFrame / 30) % 2)
		{
			width = GetDrawStringWidthToHandle(kKeyWaitMessage, strlen(kKeyWaitMessage), m_waitFontHandle);
			DrawStringToHandle((Game::kScreenWidth - width) / 2, 480, kKeyWaitMessage, 0xff0000, m_waitFontHandle);
		}

		for (int i = 0; i < m_waitFrame; i+=2)	// 見た目きれいならいいので半分間引く
		{
			float rate = static_cast<float>(i) / (kDemoWaitSec * 60);
			float angle = rate * DX_TWO_PI_F - DX_PI_F / 2.0f;
			float posX = cosf(angle) * 64 + Game::kScreenWidth / 2;
			float posY = sinf(angle) * 64 + 640;
		//	DrawCircle(posX, posY, 8, 0x0000ff, true);
			DrawLine(Game::kScreenWidth / 2, 640,
				posX, posY, 0x0000ff, 2);
		}
		DrawCircle(Game::kScreenWidth / 2, 640, 64, 0xffffff, false,4);
	}

//	if (m_updateFunc == &SceneMain::updateDemoMovie)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_demoAlpha);

		DrawModiGraph(0, 0, 
			Game::kScreenWidth, 0,
			Game::kScreenWidth, Game::kScreenHeight, 
			0, Game::kScreenHeight,
			m_movieHandle, false);

		width = GetDrawStringWidthToHandle(kDemoMessage, strlen(kDemoMessage), m_titleFontHandle);
		if ((m_waitFrame / 30) % 2)
		{
			DrawStringToHandle(Game::kScreenWidth - width, Game::kScreenHeight - 128, kDemoMessage, 0xff0000, m_titleFontHandle);
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void SceneMain::updateNormal()
{
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (pad)	// いずれかのキーが押されていた場合
	{
		m_waitFrame = 0;
	}

	m_demoAlpha -= kCrossFadeSpeed;
	if (m_demoAlpha < 0)	m_demoAlpha = 0;

	m_wallOffset++;
	if (m_wallOffset >= 64)
	{
		m_wallOffset -= 64;
	}

	m_waitFrame++;
	if(m_waitFrame >= kDemoWaitSec * 60)
	{
		m_updateFunc = &SceneMain::updateDemoMovie;
		
		int seekTime = GetRand(5) * 20 * 1000;	// 開始位置をずらして複数デモがあるよう偽装
		SeekMovieToGraph(m_movieHandle, seekTime);
		PlayMovieToGraph(m_movieHandle);
		SetMovieVolumeToGraph(0, m_movieHandle);
		m_waitFrame = 0;
		m_demoAlpha = 0;
	}
}

void SceneMain::updateDemoMovie()
{
	m_waitFrame++;

	m_wallOffset++;
	if (m_wallOffset >= 64)
	{
		m_wallOffset -= 64;
	}

	m_demoAlpha += kCrossFadeSpeed;
	if (m_demoAlpha > 255)	m_demoAlpha = 255;

	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if( (m_waitFrame >= kDemoEndSec * 60) ||
		(pad & PAD_INPUT_1) )
	{
		m_updateFunc = &SceneMain::updateNormal;
		m_waitFrame = 0;
	}
}
