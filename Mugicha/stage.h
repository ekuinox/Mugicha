#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "plain_square_polygon.h"
#include "player.h"
#include "background.h"
#include "scalable_object.h"
#include "enemy.h"
#include "ragged_floor.h"
#include "thorns.h"

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
	enum status status;
	_GameInfo() {}
	_GameInfo(int _score, enum status _status) : score(_score), status(_status) {}
};

enum Sign {
	ZERO,
	PLUS,
	MINUS,
};

class Stage {
private:
	std::map<std::string, LPDIRECT3DTEXTURE9> textures;
	std::map<enum PolygonTypes, polygon_vec> polygons;
	Background *background;
	Player *player; // プレイヤの変数
	std::vector<Enemy*> enemies; // 敵の可変長配列

	GameInfo info; // 続行管理と結果
	DWORD latest_update; // 最終更新
	DWORD latest_draw; // 最終描画
	DWORD elapsed_time; // ゲーム内の時間，残り時間の表示などに用意
	char stage_select; // ステージ選択番号
	D3DXVECTOR2 camera;
	enum Sign zoom_sign; // 拡大状態か縮小状態かってアレです
	POLSIZE zoom_level_target; // どこまで拡縮するかというアレ
	POLSIZE zoom_level;
	void multi_texture_loader(std::map<std::string, const char *> _textures);
	void init();
	void init1();
	void init2();
	void init3();
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