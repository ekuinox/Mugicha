#pragma once

#include "polygonsquare.h"
#include "player.h"
#include "scalable_object.h"


// SquarePolygonBaseから派生させたクラス同士の当たり判定を纏めます
// 基本的にSquarePolygonBase*のvectorで持ってるので，使う時はキャストする必要がある．まあ，別にそのままやっても問題はないと思うけど．
// というか当たり判定のとり方をイマイチわかっておらんので，助けて

// 基底 : 基底
bool is_collision(SquarePolygonBase* _a, SquarePolygonBase* _b);

// 基底 :　拡縮可ポリゴン
bool is_collision(SquarePolygonBase * _a, ScalableObject * _b);