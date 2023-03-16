#include "DxLib.h"

#include "game.h"

namespace
{
	constexpr int kStretchWindowWidth = 640;
	constexpr int kStretchWindowHeight = 180;
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

		// ��ʂ̃N���A
		ClearDrawScreen();

		DrawString(16, 16, "UI�p�̑f�ނ��g�p���ĉσT�C�Y�̃E�C���h�E��\������T���v��", 0xffffff);

		DrawGraph(64,64, window, true);
		DrawString(64, 64 + 96, "��96x96�̑f�ނ��g�p���ăE�C���h�E��\������", 0xffffff);

		DrawExtendGraph(64, 220, 64 + width, 220 + height, window, true);
		DrawString(64, 220 + kStretchWindowHeight, "���_���ȗ�@�P���Ɋg�傷��@�p�̕����̊ۂ݂�A�X�y�N�g�䂪�E�C���h�E�T�C�Y�ɂ���ĕω����Ă��܂�", 0xffffff);

		DrawGraph(64, 450, window, true);
		DrawLine(64, 450 + 32, 64 + 96, 450 + 32,0xf0f0f0);
		DrawLine(64, 450 + 64, 64 + 96, 450 + 64, 0xf0f0f0);
		DrawLine(64 + 32, 450, 64 + 32, 450 + 96, 0xf0f0f0);
		DrawLine(64 + 64, 450, 64 + 64, 450 + 96, 0xf0f0f0);
		DrawString(64, 450 + 96, "��96x96�̑f��9�ɕ������Ċp�A�e�ӁA�E�C���h�E�����Ƃ��Ďg�p����", 0xffffff);

		// 256*96�̃E�C���h�E�\��
		DrawRectGraph(64, 600,  0,  0, 32, 32, window, true);	// ����
		DrawRectGraph(64, 600 + height - 32,  0, 64, 32, 32, window, true);	// ����
		DrawRectGraph(64 + width - 32, 600, 64,  0, 32, 32, window, true);	// �E��
		DrawRectGraph(64 + width - 32, 600 + height - 32, 64, 64, 32, 32, window, true);	// �E��
		DrawString(64, 600 + kStretchWindowHeight, "1.�p�����ꂼ�ꓙ�{�ŕ\��", 0xffffff);
		if (frameCount > 120 * 1)
		{
			DrawRectExtendGraph(64 + 32, 600, 64 + width - 32, 600 + 32, 32, 0, 32, 32, window, true);	// ���
			DrawRectExtendGraph(64 + 32, 600 + height - 32, 64 + width - 32, 600 + height - 32 + 32, 32, 64, 32, 32, window, true);	// ����
			DrawRectExtendGraph(64, 600 + 32, 64 + 32, 600 + height - 32, 0, 32, 32, 32, window, true);// ����
			DrawRectExtendGraph(64 + width - 32, 600 + 32, 64 + width, 600 + height - 32, 64, 32, 32, 32, window, true);// �E��
			DrawString(64, 600 + kStretchWindowHeight + 16, "2.�e�ӂ���ׂĕ\���@�L�΂��Ė��Ȃ������Ȃ�L�΂��ĕ\��", 0xffffff);
		}
		
		if (frameCount > 120 * 2)
		{
			DrawRectExtendGraph(64 + 32, 600 + 32, 64 + width - 32, 600 + height - 32, 32, 32, 32, 32, window, true);	// �E�C���h�E����
			DrawString(64, 600 + kStretchWindowHeight + 32, "3.�E�C���h�E�̒��g������ׂĕ\���@�L�΂��Ė��Ȃ������Ȃ�L�΂��ĕ\��", 0xffffff);
		}
		
		DrawRectRotaGraph(1280, 120, (frameCount/8) % 2 * 32, 0, 32, 32, 1.0, 0, player, true);
		DrawString(1200, 120 + 32, "���摜", 0xffffff);

		DrawRectRotaGraph(1280, 240, (frameCount / 8) % 2 * 32, 0, 32, 32, 2.0, 0, player, true);
		DrawRectRotaGraph(1400, 240, (frameCount / 8) % 2 * 32, 0, 32, 32, 4.0, 0, player, true);
		DrawString(1200, 240 + 64, "�c�������䗦�Ŋg�偨�Z", 0xffffff);

		DrawRectRotaGraph3F(1280, 400, (frameCount / 8) % 2 * 32, 0, 32, 32, 16, 16, 4.0, 1.0,0.0,player,true);
		DrawRectRotaGraph3F(1400, 400, (frameCount / 8) % 2 * 32, 0, 32, 32, 16, 16, 1.0, 5.0, 0.0, player, true);
		DrawRectRotaGraph3F(1520, 400, (frameCount / 8) % 2 * 32, 0, 32, 32, 16, 16, 2.5, 2.0, 0.0, player, true);
		DrawString(1200, 400 + 64, "�A�X�y�N�g��(�c����)�������������~", 0xffffff);
		DrawString(1200, 400 + 64 + 16, "���쌠�I�ɉ��ςƂƂ��鋰�ꂠ��", 0xffffff);
		DrawString(1200, 400 + 64 + 32, "����ȑO�Ɍ����ڂ����������A�ƋC�t���銴�o��", 0xffffff);
		DrawString(1200, 400 + 64 + 48, "�_���[�W���󂯂����Ɉ�u�ό`�A���͂���", 0xffffff);
		DrawString(1200, 400 + 64 + 64, "���������쌠�́c", 0xffffff);
		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	DeleteGraph(window);

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}