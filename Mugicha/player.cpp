#include "collision_checker.h"
#include "player.h"
#include "thorn.h"
#include "bullet.h"

bool Player::collision_for_enemies()
{
	// 敵との当たり判定
	for (const auto& enemy : polygons[SquarePolygonBase::PolygonTypes::ENEMY])
	{
		if (enemy->is_active() && is_collision(enemy->get_square(), get_square()))
		{
			if (zoom_level.w >= 1.0f)
			{
#ifndef _WITHOUT_DEATH
				// プレイヤの負け
				kill(DeadReason::HitEnemy);
				return true;
#endif
			}
			else
			{
				// 敵の負け
				enemy->off();
			}
		}
	}
	return false;
}

bool Player::collision_for_thorns()
{
	// トゲとの当たり判定
	// 通常状態より自分がデカい状態で接触死の判定を取る
	if (zoom_level.w > 1.0f) return false;
	for (const auto& thorn : polygons[SquarePolygonBase::PolygonTypes::THORN])
	{
		auto self = get_square();
		auto another = thorn->get_square();

		// トゲを落とす
		if (vec != Player::Vec::CENTER && static_cast<Thorn*>(thorn)->attack)
		{
			if (is_collision(SQUARE(get_square().x + get_square().w * (vec == Player::Vec::RIGHT ? 1 : -1), get_square().y + get_square().h * 2, w * 2, get_square().h * 10), another))
			{
				static_cast<Thorn*>(thorn)->trigger_falling();
			}
			else
			{
				static_cast<Thorn*>(thorn)->stop_falling();
			}
		}

		switch (static_cast<Thorn*>(thorn)->get_vec())
		{
		case Thorn::Vec::UP:
			if (hit_bottom(self, another))
			{
#ifndef _WITHOUT_DEATH
				kill(DeadReason::HitThorn);
				return true;
#endif
			}
			break;
		case Thorn::Vec::DOWN:
			if (hit_top(self, another))
			{
#ifndef _WITHOUT_DEATH
				kill(DeadReason::HitThorn);
				return true;
#endif
			}
			break;
		case Thorn::Vec::RIGHT:
			if (hit_left(self, another))
			{
#ifndef _WITHOUT_DEATH
				kill(DeadReason::HitThorn);
				return true;
#endif
			}
			break;
		case Thorn::Vec::LEFT:
			if (hit_right(self, another))
			{
#ifndef _WITHOUT_DEATH
				kill(DeadReason::HitThorn);
				return true;
#endif
			}
			break;
		}
	}

	return false;
}

bool Player::collision_for_magmas()
{
	// マグマとの当たり判定
	for (const auto& magma : polygons[SquarePolygonBase::PolygonTypes::MAGMA])
	{
		if (is_collision(get_square(), magma->get_square()))
		{
#ifndef _WITHOUT_DEATH
			kill(DeadReason::HitMagma);
			return true;
#endif
		}
	}
	return false;
}

bool Player::collision_for_bullets()
{
	// 弾丸との当たり判定
	for (const auto& bullet : polygons[SquarePolygonBase::PolygonTypes::BULLET])
	{
		if (is_collision(get_square(), bullet->get_square()))
		{
			bullet->init();
#ifndef _WITHOUT_DEATH
			kill(DeadReason::Shot);
			return true;
#endif
		}
	}
	return false;
}

bool Player::collision_for_knockback_bullets(D3DXVECTOR2 &knockback)
{
	auto self = get_square();
	
	for (const auto& bullet : polygons[SquarePolygonBase::PolygonTypes::KNOCKBACK_BULLET])
	{

		auto another = bullet->get_square();
		auto vec = static_cast<Bullet*>(bullet)->get_vec();
		auto result = where_collision(another, self, 0.0f);

		if (result & HitLine::TOP && vec == Bullet::Vec::UP)
		{
			knockback.y += CELL_WIDTH;
			bullet->init();
		}
		else if (result & HitLine::BOTTOM && vec == Bullet::Vec::DOWN)
		{	
			knockback.y -= CELL_WIDTH;
			bullet->init();
		}
		else if (result & HitLine::LEFT && vec == Bullet::Vec::LEFT)
		{
			knockback.x -= CELL_WIDTH;
			bullet->init();
		}
		else if (result & HitLine::RIGHT && vec == Bullet::Vec::RIGHT)
		{
			knockback.x += CELL_WIDTH;
			bullet->init();
		}


	}
	return false;
	
}

// コンストラクタ 座標とかをセットしていく
Player::Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>>& _polygons, int _layer, float _x, float _y, float _w, float _h, float _u, float _v, float _uw, float _vh)
	: polygons(_polygons), PlainSquarePolygon(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), before_zoom_level(1, 1), dead_reason(DeadReason::ALIVE), vec(Player::Vec::CENTER), item(nullptr), holding_item(false)
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

	auto current = std::chrono::system_clock::now();

	// 操作
	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_update).count() > UPDATE_INTERVAL) // 1ms間隔で
	{
		auto vector = D3DXVECTOR2(0, 0); // いくら移動したかをここに

		if (collision_for_enemies())
		{
			// 死
			return;
		}

		if (collision_for_thorns())
		{
			// 死
			return;
		}

		if (collision_for_magmas())
		{
			// 死
			return;
		}

		if (collision_for_bullets())
		{
			// 死～
			return;
		}
		
		// 当たり精査
		char result = 0x00;
		for (const auto& type : {
			// 当たり判定を取るポリゴンのラベルまとめ
			SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT,
			SquarePolygonBase::PolygonTypes::RAGGED_FLOOR,
			SquarePolygonBase::PolygonTypes::THORN,
			SquarePolygonBase::PolygonTypes::AIRCANNON,
			SquarePolygonBase::PolygonTypes::GIMMICK_SWITCH,
			})
			for (const auto& polygon : polygons[type])
				result |= where_collision(this, polygon);

		if (controll_lock)
		{
			// 移動前の座標と拡縮する前のズームレベルと現在のズームレベルから割り出したモノをかけていく．
			x = when_locked_coords.x * (zoom_level.w / before_zoom_level.w);
			y = when_locked_coords.y * (zoom_level.h / before_zoom_level.h);
		}
		else
		{
			// ノックバックについて
			auto knockback = D3DXVECTOR2(0, 0);

			collision_for_knockback_bullets(knockback);

			// 左右
			if ((knockback.x < 0 && !(result & HitLine::LEFT)) || knockback.x > 0 && !(result & HitLine::RIGHT))
			{
				vector.x += knockback.x;
			}

			// 上下
			if ((knockback.y < 0 && !(result & HitLine::BOTTOM)) || knockback.y > 0 && !(result & HitLine::TOP))
			{
				vector.y += knockback.y;
			}

			// 挟まれ判定
			if (result & HitLine::BOTTOM && result & HitLine::TOP && result & HitLine::LEFT && result & HitLine::RIGHT)
			{
				kill(DeadReason::Sandwiched);
			}

			// 枠外落下判定
			if (y < -50)
			{
				kill(DeadReason::Falling);
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
				vec = Player::Vec::LEFT;
			}
			if (!(result & HitLine::RIGHT) && (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW))) // 右方向への移動
			{
				vector.x += speed;
				vec = Player::Vec::RIGHT;
			}

#ifdef _DEBUG
			if (GetKeyboardPress(DIK_W))
			{
				vector.y += 5;
			}
			if (GetKeyboardPress(DIK_S))
			{
				vector.y -= 5;
			}
#endif

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
		
		// itemを持っているならitemの位置を修正してあげる
		if (holding_item) item->move(D3DXVECTOR2(x + w / (vec == Player::Vec::RIGHT ? 2 : -2), y));

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

bool Player::catch_item()
{
	for (const auto& _item : polygons[SquarePolygonBase::PolygonTypes::ITEM])
	{
		if (static_cast<Item*>(_item)->hold(get_square()))
		{
			item = static_cast<Item*>(_item);
			holding_item = true;
			return true;
		}
	}

	return false;
}

void Player::release_item()
{
	holding_item = false;
	item->release();
}

bool Player::is_holding_item()
{
	return holding_item;
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

void Player::kill(const DeadReason & _dead_reason)
{
#ifndef _WITHOUT_DEATH
	dead_reason = _dead_reason;
#endif
}

Player::DeadReason Player::dead()
{
	return dead_reason;
}

Player::Vec Player::get_vec()
{
	return vec;
}

// === Player END ===