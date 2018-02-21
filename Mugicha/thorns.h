#pragma once

#include "scalable_object.h"
#include "player.h"
#include "ragged_floor.h"

/*
** Thorns: トゲクラス
** プレイヤに当たると死ぬ
** トゲには向きがあり，下向きのものは一つの足場ブロックなどに張り付いている
** 
*/

class Thorns : public ScalableObject
{
private:
	SquarePolygonBase *floor; // RaggedFloorである必要はない（？）
	Player *player;
	bool up; // 向き，TRUEで上向き，FALSEで下向き

public:
	Thorns();
	Thorns(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 *_camera, SquarePolygonBase *_floor, Player *_player, bool _up, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void init();
	void update();
};