#include "air_cannon.h"
#include "collision_checker.h"

AirCannon::AirCannon(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, LPDIRECT3DTEXTURE9 _bullet_tex, int _layer, D3DXVECTOR2 & _camera, AirCannon::Vec _vec, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), vec(_vec), polygons(_polygons)
{
	auto bullet_coords = D3DXVECTOR2(x, y);
	Bullet::Vec bullet_vec;

	if (vec == AirCannon::Vec::UP)
	{
		bullet_coords.y += h;
		bullet_vec = Bullet::Vec::UP;
	}
	else if (vec == AirCannon::Vec::DOWN)
	{
		bullet_coords.y -= h;
		bullet_vec = Bullet::Vec::DOWN;
	}
	else if (vec == AirCannon::Vec::LEFT)
	{
		bullet_coords.x -= w;
		bullet_vec = Bullet::Vec::LEFT;
	}
	else if (vec == AirCannon::Vec::RIGHT)
	{
		bullet_coords.x += w;
		bullet_vec = Bullet::Vec::RIGHT;
	}

	bullet = new Bullet(bullet_coords.x, bullet_coords.y, CELL_WIDTH * 0.8, CELL_HEIGHT * 0.8, _bullet_tex, 1, _camera, bullet_vec);
	bullet->on();
	bullet->trigger();
}

void AirCannon::update()
{
	bullet->update();

	// •Ç‚Ö‚Ì“–‚½‚è‚ð‚Ý‚Äinit‚µ‚Ä‚â‚é
	if (bullet->is_triggered())
	{
		for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR })
		{
			for (const auto& polygon : polygons[type])
			{
				if (is_collision(bullet->get_square(), polygon->get_square()))
				{
					bullet->init();
					break;
				}
			}
		}
	}
	else
	{
		bullet->trigger();
	}
}

void AirCannon::draw()
{
	bullet->draw();
	ScalableObject::draw();
}

Bullet *AirCannon::get_bullet()
{
	return bullet;
}
