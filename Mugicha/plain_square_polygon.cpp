#include "plain_square_polygon.h"

/*
* PlainSquarePolygon
* SquarePolygonBase���p�������ȒP�ȃ|���S���N���X
*/

PlainSquarePolygon::PlainSquarePolygon()
{

}

// �R���X�g���N�^ ���W�Ƃ����Z�b�g���Ă���
PlainSquarePolygon::PlainSquarePolygon(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 *_camera, float _u, float _v, float _uw, float _vh)
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
	layer = _layer;
	camera = _camera;
}

// �f�X�g���N�^
PlainSquarePolygon::~PlainSquarePolygon()
{
	// ���ɂȂ�
}

void PlainSquarePolygon::update()
{
	if (status)
	{
	}
	drawing_coord.x = x - (camera->x - SCREEN_WIDTH / 2);
	drawing_coord.y = (y - (camera->y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;
	
}

void PlainSquarePolygon::draw()
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
bool PlainSquarePolygon::is_drawing()
{
	return drawing;
}

void PlainSquarePolygon::show()
{
	drawing = true;
}

void PlainSquarePolygon::hide()
{
	drawing = false;
}

// �e�N�X�`���̕ύX
void PlainSquarePolygon::change_texture(LPDIRECT3DTEXTURE9 _tex)
{
	tex = _tex;
}

D3DXVECTOR2 PlainSquarePolygon::get_coords()
{
	return D3DXVECTOR2(x, y);
}

POLSIZE PlainSquarePolygon::get_size()
{
	return POLSIZE(w, h);
}

VERTEX_2D * PlainSquarePolygon::get_vertexes()
{
	return vertexes;
}

void PlainSquarePolygon::add_coord(float _x, float _y)
{
	x += _x;
	y += _y;
}

void PlainSquarePolygon::zoom(POLSIZE _zoom_level)
{
	// �����Ȃ�
}

bool PlainSquarePolygon::is_active()
{
	return status;
}

void PlainSquarePolygon::enable()
{
	status = true;
}

void PlainSquarePolygon::disable()
{
	status = false;
}

// ���W�ƃT�C�Y����vertexes�𐶐����܂�
void PlainSquarePolygon::generate_vertexes()
{
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

// === PlainSquarePolygon END ===