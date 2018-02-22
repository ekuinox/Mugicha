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
	}

	// プレイヤがゴールしているか
	if (goal->is_completed())
	{
		info.status = clear;
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
	/*
	std::vector<std::vector<std::string>> table;

	if (!(csv_loader(filepath, table))) return; // csvの読み込みに失敗
	if (table[0].size() != 2) return; // 一行目にはw,hにしておいて欲しいので

	// すべて無にする　後のことは考えてない　すみません
	std::map<enum PolygonTypes, polygon_vec>().swap(polygons);
	
	// マップのサイズを入れたい
	auto map_size = POLSIZE(std::atoi(table[0][0].c_str()) * CELL_WIDTH, std::atoi(table[0][1].c_str()) * CELL_HEIGHT);
	
	for (auto i = 1; i < table.size(); ++i)
	{
		for (auto j = 0; j < table[i].size(); ++j)
		{
			// ここで登録をキメていく
			auto num = std::atoi(table[i][j].c_str());
			
			// やること
			// x, yを計算して出し，マクロ呼出しでポリゴンを登録する．

			switch (num)
			{
			case 0: // 空白である
				break;
			case 1: // 地面
				break;
			case 2: // 落ちる床
				break;
			case 3: // スタート位置 プレイヤの初期位置である
				break;
			case 4: // ゴール位置
				break;
			case 5: // トゲの配置
				break;
			case 6: // 落下する種のトゲ
				break;
			case 7: // 空気砲
				break;
			}
		}
	}
	*/

	// 背景の登録
	background = push_polygon_back(BACKGROUND, new Background(textures["BACKGROUND"], &camera));
	background->on();

	// プレイヤの登録
	player = push_polygon_back(PLAYER, REGISTER_PLAYER(300, 50, textures["PLAYER"], &camera, polygons));
	player->on();

	// 拡縮できるオブジェクトを登録
	polygons[SCALABLE_OBJECT].push_back(REGISTER_BLOCK(25, 25, textures["SAMPLE1"], &camera));
	polygons[SCALABLE_OBJECT].back()->on();

	polygons[SCALABLE_OBJECT].push_back(REGISTER_BLOCK(100, 100, textures["SAMPLE1"], &camera));
	polygons[SCALABLE_OBJECT].back()->on();

	// 落ちる床を追加します
	auto floor = push_polygon_back(SCALABLE_OBJECT, REGISTER_RAGGED_FLOOR_1(200, 200, textures["BLOCK2"], &camera, player));
	floor->on();

	// トゲ
	push_polygon_back(SCALABLE_OBJECT, REGISTER_THORNS(200, 0, textures["ORIGIN"], &camera, floor, player, true))->on();
	push_polygon_back(SCALABLE_OBJECT, REGISTER_THORNS(200, 0, textures["ORIGIN"], &camera, floor, player, false))->on();

	// ゴール
	(goal = push_polygon_back(GOAL, REGISTER_GOAL(700, 25, textures["ORIGIN"], &camera, player)))->on();

#ifdef _DEBUG

	// 原点
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 20, 20, textures["ORIGIN"], 2, &camera));
	polygons[PLAIN].back()->on();

	// x軸ガイドライン
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 100000, 10, textures["BLOCK2"], 3, &camera));
	polygons[PLAIN].back()->on();

	// y軸ガイドライン
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 10, 100000, textures["BLOCK2"], 3, &camera));
	polygons[PLAIN].back()->on();

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
	camera.x = polygons[PLAYER][0]->get_coords().x;
	camera.y = polygons[PLAYER][0]->get_coords().y + 200; // プレイヤからのカメラの高さ，同じじゃなんか変だと思う
	
	// 画面外は見せないようにする
	if (camera.x < BACKGROUND_X) camera.x = BACKGROUND_X;
	if (camera.y < BACKGROUND_X) camera.y = BACKGROUND_X;

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
	polygons[type].push_back(polygon);

	return polygon;
}
