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
PlainSquarePolygon::PlainSquarePolygon(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, float _u, float _v, float _uw, float _vh)
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
		// 特にやることなし
	}
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

// drawingフラグを引数で変更する
void PlainSquarePolygon::switch_drawing(bool _drawing)
{
	drawing = _drawing;
}

// drawingフラグの反転を行う
void PlainSquarePolygon::switch_drawing()
{
	drawing = !drawing;
}

// テクスチャの変更
void PlainSquarePolygon::change_texture(LPDIRECT3DTEXTURE9 _tex)
{
	tex = _tex;
}

// 当たり判定を取る
bool PlainSquarePolygon::is_collision(SquarePolygonBase *pol)
{
	return
		this->x - this->w / 2 <= pol->x + pol->w / 2 // 左と右
		&& this->x + this->w / 2 >= pol->x - pol->w // 右と左
		&& this->y - this-> h / 2 <= pol->y + pol->h / 2 // 上と下
		&& this->y + this->h / 2 >= pol->y - pol->h / 2 // 下と上
		? true : false;
}

void PlainSquarePolygon::switch_status(bool _status)
{
	status = _status;
}

bool PlainSquarePolygon::is_active()
{
	return status;
}

// 座標とサイズからvertexesを生成します
void PlainSquarePolygon::generate_vertexes()
{
	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = {
			this->x + this->w / (i % 3 == 0 ? -2 : 2),
			this->y + this->h / (i < 2 ? -2 : 2),
			0.0f,
			1.0f,
			D3DCOLOR_RGBA(255, 255, 255, 200),
			this->u + (i % 3 == 0 ? 0 : this->uw),
			this->v + (i < 2 ? 0 : this->vh)

		};
	}
}

// === PlainSquarePolygon END ===