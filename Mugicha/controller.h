#pragma once
#include <map>
#include <vector>
#include <string>
#include "stage.h"
#include "selector.h"

// シーン管理用の列挙型
enum scene
{
	Ready,
	Title,
	Select,
	Gaming,
	GameOver,
	GameClear,
	End,
};

// ゲーム管理用のクラス
class Controller
{
private:
	enum scene scene;
	int loops;
	std::chrono::system_clock::time_point latest_draw;
	std::chrono::system_clock::time_point latest_update;
	std::map<const char*, LPDIRECT3DTEXTURE9> textures;
	polygon_vec polygons;
	Background *background; // 背景ポリゴン
	Selector *selector; // セレクターポリゴン
	Stage *stage;
	D3DXVECTOR2 camera;
	void update();
	void draw();
public:
	Controller();
	~Controller();
	void init();
	void exec();
	void switch_scene(enum scene _scene);
};