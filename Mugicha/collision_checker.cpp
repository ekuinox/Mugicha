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

bool is_collision(SquarePolygonBase * _self, SquarePolygonBase * _another)
{
	auto another = _self->get_vertexes();
	auto self = _another->get_vertexes();

	return is_collision(another[0].x, another[1].x, self[0].x, self[1].x, another[0].y, another[2].y, self[0].y, self[2].y);

}

bool is_collision(SQUARE _self, SQUARE _another)
{
	return is_collision(
		_self.x - _self.w / 2, _self.x + _self.w / 2,
		_another.x - _another.w / 2, _another.x + _another.w / 2,
		_self.y - _self.h / 2, _self.y + _self.h / 2,
		_another.y - _another.h / 2, _another.y + _another.h / 2
	);
}

bool is_collisionA(SquarePolygonBase * _self, SquarePolygonBase * _another)
{
	return is_collision(_self->get_square(), _another->get_square());
}

HitLine where_collision(SquarePolygonBase * _self, SquarePolygonBase * _another, float sugar, bool without_precheck)
{
	char result = 0x00;
	if (without_precheck || is_collisionA(_self, _another))
	{
		auto self = _self->get_square();
		auto another = _another->get_square();

		if (
			another.left() < self.left()
			&& another.right() > self.left()
			&& another.top() > self.bottom() + sugar
			&& another.bottom() < self.top() - sugar
			) result |= LEFT;


		if (another.left() < self.right()
			&& another.right() > self.right()
			&& another.top() > self.bottom() + sugar
			&& another.bottom() < self.top() - sugar
			) result |= RIGHT;

		if (another.top() > self.top()
			&& another.bottom() < self.top()
			&& another.left() < self.right() - sugar
			&& another.right() > self.left() + sugar
			) result |= TOP;

		if (another.top() > self.bottom()
			&& another.bottom() < self.bottom()
			&& another.left() < self.right() - sugar
			&& another.right() > self.left() + sugar
			) result |= BOTTOM;
	}

	return static_cast<HitLine>(result);
}

HitLine where_collision(SquarePolygonBase *_self, SquarePolygonBase *_another)
{
	return where_collision(_self, _another, 0.0f);
}

HitLine where_collision(Player * _self, SquarePolygonBase * _another)
{
	return where_collision(_self, _another, 1.0f);
}

