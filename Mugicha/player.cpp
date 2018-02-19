#include "player.h"
#include "collision_checker.h"

// コンストラクタ 座標とかをセットしていく
Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 * _camera, std::map<enum PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh) : polygons(_polygons)
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
	status = true;
	speed = 0.5f;
	angle = 0.0f;
	layer = _layer;
	camera = _camera;
	ground = false;
}

// デストラクタ
Player::~Player()
{
	// 特になし
}

void Player::update()
{
	// TODO: 当たり判定自体はできているが，当たり判定を用いてうんたらはできていない

	if (!status) return; // statusみて切る

	DWORD current = timeGetTime();

	// 操作
	if (current - latest_update > 1) // 1ms間隔で
	{
		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW)) // 左方向への移動
		{
			x -= speed;
		}
		else if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW)) // 右方向への移動
		{
			x += speed;
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

		// TODO: ジャンプ量とジャンプしている時間を調整する必要アリ
		if (jumping)
		{
			if (timeGetTime() - jumped_at > 500) jumping = false;
			y += 1.0f;
		}

		// TODO: 同様に落下速度も調整する必要がある
		unless(ground)
		{
			y -= 0.5f;
		}

		// 枠外に出ないように => 壁の当たり判定でやりたい
		if (x - w / 2 < 0) x = w / 2;
		if (y - h / 2 < 0)
		{
			y = h / 2;
			ground = true; // 接地判定
		}

#ifdef _DEBUG
		if (is_collision(this, polygons[SCALABLE_OBJECT].front()))
		{
			std::cout << "イッテエヨオ～！";
		}
#endif

		latest_update = current;
	}
}

bool Player::jump()
{
	unless (ground) return false;
	ground = false;
	jumped_at = timeGetTime();
	jumping = true;
	return true;
}

// === Player END ===