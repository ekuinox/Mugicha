#include "stage.h"
#include "collision_checker.h"

// コンストラクタ
Stage::Stage()
{
	latest_update = timeGetTime();
	latest_draw = timeGetTime();
	status = prep;

	init();
}

Stage::Stage(char _stage_select)
{
	// _stage_selectによって読み込むステージ情報を切り替えたい
	latest_update = timeGetTime();
	latest_draw = timeGetTime();
	status = prep;

//	init();
	if (_stage_select == 1) init1();
	if (_stage_select == 2) init2();
	if (_stage_select == 3) init3();

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
enum status Stage::exec()
{
	update();
	draw();
	return status;
}

void Stage::multi_texture_loader(std::map<std::string, const char *> _textures)
{
	for (auto _texture : _textures)
	{
		D3DXCreateTextureFromFile(d3d_device, _texture.second, &textures[_texture.first]); // 一斉読み込み
	}
}

// 初期化, コンストラクタから呼ぶ
void Stage::init()
{
	if (textures.empty())
	{
		multi_texture_loader({
//			{ "__TAG__", "__FILE_PATH__" },
			{ "BACKGROUND", "./resources/textures/background.jpg" },
			{ "PLAYER", "./resources/textures/player.jpg" },
			{ "BLOCK", "./resources/textures/block.png" },
			{ "ORIGIN", "./resources/textures/origin.png" },
			{ "BLOCK2", "./resources/textures/block2.png" },
			{ "SAMPLE1", "./resources/textures/sample1.png"},
			});
	}

	std::map<enum PolygonTypes, polygon_vec>().swap(polygons); // すべて無にする　後のことは考えてない　すみません

	// 背景の登録
	background = push_polygon_back(BACKGROUND, new Background(textures["BACKGROUND"], &camera));
	background->enable();
	background->show();

	// プレイヤの登録
	player = push_polygon_back(PLAYER, new Player(textures["PLAYER"], &camera, polygons));
	player->enable();
	player->show();

	// 拡縮できるオブジェクトを登録
	polygons[SCALABLE_OBJECT].push_back(new ScalableObject(25, 25, 50, 50, textures["SAMPLE1"], 1, &camera));
	polygons[SCALABLE_OBJECT].back()->enable();
	polygons[SCALABLE_OBJECT].back()->show();
	
	// 原点
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 20, 20, textures["ORIGIN"], 2, &camera));
	polygons[PLAIN].back()->enable();
	polygons[PLAIN].back()->show();

	// x軸ガイドライン
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 100000, 10, textures["BLOCK2"], 3, &camera));
	polygons[PLAIN].back()->enable();
	polygons[PLAIN].back()->show();

	// y軸ガイドライン
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 10, 100000, textures["BLOCK2"], 3, &camera));
	polygons[PLAIN].back()->enable();
	polygons[PLAIN].back()->show();

	zoom_level = { 1, 1 };
}

// 更新処理
void Stage::update()
{
	// 時間を気にしないもの

	// タイトルに戻る（無確認）
	if (GetKeyboardTrigger(DIK_1))
	{
		status = end;
		return;
	}

	// 拡縮
	if (GetKeyboardTrigger(DIK_O) && zoom_level.h < 2.0f) // 拡大
	{
		zoom_level.w *= 2;
		zoom_level.h *= 2;
#ifdef _DEBUG
		printf("zoom_level: (%f, %f)\n", zoom_level.w, zoom_level.h);
#endif
	}
	else if (GetKeyboardTrigger(DIK_L) && zoom_level.w > 0.5f) // 縮小
	{
		zoom_level.w /= 2;
		zoom_level.h /= 2;
#ifdef _DEBUG
		printf("zoom_level: (%f, %f)\n", zoom_level.w, zoom_level.h);
#endif
	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
#ifdef _DEBUG
		std::cout << "Trigger SPACE" << std::endl;
#endif
		static_cast<Player*>(polygons[PLAYER][0])->jump();
	}


	// 時間を気にするもの
	auto current = timeGetTime();
	if (current - latest_update < 1) return;
	latest_update = current;

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
