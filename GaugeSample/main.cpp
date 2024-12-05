#include "DxLib.h"
#include "game.h"
#include <cmath>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// window���[�h�ݒ�
	ChangeWindowMode(Game::kWindowMode);
	// �E�C���h�E���ݒ�
	SetMainWindowText(Game::kTitleText);
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);
	// �o�b�N�O���E���h���s��ON��
//	SetAlwaysRunFlag(true);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	int handle = LoadGraph("gaugeImage.bmp");
	float sinRate = 0.0f;

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
		ClearDrawScreen();

		sinRate += 0.01f;
		float rate = sin(sinRate)*0.5f + 0.5f;

		DrawFormatString(64, 32, 0xffffff, "HP RATE : %f%%", rate*100);

		DrawString(64, 64, "DrawGraph �摜�����̂܂ܕ\��", 0xffffff);
		DrawGraph(64, 64 + 16, handle, true);

		DrawString(64, 128, "DrawBox �c��HP�̊����ɉ�����Box�̕\���T�C�Y��ύX����HP�o�[��", 0xffffff);
		DrawBox(64, 128 + 16, 64 + 256 * rate, 128 + 16 + 32, 0xff0000, true);

		DrawString(64, 192, "DrawQuadrangle �l�p�`�̊e���_�̍��W���w�肵�ĕ\��", 0xffffff);
		DrawQuadrangle(64       , 192 + 16,
			64 + 256 * rate     , 192 + 16,
			64 + 256 * rate + 16, 192 + 16 + 32,
			64 + 16             , 192 + 16 + 32,
			0xff0000, true);

		DrawString(64, 256, "DrawRectGraph HP�o�[�Ɏg�p����O���t�B�b�N�̐؂���T�C�Y��ς��邱�Ƃ�HP�̗ʂ�\��", 0xffffff);
		DrawRectGraph(64, 256+16,
			0, 0, 256 * rate, 32,
			handle, true);

		DrawString(64, 320, "DrawModiGraph �O���t�B�b�N��4�̒��_���W���w�肵�ĕ\�����邱�Ƃ��ł���", 0xffffff);
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

		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}


	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}