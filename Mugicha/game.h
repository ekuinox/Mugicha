#pragma once
#include <map>
#include <vector>
#include <string>
#include "polygonsquare.h"
#include "player.h"
#include "background.h"
#include "scalable_object.h"

using polygon_vec = std::vector<SquarePolygonBase*>;

// シーン管理用の列挙型
enum scene
{
	TITLE,
	MAIN,
	GAMEOVER,
	GAMECLEAR,
	END,
};

// ゲーム管理用のクラス
class Game
{
private:
	enum scene scene;
	int loops;
	DWORD exec_latest;
	DWORD latest_update; // for main
	std::map<std::string, LPDIRECT3DTEXTURE9> textures;
	std::map<std::string, polygon_vec> polygons;
	void update();
	void draw();
	void multi_texture_loader(std::map<std::string, std::string>);
public:
	Game();
	~Game();
	void init();
	void exec();
	void switch_scene(enum scene _scene);
	bool main(bool reset = false);
};