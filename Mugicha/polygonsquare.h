#pragma once
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#include "d3dx9.h"
#include "input.h"
#include <cmath>
#include <vector>
#include <time.h>
#include "conf.h"

#ifdef _DEBUG
#include <iostream>
#include "debug_console.h"
#endif

/*
* 四角形のPolygonを管理描画するクラス
* SquarePolygonBase => 抽象
* PlainSquarePolygon => SquarePolygonBaseを継承したシンプルな四角形用のクラス
*/

// ドローする本体
using VERTEX_2D = struct
{
	float x, y, z;
	float rhw;
	D3DCOLOR diffuse;
	float u, v;
};

using POLSIZE = struct _POLSIZE
{
	float w;
	float h;
	_POLSIZE() {};
	_POLSIZE(float _w, float _h) : w(_w), h(_h) {};
};

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
	float x, y, w, h; // x, y => 中心座標
	D3DXVECTOR2 drawing_coord; // 描画用の座標
	float u, v, uw, vh; // u,v => 左上，uv, vh => 幅高さ
	float speed; // 移動速度
	bool drawing; // 描画するかどうかのフラグ
	bool status; // 有効/無効 => update用の
	LPDIRECT3DTEXTURE9 tex; // テクスチャ
	VERTEX_2D vertexes[4]; // ポリゴン頂点情報 => 実際ドローするときに生成して使います

	D3DXVECTOR2 *camera; // 描画中心座標 => 基本的にプレイヤ中心

	// === 関数 ===
	virtual void generate_vertexes() = 0; // vertexesを生成するのに使う．draw()より呼ばれるべきで，publicにはしてません

public:
	// === 変数 ===
	int layer; // レイヤー番号 重複は可，数字が大きいものから描画したい

	// === 関数 ===
	// Update & Draw
	virtual void update() = 0;
	virtual void draw() = 0; // drawing見てからdraw
							 // drawingフラグに関して
	virtual bool is_drawing() = 0; // return drawing;
	virtual void show() = 0; // 描画する
	virtual void hide() = 0; // 描画しなくする
	virtual void change_texture(LPDIRECT3DTEXTURE9) = 0; // テクスチャ変更に使います

	virtual bool is_active() = 0; // return status;
	virtual void enable() = 0; // 更新させる
	virtual void disable() = 0; // 更新しなくする

	virtual D3DXVECTOR2 get_coords() = 0;
	virtual POLSIZE get_size() = 0;
	virtual VERTEX_2D *get_vertexes() = 0;

	virtual void add_coord(float _x, float _y) = 0; // x, y座標に加算

	virtual void zoom(POLSIZE _zoom_level) = 0; // 倍率変更を行う
};

class PlainSquarePolygon : public SquarePolygonBase
{
private:
	void generate_vertexes();
public:
	PlainSquarePolygon();
	PlainSquarePolygon(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 *_camera, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~PlainSquarePolygon();
	void update();
	void draw();
	bool is_drawing();
	void show();
	void hide();
	bool is_active();
	void enable();
	void disable();
	void change_texture(LPDIRECT3DTEXTURE9 _tex);

	D3DXVECTOR2 get_coords();
	POLSIZE get_size();
	VERTEX_2D *get_vertexes();

	void add_coord(float _x, float _y);

	void zoom(POLSIZE _zoom_level);
};

/* global variable */
extern LPDIRECT3DDEVICE9 d3d_device; // Devece obj