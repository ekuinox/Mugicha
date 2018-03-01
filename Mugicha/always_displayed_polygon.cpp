#include "always_displayed_polygon.h"

AlwaysDisplayedPolygon::AlwaysDisplayedPolygon(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, float _u, float _v, float _uw, float _vh)
	: SquarePolygonBase(_x, _y, _w, _h, _tex, _layer, _u, _v, _uw, _vh)
{
}

AlwaysDisplayedPolygon::~AlwaysDisplayedPolygon()
{
}

void AlwaysDisplayedPolygon::init()
{
}

void AlwaysDisplayedPolygon::update()
{
	unless(status) return;
}

void AlwaysDisplayedPolygon::draw()
{
	unless(drawing) return; // ƒtƒ‰ƒO”»’è

	generate_vertexes();

	// ‰æ–ÊŠO‚È‚ç•`‰æ‚µ‚È‚¢
	unless(vertexes[0].x <= SCREEN_WIDTH && vertexes[1].x >= 0 && vertexes[0].y <= SCREEN_HEIGHT && vertexes[2].y >= 0) return;

	d3d_device->SetTexture(0, tex);
	d3d_device->SetFVF(FVF_VERTEX_2D);

	d3d_device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		sizeof(this->vertexes) / sizeof(VERTEX_2D) - 2, // ƒ|ƒŠƒSƒ“”
		this->vertexes,
		sizeof(VERTEX_2D)
	);
}

bool AlwaysDisplayedPolygon::is_drawing()
{
	return drawing;
}

void AlwaysDisplayedPolygon::show()
{
	drawing = true;
}

void AlwaysDisplayedPolygon::hide()
{
	drawing = false;
}

bool AlwaysDisplayedPolygon::is_active()
{
	return status;
}

void AlwaysDisplayedPolygon::enable()
{
	status = true;
}

void AlwaysDisplayedPolygon::disable()
{
	status = false;
}

void AlwaysDisplayedPolygon::on()
{
	enable();
	show();
}

void AlwaysDisplayedPolygon::off()
{
	disable();
	hide();
}

void AlwaysDisplayedPolygon::change_texture(LPDIRECT3DTEXTURE9 _tex)
{
	tex = _tex;
}

SQUARE AlwaysDisplayedPolygon::get_square()
{
	return SQUARE(x, y, w, h);
}

D3DXVECTOR2 AlwaysDisplayedPolygon::get_coords()
{
	return D3DXVECTOR2(x, y);
}

POLSIZE AlwaysDisplayedPolygon::get_size()
{
	return POLSIZE(w, h);
}

VERTEX_2D * AlwaysDisplayedPolygon::get_vertexes()
{
	return vertexes;
}

void AlwaysDisplayedPolygon::add_coord(float _x, float _y)
{
	x += _x;
	y += _y;
}

void AlwaysDisplayedPolygon::zoom(float _zoom_level)
{
}

AlwaysDisplayedPolygon * AlwaysDisplayedPolygon::operator->()
{
	return this;
}

void AlwaysDisplayedPolygon::generate_vertexes()
{
	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = VERTEX_2D(
			x + w / (i % 3 == 0 ? -2 : 2),
			y + h / (i < 2 ? -2 : 2),
			u + (i % 3 == 0 ? 0 : uw),
			v + (i < 2 ? 0 : vh)
		);
	}
}
