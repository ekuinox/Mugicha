#pragma once
#include <map>
#include "gimmick_switch.h"

/*
* Item
* スイッチ押したりするためのアイテム
* プレイヤに掴まれている時は拡縮の影響を受けない
* プレイヤに掴まれる，また離されるときにはそのサイズを保持する
*/


class Item : public ScalableObject
{
public:
	Item(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, PolygonsContainer &_polygons, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void update();
	SQUARE get_square();
	void hold();
	bool hold(SQUARE sq);
	void release();
	bool is_held();
	bool move(D3DXVECTOR2 vec);
private:
	PolygonsContainer &polygons; // 当たり見るように持っときます
	bool held; // プレイヤに掴まれているか
	bool on_ground;
	POLSIZE default_size;
	GimmickSwitch *gimmick_switch;
	void generate_vertexes();
};

