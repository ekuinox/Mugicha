#pragma once

#include "scalable_object.h"
#include "helpers.h"

// �����l�Ȃ�
#define COLLISION_CHECK_POLYGONTYPES { \
	SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, \
	SquarePolygonBase::PolygonTypes::RAGGED_FLOOR, \
	SquarePolygonBase::PolygonTypes::THORN, \
	SquarePolygonBase::PolygonTypes::AIRCANNON, \
	SquarePolygonBase::PolygonTypes::GIMMICK_SWITCH \
	} // �����蔻������|���S���̃^�C�v�܂Ƃ�
#define ENEMY_WIDTH (CELL_WIDTH * 0.8) // ��
#define ENEMY_HEIGHT (CELL_HEIGHT * 0.8) // ����
#define ENEMY_SPEED (0.3f)
#define ENEMY_JUMP_POWER (1.01f)
#define ENEMY_FALLING PLAYER_FALLING
#define ENEMY_JUMP_TIME PLAYER_JUMP_TIME

/*
* Enemy
* �G����̃N���X
*/

class Enemy : public ScalableObject
{
public:
	enum class Vec{ LEFT, RIGHT}; // �i�ތ���
private:
	bool on_ground; // �ڒn
	bool jumping; // �W�����v���Ă���
	bool moving; // �s��
	time_point jumped_at; // �W�����v�������_�̎���
	Vec vec;
	PolygonsContainer &polygons;
public:
	Enemy(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, int _layer, float _x, float _y, Vec _vec, PolygonsContainer &_polygons, float _w = ENEMY_WIDTH, float _h = ENEMY_HEIGHT, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void update();
};