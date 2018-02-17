#include "background.h"


void Background::generate_vertexes()
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

Background::Background(LPDIRECT3DTEXTURE9 _tex, float _u, float _v, float _uw, float _vh)
{
	x = BACKGROUND_X;
	y = BACKGROUND_Y;
	w = BACKGROUND_WIDTH;
	h = BACKGROUND_HEIGHT;
	tex = _tex;
	aspect_ratio = h / w;
	// 引数でuv受け取れるようにしてあるけど，実際使うときにuvはもうクラス内でいじるようにしていいと思う
	u = _u;
	v = _v;
	uw = _uw;
	vh = _vh;
	layer = INT_MAX;
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
}

bool Background::is_drawing()
{
	return drawing;
}

void Background::show()
{
	drawing = true;
}

void Background::hide()
{
	drawing = false;
}

void Background::change_texture(LPDIRECT3DTEXTURE9 _tex)
{
	tex = _tex;
}

bool Background::is_collision(SquarePolygonBase * pol)
{
	return
		this->x - this->w / 2 <= pol->x + pol->w / 2 // 左と右
		&& this->x + this->w / 2 >= pol->x - pol->w // 右と左
		&& this->y - this->h / 2 <= pol->y + pol->h / 2 // 上と下
		&& this->y + this->h / 2 >= pol->y - pol->h / 2 // 下と上
		? true : false;
}

bool Background::is_active()
{
	return status;
}

void Background::enable()
{
	status = true;
}

void Background::disable()
{
	status = false;
}
