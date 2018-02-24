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
#define REGISTER_ENEMY REGISTER_BLOCK
#define REGISTER_BLOCK(x, y, tex, camera) new ScalableObject(x, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera)
#define REGISTER_RAGGED_FLOOR(x, y, tex, camera, player) new RaggedFloor(x, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player)
#define REGISTER_GOAL(x, y, tex, camera, player) new Goal(x, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player)
#define REGISTER_THORNS(x, y, tex, camera, player, vec) new Thorns(x, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player, vec)
#define REGISTER_THORNS_UP(x, y, tex, camera, player) new Thorns(x, y - 1, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player, Thorns::Vec::UP)
#define REGISTER_THORNS_DOWN(x, y, tex, camera, player) new Thorns(x, y + 1, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player, Thorns::Vec::DOWN)
#define REGISTER_THORNS_LEFT(x, y, tex, camera, player) new Thorns(x + 1, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player, Thorns::Vec::LEFT)
#define REGISTER_THORNS_RIGHT(x, y, tex, camera, player) new Thorns(x - 1, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player, Thorns::Vec::RIGHT)
#define REGISTER_THORNS_WITH_FLOOR(x, y, tex, camera, player, vec, floor) new Thorns(x, y, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player, vec, floor)
#define REGISTER_AIRCANNON(x, y, tex, camera, floor, player) REGISTER_BLOCK(x, y, tex, camera) // 空気砲はまだ作れておらず
#define REGISTER_GIMMICK REGISTER_BLOCK
#define REGISTER_SWITCH REGISTER_BLOCK
#define REGISTER_MAGMA REGISTER_BLOCK

// 横長なモノ

#define REGISTER_RAGGED_FLOOR_1(x, y, tex, camera, player) new RaggedFloor(x, y, CELL_WIDTH * 2, CELL_HEIGHT, tex, 1, camera, player)

// 0から通しで
#define REGISTER_BLOCK_NUMBER_0 nullptr
#define REGISTER_BLOCK_NUMBER_1 REGISTER_PLAYER
#define REGISTER_BLOCK_NUMBER_2 REGISTER_ENEMY
#define REGISTER_BLOCK_NUMBER_3 REGISTER_GOAL
#define REGISTER_BLOCK_NUMBER_4 REGISTER_BLOCK
#define REGISTER_BLOCK_NUMBER_5 REGISTER_BLOCK
#define REGISTER_BLOCK_NUMBER_6 REGISTER_BLOCK
#define REGISTER_BLOCK_NUMBER_7 REGISTER_BLOCK

#define REGISTER_BLOCK_NUMBER_11 REGISTER_THORNS
#define REGISTER_BLOCK_NUMBER_12 REGISTER_THORNS
#define REGISTER_BLOCK_NUMBER_13 REGISTER_THORNS
#define REGISTER_BLOCK_NUMBER_14 REGISTER_THORNS
#define REGISTER_BLOCK_NUMBER_15 REGISTER_THORNS
#define REGISTER_BLOCK_NUMBER_16 REGISTER_THORNS

#define REGISTER_BLOCK_NUMBER_21 REGISTER_AIRCANNON
#define REGISTER_BLOCK_NUMBER_22 REGISTER_AIRCANNON
#define REGISTER_BLOCK_NUMBER_23 REGISTER_AIRCANNON
#define REGISTER_BLOCK_NUMBER_24 REGISTER_AIRCANNON

#define REGISTER_BLOCK_NUMBER_31 REGISTER_GIMMICK
#define REGISTER_BLOCK_NUMBER_32 REGISTER_SWITCH
#define REGISTER_BLOCK_NUMBER_33 REGISTER_MAGMA

