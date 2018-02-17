#include "player.h"

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
Player::Player(LPDIRECT3DTEXTURE9 _tex, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh)
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
		if (GetKeyboardTrigger(DIK_SPACE))
		{
			jumping = true;
 			direction = (direction + 90) / 2;
		}

		if (moving)
		{
			x += cos(D3DXToRadian(direction)) * speed;
			y -= sin(D3DXToRadian(direction)) * speed;
			if (x > SCREEN_WIDTH)
			{
				x = SCREEN_WIDTH;
			}
			if (x < 0)
			{
				x = 0;
			}

		}

		if (jumping)
		{
			direction -= 0.1f;
			if (direction < 0) jumping = false;
		}
		latest_update = current;

	}

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

// drawing�t���O�������ŕύX����
void Player::switch_drawing(bool _drawing)
{
	drawing = _drawing;
}

// drawing�t���O�̔��]���s��
void Player::switch_drawing()
{
	drawing = !drawing;
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
		this->x - this->w / 2 <= pol->x + pol->w / 2 // ���ƉE
		&& this->x + this->w / 2 >= pol->x - pol->w // �E�ƍ�
		&& this->y - this->h / 2 <= pol->y + pol->h / 2 // ��Ɖ�
		&& this->y + this->h / 2 >= pol->y - pol->h / 2 // ���Ə�
		? true : false;
}

void Player::switch_status(bool _status)
{
	status = _status;
}

bool Player::is_active()
{
	return status;
}

// ���W�ƃT�C�Y����vertexes�𐶐����܂�
void Player::generate_vertexes()
{
#ifdef __ENABLE_PLAYER_ROTATION
	D3DXMATRIX matRot;
	D3DXMatrixRotationZ(&matRot, angle);
#endif
	for (auto i = 0; i < 4; ++i)
	{
#ifdef __ENABLE_PLAYER_ROTATION
		D3DXVECTOR3 srcVec(this->x + this->w / (i % 3 == 0 ? -2 : 2), this->y + this->h / (i < 2 ? -2 : 2), 0), destVec;
		D3DXVec3TransformCoord(&destVec, &srcVec, &matRot);
#endif
		vertexes[i] = {
#ifdef __ENABLE_PLAYER_ROTATION
			destVec.x + this->x + this->w / (i % 3 == 0 ? -2 : 2),
			destVec.y + this->y + this->h / (i < 2 ? -2 : 2),
#else
			this->x + this->w / (i % 3 == 0 ? -2 : 2),
			this->y + this->h / (i < 2 ? -2 : 2),
#endif
			0.0f,
			1.0f,
			D3DCOLOR_RGBA(255, 255, 255, 200),
			this->u + (i % 3 == 0 ? 0 : this->uw),
			this->v + (i < 2 ? 0 : this->vh)

		};
	}
}

// === Player END ==="