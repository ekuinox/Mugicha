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
	//これが正解かわからないけど，x,y正負に加減算するたびにgenerate_vertexして，is_collisonを取るととりあえず通る
	if (!status) return; // statusみて切る

	DWORD current = timeGetTime();

	std::vector<SquarePolygonBase*> to_check_polygons;
	PolygonTypes types[] = { PLAIN, SCALABLE_OBJECT };
	for (const auto& type : types) to_check_polygons.insert(to_check_polygons.end(), polygons[type].begin(), polygons[type].end());

	// 操作
	if (current - latest_update > 1) // 1ms間隔で
	{
		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW)) // 左方向への移動
		{
			x -= speed;
			generate_vertexes();
			for (const auto& polygon : to_check_polygons)
			{
				if (is_collision(this, polygon))
				{
					x += speed;
					break;
				}
			}
		}
		else if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW)) // 右方向への移動
		{
			x += speed;
			generate_vertexes();
			for (const auto& polygon : to_check_polygons)
			{
				if (is_collision(this, polygon))
				{
					x -= speed;
					break;
				}
			}
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

		// これ落下していって足元にブロックがあるかの判定ですね
		generate_vertexes();
		for (const auto& polygon : to_check_polygons)
		{
			unless(is_collision(this, polygon))
			{
				ground = false;
				break;
			}
		}

		// TODO: ジャンプ量とジャンプしている時間を調整する必要アリ
		if (jumping)
		{
			if (timeGetTime() - jumped_at > 500) jumping = false;
			y += 1.0f;

			generate_vertexes();
			for (const auto& polygon : to_check_polygons)
			{
				if (is_collision(this, polygon))
				{
					y -= 1.0f;
					break;
				}
			}
		}

		// TODO: 同様に落下速度も調整する必要がある
		unless(ground)
		{
			y -= 0.5f;

			generate_vertexes();
			for (const auto& polygon : to_check_polygons)
			{
				if (is_collision(this, polygon))
				{
					y += 0.5f;
					ground = true;
					break;
				}
			}
		}

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