#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "polygons_register.h"


using polygon_vec = std::vector<SquarePolygonBase*>;

enum status {
	prep, // 準備中
	ready, // 準備完了
	playing, // プレイ中
	pause, // ポーズ
	clear, // クリア
	failed, // 失敗
	retire, // 諦めた場合
	end, // 終了
};

using GameInfo = struct _GameInfo
{
	int score;
	int stage_number;
	enum status status;
	_GameInfo() {}
	_GameInfo(int _score, enum status _status, int _stage_number) : score(_score), status(_status), stage_number(_stage_number) {}
};

enum Sign {
	ZERO,
	PLUS,
	MINUS,
};

class Stage {
private:
	// vars
	std::map<std::string, LPDIRECT3DTEXTURE9> textures;
	std::map<enum PolygonTypes, polygon_vec> polygons;
	Background *background;
	Goal *goal;
	Player *player; // プレイヤの変数
	std::vector<Enemy*> enemies; // 敵の可変長配列
	GameInfo info; // 続行管理と結果
	DWORD latest_update; // 最終更新
	DWORD latest_draw; // 最終描画
	DWORD elapsed_time; // ゲーム内の時間，残り時間の表示などに用意
	D3DXVECTOR2 camera;
	enum Sign zoom_sign; // 拡大状態か縮小状態かってアレです
	POLSIZE zoom_level_target; // どこまで拡縮するかというアレ
	POLSIZE zoom_level;
	POLSIZE map_size; // 背景のデカさになります

	// funcs
	void multi_texture_loader(std::map<std::string, const char *> _textures);
	void multi_texture_loader(const char *filepath);
	void stagefile_loader(const char* filepath);
	void init();
	void update();
	void draw();

	template<typename _T>
	_T push_polygon_back(enum PolygonTypes type, _T polygon);
public:
	Stage();
	Stage(char _stage_select);
	~Stage();

	GameInfo exec();
};