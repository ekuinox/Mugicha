#pragma once
#include <map>
#include <vector>
#include <string>
#include "stage.h"

// シーン管理用の列挙型
enum scene
{
	Title,
	Select,
	Gaming,
	End,
};

// ゲーム管理用のクラス
class Controller
{
private:
	enum scene scene;
	int loops;
	DWORD latest_draw;
	DWORD latest_update;
	std::map<const char*, LPDIRECT3DTEXTURE9> textures;
	polygon_vec polygons;
	char stage_select;
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