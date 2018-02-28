#include "collision_checker.h"
#include "player.h"
#include "thorn.h"

bool Player::collision_for_enemies()
{
	// �G�Ƃ̓����蔻��
	for (const auto& enemy : polygons[SquarePolygonBase::PolygonTypes::ENEMY])
	{
		if (enemy->is_active() && is_collision(enemy->get_square(), get_square()))
		{
			if (zoom_level.w >= 1.0f)
			{
#ifndef _WITHOUT_DEATH
				// �v���C���̕���
				kill(DeadReason::HitEnemy);
				return true;
#endif
			}
			else
			{
				// �G�̕���
				enemy->off();
			}
		}
	}
	return false;
}

bool Player::collision_for_thorns()
{
	// �g�Q�Ƃ̓����蔻��
	// �ʏ��Ԃ�莩�����f�J����ԂŐڐG���̔�������
	if (zoom_level.w > 1.0f) return false;
	for (const auto& thorn : polygons[SquarePolygonBase::PolygonTypes::THORN])
	{
		auto self = get_square();
		auto another = thorn->get_square();

		// �g�Q�𗎂Ƃ�
		if (vec != Player::Vec::CENTER && static_cast<Thorn*>(thorn)->attack)
		{
			if (is_collision(SQUARE(get_square().x + get_square().w * (vec == Player::Vec::RIGHT ? 1 : -1), get_square().y + get_square().h * 2, w * 2, get_square().h * 10), another))
			{
				static_cast<Thorn*>(thorn)->trigger_falling();
			}
			else
			{
				static_cast<Thorn*>(thorn)->stop_falling();
			}
		}

		switch (static_cast<Thorn*>(thorn)->get_vec())
		{
		case Thorn::Vec::UP:
			if (hit_bottom(self, another))
			{
#ifndef _WITHOUT_DEATH
				kill(DeadReason::HitThorn);
				return true;
#endif
			}
			break;
		case Thorn::Vec::DOWN:
			if (hit_top(self, another))
			{
#ifndef _WITHOUT_DEATH
				kill(DeadReason::HitThorn);
				return true;
#endif
			}
			break;
		case Thorn::Vec::RIGHT:
			if (hit_left(self, another))
			{
#ifndef _WITHOUT_DEATH
				kill(DeadReason::HitThorn);
				return true;
#endif
			}
			break;
		case Thorn::Vec::LEFT:
			if (hit_right(self, another))
			{
#ifndef _WITHOUT_DEATH
				kill(DeadReason::HitThorn);
				return true;
#endif
			}
			break;
		}
	}

	return false;
}

bool Player::collision_for_magmas()
{
	// �}�O�}�Ƃ̓����蔻��
	for (const auto& magma : polygons[SquarePolygonBase::PolygonTypes::MAGMA])
	{
		if (is_collision(get_square(), magma->get_square()))
		{
#ifndef _WITHOUT_DEATH
			kill(DeadReason::HitMagma);
			return true;
#endif
		}
	}
	return false;
}

bool Player::collision_for_bullets()
{
	// �e�ۂƂ̓����蔻��
	for (const auto& magma : polygons[SquarePolygonBase::PolygonTypes::BULLET])
	{
		if (is_collision(get_square(), magma->get_square()))
		{
#ifndef _WITHOUT_DEATH
			kill(DeadReason::Shot);
			return true;
#endif
		}
	}
	return false;
}

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh)
	: polygons(_polygons), PlainSquarePolygon(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), before_zoom_level(1, 1), dead_reason(DeadReason::ALIVE), vec(Player::Vec::CENTER)
{
	init();
}

// �f�X�g���N�^
Player::~Player()
{
	// ���ɂȂ�
}

void Player::init()
{
	speed = PLAYER_SPEED;
	ground = false;
	controll_lock = false;
}

void Player::zoom(POLSIZE _zoom_level)
{
	zoom_level = _zoom_level;
}

void Player::update()
{
	if (!status) return; // status�݂Đ؂�

	auto current = std::chrono::system_clock::now();

	// ����
	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_update).count() > UPDATE_INTERVAL) // 1ms�Ԋu��
	{
		if (collision_for_enemies())
		{
			// ��
			return;
		}

		if (collision_for_thorns())
		{
			// ��
			return;
		}

		if (collision_for_magmas())
		{
			// ��
			return;
		}

		if (collision_for_bullets())
		{
			// ���`
			return;
		}
		
		// �����萸��
		char result = 0x00;
		for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR, SquarePolygonBase::PolygonTypes::THORN, SquarePolygonBase::PolygonTypes::AIRCANNON })
			for (const auto& polygon : polygons[type])
				result |= where_collision(this, polygon);

		if (controll_lock)
		{
			// �ړ��O�̍��W�Ɗg�k����O�̃Y�[�����x���ƌ��݂̃Y�[�����x�����犄��o�������m�������Ă����D
			x = when_locked_coords.x * (zoom_level.w / before_zoom_level.w);
			y = when_locked_coords.y * (zoom_level.h / before_zoom_level.h);
		}
		else
		{
			auto vector = D3DXVECTOR2(0, 0); // ������ړ���������������

			// ���܂ꔻ��
			if (result & HitLine::BOTTOM && result & HitLine::TOP && result & HitLine::LEFT && result & HitLine::RIGHT)
			{
				kill(DeadReason::Sandwiched);
			}

			// �g�O��������
			if (y < -50)
			{
				kill(DeadReason::Falling);
			}

			// �ڒn����
			if (result & HitLine::BOTTOM)
			{
				ground = true;
			}
			else
			{
				ground = false;
			}

			if (!(result & HitLine::LEFT) && (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW))) // �������ւ̈ړ�
			{
				vector.x -= speed;
				vec = Player::Vec::LEFT;
			}
			if (!(result & HitLine::RIGHT) && (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW))) // �E�����ւ̈ړ�
			{
				vector.x += speed;
				vec = Player::Vec::RIGHT;
			}

#ifdef _DEBUG
			if (GetKeyboardPress(DIK_W))
			{
				vector.y += 5;
			}
			if (GetKeyboardPress(DIK_S))
			{
				vector.y -= 5;
			}
#endif

			// TODO: �W�����v�ʂƃW�����v���Ă��鎞�Ԃ𒲐�����K�v�A��
			if (!(result & HitLine::TOP) && jumping)
			{
				if (timeGetTime() - jumped_at > PLAYER_JUMP_TIME) jumping = false;
				vector.y += PLAYER_JUMP_POWER;
			}

			if(result & HitLine::TOP)
			{
				jumping = false;
			}

			// TODO: ���l�ɗ������x����������K�v������
			unless(ground)
			{
				vector.y -= PLAYER_FALLING;
			}

			// �ύX�����Z���ďI��
			x += vector.x;
			y += vector.y;
		}
		
		latest_update = current;
	}
}

bool Player::jump()
{
	unless(ground) return false;
	if (controll_lock) return false;
	ground = false;
	jumped_at = timeGetTime();
	jumping = true;
	return true;
}

void Player::lock()
{
	controll_lock = true;
	when_locked_coords = { x, y };
	before_zoom_level = zoom_level;
}

void Player::unlock()
{
	controll_lock = false;
}

void Player::kill(const DeadReason & _dead_reason)
{
#ifndef _WITHOUT_DEATH
	dead_reason = _dead_reason;
#endif
}

Player::DeadReason Player::dead()
{
	return dead_reason;
}

// === Player END ===