#pragma once

#include "scalable_object.h"
#include "player.h"
#include "ragged_floor.h"

/*
** Thorn: �g�Q�N���X
** �v���C���ɓ�����Ǝ���
** �g�Q�ɂ͌���������C�������̂��͈̂�̑���u���b�N�Ȃǂɒ���t���Ă���
** 
*/

class Thorn : public ScalableObject
{
public:
	enum class Vec { UP, DOWN, LEFT, RIGHT };
private:
	SquarePolygonBase *floor; // RaggedFloor�ł���K�v�͂Ȃ��i�H�j
	enum Vec vec;
	bool falling;
	PolygonsContainer &polygons;
	std::chrono::system_clock::time_point stop_time;
	const long interval; // �đ��U����
public:
	Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, enum Vec _vec, PolygonsContainer &_polygons, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	Thorn(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, enum Vec _vec, PolygonsContainer &_polygons, bool _attack, long _interval = 5000, float _speed = 1.0f, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void set_floor(); // polygons���猟������
	void update();

	// original
	const bool attack;

	Thorn::Vec get_vec();
	void trigger_falling();
	void stop_falling();
};