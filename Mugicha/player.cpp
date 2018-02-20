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
}

// �f�X�g���N�^
Player::~Player()
{
	// ���ɂȂ�
}

void Player::update()
{
	// TODO: �����蔻�莩�̂͂ł��Ă��邪�C�����蔻���p���Ă��񂽂�͂ł��Ă��Ȃ�
	//���ꂪ�������킩��Ȃ����ǁCx,y�����ɉ����Z���邽�т�generate_vertex���āCis_collison�����ƂƂ肠�����ʂ�
	if (!status) return; // status�݂Đ؂�

	DWORD current = timeGetTime();

	std::vector<SquarePolygonBase*> to_check_polygons;
	for (const auto& type : { PLAIN, SCALABLE_OBJECT }) to_check_polygons.insert(to_check_polygons.end(), polygons[type].begin(), polygons[type].end());

	auto old_pos = D3DXVECTOR2(x, y);

	// ����
	if (current - latest_update > 1) // 1ms�Ԋu��
	{
		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW)) // �������ւ̈ړ�
		{
			x -= speed;
		}
		else if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW)) // �E�����ւ̈ړ�
		{
			x += speed;
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

		// �����萸��
		for (const auto& polygon : to_check_polygons)
		{
			char result = where_collision(this, polygon);
			if (result & BOTTOM)
			{
				y = old_pos.y;
				ground = true;
#ifdef _DEBUG
			//	printf("%d\n", result);
#endif
			}
			if (result & TOP)
			{
				y = old_pos.y;
			}
			if (result & LEFT)
			{
				x = old_pos.x;
			}
			if (result & RIGHT)
			{
				x = old_pos.x;
			}

			/*
			switch (result)
			{
			case TOP:
				y = old_pos.y;
				break;
			case BOTTOM:
				y = old_pos.y;
				ground = true;
				break;
			case LEFT:
				x = old_pos.x;
				break;
			case RIGHT:
				x = old_pos.x;
				break;
			}
			if (result == BOTTOM) ground = false;
			*/
		}

		latest_update = current;
	}
}

bool Player::jump()
{
	unless (ground) return false; 
	ground = false;
	jumped_at = timeGetTime();
	jumping = true;
	return true;
}

// === Player END ===