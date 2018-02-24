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
	auto vector = D3DXVECTOR2(0, 0);

	// 操作
	if (current - latest_update > 1) // 1ms間隔で
	{
		// 当たり精査

		// TODO: 当たると登りやがる
		// TODO: 連続したポリゴンの上で滑れないというかなんというかアレ
		// TODO: 挟まれた際，無に行ってしまうバグがある

		char result = 0x00;
		for (const auto& polygon : to_check_polygons) result |= where_collision(this, polygon);
		
#ifdef _DEBUG
		if (result & LEFT) printf("LEFT ");
		if (result & RIGHT)	printf("RIGHT ");
		if (result & BOTTOM) printf("BOTTOM ");
		if (result & TOP) printf("TOP ");
		printf("\n");
#endif
		
		// 
		if (result & BOTTOM)
		{
			ground = true;
		}
		else
		{
			ground = false;
		}

		unless(controll_lock)
		{
			if (!(result & LEFT) && (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW))) // 左方向への移動
			{
				vector.x -= speed;
			}
			if (!(result & RIGHT) && (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW))) // 右方向への移動
			{
				vector.x += speed;
			}
		}


		// TODO: ジャンプ量とジャンプしている時間を調整する必要アリ
		if (!(result & TOP) && jumping)
		{
			if (timeGetTime() - jumped_at > 500) jumping = false;
			vector.y += 1.0f;
		}

		// TODO: 同様に落下速度も調整する必要がある
		unless(ground)
		{
			vector.y -= 0.5f;
		}

#ifdef _DEBUG
		if (!(result & TOP) && GetKeyboardPress(DIK_W))
		{
			vector.y += 1;
		}
		if (!(result & BOTTOM) && GetKeyboardPress(DIK_S))
		{
			vector.y -= 1;
		}
#endif

		// 変更を加算して終了
		x += vector.x;
		y += vector.y;

		latest_update = current;
	}
}

bool Player::jump()
{
	unless(ground)
	{
#ifdef _DEBUG
		printf("飛べねえ！！！\n");
#endif
		return false;
	}
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