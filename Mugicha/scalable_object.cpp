#include "scalable_object.h"

/*
* ScalableObject
* SquarePolygonBaseを継承した簡単なポリゴンクラス
*/

// コンストラクタ 座標とかをセットしていく
ScalableObject::ScalableObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, char _scaling_dir, int _layer, float _u, float _v, float _uw, float _vh)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	tex = _tex;
	u = _u;
	v = _v;
	uw = _uw;
	vh = _vh;
	aspect_ratio = h / w;
	drawing = false;
	scaling_dir = _scaling_dir;
	scaling_base.x = x + w / (scaling_dir % 3 == 0 ? 2 : -2); // 左に大きくするならbaseを右に，逆もまたしかり
	scaling_base.y = y + h / (scaling_dir < 2 ? 2 : -2); // 上に大きくするならbaseを下に，逆もまた
	layer = _layer;
}

// デストラクタ
ScalableObject::~ScalableObject()
{
	// 特になし
}

void ScalableObject::update()
{
	// 無
	if (!status) return;

	DWORD current = timeGetTime();

	// 操作
	if (current - latest_update > 1)
	{
		// 拡縮
		
		if (GetKeyboardPress(DIK_NUMPAD8)) // 拡大
		{
			w *= 1.01f;
			h *= 1.01f;

			scaling_base.x *= 1.01f;
			scaling_base.y /= 1.01f;
		}
		else if (GetKeyboardPress(DIK_NUMPAD2)) // 縮小
		{
			w /= 1.01f;
			h /= 1.01f;

			scaling_base.x /= 1.01f;
			scaling_base.y *= 1.01f;
		}
		
		if (GetKeyboardTrigger(DIK_O)) // 拡大
		{
			w *= 2;
			h *= 2;

			scaling_base.x *= 2;
			scaling_base.y /= 2;
		}
		else if (GetKeyboardTrigger(DIK_L)) // 縮小
		{
			w /= 2;
			h /= 2;

			scaling_base.x /= 2;
			scaling_base.y *= 2;
		}


		//  限界調整
		if (w < 1) w = 1;
		if (h < 1) h = 1;

		latest_update = current;
	}
}

// 座標とサイズからvertexesを生成します
void ScalableObject::generate_vertexes()
{
	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = {
//			this->x + this->w / (i % 3 == 0 ? -2 : 2),
			[&]() {
				if (scaling_dir % 3 == 0) // dirが左の場合，baseは右にある
				{
					return scaling_base.x - (i % 3 == 0 ? w : 0);
				}
				else // dirが右の場合，baseは左にある
				{
					return scaling_base.x + (i % 3 == 0 ? w : 0);
				}
			}(),
//			this->y + this->h / (i < 2 ? -2 : 2),
			[&]() {
				if (scaling_dir < 2) // dirが上の場合，baseは下にある
				{
					return scaling_base.y - (i < 2 ? h : 0);
				}
				else // dirが下の場合，baseは上にある
				{
					return scaling_base.y + (i < 2 ? h : 0);
				}
			}(),
			0.0f,
			1.0f,
			D3DCOLOR_RGBA(255, 255, 255, 200),
			this->u + (i % 3 == 0 ? 0 : this->uw),
			this->v + (i < 2 ? 0 : this->vh)

		};
	}
}

void ScalableObject::draw()
{
	if (!drawing) return; // フラグ判定

	generate_vertexes();
	d3d_device->SetTexture(0, tex);
	d3d_device->SetFVF(FVF_VERTEX_2D);

	d3d_device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		sizeof(this->vertexes) / sizeof(VERTEX_2D) - 2, // ポリゴン数
		this->vertexes,
		sizeof(VERTEX_2D)
	);
}

// drawingフラグを返す
bool ScalableObject::is_drawing()
{
	return drawing;
}

void ScalableObject::show()
{
	drawing = true;
}

void ScalableObject::hide()
{
	drawing = false;
}

// テクスチャの変更
void ScalableObject::change_texture(LPDIRECT3DTEXTURE9 _tex)
{
	tex = _tex;
}

// 当たり判定を取る
bool ScalableObject::is_collision(SquarePolygonBase *pol)
{
	return
		this->x - this->w / 2 <= pol->x + pol->w / 2 // 左と右
		&& this->x + this->w / 2 >= pol->x - pol->w // 右と左
		&& this->y - this->h / 2 <= pol->y + pol->h / 2 // 上と下
		&& this->y + this->h / 2 >= pol->y - pol->h / 2 // 下と上
		? true : false;
}

bool ScalableObject::is_active()
{
	return status;
}

void ScalableObject::enable()
{
	status = true;
}

void ScalableObject::disable()
{
	status = false;
}

// === ScalableObject END ===