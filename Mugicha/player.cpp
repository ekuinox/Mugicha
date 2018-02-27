#include "player.h"
#include "collision_checker.h"

// コンストラクタ 座標とかをセットしていく
Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh)
	: polygons(_polygons), PlainSquarePolygon(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), before_zoom_level(1, 1)
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
	speed = PLAYER_SPEED;
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

	auto current = timeGetTime();

	// 操作
	if (current - latest_update > 1) // 1ms間隔で
	{
		// 当たり精査
		char result = 0x00;
		float ground_height = y;
		for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR, SquarePolygonBase::PolygonTypes::THORNS, })
		{
			for (const auto& polygon : polygons[type])
			{
				auto _result = where_collision(this, polygon);
				if (_result & BOTTOM)
				{
					if (ground_height < polygon->get_square().top())
					{
						ground_height = (polygon->get_square().top() + h / 2) * (zoom_level.h / before_zoom_level.h);
					}
				}
				result |= _result;
			}
		}

		if (controll_lock)
		{
			// 移動前の座標と拡縮する前のズームレベルと現在のズームレベルから割り出したモノをかけていく．
			x = when_locked_coords.x * (zoom_level.w / before_zoom_level.w);
			y = when_locked_coords.y * (zoom_level.h / before_zoom_level.h);

			// 挟まれ判定
			if ((result & HitLine::BOTTOM && result & HitLine::TOP) || (result & HitLine::LEFT && result & HitLine::RIGHT))
			{
				// 挟まれとんやが！！！！
			}
			if (result & HitLine::BOTTOM)
			{
				y = ground_height;
			}
		}
		else
		{
			auto vector = D3DXVECTOR2(0, 0); // いくら移動したかをここに

			// 挟まれ判定
			if ((result & HitLine::BOTTOM && result & HitLine::TOP) || (result & HitLine::LEFT && result & HitLine::RIGHT))
			{
				// 挟まれとんやが！！！！
			}

			// 枠外落下判定
			if (y < -50)
			{
				// 死ぬ～～
			}

			// 接地判定
			if (result & HitLine::BOTTOM)
			{
				ground = true;
			}
			else
			{
				ground = false;
			}

			if (!(result & HitLine::LEFT) && (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW))) // 左方向への移動
			{
				vector.x -= speed;
			}
			if (!(result & HitLine::RIGHT) && (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW))) // 右方向への移動
			{
				vector.x += speed;
			}

			// TODO: ジャンプ量とジャンプしている時間を調整する必要アリ
			if (!(result & HitLine::TOP) && jumping)
			{
				if (timeGetTime() - jumped_at > PLAYER_JUMP_TIME) jumping = false;
				vector.y += PLAYER_JUMP_POWER;
			}

			if(result & HitLine::TOP)
			{
				jumping = false;
			}

			// TODO: 同様に落下速度も調整する必要がある
			unless(ground)
			{
				vector.y -= PLAYER_FALLING;
			}

			// 変更を加算して終了
			x += vector.x;
			y += vector.y;
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
	when_locked_coords = { x, y };
	before_zoom_level = zoom_level;
}

void Player::unlock()
{
	controll_lock = false;
}

void Player::kill()
{
	alive = false;
#ifdef _PLAYER_DEBUG
	std::cout << "今の本番やったら死んどったからな～～！" << std::endl;
#endif
}

bool Player::dead()
{
	return !alive;
}

// === Player END ===