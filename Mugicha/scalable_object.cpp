#include "scalable_object.h"

/*
* ScalableObject
* SquarePolygonBase���p�������ȒP�ȃ|���S���N���X
*/

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
ScalableObject::ScalableObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, char _scaling_dir, int _layer, float _u, float _v, float _uw, float _vh)
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
	aspect_ratio = h / w;
	drawing = false;
	scaling_dir = _scaling_dir;
	scaling_base.x = x + w / (scaling_dir % 3 == 0 ? 2 : -2); // ���ɑ傫������Ȃ�base���E�ɁC�t���܂�������
	scaling_base.y = y + h / (scaling_dir < 2 ? 2 : -2); // ��ɑ傫������Ȃ�base�����ɁC�t���܂�
	layer = _layer;
}

// �f�X�g���N�^
ScalableObject::~ScalableObject()
{
	// ���ɂȂ�
}

void ScalableObject::update()
{
	// ��
	if (!status) return;

	DWORD current = timeGetTime();

	// ����
	if (current - latest_update > 1)
	{
		// �g�k
		
		if (GetKeyboardPress(DIK_NUMPAD8)) // �g��
		{
			w *= 1.01f;
			h *= 1.01f;

			scaling_base.x *= 1.01f;
			scaling_base.y /= 1.01f;
		}
		else if (GetKeyboardPress(DIK_NUMPAD2)) // �k��
		{
			w /= 1.01f;
			h /= 1.01f;

			scaling_base.x /= 1.01f;
			scaling_base.y *= 1.01f;
		}
		
		if (GetKeyboardTrigger(DIK_O)) // �g��
		{
			w *= 2;
			h *= 2;

			scaling_base.x *= 2;
			scaling_base.y /= 2;
		}
		else if (GetKeyboardTrigger(DIK_L)) // �k��
		{
			w /= 2;
			h /= 2;

			scaling_base.x /= 2;
			scaling_base.y *= 2;
		}


		//  ���E����
		if (w < 1) w = 1;
		if (h < 1) h = 1;

		latest_update = current;
	}
}

// ���W�ƃT�C�Y����vertexes�𐶐����܂�
void ScalableObject::generate_vertexes()
{
	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = {
//			this->x + this->w / (i % 3 == 0 ? -2 : 2),
			[&]() {
				if (scaling_dir % 3 == 0) // dir�����̏ꍇ�Cbase�͉E�ɂ���
				{
					return scaling_base.x - (i % 3 == 0 ? w : 0);
				}
				else // dir���E�̏ꍇ�Cbase�͍��ɂ���
				{
					return scaling_base.x + (i % 3 == 0 ? w : 0);
				}
			}(),
//			this->y + this->h / (i < 2 ? -2 : 2),
			[&]() {
				if (scaling_dir < 2) // dir����̏ꍇ�Cbase�͉��ɂ���
				{
					return scaling_base.y - (i < 2 ? h : 0);
				}
				else // dir�����̏ꍇ�Cbase�͏�ɂ���
				{
					return scaling_base.y + (i < 2 ? h : 0);
				}
			}(),
			0.0f,
			1.0f,
			D3DCOLOR_RGBA(255, 255, 255, 200),
			this->u + (i % 3 == 0 ? 0 : this->uw),
			this->v + (i < 2 ? 0 : this->vh)

		};
	}
}

void ScalableObject::draw()
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
bool ScalableObject::is_drawing()
{
	return drawing;
}

void ScalableObject::show()
{
	drawing = true;
}

void ScalableObject::hide()
{
	drawing = false;
}

// �e�N�X�`���̕ύX
void ScalableObject::change_texture(LPDIRECT3DTEXTURE9 _tex)
{
	tex = _tex;
}

// �����蔻������
bool ScalableObject::is_collision(SquarePolygonBase *pol)
{
	return
		this->x - this->w / 2 <= pol->x + pol->w / 2 // ���ƉE
		&& this->x + this->w / 2 >= pol->x - pol->w // �E�ƍ�
		&& this->y - this->h / 2 <= pol->y + pol->h / 2 // ��Ɖ�
		&& this->y + this->h / 2 >= pol->y - pol->h / 2 // ���Ə�
		? true : false;
}

bool ScalableObject::is_active()
{
	return status;
}

void ScalableObject::enable()
{
	status = true;
}

void ScalableObject::disable()
{
	status = false;
}

// === ScalableObject END ===