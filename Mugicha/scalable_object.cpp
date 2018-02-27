#include "scalable_object.h"

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
ScalableObject::ScalableObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, float _u, float _v, float _uw, float _vh)
	: PlainSquarePolygon(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), zoom_level(1, 1)
{
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
	auto current = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_update).count() > 1)
	{		
		latest_update = current;
	}

}

SQUARE ScalableObject::get_square()
{
//	return SQUARE(x * zoom_level.w, y * zoom_level.w, w * zoom_level.w, h * zoom_level.h);
	return SQUARE(x, y, w, h) * zoom_level;
}

// ���W�ƃT�C�Y����vertexes�𐶐����܂�
void ScalableObject::generate_vertexes()
{
	auto zoomed_coords = D3DXVECTOR2(x * zoom_level.w, y * zoom_level.h);
	POLSIZE zoomed_size = { w * zoom_level.w, h * zoom_level.h };

	drawing_coord.x = zoomed_coords.x - (camera.x - SCREEN_WIDTH / 2);
	drawing_coord.y = (zoomed_coords.y - (camera.y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;

	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = VERTEX_2D(
			drawing_coord.x + zoomed_size.w / (i % 3 == 0 ? -2 : 2),
			drawing_coord.y + zoomed_size.h / (i < 2 ? -2 : 2),
			this->u + (i % 3 == 0 ? 0 : this->uw),
			this->v + (i < 2 ? 0 : this->vh)
		);
	}
}

void ScalableObject::zoom(POLSIZE _zoom_level)
{
	zoom_level = _zoom_level;
}
// === ScalableObject END ===