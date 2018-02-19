#include "player.h"
#include "collision_checker.h"

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 *_camera, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh)
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
	speed = 0.5f;
	status = true;
	angle = 0.0f;
	layer = _layer;
	camera = _camera;
}

// �f�X�g���N�^
Player::~Player()
{
	// ���ɂȂ�
}

void Player::update()
{
	// TODO: �W�����v�̏������I����Ă��Ȃ��̂ł��Ȃ��̂����Ȃ�
	// TODO: �����蔻�莩�̂͂ł��Ă��邪�C�����蔻���p���Ă��񂽂�͂ł��Ă��Ȃ�

	if (!status) return; // status�݂Đ؂�

	DWORD current = timeGetTime();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_0))
	{
		std::cout << "�҂傢�`��" << std::endl;
	}
#endif

	// ����
	if (current - latest_update > 1) // 1ms�Ԋu��
	{
		bool moving = false;
		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW)) // �������ւ̈ړ�
		{
			direction = 180;
			moving = true;
		}
		else if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW)) // �E�����ւ̈ړ�
		{
			direction = 0;
			moving = true;
		}

#ifdef _DEBUG
		else if (GetKeyboardPress(DIK_W))
		{
			y += 1;
		}
		else if (GetKeyboardPress(DIK_S))
		{
			y -= 1;
		}
#endif

		if (moving)
		{
			x += cos(D3DXToRadian(direction)) * speed;
			y -= sin(D3DXToRadian(direction)) * speed;

		}

		if (jumping)
		{
			direction -= 0.1f;
			if (direction < 0) jumping = false;
		}

		// �g�O�ɏo�Ȃ��悤�� => �ǂ̓����蔻��ł�肽��
		if (x - w / 2 < 0) x = w / 2;
		if (y - h / 2 < 0) y = h / 2;

		latest_update = current;

	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		jumping = true;
		direction = (direction + 90) / 2;
	}

	drawing_coord.x = x - (camera->x - SCREEN_WIDTH / 2);
	drawing_coord.y = (y - (camera->y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;
}

// === Player END ===