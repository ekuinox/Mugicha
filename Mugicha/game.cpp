#include "game.h"

/* ゲーム本体 */
Game::Game()
{
	scene = scene::TITLE;
	loops = 0;
	exec_latest = timeGetTime();
	init();
	timeBeginPeriod(1);
}

Game::~Game()
{
	// テクスチャクリア
	for (auto tex : textures)
	{
		if (tex.second)
		{
			tex.second->Release();
			tex.second = NULL;
		}
	}
	timeEndPeriod(1);
}

void Game::init()
{
	if (textures.empty())
	{
		multi_texture_loader({
//			{ "__TAG__", "__FILE_PATH__" },
			{ "BACKGROUND", "./resources/textures/background.jpg" },
			{ "PLAYER", "./resources/textures/player.jpg" },
		});
	}
	
	std::map<std::string, polygon_vec>().swap(polygons); // すべて無にする　後のことは考えてない　すみません

	// 背景の登録
	polygons["BACKGROUNDS"].push_back(new Background(textures["BACKGROUND"]));
	polygons["BACKGROUNDS"][0]->switch_status(true);

	// プレイヤの登録
	polygons["PLAYERS"].push_back(new Player(textures["PLAYER"]));
	polygons["PLAYERS"][0]->switch_status(false);

	// シーン切り替え
	switch_scene(scene::TITLE);
}

// メインのループ用の処理，60fpsで画面描画をする
void Game::exec()
{
	auto current = timeGetTime();
	update();
	if (1000 / 60 < current - exec_latest)
	{
		draw();
		exec_latest = current;
	}
}

// シーンの切り替え
void Game::switch_scene(enum scene _scene)
{
	scene = _scene;
	for (const auto& _polygons : polygons)
	{
		for(const auto& polygon : _polygons.second) polygon->switch_drawing(false); // 一度すべてのポリゴンを非表示にする
	}

	std::vector<std::string> labels; // ドローするポリゴンのラベルを詰めていく

	// 表示するポリゴンをシーンで振り分けるけど，よくない
	// なにが良くないかって常にポリゴンはあって，そのシーンでは必要のないポリゴンも生きてる
	// なのでこの先修正されたい
	switch (scene)
	{
	case scene::TITLE:
		labels = {
			"BACKGROUNDS"
		};
		break;
	case scene::MAIN:
		labels = {
			"BACKGROUNDS",
			"PLAYERS",
		};
		break;
	case scene::GAMEOVER:
		labels = {
			"BACKGROUNDS"
		};
		break;
	case scene::GAMECLEAR:
		labels = {
			"BACKGROUNDS"
		};
		break;
	case scene::END:
		break;
	}

	for (const auto& label : labels)
	{
		for(const auto& polygon : polygons[label]) polygon->switch_drawing(true); // ラベルで指定されたポリゴンを表示にしていく
	}
}

// メインのアップデート処理（分割したい）
void Game::update()
{
	UpdateInput();

	DWORD current = timeGetTime();
	static DWORD latest_scene_update;

	// タイトル画面
	if (scene == scene::TITLE)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			switch_scene(scene::MAIN);
			latest_scene_update = current;
		}
	}

	// メイン
	if (scene == scene::MAIN)
	{
		// メインで使うポリゴンの有効化
		polygons["PLAYERS"][0]->switch_status(TRUE);

		main();
		latest_scene_update = current;
	}

	// ゲームオーバ
	if (scene == scene::GAMEOVER)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			init();
			switch_scene(scene::MAIN);
			latest_scene_update = current;
		}
	}

	// ゲームクリア
	if (scene == scene::GAMECLEAR)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			init();
			switch_scene(scene::MAIN);
			latest_scene_update = current;
		}
	}

	// ポリゴンの全更新
	for (const auto& _polygons : polygons)
	{
		for (const auto& polygon : _polygons.second) polygon->update();
	}
}

// ポリゴンをまとめてドロー
void Game::draw()
{
	d3d_device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(d3d_device->BeginScene()))
	{
		for (const auto& _polygons : polygons)
		{
			// 描画順を管理できるようにする必要がいずれ来る
			for (const auto& polygon : _polygons.second) polygon->draw();
		}
	}
	d3d_device->EndScene();

	d3d_device->Present(NULL, NULL, NULL, NULL);
}


// ゲームの一番メイン
bool Game::main(bool reset)
{
	DWORD current = timeGetTime();

	// 操作
	if (current - latest_update > 1) // 1ms間隔で
	{ 
		
	}

	return true;
}

void Game::multi_texture_loader(std::map<std::string, std::string> _textures)
{
	for (auto _texture : _textures)
	{
		D3DXCreateTextureFromFile(d3d_device, _texture.second.c_str(), &textures[_texture.first]); // 一斉読み込み
	}
}