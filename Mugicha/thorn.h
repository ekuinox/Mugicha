#pragma once

#include "scalable_object.h"
#include "player.h"
#include "ragged_floor.h"

/*
** Thorn: トゲクラス
** プレイヤに当たると死ぬ
** トゲには向きがあり，下向きのものは一つの足場ブロックなどに張り付いている
** 
*/

class Thorn : public ScalableObject
{
public:
	enum class Vec { UP, DOWN, LEFT, RIGHT };
private:
	SquarePolygonBase *floor; // RaggedFloorである必要はない（？）
	enum Vec vec;
	bool falling;
	PolygonsContainer &polygons;
	std::chrono::system_clock::time_point stop_time;
	const long interval; // 再装填時間
public:
	Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, enum Vec _vec, PolygonsContainer &_polygons, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, enum Vec _vec, PolygonsContainer &_polygons, bool _attack, long _interval = 5000, float _speed = 1.0f, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void set_floor(); // polygonsから検索する
	void update();

	// original
	const bool attack;

	Thorn::Vec get_vec();
	void trigger_falling();
	void stop_falling();
};