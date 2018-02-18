#include "player.h"

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

		latest_update = current;

	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		jumping = true;
		direction = (direction + 90) / 2;
	}

	drawing_coord.x = x - (camera->x - SCREEN_WIDTH / 2);
	drawing_coord.y = (y - (camera->y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;

#ifdef _DEBUG
	printf("PLAYER: %f, %f\n", x, y);
#endif
}

void Player::draw()
{
	if (!drawing) return; // �t���O����

	generate_vertexes();
	d3d_device->SetTexture(0, tex);
	d3d_device->SetFVF(FVF_VERTEX_2D);

	d3d_device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		sizeof(this->vertexes) / sizeof(VERTEX_2D) - 2, // �|���S����
		this->vertexes,
		sizeof(VERTEX_2D)
	);
}

// drawing�t���O��Ԃ�
bool Player::is_drawing()
{
	return drawing;
}

void Player::show()
{
	drawing = true;
}

void Player::hide()
{
	drawing = false;
}

// �e�N�X�`���̕ύX
void Player::change_texture(LPDIRECT3DTEXTURE9 _tex)
{
	tex = _tex;
}

// �����蔻������
bool Player::is_collision(SquarePolygonBase *pol)
{
	return
		this->x - this->w / 2 <= pol->get_coords().x + pol->get_size().w / 2 // ���ƉE
		&& this->x + this->w / 2 >= pol->get_coords().x - pol->get_size().w // �E�ƍ�
		&& this->y - this->h / 2 <= pol->get_coords().y + pol->get_size().h / 2 // ��Ɖ�
		&& this->y + this->h / 2 >= pol->get_coords().y - pol->get_size().h / 2 // ���Ə�
		? true : false;
}

D3DXVECTOR2 Player::get_coords()
{
	return D3DXVECTOR2(x, y);
}

POLSIZE Player::get_size()
{
	return { w, h };
}

void Player::add_coord(float _x, float _y)
{
	// �O������͑��삳���Ȃ����I
}

void Player::zoom(POLSIZE _zoom_level)
{
	// �����Ȃ�
}

bool Player::is_active()
{
	return status;
}

void Player::enable()
{
	status = true;
}

void Player::disable()
{
	status = false;
}

// ���W�ƃT�C�Y����vertexes�𐶐����܂�
void Player::generate_vertexes()
{
	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = {
			drawing_coord.x + this->w / (i % 3 == 0 ? -2 : 2),
			drawing_coord.y + this->h / (i < 2 ? -2 : 2),
			0.0f,
			1.0f,
			D3DCOLOR_RGBA(255, 255, 255, 200),
			this->u + (i % 3 == 0 ? 0 : this->uw),
			this->v + (i < 2 ? 0 : this->vh)
		};
	}
}

// === Player END ==="