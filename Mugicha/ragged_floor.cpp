#include "ragged_floor.h"
#include "collision_checker.h"

void RaggedFloor::init()
{
	burdened = false;
	burdened_time = 0;
	falling = false;
	disapper_time = 0;
	x = init_pos.x;
	y = init_pos.y;
}

RaggedFloor::RaggedFloor()
{

}

RaggedFloor::RaggedFloor(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 * _camera, Player * _player, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), player(_player), init_pos(D3DXVECTOR2(_x, _y))
{
	init();
}

void RaggedFloor::update()
{
	auto current = timeGetTime();
	auto result = where_collision(player, this);
	
	if (result & BOTTOM) // 上にプレイヤがおるッ！！！
	{
		burdened = true;
	}
	else
	{
		burdened = false;
	}

	if (current - latest_update > 1)
	{
		if (burdened) // 踏まれている間のみ加算する
		{
			burdened_time += 1;
		}

		if (burdened_time > 5000) // 5秒も踏んづけやがって．．．
		{
			burdened = false; // 踏みつけオフ
			
			falling = true; // 落下オン
		}

		if (falling)
		{
			y -= 1;
		}
		
		if (y < 0) // 画面外に行っちゃったところからカウントスタート
		{
			disapper_time += 1;
		}
		if (disapper_time > 5000)
		{
			init();
		}

#ifdef _DEBUG
		printf("%f, %f, %s, %s, %f, %f\n", x, y, B_TO_S(burdened), B_TO_S(falling), burdened_time, disapper_time);

#endif
	}
}
