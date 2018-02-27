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
public:
	enum class Vec { UP, DOWN, LEFT, RIGHT };
private:
	SquarePolygonBase *floor; // RaggedFloorである必要はない（？）
	Player *player;
	enum Vec vec;
	
public:
	Thorns(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, Player *_player, enum Vec _vec, SquarePolygonBase *_floor = nullptr, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void set_floor(std::vector<SquarePolygonBase*> _floors); // floorがnullptrのままの場合ここで設定しなおします．
	void set_floor(SquarePolygonBase *_floor); // floorがnullptrのままの場合ここで設定しなおします．
	void init();
	void update();
};