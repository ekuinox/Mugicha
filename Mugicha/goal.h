#pragma once

#include "scalable_object.h"
#include "player.h"

class Goal : public ScalableObject
{
private:
	bool completed;
	Player *player;
public:
	Goal();
	Goal(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 *_camera, Player *_player, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~Goal();

	void update();
	void init();
	// �Ǝ��֐�

	bool is_completed();
};
