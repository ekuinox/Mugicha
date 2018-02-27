#include "player.h"
#include "collision_checker.h"

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh)
	: polygons(_polygons), PlainSquarePolygon(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), before_zoom_level(1, 1)
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

	auto current = timeGetTime();

	// ����
	if (current - latest_update > 1) // 1ms�Ԋu��
	{
		// �����萸��
		char result = 0x00;
		float ground_height = y;
		for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR, SquarePolygonBase::PolygonTypes::THORNS, })
		{
			for (const auto& polygon : polygons[type])
			{
				auto _result = where_collision(this, polygon);
				if (_result & BOTTOM)
				{
					if (ground_height < polygon->get_square().top())
					{
						ground_height = (polygon->get_square().top() + h / 2) * (zoom_level.h / before_zoom_level.h);
					}
				}
				result |= _result;
			}
		}

		if (controll_lock)
		{
			// �ړ��O�̍��W�Ɗg�k����O�̃Y�[�����x���ƌ��݂̃Y�[�����x�����犄��o�������m�������Ă����D
			x = when_locked_coords.x * (zoom_level.w / before_zoom_level.w);
			y = when_locked_coords.y * (zoom_level.h / before_zoom_level.h);

			// ���܂ꔻ��
			if ((result & HitLine::BOTTOM && result & HitLine::TOP) || (result & HitLine::LEFT && result & HitLine::RIGHT))
			{
				// ���܂�Ƃ�₪�I�I�I�I
			}
			if (result & HitLine::BOTTOM)
			{
				y = ground_height;
			}
		}
		else
		{
			auto vector = D3DXVECTOR2(0, 0); // ������ړ���������������

			// ���܂ꔻ��
			if ((result & HitLine::BOTTOM && result & HitLine::TOP) || (result & HitLine::LEFT && result & HitLine::RIGHT))
			{
				// ���܂�Ƃ�₪�I�I�I�I
			}

			// �g�O��������
			if (y < -50)
			{
				// ���ʁ`�`
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
			}
			if (!(result & HitLine::RIGHT) && (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW))) // �E�����ւ̈ړ�
			{
				vector.x += speed;
			}

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

void Player::kill()
{
	alive = false;
#ifdef _PLAYER_DEBUG
	std::cout << "���̖{�Ԃ�����玀��ǂ�������ȁ`�`�I" << std::endl;
#endif
}

bool Player::dead()
{
	return !alive;
}

// === Player END ===