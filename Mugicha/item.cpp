#include "item.h"
#include "collision_checker.h"

Item::Item(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), held(false), polygons(_polygons), player(nullptr), on_ground(false)
{
}

void Item::update()
{
	auto current = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_update).count() > UPDATE_INTERVAL)
	{
		if (held)
		{
			auto vec = player->get_vec();

			if (vec == Player::Vec::LEFT)
			{
				y = player->get_coords().y;
				x = get_square().left();
			}
			else if (vec == Player::Vec::RIGHT)
			{
				y = player->get_coords().y;
				x = get_square().right();
			}
		}
		
		// “–‚½‚è‚ðŽæ‚Á‚Ä‚¢‚«‚Ü‚·
		char result = 0x00;
		for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR, SquarePolygonBase::PolygonTypes::THORN, SquarePolygonBase::PolygonTypes::AIRCANNON})
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

		latest_update = current;
	}
}

SQUARE Item::get_square()
{
	if (held) PlainSquarePolygon::get_square();
	else ScalableObject::get_square();
}

void Item::hold()
{
	held = true;
}

bool Item::hold(Player *_player)
{
	if (is_collision(_player->get_square(), get_square()))
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

void Item::generate_vertexes()
{
	if (held) PlainSquarePolygon::generate_vertexes();
	else ScalableObject::generate_vertexes();
}
