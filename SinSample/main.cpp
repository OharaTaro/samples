#include "DxLib.h"

#include "game.h"
#include "Vec2.h"

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

	int handle = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);

	float sinRate = 0.0f;
	Vec2 basePos{128, 128+32};

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
		ClearDrawScreen();

		// update
		float randSinRate = static_cast<float>(GetRand(360)) / 360.0f * DX_TWO_PI_F;
		sinRate += 0.05f;	// �������ɕω�����p�x
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

		DrawString(16, 8, "sin(),cos()���g�p���Ă��������ɓ�����", 0xffffff);
		DrawFormatString(16, 24, 0xffffff, "DrawCall: %d", GetDrawCallCount());

		// �V���v���ȃT�C���J�[�u
		Vec2 pos = basePos;
		pos.y += sinf(sinRate) * 100;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);
		DrawString(128, 128, "�V���v���ȃT�C���J�[�u", 0xffffff);

		pos = Vec2{196, 280+128};
		DrawString(pos.x, pos.y, "cos(),sin()��\n�~�̋O���𐶐�����", 0xffffff);
		pos.x += cosf(sinRate) * 128;
		pos.y += sinf(sinRate) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280 * 1, 280 + 128 };
		DrawString(pos.x, pos.y, "XY�̎�����ύX���邱�Ƃ�\n�O����ω�������", 0xffffff);
		pos.x += cosf(sinRate) * 128;
		pos.y += sinf(sinRate*2) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280 * 2, 280 + 128 };
	//	DrawString(pos.x, pos.y, "������ύX���邱�Ƃ�\n�O����ω�������", 0xffffff);
		pos.x += cosf(sinRate*4) * 128;
		pos.y += sinf(sinRate) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280 * 3, 280 + 128 };
		DrawString(pos.x, pos.y, "X����,Y�����̂�sin()�ňړ�������\n���̏�ō��E�A�㉺�ɓ����G�Ɏg�p�����肷��", 0xffffff);
	//	pos.x += cosf(sinRate) * 128;
		pos.y += sinf(sinRate) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280 * 4, 280 + 128 };
		pos.x += cosf(sinRate) * 128;
	//	pos.y += sinf(sinRate) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);
		

		pos = Vec2{ 196, 280*2 + 128 };
		DrawString(pos.x, pos.y, "X����,Y�����̔��a��ς��邱�Ƃőȉ~��\��", 0xffffff);
		pos.x += cosf(sinRate) * 128;
		pos.y += sinf(sinRate) * 64;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280*1, 280 * 2 + 128 };
		pos.x += cosf(sinRate) * 64;
		pos.y += sinf(sinRate) * 128;
		DrawCircle(pos.x, pos.y, 4, 0xff0000, true);

		pos = Vec2{ 196 + 280 * 2, 280 * 2 + 128 };
		DrawString(pos.x, pos.y, "Y�����Ɋւ��Đ�Βl��\n�Ƃ�悤�ɂ��Ĕ��~��\��", 0xffffff);
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

		DrawString(32, Game::kScreenHeight-48, "���O�̃t���[���̕\�����e�����S�ɏ������Ɏc���Ă������Ƃ�\n��ʑS�̂ɑ΂���u���[��\������", 0xffffff);

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