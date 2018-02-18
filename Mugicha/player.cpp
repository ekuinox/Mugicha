#include "player.h"

// コンストラクタ 座標とかをセットしていく
Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 *_camera, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh)
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
	speed = 0.5f;
	status = true;
	angle = 0.0f;
	layer = _layer;
	camera = _camera;
}

// デストラクタ
Player::~Player()
{
	// 特になし
}

void Player::update()
{

	if (!status) return; // statusみて切る

	DWORD current = timeGetTime();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_0))
	{
		std::cout << "ぴょい～ん" << std::endl;
	}
#endif

	// 操作
	if (current - latest_update > 1) // 1ms間隔で
	{
		bool moving = false;
		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW)) // 左方向への移動
		{
			direction = 180;
			moving = true;
		}
		else if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW)) // 右方向への移動
		{
			direction = 0;
			moving = true;
		}

#ifdef _DEBUG
		else if (GetKeyboardPress(DIK_W))
		{
			y += 1;
		}
		else if (GetKeyboardPress(DIK_S))
		{
			y -= 1;
		}
#endif

		if (moving)
		{
			x += cos(D3DXToRadian(direction)) * speed;
			y -= sin(D3DXToRadian(direction)) * speed;

		}

		if (jumping)
		{
			direction -= 0.1f;
			if (direction < 0) jumping = false;
		}

		latest_update = current;

	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		jumping = true;
		direction = (direction + 90) / 2;
	}

	drawing_coord.x = x - (camera->x - SCREEN_WIDTH / 2);
	drawing_coord.y = (y - (camera->y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;

#ifdef _DEBUG
	printf("PLAYER: %f, %f\n", x, y);
#endif
}

void Player::draw()
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
bool Player::is_drawing()
{
	return drawing;
}

void Player::show()
{
	drawing = true;
}

void Player::hide()
{
	drawing = false;
}

// テクスチャの変更
void Player::change_texture(LPDIRECT3DTEXTURE9 _tex)
{
	tex = _tex;
}

// 当たり判定を取る
bool Player::is_collision(SquarePolygonBase *pol)
{
	return
		this->x - this->w / 2 <= pol->get_coords().x + pol->get_size().w / 2 // 左と右
		&& this->x + this->w / 2 >= pol->get_coords().x - pol->get_size().w // 右と左
		&& this->y - this->h / 2 <= pol->get_coords().y + pol->get_size().h / 2 // 上と下
		&& this->y + this->h / 2 >= pol->get_coords().y - pol->get_size().h / 2 // 下と上
		? true : false;
}

D3DXVECTOR2 Player::get_coords()
{
	return D3DXVECTOR2(x, y);
}

POLSIZE Player::get_size()
{
	return { w, h };
}

void Player::add_coord(float _x, float _y)
{
	// 外部からは操作させないぞ！
}

void Player::zoom(POLSIZE _zoom_level)
{
	// 実装なし
}

bool Player::is_active()
{
	return status;
}

void Player::enable()
{
	status = true;
}

void Player::disable()
{
	status = false;
}

// 座標とサイズからvertexesを生成します
void Player::generate_vertexes()
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

// === Player END ==="