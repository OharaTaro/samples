#include "DxLib.h"

#include "game.h"
#include <string>

namespace
{
	// �^�C�g���ƃT���v���̐���
	const char* const kTitkeMessage = "�������\������T���v��";
	const char* const kPracticeMessage = "������̓p�b�ƑS�̂�\������̂ł͂Ȃ�1�������\�����邱�Ƃ�����";

	// \0�̈ʒu�����炵�ĕ�����\������
	const char* const kBasicDispMessage0 = "�I�[�ʒu�����炵�Ĉꕶ�����\������̂���{";
	const char* const kBasicDispMessage1 = "���ׂĂ��Q�o�C�g�����Ȃ�I�[���Q�o�C�g�����点�΂�����";
	const char* const kBasicDispMessage2 = "������̒���1�o�C�g�̕���������ƈȍ~�����(�o�O��)";
	const char* const kBasicDispMessage3 = "���̕��������o�C�g�̕�������check���Ă��炷�ʂ�ς��Ȃ��Ƃ����Ȃ�";

	// ��x�O���t�B�b�N�f�[�^�ɂ��ėl�X�Ȍ��ʂ�������
	const char* const kGraphicDispMessage = "�{��Ƃ͂���邪��x�O���t�B�b�N�ɂ��ĐF�X�Ȍ��ʂ�������Ƃ����l����������";
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

	

	int frameCount = 12;
	// ���o�C�g��������\�����邩�w�肷��
	int dispNum0 = 0;
	int dispNum1 = 0;
	int dispNum2 = 0;
	// �������\�����邩���w�肷��
	int dispCharCount0 = 0;

	int messageHeight = 16;
	int messageWidth = GetDrawStringWidth(kGraphicDispMessage, strlen(kGraphicDispMessage));
	int messageDispWidth = 0;

	// ������O���t�B�b�N�𐶐�
	int handle = MakeScreen(messageWidth, messageHeight, true);

	SetDrawScreen(handle);
	DrawString(0, 0, kGraphicDispMessage, 0xffffff);

	// �t�H���g
	int fontHandle = CreateFontToHandle("Showcard Gothic", 64, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	int menuItemHandle = CreateFontToHandle("���C���I", 24, 8, DX_FONTTYPE_ANTIALIASING_8X8);

	float sinRate = 0.0f;

	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
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

			dispCharCount0++;	// ������Â炭�Ȃ���̂�0�ɖ߂��͕̂\��������
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

		// dispCharCount0	// �\�����镶����(�o�C�g���ł͂Ȃ�)
		int byteCount = 0;	// �\������o�C�g���ɕϊ�����
		temp = "���̕��������o�C�g����1��������check���ĕ�����̐擪���牽byte�\�����邩�w�肵�Ȃ��Ƃ����Ȃ�";
		for (int i = 0; i < dispCharCount0; i++)
		{
			if (byteCount > temp.size())
			{
				dispCharCount0 = 0;
				byteCount = 0;
				break;
			}
			unsigned char charData = temp[byteCount];	// �`�F�b�N���镶��
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

		byteCount = 0;	// �\������o�C�g���ɕϊ�����
		temp = "DrawString()�ŕ�������܂Ƃ߂ēn���̂ł͂Ȃ�1�������\�������邱�Ƃł���ȕ\����";
		int charX = 32;
		int charY = 96 + 64;
		while (1)
		{
			if (byteCount > temp.size())	break;
			unsigned char charData = temp[byteCount];	// �`�F�b�N���镶��
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

		// �O���t�B�b�N�ɕϊ����Ă���\��
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
		
		DrawString(32, 480, "���O���t�B�b�N�ł͂Ȃ��t�H���g�݂̂ŕ\��", 0xffffff);
		DrawString(32, 480+16, "�^�C�g����P���ɕ\������̂ł͂Ȃ��������炵�ďd�˂�", 0xffffff);
		DrawString(32, 480+32, "CreateFontToHandle()�̈������F�X�����Ă݂悤", 0xffffff);


		byteCount = 0;	// �\������o�C�g���ɕϊ�����
		temp = "GAMEOVER!";
	//	temp = "CLEAR!";
		charX = 500;
		charY = 400;
		float tempSin = sinRate;
		while (1)
		{
			if (byteCount > temp.size())	break;
			unsigned char charData = temp[byteCount];	// �`�F�b�N���镶��
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
		DrawString(500, 480+16, "1�������\�� �㉺�ړ��͖��x���Ȃ���sin()", 0xffffff);

		temp = "�������傩��";
		charX = 64;
		charY = 640;
		byteCount = 0;
		while (1)
		{
			if (byteCount > temp.size())	break;
			unsigned char charData = temp[byteCount];	// �`�F�b�N���镶��
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
		
		int cursorHeight = sinf(tempSin) * 12.0f;	// ���ۂɂ�Height/2
		DrawTriangle(56, 640 + 12, 
					46, 640 + 12 - cursorHeight,
					46, 640 + 12 + cursorHeight, 0xff0000, true);
		DrawTriangle(56, 640 + 12,
					46, 640 + 12 - cursorHeight,
					46, 640 + 12 + cursorHeight, 0xffffff, false);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawStringToHandle(64, 640 + 32, "�Â�����", 0x808080, menuItemHandle);
		DrawStringToHandle(64, 640 + 64, "�I�v�V����", 0x808080, menuItemHandle);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawString(32, 760, "�J�[�\����\�����邾���łȂ��A\n�F�⓮���łǂꂪ�I������Ă��邩�킩��悤��", 0xffffff);

		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	// ������O���t�B�b�N�폜
	DeleteGraph(handle);

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}