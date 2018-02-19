#include "player.h"
#include "collision_checker.h"

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
	// TODO: ジャンプの処理が終わっていないのでやらないのいけない
	// TODO: 当たり判定自体はできているが，当たり判定を用いてうんたらはできていない

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

		// 枠外に出ないように => 壁の当たり判定でやりたい
		if (x - w / 2 < 0) x = w / 2;
		if (y - h / 2 < 0) y = h / 2;

		latest_update = current;

	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		jumping = true;
		direction = (direction + 90) / 2;
	}

	drawing_coord.x = x - (camera->x - SCREEN_WIDTH / 2);
	drawing_coord.y = (y - (camera->y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;
}

// === Player END ===