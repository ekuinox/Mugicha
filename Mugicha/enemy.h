#pragma once

#include "scalable_object.h"
#include "helpers.h"

// �����l�Ȃ�
#define ENEMY_COLLISION_CHECK_POLYGONTYPES { \
	SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, \
	SquarePolygonBase::PolygonTypes::RAGGED_FLOOR, \
	SquarePolygonBase::PolygonTypes::THORN, \
	SquarePolygonBase::PolygonTypes::AIRCANNON, \
	SquarePolygonBase::PolygonTypes::GIMMICK_SWITCH \
	} // �����蔻������|���S���̃^�C�v�܂Ƃ�
#define ENEMY_WIDTH (CELL_WIDTH * 0.8) // ��
#define ENEMY_HEIGHT (CELL_HEIGHT * 0.8) // ����
#define ENEMY_SPEED (0.3f)
#define ENEMY_JUMP_POWER (2.0f)
#define ENEMY_JUMP_HEIGHT (CELL_HEIGHT * 2.0)
#define ENEMY_FALLING (1.0f)
#define RESPAWN_TIME (5000) // ���X�|�[���ɂ����鎞�ԁi�����l�j
#define ACTIVE_AREA_WIDTH (SCREEN_WIDTH * 2)
#define ACTIVE_AREA_HEIGHT (SCREEN_HEIGHT * 2)

/*
* Enemy
* �G����̃N���X
*/

class Enemy : public ScalableObject
{
public:
	enum class Vec{ LEFT, RIGHT}; // �i�ތ���
private:
	D3DXVECTOR2 init_coords; // �����ʒu
	bool alive; // �����Ă��邩
	time_point dead_time; // ���񂾃^�C�~���O
	const float respawn_time; // ���X�|�[���ɂ����鎞��
	bool on_ground; // �ڒn
	bool jumping; // �W�����v���Ă���
	bool moving; // �s��
	float jumped_at; // �W�����v�������_�̎���
	Vec vec;
	PolygonsContainer &polygons;

	void init(); // ������
	void generate_vertexes();
public:
	Enemy(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, int _layer, float _x, float _y, Vec _vec, PolygonsContainer &_polygons, float _w = ENEMY_WIDTH, float _h = ENEMY_HEIGHT, float _respawn_time = RESPAWN_TIME, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void update();
	bool is_alive();
	void kill();
};