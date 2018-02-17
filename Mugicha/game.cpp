#include "game.h"

/* ゲーム本体 */
Game::Game()
{
	scene = scene::TITLE;
	loops = 0;
	latest_update = timeGetTime();
	latest_draw = timeGetTime();
	init();
	timeBeginPeriod(1);
}

Game::~Game()
{
	timeEndPeriod(1);
}

void Game::init()
{
	stage = new Stage();
	// シーン切り替え
	switch_scene(scene::TITLE);
}

// メインのループ用の処理，60fpsで画面描画をする
void Game::exec()
{
	update();
	draw();
}

// シーンの切り替え
void Game::switch_scene(enum scene _scene)
{
	scene = _scene;
}

// メインのアップデート処理（分割したい）
void Game::update()
{
	UpdateInput();

	auto current = timeGetTime();
	if (current - latest_update < 1) return;

	// タイトル画面
	if (scene == scene::TITLE)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			switch_scene(scene::MAIN);
			latest_update = current;
		}
	}

	// メイン
	if (scene == scene::MAIN)
	{
		main();
		latest_update = current;
	}

	// ゲームオーバ
	if (scene == scene::GAMEOVER)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			init();
			switch_scene(scene::MAIN);
			latest_update = current;
		}
	}

	// ゲームクリア
	if (scene == scene::GAMECLEAR)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			init();
			switch_scene(scene::MAIN);
			latest_update = current;
		}
	}
}

// ポリゴンをまとめてドロー
void Game::draw()
{
	auto current = timeGetTime();
	if (current - latest_draw < 1000 / 60) return;
	latest_draw = current;
}


// ゲームの一番メイン
bool Game::main(bool reset)
{
	stage->exec();

	return true;
}