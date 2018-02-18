#include "scalable_object.h"

/*
* ScalableObject
* SquarePolygonBase���p�������ȒP�ȃ|���S���N���X
*/

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
ScalableObject::ScalableObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, char _scaling_dir, int _layer, D3DXVECTOR2 *_camera, float _u, float _v, float _uw, float _vh)
{
	x = _x;
	y = _y;
	drawing_coord.x = x;
	drawing_coord.y = y * -1 + SCREEN_HEIGHT;
	w = _w;
	h = _h;
	tex = _tex;
	u = _u;
	v = _v;
	uw = _uw;
	vh = _vh;
	drawing = false;
	scaling_dir = _scaling_dir;
	zoom_level = { 1, 1 }; // 1�{�ŏ�����
	scaling_base.x = drawing_coord.x + w / (scaling_dir % 3 == 0 ? 2 : -2); // ���ɑ傫������Ȃ�base���E�ɁC�t���܂�������
	scaling_base.y = drawing_coord.y + h / (scaling_dir < 2 ? 2 : -2); // ��ɑ傫������Ȃ�base�����ɁC�t���܂�
	layer = _layer;
	camera = _camera;
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
		latest_update = current;
	}
}

// ���W�ƃT�C�Y����vertexes�𐶐����܂�
void ScalableObject::generate_vertexes()
{
	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = {
			[&]() {
				if (scaling_dir % 3 == 0) // dir�����̏ꍇ�Cbase�͉E�ɂ���
				{
					return (x * -zoom_level.w + w / (i % 3 == 0 ? 2 : -2) * zoom_level.w) - (camera->x - SCREEN_WIDTH / 2);
				}
				else // dir���E�̏ꍇ�Cbase�͍��ɂ���
				{
					return (x * zoom_level.w + w / (i % 3 == 0 ? 2 : -2) * zoom_level.w) - (camera->x - SCREEN_WIDTH / 2);
				}
			}(),
			[&]() {
				if (scaling_dir < 2) // dir����̏ꍇ�Cbase�͉��ɂ���
				{
					return (y * -zoom_level.h + h / (i < 2 ? 2 : -2) * zoom_level.h) - (camera->y - SCREEN_HEIGHT / 2);
				}
				else // dir�����̏ꍇ�Cbase�͏�ɂ���
				{
					return (y * zoom_level.h + h / (i < 2 ? 2 : -2) * zoom_level.h) - (camera->y - SCREEN_HEIGHT / 2);
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
		this->x - this->w / 2 <= pol->get_coords().x + pol->get_size().w / 2 // ���ƉE
		&& this->x + this->w / 2 >= pol->get_coords().x - pol->get_size().w // �E�ƍ�
		&& this->y - this->h / 2 <= pol->get_coords().y + pol->get_size().h / 2 // ��Ɖ�
		&& this->y + this->h / 2 >= pol->get_coords().y - pol->get_size().h / 2 // ���Ə�
		? true : false;
}

D3DXVECTOR2 ScalableObject::get_coords()
{
	return D3DXVECTOR2(x, y);
}

POLSIZE ScalableObject::get_size()
{
	return {w, h};
}

void ScalableObject::add_coord(float _x, float _y)
{
	
}

void ScalableObject::zoom(POLSIZE _zoom_level)
{
	zoom_level = _zoom_level;
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