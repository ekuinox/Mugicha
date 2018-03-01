#include "item.h"
#include "collision_checker.h"
#include "player.h"
#include "gimmick_switch.h"

Item::Item(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), held(false), polygons(_polygons), on_ground(false)
{
}

void Item::update()
{
	auto current = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_update).count() > UPDATE_INTERVAL)
	{
		unless(held)
		{
			// 当たりを取っていきます
			char result = 0x00;
			for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR, SquarePolygonBase::PolygonTypes::THORN, SquarePolygonBase::PolygonTypes::AIRCANNON })
				for (const auto& polygon : polygons[type])
					result |= where_collision(this, polygon, 1.0f);

			if (result & HitLine::BOTTOM)
			{
				on_ground = true;
			}
			else
			{
				on_ground = false;
			}

			if (!(on_ground || held))
			{
				y -= 1.0f;
			}
		}

		// スイッチ類との当たり判定
		for (const auto& gimmick_switch : polygons[SquarePolygonBase::PolygonTypes::GIMMICK_SWITCH])
		{
			auto another = gimmick_switch->get_square();
			auto self = get_square();

			// スイッチの上に当たっとるか
			if (hit_top(another, self))
			{
				static_cast<GimmickSwitch*>(gimmick_switch)->press();
			}
			else
			{
				static_cast<GimmickSwitch*>(gimmick_switch)->release();
			}
		}

		latest_update = current;
	}
}

SQUARE Item::get_square()
{
	if (held) return PlainSquarePolygon::get_square();
	else return ScalableObject::get_square();
}

void Item::hold()
{
	held = true;
}

bool Item::hold(SQUARE sq)
{
	if (is_collision(sq, get_square()))
	{
		hold();
		return true;
	}
	return false;
}

void Item::release()
{
	held = false;
}

bool Item::is_held()
{
	return held;
}

bool Item::move(D3DXVECTOR2 vec)
{
	if(!held) return false;
	x = vec.x;
	y = vec.y;
	return true;
}

void Item::generate_vertexes()
{
	if (held) PlainSquarePolygon::generate_vertexes();
	else ScalableObject::generate_vertexes();
}
