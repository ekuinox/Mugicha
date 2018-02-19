#include "controller.h"

/* ゲーム本体 */
Controller::Controller()
{
	scene = scene::Title;
	loops = 0;
	latest_update = timeGetTime();
	latest_draw = timeGetTime();
	init();
	timeBeginPeriod(1);
}

Controller::~Controller()
{
	timeEndPeriod(1);
}

void Controller::init()
{
	std::map<const char*, const char*> texture_files = {
		{"TITLE_BG", "./resources/textures/title_bg.png"},
		{"STAGE_SELECT_BG", "./resources/textures/stage_select_bg.png" },
		{"SELECTOR", "./resources/textures/selector.png"}
	};

	for (const auto& texture_file : texture_files)
	{
		if (FAILED(D3DXCreateTextureFromFile(d3d_device, texture_file.second, &textures[texture_file.first])))
		{
#ifdef _DEBUG
			printf("テクスチャ読み込み失敗！");
#endif
		}
	}
	camera = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	polygons = {
		{new Background(textures["TITLE_BG"], &camera)},
		// TODO: カーソルはカーソルで一つのクラスにわけてやったほうがいいかもね，いやしましょう
		{new PlainSquarePolygon(400, 600, 100, 100, textures["SELECTOR"], 0, &camera)}
	};

	// シーン切り替え
	switch_scene(scene::Title);
}

// メインのループ用の処理，60fpsで画面描画をする
void Controller::exec()
{
	update();
	draw();
}

// シーンの切り替え
void Controller::switch_scene(enum scene _scene)
{
	scene = _scene;

	for (const auto& polygon : polygons)
	{
		polygon->disable();
		polygon->hide();
	}

	switch (scene)
	{
	case Title:
		polygons[0]->change_texture(textures["TITLE_BG"]);
		polygons[0]->enable();
		polygons[0]->show();
		break;
	case Select:
		// 背景
		polygons[0]->change_texture(textures["STAGE_SELECT_BG"]);
		polygons[0]->enable();
		polygons[0]->show();

		// セレクタ
		polygons[1]->enable();
		polygons[1]->show();

		stage_select = 2;

		break;
	case Gaming:
		stage = new Stage(stage_select);
		break;
	default:
		break;
	}
}

// メインのアップデート処理（分割したい）
void Controller::update()
{
	UpdateInput();

	auto current = timeGetTime();
	if (current - latest_update < 1) return;

	if (scene != Gaming)
	{
		for (const auto& polygon : polygons)
		{
			polygon->update();
		}
	}

	switch (scene)
	{
	case Title:
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			switch_scene(Select);
		}
		break;
	case Select:
		if (stage_select >= 2 && (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFTARROW))) // 選択左
		{
			stage_select -= 1;
			polygons[1]->add_coord(-200, 0);
		}

		if (stage_select <= 2 && (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHTARROW))) // 選択右
		{
			stage_select += 1;
			polygons[1]->add_coord(200, 0);
		}

		polygons[1]->update();

		if (GetKeyboardTrigger(DIK_RETURN))
		{
			switch_scene(Gaming);
		}
		break;
	case Gaming:
		if (stage->exec() == end) // execがStageのstatusを返すのでそれを見てウンたらしていきたい
		{
			delete stage;
			switch_scene(Title);
		}
		break;
	default:
		break;
	}
}

// ポリゴンをまとめてドロー
void Controller::draw()
{
	if (scene == Gaming) return; // ゲーム中はStage->exec();にまかせて

	auto current = timeGetTime();
	if (current - latest_draw < 1000 / 60) return;
	latest_draw = current;

	polygon_vec drawing_polygons;
	for (const auto& polygon : polygons)
	{
		drawing_polygons.push_back(polygon);
	}

	// 大きいものが前に来るように
	sort(drawing_polygons.begin(), drawing_polygons.end(), [](const SquarePolygonBase* x, const SquarePolygonBase* y) {return x->layer > y->layer; });

	d3d_device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(d3d_device->BeginScene()))
	{
		for (const auto& drawing_polygon : drawing_polygons) drawing_polygon->draw();
	}

	d3d_device->EndScene();

	d3d_device->Present(NULL, NULL, NULL, NULL);
}