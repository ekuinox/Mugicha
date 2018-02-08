#include "scalable_object.h"

/*
* ScalableObject
* SquarePolygonBaseを継承した簡単なポリゴンクラス
*/

// コンストラクタ 座標とかをセットしていく
ScalableObject::ScalableObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, float _u, float _v, float _uw, float _vh)
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
}

// デストラクタ
ScalableObject::~ScalableObject()
{
	// 特になし
}

void ScalableObject::update()
{
	if (status)
	{
		// 拡縮
		if (GetKeyboardPress(DIK_NUMPAD8)) // 拡大
		{
			w += 1.0f;
			h += 1.0f * aspect_ratio;
		}
		else if (GetKeyboardPress(DIK_NUMPAD2)) // 縮小
		{
			w -= 1.0f;
			h -= 1.0f * aspect_ratio;
		}
	}
}

void ScalableObject::draw()
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
bool ScalableObject::is_drawing()
{
	return drawing;
}

// drawingフラグを引数で変更する
void ScalableObject::switch_drawing(bool _drawing)
{
	drawing = _drawing;
}

// drawingフラグの反転を行う
void ScalableObject::switch_drawing()
{
	drawing = !drawing;
}

// テクスチャの変更
void ScalableObject::change_texture(LPDIRECT3DTEXTURE9 _tex)
{
	tex = _tex;
}

// 当たり判定を取る
bool ScalableObject::is_collision(SquarePolygonBase *pol)
{
	return
		this->x - this->w / 2 <= pol->x + pol->w / 2 // 左と右
		&& this->x + this->w / 2 >= pol->x - pol->w // 右と左
		&& this->y - this->h / 2 <= pol->y + pol->h / 2 // 上と下
		&& this->y + this->h / 2 >= pol->y - pol->h / 2 // 下と上
		? true : false;
}

void ScalableObject::switch_status(bool _status)
{
	status = _status;
}

bool ScalableObject::is_active()
{
	return status;
}

// 座標とサイズからvertexesを生成します
void ScalableObject::generate_vertexes()
{
	auto left_bottom_x = x - w / 2;
	auto left_bottom_y = y + h / 2;
	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = {
			x + (i % 3 == 0 ? 0 : this->w),
			y - (i < 2 ? this->h : 0),
			0.0f,
			1.0f,
			D3DCOLOR_RGBA(255, 255, 255, 200),
			this->u + (i % 3 == 0 ? 0 : this->uw),
			this->v + (i < 2 ? 0 : this->vh)

		};
	}
}

// === ScalableObject END ===