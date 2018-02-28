#pragma once
#include "scalable_object.h"
class Bullet : public ScalableObject
{
public:
	enum class Vec
	{
		UP,
		DOWN,
		RIGHT,
		LEFT,
	};
	Bullet(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, Bullet::Vec _vec, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void init();
	void update();
	void trigger();
private:
	Bullet::Vec vec;
	D3DXVECTOR2 init_coords;
	bool triggered;
};

