#include "thorn.h"
#include "collision_checker.h"

Thorn::Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, Player * _player, Vec _vec, SquarePolygonBase * _floor, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), floor(_floor), vec(_vec), player(_player)
{
	init();
}
void Thorn::set_floor(std::vector<SquarePolygonBase*> _floors)
{
	if (floor != nullptr) return;

	// ���̌����ɍ��킹�āC���ɂȂ�u���b�N��T��
	for (const auto& _floor : _floors)
	{
		char result = where_collision(this, _floor, 0);

		if (vec == Vec::UP && result == HitLine::BOTTOM)
		{
			set_floor(_floor);
			break;
		}
		if (vec == Vec::DOWN && result == HitLine::TOP)
		{
			set_floor(_floor);
			break;
		}
		if (vec == Vec::LEFT && result == HitLine::RIGHT)
		{
			set_floor(_floor);
			break;
		}
		if (vec == Vec::RIGHT && result == HitLine::LEFT)
		{
			set_floor(_floor);
			break;
		}
	}
}
void Thorn::set_floor(SquarePolygonBase *_floor)
{
	if (floor != nullptr) return;

	floor = _floor;
}

void Thorn::init()
{
}

void Thorn::update()
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

	/*
	if (zoom_level.w <= 1)
	{
		char result = where_collision(this, player, 1.0f);
		if ((result & HitLine::BOTTOM && vec == Vec::DOWN) || (result & HitLine::TOP && vec == Vec::UP) || (result & HitLine::LEFT && vec == Vec::LEFT) || (result & HitLine::RIGHT && vec == Vec::RIGHT))
		{
			player->kill("�g�Q�ɓ������Ď�");
		}
	}
	*/

}

Thorn::Vec Thorn::get_vec()
{
	return vec;
}
