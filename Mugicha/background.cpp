#include "background.h"


void Background::generate_vertexes()
{
	auto left_bottom_x = 0;
	auto left_bottom_y = SCREEN_HEIGHT; 
	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = {
//			this->x + this->w / (i % 3 == 0 ? -2 : 2),
			left_bottom_x + (i % 3 == 0 ? 0 : this->w),
//			this->y + this->h / (i < 2 ? -2 : 2),
			left_bottom_y - (i < 2 ? this->h : 0),
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
	// à¯êîÇ≈uvéÛÇØéÊÇÍÇÈÇÊÇ§Ç…ÇµÇƒÇ†ÇÈÇØÇ«ÅCé¿ç€égÇ§Ç∆Ç´Ç…uvÇÕÇ‡Ç§ÉNÉâÉXì‡Ç≈Ç¢Ç∂ÇÈÇÊÇ§Ç…ÇµÇƒÇ¢Ç¢Ç∆évÇ§
	u = _u;
	v = _v;
	uw = _uw;
	vh = _vh;

}

Background::~Background()
{
	// ñ≥
}

void Background::update()
{
	// ñ≥
	if (!status) return;
	
	DWORD current = timeGetTime();

	// ëÄçÏ
	if (current - latest_update > 1) // 1msä‘äuÇ≈
	{
		// uvílÇÃïœçXÇ»Ç«ÇÇ∑ÇÈ

		// ÉvÉåÉCÉÑÇÃà⁄ìÆÇ…çáÇÌÇπÇƒîwåiÇç∂âEÇ…à⁄ìÆÇ≥ÇπÇÈ
		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW)) // ç∂ï˚å¸Ç÷ÇÃà⁄ìÆ
		{
			u -= 0.0001f;
		}
		else if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW)) // âEï˚å¸Ç÷ÇÃà⁄ìÆ
		{
			u += 0.0001f;
		}

		// ägèk
		if (GetKeyboardPress(DIK_NUMPAD8)) // ägëÂ
		{
			w += 1.0f;
			h += 1.0f * aspect_ratio;
		}
		else if (GetKeyboardPress(DIK_NUMPAD2)) // èkè¨
		{
			w -= 1.0f;
			h -= 1.0f * aspect_ratio;
		}

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
		sizeof(this->vertexes) / sizeof(VERTEX_2D) - 2, // É|ÉäÉSÉìêî
		this->vertexes,
		sizeof(VERTEX_2D)
	);
}

bool Background::is_drawing()
{
	return drawing;
}

void Background::switch_drawing(bool _drawing)
{
	drawing = _drawing;
}

void Background::switch_drawing()
{
	drawing = !drawing;
}

void Background::change_texture(LPDIRECT3DTEXTURE9 _tex)
{
	tex = _tex;
}

bool Background::is_collision(SquarePolygonBase * pol)
{
	return
		this->x - this->w / 2 <= pol->x + pol->w / 2 // ç∂Ç∆âE
		&& this->x + this->w / 2 >= pol->x - pol->w // âEÇ∆ç∂
		&& this->y - this->h / 2 <= pol->y + pol->h / 2 // è„Ç∆â∫
		&& this->y + this->h / 2 >= pol->y - pol->h / 2 // â∫Ç∆è„
		? true : false;
}

void Background::switch_status(bool _status)
{
	status = _status;
}

bool Background::is_active()
{
	return status;
}
