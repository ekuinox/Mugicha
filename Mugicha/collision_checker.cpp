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

bool is_collisionA(SquarePolygonBase * _self, SquarePolygonBase * _another)
{
	auto self = _self->get_square();
	auto another = _another->get_square();
	return is_collision(
		self.x - self.w / 2, self.x + self.w / 2,
		another.x - another.w / 2, another.x + another.w / 2,
		self.y - self.h / 2, self.y + self.h / 2,
		another.y - another.h / 2, another.y + another.h / 2
	);
}

HitLine where_collision(SquarePolygonBase *_self, SquarePolygonBase *_another)
{
	char result = 0x00;
	if (is_collisionA(_self, _another))
	{
		auto self = _self->get_vertexes();
		auto another = _another->get_vertexes();

		if (self[1].x <= another[0].x) result |= RIGHT;
		if (self[0].x >= another[1].x) result |= LEFT;

		if (self[0].y > another[2].y) result |= TOP;
		if (self[0].y < another[2].y) result |= BOTTOM;
		
		/*
		auto self = _self->get_square();
		auto another = _another->get_square();

		if (self.x + self.w / 2 <= another.x - another.w / 2) result |= RIGHT;
		if (self.x - self.w / 2 >= another.x + another.w / 2) result |= LEFT;

		if (self.y - self.h / 2 >= another.y + another.h / 2) result |= TOP;
		if (self.y - self.h / 2 <= another.y + another.h / 2) result |= BOTTOM;
		*/
		
		/*
		if (self.x + self.w / 2 > another.x - another.w / 2) result |= RIGHT; // SELF‚Ì‰E‚ÆANOTHER‚Ì¶‚ð”äŠr‚·‚é >=
		if (self.x - self.w / 2 < another.x + another.w / 2) result |= LEFT; // SELF‚Ì¶‚ÆANOTHER‚Ì‰E‚ð”äŠr‚·‚é <=
		if (self.y + self.h / 2 > another.y - another.h / 2) result |= TOP; // SELF‚Ìã‚ÆANOTHER‚Ì‰º‚ð”äŠr‚·‚é >=
		if (self.y - self.h / 2 < another.y + another.h / 2) result |= BOTTOM; // SELF‚Ì‰º‚ÆANOTHER‚Ìã‚ð”äŠr‚·‚é <=
		*/
	}

	return static_cast<HitLine>(result);
}

