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
	Bullet(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, Bullet::Vec _vec, int _interval = 3000, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void init();
	void update();
	void trigger();
	Bullet::Vec get_vec();
	bool is_triggered();
private:
	Bullet::Vec vec;
	D3DXVECTOR2 init_coords;
	bool triggered;
	const int interval;
	std::chrono::system_clock::time_point disappeared_time; // �������^�C�~���O
};

