#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "polygons_register.h"
#include "helpers.h"

using polygon_vec = std::vector<SquarePolygonBase*>;

class Stage {
public:
	enum class Status {
		Prep,
		LoadError,
		Ready,
		Playing,
		Pause,
		Clear,
		Failed,
		Retire,
		End
	};
	using GameInfo = struct _GameInfo
	{
		int score;
		int stage_number;
		enum Stage::Status status;
		Player::DeadReason dead_reason;
		_GameInfo() {}
		_GameInfo(int _score, enum Stage::Status _status, int _stage_number) : score(_score), status(_status), stage_number(_stage_number), dead_reason(Player::DeadReason::ALIVE){}
	};
private:
	// 変数群
	std::map<std::string, LPDIRECT3DTEXTURE9> textures; // テクスチャ格納
	D3DXVECTOR2 camera; // カメラ位置
	
	PolygonsContainer polygons; // すべてのポリゴンがここにくる
	Goal *goal; // ゴール
	Player *player; // プレイヤ
	Gage* gage; // 拡縮のゲージ

	Stage::GameInfo info; // 続行管理と結果
	
	time_point latest_update; // 最終更新
	time_point latest_draw; // 最終描画
	
	enum class Sign { ZERO,	PLUS, MINUS} zoom_sign; // 拡大状態か縮小状態かってアレです
	float zoom_level_target; // どこまで拡縮するかというアレ
	float zoom_level; // 最新のズームレベル

	POLSIZE map_size; // 背景のデカさになります
	
	std::map<std::string, bool> switches; // スイッチマップ

	// 関数群
	void multi_texture_loader(std::map<std::string, const char *> _textures); // テクスチャを読み込む
	void multi_texture_loader(const char *filepath); // テクスチャを読み込むためのmapを作成して渡す
	bool stagefile_loader(const char* filepath); // ステージファイルをロードする
	void init(); // ステージの初期設定をやる
	void update();
	void draw();

	template<typename _T>
	_T emplace_polygon_back(SquarePolygonBase::PolygonTypes type, _T polygon); // polygonsの指定されたラベルの最後にで追加する
public:
	Stage(char _stage_select);
	~Stage();

	GameInfo exec(); // 外からはこれしか叩かない
};