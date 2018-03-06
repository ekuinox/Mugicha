#include "stage_thumbnail.h"
#include "helpers.h"

StageThumbnail::StageThumbnail(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 & _camera, float _x, float _y, float _w, float _h)
	: PlainSquarePolygon(SCREEN_WIDTH + _x, _y, _w, _h, _tex, STAGE_THUMBNAIL_LAYER, _camera, 0, 0, 1, 1), plus(STAGE_THUMBNAIL_SIZE_PLUS), triggered(false), init_coords(_x, _y)
{
	speed = STAGE_THUMBNAIL_ZOOM_SPEED;
}

void StageThumbnail::trigger()
{
	layer = 1;
	triggered = true;
}

void StageThumbnail::release()
{
	layer = STAGE_THUMBNAIL_LAYER;
	triggered = false;
}

bool StageThumbnail::is_triggered()
{
	return triggered;
}

void StageThumbnail::update()
{
	if (!status) return;

	auto current = SCNOW;
	if (time_diff(latest_update, current) > UPDATE_INTERVAL)
	{

		if (triggered)
		{
			// �g���K����Ă����ԂȂ�Cplus�ɍ��킹�Ċg��k������
			if(w < STAGE_THUMBNAIL_SIZE_MAX) w = h += speed;

		}
		else
		{
			// �g���K����������Ă����ԂȂ�C�ʏ�̃T�C�Y�ɃW���W���߂�
			if (std::abs(w - STAGE_THUMBNAIL_SIZE) < 1)
			{
				w = h = STAGE_THUMBNAIL_SIZE;
			}
			else if (w < STAGE_THUMBNAIL_SIZE)
			{
				w = h += speed;
			}
			else if (w > STAGE_THUMBNAIL_SIZE)
			{
				w = h -= speed;
			}
		}
	}
}