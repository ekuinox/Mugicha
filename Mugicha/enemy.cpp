#include "enemy.h"
#include "collision_checker.h"

Enemy::Enemy(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 * _camera, int _layer, float _x, float _y, float _w, float _h, Vec _vec, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), vec(_vec), alive(true), polygons(_polygons), player(static_cast<Player*>(_polygons[SquarePolygonBase::PolygonTypes::PLAYER].front())), moving(false)
{
}

void Enemy::update()
{
	if (!status) return;
	
	auto current = timeGetTime();

	// �v���C���Ƃ̋������݂Ĉړ����J�n���� �G�̃v���C�x�[�g�ȃG���A�ɓ�������L���Ă��銴����
	moving = is_collision(SQUARE(x, y, w * 10, h * 10), player->get_square()) ? true : false;

	if (current - latest_update > 1 && moving) // 1ms�Ԋu��
	{
		// �����蔻����݂�|���S���̃x�N�^�����
		std::vector<SquarePolygonBase*> to_check_polygons;
		for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR }) to_check_polygons.insert(to_check_polygons.end(), polygons[type].begin(), polygons[type].end());

		// �����萸��
		char result = 0x00;
		float ground_height = y;
		for (const auto& polygon : to_check_polygons) result |= where_collision(this, polygon, 0);

		auto vector = D3DXVECTOR2(0, 0); // ������ړ���������������

		// �A�^�}�Ԃ��Ƃ�
		if (result & TOP) jumping = false;

		// �������Ă���
		if (result & BOTTOM) on_ground = true;

		// �����Ԃ����Ă���
		if (result & LEFT) vec = Vec::RIGHT;

		// �E���Ԃ����Ă���
		if (result & RIGHT) vec = Vec::LEFT;
		
		vector.x += vec == Vec::RIGHT ? 1 : -1;

		// �W�����v�J�n���玞�Ԃ��o��
		if (timeGetTime() - jumped_at > 500) jumping = false;
		
		// �W�����v�Ȃ�
		if (jumping) vector.y += 2.0f;
		
		// �󒆂ɋ���Ԃ͗�����������
		if (!on_ground) vector.y -= 1.0f;

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
