#include "DxLib.h"

#include "game.h"
#include <cmath>
#include <cassert>

namespace
{
	// �^�C�g���ƃT���v���̐���
	const char* const kTitkeMessage = "����������UI�T���v��";
	const char* const kMenuItemMessage[] =
	{
		"�������傩��",
		"�Â�����",
		"�I�v�V����",
	};
	// �����t�H���g�̃T�C�Y
	constexpr int kFontWidth = 16;
	constexpr int kFontHeight = 32;
	// �J�[�\��
	constexpr int kCursorMoveFrame = 30;
	// �E�C���h�E
	constexpr int kWindowAppearFrame = 30;
}

// leftX:�E�[���W�w��	�������ς���ĕ\���ʒu������Ȃ��悤�ɂ��邽�߉E�[
// digit:�����̎w��	�w�肳�ꂽ���������������ꍇ��0����,�傫���ꍇ�͉��̌�����\���ł��镪�\��
void dispGraphNum(int leftX, int y, int dispNum, int handle, int digit = -1)
{
	int digitNum = 0;
	int temp = dispNum;
	int cutNum = 10;	// �\�������w�莞�ɕ\���������߂邽�߂Ɏg�p����

	// �\�����鐔���̌���������
	while (temp > 0)
	{
		digitNum++;
		temp /= 10;
		cutNum *= 10;
	}
	if (digitNum <= 0)
	{
		digitNum = 1;	// 0�̏ꍇ��1���\������
	}
	
	// �\�������w��
	temp = dispNum;
	if (digit >= 0)
	{
		if (digitNum > digit)
		{
			// ������w�茅����\�����邽�߂͂ݏo���͈͂�؂�̂�
			temp %= cutNum;
		}
		digitNum = digit;
	}
	// ��ԉ��̌�����\��
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

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// window���[�h�ݒ�
	ChangeWindowMode(Game::kWindowMode);
	// �E�C���h�E���ݒ�
	SetMainWindowText(Game::kTitleText);
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	SetAlwaysRunFlag(true);

	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	int numFont = LoadGraph("numfont.png");

	// �Œ萔�l�\��
	int dispNum = 2023;
	// �J�[�\���ړ�
	int cursorIndex = 0;
	int cursorNext = 0;
	int cursorMoveFrame = 0;

	int frameCount = 0;
	float sinRate = 0.0f;

	int menuItemHandle = CreateFontToHandle("���C���I", 24, 8, DX_FONTTYPE_ANTIALIASING_8X8);

	// �E�C���h�E�\�����o�p�E�C���h�E
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

	DrawStringToHandle(32, 16 + 30 * 0, "����", 0xff4040, menuItemHandle);
	DrawStringToHandle(32, 16 + 30 * 1, "����", 0x808080, menuItemHandle);
	DrawStringToHandle(32, 16 + 30 * 2, "��������", 0x808080, menuItemHandle);

	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
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

		DrawString(32, 40, "�X�R�A���̏d�v�Ȑ��l��DrawStirng()�n�֐��łȂ��O���t�B�b�N�t�H���g���g�p���ĕ\������", 0xffffff);
		dispGraphNum(128, 64, dispNum, numFont);
		dispGraphNum(320, 64, dispNum, numFont, 6);

		DrawGraph(500, 64, numFont, true);
		DrawString(720, 64, "���g�����O���t�B�b�N", 0xffffff);

		DrawString(32, 120, "������ω������鎞�̓J�E���g�A�b�v�A�_�E����������", 0xffffff);
		{
			int tempFrame = frameCount % 400;
			if (tempFrame < 100)
			{
				dispGraphNum(128, 144, dispNum, numFont);
			}
			else if (tempFrame < 200)
			{
				// �J�E���g�A�b�v
				dispGraphNum(128, 144, dispNum + tempFrame - 100, numFont);
			}
			else if (tempFrame < 300)
			{
				dispGraphNum(128, 144, dispNum + 100, numFont);
			}
			else if (tempFrame < 400)
			{
				// �J�E���g�_�E��
				dispGraphNum(128, 144, dispNum + 100 - (tempFrame - 300), numFont);
			}
		}

		DrawString(32, 200, "�����������Č������ς���Ă��킩��₷���悤��ԉ��̌��̈ʒu���Œ肷��", 0xffffff);
		{
			int tempFrame = frameCount % 300;
			if (tempFrame < 100)
			{
				dispGraphNum(128, 224, 0, numFont);
			}
			else if (tempFrame < 200)
			{
				// �J�E���g�A�b�v
				dispGraphNum(128, 224, (tempFrame - 100), numFont);
			}
			else if (tempFrame < 300)
			{
				dispGraphNum(128, 224, 100, numFont);
			}
		}

		DrawString(32, 300, "�J�[�\���̈ړ��̓p�b�ƈړ�������̂ł͂Ȃ����t���[�������čs�� �ω����킩��悤�ɂ������ړ� ���ۂɂ͕s���ɂȂ�Ȃ����x�̑�����", 0xffffff);
	
		for (int i = 0; i < _countof(kMenuItemMessage); i++)
		{
			int color = 0x808080;
			int posY = 330 + 32 * i;
			if (cursorNext == i)
			{
				color = 0xff4040;
				int cursorHeight = sinf(sinRate) * 12.0f;	// ���ۂɂ�Height/2
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
				// cursorMoveFrame kCursorMoveFrame->0�ƕω�����
				float rate = static_cast<float>(cursorMoveFrame) / static_cast<float>(kCursorMoveFrame);	// 1.0->0.0
				if (cursorIndex == i)
				{
					// ���񂾂�Â��Ȃ�
					r = 0xff * rate + 0x80 * (1.0f - rate);
					g = 0x40 * rate + 0x80 * (1.0f - rate);
					b = 0x40 * rate + 0x80 * (1.0f - rate);
				}
				else if (cursorNext == i)
				{
					// ���񂾂񖾂邭�Ȃ�
					r = 0xff * (1.0f - rate) + 0x80 * rate;
					g = 0x40 * (1.0f - rate) + 0x80 * rate;
					b = 0x40 * (1.0f - rate) + 0x80 * rate;
				}
				cursorY = (330 + 32 * cursorNext) * (1.0f - rate) + (330 + 32 * cursorIndex) * rate;
			}
			DrawStringToHandle(256, posY, kMenuItemMessage[i], GetColor(r,g,b), menuItemHandle);
		}
		{
			int cursorHeight = sinf(sinRate) * 12.0f;	// ���ۂɂ�Height/2
			DrawTriangle(248, cursorY + 12,
				238, cursorY + 12 - cursorHeight,
				238, cursorY + 12 + cursorHeight, 0xff0000, true);
			DrawTriangle(248, cursorY + 12,
				238, cursorY + 12 - cursorHeight,
				238, cursorY + 12 + cursorHeight, 0xffffff, false);
		}

		DrawString(32, 480, "�E�C���h�E���p�b�ƕ\������̂ł͂Ȃ��E�C���h�E�̓o�ꉉ�o������\n�E�C���h�E�̘g���O���t�B�b�N���g������ADrawBox()���g���ɂ��Ă��F��ς��ăG���{�X��\������", 0xffffff);
		{
			int tempFrame = frameCount % 480;
			int dispFrame = 0;
			if (tempFrame >= 360)
			{
				// ������
				dispFrame = tempFrame - 360;
				dispFrame = 120 - dispFrame;
			}
			else if (tempFrame >= 120)
			{
				dispFrame = tempFrame - 120;
			}
			// else�͔�\��

			if (dispFrame > 0)
			{
				
				// �X���C�h�C��
				{
					float rate = static_cast<float>(dispFrame) / 120.0f;
					rate *= 12.0f;
					if (rate >= 1.0f)	rate = 1.0f;
					DrawGraph(32 - 240 + 240 * rate, 520, menuWindowHandle, true);
				}
				// �g�k+�A���t�@
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
		
		
		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	DeleteFontToHandle(menuItemHandle);
	DeleteGraph(menuWindowHandle);
	DeleteGraph(numFont);

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}