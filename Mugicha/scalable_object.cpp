#include "scalable_object.h"

ScalableObject::ScalableObject()
{
	// ����̃R���X�g���N�^���Ă��
}

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
ScalableObject::ScalableObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 *_camera, float _u, float _v, float _uw, float _vh)
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
	zoom_level = { 1, 1 }; // 1�{�ŏ�����
	layer = _layer;
	camera = _camera;
}

// �f�X�g���N�^
ScalableObject::~ScalableObject()
{
	// ���ɂȂ�
}

// �X�V����
void ScalableObject::update()
{
	// status���L���ɂȂ��Ă��Ȃ���Δ����܂�
	if (!status) return;

	// �O��̍X�V����1ms�o���Ă��Ȃ���΍s��Ȃ�
	DWORD current = timeGetTime();
	if (current - latest_update > 1)
	{		
		latest_update = current;
	}

}

// ���W�ƃT�C�Y����vertexes�𐶐����܂�
void ScalableObject::generate_vertexes()
{
	// �����ɂ��낢���肪����
	
	auto zoomed_coords = D3DXVECTOR2(x * zoom_level.w, y * zoom_level.h);
	POLSIZE zoomed_size = { w * zoom_level.w, h * zoom_level.h };

	drawing_coord.x = zoomed_coords.x - (camera->x - SCREEN_WIDTH / 2);
	drawing_coord.y = (zoomed_coords.y - (camera->y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;

	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = {
			drawing_coord.x + zoomed_size.w / (i % 3 == 0 ? -2 : 2),
			drawing_coord.y + zoomed_size.h / (i < 2 ? -2 : 2),
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

VERTEX_2D * ScalableObject::get_vertexes()
{
	return vertexes;
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