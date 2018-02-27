#include "thorn.h"
#include "collision_checker.h"

Thorn::Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, Vec _vec, SquarePolygonBase * _floor, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), floor(_floor), vec(_vec)
{
	init();
}
void Thorn::set_floor(std::vector<SquarePolygonBase*> _floors)
{
	if (floor != nullptr) return;

	// その向きに合わせて，床になるブロックを探す
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
	// とくに時間管理することない？
	auto current = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_update).count() > 1)
	{
		// 高さを設定しなおす
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
		latest_update = current;
	}
	
}

Thorn::Vec Thorn::get_vec()
{
	return vec;
}
