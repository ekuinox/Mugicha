#pragma once

#ifdef _DEBUG_
#define _PLAYER_DEBUG
#endif

#include "plain_square_polygon.h"
#include "conf.h"
#include <map>
#include "input.h"

// �v���C���[�N���X

class Player : public PlainSquarePolygon
{
public:
	enum class DeadReason
	{
		ALIVE, // �����Ƃ�킢
		Sandwiched, // ���܂�Ď���
		HitEnemy, // �G�ɐڐG
		HitThorn, // �g�Q�ɐڐG
		Falling, // ���ɗ���
		HitMagma, // �}�O�}�ŃA�`�`
		Shot, // Bullet�Ƀh�K�[��

	};
	enum class Vec
	{
		CENTER,
		RIGHT,
		LEFT
	};
private:
	DeadReason dead_reason; // �����Ă��邩�C�܂�����ł���Ȃ炻�̗��R
	bool jumping; // �W�����v���Ă���
	bool ground; // ���n���
	bool moving; // �����Ă���(x��)
	DWORD jumped_at;
	bool controll_lock; // ������󂯕t���Ȃ�����
	D3DXVECTOR2 when_locked_coords; // ���b�N���ꂽ�Ƃ��̍��W�l
	POLSIZE zoom_level; // �Y�[���T�C�Y
	POLSIZE before_zoom_level; // �O�̃��c
	std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &polygons; // �����茩��悤�Ɏ����Ƃ��܂�
	Player::Vec vec;

	bool collision_for_enemies();
	bool collision_for_thorns();
	bool collision_for_magmas();
	bool collision_for_bullets();
public:
	Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, int _layer, float _x, float _y, float _w, float _h, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~Player();
	
	void init();
	void update();
	void zoom(POLSIZE _zoom_level);

	// �Ǝ��֐�
	bool jump();
	void lock();
	void unlock();
	void kill(const DeadReason &_dead_reason);
	Player::DeadReason dead();
	Player::Vec get_vec();
};