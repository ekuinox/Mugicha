#include "enemy.h"
#include "collision_checker.h"
#include "gimmick_floor.h"

void Enemy::init()
{
	x = init_coords.x;
	y = init_coords.y;

	moving = false;
	jumping = false;
	on_ground = true;
}

void Enemy::generate_vertexes()
{
	PlainSquarePolygon::generate_vertexes();

	// �t�����ɂ���
	if (vec == Enemy::Vec::LEFT)
	{
		for (auto i = 0; i < 4; ++i)
		{
			vertexes[i].u = u + (i % 3 == 0 ? uw : 0);
		}
	}
}

Enemy::Enemy(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, int _layer, float _x, float _y, Vec _vec, PolygonsContainer &_polygons, float _w, float _h, float _respawn_time, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), vec(_vec),
	polygons(_polygons), moving(false), jumping(false), jumped_at(_y), on_ground(true), respawn_time(_respawn_time)
{
	speed = ENEMY_SPEED;
	init_coords = { _x, _y };
	init();
}

void Enemy::update()
{
	unless (status) return;

	// ��ʊO���ǂ�����moving��؂蕪����
	moving = (vertexes[0].x <= ACTIVE_AREA_WIDTH && vertexes[1].x >= 0 && vertexes[0].y <= ACTIVE_AREA_HEIGHT && vertexes[2].y >= 0) ? true : false;

	if (alive && moving) // 1ms�Ԋu��
	{
		// �����萸��
		char result = 0x00;
		for (const auto& type : ENEMY_COLLISION_CHECK_POLYGONTYPES)
			for (const auto& polygon : polygons[type])
				result |= where_collision(this, polygon, 1.0f);

		// GIMMICK_FLOOR����appearing������
		for (const auto& polygon : polygons[SquarePolygonBase::PolygonTypes::GIMMICK_FLOOR])
			if (static_cast<GimmickFloor*>(polygon)->is_appearing())
				result |= where_collision(this, polygon, 1.0f);

		auto vector = D3DXVECTOR2(0, 0); // ������ړ���������������

		// �A�^�}�Ԃ��Ƃ�
		if (result & TOP) jumping = false;

		// �������Ă���
		if (result & BOTTOM) on_ground = true;

		// �����Ԃ����Ă���
		if (result & LEFT) vec = Vec::RIGHT;

		// �E���Ԃ����Ă���
		if (result & RIGHT) vec = Vec::LEFT;
		
		// �W�����v�J�n���玞�Ԃ��o��
		if (y - jumped_at > ENEMY_JUMP_HEIGHT) jumping = false;

		// ���X�W�����v���Ă���
		if (on_ground && !jumping)
		{
			jumping = true;
			on_ground = false;
			jumped_at = y;
		}

		// ���ԊǗ�����
		auto current = SCNOW;
		if (time_diff(latest_update, current) > UPDATE_INTERVAL)
		{
			latest_update = current;

			// x�ړ�
			vector.x += speed * (vec == Vec::RIGHT ? 1 : -1);

			// �W�����v�Ȃ�
			if (jumping) vector.y += ENEMY_JUMP_POWER;

			// �󒆂ɋ���Ԃ͗�����������
			if (!on_ground) vector.y -= ENEMY_FALLING;
		}


		// ���f
		x += vector.x;
		y += vector.y;
	}

	// ���X�|�[��
	if (time_diff(dead_time) > respawn_time)
	{
		alive = true;
		show();
	}
}

bool Enemy::is_alive()
{
	return alive;
}

void Enemy::kill()
{
	alive = false;
	dead_time = SCNOW;
	hide();
}
