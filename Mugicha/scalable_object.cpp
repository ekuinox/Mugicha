#include "scalable_object.h"

ScalableObject::ScalableObject()
{
	// 既定のコンストラクタってやつ
}

// コンストラクタ 座標とかをセットしていく
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
	zoom_level = { 1, 1 }; // 1倍で初期化
	layer = _layer;
	camera = _camera;
}

// デストラクタ
ScalableObject::~ScalableObject()
{
	// 特になし
}

// 更新処理
void ScalableObject::update()
{
	// statusが有効になっていなければ抜けます
	if (!status) return;

	// 前回の更新から1ms経っていなければ行わない
	DWORD current = timeGetTime();
	if (current - latest_update > 1)
	{		
		latest_update = current;
	}

}

// 座標とサイズからvertexesを生成します
void ScalableObject::generate_vertexes()
{
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

void ScalableObject::zoom(POLSIZE _zoom_level)
{
	zoom_level = _zoom_level;
}
// === ScalableObject END ===