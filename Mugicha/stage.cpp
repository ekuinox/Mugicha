#include "stage.h"
#include "collision_checker.h"
#include "csv_loader.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <typeinfo>

Stage::Stage(char _stage_select)
	: latest_update(std::chrono::system_clock::now()), latest_draw(SCNOW), info(0, Stage::Status::Prep, _stage_select)
{
	init();
}

Stage::~Stage()
{
	// テクスチャの開放
	for (auto&& tex : textures)
	{
		if (tex.second)
		{
			tex.second->Release();
			tex.second = NULL;
		}
	}

	// ポリゴンの開放
	for (const auto& _polygons : polygons)
		for (const auto& polygon : _polygons.second)
			delete polygon;
}

Stage::GameInfo Stage::exec()
{
	update();
	draw();

	// いろいろな続行判定をする

	// プレイヤが生きているかでゲームの続行を判定
	if (player->dead() != Player::DeadReason::ALIVE)
	{
		info.status = Stage::Status::Failed;
		info.dead_reason = player->dead();
	}

	// プレイヤがゴールしているか
	if (goal->is_completed())
	{
		info.status = Stage::Status::Clear;
	}

	return info;
}

void Stage::multi_texture_loader(std::map<std::string, const char *> _textures)
{
	std::map<std::string, LPDIRECT3DTEXTURE9>().swap(textures); // clear
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
			sprintf_s(texture_file, "%s%s", TEXTURES_DIR, record[1].c_str());
			if (FAILED(D3DXCreateTextureFromFile(d3d_device, texture_file, &textures[record[0]])))
			{
#ifdef _DEBUG
				printf("Failed to load texture file: %s\n", record[1].c_str());
#endif
			}
		}
	}

}

bool Stage::stagefile_loader(const char * filepath)
{
	// TODO: ステージファイルからのローダを作る
	
	std::vector<std::vector<std::string>> table;

	if (!(csv_loader(filepath, table))) return false; // csvの読み込みに失敗
	
	// MAP_WIDTH,MAP_HEIGHT,PLAYER_COORD_X, PLAYER_COORD_Y,HELLGATE_START_X, HELLGATE_START_Y
	if (table[0].size() != 6) return false;

	// すべて無にする　後のことは考えてない　すみません
	PolygonsContainer().swap(polygons);
	
	// マップのサイズを入れたい
	map_size = POLSIZE(static_cast<float>(std::atof(table[0][0].c_str()) * CELL_WIDTH), static_cast<float>(std::atof(table[0][1].c_str()) * CELL_HEIGHT));
	
	// 背景の登録
	emplace_polygon_back(SquarePolygonBase::PolygonTypes::BACKGROUND, new StageBackground(textures["BACKGROUND"], camera))->on();

	// プレイヤの登録
	// プレイヤは先に登録しておかないと後々だるいです
	(player = emplace_polygon_back(SquarePolygonBase::PolygonTypes::PLAYER, REGISTER_PLAYER(std::atof(table[0][2].c_str()) * CELL_WIDTH - CELL_WIDTH / 2, std::atof(table[0][3].c_str()) * CELL_HEIGHT - CELL_HEIGHT / 2, textures["PLAYER"], camera, polygons)))->on();

	// HELLGATEのセット
	(emplace_polygon_back(
		SquarePolygonBase::PolygonTypes::HELLGATE,
		new HellGate(
			map_size.w / 2,
			-1000,
			map_size.w, 300,
			textures["HELLGATE_01"],
			0,
			camera,
			player,
			D3DXVECTOR2(std::atof(table[0][4].c_str()) * CELL_WIDTH - CELL_WIDTH / 2, std::atof(table[0][5].c_str()) * CELL_HEIGHT - CELL_HEIGHT / 2)
		)
	))->on();

	// tmps
	AirCannon* aircannon;

	// ギミックのスイッチに関して
	switches["SAMPLE"] = false;

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
				break;
			case 2:
				// 敵
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::ENEMY,
					REGISTER_ENEMY_LEFT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ENEMY_01"], camera, polygons)
				)->on();
				break;
			case 3:
				// ゴール
				(goal = emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::GOAL,
					REGISTER_GOAL(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["GOAL_01"], camera, player)
				))->on();
				break;
			case 4:
				// 壁床
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT,
					REGISTER_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["FLOOR_01"], camera)
				)->on();
				break;
			case 5:
				// ハーフ床（上）
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT,
					REGISTER_HALF_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.75, textures["FLOOR_01"], camera, player)
				)->on();
				break;
			case 6:
				// ハーフ床（下）
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT,
					REGISTER_HALF_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.25, textures["FLOOR_01"], camera, player)
				)->on();
				break;
			case 7:
				// Switchで出現するなにか
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::GIMMICK_FLOOR,
					REGISTER_GIMMICK_FLOOR(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.25, textures["FLOOR_01"], camera, switches["FLOOR_SWITCH_01"])
				)->on();
				break;
			case 8:
				// 落ちる床
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT,
					REGISTER_RAGGED_FLOOR(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["FLOOR_01"], camera, player)
				)->on();
				break;
			case 9:
				// ハーフ壁（右）
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT,
					REGISTER_HALF_WALL(j * CELL_WIDTH + CELL_WIDTH * 0.75, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["FLOOR_01"], camera, player)
				)->on();
				break;
			case 10:
				// ハーフ壁（左）
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT,
					REGISTER_HALF_WALL(j * CELL_WIDTH + CELL_WIDTH * 0.25, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["FLOOR_01"], camera, player)
				)->on();
				break;
			case 11:
				// トゲ（下）
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::THORN,
					REGISTER_THORN_DOWN(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["THORN_DOWN"], camera, polygons)
				)->on();
				break;
			case 12:
				// トゲ（上）
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::THORN,
					REGISTER_THORN_UP(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["THORN_UP"], camera, polygons)
				)->on();
				break;
			case 13:
				// トゲ（右）
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::THORN,
					REGISTER_THORN_RIGHT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["THORN_RIGHT"], camera, polygons)
				)->on();
				break;
			case 14:
				// トゲ（左）
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::THORN,
					REGISTER_THORN_LEFT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["THORN_LEFT"], camera, polygons)
				)->on();
				break;
			case 15:
				// 壁（上）
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT,
					REGISTER_HALF_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.75, textures["FLOOR_01"], camera, player)
				)->on();
				// トゲ（下）
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::THORN,
					REGISTER_THORN_DOWN_HALF(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.25, textures["THORN_DOWN"], camera, polygons)
				)->on();
				break;
			case 16:
				// トゲ（上）
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::THORN,
					REGISTER_THORN_UP_HALF(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.75, textures["THORN_UP"], camera, polygons)
				)->on();
				// 壁（下）
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT,
					REGISTER_HALF_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.25, textures["FLOOR_01"], camera, player)
				)->on();
				break;
			case 17:
				// トゲ（落ちる）プレイヤに向かって
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::THORN,
					REGISTER_THORN_FALL_DOWN(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["THORN_DOWN"], camera, polygons)
				)->on();
				break;
			case 21:
				// 空気砲（上）
				(aircannon = emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::AIRCANNON,
					REGISTER_AIRCANNON_UP(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["STORM_01"], camera, polygons)
				))->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::KNOCKBACK_BULLET, aircannon->get_bullet());
				break;
			case 22:
				// 空気砲（下）
				(aircannon = emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::AIRCANNON,
					REGISTER_AIRCANNON_DOWN(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["STORM_01"], camera, polygons)
				))->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::KNOCKBACK_BULLET, aircannon->get_bullet());
				break;
			case 23:
				// 空気砲（右）
				(aircannon = emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::AIRCANNON,
					REGISTER_AIRCANNON_RIGHT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["STORM_01"], camera, polygons)
				))->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::KNOCKBACK_BULLET, aircannon->get_bullet());
				break;
			case 24:
				// 空気砲（左）
				(aircannon = emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::AIRCANNON,
					REGISTER_AIRCANNON_LEFT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["STORM_01"], camera, polygons)
				))->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::KNOCKBACK_BULLET, aircannon->get_bullet());
				break;
			case 31:
				// 持てるオブジェクト
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::ITEM,
					REGISTER_ITEM(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["ITEM_01"], camera, polygons)
				)->on();
				break;
			case 32:
				// スイッチ
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::GIMMICK_SWITCH,
					REGISTER_GIMMICK_SWITCH(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["SWITCH_01"], camera, switches["FLOOR_SWITCH_01"])
				)->on();
				break;
			case 33:
				// マグマ
				emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::MAGMA,
					REGISTER_MAGMA(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["MAGMA_01"], camera)
				)->on();
				break;
			case 41:
				// キル（上）
				(aircannon = emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::AIRCANNON,
					REGISTER_AIRCANNON_UP(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera, polygons)
				))->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::BULLET, aircannon->get_bullet());
				break;
			case 42:
				// キル（下）
				(aircannon = emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::AIRCANNON,
					REGISTER_AIRCANNON_DOWN(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera, polygons)
				))->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::BULLET, aircannon->get_bullet());
				break;
			case 43:
				// キル（右）
				(aircannon = emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::AIRCANNON,
					REGISTER_AIRCANNON_RIGHT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera, polygons)
				))->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::BULLET, aircannon->get_bullet());
				break;
			case 44:
				// キル砲（左）
				(aircannon = emplace_polygon_back(
					SquarePolygonBase::PolygonTypes::AIRCANNON,
					REGISTER_AIRCANNON_LEFT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera, polygons)
				))->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::BULLET, aircannon->get_bullet());
				break;
			}
		}
	}

	// トゲの床をセットしていきます
	for (const auto& thorn : polygons[SquarePolygonBase::PolygonTypes::THORN]) static_cast<Thorn*>(thorn)->set_floor();

	// ゲージのセット
	(gage = emplace_polygon_back(SquarePolygonBase::PolygonTypes::GAGE, new Gage(25, 50, textures["GAGE_01"])))->on();

#ifdef _DEBUG // デバッグ用のラインとか
	// 原点
	emplace_polygon_back(SquarePolygonBase::PolygonTypes::DEBUG_GUIDE, new PlainSquarePolygon(0, 0, 20, 20, textures["ORIGIN"], 2, camera))->on();

	// x軸ガイドライン
	emplace_polygon_back(SquarePolygonBase::PolygonTypes::DEBUG_GUIDE, new PlainSquarePolygon(0, 0, 100000, 10, textures["BLOCK2"], 3, camera))->on();

	// y軸ガイドライン
	emplace_polygon_back(SquarePolygonBase::PolygonTypes::DEBUG_GUIDE, new PlainSquarePolygon(0, 0, 10, 100000, textures["BLOCK2"], 3, camera))->on();
#endif

	// layerに合わせてソート
	sort_pols();
	draw_pols_length = draw_pols.size();

	return true;
}

void Stage::init()
{
#ifdef _DEBUG
	const auto exec_start = SCNOW;
	std::cout << "Stage Loading Started..." << std::endl;
#endif

	char filepath[256]; // ファイルパス格納

	sprintf_s(filepath, STAGEFILES_DIR "textures_%02d.csv", info.stage_number);
	multi_texture_loader(filepath); // テクスチャのリストを読み込む => こっち先

	sprintf_s(filepath, STAGEFILES_DIR "stage_%02d.csv", info.stage_number);
	auto exec_result = stagefile_loader(filepath); // ポリゴンファイルパスを指定して読み込む

	zoom_level = 1.0f;
	zoom_sign = Stage::Sign::ZERO;

	if (exec_result) info.status = Stage::Status::Ready;
	else info.status = Stage::Status::LoadError;

	audiocontroller = new AudioController({
		{"AUDIO_01", { AUDIOS_DIR "akumu.wav", true}}
	});

	audiocontroller->play("AUDIO_01");


#ifdef _DEBUG
	std::cout << "Stage Load Time: ";
	std::cout << time_diff(exec_start) << std::endl;
#endif
}

void Stage::update()
{
	// 時間を気にしないもの

	// タイトルに戻る（無確認）
	if (GetKeyboardTrigger(DIK_F5))
	{
		info.status = Stage::Status::Retire;
		return;
	}

	// 時間を気にしないタイプの操作をここで呼ぶ
	trigger_controlls();

#ifdef _DEBUG
	
	// プレイヤの位置とかを吐かせる
	if (GetKeyboardTrigger(DIK_F2)) printf("Camera: (%f, %f) Player: (%f, %f)\n", camera.x, camera.y, player->get_coords().x, player->get_coords().y);

	// ゲージ全回復じゃ～い
	if (GetKeyboardTrigger(DIK_F3)) gage->cure();

#endif

	// 時間を気にするもの
	auto current = SCNOW;
	unless (time_diff(latest_update, current) < UPDATE_INTERVAL)
	{
		latest_update = current;

		// ズーム処理
		zoom();

		// ここから更新処理
		controll_camera();

		// ポリゴンの全更新
		for (const auto& _polygons : polygons)
			for (const auto& polygon : _polygons.second)
				polygon->update();
	}
}

void Stage::draw()
{
	auto current = SCNOW;
	if (time_diff(latest_draw, current) < 1000 / FRAME_RATES) return;
	latest_draw = current;

	// サイズ変わっていたらソートをもう一度行う
	if (draw_pols.size() != draw_pols_length)
	{
		draw_pols_length = draw_pols.size();
		sort_pols();
	}

	// ここから描画処理
	
	d3d_device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(d3d_device->BeginScene()))
	{
		for (const auto& drawing_polygon : draw_pols)
			drawing_polygon->draw();
			
	}
	d3d_device->EndScene();

	d3d_device->Present(NULL, NULL, NULL, NULL);
}

void Stage::zoom()
{

	if (zoom_sign == Stage::Sign::PLUS)
	{
		if (zoom_level < zoom_level_target)
		{
			zoom_level *= 1.01f;
			zoom_level *= 1.01f;
		}
		else
		{
			zoom_level = zoom_level_target;
			zoom_sign = Stage::Sign::ZERO;
			player->unlock();
		}
	}

	if (zoom_sign == Stage::Sign::MINUS)
	{
		if (zoom_level > zoom_level_target)
		{
			zoom_level /= 1.01f;
			zoom_level /= 1.01f;
		}
		else
		{
			zoom_level = zoom_level_target;
			zoom_sign = Stage::Sign::ZERO;
			player->unlock();
		}
	}

	// 全ズーム変更
	for (const auto& _polygons : polygons)
		for (const auto& polygon : _polygons.second)
			polygon->zoom(zoom_level);
}

void Stage::trigger_controlls()
{
	player->trigger_controlls();

	// プレイヤのジャンプ中は拡縮ができない
	if (player->is_jumping()) return;

	// 拡縮
	if (zoom_sign == Stage::Sign::ZERO && gage->can_consume())
	{
		// 最小にする => 自分は大きくなる
		if (zoom_level > 0.5f && GetKeyboardTrigger(DIK_NUMPAD1))
		{
			zoom_level_target = 0.5f;
			zoom_sign = Stage::Sign::MINUS;
			player->lock();
			gage->consume();
		}

		// 通常状態
		if (zoom_level != 1 && GetKeyboardTrigger(DIK_NUMPAD2))
		{
			zoom_level_target = 1.0f;
			zoom_sign = (zoom_level < 1 ? Stage::Sign::PLUS : Stage::Sign::MINUS);
			player->lock();
			gage->consume();
		}

		// 最大化 => 自分は小さくなる
		if (zoom_level < 2.0f && GetKeyboardTrigger(DIK_NUMPAD3))
		{
			zoom_level_target = 2.0f;
			zoom_sign = Stage::Sign::PLUS;
			player->lock();
			gage->consume();
		}

#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_O) && zoom_level < 2.0f) // 拡大
		{
			zoom_sign = Stage::Sign::PLUS;
			zoom_level_target = zoom_level * 2;
			player->lock();
		}
		if (GetKeyboardTrigger(DIK_L) && zoom_level > 0.5f) // 縮小
		{
			zoom_sign = Stage::Sign::MINUS;
			zoom_level_target = zoom_level / 2;
			player->lock();
		}
#endif
	}
}

void Stage::controll_camera()
{
	camera.x = player->get_coords().x;
	camera.y = player->get_coords().y + CAMERA_HEIGHT; // プレイヤからのカメラの高さ，同じじゃなんか変だと思う

#ifndef _DEBUG // 本番は見せないように
													   // 画面外は見せないようにする
	unless(camera.x < map_size.w * zoom_level - SCREEN_WIDTH / 2) camera.x = map_size.w * zoom_level - SCREEN_WIDTH / 2;
	unless(camera.y < map_size.h * zoom_level - SCREEN_HEIGHT / 2) camera.y = map_size.h * zoom_level - SCREEN_HEIGHT / 2;
	if (camera.x < SCREEN_WIDTH / 2) camera.x = SCREEN_WIDTH / 2;
	if (camera.y < SCREEN_HEIGHT / 2) camera.y = SCREEN_HEIGHT / 2;
#endif
}

void Stage::sort_pols()
{
	// ソート
	for (const auto& pol_vec : polygons)
		for (const auto& polygon : pol_vec.second)
			if (polygon->is_drawing())
				draw_pols.emplace_back(polygon);

	// 大きいものが前に来るように
	sort(draw_pols.begin(), draw_pols.end(), [](const SquarePolygonBase* x, const SquarePolygonBase* y) {return x->layer > y->layer; });
}

template<typename _T> _T Stage::emplace_polygon_back(SquarePolygonBase::PolygonTypes type, _T polygon)
{
	polygons[type].emplace_back(polygon);

	return polygon;
}
