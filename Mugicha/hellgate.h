#pragma once
#include "scalable_object.h"
#include "player.h"

#define HELLGATE_SPEED (0.01f)
#define AREA (2)
/*
* HellGate
* ‰º‚©‚ç‚¹‚Ü‚è‚­‚éƒAƒŒ
*/

class HellGate : public ScalableObject
{
public:
	HellGate(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, Player *_player, D3DXVECTOR2 _start, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void update();
	bool is_started();
	bool started;
private:
	Player *player;
	D3DXVECTOR2 start;
};