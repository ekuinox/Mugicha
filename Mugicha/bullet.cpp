#include "bullet.h"

Bullet::Bullet(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, Bullet::Vec _vec, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), vec(_vec), init_coords(_x, _y), triggered(false)
{
	speed = 1.0f;
}

void Bullet::init()
{
	x = init_coords.x;
	y = init_coords.y;
	triggered = false;
}

void Bullet::update()
{
	// UpdateÇÃéûä‘ä«óùÇÕAirCannonë§Ç≈Ç‚Ç¡ÇƒÇ≠ÇÍÇƒÇÈÇÃÇ≈çló∂ÇµÇ»Ç¢
	if (!triggered) return;

	switch (vec)
	{
	case Bullet::Vec::UP:
		y += speed;
		break;
	case Bullet::Vec::DOWN:
		y -= speed;
		break;
	case Bullet::Vec::LEFT:
		x -= speed;
		break;
	case Bullet::Vec::RIGHT:
		x += speed;
		break;
	}
}

void Bullet::trigger()
{
	triggered = true;
}

Bullet::Vec Bullet::get_vec()
{
	return vec;
}

bool Bullet::is_triggered()
{
	return triggered;
}

