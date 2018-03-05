#include "collision_checker.h"
#include "player.h"
#include "thorn.h"
#include "bullet.h"
#include "gimmick_floor.h"
#include "keyconf.h"

bool Player::collision_for_enemies()
{
	// �G�Ƃ̓����蔻��
	for (const auto& _enemy : polygons[SquarePolygonBase::PolygonTypes::ENEMY])
	{
		const auto& enemy = static_cast<Enemy*>(_enemy);
		if (enemy->is_alive() && is_collision(enemy->get_square(), get_square()))
		{
			if (zoom_level >= 1.0f)
			{
				// �v���C���̕���
				kill(DeadReason::HitEnemy);
				return true;
			}
			else
			{
				// �G�̕���
				enemy->kill();
			}
		}
	}
	return false;
}

bool Player::collision_for_thorns()
{
	// �g�Q�Ƃ̓����蔻��
	// �ʏ��Ԃ�莩�����f�J����ԂŐڐG���̔�������
	if (zoom_level > 1.0f) return false;
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
		}

		switch (static_cast<Thorn*>(thorn)->get_vec())
		{
		case Thorn::Vec::UP:
			if (hit_bottom(self, another))
			{
				kill(DeadReason::HitThorn);
				return true;
			}
			break;
		case Thorn::Vec::DOWN:
			if (hit_top(self, another))
			{
				kill(DeadReason::HitThorn);
				return true;
			}
			break;
		case Thorn::Vec::RIGHT:
			if (hit_left(self, another))
			{
				kill(DeadReason::HitThorn);
				return true;
			}
			break;
		case Thorn::Vec::LEFT:
			if (hit_right(self, another))
			{
				kill(DeadReason::HitThorn);
				return true;
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
			kill(DeadReason::HitMagma);
			return true;
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
			kill(DeadReason::Shot);
			return true;
		}
	}
	return false;
}

D3DXVECTOR2 Player::collision_for_knockback_bullets()
{
	auto knockback = D3DXVECTOR2(0, 0);

	auto self = get_square();
	
	for (const auto& _bullet : polygons[SquarePolygonBase::PolygonTypes::KNOCKBACK_BULLET])
	{
		const auto& bullet = static_cast<Bullet*>(_bullet);

		// ���˂���Ă��Ȃ���Ζ���
		if (bullet->is_triggered())
		{
			auto another = bullet->get_square();
			auto vec = bullet->get_vec();
			auto result = where_collision(another, self, 0.0f);

			if (result & HitLine::TOP && vec == Bullet::Vec::UP)
			{
				knockback.y += KNOCKBACK_VOLUME;
				bullet->init();
			}
			else if (result & HitLine::BOTTOM && vec == Bullet::Vec::DOWN)
			{
				knockback.y -= KNOCKBACK_VOLUME;
				bullet->init();
			}
			else if (result & HitLine::LEFT && vec == Bullet::Vec::LEFT)
			{
				knockback.x -= KNOCKBACK_VOLUME;
				bullet->init();
			}
			else if (result & HitLine::RIGHT && vec == Bullet::Vec::RIGHT)
			{
				knockback.x += KNOCKBACK_VOLUME;
				bullet->init();
			}
		}
	}
	return knockback;
}

void Player::collision_for_knockback_bullets(D3DXVECTOR2 & vector, char & result)
{
	auto knockback = collision_for_knockback_bullets();

	if ((knockback.x < 0 && !(result & HitLine::LEFT)) || knockback.x > 0 && !(result & HitLine::RIGHT))
	{
		vector.x += knockback.x;
	}

	// �㉺
	if ((knockback.y < 0 && !(result & HitLine::BOTTOM)) || knockback.y > 0 && !(result & HitLine::TOP))
	{
		vector.y += knockback.y;
	}
}

bool Player::is_fallen_hellgate()
{
	auto self = get_square();
	self.h *= 0.9; // �Â�
	if (
		polygons[PlainSquarePolygon::PolygonTypes::HELLGATE].front()->is_drawing()
		&& is_collision(self, polygons[PlainSquarePolygon::PolygonTypes::HELLGATE].front()->get_square())
		)
	{
		// �v���C���̕���
		kill(DeadReason::FallenHellGate);
		return true;
	}
	return false;
}

char Player::collision_check()
{
	// �����萸��
	char result = 0x00;
	for (const auto& type : COLLISION_CHECK_POLYGONTYPES)
		for (const auto& polygon : polygons[type])
			result |= where_collision(this, polygon);

	// GIMMICK_FLOOR����appearing������
	for (const auto& polygon : polygons[SquarePolygonBase::PolygonTypes::GIMMICK_FLOOR])
		if(static_cast<GimmickFloor*>(polygon)->is_appearing())
			result |= where_collision(this, polygon);

	return result;
}

void Player::sandwich_check(char &result)
{
	// ���܂ꔻ��
	if (result & HitLine::BOTTOM && result & HitLine::TOP && result & HitLine::LEFT && result & HitLine::RIGHT)
	{
		kill(DeadReason::Sandwiched);
	}
}

void Player::falling_out_check(char &result)
{
	if (y < FALLING_OUT_Y)
	{
		kill(DeadReason::Falling);
	}

}

void Player::ground_check(char &result)
{
	if (result & HitLine::BOTTOM)
	{
		ground = true;
	}
	else
	{
		ground = false;
	}
}

void Player::head_check(char & result)
{
	if (result & HitLine::TOP) jumping = false;
}

void Player::controlls(D3DXVECTOR2 & vector, char & result)
{
	vec = Player::Vec::CENTER;

	// �������ւ̈ړ�
	if (!(result & HitLine::LEFT) && PLAYER_MOVE_LEFT)
	{
		vector.x -= speed;
		vec = Player::Vec::LEFT;
		old_vec = vec;
	}
	
	// �E�����ւ̈ړ�
	if (!(result & HitLine::RIGHT) && PLAYER_MOVE_RIGHT)
	{
		vector.x += speed;
		vec = Player::Vec::RIGHT;
		old_vec = vec;
	}

#if defined(_DEBUG) || defined(_STAGE_DEBUG) // ���������s�����Ⴄ���f�o�b�O���[�h
	if (GetKeyboardPress(DIK_UPARROW)) vector.y += 5;
	if (GetKeyboardPress(DIK_DOWNARROW)) vector.y -= 5;
	if (GetKeyboardPress(DIK_LEFTARROW)) vector.x -= 5;
	if (GetKeyboardPress(DIK_RIGHTARROW)) vector.x += 5;
#endif
}

void Player::jump(D3DXVECTOR2 & vector, char & result)
{
	if (!(result & HitLine::TOP) && jumping)
	{
		auto diff = y - jumped_at;
		if (diff < PLAYER_JUMP_HEIGHT || (PLAYER_JUMP_HOLD && diff < PLAYER_HOLD_JUMP_HEIGHT)) vector.y += PLAYER_JUMP_POWER;
		else jumping = false;
	}
}

void Player::drifting(D3DXVECTOR2 & vector)
{
	unless(ground) vector.y -= PLAYER_FALLING;
}

bool Player::catch_item()
{
	for (const auto& _item : polygons[SquarePolygonBase::PolygonTypes::ITEM])
	{
		if (static_cast<Item*>(_item)->hold(get_square()))
		{
			item = static_cast<Item*>(_item);
			return true;
		}
	}

	return false;
}

void Player::release_item()
{
	item->release();
	item = nullptr;
}

bool Player::is_holding_item()
{
	return item != nullptr;
}

void Player::generate_vertexes()
{
	// u�l��ς��Ă�肽���񂾂�
	if (vec != Player::Vec::CENTER)
	{
		u += uw * (vec == Player::Vec::RIGHT ? 1 : -1);
	}

	PlainSquarePolygon::generate_vertexes();

	if ((vec == Player::Vec::CENTER && old_vec == Player::Vec::LEFT) || vec == Player::Vec::LEFT)
	{
		if (old_vec == Player::Vec::LEFT)
		{
			for (auto i = 0; i < 4; ++i)
			{
				vertexes[i].u = u + (i % 3 == 0 ? uw : 0);
			}
		}
	}
}

Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, PolygonsContainer & _polygons, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh)
	: PlainSquarePolygon(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh),
	polygons(_polygons), before_zoom_level(1.0f), dead_reason(DeadReason::ALIVE),
	vec(Player::Vec::CENTER), item(nullptr), jumping(false), jumped_at(_y), old_vec(Player::Vec::CENTER)
{
	init();
}

Player::~Player()
{
	delete audiocontroller;
}

void Player::init()
{
	speed = PLAYER_SPEED;
	ground = false;
	controll_lock = false;

	audiocontroller = new AudioController({
	//	{ "WALK_01",{ AUDIOS_DIR "���[�t�@�[.wav", false } },		
		});
}

void Player::zoom(float _zoom_level)
{
	zoom_level = _zoom_level;
}

void Player::update()
{
	// status�݂Đ؂�
	unless (status) return; 

	if (
		collision_for_enemies() // �G�Ƃ̓����蔻��
		|| collision_for_thorns() // �g�Q�Ƃ̓����蔻��
		|| collision_for_magmas() // �}�O�}�Ƃ̓����蔻��
		|| collision_for_bullets() // �e�ۂƂ̓����蔻��
		|| is_fallen_hellgate() // ������オ���Ă���A���Ƃ̓����蔻��
		)
	{
#ifndef _NEVER_DIE
		return;
#endif
	}
		
	if (controll_lock)
	{
		// �ړ��O�̍��W�Ɗg�k����O�̃Y�[�����x���ƌ��݂̃Y�[�����x�����犄��o�������m�������Ă����D
		x = when_locked_coords.x * (zoom_level / before_zoom_level);
		y = when_locked_coords.y * (zoom_level / before_zoom_level);

		// �����萸��
		auto self = get_square();
		bool run = true;
		for (const auto& type : COLLISION_CHECK_POLYGONTYPES)
		{
			for (const auto& polygon : polygons[type])
			{
				auto sq = polygon->get_square();
				if (hit_bottom(self, sq))
				{
					y += CELL_HEIGHT * zoom_level; // �ӂ���Ƃ����Ă��
					run = false;
					break;
				}
			}
			unless(run) break;
		}

		// GIMMICK_FLOOR����appearing������
		for (const auto& polygon : polygons[SquarePolygonBase::PolygonTypes::GIMMICK_FLOOR])
		{
			if (static_cast<GimmickFloor*>(polygon)->is_appearing())
			{
				auto sq = polygon->get_square();
				if (hit_bottom(self, sq))
				{
					y += CELL_HEIGHT * zoom_level; // �ӂ���Ƃ����Ă��
					run = false;
					break;
				}
			}
		}
		
	}
	else
	{
		// ������ړ���������������
		auto vector = D3DXVECTOR2(0, 0);
		
		// ��Q���ȂǂƂ̓����蔻��								 
		auto result = collision_check(); 
		
		// �m�b�N�o�b�N�̂����C�C�ɂ��ē����蔻�������
		collision_for_knockback_bullets(vector, result);

		// ���܂�Ă��邩����
		sandwich_check(result);

		// �g�O��������
		falling_out_check(result);
		
		// �ڒn����
		ground_check(result);

		// �����Ԃ��Ă��Ȃ���
		head_check(result);

		// ���ԊǗ����܂�
		auto current = SCNOW;
		if (time_diff(latest_update, current) > UPDATE_INTERVAL)
		{
			latest_update = current;
			
			// ����
			controlls(vector, result);

			// �W�����v����
			jump(vector, result);

			// �����Ă�����
			drifting(vector);
		}

		// �ύX�����Z���ďI��
		x += vector.x;
		y += vector.y;
	}
		
	// item�������Ă���Ȃ�item�̈ʒu���C�����Ă�����
	if (is_holding_item()) item->move(D3DXVECTOR2(x + w / ( (vec == Player::Vec::CENTER ? old_vec : vec) == Player::Vec::RIGHT ? 2 : -2), y));
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
#ifdef _DEBUG
	switch (_dead_reason)
	{
	case DeadReason::ALIVE:
		puts(STRING(DeadReason::ALIVE));
		break;
	case DeadReason::Sandwiched:
		puts(STRING(DeadReason::Sandwiched));
		break;
	case DeadReason::HitEnemy:
		puts(STRING(DeadReason::HitEnemy));
		break;
	case DeadReason::HitThorn:
		puts(STRING(DeadReason::HitThorn));
		break;
	case DeadReason::Falling:
		puts(STRING(DeadReason::Falling));
		break;
	case DeadReason::HitMagma:
		puts(STRING(DeadReason::HitMagma));
		break;
	case DeadReason::Shot:
		puts(STRING(DeadReason::Shot));
		break;
	case DeadReason::FallenHellGate:
		puts(STRING(DeadReason::FallenHellGate));
		break;
	}
#endif
#ifndef _NEVER_DIE
	dead_reason = _dead_reason;
#endif
}

bool Player::is_jumping()
{
	return jumping;
}

void Player::trigger_controlls()
{
	// �v���C�����W�����v������
	if (!controll_lock && ground && PLAYER_JUMP)
	{
		ground = false;
		jumped_at = y;
		jumping = true;
	}

	// �v���C���ɒ͂܂����肷��
	if (PLAYER_ITEM_USE)
	{
		if (is_holding_item()) release_item();
		else catch_item();
	}
}

Player::DeadReason Player::dead()
{
	return dead_reason;
}

Player::Vec Player::get_vec()
{
	return vec;
}

SQUARE Player::get_square()
{
	// �ӂ蔼���ɂ��܂�
	auto sq = PlainSquarePolygon::get_square();
	sq.w *= 0.8;
	return sq;
}

// === Player END ===