#include "DxLib.h"

#include "game.h"
#include "Vec2.h"

namespace
{
	constexpr int kParticleNum = 256;
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

	float sinRate = 0.0f;

	int gameScreen = LoadGraph("clear.png");
	int effectScreen = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);

	Vec2 pos[kParticleNum];
	Vec2 vec[kParticleNum];
	for (int i = 0; i < kParticleNum; i++)
	{
		pos[i].x = GetRand(Game::kScreenWidth);
		pos[i].y = -GetRand(Game::kScreenHeight);

		vec[i].x = 0.0f;
		vec[i].y = GetRand(20)+20;
		vec[i].y /= 10.0f;
	}

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// update
		for (int i = 0; i < kParticleNum; i++)
		{
			pos[i] += vec[i];
			if (pos[i].y > Game::kScreenHeight)
			{
				pos[i].x = GetRand(Game::kScreenWidth);
				pos[i].y = 0.0f;

				vec[i].x = 0.0f;
				vec[i].y = GetRand(20) + 20;
				vec[i].y /= 10.0f;
			}
		}
		sinRate += 0.05f;
		
		// draw
		SetDrawScreen(effectScreen);
		ClearDrawScreen();
		BeginAADraw();
		for (int i = 0; i < kParticleNum; i++)
		{
			DrawCircleAA(pos[i].x, pos[i].y, 4 * (vec[i].length() / 4.0f), 32, 0x808020, true);
		}
		EndAADraw();
		SetDrawScreen(DX_SCREEN_BACK);
		// ��ʂ̃N���A
		ClearDrawScreen();

		DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, gameScreen, false);

		
		//���Z��������
		SetDrawBlendMode(DX_BLENDMODE_ADD, 192);

		DrawGraph(0, 0, effectScreen, true);
		//��ʂ��ڂ���
		GraphFilter(effectScreen, DX_GRAPH_FILTER_GAUSS, 16, 800);
		for (int i = 0; i < 8; i++)
		{
			DrawGraph(GetRand(4)-2, GetRand(4) - 2, effectScreen, false);
		}

		GraphFilter(effectScreen, DX_GRAPH_FILTER_GAUSS, 32, 2400);
		for (int i = 0; i < 8; i++)
		{
			DrawGraph(GetRand(8) - 4, GetRand(8) - 4, effectScreen, false);
		}
		//���ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawFormatString(32, 32, 0xffffff, "FPS = %f",GetFPS());
		DrawFormatString(32, 64, 0xffffff, "DC = %d", GetDrawCallCount());

		DrawFormatString(32, 720 + sinf(sinRate) * 16, 0xffffff, "��ʂ����点�鏈�������p���Č���p�[�e�B�N�� �Q�[����ʂ͌����ĂȂ�");
		DrawFormatString(32, 736 + sinf(sinRate) * 16, 0xffffff, "��ʃT�C�Y�̕`����J��Ԃ��Ă���̂ŏ����͏d��(�͂�)");
		DrawFormatString(32, 780 + sinf(sinRate) * 16, 0xffffff, "�v���C���[�ɒ��ڂ��Ă��炢���������͓�����(���̕��͂̂���)");

		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	DeleteGraph(gameScreen);
	DeleteGraph(effectScreen);

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}