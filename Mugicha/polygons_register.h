#pragma once

#include "plain_square_polygon.h"
#include "player.h"
#include "background.h"
#include "scalable_object.h"
#include "enemy.h"
#include "ragged_floor.h"
#include "thorns.h"
#include "goal.h"

// CELL_WIDTH * CELL_HEIGHTな1コマサイズのポリゴンレジスタマクロ

#define REGISTER_BACKGROUND(x, y, tex, camera, polygons) new Background(x, y, w, h, tex, camera)
#define REGISTER_PLAYER(x, y, tex, camera, polygons) new Player(tex, camera, polygons, 0, x, y, CELL_WIDTH, CELL_HEIGHT)
#define REGISTER_BLOCK(x, y, tex, camera) new ScalableObject(x, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera)
#define REGISTER_RAGGED_FLOOR(x, y, tex, camera, player) new RaggedFloor(x, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player)
#define REGISTER_GOAL(x, y, tex, camera, player) new Goal(x, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player)
#define REGISTER_THORNS(x, y, tex, camera, player, vec) new Thorns(x, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player, vec)
#define REGISTER_THORNS_WITH_FLOOR(x, y, tex, camera, player, vec, floor) new Thorns(x, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player, vec, floor)
#define REGISTER_CANNON(x, y, tex, camera, floor, player) REGISTER_BLOCK(x, y, tex, camera) // 空気砲はまだ作れておらず

// 横長なモノ

#define REGISTER_RAGGED_FLOOR_1(x, y, tex, camera, player) new RaggedFloor(x, y, CELL_WIDTH * 2, CELL_HEIGHT, tex, 1, camera, player)