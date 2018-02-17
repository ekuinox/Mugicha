#pragma once
#include <map>
#include <vector>
#include <string>
#include "stage.h"

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
	DWORD latest_draw;
	DWORD latest_update;
	Stage *stage;
	void update();
	void draw();
public:
	Game();
	~Game();
	void init();
	void exec();
	void switch_scene(enum scene _scene);
	bool main(bool reset = false);
};