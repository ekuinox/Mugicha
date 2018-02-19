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
	// 無
	if (!status) return;
	
	DWORD current = timeGetTime();

	// 操作
	if (current - latest_update > 1) // 1ms間隔で
	{
		// uv値の変更などをする

		/*
		// プレイヤの移動に合わせて背景を左右に移動させる
		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW)) // 左方向への移動
		{
			u -= 0.0001f;
		}
		else if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW)) // 右方向への移動
		{
			u += 0.0001f;
		}
		
		
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

		// 限界調整
		if (w < SCREEN_WIDTH) w = SCREEN_WIDTH;
		if (h < SCREEN_HEIGHT) h = SCREEN_HEIGHT;
		*/

		latest_update = current;
	}
}

void Background::draw()
{
	if(!drawing) return;
	
	generate_vertexes();
	d3d_device->SetTexture(0, tex);
	d3d_device->SetFVF(FVF_VERTEX_2D);

	d3d_device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		sizeof(this->vertexes) / sizeof(VERTEX_2D) - 2, // ポリゴン数
		this->vertexes,
		sizeof(VERTEX_2D)
	);

#ifdef __DEBUG
	for (const auto vertex : vertexes)
	{
		std::cout << "(" << vertex.x << ", " << vertex.y << "),";
	}
	std::cout << std::endl;
#endif
}