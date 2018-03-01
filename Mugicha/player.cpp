#include "collision_checker.h"
#include "player.h"
#include "thorn.h"
#include "bullet.h"

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
	for (const auto& bullet : polygons[SquarePolygonBase::PolygonTypes::BULLET])
	{
		if (is_collision(get_square(), bullet->get_square()))
		{
			bullet->init();
#ifndef _WITHOUT_DEATH
			kill(DeadReason::Shot);
			return true;
#endif
		}
	}
	return false;
}

bool Player::collision_for_knockback_bullets(D3DXVECTOR2 &knockback)
{
	auto self = get_square();
	
	for (const auto& bullet : polygons[SquarePolygonBase::PolygonTypes::KNOCKBACK_BULLET])
	{

		auto another = bullet->get_square();
		auto vec = static_cast<Bullet*>(bullet)->get_vec();
		auto result = where_collision(another, self, 0.0f);

		if (result & HitLine::TOP && vec == Bullet::Vec::UP)
		{
			knockback.y += CELL_WIDTH;
			bullet->init();
		}
		else if (result & HitLine::BOTTOM && vec == Bullet::Vec::DOWN)
		{	
			knockback.y -= CELL_WIDTH;
			bullet->init();
		}
		else if (result & HitLine::LEFT && vec == Bullet::Vec::LEFT)
		{
			knockback.x -= CELL_WIDTH;
			bullet->init();
		}
		else if (result & HitLine::RIGHT && vec == Bullet::Vec::RIGHT)
		{
			knockback.x += CELL_WIDTH;
			bullet->init();
		}


	}
	return false;
	
}

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh)
	: polygons(_polygons), PlainSquarePolygon(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), before_zoom_level(1, 1), dead_reason(DeadReason::ALIVE), vec(Player::Vec::CENTER), item(nullptr), holding_item(false)
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
		auto vector = D3DXVECTOR2(0, 0); // ������ړ���������������

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
		for (const auto& type : {
			// �����蔻������|���S���̃��x���܂Ƃ�
			SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT,
			SquarePolygonBase::PolygonTypes::RAGGED_FLOOR,
			SquarePolygonBase::PolygonTypes::THORN,
			SquarePolygonBase::PolygonTypes::AIRCANNON,
			SquarePolygonBase::PolygonTypes::GIMMICK_SWITCH,
			})
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
			// �m�b�N�o�b�N�ɂ���
			auto knockback = D3DXVECTOR2(0, 0);

			collision_for_knockback_bullets(knockback);

			// ���E
			if ((knockback.x < 0 && !(result & HitLine::LEFT)) || knockback.x > 0 && !(result & HitLine::RIGHT))
			{
				vector.x += knockback.x;
			}

			// �㉺
			if ((knockback.y < 0 && !(result & HitLine::BOTTOM)) || knockback.y > 0 && !(result & HitLine::TOP))
			{
				vector.y += knockback.y;
			}

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
		
		// item�������Ă���Ȃ�item�̈ʒu���C�����Ă�����
		if (holding_item) item->move(D3DXVECTOR2(x + w / (vec == Player::Vec::RIGHT ? 2 : -2), y));

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

bool Player::catch_item()
{
	for (const auto& _item : polygons[SquarePolygonBase::PolygonTypes::ITEM])
	{
		if (static_cast<Item*>(_item)->hold(get_square()))
		{
			item = static_cast<Item*>(_item);
			holding_item = true;
			return true;
		}
	}

	return false;
}

void Player::release_item()
{
	holding_item = false;
	item->release();
}

bool Player::is_holding_item()
{
	return holding_item;
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

Player::Vec Player::get_vec()
{
	return vec;
}

// === Player END ===