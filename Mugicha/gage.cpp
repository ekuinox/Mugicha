#include "gage.h"
#include "helpers.h"

Gage::Gage(LPDIRECT3DTEXTURE9 _tex)
	: AlwaysDisplayedPolygon(GAGE_X, GAGE_Y, GAGE_WIDTH, GAGE_HEIGHT, _tex, 1.0f / 3.0f * cost_gage_max, 0, 0, 1, 1.0f / 3.0f), current_cost(cost_gage_max)
{
}

void Gage::update()
{
	if (!status) return; // status‚Ý‚ÄØ‚é

	auto current = SCNOW;
	if (time_diff(latest_update, current) > UPDATE_INTERVAL)
	{
		latest_update = current;

		current_cost += cost_recovery_speed;

		if (current_cost > cost_gage_max) current_cost = cost_gage_max;

		v = current_cost / 3.0f;
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

bool Gage::can_consume()
{
	return current_cost >= cost;
}

#if defined(_DEBUG) || defined(_STAGE_DEBUG)
void Gage::cure()
{
	current_cost = cost_gage_max;
}
#endif
