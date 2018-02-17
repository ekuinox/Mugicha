#include "stage.h"

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
void Stage::exec()
{
	update();
	draw();
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
			});
	}

	std::map<std::string, polygon_vec>().swap(polygons); // すべて無にする　後のことは考えてない　すみません

	// 背景の登録
	polygons["BACKGROUNDS"].push_back(new Background(textures["BACKGROUND"]));
	polygons["BACKGROUNDS"][0]->enable();
	polygons["BACKGROUNDS"][0]->show();

	// プレイヤの登録
	polygons["PLAYERS"].push_back(new Player(textures["PLAYER"], &camera));
	polygons["PLAYERS"][0]->enable();
	polygons["PLAYERS"][0]->show();

	// 拡縮できるオブジェクトを登録
	polygons["SCALABLE_OBJECTS"].push_back(new ScalableObject(210, 200, 50, 50, textures["BLOCK"], 1, 1, &camera));
	polygons["SCALABLE_OBJECTS"][0]->enable();
	polygons["SCALABLE_OBJECTS"][0]->show();

	polygons["SCALABLE_OBJECTS"].push_back(new ScalableObject(261, 200, 50, 50, textures["BLOCK"], 1, 1, &camera));
	polygons["SCALABLE_OBJECTS"][1]->enable();
	polygons["SCALABLE_OBJECTS"][1]->show();

	// ただのブロック
	polygons["BLOCKS"].push_back(new PlainSquarePolygon(100, 200, 100, 100, textures["BLOCK"], 1, &camera));
	polygons["BLOCKS"][0]->enable();
	polygons["BLOCKS"][0]->show();
}

// 更新処理
void Stage::update()
{
	auto current = timeGetTime();
	if (current - latest_update < 1) return;
	latest_update = current;

	// ここから更新処理
	camera.x = polygons["PLAYERS"][0]->get_coords().x;
	camera.y = polygons["PLAYERS"][0]->get_coords().y;

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