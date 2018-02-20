#pragma once

#include "plain_square_polygon.h"
#include "player.h"
#include "scalable_object.h"
#include "background.h"
#include "enemy.h"

// SquarePolygonBaseから派生させたクラス同士の当たり判定を纏めます
// 基本的にSquarePolygonBase*のvectorで持ってるので，使う時はキャストする必要がある．まあ，別にそのままやっても問題はないと思うけど．
// というか当たり判定のとり方をイマイチわかっておらんので，助けて

// なんでポインタなんだ？？？？？

// 基底 : 基底
bool is_collision(SquarePolygonBase *_a, SquarePolygonBase *_b);

// 基底 :　拡縮可ポリゴン
bool is_collision(SquarePolygonBase * _a, ScalableObject * _b);

// coordsとsizeから取る

enum HitLine {
	NONE = 0x00,
	TOP = 0x01,
	BOTTOM = 0x02,
	LEFT = 0x03,
	RIGHT = 0x04
};

bool is_collision(const PlainSquarePolygon _a, const PlainSquarePolygon _b);

enum HitLine where_collision(SquarePolygonBase *_a, SquarePolygonBase *_b);