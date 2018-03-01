#pragma once

#include "plain_square_polygon.h"
#include "player.h"
#include "background.h"
#include "scalable_object.h"
#include "enemy.h"
#include "ragged_floor.h"
#include "thorn.h"
#include "goal.h"
#include "stage_background.h"
#include "magma.h"
#include "air_cannon.h"
#include "item.h"
#include "gimmick_switch.h"
#include "always_displayed_polygon.h"
#include "gage.h"
#include "hellgate.h"

// CELL_WIDTH * CELL_HEIGHTな1コマサイズのポリゴンレジスタマクロ

// いろいろ見づらすぎるしんどい

#define REGISTER_BACKGROUND(x, y, w, h, tex, camera) new Background(static_cast<float>(x), static_cast<float>(y), w, h, tex, camera)

#define REGISTER_PLAYER(x, y, tex, camera, polygons) new Player(tex, camera, polygons, 0, static_cast<float>(x), static_cast<float>(y), PLAYER_WIDTH, PLAYER_HEIGHT)
#define REGISTER_ENEMY(x, y, tex, camera, polygons, vec) new Enemy(tex, camera, 1, static_cast<float>(x), static_cast<float>(y), CELL_WIDTH, CELL_HEIGHT, vec, polygons)
#define REGISTER_ENEMY_LEFT(x, y, tex, camera, polygons) REGISTER_ENEMY(static_cast<float>(x), static_cast<float>(y), tex, camera, polygons, Enemy::Vec::LEFT)
#define REGISTER_ENEMY_RIGHT(x, y, tex, camera, polygons) REGISTER_ENEMY(static_cast<float>(x), static_cast<float>(y), tex, camera, polygons, Enemy::Vec::RIGHT)

#define REGISTER_BLOCK(x, y, tex, camera) new ScalableObject(static_cast<float>(x), static_cast<float>(y), CELL_WIDTH, CELL_HEIGHT, tex, 1, camera)

#define REGISTER_HALF_BLOCK(x, y, tex, camera, player) new ScalableObject(static_cast<float>(x), static_cast<float>(y), CELL_WIDTH, CELL_HEIGHT / 2, tex, 1, camera)
#define REGISTER_HALF_WALL(x, y, tex, camera, player) new ScalableObject(static_cast<float>(x), static_cast<float>(y), CELL_WIDTH / 2, CELL_HEIGHT, tex, 1, camera)

#define REGISTER_RAGGED_FLOOR(x, y, tex, camera, player) new RaggedFloor(static_cast<float>(x), static_cast<float>(y), CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player)

#define REGISTER_GOAL(x, y, tex, camera, player) new Goal(static_cast<float>(x), static_cast<float>(y), CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, player)

#define REGISTER_THORN(x, y, tex, camera, vec) new Thorn(static_cast<float>(x), static_cast<float>(y), CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, vec)
#define REGISTER_THORN_HALF(x, y, tex, camera, vec) new Thorn(static_cast<float>(x), static_cast<float>(y), CELL_WIDTH, CELL_HEIGHT / 2, tex, 1, camera, vec)
#define REGISTER_THORN_UP(x, y, tex, camera) REGISTER_THORN(static_cast<float>(x), static_cast<float>(y) - 1, tex, camera, Thorn::Vec::UP)
#define REGISTER_THORN_UP_HALF(x, y, tex, camera) REGISTER_THORN_HALF(static_cast<float>(x), static_cast<float>(y) - 1, tex, camera, Thorn::Vec::UP)
#define REGISTER_THORN_DOWN(x, y, tex, camera) REGISTER_THORN(static_cast<float>(x), static_cast<float>(y) + 1, tex, camera, Thorn::Vec::DOWN)
#define REGISTER_THORN_DOWN_HALF(x, y, tex, camera) REGISTER_THORN_HALF(static_cast<float>(x), static_cast<float>(y) + 1, tex, camera, Thorn::Vec::DOWN)
#define REGISTER_THORN_LEFT(x, y, tex, camera) REGISTER_THORN(static_cast<float>(x) + 1, static_cast<float>(y), tex, camera, Thorn::Vec::LEFT)
#define REGISTER_THORN_RIGHT(x, y, tex, camera) REGISTER_THORN(static_cast<float>(x) + 1, static_cast<float>(y), tex, camera, Thorn::Vec::RIGHT)
#define REGISTER_THORN_FALL_DOWN(x, y, tex, camera) new Thorn(static_cast<float>(x), static_cast<float>(y) + 1, CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, Thorn::Vec::DOWN, true)
#define REGISTER_THORN_WITH_FLOOR(x, y, tex, camera, vec, floor) new Thorn(static_cast<float>(x), static_cast<float>(y), CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, vec, floor)

#define REGISTER_AIRCANNON(x, y, tex, bul_tex, camera, vec) new AirCannon(static_cast<float>(x), static_cast<float>(y), CELL_WIDTH, CELL_HEIGHT, tex, bul_tex, 1, camera, vec)
#define REGISTER_AIRCANNON_UP(x, y, tex, bul_tex, camera) REGISTER_AIRCANNON(static_cast<float>(x), static_cast<float>(y), tex, bul_tex, camera, AirCannon::Vec::UP)
#define REGISTER_AIRCANNON_DOWN(x, y, tex, bul_tex, camera) REGISTER_AIRCANNON(static_cast<float>(x), static_cast<float>(y), tex, bul_tex, camera, AirCannon::Vec::DOWN)
#define REGISTER_AIRCANNON_LEFT(x, y, tex, bul_tex, camera) REGISTER_AIRCANNON(static_cast<float>(x), static_cast<float>(y), tex, bul_tex, camera, AirCannon::Vec::LEFT)
#define REGISTER_AIRCANNON_RIGHT(x, y, tex, bul_tex, camera) REGISTER_AIRCANNON(static_cast<float>(x), static_cast<float>(y), tex, bul_tex, camera, AirCannon::Vec::RIGHT)

#define REGISTER_ITEM(x, y, tex, camera, polygons) new Item(static_cast<float>(x), static_cast<float>(y), CELL_WIDTH, CELL_HEIGHT, tex, 1, camera, polygons)

#define REGISTER_GIMMICK_SWITCH(x, y, tex, camera, flag) new GimmickSwitch(static_cast<float>(x), static_cast<float>(y - 5), CELL_WIDTH -10, CELL_HEIGHT - 10, tex, 1, camera, flag)

#define REGISTER_MAGMA(x, y, tex, camera) new Magma(static_cast<float>(x), static_cast<float>(y) - 5, CELL_WIDTH, CELL_HEIGHT - 10, tex, 1, camera)

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

#define REGISTER_BLOCK_NUMBER_11 REGISTER_THORN
#define REGISTER_BLOCK_NUMBER_12 REGISTER_THORN
#define REGISTER_BLOCK_NUMBER_13 REGISTER_THORN
#define REGISTER_BLOCK_NUMBER_14 REGISTER_THORN
#define REGISTER_BLOCK_NUMBER_15 REGISTER_THORN
#define REGISTER_BLOCK_NUMBER_16 REGISTER_THORN

#define REGISTER_BLOCK_NUMBER_21 REGISTER_AIRCANNON
#define REGISTER_BLOCK_NUMBER_22 REGISTER_AIRCANNON
#define REGISTER_BLOCK_NUMBER_23 REGISTER_AIRCANNON
#define REGISTER_BLOCK_NUMBER_24 REGISTER_AIRCANNON

#define REGISTER_BLOCK_NUMBER_31 REGISTER_GIMMICK_SWITCH
#define REGISTER_BLOCK_NUMBER_32 REGISTER_GIMMICK_SWITCH
#define REGISTER_BLOCK_NUMBER_33 REGISTER_MAGMA

