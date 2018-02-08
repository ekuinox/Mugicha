#pragma once
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#include "d3dx9.h"
#include "input.h"
#include <cmath>
#include <vector>
#include <time.h>
#include "conf.h"

/*
* 四角形のPolygonを管理描画するクラス
* SquarePolygonBase => 抽象
* PlainSquarePolygon => SquarePolygonBaseを継承したシンプルな四角形用のクラス
*/

// ドローする本体
typedef struct
{
	float x, y, z;
	float rhw;
	D3DCOLOR diffuse;
	float u, v;
} VERTEX_2D;

/*
* NOTES
* protectedな変数をいじるpublicな関数をいれとかなきゃ多分詰む
*/

class SquarePolygonBase // 抽象クラス
{
private:
	// 多分ない
protected:
	// === 変数 ===
	DWORD latest_update;
	float direction; // 移動方向
	float angle; // 本体の回転角度
	float u, v, uw, vh; // u,v => 左上，uv, vh => 幅高さ
	float speed; // 移動速度
	bool drawing; // 描画するかどうかのフラグ
	bool status; // 有効/無効 => update用の
	LPDIRECT3DTEXTURE9 tex; // テクスチャ
	VERTEX_2D vertexes[4]; // ポリゴン頂点情報 => 実際ドローするときに生成して使います

						   // === 関数 ===
	virtual void generate_vertexes() = 0; // vertexesを生成するのに使う．draw()より呼ばれるべきで，publicにはしてません

public:
	// == 変数 ===
	float x, y, w, h; // x, y => 中心座標
	int priority; // 描画優先度 => 今のところ使っていない
	bool flags[3]; // 予備のフラグ，何かに使える多分
				   // === 関数 ===
	SquarePolygonBase();
	virtual ~SquarePolygonBase();

	// Update & Draw
	virtual void update() = 0;
	virtual void draw() = 0; // drawing見てからdraw
							 // drawingフラグに関して
	virtual bool is_drawing() = 0; // return drawing;
	virtual void switch_drawing(bool) = 0; // 受け取った変数でdrawingフラグを切り替える
	virtual void switch_drawing() = 0; // drawing = !drawing;
	virtual void change_texture(LPDIRECT3DTEXTURE9) = 0; // テクスチャ変更に使います
	virtual bool is_collision(SquarePolygonBase*) = 0; // ポリゴン同士の当たり判定に

	virtual void switch_status(bool) = 0; // Update用のフラグ，更新するかを判断するのに
	virtual bool is_active() = 0; // return status;
};

class PlainSquarePolygon : public SquarePolygonBase
{
private:
	void generate_vertexes();
public:
	PlainSquarePolygon(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~PlainSquarePolygon();
	void update();
	void draw();
	bool is_drawing();
	void switch_drawing(bool _drawing);
	void switch_drawing();
	void change_texture(LPDIRECT3DTEXTURE9 _tex);
	bool is_collision(SquarePolygonBase *pol);
	void switch_status(bool _status);
	bool is_active();
};

/* global variable */
extern LPDIRECT3DDEVICE9 d3d_device; // Devece obj