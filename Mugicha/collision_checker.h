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

// 上が当たったかだけを
bool hit_top(SQUARE &_self, SQUARE &_another, float sugar = 0.0f);

// 下が当たったかだけを
bool hit_bottom(SQUARE &_self, SQUARE &_another, float sugar = 0.0f);

// 右が当たったかだけを
bool hit_right(SQUARE &_self, SQUARE &_another, float sugar = 0.0f);

// 左が当たったかだけを
bool hit_left(SQUARE &_self, SQUARE &_another, float sugar = 0.0f);

// selfに甘さを追加する
enum HitLine where_collision(SquarePolygonBase *_self, SquarePolygonBase *_another, float sugar = 0.0f, bool without_precheck = true);

// どこに当たったかを拾います _aのどの辺に対してか
enum HitLine where_collision(SquarePolygonBase *_self, SquarePolygonBase *_another);

// プレイヤとSPBで
enum HitLine where_collision(Player *_self, SquarePolygonBase *_another);