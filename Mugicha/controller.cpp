#include "controller.h"

/* ゲーム本体 */
Controller::Controller()
	: scene(Scene::Ready), latest_update(SCNOW), latest_draw(SCNOW)
{
	init();
}

void Controller::init()
{
	std::map<const char*, const char*> texture_files = {
		{ "TITLE_BG", TEXTURES_DIR "title.jpg"},
		{ "Z", TEXTURES_DIR "z.png" },
		{ "OOMING", TEXTURES_DIR "ooming.png" },
		{ "STAGE_01", TEXTURES_DIR "stage_01.png" },
		{ "STAGE_02", TEXTURES_DIR "stage_02.png" },
		{ "STAGE_03", TEXTURES_DIR "stage_03.png" },
		{ "STAGE_SELECT_BG", TEXTURES_DIR "stage_select_bg.png" },
		{ "GAMEOVER_BG", TEXTURES_DIR "gameover_bg.jpg" },
		{ "GAMECLEAR_BG", TEXTURES_DIR "gameclear_bg.png" },
		{ "SELECTOR", TEXTURES_DIR "selector.png"},
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
	background = new AlwaysDisplayedPolygon(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT, textures["TITLE_BG"], INT_MAX);
	polygons.emplace_back(background);
	// セレクタの登録
	selector = new Selector(textures["SELECTOR"]);
	polygons.emplace_back(selector);

	// Z
	zooming_z = new ZoomingZ(textures["Z"], camera);
	polygons.emplace_back(zooming_z);

	// ステージ用のサムネ
	stage_thumbnails[0] = new StageThumbnail(textures["STAGE_01"], camera, SCREEN_WIDTH * 1.25f, SCREEN_HEIGHT / 2);
	stage_thumbnails[1] = new StageThumbnail(textures["STAGE_02"], camera, SCREEN_WIDTH * 1.5f, SCREEN_HEIGHT / 2);
	stage_thumbnails[2] = new StageThumbnail(textures["STAGE_03"], camera, SCREEN_WIDTH * 1.75f, SCREEN_HEIGHT / 2);
	for (const auto& thumb : stage_thumbnails) polygons.emplace_back(thumb);

	// シーン切り替え
	switch_scene(Scene::Title);
}

// メインのループ用の処理，60fpsで画面描画をする
void Controller::exec()
{
	update();
	draw();
}

Controller::Scene Controller::get_scene()
{
	return scene;
}

// シーンの切り替え
void Controller::switch_scene(Scene _scene)
{
	if (scene == _scene) return; // 変化なしで切る

	// 一度全て無効かつ描画しない状態にする
	for (const auto& polygon : polygons)
	{
		polygon->off();
	}

	// 変更前のシーンの終了処理
	switch (scene)
	{
	case Scene::Title:
		background->off();
		zooming_z->off();
		break;
	case Scene::AnimetionTitleToSelect:
		background->off();
		for (const auto& thumb : stage_thumbnails) thumb->off();
		break;
	case Scene::Select:
		background->off();
		for (const auto& thumb : stage_thumbnails) thumb->off();
		break;
	case Scene::Gaming: // ステージから抜けて来たときの処理
		delete stage;
		break;
	case Scene::GameOver:
		break;
	case Scene::GameClear:
		break;
	case Scene::End:
		break;
	}
	
	// 変更後のシーンの初期化
	switch (_scene)
	{
	case Scene::Title:
		background->change_texture(textures["TITLE_BG"]);
		background->on();
		zooming_z->on();
		camera.x = SCREEN_WIDTH / 2;
		break;
	case Scene::AnimetionTitleToSelect:
		for (const auto& thumb : stage_thumbnails) thumb->on();
		background->on();
		break;
	case Scene::Select:
		// 背景
		background->change_texture(textures["TITLE_BG"]);
		background->on();

		// セレクタ
		selector->init();
		selector->on();

		for (const auto& thumb : stage_thumbnails) thumb->on();
		
		break;
	case Scene::Gaming:
		stage = new Stage(selector->get_selection());
		break;
	case Scene::GameOver:
		// 背景
		background->change_texture(textures["GAMEOVER_BG"]);
		background->on();
		break;
	case Scene::GameClear:
		// 背景
		background->change_texture(textures["GAMECLEAR_BG"]);
		background->on();
		break;
	case Scene::End:
		break;
	}

	// シーン情報を代入して終わり
	scene = _scene;
}

// メインのアップデート処理（分割したい）
void Controller::update()
{
	UpdateInput();

	// xbox
	UpdateController();

	if (scene == Scene::Gaming)
	{
		game_info = stage->exec();
		switch (game_info.status)
		{
		case Stage::Status::LoadError: // ロード失敗
			switch_scene(Scene::Select);
		case Stage::Status::Clear: // ゲームクリア時
			switch_scene(Scene::GameClear); // リザルトを見せてやるべきだけどとりあえず
			break;
		case Stage::Status::Failed:  // こちらもリザルト画面を見せてやるべきだけどとりあえず
			switch_scene(Scene::GameOver);
			break;
		case Stage::Status::Retire:
			switch_scene(Scene::GameOver);
			break;
		default:
			break;
		}

	}
	else
	{
		for (const auto& polygon : polygons) polygon->update();
		auto current = SCNOW;
		switch (scene)
		{
		case Scene::Title:
			if (GetKeyboardTrigger(DIK_RETURN) || GetControllerButtonTrigger(XIP_START))
			{
				switch_scene(Scene::AnimetionTitleToSelect);
			}
			break;
		case Scene::AnimetionTitleToSelect:
			//  アニメーションする
			if (camera.x < SCREEN_WIDTH * 1.5f)
			{
				if (time_diff(latest_update, current) > UPDATE_INTERVAL)
				{
					latest_update = current;
					camera.x += CAMERA_MOVE_SPEED;
				}
				
			}
			else switch_scene(Scene::Select);
			
			break;
		case Scene::Select:
			// カーソルを左に
			if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFTARROW) || GetControllerButtonTrigger(XIP_D_LEFT))
			{
				selector->left();
			}

			// カーソルを右に
			if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHTARROW) || GetControllerButtonTrigger(XIP_D_RIGHT))
			{
				selector->right();
			}

			// ゲーム開始
			if (GetKeyboardTrigger(DIK_RETURN) || GetControllerButtonTrigger(XIP_START))
			{
				switch_scene(Scene::Gaming);
			}

			// タイトルに戻す
			if (GetKeyboardTrigger(DIK_F5) || GetControllerButtonTrigger(XIP_BACK))
			{
				switch_scene(Scene::Title);
			}
			break;
		case Scene::GameOver:
			if (GetKeyboardTrigger(DIK_RETURN) || GetControllerButtonTrigger(XIP_START))
			{
				switch_scene(Scene::Select);
			}
			break;
		case Scene::GameClear:
			if (GetKeyboardTrigger(DIK_RETURN) || GetControllerButtonTrigger(XIP_START))
			{
				switch_scene(Scene::Select);
			}
			break;
		case Scene::End:
			break;
		}
	}	
}

// ポリゴンをまとめてドロー
void Controller::draw()
{
	if (scene == Scene::Gaming) return; // ゲーム中はStage->exec();にまかせて

	auto current = SCNOW;
	if (time_diff(latest_draw, current) < 1000 / FRAME_RATES) return;
	latest_draw = current;

	PolygonsVector drawing_polygons;
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