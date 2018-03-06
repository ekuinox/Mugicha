#include "hellgate.h"
#include "collision_checker.h"

HellGate::HellGate(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, Player *_player, D3DXVECTOR2 _start, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh),
	player(_player),  start(_start), started(false)
{
#ifdef _DEBUG
	printf("HELLGATE: %f, %f\n", start.x, start.y);
#endif
	speed = HELLGATE_SPEED;
}

void HellGate::update()
{
	unless(status) return;

	if (started)
	{
		y += speed;
	}
	else if(player->get_square().y > start.y * zoom_level)
	{
		// スタート
#ifdef _DEBUG
		puts("HELLGATE STARTED!!");
#endif
		y = start.y - h - CELL_HEIGHT * 2;
		started = true;
	}
}

bool HellGate::is_started()
{
	return started;
}
