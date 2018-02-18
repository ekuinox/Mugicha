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
	// ˆø”‚Åuvó‚¯æ‚ê‚é‚æ‚¤‚É‚µ‚Ä‚ ‚é‚¯‚ÇCÀÛg‚¤‚Æ‚«‚Éuv‚Í‚à‚¤ƒNƒ‰ƒX“à‚Å‚¢‚¶‚é‚æ‚¤‚É‚µ‚Ä‚¢‚¢‚Æv‚¤
	u = _u;
	v = _v;
	uw = _uw;
	vh = _vh;
	layer = INT_MAX;
}

Background::~Background()
{
	// –³
}

void Background::update()
{
	// –³
	if (!status) return;
	
	DWORD current = timeGetTime();

	// ‘€ì
	if (current - latest_update > 1) // 1msŠÔŠu‚Å
	{
		// uv’l‚Ì•ÏX‚È‚Ç‚ğ‚·‚é

		// ƒvƒŒƒCƒ„‚ÌˆÚ“®‚É‡‚í‚¹‚Ä”wŒi‚ğ¶‰E‚ÉˆÚ“®‚³‚¹‚é
		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW)) // ¶•ûŒü‚Ö‚ÌˆÚ“®
		{
			u -= 0.0001f;
		}
		else if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW)) // ‰E•ûŒü‚Ö‚ÌˆÚ“®
		{
			u += 0.0001f;
		}

		// Šgk
		if (GetKeyboardPress(DIK_NUMPAD8)) // Šg‘å
		{
			w += 1.0f;
			h += 1.0f * aspect_ratio;
		}
		else if (GetKeyboardPress(DIK_NUMPAD2)) // k¬
		{
			w -= 1.0f;
			h -= 1.0f * aspect_ratio;
		}

		// ŒÀŠE’²®
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
		sizeof(this->vertexes) / sizeof(VERTEX_2D) - 2, // ƒ|ƒŠƒSƒ“”
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

D3DXVECTOR2 Background::get_coords()
{
	return D3DXVECTOR2(x, y);
}

POLSIZE Background::get_size()
{
	return { w, h };
}

void Background::add_coord(float _x, float _y)
{
}

void Background::zoom(POLSIZE _zoom_level)
{
	// ‚ ‚Æ‚Åì‚é
}

VERTEX_2D * Background::get_vertexes()
{
	return vertexes;
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
