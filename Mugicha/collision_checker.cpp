#include "collision_checker.h"

bool is_collision(float a_x1, float a_x2, float b_x1, float b_x2, float a_y1, float a_y2, float b_y1, float b_y2)
{
	return
		a_x1 <= b_x2
		&& a_x2 >= b_x1
		&& a_y1 <= b_y2
		&& a_y2 >= b_y1
		? true : false;
}

bool is_collision(SquarePolygonBase * _a, SquarePolygonBase * _b)
{
	auto a = _a->get_vertexes();
	auto b = _b->get_vertexes();

	return is_collision(a[0].x, a[1].x, b[0].x, b[1].x, a[0].y, a[2].y, b[0].y, b[2].y);

}

bool is_collisionA(SquarePolygonBase * _a, SquarePolygonBase * _b)
{
	auto a = _a->get_square();
	auto b = _b->get_square();
	return is_collision(
		a.x - a.w / 2, a.x + a.w / 2,
		b.x - b.w / 2, b.x + b.w / 2,
		a.y - a.h / 2, a.y + a.h / 2,
		b.y - b.h / 2, b.y + b.h / 2
		);
}

HitLine where_collision(SquarePolygonBase *_a, SquarePolygonBase *_b)
{
	char result = 0x00;
	if (is_collisionA(_a, _b))
	{
		auto a = _a->get_vertexes();
		auto b = _b->get_vertexes();

		if (a[1].x <= b[0].x) result |= RIGHT;
		if (a[0].x >= b[1].x) result |= LEFT;

		if (a[0].y >= b[2].y) result |= TOP;
		if (a[0].y <= b[2].y) result |= BOTTOM;
	}

	return static_cast<HitLine>(result);
}

