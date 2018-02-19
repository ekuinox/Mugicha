#include "background.h"

Background::Background(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 *_camera, float _u, float _v, float _uw, float _vh)
{
	x = BACKGROUND_X;
	y = BACKGROUND_Y;
	w = BACKGROUND_WIDTH;
	h = BACKGROUND_HEIGHT;
	tex = _tex;
	// 引数でuv受け取れるようにしてあるけど，実際使うときにuvはもうクラス内でいじるようにしていいと思う
	u = _u;
	v = _v;
	uw = _uw;
	vh = _vh;
	layer = INT_MAX;
	camera = _camera;
	zoom_level = { 1, 1 };
}

Background::~Background()
{
	// 無
}

void Background::update()
{
	if (!status) return;
	
	DWORD current = timeGetTime();

	// 操作
	if (current - latest_update > 1) // 1ms間隔で
	{
		// uv値の変更などをする

		latest_update = current;
	}
}