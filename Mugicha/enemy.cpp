#include "enemy.h"
#include "collision_checker.h"

Enemy::Enemy(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, int _layer, float _x, float _y, float _w, float _h, Vec _vec, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), vec(_vec), alive(true), polygons(_polygons), moving(false)
{
	speed = 0.3f;
}

void Enemy::update()
{
	if (!status) return;
	
	auto current = std::chrono::system_clock::now();

	// ��ʊO���ǂ�����moving��؂蕪����
	moving = (vertexes[0].x <= SCREEN_WIDTH * 1.25 && vertexes[1].x >= 0 && vertexes[0].y <= SCREEN_HEIGHT * 1.25 && vertexes[2].y >= 0) ? true : false;

	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_update).count() > 1 && moving) // 1ms�Ԋu��
	{
		// �����萸��
		char result = 0x00;
		for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR, SquarePolygonBase::PolygonTypes::THORN })
			for (const auto& polygon : polygons[type])
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
		
		vector.x += speed * (vec == Vec::RIGHT ? 1 : -1);

		// �W�����v�J�n���玞�Ԃ��o��
		if (timeGetTime() - jumped_at > PLAYER_JUMP_TIME) jumping = false;
		
		// �W�����v�Ȃ�
		if (jumping) vector.y += 1.01f;
		
		// �󒆂ɋ���Ԃ͗�����������
		if (!on_ground) vector.y -= PLAYER_FALLING;

		// ���X�W�����v���Ă���
		if (on_ground && !jumping)
		{
			jumping = true;
			on_ground = false;
			jumped_at = timeGetTime();
		}

		// ���f
		x += vector.x;
		y += vector.y;

		latest_update = current;
	}
}
