#include "item.h"
#include "collision_checker.h"
#include "player.h"

Item::Item(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, PolygonsContainer& _polygons, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), held(false), polygons(_polygons), on_ground(false), gimmick_switch(nullptr)
{
}

void Item::update()
{
	unless(held)
	{
		// ìñÇΩÇËÇéÊÇ¡ÇƒÇ¢Ç´Ç‹Ç∑
		char result = 0x00;
		for (const auto& type : {
			SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT,
			SquarePolygonBase::PolygonTypes::RAGGED_FLOOR,
			SquarePolygonBase::PolygonTypes::THORN,
			SquarePolygonBase::PolygonTypes::AIRCANNON,
			SquarePolygonBase::PolygonTypes::GIMMICK_SWITCH,
			SquarePolygonBase::PolygonTypes::GIMMICK_FLOOR
			})
		{
			for (const auto& polygon : polygons[type])
			{
				auto _result = where_collision(this, polygon, 1.0f);
				result |= _result;
				if (type == SquarePolygonBase::PolygonTypes::GIMMICK_SWITCH && _result & HitLine::BOTTOM)
				{
					gimmick_switch = static_cast<GimmickSwitch*>(polygon);
					gimmick_switch->press();
				}

			}
		}

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
}

SQUARE Item::get_square()
{
	if (held) return PlainSquarePolygon::get_square();
	else return ScalableObject::get_square();
}

void Item::hold()
{
	held = true;
	if(gimmick_switch != nullptr) gimmick_switch->release();
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
	// ÉYÅ[ÉÄÉåÉxÉãÇ…çáÇÌÇµÇΩèÍèäÇ…ñﬂÇµÇƒÇ‚ÇÈ
	x /= zoom_level;
	y /= zoom_level;
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
