#include "player.h"
#include "collision_checker.h"

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 * _camera, std::map<enum PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh) : polygons(_polygons)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	tex = _tex;
	u = _u;
	v = _v;
	uw = _uw;
	vh = _vh;
	drawing = false;
	status = true;
	speed = 0.5f;
	angle = 0.0f;
	layer = _layer;
	camera = _camera;
	ground = false;
	controll_lock = false;
}

// �f�X�g���N�^
Player::~Player()
{
	// ���ɂȂ�
}

void Player::update()
{
	if (!status) return; // status�݂Đ؂�

	DWORD current = timeGetTime();

	std::vector<SquarePolygonBase*> to_check_polygons;
	for (const auto& type : { PLAIN, SCALABLE_OBJECT }) to_check_polygons.insert(to_check_polygons.end(), polygons[type].begin(), polygons[type].end());

	auto old_pos = D3DXVECTOR2(x, y);

	// ����
	if (current - latest_update > 1) // 1ms�Ԋu��
	{
		unless(controll_lock)
		{
			if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW)) // �������ւ̈ړ�
			{
				x -= speed;
			}
			if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW)) // �E�����ւ̈ړ�
			{
				x += speed;
			}
		}


		// TODO: �W�����v�ʂƃW�����v���Ă��鎞�Ԃ𒲐�����K�v�A��
		if (jumping)
		{
			if (timeGetTime() - jumped_at > 500) jumping = false;
			y += 1.0f;
		}

		// TODO: ���l�ɗ������x����������K�v������
		unless(ground)
		{
			y -= 0.5f;
		}

#ifdef _DEBUG
		if (GetKeyboardPress(DIK_W))
		{
			y += 1;
		}
		if (GetKeyboardPress(DIK_S))
		{
			y -= 1;
		}
#endif

		// �����萸��

		// TODO: ������Ɠo��₪��
		// TODO: �A�������|���S���̏�Ŋ���Ȃ��Ƃ������Ȃ�Ƃ������A��
		ground = false;
		for (const auto& polygon : to_check_polygons)
		{
			char result = where_collision(this, polygon);
			if (result & LEFT)
			{
				auto square = polygon->get_square();
				x = square.x + square.w / 2 + w / 2;
			}
			if (result & RIGHT)
			{
				auto square = polygon->get_square();
				x = square.x - square.w / 2 - w / 2;
			}
			if (result == BOTTOM)
			{
				ground = true;
				auto square = polygon->get_square();
				y = square.y + square.h / 2 + h / 2;
			}
			if (result == TOP)
			{
				if (jumping) // �W�����v���ē��Ԃ����ꍇ�͂�������
				{
					jumping = false; // ���Ԃ������_�ŃW�����v����
					auto square = polygon->get_square();
					y = square.y - square.h / 2 - h / 2;
				}
				else if (ground) // ���̏ꍇ�͊g�k�ŏk�񂾃|���S���ɉ����ׂ��Ă���̂ŁC
				{
					// �����Ŏ��S�������Ȃ��Ƃ����Ȃ�
#ifdef _DEBUG
					std::cout << "�O�G�[���܂ꂽ���S\n";
#endif
				}
			}
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
}

void Player::unlock()
{
	controll_lock = false;
}

// === Player END ===