#include "thorns.h"
#include "collision_checker.h"

Thorns::Thorns()
{
}

Thorns::Thorns(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 * _camera, SquarePolygonBase *_floor, Player * _player, bool _up, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), floor(_floor), up(_up), player(_player)
{
	init();
}

void Thorns::init()
{
	
}

void Thorns::update()
{
	// �Ƃ��Ɏ��ԊǗ����邱�ƂȂ��H
	
	// ������ݒ肵�Ȃ���
	y = floor->get_coords().y + (floor->get_size().h + h) / (up ? 2 : - 2); // ��������������Ŕ��f����

	if (is_collisionA(player, this) && zoom_level.w >= 1)
	{
		// �����蔻��
		player->kill();
	}
}

