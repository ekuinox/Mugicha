#include "goal.h"
#include "collision_checker.h"

Goal::Goal()
{
	// 特に無
}

Goal::Goal(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 * _camera, Player *_player, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), player(_player), completed(false)
{
	init();
}

Goal::~Goal()
{
	// 無
}

void Goal::update()
{
	// プレイヤとの当たりだけ見る
	if (is_collision(player, this))
	{
		// 触れただけでゴールってのもアレなので，アレする
		auto core = SQUARE(static_cast<float>(x * zoom_level.w), static_cast<float>(y * zoom_level.w), static_cast<float>(w * zoom_level.w / 2), static_cast<float>(h * zoom_level.h * 0.8));

		if (is_collision(player->get_square(), core)) // コアに当たり申した
		{
#ifdef _DEBUG
			std::cout << "ここがゴールわよ\n";
#endif
			completed = true;
		}

	}
}

void Goal::init()
{
	completed = false;
}

bool Goal::is_completed()
{
	return completed;
}
