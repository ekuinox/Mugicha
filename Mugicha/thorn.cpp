#include "thorn.h"
#include "collision_checker.h"

Thorn::Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, Vec _vec, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, float _u, float _v, float _uw, float _vh)
	: Thorn(_x, _y, _w, _h, _tex, _layer, _camera, _vec, _polygons, false)
{
}

Thorn::Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, Vec _vec, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, bool _attack, long _interval, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), vec(_vec), attack(_attack), falling(false), polygons(_polygons), floor(nullptr), stop_time(std::chrono::system_clock::now()), interval(_interval)
{
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
	if (falling)
	{
		y -= 1.0f;

		// ここをブロックと接触すれば終了にする
		if (attack)
		{
			auto self = get_square();
			for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR })
			{
				for (const auto& block : polygons[type])
				{
					auto block_sq = block->get_square();
					if(hit_bottom(self, block_sq))
					{
						stop_falling();
						break;
					}
				}
			}
			
		}
	}
	// 高さを設定しなおす
	else if (floor != nullptr)
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
	
}

Thorn::Vec Thorn::get_vec()
{
	return vec;
}

void Thorn::trigger_falling()
{
	if (attack && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - stop_time).count() > interval) falling = true;
}

void Thorn::stop_falling()
{
	if (attack)
	{
		falling = false;
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
		stop_time = std::chrono::system_clock::now(); // 終了した時刻を保管
	}
}
