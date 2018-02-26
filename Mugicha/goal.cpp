#include "goal.h"
#include "collision_checker.h"

Goal::Goal()
{
	// ���ɖ�
}

Goal::Goal(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 * _camera, Player *_player, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), player(_player), completed(false)
{
	init();
}

Goal::~Goal()
{
	// ��
}

void Goal::update()
{
	// �v���C���Ƃ̓����肾������
	if (is_collision(player, this))
	{
		// �G�ꂽ�����ŃS�[�����Ă̂��A���Ȃ̂ŁC�A������
		auto core = SQUARE(static_cast<float>(x * zoom_level.w), static_cast<float>(y * zoom_level.w), static_cast<float>(w * zoom_level.w / 2), static_cast<float>(h * zoom_level.h * 0.8));

		if (is_collision(player->get_square(), core)) // �R�A�ɓ�����\����
		{
#ifdef _DEBUG
			std::cout << "�������S�[�����\n";
#endif
			completed = true;
		}

	}
}

void Goal::init()
{
	completed = false;
}

bool Goal::is_completed()
{
	return completed;
}
