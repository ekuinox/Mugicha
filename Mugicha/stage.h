#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "polygonsquare.h"
#include "player.h"
#include "background.h"
#include "scalable_object.h"

using polygon_vec = std::vector<SquarePolygonBase*>;

enum status {
	prep, // 準備中
	ready, // 準備完了
	playing, // プレイ中
	pause, // ポーズ
	clear, // クリア
	failed, // 失敗
	end, // 終了
};

class Stage {
private:
	std::map<std::string, LPDIRECT3DTEXTURE9> textures;
	std::map<std::string, polygon_vec> polygons;
	enum status status;
	DWORD latest_update; // 最終更新
	DWORD latest_draw; // 最終描画

	void multi_texture_loader(std::map<std::string, std::string> _textures);
	void init();
	void update();
	void draw();
public:
	Stage();
	Stage(char _stage_select);
	~Stage();

	void exec();
};