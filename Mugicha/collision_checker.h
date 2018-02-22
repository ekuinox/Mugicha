#pragma once

#include "plain_square_polygon.h"
#include "player.h"
#include "scalable_object.h"
#include "background.h"
#include "enemy.h"

enum HitLine {
	NONE = 0x00,
	TOP = 0x01,
	BOTTOM = 0x02,
	LEFT = 0x04,
	RIGHT = 0x08
};

// SquarePolygonBaseから派生させたクラス同士の当たり判定を纏めます
// 基本的にSquarePolygonBase*のvectorで持ってるので，使う時はキャストする必要がある．まあ，別にそのままやっても問題はないと思うけど．
// というか当たり判定のとり方をイマイチわかっておらんので，助けて

bool is_collision(float a_x1, float a_x2, float b_x1, float b_x2, float a_y1, float a_y2, float b_y1, float b_y2);

// 基底 : 基底
bool is_collision(SquarePolygonBase *_self, SquarePolygonBase *_another);

// SQUARE vs SQUARE
bool is_collision(SQUARE _self, SQUARE _another);

// coordsとsizeから取る
bool is_collisionA(SquarePolygonBase *_self, SquarePolygonBase *_another);

// どこに当たったかを拾います _aのどの辺に対してか
enum HitLine where_collision(SquarePolygonBase *_self, SquarePolygonBase *_another);