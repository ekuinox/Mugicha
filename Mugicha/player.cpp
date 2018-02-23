#include "player.h"
#include "collision_checker.h"

void Player::generate_vertexes()
{
	drawing_coord.x = x - (camera->x - SCREEN_WIDTH / 2);
	drawing_coord.y = (y - (camera->y - SCREEN_HEIGHT / 2)) * -1 + SCREEN_HEIGHT;

	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = VERTEX_2D(
			drawing_coord.x + this->w / (i % 3 == 0 ? -2 : 2),
			drawing_coord.y + this->h / (i < 2 ? -2 : 2),
			this->u + (i % 3 == 0 ? 0 : this->uw),
			this->v + (i < 2 ? 0 : this->vh)
		);
	}
}

// コンストラクタ 座標とかをセットしていく
Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 * _camera, std::map<enum PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh)
	: polygons(_polygons), PlainSquarePolygon(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh)
{
	init();
}

// デストラクタ
Player::~Player()
{
	// 特になし
}

void Player::init()
{
	speed = 0.5f;
	ground = false;
	controll_lock = false;
}

void Player::zoom(POLSIZE _zoom_level)
{
	zoom_level = _zoom_level;
}

void Player::update()
{
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

#ifdef _DEBUG
		if (GetKeyboardPress(DIK_W))
		{
			y += 1;
		}
		if (GetKeyboardPress(DIK_S))
		{
			y -= 1;
		}
#endif

		// 当たり精査

		// TODO: 当たると登りやがる
		// TODO: 連続したポリゴンの上で滑れないというかなんというかアレ
		// TODO: 挟まれた際，無に行ってしまうバグがある

		ground = false;
		for (const auto& polygon : to_check_polygons)
		{
			char result = where_collision(this, polygon);
			
			if (result & LEFT)
			{
				auto square = polygon->get_square();
				x = square.x + square.w / 2 + w / 2;
			}
			if (result & RIGHT)
			{
				auto square = polygon->get_square();
				x = square.x - square.w / 2 - w / 2;
			}
			
			if (result == BOTTOM)
			{
				ground = true;
				auto square = polygon->get_square();
				y = square.y + square.h / 2 + h / 2;
			}
			if (result == TOP)
			{
				if (jumping) // ジャンプして頭ぶつけた場合はいいけど
				{
					jumping = false; // 頭ぶつけた時点でジャンプ解除
					auto square = polygon->get_square();
					y = square.y - square.h / 2 - h / 2;
				}
				else if (ground) // この場合は拡縮で縮んだポリゴンに押し潰さているので，
				{
					// ここで死亡処理作らないといけない
#ifdef _DEBUG
					std::cout << "グエー挟まれたンゴ\n";
#endif
					kill();
				}
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

void Player::kill()
{
	alive = false;
#ifdef _DEBUG
	std::cout << "今の本番やったら死んどったからな～～！" << std::endl;
#endif
}

bool Player::dead()
{
	return !alive;
}

// === Player END ===