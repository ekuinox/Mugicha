#include "scalable_object.h"

/*
* ScalableObject
* SquarePolygonBaseを継承した簡単なポリゴンクラス
*/

// コンストラクタ 座標とかをセットしていく
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
	zoom_level = { 1, 1 }; // 1倍で初期化
	scaling_base.x = drawing_coord.x + w / (scaling_dir % 3 == 0 ? 2 : -2); // 左に大きくするならbaseを右に，逆もまたしかり
	scaling_base.y = drawing_coord.y + h / (scaling_dir < 2 ? 2 : -2); // 上に大きくするならbaseを下に，逆もまた
	layer = _layer;
	camera = _camera;
}

// デストラクタ
ScalableObject::~ScalableObject()
{
	// 特になし
}

void ScalableObject::update()
{
	// 無
	if (!status) return;

	DWORD current = timeGetTime();

	// 操作
	if (current - latest_update > 1)
	{		
		latest_update = current;
	}
}

// 座標とサイズからvertexesを生成します
void ScalableObject::generate_vertexes()
{
	// ここにいろいろ問題がある
	
	for (auto i = 0; i < 4; ++i)
	{
		// 結局x, y中心に拡縮が行われてしまっているので，ここは修正しないといけない
		vertexes[i] = {
			[&]() {
				if (scaling_dir % 3 == 0) // dirが左の場合，baseは右にある
				{
					return (x - w / (i % 3 == 0 ? 2 : -2)) * -zoom_level.w - (camera->x - SCREEN_WIDTH / 2);

				}
				else // dirが右の場合，baseは左にある
				{
					return (x - w / (i % 3 == 0 ? 2 : -2)) * zoom_level.w - (camera->x - SCREEN_WIDTH / 2);
				}
			}(),
			[&]() {
				if (scaling_dir < 2) // dirが上の場合，baseは下にある
				{
					return ((y - h / (i < 2 ? 2 : -2)) * zoom_level.h - (camera->y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;
				}
				else // dirが下の場合，baseは上にある
				{
					return ((y - h / (i < 2 ? 2 : -2)) * -zoom_level.h - (camera->y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;
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

void ScalableObject::show()
{
	drawing = true;
}

void ScalableObject::hide()
{
	drawing = false;
}

// テクスチャの変更
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