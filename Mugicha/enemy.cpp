#include "enemy.h"
#include "collision_checker.h"

Enemy::Enemy(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 * _camera, int _layer, float _x, float _y, float _w, float _h, Vec _vec, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), vec(_vec), alive(true), polygons(_polygons), player(static_cast<Player*>(_polygons[SquarePolygonBase::PolygonTypes::PLAYER].front())), moving(false)
{
}

void Enemy::update()
{
	if (!status) return;
	
	auto current = timeGetTime();

	// プレイヤとの距離をみて移動を開始する 敵のプライベートなエリアに入ったらキレてくる感じで
	moving = is_collision(SQUARE(x, y, w * 10, h * 10), player->get_square()) ? true : false;

	if (current - latest_update > 1 && moving) // 1ms間隔で
	{
		// 当たり判定をみるポリゴンのベクタを作る
		std::vector<SquarePolygonBase*> to_check_polygons;
		for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR }) to_check_polygons.insert(to_check_polygons.end(), polygons[type].begin(), polygons[type].end());

		// 当たり精査
		char result = 0x00;
		float ground_height = y;
		for (const auto& polygon : to_check_polygons) result |= where_collision(this, polygon, 0);

		auto vector = D3DXVECTOR2(0, 0); // いくら移動したかをここに

		// アタマぶつけとる
		if (result & TOP) jumping = false;

		// 足がついている
		if (result & BOTTOM) on_ground = true;

		// 左がぶつかっている
		if (result & LEFT) vec = Vec::RIGHT;

		// 右がぶつかっている
		if (result & RIGHT) vec = Vec::LEFT;
		
		vector.x += vec == Vec::RIGHT ? 1 : -1;

		// ジャンプ開始から時間が経過
		if (timeGetTime() - jumped_at > 500) jumping = false;
		
		// ジャンプなう
		if (jumping) vector.y += 2.0f;
		
		// 空中に居る間は落下し続ける
		if (!on_ground) vector.y -= 1.0f;

		// 次々ジャンプしていけ
		if (on_ground && !jumping)
		{
			jumping = true;
			on_ground = false;
			jumped_at = timeGetTime();
		}

		// 反映
		x += vector.x;
		y += vector.y;

		latest_update = current;
	}
}
