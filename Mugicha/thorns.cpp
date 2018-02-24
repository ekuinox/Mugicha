#include "thorns.h"
#include "collision_checker.h"


Thorns::Thorns()
{
}

Thorns::Thorns(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 * _camera, Player * _player, Vec _vec, SquarePolygonBase * _floor, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), floor(_floor), vec(_vec), player(_player)
{
	init();
}
void Thorns::set_floor(std::vector<SquarePolygonBase*> _floors)
{
	if (floor != nullptr) return;

	// ���̌����ɍ��킹�āC���ɂȂ�u���b�N��T��
	for (const auto& _floor : _floors)
	{
		char result = where_collision(this, _floor, 0);
		printf("%d ", result);
		if (vec == Vec::UP && result == BOTTOM)
		{
			set_floor(_floor);
			break;
		}
		else if (vec == Vec::DOWN && result == TOP)
		{
			set_floor(_floor);
			break;
		}
		else if (vec == Vec::LEFT && result == RIGHT)
		{
			set_floor(_floor);
			break;
		}
		else if (vec == Vec::RIGHT && result == LEFT)
		{
			set_floor(_floor);
			break;
		}
	}
	printf("\n");
}
void Thorns::set_floor(SquarePolygonBase *_floor)
{
	if (floor != nullptr) return;

	floor = _floor;
}

void Thorns::init()
{
}

void Thorns::update()
{
	// �Ƃ��Ɏ��ԊǗ����邱�ƂȂ��H
	
	// ������ݒ肵�Ȃ���
	if (floor != nullptr)
	{
		switch (vec)
		{
		case Vec::UP:
			y = floor->get_coords().y + (floor->get_size().h + h) / 2;
			break;
		case Vec::DOWN:
			y = floor->get_coords().y + (floor->get_size().h + h) / -2;
			break;
		case Vec::LEFT:
			y = floor->get_coords().y;
			break;
		case Vec::RIGHT:
			y = floor->get_coords().y;
			break;
		}
	}
	
	

	if (is_collisionA(player, this) && zoom_level.w <= 1)
	{
		// �����蔻��
		player->kill();
#ifdef _DEBUG
		printf("�g�Q�������Ƃ񂼁I");
#endif
	}
}