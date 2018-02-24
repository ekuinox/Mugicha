#include "stage.h"
#include "collision_checker.h"
#include "csv_loader.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <typeinfo>

// コンストラクタ
Stage::Stage()
{
	latest_update = timeGetTime();
	latest_draw = timeGetTime();
	info.status = prep;

	init();
}

Stage::Stage(char _stage_select)
	: latest_update(timeGetTime()), latest_draw(timeGetTime()), info(0, prep, _stage_select)
{
	init();
}

// デストラクタ
Stage::~Stage()
{
	for (auto tex : textures)
	{
		if (tex.second)
		{
			tex.second->Release();
			tex.second = NULL;
		}
	}
}

// ここを叩いてステージを更新したり描画したりする
GameInfo Stage::exec()
{
	update();
	draw();

	// いろいろな続行判定をする

	// プレイヤが生きているかでゲームの続行を判定
	if (player->dead())
	{
		info.status = failed;
#ifdef _DEBUG
		printf("アカンしんでもた！\n");
#endif
	}

	// プレイヤがゴールしているか
	if (goal->is_completed())
	{
		info.status = clear;
#ifdef _DEBUG
		printf("ゲームクリアー！\n");
#endif
	}

	return info;
}

void Stage::multi_texture_loader(std::map<std::string, const char *> _textures)
{
	for (auto _texture : _textures)
	{
		D3DXCreateTextureFromFile(d3d_device, _texture.second, &textures[_texture.first]); // 一斉読み込み
	}
}

void Stage::multi_texture_loader(const char * filepath)
{
	std::vector<std::vector<std::string>> table;

	if (!(csv_loader(filepath, table))) return;

	for (const auto& record : table)
	{
		if (record.size() == 2)
		{
			char texture_file[256];
			sprintf(texture_file, "%s%s", TEXTURES_DIR, record[1].c_str());
			if (FAILED(D3DXCreateTextureFromFile(d3d_device, texture_file, &textures[record[0]])))
			{
#ifdef _DEBUG
				printf("Failed to load texture file: %s\n", record[1].c_str());
#endif
			}
		}
	}

}

void Stage::stagefile_loader(const char * filepath)
{
	// TODO: ステージファイルからのローダを作る
	
	std::vector<std::vector<std::string>> table;

	if (!(csv_loader(filepath, table))) return; // csvの読み込みに失敗
	if (table[0].size() != 4) return; // 一行目にはw,hにしておいて欲しいので

	// すべて無にする　後のことは考えてない　すみません
	std::map<enum PolygonTypes, polygon_vec>().swap(polygons);
	
	// マップのサイズを入れたい
	map_size = POLSIZE(std::atoi(table[0][0].c_str()) * CELL_WIDTH, std::atoi(table[0][1].c_str()) * CELL_HEIGHT);
	
	// 背景の登録
	(background = push_polygon_back(BACKGROUND, new Background(map_size.w / 2, map_size.h / 2, map_size.w, map_size.h, textures["BACKGROUND"], &camera)))->on();

	// プレイヤの登録
	// プレイヤは先に登録しておかないと後々だるいです
	(player = push_polygon_back(PLAYER, REGISTER_PLAYER(std::atoi(table[0][2].c_str()) * CELL_WIDTH - CELL_WIDTH / 2, std::atoi(table[0][3].c_str()) * CELL_HEIGHT - CELL_HEIGHT / 2, textures["PLAYER"], &camera, polygons)))->on();

	for (auto i = map_size.h / CELL_HEIGHT; i >= 1; --i) // ケツから一番上まで
	{
		for (auto j = 0; j < map_size.w / CELL_WIDTH; ++j) // 余計な要素まで読み込まないように
		{
			// ここで登録をキメていく
			auto num = std::atoi(table[i][j].c_str());
			
			// やること
			// x, yを計算して出し，マクロ呼出しでポリゴンを登録する．

			switch (num)
			{
			case 0:
				break;
			case 1:
				// ここでプレイヤを初期化するのはやめます．
			//	(player = push_polygon_back(PLAYER, REGISTER_PLAYER(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["PLAYER"], &camera, polygons)))->on();
				break;
			case 2:
				break;
			case 3:
				(goal = push_polygon_back(GOAL, REGISTER_GOAL(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ORIGIN"], &camera, player)))->on();
				break;
			case 4:
				push_polygon_back(SCALABLE_OBJECT, REGISTER_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["SAMPLE1"], &camera))->on();
			//	push_polygon_back(RAGGED_FLOOR, REGISTER_RAGGED_FLOOR(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["BLOCK2"], &camera, player))->on();
				break;
			case 5:
			//	push_polygon_back(SCALABLE_OBJECT, REGISTER_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["SAMPLE1"], &camera))->on();
				push_polygon_back(RAGGED_FLOOR, REGISTER_RAGGED_FLOOR(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["BLOCK2"], &camera, player))->on();
				break;
			case 6:
				break;
			case 7:
				break;
			case 11:
				push_polygon_back(THORNS, REGISTER_THORNS_DOWN(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ORIGIN"], &camera, player))->on();
				break;
			case 12:
				push_polygon_back(THORNS, REGISTER_THORNS_UP(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ORIGIN"], &camera, player))->on();
				break;
			case 13:
				push_polygon_back(THORNS, REGISTER_THORNS_LEFT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ORIGIN"], &camera, player))->on();
				break;
			case 14:
				push_polygon_back(THORNS, REGISTER_THORNS_RIGHT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ORIGIN"], &camera, player))->on();
				break;
			case 15:
				break;
			case 16:
				break;
			case 21:
				break;
			case 22:
				break;
			case 23:
				break;
			case 24:
				break;
			case 31:
				break;
			case 32:
				break;
			case 33:
				break;
			}
		}
	}

	// トゲの床をセットしていきます
	std::vector<SquarePolygonBase*> floors;
	for (const auto& type : { SCALABLE_OBJECT, ENEMY, RAGGED_FLOOR}) floors.insert(floors.end(), polygons[type].begin(), polygons[type].end());
	for (const auto& thorns : polygons[THORNS]) static_cast<Thorns*>(thorns)->set_floor(floors);

#ifdef _DEBUG

	// 原点
	polygons[DEBUG_GUIDE].push_back(new PlainSquarePolygon(0, 0, 20, 20, textures["ORIGIN"], 2, &camera));
	polygons[DEBUG_GUIDE].back()->on();

	// x軸ガイドライン
	polygons[DEBUG_GUIDE].push_back(new PlainSquarePolygon(0, 0, 100000, 10, textures["BLOCK2"], 3, &camera));
	polygons[DEBUG_GUIDE].back()->on();

	// y軸ガイドライン
	polygons[DEBUG_GUIDE].push_back(new PlainSquarePolygon(0, 0, 10, 100000, textures["BLOCK2"], 3, &camera));
	polygons[DEBUG_GUIDE].back()->on();

#endif
}

// 初期化, コンストラクタから呼ぶ
void Stage::init()
{
	char filepath[256]; // ファイルパス格納

	sprintf(filepath, STAGEFILES_DIR "textures_%02d.csv", info.stage_number);
	multi_texture_loader(filepath); // テクスチャのリストを読み込む => こっち先

	sprintf(filepath, STAGEFILES_DIR "stage_%02d.csv", info.stage_number);
	stagefile_loader(filepath); // ポリゴンファイルパスを指定して読み込む

	zoom_level = { 1, 1 };
	zoom_sign = ZERO;
}

// 更新処理
void Stage::update()
{
	// 時間を気にしないもの

	// タイトルに戻る（無確認）
	if (GetKeyboardTrigger(DIK_1))
	{
		info.status = retire;
		return;
	}

	// 拡縮
	if (zoom_sign == ZERO)
	{
		if (GetKeyboardTrigger(DIK_O) && zoom_level.h < 2.0f) // 拡大
		{
			zoom_sign = PLUS;
			zoom_level_target.w = zoom_level.w * 2;
			zoom_level_target.h = zoom_level.h * 2;
			player->lock();
		}
		if (GetKeyboardTrigger(DIK_L) && zoom_level.w > 0.5f) // 縮小
		{
			zoom_sign = MINUS;
			zoom_level_target.w = zoom_level.w / 2;
			zoom_level_target.h = zoom_level.h / 2;
			player->lock();
		}
	}
	
	// プレイヤをジャンプさせる
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		player->jump();
	}


	// 時間を気にするもの
	auto current = timeGetTime();
	if (current - latest_update < 1) return;
	latest_update = current;

	if (zoom_sign == PLUS)
	{
		if (zoom_level.w < zoom_level_target.w)
		{
			zoom_level.w *= 1.001f;
			zoom_level.h *= 1.001f;
		}
		else
		{
			zoom_level = zoom_level_target;
			zoom_sign = ZERO;
			player->unlock();
		}
	}

	if (zoom_sign == MINUS)
	{
		if (zoom_level.w > zoom_level_target.w)
		{
			zoom_level.w /= 1.001f;
			zoom_level.h /= 1.001f;
		}
		else
		{
			zoom_level = zoom_level_target;
			zoom_sign = ZERO;
			player->unlock();
		}
	}

	// 全ズーム変更
	for (const auto& _polygons : polygons)
	{
		for (const auto& polygon : _polygons.second) polygon->zoom(zoom_level);
	}

	// ここから更新処理
	camera.x = player->get_coords().x;
	camera.y = player->get_coords().y + 200; // プレイヤからのカメラの高さ，同じじゃなんか変だと思う
	
	// 画面外は見せないようにする
	unless(camera.x < map_size.w * zoom_level.w - SCREEN_WIDTH / 2) camera.x = map_size.w * zoom_level.w - SCREEN_WIDTH / 2;
	unless (camera.y < map_size.h * zoom_level.h - SCREEN_HEIGHT / 2) camera.y = map_size.h * zoom_level.h - SCREEN_HEIGHT / 2;
	if (camera.x < SCREEN_WIDTH / 2) camera.x = SCREEN_WIDTH / 2;
	if (camera.y < SCREEN_HEIGHT / 2) camera.y = SCREEN_HEIGHT / 2;

	// ポリゴンの全更新
	for (const auto& _polygons : polygons)
	{
		for (const auto& polygon : _polygons.second) polygon->update();
	}
}

// 描画処理
void Stage::draw()
{
	auto current = timeGetTime();
	if (current - latest_draw < 1000 / 60) return;
	latest_draw = current;

	// ここから描画処理

	// ソート
	polygon_vec drawing_polygons;
	for (const auto& pol_vec : polygons)
	{
		for (const auto& polygon : pol_vec.second)
		{
			drawing_polygons.push_back(polygon);
		}
	}

	// 大きいものが前に来るように
	sort(drawing_polygons.begin(), drawing_polygons.end(), [](const SquarePolygonBase* x, const SquarePolygonBase* y) {return x->layer > y->layer; });
	
	d3d_device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(d3d_device->BeginScene()))
	{
		for (const auto& _polygons : polygons)
		{
			for (const auto& drawing_polygon : drawing_polygons) drawing_polygon->draw();
		}
	}
	d3d_device->EndScene();

	d3d_device->Present(NULL, NULL, NULL, NULL);
}

// polygonsの指定したラベルにポリゴンを追加
template<typename _T>
_T Stage::push_polygon_back(PolygonTypes type, _T polygon)
{
	polygons[type].emplace_back(polygon);

	return polygon;
}
