#include "air_cannon.h"

AirCannon::AirCannon(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, LPDIRECT3DTEXTURE9 _bullet_tex, int _layer, D3DXVECTOR2 & _camera, AirCannon::Vec _vec, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), vec(_vec)
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

	bullet = new Bullet(bullet_coords.x, bullet_coords.y, CELL_WIDTH, CELL_HEIGHT, _bullet_tex, 1, _camera, bullet_vec);
	bullet->on();
	bullet->trigger();
}

void AirCannon::update()
{
	auto current = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_update).count() > UPDATE_INTERVAL)
	{
		if(std::abs(x - bullet->get_coords().x) < SCREEN_WIDTH && std::abs(y - bullet->get_coords().y) < SCREEN_HEIGHT)
		{
			bullet->update();
		}
		else
		{
			bullet->init();
		}
		latest_update = current;
	}
}

void AirCannon::draw()
{
	ScalableObject::draw();
	bullet->draw();
}

Bullet *AirCannon::get_bullet()
{
	return bullet;
}
