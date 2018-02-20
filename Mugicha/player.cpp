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
	controll_lock = false;
}

// デストラクタ
Player::~Player()
{
	// 特になし
}

void Player::update()
{
	// TODO: 当たり判定自体はできているが，当たり判定を用いてうんたらはできていない
	//これが正解かわからないけど，x,y正負に加減算するたびにgenerate_vertexして，is_collisonを取るととりあえず通る
	if (!status) return; // statusみて切る

	DWORD current = timeGetTime();

	std::vector<SquarePolygonBase*> to_check_polygons;
	for (const auto& type : { PLAIN, SCALABLE_OBJECT }) to_check_polygons.insert(to_check_polygons.end(), polygons[type].begin(), polygons[type].end());

	auto old_pos = D3DXVECTOR2(x, y);

	// 操作
	if (current - latest_update > 1) // 1ms間隔で
	{
		unless(controll_lock)
		{
			if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW)) // 左方向への移動
			{
				x -= speed;
			}
			if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW)) // 右方向への移動
			{
				x += speed;
			}
		}
		

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

		// 当たり精査
		ground = false;
		for (const auto& polygon : to_check_polygons)
		{
			char result = where_collision(this, polygon);
			if (result & BOTTOM)
			{
				ground = true;
			}
			if (result & TOP)
			{
				jumping = false; // 頭ぶつけた時点でジャンプ解除
				y = old_pos.y;
			}
			if (result & (LEFT | RIGHT))
			{
				x = old_pos.x;
			}
		}

		latest_update = current;
	}
}

bool Player::jump()
{
	unless(ground) return false;
	if (controll_lock) return false;
	ground = false;
	jumped_at = timeGetTime();
	jumping = true;
	return true;
}

void Player::lock()
{
	controll_lock = true;
}

void Player::unlock()
{
	controll_lock = false;
}

// === Player END ===