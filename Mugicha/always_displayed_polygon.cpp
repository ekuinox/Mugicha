#include "always_displayed_polygon.h"

void AlwaysDisplayedPolygon::generate_vertexes()
{
	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = VERTEX_2D(
			x + w / (i % 3 == 0 ? -2 : 2),
			y + h / (i < 2 ? -2 : 2),
			u + (i % 3 == 0 ? 0 : uw),
			v + (i < 2 ? 0 : vh)
		);
	}
}
