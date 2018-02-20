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

	// カメラ位置の初期化
	camera = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	
	// 背景の登録
	background = new Background(textures["TITLE_BG"], &camera);
	polygons.push_back(background);

	// セレクタの登録
	selector = new Selector(textures["SELECTOR"], &camera);
	polygons.push_back(selector);

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
		background->change_texture(textures["TITLE_BG"]);
		background->enable();
		background->show();
		break;
	case Select:
		// 背景
		background->change_texture(textures["STAGE_SELECT_BG"]);
		background->enable();
		background->show();

		// セレクタ
		selector->enable();
		selector->show();

		break;
	case Gaming:
		stage = new Stage(selector->get_selection());
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
		if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFTARROW)) // 選択左
		{
			selector->left();
		}

		if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHTARROW)) // 選択右
		{
			selector->right();
		}

		if (GetKeyboardTrigger(DIK_RETURN))
		{
			selector->init();
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