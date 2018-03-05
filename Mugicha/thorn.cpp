#include "thorn.h"
#include "collision_checker.h"

Thorn::Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, Vec _vec, PolygonsContainer &_polygons, float _u, float _v, float _uw, float _vh)
	: Thorn(_x, _y, _w, _h, _tex, _layer, _camera, _vec, _polygons, false)
{
}

Thorn::Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, Vec _vec, PolygonsContainer &_polygons, bool _attack, long _interval, float _speed, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh),
	vec(_vec), attack(_attack), falling(false), polygons(_polygons), floor(nullptr), stop_time(SCNOW), interval(_interval)
{
	speed = _speed;
}

void Thorn::set_floor()
{
	if (floor != nullptr) return;

	auto self = get_square();

	for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR })
	{
		for (const auto& block : polygons[type])
		{
			auto block_sq = block->get_square();

			if ((vec == Vec::UP && hit_bottom(self, block_sq))
				|| (vec == Vec::DOWN && hit_top(self, block_sq))
				|| (vec == Vec::LEFT && hit_right(self, block_sq))
				|| (vec == Vec::RIGHT && hit_left(self, block_sq)))
			{
				floor = block;
				break;
			}
			
		}
	}
}

void Thorn::update()
{
	if (falling)
	{

		// 時間管理します
		auto current = SCNOW;
		if (time_diff(latest_update, current) > UPDATE_INTERVAL)
		{
			latest_update = current;

			// 落下マシマシ
			y -= speed;

			// ここをブロックと接触すれば終了にする
			if (attack)
			{
				auto self = get_square();
				for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR })
				{
					for (const auto& block : polygons[type])
					{
						auto block_sq = block->get_square();
						if (hit_bottom(self, block_sq))
						{
							stop_falling();
							break;
						}
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
	// 攻撃するかつ，前回の落下終了から一定時刻が経過していれば，再び落下を開始する
	if (attack && time_diff(stop_time) > interval)falling = true;
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
		stop_time = SCNOW; // 終了した時刻を保管
	}
}
