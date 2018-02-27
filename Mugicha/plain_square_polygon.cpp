#include "plain_square_polygon.h"

/*
* PlainSquarePolygon
* SquarePolygonBaseを継承した簡単なポリゴンクラス
*/
// コンストラクタ 座標とかをセットしていく
PlainSquarePolygon::PlainSquarePolygon(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, float _u, float _v, float _uw, float _vh)
	: SquarePolygonBase(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh)
{
	init();
}

// デストラクタ
PlainSquarePolygon::~PlainSquarePolygon()
{
	// 特になし
}

void PlainSquarePolygon::init()
{
	// 特にやることもなし
}

void PlainSquarePolygon::update()
{
	unless(status) return;
}

void PlainSquarePolygon::draw()
{
	unless (drawing) return; // フラグ判定

	generate_vertexes();

	// 画面外なら描画しない
	unless (vertexes[0].x <= SCREEN_WIDTH && vertexes[1].x >= 0 && vertexes[0].y <= SCREEN_HEIGHT && vertexes[2].y >= 0) return;

	d3d_device->SetTexture(0, tex);
	d3d_device->SetFVF(FVF_VERTEX_2D);

	d3d_device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		sizeof(this->vertexes) / sizeof(VERTEX_2D) - 2, // ポリゴン数
		this->vertexes,
		sizeof(VERTEX_2D)
	);
}

// drawingフラグを返す
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

// テクスチャの変更
void PlainSquarePolygon::change_texture(LPDIRECT3DTEXTURE9 _tex)
{
	tex = _tex;
}

SQUARE PlainSquarePolygon::get_square()
{
	return SQUARE(x, y, w, h);
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
	// 実装なし
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

void PlainSquarePolygon::on()
{
	enable();
	show();
}

void PlainSquarePolygon::off()
{
	disable();
	hide();
}

// 座標とサイズからvertexesを生成します
void PlainSquarePolygon::generate_vertexes()
{
	drawing_coord.x = x - (camera.x - SCREEN_WIDTH / 2);
	drawing_coord.y = (y - (camera.y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;

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