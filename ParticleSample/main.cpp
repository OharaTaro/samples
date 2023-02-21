#include "DxLib.h"

#include "game.h"
#include "Vec2.h"
#include <cmath>
#include <array>
#include <memory>

#include "ParticleBase.h"

namespace
{
	constexpr int kParticleNum = 4096;
	constexpr int kShowerInterval = 2;
	constexpr int kFlowerInterval = 120;
	constexpr int kAuraInterval = 2;
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

//	ParticleBase particle[kParticleNum];
	std::array<std::shared_ptr<ParticleBase>, kParticleNum> particle;
	for (auto& pPart : particle)
	{
		pPart = std::make_shared<ParticleBase>();
	}
	int showerFrame = kShowerInterval;
	int flowerFrame = kFlowerInterval;
	int auraFrame = kAuraInterval;

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
		ClearDrawScreen();

		// update
		sinRate += 0.20f;
		int mouseX, mouseY;
		GetMousePoint(&mouseX, &mouseY);

		for (auto& pPart : particle)
		{
			if (!pPart->isExist())	continue;
			pPart->update();
		}
		// �p�[�e�B�N������
		showerFrame--;
		if (showerFrame <= 0)
		{
			int count = 0;
			// �����ʒu�����ɔ��ŗ����Ă���
			for (auto& pPart : particle)
			{
				if (pPart->isExist())	continue;

				Vec2 pos;
				pos.x = 1300 + sinf(sinRate) * 32.0f;
				pos.y = 256;

				Vec2 vec;
				vec.x = static_cast<float>(GetRand(8)) - 4.0f;
				vec.y = -static_cast<float>(GetRand(12));

				pPart->start(pos);
				pPart->setVec(vec);
				pPart->setGravity(0.8f);

				count++;
				if (count >= 4)
				{
					break;
				}
			}

			showerFrame = kShowerInterval;
		}

		flowerFrame--;
		
		if (flowerFrame <= 0)
		{
			int count = 0;
			// �����ʒu�����ɔ��ŗ����Ă���
			for (auto& pPart : particle)
			{
				if (pPart->isExist())	continue;

				float randSin = static_cast<float>(GetRand(360)) / 360.0f;
				randSin *= DX_TWO_PI_F;
				float randSpeed = static_cast<float>(GetRand(160)) / 10.0f + 1.0f;

				Vec2 pos;
				pos.x = 256 + cosf(randSin) * 2.0f;
				pos.y = 256 + sinf(randSin) * 2.0f;

				Vec2 vec;
				vec.x = cosf(randSin) * randSpeed;
				vec.y = sinf(randSin) * randSpeed;

				pPart->start(pos);
				pPart->setVec(vec);
				pPart->setRadius(2.0f);
				pPart->setColor(0xff2020);
				pPart->setGravity(0.4f);

				count++;
				if (count >= 64)
				{
					break;
				}
			}
			flowerFrame = kFlowerInterval;
		}
		auraFrame--;
		if (auraFrame <= 0)
		{
			int count = 0;
			for (auto& pPart : particle)
			{
				if (pPart->isExist())	continue;

				float randSin = static_cast<float>(GetRand(360)) / 360.0f;
				randSin *= DX_TWO_PI_F;

				float randDist = GetRand(6) + 29.0f;

				Vec2 pos;
				pos.x = 512 + cosf(randSin) * randDist;
				pos.y = 256 + sinf(randSin) * randDist * 0.5f;

				Vec2 vec;
				vec.x = 0.0f;
				vec.y = -GetRand(4) - 1.0f;

				pPart->start(pos);
				pPart->setVec(vec);
				pPart->setRadius(4.0f);
			//	pPart->setColor(0xa0a0ff);
				pPart->setColor(0x802010);
				pPart->setGravity(-0.4f);
				pPart->setAlphaDec(16);
				pPart->setRadiusAcc(0.1f);

				count++;
				if (count >= 64)
				{
					break;
				}
			}

			count = 0;
			for (auto& pPart : particle)
			{
				if (pPart->isExist())	continue;

				float randSin = static_cast<float>(GetRand(360)) / 360.0f;
				randSin *= DX_TWO_PI_F;
				float randSpeed = static_cast<float>(GetRand(60)) / 10.0f + 1.0f;

				Vec2 pos;
				pos.x = mouseX + cosf(sinRate) * 128.0f;
				pos.y = mouseY + sinf(sinRate) * 128.0f;

				Vec2 vec;
				vec.x = cosf(randSin) * randSpeed;
				vec.y = sinf(randSin) * randSpeed;

				pPart->start(pos);
				pPart->setVec(vec);
				pPart->setRadius(4.0f);
				pPart->setColor(0xf0f0ff);
				pPart->setGravity(0.0f);
				pPart->setAlphaDec(16);

				count++;
				if (count >= 32)
				{
					break;
				}
			}

			count = 0;
			for (auto& pPart : particle)
			{
				if (pPart->isExist())	continue;

				float randSin = static_cast<float>(GetRand(360)) / 360.0f;
				randSin *= DX_TWO_PI_F;
				float randSpeed = static_cast<float>(GetRand(60)) / 10.0f + 1.0f;

				Vec2 pos;
				float dist = static_cast<float>(128 + GetRand(32));
				pos.x = 256*3 + cosf(randSin) * dist;
				pos.y = 512 + sinf(randSin) * dist;
				
				Vec2 vec;
				vec.x = -cosf(randSin) * randSpeed;
				vec.y = -sinf(randSin) * randSpeed;

				pPart->start(pos);
				pPart->setVec(vec);
				pPart->setRadius(4.0f);
				pPart->setColor(0x80ff80);
				pPart->setGravity(0.0f);
				pPart->setAlphaDec(16);
				pPart->setRadiusAcc(-0.05f);

				count++;
				if (count >= 32)
				{
					break;
				}
			}

			auraFrame = kAuraInterval;
		}


		// draw
		DrawString(16, 8, "�p�[�e�B�N��(�Ƃ������̂����̊�)���ړ������ĉ��o���s��", 0xffffff);

		DrawString(512-64, 300, "sin,cos���g���ăp�[�e�B�N���̔����ʒu��ȉ~��ɂ���\n�d�͂�������ɐݒ肵�Ă��񂾂񑁂��ړ�����悤��", 0xffffff);

		DrawString(1200, 32 + 240 + 24, "�ۂ���̕��Ɉړ�����x�N�g����\n��������ԂŐ�����A\n�����ɂ��d�ʂɂ��������ė��Ƃ�", 0xffffff);

		DrawString(mouseX, mouseY + 64, "�}�E�X�̂܂������悤�Ƀp�[�e�B�N������", 0xffffff);

		int count = 0;
		for (auto& pPart : particle)
		{
			if (!pPart->isExist())	continue;
			pPart->draw();
			count++;
		}
		DrawFormatString(24, 24, 0xffffff, "Particle Num : %d", count);
		DrawFormatString(24, 40, 0xffffff, "DrawCall : %d", GetDrawCallCount());

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