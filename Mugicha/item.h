#pragma once
#include <map>
#include "gimmick_switch.h"

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
	PolygonsContainer &polygons; // �����茩��悤�Ɏ����Ƃ��܂�
	bool held; // �v���C���ɒ͂܂�Ă��邩
	bool on_ground;
	GimmickSwitch *gimmick_switch;
	float def_w;
	float def_h;
	void generate_vertexes();
};

