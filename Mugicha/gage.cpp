#include "gage.h"

Gage::Gage(float _x, float _y, LPDIRECT3DTEXTURE9 _tex)
	: AlwaysDisplayedPolygon(_x, _y, GAGE_WIDTH, GAGE_HEIGHT, _tex, 0, 0, 0, 1, 0.5f), current_cost(cost_gage_max)
{
}

void Gage::update()
{
	if (!status) return; // statusみて切る

	auto current = std::chrono::system_clock::now();

	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_update).count() > UPDATE_INTERVAL) // 1ms間隔で
	{
		current_cost += cost_recovery_speed;

		if (current_cost > cost_gage_max) current_cost = cost_gage_max;

		// costに合わせてu,v値を変えてやる

		v = current_cost / 4;

		latest_update = current;
	}
}

float Gage::consume()
{
	if (current_cost < cost)
	{
		return -1;
	}
	else
	{
		current_cost - cost;
		return current_cost;
	}
}
