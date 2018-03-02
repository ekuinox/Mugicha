#include "bullet.h"
#include "collision_checker.h"

Bullet::Bullet(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, Bullet::Vec _vec, int _interval, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), vec(_vec), init_coords(_x, _y), triggered(false), disappeared_time(std::chrono::system_clock::now()), interval(_interval)
{
	speed = 1.0f;
	init();
}

void Bullet::init()
{
	triggered = false;
	hide(); // î≠éÀÇ≥ÇÍÇÈÇ‹Ç≈âBÇ∑
	x = init_coords.x;
	y = init_coords.y;
	disappeared_time = std::chrono::system_clock::now();
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
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - disappeared_time).count() > interval)
	{
		triggered = true;
		show(); // î≠éÀÇ≥ÇÍÇΩíiäKÇ≈å©Ç¶ÇÈÇÊÇ§Ç…
	}
}

Bullet::Vec Bullet::get_vec()
{
	return vec;
}

bool Bullet::is_triggered()
{
	return triggered;
}

