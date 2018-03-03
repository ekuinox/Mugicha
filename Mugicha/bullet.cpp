#include "bullet.h"
#include "collision_checker.h"

Bullet::Bullet(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, Bullet::Vec _vec, int _interval, float _speed, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh),
	vec(_vec), init_coords(_x, _y), triggered(false), disappeared_time(SCNOW), interval(_interval)
{
	speed = _speed;
	init();
}

void Bullet::init()
{
	triggered = false;
	hide(); // ”­ŽË‚³‚ê‚é‚Ü‚Å‰B‚·
	x = init_coords.x;
	y = init_coords.y;
	disappeared_time = SCNOW;
}

void Bullet::update()
{
	// Update‚ÌŽžŠÔŠÇ—‚ÍAirCannon‘¤‚Å‚â‚Á‚Ä‚­‚ê‚Ä‚é‚Ì‚Ål—¶‚µ‚È‚¢
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
	if (time_diff(disappeared_time) > interval)
	{
		triggered = true;
		show(); // ”­ŽË‚³‚ê‚½’iŠK‚ÅŒ©‚¦‚é‚æ‚¤‚É
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

