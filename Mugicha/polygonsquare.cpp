#include "polygonsquare.h"

/*
* SquarePolygonBase
* 四角のポリゴン用の抽象クラス
*/

// デフォルトコンストラクタ なんでこれ書かなきゃいけないのか未だにわからん
SquarePolygonBase::SquarePolygonBase()
{
	// なにもしない
}

// デストラクタ
SquarePolygonBase::~SquarePolygonBase()
{
	// なにもしない
}

// === SquarePolygonBase END ===


/*
* PlainSquarePolygon
* SquarePolygonBaseを継承した簡単なポリゴンクラス
*/

// コンストラクタ 座標とかをセットしていく
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

// デストラクタ
PlainSquarePolygon::~PlainSquarePolygon()
{
	// 特になし
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
	if (!drawing) return; // フラグ判定

	generate_vertexes();
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

D3DXVECTOR2 PlainSquarePolygon::get_coords()
{
	return D3DXVECTOR2(x, y);
}

POLSIZE PlainSquarePolygon::get_size()
{
	return { w, h };
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

// 当たり判定を取る
bool PlainSquarePolygon::is_collision(SquarePolygonBase *pol)
{
	return
		this->x - this->w / 2 <= pol->get_coords().x + pol->get_size().w / 2 // 左と右
		&& this->x + this->w / 2 >= pol->get_coords().x - pol->get_size().w // 右と左
		&& this->y - this->h / 2 <= pol->get_coords().y + pol->get_size().h / 2 // 上と下
		&& this->y + this->h / 2 >= pol->get_coords().y - pol->get_size().h / 2 // 下と上
		? true : false;
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

// 座標とサイズからvertexesを生成します
void PlainSquarePolygon::generate_vertexes()
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

// === PlainSquarePolygon END ===