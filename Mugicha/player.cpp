#include "player.h"
#include "collision_checker.h"

void Player::generate_vertexes()
{
	drawing_coord.x = x - (camera->x - SCREEN_WIDTH / 2);
	drawing_coord.y = (y - (camera->y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;

	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = VERTEX_2D(
			drawing_coord.x + this->w / (i % 3 == 0 ? -2 : 2),
			drawing_coord.y + this->h / (i < 2 ? -2 : 2),
			this->u + (i % 3 == 0 ? 0 : this->uw),
			this->v + (i < 2 ? 0 : this->vh)
		);
	}
}

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 * _camera, std::map<enum PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh)
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
	speed = 0.5f;
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

	DWORD current = timeGetTime();

	std::vector<SquarePolygonBase*> to_check_polygons;
	for (const auto& type : { SCALABLE_OBJECT, ENEMY, RAGGED_FLOOR, THORNS,}) to_check_polygons.insert(to_check_polygons.end(), polygons[type].begin(), polygons[type].end());

	// ����
	if (current - latest_update > 1) // 1ms�Ԋu��
	{
		if (controll_lock)
		{
			// �ړ��O�̍��W�Ɗg�k����O�̃Y�[�����x���ƌ��݂̃Y�[�����x�����犄��o�������m�������Ă����D
			x = when_locked_coords.x * (zoom_level.w / before_zoom_level.w);
			y = when_locked_coords.y * (zoom_level.h / before_zoom_level.h);
		}
		else
		{
			auto vector = D3DXVECTOR2(0, 0); // ������ړ���������������

			// �����萸��
			char result = 0x00;
			for (const auto& polygon : to_check_polygons) result |= where_collision(this, polygon);

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
				if (timeGetTime() - jumped_at > 500) jumping = false;
				vector.y += 1.0f;
			}

			// TODO: ���l�ɗ������x����������K�v������
			unless(ground)
			{
				vector.y -= 0.5f;
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