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
	std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &polygons;
public:
	Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, enum Vec _vec, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, enum Vec _vec, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, bool _attack, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void set_floor(std::vector<SquarePolygonBase*> _floors); // floorがnullptrのままの場合ここで設定しなおします．
	void set_floor(SquarePolygonBase *_floor); // floorがnullptrのままの場合ここで設定しなおします．
	void init();
	void update();

	// original
	const bool attack;

	Thorn::Vec get_vec();
	void trigger_falling();
	void stop_falling();
};