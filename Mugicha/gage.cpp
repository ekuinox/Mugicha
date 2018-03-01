#include "gage.h"

Gage::Gage(float _x, float _y, LPDIRECT3DTEXTURE9 _tex)
	: AlwaysDisplayedPolygon(_x, _y, GAGE_WIDTH, GAGE_HEIGHT, _tex, 1.0f / 3.0f * cost_gage_max, 0, 0, 1, 1.0f / 3.0f), current_cost(cost_gage_max)
{
}

void Gage::update()
{
	if (!status) return; // status‚İ‚ÄØ‚é

	auto current = std::chrono::system_clock::now();

	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_update).count() > 1000)
	{
		current_cost += cost_recovery_speed;

		if (current_cost > cost_gage_max) current_cost = cost_gage_max;

		v = current_cost / 3.0f;
		
		// cost‚É‡‚í‚¹‚Äu,v’l‚ğ•Ï‚¦‚Ä‚â‚é
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
		current_cost -= cost;
		return current_cost;
	}
}
